/*******************************************************************************
*              (c), Copyright 2001, Marvell International Ltd.                 *
* THIS CODE CONTAINS CONFIDENTIAL INFORMATION OF MARVELL SEMICONDUCTOR, INC.   *
* NO RIGHTS ARE GRANTED HEREIN UNDER ANY PATENT, MASK WORK RIGHT OR COPYRIGHT  *
* OF MARVELL OR ANY THIRD PARTY. MARVELL RESERVES THE RIGHT AT ITS SOLE        *
* DISCRETION TO REQUEST THAT THIS CODE BE IMMEDIATELY RETURNED TO MARVELL.     *
* THIS CODE IS PROVIDED "AS IS". MARVELL MAKES NO WARRANTIES, EXPRESSED,       *
* IMPLIED OR OTHERWISE, REGARDING ITS ACCURACY, COMPLETENESS OR PERFORMANCE.   *
********************************************************************************
* prvTgfIpNonExactMatchRoutingIpv4Uc.c
*
* DESCRIPTION:
*       Non-exact match IPv4 UC prefix routing.
*
* FILE REVISION NUMBER:
*       $Revision: 1.1.1.1 $
*
*******************************************************************************/

#include <cpssCommon/cpssPresteraDefs.h>
#include <cpss/extServices/private/prvCpssBindFunc.h>

#include <utf/utfMain.h>
#include <utf/private/prvUtfHelpers.h>

#include <trafficEngine/tgfTrafficGenerator.h>
#include <trafficEngine/tgfTrafficTable.h>

#include <common/tgfCommon.h>
#include <common/tgfBridgeGen.h>
#include <common/tgfIpGen.h>

#include <ip/prvTgfIpNonExactMatchRoutingIpv4Uc.h>

/******************************************************************************\
 *                       Test configuration section                           *
\******************************************************************************/

/* Send VLAN */
#define PRV_TGF_SEND_VLANID_CNS           5

/* Nexthop VLAN */
#define PRV_TGF_NEXTHOP_VLANID_CNS        6

/* Port index to send traffic */
#define PRV_TGF_SEND_PORT_IDX_CNS         0

/* Second port index for send VLAN */
#define PRV_TGF_SEND_PORT_IDX_2_CNS       1

/* Port indexes to receive traffic */
#define PRV_TGF_RCV1_PORT_IDX_CNS         2
#define PRV_TGF_RCV2_PORT_IDX_CNS         3

/* Number of packets to send */
#define PRV_TGF_BURST_COUNT_CNS          32

/* Max prefix length */
#define PRV_TGF_MAX_PREFIX_LEN_CNS       32

/* The Route entry index for Route entry Table */
static GT_U32 prvTgfRouteEntryBaseIndex = 10;

/* the LPM DB id for LPM Table */
static GT_U32 prvTgfLpmDBId             = 0;

/* the Virtual Router id for LPM Table */
static GT_U32 prvTgfVrId                = 0;

/* The Arp Address index of the Router ARP Table
   to write to the UC Route entry Arp nextHopARPPointer field */
static GT_U32 prvTgfRouterArpIndex      = 1;

/* The ARP MAC address to write to the Router ARP Table */
static TGF_MAC_ADDR  prvTgfArpMac = {0x00, 0x00, 0x00, 0x00, 0x00, 0x22};

/* Base unicast IP address */
static TGF_IPV4_ADDR prvTgfBaseUcIpv4 = {10, 153, 102, 170};

/****************************** Test packet *********************************/

/* L2 part of UC packets */
static TGF_PACKET_L2_STC prvTgfPacketL2PartUc =
{
    {0x00, 0x00, 0x00, 0x00, 0x34, 0x02}, /* daMac */
    {0x00, 0x00, 0x00, 0x00, 0x00, 0x01}  /* saMac */
};

/* VLAN_TAG part */
static TGF_PACKET_VLAN_TAG_STC prvTgfPacketVlanTagPart =
{
    TGF_ETHERTYPE_8100_VLAN_TAG_CNS,      /* etherType */
    0, 0, PRV_TGF_SEND_VLANID_CNS         /* pri, cfi, VlanId */
};

/* Packet's IPv4 ethertype */
static TGF_PACKET_ETHERTYPE_STC prvTgfPacketEtherTypePartIpv4 =
{
    TGF_ETHERTYPE_0800_IPV4_TAG_CNS
};

