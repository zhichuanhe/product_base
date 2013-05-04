#ifndef __AX71_1X12G12S_H_
#define __AX71_1X12G12S_H_


#define SLAVE_TEMP1_INPUT  "/sys/bus/i2c/devices/0-004c/temp1_input"
#define SLAVE_TEMP2_INPUT  "/sys/bus/i2c/devices/0-004c/temp2_input"

#define TMP421_SLAVE_TEMP1_INPUT  "/sys/bus/i2c/devices/0-004d/temp1_input"
#define TMP421_SLAVE_TEMP2_INPUT  "/sys/bus/i2c/devices/0-004d/temp2_input"


#define SYSTEM_STATE_REG_ADDR      (0x800000001d070006ull)
#define SYSTEM_FPGA_RELOAD_COUNT      (1)

#define AX71_1X12G12S_ETHPORT_DEFAULT_ATTR  \
	{ETH_ATTR_ENABLE,1518,PORT_FULL_DUPLEX_E,PORT_SPEED_10000_E,	\
	 ETH_ATTR_DISABLE,ETH_ATTR_DISABLE,ETH_ATTR_OFF,ETH_ATTR_OFF,	\
	 ETH_ATTR_OFF,ETH_ATTR_OFF,COMBO_PHY_MEDIA_PREFER_FIBER} 


#define FPGA_BM_FILE_PATH	"/dev/bm0"
#define BM_IOC_FPGA_WRITE 				_IOWR(BM_IOC_MAGIC,20,fpga_file) //FPGA online burning

/*
typedef struct fpga_abnormal_tipc_s
{
    int slot_id;//the slot id of abnormal fpga board
    int count;
}fpga_abnormal_tipc_t;
*/

#endif
