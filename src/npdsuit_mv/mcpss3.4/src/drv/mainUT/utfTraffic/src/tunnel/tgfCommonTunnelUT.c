/*******************************************************************************
*              (c), Copyright 2001, Marvell International Ltd.                 *
* THIS CODE CONTAINS CONFIDENTIAL INFORMATION OF MARVELL SEMICONDUCTOR, INC.   *
* NO RIGHTS ARE GRANTED HEREIN UNDER ANY PATENT, MASK WORK RIGHT OR COPYRIGHT  *
* OF MARVELL OR ANY THIRD PARTY. MARVELL RESERVES THE RIGHT AT ITS SOLE        *
* DISCRETION TO REQUEST THAT THIS CODE BE IMMEDIATELY RETURNED TO MARVELL.     *
* THIS CODE IS PROVIDED "AS IS". MARVELL MAKES NO WARRANTIES, EXPRESSED,       *
* IMPLIED OR OTHERWISE, REGARDING ITS ACCURACY, COMPLETENESS OR PERFORMANCE.   *
********************************************************************************
* tgfCommonTunnelUT.c
*
* DESCRIPTION:
*       Enhanced UTs for CPSS Tunneling
*
* FILE REVISION NUMBER:
*       $Revision: 1.1.1.1 $
*******************************************************************************/

#include <cpssCommon/cpssPresteraDefs.h>
#include <utf/utfMain.h>
#include <utf/private/prvUtfExtras.h>

#include <common/tgfCommon.h>
#include <cpss/dxCh/dxChxGen/config/private/prvCpssDxChInfo.h>
#include <tunnel/prvTgfTunnelTermIpv4overIpv4.h>
#include <tunnel/prvTgfTunnelTermPortGroupIpv4overIpv4.h>
#include <tunnel/prvTgfTunnelTermIpv4overIpv4Trap.h>
#include <tunnel/prvTgfTunnelStartIpv4overGreIpv4.h>
#include <tunnel/prvTgfTunnelStartIpv6overIpv4.h>
#include <tunnel/prvTgfTunnelTermIpv6overIpv4.h>
#include <tunnel/prvTgfTunnelMimStart.h>
#include <tunnel/prvTgfTunnelMacInMacKey.h>
#include <tunnel/prvTgfDetailedTtiActionType2Entry.h>
#include <tunnel/prvTgfDetailedMimTunnelStart.h>
#include <tunnel/prvTgfTunnelTermEtherType.h>
#include <tunnel/prvTgfTunnelTermGlobalPort2LocalPort.h>
#include <tunnel/prvTgfTunnelStartEtherOverIpv4GreRedirectToEgress.h>
#include <tunnel/prvTgfTunnelStartEtherOverIpv4GreRedirectToEgressOffset.h>
#include <tunnel/prvTgfTunnelTermEtherOverIpv4RedirectToEgress.h>
#include <tunnel/prvTgfTunnelTermEtherOverIpv4GreRedirectToEgress.h>
#include <tunnel/prvTgfEtherRedirectToEgress.h>
#include <common/tgfTunnelGen.h>
#include <common/tgfIpGen.h>

/* original prvTgfPortsArray  */
static GT_U8   prvTgfPortsArrayOrig[4];

#define SAVE_ORIG_PORTS_MAC                             \
    prvTgfPortsArrayOrig[0] = prvTgfPortsArray[0];      \
    prvTgfPortsArrayOrig[1] = prvTgfPortsArray[1];      \
    prvTgfPortsArrayOrig[2] = prvTgfPortsArray[2];      \
    prvTgfPortsArrayOrig[3] = prvTgfPortsArray[3]

#define RESTORE_ORIG_PORTS_MAC                          \
    prvTgfPortsArray[0] = prvTgfPortsArrayOrig[0];      \
    prvTgfPortsArray[1] = prvTgfPortsArrayOrig[1];      \
    prvTgfPortsArray[2] = prvTgfPortsArrayOrig[2];      \
    prvTgfPortsArray[3] = prvTgfPortsArrayOrig[3]

extern GT_U32 prvTgfPortSendArray[];

/*----------------------------------------------------------------------------*/
/*
    Test tunnel termination IPv4 over IPv4
*/
UTF_TEST_CASE_MAC(tgfTunnelTermIpv4overIpv4)
{
/*
    1. Set Base configuration
    2. Set Route configuration
    3. Set TTI configuration
    4. Generate traffic
    5. Restore configuration
*/

    PRV_TGF_NOT_SUPPORTED_DEV_CHECK_MAC(prvTgfDevNum, UTF_XCAT2_E);

    if(GT_FALSE == prvTgfBrgTtiCheck())
    {
        return;
    }

    /* Set Base configuration */
    prvTgfTunnelTermIpv4overIpv4BaseConfigurationSet();

    /* Set Route configuration */
    prvTgfTunnelTermIpv4overIpv4RouteConfigurationSet();

    /* Set TTI configuration */
    prvTgfTunnelTermIpv4overIpv4TtiConfigurationSet();

    /* Generate traffic */
    prvTgfTunnelTermIpv4overIpv4TrafficGenerate();

    /* Restore configuration */
    prvTgfTunnelTermIpv4overIpv4ConfigurationRestore();

    prvTgfPclRestore();
}

/*----------------------------------------------------------------------------*/
/*
    Test tunnel termination IPv4 over IPv4
*/
UTF_TEST_CASE_MAC(tgfTunnelTermIpv4overIpv4SinglePortGroup)
{
/*
    1. Set Base configuration
    2. Set Route configuration
    3. Set TTI configuration
    4. Generate traffic
    5. Restore configuration
*/
    GT_U8 iteration;

    GT_PORT_GROUPS_BMP  portGroupsBmp;

    PRV_TGF_NOT_SUPPORTED_DEV_CHECK_MAC(prvTgfDevNum, UTF_CH1_E | UTF_CH1_DIAMOND_E | UTF_CH2_E | UTF_CH3_E | UTF_XCAT_E | UTF_XCAT2_E);

    if(GT_FALSE == prvTgfBrgTtiCheck())
    {
        return;
    }

    SAVE_ORIG_PORTS_MAC;

    /******************** test for ports 0 18 37 54 *********************/
    prvTgfPortsArray[0] = 0;
    prvTgfPortsArray[1] = 18;
    prvTgfPortsArray[2] = 37;
    prvTgfPortsArray[3] = 54;

    for (iteration = 0; iteration < CPSS_MAX_PORT_GROUPS_CNS; iteration++)
    {
        /* send port is different for each port group */
        prvTgfPortsArray[0] = (GT_U8)prvTgfPortSendArray[iteration];

        prvTgfSendPort = (GT_U8)prvTgfPortSendArray[iteration];

        /* write the rule only to a single port group */
        portGroupsBmp = 1 << iteration;

        /* Set Base configuration */
        prvTgfTunnelTermPortGroupIpv4overIpv4BaseConfigurationSet();

            /* Set Route configuration */
        prvTgfTunnelTermPortGroupIpv4overIpv4RouteConfigurationSet();

        /* Set TTI configuration */
        prvTgfTunnelTermPortGroupIpv4overIpv4TtiConfigurationSet(portGroupsBmp);

        /* Generate traffic */
        prvTgfTunnelTermPortGroupIpv4overIpv4TrafficGenerate();
    }

    /* Restore configuration */
    prvTgfTunnelTermPortGroupIpv4overIpv4ConfigurationRestore();

    RESTORE_ORIG_PORTS_MAC;

    prvTgfPclRestore();
}

