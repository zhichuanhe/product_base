/*******************************************************************************
*              (c), Copyright 2001, Marvell International Ltd.                 *
* THIS CODE CONTAINS CONFIDENTIAL INFORMATION OF MARVELL SEMICONDUCTOR, INC.   *
* NO RIGHTS ARE GRANTED HEREIN UNDER ANY PATENT, MASK WORK RIGHT OR COPYRIGHT  *
* OF MARVELL OR ANY THIRD PARTY. MARVELL RESERVES THE RIGHT AT ITS SOLE        *
* DISCRETION TO REQUEST THAT THIS CODE BE IMMEDIATELY RETURNED TO MARVELL.     *
* THIS CODE IS PROVIDED "AS IS". MARVELL MAKES NO WARRANTIES, EXPRESSED,       *
* IMPLIED OR OTHERWISE, REGARDING ITS ACCURACY, COMPLETENESS OR PERFORMANCE.   *
********************************************************************************
* prvTgfNstPortIsolation.c
*
* DESCRIPTION:
*       Port Isolation
*
* FILE REVISION NUMBER:
*       $Revision: 1.1.1.1 $
*
*******************************************************************************/

#include <cpssCommon/cpssPresteraDefs.h>
#include <cpss/generic/cpssCommonDefs.h>
#include <cpss/generic/bridge/cpssGenBrgVlanTypes.h>
#include <cpss/extServices/private/prvCpssBindFunc.h>

#include <utf/utfMain.h>
#include <utf/private/prvUtfExtras.h>
#include <utf/private/prvUtfHelpers.h>

#include <trafficEngine/tgfTrafficGenerator.h>
#include <trafficEngine/tgfTrafficTable.h>

#include <common/tgfCommon.h>
#include <common/tgfBridgeGen.h>
#include <common/tgfIpGen.h>
#include <common/tgfNstGen.h>
#include <common/tgfTrunkGen.h>
#include <nst/prvTgfNstPortIsolation.h>


/******************************************************************************\
 *                       Test configuration section                           *
\******************************************************************************/
/* default VLAN0 Id */
#define PRV_TGF_VLANID_CNS  5

/* nextHop VLAN Id */
#define PRV_TGF_NEXTHOPE_VLANID_CNS   6

/* default Trunk Id */
#define PRV_TGF_TRUNK_ID_CNS  1

/* port number to send traffic to */
#define PRV_TGF_SEND_PORT_IDX_CNS         1

/* another port number to send traffic to */
#define PRV_TGF_SEND_PORT_IDX_ANOTHER_CNS 0

/* count of packets to send */
#define PRV_TGF_SEND_COUNT_CNS 2

/* default number of packets to send */
static GT_U32  prvTgfBurstCount   = 1;

/* port bitmap VLAN members */
static CPSS_PORTS_BMP_STC localPortsVlanMembers = {{0, 0}};

/* port bitmap L2 Port Isolation Table */
static CPSS_PORTS_BMP_STC localPortsMembers;

/* port bitmap L2 and L3 Port Isolation Table */
static GT_U32 localL2L3PortsIdxMembers[] = {1, 2};

/* array of trunk members */
static CPSS_TRUNK_MEMBER_STC   trunkMembersArray[2] = {{0, 0}, {0, 0}};

/* type of traffic for port isolation */
static PRV_TGF_NST_PORT_ISOLATION_TRAFFIC_TYPE_ENT portIsolationTraffic = PRV_TGF_NST_PORT_ISOLATION_TRAFFIC_TYPE_L2_E;

/* port isolation command of VLAN entry */
static PRV_TGF_BRG_VLAN_PORT_ISOLATION_CMD_ENT portIsolationCmd = PRV_TGF_BRG_VLAN_PORT_ISOLATION_L2_CMD_E;

/* the Arp Address indexes of the Router ARP Table
   to write to the UC Route entry Arp nextHopARPPointer field */
static GT_U32        prvTgfRouterArpIndexArray[]      = {1, 5};

/* the Route entry index for UC Route entry Table */
static GT_U32        prvTgfRouteEntryBaseIndexArray[] = {3, 5};

/* nextHop port numbers to receive traffic from */
static GT_U8         prvTgfNextHopePortIdxArray[]     = {2, 3};

/* the ARP MAC addresses to write to the Router ARP Table */
static TGF_MAC_ADDR  prvTgfArpMacArray[] = {{0x00, 0x00, 0x00, 0x00, 0x00, 0x22},
                                            {0x00, 0x00, 0x00, 0x00, 0x00, 0x33}};

/* the dst ip addresses for test packets */
static TGF_IPV4_ADDR prvTgfIpv4AddrArray[] = {{ 1,  1,  1,  3}, { 1,  2,  3,  4}};

/* the LPM DB id for LPM Table */
static GT_U32        prvTgfLpmDBId       = 0;

/******************************* Test packet **********************************/

/* L2 part of packet */
static TGF_PACKET_L2_STC prvTgfPacketL2Part = {
    {0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF},               /* dstMac */
    {0x00, 0x00, 0x00, 0x00, 0x00, 0x02}                /* srcMac */
};

/* First VLAN_TAG part */
static TGF_PACKET_VLAN_TAG_STC prvTgfPacketVlanTag0Part = {
    TGF_ETHERTYPE_8100_VLAN_TAG_CNS,                    /* etherType */
    0, 0, PRV_TGF_VLANID_CNS                            /* pri, cfi, VlanId */
};


/* DATA of packet */
static GT_U8 prvTgfPayloadDataArr[] = {
    0x55, 0x55, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00
};
/* PAYLOAD part */
static TGF_PACKET_PAYLOAD_STC prvTgfPacketPayloadPart = {
    sizeof(prvTgfPayloadDataArr),                       /* dataLength */
    prvTgfPayloadDataArr                                /* dataPtr */
};

/* PARTS of packet */
static TGF_PACKET_PART_STC prvTgfPacketPartArray[] = {
    {TGF_PACKET_PART_L2_E,       &prvTgfPacketL2Part},  /* type, partPtr */
    {TGF_PACKET_PART_VLAN_TAG_E, &prvTgfPacketVlanTag0Part},
    {TGF_PACKET_PART_PAYLOAD_E,  &prvTgfPacketPayloadPart}
};

/* LENGTH of packet */
#define PRV_TGF_PACKET_LEN_CNS \
    TGF_L2_HEADER_SIZE_CNS + TGF_VLAN_TAG_SIZE_CNS + sizeof(prvTgfPayloadDataArr)

/* LENGTH of packet with CRC */
#define PRV_TGF_PACKET_CRC_LEN_CNS  PRV_TGF_PACKET_LEN_CNS + TGF_CRC_LEN_CNS

/* PACKET to send */
static TGF_PACKET_STC prvTgfPacketInfo = {
    PRV_TGF_PACKET_LEN_CNS,                                      /* totalLen */
    sizeof(prvTgfPacketPartArray) / sizeof(TGF_PACKET_PART_STC), /* numOfParts */
    prvTgfPacketPartArray                                        /* partsArray */
};

/*************************** Test packet Ipv4 *********************************/

/* L2 part of packet */
static TGF_PACKET_L2_STC prvTgfPacketIpv4L2Part = {
    {0x00, 0x00, 0x00, 0x00, 0x34, 0x02},               /* daMac */
    {0x00, 0x00, 0x00, 0x00, 0x00, 0x01}                /* saMac */
};

/* First VLAN_TAG part */
static TGF_PACKET_VLAN_TAG_STC prvTgfPacketIpv4VlanTag0Part = {
    TGF_ETHERTYPE_8100_VLAN_TAG_CNS,                    /* etherType */
    0, 0, PRV_TGF_VLANID_CNS                            /* pri, cfi, VlanId */
};

/* packet's ethertype */
static TGF_PACKET_ETHERTYPE_STC prvTgfPacketIpv4EtherTypePart = {TGF_ETHERTYPE_0800_IPV4_TAG_CNS};

/* packet's IPv4 */
static TGF_PACKET_IPV4_STC prvTgfPacketIpv4Part = {
    4,                  /* version */
    5,                  /* headerLen */
    0,                  /* typeOfService */
    0x2A,               /* totalLen */
    0,                  /* id */
    0,                  /* flags */
    0,                  /* offset */
    0x40,               /* timeToLive */
    0x04,               /* protocol */
    0x76CB,             /* csum */
    { 1,  1,  1,  1},   /* srcAddr */
    { 1,  1,  1,  3}    /* dstAddr */
};

/* DATA of packet */
static GT_U8 prvTgfPacketIpv4PayloadDataArr[] = {
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00
};

/* PAYLOAD part */
static TGF_PACKET_PAYLOAD_STC prvTgfPacketIpv4PayloadPart = {
    sizeof(prvTgfPacketIpv4PayloadDataArr),             /* dataLength */
    prvTgfPacketIpv4PayloadDataArr                      /* dataPtr */
};

/* PARTS of packet */
static TGF_PACKET_PART_STC prvTgfPacketIpv4PartArray[] = {
    {TGF_PACKET_PART_L2_E,        &prvTgfPacketIpv4L2Part},  /* type, partPtr */
    {TGF_PACKET_PART_VLAN_TAG_E,  &prvTgfPacketIpv4VlanTag0Part},
    {TGF_PACKET_PART_ETHERTYPE_E, &prvTgfPacketIpv4EtherTypePart},
    {TGF_PACKET_PART_IPV4_E,      &prvTgfPacketIpv4Part},
    {TGF_PACKET_PART_PAYLOAD_E,   &prvTgfPacketIpv4PayloadPart}
};

/* LENGTH of packet */
#define PRV_TGF_PACKET_IPV4_LEN_CNS \
    TGF_L2_HEADER_SIZE_CNS + TGF_VLAN_TAG_SIZE_CNS + TGF_ETHERTYPE_SIZE_CNS + \
    TGF_IPV4_HEADER_SIZE_CNS + sizeof(prvTgfPacketIpv4PayloadDataArr)

/* LENGTH of packet with CRC */
#define PRV_TGF_PACKET_IPV4_CRC_LEN_CNS  PRV_TGF_PACKET_IPV4_LEN_CNS + TGF_CRC_LEN_CNS

/* PACKET to send */
static TGF_PACKET_STC prvTgfPacketIpv4Info = {
    PRV_TGF_PACKET_IPV4_LEN_CNS,                                     /* totalLen */
    sizeof(prvTgfPacketIpv4PartArray) / sizeof(TGF_PACKET_PART_STC), /* numOfParts */
    prvTgfPacketIpv4PartArray                                        /* partsArray */
};

/******************************************************************************\
 *                            Private test functions                          *
\******************************************************************************/
/*******************************************************************************
* prvTgfNstPortisolationTestInit
*
* DESCRIPTION:
*       Set VLAN entry with recieved port isolation command.
*
* INPUTS:
*       vlanId           - vlanId to be configured
*       portIsolationCmd - port isolation command
*       portsMembers     - VLAN's ports Members
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
static GT_VOID prvTgfNstPortIsolationTestInit
(
    IN GT_U16                                  vlanId,
    IN PRV_TGF_BRG_VLAN_PORT_ISOLATION_CMD_ENT portIsolationCmd,
    CPSS_PORTS_BMP_STC                         portsMembers
)
{
    GT_STATUS                   rc = GT_OK;
    CPSS_PORTS_BMP_STC          portsTagging;
    PRV_TGF_BRG_VLAN_INFO_STC   vlanInfo;
    PRV_TGF_BRG_VLAN_PORTS_TAG_CMD_STC portsTaggingCmd;

    /* clear entry */
    cpssOsMemSet(&vlanInfo, 0, sizeof(vlanInfo));
    cpssOsMemSet(&portsTaggingCmd, 0, sizeof(PRV_TGF_BRG_VLAN_PORTS_TAG_CMD_STC));
    portsTagging.ports[0] = 0;
    portsTagging.ports[1] = 0;

    /* set vlan entry */
    vlanInfo.unkSrcAddrSecBreach  = GT_FALSE;
    vlanInfo.unregNonIpMcastCmd   = CPSS_PACKET_CMD_FORWARD_E;
    vlanInfo.unregIpv4McastCmd    = CPSS_PACKET_CMD_FORWARD_E;
    vlanInfo.unregIpv6McastCmd    = CPSS_PACKET_CMD_FORWARD_E;
    vlanInfo.unkUcastCmd          = CPSS_PACKET_CMD_FORWARD_E;
    vlanInfo.unregIpv4BcastCmd    = CPSS_PACKET_CMD_FORWARD_E;
    vlanInfo.unregNonIpv4BcastCmd = CPSS_PACKET_CMD_FORWARD_E;
    vlanInfo.ipv4IgmpToCpuEn      = GT_FALSE;
    vlanInfo.mirrToRxAnalyzerEn   = GT_FALSE;
    vlanInfo.ipv6IcmpToCpuEn      = GT_FALSE;
    vlanInfo.ipCtrlToCpuEn        = PRV_TGF_BRG_IP_CTRL_NONE_E;
    vlanInfo.ipv4IpmBrgMode       = CPSS_BRG_IPM_SGV_E;
    vlanInfo.ipv6IpmBrgMode       = CPSS_BRG_IPM_SGV_E;
    vlanInfo.ipv4IpmBrgEn         = GT_FALSE;
    vlanInfo.ipv6IpmBrgEn         = GT_FALSE;
    vlanInfo.ipv6SiteIdMode       = CPSS_IP_SITE_ID_INTERNAL_E;
    vlanInfo.ipv4UcastRouteEn     = GT_FALSE;
    vlanInfo.ipv4McastRouteEn     = GT_FALSE;
    vlanInfo.ipv6UcastRouteEn     = GT_FALSE;
    vlanInfo.ipv6McastRouteEn     = GT_FALSE;
    vlanInfo.stgId                = 0;
    vlanInfo.autoLearnDisable     = GT_TRUE;/* working in controlled learning */
    vlanInfo.naMsgToCpuEn         = GT_TRUE;/* working in controlled learning */
    vlanInfo.mruIdx               = 0;
    vlanInfo.bcastUdpTrapMirrEn   = GT_FALSE;
    vlanInfo.floodVidx            = 0xFFF;
    vlanInfo.floodVidxMode        = PRV_TGF_BRG_VLAN_FLOOD_VIDX_MODE_ALL_FLOODED_TRAFFIC_E;
    vlanInfo.portIsolationMode    = portIsolationCmd;
    vlanInfo.ucastLocalSwitchingEn = GT_FALSE;
    vlanInfo.mcastLocalSwitchingEn = GT_FALSE;

    /* set vlan entry */
    rc = prvTgfBrgVlanEntryWrite(prvTgfDevNum,
                                 vlanId,
                                 &portsMembers,
                                 &portsTagging,
                                 &vlanInfo,
                                 &portsTaggingCmd);
    UTF_VERIFY_EQUAL1_STRING_MAC(GT_OK, rc, "prvTgfBrgVlanEntryWrite: %d", vlanId);
}

