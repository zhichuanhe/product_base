#ifndef __PRODUCT_AX7605I_H_
#define __PRODUCT_AX7605I_H_


#define DISTRIBUTE_AX7605I_SLOT_NUM 				3
#define DISTRIBUTE_AX7605I_FIRST_MASTER_SLOT		0
#define DISTRIBUTE_AX7605I_SECOND_MASTER_SLOT	1
#define DISTRIBUTE_AX7605I_MASTER_SLOT_NUM 		2

#define REMOTE_MASTER_STATE_MASK	0x1

#define REMOTE_MASTER_INSERT_STATE_MASK	0x1

#define SEM_NET_ID		1111

#define STANDBY_DELAY_SECONDS	10

#define ACTIVE_MCB_WATI_TIMEOUT		60


#define GPIO_DATA_REG	0x8001070000000880ull

/*board type*/
enum
{
	BOARD_TYPE_AX71_CRSMU,
	BOARD_TYPE_AX71_2S12G12S,
	BOARD_TYPE_AX71_1X12G12S,
	BOARD_TYPE_AX71_MAX
};

#define AX71_CRSMU_BOARD_CODE		0x3
#define AX71_2X12G12S_BOARD_CODE	0x4
#define AX71_1X12G12S_BOARD_CODE	0x5



#endif