/*----------------------------------------------------------------------------*/
/*
    Test tunnel termination IPv4 over IPv4 invalidate rule.
*/
UTF_TEST_CASE_MAC(tgfTunnelTermIpv4overIpv4Invalidate)
{
/*
    1. Set Base configuration
    2. Set Route configuration
    3. Set TTI configuration
    4. Generate traffic
    5. Invalidate Rule
    6. Generate traffic
    7. Validate rule
    8. Generate traffic
    9. Restore configuration
*/

    PRV_TGF_NOT_SUPPORTED_DEV_CHECK_MAC(prvTgfDevNum, UTF_XCAT2_E);

    if(GT_FALSE == prvTgfBrgTtiCheck())
    {
        return;
    }

    /* Set Base configuration */
    prvTgfTunnelTermIpv4overIpv4BaseConfigurationSet();

    /* Set Route configuration */
    prvTgfTunnelTermIpv4overIpv4RouteConfigurationSet();

    /* Set TTI configuration */
    prvTgfTunnelTermIpv4overIpv4TtiConfigurationSet();

    /* Generate traffic */
    prvTgfTunnelTermIpv4overIpv4TrafficGenerate();

    /* invalidate TTI rules */
    prvTgfTunnelTermRuleValidStatusSet(GT_FALSE);

    /* Generate traffic -- check no traffic */
    prvTgfTunnelTermIpv4overIpv4TrafficGenerateExpectNoTraffic();

    /* validate TTI rules */
    prvTgfTunnelTermRuleValidStatusSet(GT_TRUE);

    /* Generate traffic */
    prvTgfTunnelTermIpv4overIpv4TrafficGenerate();

    /* Restore configuration */
    prvTgfTunnelTermIpv4overIpv4ConfigurationRestore();

    prvTgfPclRestore();
}

/*----------------------------------------------------------------------------*/
/*
    Test tunnel termination IPv6 over IPv4
*/
UTF_TEST_CASE_MAC(tgfTunnelTermIpv6overIpv4)
{
/*
    1. Set Base configuration
    2. Set Route configuration
    3. Set TTI configuration
    4. Generate traffic
    5. Restore configuration
*/

    PRV_TGF_NOT_SUPPORTED_DEV_CHECK_MAC(prvTgfDevNum, UTF_XCAT2_E);

    if(GT_FALSE == prvTgfBrgTtiCheck())
    {
        return;
    }

    /* Set Base configuration */
    prvTgfTunnelTermIpv6overIpv4BaseConfigurationSet();

    /* Set Route configuration */
    prvTgfTunnelTermIpv6overIpv4RouteConfigurationSet();

    /* Set TTI configuration */
    prvTgfTunnelTermIpv6overIpv4TtiConfigurationSet();

    /* Generate traffic */
    prvTgfTunnelTermIpv6overIpv4TrafficGenerate();

    /* Restore configuration */
    prvTgfTunnelTermIpv6overIpv4ConfigurationRestore();

    prvTgfPclRestore();
}

/*----------------------------------------------------------------------------*/
/*
    Test tunnel termination IPv4 over IPv4 with action TRAP
*/
UTF_TEST_CASE_MAC(tgfTunnelTermIpv4overIpv4Trap)
{
/*
    1. Set Base configuration
    2. Set IP configuration
    3. Set TCAM configuration
    2. Set Route configuration
    3. Set TTI configuration
    4. Generate traffic
    5. Restore configuration
*/

    PRV_TGF_NOT_SUPPORTED_DEV_CHECK_MAC(prvTgfDevNum, UTF_XCAT2_E);

    if(GT_FALSE == prvTgfBrgTtiCheck())
    {
        return;
    }

    /* Set Base configuration */
    prvTgfTunnelTermIpv4overIpv4TrapBaseConfigSet();

    /* Set IP configuration */
    prvTgfTunnelTermIpv4overIpv4TrapIpConfigSet();

    /* Set TCAM configuration */
    prvTgfTunnelTermIpv4overIpv4TrapTcamConfigSet();

    /* Generate traffic */
    prvTgfTunnelTermIpv4overIpv4TrapTrafficGenerate();

    /* Restore configuration */
    prvTgfTunnelTermIpv4overIpv4TrapConfigRestore();

    prvTgfPclRestore();
}

/*----------------------------------------------------------------------------*/
/*
    Test tunnel start IPv4 over GRE IPv4
*/
UTF_TEST_CASE_MAC(tgfTunnelStartIpv4overGreIpv4)
{
/*
    1. Set Base configuration
    2. Set Route configuration
    3. Set Tunnel configuration
    4. Generate traffic
    5. Restore configuration
*/
    if(GT_FALSE == prvTgfBrgTtiCheck())
    {
        return;
    }

    /* NOTE: Test is wrong for PBR mode!!!
             can't do tunnel start and routing though PCL in PBR mode,
             in "IPCL Action as Next Hop Entry" there is no support for tunnel start */
    if (prvUtfIsPbrModeUsed())
    {
        return;
    }

    /* Set Base configuration */
    prvTgfTunnelStartIpv4overGreIpv4BaseConfigurationSet();

    /* Set Route configuration */
    prvTgfTunnelStartIpv4overGreIpv4RouteConfigurationSet();

    /* Set Tunnel configuration */
    prvTgfTunnelStartIpv4overGreIpv4TunnelConfigurationSet();

    /* Generate traffic */
    prvTgfTunnelStartIpv4overGreIpv4TrafficGenerate();

    /* Restore configuration */
    prvTgfTunnelStartIpv4overGreIpv4ConfigurationRestore();

    prvTgfPclRestore();
}

/*----------------------------------------------------------------------------*/
/*
    Test tunnel start IPv6 over IPv4
*/
UTF_TEST_CASE_MAC(tgfTunnelStartIpv6overIpv4)
{
/*
    1. Set Base configuration
    2. Set Route configuration
    3. Set Tunnel configuration
    4. Generate traffic
    5. Restore configuration
*/
    /* NOTE: Test is wrong for PBR mode!!!
             can't do tunnel start and routing though PCL in PBR mode,
             in "IPCL Action as Next Hop Entry" there is no support for tunnel start */
    if (prvUtfIsPbrModeUsed())
    {
        return;
    }

    /* Set Base configuration */
    prvTgfTunnelStartIpv6overIpv4BaseConfigurationSet();

    /* Set Route configuration */
    prvTgfTunnelStartIpv6overIpv4RouteConfigurationSet();

    /* Set Tunnel configuration */
    prvTgfTunnelStartIpv6overIpv4TunnelConfigurationSet();

    /* Generate traffic */
    prvTgfTunnelStartIpv6overIpv4TrafficGenerate();

    /* Restore configuration */
    prvTgfTunnelStartIpv6overIpv4ConfigurationRestore();

    prvTgfPclRestore();
}

