#ifndef __AX71_CRSMU_H_
#define __AX71_CRSMU_H_

#define AX71_CRSMU_CPLD_HW_STATE   0x11

#define AX71_CRSMU_AP_MAX 1024

#define AX71_CRSMU_ETHPORT_DEFAULT_ATTR \
	{ETH_ATTR_ENABLE,1518,PORT_FULL_DUPLEX_E,PORT_SPEED_1000_E,	\
	ETH_ATTR_DISABLE,ETH_ATTR_DISABLE,ETH_ATTR_ON,ETH_ATTR_ON,	\
	ETH_ATTR_ON,ETH_ATTR_OFF,COMBO_PHY_MEDIA_PREFER_COPPER}

#endif