/*******************************************************************************
* prvTgfNstPortIsolationTestPacketSend
*
* DESCRIPTION:
*       Function sends packet, performs trace and check expected results.
*
* INPUTS:
*       portNum       - port number to send traffic from
*       packetInfoPtr - PACKET to send
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
static GT_VOID prvTgfNstPortIsolationTestPacketSend
(
    IN GT_U8           portNum,
    IN TGF_PACKET_STC *packetInfoPtr
)
{
    GT_STATUS       rc           = GT_OK;

    /* use different 'etherType for vlan tagging' instead of 0x8100 */
    /* this will make our packet that has '0x8100' to be recognized as 'untagged'
      by the 'traffic generator' and also the CPSS will not override the ethertype
      of tag0 with the DSA tag */
    tgfTrafficGeneratorEtherTypeForVlanTagSet(TGF_ETHERTYPE_NON_VALID_TAG_CNS);

    /* setup Packet */
    rc = prvTgfSetTxSetupEth(prvTgfDevNum, packetInfoPtr, prvTgfBurstCount, 0, NULL);
    UTF_VERIFY_EQUAL4_STRING_MAC(GT_OK, rc, "ERROR of SetTxSetup: %d, %d",
                                 prvTgfDevNum, prvTgfBurstCount, 0, NULL);

    /* send Packet from port portNum */
    rc = prvTgfStartTransmitingEth(prvTgfDevNum, portNum);
    UTF_VERIFY_EQUAL2_STRING_MAC(GT_OK, rc, "ERROR of StartTransmitting: %d, %d",
                                 prvTgfDevNum, portNum);

    /* restore default ethertype */
    tgfTrafficGeneratorEtherTypeForVlanTagSet(TGF_ETHERTYPE_8100_VLAN_TAG_CNS);
}

/*******************************************************************************
* prvTgfNstPortIsolationTestTableReset
*
* DESCRIPTION:
*       Function clears FDB, VLAN tables and internal table of captured packets.
*
* INPUTS:
*       vlanId - vlanId to be cleared
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
static GT_VOID prvTgfNstPortIsolationTestTableReset
(
    IN GT_U16 vlanId
)
{
    GT_STATUS   rc = GT_OK;
    CPSS_INTERFACE_INFO_STC srcInterface = UTF_INTERFACE_INIT_VAL_CNS;
    GT_U32      portIter;
    PRV_TGF_IP_ROUTING_MODE_ENT routingMode;

    rc = tgfTrafficTableRxPcktTblClear();
    UTF_VERIFY_EQUAL0_STRING_MAC(GT_OK, rc, "tgfTrafficTableRxPcktTblClear");

    /* get routing mode */
    rc = prvTgfIpRoutingModeGet(&routingMode);
    UTF_VERIFY_EQUAL1_STRING_MAC(GT_OK, rc, "prvTgfIpRoutingModeGet: %d", prvTgfDevNum);

    if (routingMode == PRV_TGF_IP_ROUTING_MODE_IP_LTT_ENTRY_E)
    {
        /* disable Unicast IPv4 Routing on a Port */
        rc = prvTgfIpPortRoutingEnable(prvTgfPortsArray[PRV_TGF_SEND_PORT_IDX_ANOTHER_CNS], CPSS_IP_UNICAST_E,
                                       CPSS_IP_PROTOCOL_IPV4_E, GT_FALSE);
        UTF_VERIFY_EQUAL2_STRING_MAC(GT_OK, rc, "prvTgfIpPortRoutingEnable: %d %d",
                                     prvTgfDevNum, prvTgfPortsArray[PRV_TGF_SEND_PORT_IDX_ANOTHER_CNS]);
    }

    /* disable IPv4 Unicast Routing on Vlan */
    rc = prvTgfIpVlanRoutingEnable(PRV_TGF_VLANID_CNS, CPSS_IP_UNICAST_E, CPSS_IP_PROTOCOL_IPV4_E, GT_FALSE);
    UTF_VERIFY_EQUAL2_STRING_MAC(GT_OK, rc, "prvTgfIpVlanRoutingEnable: %d %d",
                                 prvTgfDevNum, prvTgfPortsArray[PRV_TGF_SEND_PORT_IDX_ANOTHER_CNS]);

    /* flush FDB include static entries */
    rc = prvTgfBrgFdbFlush(GT_TRUE);
    UTF_VERIFY_EQUAL2_STRING_MAC(GT_OK, rc, "prvTgfBrgFdbFlush: %d, %d", prvTgfDevNum, GT_TRUE);

    /* invalidate vlan entry (and reset vlan entry) */
    rc = prvTgfBrgDefVlanEntryInvalidate(vlanId);
    UTF_VERIFY_EQUAL2_STRING_MAC(GT_OK, rc, "prvTgfBrgDefVlanEntryInvalidate: %d, %d",
                                 prvTgfDevNum, vlanId);

    srcInterface.type = CPSS_INTERFACE_PORT_E;
    srcInterface.devPort.devNum = prvTgfDevNum;

    /* clear port isolation entries */

    for( portIter = 0 ; portIter < 4 ; portIter++ )
    {
        srcInterface.devPort.portNum = prvTgfPortsArray[portIter];

        rc = prvTgfNstPortIsolationTableEntrySet(srcInterface.devPort.devNum,
                                                 PRV_TGF_NST_PORT_ISOLATION_TRAFFIC_TYPE_L2_E,
                                                 srcInterface,
                                                 GT_FALSE,
                                                 &(PRV_CPSS_PP_MAC(prvTgfDevNum)->existingPorts));

        UTF_VERIFY_EQUAL3_STRING_MAC(GT_OK, rc, "prvTgfNstPortIsolationTableEntrySet: devNum = %d portNum = %d trafficType = %d",
                                 srcInterface.devPort.devNum, srcInterface.devPort.portNum, PRV_TGF_NST_PORT_ISOLATION_TRAFFIC_TYPE_L2_E);

        rc = prvTgfNstPortIsolationTableEntrySet(srcInterface.devPort.devNum,
                                                 PRV_TGF_NST_PORT_ISOLATION_TRAFFIC_TYPE_L3_E,
                                                 srcInterface,
                                                 GT_FALSE,
                                                 &(PRV_CPSS_PP_MAC(prvTgfDevNum)->existingPorts));

        UTF_VERIFY_EQUAL3_STRING_MAC(GT_OK, rc, "prvTgfNstPortIsolationTableEntrySet: devNum = %d portNum = %d trafficType = %d",
                                 srcInterface.devPort.devNum, srcInterface.devPort.portNum, PRV_TGF_NST_PORT_ISOLATION_TRAFFIC_TYPE_L3_E);
    }

    srcInterface.type    = CPSS_INTERFACE_TRUNK_E;
    srcInterface.trunkId = PRV_TGF_TRUNK_ID_CNS;
    CONVERT_TRUNK_ID_TEST_TO_CPSS_MAC(srcInterface.trunkId);

    rc = prvTgfNstPortIsolationTableEntrySet(prvTgfDevNum,
                                             portIsolationTraffic,
                                             srcInterface,
                                             GT_FALSE,
                                             &(PRV_CPSS_PP_MAC(prvTgfDevNum)->existingPorts));

    UTF_VERIFY_EQUAL2_STRING_MAC(GT_OK, rc, "prvTgfNstPortIsolationTableEntrySet: devNum = %d, trafficType = %d",
                                 prvTgfDevNum, portIsolationTraffic);
}

/*******************************************************************************
* prvTgfNstPortIsolationUcIpv4LttRoutingSet
*
* DESCRIPTION:
*       Set LTT Configuration
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
static GT_VOID prvTgfNstPortIsolationUcIpv4LttRoutingSet
(
    IN GT_U32        routerArpIndex,
    IN TGF_MAC_ADDR  routerArpMac,
    IN GT_U32        routeEntryBaseIndex,
    IN TGF_IPV4_ADDR dstIpv4Addr,
    IN GT_U8         nextHopPortNum
)
{
    GT_STATUS                     rc = GT_OK;
    PRV_TGF_BRG_MAC_ENTRY_STC     macEntry;
    GT_ETHERADDR                  arpMacAddr;
    PRV_TGF_IP_UC_ROUTE_ENTRY_STC routeEntriesArray[1];
    PRV_TGF_IP_UC_ROUTE_ENTRY_STC *regularEntryPtr;
    PRV_TGF_IP_TCAM_ROUTE_ENTRY_INFO_UNT nextHopInfo;
    GT_IPADDR                     ipAddr;

    PRV_UTF_LOG0_MAC("==== Setting LTT Configuration ====\n");

    /* -------------------------------------------------------------------------
     * 1. Set FDB Table
     */

    /* enable VLAN based MAC learning */
    rc = prvTgfBrgVlanLearningStateSet(PRV_TGF_VLANID_CNS, GT_TRUE);
    UTF_VERIFY_EQUAL1_STRING_MAC(GT_OK, rc, "prvTgfBrgVlanLearningStateSet: %d", prvTgfDevNum);

    /* create a macEntry with .daRoute = GT_TRUE */
    cpssOsMemSet(&macEntry, 0, sizeof(macEntry));
    cpssOsMemCpy(macEntry.key.key.macVlan.macAddr.arEther, prvTgfPacketIpv4L2Part.daMac, sizeof(TGF_MAC_ADDR));

    macEntry.key.entryType                  = PRV_TGF_FDB_ENTRY_TYPE_MAC_ADDR_E;
    macEntry.key.key.macVlan.vlanId         = PRV_TGF_VLANID_CNS;
    macEntry.dstInterface.type              = CPSS_INTERFACE_PORT_E;
    macEntry.dstInterface.devPort.devNum    = prvTgfDevNum;
    macEntry.dstInterface.devPort.portNum   = prvTgfPortsArray[PRV_TGF_SEND_PORT_IDX_CNS];
    macEntry.isStatic                       = GT_TRUE;
    macEntry.daCommand                      = PRV_TGF_PACKET_CMD_FORWARD_E;
    macEntry.saCommand                      = PRV_TGF_PACKET_CMD_FORWARD_E;
    macEntry.daRoute                        = GT_TRUE;
    macEntry.sourceId                       = 0;
    macEntry.userDefined                    = 0;
    macEntry.daQosIndex                     = 0;
    macEntry.saQosIndex                     = 0;
    macEntry.daSecurityLevel                = 0;
    macEntry.saSecurityLevel                = 0;
    macEntry.appSpecificCpuCode             = GT_FALSE;
    macEntry.mirrorToRxAnalyzerPortEn       = GT_FALSE;

    rc = prvTgfBrgFdbMacEntrySet(&macEntry);
    UTF_VERIFY_EQUAL1_STRING_MAC(GT_OK, rc, "prvTgfBrgFdbMacEntryWrite: %d", prvTgfDevNum);

    /* -------------------------------------------------------------------------
     * 2. Enable Routing
     */

    /* enable Unicast IPv4 Routing on a Port */
    rc = prvTgfIpPortRoutingEnable(prvTgfPortsArray[PRV_TGF_SEND_PORT_IDX_ANOTHER_CNS], CPSS_IP_UNICAST_E,
                                   CPSS_IP_PROTOCOL_IPV4_E, GT_TRUE);
    UTF_VERIFY_EQUAL2_STRING_MAC(GT_OK, rc, "prvTgfIpPortRoutingEnable: %d %d",
                                 prvTgfDevNum, prvTgfPortsArray[PRV_TGF_SEND_PORT_IDX_ANOTHER_CNS]);

    /* enable IPv4 Unicast Routing on Vlan */
    rc = prvTgfIpVlanRoutingEnable(PRV_TGF_VLANID_CNS, CPSS_IP_UNICAST_E, CPSS_IP_PROTOCOL_IPV4_E, GT_TRUE);
    UTF_VERIFY_EQUAL2_STRING_MAC(GT_OK, rc, "prvTgfIpVlanRoutingEnable: %d %d",
                                 prvTgfDevNum, prvTgfPortsArray[PRV_TGF_SEND_PORT_IDX_ANOTHER_CNS]);

    /* add device to LPM DB */
    rc = prvTgfIpLpmDBDevListAdd(prvTgfLpmDBId, &prvTgfDevNum, 1);
    UTF_VERIFY_EQUAL2_STRING_MAC(GT_OK, rc, "prvTgfIpLpmDBDevListAdd: %d %d",
                                 prvTgfLpmDBId, prvTgfDevNum);

    /* -------------------------------------------------------------------------
     * 3. Create the Route entry (Next hop) in Route table and Router ARP Table
     */

    /* write a ARP MAC address to the Router ARP Table */
    cpssOsMemCpy(arpMacAddr.arEther, routerArpMac, sizeof(TGF_MAC_ADDR));

    rc = prvTgfIpRouterArpAddrWrite(routerArpIndex, &arpMacAddr);
    UTF_VERIFY_EQUAL1_STRING_MAC(GT_OK, rc, "prvTgfIpRouterArpAddrWrite: %d", prvTgfDevNum);

    /* write a UC Route entry to the Route Table */
    cpssOsMemSet(routeEntriesArray, 0, sizeof(routeEntriesArray));

    regularEntryPtr = &routeEntriesArray[0];
    regularEntryPtr->cmd                        = CPSS_PACKET_CMD_ROUTE_E;
    regularEntryPtr->cpuCodeIndex               = 0;
    regularEntryPtr->appSpecificCpuCodeEnable   = GT_FALSE;
    regularEntryPtr->unicastPacketSipFilterEnable = GT_FALSE;
    regularEntryPtr->ttlHopLimitDecEnable       = GT_FALSE;
    regularEntryPtr->ttlHopLimDecOptionsExtChkByPass = GT_FALSE;
    regularEntryPtr->ingressMirror              = GT_FALSE;
    regularEntryPtr->qosProfileMarkingEnable    = GT_FALSE;
    regularEntryPtr->qosProfileIndex            = 0;
    regularEntryPtr->qosPrecedence              = CPSS_PACKET_ATTRIBUTE_ASSIGN_PRECEDENCE_SOFT_E;
    regularEntryPtr->modifyUp                   = CPSS_PACKET_ATTRIBUTE_MODIFY_KEEP_PREVIOUS_E;
    regularEntryPtr->modifyDscp                 = CPSS_PACKET_ATTRIBUTE_MODIFY_KEEP_PREVIOUS_E;
    regularEntryPtr->countSet                   = CPSS_IP_CNT_SET0_E;
    regularEntryPtr->trapMirrorArpBcEnable      = GT_FALSE;
    regularEntryPtr->sipAccessLevel             = 0;
    regularEntryPtr->dipAccessLevel             = 0;
    regularEntryPtr->ICMPRedirectEnable         = GT_FALSE;
    regularEntryPtr->scopeCheckingEnable        = GT_FALSE;
    regularEntryPtr->siteId                     = CPSS_IP_SITE_ID_INTERNAL_E;
    regularEntryPtr->mtuProfileIndex            = 0;
    regularEntryPtr->isTunnelStart              = GT_FALSE;
    regularEntryPtr->nextHopVlanId              = PRV_TGF_NEXTHOPE_VLANID_CNS;
    regularEntryPtr->nextHopInterface.type      = CPSS_INTERFACE_PORT_E;
    regularEntryPtr->nextHopInterface.devPort.devNum = prvTgfDevNum;
    regularEntryPtr->nextHopInterface.devPort.portNum = nextHopPortNum;
    regularEntryPtr->nextHopARPPointer          = routerArpIndex;
    regularEntryPtr->nextHopTunnelPointer       = 0;

    rc = prvTgfIpUcRouteEntriesWrite(routeEntryBaseIndex, routeEntriesArray, 1);
    UTF_VERIFY_EQUAL1_STRING_MAC(GT_OK, rc, "prvTgfIpUcRouteEntriesWrite: %d", prvTgfDevNum);

    /* -------------------------------------------------------------------------
     * 4. Create a new Ipv4 prefix in a Virtual Router for the specified LPM DB
     */

    /* fill a nexthope info for the prefix */
    cpssOsMemSet(&nextHopInfo, 0, sizeof(nextHopInfo));
    nextHopInfo.ipLttEntry.routeEntryBaseIndex = routeEntryBaseIndex;

    /* fill a destination IP address for the prefix */
    cpssOsMemCpy(ipAddr.arIP, dstIpv4Addr, sizeof(ipAddr.arIP));

    /* call CPSS function */
    rc = prvTgfIpLpmIpv4UcPrefixAdd(prvTgfLpmDBId, 0, ipAddr, 32, &nextHopInfo, GT_FALSE);
    UTF_VERIFY_EQUAL1_STRING_MAC(GT_OK, rc, "prvTgfIpLpmIpv4UcPrefixAdd: %d", prvTgfDevNum);
}

