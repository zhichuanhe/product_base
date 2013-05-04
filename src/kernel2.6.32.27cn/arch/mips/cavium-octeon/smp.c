/*
 * This file is subject to the terms and conditions of the GNU General Public
 * License.  See the file "COPYING" in the main directory of this archive
 * for more details.
 *
 * Copyright (C) 2004-2008 Cavium Networks
 */
#include <linux/cpu.h>
#include <linux/init.h>
#include <linux/delay.h>
#include <linux/smp.h>
#include <linux/interrupt.h>
#include <linux/kernel_stat.h>
#include <linux/sched.h>
#include <linux/module.h>

#include <asm/mmu_context.h>
#include <asm/system.h>
#include <asm/time.h>

#include <asm/octeon/octeon.h>

#include <asm/octeon/octeon-boot-info.h>
#include <asm/octeon/cvmx-app-hotplug.h>

#include <asm/octeon/cvmx-debug.h>

volatile unsigned long octeon_processor_boot = 0xff;
volatile unsigned long octeon_processor_sp;
volatile unsigned long octeon_processor_gp;

#ifdef CONFIG_HOTPLUG_CPU
static uint32_t octeon_hotplug_entry_addr;
extern void octeon_hotplug_entry(void);
#endif
struct cvmx_app_hotplug_global *octeon_hotplug_global_ptr;

static void octeon_reschedule_self(void)
{
	/* Do nothing. */
}

static void octeon_icache_flush(void)
{
	asm volatile ("synci 0($0)\n");
}

static octeon_message_fn_t  octeon_message_functions[8] = {
	octeon_reschedule_self,
	smp_call_function_interrupt,
	octeon_icache_flush,
};

static  int octeon_message_free_mask = 0xf8;
static DEFINE_SPINLOCK(octeon_message_functions_lock);

int octeon_request_ipi_handler(octeon_message_fn_t fn)
{
	int i;
	int message;
	spin_lock(&octeon_message_functions_lock);

	for (i = 0; i < ARRAY_SIZE(octeon_message_functions); i++) {
		message = (1 << i);
		if (message & octeon_message_free_mask) {
			/* found a slot. */
			octeon_message_free_mask ^= message;
			octeon_message_functions[i] = fn;
			goto out;
		}
	}
	message = -ENOMEM;
out:
	spin_unlock(&octeon_message_functions_lock);
	return message;
}
EXPORT_SYMBOL(octeon_request_ipi_handler);

void octeon_release_ipi_handler(int action)
{
	int i;
	int message;
	spin_lock(&octeon_message_functions_lock);

	for (i = 0; i < ARRAY_SIZE(octeon_message_functions); i++) {
		message = (1 << i);
		if (message == action) {
			octeon_message_functions[i] = NULL;
			octeon_message_free_mask |= message;
			goto out;
		}
	}
	pr_err("octeon_release_ipi_handler: Unknown action: %x\n", action);
out:
	spin_unlock(&octeon_message_functions_lock);
}
EXPORT_SYMBOL(octeon_release_ipi_handler);

static irqreturn_t mailbox_interrupt(int irq, void *dev_id)
{
	u64 mbox_clrx = CVMX_CIU_MBOX_CLRX(cvmx_get_core_num());
	u64 action;
	int i;

	/* Make sure the function array initialization remains correct. */
	BUILD_BUG_ON(SMP_RESCHEDULE_YOURSELF != (1 << 0));
	BUILD_BUG_ON(SMP_CALL_FUNCTION       != (1 << 1));
	BUILD_BUG_ON(SMP_ICACHE_FLUSH        != (1 << 2));

	/* Load the mailbox register to figure out what we're supposed to do */
	action = cvmx_read_csr(mbox_clrx);

	if (OCTEON_IS_MODEL(OCTEON_CN68XX))
		action &= 0xff;
	else
		action &= 0xffff;

	/* Clear the mailbox to clear the interrupt */
	cvmx_write_csr(mbox_clrx, action);

	for (i = 0; i < ARRAY_SIZE(octeon_message_functions) && action;) {
		if (action & 1) {
			octeon_message_fn_t fn = octeon_message_functions[i];
			if (fn)
				fn();
		}
		action >>= 1;
		i++;
	}

	return IRQ_HANDLED;
}