/*----------------------------------------------------------------------------*/
/*
    3.1.1 Test MIM Tunnel Start (Forwarded From TTI)
    3.1.2 Test MIM Tunnel Start (Forwarded From PCL)
*/
UTF_TEST_CASE_MAC(tgfTunnelMimStartFromTti)
{
    PRV_TGF_NOT_SUPPORTED_DEV_CHECK_MAC(prvTgfDevNum, UTF_CH1_E | UTF_CH1_DIAMOND_E | UTF_CH2_E | UTF_CH3_E);

    if(GT_FALSE == prvTgfBrgTtiCheck())
    {
        return;
    }

    /* in xCat PBR mode there is no Tunnel Start*/
    if (prvUtfIsPbrModeUsed() && PRV_CPSS_PP_MAC(prvTgfDevNum)->devFamily == CPSS_PP_FAMILY_DXCH_XCAT_E)
    {
        return;
    }

    prvTgfTunnelMimStart(PRV_TGF_TUNNEL_MIM_START_FROM_TTI_E);

    prvTgfPclRestore();
}

UTF_TEST_CASE_MAC(tgfTunnelMimStartFromPcl)
{
    PRV_TGF_NOT_SUPPORTED_DEV_CHECK_MAC(prvTgfDevNum, UTF_CH1_E | UTF_CH1_DIAMOND_E | UTF_CH2_E | UTF_CH3_E);

    if(GT_FALSE == prvTgfBrgTtiCheck())
    {
        return;
    }

    /* in xCat PBR mode there is no Tunnel Start*/
    if (prvUtfIsPbrModeUsed() && PRV_CPSS_PP_MAC(prvTgfDevNum)->devFamily == CPSS_PP_FAMILY_DXCH_XCAT_E)
    {
        return;
    }

    prvTgfTunnelMimStart(PRV_TGF_TUNNEL_MIM_START_FROM_PCL_E);

    prvTgfPclRestore();
}

/*----------------------------------------------------------------------------*/
/*
    3.1.3 Verify basic MIM tunnel termination functionality
*/
UTF_TEST_CASE_MAC(tgfTunnelTermMimBasic)
{
/*
    1. Set Base configuration
    2. Generate traffic
    3. Restore configuration
*/

    PRV_TGF_NOT_SUPPORTED_DEV_CHECK_MAC(prvTgfDevNum, UTF_CH1_E | UTF_CH1_DIAMOND_E | UTF_CH2_E | UTF_CH3_E);

    if(GT_FALSE == prvTgfBrgTtiCheck())
    {
        return;
    }

    /* Set Base configuration */
    prvTgfTunnelTermMimBasicConfigSet();

    /* Generate traffic */
    prvTgfTunnelTermMimBasicTrafficGenerate();

    /* Restore configuration */
    prvTgfTunnelTermBasicMimConfigRestore();

    prvTgfPclRestore();
}

/*----------------------------------------------------------------------------*/
/*
    3.2.1 MPLS LSR Push 1 label
    3.2.2 MPLS LSR Push 2 label
    3.2.3 MPLS LSR Push 3 label
*/
UTF_TEST_CASE_MAC(tgfTunnelMplsLsrPush1Label)
{
    PRV_TGF_NOT_SUPPORTED_DEV_CHECK_MAC(prvTgfDevNum, UTF_CH1_E | UTF_CH1_DIAMOND_E | UTF_CH2_E | UTF_CH3_E);

    if(GT_FALSE == prvTgfBrgTtiCheck())
    {
        return;
    }

    /* in xCat PBR mode there is no Tunnel Start*/
    if (prvUtfIsPbrModeUsed() && PRV_CPSS_PP_MAC(prvTgfDevNum)->devFamily == CPSS_PP_FAMILY_DXCH_XCAT_E)
    {
        return;
    }

    prvTgfTunnelMplsPush(PRV_TGF_TUNNEL_MPLS_LSR_PUSH_1_LABEL_E);

    prvTgfPclRestore();
}

UTF_TEST_CASE_MAC(tgfTunnelMplsLsrPush2Label)
{
    PRV_TGF_NOT_SUPPORTED_DEV_CHECK_MAC(prvTgfDevNum, UTF_CH1_E | UTF_CH1_DIAMOND_E | UTF_CH2_E | UTF_CH3_E);

    if(GT_FALSE == prvTgfBrgTtiCheck())
    {
        return;
    }

    /* in xCat PBR mode there is no Tunnel Start*/
    if (prvUtfIsPbrModeUsed() && PRV_CPSS_PP_MAC(prvTgfDevNum)->devFamily == CPSS_PP_FAMILY_DXCH_XCAT_E)
    {
        return;
    }

    prvTgfTunnelMplsPush(PRV_TGF_TUNNEL_MPLS_LSR_PUSH_2_LABEL_E);

    prvTgfPclRestore();
}
UTF_TEST_CASE_MAC(tgfTunnelMplsLsrPush3Label)
{
    PRV_TGF_NOT_SUPPORTED_DEV_CHECK_MAC(prvTgfDevNum, UTF_CH1_E | UTF_CH1_DIAMOND_E | UTF_CH2_E | UTF_CH3_E);

    if(GT_FALSE == prvTgfBrgTtiCheck())
    {
        return;
    }

    /* in xCat PBR mode there is no Tunnel Start*/
    if (prvUtfIsPbrModeUsed() && PRV_CPSS_PP_MAC(prvTgfDevNum)->devFamily == CPSS_PP_FAMILY_DXCH_XCAT_E)
    {
        return;
    }

    prvTgfTunnelMplsPush(PRV_TGF_TUNNEL_MPLS_LSR_PUSH_3_LABEL_E);

    prvTgfPclRestore();
}

/*----------------------------------------------------------------------------*/
/*
    3.2.4 MPLS LSR Pop 1 label
    3.2.5 MPLS LSR Pop 2 label
*/
UTF_TEST_CASE_MAC(tgfTunnelMplsLsrPop1Label)
{
    PRV_TGF_NOT_SUPPORTED_DEV_CHECK_MAC(prvTgfDevNum, UTF_CH1_E | UTF_CH1_DIAMOND_E | UTF_CH2_E | UTF_CH3_E);
    if(GT_FALSE == prvTgfBrgTtiCheck())
    {
        return;
    }
    prvTgfTunnelMplsPop(PRV_TGF_TUNNEL_MPLS_LSR_POP_1_LABEL_E);

    prvTgfPclRestore();
}
UTF_TEST_CASE_MAC(tgfTunnelMplsLsrPop2Label)
{
    PRV_TGF_NOT_SUPPORTED_DEV_CHECK_MAC(prvTgfDevNum, UTF_CH1_E | UTF_CH1_DIAMOND_E | UTF_CH2_E | UTF_CH3_E);
    if(GT_FALSE == prvTgfBrgTtiCheck())
    {
        return;
    }
    prvTgfTunnelMplsPop(PRV_TGF_TUNNEL_MPLS_LSR_POP_2_LABEL_E);

    prvTgfPclRestore();
}