/*******************************************************************************
* prvTgfNstPortIsolationUcIpv4PbrRoutingSet
*
* DESCRIPTION:
*       Set PBR Configuration
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
static GT_VOID prvTgfNstPortIsolationUcIpv4PbrRoutingSet
(
    IN GT_U32        routerArpIndex,
    IN TGF_MAC_ADDR  routerArpMac,
    IN TGF_IPV4_ADDR dstIpv4Addr,
    IN GT_U8         nextHopPortNum
)
{
    GT_STATUS                               rc = GT_OK;
    PRV_TGF_BRG_MAC_ENTRY_STC               macEntry;
    GT_ETHERADDR                            arpMacAddr;
    PRV_TGF_IP_TCAM_ROUTE_ENTRY_INFO_UNT    nextHopInfo;
    GT_IPADDR                               ipAddr;

    PRV_UTF_LOG0_MAC("==== Setting PBR Configuration ====\n");

    /* -------------------------------------------------------------------------
     * 1. Set FDB Table
     */

    /* enable VLAN based MAC learning */
    rc = prvTgfBrgVlanLearningStateSet(PRV_TGF_VLANID_CNS, GT_TRUE);
    UTF_VERIFY_EQUAL1_STRING_MAC(GT_OK, rc, "prvTgfBrgVlanLearningStateSet: %d", prvTgfDevNum);

    /* create a macEntry with .daRoute = GT_TRUE */
    cpssOsMemSet(&macEntry, 0, sizeof(macEntry));
    cpssOsMemCpy(macEntry.key.key.macVlan.macAddr.arEther, prvTgfPacketIpv4L2Part.daMac, sizeof(TGF_MAC_ADDR));

    macEntry.key.entryType                  = PRV_TGF_FDB_ENTRY_TYPE_MAC_ADDR_E;
    macEntry.key.key.macVlan.vlanId         = PRV_TGF_VLANID_CNS;
    macEntry.dstInterface.type              = CPSS_INTERFACE_PORT_E;
    macEntry.dstInterface.devPort.devNum    = prvTgfDevNum;
    macEntry.dstInterface.devPort.portNum   = prvTgfPortsArray[PRV_TGF_SEND_PORT_IDX_CNS];
    macEntry.isStatic                       = GT_TRUE;
    macEntry.daCommand                      = PRV_TGF_PACKET_CMD_FORWARD_E;
    macEntry.saCommand                      = PRV_TGF_PACKET_CMD_FORWARD_E;
    macEntry.daRoute                        = GT_TRUE;
    macEntry.sourceId                       = 0;
    macEntry.userDefined                    = 0;
    macEntry.daQosIndex                     = 0;
    macEntry.saQosIndex                     = 0;
    macEntry.daSecurityLevel                = 0;
    macEntry.saSecurityLevel                = 0;
    macEntry.appSpecificCpuCode             = GT_FALSE;
    macEntry.mirrorToRxAnalyzerPortEn       = GT_FALSE;

    rc = prvTgfBrgFdbMacEntrySet(&macEntry);
    UTF_VERIFY_EQUAL1_STRING_MAC(GT_OK, rc, "prvTgfBrgFdbMacEntryWrite: %d", prvTgfDevNum);

    /* -------------------------------------------------------------------------
     * 2. PCL Config
     */

    /* init PCL Engine for send port */
    rc = prvTgfPclDefPortInitExt1(
        prvTgfPortsArray[PRV_TGF_SEND_PORT_IDX_ANOTHER_CNS],
        CPSS_PCL_DIRECTION_INGRESS_E,
        CPSS_PCL_LOOKUP_1_E,/* PBR must be in lookup 1 ! */
        PRV_TGF_PCL_PBR_ID_FOR_VIRTUAL_ROUTER_CNS, /* pclId */
        PRV_TGF_PCL_RULE_FORMAT_INGRESS_STD_NOT_IP_E /*nonIpKey*/,
        PRV_TGF_PCL_RULE_FORMAT_INGRESS_STD_IPV4_L4_E /*ipv4Key*/,
        PRV_TGF_PCL_RULE_FORMAT_INGRESS_STD_IPV6_DIP_E /*ipv6Key*/);
    UTF_VERIFY_EQUAL1_STRING_MAC(
        GT_OK, rc, "prvTgfPclDefPortInitExt1: %d", prvTgfDevNum);

    /* -------------------------------------------------------------------------
     * 3. IP Config
     */

    /* enable IPv4 Unicast Routing on Vlan */
    rc = prvTgfIpVlanRoutingEnable(PRV_TGF_VLANID_CNS, CPSS_IP_UNICAST_E, CPSS_IP_PROTOCOL_IPV4_E, GT_TRUE);
    UTF_VERIFY_EQUAL2_STRING_MAC(GT_OK, rc, "prvTgfIpVlanRoutingEnable: %d %d",
                                 prvTgfDevNum, prvTgfPortsArray[PRV_TGF_SEND_PORT_IDX_CNS]);

    /* write ARP MAC address to the Router ARP Table */
    cpssOsMemCpy(arpMacAddr.arEther, routerArpMac, sizeof(TGF_MAC_ADDR));

    rc = prvTgfIpRouterArpAddrWrite(routerArpIndex, &arpMacAddr);
    UTF_VERIFY_EQUAL1_STRING_MAC(GT_OK, rc, "prvTgfIpRouterArpAddrWrite: %d", prvTgfDevNum);

    /* -------------------------------------------------------------------------
     * 4. Create a new Ipv4 prefix in a Virtual Router for the specified LPM DB
     */

    /* fill a nexthope info for the prefix */
    cpssOsMemSet(&nextHopInfo, 0, sizeof(nextHopInfo));

    nextHopInfo.pclIpUcAction.pktCmd                        = CPSS_PACKET_CMD_FORWARD_E;
    nextHopInfo.pclIpUcAction.mirror.cpuCode                = CPSS_NET_CONTROL_E;
    nextHopInfo.pclIpUcAction.matchCounter.enableMatchCount = GT_TRUE;
    nextHopInfo.pclIpUcAction.vlan.vlanId                   = PRV_TGF_NEXTHOPE_VLANID_CNS;
    nextHopInfo.pclIpUcAction.ipUcRoute.doIpUcRoute         = GT_TRUE;
    nextHopInfo.pclIpUcAction.ipUcRoute.arpDaIndex          = routerArpIndex;

    nextHopInfo.pclIpUcAction.redirect.redirectCmd          = PRV_TGF_PCL_ACTION_REDIRECT_CMD_OUT_IF_E;
    nextHopInfo.pclIpUcAction.redirect.data.outIf.outInterface.type            = CPSS_INTERFACE_PORT_E;
    nextHopInfo.pclIpUcAction.redirect.data.outIf.outInterface.devPort.devNum  = prvTgfDevNum;
    nextHopInfo.pclIpUcAction.redirect.data.outIf.outInterface.devPort.portNum = nextHopPortNum;

    /* fill a destination IP address for the prefix */
    cpssOsMemCpy(ipAddr.arIP, dstIpv4Addr, sizeof(ipAddr.arIP));

    /* call CPSS function */
    rc = prvTgfIpLpmIpv4UcPrefixAdd(prvTgfLpmDBId, 0, ipAddr, 32, &nextHopInfo, GT_TRUE);
    UTF_VERIFY_EQUAL1_STRING_MAC(GT_OK, rc, "prvTgfIpLpmIpv4UcPrefixAdd: %d", prvTgfDevNum);
}

/******************************************************************************\
 *                            Test implementation                             *
\******************************************************************************/

/*******************************************************************************
* prvTgfNstL2PortIsolationConfigurationSet
*
* DESCRIPTION:
*       Set test configuration:
*           Configure portIsolationMode on VLAN entry (VID = 5).
*           Enable Port Isolation feature.
*           Set port isolation table entry for L2 with:
*               devNum        = 0
*               trafficType   = CPSS_DXCH_NST_PORT_ISOLATION_TRAFFIC_TYPE_L2_E
*               srcInterface  = devNum = 0, portNum = 8
*               cpuPortMember = GT_FALSE
*               localPortsMembersPtr = 0x40001
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
GT_VOID prvTgfNstL2PortIsolationConfigurationSet
(
    GT_VOID
)
{
    CPSS_INTERFACE_INFO_STC srcInterface = UTF_INTERFACE_INIT_VAL_CNS;
    GT_STATUS               rc = GT_OK;
    GT_U32                  portIter = 0;

    srcInterface.type = CPSS_INTERFACE_PORT_E;
    srcInterface.devPort.devNum = prvTgfDevNum;
    srcInterface.devPort.portNum = prvTgfPortsArray[PRV_TGF_SEND_PORT_IDX_CNS];

    portIsolationCmd = PRV_TGF_BRG_VLAN_PORT_ISOLATION_L2_CMD_E;

    /* clear entry */
    cpssOsMemSet(&localPortsVlanMembers, 0, sizeof(localPortsVlanMembers));

    /* ports 0, 8, 18, 23 are VLAN Members */
    for (portIter = 0; portIter < prvTgfPortsNum; portIter++)
    {
        localPortsVlanMembers.ports[prvTgfPortsArray[portIter] >> 5] |= 1 << (prvTgfPortsArray[portIter] & 0x1f);
    }

    /* set VLAN entry */
    prvTgfNstPortIsolationTestInit(PRV_TGF_VLANID_CNS, portIsolationCmd, localPortsVlanMembers);

    /* enable Port Isolation Feature */
    rc = prvTgfNstPortIsolationEnableSet(srcInterface.devPort.devNum, GT_TRUE);
    UTF_VERIFY_EQUAL2_STRING_MAC(GT_OK, rc, "prvTgfNstPortIsolationEnableSet: %d %d", srcInterface.devPort.devNum, GT_TRUE);

    /* set L2 port isolation table entry */
    portIsolationTraffic = PRV_TGF_NST_PORT_ISOLATION_TRAFFIC_TYPE_L2_E;

    /* clear entry */
    cpssOsMemSet(&localPortsMembers, 0, sizeof(localPortsMembers));

    /* ports 0, 18 are PortMembers */
    localPortsMembers.ports[prvTgfPortsArray[0] >> 5] |= 1 << (prvTgfPortsArray[0] & 0x1f);
    localPortsMembers.ports[prvTgfPortsArray[2] >> 5] |= 1 << (prvTgfPortsArray[2] & 0x1f);

    rc = prvTgfNstPortIsolationTableEntrySet(srcInterface.devPort.devNum,
                                             portIsolationTraffic,
                                             srcInterface,
                                             GT_FALSE,
                                             &localPortsMembers);
    UTF_VERIFY_EQUAL3_STRING_MAC(GT_OK, rc, "prvTgfNstPortIsolationTableEntrySet: devNum = %d portNum = %d trafficType = %d",
                                 srcInterface.devPort.devNum, srcInterface.devPort.devNum, portIsolationTraffic);
}