/**
 * Cause the function described by call_data to be executed on the passed
 * cpu.  When the function has finished, increment the finished field of
 * call_data.
 */
void octeon_send_ipi_single(int cpu, unsigned int action)
{
	int coreid = cpu_logical_map(cpu);
	/*
	pr_info("SMP: Mailbox send cpu=%d, coreid=%d, action=%u\n", cpu,
	       coreid, action);
	*/
	cvmx_write_csr(CVMX_CIU_MBOX_SETX(coreid), action);
}
EXPORT_SYMBOL(octeon_send_ipi_single);

static inline void octeon_send_ipi_mask(const struct cpumask *mask,
					unsigned int action)
{
	unsigned int i;

	for_each_cpu_mask(i, *mask)
		octeon_send_ipi_single(i, action);
}

static void octeon_smp_setup(void)
{
	const int coreid = cvmx_get_core_num();
	int cpus;
	int id;
	int core_mask = octeon_get_boot_coremask();
#ifdef CONFIG_HOTPLUG_CPU
	unsigned int num_cores = cvmx_octeon_num_cores();
#endif
	struct linux_app_boot_info *labi;

	/* The present CPUs are initially just the boot cpu (CPU 0). */
	for (id = 0; id < NR_CPUS; id++) {
		set_cpu_possible(id, id == 0);
		set_cpu_present(id, id == 0);
	}

	__cpu_number_map[coreid] = 0;
	__cpu_logical_map[0] = coreid;

	/* The present CPUs get the lowest CPU numbers. */
	cpus = 1;
	for (id = 0; id < NR_CPUS; id++) {
		if ((id != coreid) && (core_mask & (1 << id))) {
			set_cpu_possible(cpus, true);
			set_cpu_present(cpus, true);
			__cpu_number_map[id] = cpus;
			__cpu_logical_map[cpus] = id;
			cpus++;
		}
	}

#ifdef CONFIG_HOTPLUG_CPU

	/*
	 * HOTPLUG_CPU not possible if we don't know the bootloader
	 * entrypoint.  Stop now before lulling people into thinking
	 * it might work.
	 */
	if (!octeon_bootloader_entry_addr) {
		if ((octeon_bootinfo->core_mask & 1) == 0)
			panic("Linux not booted on core 0 with HOTPLUG_CPU");
		else
			panic("The bootloader version on this board is not compatible with HOTPLUG_CPU.");
	}

	/*
	 * The possible CPUs are all those present on the chip.  We
	 * will assign CPU numbers for possible cores as well.  Cores
	 * are always consecutively numberd from 0.
	 */
	for (id = 0; id < num_cores && id < NR_CPUS; id++) {
		if (!(core_mask & (1 << id))) {
			set_cpu_possible(cpus, true);
			__cpu_number_map[id] = cpus;
			__cpu_logical_map[cpus] = id;
			cpus++;
		}
	}
#endif
	octeon_hotplug_global_ptr =
		(struct cvmx_app_hotplug_global *) cvmx_bootmem_alloc_named_range(
			CVMX_APP_HOTPLUG_INFO_REGION_SIZE, 0x0, 0x0, 0,
			CVMX_APP_HOTPLUG_INFO_REGION_NAME);
	if (!octeon_hotplug_global_ptr) {
		pr_err("Failed to allocate memory for Hotplug memory block\n");
		return;
	}
	memset(octeon_hotplug_global_ptr, 0, CVMX_APP_HOTPLUG_INFO_REGION_SIZE);

	labi = phys_to_virt(LABI_ADDR_IN_BOOTLOADER);
	octeon_hotplug_global_ptr->avail_coremask = labi->avail_coremask;

	pr_info("Cavium Hotplug: Available coremask 0x%x\n",
		octeon_hotplug_global_ptr->avail_coremask);
}

/**
 * Firmware CPU startup hook
 *
 */