/* Packet's UC IPv4 */
static TGF_PACKET_IPV4_STC prvTgfPacketUcIpv4Part =
{
    4,                                /* version */
    5,                                /* headerLen */
    0,                                /* typeOfService */
    0x2A,                             /* totalLen */
    0,                                /* id */
    0,                                /* flags */
    0,                                /* offset */
    0x40,                             /* timeToLive */
    0x04,                             /* protocol */
    TGF_PACKET_AUTO_CALC_CHECKSUM_CNS,/* csum */
    { 1,   1,   1,   1},              /* srcAddr = ipSrc */
    {10, 153, 102, 170}               /* dstAddr */
};

/* Packet's payload */
static GT_U8 prvTgfPayloadDataArr[] =
{
    0x00, 0xFF, 0xFF, 0xFF, 0xFF, 0x06, 0x07, 0x08,
    0x09, 0x0A, 0x0B, 0x0C, 0x0D, 0x0E, 0x0F, 0x10,
    0x12, 0x13, 0x14, 0x15, 0x16, 0x17, 0x18, 0x19,
    0x1A, 0x1B
};

/* PAYLOAD part */
static TGF_PACKET_PAYLOAD_STC prvTgfPacketPayloadPart =
{
    sizeof(prvTgfPayloadDataArr),  /* dataLength */
    prvTgfPayloadDataArr           /* dataPtr */
};

/* PARTS of UC IPv4 packet */
static TGF_PACKET_PART_STC prvTgfPacketPartArrayUcIpv4[] =
{
    {TGF_PACKET_PART_L2_E,        &prvTgfPacketL2PartUc},
    {TGF_PACKET_PART_VLAN_TAG_E,  &prvTgfPacketVlanTagPart},
    {TGF_PACKET_PART_ETHERTYPE_E, &prvTgfPacketEtherTypePartIpv4},
    {TGF_PACKET_PART_IPV4_E,      &prvTgfPacketUcIpv4Part},
    {TGF_PACKET_PART_PAYLOAD_E,   &prvTgfPacketPayloadPart}
};

/* LENGTH of IPv4 packet */
#define PRV_TGF_IPV4_PACKET_LEN_CNS \
    TGF_L2_HEADER_SIZE_CNS + TGF_VLAN_TAG_SIZE_CNS + TGF_ETHERTYPE_SIZE_CNS + \
    TGF_IPV4_HEADER_SIZE_CNS + sizeof(prvTgfPayloadDataArr)

/* UC IPv4 PACKET to send */
static TGF_PACKET_STC prvTgfPacketInfoUcIpv4 =
{
    PRV_TGF_IPV4_PACKET_LEN_CNS,                                       /* totalLen */
    sizeof(prvTgfPacketPartArrayUcIpv4) / sizeof(TGF_PACKET_PART_STC), /* numOfParts */
    prvTgfPacketPartArrayUcIpv4                                        /* partsArray */
};

/* Offset of destination IPv4 address */
#define PRV_TGF_DST_IPV4_OFFSET_CNS    ( TGF_L2_HEADER_SIZE_CNS   +  \
                                         TGF_VLAN_TAG_SIZE_CNS    +  \
                                         TGF_ETHERTYPE_SIZE_CNS   +  \
                                         TGF_IPV4_HEADER_SIZE_CNS -  \
                                         sizeof(TGF_IPV4_ADDR) )

/* Offset to store index of modified bit in IPv4 packet*/
#define PRV_TGF_MOD_BIT_IND_IPV4_OFFSET_CNS ( TGF_L2_HEADER_SIZE_CNS + \
                                         TGF_VLAN_TAG_SIZE_CNS       + \
                                         TGF_ETHERTYPE_SIZE_CNS      + \
                                         TGF_IPV4_HEADER_SIZE_CNS )

/*************************** Restore config ***********************************/

/* Parameters needed to be restored */
static struct
{
    GT_U16 vid;
} prvTgfRestoreCfg;

/******************************************************************************\
 *                            Private test functions                           *
\******************************************************************************/