/*----------------------------------------------------------------------------*/
/*
    3.2.6 MPLS LSR Swap
    3.2.7 MPLS LSR Pop-Swap
*/
UTF_TEST_CASE_MAC(tgfTunnelMplsLsrSwap)
{
    PRV_TGF_NOT_SUPPORTED_DEV_CHECK_MAC(prvTgfDevNum, UTF_CH1_E | UTF_CH1_DIAMOND_E | UTF_CH2_E | UTF_CH3_E);

    if(GT_FALSE == prvTgfBrgTtiCheck())
    {
        return;
    }

    /* in xCat PBR mode there is no Tunnel Start*/
    if (prvUtfIsPbrModeUsed() && PRV_CPSS_PP_MAC(prvTgfDevNum)->devFamily == CPSS_PP_FAMILY_DXCH_XCAT_E)
    {
        return;
    }

    prvTgfTunnelMplsPush(PRV_TGF_TUNNEL_MPLS_LSR_SWAP_E);

    prvTgfPclRestore();
}
UTF_TEST_CASE_MAC(tgfTunnelMplsLsrPopSwap)
{
    PRV_TGF_NOT_SUPPORTED_DEV_CHECK_MAC(prvTgfDevNum, UTF_CH1_E | UTF_CH1_DIAMOND_E | UTF_CH2_E | UTF_CH3_E);

    if(GT_FALSE == prvTgfBrgTtiCheck())
    {
        return;
    }

    /* in xCat PBR mode there is no Tunnel Start*/
    if (prvUtfIsPbrModeUsed() && PRV_CPSS_PP_MAC(prvTgfDevNum)->devFamily == CPSS_PP_FAMILY_DXCH_XCAT_E)
    {
        return;
    }

    prvTgfTunnelMplsPush(PRV_TGF_TUNNEL_MPLS_LSR_POP_SWAP_E);

    prvTgfPclRestore();
}

/*----------------------------------------------------------------------------*/
/*
    4.1.1 Verify the functionality of UP marking in MIM tunnel start
*/
UTF_TEST_CASE_MAC(tgfTunnelStartUpMarkingMode)
{
/*
    1. Set Base configuration
    2. Generate traffic
    3. Restore configuration
*/

    PRV_TGF_NOT_SUPPORTED_DEV_CHECK_MAC(prvTgfDevNum, UTF_CH1_E | UTF_CH1_DIAMOND_E | UTF_CH2_E | UTF_CH3_E);

    if(GT_FALSE == prvTgfBrgTtiCheck())
    {
        return;
    }

    /* in xCat PBR mode there is no Tunnel Start*/
    if (prvUtfIsPbrModeUsed() && PRV_CPSS_PP_MAC(prvTgfDevNum)->devFamily == CPSS_PP_FAMILY_DXCH_XCAT_E)
    {
        return;
    }

    /* Set Base configuration */
    prvTgfTunnelStartUpMarkingModeConfigSet();

    /* Generate traffic */
    prvTgfTunnelStartUpMarkingModeTrafficGenerate();

    /* Restore configuration */
    prvTgfTunnelStartUpMarkingModeConfigRestore();

    prvTgfPclRestore();
}

/*----------------------------------------------------------------------------*/
/*
    4.1.2 Verify the functionality of the globally set MIM ether type
*/
UTF_TEST_CASE_MAC(tgfTunnelStartMimEtherType)
{
/*
    1. Set Base configuration
    2. Generate traffic
    3. Restore configuration
*/
    PRV_TGF_NOT_SUPPORTED_DEV_CHECK_MAC(prvTgfDevNum, UTF_CH1_E | UTF_CH1_DIAMOND_E | UTF_CH2_E | UTF_CH3_E);

    if(GT_FALSE == prvTgfBrgTtiCheck())
    {
        return;
    }

    /* in xCat PBR mode there is no Tunnel Start*/
    if (prvUtfIsPbrModeUsed() && PRV_CPSS_PP_MAC(prvTgfDevNum)->devFamily == CPSS_PP_FAMILY_DXCH_XCAT_E)
    {
        return;
    }

    /* Set Base configuration */
    prvTgfTunnelStartMimEtherTypeConfigSet();

    /* Generate traffic */
    prvTgfTunnelStartMimEtherTypeTrafficGenerate();

    /* Restore configuration */
    prvTgfTunnelStartMimEtherTypeConfigRestore();

    prvTgfPclRestore();
}

/*----------------------------------------------------------------------------*/
/*
    4.1.3 Verify the functionality of I-DEI marking in MIM tunnel start
*/
UTF_TEST_CASE_MAC(tgfTunnelStartIdeiMarkingMode)
{
/*
    1. Set Base configuration
    2. Generate traffic
    3. Restore configuration
*/

    PRV_TGF_NOT_SUPPORTED_DEV_CHECK_MAC(prvTgfDevNum, UTF_CH1_E | UTF_CH1_DIAMOND_E | UTF_CH2_E | UTF_CH3_E);

    if(GT_FALSE == prvTgfBrgTtiCheck())
    {
        return;
    }

    /* in xCat PBR mode there is no Tunnel Start*/
    if (prvUtfIsPbrModeUsed() && PRV_CPSS_PP_MAC(prvTgfDevNum)->devFamily == CPSS_PP_FAMILY_DXCH_XCAT_E)
    {
        return;
    }

    /* Set Base configuration */
    prvTgfTunnelStartIdeiMarkingModeConfigSet();

    /* Generate traffic */
    prvTgfTunnelStartIdeiMarkingModeTrafficGenerate();

    /* Restore configuration */
    prvTgfTunnelStartIdeiMarkingModeConfigRestore();

    prvTgfPclRestore();
}

/*----------------------------------------------------------------------------*/
/*
    4.1.4 Test functionality of MIM MAC SA Assignment TODO
*/

/*----------------------------------------------------------------------------*/
/*
    4.2.1 Test functionality of MIM Ether Type TODO
*/

/*----------------------------------------------------------------------------*/
/*
    4.2.2 Test functionality of MIM Mac2Me Restriction TODO
*/

/*----------------------------------------------------------------------------*/
/*
    4.3.1 Test functionality of MIM TTI key MAC DA or MAC SA
*/
UTF_TEST_CASE_MAC(tgfTunnelTermUseMacSaInMimTtiLookup)
{
/*
    1. Set Base configuration
    2. Generate traffic
    3. Restore configuration
*/

    PRV_TGF_NOT_SUPPORTED_DEV_CHECK_MAC(prvTgfDevNum, UTF_CH1_E | UTF_CH1_DIAMOND_E | UTF_CH2_E | UTF_CH3_E);

    if(GT_FALSE == prvTgfBrgTtiCheck())
    {
        return;
    }

    /* Set Base configuration */
    prvTgfTunnelTermUseMacSaInMimTtiLookupConfigSet();

    /* Generate traffic */
    prvTgfTunnelTermUseMacSaInMimTtiLookupTrafficGenerate();

    /* Restore configuration */
    prvTgfTunnelTermUseMacSaInMimTtiLookupConfigRestore();

    prvTgfPclRestore();
}

/*----------------------------------------------------------------------------*/
/*
    4.3.2 Test functionality of masked fields in the MIM TTI key
*/
UTF_TEST_CASE_MAC(tgfTunnelTermMaskMacInMimTtiLookup)
{
/*
    1. Set Base configuration
    2. Generate traffic
    3. Restore configuration
*/

    PRV_TGF_NOT_SUPPORTED_DEV_CHECK_MAC(prvTgfDevNum, UTF_CH1_E | UTF_CH1_DIAMOND_E | UTF_CH2_E | UTF_CH3_E);

    if(GT_FALSE == prvTgfBrgTtiCheck())
    {
        return;
    }

    /* Set Base configuration */
    prvTgfTunnelTermMaskMacInMimTtiLookupConfigSet();

    /* Generate traffic */
    prvTgfTunnelTermMaskMacInMimTtiLookupTrafficGenerate();

    /* Restore configuration */
    prvTgfTunnelTermMaskMacInMimTtiLookupConfigRestore();

    prvTgfPclRestore();
}

