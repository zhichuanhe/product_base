/***********************license start***************
 * Author: Cavium Networks
 *
 * Contact: support@caviumnetworks.com
 * This file is part of the OCTEON SDK
 *
 * Copyright (c) 2003-2008 Cavium Networks
 *
 * This file is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License, Version 2, as
 * published by the Free Software Foundation.
 *
 * This file is distributed in the hope that it will be useful, but
 * AS-IS and WITHOUT ANY WARRANTY; without even the implied warranty
 * of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE, TITLE, or
 * NONINFRINGEMENT.  See the GNU General Public License for more
 * details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this file; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA 02110-1301 USA
 * or visit http://www.gnu.org/licenses/.
 *
 * This file may also be available under a different license from Cavium.
 * Contact Cavium Networks for more information
 ***********************license end**************************************/

#ifndef __CVMX_H__
#define __CVMX_H__

#include <linux/kernel.h>
#include <linux/string.h>
#include <linux/delay.h>
#include <linux/io.h>

#include <asm/byteorder.h>

#ifndef CVMX_ENABLE_CSR_ADDRESS_CHECKING
#define CVMX_ENABLE_CSR_ADDRESS_CHECKING 0
#endif

#ifndef CVMX_ENABLE_PARAMETER_CHECKING
#define CVMX_ENABLE_PARAMETER_CHECKING 0
#endif

#ifndef CVMX_ENABLE_POW_CHECKS
#define CVMX_ENABLE_POW_CHECKS 0
#endif

#define CVMX_SHARED
#ifndef __BYTE_ORDER
# ifdef __BIG_ENDIAN
#  define __BYTE_ORDER __BIG_ENDIAN
# else
#  define __BYTE_ORDER __LITTLE_ENDIAN
# endif
#endif

#define cvmx_local_irq_save local_irq_save
#define cvmx_local_irq_restore local_irq_restore

#define cvmx_safe_printf pr_err
#define cvmx_warn pr_warning
#define cvmx_warn_if(p, fmt, ...) ({if (p) pr_warning(pr_fmt(fmt), ##__VA_ARGS__);})

#define cvmx_unlikely unlikely
#define cvmx_likely likely
#define CVMX_ENABLE_HELPER_FUNCTIONS 1
#define CVMX_ENABLE_PKO_FUNCTIONS 1
/*
 * Returns processor ID, different Linux and simple exec versions
 * provided in the cvmx-app-init*.c files.
 */
static inline uint32_t cvmx_get_proc_id(void) __attribute__ ((pure));
static inline uint32_t cvmx_get_proc_id(void)
{
	uint32_t id;
	asm("mfc0 %0, $15,0" : "=r"(id));
	return id;
}

#ifndef CVMX_ENABLE_DEBUG_PRINTS
#define CVMX_ENABLE_DEBUG_PRINTS 1
#endif

#if CVMX_ENABLE_DEBUG_PRINTS
#define cvmx_dprintf        printk
#define cvmx_dvprintf       vprintk
#else
#define cvmx_dprintf(...)   {}
#define cvmx_dvprintf(a, b)   {(void)(a);(void)(b);}
#endif

#define CVMX_CACHE_LINE_SIZE    (128)	/* In bytes */
#define CVMX_CACHE_LINE_MASK    (CVMX_CACHE_LINE_SIZE - 1)	/* In bytes */
#define CVMX_CACHE_LINE_ALIGNED __attribute__ ((aligned(CVMX_CACHE_LINE_SIZE)))
#define CAST64(v) ((long long)(long)(v))
#define CASTPTR(type, v) ((type *)(long)(v))


/* turn the variable name into a string */
#define CVMX_TMP_STR(x) CVMX_TMP_STR2(x)
#define CVMX_TMP_STR2(x) #x

/**
 * Builds a bit mask given the required size in bits.
 *
 * @bits:   Number of bits in the mask
 * Returns The mask
 */ static inline uint64_t cvmx_build_mask(uint64_t bits)
{
	return ~((~0x0ull) << bits);
}

/**
 * Builds a memory address for I/O based on the Major and Sub DID.
 *
 * @major_did: 5 bit major did
 * @sub_did:   3 bit sub did
 * Returns I/O base address
 */
