/*******************************************************************************
*              (c), Copyright 2001, Marvell International Ltd.                 *
* THIS CODE CONTAINS CONFIDENTIAL INFORMATION OF MARVELL SEMICONDUCTOR, INC.   *
* NO RIGHTS ARE GRANTED HEREIN UNDER ANY PATENT, MASK WORK RIGHT OR COPYRIGHT  *
* OF MARVELL OR ANY THIRD PARTY. MARVELL RESERVES THE RIGHT AT ITS SOLE        *
* DISCRETION TO REQUEST THAT THIS CODE BE IMMEDIATELY RETURNED TO MARVELL.     *
* THIS CODE IS PROVIDED "AS IS". MARVELL MAKES NO WARRANTIES, EXPRESSED,       *
* IMPLIED OR OTHERWISE, REGARDING ITS ACCURACY, COMPLETENESS OR PERFORMANCE.   *
********************************************************************************
* prvTgfCscd.h
*
* DESCRIPTION:
*       CPSS Mirror
*
* FILE REVISION NUMBER:
*       $Revision: 1.1.1.1 $
*
*******************************************************************************/
#ifndef prvTgfCscd
#define prvTgfCscd

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

/*******************************************************************************
* prvTgfCscdSingleTargetDestinationConfigurationSet
*
* DESCRIPTION:
*       Set test configuration:
*         - Set FDB entry with device number 31, port 27,  MAC 00:00:00:00: 00:56. 
*         -	Set lookup mode to Port for accessing the Device Map table. 
*         -	Set the cascade map table 
*            - Target device 0, target port 0, link type port, link number 23. 
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
GT_VOID prvTgfCscdSingleTargetDestinationConfigurationSet
(
    GT_VOID
);

/*******************************************************************************
* prvTgfCscdSingleTargetDestinationTrafficGenerate
*
* DESCRIPTION:
*       Generate traffic:
*           Send to device's port 8 packet:
*               macDa = 00:00:00:00:00:56,
*               macSa = 00:00:00:00:00:02,
*           Success Criteria:
*                1 packet is captured on ports 23.
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
GT_VOID prvTgfCscdSingleTargetDestinationTrafficGenerate
(
    GT_VOID
);

/*******************************************************************************
* prvTgfCscdSingleTargetDestinationConfigurationRestore
*
* DESCRIPTION:
*       Restore test configuration
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
GT_VOID prvTgfCscdSingleTargetDestinationConfigurationRestore
(
    GT_VOID
);

/*******************************************************************************
* prvTgfCscdSingleTargetDestinationConfigurationSet
*
* DESCRIPTION:
*       Set test configuration:
*         - Set FDB entry with device number 0, port 1,  MAC 00:00:00:00: 00:56. 
*         -	Set lookup mode to Port for accessing the Device Map table. 
*         -	Set the cascade map table 
*            - Target device 0, target port 1, link type port, link number 23. 
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
GT_VOID prvTgfCscdSingleTargetDestinationLocalDeviceConfigurationSet
(
    GT_VOID
);

/*******************************************************************************
* prvTgfCscdSingleTargetDestinationTrafficGenerate
*
* DESCRIPTION:
*       Generate traffic:
*           Send to device's port 8 packet:
*               macDa = 00:00:00:00:00:56,
*               macSa = 00:00:00:00:00:02,
*           Success Criteria:
*                No packets are captured.
*           Set FDB entry with device number 13, port 27,  MAC 00:00:00:00: 00:56. 
*          	Set the cascade map table 
*            - Target device 13, target port 27, link type port, link number 23. 
*           Send to device's port 8 packet:
*               macDa = 00:00:00:00:00:56,
*               macSa = 00:00:00:00:00:02,
*           Success Criteria:
*                1 packet is captured on ports 23.
*           Set FDB entry with device number 31, port 58,  MAC 00:00:00:00: 00:56. 
*          	Set the cascade map table 
*            - Target device 31, target port 58, link type port, link number 0. 
*           Send to device's port 8 packet:
*               macDa = 00:00:00:00:00:56,
*               macSa = 00:00:00:00:00:02,
*           Success Criteria:
*                1 packet is captured on ports 0.
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
GT_VOID prvTgfCscdSingleTargetDestinationLocalDeviceTrafficGenerate
(
    GT_VOID
);

/*******************************************************************************
* prvTgfCscdSingleTargetDestinationLocalDeviceConfigurationRestore
*
* DESCRIPTION:
*       Restore test configuration
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
GT_VOID prvTgfCscdSingleTargetDestinationLocalDeviceConfigurationRestore
(
    GT_VOID
);







#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* __prvTgfCscd */