/*----------------------------------------------------------------------------*/
/*
    4.4.5 Verify the functionality of Ethernet TTI key type.
*/
UTF_TEST_CASE_MAC(tgfTunnelTermEtherType)
{
/*
    1. Set TTI Ether Type Key Base configuration
    2. Generate traffic -- check traffic is blocked
    3. Invalidate Rule
    4. Generate traffic -- check traffic is not blocked
    5. Validate rule
    6. Generate traffic -- check traffic is blocked
    7. Generate traffic on different MAC DA -- check traffic is not blocked
    8. Restore configuration
*/
    if(GT_FALSE == prvTgfBrgTtiCheck())
    {
        return;
    }

    /* Set TTI Ether Type Key Base configuration */
    prvTgfTunnelTermEtherTypeBaseConfigurationSet();

    /* Generate traffic -- check no traffic */
    prvTgfTunnelTermEtherTypeTrafficGenerateExpectNoTraffic();

    /* invalidate TTI rules */
    prvTgfTunnelTermRuleValidStatusSet(GT_FALSE);

    /* Generate traffic */
    prvTgfTunnelTermEtherTypeTrafficGenerate();

    /* validate TTI rules */
    prvTgfTunnelTermRuleValidStatusSet(GT_TRUE);

   /* Generate traffic -- check no traffic */
    prvTgfTunnelTermEtherTypeTrafficGenerateExpectNoTraffic();

    /* Generate traffic on different MAC DA -- check traffic is not blocked*/
    prvTgfTunnelTermEtherTypeDifferentTrafficGenerate();

    /* Restore configuration */
    prvTgfTunnelTermEtherTypeConfigurationRestore();

    prvTgfPclRestore();
}

/*----------------------------------------------------------------------------*/
/*
    4.4.1 Verify the functionality of the Tag1 VLAN command field in TTI action entry
*/
UTF_TEST_CASE_MAC(tgfTunnelTermTag1CmdField)
{
/*
    1. Set Base configuration
    2. Generate traffic
    3. Restore configuration
*/

    PRV_TGF_NOT_SUPPORTED_DEV_CHECK_MAC(prvTgfDevNum, UTF_CH1_E | UTF_CH1_DIAMOND_E | UTF_CH2_E | UTF_CH3_E);

    if(GT_FALSE == prvTgfBrgTtiCheck())
    {
        return;
    }

    /* Set Base configuration */
    prvTgfTunnelTermTag1VlanCommandFieldConfigSet();

    /* Generate traffic */
    prvTgfTunnelTermTag1VlanCommandFieldTrafficGenerate();

    /* Restore configuration */
    prvTgfTunnelTermTag1VlanCommandFieldConfigRestore();

    prvTgfPclRestore();
}

/*----------------------------------------------------------------------------*/
/*
    4.4.2 Verify the functionality of trust EXP QoS field in TTI action entry
*/
UTF_TEST_CASE_MAC(tgfTunnelTermExpQosField)
{
/*
    1. Set Base configuration
    2. Generate traffic
    3. Restore configuration
*/
    PRV_TGF_NOT_SUPPORTED_DEV_CHECK_MAC(prvTgfDevNum, UTF_XCAT2_E);

    if(GT_FALSE == prvTgfBrgTtiCheck())
    {
        return;
    }

    /* Set Base configuration */
    prvTgfTunnelTermTrustExpQosFieldConfigSet();

    /* Generate traffic */
    prvTgfTunnelTermTrustExpQosFieldTrafficGenerate();

    /* Restore configuration */
    prvTgfTunnelTermTrustExpQosFieldConfigRestore();

    prvTgfPclRestore();
}

/*----------------------------------------------------------------------------*/
/*
    4.4.3 Verify the functionality of trust EXP QoS field in TTI action entry
*/
UTF_TEST_CASE_MAC(tgfTunnelExpQosFieldNonTerm)
{
/*
    1. Set Base configuration
    2. Generate traffic
    3. Restore configuration
*/
    if(GT_FALSE == prvTgfBrgTtiCheck())
    {
        return;
    }

    /* Set Base configuration */
    prvTgfTunnelTrustExpQosFieldNonTermConfigSet();

    /* Generate traffic */
    prvTgfTunnelTrustExpQosFieldNonTermTrafficGenerate();

    /* Restore configuration */
    prvTgfTunnelTrustExpQosFieldNonTermConfigRestore();

    prvTgfPclRestore();
}

/*----------------------------------------------------------------------------*/
/*
    4.4.4 Verify the functionality of QoS profile field in TTI action entry
*/
UTF_TEST_CASE_MAC(tgfTunnelTermTrustAnyQosField)
{
/*
    1. Set Base configuration
    2. Generate traffic
    3. Restore configuration
*/
    PRV_TGF_NOT_SUPPORTED_DEV_CHECK_MAC(prvTgfDevNum, UTF_XCAT2_E);

    if(GT_FALSE == prvTgfBrgTtiCheck())
    {
        return;
    }

    /* Set Base configuration */
    prvTgfTunnelTermTrustAnyQosFieldConfigSet();

    /* Generate traffic */
    prvTgfTunnelTermTrustAnyQosFieldTrafficGenerate();

    /* Restore configuration */
    prvTgfTunnelTermTrustAnyQosFieldConfigRestore();

    prvTgfPclRestore();
}

/*----------------------------------------------------------------------------*/
/*
    4.4.5 Test functionality of Trust DSCP QoS Profile Field TODO
*/
UTF_TEST_CASE_MAC(tgfTunnelTermTrustDscpQosField)
{
/*
    1. Set Base configuration
    2. Generate traffic
    3. Restore configuration
*/
    PRV_TGF_NOT_SUPPORTED_DEV_CHECK_MAC(prvTgfDevNum, UTF_XCAT2_E);

    if(GT_FALSE == prvTgfBrgTtiCheck())
    {
        return;
    }

    /* Set Base configuration */
    prvTgfTunnelTermTrustDscpQosFieldConfigSet();

    /* Generate traffic */
    prvTgfTunnelTermTrustDscpQosFieldTrafficGenerate();

    /* Restore configuration */
    prvTgfTunnelTermTrustDscpQosFieldConfigRestore();

    prvTgfPclRestore();
}

/*----------------------------------------------------------------------------*/
/*
    4.4.6 Test functionality of Trust UP QoS Profile Field TODO
*/
UTF_TEST_CASE_MAC(tgfTunnelTermTrustUpQosField)
{
/*
    1. Set Base configuration
    2. Generate traffic
    3. Restore configuration
*/
    PRV_TGF_NOT_SUPPORTED_DEV_CHECK_MAC(prvTgfDevNum, UTF_XCAT2_E);

    if(GT_FALSE == prvTgfBrgTtiCheck())
    {
        return;
    }

    /* Set Base configuration */
    prvTgfTunnelTermTrustUpQosFieldConfigSet();

    /* Generate traffic */
    prvTgfTunnelTermTrustUpQosFieldTrafficGenerate();

    /* Restore configuration */
    prvTgfTunnelTermTrustUpQosFieldConfigRestore();

    prvTgfPclRestore();
}

/*----------------------------------------------------------------------------*/
/*
    4.4.7 Test functionality of Other TTI Action Type 2 Fields TODO
*/

/*----------------------------------------------------------------------------*/
/*
    4.5.1 Test functionality of MPLS Illegal TTL Exception TODO
*/

/*----------------------------------------------------------------------------*/
/*
    4.5.2 Test functionality of MPLS Unsupported Exception TODO
*/

