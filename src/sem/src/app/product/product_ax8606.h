#ifndef __PRODUCT_AX8606_H_
#define __PRODUCT_AX8606_H_

#define DISTRIBUTE_AX8606_SLOT_NUM 				6
#define DISTRIBUTE_AX8606_FIRST_MASTER_SLOT		2
#define DISTRIBUTE_AX8606_SECOND_MASTER_SLOT	3
#define DISTRIBUTE_AX8606_MASTER_SLOT_NUM 		2

#define AX8606_DEFAULT_BOARD_ON_MASK 0x3F

#define REMOTE_MASTER_INSERT_STATE_MASK	0x1
#define REMOTE_MASTER_STATE_MASK	0x1

#define SEM_NET_ID		1111

#define STANDBY_DELAY_SECONDS	10

#define ACTIVE_MCB_WATI_TIMEOUT		60

#define GPIO_DATA_REG	0x8001070000000880ull

#define AX81_CRSMU_BOARD_CODE		0x0
#define AX81_AC12C_BOARD_CODE 		0x1
#define AX81_AC8C_BOARD_CODE 		0x2
#define AX81_2X12G12S_BOARD_CODE	0x4
#define AX81_1X12G12S_BOARD_CODE	0x5
#define AX81_12X_BOARD_CODE			0x3
#define AX81_AC4X_BOARD_CODE		0x6
#define AX81_CRSMUE_BOARD_CODE		0x7



#endif