/*******************************************************************************
* prvTgfNstL2PortIsolationTrafficGenerate
*
* DESCRIPTION:
*       Generate traffic:
*           Send to device's port 8 packet:
*               macDa = FF:FF:FF:FF:FF:FF,
*               macSa = 00:00:00:00:00:05,
*           Success Criteria:
*               Packet is captured on ports 0 and 18
*       Additional Configuration:
*           Disable port isolation feature in VLAN entry.
*       Generate Traffic:
*           Send to device's port 8 packet:
*               macDa = FF:FF:FF:FF:FF:FF,
*               macSa = 00:00:00:00:00:05,
*           Success Criteria:
*               Packet is captured on ports: 0,18,23
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
GT_VOID prvTgfNstL2PortIsolationTrafficGenerate
(
    GT_VOID
)
{
    GT_STATUS    rc = GT_OK;
    GT_U32       portIter = 0;
    GT_U32       expectedGoodOctetsValue;
    GT_U32       expectedGoodPctsValue;
    CPSS_PORT_MAC_COUNTER_SET_STC portCntrs;

    /* reset counters */
    for (portIter = 0; portIter < prvTgfPortsNum; portIter++)
    {
        rc = prvTgfResetCountersEth(prvTgfDevNum, prvTgfPortsArray[portIter]);
        UTF_VERIFY_EQUAL2_STRING_MAC(GT_OK, rc, "prvTgfResetCountersEth: %d, %d",
                                     prvTgfDevNum, prvTgfPortsArray[portIter]);
    }

    /* send packet */
    prvTgfNstPortIsolationTestPacketSend(prvTgfPortsArray[PRV_TGF_SEND_PORT_IDX_CNS], &prvTgfPacketInfo);

    /* check counters */
    for (portIter = 0; portIter < prvTgfPortsNum; portIter++)
    {
        /* read counters */
        rc = prvTgfReadPortCountersEth(prvTgfDevNum, prvTgfPortsArray[portIter], GT_TRUE, &portCntrs);
        UTF_VERIFY_EQUAL2_STRING_MAC(GT_OK, rc, "prvTgfReadPortCountersEth: %d, %d",
                                     prvTgfDevNum, prvTgfPortsArray[portIter]);

        if (PRV_TGF_SEND_PORT_IDX_CNS == portIter)
        {
            /* check Rx counters */
            UTF_VERIFY_EQUAL0_STRING_MAC((PRV_TGF_PACKET_LEN_CNS + TGF_CRC_LEN_CNS) * prvTgfBurstCount, portCntrs.goodOctetsRcv.l[0],
                                         "get another goodOctetsRcv counter than expected");
            UTF_VERIFY_EQUAL0_STRING_MAC(prvTgfBurstCount, portCntrs.goodPktsRcv.l[0],
                                         "get another goodPktsRcv counter than expected");

            continue;
        }

        if (((localPortsMembers.ports[prvTgfPortsArray[portIter] >> 5] >> (prvTgfPortsArray[portIter] & 0x1f)) & 1 ) != 0)
        {
            expectedGoodOctetsValue = (PRV_TGF_PACKET_LEN_CNS + TGF_CRC_LEN_CNS) * prvTgfBurstCount;
            expectedGoodPctsValue = prvTgfBurstCount;
        }
        else
        {
            expectedGoodOctetsValue = 0;
            expectedGoodPctsValue = 0;
        }

        /* check Tx counters */
        UTF_VERIFY_EQUAL0_STRING_MAC(expectedGoodOctetsValue, portCntrs.goodOctetsSent.l[0],
                                     "get another goodOctetsSent counter than expected");
        UTF_VERIFY_EQUAL0_STRING_MAC(expectedGoodPctsValue, portCntrs.goodPktsSent.l[0],
                                     "get another goodPktsSent counter than expected");
    }

    /* reset counters */
    for (portIter = 0; portIter < prvTgfPortsNum; portIter++)
    {
        rc = prvTgfResetCountersEth(prvTgfDevNum, prvTgfPortsArray[portIter]);
        UTF_VERIFY_EQUAL2_STRING_MAC(GT_OK, rc, "prvTgfResetCountersEth: %d, %d",
                                     prvTgfDevNum, prvTgfPortsArray[portIter]);
    }

    /* disable port isolation feature per VLAN */
    prvTgfNstPortIsolationTestInit(PRV_TGF_VLANID_CNS,
             PRV_TGF_BRG_VLAN_PORT_ISOLATION_DISABLE_CMD_E, localPortsVlanMembers);

    /* send packet */
    prvTgfNstPortIsolationTestPacketSend(prvTgfPortsArray[PRV_TGF_SEND_PORT_IDX_CNS], &prvTgfPacketInfo);

    /* check counters */
    for (portIter = 0; portIter < prvTgfPortsNum; portIter++)
    {
        /* read counters */
        rc = prvTgfReadPortCountersEth(prvTgfDevNum, prvTgfPortsArray[portIter], GT_TRUE, &portCntrs);
        UTF_VERIFY_EQUAL2_STRING_MAC(GT_OK, rc, "prvTgfReadPortCountersEth: %d, %d",
                                     prvTgfDevNum, prvTgfPortsArray[portIter]);

        if (PRV_TGF_SEND_PORT_IDX_CNS == portIter)
        {
            /* check Rx counters */
            UTF_VERIFY_EQUAL0_STRING_MAC((PRV_TGF_PACKET_LEN_CNS + TGF_CRC_LEN_CNS) * prvTgfBurstCount, portCntrs.goodOctetsRcv.l[0],
                                         "get another goodOctetsRcv counter than expected");
            UTF_VERIFY_EQUAL0_STRING_MAC(prvTgfBurstCount, portCntrs.goodPktsRcv.l[0],
                                         "get another goodPktsRcv counter than expected");

            continue;
        }

        /* check Tx counters */
        UTF_VERIFY_EQUAL0_STRING_MAC((PRV_TGF_PACKET_LEN_CNS + TGF_CRC_LEN_CNS) * prvTgfBurstCount, portCntrs.goodOctetsSent.l[0],
                                     "get another goodOctetsSent counter than expected");
        UTF_VERIFY_EQUAL0_STRING_MAC(prvTgfBurstCount, portCntrs.goodPktsSent.l[0],
                                     "get another goodPktsSent counter than expected");
    }

    /* reset counters */
    for (portIter = 0; portIter < prvTgfPortsNum; portIter++)
    {
        rc = prvTgfResetCountersEth(prvTgfDevNum, prvTgfPortsArray[portIter]);
        UTF_VERIFY_EQUAL2_STRING_MAC(GT_OK, rc, "prvTgfResetCountersEth: %d, %d",
                                     prvTgfDevNum, prvTgfPortsArray[portIter]);
    }
}

/*******************************************************************************
* prvTgfNstL2PortIsolationConfigurationRestore
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
GT_VOID prvTgfNstL2PortIsolationConfigurationRestore
(
    GT_VOID
)
{
    GT_STATUS rc;

    prvTgfNstPortIsolationTestTableReset(PRV_TGF_VLANID_CNS);

    /* disable Port Isolation Feature */
    rc = prvTgfNstPortIsolationEnableSet(prvTgfDevNum, GT_FALSE);
    UTF_VERIFY_EQUAL2_STRING_MAC(GT_OK, rc, "prvTgfNstPortIsolationEnableSet: %d %d", prvTgfDevNum, GT_FALSE);
}

/*******************************************************************************
* prvTgfNstL3PortIsolationConfigurationSet
*
* DESCRIPTION:
*       Set test configuration:
*           Configure portIsolationMode on VLAN entry (VID = 5).
*           Enable Port Isolation feature.
*           Set port isolation table entry for L3 with:
*               devNum        = 0
*               trafficType   = CPSS_DXCH_NST_PORT_ISOLATION_TRAFFIC_TYPE_L3_E
*               srcInterface  = devNum = 0, portNum = 0
*               cpuPortMember = GT_FALSE
*               localPortsMembersPtr = 0x40000
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
GT_VOID prvTgfNstL3PortIsolationConfigurationSet
(
    GT_VOID
)
{
    CPSS_INTERFACE_INFO_STC srcInterface = UTF_INTERFACE_INIT_VAL_CNS;
    GT_STATUS               rc = GT_OK;
    GT_U32                  entryIndex;
    PRV_TGF_IP_ROUTING_MODE_ENT routingMode;

    srcInterface.type = CPSS_INTERFACE_PORT_E;
    srcInterface.devPort.devNum = prvTgfDevNum;
    srcInterface.devPort.portNum = prvTgfPortsArray[PRV_TGF_SEND_PORT_IDX_ANOTHER_CNS];

    portIsolationCmd = PRV_TGF_BRG_VLAN_PORT_ISOLATION_L3_CMD_E;

    /* clear entry */
    cpssOsMemSet(&localPortsVlanMembers, 0, sizeof(localPortsVlanMembers));

    /* ports 18, 23 are VLAN_2 Members */
    localPortsVlanMembers.ports[prvTgfPortsArray[2] >> 5] |= 1 << (prvTgfPortsArray[2] & 0x1f);
    localPortsVlanMembers.ports[prvTgfPortsArray[3] >> 5] |= 1 << (prvTgfPortsArray[3] & 0x1f);

    /* set VLAN_2 entry */
    prvTgfNstPortIsolationTestInit(PRV_TGF_NEXTHOPE_VLANID_CNS, portIsolationCmd, localPortsVlanMembers);

    /* clear entry */
    cpssOsMemSet(&localPortsVlanMembers, 0, sizeof(localPortsVlanMembers));

    /* ports 0, 8 are VLAN_1 Members */
    localPortsVlanMembers.ports[prvTgfPortsArray[0] >> 5] |= 1 << (prvTgfPortsArray[0] & 0x1f);
    localPortsVlanMembers.ports[prvTgfPortsArray[1] >> 5] |= 1 << (prvTgfPortsArray[1] & 0x1f);

    /* set VLAN_1 entry */
    prvTgfNstPortIsolationTestInit(PRV_TGF_VLANID_CNS, portIsolationCmd, localPortsVlanMembers);

    /* enable Port Isolation Feature */
    rc = prvTgfNstPortIsolationEnableSet(srcInterface.devPort.devNum, GT_TRUE);
    UTF_VERIFY_EQUAL2_STRING_MAC(GT_OK, rc, "prvTgfNstPortIsolationEnableSet: %d %d", srcInterface.devPort.devNum, GT_TRUE);

    /* set L3 port isolation table entry */
    portIsolationTraffic = PRV_TGF_NST_PORT_ISOLATION_TRAFFIC_TYPE_L3_E;

    /* clear entry */
    cpssOsMemSet(&localPortsMembers, 0, sizeof(localPortsMembers));

    /* port 18 is PortMembers */
    localPortsMembers.ports[prvTgfPortsArray[2] >> 5] |= 1 << (prvTgfPortsArray[2] & 0x1f);

    rc = prvTgfNstPortIsolationTableEntrySet(srcInterface.devPort.devNum,
                                             portIsolationTraffic,
                                             srcInterface,
                                             GT_FALSE,
                                             &localPortsMembers);
    UTF_VERIFY_EQUAL3_STRING_MAC(GT_OK, rc, "prvTgfNstPortIsolationTableEntrySet: devNum = %d portNum = %d trafficType = %d",
                                 srcInterface.devPort.devNum, srcInterface.devPort.portNum, portIsolationTraffic);

    /* get routing mode */
    rc = prvTgfIpRoutingModeGet(&routingMode);
    UTF_VERIFY_EQUAL1_STRING_MAC(GT_OK, rc, "prvTgfIpRoutingModeGet: %d", prvTgfDevNum);

    /* set Ipv4 Routing */
    for (entryIndex = 0; entryIndex < PRV_TGF_SEND_COUNT_CNS; entryIndex++)
        switch(routingMode)
        {
            case PRV_TGF_IP_ROUTING_MODE_PCL_ACTION_E:

                prvTgfNstPortIsolationUcIpv4PbrRoutingSet(
                        prvTgfRouterArpIndexArray[entryIndex],
                        prvTgfArpMacArray[entryIndex],
                        prvTgfIpv4AddrArray[entryIndex],
                        prvTgfPortsArray[prvTgfNextHopePortIdxArray[entryIndex]]);
                break;

            case PRV_TGF_IP_ROUTING_MODE_IP_LTT_ENTRY_E:

                prvTgfNstPortIsolationUcIpv4LttRoutingSet(
                        prvTgfRouterArpIndexArray[entryIndex],
                        prvTgfArpMacArray[entryIndex],
                        prvTgfRouteEntryBaseIndexArray[entryIndex],
                        prvTgfIpv4AddrArray[entryIndex],
                        prvTgfPortsArray[prvTgfNextHopePortIdxArray[entryIndex]]);
                break;

            case PRV_TGF_IP_ROUTING_MODE_UNDEFINED_E:

                break;
        }
}