/*----------------------------------------------------------------------------*/
/*
    4.6.1 Test tunnel termination IPv4 over IPv4 to All Port Groups
*/
UTF_TEST_CASE_MAC(tgfTunnelTermGlobalPort2LocalPortConf1)
{
/*
    1. Set Base configuration
    2. Set Route configuration
    3. Set TTI configuration1
    4. Generate traffic
    5. Restore configuration
*/
    int i,j;

    PRV_TGF_NOT_SUPPORTED_DEV_CHECK_MAC(prvTgfDevNum, UTF_CH1_E | UTF_CH1_DIAMOND_E | UTF_CH2_E | UTF_CH3_E | UTF_XCAT_E | UTF_XCAT2_E);

    if(GT_FALSE == prvTgfBrgTtiCheck())
    {
        return;
    }

    SAVE_ORIG_PORTS_MAC;
    /******************** test for ports 3 19 35 *********************/
    prvTgfPortsArray[0] = PRV_TGF_SEND_PORT1_NUM_CNS;
    prvTgfPortsArray[1] = PRV_TGF_SEND_PORT2_NUM_CNS;
    prvTgfPortsArray[2] = PRV_TGF_SEND_PORT3_NUM_CNS;
    prvTgfPortsArray[3] = PRV_TGF_NEXTHOPE_PORT_NUM_CNS;

    /* Set Base configuration */
    prvTgfTunnelTermGlobalPort2LocalPortBaseConfigurationSet();

    /* Set Route configuration */
    prvTgfTunnelTermGlobalPort2LocalPortRouteConfigurationSet();

    for (i=0; i<3; i++)
    {
        /* Set TTI configuration */
        prvTgfTunnelTermGlobalPort2LocalPortTtiConfigurationSet(1,i);

        /* Generate traffic */
        for (j=0; j<3; j++)
            prvTgfTunnelTermGlobalPort2LocalPortTrafficGenerate(1,i,j);
    }

    /* Restore configuration */
    prvTgfTunnelTermGlobalPort2LocalPortConfigurationRestore();

    /******************** test for ports 19 35 51 *********************/
    prvTgfPortsArray[0] = PRV_TGF_SEND_PORT2_NUM_CNS;
    prvTgfPortsArray[1] = PRV_TGF_SEND_PORT3_NUM_CNS;
    prvTgfPortsArray[2] = PRV_TGF_SEND_PORT4_NUM_CNS;
    prvTgfPortsArray[3] = PRV_TGF_NEXTHOPE_PORT_NUM_CNS;

    /* Set Base configuration */
    prvTgfTunnelTermGlobalPort2LocalPortBaseConfigurationSet();

    /* Set Route configuration */
    prvTgfTunnelTermGlobalPort2LocalPortRouteConfigurationSet();

    for (i=1; i<4; i++)
    {
        /* Set TTI configuration */
        prvTgfTunnelTermGlobalPort2LocalPortTtiConfigurationSet(1,i);

        /* Generate traffic */
        for (j=1; j<4; j++)
            prvTgfTunnelTermGlobalPort2LocalPortTrafficGenerate(1,i,j);
    }

    /* Restore configuration */
    prvTgfTunnelTermGlobalPort2LocalPortConfigurationRestore();

    RESTORE_ORIG_PORTS_MAC;

    prvTgfPclRestore();
}

/*----------------------------------------------------------------------------*/
/*
    4.6.2 Test tunnel termination IPv4 over IPv4 to a single Port Group
*/
UTF_TEST_CASE_MAC(tgfTunnelTermGlobalPort2LocalPortConf2)
{
/*
    1. Set Base configuration
    2. Set Route configuration
    3. Set TTI configuration2
    4. Generate traffic
    5. Restore configuration
*/

    int i,j;

    PRV_TGF_NOT_SUPPORTED_DEV_CHECK_MAC(prvTgfDevNum, UTF_CH1_E | UTF_CH1_DIAMOND_E | UTF_CH2_E | UTF_CH3_E | UTF_XCAT_E | UTF_XCAT2_E);

    if(GT_FALSE == prvTgfBrgTtiCheck())
    {
        return;
    }

    SAVE_ORIG_PORTS_MAC;

    /******************** test for ports 3 19 35 *********************/
    prvTgfPortsArray[0] = PRV_TGF_SEND_PORT1_NUM_CNS;
    prvTgfPortsArray[1] = PRV_TGF_SEND_PORT2_NUM_CNS;
    prvTgfPortsArray[2] = PRV_TGF_SEND_PORT3_NUM_CNS;
    prvTgfPortsArray[3] = PRV_TGF_NEXTHOPE_PORT_NUM_CNS;

    /* Set Base configuration */
    prvTgfTunnelTermGlobalPort2LocalPortBaseConfigurationSet();

    /* Set Route configuration */
    prvTgfTunnelTermGlobalPort2LocalPortRouteConfigurationSet();

    for (i=0; i<3; i++)
    {
        /* Set TTI configuration */
        prvTgfTunnelTermGlobalPort2LocalPortTtiConfigurationSet(2,i);

        /* Generate traffic */
        for (j=0; j<3; j++)
            prvTgfTunnelTermGlobalPort2LocalPortTrafficGenerate(2,i,j);
    }

    /* Restore configuration */
    prvTgfTunnelTermGlobalPort2LocalPortConfigurationRestore();

    /******************** test for ports 19 35 51 *********************/
    prvTgfPortsArray[0] = PRV_TGF_SEND_PORT2_NUM_CNS;
    prvTgfPortsArray[1] = PRV_TGF_SEND_PORT3_NUM_CNS;
    prvTgfPortsArray[2] = PRV_TGF_SEND_PORT4_NUM_CNS;
    prvTgfPortsArray[3] = PRV_TGF_NEXTHOPE_PORT_NUM_CNS;

    /* Set Base configuration */
    prvTgfTunnelTermGlobalPort2LocalPortBaseConfigurationSet();

    /* Set Route configuration */
    prvTgfTunnelTermGlobalPort2LocalPortRouteConfigurationSet();

    for (i=1; i<4; i++)
    {
        /* Set TTI configuration */
        prvTgfTunnelTermGlobalPort2LocalPortTtiConfigurationSet(2,i);

        /* Generate traffic */
        for (j=1; j<4; j++)
            prvTgfTunnelTermGlobalPort2LocalPortTrafficGenerate(2,i,j);
    }

    /* Restore configuration */
    prvTgfTunnelTermGlobalPort2LocalPortConfigurationRestore();

    RESTORE_ORIG_PORTS_MAC;

    prvTgfPclRestore();
}