static inline uint64_t cvmx_build_io_address(uint64_t major_did,
					     uint64_t sub_did)
{
	return (0x1ull << 48) | (major_did << 43) | (sub_did << 40);
}

/**
 * Perform mask and shift to place the supplied value into
 * the supplied bit rage.
 *
 * Example: cvmx_build_bits(39,24,value)
 * <pre>
 * 6       5       4       3       3       2       1
 * 3       5       7       9       1       3       5       7      0
 * +-------+-------+-------+-------+-------+-------+-------+------+
 * 000000000000000000000000___________value000000000000000000000000
 * </pre>
 *
 * @high_bit: Highest bit value can occupy (inclusive) 0-63
 * @low_bit:  Lowest bit value can occupy inclusive 0-high_bit
 * @value:    Value to use
 * Returns Value masked and shifted
 */
static inline uint64_t cvmx_build_bits(uint64_t high_bit,
				       uint64_t low_bit, uint64_t value)
{
	return (value & cvmx_build_mask(high_bit - low_bit + 1)) << low_bit;
}

#include "cvmx-address.h"

/**
 * Convert a memory pointer (void*) into a hardware compatable
 * memory address (uint64_t). Octeon hardware widgets don't
 * understand logical addresses.
 *
 * @ptr:    C style memory pointer
 * Returns Hardware physical address
 */
static inline uint64_t cvmx_ptr_to_phys(void *ptr)
{
	return virt_to_phys(ptr);
}

/**
 * Convert a hardware physical address (uint64_t) into a
 * memory pointer (void *).
 *
 * @physical_address:
 *               Hardware physical address to memory
 * Returns Pointer to memory
 */
static inline void *cvmx_phys_to_ptr(uint64_t physical_address)
{
	return phys_to_virt(physical_address);
}

/* The following #if controls the definition of the macro
    CVMX_BUILD_WRITE64. This macro is used to build a store operation to
    a full 64bit address. With a 64bit ABI, this can be done with a simple
    pointer access. 32bit ABIs require more complicated assembly */

/* We have a full 64bit ABI. Writing to a 64bit address can be done with
    a simple volatile pointer */
