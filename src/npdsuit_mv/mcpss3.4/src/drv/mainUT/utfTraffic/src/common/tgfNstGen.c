/*******************************************************************************
*              (c), Copyright 2001, Marvell International Ltd.                 *
* THIS CODE CONTAINS CONFIDENTIAL INFORMATION OF MARVELL SEMICONDUCTOR, INC.   *
* NO RIGHTS ARE GRANTED HEREIN UNDER ANY PATENT, MASK WORK RIGHT OR COPYRIGHT  *
* OF MARVELL OR ANY THIRD PARTY. MARVELL RESERVES THE RIGHT AT ITS SOLE        *
* DISCRETION TO REQUEST THAT THIS CODE BE IMMEDIATELY RETURNED TO MARVELL.     *
* THIS CODE IS PROVIDED "AS IS". MARVELL MAKES NO WARRANTIES, EXPRESSED,       *
* IMPLIED OR OTHERWISE, REGARDING ITS ACCURACY, COMPLETENESS OR PERFORMANCE.   *
********************************************************************************
* tgfNstGen.c
*
* DESCRIPTION:
*       Generic API implementation for Nst
*
* FILE REVISION NUMBER:
*       $Revision: 1.1.1.1 $
*
*******************************************************************************/

#include <cpssCommon/cpssPresteraDefs.h>
#include <cpss/extServices/private/prvCpssBindFunc.h>

#include <utf/private/prvUtfHelpers.h>
#include <utf/utfMain.h>
#include <utf/private/prvUtfExtras.h>

#include <trafficEngine/tgfTrafficEngine.h>
#include <trafficEngine/tgfTrafficGenerator.h>
#include <trafficEngine/tgfTrafficTable.h>

#include <common/tgfCommon.h>
#include <common/tgfNstGen.h>

#ifdef CHX_FAMILY
    #include <cpss/dxCh/dxChxGen/nst/cpssDxChNstPortIsolation.h>
#endif /* CHX_FAMILY */

/******************************************************************************\
 *                           Private declarations                             *
\******************************************************************************/

/******************************************************************************\
 *                     Private function's implementation                      *
\******************************************************************************/

/******************************************************************************\
 *                       CPSS generic API section                             *
\******************************************************************************/
/*******************************************************************************
* prvTgfNstPortIsolationEnableSet
*
* DESCRIPTION:
*       Function enables/disables the port isolation feature.
*
* INPUTS:
*       devNum    - device number
*       enable    - port isolation feature enable/disable
*                   GT_TRUE  - enable
*                   GT_FALSE - disable
*
* OUTPUTS:
*       None.
*
* RETURNS:
*       GT_OK                    - on success
*       GT_FAIL                  - on error.
*       GT_HW_ERROR              - on hardware error
*       GT_BAD_PARAM             - wrong devNum
*       GT_NOT_APPLICABLE_DEVICE - on not applicable device
*
* COMMENTS:
*
*******************************************************************************/
GT_STATUS prvTgfNstPortIsolationEnableSet
(
    IN  GT_U8                devNum,
    IN  GT_BOOL              enable
)
{
#ifdef CHX_FAMILY
    /* call device specific API */
    return cpssDxChNstPortIsolationEnableSet(devNum, enable);
#endif /* CHX_FAMILY */

#ifdef EXMXPM_FAMILY
    /* avoid warnings */
    TGF_PARAM_NOT_USED(devNum);
    TGF_PARAM_NOT_USED(enable);

    /* call device specific API */
    return GT_NOT_SUPPORTED;
#endif /* EXMXPM_FAMILY */

#if (!defined CHX_FAMILY) && (!defined EXMXPM_FAMILY)
    return GT_BAD_STATE;
#endif
}