/*----------------------------------------------------------------------------*/
/*
    4.6.3 Test tunnel termination IPv4 over IPv4 to spesific Port Groups
*/
UTF_TEST_CASE_MAC(tgfTunnelTermGlobalPort2LocalPortConf3)
{
/*
    1. Set Base configuration
    2. Set Route configuration
    3. Set TTI configuration3
    4. Generate traffic
    5. Restore configuration
*/

    int i,j;

    PRV_TGF_NOT_SUPPORTED_DEV_CHECK_MAC(prvTgfDevNum, UTF_CH1_E | UTF_CH1_DIAMOND_E | UTF_CH2_E | UTF_CH3_E | UTF_XCAT_E | UTF_XCAT2_E);

    if(GT_FALSE == prvTgfBrgTtiCheck())
    {
        return;
    }

    SAVE_ORIG_PORTS_MAC;

    /******************** test for ports 3 19 35 *********************/
    prvTgfPortsArray[0] = PRV_TGF_SEND_PORT1_NUM_CNS;
    prvTgfPortsArray[1] = PRV_TGF_SEND_PORT2_NUM_CNS;
    prvTgfPortsArray[2] = PRV_TGF_SEND_PORT3_NUM_CNS;
    prvTgfPortsArray[3] = PRV_TGF_NEXTHOPE_PORT_NUM_CNS;

    /* Set Base configuration */
    prvTgfTunnelTermGlobalPort2LocalPortBaseConfigurationSet();

    /* Set Route configuration */
    prvTgfTunnelTermGlobalPort2LocalPortRouteConfigurationSet();

    for (i=0; i<3; i++)
    {
        /* Set TTI configuration */
        prvTgfTunnelTermGlobalPort2LocalPortTtiConfigurationSet(3,i);

        /* Generate traffic */
        for (j=0; j<3; j++)
            prvTgfTunnelTermGlobalPort2LocalPortTrafficGenerate(3,i,j);
    }

    /* Restore configuration */
    prvTgfTunnelTermGlobalPort2LocalPortConfigurationRestore();

    /******************** test for ports 19 35 51 *********************/
    prvTgfPortsArray[0] = PRV_TGF_SEND_PORT2_NUM_CNS;
    prvTgfPortsArray[1] = PRV_TGF_SEND_PORT3_NUM_CNS;
    prvTgfPortsArray[2] = PRV_TGF_SEND_PORT4_NUM_CNS;
    prvTgfPortsArray[3] = PRV_TGF_NEXTHOPE_PORT_NUM_CNS;

    /* Set Base configuration */
    prvTgfTunnelTermGlobalPort2LocalPortBaseConfigurationSet();

    /* Set Route configuration */
    prvTgfTunnelTermGlobalPort2LocalPortRouteConfigurationSet();

    for (i=1; i<4; i++)
    {
        /* Set TTI configuration */
        prvTgfTunnelTermGlobalPort2LocalPortTtiConfigurationSet(3,i);

        /* Generate traffic */
        for (j=1; j<4; j++)
            prvTgfTunnelTermGlobalPort2LocalPortTrafficGenerate(3,i,j);
    }

    /* Restore configuration */
    prvTgfTunnelTermGlobalPort2LocalPortConfigurationRestore();

    RESTORE_ORIG_PORTS_MAC;

    prvTgfPclRestore();
}

/*----------------------------------------------------------------------------*/
/*
   5. Test tunnel termination ethernet over IPv4 redirect to egress
*/
UTF_TEST_CASE_MAC(tgfTunnelTermEtherOverIpv4RedirectToEgress)
{
/*
    1. Set Base configuration
    2. Set Route configuration
    3. Set TTI configuration
    4. Generate traffic
    5. Restore configuration
*/

    /* check xcat and above */
    PRV_TGF_NOT_SUPPORTED_DEV_CHECK_MAC(prvTgfDevNum, UTF_CH1_E | UTF_CH1_DIAMOND_E | UTF_CH2_E | UTF_CH3_E);

    /* check if the device supports the tti */
    if(GT_FALSE == prvTgfBrgTtiCheck())
    {
        return;
    }

    /* verify this is not xCat A1 / xCat A2 */
    if (GT_TRUE != PRV_CPSS_DXCH_XCAT_A3_ONLY_CHECK_MAC(prvTgfDevNum))
    {
        return;
    }

    /* Set Base configuration */
    prvTgfTunnelTermEtherOverIpv4RedirectToEgressBridgeConfigSet();

    /* Set TTI configuration */
    prvTgfTunnelTermEtherOverIpv4RedirectToEgressTtiConfigSet();

    /* Generate traffic */
    prvTgfTunnelTermEtherOverIpv4RedirectToEgressTrafficGenerate();

    /* Restore configuration */
    prvTgfTunnelTermEtherOverIpv4RedirectToEgressConfigurationRestore();
}

/*----------------------------------------------------------------------------*/
/*
   5.1 Test tunnel termination ethernet over IPv4 GRE redirect to egress
*/
UTF_TEST_CASE_MAC(tgfTunnelTermEtherOverIpv4GreRedirectToEgress)
{
/*
    1. Set Base configuration
    2. Set Route configuration
    3. Set TTI configuration
    4. Generate traffic
    5. Restore configuration
*/

    /* check xcat and above */
    PRV_TGF_NOT_SUPPORTED_DEV_CHECK_MAC(prvTgfDevNum, UTF_CH1_E | UTF_CH1_DIAMOND_E | UTF_CH2_E | UTF_CH3_E);

    /* check if the device supports the tti */
    if(GT_FALSE == prvTgfBrgTtiCheck())
    {
        return;
    }

    /* verify this is not xCat A1 / xCat A2 */
    if (GT_TRUE != PRV_CPSS_DXCH_XCAT_A3_ONLY_CHECK_MAC(prvTgfDevNum))
    {
        return;
    }

    /* Set Bridge configuration */
    prvTgfTunnelTermEtherOverIpv4GreRedirectToEgressBridgeConfigSet(GT_FALSE);

    /* Set TTI configuration */
    prvTgfTunnelTermEtherOverIpv4GreRedirectToEgressTtiConfigSet();

    /* Generate traffic */
    prvTgfTunnelTermEtherOverIpv4GreRedirectToEgressTrafficGenerateUntagged();

    /* Change all ports to be tagged */
    prvTgfTunnelTermEtherOverIpv4GreRedirectToEgressBridgeConfigSet(GT_TRUE);

    /* Generate traffic */
    prvTgfTunnelTermEtherOverIpv4GreRedirectToEgressTrafficGenerateTagged();

    /* Restore configuration */
    prvTgfTunnelTermEtherOverIpv4GreRedirectToEgressConfigurationRestore();
}

/*----------------------------------------------------------------------------*/
/*
   5. Test tunnel start ethernet over IPv4 redirect to egress
*/
UTF_TEST_CASE_MAC(tgfTunnelStartEtherOverIpv4GreRedirectToEgress)
{
/*
    1. Set Base configuration
    2. Set Route configuration
    3. Set TTI configuration
    4. Generate traffic
    5. Restore configuration
*/

    /* check xcat and above */
    PRV_TGF_NOT_SUPPORTED_DEV_CHECK_MAC(prvTgfDevNum, UTF_CH1_E | UTF_CH1_DIAMOND_E | UTF_CH2_E | UTF_CH3_E);

    /* check if the device supports the tti */
    if(GT_FALSE == prvTgfBrgTtiCheck())
    {
        return;
    }

    /* verify this is not xCat A1 / xCat A2 */
    if (GT_TRUE != PRV_CPSS_DXCH_XCAT_A3_ONLY_CHECK_MAC(prvTgfDevNum))
    {
        return;
    }

    /* Set Base configuration */
    prvTgfTunnelStartEtherOverIpv4GreRedirectToEgressBridgeConfigSet();

    /* Set Tunnel configuration */
    prvTgfTunnelStartEtherOverIpv4GreRedirectToEgressTunnelConfigSet();

    /* Set Pcl configuration */
    prvTgfTunnelStartEtherOverIpv4GreRedirectToEgressPclConfigSet();

    /* Generate traffic */
    prvTgfTunnelStartEtherOverIpv4GreRedirectToEgressTrafficGenerate();

    /* Restore configuration */
    prvTgfTunnelStartEtherOverIpv4GreRedirectToEgressConfigurationRestore();
}