#define CVMX_BUILD_WRITE64(TYPE, ST)                                    \
static inline void cvmx_write64_##TYPE(uint64_t addr, TYPE##_t val)     \
{                                                                       \
    *CASTPTR(volatile TYPE##_t, addr) = val;                            \
}


/* The following #if controls the definition of the macro
    CVMX_BUILD_READ64. This macro is used to build a load operation from
    a full 64bit address. With a 64bit ABI, this can be done with a simple
    pointer access. 32bit ABIs require more complicated assembly */

/* We have a full 64bit ABI. Writing to a 64bit address can be done with
    a simple volatile pointer */
#define CVMX_BUILD_READ64(TYPE, LT)                                     \
static inline TYPE##_t cvmx_read64_##TYPE(uint64_t addr)                \
{                                                                       \
	return *CASTPTR(volatile TYPE##_t, addr);			\
}


/* The following defines 8 functions for writing to a 64bit address. Each
    takes two arguments, the address and the value to write.
    cvmx_write64_int64      cvmx_write64_uint64
    cvmx_write64_int32      cvmx_write64_uint32
    cvmx_write64_int16      cvmx_write64_uint16
    cvmx_write64_int8       cvmx_write64_uint8 */
CVMX_BUILD_WRITE64(int64, "sd");
CVMX_BUILD_WRITE64(int32, "sw");
CVMX_BUILD_WRITE64(int16, "sh");
CVMX_BUILD_WRITE64(int8, "sb");
CVMX_BUILD_WRITE64(uint64, "sd");
CVMX_BUILD_WRITE64(uint32, "sw");
CVMX_BUILD_WRITE64(uint16, "sh");
CVMX_BUILD_WRITE64(uint8, "sb");
#define cvmx_write64 cvmx_write64_uint64

/* The following defines 8 functions for reading from a 64bit address. Each
    takes the address as the only argument
    cvmx_read64_int64       cvmx_read64_uint64
    cvmx_read64_int32       cvmx_read64_uint32
    cvmx_read64_int16       cvmx_read64_uint16
    cvmx_read64_int8        cvmx_read64_uint8 */
CVMX_BUILD_READ64(int64, "ld");
CVMX_BUILD_READ64(int32, "lw");
CVMX_BUILD_READ64(int16, "lh");
CVMX_BUILD_READ64(int8, "lb");
CVMX_BUILD_READ64(uint64, "ld");
CVMX_BUILD_READ64(uint32, "lw");
CVMX_BUILD_READ64(uint16, "lhu");
CVMX_BUILD_READ64(uint8, "lbu");
#define cvmx_read64 cvmx_read64_uint64

static inline uint64_t cvmx_read_csr(uint64_t csr_addr)
{
	uint64_t val = cvmx_read64(csr_addr);
	return val;
}

#include <asm/octeon/octeon-model.h>

#include "cvmx-csr-enums.h"

#include "cvmx-ciu-defs.h"
#include "cvmx-gpio-defs.h"
#include "cvmx-iob-defs.h"
#include "cvmx-ipd-defs.h"
#include "cvmx-l2c-defs.h"
#include "cvmx-l2d-defs.h"
#include "cvmx-l2t-defs.h"
#include "cvmx-led-defs.h"
#include "cvmx-mio-defs.h"
#include "cvmx-pow-defs.h"
#include "cvmx-rnm-defs.h"

static inline void cvmx_write_csr(uint64_t csr_addr, uint64_t val)
{
	cvmx_write64(csr_addr, val);

	/*
	 * Perform an immediate read after every write to an RSL
	 * register to force the write to complete. It doesn't matter
	 * what RSL read we do, so we choose CVMX_MIO_BOOT_BIST_STAT
	 * because it is fast and harmless.
	 */
	if (((csr_addr >> 40) & 0x7ffff) == (0x118))
		cvmx_read64(CVMX_MIO_BOOT_BIST_STAT);
}

static inline void cvmx_write_io(uint64_t io_addr, uint64_t val)
{
	cvmx_write64(io_addr, val);

}

#include "cvmx-asm.h"
#include "cvmx-packet.h"
#include "cvmx-sysinfo.h"

#include "cvmx-bootinfo.h"
#include "cvmx-bootmem.h"
#include "cvmx-l2c.h"

static inline void cvmx_send_single(uint64_t data)
{
	const uint64_t CVMX_IOBDMA_SENDSINGLE = 0xffffffffffffa200ull;
	cvmx_write64(CVMX_IOBDMA_SENDSINGLE, data);
}

static inline void cvmx_read_csr_async(uint64_t scraddr, uint64_t csr_addr)
{
	union {
		uint64_t u64;
		struct {
			uint64_t scraddr:8;
			uint64_t len:8;
			uint64_t addr:48;
		} s;
	} addr;
	addr.u64 = csr_addr;
	addr.s.scraddr = scraddr >> 3;
	addr.s.len = 1;
	cvmx_send_single(addr.u64);
}

static inline unsigned int cvmx_get_core_num(void)
{
	unsigned int core_num;
	CVMX_RDHWRNV(core_num, 0);
	return core_num;
}

/**
 * Returns the number of bits set in the provided value.
 * Simple wrapper for POP instruction.
 *
 * @val:    32 bit value to count set bits in
 *
 * Returns Number of bits set
 */
static inline uint32_t cvmx_pop(uint32_t val)
{
	uint32_t pop;
	CVMX_POP(pop, val);
	return pop;
}

/**
 * Returns the number of bits set in the provided value.
 * Simple wrapper for DPOP instruction.
 *
 * @val:    64 bit value to count set bits in
 *
 * Returns Number of bits set
 */
static inline int cvmx_dpop(uint64_t val)
{
	int pop;
	CVMX_DPOP(pop, val);
	return pop;
}

/**
 * Provide current cycle counter as a return value
 *
 * Returns current cycle counter
 */

static inline uint64_t cvmx_get_cycle(void)
{
	uint64_t cycle;
	CVMX_RDHWR(cycle, 31);
	return cycle;
}

/**
 * Wait for the specified number of cycle
 *
 */
#define cvmx_wait(c) __delay(c)

/**
 * Wait for the specified number of micro seconds
 *
 * @param usec   micro seconds to wait
 */
#define cvmx_wait_usec(usec) udelay(usec)

/**
 * Reads a chip global cycle counter.  This counts CPU cycles since
 * chip reset.  The counter is 64 bit.
 * This register does not exist on CN38XX pass 1 silicion
 *
 * Returns Global chip cycle count since chip reset.
 */
static inline uint64_t cvmx_get_cycle_global(void)
{
	return cvmx_read64(CVMX_IPD_CLK_COUNT);
}
extern uint64_t octeon_get_clock_rate(void);

/**
 * This macro spins on a field waiting for it to reach a value. It
 * is common in code to need to wait for a specific field in a CSR
 * to match a specific value. Conceptually this macro expands to:
 *
 * 1) read csr at "address" with a csr typedef of "type"
 * 2) Check if ("type".s."field" "op" "value")
 * 3) If #2 isn't true loop to #1 unless too much time has passed.
 */
#define CVMX_WAIT_FOR_FIELD64(address, type, field, op, value, timeout_usec)\
    (									\
{									\
	int result;							\
	do {								\
		uint64_t done = cvmx_get_cycle() + (uint64_t)timeout_usec * \
			octeon_get_clock_rate() / 1000000;	\
		type c;							\
		while (1) {						\
			c.u64 = cvmx_read_csr(address);			\
			if ((c.s.field) op(value)) {			\
				result = 0;				\
				break;					\
			} else if (cvmx_get_cycle() > done) {		\
				result = -1;				\
				break;					\
			} else						\
				cvmx_wait(100);				\
		}							\
	} while (0);							\
	result;								\
})

/***************************************************************************/

static inline void cvmx_reset_octeon(void)
{
	union cvmx_ciu_soft_rst ciu_soft_rst;
	ciu_soft_rst.u64 = 0;
	ciu_soft_rst.s.soft_rst = 1;
	cvmx_write_csr(CVMX_CIU_SOFT_RST, ciu_soft_rst.u64);
}

/* Return the number of cores available in the chip */
static inline uint32_t cvmx_octeon_num_cores(void)
{
	uint32_t ciu_fuse = (uint32_t) cvmx_read_csr(CVMX_CIU_FUSE) & 0xffffffff;
	return cvmx_pop(ciu_fuse);
}

/**
 * Read a byte of fuse data
 * @byte_addr:   address to read
 *
 * Returns fuse value: 0 or 1
 */
static uint8_t cvmx_fuse_read_byte(int byte_addr)
{
	union cvmx_mio_fus_rcmd read_cmd;

	read_cmd.u64 = 0;
	read_cmd.s.addr = byte_addr;
	read_cmd.s.pend = 1;
	cvmx_write_csr(CVMX_MIO_FUS_RCMD, read_cmd.u64);
	while ((read_cmd.u64 = cvmx_read_csr(CVMX_MIO_FUS_RCMD))
	       && read_cmd.s.pend)
		;
	return read_cmd.s.dat;
}

/**
 * Read a single fuse bit
 *
 * @fuse:   Fuse number (0-1024)
 *
 * Returns fuse value: 0 or 1
 */
static inline int cvmx_fuse_read(int fuse)
{
	return (cvmx_fuse_read_byte(fuse >> 3) >> (fuse & 0x7)) & 1;
}

#include "octeon-feature.h"

static inline int cvmx_octeon_model_CN36XX(void)
{
	return OCTEON_IS_MODEL(OCTEON_CN38XX)
		&& cvmx_fuse_read(264);
}

static inline int cvmx_octeon_zip_present(void)
{
	return octeon_has_feature(OCTEON_FEATURE_ZIP);
}

static inline int cvmx_octeon_dfa_present(void)
{
	if (!OCTEON_IS_MODEL(OCTEON_CN38XX)
	    && !OCTEON_IS_MODEL(OCTEON_CN31XX)
	    && !OCTEON_IS_MODEL(OCTEON_CN58XX))
		return 0;
	else if (OCTEON_IS_MODEL(OCTEON_CN3020))
		return 0;
	else
		return !cvmx_fuse_read(120);
}

static inline int cvmx_octeon_crypto_present(void)
{
	return octeon_has_feature(OCTEON_FEATURE_CRYPTO);
}

#endif /*  __CVMX_H__  */