/*******************************************************************************
* prvTgfNstL3PortIsolationTrafficGenerate
*
* DESCRIPTION:
*       Generate traffic:
*           Send to device's port 0 two IP packets:
*               0000: 00 00 00 00 34 02 00 00 00 00 00 01 81 00 00 05
*               0010: 08 00 45 00 00 2A 00 00 00 00 40 04 76 CB 01 01
*               0020: 01 01 01 01 01 03 00 00 00 00 00 00 00 00 00 00
*               0030: 00 00 00 00 00 00 00 00 00 00 00 00
*
*               0000: 00 00 00 00 34 02 00 00 00 00 00 01 81 00 00 05
*               0010: 08 00 45 00 00 2A 00 00 00 00 40 04 72 C7 02 02
*               0020: 02 02 01 02 03 04 00 00 00 00 00 00 00 00 00 00
*               0030: 00 00 00 00 00 00 00 00 00 00 00 00
*
*           Success Criteria:
*               One packet is captured on port 18
*
*           Additional Configuration:
*               Disable port isolation feature in VLAN entry.
*
*           Send to device's port 0 two IP packets:
*               0000: 00 00 00 00 34 02 00 00 00 00 00 01 81 00 00 05
*               0010: 08 00 45 00 00 2A 00 00 00 00 40 04 76 CB 01 01
*               0020: 01 01 01 01 01 03 00 00 00 00 00 00 00 00 00 00
*               0030: 00 00 00 00 00 00 00 00 00 00 00 00
*
*               0000: 00 00 00 00 34 02 00 00 00 00 00 01 81 00 00 05
*               0010: 08 00 45 00 00 2A 00 00 00 00 40 04 72 C7 02 02
*               0020: 02 02 01 02 03 04 00 00 00 00 00 00 00 00 00 00
*               0030: 00 00 00 00 00 00 00 00 00 00 00 00
*
*           Success Criteria:
*               Packet is captured on ports: 18,23
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
GT_VOID prvTgfNstL3PortIsolationTrafficGenerate
(
    GT_VOID
)
{
    GT_STATUS    rc = GT_OK;
    GT_U32       portIter = 0;
    GT_U32       sendIter = 0;
    GT_U32       expectedGoodOctetsValue;
    GT_U32       expectedGoodPctsValue;
    CPSS_PORT_MAC_COUNTER_SET_STC portCntrs;

    /* send two packets */
    for (sendIter = 0; sendIter < PRV_TGF_SEND_COUNT_CNS; sendIter++)
    {
        /* reset counters */
        for (portIter = 0; portIter < prvTgfPortsNum; portIter++)
        {
            rc = prvTgfResetCountersEth(prvTgfDevNum, prvTgfPortsArray[portIter]);
            UTF_VERIFY_EQUAL2_STRING_MAC(GT_OK, rc, "prvTgfResetCountersEth: %d, %d",
                                         prvTgfDevNum, prvTgfPortsArray[portIter]);
        }

        /* setting IP addresses and checksum for packet 1 */
        prvTgfPacketIpv4Part.csum = 0x76CB;
        prvTgfPacketIpv4Part.srcAddr[0] = 1;
        prvTgfPacketIpv4Part.srcAddr[1] = 1;
        prvTgfPacketIpv4Part.srcAddr[2] = 1;
        prvTgfPacketIpv4Part.srcAddr[3] = 1;
        cpssOsMemCpy(prvTgfPacketIpv4Part.dstAddr, prvTgfIpv4AddrArray[0], sizeof(prvTgfPacketIpv4Part.dstAddr));

        /* send packet 1 */
        prvTgfNstPortIsolationTestPacketSend(prvTgfPortsArray[PRV_TGF_SEND_PORT_IDX_ANOTHER_CNS], &prvTgfPacketIpv4Info);

        /* setting IP addresses and checksum for packet 2 */
        prvTgfPacketIpv4Part.csum = 0x72C7;
        prvTgfPacketIpv4Part.srcAddr[0] = 2;
        prvTgfPacketIpv4Part.srcAddr[1] = 2;
        prvTgfPacketIpv4Part.srcAddr[2] = 2;
        prvTgfPacketIpv4Part.srcAddr[3] = 2;
        cpssOsMemCpy(prvTgfPacketIpv4Part.dstAddr, prvTgfIpv4AddrArray[1], sizeof(prvTgfPacketIpv4Part.dstAddr));

        /* send packet 2 */
        prvTgfNstPortIsolationTestPacketSend(prvTgfPortsArray[PRV_TGF_SEND_PORT_IDX_ANOTHER_CNS], &prvTgfPacketIpv4Info);

        /* check counters */
        for (portIter = 0; portIter < prvTgfPortsNum; portIter++)
        {
            /* read counters */
            rc = prvTgfReadPortCountersEth(prvTgfDevNum, prvTgfPortsArray[portIter], GT_TRUE, &portCntrs);
            UTF_VERIFY_EQUAL2_STRING_MAC(GT_OK, rc, "prvTgfReadPortCountersEth: %d, %d",
                                         prvTgfDevNum, prvTgfPortsArray[portIter]);

            if (PRV_TGF_SEND_PORT_IDX_ANOTHER_CNS == portIter)
            {
                /* check Rx counters */
                UTF_VERIFY_EQUAL0_STRING_MAC((PRV_TGF_PACKET_LEN_CNS + TGF_CRC_LEN_CNS) * prvTgfBurstCount * 2, portCntrs.goodOctetsRcv.l[0],
                                             "get another goodOctetsRcv counter than expected");
                UTF_VERIFY_EQUAL0_STRING_MAC(prvTgfBurstCount * 2, portCntrs.goodPktsRcv.l[0],
                                             "get another goodPktsRcv counter than expected");

                continue;
            }

            if (sendIter == 0)
            {
                if (prvTgfNextHopePortIdxArray[0] == portIter)
                {
                    expectedGoodOctetsValue = (PRV_TGF_PACKET_LEN_CNS + TGF_CRC_LEN_CNS) * prvTgfBurstCount;
                    expectedGoodPctsValue = prvTgfBurstCount;
                }
                else
                {
                    expectedGoodOctetsValue = 0;
                    expectedGoodPctsValue = 0;
                }
            }
            else /* sendIter == 1 */
            {
                if (prvTgfNextHopePortIdxArray[0] == portIter ||
                    prvTgfNextHopePortIdxArray[1] == portIter)
                {
                    expectedGoodOctetsValue = (PRV_TGF_PACKET_LEN_CNS + TGF_CRC_LEN_CNS) * prvTgfBurstCount;
                    expectedGoodPctsValue = prvTgfBurstCount;
                }
                else
                {
                    expectedGoodOctetsValue = 0;
                    expectedGoodPctsValue = 0;
                }
            }

            /* check Tx counters */
            UTF_VERIFY_EQUAL0_STRING_MAC(expectedGoodOctetsValue, portCntrs.goodOctetsSent.l[0],
                                         "get another goodOctetsSent counter than expected");
            UTF_VERIFY_EQUAL0_STRING_MAC(expectedGoodPctsValue, portCntrs.goodPktsSent.l[0],
                                         "get another goodPktsSent counter than expected");
        }

        /* disable port isolation feature per Next HOP VLAN */

        /* clear entry */
        cpssOsMemSet(&localPortsVlanMembers, 0, sizeof(localPortsVlanMembers));

        /* ports 18, 23 are VLAN_2 Members */
        localPortsVlanMembers.ports[prvTgfPortsArray[2] >> 5] |= 1 << (prvTgfPortsArray[2] & 0x1f);
        localPortsVlanMembers.ports[prvTgfPortsArray[3] >> 5] |= 1 << (prvTgfPortsArray[3] & 0x1f);

        /* set VLAN_2 entry */
        prvTgfNstPortIsolationTestInit(PRV_TGF_NEXTHOPE_VLANID_CNS, PRV_TGF_BRG_VLAN_PORT_ISOLATION_DISABLE_CMD_E, localPortsVlanMembers);

    }
}

/*******************************************************************************
* prvTgfNstL3PortIsolationConfigurationRestore
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
GT_VOID prvTgfNstL3PortIsolationConfigurationRestore
(
    GT_VOID
)
{
    GT_STATUS rc;
    GT_IPADDR ipAddr;
    GT_U32    entryIndex;

    prvTgfNstPortIsolationTestTableReset(PRV_TGF_VLANID_CNS);
    prvTgfNstPortIsolationTestTableReset(PRV_TGF_NEXTHOPE_VLANID_CNS);

    /* delete the Ipv4 prefixes */
    for (entryIndex = 0; entryIndex < PRV_TGF_SEND_COUNT_CNS; entryIndex++)
    {
        /* fill the destination IP address for Ipv4 prefix in Virtual Router */
        cpssOsMemCpy(ipAddr.arIP, prvTgfIpv4AddrArray[entryIndex], sizeof(ipAddr.arIP));

        /* delete the Ipv4 prefix */
        rc = prvTgfIpLpmIpv4UcPrefixDel(prvTgfLpmDBId, 0, ipAddr, 32);
        UTF_VERIFY_EQUAL1_STRING_MAC(GT_OK, rc, "prvTgfIpLpmIpv4UcPrefixDel: %d", prvTgfDevNum);
    }

    /* disable Port Isolation Feature */
    rc = prvTgfNstPortIsolationEnableSet(prvTgfDevNum, GT_FALSE);
    UTF_VERIFY_EQUAL2_STRING_MAC(GT_OK, rc, "prvTgfNstPortIsolationEnableSet: %d %d", prvTgfDevNum, GT_FALSE);
}

/*******************************************************************************
* prvTgfNstL2L3PortIsolationConfigurationSet
*
* DESCRIPTION:
*       Set test configuration:
*           Configure portIsolationMode on VLAN entry (VID = 5).
*           Enable Port Isolation feature.
*           Set port isolation table entry for L3 with:
*               devNum        = 0
*               trafficType   = CPSS_DXCH_NST_PORT_ISOLATION_TRAFFIC_TYPE_L3_E
*               srcInterface  = devNum = 0, portNum = 0
*               cpuPortMember = GT_FALSE
*               localPortsMembersPtr = 0x40000
*           Set port isolation table entry for L2 with:
*               devNum        = 0
*               trafficType   = CPSS_DXCH_NST_PORT_ISOLATION_TRAFFIC_TYPE_L2_E
*               srcInterface  = devNum = 0, portNum = 0
*               cpuPortMember = GT_FALSE
*               localPortsMembersPtr = 0x100
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
GT_VOID prvTgfNstL2L3PortIsolationConfigurationSet
(
    GT_VOID
)
{
    CPSS_INTERFACE_INFO_STC srcInterface = UTF_INTERFACE_INIT_VAL_CNS;
    GT_STATUS               rc = GT_OK;
    GT_U32                  entryIndex;
    PRV_TGF_IP_ROUTING_MODE_ENT routingMode;

    srcInterface.type = CPSS_INTERFACE_PORT_E;
    srcInterface.devPort.devNum = prvTgfDevNum;
    srcInterface.devPort.portNum = prvTgfPortsArray[PRV_TGF_SEND_PORT_IDX_ANOTHER_CNS];

    portIsolationCmd = PRV_TGF_BRG_VLAN_PORT_ISOLATION_L2_L3_CMD_E;

    /* clear entry */
    cpssOsMemSet(&localPortsVlanMembers, 0, sizeof(localPortsVlanMembers));

    /* ports 18, 23 are VLAN_2 Members */
    localPortsVlanMembers.ports[prvTgfPortsArray[2] >> 5] |= 1 << (prvTgfPortsArray[2] & 0x1f);
    localPortsVlanMembers.ports[prvTgfPortsArray[3] >> 5] |= 1 << (prvTgfPortsArray[3] & 0x1f);

    /* set VLAN_2 entry */
    prvTgfNstPortIsolationTestInit(PRV_TGF_NEXTHOPE_VLANID_CNS, portIsolationCmd, localPortsVlanMembers);

    /* clear entry */
    cpssOsMemSet(&localPortsVlanMembers, 0, sizeof(localPortsVlanMembers));

    /* ports 0, 8 are VLAN_1 Members */
    localPortsVlanMembers.ports[prvTgfPortsArray[0] >> 5] |= 1 << (prvTgfPortsArray[0] & 0x1f);
    localPortsVlanMembers.ports[prvTgfPortsArray[1] >> 5] |= 1 << (prvTgfPortsArray[1] & 0x1f);

    /* set VLAN_1 entry */
    prvTgfNstPortIsolationTestInit(PRV_TGF_VLANID_CNS, portIsolationCmd, localPortsVlanMembers);

    /* enable Port Isolation Feature */
    rc = prvTgfNstPortIsolationEnableSet(srcInterface.devPort.devNum, GT_TRUE);
    UTF_VERIFY_EQUAL2_STRING_MAC(GT_OK, rc, "prvTgfNstPortIsolationEnableSet: %d %d", srcInterface.devPort.devNum, GT_TRUE);

    /* set L3 port isolation table entry */
    portIsolationTraffic = PRV_TGF_NST_PORT_ISOLATION_TRAFFIC_TYPE_L3_E;

    /* clear entry */
    cpssOsMemSet(&localPortsMembers, 0, sizeof(localPortsMembers));

    /* port 18 is PortMembers */
    localPortsMembers.ports[prvTgfPortsArray[localL2L3PortsIdxMembers[1]] >> 5] |= 1 << (prvTgfPortsArray[localL2L3PortsIdxMembers[1]] & 0x1f);

    rc = prvTgfNstPortIsolationTableEntrySet(srcInterface.devPort.devNum,
                                             portIsolationTraffic,
                                             srcInterface,
                                             GT_FALSE,
                                             &localPortsMembers);
    UTF_VERIFY_EQUAL3_STRING_MAC(GT_OK, rc, "prvTgfNstPortIsolationTableEntrySet: %d, %d, %d",
                                 srcInterface.devPort.devNum, srcInterface.devPort.devNum, portIsolationTraffic);

    /* clear entry */
    cpssOsMemSet(&localPortsMembers, 0, sizeof(localPortsMembers));

    /* port 8 is PortMembers */
    localPortsMembers.ports[prvTgfPortsArray[localL2L3PortsIdxMembers[0]] >> 5] |= 1 << (prvTgfPortsArray[localL2L3PortsIdxMembers[0]] & 0x1f);

    /* set L2 port isolation table entry */
    portIsolationTraffic = PRV_TGF_NST_PORT_ISOLATION_TRAFFIC_TYPE_L2_E;

    rc = prvTgfNstPortIsolationTableEntrySet(srcInterface.devPort.devNum,
                                             portIsolationTraffic,
                                             srcInterface,
                                             GT_FALSE,
                                             &localPortsMembers);
    UTF_VERIFY_EQUAL3_STRING_MAC(GT_OK, rc, "prvTgfNstPortIsolationTableEntrySet: devNum = %d portNum = %d trafficType = %d",
                                 srcInterface.devPort.devNum, srcInterface.devPort.portNum, portIsolationTraffic);

    /* get routing mode */
    rc = prvTgfIpRoutingModeGet(&routingMode);
    UTF_VERIFY_EQUAL1_STRING_MAC(GT_OK, rc, "prvTgfIpRoutingModeGet: %d", prvTgfDevNum);

    /* set Ipv4 Routing */
    for (entryIndex = 0; entryIndex < PRV_TGF_SEND_COUNT_CNS; entryIndex++)
        switch(routingMode)
        {
            case PRV_TGF_IP_ROUTING_MODE_PCL_ACTION_E:

                prvTgfNstPortIsolationUcIpv4PbrRoutingSet(
                        prvTgfRouterArpIndexArray[entryIndex],
                        prvTgfArpMacArray[entryIndex],
                        prvTgfIpv4AddrArray[entryIndex],
                        prvTgfPortsArray[prvTgfNextHopePortIdxArray[entryIndex]]);
                break;

            case PRV_TGF_IP_ROUTING_MODE_IP_LTT_ENTRY_E:

                prvTgfNstPortIsolationUcIpv4LttRoutingSet(
                        prvTgfRouterArpIndexArray[entryIndex],
                        prvTgfArpMacArray[entryIndex],
                        prvTgfRouteEntryBaseIndexArray[entryIndex],
                        prvTgfIpv4AddrArray[entryIndex],
                        prvTgfPortsArray[prvTgfNextHopePortIdxArray[entryIndex]]);
                break;

            case PRV_TGF_IP_ROUTING_MODE_UNDEFINED_E:

                break;
        }
}