/*----------------------------------------------------------------------------*/
/*
   5. Test tunnel start ethernet over IPv4 redirect to egress (with offset)
*/
UTF_TEST_CASE_MAC(tgfTunnelStartEtherOverIpv4GreRedirectToEgressOffset)
{
/*
    1. Set Base configuration
    2. Set Route configuration
    3. Set TTI configuration
    4. Generate traffic
    5. Restore configuration
*/

    /* check xcat and above */
    PRV_TGF_NOT_SUPPORTED_DEV_CHECK_MAC(prvTgfDevNum, UTF_CH1_E | UTF_CH1_DIAMOND_E | UTF_CH2_E | UTF_CH3_E);

    /* check if the device supports the tti */
    if(GT_FALSE == prvTgfBrgTtiCheck())
    {
        return;
    }

    /* verify this is not xCat A1 / xCat A2 */
    if (GT_TRUE != PRV_CPSS_DXCH_XCAT_A3_ONLY_CHECK_MAC(prvTgfDevNum))
    {
        return;
    }

    /* Set Base configuration */
    prvTgfTunnelStartEtherOverIpv4GreRedirectToEgressBridgeConfigSetOffset();

    /* Set Tunnel configuration */
    prvTgfTunnelStartEtherOverIpv4GreRedirectToEgressTunnelConfigSetOffset();

    /* Set Pcl configuration */
    prvTgfTunnelStartEtherOverIpv4GreRedirectToEgressPclConfigSetOffset();

    /* Generate traffic */
    prvTgfTunnelStartEtherOverIpv4GreRedirectToEgressTrafficGenerateOffset();

    /* Restore configuration */
    prvTgfTunnelStartEtherOverIpv4GreRedirectToEgressConfigurationRestoreOffset();
}

/*----------------------------------------------------------------------------*/
/*
   6. Test ethernet lookup and redirect to egress
*/
UTF_TEST_CASE_MAC(tgfEtherRedirectToEgress)
{
/*
    1. Set Base configuration
    2. Set Route configuration
    3. Set TTI configuration
    4. Generate traffic
    5. Restore configuration
*/

    /* check xcat and above */
    PRV_TGF_NOT_SUPPORTED_DEV_CHECK_MAC(prvTgfDevNum, UTF_CH1_E | UTF_CH1_DIAMOND_E | UTF_CH2_E);

    /* check if the device supports the tti */
    if(GT_FALSE == prvTgfBrgTtiCheck())
    {
        return;
    }

    /* Set Base configuration */
    prvTgfEtherRedirectToEgressBridgeConfigSet();

    /* Set TTI configuration */
    prvTgfEtherRedirectToEgressTtiConfigSet();

    /* Generate traffic */
    prvTgfEtherRedirectToEgressTrafficGenerate();

    /* Restore configuration */
    prvTgfEtherRedirectToEgressConfigurationRestore();
}

/*
 * Configuration of tgfTunnel suit
 */
UTF_SUIT_BEGIN_TESTS_MAC(tgfTunnel)

    UTF_SUIT_DECLARE_TEST_MAC(tgfTunnelTermIpv6overIpv4)
    UTF_SUIT_DECLARE_TEST_MAC(tgfTunnelTermIpv4overIpv4)
    UTF_SUIT_DECLARE_TEST_MAC(tgfTunnelTermIpv4overIpv4Invalidate)
    UTF_SUIT_DECLARE_TEST_MAC(tgfTunnelTermIpv4overIpv4Trap)

    UTF_SUIT_DECLARE_TEST_MAC(tgfTunnelTermIpv4overIpv4SinglePortGroup)

    UTF_SUIT_DECLARE_TEST_MAC(tgfTunnelMplsLsrPop1Label)
    UTF_SUIT_DECLARE_TEST_MAC(tgfTunnelMplsLsrPop2Label)

    UTF_SUIT_DECLARE_TEST_MAC(tgfTunnelTermMimBasic)
    UTF_SUIT_DECLARE_TEST_MAC(tgfTunnelTermEtherType)

    UTF_SUIT_DECLARE_TEST_MAC(tgfTunnelTermUseMacSaInMimTtiLookup)
    UTF_SUIT_DECLARE_TEST_MAC(tgfTunnelTermMaskMacInMimTtiLookup)

    UTF_SUIT_DECLARE_TEST_MAC(tgfTunnelTermTag1CmdField)
    UTF_SUIT_DECLARE_TEST_MAC(tgfTunnelTermExpQosField)
    UTF_SUIT_DECLARE_TEST_MAC(tgfTunnelExpQosFieldNonTerm)

    UTF_SUIT_DECLARE_TEST_MAC(tgfTunnelTermTrustAnyQosField)
    UTF_SUIT_DECLARE_TEST_MAC(tgfTunnelTermTrustDscpQosField)
    UTF_SUIT_DECLARE_TEST_MAC(tgfTunnelTermTrustUpQosField)

    UTF_SUIT_DECLARE_TEST_MAC(tgfTunnelTermGlobalPort2LocalPortConf1)
    UTF_SUIT_DECLARE_TEST_MAC(tgfTunnelTermGlobalPort2LocalPortConf2)
    UTF_SUIT_DECLARE_TEST_MAC(tgfTunnelTermGlobalPort2LocalPortConf3)

    UTF_SUIT_DECLARE_TEST_MAC(tgfTunnelTermEtherOverIpv4RedirectToEgress)
    UTF_SUIT_DECLARE_TEST_MAC(tgfTunnelStartEtherOverIpv4GreRedirectToEgress)
    UTF_SUIT_DECLARE_TEST_MAC(tgfTunnelStartEtherOverIpv4GreRedirectToEgressOffset)
    UTF_SUIT_DECLARE_TEST_MAC(tgfTunnelTermEtherOverIpv4GreRedirectToEgress)

    UTF_SUIT_DECLARE_TEST_MAC(tgfEtherRedirectToEgress)

    UTF_SUIT_DECLARE_TEST_MAC(tgfTunnelStartIpv4overGreIpv4)
    UTF_SUIT_DECLARE_TEST_MAC(tgfTunnelStartIpv6overIpv4)

    UTF_SUIT_DECLARE_TEST_MAC(tgfTunnelStartUpMarkingMode)
    UTF_SUIT_DECLARE_TEST_MAC(tgfTunnelStartMimEtherType)
    UTF_SUIT_DECLARE_TEST_MAC(tgfTunnelStartIdeiMarkingMode)

    UTF_SUIT_DECLARE_TEST_MAC(tgfTunnelMimStartFromTti)
    UTF_SUIT_DECLARE_TEST_MAC(tgfTunnelMimStartFromPcl)

    UTF_SUIT_DECLARE_TEST_MAC(tgfTunnelMplsLsrPush1Label)
    UTF_SUIT_DECLARE_TEST_MAC(tgfTunnelMplsLsrPush2Label)
    UTF_SUIT_DECLARE_TEST_MAC(tgfTunnelMplsLsrPush3Label)

    UTF_SUIT_DECLARE_TEST_MAC(tgfTunnelMplsLsrSwap)
    UTF_SUIT_DECLARE_TEST_MAC(tgfTunnelMplsLsrPopSwap)
UTF_SUIT_END_TESTS_MAC(tgfTunnel)