/*******************************************************************************
* prvTgfNstPortIsolationTableEntrySet
*
* DESCRIPTION:
*       Function sets port isolation table entry.
*       Each entry represent single source port/device or trunk.
*       Each entry holds bitmap of all local device ports (and CPU port), where
*       for each local port there is a bit marking. If it's a member of source
*       interface (if outgoing traffic from this source interface is allowed to
*       go out at this specific local port).
*
* INPUTS:
*       devNum               - device number
*       trafficType          - packets traffic type - L2 or L3
*       srcInterface         - table index is calculated from source interface.
*                              Only portDev and Trunk are supported.
*       cpuPortMember        - port isolation for CPU Port
*                               GT_TRUE - member
*                               GT_FALSE - not member
*       localPortsMembersPtr - (pointer to) port bitmap to be written to the
*                              L2/L3 PI table
*
* OUTPUTS:
*       None.
*
* RETURNS:
*       GT_OK                    - on success
*       GT_FAIL                  - on error.
*       GT_HW_ERROR              - on hardware error
*       GT_BAD_PARAM             - wrong devNum, srcInterface or
*                                  localPortsMembersPtr
*       GT_BAD_PTR               - on NULL pointer
*       GT_NOT_APPLICABLE_DEVICE - on not applicable device
*
* COMMENTS:
*       Traffic is separated for L2 and L3 (means that application may
*       block L2 traffic while allowing L3).
*
*******************************************************************************/
GT_STATUS prvTgfNstPortIsolationTableEntrySet
(
    IN GT_U8                                        devNum,
    IN PRV_TGF_NST_PORT_ISOLATION_TRAFFIC_TYPE_ENT  trafficType,
    IN CPSS_INTERFACE_INFO_STC                      srcInterface,
    IN GT_BOOL                                      cpuPortMember,
    IN CPSS_PORTS_BMP_STC                           *localPortsMembersPtr
)
{
#ifdef CHX_FAMILY
    GT_STATUS rc; /* return code */
    CPSS_DXCH_NST_PORT_ISOLATION_TRAFFIC_TYPE_ENT dxChTrafficType;
    PRV_TGF_MEMBER_FORCE_INFO_STC *currMemPtr;
    GT_U32                      ii;

    /* add the forced ring/cascade ports to the port members */

    /* look for this member in the DB */
    currMemPtr = &prvTgfDevPortForceArray[0];
    for(ii = 0 ; ii < prvTgfDevPortForceNum; ii++,currMemPtr++)
    {
        if(prvTgfDevNum != currMemPtr->member.devNum)
        {
            continue;
        }

        if(currMemPtr->forceToVlan == GT_FALSE)
        {
            /* member not need to be forced to any vlan */
            continue;
        }

        if(currMemPtr->member.portNum >= CPSS_MAX_PORTS_NUM_CNS)
        {
            return GT_BAD_PARAM;
        }

        /* set the member */
        CPSS_PORTS_BMP_PORT_SET_MAC(localPortsMembersPtr,currMemPtr->member.portNum);
    }

    switch(trafficType)
    {
        case PRV_TGF_NST_PORT_ISOLATION_TRAFFIC_TYPE_L2_E:
            dxChTrafficType = CPSS_DXCH_NST_PORT_ISOLATION_TRAFFIC_TYPE_L2_E;
            break;
        case PRV_TGF_NST_PORT_ISOLATION_TRAFFIC_TYPE_L3_E:
            dxChTrafficType = CPSS_DXCH_NST_PORT_ISOLATION_TRAFFIC_TYPE_L3_E;
            break;
        default:
            return GT_BAD_PARAM;
    }

    if( CPSS_INTERFACE_PORT_E == srcInterface.type )
    {
        rc = prvUtfHwFromSwDeviceNumberGet(srcInterface.devPort.devNum,
                                           &(srcInterface.devPort.devNum));
        if(GT_OK != rc)
            return rc;
    }

    /* call device specific API */
    rc = cpssDxChNstPortIsolationTableEntrySet(devNum,
                                               dxChTrafficType,
                                               srcInterface,
                                               cpuPortMember,
                                               localPortsMembersPtr);
    if(GT_OK != rc)
        return rc;

    if( CPSS_INTERFACE_PORT_E == srcInterface.type )
    {
        return prvUtfSwFromHwDeviceNumberGet(srcInterface.devPort.devNum,
                                           &(srcInterface.devPort.devNum));
    }

    return GT_OK;
#endif /* CHX_FAMILY */

#ifdef EXMXPM_FAMILY
    /* avoid warnings */
    TGF_PARAM_NOT_USED(devNum);
    TGF_PARAM_NOT_USED(trafficType);
    TGF_PARAM_NOT_USED(srcInterface);
    TGF_PARAM_NOT_USED(cpuPortMember);
    TGF_PARAM_NOT_USED(localPortsMembersPtr);

    /* call device specific API */
    return GT_NOT_SUPPORTED;
#endif /* EXMXPM_FAMILY */

#if (!defined CHX_FAMILY) && (!defined EXMXPM_FAMILY)
    return GT_BAD_STATE;
#endif
}

/******************************************************************************\
 *                       API for default settings                             *
\******************************************************************************/