/*******************************************************************************
* prvTgfNstL2L3PortIsolationTrafficGenerate
*
* DESCRIPTION:
*       Generate traffic:
*           Send to device's port 0 packet:
*               macDa = FF:FF:FF:FF:FF:FF,
*               macSa = 00:00:00:00:00:05,
*           Send to device's port 0 two IP packets:
*               0000: 00 00 00 00 34 02 00 00 00 00 00 01 81 00 00 05
*               0010: 08 00 45 00 00 2A 00 00 00 00 40 04 76 CB 01 01
*               0020: 01 01 01 01 01 03 00 00 00 00 00 00 00 00 00 00
*               0030: 00 00 00 00 00 00 00 00 00 00 00 00
*
*               0000: 00 00 00 00 34 02 00 00 00 00 00 01 81 00 00 05
*               0010: 08 00 45 00 00 2A 00 00 00 00 40 04 72 C7 02 02
*               0020: 02 02 01 02 03 04 00 00 00 00 00 00 00 00 00 00
*               0030: 00 00 00 00 00 00 00 00 00 00 00 00
*
*           Success Criteria:
*               L2 packet is captured on port 8
*               L3 packet is captured on port 18
*       Additional Configuration:
*           Disable port isolation feature in VLAN entry
*       Generate Traffic:
*           Send to device's port 0 packet:
*               macDa = FF:FF:FF:FF:FF:FF,
*               macSa = 00:00:00:00:00:05,
*           Send to device's port 0 two IP packets:
*               0000: 00 00 00 00 34 02 00 00 00 00 00 01 81 00 00 05
*               0010: 08 00 45 00 00 2A 00 00 00 00 40 04 76 CB 01 01
*               0020: 01 01 01 01 01 03 00 00 00 00 00 00 00 00 00 00
*               0030: 00 00 00 00 00 00 00 00 00 00 00 00
*
*               0000: 00 00 00 00 34 02 00 00 00 00 00 01 81 00 00 05
*               0010: 08 00 45 00 00 2A 00 00 00 00 40 04 72 C7 02 02
*               0020: 02 02 01 02 03 04 00 00 00 00 00 00 00 00 00 00
*               0030: 00 00 00 00 00 00 00 00 00 00 00 00
*
*           Success Criteria:
*               L2 packet is captured on port 8
*               L3 packet is captured on port 18, 23
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
GT_VOID prvTgfNstL2L3PortIsolationTrafficGenerate
(
    GT_VOID
)
{
    GT_STATUS    rc = GT_OK;
    GT_U32       portIter = 0;
    GT_U32       sendIter = 0;
    GT_U32       expectedGoodOctetsValue;
    GT_U32       expectedGoodPctsValue;
    CPSS_PORT_MAC_COUNTER_SET_STC portCntrs;

    /* send two packets */
    for (sendIter = 0; sendIter < PRV_TGF_SEND_COUNT_CNS; sendIter++)
    {
        /* reset counters */
        for (portIter = 0; portIter < prvTgfPortsNum; portIter++)
        {
            rc = prvTgfResetCountersEth(prvTgfDevNum, prvTgfPortsArray[portIter]);
            UTF_VERIFY_EQUAL2_STRING_MAC(GT_OK, rc, "prvTgfResetCountersEth: %d, %d",
                                         prvTgfDevNum, prvTgfPortsArray[portIter]);
        }

        /* send IP packet 1 for L2 */
        prvTgfNstPortIsolationTestPacketSend(prvTgfPortsArray[PRV_TGF_SEND_PORT_IDX_ANOTHER_CNS], &prvTgfPacketInfo);

        /* setting IP addresses and checksum for packet 2 */
        prvTgfPacketIpv4Part.csum = 0x76CB;
        prvTgfPacketIpv4Part.srcAddr[0] = 1;
        prvTgfPacketIpv4Part.srcAddr[1] = 1;
        prvTgfPacketIpv4Part.srcAddr[2] = 1;
        prvTgfPacketIpv4Part.srcAddr[3] = 1;
        cpssOsMemCpy(prvTgfPacketIpv4Part.dstAddr, prvTgfIpv4AddrArray[0], sizeof(prvTgfPacketIpv4Part.dstAddr));

        /* send packet 2 for L3 */
        prvTgfNstPortIsolationTestPacketSend(prvTgfPortsArray[PRV_TGF_SEND_PORT_IDX_ANOTHER_CNS], &prvTgfPacketIpv4Info);

        /* setting IP addresses and checksum for packet 3 */
        prvTgfPacketIpv4Part.csum = 0x72C7;
        prvTgfPacketIpv4Part.srcAddr[0] = 2;
        prvTgfPacketIpv4Part.srcAddr[1] = 2;
        prvTgfPacketIpv4Part.srcAddr[2] = 2;
        prvTgfPacketIpv4Part.srcAddr[3] = 2;
        cpssOsMemCpy(prvTgfPacketIpv4Part.dstAddr, prvTgfIpv4AddrArray[1], sizeof(prvTgfPacketIpv4Part.dstAddr));

        /* send packet 3 for L3 */
        prvTgfNstPortIsolationTestPacketSend(prvTgfPortsArray[PRV_TGF_SEND_PORT_IDX_ANOTHER_CNS], &prvTgfPacketIpv4Info);

        /* check counters */
        for (portIter = 0; portIter < prvTgfPortsNum; portIter++)
        {
            /* read counters */
            rc = prvTgfReadPortCountersEth(prvTgfDevNum, prvTgfPortsArray[portIter], GT_TRUE, &portCntrs);
            UTF_VERIFY_EQUAL2_STRING_MAC(GT_OK, rc, "prvTgfReadPortCountersEth: %d, %d",
                                         prvTgfDevNum, prvTgfPortsArray[portIter]);

            if (PRV_TGF_SEND_PORT_IDX_ANOTHER_CNS == portIter)
            {
                /* check Rx counters */
                UTF_VERIFY_EQUAL0_STRING_MAC((PRV_TGF_PACKET_LEN_CNS + TGF_CRC_LEN_CNS) * prvTgfBurstCount * 3, portCntrs.goodOctetsRcv.l[0],
                                             "get another goodOctetsRcv counter than expected");
                UTF_VERIFY_EQUAL0_STRING_MAC(prvTgfBurstCount * 3, portCntrs.goodPktsRcv.l[0],
                                             "get another goodPktsRcv counter than expected");

                continue;
            }

            if (sendIter == 0)
            {
                if (prvTgfNextHopePortIdxArray[1] != portIter)
                {
                    expectedGoodOctetsValue = (PRV_TGF_PACKET_LEN_CNS + TGF_CRC_LEN_CNS) * prvTgfBurstCount;
                    expectedGoodPctsValue = prvTgfBurstCount;
                }
                else
                {
                    expectedGoodOctetsValue = 0;
                    expectedGoodPctsValue = 0;
                }
            }
            else /* sendIter == 1 */
            {
                expectedGoodOctetsValue = (PRV_TGF_PACKET_LEN_CNS + TGF_CRC_LEN_CNS) * prvTgfBurstCount;
                expectedGoodPctsValue = prvTgfBurstCount;
            }

            /* check Tx counters */
            UTF_VERIFY_EQUAL0_STRING_MAC(expectedGoodOctetsValue, portCntrs.goodOctetsSent.l[0],
                                         "get another goodOctetsSent counter than expected");
            UTF_VERIFY_EQUAL0_STRING_MAC(expectedGoodPctsValue, portCntrs.goodPktsSent.l[0],
                                         "get another goodPktsSent counter than expected");
        }

        /* disable port isolation feature per Ingress VLAN */

        /* clear entry */
        cpssOsMemSet(&localPortsVlanMembers, 0, sizeof(localPortsVlanMembers));

        /* ports 0, 8 are VLAN_1 Members */
        localPortsVlanMembers.ports[prvTgfPortsArray[0] >> 5] |= 1 << (prvTgfPortsArray[0] & 0x1f);
        localPortsVlanMembers.ports[prvTgfPortsArray[1] >> 5] |= 1 << (prvTgfPortsArray[1] & 0x1f);

        prvTgfNstPortIsolationTestInit(PRV_TGF_VLANID_CNS,
                 PRV_TGF_BRG_VLAN_PORT_ISOLATION_DISABLE_CMD_E, localPortsVlanMembers);

        /* enable IPv4 Unicast Routing on Vlan because prvTgfNstPortIsolationTestInit disables it */
        rc = prvTgfIpVlanRoutingEnable(PRV_TGF_VLANID_CNS, CPSS_IP_UNICAST_E, CPSS_IP_PROTOCOL_IPV4_E, GT_TRUE);
        UTF_VERIFY_EQUAL2_STRING_MAC(GT_OK, rc, "prvTgfNstL2L3PortIsolationTrafficGenerate: %d %d",
                                     prvTgfDevNum, prvTgfPortsArray[PRV_TGF_SEND_PORT_IDX_ANOTHER_CNS]);

        /* disable port isolation feature per Next HOP VLAN */

        /* clear entry */
        cpssOsMemSet(&localPortsVlanMembers, 0, sizeof(localPortsVlanMembers));

        /* ports 18, 23 are VLAN_2 Members */
        localPortsVlanMembers.ports[prvTgfPortsArray[2] >> 5] |= 1 << (prvTgfPortsArray[2] & 0x1f);
        localPortsVlanMembers.ports[prvTgfPortsArray[3] >> 5] |= 1 << (prvTgfPortsArray[3] & 0x1f);

        /* set VLAN_2 entry */
        prvTgfNstPortIsolationTestInit(PRV_TGF_NEXTHOPE_VLANID_CNS, PRV_TGF_BRG_VLAN_PORT_ISOLATION_DISABLE_CMD_E, localPortsVlanMembers);
    }
}

/*******************************************************************************
* prvTgfNstL2L3PortIsolationConfigurationRestore
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
GT_VOID prvTgfNstL2L3PortIsolationConfigurationRestore
(
    GT_VOID
)
{
    GT_STATUS rc;
    GT_IPADDR ipAddr;
    GT_U32    entryIndex;

    prvTgfNstPortIsolationTestTableReset(PRV_TGF_VLANID_CNS);
    prvTgfNstPortIsolationTestTableReset(PRV_TGF_NEXTHOPE_VLANID_CNS);

    /* delete the Ipv4 prefixes */
    for (entryIndex = 0; entryIndex < PRV_TGF_SEND_COUNT_CNS; entryIndex++)
    {
        /* fill the destination IP address for Ipv4 prefix in Virtual Router */
        cpssOsMemCpy(ipAddr.arIP, prvTgfIpv4AddrArray[entryIndex], sizeof(ipAddr.arIP));

        /* delete the Ipv4 prefix */
        rc = prvTgfIpLpmIpv4UcPrefixDel(prvTgfLpmDBId, 0, ipAddr, 32);
        UTF_VERIFY_EQUAL1_STRING_MAC(GT_OK, rc, "prvTgfIpLpmIpv4UcPrefixDel: %d", prvTgfDevNum);
    }

    /* disable Port Isolation Feature */
    rc = prvTgfNstPortIsolationEnableSet(prvTgfDevNum, GT_FALSE);
    UTF_VERIFY_EQUAL2_STRING_MAC(GT_OK, rc, "prvTgfNstPortIsolationEnableSet: %d %d", prvTgfDevNum, GT_FALSE);
}