/*******************************************************************************
* prvTgfIpNonExactMatchRoutingIpv4UcPortCountersCheck
*
* DESCRIPTION:
*       Read and check port's counters
*
* INPUTS:
*       portInterfacePtr - (pointer to) port interface
*       packetsCountRx   - Expected number of Rx packets
*       packetsCountTx   - Expected number of Tx packets
*       packetSize       - Size of packets
*       prefixLen        - current prefix length
*
* OUTPUTS:
*       None
*
* RETURNS:
*       GT_OK        - on success
*       GT_FAIL      - on error
*       GT_BAD_PARAM - wrong interface
*
* COMMENTS:
*
*******************************************************************************/
static GT_STATUS prvTgfIpNonExactMatchRoutingIpv4UcPortCountersCheck
(
    IN CPSS_INTERFACE_INFO_STC *portInterfacePtr,
    IN GT_U32 packetsCountRx,
    IN GT_U32 packetsCountTx,
    IN GT_U32 packetSize,
    IN GT_U32 prefixLen
)
{
    GT_STATUS                     rc = GT_OK;
    CPSS_PORT_MAC_COUNTER_SET_STC portCntrs;
    GT_BOOL                       isCountersOk;

    /* Read counters */
    rc = prvTgfReadPortCountersEth(portInterfacePtr->devPort.devNum,
                                   portInterfacePtr->devPort.portNum, GT_FALSE, &portCntrs);
    PRV_UTF_VERIFY_GT_OK(rc, "prvTgfReadPortCountersEth");

    /* Check Tx and Rx counters */
    isCountersOk = portCntrs.goodPktsSent.l[0]   == packetsCountTx &&
                   portCntrs.goodPktsRcv.l[0]    == packetsCountRx &&
                   portCntrs.goodOctetsSent.l[0] == (packetSize + TGF_CRC_LEN_CNS) * packetsCountTx &&
                   portCntrs.goodOctetsRcv.l[0]  == (packetSize + TGF_CRC_LEN_CNS) * packetsCountRx;

    /* Print expected values in case unexpected number of packets is received */
    if (GT_FALSE == isCountersOk)
    {
        PRV_UTF_LOG1_MAC("Unexpected number of packets received for IP prefix length = %u:\n", prefixLen);
        PRV_UTF_LOG2_MAC("  goodPktsSent   = %u, expected value = %u\n",
                portCntrs.goodPktsSent.l[0], packetsCountTx);
        PRV_UTF_LOG2_MAC("  goodPktsRcv    = %u, expected value = %u\n",
                portCntrs.goodPktsRcv.l[0], packetsCountRx);
        PRV_UTF_LOG2_MAC("  goodOctetsSent = %u, expected value = %u\n",
                portCntrs.goodOctetsSent.l[0],((packetSize + TGF_CRC_LEN_CNS) * packetsCountTx));
        PRV_UTF_LOG2_MAC("  goodOctetsRcv  = %u, expected value = %u\n",
                portCntrs.goodOctetsRcv.l[0], ((packetSize + TGF_CRC_LEN_CNS) * packetsCountRx));
    }

    return GT_FALSE == isCountersOk ? GT_FAIL : rc;
}

