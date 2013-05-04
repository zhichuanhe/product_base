/*******************************************************************************
*              (c), Copyright 2001, Marvell International Ltd.                 *
* THIS CODE CONTAINS CONFIDENTIAL INFORMATION OF MARVELL SEMICONDUCTOR, INC.   *
* NO RIGHTS ARE GRANTED HEREIN UNDER ANY PATENT, MASK WORK RIGHT OR COPYRIGHT  *
* OF MARVELL OR ANY THIRD PARTY. MARVELL RESERVES THE RIGHT AT ITS SOLE        *
* DISCRETION TO REQUEST THAT THIS CODE BE IMMEDIATELY RETURNED TO MARVELL.     *
* THIS CODE IS PROVIDED "AS IS". MARVELL MAKES NO WARRANTIES, EXPRESSED,       *
* IMPLIED OR OTHERWISE, REGARDING ITS ACCURACY, COMPLETENESS OR PERFORMANCE.   *
********************************************************************************
* prvTgfPortTxResourceHistogram.h
*
* DESCRIPTION:
*       Resource Histigram Counters features testing
*
* FILE REVISION NUMBER:
*       $Revision: 1.1.1.1 $
*
*******************************************************************************/
#ifndef __prvTgfPortTxResourceHistogramh
#define __prvTgfPortTxResourceHistogramh

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

/*******************************************************************************
* prvTgfPortTxResourceHistogramVlanConfigurationSet
*
* DESCRIPTION:
*       Set Default Vlan Port MAC Counters configuration.
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
GT_VOID prvTgfPortTxResourceHistogramVlanConfigurationSet
(
    GT_VOID    
);

/*******************************************************************************
* prvTgfPortTxResourceHistogramConfigurationSet
*
* DESCRIPTION:
*       Set configuration.
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
GT_VOID prvTgfPortTxResourceHistogramConfigurationSet
(
    GT_VOID
);

/*******************************************************************************
* prvTgfPortTxResourceHistogramRestore
*
* DESCRIPTION:
*       None
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
GT_VOID prvTgfPortTxResourceHistogramRestore
(
    GT_VOID
);

/*******************************************************************************
* prvTgPortTxResourceHistogramTrafficGenerate
*
* DESCRIPTION:
*       Generate traffic:
*           Send from port 0packets:
*               macDa = 00:00:00:00:11:11,
*               macSa = 00:00:00:00:22:22,
*           Success Criteria:
*               MAC MIB Captured counters:
*               1 Unicast Packet sent from port 18.
*               1 Unicast Packets received on ports 0, 8 and 23.
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
GT_VOID prvTgfPortTxResourceHistogramTrafficGenerate
(
    void
);

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* __prvTgfPortTxResourceHistogram */