/*******************************************************************************
* prvTgfNstL2TrunkPortIsolationConfigurationSet
*
* DESCRIPTION:
*       Set test configuration:
*           Configure port 8 and port 23 as members in trunk with trunkId = 1
*           Configure portIsolationMode on VLAN entry (VID = 5).
*           Enable Port Isolation feature
*           Set port isolation table entry for L2 with:
*               devNum        = 0
*               trafficType   = CPSS_DXCH_NST_PORT_ISOLATION_TRAFFIC_TYPE_L2_E
*               srcInterface.type    = CPSS_INTERFACE_TRUNK_E
*               srcInterface.trunkId = 1.
*               cpuPortMember        = GT_FALSE
*               localPortsMembersPtr = 0x40000
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
GT_VOID prvTgfNstL2TrunkPortIsolationConfigurationSet
(
    GT_VOID
)
{
    CPSS_INTERFACE_INFO_STC srcInterface = UTF_INTERFACE_INIT_VAL_CNS;
    GT_STATUS               rc = GT_OK;
    GT_U32                  portIter = 0;
    GT_TRUNK_ID         trunkId;

    /* set trunk members */
    trunkMembersArray[0].device = prvTgfDevNum;
    trunkMembersArray[1].device = prvTgfDevNum;

    trunkMembersArray[0].port = prvTgfPortsArray[1];
    trunkMembersArray[1].port = prvTgfPortsArray[3];

    trunkId = PRV_TGF_TRUNK_ID_CNS;
    CONVERT_TRUNK_ID_TEST_TO_CPSS_MAC(trunkId);
    rc = prvTgfTrunkMembersSet(trunkId, 2, trunkMembersArray, 0, NULL);
    UTF_VERIFY_EQUAL2_STRING_MAC(GT_OK, rc, "prvTgfTrunkMembersSet: %d %d", prvTgfDevNum, PRV_TGF_TRUNK_ID_CNS);

    portIsolationCmd = PRV_TGF_BRG_VLAN_PORT_ISOLATION_L2_CMD_E;

    /* clear entry */
    cpssOsMemSet(&localPortsVlanMembers, 0, sizeof(localPortsVlanMembers));

    /* ports 0, 8, 18, 23 are VLAN Members */
    for (portIter = 0; portIter < prvTgfPortsNum; portIter++)
    {
        localPortsVlanMembers.ports[prvTgfPortsArray[portIter] >> 5] |= 1 << (prvTgfPortsArray[portIter] & 0x1f);
    }

    /* set VLAN entry */
    prvTgfNstPortIsolationTestInit(PRV_TGF_VLANID_CNS, portIsolationCmd, localPortsVlanMembers);

    /* enable Port Isolation Feature */
    rc = prvTgfNstPortIsolationEnableSet(prvTgfDevNum, GT_TRUE);
    UTF_VERIFY_EQUAL2_STRING_MAC(GT_OK, rc, "prvTgfNstPortIsolationEnableSet: %d %d", prvTgfDevNum, GT_TRUE);

    /* set L2 port isolation table entry */
    portIsolationTraffic = PRV_TGF_NST_PORT_ISOLATION_TRAFFIC_TYPE_L2_E;

    /* clear entry */
    cpssOsMemSet(&localPortsMembers, 0, sizeof(localPortsMembers));

    /* port 18 is PortMembers */
    localPortsMembers.ports[prvTgfPortsArray[2] >> 5] |= 1 << (prvTgfPortsArray[2] & 0x1f);

    /* configure port isolation */
    srcInterface.type    = CPSS_INTERFACE_TRUNK_E;
    srcInterface.trunkId = PRV_TGF_TRUNK_ID_CNS;
    CONVERT_TRUNK_ID_TEST_TO_CPSS_MAC(srcInterface.trunkId);

    rc = prvTgfNstPortIsolationTableEntrySet(prvTgfDevNum,
                                             portIsolationTraffic,
                                             srcInterface,
                                             GT_FALSE,
                                             &localPortsMembers);
    UTF_VERIFY_EQUAL2_STRING_MAC(GT_OK, rc, "prvTgfNstPortIsolationTableEntrySet: devNum = %d, trafficType = %d",
                                 prvTgfDevNum, portIsolationTraffic);

    /* WA for port isolation on trunk errata - */
    /* configuring the port isolation entries for trunk members */
    if ( ((PRV_CPSS_PP_MAC(prvTgfDevNum)->devFamily == CPSS_PP_FAMILY_DXCH_XCAT_E) &&
          (PRV_CPSS_PP_MAC(prvTgfDevNum)->revision > 2)) ||
         (PRV_CPSS_PP_MAC(prvTgfDevNum)->devFamily == CPSS_PP_FAMILY_DXCH_LION_E) )
    {
        srcInterface.type = CPSS_INTERFACE_PORT_E;
        srcInterface.devPort.devNum = prvTgfDevNum;
        srcInterface.devPort.portNum = trunkMembersArray[0].port;

        rc = prvTgfNstPortIsolationTableEntrySet(prvTgfDevNum,
                                                 portIsolationTraffic,
                                                 srcInterface,
                                                 GT_FALSE,
                                                 &localPortsMembers);
        UTF_VERIFY_EQUAL2_STRING_MAC(GT_OK, rc, "prvTgfNstPortIsolationTableEntrySet: devNum = %d, trafficType = %d",
                                     prvTgfDevNum, portIsolationTraffic);

        srcInterface.devPort.portNum = trunkMembersArray[1].port;

        rc = prvTgfNstPortIsolationTableEntrySet(prvTgfDevNum,
                                                 portIsolationTraffic,
                                                 srcInterface,
                                                 GT_FALSE,
                                                 &localPortsMembers);
        UTF_VERIFY_EQUAL2_STRING_MAC(GT_OK, rc, "prvTgfNstPortIsolationTableEntrySet: devNum = %d, trafficType = %d",
                                     prvTgfDevNum, portIsolationTraffic);
    }

}

/*******************************************************************************
* prvTgfNstL2TrunkPortIsolationTrafficGenerate
*
* DESCRIPTION:
*       Generate traffic:
*           Send to device's port 8 packet:
*               macDa = FF:FF:FF:FF:FF:FF,
*
*           Success Criteria:
*               Packet is captured on port 18
*
*       Additional Configuration:
*           Disable port isolation feature in VLAN entry
*       Generate Traffic:
*           Send to device's port 8 packet:
*               macDa = FF:FF:FF:FF:FF:FF,
*               macSa = 00:00:00:00:00:05,
*
*           Success Criteria:
*               Packet is captured on ports 0, 18
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
GT_VOID prvTgfNstL2TrunkPortIsolationTrafficGenerate
(
    GT_VOID
)
{
    GT_STATUS                       rc       = GT_OK;
    GT_U32                          portIter = 0;
    GT_U32                          sendIter = 0;
    GT_U32                          expectedGoodOctetsValue;
    GT_U32                          expectedGoodPctsValue;
    CPSS_PORT_MAC_COUNTER_SET_STC   portCntrs;


    /* send two packets */
    for (sendIter = 0; sendIter < PRV_TGF_SEND_COUNT_CNS; sendIter++)
    {
        /* reset counters */
        for (portIter = 0; portIter < prvTgfPortsNum; portIter++)
        {
            rc = prvTgfResetCountersEth(prvTgfDevNum, prvTgfPortsArray[portIter]);
            UTF_VERIFY_EQUAL2_STRING_MAC(GT_OK, rc, "prvTgfResetCountersEth: %d, %d",
                                         prvTgfDevNum, prvTgfPortsArray[portIter]);
        }

        /* send packet */
        prvTgfNstPortIsolationTestPacketSend(prvTgfPortsArray[PRV_TGF_SEND_PORT_IDX_CNS], &prvTgfPacketInfo);

        /* check counters */
        for (portIter = 0; portIter < prvTgfPortsNum; portIter++)
        {
            /* read counters */
            rc = prvTgfReadPortCountersEth(prvTgfDevNum, prvTgfPortsArray[portIter], GT_TRUE, &portCntrs);
            UTF_VERIFY_EQUAL2_STRING_MAC(GT_OK, rc, "prvTgfReadPortCountersEth: %d, %d",
                                         prvTgfDevNum, prvTgfPortsArray[portIter]);

            if (PRV_TGF_SEND_PORT_IDX_CNS == portIter)
            {
                /* check Rx counters */
                UTF_VERIFY_EQUAL0_STRING_MAC((PRV_TGF_PACKET_LEN_CNS + TGF_CRC_LEN_CNS) * prvTgfBurstCount, portCntrs.goodOctetsRcv.l[0],
                                             "get another goodOctetsRcv counter than expected");
                UTF_VERIFY_EQUAL0_STRING_MAC(prvTgfBurstCount, portCntrs.goodPktsRcv.l[0],
                                             "get another goodPktsRcv counter than expected");

                continue;
            }

            /* set expected counters */
            if (0 == sendIter)
            {
                if (((localPortsMembers.ports[prvTgfPortsArray[portIter] >> 5] >> (prvTgfPortsArray[portIter] & 0x1f)) & 1 ) != 0)
                {
                    expectedGoodOctetsValue = (PRV_TGF_PACKET_LEN_CNS + TGF_CRC_LEN_CNS) * prvTgfBurstCount;
                    expectedGoodPctsValue = prvTgfBurstCount;
                }
                else
                {
                    expectedGoodOctetsValue = 0;
                    expectedGoodPctsValue = 0;
                }
            }
            else
            {
                if ((trunkMembersArray[0].port != prvTgfPortsArray[portIter]) &&
                    (trunkMembersArray[1].port != prvTgfPortsArray[portIter]))
                {
                    expectedGoodOctetsValue = (PRV_TGF_PACKET_LEN_CNS + TGF_CRC_LEN_CNS) * prvTgfBurstCount;
                    expectedGoodPctsValue = prvTgfBurstCount;
                }
                else
                {
                    expectedGoodOctetsValue = 0;
                    expectedGoodPctsValue = 0;
                }
            }

            /* check Tx counters */
            UTF_VERIFY_EQUAL0_STRING_MAC(expectedGoodOctetsValue, portCntrs.goodOctetsSent.l[0],
                                         "get another goodOctetsSent counter than expected");
            UTF_VERIFY_EQUAL0_STRING_MAC(expectedGoodPctsValue, portCntrs.goodPktsSent.l[0],
                                         "get another goodPktsSent counter than expected");
        }

        /* disable port isolation feature per VLAN */
        prvTgfNstPortIsolationTestInit(PRV_TGF_VLANID_CNS,
                 PRV_TGF_BRG_VLAN_PORT_ISOLATION_DISABLE_CMD_E, localPortsVlanMembers);
    }
}

/*******************************************************************************
* prvTgfNstL2TrunkPortIsolationConfigurationRestore
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
GT_VOID prvTgfNstL2TrunkPortIsolationConfigurationRestore
(
    GT_VOID
)
{
    GT_STATUS rc = GT_OK;
    GT_TRUNK_ID         trunkId;

    prvTgfNstPortIsolationTestTableReset(PRV_TGF_VLANID_CNS);

    /* disable Port Isolation Feature */
    rc = prvTgfNstPortIsolationEnableSet(prvTgfDevNum, GT_FALSE);
    UTF_VERIFY_EQUAL2_STRING_MAC(GT_OK, rc, "prvTgfNstPortIsolationEnableSet: %d %d", prvTgfDevNum, GT_FALSE);

    trunkId = PRV_TGF_TRUNK_ID_CNS;
    CONVERT_TRUNK_ID_TEST_TO_CPSS_MAC(trunkId);
    /* invalidate trunk entry */
    rc = prvTgfTrunkMembersSet(trunkId, 0, NULL, 0, NULL);
    UTF_VERIFY_EQUAL2_STRING_MAC(GT_OK, rc, "prvTgfTrunkMembersSet: %d %d", prvTgfDevNum, PRV_TGF_TRUNK_ID_CNS);
}