/*******************************************************************************
* prvTgfIpNonExactMatchRoutingIpv4UcCapturedPacketsVerify
*
* DESCRIPTION:
*       Verify that Rx Table contains expected captured packets.
*
* INPUTS:
*       portInterfacePtr - (pointer to) port interface
*       prefixLen        - current prefix length
*
* OUTPUTS:
*
* RETURNS:
*       GT_OK        - on success
*       GT_FAIL      - on error
*       GT_BAD_PARAM - wrong interface
*       GT_BAD_PTR   - on NULL pointer
*
* COMMENTS:
*
*******************************************************************************/
GT_STATUS prvTgfIpNonExactMatchRoutingIpv4UcCapturedPacketsVerify
(
    IN CPSS_INTERFACE_INFO_STC *portInterfacePtr,
    IN GT_U8 prefixLen
)
{
    GT_STATUS       rc            = GT_OK;
    GT_U32          packetLen     = PRV_TGF_IPV4_PACKET_LEN_CNS;
    GT_U32          origPacketLen = 0;
    GT_BOOL         getFirst      = GT_TRUE;
    GT_U8           devNum        = 0;
    GT_U8           queue         = 0;
    TGF_NET_DSA_STC rxParam       = {0};
    GT_U8           packetBuf[PRV_TGF_IPV4_PACKET_LEN_CNS];

    /* Go through entries from rxNetworkIf table */
    while (GT_OK == rc)
    {
        rc = tgfTrafficGeneratorRxInCpuGet(TGF_PACKET_TYPE_CAPTURE_E,
                                           getFirst, GT_FALSE, packetBuf,
                                           &packetLen, &origPacketLen,
                                           &devNum, &queue, &rxParam);
        getFirst = GT_FALSE; /* now we get the next */

        /* Skip all not expected packets */
        if ((rxParam.srcIsTrunk || !rxParam.dsaCmdIsToCpu) ||
            (rxParam.devNum != portInterfacePtr->devPort.devNum) ||
            (rxParam.portNum != portInterfacePtr->devPort.portNum) )
        {
            continue;
        }

        /* Validate stored modified bit index value */
        if (GT_OK == rc)
        {
            /* Show error log in case unexpected value is received */
            if (packetBuf[PRV_TGF_MOD_BIT_IND_IPV4_OFFSET_CNS] < prefixLen)
            {
                PRV_UTF_LOG0_MAC("Unexpected packet received\n");
                PRV_UTF_LOG1_MAC(" Modified bit index = %u\n", packetBuf[PRV_TGF_MOD_BIT_IND_IPV4_OFFSET_CNS]);
                PRV_UTF_LOG1_MAC(" IP prefix length = %u\n", prefixLen);
                PRV_UTF_LOG4_MAC(" Destination IP = %u.%u.%u.%u\n", packetBuf[PRV_TGF_DST_IPV4_OFFSET_CNS],
                        packetBuf[PRV_TGF_DST_IPV4_OFFSET_CNS + 1], packetBuf[PRV_TGF_DST_IPV4_OFFSET_CNS + 2],
                        packetBuf[PRV_TGF_DST_IPV4_OFFSET_CNS + 3]);
            }
        }
    }

    /* Value GT_NO_MORE is ok in this case */
    return (GT_NO_MORE == rc ? GT_OK : rc);
}

/******************************************************************************\
 *                            Public test functions                           *
\******************************************************************************/

/*******************************************************************************
* prvTgfIpNonExactMatchRoutingIpv4UcBaseConfigurationSet
*
* DESCRIPTION:
*       Set Base Configuration
*
* INPUTS:
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
GT_VOID prvTgfIpNonExactMatchRoutingIpv4UcBaseConfigurationSet
(
    GT_VOID
)
{
    GT_STATUS                 rc          = GT_OK;
    GT_U8                     tagArray[]  = {1, 1, 1, 1};
    PRV_TGF_BRG_MAC_ENTRY_STC macEntry;

    /* Create SEND VLAN on ports [0, 1] */
    rc = prvTgfBrgDefVlanEntryWithPortsSet(PRV_TGF_SEND_VLANID_CNS,
                                           prvTgfPortsArray, NULL, tagArray, 2);
    UTF_VERIFY_EQUAL1_STRING_MAC(GT_OK, rc, "prvTgfBrgDefVlanEntryWithPortsSet: %d", prvTgfDevNum);

    /* Create a NEXTHOP VLAN for on ports [2, 3] */
    rc = prvTgfBrgDefVlanEntryWithPortsSet(PRV_TGF_NEXTHOP_VLANID_CNS,
                                           (prvTgfPortsArray + 2), NULL, tagArray, 2);
    UTF_VERIFY_EQUAL1_STRING_MAC(GT_OK, rc, "prvTgfBrgDefVlanEntryWithPortsSet: %d", prvTgfDevNum);

    /* Save default vlanId for restore */
    rc = prvTgfBrgVlanPortVidGet(prvTgfDevNum,
                                 prvTgfPortsArray[PRV_TGF_SEND_PORT_IDX_CNS],
                                 &prvTgfRestoreCfg.vid);
    UTF_VERIFY_EQUAL2_STRING_MAC(GT_OK, rc, "prvTgfBrgVlanPortVidGet: %d, %d",
                                 prvTgfDevNum, &prvTgfRestoreCfg.vid);

    /* Set default vlanId */
    rc = prvTgfBrgVlanPortVidSet(prvTgfDevNum,
                                 prvTgfPortsArray[PRV_TGF_SEND_PORT_IDX_CNS],
                                 PRV_TGF_SEND_VLANID_CNS);
    UTF_VERIFY_EQUAL2_STRING_MAC(GT_OK, rc, "prvTgfBrgVlanPortVidSet: %d, %d",
                                 prvTgfDevNum, prvTgfPortsArray[PRV_TGF_SEND_PORT_IDX_CNS]);

    /* create a macEntry with .daRoute = GT_TRUE */
    cpssOsMemSet(&macEntry, 0, sizeof(macEntry));
    cpssOsMemCpy(macEntry.key.key.macVlan.macAddr.arEther, prvTgfPacketL2PartUc.daMac, sizeof(TGF_MAC_ADDR));
    macEntry.key.entryType                  = PRV_TGF_FDB_ENTRY_TYPE_MAC_ADDR_E;
    macEntry.key.key.macVlan.vlanId         = PRV_TGF_SEND_VLANID_CNS;
    macEntry.dstInterface.type              = CPSS_INTERFACE_PORT_E;
    macEntry.dstInterface.devPort.devNum    = prvTgfDevNum;
    macEntry.dstInterface.devPort.portNum   = prvTgfPortsArray[PRV_TGF_SEND_PORT_IDX_CNS];
    macEntry.isStatic                       = GT_TRUE;
    macEntry.daCommand                      = PRV_TGF_PACKET_CMD_FORWARD_E;
    macEntry.saCommand                      = PRV_TGF_PACKET_CMD_FORWARD_E;
    macEntry.daRoute                        = GT_TRUE;

    rc = prvTgfBrgFdbMacEntrySet(&macEntry);
    UTF_VERIFY_EQUAL1_STRING_MAC(GT_OK, rc, "prvTgfBrgFdbMacEntryWrite: %d", prvTgfDevNum);
}