static void octeon_boot_secondary(int cpu, struct task_struct *idle)
{
	int count;

	pr_info("SMP: Booting CPU%02d (CoreId %2d)...\n", cpu,
		cpu_logical_map(cpu));

	octeon_processor_sp = __KSTK_TOS(idle);
	octeon_processor_gp = (unsigned long)(task_thread_info(idle));
	octeon_processor_boot = cpu_logical_map(cpu);
	mb();

	count = 100000;
	while (octeon_processor_sp && count) {
		/* Waiting for processor to get the SP and GP */
		udelay(1);
		count--;
	}
	if (count == 0)
		pr_err("Secondary boot timeout\n");
}

/**
 * After we've done initial boot, this function is called to allow the
 * board code to clean up state, if needed
 */
static void octeon_init_secondary(void)
{
	write_c0_ebase((u32)ebase);

	octeon_check_cpu_bist();
	octeon_init_cvmcount();

	octeon_irq_setup_secondary();
	raw_local_irq_enable();
}

/**
 * Callout to firmware before smp_init
 *
 */
void octeon_prepare_cpus(unsigned int max_cpus)
{
	u64 mask;
#ifdef CONFIG_HOTPLUG_CPU
	unsigned long t;
	struct linux_app_boot_info *labi;

	labi = phys_to_virt(LABI_ADDR_IN_BOOTLOADER);

	if (labi->labi_signature != LABI_SIGNATURE)
		panic("The bootloader version on this board is incorrect.");

	t = __pa_symbol(octeon_hotplug_entry);

	if (t >= 0x20000000)
		panic("The kernel physical load address (%lx) is not compatible with CONFIG_HOTPLUG_CPU.\n", t);

	octeon_hotplug_entry_addr = (uint32_t)CKSEG0ADDR(t);
#endif
	/* Only the low order MBOX bits are used.  Leave the other alone. */
	if (OCTEON_IS_MODEL(OCTEON_CN68XX))
		mask = 0xff;
	else
		mask = 0xffff;
	cvmx_write_csr(CVMX_CIU_MBOX_CLRX(cvmx_get_core_num()), mask);
	if (request_irq(OCTEON_IRQ_MBOX0, mailbox_interrupt, IRQF_DISABLED,
			"mailbox0", mailbox_interrupt)) {
		panic("Cannot request_irq(OCTEON_IRQ_MBOX0)\n");
	}
}

/**
 * Last chance for the board code to finish SMP initialization before
 * the CPU is "online".
 */
static void octeon_smp_finish(void)
{
#ifdef CONFIG_CAVIUM_GDB
	unsigned long tmp;
	/* Pulse MCD0 signal on Ctrl-C to stop all the cores. Also set the MCD0
	   to be not masked by this core so we know the signal is received by
	   someone */
	asm volatile ("dmfc0 %0, $22\n"
		      "ori   %0, %0, 0x9100\n" "dmtc0 %0, $22\n" : "=r" (tmp));
#endif

	octeon_user_io_init();

	/* to generate the first CPU timer interrupt */
	write_c0_compare(read_c0_count() + mips_hpt_frequency / HZ);
}

/**
 * Hook for after all CPUs are online
 */
static void octeon_cpus_done(void)
{
#ifdef CONFIG_CAVIUM_GDB
	unsigned long tmp;
	/* Pulse MCD0 signal on Ctrl-C to stop all the cores. Also set the MCD0
	   to be not masked by this core so we know the signal is received by
	   someone */
	asm volatile ("dmfc0 %0, $22\n"
		      "ori   %0, %0, 0x9100\n" "dmtc0 %0, $22\n" : "=r" (tmp));
#endif
}

#ifdef CONFIG_HOTPLUG_CPU

/* State of each CPU. */
DEFINE_PER_CPU(int, cpu_state);

extern void fixup_irqs(void);

static DEFINE_SPINLOCK(smp_reserve_lock);

static int octeon_cpu_disable(void)
{
	unsigned int cpu = smp_processor_id();

	if (cpu == 0)
		return -EBUSY;

	spin_lock(&smp_reserve_lock);

	cpu_clear(cpu, cpu_online_map);
	cpu_clear(cpu, cpu_callin_map);
	fixup_irqs();
	flush_cache_all();
	local_flush_tlb_all();

	spin_unlock(&smp_reserve_lock);

	return 0;
}

