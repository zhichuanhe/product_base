/*******************************************************************************
*              (c), Copyright 2001, Marvell International Ltd.                 *
* THIS CODE CONTAINS CONFIDENTIAL INFORMATION OF MARVELL SEMICONDUCTOR, INC.   *
* NO RIGHTS ARE GRANTED HEREIN UNDER ANY PATENT, MASK WORK RIGHT OR COPYRIGHT  *
* OF MARVELL OR ANY THIRD PARTY. MARVELL RESERVES THE RIGHT AT ITS SOLE        *
* DISCRETION TO REQUEST THAT THIS CODE BE IMMEDIATELY RETURNED TO MARVELL.     *
* THIS CODE IS PROVIDED "AS IS". MARVELL MAKES NO WARRANTIES, EXPRESSED,       *
* IMPLIED OR OTHERWISE, REGARDING ITS ACCURACY, COMPLETENESS OR PERFORMANCE.   *
********************************************************************************
* prvTgfIpLpmDBCapacityUpdate.h
*
* DESCRIPTION:
*       Basic IPV4 UC Routing
*
* FILE REVISION NUMBER:
*       $Revision: 1.1.1.1 $
*
*******************************************************************************/
#ifndef __prvTgfIpLpmDBCapacityUpdateh
#define __prvTgfIpLpmDBCapacityUpdateh

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */


/*******************************************************************************
* prvTgfIpLpmDBCapacityUpdateBaseConfigurationSet
*
* DESCRIPTION:
*       Set Base Configuration
*
* INPUTS:
*       None
*
* OUTPUTS:
*       None
*
* RETURNS:
*       None
*
* COMMENTS:
*
*******************************************************************************/
GT_VOID prvTgfIpLpmDBCapacityUpdateBaseConfigurationSet
(
    GT_VOID
);

/*******************************************************************************
* prvTgfIpLpmDBCapacityUpdateRouteConfigurationSet
*
* DESCRIPTION:
*       Set Route Configuration
*
* INPUTS:
*       None
*
* OUTPUTS:
*       None
*
* RETURNS:
*       None
*
* COMMENTS:
*
*******************************************************************************/
GT_VOID prvTgfIpLpmDBCapacityUpdateRouteConfigurationSet
(
    GT_VOID
);

/*******************************************************************************
* prvTgfIpLpmDBCapacityUpdateRouteConfigurationUpdate
*
* DESCRIPTION:
*       Update Route Configuration
*
* INPUTS:
*       None
*
* OUTPUTS:
*       None
*
* RETURNS:
*       None
*
* COMMENTS:
*
*******************************************************************************/
GT_VOID prvTgfIpLpmDBCapacityUpdateRouteConfigurationUpdate
(
    GT_VOID
);

/*******************************************************************************
* prvTgfIpLpmDBCapacityUpdateTrafficGenerate
*
* DESCRIPTION:
*       Generate traffic
*
* INPUTS:
*       None
*
* OUTPUTS:
*       None
*
* RETURNS:
*       None
*
* COMMENTS:
*
*******************************************************************************/
GT_VOID prvTgfIpLpmDBCapacityUpdateTrafficGenerate
(
    GT_BOOL isPacketMatched
);

/*******************************************************************************
* prvTgfIpLpmDBCapacityUpdateConfigurationRestore
*
* DESCRIPTION:
*       Restore configuration
*
* INPUTS:
*       None
*
* OUTPUTS:
*       None
*
* RETURNS:
*       None
*
* COMMENTS:
*       2. Restore Route Configuration
*       1. Restore Base Configuration
*
*******************************************************************************/
GT_VOID prvTgfIpLpmDBCapacityUpdateConfigurationRestore
(
    GT_VOID
);

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* __prvTgfIpLpmDBCapacityUpdateh */