/*******************************************************************************
* prvTgfIpNonExactMatchRoutingIpv4UcRouteConfigurationSet
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
GT_VOID prvTgfIpNonExactMatchRoutingIpv4UcRouteConfigurationSet
(
    GT_VOID
)
{
    GT_STATUS                     rc = GT_OK;
    GT_ETHERADDR                  arpMacAddr;
    PRV_TGF_IP_UC_ROUTE_ENTRY_STC routeEntriesArray[1];

    /* Enable Routing on a Port */
    rc = prvTgfIpPortRoutingEnable(prvTgfPortsArray[PRV_TGF_SEND_PORT_IDX_CNS],
                                   CPSS_UNICAST_E, CPSS_IP_PROTOCOL_IPV4_E, GT_TRUE);
    UTF_VERIFY_EQUAL2_STRING_MAC(GT_OK, rc, "prvTgfIpPortRoutingEnable: %d %d",
                                 prvTgfDevNum, prvTgfPortsArray[PRV_TGF_SEND_PORT_IDX_CNS]);

    /* Enable Routing on Vlan */
    rc = prvTgfIpVlanRoutingEnable(PRV_TGF_SEND_VLANID_CNS,
                                   CPSS_UNICAST_E, CPSS_IP_PROTOCOL_IPV4_E, GT_TRUE);
    UTF_VERIFY_EQUAL2_STRING_MAC(GT_OK, rc, "prvTgfIpVlanRoutingEnable: %d %d",
                                 prvTgfDevNum, PRV_TGF_SEND_VLANID_CNS);

    /* Write a ARP MAC address to the Router ARP Table */
    cpssOsMemCpy(arpMacAddr.arEther, prvTgfArpMac, sizeof(arpMacAddr));

    rc = prvTgfIpRouterArpAddrWrite(prvTgfRouterArpIndex, &arpMacAddr);
    UTF_VERIFY_EQUAL1_STRING_MAC(GT_OK, rc, "prvTgfIpRouterArpAddrWrite: %d", prvTgfDevNum);

    /* Write a UC Route entry to the Route Table */
    cpssOsMemSet(routeEntriesArray, 0, sizeof(routeEntriesArray));
    routeEntriesArray[0].cmd                              = CPSS_PACKET_CMD_ROUTE_E;
    routeEntriesArray[0].nextHopVlanId                    = PRV_TGF_NEXTHOP_VLANID_CNS;
    routeEntriesArray[0].nextHopInterface.type            = CPSS_INTERFACE_PORT_E;
    routeEntriesArray[0].nextHopInterface.devPort.devNum  = prvTgfDevNum;
    routeEntriesArray[0].nextHopInterface.devPort.portNum = prvTgfPortsArray[PRV_TGF_RCV2_PORT_IDX_CNS];
    routeEntriesArray[0].nextHopARPPointer                = prvTgfRouterArpIndex;

    rc = prvTgfIpUcRouteEntriesWrite(prvTgfRouteEntryBaseIndex, routeEntriesArray, 1);
    UTF_VERIFY_EQUAL1_STRING_MAC(GT_OK, rc, "prvTgfIpUcRouteEntriesWrite: %d", prvTgfDevNum);

    /* Reading and checking the UC Route entry from the Route Table */
    cpssOsMemSet(routeEntriesArray, 0, sizeof(routeEntriesArray));

    rc = prvTgfIpUcRouteEntriesRead(prvTgfDevNum, prvTgfRouteEntryBaseIndex, routeEntriesArray, 1);
    UTF_VERIFY_EQUAL1_STRING_MAC(GT_OK, rc, "prvTgfIpUcRouteEntriesRead: %d", prvTgfDevNum);
    PRV_UTF_LOG2_MAC("nextHopVlanId = %d, portNum = %d\n",
                     routeEntriesArray[0].nextHopVlanId,
                     routeEntriesArray[0].nextHopInterface.devPort.portNum);
}