static void octeon_cpu_die(unsigned int cpu)
{
	int coreid = cpu_logical_map(cpu);
	uint32_t mask, new_mask;
	const struct cvmx_bootmem_named_block_desc *block_desc;

	while (per_cpu(cpu_state, cpu) != CPU_DEAD)
		cpu_relax();

	/*
	 * This is a bit complicated strategics of getting/settig available
	 * cores mask, copied from bootloader
	 */

	mask = 1 << coreid;
	octeon_hotplug_global_ptr->avail_coremask |= mask;

	/* LINUX_APP_BOOT_BLOCK is initialized in bootoct binary */
	block_desc = cvmx_bootmem_find_named_block(LINUX_APP_BOOT_BLOCK_NAME);

	if (!block_desc) {
		struct linux_app_boot_info *labi;

		labi = phys_to_virt(LABI_ADDR_IN_BOOTLOADER);

		labi->avail_coremask |= mask;
		new_mask = labi->avail_coremask;
	} else {		       /* alternative, already initialized */
		new_mask = octeon_hotplug_global_ptr->avail_coremask;
	}

	mb();

	pr_info("Reset core %d. Available Coremask = 0x%x \n", coreid, new_mask);
	cvmx_write_csr(CVMX_CIU_NMI, 1 << coreid);
}

void play_dead(void)
{
	int cpu = cpu_number_map(cvmx_get_core_num());

	idle_task_exit();
	octeon_processor_boot = 0xff;
	per_cpu(cpu_state, cpu) = CPU_DEAD;

	wmb(); /* nudge writeback */

	while (1)	/* core will be reset here */
		;
}

static int octeon_update_boot_vector(unsigned int cpu)
{

	int coreid = cpu_logical_map(cpu);
	unsigned int avail_coremask;
	const struct cvmx_bootmem_named_block_desc *block_desc;
	boot_init_vector_t *boot_vect =
		(boot_init_vector_t *) phys_to_virt(BOOTLOADER_BOOT_VECTOR);

	block_desc = cvmx_bootmem_find_named_block(LINUX_APP_BOOT_BLOCK_NAME);

	if (!block_desc) {
		struct linux_app_boot_info *labi;

		labi = phys_to_virt(LABI_ADDR_IN_BOOTLOADER);

        	avail_coremask = labi->avail_coremask;
		labi->avail_coremask &= ~(1 << coreid);
	} else {		       /* alternative, already initialized */
		avail_coremask = octeon_hotplug_global_ptr->avail_coremask;
		if (!(avail_coremask & (1<<coreid)))
			return -1;
	}
	/* This core is no longer available */
	octeon_hotplug_global_ptr->avail_coremask &= ~(1<<coreid);

	boot_vect[coreid].app_start_func_addr = octeon_hotplug_entry_addr;
	boot_vect[coreid].code_addr = octeon_bootloader_entry_addr;

	mb();

	cvmx_write_csr(CVMX_CIU_NMI, (1 << coreid) & avail_coremask);

	return 0;
}

static int __cpuinit octeon_cpu_callback(struct notifier_block *nfb,
	unsigned long action, void *hcpu)
{
	unsigned int cpu = (unsigned long)hcpu;

	switch (action) {
	case CPU_UP_PREPARE:
		octeon_update_boot_vector(cpu);
		break;
	case CPU_ONLINE:
		pr_info("Cpu %d online\n", cpu);
		break;
	case CPU_DEAD:
		break;
	}

	return NOTIFY_OK;
}

static int __cpuinit register_cavium_notifier(void)
{
	hotcpu_notifier(octeon_cpu_callback, 0);

	return 0;
}

late_initcall(register_cavium_notifier);

#endif  /* CONFIG_HOTPLUG_CPU */

struct plat_smp_ops octeon_smp_ops = {
	.send_ipi_single	= octeon_send_ipi_single,
	.send_ipi_mask		= octeon_send_ipi_mask,
	.init_secondary		= octeon_init_secondary,
	.smp_finish		= octeon_smp_finish,
	.cpus_done		= octeon_cpus_done,
	.boot_secondary		= octeon_boot_secondary,
	.smp_setup		= octeon_smp_setup,
	.prepare_cpus		= octeon_prepare_cpus,
#ifdef CONFIG_HOTPLUG_CPU
	.cpu_disable		= octeon_cpu_disable,
	.cpu_die		= octeon_cpu_die,
#endif
};
