/*******************************************************************************
*                Copyright 2001, Marvell International Ltd.
* This code contains confidential information of Marvell semiconductor, inc.
* no rights are granted herein under any patent, mask work right or copyright
* of Marvell or any third party.
* Marvell reserves the right at its sole discretion to request that this code
* be immediately returned to Marvell. This code is provided "as is".
* Marvell makes no warranties, express, implied or otherwise, regarding its
* accuracy, completeness or performance.
********************************************************************************
* wrapCpssGenEvents.h
*
* DESCRIPTION:
*       This file implements Receive/Transmit functionaly user exit functions
*       for Galtis.
*
* DEPENDENCIES:
*
* FILE REVISION NUMBER:
*       $Revision: 1.1.1.1 $
*
******************************************************************************/

#ifndef __wrapCpssGenEvents_h__
#define __wrapCpssGenEvents_h__

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

#include <cpss/generic/cpssTypes.h>
#include <cpss/generic/cos/cpssCosTypes.h>

typedef GT_STATUS EVENT_COUNTER_INCREMENT_FUNC
(
    IN  GT_U8   devNum,
    IN GT_U32   uniEvCounter
);

extern GT_STATUS wrCpssGenEventCountersCbGet
(
	EVENT_COUNTER_INCREMENT_FUNC **eventCounterBlockGetCB
);

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /*__cmdNetTransmit_h__*/