/*******************************************************************************
* prvTgfIpNonExactMatchRoutingIpv4UcTrafficGenerate
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
GT_VOID prvTgfIpNonExactMatchRoutingIpv4UcTrafficGenerate
(
    GT_VOID
)
{
    GT_STATUS rc          = GT_OK;
    GT_U32    prefixIndex = 0;
    GT_U32    exRxCount;
    GT_U32    exTxCount;
    GT_U8     prefixLen;
    GT_U8     mask;
    GT_U8     byteIter;
    GT_U8     bitIter;
    GT_IPADDR ipAddr;
    CPSS_INTERFACE_INFO_STC portInt = {0};
    PRV_TGF_IP_TCAM_ROUTE_ENTRY_INFO_UNT nextHopInfo;
    GT_IPADDR ipAddrArray[PRV_TGF_BURST_COUNT_CNS];
    TGF_VFD_INFO_STC vfdArray[2] = {{0, 0}};

    /* Prepare IP address */
    cpssOsMemCpy(ipAddr.arIP, prvTgfBaseUcIpv4, sizeof(prvTgfBaseUcIpv4));

    /* Display used IP prefix */
    PRV_UTF_LOG4_MAC("Used IP prefix = %u.%u.%u.%u\n", prvTgfBaseUcIpv4[0],
            prvTgfBaseUcIpv4[1], prvTgfBaseUcIpv4[2], prvTgfBaseUcIpv4[3]);

    /* Setup port interface */
    portInt.type            = CPSS_INTERFACE_PORT_E;
    portInt.devPort.devNum  = prvTgfDevNum;
    portInt.devPort.portNum = prvTgfPortsArray[PRV_TGF_RCV2_PORT_IDX_CNS];

    /* Fill a nexthope info for the prefix */
    cpssOsMemSet(&nextHopInfo, 0, sizeof(nextHopInfo));
    nextHopInfo.ipLttEntry.routeEntryBaseIndex = prvTgfRouteEntryBaseIndex;

    /* Generate Array of IP addresses */
    for (byteIter = 0; byteIter < 4; ++byteIter)
    {
        /* Invert appropriate bit */
        for (bitIter = 0; bitIter < 8; ++bitIter)
        {
            cpssOsMemCpy(ipAddrArray[prefixIndex].arIP, prvTgfBaseUcIpv4, sizeof(prvTgfBaseUcIpv4));
            mask = (GT_U8)(1 << (7 - bitIter));
            ipAddrArray[prefixIndex].arIP[byteIter] = (GT_U8)(prvTgfBaseUcIpv4[byteIter] ^ mask);
            ++prefixIndex;
        }
    }

    /* Prepare VFD array entry for DST IP address*/
    vfdArray[0].mode               = TGF_VFD_MODE_ARRAY_PATTERN_E;
    vfdArray[0].arrayPatternOffset = 0;
    vfdArray[0].offset             = PRV_TGF_DST_IPV4_OFFSET_CNS;
    vfdArray[0].arrayPatternSize   = sizeof(GT_IPADDR);
    vfdArray[0].cycleCount         = sizeof(GT_IPADDR);
    vfdArray[0].arrayPatternPtr    = (GT_U8*)ipAddrArray;

    /* Prepare VFD array entry to transfer modified bit index */
    vfdArray[1].mode       = TGF_VFD_MODE_INCREMENT_E;
    vfdArray[1].incValue   = 1;
    vfdArray[1].offset     = PRV_TGF_MOD_BIT_IND_IPV4_OFFSET_CNS;
    vfdArray[1].cycleCount = 1;

    /* Go trough the all prefix lengths */
    for (prefixLen = 1; prefixLen < PRV_TGF_MAX_PREFIX_LEN_CNS; ++prefixLen)
    {
        /* Add appropriate prefix to the LPM DB*/
        rc = prvTgfIpLpmIpv4UcPrefixAdd(prvTgfLpmDBId, prvTgfVrId, ipAddr,
                                        prefixLen, &nextHopInfo, GT_FALSE);
        UTF_VERIFY_EQUAL1_STRING_MAC(GT_OK, rc, "prvTgfIpLpmIpv4UcPrefixAdd: %d", prvTgfDevNum);

        /* Reset Ethernet counters */
        rc = prvTgfEthCountersReset(prvTgfDevNum);
        UTF_VERIFY_EQUAL0_STRING_MAC(GT_OK, rc, "prvTgfEthCountersReset");

        /* Clear capturing RxPcktTable */
        rc = tgfTrafficTableRxPcktTblClear();
        UTF_VERIFY_EQUAL0_STRING_MAC(GT_OK, rc, "tgfTrafficTableRxPcktTblClear");

        /* Enable capturing on receive port */
        rc = tgfTrafficGeneratorPortTxEthCaptureSet(&portInt, TGF_CAPTURE_MODE_MIRRORING_E, GT_TRUE);
        UTF_VERIFY_EQUAL2_STRING_MAC(GT_OK, rc,
               "ERROR of tgfTrafficGeneratorPortTxEthCaptureSet: %d, %d\n",
               prvTgfDevNum, prvTgfPortsArray[PRV_TGF_SEND_PORT_IDX_CNS]);

        /* Setup packet */
        rc = prvTgfSetTxSetupEth(prvTgfDevNum, &prvTgfPacketInfoUcIpv4,
                                 PRV_TGF_BURST_COUNT_CNS, 2, vfdArray);
        UTF_VERIFY_EQUAL1_STRING_MAC(GT_OK, rc, "prvTgfSetTxSetupEth: %d", prvTgfDevNum);

        /* Disable packet tracing */
        rc = tgfTrafficTracePacketByteSet(GT_FALSE);
        UTF_VERIFY_EQUAL1_STRING_MAC(GT_OK, rc, "tgfTrafficTracePacketByteSet: %d", prvTgfDevNum);

        /* Send packet */
        rc = prvTgfStartTransmitingEth(prvTgfDevNum, prvTgfPortsArray[PRV_TGF_SEND_PORT_IDX_CNS]);
        UTF_VERIFY_EQUAL2_STRING_MAC(GT_OK, rc, "ERROR of StartTransmitting: %d, %d\n",
            prvTgfDevNum, prvTgfPortsArray[PRV_TGF_SEND_PORT_IDX_CNS]);

        /* Disable capture on receive port */
        rc = tgfTrafficGeneratorPortTxEthCaptureSet(&portInt, TGF_CAPTURE_MODE_MIRRORING_E, GT_FALSE);
        UTF_VERIFY_EQUAL2_STRING_MAC(GT_OK, rc,
               "ERROR of tgfTrafficGeneratorPortTxEthCaptureSet: %d, %d\n",
               prvTgfDevNum, prvTgfPortsArray[PRV_TGF_RCV2_PORT_IDX_CNS]);

        /* Check port counters contain expected number of packets */
        exRxCount = PRV_TGF_BURST_COUNT_CNS - prefixLen;
        exTxCount = PRV_TGF_BURST_COUNT_CNS - prefixLen;

        rc = prvTgfIpNonExactMatchRoutingIpv4UcPortCountersCheck(&portInt, exRxCount, exTxCount,
                prvTgfPacketInfoUcIpv4.totalLen, prefixLen);
        UTF_VERIFY_EQUAL0_STRING_MAC(GT_OK, rc, "ERROR of prvTgfPortCountersCheck");

        /* Verify captured packets */
        rc = prvTgfIpNonExactMatchRoutingIpv4UcCapturedPacketsVerify(&portInt, prefixLen);
        UTF_VERIFY_EQUAL0_STRING_MAC(GT_OK, rc, "ERROR of prvTgfCapturedPacketsVerify");

        /* Delete the Ipv4 prefix */
        rc = prvTgfIpLpmIpv4UcPrefixDel(prvTgfLpmDBId, prvTgfVrId, ipAddr, prefixLen);
        UTF_VERIFY_EQUAL1_STRING_MAC(GT_OK, rc, "prvTgfIpLpmIpv4UcPrefixDel: %d", prvTgfDevNum);
    }
}