/*******************************************************************************
* prvTgfNstL3TrunkPortIsolationConfigurationSet
*
* DESCRIPTION:
*       Set test configuration:
*           Configure port 0 as member in trunk with truckId = 1
*           Configure portIsolationMode on VLAN entry (VID = 5).
*           Enable Port Isolation feature.
*           Set port isolation table entry for L2 with:
*               devNum               = 0
*               trafficType          = CPSS_DXCH_NST_PORT_ISOLATION_TRAFFIC_TYPE_L3_E
*               srcInterface.type    = CPSS_INTERFACE_TRUNK_E
*               srcInterface.trunkId = 1.
*               cpuPortMember        = GT_FALSE
*               localPortsMembersPtr = 0x40000
*           Perform Two route entries UC IPV4 Routing:
*           Note: change port interface to truck interface.
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
GT_VOID prvTgfNstL3TrunkPortIsolationConfigurationSet
(
    GT_VOID
)
{
    CPSS_INTERFACE_INFO_STC srcInterface = UTF_INTERFACE_INIT_VAL_CNS;
    GT_STATUS               rc = GT_OK;
    GT_U32                  entryIndex;
    GT_TRUNK_ID             trunkId;
    PRV_TGF_IP_ROUTING_MODE_ENT routingMode;

    /* set trunk members */
    trunkMembersArray[0].device = prvTgfDevNum;
    trunkMembersArray[0].port = prvTgfPortsArray[PRV_TGF_SEND_PORT_IDX_ANOTHER_CNS];

    trunkId = PRV_TGF_TRUNK_ID_CNS;
    CONVERT_TRUNK_ID_TEST_TO_CPSS_MAC(trunkId);

    rc = prvTgfTrunkMembersSet(trunkId, 1, trunkMembersArray, 0, NULL);
    UTF_VERIFY_EQUAL2_STRING_MAC(GT_OK, rc, "prvTgfTrunkMembersSet: %d %d", prvTgfDevNum, PRV_TGF_TRUNK_ID_CNS);

    portIsolationCmd = PRV_TGF_BRG_VLAN_PORT_ISOLATION_L3_CMD_E;

    /* clear entry */
    cpssOsMemSet(&localPortsVlanMembers, 0, sizeof(localPortsVlanMembers));

    /* ports 18, 23 are VLAN_2 Members */
    localPortsVlanMembers.ports[prvTgfPortsArray[2] >> 5] |= 1 << (prvTgfPortsArray[2] & 0x1f);
    localPortsVlanMembers.ports[prvTgfPortsArray[3] >> 5] |= 1 << (prvTgfPortsArray[3] & 0x1f);

    /* set VLAN_2 entry */
    prvTgfNstPortIsolationTestInit(PRV_TGF_NEXTHOPE_VLANID_CNS, portIsolationCmd, localPortsVlanMembers);

    /* clear entry */
    cpssOsMemSet(&localPortsVlanMembers, 0, sizeof(localPortsVlanMembers));

    /* ports 0, 8 are VLAN_1 Members */
    localPortsVlanMembers.ports[prvTgfPortsArray[0] >> 5] |= 1 << (prvTgfPortsArray[0] & 0x1f);
    localPortsVlanMembers.ports[prvTgfPortsArray[1] >> 5] |= 1 << (prvTgfPortsArray[1] & 0x1f);

    /* set VLAN_1 entry */
    prvTgfNstPortIsolationTestInit(PRV_TGF_VLANID_CNS, portIsolationCmd, localPortsVlanMembers);

    /* enable Port Isolation Feature */
    rc = prvTgfNstPortIsolationEnableSet(prvTgfDevNum, GT_TRUE);
    UTF_VERIFY_EQUAL2_STRING_MAC(GT_OK, rc, "prvTgfNstPortIsolationEnableSet: %d %d", prvTgfDevNum, GT_TRUE);

    /* set L3 port isolation table entry */
    portIsolationTraffic = PRV_TGF_NST_PORT_ISOLATION_TRAFFIC_TYPE_L3_E;

    /* clear entry */
    cpssOsMemSet(&localPortsMembers, 0, sizeof(localPortsMembers));

    /* port 18 is PortMembers */
    localPortsMembers.ports[prvTgfPortsArray[2] >> 5] |= 1 << (prvTgfPortsArray[2] & 0x1f);

    /* configure port isolation */
    srcInterface.type    = CPSS_INTERFACE_TRUNK_E;
    srcInterface.trunkId = PRV_TGF_TRUNK_ID_CNS;
    CONVERT_TRUNK_ID_TEST_TO_CPSS_MAC(srcInterface.trunkId);

    rc = prvTgfNstPortIsolationTableEntrySet(prvTgfDevNum,
                                             portIsolationTraffic,
                                             srcInterface,
                                             GT_FALSE,
                                             &localPortsMembers);
    UTF_VERIFY_EQUAL2_STRING_MAC(GT_OK, rc, "prvTgfNstPortIsolationTableEntrySet: devNum = %d, trafficType = %d",
                                 prvTgfDevNum, portIsolationTraffic);

    /* WA for port isolation on trunk errata - */
    /* configuring the port isolation entries for trunk members */
    if ( ((PRV_CPSS_PP_MAC(prvTgfDevNum)->devFamily == CPSS_PP_FAMILY_DXCH_XCAT_E) &&
          (PRV_CPSS_PP_MAC(prvTgfDevNum)->revision > 2)) ||
         (PRV_CPSS_PP_MAC(prvTgfDevNum)->devFamily == CPSS_PP_FAMILY_DXCH_LION_E) )
    {
        srcInterface.type = CPSS_INTERFACE_PORT_E;
        srcInterface.devPort.devNum = prvTgfDevNum;
        srcInterface.devPort.portNum = trunkMembersArray[0].port;

        rc = prvTgfNstPortIsolationTableEntrySet(prvTgfDevNum,
                                                 portIsolationTraffic,
                                                 srcInterface,
                                                 GT_FALSE,
                                                 &localPortsMembers);
        UTF_VERIFY_EQUAL2_STRING_MAC(GT_OK, rc, "prvTgfNstPortIsolationTableEntrySet: devNum = %d, trafficType = %d",
                                     prvTgfDevNum, portIsolationTraffic);
    }

    /* get routing mode */
    rc = prvTgfIpRoutingModeGet(&routingMode);
    UTF_VERIFY_EQUAL1_STRING_MAC(GT_OK, rc, "prvTgfIpRoutingModeGet: %d", prvTgfDevNum);

    /* set Ipv4 Routing */
    for (entryIndex = 0; entryIndex < PRV_TGF_SEND_COUNT_CNS; entryIndex++)
        switch(routingMode)
        {
            case PRV_TGF_IP_ROUTING_MODE_PCL_ACTION_E:

                prvTgfNstPortIsolationUcIpv4PbrRoutingSet(
                        prvTgfRouterArpIndexArray[entryIndex],
                        prvTgfArpMacArray[entryIndex],
                        prvTgfIpv4AddrArray[entryIndex],
                        prvTgfPortsArray[prvTgfNextHopePortIdxArray[entryIndex]]);
                break;

            case PRV_TGF_IP_ROUTING_MODE_IP_LTT_ENTRY_E:

                prvTgfNstPortIsolationUcIpv4LttRoutingSet(
                        prvTgfRouterArpIndexArray[entryIndex],
                        prvTgfArpMacArray[entryIndex],
                        prvTgfRouteEntryBaseIndexArray[entryIndex],
                        prvTgfIpv4AddrArray[entryIndex],
                        prvTgfPortsArray[prvTgfNextHopePortIdxArray[entryIndex]]);
                break;

            case PRV_TGF_IP_ROUTING_MODE_UNDEFINED_E:

                break;
        }
}

/*******************************************************************************
* prvTgfNstL3TrunkPortIsolationTrafficGenerate
*
* DESCRIPTION:
*       Generate traffic:
*           Send to device's port 0 two IP packets:
*               0000: 00 00 00 00 34 02 00 00 00 00 00 01 81 00 00 05
*               0010: 08 00 45 00 00 2A 00 00 00 00 40 04 76 CB 01 01
*               0020: 01 01 01 01 01 03 00 00 00 00 00 00 00 00 00 00
*               0030: 00 00 00 00 00 00 00 00 00 00 00 00
*
*               0000: 00 00 00 00 34 02 00 00 00 00 00 01 81 00 00 05
*               0010: 08 00 45 00 00 2A 00 00 00 00 40 04 72 C7 02 02
*               0020: 02 02 01 02 03 04 00 00 00 00 00 00 00 00 00 00
*               0030: 00 00 00 00 00 00 00 00 00 00 00 00
*
*           Success Criteria:
*               One packet is captured on port 18
*
*           Additional Configuration:
*               Disable port isolation feature in VLAN entry.
*
*           Send to device's port 0 two IP packets:
*               0000: 00 00 00 00 34 02 00 00 00 00 00 01 81 00 00 05
*               0010: 08 00 45 00 00 2A 00 00 00 00 40 04 76 CB 01 01
*               0020: 01 01 01 01 01 03 00 00 00 00 00 00 00 00 00 00
*               0030: 00 00 00 00 00 00 00 00 00 00 00 00
*
*               0000: 00 00 00 00 34 02 00 00 00 00 00 01 81 00 00 05
*               0010: 08 00 45 00 00 2A 00 00 00 00 40 04 72 C7 02 02
*               0020: 02 02 01 02 03 04 00 00 00 00 00 00 00 00 00 00
*               0030: 00 00 00 00 00 00 00 00 00 00 00 00
*
*           Success Criteria:
*               Packet is captured on ports: 18,23
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
GT_VOID prvTgfNstL3TrunkPortIsolationTrafficGenerate
(
    GT_VOID
)
{
    GT_STATUS    rc = GT_OK;
    GT_U32       portIter = 0;
    GT_U32       sendIter = 0;
    GT_U32       expectedGoodOctetsValue;
    GT_U32       expectedGoodPctsValue;
    CPSS_PORT_MAC_COUNTER_SET_STC portCntrs;

    /* send two packets */
    for (sendIter = 0; sendIter < PRV_TGF_SEND_COUNT_CNS; sendIter++)
    {
        /* reset counters */
        for (portIter = 0; portIter < prvTgfPortsNum; portIter++)
        {
            rc = prvTgfResetCountersEth(prvTgfDevNum, prvTgfPortsArray[portIter]);
            UTF_VERIFY_EQUAL2_STRING_MAC(GT_OK, rc, "prvTgfResetCountersEth: %d, %d",
                                         prvTgfDevNum, prvTgfPortsArray[portIter]);
        }

        /* setting IP addresses and checksum for packet 1 */
        prvTgfPacketIpv4Part.csum = 0x76CB;
        prvTgfPacketIpv4Part.srcAddr[0] = 1;
        prvTgfPacketIpv4Part.srcAddr[1] = 1;
        prvTgfPacketIpv4Part.srcAddr[2] = 1;
        prvTgfPacketIpv4Part.srcAddr[3] = 1;
        cpssOsMemCpy(prvTgfPacketIpv4Part.dstAddr, prvTgfIpv4AddrArray[0], sizeof(prvTgfPacketIpv4Part.dstAddr));

        /* send packet 1 */
        prvTgfNstPortIsolationTestPacketSend(prvTgfPortsArray[PRV_TGF_SEND_PORT_IDX_ANOTHER_CNS], &prvTgfPacketIpv4Info);

        /* setting IP addresses and checksum for packet 2 */
        prvTgfPacketIpv4Part.csum = 0x72C7;
        prvTgfPacketIpv4Part.srcAddr[0] = 2;
        prvTgfPacketIpv4Part.srcAddr[1] = 2;
        prvTgfPacketIpv4Part.srcAddr[2] = 2;
        prvTgfPacketIpv4Part.srcAddr[3] = 2;
        cpssOsMemCpy(prvTgfPacketIpv4Part.dstAddr, prvTgfIpv4AddrArray[1], sizeof(prvTgfPacketIpv4Part.dstAddr));

        /* send packet 2 */
        prvTgfNstPortIsolationTestPacketSend(prvTgfPortsArray[PRV_TGF_SEND_PORT_IDX_ANOTHER_CNS], &prvTgfPacketIpv4Info);

        /* check counters */
        for (portIter = 0; portIter < prvTgfPortsNum; portIter++)
        {
            /* read counters */
            rc = prvTgfReadPortCountersEth(prvTgfDevNum, prvTgfPortsArray[portIter], GT_TRUE, &portCntrs);
            UTF_VERIFY_EQUAL2_STRING_MAC(GT_OK, rc, "prvTgfReadPortCountersEth: %d, %d",
                                         prvTgfDevNum, prvTgfPortsArray[portIter]);

            if (PRV_TGF_SEND_PORT_IDX_ANOTHER_CNS == portIter)
            {
                /* check Rx counters */
                UTF_VERIFY_EQUAL0_STRING_MAC((PRV_TGF_PACKET_LEN_CNS + TGF_CRC_LEN_CNS) * prvTgfBurstCount * 2, portCntrs.goodOctetsRcv.l[0],
                                             "get another goodOctetsRcv counter than expected");
                UTF_VERIFY_EQUAL0_STRING_MAC(prvTgfBurstCount * 2, portCntrs.goodPktsRcv.l[0],
                                             "get another goodPktsRcv counter than expected");

                continue;
            }

            if (sendIter == 0)
            {
                if (prvTgfNextHopePortIdxArray[0] == portIter)
                {
                    expectedGoodOctetsValue = (PRV_TGF_PACKET_LEN_CNS + TGF_CRC_LEN_CNS) * prvTgfBurstCount;
                    expectedGoodPctsValue = prvTgfBurstCount;
                }
                else
                {
                    expectedGoodOctetsValue = 0;
                    expectedGoodPctsValue = 0;
                }
            }
            else /* sendIter == 1 */
            {
                if (prvTgfNextHopePortIdxArray[0] == portIter ||
                    prvTgfNextHopePortIdxArray[1] == portIter)
                {
                    expectedGoodOctetsValue = (PRV_TGF_PACKET_LEN_CNS + TGF_CRC_LEN_CNS) * prvTgfBurstCount;
                    expectedGoodPctsValue = prvTgfBurstCount;
                }
                else
                {
                    expectedGoodOctetsValue = 0;
                    expectedGoodPctsValue = 0;
                }
            }

            /* check Tx counters */
            UTF_VERIFY_EQUAL0_STRING_MAC(expectedGoodOctetsValue, portCntrs.goodOctetsSent.l[0],
                                         "get another goodOctetsSent counter than expected");
            UTF_VERIFY_EQUAL0_STRING_MAC(expectedGoodPctsValue, portCntrs.goodPktsSent.l[0],
                                         "get another goodPktsSent counter than expected");
        }
        /* disable port isolation feature per Next HOP VLAN */

        /* clear entry */
        cpssOsMemSet(&localPortsVlanMembers, 0, sizeof(localPortsVlanMembers));

        /* ports 18, 23 are VLAN_2 Members */
        localPortsVlanMembers.ports[prvTgfPortsArray[2] >> 5] |= 1 << (prvTgfPortsArray[2] & 0x1f);
        localPortsVlanMembers.ports[prvTgfPortsArray[3] >> 5] |= 1 << (prvTgfPortsArray[3] & 0x1f);

        /* set VLAN_2 entry */
        prvTgfNstPortIsolationTestInit(PRV_TGF_NEXTHOPE_VLANID_CNS, PRV_TGF_BRG_VLAN_PORT_ISOLATION_DISABLE_CMD_E, localPortsVlanMembers);

    }
}

/*******************************************************************************
* prvTgfNstL3TrunkPortIsolationConfigurationRestore
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
GT_VOID prvTgfNstL3TrunkPortIsolationConfigurationRestore
(
    GT_VOID
)
{
    GT_STATUS rc;
    GT_IPADDR ipAddr;
    GT_U32    entryIndex;
    GT_TRUNK_ID         trunkId;

    prvTgfNstPortIsolationTestTableReset(PRV_TGF_VLANID_CNS);
    prvTgfNstPortIsolationTestTableReset(PRV_TGF_NEXTHOPE_VLANID_CNS);

    /* delete the Ipv4 prefixes */
    for (entryIndex = 0; entryIndex < PRV_TGF_SEND_COUNT_CNS; entryIndex++)
    {
        /* fill the destination IP address for Ipv4 prefix in Virtual Router */
        cpssOsMemCpy(ipAddr.arIP, prvTgfIpv4AddrArray[entryIndex], sizeof(ipAddr.arIP));

        /* delete the Ipv4 prefix */
        rc = prvTgfIpLpmIpv4UcPrefixDel(prvTgfLpmDBId, 0, ipAddr, 32);
        UTF_VERIFY_EQUAL1_STRING_MAC(GT_OK, rc, "prvTgfIpLpmIpv4UcPrefixDel: %d", prvTgfDevNum);
    }

    /* disable Port Isolation Feature */
    rc = prvTgfNstPortIsolationEnableSet(prvTgfDevNum, GT_FALSE);
    UTF_VERIFY_EQUAL2_STRING_MAC(GT_OK, rc, "prvTgfNstPortIsolationEnableSet: %d %d", prvTgfDevNum, GT_FALSE);

    trunkId = PRV_TGF_TRUNK_ID_CNS;
    CONVERT_TRUNK_ID_TEST_TO_CPSS_MAC(trunkId);

    /* invalidate trunk entry */
    rc = prvTgfTrunkMembersSet(trunkId, 0, NULL, 0, NULL);
    UTF_VERIFY_EQUAL2_STRING_MAC(GT_OK, rc, "prvTgfTrunkMembersSet: %d %d", prvTgfDevNum, PRV_TGF_TRUNK_ID_CNS);
}