/*******************************************************************************
* prvTgfIpNonExactMatchRoutingIpv4UcConfigurationRestore
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
*       1. Restore Route Configuration
*       2. Restore Base Configuration
*
*******************************************************************************/
GT_VOID prvTgfIpNonExactMatchRoutingIpv4UcConfigurationRestore
(
    GT_VOID
)
{
    GT_STATUS rc = GT_OK;

    PRV_UTF_LOG0_MAC("======= Restoring Configuration =======\n");

    /* Clear capturing RxPcktTable */
    rc = tgfTrafficTableRxPcktTblClear();
    UTF_VERIFY_EQUAL0_STRING_MAC(GT_OK, rc, "tgfTrafficTableRxPcktTblClear");

    /* -------------------------------------------------------------------------
     * 2. Restore Route Configuration
     */

    /* Flash all UC Ipv4 prefixes */
    rc = prvTgfIpLpmIpv4UcPrefixesFlush(prvTgfLpmDBId, prvTgfVrId);
    UTF_VERIFY_EQUAL0_STRING_MAC(GT_OK, rc, "prvTgfIpLpmIpv4UcPrefixesFlush");

    /* Disable Unicast IPv4 Routing on a Port */
    rc = prvTgfIpPortRoutingEnable(prvTgfPortsArray[PRV_TGF_SEND_PORT_IDX_CNS],
            CPSS_IP_UNICAST_E, CPSS_IP_PROTOCOL_IPV4_E, GT_FALSE);
    UTF_VERIFY_EQUAL2_STRING_MAC(GT_OK, rc, "prvTgfIpPortRoutingEnable: %d %d",
            prvTgfDevNum, prvTgfPortsArray[PRV_TGF_SEND_PORT_IDX_CNS]);

    /* Disable IPv4 Unicast Routing on Vlan */
    rc = prvTgfIpVlanRoutingEnable(PRV_TGF_SEND_VLANID_CNS, CPSS_IP_UNICAST_E,
                                   CPSS_IP_PROTOCOL_IPV4_E, GT_FALSE);
    UTF_VERIFY_EQUAL2_STRING_MAC(GT_OK, rc, "prvTgfIpVlanRoutingEnable: %d %d",
                                 prvTgfDevNum, PRV_TGF_SEND_VLANID_CNS);

    /* -------------------------------------------------------------------------
     * 1. Restore Base Configuration
     */

    /* Delete the FDB entry */
    rc = prvTgfBrgFdbFlush(GT_TRUE);
    UTF_VERIFY_EQUAL1_STRING_MAC(GT_OK, rc, "prvTgfBrgFdbFlush: %d", prvTgfDevNum);

    /* Restore default vlanId to all ports */
    rc = prvTgfBrgVlanPvidSet(prvTgfRestoreCfg.vid);
    UTF_VERIFY_EQUAL2_STRING_MAC(GT_OK, rc, "prvTgfBrgVlanPvidSet: %d, %d",
                                 prvTgfDevNum, prvTgfRestoreCfg.vid);

    /* Invalidate SEND VLAN entry */
    rc = prvTgfBrgDefVlanEntryInvalidate(PRV_TGF_SEND_VLANID_CNS);
    UTF_VERIFY_EQUAL1_STRING_MAC(GT_OK, rc, "prvTgfBrgDefVlanEntryInvalidate: %d",
                                 PRV_TGF_SEND_VLANID_CNS);

    /* Invalidate nexthop VLAN entry */
    rc = prvTgfBrgDefVlanEntryInvalidate(PRV_TGF_NEXTHOP_VLANID_CNS);
    UTF_VERIFY_EQUAL1_STRING_MAC(GT_OK, rc, "prvTgfBrgDefVlanEntryInvalidate: %d",
                                 PRV_TGF_NEXTHOP_VLANID_CNS);
}
