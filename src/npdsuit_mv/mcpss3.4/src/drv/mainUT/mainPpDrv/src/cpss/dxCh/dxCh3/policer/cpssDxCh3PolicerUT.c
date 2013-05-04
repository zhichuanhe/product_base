/*******************************************************************************
*              (c), Copyright 2001, Marvell International Ltd.                 *
* THIS CODE CONTAINS CONFIDENTIAL INFORMATION OF MARVELL SEMICONDUCTOR, INC.   *
* NO RIGHTS ARE GRANTED HEREIN UNDER ANY PATENT, MASK WORK RIGHT OR COPYRIGHT  *
* OF MARVELL OR ANY THIRD PARTY. MARVELL RESERVES THE RIGHT AT ITS SOLE        *
* DISCRETION TO REQUEST THAT THIS CODE BE IMMEDIATELY RETURNED TO MARVELL.     *
* THIS CODE IS PROVIDED "AS IS". MARVELL MAKES NO WARRANTIES, EXPRESSED,       *
* IMPLIED OR OTHERWISE, REGARDING ITS ACCURACY, COMPLETENESS OR PERFORMANCE.   *
********************************************************************************
* cpssDxCh3PolicerUT.c
*
* DESCRIPTION:
*       Unit tests for cpssDxCh3Policer, that provides
*       CPSS DxCh3 Policing Engine API.
*       The Policing Engine (or Traffic Conditioner) is responsible for:
*           - Ingress Metering - implemented using SrTCM Token Bucket,
*             TrTCM Token Bucket and Two Color Mode algorithms.
*           - Ingress Billing - byte/packet counters that keep track of amount
*             of traffic belonging to a flow.
*             Re-Marking - QoS assigned to the packet according to metering
*             results.
*
* FILE REVISION NUMBER:
*       $Revision: 1.1.1.1 $
*
*******************************************************************************/

/* includes */
#include <cpss/dxCh/dxCh3/policer/cpssDxCh3Policer.h>
#include <cpss/dxCh/dxChxGen/config/cpssDxChCfgInit.h>
#include <cpss/dxCh/dxChxGen/config/private/prvCpssDxChInfo.h>

#include <utf/utfMain.h>
#include <utf/private/prvUtfExtras.h>

/* Defines */

/* Valid Physical port */
#define POLICER_VALID_PHY_PORT_NUM_CNS  0

/* Invalid Physical port */
#define POLICER_INVALID_PHY_PORT_NUM_CNS  128

/* Invalid enum */
#define POLICER_INVALID_ENUM_CNS        0x5AAAAAA5

/* Policer stage number: Ingress #0, Ingress #1 or Egress */
static CPSS_DXCH_POLICER_STAGE_TYPE_ENT plrStage = CPSS_DXCH_POLICER_STAGE_INGRESS_0_E;

/* check if stage is supported */
#define IS_STAGE_EXIST_MAC(_dev, _stage) (PRV_CPSS_DXCH_PP_MAC(_dev)->policer.memSize[_stage])

/*----------------------------------------------------------------------------*/
/*
GT_STATUS cpssDxCh3PolicerMeteringEnableSet
(
    IN GT_U8    devNum,
    IN GT_BOOL  enable
)
*/
UTF_TEST_CASE_MAC(cpssDxCh3PolicerMeteringEnableSet)
{
/*
    ITERATE_DEVICES (DxCh3)
    1.1. Call function with enable[GT_FALSE / GT_TRUE]
    Expected: GT_OK.
    1.2. Call cpssDxCh3PolicerMeteringEnableGet with not NULL enablePtr.
    Expected: GT_OK and the same enable as was set.
*/
    GT_STATUS   st = GT_OK;
    GT_U8       dev;

    GT_BOOL     enable    = GT_FALSE;
    GT_BOOL     enableGet = GT_FALSE;


    /* prepare device iterator */
    PRV_TGF_NOT_APPLIC_DEV_RESET_MAC(&dev, UTF_CH1_E | UTF_CH1_DIAMOND_E | UTF_CH2_E);

    /* 1. Go over all active devices. */
    while (GT_OK == prvUtfNextDeviceGet(&dev, GT_TRUE))
    {
        /*
            1.1. Call function with enable[GT_FALSE / GT_TRUE]
            Expected: GT_OK.
        */
        /* iterate with enable = GT_FALSE */
        enable = GT_FALSE;

        st = cpssDxCh3PolicerMeteringEnableSet(dev, plrStage, enable);
        UTF_VERIFY_EQUAL2_PARAM_MAC(GT_OK, st, dev, enable);

        /*
            1.2. Call cpssDxCh3PolicerMeteringEnableGet with not NULL enablePtr.
            Expected: GT_OK and the same enable as was set.
        */
        st = cpssDxCh3PolicerMeteringEnableGet(dev, plrStage, &enableGet);
        UTF_VERIFY_EQUAL1_STRING_MAC(GT_OK, st,
                   "cpssDxCh3PolicerMeteringEnableGet: %d", dev);

        /* verifying values */
        UTF_VERIFY_EQUAL1_STRING_MAC(enable, enableGet,
                   "got another enable then was set: %d", dev);

        /* iterate with enable = GT_TRUE */
        enable = GT_TRUE;

        st = cpssDxCh3PolicerMeteringEnableSet(dev, plrStage, enable);
        UTF_VERIFY_EQUAL2_PARAM_MAC(GT_OK, st, dev, enable);

        /*
            1.2. Call cpssDxCh3PolicerMeteringEnableGet with not NULL enablePtr.
            Expected: GT_OK and the same enable as was set.
        */
        st = cpssDxCh3PolicerMeteringEnableGet(dev, plrStage, &enableGet);
        UTF_VERIFY_EQUAL1_STRING_MAC(GT_OK, st,
                   "cpssDxCh3PolicerMeteringEnableGet: %d", dev);

        /* verifying values */
        UTF_VERIFY_EQUAL1_STRING_MAC(enable, enableGet,
                   "got another enable then was set: %d", dev);
    }

    enable = GT_FALSE;

    /* 2. For not-active devices and devices from non-applicable family */
    /* check that function returns GT_BAD_PARAM.                        */

    /* prepare device iterator */
    PRV_TGF_NOT_APPLIC_DEV_RESET_MAC(&dev, UTF_CH1_E | UTF_CH1_DIAMOND_E | UTF_CH2_E);

    /* go over all non active devices */
    while(GT_OK == prvUtfNextDeviceGet(&dev, GT_FALSE))
    {
        st = cpssDxCh3PolicerMeteringEnableSet(dev, plrStage, enable);
        UTF_VERIFY_EQUAL1_PARAM_MAC(GT_NOT_APPLICABLE_DEVICE, st, dev);
    }

    /* 3. Call function with out of bound value for device id.*/
    dev = PRV_CPSS_MAX_PP_DEVICES_CNS;

    st = cpssDxCh3PolicerMeteringEnableSet(dev, plrStage, enable);
    UTF_VERIFY_EQUAL1_PARAM_MAC(GT_BAD_PARAM, st, dev);
}

/*----------------------------------------------------------------------------*/
/*
GT_STATUS cpssDxCh3PolicerMeteringEnableGet
(
    IN  GT_U8       devNum,
    OUT GT_BOOL     *enablePtr
)
*/
UTF_TEST_CASE_MAC(cpssDxCh3PolicerMeteringEnableGet)
{
/*
    ITERATE_DEVICES (DxCh3)
    1.1. Call function with not NULL enablePtr.
    Expected: GT_OK.
    1.2. Call function with enablePtr [NULL].
    Expected: GT_BAD_PTR.
*/
    GT_STATUS   st = GT_OK;
    GT_U8       dev;

    GT_BOOL     enable = GT_FALSE;


    /* prepare device iterator */
    PRV_TGF_NOT_APPLIC_DEV_RESET_MAC(&dev, UTF_CH1_E | UTF_CH1_DIAMOND_E | UTF_CH2_E);

    /* 1. Go over all active devices. */
    while (GT_OK == prvUtfNextDeviceGet(&dev, GT_TRUE))
    {
        /*
            1.1. Call function with not NULL enablePtr.
            Expected: GT_OK.
        */
        st = cpssDxCh3PolicerMeteringEnableGet(dev, plrStage, &enable);
        UTF_VERIFY_EQUAL1_PARAM_MAC(GT_OK, st, dev);

        /*
            1.2. Call function with enablePtr [NULL].
            Expected: GT_BAD_PTR.
        */
        st = cpssDxCh3PolicerMeteringEnableGet(dev, plrStage, NULL);
        UTF_VERIFY_EQUAL1_STRING_MAC(GT_BAD_PTR, st, "%d, enablePtr = NULL", dev);
    }

    /* 2. For not-active devices and devices from non-applicable family */
    /* check that function returns GT_BAD_PARAM.                        */

    /* prepare device iterator */
    PRV_TGF_NOT_APPLIC_DEV_RESET_MAC(&dev, UTF_CH1_E | UTF_CH1_DIAMOND_E | UTF_CH2_E);

    /* go over all non active devices */
    while(GT_OK == prvUtfNextDeviceGet(&dev, GT_FALSE))
    {
        st = cpssDxCh3PolicerMeteringEnableGet(dev, plrStage, &enable);
        UTF_VERIFY_EQUAL1_PARAM_MAC(GT_NOT_APPLICABLE_DEVICE, st, dev);
    }

    /* 3. Call function with out of bound value for device id.*/
    dev = PRV_CPSS_MAX_PP_DEVICES_CNS;

    st = cpssDxCh3PolicerMeteringEnableGet(dev, plrStage, &enable);
    UTF_VERIFY_EQUAL1_PARAM_MAC(GT_BAD_PARAM, st, dev);
}

/*----------------------------------------------------------------------------*/
/*
GT_STATUS cpssDxCh3PolicerBillingCountingEnableSet
(
    IN GT_U8    devNum,
    IN GT_BOOL  enable
)
*/
UTF_TEST_CASE_MAC(cpssDxCh3PolicerBillingCountingEnableSet)
{
/*
    ITERATE_DEVICES (DxCh3)
    1.1. Call function with enable [GT_FALSE / GT_TRUE]
    Expected: GT_OK.
    1.2. Call cpssDxCh3PolicerBillingCountingEnableGet with not NULL enablePtr.
    Expected: GT_OK and the same enable as was set.
*/
    GT_STATUS   st = GT_OK;
    GT_U8       dev;

    GT_BOOL     enable    = GT_FALSE;
    GT_BOOL     enableGet = GT_FALSE;


    /* prepare device iterator */
    PRV_TGF_NOT_APPLIC_DEV_RESET_MAC(&dev, UTF_CH1_E | UTF_CH1_DIAMOND_E | UTF_CH2_E | UTF_XCAT_E | UTF_LION_E | UTF_XCAT2_E);

    /* 1. Go over all active devices. */
    while (GT_OK == prvUtfNextDeviceGet(&dev, GT_TRUE))
    {
        /*
            1.1. Call function with enable [GT_FALSE / GT_TRUE]
            Expected: GT_OK.
        */
        /* iterate with enable = GT_FALSE */
        enable = GT_FALSE;

        st = cpssDxCh3PolicerBillingCountingEnableSet(dev, enable);
        UTF_VERIFY_EQUAL2_PARAM_MAC(GT_OK, st, dev, enable);

        /*
            1.2. Call cpssDxCh3PolicerBillingCountingEnableGet with not NULL enablePtr.
            Expected: GT_OK and the same enable as was set.
        */
        st = cpssDxCh3PolicerBillingCountingEnableGet(dev, &enableGet);
        UTF_VERIFY_EQUAL1_STRING_MAC(GT_OK, st,
                   "cpssDxCh3PolicerBillingCountingEnableSet: %d", dev);

        /* verifying values */
        UTF_VERIFY_EQUAL1_STRING_MAC(enable, enableGet,
                   "got another enable then was set: %d", dev);

        /* iterate with enable = GT_TRUE */
        enable = GT_TRUE;

        st = cpssDxCh3PolicerBillingCountingEnableSet(dev, enable);
        UTF_VERIFY_EQUAL2_PARAM_MAC(GT_OK, st, dev, enable);

        /*
            1.2. Call cpssDxCh3PolicerBillingCountingEnableGet with not NULL enablePtr.
            Expected: GT_OK and the same enable as was set.
        */
        st = cpssDxCh3PolicerBillingCountingEnableGet(dev, &enableGet);
        UTF_VERIFY_EQUAL1_STRING_MAC(GT_OK, st,
                   "cpssDxCh3PolicerBillingCountingEnableSet: %d", dev);

        /* verifying values */
        UTF_VERIFY_EQUAL1_STRING_MAC(enable, enableGet,
                   "got another enable then was set: %d", dev);
    }

    enable = GT_FALSE;

    /* 2. For not-active devices and devices from non-applicable family */
    /* check that function returns GT_BAD_PARAM.                        */

    /* prepare device iterator */
    PRV_TGF_NOT_APPLIC_DEV_RESET_MAC(&dev, UTF_CH1_E | UTF_CH1_DIAMOND_E | UTF_CH2_E | UTF_XCAT_E | UTF_LION_E | UTF_XCAT2_E);

    /* go over all non active devices */
    while(GT_OK == prvUtfNextDeviceGet(&dev, GT_FALSE))
    {
        st = cpssDxCh3PolicerBillingCountingEnableSet(dev, enable);
        UTF_VERIFY_EQUAL1_PARAM_MAC(GT_NOT_APPLICABLE_DEVICE, st, dev);
    }

    /* 3. Call function with out of bound value for device id.*/
    dev = PRV_CPSS_MAX_PP_DEVICES_CNS;

    st = cpssDxCh3PolicerBillingCountingEnableSet(dev, enable);
    UTF_VERIFY_EQUAL1_PARAM_MAC(GT_BAD_PARAM, st, dev);
}

/*----------------------------------------------------------------------------*/
/*
GT_STATUS cpssDxCh3PolicerBillingCountingEnableGet
(
    IN  GT_U8       devNum,
    OUT GT_BOOL     *enablePtr
)
*/
UTF_TEST_CASE_MAC(cpssDxCh3PolicerBillingCountingEnableGet)
{
/*
    ITERATE_DEVICES (DxCh3)
    1.1. Call function with not NULL enablePtr.
    Expected: GT_OK.
    1.2. Call function with enablePtr [NULL].
    Expected: GT_BAD_PTR.
*/
    GT_STATUS   st = GT_OK;
    GT_U8       dev;

    GT_BOOL     enable = GT_FALSE;


    /* prepare device iterator */
    PRV_TGF_NOT_APPLIC_DEV_RESET_MAC(&dev, UTF_CH1_E | UTF_CH1_DIAMOND_E | UTF_CH2_E | UTF_XCAT_E | UTF_LION_E | UTF_XCAT2_E);

    /* 1. Go over all active devices. */
    while (GT_OK == prvUtfNextDeviceGet(&dev, GT_TRUE))
    {
        /*
            1.1. Call function with not NULL enablePtr.
            Expected: GT_OK.
        */
        st = cpssDxCh3PolicerBillingCountingEnableGet(dev, &enable);
        UTF_VERIFY_EQUAL1_PARAM_MAC(GT_OK, st, dev);

        /*
            1.2. Call function with enablePtr [NULL].
            Expected: GT_BAD_PTR.
        */
        st = cpssDxCh3PolicerBillingCountingEnableGet(dev, NULL);
        UTF_VERIFY_EQUAL1_STRING_MAC(GT_BAD_PTR, st, "%d, enablePtr = NULL", dev);
    }

    /* 2. For not-active devices and devices from non-applicable family */
    /* check that function returns GT_BAD_PARAM.                        */

    /* prepare device iterator */
    PRV_TGF_NOT_APPLIC_DEV_RESET_MAC(&dev, UTF_CH1_E | UTF_CH1_DIAMOND_E | UTF_CH2_E | UTF_XCAT_E | UTF_LION_E | UTF_XCAT2_E);

    /* go over all non active devices */
    while(GT_OK == prvUtfNextDeviceGet(&dev, GT_FALSE))
    {
        st = cpssDxCh3PolicerBillingCountingEnableGet(dev, &enable);
        UTF_VERIFY_EQUAL1_PARAM_MAC(GT_NOT_APPLICABLE_DEVICE, st, dev);
    }

    /* 3. Call function with out of bound value for device id.*/
    dev = PRV_CPSS_MAX_PP_DEVICES_CNS;

    st = cpssDxCh3PolicerBillingCountingEnableGet(dev, &enable);
    UTF_VERIFY_EQUAL1_PARAM_MAC(GT_BAD_PARAM, st, dev);
}

/*----------------------------------------------------------------------------*/
/*
GT_STATUS cpssDxCh3PolicerBillingCountersModeSet
(
    IN GT_U8                                        devNum,
    IN CPSS_DXCH3_POLICER_BILLING_CNTR_MODE_ENT     cntrMode
)
*/
UTF_TEST_CASE_MAC(cpssDxCh3PolicerBillingCountersModeSet)
{
/*
    ITERATE_DEVICES (DxCh3)
    1.1. Call function with cntrMode [CPSS_DXCH3_POLICER_BILLING_CNTR_1_BYTE_E /
                                      CPSS_DXCH3_POLICER_BILLING_CNTR_16_BYTES_E /
                                      CPSS_DXCH3_POLICER_BILLING_CNTR_PACKET_E]
    Expected: GT_OK.
    1.2. Call cpssDxCh3PolicerBillingCountersModeGet with not NULL cntrModePtr.
    Expected: GT_OK and the same cntrMode as was set.
    1.3. Call function with out of range cntrMode [0x5AAAAA5]
    Expected: GT_BAD_PARAM.
*/
    GT_STATUS   st = GT_OK;
    GT_U8       dev;

    CPSS_DXCH3_POLICER_BILLING_CNTR_MODE_ENT    cntrMode    = CPSS_DXCH3_POLICER_BILLING_CNTR_1_BYTE_E;
    CPSS_DXCH3_POLICER_BILLING_CNTR_MODE_ENT    cntrModeGet = CPSS_DXCH3_POLICER_BILLING_CNTR_1_BYTE_E;


    /* prepare device iterator */
    PRV_TGF_NOT_APPLIC_DEV_RESET_MAC(&dev, UTF_CH1_E | UTF_CH1_DIAMOND_E | UTF_CH2_E | UTF_XCAT_E | UTF_LION_E | UTF_XCAT2_E);

    /* 1. Go over all active devices. */
    while (GT_OK == prvUtfNextDeviceGet(&dev, GT_TRUE))
    {
        /*
            1.1. Call function with cntrMode [CPSS_DXCH3_POLICER_BILLING_CNTR_1_BYTE_E /
                                              CPSS_DXCH3_POLICER_BILLING_CNTR_16_BYTES_E /
                                              CPSS_DXCH3_POLICER_BILLING_CNTR_PACKET_E]
            Expected: GT_OK.
        */

        /* iterate with cntrMode = CPSS_DXCH3_POLICER_BILLING_CNTR_1_BYTE_E */
        cntrMode = CPSS_DXCH3_POLICER_BILLING_CNTR_1_BYTE_E;

        st = cpssDxCh3PolicerBillingCountersModeSet(dev, cntrMode);
        UTF_VERIFY_EQUAL2_PARAM_MAC(GT_OK, st, dev, cntrMode);

        /*
            1.2. Call cpssDxCh3PolicerBillingCountersModeGet with not NULL cntrModePtr.
            Expected: GT_OK and the same cntrMode as was set.
        */
        st = cpssDxCh3PolicerBillingCountersModeGet(dev, &cntrModeGet);
        UTF_VERIFY_EQUAL1_STRING_MAC(GT_OK, st,
                   "cpssDxCh3PolicerBillingCountersModeGet: %d", dev);

        /* verifying values */
        UTF_VERIFY_EQUAL1_STRING_MAC(cntrMode, cntrModeGet,
                   "got another cntrMode then was set: %d", dev);

        /* iterate with cntrMode = CPSS_DXCH3_POLICER_BILLING_CNTR_16_BYTES_E */
        cntrMode = CPSS_DXCH3_POLICER_BILLING_CNTR_16_BYTES_E;

        st = cpssDxCh3PolicerBillingCountersModeSet(dev, cntrMode);
        UTF_VERIFY_EQUAL2_PARAM_MAC(GT_OK, st, dev, cntrMode);

        /*
            1.2. Call cpssDxCh3PolicerBillingCountersModeGet with not NULL cntrModePtr.
            Expected: GT_OK and the same cntrMode as was set.
        */
        st = cpssDxCh3PolicerBillingCountersModeGet(dev, &cntrModeGet);
        UTF_VERIFY_EQUAL1_STRING_MAC(GT_OK, st,
                   "cpssDxCh3PolicerBillingCountersModeGet: %d", dev);

        /* verifying values */
        UTF_VERIFY_EQUAL1_STRING_MAC(cntrMode, cntrModeGet,
                   "got another cntrMode then was set: %d", dev);

        /* iterate with cntrMode = CPSS_DXCH3_POLICER_BILLING_CNTR_PACKET_E */
        cntrMode = CPSS_DXCH3_POLICER_BILLING_CNTR_PACKET_E;

        st = cpssDxCh3PolicerBillingCountersModeSet(dev, cntrMode);
        UTF_VERIFY_EQUAL2_PARAM_MAC(GT_OK, st, dev, cntrMode);

        /*
            1.2. Call cpssDxCh3PolicerBillingCountersModeGet with not NULL cntrModePtr.
            Expected: GT_OK and the same cntrModePtr as was set.
        */
        st = cpssDxCh3PolicerBillingCountersModeGet(dev, &cntrModeGet);
        UTF_VERIFY_EQUAL1_STRING_MAC(GT_OK, st,
                   "cpssDxCh3PolicerBillingCountersModeGet: %d", dev);

        /* verifying values */
        UTF_VERIFY_EQUAL1_STRING_MAC(cntrMode, cntrModeGet,
                   "got another cntrMode then was set: %d", dev);

        /*
            1.3. Call function with out of range cntrMode [0x5AAAAA5]
            Expected: GT_BAD_PARAM.
        */
        cntrMode = POLICER_INVALID_ENUM_CNS;

        st = cpssDxCh3PolicerBillingCountersModeSet(dev, cntrMode);
        UTF_VERIFY_EQUAL2_PARAM_MAC(GT_BAD_PARAM, st, dev, cntrMode);
    }

    cntrMode = CPSS_DXCH3_POLICER_BILLING_CNTR_1_BYTE_E;

    /* 2. For not-active devices and devices from non-applicable family */
    /* check that function returns GT_BAD_PARAM.                        */

    /* prepare device iterator */
    PRV_TGF_NOT_APPLIC_DEV_RESET_MAC(&dev, UTF_CH1_E | UTF_CH1_DIAMOND_E | UTF_CH2_E | UTF_XCAT_E | UTF_LION_E | UTF_XCAT2_E);

    /* go over all non active devices */
    while(GT_OK == prvUtfNextDeviceGet(&dev, GT_FALSE))
    {
        st = cpssDxCh3PolicerBillingCountersModeSet(dev, cntrMode);
        UTF_VERIFY_EQUAL1_PARAM_MAC(GT_NOT_APPLICABLE_DEVICE, st, dev);
    }

    /* 3. Call function with out of bound value for device id.*/
    dev = PRV_CPSS_MAX_PP_DEVICES_CNS;

    st = cpssDxCh3PolicerBillingCountersModeSet(dev, cntrMode);
    UTF_VERIFY_EQUAL1_PARAM_MAC(GT_BAD_PARAM, st, dev);
}

/*----------------------------------------------------------------------------*/
/*
GT_STATUS cpssDxCh3PolicerBillingCountersModeGet
(
    IN GT_U8                                        devNum,
    OUT CPSS_DXCH3_POLICER_BILLING_CNTR_MODE_ENT    *cntrModePtr
)
*/
UTF_TEST_CASE_MAC(cpssDxCh3PolicerBillingCountersModeGet)
{
/*
    ITERATE_DEVICES (DxCh3)
    1.1. Call function with not NULL cntrModePtr.
    Expected: GT_OK.
    1.2. Call function with cntrModePtr [NULL].
    Expected: GT_BAD_PTR.
*/
    GT_STATUS   st = GT_OK;
    GT_U8       dev;

    CPSS_DXCH3_POLICER_BILLING_CNTR_MODE_ENT    cntrMode = CPSS_DXCH3_POLICER_BILLING_CNTR_1_BYTE_E;


    /* prepare device iterator */
    PRV_TGF_NOT_APPLIC_DEV_RESET_MAC(&dev, UTF_CH1_E | UTF_CH1_DIAMOND_E | UTF_CH2_E | UTF_XCAT_E | UTF_LION_E | UTF_XCAT2_E);

    /* 1. Go over all active devices. */
    while (GT_OK == prvUtfNextDeviceGet(&dev, GT_TRUE))
    {
        /*
            1.1. Call function with not NULL cntrModePtr.
            Expected: GT_OK.
        */
        st = cpssDxCh3PolicerBillingCountersModeGet(dev, &cntrMode);
        UTF_VERIFY_EQUAL1_PARAM_MAC(GT_OK, st, dev);

        /*
            1.2. Call function with cntrModePtr [NULL].
            Expected: GT_BAD_PTR.
        */
        st = cpssDxCh3PolicerBillingCountersModeGet(dev, NULL);
        UTF_VERIFY_EQUAL1_STRING_MAC(GT_BAD_PTR, st, "%d, cntrModePtr = NULL", dev);
    }

    /* 2. For not-active devices and devices from non-applicable family */
    /* check that function returns GT_BAD_PARAM.                        */

    /* prepare device iterator */
    PRV_TGF_NOT_APPLIC_DEV_RESET_MAC(&dev, UTF_CH1_E | UTF_CH1_DIAMOND_E | UTF_CH2_E | UTF_XCAT_E | UTF_LION_E | UTF_XCAT2_E);

    /* go over all non active devices */
    while(GT_OK == prvUtfNextDeviceGet(&dev, GT_FALSE))
    {
        st = cpssDxCh3PolicerBillingCountersModeGet(dev, &cntrMode);
        UTF_VERIFY_EQUAL1_PARAM_MAC(GT_NOT_APPLICABLE_DEVICE, st, dev);
    }

    /* 3. Call function with out of bound value for device id.*/
    dev = PRV_CPSS_MAX_PP_DEVICES_CNS;

    st = cpssDxCh3PolicerBillingCountersModeGet(dev, &cntrMode);
    UTF_VERIFY_EQUAL1_PARAM_MAC(GT_BAD_PARAM, st, dev);
}

/*----------------------------------------------------------------------------*/
/*
GT_STATUS cpssDxCh3PolicerPacketSizeModeSet
(
    IN GT_U8                                devNum,
    IN CPSS_POLICER_PACKET_SIZE_MODE_ENT    packetSize
)
*/
UTF_TEST_CASE_MAC(cpssDxCh3PolicerPacketSizeModeSet)
{
/*
    ITERATE_DEVICES (DxCh3)
    1.1. Call function with packetSize [CPSS_POLICER_PACKET_SIZE_L3_ONLY_E /
                                        CPSS_POLICER_PACKET_SIZE_L2_INCLUDE_E /
                                        CPSS_POLICER_PACKET_SIZE_L1_INCLUDE_E]
    Expected: GT_OK.
    1.2. Call cpssDxCh3PolicerPacketSizeModeGet with not NULL packetSizePtr.
    Expected: GT_OK and the same packetSize as was set.
    1.3. Call function with packetSize [CPSS_POLICER_PACKET_SIZE_TUNNEL_PASSENGER_E] (not supported)
    Expected: NOT GT_OK.
    1.4. Call function with out of range packetSize [0x5AAAAA5].
    Expected: GT_BAD_PARAM.
*/
    GT_STATUS   st = GT_OK;
    GT_U8       dev;

    CPSS_POLICER_PACKET_SIZE_MODE_ENT    packetSize    = CPSS_POLICER_PACKET_SIZE_TUNNEL_PASSENGER_E;
    CPSS_POLICER_PACKET_SIZE_MODE_ENT    packetSizeGet = CPSS_POLICER_PACKET_SIZE_TUNNEL_PASSENGER_E;


    /* prepare device iterator */
    PRV_TGF_NOT_APPLIC_DEV_RESET_MAC(&dev, UTF_CH1_E | UTF_CH1_DIAMOND_E | UTF_CH2_E);

    /* 1. Go over all active devices. */
    while (GT_OK == prvUtfNextDeviceGet(&dev, GT_TRUE))
    {
        /*
            1.1. Call function with packetSize [CPSS_POLICER_PACKET_SIZE_L3_ONLY_E /
                                                CPSS_POLICER_PACKET_SIZE_L2_INCLUDE_E /
                                                CPSS_POLICER_PACKET_SIZE_L1_INCLUDE_E]
            Expected: GT_OK.
        */
        /* iterate with packetSize = CPSS_POLICER_PACKET_SIZE_L3_ONLY_E */
        packetSize = CPSS_POLICER_PACKET_SIZE_L3_ONLY_E;

        st = cpssDxCh3PolicerPacketSizeModeSet(dev, plrStage, packetSize);
        UTF_VERIFY_EQUAL2_PARAM_MAC(GT_OK, st, dev, packetSize);

        /*
            1.2. Call cpssDxCh3PolicerPacketSizeModeGet with not NULL packetSizePtr.
            Expected: GT_OK and the same packetSize as was set.
        */
        st = cpssDxCh3PolicerPacketSizeModeGet(dev, plrStage, &packetSizeGet);
        UTF_VERIFY_EQUAL1_STRING_MAC(GT_OK, st,
                   "cpssDxCh3PolicerPacketSizeModeGet: %d", dev);

        /* verifying values */
        UTF_VERIFY_EQUAL1_STRING_MAC(packetSize, packetSizeGet,
                   "got another packetSize then was set: %d", dev);

        /* iterate with packetSize = CPSS_POLICER_PACKET_SIZE_L2_INCLUDE_E */
        packetSize = CPSS_POLICER_PACKET_SIZE_L2_INCLUDE_E;

        st = cpssDxCh3PolicerPacketSizeModeSet(dev, plrStage, packetSize);
        UTF_VERIFY_EQUAL2_PARAM_MAC(GT_OK, st, dev, packetSize);

        /*
            1.2. Call cpssDxCh3PolicerPacketSizeModeGet with not NULL packetSizePtr.
            Expected: GT_OK and the same packetSize as was set.
        */
        st = cpssDxCh3PolicerPacketSizeModeGet(dev, plrStage, &packetSizeGet);
        UTF_VERIFY_EQUAL1_STRING_MAC(GT_OK, st,
                   "cpssDxCh3PolicerPacketSizeModeGet: %d", dev);

        /* verifying values */
        UTF_VERIFY_EQUAL1_STRING_MAC(packetSize, packetSizeGet,
                   "got another packetSize then was set: %d", dev);

        /* iterate with packetSize = CPSS_POLICER_PACKET_SIZE_L1_INCLUDE_E */
        packetSize = CPSS_POLICER_PACKET_SIZE_L1_INCLUDE_E;

        st = cpssDxCh3PolicerPacketSizeModeSet(dev, plrStage, packetSize);
        UTF_VERIFY_EQUAL2_PARAM_MAC(GT_OK, st, dev, packetSize);

        /*
            1.2. Call cpssDxCh3PolicerPacketSizeModeGet with not NULL packetSizePtr.
            Expected: GT_OK and the same packetSize as was set.
        */
        st = cpssDxCh3PolicerPacketSizeModeGet(dev, plrStage, &packetSizeGet);
        UTF_VERIFY_EQUAL1_STRING_MAC(GT_OK, st,
                   "cpssDxCh3PolicerPacketSizeModeGet: %d", dev);

        /* verifying values */
        UTF_VERIFY_EQUAL1_STRING_MAC(packetSize, packetSizeGet,
                   "got another packetSize then was set: %d", dev);

        /*
            1.3. Call function with packetSize [CPSS_POLICER_PACKET_SIZE_TUNNEL_PASSENGER_E] (not supported)
            Expected: NOT GT_OK.
        */
        packetSize = CPSS_POLICER_PACKET_SIZE_TUNNEL_PASSENGER_E;

        st = cpssDxCh3PolicerPacketSizeModeSet(dev, plrStage, packetSize);
        UTF_VERIFY_NOT_EQUAL2_PARAM_MAC(GT_OK, st, dev, packetSize);

        /*
            1.4. Call function with out of range packetSize [0x5AAAAA5].
            Expected: GT_BAD_PARAM.
        */
        packetSize = POLICER_INVALID_ENUM_CNS;

        st = cpssDxCh3PolicerPacketSizeModeSet(dev, plrStage, packetSize);
        UTF_VERIFY_EQUAL2_PARAM_MAC(GT_BAD_PARAM, st, dev, packetSize);
    }

    packetSize = CPSS_POLICER_PACKET_SIZE_L1_INCLUDE_E;

    /* 2. For not-active devices and devices from non-applicable family */
    /* check that function returns GT_BAD_PARAM.                        */

    /* prepare device iterator */
    PRV_TGF_NOT_APPLIC_DEV_RESET_MAC(&dev, UTF_CH1_E | UTF_CH1_DIAMOND_E | UTF_CH2_E);

    /* go over all non active devices */
    while(GT_OK == prvUtfNextDeviceGet(&dev, GT_FALSE))
    {
        st = cpssDxCh3PolicerPacketSizeModeSet(dev, plrStage, packetSize);
        UTF_VERIFY_EQUAL1_PARAM_MAC(GT_NOT_APPLICABLE_DEVICE, st, dev);
    }

    /* 3. Call function with out of bound value for device id.*/
    dev = PRV_CPSS_MAX_PP_DEVICES_CNS;

    st = cpssDxCh3PolicerPacketSizeModeSet(dev, plrStage, packetSize);
    UTF_VERIFY_EQUAL1_PARAM_MAC(GT_BAD_PARAM, st, dev);
}

/*----------------------------------------------------------------------------*/
/*
GT_STATUS cpssDxCh3PolicerPacketSizeModeGet
(
    IN  GT_U8                                devNum,
    OUT CPSS_POLICER_PACKET_SIZE_MODE_ENT    *packetSizePtr
)
*/
UTF_TEST_CASE_MAC(cpssDxCh3PolicerPacketSizeModeGet)
{
/*
    ITERATE_DEVICES (DxCh3)
    1.1. Call function with not NULL packetSizePtr.
    Expected: GT_OK.
    1.2. Call function with packetSizePtr [NULL].
    Expected: GT_BAD_PTR.
*/
    GT_STATUS   st = GT_OK;
    GT_U8       dev;

    CPSS_POLICER_PACKET_SIZE_MODE_ENT   packetSize = CPSS_POLICER_PACKET_SIZE_TUNNEL_PASSENGER_E;;


    /* prepare device iterator */
    PRV_TGF_NOT_APPLIC_DEV_RESET_MAC(&dev, UTF_CH1_E | UTF_CH1_DIAMOND_E | UTF_CH2_E);

    /* 1. Go over all active devices. */
    while (GT_OK == prvUtfNextDeviceGet(&dev, GT_TRUE))
    {
        /*
            1.1. Call function with not NULL packetSizePtr.
            Expected: GT_OK.
        */
        st = cpssDxCh3PolicerPacketSizeModeGet(dev, plrStage, &packetSize);
        UTF_VERIFY_EQUAL1_PARAM_MAC(GT_OK, st, dev);

        /*
            1.2. Call function with packetSizePtr [NULL].
            Expected: GT_BAD_PTR.
        */
        st = cpssDxCh3PolicerPacketSizeModeGet(dev, plrStage, NULL);
        UTF_VERIFY_EQUAL1_STRING_MAC(GT_BAD_PTR, st, "%d, packetSizePtr = NULL", dev);
    }

    /* 2. For not-active devices and devices from non-applicable family */
    /* check that function returns GT_BAD_PARAM.                        */

    /* prepare device iterator */
    PRV_TGF_NOT_APPLIC_DEV_RESET_MAC(&dev, UTF_CH1_E | UTF_CH1_DIAMOND_E | UTF_CH2_E);

    /* go over all non active devices */
    while(GT_OK == prvUtfNextDeviceGet(&dev, GT_FALSE))
    {
        st = cpssDxCh3PolicerPacketSizeModeGet(dev, plrStage, &packetSize);
        UTF_VERIFY_EQUAL1_PARAM_MAC(GT_NOT_APPLICABLE_DEVICE, st, dev);
    }

    /* 3. Call function with out of bound value for device id.*/
    dev = PRV_CPSS_MAX_PP_DEVICES_CNS;

    st = cpssDxCh3PolicerPacketSizeModeGet(dev, plrStage, &packetSize);
    UTF_VERIFY_EQUAL1_PARAM_MAC(GT_BAD_PARAM, st, dev);
}

/*----------------------------------------------------------------------------*/
/*
GT_STATUS cpssDxCh3PolicerMeterResolutionSet
(
    IN GT_U8                                    devNum,
    IN CPSS_DXCH3_POLICER_METER_RESOLUTION_ENT  resolution
)
*/
UTF_TEST_CASE_MAC(cpssDxCh3PolicerMeterResolutionSet)
{
/*
    ITERATE_DEVICES (DxCh3)
    1.1. Call function with resolution [CPSS_DXCH3_POLICER_METER_RESOLUTION_BYTES_E /
                                        CPSS_DXCH3_POLICER_METER_RESOLUTION_PACKETS_E]
    Expected: GT_OK.
    1.2. Call cpssDxCh3PolicerMeterResolutionGet with not NULL resolutionPtr.
    Expected: GT_OK and the same resolution as was set.
    1.3. Call function with out of range resolution [0x5AAAAA5]
    Expected: GT_BAD_PARAM.
*/
    GT_STATUS   st = GT_OK;
    GT_U8       dev;

    CPSS_DXCH3_POLICER_METER_RESOLUTION_ENT  resolution    = CPSS_DXCH3_POLICER_METER_RESOLUTION_BYTES_E;
    CPSS_DXCH3_POLICER_METER_RESOLUTION_ENT  resolutionGet = CPSS_DXCH3_POLICER_METER_RESOLUTION_BYTES_E;


    /* prepare device iterator */
    PRV_TGF_NOT_APPLIC_DEV_RESET_MAC(&dev, UTF_CH1_E | UTF_CH1_DIAMOND_E | UTF_CH2_E);

    /* 1. Go over all active devices. */
    while (GT_OK == prvUtfNextDeviceGet(&dev, GT_TRUE))
    {
        /*
            1.1. Call function with resolution [CPSS_DXCH3_POLICER_METER_RESOLUTION_BYTES_E /
                                                CPSS_DXCH3_POLICER_METER_RESOLUTION_PACKETS_E]
            Expected: GT_OK.
        */
        /* iterate with resolution = CPSS_DXCH3_POLICER_METER_RESOLUTION_BYTES_E */
        resolution = CPSS_DXCH3_POLICER_METER_RESOLUTION_BYTES_E;

        st = cpssDxCh3PolicerMeterResolutionSet(dev, plrStage, resolution);
        UTF_VERIFY_EQUAL2_PARAM_MAC(GT_OK, st, dev, resolution);

        /*
            1.2. Call cpssDxCh3PolicerMeterResolutionGet with not NULL resolutionPtr.
            Expected: GT_OK and the same resolution as was set.
        */
        st = cpssDxCh3PolicerMeterResolutionGet(dev, plrStage, &resolutionGet);
        UTF_VERIFY_EQUAL1_STRING_MAC(GT_OK, st,
                   "cpssDxCh3PolicerMeterResolutionGet: %d", dev);

        /* verifying values */
        UTF_VERIFY_EQUAL1_STRING_MAC(resolution, resolutionGet,
                   "got another resolution then was set: %d", dev);

        /* iterate with resolution = CPSS_DXCH3_POLICER_METER_RESOLUTION_PACKETS_E */
        resolution = CPSS_DXCH3_POLICER_METER_RESOLUTION_PACKETS_E;

        st = cpssDxCh3PolicerMeterResolutionSet(dev, plrStage, resolution);
        UTF_VERIFY_EQUAL2_PARAM_MAC(GT_OK, st, dev, resolution);

        /*
            1.2. Call cpssDxCh3PolicerMeterResolutionGet with not NULL resolutionPtr.
            Expected: GT_OK and the same resolution as was set.
        */
        st = cpssDxCh3PolicerMeterResolutionGet(dev, plrStage, &resolutionGet);
        UTF_VERIFY_EQUAL1_STRING_MAC(GT_OK, st,
                   "cpssDxCh3PolicerMeterResolutionGet: %d", dev);

        /* verifying values */
        UTF_VERIFY_EQUAL1_STRING_MAC(resolution, resolutionGet,
                   "got another resolution then was set: %d", dev);

        /*
            1.3. Call function with out of range resolution [0x5AAAAA5]
            Expected: GT_BAD_PARAM.
        */
        resolution = POLICER_INVALID_ENUM_CNS;

        st = cpssDxCh3PolicerMeterResolutionSet(dev, plrStage, resolution);
        UTF_VERIFY_EQUAL2_PARAM_MAC(GT_BAD_PARAM, st, dev, resolution);
    }

    resolution = CPSS_DXCH3_POLICER_METER_RESOLUTION_BYTES_E;

    /* 2. For not-active devices and devices from non-applicable family */
    /* check that function returns GT_BAD_PARAM.                        */

    /* prepare device iterator */
    PRV_TGF_NOT_APPLIC_DEV_RESET_MAC(&dev, UTF_CH1_E | UTF_CH1_DIAMOND_E | UTF_CH2_E);

    /* go over all non active devices */
    while(GT_OK == prvUtfNextDeviceGet(&dev, GT_FALSE))
    {
        st = cpssDxCh3PolicerMeterResolutionSet(dev, plrStage, resolution);
        UTF_VERIFY_EQUAL1_PARAM_MAC(GT_NOT_APPLICABLE_DEVICE, st, dev);
    }

    /* 3. Call function with out of bound value for device id.*/
    dev = PRV_CPSS_MAX_PP_DEVICES_CNS;

    st = cpssDxCh3PolicerMeterResolutionSet(dev, plrStage, resolution);
    UTF_VERIFY_EQUAL1_PARAM_MAC(GT_BAD_PARAM, st, dev);
}

/*----------------------------------------------------------------------------*/
/*
GT_STATUS cpssDxCh3PolicerMeterResolutionGet
(
    IN GT_U8                                    devNum,
    OUT CPSS_DXCH3_POLICER_METER_RESOLUTION_ENT *resolutionPtr
)
*/
UTF_TEST_CASE_MAC(cpssDxCh3PolicerMeterResolutionGet)
{
/*
    ITERATE_DEVICES (DxCh3)
    1.1. Call function with not NULL resolutionPtr.
    Expected: GT_OK.
    1.2. Call function with resolutionPtr [NULL].
    Expected: GT_BAD_PTR.
*/
    GT_STATUS   st = GT_OK;
    GT_U8       dev;

    CPSS_DXCH3_POLICER_METER_RESOLUTION_ENT resolution = CPSS_DXCH3_POLICER_METER_RESOLUTION_BYTES_E;


    /* prepare device iterator */
    PRV_TGF_NOT_APPLIC_DEV_RESET_MAC(&dev, UTF_CH1_E | UTF_CH1_DIAMOND_E | UTF_CH2_E);

    /* 1. Go over all active devices. */
    while (GT_OK == prvUtfNextDeviceGet(&dev, GT_TRUE))
    {
        /*
            1.1. Call function with not NULL resolutionPtr.
            Expected: GT_OK.
        */
        st = cpssDxCh3PolicerMeterResolutionGet(dev, plrStage, &resolution);
        UTF_VERIFY_EQUAL1_PARAM_MAC(GT_OK, st, dev);

        /*
            1.2. Call function with resolutionPtr [NULL].
            Expected: GT_BAD_PTR.
        */
        st = cpssDxCh3PolicerMeterResolutionGet(dev, plrStage, NULL);
        UTF_VERIFY_EQUAL1_STRING_MAC(GT_BAD_PTR, st, "%d, resolutionPtr = NULL", dev);
    }

    /* 2. For not-active devices and devices from non-applicable family */
    /* check that function returns GT_BAD_PARAM.                        */

    /* prepare device iterator */
    PRV_TGF_NOT_APPLIC_DEV_RESET_MAC(&dev, UTF_CH1_E | UTF_CH1_DIAMOND_E | UTF_CH2_E);

    /* go over all non active devices */
    while(GT_OK == prvUtfNextDeviceGet(&dev, GT_FALSE))
    {
        st = cpssDxCh3PolicerMeterResolutionGet(dev, plrStage, &resolution);
        UTF_VERIFY_EQUAL1_PARAM_MAC(GT_NOT_APPLICABLE_DEVICE, st, dev);
    }

    /* 3. Call function with out of bound value for device id.*/
    dev = PRV_CPSS_MAX_PP_DEVICES_CNS;

    st = cpssDxCh3PolicerMeterResolutionGet(dev, plrStage, &resolution);
    UTF_VERIFY_EQUAL1_PARAM_MAC(GT_BAD_PARAM, st, dev);
}

/*----------------------------------------------------------------------------*/
/*
GT_STATUS cpssDxCh3PolicerDropTypeSet
(
    IN GT_U8                    devNum,
    IN CPSS_DROP_MODE_TYPE_ENT  dropType
)
*/
UTF_TEST_CASE_MAC(cpssDxCh3PolicerDropTypeSet)
{
/*
    ITERATE_DEVICES (DxCh3)
    1.1. Call function with dropType [CPSS_DROP_MODE_SOFT_E /
                                      CPSS_DROP_MODE_HARD_E]
    Expected: GT_OK.
    1.2. Call cpssDxCh3PolicerDropTypeGet with not NULL dropTypePtr.
    Expected: GT_OK and the same dropTypePtr as was set.
    1.3. Call function with out of range dropType [0x5AAAAA5]
    Expected: GT_BAD_PARAM.
*/
    GT_STATUS   st = GT_OK;
    GT_U8       dev;

    CPSS_DROP_MODE_TYPE_ENT  dropType    = CPSS_DROP_MODE_SOFT_E;
    CPSS_DROP_MODE_TYPE_ENT  dropTypeGet = CPSS_DROP_MODE_SOFT_E;


    /* prepare device iterator */
    PRV_TGF_NOT_APPLIC_DEV_RESET_MAC(&dev, UTF_CH1_E | UTF_CH1_DIAMOND_E | UTF_CH2_E);

    /* 1. Go over all active devices. */
    while (GT_OK == prvUtfNextDeviceGet(&dev, GT_TRUE))
    {
        /*
            1.1. Call function with dropType [CPSS_DROP_MODE_SOFT_E /
                                              CPSS_DROP_MODE_HARD_E]
            Expected: GT_OK.
        */
        /* iterate with dropType = CPSS_DROP_MODE_SOFT_E */
        dropType = CPSS_DROP_MODE_SOFT_E;

        st = cpssDxCh3PolicerDropTypeSet(dev, plrStage, dropType);
        UTF_VERIFY_EQUAL2_PARAM_MAC(GT_OK, st, dev, dropType);

        /*
            1.2. Call cpssDxCh3PolicerDropTypeGet with not NULL dropTypePtr.
            Expected: GT_OK and the same dropType as was set.
        */
        st = cpssDxCh3PolicerDropTypeGet(dev, plrStage, &dropTypeGet);
        UTF_VERIFY_EQUAL1_STRING_MAC(GT_OK, st,
                   "cpssDxCh3PolicerDropTypeGet: %d", dev);

        /* verifying values */
        UTF_VERIFY_EQUAL1_STRING_MAC(dropType, dropTypeGet,
                   "got another dropType then was set: %d", dev);

        /* iterate with dropType = CPSS_DROP_MODE_HARD_E */
        dropType = CPSS_DROP_MODE_HARD_E;

        st = cpssDxCh3PolicerDropTypeSet(dev, plrStage, dropType);
        UTF_VERIFY_EQUAL2_PARAM_MAC(GT_OK, st, dev, dropType);

        /*
            1.2. Call cpssDxCh3PolicerDropTypeGet with not NULL dropTypePtr.
            Expected: GT_OK and the same dropType as was set.
        */
        st = cpssDxCh3PolicerDropTypeGet(dev, plrStage, &dropTypeGet);
        UTF_VERIFY_EQUAL1_STRING_MAC(GT_OK, st,
                   "cpssDxCh3PolicerDropTypeGet: %d", dev);

        /* verifying values */
        UTF_VERIFY_EQUAL1_STRING_MAC(dropType, dropTypeGet,
                   "got another dropType then was set: %d", dev);

        /*
            1.3. Call function with out of range dropType [0x5AAAAA5]
            Expected: GT_BAD_PARAM.
        */
        dropType = POLICER_INVALID_ENUM_CNS;

        st = cpssDxCh3PolicerDropTypeSet(dev, plrStage, dropType);
        UTF_VERIFY_EQUAL2_PARAM_MAC(GT_BAD_PARAM, st, dev, dropType);
    }

    dropType = CPSS_DROP_MODE_SOFT_E;

    /* 2. For not-active devices and devices from non-applicable family */
    /* check that function returns GT_BAD_PARAM.                        */

    /* prepare device iterator */
    PRV_TGF_NOT_APPLIC_DEV_RESET_MAC(&dev, UTF_CH1_E | UTF_CH1_DIAMOND_E | UTF_CH2_E);

    /* go over all non active devices */
    while(GT_OK == prvUtfNextDeviceGet(&dev, GT_FALSE))
    {
        st = cpssDxCh3PolicerDropTypeSet(dev, plrStage, dropType);
        UTF_VERIFY_EQUAL1_PARAM_MAC(GT_NOT_APPLICABLE_DEVICE, st, dev);
    }

    /* 3. Call function with out of bound value for device id.*/
    dev = PRV_CPSS_MAX_PP_DEVICES_CNS;

    st = cpssDxCh3PolicerDropTypeSet(dev, plrStage, dropType);
    UTF_VERIFY_EQUAL1_PARAM_MAC(GT_BAD_PARAM, st, dev);
}

/*----------------------------------------------------------------------------*/
/*
GT_STATUS cpssDxCh3PolicerDropTypeGet
(
    IN  GT_U8                      devNum,
    OUT CPSS_DROP_MODE_TYPE_ENT    *dropTypePtr
)
*/
UTF_TEST_CASE_MAC(cpssDxCh3PolicerDropTypeGet)
{
/*
    ITERATE_DEVICES (DxCh3)
    1.1. Call function with not NULL dropTypePtr.
    Expected: GT_OK.
    1.2. Call function with dropTypePtr [NULL].
    Expected: GT_BAD_PTR.
*/
    GT_STATUS   st = GT_OK;
    GT_U8       dev;

    CPSS_DROP_MODE_TYPE_ENT dropType = CPSS_DROP_MODE_SOFT_E;


    /* prepare device iterator */
    PRV_TGF_NOT_APPLIC_DEV_RESET_MAC(&dev, UTF_CH1_E | UTF_CH1_DIAMOND_E | UTF_CH2_E);

    /* 1. Go over all active devices. */
    while (GT_OK == prvUtfNextDeviceGet(&dev, GT_TRUE))
    {
        /*
            1.1. Call function with not NULL dropTypePtr.
            Expected: GT_OK.
        */
        st = cpssDxCh3PolicerDropTypeGet(dev, plrStage, &dropType);
        UTF_VERIFY_EQUAL1_PARAM_MAC(GT_OK, st, dev);

        /*
            1.2. Call function with dropTypePtr [NULL].
            Expected: GT_BAD_PTR.
        */
        st = cpssDxCh3PolicerDropTypeGet(dev, plrStage, NULL);
        UTF_VERIFY_EQUAL1_STRING_MAC(GT_BAD_PTR, st, "%d, dropTypePtr = NULL", dev);
    }

    /* 2. For not-active devices and devices from non-applicable family */
    /* check that function returns GT_BAD_PARAM.                        */

    /* prepare device iterator */
    PRV_TGF_NOT_APPLIC_DEV_RESET_MAC(&dev, UTF_CH1_E | UTF_CH1_DIAMOND_E | UTF_CH2_E);

    /* go over all non active devices */
    while(GT_OK == prvUtfNextDeviceGet(&dev, GT_FALSE))
    {
        st = cpssDxCh3PolicerDropTypeGet(dev, plrStage, &dropType);
        UTF_VERIFY_EQUAL1_PARAM_MAC(GT_NOT_APPLICABLE_DEVICE, st, dev);
    }

    /* 3. Call function with out of bound value for device id.*/
    dev = PRV_CPSS_MAX_PP_DEVICES_CNS;

    st = cpssDxCh3PolicerDropTypeGet(dev, plrStage, &dropType);
    UTF_VERIFY_EQUAL1_PARAM_MAC(GT_BAD_PARAM, st, dev);
}

/*----------------------------------------------------------------------------*/
/*
GT_STATUS cpssDxCh3PolicerCountingColorModeSet
(
    IN GT_U8                                    devNum,
    IN CPSS_DXCH3_POLICER_COLOR_COUNT_MODE_ENT  mode
)
*/
UTF_TEST_CASE_MAC(cpssDxCh3PolicerCountingColorModeSet)
{
/*
    ITERATE_DEVICES (DxCh3)
    1.1. Call function with mode [CPSS_DXCH3_POLICER_COLOR_COUNT_CL_E /
                                  CPSS_DXCH3_POLICER_COLOR_COUNT_DP_E]
    Expected: GT_OK.
    1.2. Call cpssDxCh3PolicerCountingColorModeGet with not NULL modePtr.
    Expected: GT_OK and the same mode as was set.
    1.3. Call function with out of range mode [0x5AAAAA5].
    Expected: GT_BAD_PARAM.
*/
    GT_STATUS   st = GT_OK;
    GT_U8       dev;

    CPSS_DXCH3_POLICER_COLOR_COUNT_MODE_ENT  mode    = CPSS_DXCH3_POLICER_COLOR_COUNT_CL_E;
    CPSS_DXCH3_POLICER_COLOR_COUNT_MODE_ENT  modeGet = CPSS_DXCH3_POLICER_COLOR_COUNT_CL_E;


    /* prepare device iterator */
    PRV_TGF_NOT_APPLIC_DEV_RESET_MAC(&dev, UTF_CH1_E | UTF_CH1_DIAMOND_E | UTF_CH2_E);

    /* 1. Go over all active devices. */
    while (GT_OK == prvUtfNextDeviceGet(&dev, GT_TRUE))
    {
        /*
            1.1. Call function with mode [CPSS_DXCH3_POLICER_COLOR_COUNT_CL_E /
                                          CPSS_DXCH3_POLICER_COLOR_COUNT_DP_E]
            Expected: GT_OK.
        */
        /* iterate with mode = CPSS_DXCH3_POLICER_COLOR_COUNT_CL_E */
        mode = CPSS_DXCH3_POLICER_COLOR_COUNT_CL_E;

        st = cpssDxCh3PolicerCountingColorModeSet(dev, plrStage, mode);
        UTF_VERIFY_EQUAL2_PARAM_MAC(GT_OK, st, dev, mode);

        /*
            1.2. Call cpssDxCh3PolicerCountingColorModeGet with not NULL modePtr.
            Expected: GT_OK and the same mode as was set.
        */
        st = cpssDxCh3PolicerCountingColorModeGet(dev, plrStage, &modeGet);
        UTF_VERIFY_EQUAL1_STRING_MAC(GT_OK, st,
                   "cpssDxCh3PolicerCountingColorModeGet: %d", dev);

        /* verifying values */
        UTF_VERIFY_EQUAL1_STRING_MAC(mode, modeGet,
                   "got another mode then was set: %d", dev);

        /* iterate with mode = CPSS_DXCH3_POLICER_COLOR_COUNT_DP_E */
        mode = CPSS_DXCH3_POLICER_COLOR_COUNT_DP_E;

        st = cpssDxCh3PolicerCountingColorModeSet(dev, plrStage, mode);
        UTF_VERIFY_EQUAL2_PARAM_MAC(GT_OK, st, dev, mode);

        /*
            1.2. Call cpssDxCh3PolicerCountingColorModeGet with not NULL modePtr.
            Expected: GT_OK and the same mode as was set.
        */
        st = cpssDxCh3PolicerCountingColorModeGet(dev, plrStage, &modeGet);
        UTF_VERIFY_EQUAL1_STRING_MAC(GT_OK, st,
                   "cpssDxCh3PolicerCountingColorModeGet: %d", dev);

        /* verifying values */
        UTF_VERIFY_EQUAL1_STRING_MAC(mode, modeGet,
                   "got another mode then was set: %d", dev);

        /*
            1.3. Call function with out of range mode [0x5AAAAA5].
            Expected: GT_BAD_PARAM.
        */
        mode = POLICER_INVALID_ENUM_CNS;

        st = cpssDxCh3PolicerCountingColorModeSet(dev, plrStage, mode);
        UTF_VERIFY_EQUAL2_PARAM_MAC(GT_BAD_PARAM, st, dev, mode);
    }

    mode = CPSS_DXCH3_POLICER_COLOR_COUNT_CL_E;

    /* 2. For not-active devices and devices from non-applicable family */
    /* check that function returns GT_BAD_PARAM.                        */

    /* prepare device iterator */
    PRV_TGF_NOT_APPLIC_DEV_RESET_MAC(&dev, UTF_CH1_E | UTF_CH1_DIAMOND_E | UTF_CH2_E);

    /* go over all non active devices */
    while(GT_OK == prvUtfNextDeviceGet(&dev, GT_FALSE))
    {
        st = cpssDxCh3PolicerCountingColorModeSet(dev, plrStage, mode);
        UTF_VERIFY_EQUAL1_PARAM_MAC(GT_NOT_APPLICABLE_DEVICE, st, dev);
    }

    /* 3. Call function with out of bound value for device id.*/
    dev = PRV_CPSS_MAX_PP_DEVICES_CNS;

    st = cpssDxCh3PolicerCountingColorModeSet(dev, plrStage, mode);
    UTF_VERIFY_EQUAL1_PARAM_MAC(GT_BAD_PARAM, st, dev);
}

/*----------------------------------------------------------------------------*/
/*
GT_STATUS cpssDxCh3PolicerCountingColorModeGet
(
    IN  GT_U8                                       devNum,
    OUT CPSS_DXCH3_POLICER_COLOR_COUNT_MODE_ENT     *modePtr
)
*/
UTF_TEST_CASE_MAC(cpssDxCh3PolicerCountingColorModeGet)
{
/*
    ITERATE_DEVICES (DxCh3)
    1.1. Call function with not NULL modePtr.
    Expected: GT_OK.
    1.2. Call function with modePtr [NULL].
    Expected: GT_BAD_PTR.
*/
    GT_STATUS   st = GT_OK;
    GT_U8       dev;

    CPSS_DXCH3_POLICER_COLOR_COUNT_MODE_ENT mode = CPSS_DXCH3_POLICER_COLOR_COUNT_CL_E;


    /* prepare device iterator */
    PRV_TGF_NOT_APPLIC_DEV_RESET_MAC(&dev, UTF_CH1_E | UTF_CH1_DIAMOND_E | UTF_CH2_E);

    /* 1. Go over all active devices. */
    while (GT_OK == prvUtfNextDeviceGet(&dev, GT_TRUE))
    {
        /*
            1.1. Call function with not NULL modePtr.
            Expected: GT_OK.
        */
        st = cpssDxCh3PolicerCountingColorModeGet(dev, plrStage, &mode);
        UTF_VERIFY_EQUAL1_PARAM_MAC(GT_OK, st, dev);

        /*
            1.2. Call function with modePtr [NULL].
            Expected: GT_BAD_PTR.
        */
        st = cpssDxCh3PolicerCountingColorModeGet(dev, plrStage, NULL);
        UTF_VERIFY_EQUAL1_STRING_MAC(GT_BAD_PTR, st, "%d, modePtr = NULL", dev);
    }

    /* 2. For not-active devices and devices from non-applicable family */
    /* check that function returns GT_BAD_PARAM.                        */

    /* prepare device iterator */
    PRV_TGF_NOT_APPLIC_DEV_RESET_MAC(&dev, UTF_CH1_E | UTF_CH1_DIAMOND_E | UTF_CH2_E);

    /* go over all non active devices */
    while(GT_OK == prvUtfNextDeviceGet(&dev, GT_FALSE))
    {
        st = cpssDxCh3PolicerCountingColorModeGet(dev, plrStage, &mode);
        UTF_VERIFY_EQUAL1_PARAM_MAC(GT_NOT_APPLICABLE_DEVICE, st, dev);
    }

    /* 3. Call function with out of bound value for device id.*/
    dev = PRV_CPSS_MAX_PP_DEVICES_CNS;

    st = cpssDxCh3PolicerCountingColorModeGet(dev, plrStage, &mode);
    UTF_VERIFY_EQUAL1_PARAM_MAC(GT_BAD_PARAM, st, dev);
}

/*----------------------------------------------------------------------------*/
/*
GT_STATUS cpssDxCh3PolicerManagementCntrsResolutionSet
(
    IN GT_U8                                        devNum,
    IN CPSS_DXCH3_POLICER_MNG_CNTR_SET_ENT          cntrSet,
    IN CPSS_DXCH3_POLICER_MNG_CNTR_RESOLUTION_ENT   cntrResolution
)
*/
UTF_TEST_CASE_MAC(cpssDxCh3PolicerManagementCntrsResolutionSet)
{
/*
    ITERATE_DEVICES (DxCh3)
    1.1. Call function with cntrSet [CPSS_DXCH3_POLICER_MNG_CNTR_SET0_E /
                                     CPSS_DXCH3_POLICER_MNG_CNTR_SET2_E],
                            cntrResolution [CPSS_DXCH3_POLICER_MNG_CNTR_RESOLUTION_1B_E /
                                            CPSS_DXCH3_POLICER_MNG_CNTR_RESOLUTION_16B_E]
    Expected: GT_OK.
    1.2. Call cpssDxCh3PolicerManagementCntrsResolutionGet with not NULL cntrResolutionPtr
                                                           and other param from 1.1.
    Expected: GT_OK and the same cntrResolution as was set.
    1.3. Call function with out of range cntrSet [0x5AAAAA5]
                            and other param from 1.1.
    Expected: GT_BAD_PARAM.
    1.4. Call function with out of range cntrResolution [0x5AAAAAA5]
                            and other param from 1.1.
    Expected: GT_BAD_PARAM.
*/
    GT_STATUS   st = GT_OK;
    GT_U8       dev;

    CPSS_DXCH3_POLICER_MNG_CNTR_SET_ENT          cntrSet           = CPSS_DXCH3_POLICER_MNG_CNTR_SET0_E;
    CPSS_DXCH3_POLICER_MNG_CNTR_RESOLUTION_ENT   cntrResolution    = CPSS_DXCH3_POLICER_MNG_CNTR_RESOLUTION_1B_E;
    CPSS_DXCH3_POLICER_MNG_CNTR_RESOLUTION_ENT   cntrResolutionGet = CPSS_DXCH3_POLICER_MNG_CNTR_RESOLUTION_1B_E;


    /* prepare device iterator */
    PRV_TGF_NOT_APPLIC_DEV_RESET_MAC(&dev, UTF_CH1_E | UTF_CH1_DIAMOND_E | UTF_CH2_E);

    /* 1. Go over all active devices. */
    while (GT_OK == prvUtfNextDeviceGet(&dev, GT_TRUE))
    {
        /*
            1.1. Call function with cntrSet [CPSS_DXCH3_POLICER_MNG_CNTR_SET0_E /
                                             CPSS_DXCH3_POLICER_MNG_CNTR_SET2_E],
                                    cntrResolution [CPSS_DXCH3_POLICER_MNG_CNTR_RESOLUTION_1B_E /
                                                    CPSS_DXCH3_POLICER_MNG_CNTR_RESOLUTION_16B_E]
            Expected: GT_OK.
        */
        /* iterate with cntrSet = CPSS_DXCH3_POLICER_MNG_CNTR_SET0_E */
        cntrSet        = CPSS_DXCH3_POLICER_MNG_CNTR_SET0_E;
        cntrResolution = CPSS_DXCH3_POLICER_MNG_CNTR_RESOLUTION_1B_E;

        st = cpssDxCh3PolicerManagementCntrsResolutionSet(dev, plrStage, cntrSet,
                                                          cntrResolution);
        UTF_VERIFY_EQUAL3_PARAM_MAC(GT_OK, st, dev, cntrSet, cntrResolution);

        /*
            1.2. Call cpssDxCh3PolicerManagementCntrsResolutionGet with not NULL cntrResolutionPtr
                                                                   and other param from 1.1.
            Expected: GT_OK and the same cntrResolution as was set.
        */
        st = cpssDxCh3PolicerManagementCntrsResolutionGet(dev, plrStage, cntrSet,
                                                          &cntrResolutionGet);
        UTF_VERIFY_EQUAL2_STRING_MAC(GT_OK, st,
                   "cpssDxCh3PolicerManagementCntrsResolutionGet: %d, %d", dev, cntrSet);

        /* verifying values */
        UTF_VERIFY_EQUAL1_STRING_MAC(cntrResolution, cntrResolutionGet,
                   "got another cntrResolution then was set: %d", dev);

        /* iterate with cntrSet = CPSS_DXCH3_POLICER_MNG_CNTR_SET2_E */
        cntrSet        = CPSS_DXCH3_POLICER_MNG_CNTR_SET2_E;
        cntrResolution = CPSS_DXCH3_POLICER_MNG_CNTR_RESOLUTION_16B_E;

        st = cpssDxCh3PolicerManagementCntrsResolutionSet(dev, plrStage, cntrSet,
                                                          cntrResolution);
        UTF_VERIFY_EQUAL3_PARAM_MAC(GT_OK, st, dev, cntrSet, cntrResolution);

        /*
            1.2. Call cpssDxCh3PolicerManagementCntrsResolutionGet with not NULL cntrResolutionPtr
                                                                   and other param from 1.1.
            Expected: GT_OK and the same cntrResolution as was set.
        */
        st = cpssDxCh3PolicerManagementCntrsResolutionGet(dev, plrStage, cntrSet,
                                                          &cntrResolutionGet);
        UTF_VERIFY_EQUAL2_STRING_MAC(GT_OK, st,
                   "cpssDxCh3PolicerManagementCntrsResolutionGet: %d, %d", dev, cntrSet);

        /* verifying values */
        UTF_VERIFY_EQUAL1_STRING_MAC(cntrResolution, cntrResolutionGet,
                   "got another cntrResolution then was set: %d", dev);

        /*
            1.3. Call function with out of range cntrSet [0x5AAAAA5]
                                    and other param from 1.1.
            Expected: GT_BAD_PARAM.
        */
        cntrSet = POLICER_INVALID_ENUM_CNS;

        st = cpssDxCh3PolicerManagementCntrsResolutionSet(dev, plrStage, cntrSet,
                                                          cntrResolution);
        UTF_VERIFY_EQUAL2_PARAM_MAC(GT_BAD_PARAM, st, dev, cntrSet);

        cntrSet = CPSS_DXCH3_POLICER_MNG_CNTR_DISABLED_E;

        /*
            1.4. Call function with out of range cntrResolution [0x5AAAAA5]
                                    and other param from 1.1.
            Expected: GT_BAD_PARAM.
        */
        cntrResolution = POLICER_INVALID_ENUM_CNS;

        st = cpssDxCh3PolicerManagementCntrsResolutionSet(dev, plrStage, cntrSet,
                                                          cntrResolution);
        UTF_VERIFY_EQUAL2_STRING_MAC(GT_BAD_PARAM, st, "%d, cntrResolution = %d", dev, cntrResolution);
    }

    cntrSet        = CPSS_DXCH3_POLICER_MNG_CNTR_SET0_E;
    cntrResolution = CPSS_DXCH3_POLICER_MNG_CNTR_RESOLUTION_1B_E;

    /* 2. For not-active devices and devices from non-applicable family */
    /* check that function returns GT_BAD_PARAM.                        */

    /* prepare device iterator */
    PRV_TGF_NOT_APPLIC_DEV_RESET_MAC(&dev, UTF_CH1_E | UTF_CH1_DIAMOND_E | UTF_CH2_E);

    /* go over all non active devices */
    while(GT_OK == prvUtfNextDeviceGet(&dev, GT_FALSE))
    {
        st = cpssDxCh3PolicerManagementCntrsResolutionSet(dev, plrStage, cntrSet,
                                                          cntrResolution);
        UTF_VERIFY_EQUAL1_PARAM_MAC(GT_NOT_APPLICABLE_DEVICE, st, dev);
    }

    /* 3. Call function with out of bound value for device id.*/
    dev = PRV_CPSS_MAX_PP_DEVICES_CNS;

    st = cpssDxCh3PolicerManagementCntrsResolutionSet(dev, plrStage, cntrSet, cntrResolution);
    UTF_VERIFY_EQUAL1_PARAM_MAC(GT_BAD_PARAM, st, dev);
}

/*----------------------------------------------------------------------------*/
/*
GT_STATUS cpssDxCh3PolicerManagementCntrsResolutionGet
(
    IN  GT_U8                                       devNum,
    IN  CPSS_DXCH3_POLICER_MNG_CNTR_SET_ENT         cntrSet,
    OUT CPSS_DXCH3_POLICER_MNG_CNTR_RESOLUTION_ENT  *cntrResolutionPtr
)
*/
UTF_TEST_CASE_MAC(cpssDxCh3PolicerManagementCntrsResolutionGet)
{
/*
    ITERATE_DEVICES (DxCh3)
    1.1. Call function with cntrSet [CPSS_DXCH3_POLICER_MNG_CNTR_SET0_E /
                                     CPSS_DXCH3_POLICER_MNG_CNTR_SET2_E]
                            and not NULL cntrResolutionPtr.
    Expected: GT_OK.
    1.2. Call function with out of range cntrSet [0x5AAAAA5]
                            and other param from 1.1.
    Expected: GT_BAD_PARAM.
    1.3. Call function with cntrResolutionPtr [NULL]
                            and other param from 1.1.
    Expected: GT_BAD_PTR.
*/
    GT_STATUS   st  = GT_OK;
    GT_U8       dev;

    CPSS_DXCH3_POLICER_MNG_CNTR_SET_ENT         cntrSet        = CPSS_DXCH3_POLICER_MNG_CNTR_SET0_E;
    CPSS_DXCH3_POLICER_MNG_CNTR_RESOLUTION_ENT  cntrResolution = CPSS_DXCH3_POLICER_MNG_CNTR_RESOLUTION_1B_E;


    /* prepare device iterator */
    PRV_TGF_NOT_APPLIC_DEV_RESET_MAC(&dev, UTF_CH1_E | UTF_CH1_DIAMOND_E | UTF_CH2_E);

    /* 1. Go over all active devices. */
    while (GT_OK == prvUtfNextDeviceGet(&dev, GT_TRUE))
    {
        /*
            1.1. Call function with cntrSet [CPSS_DXCH3_POLICER_MNG_CNTR_SET0_E /
                                             CPSS_DXCH3_POLICER_MNG_CNTR_SET2_E]
                                    and not NULL cntrResolutionPtr.
            Expected: GT_OK.
        */
        /* iterate with cntrSet = CPSS_DXCH3_POLICER_MNG_CNTR_SET0_E */
        cntrSet = CPSS_DXCH3_POLICER_MNG_CNTR_SET0_E;

        st = cpssDxCh3PolicerManagementCntrsResolutionGet(dev, plrStage, cntrSet,
                                                          &cntrResolution);
        UTF_VERIFY_EQUAL2_PARAM_MAC(GT_OK, st, dev, cntrSet);

        /* iterate with cntrSet = CPSS_DXCH3_POLICER_MNG_CNTR_SET2_E */
        cntrSet = CPSS_DXCH3_POLICER_MNG_CNTR_SET2_E;

        st = cpssDxCh3PolicerManagementCntrsResolutionGet(dev, plrStage, cntrSet, &cntrResolution);
        UTF_VERIFY_EQUAL2_PARAM_MAC(GT_OK, st, dev, cntrSet);

        /*
            1.2. Call function with out of range cntrSet [0x5AAAAA5]
                                    and other param from 1.1.
            Expected: GT_BAD_PARAM.
        */
        cntrSet = POLICER_INVALID_ENUM_CNS;

        st = cpssDxCh3PolicerManagementCntrsResolutionGet(dev, plrStage, cntrSet,
                                                          &cntrResolution);
        UTF_VERIFY_EQUAL2_PARAM_MAC(GT_BAD_PARAM, st, dev, cntrSet);

        cntrSet = CPSS_DXCH3_POLICER_MNG_CNTR_SET0_E;

        /*
            1.3. Call function with cntrResolutionPtr [NULL]
                                    and other param from 1.1.
            Expected: GT_BAD_PTR.
        */
        st = cpssDxCh3PolicerManagementCntrsResolutionGet(dev, plrStage, cntrSet, NULL);
        UTF_VERIFY_EQUAL1_STRING_MAC(GT_BAD_PTR, st, "%d, cntrResolution = NULL", dev);
    }

    cntrSet = CPSS_DXCH3_POLICER_MNG_CNTR_SET0_E;

    /* 2. For not-active devices and devices from non-applicable family */
    /* check that function returns GT_BAD_PARAM.                        */

    /* prepare device iterator */
    PRV_TGF_NOT_APPLIC_DEV_RESET_MAC(&dev, UTF_CH1_E | UTF_CH1_DIAMOND_E | UTF_CH2_E);

    /* go over all non active devices */
    while(GT_OK == prvUtfNextDeviceGet(&dev, GT_FALSE))
    {
        st = cpssDxCh3PolicerManagementCntrsResolutionGet(dev, plrStage, cntrSet,
                                                          &cntrResolution);
        UTF_VERIFY_EQUAL1_PARAM_MAC(GT_NOT_APPLICABLE_DEVICE, st, dev);
    }

    /* 3. Call function with out of bound value for device id.*/
    dev = PRV_CPSS_MAX_PP_DEVICES_CNS;

    st = cpssDxCh3PolicerManagementCntrsResolutionGet(dev, plrStage, cntrSet,
                                                      &cntrResolution);
    UTF_VERIFY_EQUAL1_PARAM_MAC(GT_BAD_PARAM, st, dev);
}

/*----------------------------------------------------------------------------*/
/*
GT_STATUS cpssDxCh3PolicerPacketSizeModeForTunnelTermSet
(
    IN  GT_U8                                       devNum,
    IN  CPSS_DXCH3_POLICER_TT_PACKET_SIZE_MODE_ENT  ttPacketSizeMode
)
*/
UTF_TEST_CASE_MAC(cpssDxCh3PolicerPacketSizeModeForTunnelTermSet)
{
/*
    ITERATE_DEVICES (DxCh3)
    1.1. Call function with ttPacketSizeMode [CPSS_DXCH3_POLICER_TT_PACKET_SIZE_REGULAR_ENT /
                                              CPSS_DXCH3_POLICER_TT_PACKET_SIZE_PASSENGER_ENT]
    Expected: GT_OK.
    1.2. Call cpssDxCh3PolicerPacketSizeModeForTunnelTermGet with not NULL ttPacketSizeModePtr.
    Expected: GT_OK and the same ttPacketSizeMode as was set.
    1.3. Call function with out of range ttPacketSizeMode [0x5AAAAA5].
    Expected: GT_BAD_PARAM.
*/
    GT_STATUS   st = GT_OK;
    GT_U8       dev;

    CPSS_DXCH3_POLICER_TT_PACKET_SIZE_MODE_ENT  ttPacketSizeMode    = CPSS_DXCH3_POLICER_TT_PACKET_SIZE_REGULAR_ENT;
    CPSS_DXCH3_POLICER_TT_PACKET_SIZE_MODE_ENT  ttPacketSizeModeGet = CPSS_DXCH3_POLICER_TT_PACKET_SIZE_REGULAR_ENT;


    /* prepare device iterator */
    PRV_TGF_NOT_APPLIC_DEV_RESET_MAC(&dev, UTF_CH1_E | UTF_CH1_DIAMOND_E | UTF_CH2_E | UTF_XCAT2_E);

    /* 1. Go over all active devices. */
    while (GT_OK == prvUtfNextDeviceGet(&dev, GT_TRUE))
    {
        /*
            1.1. Call function with ttPacketSizeMode [CPSS_DXCH3_POLICER_TT_PACKET_SIZE_REGULAR_ENT /
                                                      CPSS_DXCH3_POLICER_TT_PACKET_SIZE_PASSENGER_ENT]
            Expected: GT_OK.
        */
        /* iterate with ttPacketSizeMode = CPSS_DXCH3_POLICER_TT_PACKET_SIZE_REGULAR_ENT */
        ttPacketSizeMode = CPSS_DXCH3_POLICER_TT_PACKET_SIZE_REGULAR_ENT;

        st = cpssDxCh3PolicerPacketSizeModeForTunnelTermSet(dev, plrStage, ttPacketSizeMode);
        UTF_VERIFY_EQUAL2_PARAM_MAC(GT_OK, st, dev, ttPacketSizeMode);

        /*
            1.2. Call cpssDxCh3PolicerPacketSizeModeForTunnelTermGet with not NULL ttPacketSizeModePtr.
            Expected: GT_OK and the same ttPacketSizeMode as was set.
        */
        st = cpssDxCh3PolicerPacketSizeModeForTunnelTermGet(dev, plrStage, &ttPacketSizeModeGet);
        UTF_VERIFY_EQUAL1_STRING_MAC(GT_OK, st,
                   "cpssDxCh3PolicerPacketSizeModeForTunnelTermGet: %d", dev);

        /* verifying values */
        UTF_VERIFY_EQUAL1_STRING_MAC(ttPacketSizeMode, ttPacketSizeModeGet,
                   "got another ttPacketSizeMode then was set: %d", dev);

        /* iterate with ttPacketSizeMode = CPSS_DXCH3_POLICER_TT_PACKET_SIZE_PASSENGER_ENT */
        ttPacketSizeMode = CPSS_DXCH3_POLICER_TT_PACKET_SIZE_PASSENGER_ENT;

        st = cpssDxCh3PolicerPacketSizeModeForTunnelTermSet(dev, plrStage, ttPacketSizeMode);
        UTF_VERIFY_EQUAL2_PARAM_MAC(GT_OK, st, dev, ttPacketSizeMode);

        /*
            1.2. Call cpssDxCh3PolicerPacketSizeModeForTunnelTermGet with not NULL ttPacketSizeModePtr.
            Expected: GT_OK and the same ttPacketSizeMode as was set.
        */
        st = cpssDxCh3PolicerPacketSizeModeForTunnelTermGet(dev, plrStage, &ttPacketSizeModeGet);
        UTF_VERIFY_EQUAL1_STRING_MAC(GT_OK, st,
                   "cpssDxCh3PolicerPacketSizeModeForTunnelTermGet: %d", dev);

        /* verifying values */
        UTF_VERIFY_EQUAL1_STRING_MAC(ttPacketSizeMode, ttPacketSizeModeGet,
                   "got another ttPacketSizeMode then was set: %d", dev);

        /*
            1.3. Call function with out of range ttPacketSizeMode [0x5AAAAA5].
            Expected: GT_BAD_PARAM.
        */
        ttPacketSizeMode = POLICER_INVALID_ENUM_CNS;

        st = cpssDxCh3PolicerPacketSizeModeForTunnelTermSet(dev, plrStage,
                                                            ttPacketSizeMode);
        UTF_VERIFY_EQUAL2_PARAM_MAC(GT_BAD_PARAM, st, dev, ttPacketSizeMode);
    }

    ttPacketSizeMode = CPSS_DXCH3_POLICER_TT_PACKET_SIZE_REGULAR_ENT;

    /* 2. For not-active devices and devices from non-applicable family */
    /* check that function returns GT_BAD_PARAM.                        */

    /* prepare device iterator */
    PRV_TGF_NOT_APPLIC_DEV_RESET_MAC(&dev, UTF_CH1_E | UTF_CH1_DIAMOND_E | UTF_CH2_E | UTF_XCAT2_E);

    /* go over all non active devices */
    while(GT_OK == prvUtfNextDeviceGet(&dev, GT_FALSE))
    {
        st = cpssDxCh3PolicerPacketSizeModeForTunnelTermSet(dev, plrStage, ttPacketSizeMode);
        UTF_VERIFY_EQUAL1_PARAM_MAC(GT_NOT_APPLICABLE_DEVICE, st, dev);
    }

    /* 3. Call function with out of bound value for device id.*/
    dev = PRV_CPSS_MAX_PP_DEVICES_CNS;

    st = cpssDxCh3PolicerPacketSizeModeForTunnelTermSet(dev, plrStage, ttPacketSizeMode);
    UTF_VERIFY_EQUAL1_PARAM_MAC(GT_BAD_PARAM, st, dev);
}

/*----------------------------------------------------------------------------*/
/*
GT_STATUS cpssDxCh3PolicerPacketSizeModeForTunnelTermGet
(
    IN  GT_U8                                       devNum,
    OUT CPSS_DXCH3_POLICER_TT_PACKET_SIZE_MODE_ENT  *ttPacketSizeModePtr
)
*/
UTF_TEST_CASE_MAC(cpssDxCh3PolicerPacketSizeModeForTunnelTermGet)
{
/*
    ITERATE_DEVICES (DxCh3)
    1.1. Call function with not NULL ttPacketSizeModePtr.
    Expected: GT_OK.
    1.2. Call function with ttPacketSizeModePtr [NULL].
    Expected: GT_BAD_PTR.
*/
    GT_STATUS   st = GT_OK;
    GT_U8       dev;

    CPSS_DXCH3_POLICER_TT_PACKET_SIZE_MODE_ENT  ttPacketSizeMode = CPSS_DXCH3_POLICER_TT_PACKET_SIZE_REGULAR_ENT;


    /* prepare device iterator */
    PRV_TGF_NOT_APPLIC_DEV_RESET_MAC(&dev, UTF_CH1_E | UTF_CH1_DIAMOND_E | UTF_CH2_E | UTF_XCAT2_E);

    /* 1. Go over all active devices. */
    while (GT_OK == prvUtfNextDeviceGet(&dev, GT_TRUE))
    {
        /*
            1.1. Call function with not NULL ttPacketSizeModePtr.
            Expected: GT_OK.
        */
        st = cpssDxCh3PolicerPacketSizeModeForTunnelTermGet(dev, plrStage,
                                                            &ttPacketSizeMode);
        UTF_VERIFY_EQUAL1_PARAM_MAC(GT_OK, st, dev);

        /*
            1.2. Call function with ttPacketSizeModePtr [NULL].
            Expected: GT_BAD_PTR.
        */
        st = cpssDxCh3PolicerPacketSizeModeForTunnelTermGet(dev, plrStage, NULL);
        UTF_VERIFY_EQUAL1_STRING_MAC(GT_BAD_PTR, st, "%d, ttPacketSizeModePtr = NULL", dev);
    }

    /* 2. For not-active devices and devices from non-applicable family */
    /* check that function returns GT_BAD_PARAM.                        */

    /* prepare device iterator */
    PRV_TGF_NOT_APPLIC_DEV_RESET_MAC(&dev, UTF_CH1_E | UTF_CH1_DIAMOND_E | UTF_CH2_E | UTF_XCAT2_E);

    /* go over all non active devices */
    while(GT_OK == prvUtfNextDeviceGet(&dev, GT_FALSE))
    {
        st = cpssDxCh3PolicerPacketSizeModeForTunnelTermGet(dev, plrStage,
                                                            &ttPacketSizeMode);
        UTF_VERIFY_EQUAL1_PARAM_MAC(GT_NOT_APPLICABLE_DEVICE, st, dev);
    }

    /* 3. Call function with out of bound value for device id.*/
    dev = PRV_CPSS_MAX_PP_DEVICES_CNS;

    st = cpssDxCh3PolicerPacketSizeModeForTunnelTermGet(dev, plrStage,
                                                        &ttPacketSizeMode);
    UTF_VERIFY_EQUAL1_PARAM_MAC(GT_BAD_PARAM, st, dev);
}

/*----------------------------------------------------------------------------*/
/*
GT_STATUS cpssDxCh3PolicerMeteringAutoRefreshScanEnableSet
(
    IN GT_U8    devNum,
    IN GT_BOOL  enable
)
*/
UTF_TEST_CASE_MAC(cpssDxCh3PolicerMeteringAutoRefreshScanEnableSet)
{
/*
    ITERATE_DEVICES (DxCh3)
    1.1. Call function with enable [GT_FALSE / GT_TRUE]
    Expected: GT_OK.
    1.2. Call cpssDxCh3PolicerMeteringAutoRefreshScanEnableGet with not NULL enablePtr.
    Expected: GT_OK and the same enable as was set.
*/
    GT_STATUS   st = GT_OK;
    GT_U8       dev;

    GT_BOOL  enable    = GT_FALSE;
    GT_BOOL  enableGet = GT_FALSE;


    /* prepare device iterator */
    PRV_TGF_NOT_APPLIC_DEV_RESET_MAC(&dev, UTF_CH1_E | UTF_CH1_DIAMOND_E | UTF_CH2_E | UTF_XCAT_E | UTF_LION_E | UTF_XCAT2_E);

    /* 1. Go over all active devices. */
    while (GT_OK == prvUtfNextDeviceGet(&dev, GT_TRUE))
    {
        /*
            1.1. Call function with enable [GT_FALSE / GT_TRUE]
            Expected: GT_OK.
        */
        /* iterate with enable = GT_FALSE */
        enable = GT_FALSE;

        st = cpssDxCh3PolicerMeteringAutoRefreshScanEnableSet(dev, enable);
        UTF_VERIFY_EQUAL2_PARAM_MAC(GT_OK, st, dev, enable);

        /*
            1.2. Call cpssDxCh3PolicerMeteringAutoRefreshScanEnableGet with not NULL enablePtr.
            Expected: GT_OK and the same enable as was set.
        */
        st = cpssDxCh3PolicerMeteringAutoRefreshScanEnableGet(dev, &enableGet);
        UTF_VERIFY_EQUAL1_STRING_MAC(GT_OK, st,
                   "cpssDxCh3PolicerMeteringAutoRefreshScanEnableGet: %d", dev);

        /* verifying values */
        UTF_VERIFY_EQUAL1_STRING_MAC(enable, enableGet,
                   "got another enable then was set: %d", dev);

        /* iterate with enable = GT_TRUE */
        enable = GT_TRUE;

        st = cpssDxCh3PolicerMeteringAutoRefreshScanEnableSet(dev, enable);
        UTF_VERIFY_EQUAL2_PARAM_MAC(GT_OK, st, dev, enable);

        /*
            1.2. Call cpssDxCh3PolicerMeteringAutoRefreshScanEnableGet with not NULL enablePtr.
            Expected: GT_OK and the same enable as was set.
        */
        st = cpssDxCh3PolicerMeteringAutoRefreshScanEnableGet(dev, &enableGet);
        UTF_VERIFY_EQUAL1_STRING_MAC(GT_OK, st,
                   "cpssDxCh3PolicerMeteringAutoRefreshScanEnableGet: %d", dev);

        /* verifying values */
        UTF_VERIFY_EQUAL1_STRING_MAC(enable, enableGet,
                   "got another enable then was set: %d", dev);
    }

    enable = GT_TRUE;

    /* 2. For not-active devices and devices from non-applicable family */
    /* check that function returns GT_BAD_PARAM.                        */

    /* prepare device iterator */
    PRV_TGF_NOT_APPLIC_DEV_RESET_MAC(&dev, UTF_CH1_E | UTF_CH1_DIAMOND_E | UTF_CH2_E | UTF_XCAT_E | UTF_LION_E | UTF_XCAT2_E);

    /* go over all non active devices */
    while(GT_OK == prvUtfNextDeviceGet(&dev, GT_FALSE))
    {
        st = cpssDxCh3PolicerMeteringAutoRefreshScanEnableSet(dev, enable);
        UTF_VERIFY_EQUAL1_PARAM_MAC(GT_NOT_APPLICABLE_DEVICE, st, dev);
    }

    /* 3. Call function with out of bound value for device id.*/
    dev = PRV_CPSS_MAX_PP_DEVICES_CNS;

    st = cpssDxCh3PolicerMeteringAutoRefreshScanEnableSet(dev, enable);
    UTF_VERIFY_EQUAL1_PARAM_MAC(GT_BAD_PARAM, st, dev);
}

/*----------------------------------------------------------------------------*/
/*
GT_STATUS cpssDxCh3PolicerMeteringAutoRefreshScanEnableGet
(
    IN  GT_U8       devNum,
    OUT GT_BOOL     *enablePtr
)
*/
UTF_TEST_CASE_MAC(cpssDxCh3PolicerMeteringAutoRefreshScanEnableGet)
{
/*
    ITERATE_DEVICES (DxCh3)
    1.1. Call function with not NULL enablePtr.
    Expected: GT_OK.
    1.2. Call function with enablePtr [NULL].
    Expected: GT_BAD_PTR.
*/
    GT_STATUS   st = GT_OK;
    GT_U8       dev;

    GT_BOOL     enable = GT_FALSE;


    /* prepare device iterator */
    PRV_TGF_NOT_APPLIC_DEV_RESET_MAC(&dev, UTF_CH1_E | UTF_CH1_DIAMOND_E | UTF_CH2_E | UTF_XCAT_E | UTF_LION_E | UTF_XCAT2_E);

    /* 1. Go over all active devices. */
    while (GT_OK == prvUtfNextDeviceGet(&dev, GT_TRUE))
    {
        /*
            1.1. Call function with not NULL enablePtr.
            Expected: GT_OK.
        */
        st = cpssDxCh3PolicerMeteringAutoRefreshScanEnableGet(dev, &enable);
        UTF_VERIFY_EQUAL1_PARAM_MAC(GT_OK, st, dev);

        /*
            1.2. Call function with enablePtr [NULL].
            Expected: GT_BAD_PTR.
        */
        st = cpssDxCh3PolicerMeteringAutoRefreshScanEnableGet(dev, NULL);
        UTF_VERIFY_EQUAL1_STRING_MAC(GT_BAD_PTR, st, "%d, enablePtr = NULL", dev);
    }

    /* 2. For not-active devices and devices from non-applicable family */
    /* check that function returns GT_BAD_PARAM.                        */

    /* prepare device iterator */
    PRV_TGF_NOT_APPLIC_DEV_RESET_MAC(&dev, UTF_CH1_E | UTF_CH1_DIAMOND_E | UTF_CH2_E | UTF_XCAT_E | UTF_LION_E | UTF_XCAT2_E);

    /* go over all non active devices */
    while(GT_OK == prvUtfNextDeviceGet(&dev, GT_FALSE))
    {
        st = cpssDxCh3PolicerMeteringAutoRefreshScanEnableGet(dev, &enable);
        UTF_VERIFY_EQUAL1_PARAM_MAC(GT_NOT_APPLICABLE_DEVICE, st, dev);
    }

    /* 3. Call function with out of bound value for device id.*/
    dev = PRV_CPSS_MAX_PP_DEVICES_CNS;

    st = cpssDxCh3PolicerMeteringAutoRefreshScanEnableGet(dev, &enable);
    UTF_VERIFY_EQUAL1_PARAM_MAC(GT_BAD_PARAM, st, dev);
}

/*----------------------------------------------------------------------------*/
/*
GT_STATUS cpssDxCh3PolicerMeteringAutoRefreshRangeSet
(
    IN GT_U8    devNum,
    IN GT_U32   startAddress,
    IN GT_U32   stopAddress
)
*/
UTF_TEST_CASE_MAC(cpssDxCh3PolicerMeteringAutoRefreshRangeSet)
{
/*
    ITERATE_DEVICES (DxCh3)
    1.1. Call function with startAddress [0 / 65535],
                            stopAddress [0 / 65535]
    Expected: GT_OK.
    1.2. Call cpssDxCh3PolicerMeteringAutoRefreshRangeGet with not NULL startAddressPtr
                                                          and not NULL stopAddressPtr.
    Expected: GT_OK and the same startAddress, stopAddress as was set.
    1.3. Call with out of range startAddress [65536]
                   and other params from 1.1.
    Expected: NOT GT_OK.
    1.4. Call with out of range stopAddress [65536]
                   and other params from 1.1.
    Expected: NOT GT_OK.
*/
    GT_STATUS   st = GT_OK;
    GT_U8       dev;

    GT_U32   startAddress    = 0;
    GT_U32   stopAddress     = 0;
    GT_U32   startAddressGet = 0;
    GT_U32   stopAddressGet  = 0;


    /* prepare device iterator */
    PRV_TGF_NOT_APPLIC_DEV_RESET_MAC(&dev, UTF_CH1_E | UTF_CH1_DIAMOND_E | UTF_CH2_E | UTF_XCAT_E | UTF_LION_E | UTF_XCAT2_E);

    /* 1. Go over all active devices. */
    while (GT_OK == prvUtfNextDeviceGet(&dev, GT_TRUE))
    {
        /*
            1.1. Call function with startAddress [0 / 65535],
                                    stopAddress [0 / 65535]
            Expected: GT_OK.
        */
        /* iterate with startAddress = 0 */
        startAddress = 0;
        stopAddress  = 0;

        st = cpssDxCh3PolicerMeteringAutoRefreshRangeSet(dev, startAddress, stopAddress);
        UTF_VERIFY_EQUAL3_PARAM_MAC(GT_OK, st, dev, startAddress, stopAddress);

        /*
            1.2. Call cpssDxCh3PolicerMeteringAutoRefreshRangeGet with not NULL startAddressPtr
                                                                  and not NULL stopAddressPtr.
            Expected: GT_OK and the same startAddress, stopAddress as was set.
        */
        st = cpssDxCh3PolicerMeteringAutoRefreshRangeGet(dev, &startAddressGet,
                                                         &stopAddressGet);
        UTF_VERIFY_EQUAL1_STRING_MAC(GT_OK, st,
                   "cpssDxCh3PolicerMeteringAutoRefreshRangeGet: %d", dev);

        /* verifying values */
        UTF_VERIFY_EQUAL1_STRING_MAC(startAddress, startAddressGet,
                   "got another startAddress then was set: %d", dev);
        UTF_VERIFY_EQUAL1_STRING_MAC(stopAddress, stopAddressGet,
                   "got another stopAddress then was set: %d", dev);

        /* iterate with startAddress = 65535 */
        startAddress = 65535;
        stopAddress  = 65535;

        st = cpssDxCh3PolicerMeteringAutoRefreshRangeSet(dev, startAddress, stopAddress);
        UTF_VERIFY_EQUAL3_PARAM_MAC(GT_OK, st, dev, startAddress, stopAddress);

        /*
            1.2. Call cpssDxCh3PolicerMeteringAutoRefreshRangeGet with not NULL startAddressPtr
                                                                  and not NULL stopAddressPtr.
            Expected: GT_OK and the same startAddress, stopAddress as was set.
        */
        st = cpssDxCh3PolicerMeteringAutoRefreshRangeGet(dev, &startAddressGet,
                                                         &stopAddressGet);
        UTF_VERIFY_EQUAL1_STRING_MAC(GT_OK, st,
                   "cpssDxCh3PolicerMeteringAutoRefreshRangeGet: %d", dev);

        /* verifying values */
        UTF_VERIFY_EQUAL1_STRING_MAC(startAddress, startAddressGet,
                   "got another startAddress then was set: %d", dev);
        UTF_VERIFY_EQUAL1_STRING_MAC(stopAddress, stopAddressGet,
                   "got another stopAddress then was set: %d", dev);

        /*
            1.3. Call with out of range startAddress [65536]
                           and other params from 1.1.
            Expected: NOT GT_OK.
        */
        startAddress = 65536;

        st = cpssDxCh3PolicerMeteringAutoRefreshRangeSet(dev, startAddress, stopAddress);
        UTF_VERIFY_NOT_EQUAL2_PARAM_MAC(GT_OK, st, dev, startAddress);

        startAddress = 0;

        /*
            1.4. Call with out of range stopAddress [65536]
                           and other params from 1.1.
            Expected: NOT GT_OK.
        */
        stopAddress = 65536;

        st = cpssDxCh3PolicerMeteringAutoRefreshRangeSet(dev, startAddress, stopAddress);
        UTF_VERIFY_NOT_EQUAL2_STRING_MAC(GT_OK, st, "%d, stopAddress = %d", dev, stopAddress);
    }

    startAddress = 0;
    stopAddress  = 0;

    /* 2. For not-active devices and devices from non-applicable family */
    /* check that function returns GT_BAD_PARAM.                        */

    /* prepare device iterator */
    PRV_TGF_NOT_APPLIC_DEV_RESET_MAC(&dev, UTF_CH1_E | UTF_CH1_DIAMOND_E | UTF_CH2_E | UTF_XCAT_E | UTF_LION_E | UTF_XCAT2_E);

    /* go over all non active devices */
    while(GT_OK == prvUtfNextDeviceGet(&dev, GT_FALSE))
    {
        st = cpssDxCh3PolicerMeteringAutoRefreshRangeSet(dev, startAddress, stopAddress);
        UTF_VERIFY_EQUAL1_PARAM_MAC(GT_NOT_APPLICABLE_DEVICE, st, dev);
    }

    /* 3. Call function with out of bound value for device id.*/
    dev = PRV_CPSS_MAX_PP_DEVICES_CNS;

    st = cpssDxCh3PolicerMeteringAutoRefreshRangeSet(dev, startAddress, stopAddress);
    UTF_VERIFY_EQUAL1_PARAM_MAC(GT_BAD_PARAM, st, dev);
}

/*----------------------------------------------------------------------------*/
/*
GT_STATUS cpssDxCh3PolicerMeteringAutoRefreshRangeGet
(
    IN GT_U8    devNum,
    OUT GT_U32  *startAddressPtr,
    OUT GT_U32  *stopAddressPtr
)
*/
UTF_TEST_CASE_MAC(cpssDxCh3PolicerMeteringAutoRefreshRangeGet)
{
/*
    ITERATE_DEVICES (DxCh3)
    1.1. Call with not NULL startAddressPtr
                   and not NULL stopAddressPtr.
    Expected: GT_OK.
    1.2. Call with startAddressPtr [NULL]
                   and other params from 1.1.
    Expected: GT_BAD_PTR.
    1.3. Call with stopAddressPtr [NULL]
                   and other params from 1.1.
    Expected: GT_BAD_PTR.
*/
    GT_STATUS   st = GT_OK;
    GT_U8       dev;

    GT_U32  startAddress = 0;
    GT_U32  stopAddress  = 0;


    /* prepare device iterator */
    PRV_TGF_NOT_APPLIC_DEV_RESET_MAC(&dev, UTF_CH1_E | UTF_CH1_DIAMOND_E | UTF_CH2_E | UTF_XCAT_E | UTF_LION_E | UTF_XCAT2_E);

    /* 1. Go over all active devices. */
    while (GT_OK == prvUtfNextDeviceGet(&dev, GT_TRUE))
    {
        /*
            1.1. Call with not NULL startAddressPtr
                           and not NULL stopAddressPtr.
            Expected: GT_OK.
        */
        st = cpssDxCh3PolicerMeteringAutoRefreshRangeGet(dev, &startAddress, &stopAddress);
        UTF_VERIFY_EQUAL1_PARAM_MAC(GT_OK, st, dev);

        /*
            1.2. Call with startAddressPtr [NULL]
                           and other params from 1.1.
            Expected: GT_BAD_PTR.
        */
        st = cpssDxCh3PolicerMeteringAutoRefreshRangeGet(dev, NULL, &stopAddress);
        UTF_VERIFY_EQUAL1_STRING_MAC(GT_BAD_PTR, st, "%d, startAddressPtr = NULL", dev);

        /*
            1.3. Call with stopAddressPtr [NULL]
                           and other params from 1.1.
            Expected: GT_BAD_PTR.
        */
        st = cpssDxCh3PolicerMeteringAutoRefreshRangeGet(dev, &startAddress, NULL);
        UTF_VERIFY_EQUAL1_STRING_MAC(GT_BAD_PTR, st, "%d, stopAddressPtr = NULL", dev);
    }

    /* 2. For not-active devices and devices from non-applicable family */
    /* check that function returns GT_BAD_PARAM.                        */

    /* prepare device iterator */
    PRV_TGF_NOT_APPLIC_DEV_RESET_MAC(&dev, UTF_CH1_E | UTF_CH1_DIAMOND_E | UTF_CH2_E | UTF_XCAT_E | UTF_LION_E | UTF_XCAT2_E);

    /* go over all non active devices */
    while(GT_OK == prvUtfNextDeviceGet(&dev, GT_FALSE))
    {
        st = cpssDxCh3PolicerMeteringAutoRefreshRangeGet(dev, &startAddress, &stopAddress);
        UTF_VERIFY_EQUAL1_PARAM_MAC(GT_NOT_APPLICABLE_DEVICE, st, dev);
    }

    /* 3. Call function with out of bound value for device id.*/
    dev = PRV_CPSS_MAX_PP_DEVICES_CNS;

    st = cpssDxCh3PolicerMeteringAutoRefreshRangeGet(dev, &startAddress, &stopAddress);
    UTF_VERIFY_EQUAL1_PARAM_MAC(GT_BAD_PARAM, st, dev);
}

/*----------------------------------------------------------------------------*/
/*
GT_STATUS cpssDxCh3PolicerMeteringAutoRefreshIntervalSet
(
    IN GT_U8    devNum,
    IN GT_U32   interval
)
*/
UTF_TEST_CASE_MAC(cpssDxCh3PolicerMeteringAutoRefreshIntervalSet)
{
/*
    ITERATE_DEVICES (DxCh3)
    1.1. Call function with interval [0 / 0xFFFFFFFF/coreClock]
    Expected: GT_OK.
    1.2. Call cpssDxCh3PolicerMeteringAutoRefreshIntervalGet with not NULL intervalPtr.
    Expected: GT_OK and the same interval as was set.
    1.3. Call function with out of range interval [0xFFFFFFFF/coreClock + 1]
    Expected: GT_OK.
*/
    GT_STATUS   st = GT_OK;
    GT_U8       dev;

    GT_U32   interval    = 0;
    GT_U32   intervalGet = 0;


    /* prepare device iterator */
    PRV_TGF_NOT_APPLIC_DEV_RESET_MAC(&dev, UTF_CH1_E | UTF_CH1_DIAMOND_E | UTF_CH2_E | UTF_XCAT_E | UTF_LION_E | UTF_XCAT2_E);

    /* 1. Go over all active devices. */
    while (GT_OK == prvUtfNextDeviceGet(&dev, GT_TRUE))
    {
        /*
            1.1. Call function with interval [0 / 0xFFFFFFFF].
            Expected: GT_OK.
        */
        /* iterate with interval = 0 */
        interval = 0;

        st = cpssDxCh3PolicerMeteringAutoRefreshIntervalSet(dev, interval);
        UTF_VERIFY_EQUAL2_PARAM_MAC(GT_OK, st, dev, interval);

        /*
            1.2. Call cpssDxCh3PolicerMeteringAutoRefreshIntervalGet with not NULL intervalPtr.
            Expected: GT_OK and the same interval as was set.
        */
        st = cpssDxCh3PolicerMeteringAutoRefreshIntervalGet(dev, &intervalGet);
        UTF_VERIFY_EQUAL1_STRING_MAC(GT_OK, st,
                   "cpssDxCh3PolicerMeteringAutoRefreshIntervalGet: %d", dev);

        /* verifying values */
        UTF_VERIFY_EQUAL1_STRING_MAC(interval, intervalGet,
                   "got another interval then was set: %d", dev);

        /* iterate with interval = 0xFFFFFFFF/coreClock */
        interval = 0xFFFFFFFF / PRV_CPSS_PP_MAC(dev)->coreClock;

        st = cpssDxCh3PolicerMeteringAutoRefreshIntervalSet(dev, interval);
        UTF_VERIFY_EQUAL2_PARAM_MAC(GT_OK, st, dev, interval);

        /*
            1.2. Call cpssDxCh3PolicerMeteringAutoRefreshIntervalGet with not NULL intervalPtr.
            Expected: GT_OK and the same interval as was set.
        */
        st = cpssDxCh3PolicerMeteringAutoRefreshIntervalGet(dev, &intervalGet);
        UTF_VERIFY_EQUAL1_STRING_MAC(GT_OK, st,
                   "cpssDxCh3PolicerMeteringAutoRefreshIntervalGet: %d", dev);

        /* verifying values */
        UTF_VERIFY_EQUAL1_STRING_MAC(interval, intervalGet,
                   "got another interval then was set: %d", dev);

        /*
            1.3. Call function with out of range interval [0xFFFFFFFF/coreClock + 1]
            Expected: GT_OK.
        */
        interval = 0xFFFFFFFF / PRV_CPSS_PP_MAC(dev)->coreClock + 1;

        st = cpssDxCh3PolicerMeteringAutoRefreshIntervalSet(dev, interval);
        UTF_VERIFY_NOT_EQUAL2_PARAM_MAC(GT_OK, st, dev, interval);
    }

    interval = 0;

    /* 2. For not-active devices and devices from non-applicable family */
    /* check that function returns GT_BAD_PARAM.                        */

    /* prepare device iterator */
    PRV_TGF_NOT_APPLIC_DEV_RESET_MAC(&dev, UTF_CH1_E | UTF_CH1_DIAMOND_E | UTF_CH2_E | UTF_XCAT_E | UTF_LION_E | UTF_XCAT2_E);

    /* go over all non active devices */
    while(GT_OK == prvUtfNextDeviceGet(&dev, GT_FALSE))
    {
        st = cpssDxCh3PolicerMeteringAutoRefreshIntervalSet(dev, interval);
        UTF_VERIFY_EQUAL1_PARAM_MAC(GT_NOT_APPLICABLE_DEVICE, st, dev);
    }

    /* 3. Call function with out of bound value for device id.*/
    dev = PRV_CPSS_MAX_PP_DEVICES_CNS;

    st = cpssDxCh3PolicerMeteringAutoRefreshIntervalSet(dev, interval);
    UTF_VERIFY_EQUAL1_PARAM_MAC(GT_BAD_PARAM, st, dev);
}

/*----------------------------------------------------------------------------*/
/*
GT_STATUS cpssDxCh3PolicerMeteringAutoRefreshIntervalGet
(
    IN GT_U8    devNum,
    OUT GT_U32  *intervalPtr
)
*/
UTF_TEST_CASE_MAC(cpssDxCh3PolicerMeteringAutoRefreshIntervalGet)
{
/*
    ITERATE_DEVICES (DxCh3)
    1.1. Call function with not NULL intervalPtr.
    Expected: GT_OK.
    1.2. Call function with intervalPtr [NULL].
    Expected: GT_BAD_PTR.
*/
    GT_STATUS   st = GT_OK;
    GT_U8       dev;

    GT_U32  interval = 0;


    /* prepare device iterator */
    PRV_TGF_NOT_APPLIC_DEV_RESET_MAC(&dev, UTF_CH1_E | UTF_CH1_DIAMOND_E | UTF_CH2_E | UTF_XCAT_E | UTF_LION_E | UTF_XCAT2_E);

    /* 1. Go over all active devices. */
    while (GT_OK == prvUtfNextDeviceGet(&dev, GT_TRUE))
    {
        /*
            1.1. Call function with not NULL intervalPtr.
            Expected: GT_OK.
        */
        st = cpssDxCh3PolicerMeteringAutoRefreshIntervalGet(dev,  &interval);
        UTF_VERIFY_EQUAL1_PARAM_MAC(GT_OK, st, dev);

        /*
            1.2. Call function with intervalPtr [NULL].
            Expected: GT_BAD_PTR.
        */
        st = cpssDxCh3PolicerMeteringAutoRefreshIntervalGet(dev, NULL);
        UTF_VERIFY_EQUAL1_STRING_MAC(GT_BAD_PTR, st, "%d, intervalPtr = NULL", dev);
    }

    /* 2. For not-active devices and devices from non-applicable family */
    /* check that function returns GT_BAD_PARAM.                        */

    /* prepare device iterator */
    PRV_TGF_NOT_APPLIC_DEV_RESET_MAC(&dev, UTF_CH1_E | UTF_CH1_DIAMOND_E | UTF_CH2_E | UTF_XCAT_E | UTF_LION_E | UTF_XCAT2_E);

    /* go over all non active devices */
    while(GT_OK == prvUtfNextDeviceGet(&dev, GT_FALSE))
    {
        st = cpssDxCh3PolicerMeteringAutoRefreshIntervalGet(dev, &interval);
        UTF_VERIFY_EQUAL1_PARAM_MAC(GT_NOT_APPLICABLE_DEVICE, st, dev);
    }

    /* 3. Call function with out of bound value for device id.*/
    dev = PRV_CPSS_MAX_PP_DEVICES_CNS;

    st = cpssDxCh3PolicerMeteringAutoRefreshIntervalGet(dev, &interval);
    UTF_VERIFY_EQUAL1_PARAM_MAC(GT_BAD_PARAM, st, dev);
}

/*----------------------------------------------------------------------------*/
/*
GT_STATUS cpssDxCh3PolicerMeteringEntryRefresh
(
    IN  GT_U8   devNum,
    IN  GT_U32  entryIndex
)
*/
UTF_TEST_CASE_MAC(cpssDxCh3PolicerMeteringEntryRefresh)
{
/*
    ITERATE_DEVICES (DxCh3)
    1.1. Call function with entryIndex [0 / maxIndex]
    Expected: GT_OK.
    1.2. Call with out of range entryIndex [maxIndex+1]
    Expected: NOT GT_OK.
*/
    GT_STATUS   st = GT_OK;
    GT_U8       dev;

    GT_U32  entryIndex = 0;


    /* prepare device iterator */
    PRV_TGF_NOT_APPLIC_DEV_RESET_MAC(&dev, UTF_CH1_E | UTF_CH1_DIAMOND_E | UTF_CH2_E);

    /* 1. Go over all active devices. */
    while (GT_OK == prvUtfNextDeviceGet(&dev, GT_TRUE))
    {
        /*
            1.1. Call function with entryIndex [0 / maxIndex]
            Expected: GT_OK.
        */
        /* iterate with entryIndex = 0 */
        entryIndex = 0;

        st = cpssDxCh3PolicerMeteringEntryRefresh(dev, plrStage, entryIndex);
        UTF_VERIFY_EQUAL2_PARAM_MAC(GT_OK, st, dev, entryIndex);

        /* iterate with max entryIndex */
        if (! PRV_CPSS_DXCH_XCAT_FAMILY_CHECK_MAC(dev))
        {
            entryIndex = PRV_CPSS_DXCH_PP_MAC(dev)->policer.memSize[0] - 1;
        }
        else
        {
            entryIndex = (PRV_CPSS_DXCH_PP_MAC(dev)->policer.memSize[plrStage] - 1);
        }

        st = cpssDxCh3PolicerMeteringEntryRefresh(dev, plrStage, entryIndex);
        UTF_VERIFY_EQUAL2_PARAM_MAC(GT_OK, st, dev, entryIndex);

        /*
            1.2. Call with out of range entryIndex
                           and other params from 1.1.
            Expected: NOT GT_OK.
        */
        if (! PRV_CPSS_DXCH_XCAT_FAMILY_CHECK_MAC(dev))
        {
            entryIndex = PRV_CPSS_DXCH_PP_MAC(dev)->policer.memSize[0];
        }
        else
        {
            entryIndex = PRV_CPSS_DXCH_PP_MAC(dev)->policer.memSize[plrStage];
        }

        st = cpssDxCh3PolicerMeteringEntryRefresh(dev, plrStage, entryIndex);
        UTF_VERIFY_NOT_EQUAL2_PARAM_MAC(GT_OK, st, dev, entryIndex);
    }

    entryIndex = 0;

    /* 2. For not-active devices and devices from non-applicable family */
    /* check that function returns GT_BAD_PARAM.                        */

    /* prepare device iterator */
    PRV_TGF_NOT_APPLIC_DEV_RESET_MAC(&dev, UTF_CH1_E | UTF_CH1_DIAMOND_E | UTF_CH2_E);

    /* go over all non active devices */
    while(GT_OK == prvUtfNextDeviceGet(&dev, GT_FALSE))
    {
        st = cpssDxCh3PolicerMeteringEntryRefresh(dev, plrStage, entryIndex);
        UTF_VERIFY_EQUAL1_PARAM_MAC(GT_NOT_APPLICABLE_DEVICE, st, dev);
    }

    /* 3. Call function with out of bound value for device id.*/
    dev = PRV_CPSS_MAX_PP_DEVICES_CNS;

    st = cpssDxCh3PolicerMeteringEntryRefresh(dev, plrStage, entryIndex);
    UTF_VERIFY_EQUAL1_PARAM_MAC(GT_BAD_PARAM, st, dev);
}

/*----------------------------------------------------------------------------*/
/*
GT_STATUS cpssDxCh3PolicerPortMeteringEnableSet
(
    IN GT_U8    devNum,
    IN GT_U8    portNum,
    IN GT_BOOL  enable
)
*/
UTF_TEST_CASE_MAC(cpssDxCh3PolicerPortMeteringEnableSet)
{
/*
    ITERATE_DEVICES_PHY_CPU_PORTS (DxCh3)
    1.1.1. Call with enable[GT_FALSE / GT_TRUE].
    Expected: GT_OK.
    1.1.2. Call cpssDxCh3PolicerPortMeteringEnableGet with not NULL enablePtr.
    Expected: GT_OK and the same enable as was set.
*/
    GT_STATUS   st   = GT_OK;
    GT_U8       dev;
    GT_U8       port = POLICER_VALID_PHY_PORT_NUM_CNS;

    GT_BOOL  enable    = GT_FALSE;
    GT_BOOL  enableGet = GT_FALSE;


    /* prepare device iterator */
    PRV_TGF_NOT_APPLIC_DEV_RESET_MAC(&dev, UTF_CH1_E | UTF_CH1_DIAMOND_E | UTF_CH2_E);

    /* 1. Go over all active devices. */
    while (GT_OK == prvUtfNextDeviceGet(&dev, GT_TRUE))
    {
        st = prvUtfNextPhyPortReset(&port, dev);
        UTF_VERIFY_EQUAL0_PARAM_MAC(GT_OK, st);
        /* 1.1. For all active devices go over all available physical ports. */
        while(GT_OK == prvUtfNextPhyPortGet(&port, GT_TRUE))
        {
            /*
                1.1.1. Call function with enable[GT_FALSE / GT_TRUE]
                Expected: GT_OK.
            */
            /* iterate with enable = GT_FALSE */
            enable = GT_FALSE;

            st = cpssDxCh3PolicerPortMeteringEnableSet(dev, plrStage, port, enable);
            UTF_VERIFY_EQUAL3_PARAM_MAC(GT_OK, st, dev, port, enable);

            /*
                1.1.2. Call cpssDxCh3PolicerPortMeteringEnableGet with not NULL enablePtr.
                Expected: GT_OK and the same enable as was set.
            */
            st = cpssDxCh3PolicerPortMeteringEnableGet(dev, plrStage, port, &enableGet);
            UTF_VERIFY_EQUAL2_STRING_MAC(GT_OK, st,
                       "cpssDxCh3PolicerPortMeteringEnableGet: %d, %d", dev, port);

            /* verifying values */
            UTF_VERIFY_EQUAL2_STRING_MAC(enable, enableGet,
                       "got another enable then was set: %d, %d", dev, port);

            /* iterate with enable = GT_TRUE */
            enable = GT_TRUE;

            st = cpssDxCh3PolicerPortMeteringEnableSet(dev, plrStage, port, enable);
            UTF_VERIFY_EQUAL3_PARAM_MAC(GT_OK, st, dev, port, enable);

            /*
                1.1.2. Call cpssDxCh3PolicerPortMeteringEnableGet with not NULL enablePtr.
                Expected: GT_OK and the same enablePtr as was set.
            */
            st = cpssDxCh3PolicerPortMeteringEnableGet(dev, plrStage, port, &enableGet);
            UTF_VERIFY_EQUAL2_STRING_MAC(GT_OK, st,
                       "cpssDxCh3PolicerPortMeteringEnableGet: %d, %d", dev, port);

            /* verifying values */
            UTF_VERIFY_EQUAL2_STRING_MAC(enable, enableGet,
                       "got another enable then was set: %d, %d", dev, port);
        }

        enable = GT_FALSE;

        st = prvUtfNextPhyPortReset(&port, dev);
        UTF_VERIFY_EQUAL0_PARAM_MAC(GT_OK, st);
        /* 1.2. For all active devices go over all non available physical ports. */
        while(GT_OK == prvUtfNextPhyPortGet(&port, GT_FALSE))
        {
            /* 1.2.1. Call function for each non-active port */
            st = cpssDxCh3PolicerPortMeteringEnableSet(dev, plrStage, port, enable);
            UTF_VERIFY_EQUAL2_PARAM_MAC(GT_BAD_PARAM, st, dev, port);
        }

        /* 1.3. For active device check that function returns GT_BAD_PARAM */
        /* for out of bound value for port number.                         */
        port = PRV_CPSS_MAX_PP_PORTS_NUM_CNS;

        st = cpssDxCh3PolicerPortMeteringEnableSet(dev, plrStage, port, enable);
        UTF_VERIFY_EQUAL2_PARAM_MAC(GT_BAD_PARAM, st, dev, port);

        /* 1.4. For active device check that function returns GT_OK        */
        /* for CPU port number.                                            */
        port = CPSS_CPU_PORT_NUM_CNS;

        st = cpssDxCh3PolicerPortMeteringEnableSet(dev, plrStage, port, enable);
        UTF_VERIFY_EQUAL2_PARAM_MAC(GT_OK, st, dev, port);
    }

    enable = GT_FALSE;

    /* 2. For not-active devices and devices from non-applicable family */
    port = POLICER_VALID_PHY_PORT_NUM_CNS;

    /* check that function returns GT_BAD_PARAM.                        */

    /* prepare device iterator */
    PRV_TGF_NOT_APPLIC_DEV_RESET_MAC(&dev, UTF_CH1_E | UTF_CH1_DIAMOND_E | UTF_CH2_E);

    /* go over all non active devices */
    while(GT_OK == prvUtfNextDeviceGet(&dev, GT_FALSE))
    {
        st = cpssDxCh3PolicerPortMeteringEnableSet(dev, plrStage, port, enable);
        UTF_VERIFY_EQUAL1_PARAM_MAC(GT_NOT_APPLICABLE_DEVICE, st, dev);
    }

    /* 3. Call function with out of bound value for device id.*/
    dev = PRV_CPSS_MAX_PP_DEVICES_CNS;

    st = cpssDxCh3PolicerPortMeteringEnableSet(dev, plrStage, port, enable);
    UTF_VERIFY_EQUAL1_PARAM_MAC(GT_BAD_PARAM, st, dev);
}

/*----------------------------------------------------------------------------*/
/*
GT_STATUS cpssDxCh3PolicerPortMeteringEnableGet
(
    IN  GT_U8       devNum,
    IN  GT_U8       portNum,
    OUT GT_BOOL     *enablePtr
)
*/
UTF_TEST_CASE_MAC(cpssDxCh3PolicerPortMeteringEnableGet)
{
/*
    ITERATE_DEVICES_PHY_CPU_PORTS (DxCh3)
    1.1.1. Call function with not NULL enablePtr.
    Expected: GT_OK.
    1.1.2. Call function with enablePtr [NULL].
    Expected: GT_BAD_PTR.
*/
    GT_STATUS   st   = GT_OK;
    GT_U8       dev;
    GT_U8       port = POLICER_VALID_PHY_PORT_NUM_CNS;

    GT_BOOL     enable = GT_FALSE;


    /* prepare device iterator */
    PRV_TGF_NOT_APPLIC_DEV_RESET_MAC(&dev, UTF_CH1_E | UTF_CH1_DIAMOND_E | UTF_CH2_E);

    /* 1. Go over all active devices. */
    while (GT_OK == prvUtfNextDeviceGet(&dev, GT_TRUE))
    {
        st = prvUtfNextPhyPortReset(&port, dev);
        UTF_VERIFY_EQUAL0_PARAM_MAC(GT_OK, st);
        /* 1.1. For all active devices go over all available physical ports. */
        while(GT_OK == prvUtfNextPhyPortGet(&port, GT_TRUE))
        {
            /*
                1.1.1. Call function with not NULL enablePtr.
                Expected: GT_OK.
            */
            st = cpssDxCh3PolicerPortMeteringEnableGet(dev, plrStage, port, &enable);
            UTF_VERIFY_EQUAL2_PARAM_MAC(GT_OK, st, dev, port);

            /*
                1.1.2. Call function with enablePtr [NULL].
                Expected: GT_BAD_PTR.
            */
            st = cpssDxCh3PolicerPortMeteringEnableGet(dev, plrStage, port, NULL);
            UTF_VERIFY_EQUAL2_STRING_MAC(GT_BAD_PTR, st, "%d, %d, enablePtr = NULL", dev, port);
        }

        st = prvUtfNextPhyPortReset(&port, dev);
        UTF_VERIFY_EQUAL0_PARAM_MAC(GT_OK, st);
        /* 1.2. For all active devices go over all non available physical ports. */
        while(GT_OK == prvUtfNextPhyPortGet(&port, GT_FALSE))
        {
            /* 1.2.1. Call function for each non-active port */
            st = cpssDxCh3PolicerPortMeteringEnableGet(dev, plrStage, port, &enable);
            UTF_VERIFY_EQUAL2_PARAM_MAC(GT_BAD_PARAM, st, dev, port);
        }

        /* 1.3. For active device check that function returns GT_BAD_PARAM */
        /* for out of bound value for port number.                         */
        port = PRV_CPSS_MAX_PP_PORTS_NUM_CNS;

        st = cpssDxCh3PolicerPortMeteringEnableGet(dev, plrStage, port, &enable);
        UTF_VERIFY_EQUAL2_PARAM_MAC(GT_BAD_PARAM, st, dev, port);

        /* 1.4. For active device check that function returns GT_OK        */
        /* for CPU port number.                                            */
        port = CPSS_CPU_PORT_NUM_CNS;

        st = cpssDxCh3PolicerPortMeteringEnableGet(dev, plrStage, port, &enable);
        UTF_VERIFY_EQUAL2_PARAM_MAC(GT_OK, st, dev, port);
    }

    /* 2. For not-active devices and devices from non-applicable family */
    port = POLICER_VALID_PHY_PORT_NUM_CNS;

    /* check that function returns GT_BAD_PARAM.                        */

    /* prepare device iterator */
    PRV_TGF_NOT_APPLIC_DEV_RESET_MAC(&dev, UTF_CH1_E | UTF_CH1_DIAMOND_E | UTF_CH2_E);

    /* go over all non active devices */
    while(GT_OK == prvUtfNextDeviceGet(&dev, GT_FALSE))
    {
        st = cpssDxCh3PolicerPortMeteringEnableGet(dev, plrStage, port, &enable);
        UTF_VERIFY_EQUAL1_PARAM_MAC(GT_NOT_APPLICABLE_DEVICE, st, dev);
    }

    /* 3. Call function with out of bound value for device id.*/
    dev = PRV_CPSS_MAX_PP_DEVICES_CNS;

    st = cpssDxCh3PolicerPortMeteringEnableGet(dev, plrStage, port, &enable);
    UTF_VERIFY_EQUAL1_PARAM_MAC(GT_BAD_PARAM, st, dev);
}

/*----------------------------------------------------------------------------*/
/*
GT_STATUS cpssDxCh3PolicerMruSet
(
    IN GT_U8    devNum,
    IN GT_U32   mruSize
)
*/
UTF_TEST_CASE_MAC(cpssDxCh3PolicerMruSet)
{
/*
    ITERATE_DEVICES (DxCh3)
    1.1. Call function with mruSize [0 / 64K]
    Expected: GT_OK.
    1.2. Call cpssDxCh3PolicerMruGet with not NULL mruSizePtr.
    Expected: GT_OK and the same mruSize as was set.
    1.3. Call with out of range mruSize [64K + 1]
    Expected: NOT GT_OK.
*/
    GT_STATUS   st = GT_OK;
    GT_U8       dev;

    GT_U32   mruSize    = 0;
    GT_U32   mruSizeGet = 0;


    /* prepare device iterator */
    PRV_TGF_NOT_APPLIC_DEV_RESET_MAC(&dev, UTF_CH1_E | UTF_CH1_DIAMOND_E | UTF_CH2_E);

    /* 1. Go over all active devices. */
    while (GT_OK == prvUtfNextDeviceGet(&dev, GT_TRUE))
    {
        /*
            1.1. Call function with mruSize [0 / 64K].
            Expected: GT_OK.
        */
        /* iterate with mruSize = 0 */
        mruSize = 0;

        st = cpssDxCh3PolicerMruSet(dev, plrStage, mruSize);
        UTF_VERIFY_EQUAL2_PARAM_MAC(GT_OK, st, dev, mruSize);

        /*
            1.2. Call cpssDxCh3PolicerMruGet with not NULL mruSizePtr.
            Expected: GT_OK and the same mruSize as was set.
        */
        st = cpssDxCh3PolicerMruGet(dev, plrStage, &mruSizeGet);
        UTF_VERIFY_EQUAL1_STRING_MAC(GT_OK, st,
                   "cpssDxCh3PolicerMruGet: %d", dev);

        /* verifying values */
        UTF_VERIFY_EQUAL1_STRING_MAC(mruSize, mruSizeGet,
                   "got another mruSize then was set: %d", dev);

        /* iterate with mruSize = 64K */
        mruSize = 65535;

        st = cpssDxCh3PolicerMruSet(dev, plrStage, mruSize);
        UTF_VERIFY_EQUAL2_PARAM_MAC(GT_OK, st, dev, mruSize);

        /*
            1.2. Call cpssDxCh3PolicerMruGet with not NULL mruSizePtr.
            Expected: GT_OK and the same mruSize as was set.
        */
        st = cpssDxCh3PolicerMruGet(dev, plrStage, &mruSizeGet);
        UTF_VERIFY_EQUAL1_STRING_MAC(GT_OK, st,
                   "cpssDxCh3PolicerMruGet: %d", dev);

        /* verifying values */
        UTF_VERIFY_EQUAL1_STRING_MAC(mruSize, mruSizeGet,
                   "got another mruSize then was set: %d", dev);

        /*
            1.3. Call with out of range mruSize [64K + 1]
            Expected: NOT GT_OK.
        */
        mruSize = 65535 + 1;

        st = cpssDxCh3PolicerMruSet(dev, plrStage, mruSize);
        UTF_VERIFY_NOT_EQUAL2_PARAM_MAC(GT_OK, st, dev, mruSize);
    }

    mruSize = 0;

    /* 2. For not-active devices and devices from non-applicable family */
    /* check that function returns GT_BAD_PARAM.                        */

    /* prepare device iterator */
    PRV_TGF_NOT_APPLIC_DEV_RESET_MAC(&dev, UTF_CH1_E | UTF_CH1_DIAMOND_E | UTF_CH2_E);

    /* go over all non active devices */
    while(GT_OK == prvUtfNextDeviceGet(&dev, GT_FALSE))
    {
        st = cpssDxCh3PolicerMruSet(dev, plrStage, mruSize);
        UTF_VERIFY_EQUAL1_PARAM_MAC(GT_NOT_APPLICABLE_DEVICE, st, dev);
    }

    /* 3. Call function with out of bound value for device id.*/
    dev = PRV_CPSS_MAX_PP_DEVICES_CNS;

    st = cpssDxCh3PolicerMruSet(dev, plrStage, mruSize);
    UTF_VERIFY_EQUAL1_PARAM_MAC(GT_BAD_PARAM, st, dev);
}

/*----------------------------------------------------------------------------*/
/*
GT_STATUS cpssDxCh3PolicerMruGet
(
    IN  GT_U8   devNum,
    OUT GT_U32  *mruSizePtr
)
*/
UTF_TEST_CASE_MAC(cpssDxCh3PolicerMruGet)
{
/*
    ITERATE_DEVICES (DxCh3)
    1.1. Call function with not NULL mruSizePtr.
    Expected: GT_OK.
    1.2. Call function with mruSizePtr [NULL].
    Expected: GT_BAD_PTR.
*/
    GT_STATUS   st = GT_OK;
    GT_U8       dev;

    GT_U32  mruSize = 0;


    /* prepare device iterator */
    PRV_TGF_NOT_APPLIC_DEV_RESET_MAC(&dev, UTF_CH1_E | UTF_CH1_DIAMOND_E | UTF_CH2_E);

    /* 1. Go over all active devices. */
    while (GT_OK == prvUtfNextDeviceGet(&dev, GT_TRUE))
    {
        /*
            1.1. Call function with not NULL mruSizePtr.
            Expected: GT_OK.
        */
        st = cpssDxCh3PolicerMruGet(dev, plrStage, &mruSize);
        UTF_VERIFY_EQUAL1_PARAM_MAC(GT_OK, st, dev);

        /*
            1.2. Call function with mruSizePtr [NULL].
            Expected: GT_BAD_PTR.
        */
        st = cpssDxCh3PolicerMruGet(dev, plrStage, NULL);
        UTF_VERIFY_EQUAL1_STRING_MAC(GT_BAD_PTR, st, "%d, mruSizePtr = NULL", dev);
    }

    /* 2. For not-active devices and devices from non-applicable family */
    /* check that function returns GT_BAD_PARAM.                        */

    /* prepare device iterator */
    PRV_TGF_NOT_APPLIC_DEV_RESET_MAC(&dev, UTF_CH1_E | UTF_CH1_DIAMOND_E | UTF_CH2_E);

    /* go over all non active devices */
    while(GT_OK == prvUtfNextDeviceGet(&dev, GT_FALSE))
    {
        st = cpssDxCh3PolicerMruGet(dev, plrStage, &mruSize);
        UTF_VERIFY_EQUAL1_PARAM_MAC(GT_NOT_APPLICABLE_DEVICE, st, dev);
    }

    /* 3. Call function with out of bound value for device id.*/
    dev = PRV_CPSS_MAX_PP_DEVICES_CNS;

    st = cpssDxCh3PolicerMruGet(dev, plrStage, &mruSize);
    UTF_VERIFY_EQUAL1_PARAM_MAC(GT_BAD_PARAM, st, dev);
}

/*----------------------------------------------------------------------------*/
/*
GT_STATUS cpssDxCh3PolicerErrorGet
(
    IN  GT_U8                               devNum,
    OUT CPSS_DXCH3_POLICER_ENTRY_TYPE_ENT   *entryTypePtr,
    OUT GT_U32                              *entryAddrPtr
)
*/
UTF_TEST_CASE_MAC(cpssDxCh3PolicerErrorGet)
{
/*
    ITERATE_DEVICES (DxCh3)
    1.1. Call function with not NULL entryTypePtr
                            and not NULL entryAddrPtr.
    Expected: GT_OK.
    1.2. Call function with entryTypePtr [NULL]
                            and other params from 1.1.
    Expected: GT_BAD_PTR.
    1.3. Call function with entryAddrPtr [NULL]
                            and other params from 1.1.
    Expected: GT_BAD_PTR.
*/
    GT_STATUS   st = GT_OK;
    GT_U8       dev;

    CPSS_DXCH3_POLICER_ENTRY_TYPE_ENT   entryType = CPSS_DXCH3_POLICER_ENTRY_METERING_E;
    GT_U32                              entryAddr = 0;


    /* prepare device iterator */
    PRV_TGF_NOT_APPLIC_DEV_RESET_MAC(&dev, UTF_CH1_E | UTF_CH1_DIAMOND_E | UTF_CH2_E);

    /* 1. Go over all active devices. */
    while (GT_OK == prvUtfNextDeviceGet(&dev, GT_TRUE))
    {
        /*
            1.1. Call function with not NULL entryTypePtr
                                    and not NULL entryAddrPtr.
            Expected: GT_OK.
        */
        st = cpssDxCh3PolicerErrorGet(dev, plrStage, &entryType, &entryAddr);
        if((st != GT_OK)&&(st != GT_EMPTY))
            UTF_VERIFY_EQUAL1_PARAM_MAC(GT_OK, st, dev);

        /*
            1.2. Call function with entryTypePtr [NULL]
                                    and other params from 1.1.
            Expected: GT_BAD_PTR.
        */
        st = cpssDxCh3PolicerErrorGet(dev, plrStage, NULL, &entryAddr);
        UTF_VERIFY_EQUAL1_STRING_MAC(GT_BAD_PTR, st, "%d, entryTypePtr = NULL", dev);

        /*
            1.3. Call function with entryAddrPtr [NULL]
                                    and other params from 1.1.
            Expected: GT_BAD_PTR.
        */
        st = cpssDxCh3PolicerErrorGet(dev, plrStage, &entryType, NULL);
        UTF_VERIFY_EQUAL1_STRING_MAC(GT_BAD_PTR, st, "%d, entryAddrPtr = NULL", dev);
    }

    /* 2. For not-active devices and devices from non-applicable family */
    /* check that function returns GT_BAD_PARAM.                        */

    /* prepare device iterator */
    PRV_TGF_NOT_APPLIC_DEV_RESET_MAC(&dev, UTF_CH1_E | UTF_CH1_DIAMOND_E | UTF_CH2_E);

    /* go over all non active devices */
    while(GT_OK == prvUtfNextDeviceGet(&dev, GT_FALSE))
    {
        st = cpssDxCh3PolicerErrorGet(dev, plrStage, &entryType, &entryAddr);
        UTF_VERIFY_EQUAL1_PARAM_MAC(GT_NOT_APPLICABLE_DEVICE, st, dev);
    }

    /* 3. Call function with out of bound value for device id.*/
    dev = PRV_CPSS_MAX_PP_DEVICES_CNS;

    st = cpssDxCh3PolicerErrorGet(dev, plrStage, &entryType, &entryAddr);
    UTF_VERIFY_EQUAL1_PARAM_MAC(GT_BAD_PARAM, st, dev);
}

/*----------------------------------------------------------------------------*/
/*
GT_STATUS cpssDxCh3PolicerErrorCounterGet
(
    IN  GT_U8   devNum,
    OUT GT_U32  *cntrValuePtr
)
*/
UTF_TEST_CASE_MAC(cpssDxCh3PolicerErrorCounterGet)
{
/*
    ITERATE_DEVICES (DxCh3)
    1.1. Call function with not NULL cntrValuePtr.
    Expected: GT_OK.
    1.2. Call function with cntrValuePtr [NULL].
    Expected: GT_BAD_PTR.
*/
    GT_STATUS   st = GT_OK;
    GT_U8       dev;

    GT_U32  cntrValue = 0;


    /* prepare device iterator */
    PRV_TGF_NOT_APPLIC_DEV_RESET_MAC(&dev, UTF_CH1_E | UTF_CH1_DIAMOND_E | UTF_CH2_E);

    /* 1. Go over all active devices. */
    while (GT_OK == prvUtfNextDeviceGet(&dev, GT_TRUE))
    {
        /*
            1.1. Call function with not NULL cntrValuePtr.
            Expected: GT_OK.
        */
        st = cpssDxCh3PolicerErrorCounterGet(dev, plrStage, &cntrValue);
        UTF_VERIFY_EQUAL1_PARAM_MAC(GT_OK, st, dev);

        /*
            1.2. Call function with cntrValuePtr [NULL].
            Expected: GT_BAD_PTR.
        */
        st = cpssDxCh3PolicerErrorCounterGet(dev, plrStage, NULL);
        UTF_VERIFY_EQUAL1_STRING_MAC(GT_BAD_PTR, st, "%d, cntrValuePtr = NULL", dev);
    }

    /* 2. For not-active devices and devices from non-applicable family */
    /* check that function returns GT_BAD_PARAM.                        */

    /* prepare device iterator */
    PRV_TGF_NOT_APPLIC_DEV_RESET_MAC(&dev, UTF_CH1_E | UTF_CH1_DIAMOND_E | UTF_CH2_E);

    /* go over all non active devices */
    while(GT_OK == prvUtfNextDeviceGet(&dev, GT_FALSE))
    {
        st = cpssDxCh3PolicerErrorCounterGet(dev, plrStage, &cntrValue);
        UTF_VERIFY_EQUAL1_PARAM_MAC(GT_NOT_APPLICABLE_DEVICE, st, dev);
    }

    /* 3. Call function with out of bound value for device id.*/
    dev = PRV_CPSS_MAX_PP_DEVICES_CNS;

    st = cpssDxCh3PolicerErrorCounterGet(dev, plrStage, &cntrValue);
    UTF_VERIFY_EQUAL1_PARAM_MAC(GT_BAD_PARAM, st, dev);
}

/*----------------------------------------------------------------------------*/
/*
GT_STATUS cpssDxCh3PolicerManagementCountersSet
(
    IN GT_U8                                    devNum,
    IN CPSS_DXCH3_POLICER_MNG_CNTR_SET_ENT      mngCntrSet,
    IN CPSS_DXCH3_POLICER_MNG_CNTR_TYPE_ENT     mngCntrType,
    IN CPSS_DXCH3_POLICER_MNG_CNTR_ENTRY_STC    *mngCntrPtr
)
*/
UTF_TEST_CASE_MAC(cpssDxCh3PolicerManagementCountersSet)
{
/*
    ITERATE_DEVICES (DxCh3)
    1.1. Call function with mngCntrSet[CPSS_DXCH3_POLICER_MNG_CNTR_SET0_E /
                                       CPSS_DXCH3_POLICER_MNG_CNTR_SET2_E],
                            mngCntrType [CPSS_DXCH3_POLICER_MNG_CNTR_GREEN_E /
                                         CPSS_DXCH3_POLICER_MNG_CNTR_DROP_E],
                            mngCntrPtr { duMngCntr [0 / 0xFFFF],
                                         packetMngCntr [0 / 0xFFFF]}
    Expected: GT_OK.
    1.2. Call cpssDxCh3PolicerManagementCountersGet with not NULL mngCntrPtr.
    Expected: GT_OK and the same mngCntr as was set.
    1.3. Call with out of range mngCntrSet [0x5AAAAA5]
                   and other params from 1.1.
    Expected: GT_BAD_PARAM.
    1.4. Call with out of range mngCntrType [0x5AAAAA5]
                   and other params from 1.1.
    Expected: GT_BAD_PARAM.
*/
    GT_STATUS   st  = GT_OK;
    GT_U8       dev;

    CPSS_DXCH3_POLICER_MNG_CNTR_SET_ENT      mngCntrSet  = CPSS_DXCH3_POLICER_MNG_CNTR_SET0_E;
    CPSS_DXCH3_POLICER_MNG_CNTR_TYPE_ENT     mngCntrType = CPSS_DXCH3_POLICER_MNG_CNTR_GREEN_E;
    CPSS_DXCH3_POLICER_MNG_CNTR_ENTRY_STC    mngCntr;
    CPSS_DXCH3_POLICER_MNG_CNTR_ENTRY_STC    mngCntrGet;


    cpssOsBzero((GT_VOID*) &mngCntr, sizeof(mngCntr));
    cpssOsBzero((GT_VOID*) &mngCntrGet, sizeof(mngCntrGet));

    /* prepare device iterator */
    PRV_TGF_NOT_APPLIC_DEV_RESET_MAC(&dev, UTF_CH1_E | UTF_CH1_DIAMOND_E | UTF_CH2_E);

    /* 1. Go over all active devices. */
    while (GT_OK == prvUtfNextDeviceGet(&dev, GT_TRUE))
    {
        /*
            1.1. Call function with mngCntrSet[CPSS_DXCH3_POLICER_MNG_CNTR_SET0_E /
                                               CPSS_DXCH3_POLICER_MNG_CNTR_SET2_E],
                                    mngCntrType [CPSS_DXCH3_POLICER_MNG_CNTR_GREEN_E /
                                                 CPSS_DXCH3_POLICER_MNG_CNTR_DROP_E],
                                    mngCntrPtr { duMngCntr [0 / 0xFFFF],
                                                 packetMngCntr [0 / 0xFFFF]}
            Expected: GT_OK.
        */
        /* iterate with mngCntrSet  = CPSS_DXCH3_POLICER_MNG_CNTR_SET0_E */
        mngCntrSet  = CPSS_DXCH3_POLICER_MNG_CNTR_SET0_E;
        mngCntrType = CPSS_DXCH3_POLICER_MNG_CNTR_GREEN_E;

        mngCntr.duMngCntr.l[0] = 0;
        mngCntr.packetMngCntr = 0;

        st = cpssDxCh3PolicerManagementCountersSet(dev, plrStage, mngCntrSet,
                                                   mngCntrType, &mngCntr);
        UTF_VERIFY_EQUAL3_PARAM_MAC(GT_OK, st, dev, mngCntrSet, mngCntrType);

        /*
            1.2. Call cpssDxCh3PolicerManagementCountersGet with not NULL mngCntrPtr.
            Expected: GT_OK and the same mngCntr as was set.
        */
        st = cpssDxCh3PolicerManagementCountersGet(dev, plrStage, mngCntrSet,
                                                   mngCntrType, &mngCntrGet);
        UTF_VERIFY_EQUAL3_STRING_MAC(GT_OK, st,
                   "cpssDxCh3PolicerManagementCountersGet: %d, %d, %d",  dev, mngCntrSet, mngCntrType);

        /* verifying values */
        UTF_VERIFY_EQUAL1_STRING_MAC(mngCntr.duMngCntr.l[0], mngCntrGet.duMngCntr.l[0],
                   "got another mngCntr.duMngCntr then was set: %d", dev);
        UTF_VERIFY_EQUAL1_STRING_MAC(mngCntr.packetMngCntr, mngCntrGet.packetMngCntr,
                   "got another mngCntr.packetMngCntr then was set: %d", dev);

        /* iterate with mngCntrSet  = CPSS_DXCH3_POLICER_MNG_CNTR_SET2_E */
        mngCntrSet  = CPSS_DXCH3_POLICER_MNG_CNTR_SET2_E;
        mngCntrType = CPSS_DXCH3_POLICER_MNG_CNTR_DROP_E;

        mngCntr.duMngCntr.l[0] = 0;
        mngCntr.packetMngCntr = 0;

        st = cpssDxCh3PolicerManagementCountersSet(dev, plrStage, mngCntrSet,
                                                   mngCntrType, &mngCntr);
        UTF_VERIFY_EQUAL3_PARAM_MAC(GT_OK, st, dev, mngCntrSet, mngCntrType);

        /*
            1.2. Call cpssDxCh3PolicerManagementCountersGet with not NULL mngCntrPtr.
            Expected: GT_OK and the same mngCntr as was set.
        */
        st = cpssDxCh3PolicerManagementCountersGet(dev, plrStage, mngCntrSet,
                                                   mngCntrType, &mngCntrGet);
        UTF_VERIFY_EQUAL3_STRING_MAC(GT_OK, st,
                   "cpssDxCh3PolicerManagementCountersGet: %d, %d, %d",  dev, mngCntrSet, mngCntrType);

        /* verifying values */
        UTF_VERIFY_EQUAL1_STRING_MAC(mngCntr.duMngCntr.l[0], mngCntrGet.duMngCntr.l[0],
                   "got another mngCntr.duMngCntr then was set: %d", dev);
        UTF_VERIFY_EQUAL1_STRING_MAC(mngCntr.packetMngCntr, mngCntrGet.packetMngCntr,
                   "got another mngCntr.packetMngCntr then was set: %d", dev);

        /*
            1.3. Call with out of range mngCntrSet [0x5AAAAA5]
                           and other params from 1.1.
            Expected: GT_BAD_PARAM.
        */
        mngCntrSet = POLICER_INVALID_ENUM_CNS;

        st = cpssDxCh3PolicerManagementCountersSet(dev, plrStage, mngCntrSet,
                                                   mngCntrType, &mngCntr);
        UTF_VERIFY_EQUAL2_PARAM_MAC(GT_BAD_PARAM, st, dev, mngCntrSet);

        mngCntrSet = CPSS_DXCH3_POLICER_MNG_CNTR_SET0_E;

        /*
            1.4. Call with out of range mngCntrType [0x5AAAAA5]
                           and other params from 1.1.
            Expected: GT_BAD_PARAM.
        */
        mngCntrType = POLICER_INVALID_ENUM_CNS;

        st = cpssDxCh3PolicerManagementCountersSet(dev, plrStage, mngCntrSet, mngCntrType, &mngCntr);
        UTF_VERIFY_EQUAL2_STRING_MAC(GT_BAD_PARAM, st, "%d, mngCntrType = %d", dev, mngCntrType);
    }

    mngCntrSet  = CPSS_DXCH3_POLICER_MNG_CNTR_SET0_E;
    mngCntrType = CPSS_DXCH3_POLICER_MNG_CNTR_GREEN_E;

    mngCntr.duMngCntr.l[0]     = 0;
    mngCntr.packetMngCntr = 0;

    /* 2. For not-active devices and devices from non-applicable family */
    /* check that function returns GT_BAD_PARAM.                        */

    /* prepare device iterator */
    PRV_TGF_NOT_APPLIC_DEV_RESET_MAC(&dev, UTF_CH1_E | UTF_CH1_DIAMOND_E | UTF_CH2_E);

    /* go over all non active devices */
    while(GT_OK == prvUtfNextDeviceGet(&dev, GT_FALSE))
    {
        st = cpssDxCh3PolicerManagementCountersSet(dev, plrStage, mngCntrSet,
                                                   mngCntrType, &mngCntr);
        UTF_VERIFY_EQUAL1_PARAM_MAC(GT_NOT_APPLICABLE_DEVICE, st, dev);
    }

    /* 3. Call function with out of bound value for device id.*/
    dev = PRV_CPSS_MAX_PP_DEVICES_CNS;

    st = cpssDxCh3PolicerManagementCountersSet(dev, plrStage, mngCntrSet,
                                               mngCntrType, &mngCntr);
    UTF_VERIFY_EQUAL1_PARAM_MAC(GT_BAD_PARAM, st, dev);
}

/*----------------------------------------------------------------------------*/
/*
GT_STATUS cpssDxCh3PolicerManagementCountersGet
(
    IN  GT_U8                                   devNum,
    IN  CPSS_DXCH3_POLICER_MNG_CNTR_SET_ENT    mngCntrSet,
    IN  CPSS_DXCH3_POLICER_MNG_CNTR_TYPE_ENT   mngCntrType,
    OUT CPSS_DXCH3_POLICER_MNG_CNTR_ENTRY_STC  *mngCntrPtr
)
*/
UTF_TEST_CASE_MAC(cpssDxCh3PolicerManagementCountersGet)
{
/*
    ITERATE_DEVICES (DxCh3)
    1.1. Call function with mngCntrSet[CPSS_DXCH3_POLICER_MNG_CNTR_SET0_E /
                                       CPSS_DXCH3_POLICER_MNG_CNTR_SET2_E],
                            mngCntrType [CPSS_DXCH3_POLICER_MNG_CNTR_GREEN_E /
                                         CPSS_DXCH3_POLICER_MNG_CNTR_DROP_E]
                            and not NULL mngCntrPtr.
    Expected: GT_OK.
    1.2. Call with out of range mngCntrSet [0x5AAAAA5]
                   and other params from 1.1.
    Expected: GT_BAD_PARAM.
    1.3. Call with out of range mngCntrType [0x5AAAAA5]
                   and other params from 1.1.
    Expected: GT_BAD_PARAM.
    1.4. Call with mngCntrPtr [NULL].
                   and other params from 1.1.
    Expected: GT_BAD_PTR.
*/
    GT_STATUS   st = GT_OK;
    GT_U8       dev;

    CPSS_DXCH3_POLICER_MNG_CNTR_SET_ENT     mngCntrSet  = CPSS_DXCH3_POLICER_MNG_CNTR_SET0_E;
    CPSS_DXCH3_POLICER_MNG_CNTR_TYPE_ENT    mngCntrType = CPSS_DXCH3_POLICER_MNG_CNTR_GREEN_E;
    CPSS_DXCH3_POLICER_MNG_CNTR_ENTRY_STC   mngCntr;


    cpssOsBzero((GT_VOID*) &mngCntr, sizeof(mngCntr));

    /* prepare device iterator */
    PRV_TGF_NOT_APPLIC_DEV_RESET_MAC(&dev, UTF_CH1_E | UTF_CH1_DIAMOND_E | UTF_CH2_E);

    /* 1. Go over all active devices. */
    while (GT_OK == prvUtfNextDeviceGet(&dev, GT_TRUE))
    {
        /*
            1.1. Call function with mngCntrSet[CPSS_DXCH3_POLICER_MNG_CNTR_SET0_E /
                                               CPSS_DXCH3_POLICER_MNG_CNTR_SET2_E],
                                    mngCntrType [CPSS_DXCH3_POLICER_MNG_CNTR_GREEN_E /
                                                 CPSS_DXCH3_POLICER_MNG_CNTR_DROP_E]
                                    and not NULL mngCntrPtr.
            Expected: GT_OK.
        */
        /* iterate with mngCntrSet = CPSS_DXCH3_POLICER_MNG_CNTR_SET0_E */
        mngCntrSet  = CPSS_DXCH3_POLICER_MNG_CNTR_SET0_E;
        mngCntrType = CPSS_DXCH3_POLICER_MNG_CNTR_GREEN_E;

        st = cpssDxCh3PolicerManagementCountersGet(dev, plrStage, mngCntrSet,
                                                   mngCntrType, &mngCntr);
        UTF_VERIFY_EQUAL3_PARAM_MAC(GT_OK, st, dev, mngCntrSet, mngCntrType);

        /* iterate with mngCntrSet = CPSS_DXCH3_POLICER_MNG_CNTR_SET2_E */
        mngCntrSet  = CPSS_DXCH3_POLICER_MNG_CNTR_SET2_E;
        mngCntrType = CPSS_DXCH3_POLICER_MNG_CNTR_DROP_E;

        st = cpssDxCh3PolicerManagementCountersGet(dev, plrStage, mngCntrSet,
                                                   mngCntrType, &mngCntr);
        UTF_VERIFY_EQUAL3_PARAM_MAC(GT_OK, st, dev, mngCntrSet, mngCntrType);

        /*
            1.2. Call with out of range mngCntrSet [0x5AAAAA5]
                            and other params from 1.1.
            Expected: GT_BAD_PARAM.
        */
        mngCntrSet = POLICER_INVALID_ENUM_CNS;

        st = cpssDxCh3PolicerManagementCountersGet(dev, plrStage, mngCntrSet,
                                                   mngCntrType, &mngCntr);
        UTF_VERIFY_EQUAL2_PARAM_MAC(GT_BAD_PARAM, st, dev, mngCntrSet);

        mngCntrSet = CPSS_DXCH3_POLICER_MNG_CNTR_SET0_E;

        /*
            1.3. Call with out of range mngCntrType [0x5AAAAA5]
                           and other params from 1.1.
            Expected: GT_BAD_PARAM.
        */
        mngCntrType = POLICER_INVALID_ENUM_CNS;

        st = cpssDxCh3PolicerManagementCountersGet(dev, plrStage, mngCntrSet,
                                                   mngCntrType, &mngCntr);
        UTF_VERIFY_EQUAL2_STRING_MAC(GT_BAD_PARAM, st, "%d, mngCntrType = %d" ,dev, mngCntrType);

        mngCntrType = CPSS_DXCH3_POLICER_MNG_CNTR_DROP_E;

        /*
            1.4. Call with mngCntrPtr [NULL]
                           and other params from 1.1.
            Expected: GT_BAD_PTR.
        */
        st = cpssDxCh3PolicerManagementCountersGet(dev, plrStage, mngCntrSet, mngCntrType, NULL);
        UTF_VERIFY_EQUAL1_STRING_MAC(GT_BAD_PTR, st, "%d, mngCntrPtr = NULL", dev);
    }

    mngCntrSet  = CPSS_DXCH3_POLICER_MNG_CNTR_SET0_E;
    mngCntrType = CPSS_DXCH3_POLICER_MNG_CNTR_GREEN_E;

    /* 2. For not-active devices and devices from non-applicable family */
    /* check that function returns GT_BAD_PARAM.                        */

    /* prepare device iterator */
    PRV_TGF_NOT_APPLIC_DEV_RESET_MAC(&dev, UTF_CH1_E | UTF_CH1_DIAMOND_E | UTF_CH2_E);

    /* go over all non active devices */
    while(GT_OK == prvUtfNextDeviceGet(&dev, GT_FALSE))
    {
        st = cpssDxCh3PolicerManagementCountersGet(dev, plrStage, mngCntrSet, mngCntrType, &mngCntr);
        UTF_VERIFY_EQUAL1_PARAM_MAC(GT_NOT_APPLICABLE_DEVICE, st, dev);
    }

    /* 3. Call function with out of bound value for device id.*/
    dev = PRV_CPSS_MAX_PP_DEVICES_CNS;

    st = cpssDxCh3PolicerManagementCountersGet(dev, plrStage, mngCntrSet, mngCntrType, &mngCntr);
    UTF_VERIFY_EQUAL1_PARAM_MAC(GT_BAD_PARAM, st, dev);
}

/*----------------------------------------------------------------------------*/
/*
GT_STATUS cpssDxCh3PolicerMeteringEntrySet
(
    IN  GT_U8                                   devNum,
    IN  GT_U32                                  entryIndex,
    IN  CPSS_DXCH3_POLICER_METERING_ENTRY_STC   *entryPtr,
    OUT CPSS_DXCH3_POLICER_METER_TB_PARAMS_UNT  *tbParamsPtr
)
*/
UTF_TEST_CASE_MAC(cpssDxCh3PolicerMeteringEntrySet)
{
/*
    ITERATE_DEVICES (DxCh3)
    1.1. Call function with entryIndex [0],
                            entryPtr {countingEntryIndex[0],
                                      mngCounterSet[CPSS_DXCH3_POLICER_MNG_CNTR_SET0_E],
                                      meterColorMode[CPSS_POLICER_COLOR_BLIND_E],
                                      meterMode[CPSS_DXCH3_POLICER_METER_MODE_SR_TCM_E],
                                      tokenBucketParams.srTcmParams{cir[0],
                                                                    cbs[0],
                                                                    ebs[0]},
                                      modifyUp[CPSS_PACKET_ATTRIBUTE_MODIFY_KEEP_PREVIOUS_E],
                                      modifyDscp[CPSS_PACKET_ATTRIBUTE_MODIFY_KEEP_PREVIOUS_E],
                                      yellowPcktCmd[CPSS_DXCH3_POLICER_NON_CONFORM_CMD_NO_CHANGE_E],
                                      redPcktCmd[CPSS_DXCH3_POLICER_NON_CONFORM_CMD_NO_CHANGE_E]}
                           and not NULL tbParamsPtr.
    Expected: GT_OK.
    1.2. Call function with entryIndex [maxIndex],
                            entryPtr {countingEntryIndex[0xFF],
                                      mngCounterSet[CPSS_DXCH3_POLICER_MNG_CNTR_SET2_E],
                                      meterColorMode[CPSS_POLICER_COLOR_AWARE_E],
                                      meterMode[CPSS_DXCH3_POLICER_METER_MODE_TR_TCM_E],
                                      tokenBucketParams.trTcmParams{cir[0],
                                                                    cbs[0],
                                                                    pir[0],
                                                                    pbs[0]},
                                      modifyUp[CPSS_PACKET_ATTRIBUTE_MODIFY_ENABLE_E],
                                      modifyDscp[CPSS_PACKET_ATTRIBUTE_MODIFY_ENABLE_E],
                                      yellowPcktCmd[CPSS_DXCH3_POLICER_NON_CONFORM_CMD_REMARK_E],
                                      redPcktCmd[CPSS_DXCH3_POLICER_NON_CONFORM_CMD_REMARK_E]}
                           and not NULL tbParamsPtr.
    Expected: GT_OK.
    1.3. Call cpssDxCh3PolicerMeteringEntryGet with not NULL entryPtr and other params from 1.1.
    Expected: GT_OK and the same entry as was set.
    1.4. Call with out of range entryIndex [maxIndex+1]
                   and other params from 1.1.
    Expected: NOT GT_OK.
    1.5. Call with out of range entryPtr->mngCounterSet [0x5AAAAA5]
                   and other params from 1.1.
    Expected: GT_BAD_PARAM.
    1.6. Call with out of range entryPtr->meterColorMode [0x5AAAAA5]
                   and other params from 1.1.
    Expected: GT_BAD_PARAM.
    1.7. Call with out of range entryPtr->meterMode [0x5AAAAA5]
                   and other params from 1.1.
    Expected: GT_BAD_PARAM.
    1.8. Call with out of range entryPtr->modifyUp [0x5AAAAA5]
                    and other params from 1.1.
    Expected: GT_BAD_PARAM.
    1.9. Call with out of range entryPtr->modifyDscp [0x5AAAAA5]
                    and other params from 1.1.
    Expected: GT_BAD_PARAM.
    1.10. Call with out of range entryPtr->yellowPcktCmd [0x5AAAAA5]
                    and other params from 1.1.
    Expected: GT_BAD_PARAM.
    1.11. Call with out of range entryPtr->redPcktCmd [0x5AAAAA5]
                    and other params from 1.1.
    Expected: GT_BAD_PARAM.
    1.12. Call with entryPtr [NULL]
                    and other params from 1.1.
    Expected: GT_BAD_PTR.
    1.13. Call with tbParamsPtr [NULL]
                    and other params from 1.1.
    Expected: GT_BAD_PTR.
*/
    GT_STATUS   st = GT_OK;
    GT_U8       dev;

    GT_U32                                  entryIndex = 0;
    CPSS_DXCH3_POLICER_METERING_ENTRY_STC   entry;
    CPSS_DXCH3_POLICER_METERING_ENTRY_STC   entryGet;
    CPSS_DXCH3_POLICER_METER_TB_PARAMS_UNT  tbParams;


    cpssOsBzero((GT_VOID*) &entry, sizeof(entry));
    cpssOsBzero((GT_VOID*) &entryGet, sizeof(entryGet));
    cpssOsBzero((GT_VOID*) &tbParams, sizeof(tbParams));

    /* prepare device iterator */
    PRV_TGF_NOT_APPLIC_DEV_RESET_MAC(&dev, UTF_CH1_E | UTF_CH1_DIAMOND_E | UTF_CH2_E);

    /* 1. Go over all active devices. */
    while (GT_OK == prvUtfNextDeviceGet(&dev, GT_TRUE))
    {
        /*
            1.1. Call function with entryIndex [0],
                                    entryPtr {countingEntryIndex[0],
                                              mngCounterSet[CPSS_DXCH3_POLICER_MNG_CNTR_SET0_E],
                                              meterColorMode[CPSS_POLICER_COLOR_BLIND_E],
                                              meterMode[CPSS_DXCH3_POLICER_METER_MODE_SR_TCM_E],
                                              tokenBucketParams.srTcmParams{cir[0],
                                                                            cbs[0],
                                                                            ebs[0]},
                                              modifyUp[CPSS_PACKET_ATTRIBUTE_MODIFY_KEEP_PREVIOUS_E],
                                              modifyDscp[CPSS_PACKET_ATTRIBUTE_MODIFY_KEEP_PREVIOUS_E],
                                              yellowPcktCmd[CPSS_DXCH3_POLICER_NON_CONFORM_CMD_NO_CHANGE_E],
                                              redPcktCmd[CPSS_DXCH3_POLICER_NON_CONFORM_CMD_NO_CHANGE_E]}
                                   and not NULL tbParamsPtr.
            Expected: GT_OK.
        */
        entryIndex = 0;

        entry.countingEntryIndex = 0;
        entry.mngCounterSet      = CPSS_DXCH3_POLICER_MNG_CNTR_SET0_E;
        entry.meterColorMode     = CPSS_POLICER_COLOR_BLIND_E;
        entry.meterMode          = CPSS_DXCH3_POLICER_METER_MODE_SR_TCM_E;

        entry.tokenBucketParams.srTcmParams.cir = 0;
        entry.tokenBucketParams.srTcmParams.cbs = 0;
        entry.tokenBucketParams.srTcmParams.ebs = 0;

        entry.modifyUp      = CPSS_PACKET_ATTRIBUTE_MODIFY_KEEP_PREVIOUS_E;
        entry.modifyDscp    = CPSS_PACKET_ATTRIBUTE_MODIFY_KEEP_PREVIOUS_E;
        entry.yellowPcktCmd = CPSS_DXCH3_POLICER_NON_CONFORM_CMD_NO_CHANGE_E;
        entry.redPcktCmd    = CPSS_DXCH3_POLICER_NON_CONFORM_CMD_NO_CHANGE_E;

        st = cpssDxCh3PolicerMeteringEntrySet(dev, plrStage, entryIndex, &entry, &tbParams);
        UTF_VERIFY_EQUAL2_PARAM_MAC(GT_OK, st, dev, entryIndex);

        /*
            1.2. Call cpssDxCh3PolicerMeteringEntryGet with not NULL entryPtr and other params from 1.1.
            Expected: GT_OK and the same entry as was set.
        */
        st = cpssDxCh3PolicerMeteringEntryGet(dev, plrStage, entryIndex, &entryGet);
        UTF_VERIFY_EQUAL2_STRING_MAC(GT_OK, st,
                   "cpssDxCh3PolicerMeteringEntryGet: %d, %d", dev, entryIndex);

        /* verifying values */
        UTF_VERIFY_EQUAL1_STRING_MAC(entry.countingEntryIndex, entryGet.countingEntryIndex,
                   "got another entryPtr->countingEntryIndex then was set: %d", dev);
        UTF_VERIFY_EQUAL1_STRING_MAC(entry.mngCounterSet, entryGet.mngCounterSet,
                   "got another entryPtr->mngCounterSet then was set: %d", dev);
        UTF_VERIFY_EQUAL1_STRING_MAC(entry.meterColorMode, entryGet.meterColorMode,
                   "got another entryPtr->meterColorMode then was set: %d", dev);
        UTF_VERIFY_EQUAL1_STRING_MAC(entry.meterMode, entryGet.meterMode,
                   "got another entryPtr->meterMode then was set: %d", dev);

        UTF_VERIFY_EQUAL1_STRING_MAC(entry.tokenBucketParams.srTcmParams.cir, entryGet.tokenBucketParams.srTcmParams.cir,
                   "got another entryPtr->tokenBucketParams.srTcmParams.cir then was set: %d", dev);
        UTF_VERIFY_EQUAL1_STRING_MAC(entry.tokenBucketParams.srTcmParams.cbs, entryGet.tokenBucketParams.srTcmParams.cbs,
                   "got another entryPtr->tokenBucketParams.srTcmParams.cbs then was set: %d", dev);
        UTF_VERIFY_EQUAL1_STRING_MAC(entry.tokenBucketParams.srTcmParams.ebs, entryGet.tokenBucketParams.srTcmParams.ebs,
                   "got another entryPtr->tokenBucketParams.srTcmParams.ebs then was set: %d", dev);

        UTF_VERIFY_EQUAL1_STRING_MAC(entry.modifyUp, entryGet.modifyUp,
                   "got another entryPtr->modifyUp then was set: %d", dev);
        UTF_VERIFY_EQUAL1_STRING_MAC(entry.modifyDscp, entryGet.modifyDscp,
                   "got another entryPtr->modifyDscp then was set: %d", dev);
        UTF_VERIFY_EQUAL1_STRING_MAC(entry.yellowPcktCmd, entryGet.yellowPcktCmd,
                   "got another entryPtr->yellowPcktCmd then was set: %d", dev);
        UTF_VERIFY_EQUAL1_STRING_MAC(entry.redPcktCmd, entryGet.redPcktCmd,
                   "got another entryPtr->redPcktCmd then was set: %d", dev);

        /*
            1.2. Call function with entryIndex [maxIndex],
                                    entryPtr {countingEntryIndex[0xFF],
                                              mngCounterSet[CPSS_DXCH3_POLICER_MNG_CNTR_SET2_E],
                                              meterColorMode[CPSS_POLICER_COLOR_AWARE_E],
                                              meterMode[CPSS_DXCH3_POLICER_METER_MODE_TR_TCM_E],
                                              tokenBucketParams.trTcmParams{cir[0],
                                                                            cbs[0],
                                                                            pir[0],
                                                                            pbs[0]},
                                              modifyUp[CPSS_PACKET_ATTRIBUTE_MODIFY_ENABLE_E],
                                              modifyDscp[CPSS_PACKET_ATTRIBUTE_MODIFY_ENABLE_E],
                                              yellowPcktCmd[CPSS_DXCH3_POLICER_NON_CONFORM_CMD_REMARK_E],
                                              redPcktCmd[CPSS_DXCH3_POLICER_NON_CONFORM_CMD_REMARK_E]}
                                   and not NULL tbParamsPtr.
            Expected: GT_OK.
        */

        /* iterate with max entryIndex */
        if (! PRV_CPSS_DXCH_XCAT_FAMILY_CHECK_MAC(dev))
        {
            entryIndex = PRV_CPSS_DXCH_PP_MAC(dev)->policer.memSize[0] - 1;
        }
        else
        {
            entryIndex = (PRV_CPSS_DXCH_PP_MAC(dev)->policer.memSize[plrStage] - 1);
        }

        entry.countingEntryIndex = 0xFF;
        entry.mngCounterSet      = CPSS_DXCH3_POLICER_MNG_CNTR_SET2_E;
        entry.meterColorMode     = CPSS_POLICER_COLOR_AWARE_E;
        entry.meterMode          = CPSS_DXCH3_POLICER_METER_MODE_TR_TCM_E;

        entry.tokenBucketParams.trTcmParams.cir = 0;
        entry.tokenBucketParams.trTcmParams.cbs = 0;
        entry.tokenBucketParams.trTcmParams.pir = 0;
        entry.tokenBucketParams.trTcmParams.pbs = 0;

        entry.modifyUp      = CPSS_PACKET_ATTRIBUTE_MODIFY_ENABLE_E;
        entry.modifyDscp    = CPSS_PACKET_ATTRIBUTE_MODIFY_ENABLE_E;
        entry.yellowPcktCmd = CPSS_DXCH3_POLICER_NON_CONFORM_CMD_REMARK_E;
        entry.redPcktCmd    = CPSS_DXCH3_POLICER_NON_CONFORM_CMD_REMARK_E;

        st = cpssDxCh3PolicerMeteringEntrySet(dev, plrStage, entryIndex, &entry, &tbParams);
        UTF_VERIFY_EQUAL2_PARAM_MAC(GT_OK, st, dev, entryIndex);

        /*
            1.2. Call cpssDxCh3PolicerMeteringEntryGet with not NULL entryPtr and other params from 1.1.
            Expected: GT_OK and the same entry as was set.
        */
        st = cpssDxCh3PolicerMeteringEntryGet(dev, plrStage, entryIndex, &entryGet);
        UTF_VERIFY_EQUAL2_STRING_MAC(GT_OK, st, "cpssDxCh3PolicerMeteringEntryGet: %d, %d", dev, entryIndex);

        /* verifying values */
        UTF_VERIFY_EQUAL1_STRING_MAC(entry.countingEntryIndex, entryGet.countingEntryIndex,
                   "got another entryPtr->countingEntryIndex then was set: %d", dev);
        UTF_VERIFY_EQUAL1_STRING_MAC(entry.mngCounterSet, entryGet.mngCounterSet,
                   "got another entryPtr->mngCounterSet then was set: %d", dev);
        UTF_VERIFY_EQUAL1_STRING_MAC(entry.meterColorMode, entryGet.meterColorMode,
                   "got another entryPtr->meterColorMode then was set: %d", dev);
        UTF_VERIFY_EQUAL1_STRING_MAC(entry.meterMode, entryGet.meterMode,
                   "got another entryPtr->meterMode then was set: %d", dev);

        UTF_VERIFY_EQUAL1_STRING_MAC(entry.tokenBucketParams.trTcmParams.cir, entryGet.tokenBucketParams.trTcmParams.cir,
                   "got another entryPtr->tokenBucketParams.trTcmParams.cir then was set: %d", dev);
        UTF_VERIFY_EQUAL1_STRING_MAC(entry.tokenBucketParams.trTcmParams.cbs, entryGet.tokenBucketParams.trTcmParams.cbs,
                   "got another entryPtr->tokenBucketParams.trTcmParams.cbs then was set: %d", dev);
        UTF_VERIFY_EQUAL1_STRING_MAC(entry.tokenBucketParams.trTcmParams.pir, entryGet.tokenBucketParams.trTcmParams.pir,
                   "got another entryPtr->tokenBucketParams.trTcmParams.pir then was set: %d", dev);
        UTF_VERIFY_EQUAL1_STRING_MAC(entry.tokenBucketParams.trTcmParams.pbs, entryGet.tokenBucketParams.trTcmParams.pbs,
                   "got another entryPtr->tokenBucketParams.trTcmParams.pbs then was set: %d", dev);

        UTF_VERIFY_EQUAL1_STRING_MAC(entry.modifyUp, entryGet.modifyUp,
                   "got another entryPtr->modifyUp then was set: %d", dev);
        UTF_VERIFY_EQUAL1_STRING_MAC(entry.modifyDscp, entryGet.modifyDscp,
                   "got another entryPtr->modifyDscp then was set: %d", dev);
        UTF_VERIFY_EQUAL1_STRING_MAC(entry.yellowPcktCmd, entryGet.yellowPcktCmd,
                   "got another entryPtr->yellowPcktCmd then was set: %d", dev);
        UTF_VERIFY_EQUAL1_STRING_MAC(entry.redPcktCmd, entryGet.redPcktCmd,
                   "got another entryPtr->redPcktCmd then was set: %d", dev);

        /*
            1.4. Call with out of range entryIndex
                           and other params from 1.1.
            Expected: NOT GT_OK.
        */
        if (! PRV_CPSS_DXCH_XCAT_FAMILY_CHECK_MAC(dev))
        {
            entryIndex = PRV_CPSS_DXCH_PP_MAC(dev)->policer.memSize[0];
        }
        else
        {
            entryIndex = PRV_CPSS_DXCH_PP_MAC(dev)->policer.memSize[plrStage];
        }

        st = cpssDxCh3PolicerMeteringEntrySet(dev, plrStage, entryIndex, &entry, &tbParams);
        UTF_VERIFY_NOT_EQUAL2_PARAM_MAC(GT_OK, st, dev, entryIndex);

        entryIndex = 0;

        /*
            1.5. Call with out of range entryPtr->mngCounterSet [0x5AAAAA5]
                           and other params from 1.1.
            Expected: GT_BAD_PARAM.
        */
        entry.mngCounterSet = POLICER_INVALID_ENUM_CNS;

        st = cpssDxCh3PolicerMeteringEntrySet(dev, plrStage, entryIndex, &entry, &tbParams);
        UTF_VERIFY_EQUAL2_STRING_MAC(GT_BAD_PARAM, st, "%d, entryPtr->mngCounterSet = %d",
                                     dev, entry.mngCounterSet);

        entry.mngCounterSet = CPSS_DXCH3_POLICER_MNG_CNTR_SET0_E;

        /*
            1.6. Call with out of range entryPtr->meterColorMode [0x5AAAAA5]
                           and other params from 1.1.
            Expected: GT_BAD_PARAM.
        */
        entry.meterColorMode = POLICER_INVALID_ENUM_CNS;

        st = cpssDxCh3PolicerMeteringEntrySet(dev, plrStage, entryIndex, &entry, &tbParams);
        UTF_VERIFY_EQUAL2_STRING_MAC(GT_BAD_PARAM, st, "%d, entryPtr->meterColorMode = %d",
                                     dev, entry.meterColorMode);

        entry.meterColorMode = CPSS_POLICER_COLOR_AWARE_E;

        /*
            1.7. Call with out of range entryPtr->meterMode [0x5AAAAA5]
                           and other params from 1.1.
            Expected: GT_BAD_PARAM.
        */
        entry.meterMode = POLICER_INVALID_ENUM_CNS;

        st = cpssDxCh3PolicerMeteringEntrySet(dev, plrStage, entryIndex, &entry, &tbParams);
        UTF_VERIFY_EQUAL2_STRING_MAC(GT_BAD_PARAM, st, "%d, entryPtr->meterMode = %d",
                                     dev, entry.meterMode);

        entry.meterMode = CPSS_DXCH3_POLICER_METER_MODE_SR_TCM_E;

        /*
            1.8. Call with out of range entryPtr->modifyUp [0x5AAAAA5]
                           and other params from 1.1.
            Expected: GT_BAD_PARAM.
        */
        entry.modifyUp = POLICER_INVALID_ENUM_CNS;

        st = cpssDxCh3PolicerMeteringEntrySet(dev, plrStage, entryIndex, &entry, &tbParams);
        UTF_VERIFY_EQUAL2_STRING_MAC(GT_BAD_PARAM, st, "%d, entryPtr->modifyUp = %d",
                                     dev, entry.modifyUp);

        entry.modifyUp = CPSS_PACKET_ATTRIBUTE_MODIFY_KEEP_PREVIOUS_E;

        /*
            1.9. Call with out of range entryPtr->modifyDscp [0x5AAAAA5]
                           and other params from 1.1.
            Expected: GT_BAD_PARAM.
        */
        entry.modifyDscp = POLICER_INVALID_ENUM_CNS;

        st = cpssDxCh3PolicerMeteringEntrySet(dev, plrStage, entryIndex, &entry, &tbParams);
        UTF_VERIFY_EQUAL2_STRING_MAC(GT_BAD_PARAM, st, "%d, entryPtr->modifyDscp = %d",
                                     dev, entry.modifyDscp);

        entry.modifyDscp = CPSS_PACKET_ATTRIBUTE_MODIFY_KEEP_PREVIOUS_E;

        /*
            1.10. Call with out of range entryPtr->yellowPcktCmd [0x5AAAAA5]
                            and other params from 1.1.
            Expected: GT_BAD_PARAM.
        */
        entry.yellowPcktCmd = POLICER_INVALID_ENUM_CNS;

        st = cpssDxCh3PolicerMeteringEntrySet(dev, plrStage, entryIndex, &entry, &tbParams);
        UTF_VERIFY_EQUAL2_STRING_MAC(GT_BAD_PARAM, st, "%d, entryPtr->yellowPcktCmd = %d",
                                     dev, entry.yellowPcktCmd);

        entry.yellowPcktCmd = CPSS_DXCH3_POLICER_NON_CONFORM_CMD_NO_CHANGE_E;

        /*
            1.11. Call with out of range entryPtr->redPcktCmd [0x5AAAAA5]
                            and other params from 1.1.
            Expected: GT_BAD_PARAM.
        */
        entry.redPcktCmd = POLICER_INVALID_ENUM_CNS;

        st = cpssDxCh3PolicerMeteringEntrySet(dev, plrStage, entryIndex, &entry, &tbParams);
        UTF_VERIFY_EQUAL2_STRING_MAC(GT_BAD_PARAM, st, "%d, entryPtr->redPcktCmd = %d",
                                     dev, entry.redPcktCmd);

        entry.redPcktCmd = CPSS_DXCH3_POLICER_NON_CONFORM_CMD_DROP_E;

        /*
            1.12. Call with entryPtr [NULL]
                            and other params from 1.1.
            Expected: GT_BAD_PTR.
        */
        st = cpssDxCh3PolicerMeteringEntrySet(dev, plrStage, entryIndex, NULL, &tbParams);
        UTF_VERIFY_EQUAL1_STRING_MAC(GT_BAD_PTR, st, "%d, entryPtr = NULL", dev);

        /*
            1.13. Call with tbParamsPtr [NULL]
                            and other params from 1.1.
            Expected: GT_BAD_PTR.
        */
        st = cpssDxCh3PolicerMeteringEntrySet(dev, plrStage, entryIndex, &entry, NULL);
        UTF_VERIFY_EQUAL1_STRING_MAC(GT_BAD_PTR, st, "%d, tbParamsPtr = NULL", dev);
    }

    entryIndex = 0;

    entry.countingEntryIndex = 0;
    entry.mngCounterSet      = CPSS_DXCH3_POLICER_MNG_CNTR_SET0_E;
    entry.meterColorMode     = CPSS_POLICER_COLOR_BLIND_E;
    entry.meterMode          = CPSS_DXCH3_POLICER_METER_MODE_SR_TCM_E;

    entry.tokenBucketParams.srTcmParams.cir = 0;
    entry.tokenBucketParams.srTcmParams.cbs = 0;
    entry.tokenBucketParams.srTcmParams.ebs = 0;

    entry.modifyUp      = CPSS_PACKET_ATTRIBUTE_MODIFY_KEEP_PREVIOUS_E;
    entry.modifyDscp    = CPSS_PACKET_ATTRIBUTE_MODIFY_KEEP_PREVIOUS_E;
    entry.yellowPcktCmd = CPSS_DXCH3_POLICER_NON_CONFORM_CMD_NO_CHANGE_E;
    entry.redPcktCmd    = CPSS_DXCH3_POLICER_NON_CONFORM_CMD_NO_CHANGE_E;

    /* 2. For not-active devices and devices from non-applicable family */
    /* check that function returns GT_BAD_PARAM.                        */

    /* prepare device iterator */
    PRV_TGF_NOT_APPLIC_DEV_RESET_MAC(&dev, UTF_CH1_E | UTF_CH1_DIAMOND_E | UTF_CH2_E);

    /* go over all non active devices */
    while(GT_OK == prvUtfNextDeviceGet(&dev, GT_FALSE))
    {
        st = cpssDxCh3PolicerMeteringEntrySet(dev, plrStage, entryIndex, &entry, &tbParams);
        UTF_VERIFY_EQUAL1_PARAM_MAC(GT_NOT_APPLICABLE_DEVICE, st, dev);
    }

    /* 3. Call function with out of bound value for device id.*/
    dev = PRV_CPSS_MAX_PP_DEVICES_CNS;

    st = cpssDxCh3PolicerMeteringEntrySet(dev, plrStage, entryIndex, &entry, &tbParams);
    UTF_VERIFY_EQUAL1_PARAM_MAC(GT_BAD_PARAM, st, dev);
}

/*----------------------------------------------------------------------------*/
/*
GT_STATUS cpssDxCh3PolicerMeteringEntryGet
(
    IN  GT_U8                                   devNum,
    IN  GT_U32                                  entryIndex,
    OUT CPSS_DXCH3_POLICER_METERING_ENTRY_STC   *entryPtr
)
*/
UTF_TEST_CASE_MAC(cpssDxCh3PolicerMeteringEntryGet)
{
/*
    ITERATE_DEVICES (DxCh3)
    1.1. Call with entryIndex [0 / maxIndex]
                   and not NULL entryPtr.
    Expected: GT_OK.
    1.2. Call with out of range entryIndex [maxIndex+1]
                   and other params from 1.1.
    Expected: NOT GT_OK.
    1.3. Call with entryPtr [NULL]
                   and other params from 1.1.
    Expected: GT_BAD_PTR.
*/
    GT_STATUS   st = GT_OK;
    GT_U8       dev;

    GT_U32                                  entryIndex = 0;
    CPSS_DXCH3_POLICER_METERING_ENTRY_STC   entry;


    cpssOsBzero((GT_VOID*) &entry, sizeof(entry));

    /* prepare device iterator */
    PRV_TGF_NOT_APPLIC_DEV_RESET_MAC(&dev, UTF_CH1_E | UTF_CH1_DIAMOND_E | UTF_CH2_E);

    /* 1. Go over all active devices. */
    while (GT_OK == prvUtfNextDeviceGet(&dev, GT_TRUE))
    {
        /*
            1.1. Call with entryIndex [0 / maxIndex]
                           and not NULL entryPtr.
            Expected: GT_OK.
        */
        /* iterate with entryIndex = 0 */
        entryIndex = 0;

        st = cpssDxCh3PolicerMeteringEntryGet(dev, plrStage, entryIndex, &entry);
        UTF_VERIFY_EQUAL2_PARAM_MAC(GT_OK, st, dev, entryIndex);

        /* iterate with max entryIndex */
        if (! PRV_CPSS_DXCH_XCAT_FAMILY_CHECK_MAC(dev))
        {
            entryIndex = PRV_CPSS_DXCH_PP_MAC(dev)->policer.memSize[0] - 1;
        }
        else
        {
            entryIndex = (PRV_CPSS_DXCH_PP_MAC(dev)->policer.memSize[plrStage] - 1);
        }

        st = cpssDxCh3PolicerMeteringEntryGet(dev, plrStage, entryIndex, &entry);
        UTF_VERIFY_EQUAL2_PARAM_MAC(GT_OK, st, dev, entryIndex);

        /*
            1.2. Call with out of range entryIndex
                           and other params from 1.1.
            Expected: NOT GT_OK.
        */
        if (! PRV_CPSS_DXCH_XCAT_FAMILY_CHECK_MAC(dev))
        {
            entryIndex = PRV_CPSS_DXCH_PP_MAC(dev)->policer.memSize[0];
        }
        else
        {
            entryIndex = PRV_CPSS_DXCH_PP_MAC(dev)->policer.memSize[plrStage];
        }

        st = cpssDxCh3PolicerMeteringEntryGet(dev, plrStage, entryIndex, &entry);
        UTF_VERIFY_NOT_EQUAL2_PARAM_MAC(GT_OK, st, dev, entryIndex);

        entryIndex = 0;

        /*
            1.3. Call with entryPtr [NULL]
                           and other params from 1.1.
            Expected: GT_BAD_PTR.
        */
        st = cpssDxCh3PolicerMeteringEntryGet(dev, plrStage, entryIndex, NULL);
        UTF_VERIFY_EQUAL1_STRING_MAC(GT_BAD_PTR, st, "%d, entryPtr = NULL ", dev);
    }

    entryIndex = 0;

    /* 2. For not-active devices and devices from non-applicable family */
    /* check that function returns GT_BAD_PARAM.                        */

    /* prepare device iterator */
    PRV_TGF_NOT_APPLIC_DEV_RESET_MAC(&dev, UTF_CH1_E | UTF_CH1_DIAMOND_E | UTF_CH2_E);

    /* go over all non active devices */
    while(GT_OK == prvUtfNextDeviceGet(&dev, GT_FALSE))
    {
        st = cpssDxCh3PolicerMeteringEntryGet(dev, plrStage, entryIndex, &entry);
        UTF_VERIFY_EQUAL1_PARAM_MAC(GT_NOT_APPLICABLE_DEVICE, st, dev);
    }

    /* 3. Call function with out of bound value for device id.*/
    dev = PRV_CPSS_MAX_PP_DEVICES_CNS;

    st = cpssDxCh3PolicerMeteringEntryGet(dev, plrStage, entryIndex, &entry);
    UTF_VERIFY_EQUAL1_PARAM_MAC(GT_BAD_PARAM, st, dev);
}

/*----------------------------------------------------------------------------*/
/*
GT_STATUS cpssDxCh3PolicerEntryMeterParamsCalculate
(
    IN  GT_U8                                   devNum,
    IN  CPSS_DXCH3_POLICER_METER_MODE_ENT       meterMode,
    IN  CPSS_DXCH3_POLICER_METER_TB_PARAMS_UNT  *tbInParamsPtr,
    OUT CPSS_DXCH3_POLICER_METER_TB_PARAMS_UNT  *tbOutParamsPtr
)
*/
UTF_TEST_CASE_MAC(cpssDxCh3PolicerEntryMeterParamsCalculate)
{
/*
    ITERATE_DEVICES (DxCh3)
    1.1. Call with meterMode[CPSS_DXCH3_POLICER_METER_MODE_SR_TCM_E],
                   tbInParamsPtr.srTcmParams[cir[0],
                                             cbs[0],
                                             ebs[0]]
                   and not NULL tbOutParamsPtr.
    Expected: GT_OK.
    1.2. Call with meterMode[CPSS_DXCH3_POLICER_METER_MODE_TR_TCM_E],
                   tbInParamsPtr.trTcmParams[cir[0],
                                             cbs[0],
                                             pir[0]
                                             pbs[0]]
                   and not NULL tbOutParamsPtr.
    Expected: GT_OK.
    1.3. Call with out of range meterMode [0x5AAAAA5]
                   and other params from 1.1.
    Expected: GT_BAD_PARAM.
    1.4. Call with tbInParamsPtr [NULL]
                   and other params from 1.1.
    Expected: GT_BAD_PTR.
    1.5. Call with tbOutParamsPtr [NULL]
                   and other params from 1.1.
    Expected: GT_BAD_PTR.
*/
    GT_STATUS   st = GT_OK;
    GT_U8       dev;

    CPSS_DXCH3_POLICER_METER_MODE_ENT       meterMode = CPSS_DXCH3_POLICER_METER_MODE_SR_TCM_E;
    CPSS_DXCH3_POLICER_METER_TB_PARAMS_UNT  tbInParams;
    CPSS_DXCH3_POLICER_METER_TB_PARAMS_UNT  tbOutParams;


    cpssOsBzero((GT_VOID*) &tbInParams, sizeof(tbInParams));
    cpssOsBzero((GT_VOID*) &tbOutParams, sizeof(tbOutParams));

    /* prepare device iterator */
    PRV_TGF_NOT_APPLIC_DEV_RESET_MAC(&dev, UTF_CH1_E | UTF_CH1_DIAMOND_E | UTF_CH2_E);

    /* 1. Go over all active devices. */
    while (GT_OK == prvUtfNextDeviceGet(&dev, GT_TRUE))
    {
        /*
            1.1. Call with meterMode[CPSS_DXCH3_POLICER_METER_MODE_SR_TCM_E],
                           tbInParamsPtr.srTcmParams[cir[0],
                                                     cbs[0],
                                                     ebs[0]]
                           and not NULL tbOutParamsPtr.
            Expected: GT_OK.
        */
        meterMode = CPSS_DXCH3_POLICER_METER_MODE_SR_TCM_E;

        tbInParams.srTcmParams.cir = 0;
        tbInParams.srTcmParams.cbs = 0;
        tbInParams.srTcmParams.ebs = 0;

        st = cpssDxCh3PolicerEntryMeterParamsCalculate(dev, plrStage, meterMode, &tbInParams, &tbOutParams);
        UTF_VERIFY_EQUAL2_PARAM_MAC(GT_OK, st, dev, meterMode);

        /*
            1.2. Call with meterMode[CPSS_DXCH3_POLICER_METER_MODE_TR_TCM_E],
                           tbInParamsPtr.trTcmParams[cir[0],
                                                     cbs[0],
                                                     pir[0]
                                                     pbs[0]]
                           and not NULL tbOutParamsPtr.
            Expected: GT_OK.
        */
        meterMode = CPSS_DXCH3_POLICER_METER_MODE_TR_TCM_E;

        tbInParams.trTcmParams.cir = 0;
        tbInParams.trTcmParams.cbs = 0;
        tbInParams.trTcmParams.pir = 0;
        tbInParams.trTcmParams.pbs = 0;

        st = cpssDxCh3PolicerEntryMeterParamsCalculate(dev, plrStage, meterMode, &tbInParams, &tbOutParams);
        UTF_VERIFY_EQUAL2_PARAM_MAC(GT_OK, st, dev, meterMode);

        /*
            1.3. Call with out of range meterMode[0x5AAAAA5]
                            and other params from 1.1.
            Expected: GT_BAD_PARAM.
        */
        meterMode = POLICER_INVALID_ENUM_CNS;

        st = cpssDxCh3PolicerEntryMeterParamsCalculate(dev, plrStage, meterMode, &tbInParams, &tbOutParams);
        UTF_VERIFY_EQUAL2_PARAM_MAC(GT_BAD_PARAM, st, dev, meterMode);

        meterMode = CPSS_DXCH3_POLICER_METER_MODE_SR_TCM_E;

        /*
            1.4. Call with tbInParamsPtr [NULL]
                           and other params from 1.1.
            Expected: GT_BAD_PTR.
        */
        st = cpssDxCh3PolicerEntryMeterParamsCalculate(dev, plrStage, meterMode, NULL, &tbOutParams);
        UTF_VERIFY_EQUAL1_STRING_MAC(GT_BAD_PTR, st, "%d, tbInParamsPtr = NULL", dev);

        /*
            1.5. Call with tbOutParamsPtr [NULL]
                           and other params from 1.1.
            Expected: GT_BAD_PTR.
        */
        st = cpssDxCh3PolicerEntryMeterParamsCalculate(dev, plrStage, meterMode, &tbInParams, NULL);
        UTF_VERIFY_EQUAL1_STRING_MAC(GT_BAD_PTR, st, "%d, tbOutParamsPtr = NULL", dev);
    }

    meterMode = CPSS_DXCH3_POLICER_METER_MODE_SR_TCM_E;

    tbInParams.srTcmParams.cir = 0;
    tbInParams.srTcmParams.cbs = 0;
    tbInParams.srTcmParams.ebs = 0;

    /* 2. For not-active devices and devices from non-applicable family */
    /* check that function returns GT_BAD_PARAM.                        */

    /* prepare device iterator */
    PRV_TGF_NOT_APPLIC_DEV_RESET_MAC(&dev, UTF_CH1_E | UTF_CH1_DIAMOND_E | UTF_CH2_E);

    /* go over all non active devices */
    while(GT_OK == prvUtfNextDeviceGet(&dev, GT_FALSE))
    {
        st = cpssDxCh3PolicerEntryMeterParamsCalculate(dev, plrStage, meterMode, &tbInParams, &tbOutParams);
        UTF_VERIFY_EQUAL1_PARAM_MAC(GT_NOT_APPLICABLE_DEVICE, st, dev);
    }

    /* 3. Call function with out of bound value for device id.*/
    dev = PRV_CPSS_MAX_PP_DEVICES_CNS;

    st = cpssDxCh3PolicerEntryMeterParamsCalculate(dev, plrStage, meterMode, &tbInParams, &tbOutParams);
    UTF_VERIFY_EQUAL1_PARAM_MAC(GT_BAD_PARAM, st, dev);
}

/*----------------------------------------------------------------------------*/
/*
GT_STATUS cpssDxCh3PolicerBillingEntrySet
(
    IN  GT_U8                                   devNum,
    IN  GT_U32                                  entryIndex,
    IN  CPSS_DXCH3_POLICER_BILLING_ENTRY_STC    *billingCntrPtr
)
*/
UTF_TEST_CASE_MAC(cpssDxCh3PolicerBillingEntrySet)
{
/*
    ITERATE_DEVICES (DxCh3)
    1.1. Call with entryIndex [0 / maxIndex],
                   billingCntrPtr{ greenCntr [0 / 0xFF],
                                   yellowCntr [0 / 0xFF],
                                   redCntr [0 / 0xFF]}
    Expected: GT_OK.
    1.2. Call cpssDxCh3PolicerBillingEntryGet with not NULL billingCntrPtr,
                                                   reset [GT_FALSE]
                                                   and other params from 1.1.
    Expected: GT_OK and the same billingCntr as was set
    1.3. Call with out of range entryIndex [maxIndex+1]
                   and other params from 1.1.
    Expected: NOT GT_OK.
    1.4. Call with billingCntr [NULL]
                   and other params from 1.1.
    Expected: GT_BAD_PTR.
*/
    GT_STATUS   st = GT_OK;
    GT_U8       dev;

    GT_U32                                  entryIndex = 0;
    CPSS_DXCH3_POLICER_BILLING_ENTRY_STC    billingCntr;
    CPSS_DXCH3_POLICER_BILLING_ENTRY_STC    billingCntrGet;
    GT_BOOL                                 reset      = GT_FALSE;
    CPSS_PP_FAMILY_TYPE_ENT     devFamily = CPSS_PP_FAMILY_CHEETAH_E;

    cpssOsBzero((GT_VOID*) &billingCntr, sizeof(billingCntr));
    cpssOsBzero((GT_VOID*) &billingCntrGet, sizeof(billingCntrGet));

    /* prepare device iterator */
    PRV_TGF_NOT_APPLIC_DEV_RESET_MAC(&dev, UTF_CH1_E | UTF_CH1_DIAMOND_E | UTF_CH2_E);

    /* 1. Go over all active devices. */
    while (GT_OK == prvUtfNextDeviceGet(&dev, GT_TRUE))
    {
        /*
            1.1. Call with entryIndex [0 / maxIndex],
                           billingCntrPtr{ greenCntr [0 / 0xFF],
                                           yellowCntr [0 / 0xFF],
                                           redCntr [0 / 0xFF]}
            Expected: GT_OK.
        */
       /* Getting device family */
        st = prvUtfDeviceFamilyGet(dev, &devFamily);
        UTF_VERIFY_EQUAL1_STRING_MAC(GT_OK, st, "prvUtfDeviceFamilyGet: %d", dev);

        /* iterate with entryIndex = 0 */
        if (devFamily > CPSS_PP_FAMILY_CHEETAH3_E)
        {   /* flush counting cache before Set counters */
            st = cpssDxChPolicerCountingWriteBackCacheFlush(dev, plrStage);
            UTF_VERIFY_EQUAL1_PARAM_MAC(GT_OK, st, dev);
        }

        entryIndex = 0;

        billingCntr.greenCntr.l[0]  = 0;
        billingCntr.yellowCntr.l[0] = 0;
        billingCntr.redCntr.l[0] = 0;

        st = cpssDxCh3PolicerBillingEntrySet(dev, plrStage, entryIndex, &billingCntr);
        UTF_VERIFY_EQUAL2_PARAM_MAC(GT_OK, st, dev, entryIndex);

        /*
            1.2. Call cpssDxCh3PolicerBillingEntryGet with not NULL billingCntrPtr,
                                                           reset [GT_FALSE]
                                                           and other params from 1.1.
            Expected: GT_OK and the same billingCntr as was set
        */
        st = cpssDxCh3PolicerBillingEntryGet(dev, plrStage, entryIndex, reset,
                                             &billingCntrGet);
        UTF_VERIFY_EQUAL3_STRING_MAC(GT_OK, st,
                   "cpssDxCh3PolicerBillingEntryGet: %d, %d, %d", dev, entryIndex, reset);

        /* verifying values */
        UTF_VERIFY_EQUAL1_STRING_MAC(billingCntr.greenCntr.l[0], billingCntrGet.greenCntr.l[0],
                   "got another billingCntrPtr->greenCntr then was set: %d", dev);
        UTF_VERIFY_EQUAL1_STRING_MAC(billingCntr.yellowCntr.l[0], billingCntrGet.yellowCntr.l[0],
                   "got another billingCntrPtr->yellowCntr then was set: %d", dev);
        UTF_VERIFY_EQUAL1_STRING_MAC(billingCntr.redCntr.l[0], billingCntrGet.redCntr.l[0],
                   "got another billingCntrPtr->redCntr then was set: %d", dev);


        /* iterate with max entryIndex */
        if (! PRV_CPSS_DXCH_XCAT_FAMILY_CHECK_MAC(dev))
        {
            entryIndex = PRV_CPSS_DXCH_PP_MAC(dev)->policer.memSize[0] - 1;
        }
        else
        {
            entryIndex = (PRV_CPSS_DXCH_PP_MAC(dev)->policer.memSize[plrStage] - 1);
        }

        billingCntr.greenCntr.l[0]  = 0xFF;
        billingCntr.yellowCntr.l[0] = 0xFF;
        billingCntr.redCntr.l[0] = 0xFF;

        if (devFamily > CPSS_PP_FAMILY_CHEETAH3_E)
        {   /* flush counting cache before Set counters */
            st = cpssDxChPolicerCountingWriteBackCacheFlush(dev, plrStage);
            UTF_VERIFY_EQUAL1_PARAM_MAC(GT_OK, st, dev);
        }

        st = cpssDxCh3PolicerBillingEntrySet(dev, plrStage, entryIndex, &billingCntr);
        UTF_VERIFY_EQUAL2_PARAM_MAC(GT_OK, st, dev, entryIndex);

        /*
            1.2. Call cpssDxCh3PolicerBillingEntryGet with not NULL billingCntrPtr,
                                                           reset [GT_FALSE]
                                                           and other params from 1.1.
            Expected: GT_OK and the same billingCntr as was set
        */
        st = cpssDxCh3PolicerBillingEntryGet(dev, plrStage, entryIndex, reset,
                                             &billingCntrGet);
        UTF_VERIFY_EQUAL3_STRING_MAC(GT_OK, st,
                   "cpssDxCh3PolicerBillingEntryGet: %d, %d, %d", dev, entryIndex, reset);

        /* verifying values */
        UTF_VERIFY_EQUAL1_STRING_MAC(billingCntr.greenCntr.l[0], billingCntrGet.greenCntr.l[0],
                   "got another billingCntrPtr->greenCntr then was set: %d", dev);
        UTF_VERIFY_EQUAL1_STRING_MAC(billingCntr.yellowCntr.l[0], billingCntrGet.yellowCntr.l[0],
                   "got another billingCntrPtr->yellowCntr then was set: %d", dev);
        UTF_VERIFY_EQUAL1_STRING_MAC(billingCntr.redCntr.l[0], billingCntrGet.redCntr.l[0],
                   "got another billingCntrPtr->redCntr then was set: %d", dev);


        /*
            1.3. Call with out of range entryIndex
                           and other params from 1.1.
            Expected: NOT GT_OK.
        */
        if (! PRV_CPSS_DXCH_XCAT_FAMILY_CHECK_MAC(dev))
        {
            entryIndex = PRV_CPSS_DXCH_PP_MAC(dev)->policer.memSize[0];
        }
        else
        {
            entryIndex = PRV_CPSS_DXCH_PP_MAC(dev)->policer.memSize[plrStage];
        }

        st = cpssDxCh3PolicerBillingEntrySet(dev, plrStage, entryIndex, &billingCntr);
        UTF_VERIFY_NOT_EQUAL2_PARAM_MAC(GT_OK, st, dev, entryIndex);

        entryIndex = 0;

        /*
            1.4. Call with billingCntrPtr [NULL]
                           and other params from 1.1.
            Expected: GT_BAD_PTR.
        */
        st = cpssDxCh3PolicerBillingEntrySet(dev, plrStage, entryIndex, NULL);
        UTF_VERIFY_EQUAL1_STRING_MAC(GT_BAD_PTR, st, "%d, billingCntrPtr = NULL" ,dev);


        /* Check different counter values for both MSB and LSB */

        /* Set Maximal values to both LSB and MSB (all msb:0x3FF lsb:0xFFFFFFFF) */
        billingCntr.greenCntr.l[0]  = 0xFFFFFFFF;
        billingCntr.yellowCntr.l[0] = 0xFFFFFFFF;
        billingCntr.redCntr.l[0] = 0xFFFFFFFF;

        billingCntr.greenCntr.l[1]  = 0x3FF;
        billingCntr.yellowCntr.l[1] = 0x3FF;
        billingCntr.redCntr.l[1] = 0x3FF;

        if (devFamily > CPSS_PP_FAMILY_CHEETAH3_E)
        {   /* flush counting cache before Set counters */
            st = cpssDxChPolicerCountingWriteBackCacheFlush(dev, plrStage);
            UTF_VERIFY_EQUAL1_PARAM_MAC(GT_OK, st, dev);
        }

        st = cpssDxCh3PolicerBillingEntrySet(dev, plrStage, entryIndex, &billingCntr);
        UTF_VERIFY_EQUAL2_PARAM_MAC(GT_OK, st, dev, entryIndex);

        /*
            1.5. Call cpssDxCh3PolicerBillingEntryGet with not NULL billingCntrPtr,
                                                           reset [GT_FALSE]
                                                           and other params from 1.1.
            Expected: GT_OK and the same billingCntr as was set
        */
        st = cpssDxCh3PolicerBillingEntryGet(dev, plrStage, entryIndex, reset,
                                             &billingCntrGet);
        UTF_VERIFY_EQUAL3_STRING_MAC(GT_OK, st,
                   "cpssDxCh3PolicerBillingEntryGet: %d, %d, %d", dev, entryIndex, reset);

        /* verifying values */
        UTF_VERIFY_EQUAL1_STRING_MAC(billingCntr.greenCntr.l[0], billingCntrGet.greenCntr.l[0],
                   "got another billingCntrPtr->greenCntr.l[0] then was set: %d", dev);
        UTF_VERIFY_EQUAL1_STRING_MAC(billingCntr.yellowCntr.l[0], billingCntrGet.yellowCntr.l[0],
                   "got another billingCntrPtr->yellowCntr.l[0] then was set: %d", dev);
        UTF_VERIFY_EQUAL1_STRING_MAC(billingCntr.redCntr.l[0], billingCntrGet.redCntr.l[0],
                   "got another billingCntrPtr->redCntr.l[0] then was set: %d", dev);

        if (PRV_CPSS_DXCH_XCAT_FAMILY_CHECK_MAC(dev))
        {
            UTF_VERIFY_EQUAL1_STRING_MAC(billingCntr.greenCntr.l[1], billingCntrGet.greenCntr.l[1],
                       "got another billingCntrPtr->greenCntr.l[1] then was set: %d", dev);
            UTF_VERIFY_EQUAL1_STRING_MAC(billingCntr.yellowCntr.l[1], billingCntrGet.yellowCntr.l[1],
                       "got another billingCntrPtr->yellowCntr.l[1] then was set: %d", dev);
            UTF_VERIFY_EQUAL1_STRING_MAC(billingCntr.redCntr.l[1], billingCntrGet.redCntr.l[1],
                       "got another billingCntrPtr->redCntr.l[1] then was set: %d", dev);
        }


        /* Set the following values:
           1. Green:    msb:0x3FF lsb:0xFFFFFFFF
           2. Yellow:   msb:0x0 lsb:0x0
           3. Red:      msb:0x3FF lsb:0xFFFFFFFF
        */
        billingCntr.greenCntr.l[0]  = 0xFFFFFFFF;
        billingCntr.yellowCntr.l[0] = 0x0;
        billingCntr.redCntr.l[0] = 0xFFFFFFFF;

        billingCntr.greenCntr.l[1]  = 0x3FF;
        billingCntr.yellowCntr.l[1] = 0x0;
        billingCntr.redCntr.l[1] = 0x3FF;

        if (devFamily > CPSS_PP_FAMILY_CHEETAH3_E)
        {   /* flush counting cache before Set counters */
            st = cpssDxChPolicerCountingWriteBackCacheFlush(dev, plrStage);
            UTF_VERIFY_EQUAL1_PARAM_MAC(GT_OK, st, dev);
        }

        st = cpssDxCh3PolicerBillingEntrySet(dev, plrStage, entryIndex, &billingCntr);
        UTF_VERIFY_EQUAL2_PARAM_MAC(GT_OK, st, dev, entryIndex);

        /*
            1.6. Call cpssDxCh3PolicerBillingEntryGet with not NULL billingCntrPtr,
                                                           reset [GT_FALSE]
                                                           and other params from 1.1.
            Expected: GT_OK and the same billingCntr as was set
        */
        st = cpssDxCh3PolicerBillingEntryGet(dev, plrStage, entryIndex, reset,
                                             &billingCntrGet);
        UTF_VERIFY_EQUAL3_STRING_MAC(GT_OK, st,
                   "cpssDxCh3PolicerBillingEntryGet: %d, %d, %d", dev, entryIndex, reset);

        /* verifying values */
        UTF_VERIFY_EQUAL1_STRING_MAC(billingCntr.greenCntr.l[0], billingCntrGet.greenCntr.l[0],
                   "got another billingCntrPtr->greenCntr.l[0] then was set: %d", dev);
        UTF_VERIFY_EQUAL1_STRING_MAC(billingCntr.yellowCntr.l[0], billingCntrGet.yellowCntr.l[0],
                   "got another billingCntrPtr->yellowCntr.l[0] then was set: %d", dev);
        UTF_VERIFY_EQUAL1_STRING_MAC(billingCntr.redCntr.l[0], billingCntrGet.redCntr.l[0],
                   "got another billingCntrPtr->redCntr.l[0] then was set: %d", dev);

        if (PRV_CPSS_DXCH_XCAT_FAMILY_CHECK_MAC(dev))
        {
            UTF_VERIFY_EQUAL1_STRING_MAC(billingCntr.greenCntr.l[1], billingCntrGet.greenCntr.l[1],
                       "got another billingCntrPtr->greenCntr.l[1] then was set: %d", dev);
            UTF_VERIFY_EQUAL1_STRING_MAC(billingCntr.yellowCntr.l[1], billingCntrGet.yellowCntr.l[1],
                       "got another billingCntrPtr->yellowCntr.l[1] then was set: %d", dev);
            UTF_VERIFY_EQUAL1_STRING_MAC(billingCntr.redCntr.l[1], billingCntrGet.redCntr.l[1],
                       "got another billingCntrPtr->redCntr.l[1] then was set: %d", dev);
        }

        /* Set the following values:
           1. Green:    msb:0x0 lsb:0x0
           2. Yellow:   msb:0x3FF lsb:0xFFFFFFFF
           3. Red:      msb:0x0 lsb:0x0
        */
        billingCntr.greenCntr.l[0]  = 0x0;
        billingCntr.yellowCntr.l[0] = 0xFFFFFFFF;
        billingCntr.redCntr.l[0] = 0x0;

        billingCntr.greenCntr.l[1]  = 0x0;
        billingCntr.yellowCntr.l[1] = 0x3FF;
        billingCntr.redCntr.l[1] = 0x0;

        if (devFamily > CPSS_PP_FAMILY_CHEETAH3_E)
        {   /* flush counting cache before Set counters */
            st = cpssDxChPolicerCountingWriteBackCacheFlush(dev, plrStage);
            UTF_VERIFY_EQUAL1_PARAM_MAC(GT_OK, st, dev);
        }

        st = cpssDxCh3PolicerBillingEntrySet(dev, plrStage, entryIndex, &billingCntr);
        UTF_VERIFY_EQUAL2_PARAM_MAC(GT_OK, st, dev, entryIndex);

        /*
            1.7. Call cpssDxCh3PolicerBillingEntryGet with not NULL billingCntrPtr,
                                                           reset [GT_FALSE]
                                                           and other params from 1.1.
            Expected: GT_OK and the same billingCntr as was set
        */
        st = cpssDxCh3PolicerBillingEntryGet(dev, plrStage, entryIndex, reset,
                                             &billingCntrGet);
        UTF_VERIFY_EQUAL3_STRING_MAC(GT_OK, st,
                   "cpssDxCh3PolicerBillingEntryGet: %d, %d, %d", dev, entryIndex, reset);

        /* verifying values */
        UTF_VERIFY_EQUAL1_STRING_MAC(billingCntr.greenCntr.l[0], billingCntrGet.greenCntr.l[0],
                   "got another billingCntrPtr->greenCntr.l[0] then was set: %d", dev);
        UTF_VERIFY_EQUAL1_STRING_MAC(billingCntr.yellowCntr.l[0], billingCntrGet.yellowCntr.l[0],
                   "got another billingCntrPtr->yellowCntr.l[0] then was set: %d", dev);
        UTF_VERIFY_EQUAL1_STRING_MAC(billingCntr.redCntr.l[0], billingCntrGet.redCntr.l[0],
                   "got another billingCntrPtr->redCntr.l[0] then was set: %d", dev);

        if (PRV_CPSS_DXCH_XCAT_FAMILY_CHECK_MAC(dev))
        {
            UTF_VERIFY_EQUAL1_STRING_MAC(billingCntr.greenCntr.l[1], billingCntrGet.greenCntr.l[1],
                       "got another billingCntrPtr->greenCntr.l[1] then was set: %d", dev);
            UTF_VERIFY_EQUAL1_STRING_MAC(billingCntr.yellowCntr.l[1], billingCntrGet.yellowCntr.l[1],
                       "got another billingCntrPtr->yellowCntr.l[1] then was set: %d", dev);
            UTF_VERIFY_EQUAL1_STRING_MAC(billingCntr.redCntr.l[1], billingCntrGet.redCntr.l[1],
                       "got another billingCntrPtr->redCntr.l[1] then was set: %d", dev);
        }

        /* Set the following values:
           1. Green:    msb:0x2AA lsb:0xAAAAAAAA
           2. Yellow:   msb:0x155 lsb:0x55555555
           3. Red:      msb:0x2AA lsb:0xAAAAAAAA
        */
        billingCntr.greenCntr.l[0]  = 0xAAAAAAAA;
        billingCntr.yellowCntr.l[0] = 0x55555555;
        billingCntr.redCntr.l[0] = 0xAAAAAAAA;

        billingCntr.greenCntr.l[1]  = 0x2AA;
        billingCntr.yellowCntr.l[1] = 0x155;
        billingCntr.redCntr.l[1] = 0x2AA;

        if (devFamily > CPSS_PP_FAMILY_CHEETAH3_E)
        {   /* flush counting cache before Set counters */
            st = cpssDxChPolicerCountingWriteBackCacheFlush(dev, plrStage);
            UTF_VERIFY_EQUAL1_PARAM_MAC(GT_OK, st, dev);
        }

        st = cpssDxCh3PolicerBillingEntrySet(dev, plrStage, entryIndex, &billingCntr);
        UTF_VERIFY_EQUAL2_PARAM_MAC(GT_OK, st, dev, entryIndex);

        /*
            1.8. Call cpssDxCh3PolicerBillingEntryGet with not NULL billingCntrPtr,
                                                           reset [GT_FALSE]
                                                           and other params from 1.1.
            Expected: GT_OK and the same billingCntr as was set
        */
        st = cpssDxCh3PolicerBillingEntryGet(dev, plrStage, entryIndex, reset,
                                             &billingCntrGet);
        UTF_VERIFY_EQUAL3_STRING_MAC(GT_OK, st,
                   "cpssDxCh3PolicerBillingEntryGet: %d, %d, %d", dev, entryIndex, reset);

        /* verifying values */
        UTF_VERIFY_EQUAL1_STRING_MAC(billingCntr.greenCntr.l[0], billingCntrGet.greenCntr.l[0],
                   "got another billingCntrPtr->greenCntr.l[0] then was set: %d", dev);
        UTF_VERIFY_EQUAL1_STRING_MAC(billingCntr.yellowCntr.l[0], billingCntrGet.yellowCntr.l[0],
                   "got another billingCntrPtr->yellowCntr.l[0] then was set: %d", dev);
        UTF_VERIFY_EQUAL1_STRING_MAC(billingCntr.redCntr.l[0], billingCntrGet.redCntr.l[0],
                   "got another billingCntrPtr->redCntr.l[0] then was set: %d", dev);

        if (PRV_CPSS_DXCH_XCAT_FAMILY_CHECK_MAC(dev))
        {
            UTF_VERIFY_EQUAL1_STRING_MAC(billingCntr.greenCntr.l[1], billingCntrGet.greenCntr.l[1],
                       "got another billingCntrPtr->greenCntr.l[1] then was set: %d", dev);
            UTF_VERIFY_EQUAL1_STRING_MAC(billingCntr.yellowCntr.l[1], billingCntrGet.yellowCntr.l[1],
                       "got another billingCntrPtr->yellowCntr.l[1] then was set: %d", dev);
            UTF_VERIFY_EQUAL1_STRING_MAC(billingCntr.redCntr.l[1], billingCntrGet.redCntr.l[1],
                       "got another billingCntrPtr->redCntr.l[1] then was set: %d", dev);
        }

        /* Set the following values:
           1. Green:    msb:0x2AA lsb:0x55555555
           2. Yellow:   msb:0x155 lsb:0xAAAAAAAA
           3. Red:      msb:0x2AA lsb:0x55555555
        */
        billingCntr.greenCntr.l[0]  = 0x55555555;
        billingCntr.yellowCntr.l[0] = 0xAAAAAAAA;
        billingCntr.redCntr.l[0] = 0x55555555;

        billingCntr.greenCntr.l[1]  = 0x155;
        billingCntr.yellowCntr.l[1] = 0x2AA;
        billingCntr.redCntr.l[1] = 0x155;

        if (devFamily > CPSS_PP_FAMILY_CHEETAH3_E)
        {   /* flush counting cache before Set counters */
            st = cpssDxChPolicerCountingWriteBackCacheFlush(dev, plrStage);
            UTF_VERIFY_EQUAL1_PARAM_MAC(GT_OK, st, dev);
        }

        st = cpssDxCh3PolicerBillingEntrySet(dev, plrStage, entryIndex, &billingCntr);
        UTF_VERIFY_EQUAL2_PARAM_MAC(GT_OK, st, dev, entryIndex);

        /*
            1.9. Call cpssDxCh3PolicerBillingEntryGet with not NULL billingCntrPtr,
                                                           reset [GT_FALSE]
                                                           and other params from 1.1.
            Expected: GT_OK and the same billingCntr as was set
        */
        st = cpssDxCh3PolicerBillingEntryGet(dev, plrStage, entryIndex, reset,
                                             &billingCntrGet);
        UTF_VERIFY_EQUAL3_STRING_MAC(GT_OK, st,
                   "cpssDxCh3PolicerBillingEntryGet: %d, %d, %d", dev, entryIndex, reset);

        /* verifying values */
        UTF_VERIFY_EQUAL1_STRING_MAC(billingCntr.greenCntr.l[0], billingCntrGet.greenCntr.l[0],
                   "got another billingCntrPtr->greenCntr.l[0] then was set: %d", dev);
        UTF_VERIFY_EQUAL1_STRING_MAC(billingCntr.yellowCntr.l[0], billingCntrGet.yellowCntr.l[0],
                   "got another billingCntrPtr->yellowCntr.l[0] then was set: %d", dev);
        UTF_VERIFY_EQUAL1_STRING_MAC(billingCntr.redCntr.l[0], billingCntrGet.redCntr.l[0],
                   "got another billingCntrPtr->redCntr.l[0] then was set: %d", dev);

        if (PRV_CPSS_DXCH_XCAT_FAMILY_CHECK_MAC(dev))
        {
            UTF_VERIFY_EQUAL1_STRING_MAC(billingCntr.greenCntr.l[1], billingCntrGet.greenCntr.l[1],
                       "got another billingCntrPtr->greenCntr.l[1] then was set: %d", dev);
            UTF_VERIFY_EQUAL1_STRING_MAC(billingCntr.yellowCntr.l[1], billingCntrGet.yellowCntr.l[1],
                       "got another billingCntrPtr->yellowCntr.l[1] then was set: %d", dev);
            UTF_VERIFY_EQUAL1_STRING_MAC(billingCntr.redCntr.l[1], billingCntrGet.redCntr.l[1],
                       "got another billingCntrPtr->redCntr.l[1] then was set: %d", dev);
        }


        /*
            1.10. Call cpssDxCh3PolicerBillingEntrySet with not NULL
                                                           billingCntrPtr.billingCntrMode,
                                                           reset [GT_FALSE]
                                                           and other params from 1.1.
            Expected: GT_OK and the same billingCntr as was set
        */

        if (PRV_CPSS_DXCH_XCAT_FAMILY_CHECK_MAC(dev))
        {
            /* Billing counter mode is CPSS_DXCH3_POLICER_BILLING_CNTR_1_BYTE_E */
            billingCntr.greenCntr.l[0]  = 0x0;
            billingCntr.yellowCntr.l[0] = 0x0;
            billingCntr.redCntr.l[0] = 0x0;

            billingCntr.greenCntr.l[1]  = 0x0;
            billingCntr.yellowCntr.l[1] = 0x0;
            billingCntr.redCntr.l[1] = 0x0;

            billingCntr.billingCntrMode = CPSS_DXCH3_POLICER_BILLING_CNTR_1_BYTE_E;

            for (; billingCntr.billingCntrMode <= CPSS_DXCH3_POLICER_BILLING_CNTR_PACKET_E;
                 billingCntr.billingCntrMode++)
            {
                /* flush counting cache before Set counters */
                st = cpssDxChPolicerCountingWriteBackCacheFlush(dev, plrStage);
                UTF_VERIFY_EQUAL1_PARAM_MAC(GT_OK, st, dev);


                st = cpssDxCh3PolicerBillingEntrySet(dev, plrStage, entryIndex, &billingCntr);
                UTF_VERIFY_EQUAL2_PARAM_MAC(GT_OK, st, dev, entryIndex);

                /*
                    1.11. Call cpssDxCh3PolicerBillingEntryGet with not NULL billingCntrPtr,
                                                                   reset [GT_FALSE]
                                                                   and other params from 1.1.
                    Expected: GT_OK and the same billingCntr as was set
                */
                st = cpssDxCh3PolicerBillingEntryGet(dev, plrStage, entryIndex, reset,
                                                     &billingCntrGet);
                UTF_VERIFY_EQUAL3_STRING_MAC(GT_OK, st,
                           "cpssDxCh3PolicerBillingEntryGet: %d, %d, %d", dev, entryIndex, reset);

                UTF_VERIFY_EQUAL1_STRING_MAC(billingCntr.billingCntrMode, billingCntrGet.billingCntrMode,
                           "got another billingCntrPtr->billingCntrMode then was set: %d", dev);
            }
        }
    }

    entryIndex = 0;

    billingCntr.greenCntr.l[0]  = 0;
    billingCntr.yellowCntr.l[0] = 0;
    billingCntr.redCntr.l[0] = 0;

    /* 2. For not-active devices and devices from non-applicable family */
    /* check that function returns GT_BAD_PARAM.                        */

    /* prepare device iterator */
    PRV_TGF_NOT_APPLIC_DEV_RESET_MAC(&dev, UTF_CH1_E | UTF_CH1_DIAMOND_E | UTF_CH2_E);

    /* go over all non active devices */
    while(GT_OK == prvUtfNextDeviceGet(&dev, GT_FALSE))
    {
        st = cpssDxCh3PolicerBillingEntrySet(dev, plrStage, entryIndex, &billingCntr);
        UTF_VERIFY_EQUAL1_PARAM_MAC(GT_NOT_APPLICABLE_DEVICE, st, dev);
    }

    /* 3. Call function with out of bound value for device id.*/
    dev = PRV_CPSS_MAX_PP_DEVICES_CNS;

    st = cpssDxCh3PolicerBillingEntrySet(dev, plrStage, entryIndex, &billingCntr);
    UTF_VERIFY_EQUAL1_PARAM_MAC(GT_BAD_PARAM, st, dev);
}

/*----------------------------------------------------------------------------*/
/*
GT_STATUS cpssDxCh3PolicerBillingEntryGet
(
    IN  GT_U8                                   devNum,
    IN  GT_U32                                  entryIndex,
    IN  GT_BOOL                                 reset,
    OUT CPSS_DXCH3_POLICER_BILLING_ENTRY_STC    *billingCntrPtr
)
*/
UTF_TEST_CASE_MAC(cpssDxCh3PolicerBillingEntryGet)
{
/*
    ITERATE_DEVICES (DxCh3)
    1.1. Call with entryIndex [0 / maxIndex],
                   reset [GT_FALSE / GT_TRUE]
                   and not NULL billingCntrPtr.
    Expected: GT_OK.
    1.2. Call with out of range entryIndex [maxIndex+1]
                   and other params from 1.1.
    Expected: NOT GT_OK.
    1.3. Call with billingCntrPtr [NULL]
                   and other params from 1.1.
    Expected: GT_BAD_PTR.
*/
    GT_STATUS   st = GT_OK;
    GT_U8       dev;

    GT_U32                                  entryIndex = 0;
    GT_BOOL                                 reset      = GT_FALSE;
    CPSS_DXCH3_POLICER_BILLING_ENTRY_STC    billingCntr;


    cpssOsBzero((GT_VOID*) &billingCntr, sizeof(billingCntr));

    /* prepare device iterator */
    PRV_TGF_NOT_APPLIC_DEV_RESET_MAC(&dev, UTF_CH1_E | UTF_CH1_DIAMOND_E | UTF_CH2_E);

    /* 1. Go over all active devices. */
    while (GT_OK == prvUtfNextDeviceGet(&dev, GT_TRUE))
    {
        /*
            1.1. Call with entryIndex [0 / maxIndex],
                           reset [GT_FALSE / GT_TRUE]
                           and not NULL billingCntrPtr.
            Expected: GT_OK
        */
        /* iterate with entryIndex = 0 */
        entryIndex = 0;
        reset      = GT_FALSE;

        st = cpssDxCh3PolicerBillingEntryGet(dev, plrStage, entryIndex, reset,
                                             &billingCntr);
        UTF_VERIFY_EQUAL2_PARAM_MAC(GT_OK, st, dev, entryIndex);

        /* iterate with max entryIndex */
        if (! PRV_CPSS_DXCH_XCAT_FAMILY_CHECK_MAC(dev))
        {
            entryIndex = PRV_CPSS_DXCH_PP_MAC(dev)->policer.memSize[0] - 1;
        }
        else
        {
            entryIndex = (PRV_CPSS_DXCH_PP_MAC(dev)->policer.memSize[plrStage] - 1);
        }

        reset      = GT_TRUE;

        st = cpssDxCh3PolicerBillingEntryGet(dev, plrStage, entryIndex, reset,
                                             &billingCntr);
        UTF_VERIFY_EQUAL2_PARAM_MAC(GT_OK, st, dev, entryIndex);

        /*
            1.2. Call with out of range entryIndex
                           and other params from 1.1.
            Expected: NOT GT_OK.
        */
        if (! PRV_CPSS_DXCH_XCAT_FAMILY_CHECK_MAC(dev))
        {
            entryIndex = PRV_CPSS_DXCH_PP_MAC(dev)->policer.memSize[0];
        }
        else
        {
            entryIndex = PRV_CPSS_DXCH_PP_MAC(dev)->policer.memSize[plrStage];
        }

        st = cpssDxCh3PolicerBillingEntryGet(dev, plrStage, entryIndex, reset,
                                             &billingCntr);
        UTF_VERIFY_NOT_EQUAL2_PARAM_MAC(GT_OK, st, dev, entryIndex);

        entryIndex = 0;

        /*
            1.3. Call with billingCntrPtr [NULL]
                           and other params from 1.1.
            Expected: GT_BAD_PTR.
        */
        st = cpssDxCh3PolicerBillingEntryGet(dev, plrStage, entryIndex, reset, NULL);
        UTF_VERIFY_EQUAL1_STRING_MAC(GT_BAD_PTR, st, "%d, billingCntrPtr = NULL", dev);
    }

    entryIndex = 0;

    /* 2. For not-active devices and devices from non-applicable family */
    /* check that function returns GT_BAD_PARAM.                        */

    /* prepare device iterator */
    PRV_TGF_NOT_APPLIC_DEV_RESET_MAC(&dev, UTF_CH1_E | UTF_CH1_DIAMOND_E | UTF_CH2_E);

    /* go over all non active devices */
    while(GT_OK == prvUtfNextDeviceGet(&dev, GT_FALSE))
    {
        st = cpssDxCh3PolicerBillingEntryGet(dev, plrStage, entryIndex, reset,
                                             &billingCntr);
        UTF_VERIFY_EQUAL1_PARAM_MAC(GT_NOT_APPLICABLE_DEVICE, st, dev);
    }

    /* 3. Call function with out of bound value for device id.*/
    dev = PRV_CPSS_MAX_PP_DEVICES_CNS;

    st = cpssDxCh3PolicerBillingEntryGet(dev, plrStage, entryIndex, reset, &billingCntr);
    UTF_VERIFY_EQUAL1_PARAM_MAC(GT_BAD_PARAM, st, dev);
}

/*----------------------------------------------------------------------------*/
/*
GT_STATUS cpssDxCh3PolicerQosRemarkingEntrySet
(
    IN GT_U8    devNum,
    IN GT_U32   qosProfileIndex,
    IN GT_U32   yellowQosTableRemarkIndex,
    IN GT_U32   redQosTableRemarkIndex
)
*/
UTF_TEST_CASE_MAC(cpssDxCh3PolicerQosRemarkingEntrySet)
{
/*
    ITERATE_DEVICES (DxCh3)
    1.1. Call function with qosProfileIndex [0 / 7],
                            yellowQosTableRemarkIndex [0 / 127],
                            redQosTableRemarkIndex [0 / 127]
    Expected: GT_OK.
    1.2. Call cpssDxCh3PolicerQosRemarkingEntryGet with not NULL yellowQosTableRemarkIndexandPtr
                                                        and not NULL redQosTableRemarkIndexPtr.
    Expected: GT_OK and the same yellowQosTableRemarkIndexand, redQosTableRemarkIndex as was set.
    1.3. Call with out of range yellowQosTableRemarkIndex [128]
                   and other params from 1.1.
    Expected: NOT GT_OK.
    1.4. Call with out of range redQosTableRemarkIndex [128]
                   and other params from 1.1.
    Expected: NOT GT_OK.
*/
    GT_STATUS   st = GT_OK;
    GT_U8       dev;

    GT_U32   qosProfileIndex              = 0;
    GT_U32   yellowQosTableRemarkIndex    = 0;
    GT_U32   redQosTableRemarkIndex       = 0;
    GT_U32   yellowQosTableRemarkIndexGet = 0;
    GT_U32   redQosTableRemarkIndexGet    = 0;


    /* prepare device iterator */
    PRV_TGF_NOT_APPLIC_DEV_RESET_MAC(&dev, UTF_CH1_E | UTF_CH1_DIAMOND_E | UTF_CH2_E);

    /* 1. Go over all active devices. */
    while (GT_OK == prvUtfNextDeviceGet(&dev, GT_TRUE))
    {
        /*
            1.1. Call function with qosProfileIndex [0 / 7],
                                    yellowQosTableRemarkIndex [0 / 127],
                                    redQosTableRemarkIndex [0 / 127]
            Expected: GT_OK.
        */
        /* iterate with qosProfileIndex = 0*/
        qosProfileIndex           = 0;
        yellowQosTableRemarkIndex = 0;
        redQosTableRemarkIndex    = 0;

        st = cpssDxCh3PolicerQosRemarkingEntrySet(dev, plrStage, qosProfileIndex,
                                                  yellowQosTableRemarkIndex, redQosTableRemarkIndex);
        UTF_VERIFY_EQUAL4_PARAM_MAC(GT_OK, st, dev, qosProfileIndex,
                                    yellowQosTableRemarkIndex, redQosTableRemarkIndex);

        /*
            1.2. Call cpssDxCh3PolicerQosRemarkingEntryGet with not NULL yellowQosTableRemarkIndexandPtr
                                                                and not NULL redQosTableRemarkIndexPtr.
            Expected: GT_OK and the same yellowQosTableRemarkIndexand, redQosTableRemarkIndex as was set.
        */
        st = cpssDxCh3PolicerQosRemarkingEntryGet(dev, plrStage, qosProfileIndex,
                                                  &yellowQosTableRemarkIndexGet, &redQosTableRemarkIndexGet);
        UTF_VERIFY_EQUAL2_STRING_MAC(GT_OK, st,
                   "cpssDxCh3PolicerQosRemarkingEntryGet: %d, %d", dev, qosProfileIndex);

        /* verifying values */
        UTF_VERIFY_EQUAL1_STRING_MAC(yellowQosTableRemarkIndex, yellowQosTableRemarkIndexGet,
                   "got another yellowQosTableRemarkIndex then was set: %d", dev);
        UTF_VERIFY_EQUAL1_STRING_MAC(redQosTableRemarkIndex, redQosTableRemarkIndexGet,
                   "got another redQosTableRemarkIndex then was set: %d", dev);

        /* iterate with qosProfileIndex = 7 */
        qosProfileIndex           = 7;
        yellowQosTableRemarkIndex = 127;
        redQosTableRemarkIndex    = 127;

        st = cpssDxCh3PolicerQosRemarkingEntrySet(dev, plrStage, qosProfileIndex,
                                                  yellowQosTableRemarkIndex, redQosTableRemarkIndex);
        UTF_VERIFY_EQUAL4_PARAM_MAC(GT_OK, st, dev, qosProfileIndex,
                                    yellowQosTableRemarkIndex, redQosTableRemarkIndex);

        /*
            1.2. Call cpssDxCh3PolicerQosRemarkingEntryGet with not NULL yellowQosTableRemarkIndexandPtr
                                                                and not NULL redQosTableRemarkIndexPtr.
            Expected: GT_OK and the same yellowQosTableRemarkIndexand, redQosTableRemarkIndex as was set.
        */
        st = cpssDxCh3PolicerQosRemarkingEntryGet(dev, plrStage, qosProfileIndex,
                                                  &yellowQosTableRemarkIndexGet, &redQosTableRemarkIndexGet);
        UTF_VERIFY_EQUAL2_STRING_MAC(GT_OK, st,
                   "cpssDxCh3PolicerQosRemarkingEntryGet: %d, %d", dev, qosProfileIndex);

        /* verifying values */
        UTF_VERIFY_EQUAL1_STRING_MAC(yellowQosTableRemarkIndex, yellowQosTableRemarkIndexGet,
                   "got another yellowQosTableRemarkIndex then was set: %d", dev);
        UTF_VERIFY_EQUAL1_STRING_MAC(redQosTableRemarkIndex, redQosTableRemarkIndexGet,
                   "got another redQosTableRemarkIndex then was set: %d", dev);

        /*
            1.3. Call with out of range yellowQosTableRemarkIndex [128]
                           and other params from 1.1.
            Expected: NOT GT_OK.
        */
        yellowQosTableRemarkIndex = 128;

        st = cpssDxCh3PolicerQosRemarkingEntrySet(dev, plrStage, qosProfileIndex,
                                                  yellowQosTableRemarkIndex, redQosTableRemarkIndex);
        UTF_VERIFY_NOT_EQUAL2_STRING_MAC(GT_OK, st, "%d, yellowQosTableRemarkIndex = %d",
                                         dev, yellowQosTableRemarkIndex);

        yellowQosTableRemarkIndex = 0;

        /*
            1.4. Call with out of range redQosTableRemarkIndex [128]
                           and other params from 1.1.
            Expected: NOT GT_OK.
        */
        redQosTableRemarkIndex = 128;

        st = cpssDxCh3PolicerQosRemarkingEntrySet(dev, plrStage, qosProfileIndex,
                                                  yellowQosTableRemarkIndex, redQosTableRemarkIndex);
        UTF_VERIFY_NOT_EQUAL2_STRING_MAC(GT_OK, st, "%d, redQosTableRemarkIndex = %d",
                                         dev, redQosTableRemarkIndex);
    }

    qosProfileIndex           = 0;
    yellowQosTableRemarkIndex = 0;
    redQosTableRemarkIndex    = 0;

    /* 2. For not-active devices and devices from non-applicable family */
    /* check that function returns GT_BAD_PARAM.                        */

    /* prepare device iterator */
    PRV_TGF_NOT_APPLIC_DEV_RESET_MAC(&dev, UTF_CH1_E | UTF_CH1_DIAMOND_E | UTF_CH2_E);

    /* go over all non active devices */
    while(GT_OK == prvUtfNextDeviceGet(&dev, GT_FALSE))
    {
        st = cpssDxCh3PolicerQosRemarkingEntrySet(dev, plrStage, qosProfileIndex,
                                                  yellowQosTableRemarkIndex, redQosTableRemarkIndex);
        UTF_VERIFY_EQUAL1_PARAM_MAC(GT_NOT_APPLICABLE_DEVICE, st, dev);
    }

    /* 3. Call function with out of bound value for device id.*/
    dev = PRV_CPSS_MAX_PP_DEVICES_CNS;

    st = cpssDxCh3PolicerQosRemarkingEntrySet(dev, plrStage, qosProfileIndex,
                                              yellowQosTableRemarkIndex, redQosTableRemarkIndex);
    UTF_VERIFY_EQUAL1_PARAM_MAC(GT_BAD_PARAM, st, dev);
}

/*----------------------------------------------------------------------------*/
/*
GT_STATUS cpssDxCh3PolicerQosRemarkingEntryGet
(
    IN  GT_U8   devNum,
    IN GT_U32   qosProfileIndex,
    OUT GT_U32  *yellowQosTableRemarkIndexPtr,
    OUT GT_U32  *redQosTableRemarkIndexPtr
)
*/
UTF_TEST_CASE_MAC(cpssDxCh3PolicerQosRemarkingEntryGet)
{
/*
    ITERATE_DEVICES (DxCh3)
    1.1. Call with qosProfileIndex [0]
                   not NULL yellowQosTableRemarkIndexPtr
                   and not NULL redQosTableRemarkIndexPtr.
    Expected: GT_OK.
    1.2. Call with yellowQosTableRemarkIndexPtr [NULL]
                   and other params from 1.1.
    Expected: GT_BAD_PTR.
    1.3. Call with redQosTableRemarkIndexPtr [NULL]
                   and other params from 1.1.
    Expected: GT_BAD_PTR.
*/
    GT_STATUS   st = GT_OK;
    GT_U8       dev;

    GT_U32   qosProfileIndex           = 0;
    GT_U32   yellowQosTableRemarkIndex = 0;
    GT_U32   redQosTableRemarkIndex    = 0;


    /* prepare device iterator */
    PRV_TGF_NOT_APPLIC_DEV_RESET_MAC(&dev, UTF_CH1_E | UTF_CH1_DIAMOND_E | UTF_CH2_E);

    /* 1. Go over all active devices. */
    while (GT_OK == prvUtfNextDeviceGet(&dev, GT_TRUE))
    {
        /*
            1.1. Call with qosProfileIndex [0]
                           not NULL yellowQosTableRemarkIndexPtr
                           and not NULL redQosTableRemarkIndexPtr.
            Expected: GT_OK.
        */
        qosProfileIndex = 0;

        st = cpssDxCh3PolicerQosRemarkingEntryGet(dev, plrStage, qosProfileIndex,
                                                  &yellowQosTableRemarkIndex, &redQosTableRemarkIndex);
        UTF_VERIFY_EQUAL2_PARAM_MAC(GT_OK, st, dev, qosProfileIndex);

        /*
            1.2. Call with yellowQosTableRemarkIndexPtr [NULL]
                           and other params from 1.1.
            Expected: GT_BAD_PTR.
        */
        st = cpssDxCh3PolicerQosRemarkingEntryGet(dev, plrStage, qosProfileIndex,
                                                  NULL, &redQosTableRemarkIndex);
        UTF_VERIFY_EQUAL1_STRING_MAC(GT_BAD_PTR, st, "%d, yellowQosTableRemarkIndexPtr = NULL", dev);

        /*
            1.3. Call with redQosTableRemarkIndexPtr [NULL]
                           and other params from 1.1.
            Expected: GT_BAD_PTR.
        */
        st = cpssDxCh3PolicerQosRemarkingEntryGet(dev, plrStage, qosProfileIndex,
                                                  &yellowQosTableRemarkIndex, NULL);
        UTF_VERIFY_EQUAL1_STRING_MAC(GT_BAD_PTR, st, "%d, redQosTableRemarkIndex = NULL", dev);
    }

    qosProfileIndex = 0;

    /* 2. For not-active devices and devices from non-applicable family */
    /* check that function returns GT_BAD_PARAM.                        */

    /* prepare device iterator */
    PRV_TGF_NOT_APPLIC_DEV_RESET_MAC(&dev, UTF_CH1_E | UTF_CH1_DIAMOND_E | UTF_CH2_E);

    /* go over all non active devices */
    while(GT_OK == prvUtfNextDeviceGet(&dev, GT_FALSE))
    {
        st = cpssDxCh3PolicerQosRemarkingEntryGet(dev, plrStage, qosProfileIndex,
                                                  &yellowQosTableRemarkIndex, &redQosTableRemarkIndex);
        UTF_VERIFY_EQUAL1_PARAM_MAC(GT_NOT_APPLICABLE_DEVICE, st, dev);
    }

    /* 3. Call function with out of bound value for device id.*/
    dev = PRV_CPSS_MAX_PP_DEVICES_CNS;

    st = cpssDxCh3PolicerQosRemarkingEntryGet(dev, plrStage, qosProfileIndex,
                                              &yellowQosTableRemarkIndex, &redQosTableRemarkIndex);
    UTF_VERIFY_EQUAL1_PARAM_MAC(GT_BAD_PARAM, st, dev);
}


/*----------------------------------------------------------------------------*/
/*
GT_STATUS cpssDxChPolicerCountingModeGet
(
    IN  GT_U8                                devNum,
    IN  CPSS_DXCH_POLICER_STAGE_TYPE_ENT     stage,
    OUT CPSS_DXCH_POLICER_COUNTING_MODE_ENT  *modePtr
)
*/
UTF_TEST_CASE_MAC(cpssDxChPolicerCountingModeGet)
{
/*
    ITERATE_DEVICES(DxChXcat and above)
    1.1. Call function with stage [CPSS_DXCH_POLICER_STAGE_INGRESS_0_E
                                   CPSS_DXCH_POLICER_STAGE_INGRESS_1_E /
                                   CPSS_DXCH_POLICER_STAGE_EGRESS_E],
    Expected: GT_OK.
    1.2. Call api with wrong stage [wrong enum values].
    Expected: GT_BAD_PARAM.
    1.3. Call api with wrong modePtr [NULL].
    Expected: GT_BAD_PTR.
*/
    GT_STATUS st = GT_OK;

    GT_U8                                dev;
    CPSS_DXCH_POLICER_STAGE_TYPE_ENT     stage = CPSS_DXCH_POLICER_STAGE_INGRESS_0_E;
    CPSS_DXCH_POLICER_COUNTING_MODE_ENT  mode;

    /* prepare device iterator */
    PRV_TGF_NOT_APPLIC_DEV_RESET_MAC(&dev, UTF_CH1_E | UTF_CH1_DIAMOND_E | UTF_CH2_E | UTF_CH3_E);

    /* 1. Go over all active devices. */
    while (GT_OK == prvUtfNextDeviceGet(&dev, GT_TRUE))
    {
        /*
            1.1. Call function with stage [CPSS_DXCH_POLICER_STAGE_INGRESS_0_E
                                           CPSS_DXCH_POLICER_STAGE_INGRESS_1_E /
                                           CPSS_DXCH_POLICER_STAGE_EGRESS_E],
            Expected: GT_OK.
        */
        /*call with stage = CPSS_DXCH_POLICER_STAGE_INGRESS_0_E;*/
        stage = CPSS_DXCH_POLICER_STAGE_INGRESS_0_E;

        st = cpssDxChPolicerCountingModeGet(dev, stage, &mode);
        UTF_VERIFY_EQUAL1_PARAM_MAC(GT_OK, st, dev);

        /*call with stage = CPSS_DXCH_POLICER_STAGE_INGRESS_1_E;*/
        stage = CPSS_DXCH_POLICER_STAGE_INGRESS_1_E;

        st = cpssDxChPolicerCountingModeGet(dev, stage, &mode);
        UTF_VERIFY_EQUAL1_PARAM_MAC(GT_OK, st, dev);

        /*call with stage = CPSS_DXCH_POLICER_STAGE_EGRESS_E;*/
        stage = CPSS_DXCH_POLICER_STAGE_EGRESS_E;

        st = cpssDxChPolicerCountingModeGet(dev, stage, &mode);
        UTF_VERIFY_EQUAL1_PARAM_MAC(GT_OK, st, dev);

        /*
            1.2. Call api with wrong stage [wrong enum values].
            Expected: GT_BAD_PARAM.
        */
        UTF_ENUMS_CHECK_MAC(cpssDxChPolicerCountingModeGet
                            (dev, stage, &mode),
                            stage);

        /*
            1.3. Call api with wrong modePtr [NULL].
            Expected: GT_BAD_PTR.
        */
        st = cpssDxChPolicerCountingModeGet(dev, stage, NULL);
        UTF_VERIFY_EQUAL1_PARAM_MAC(GT_BAD_PTR, st, dev);
    }

    /* restore correct values */
    stage = CPSS_DXCH_POLICER_STAGE_INGRESS_0_E;

    /* 2. For not-active devices and devices from non-applicable family */
    /* check that function returns GT_BAD_PARAM.                        */

    /* prepare device iterator */
    PRV_TGF_NOT_APPLIC_DEV_RESET_MAC(&dev, UTF_CH1_E | UTF_CH1_DIAMOND_E | UTF_CH2_E | UTF_CH3_E);

    /* go over all non active devices */
    while(GT_OK == prvUtfNextDeviceGet(&dev, GT_FALSE))
    {
        st = cpssDxChPolicerCountingModeGet(dev, stage, &mode);
        UTF_VERIFY_EQUAL1_PARAM_MAC(GT_NOT_APPLICABLE_DEVICE, st, dev);
    }

    /* 3. Call function with out of bound value for device id.*/
    dev = PRV_CPSS_MAX_PP_DEVICES_CNS;

    st = cpssDxChPolicerCountingModeGet(dev, stage, &mode);
    UTF_VERIFY_EQUAL1_PARAM_MAC(GT_BAD_PARAM, st, dev);
}

/*----------------------------------------------------------------------------*/
/*
GT_STATUS cpssDxChPolicerCountingModeSet
(
    IN  GT_U8                               devNum,
    IN  CPSS_DXCH_POLICER_STAGE_TYPE_ENT    stage,
    IN  CPSS_DXCH_POLICER_COUNTING_MODE_ENT mode
)
*/
UTF_TEST_CASE_MAC(cpssDxChPolicerCountingModeSet)
{
/*
    ITERATE_DEVICES(DxChXcat and above)
    1.1. Call function with stage [CPSS_DXCH_POLICER_STAGE_INGRESS_0_E
                                   CPSS_DXCH_POLICER_STAGE_INGRESS_1_E /
                                   CPSS_DXCH_POLICER_STAGE_EGRESS_E],
                 and mode [CPSS_DXCH_POLICER_COUNTING_DISABLE_E,
                           CPSS_DXCH_POLICER_COUNTING_BILLING_IPFIX_E,
                           CPSS_DXCH_POLICER_COUNTING_POLICY_E,
                           CPSS_DXCH_POLICER_COUNTING_VLAN_E].
    Expected: GT_OK.
    1.2. Call cpssDxChPolicerCountingModeGet with the same stage.
    Expected: GT_OK.
    1.3. Call api with wrong stage [wrong enum values].
    Expected: GT_BAD_PARAM.
    1.4. Call api with wrong mode [wrong enum values].
    Expected: GT_BAD_PARAM.
*/
    GT_STATUS st = GT_OK;

    GT_U8                               dev;
    CPSS_DXCH_POLICER_STAGE_TYPE_ENT    stage = CPSS_DXCH_POLICER_STAGE_INGRESS_0_E;
    CPSS_DXCH_POLICER_COUNTING_MODE_ENT mode = CPSS_DXCH_POLICER_COUNTING_DISABLE_E;
    CPSS_DXCH_POLICER_COUNTING_MODE_ENT modeGet;
    GT_STATUS expectedStatus;

    /* prepare device iterator */
    PRV_TGF_NOT_APPLIC_DEV_RESET_MAC(&dev, UTF_CH1_E | UTF_CH1_DIAMOND_E | UTF_CH2_E | UTF_CH3_E);

    /* 1. Go over all active devices. */
    while (GT_OK == prvUtfNextDeviceGet(&dev, GT_TRUE))
    {
        /*
            1.1. Call function with stage [CPSS_DXCH_POLICER_STAGE_INGRESS_0_E
                                           CPSS_DXCH_POLICER_STAGE_INGRESS_1_E /
                                           CPSS_DXCH_POLICER_STAGE_EGRESS_E],
                         and mode [CPSS_DXCH_POLICER_COUNTING_DISABLE_E,
                                   CPSS_DXCH_POLICER_COUNTING_BILLING_IPFIX_E,
                                   CPSS_DXCH_POLICER_COUNTING_POLICY_E,
                                   CPSS_DXCH_POLICER_COUNTING_VLAN_E].
            Expected: GT_OK.
        */
        stage = CPSS_DXCH_POLICER_STAGE_INGRESS_0_E;
        mode = CPSS_DXCH_POLICER_COUNTING_DISABLE_E;

        st = cpssDxChPolicerCountingModeSet(dev, stage, mode);
        UTF_VERIFY_EQUAL1_PARAM_MAC(GT_OK, st, dev);

        /*
            1.2. Call cpssDxChPolicerCountingModeGet with the same stage.
            Expected: GT_OK.
        */
        st = cpssDxChPolicerCountingModeGet(dev, stage, &modeGet);
        UTF_VERIFY_EQUAL1_STRING_MAC(GT_OK, st,
                   "cpssDxChPolicerCountingModeGet: %d", dev);

        /* verifying values */
        UTF_VERIFY_EQUAL1_STRING_MAC(mode, modeGet,
                   "got another mode then was set: %d", dev);

        /*
            1.1. Call function with stage [CPSS_DXCH_POLICER_STAGE_INGRESS_0_E
                                           CPSS_DXCH_POLICER_STAGE_INGRESS_1_E /
                                           CPSS_DXCH_POLICER_STAGE_EGRESS_E],
                         and mode [CPSS_DXCH_POLICER_COUNTING_DISABLE_E,
                                   CPSS_DXCH_POLICER_COUNTING_BILLING_IPFIX_E,
                                   CPSS_DXCH_POLICER_COUNTING_POLICY_E,
                                   CPSS_DXCH_POLICER_COUNTING_VLAN_E].
            Expected: GT_OK.
        */
        stage = CPSS_DXCH_POLICER_STAGE_INGRESS_1_E;
        mode = CPSS_DXCH_POLICER_COUNTING_BILLING_IPFIX_E;

        st = cpssDxChPolicerCountingModeSet(dev, stage, mode);
        UTF_VERIFY_EQUAL1_PARAM_MAC(GT_OK, st, dev);

        /*
            1.2. Call cpssDxChPolicerCountingModeGet with the same stage.
            Expected: GT_OK.
        */
        st = cpssDxChPolicerCountingModeGet(dev, stage, &modeGet);
        UTF_VERIFY_EQUAL1_STRING_MAC(GT_OK, st,
                   "cpssDxChPolicerCountingModeGet: %d", dev);

        /* verifying values */
        UTF_VERIFY_EQUAL1_STRING_MAC(mode, modeGet,
                   "got another mode then was set: %d", dev);

        /*
            1.1. Call function with stage [CPSS_DXCH_POLICER_STAGE_INGRESS_0_E
                                           CPSS_DXCH_POLICER_STAGE_INGRESS_1_E /
                                           CPSS_DXCH_POLICER_STAGE_EGRESS_E],
                         and mode [CPSS_DXCH_POLICER_COUNTING_DISABLE_E,
                                   CPSS_DXCH_POLICER_COUNTING_BILLING_IPFIX_E,
                                   CPSS_DXCH_POLICER_COUNTING_POLICY_E,
                                   CPSS_DXCH_POLICER_COUNTING_VLAN_E].
            Expected: GT_OK.
        */
        stage = CPSS_DXCH_POLICER_STAGE_EGRESS_E;
        mode = CPSS_DXCH_POLICER_COUNTING_POLICY_E;

        st = cpssDxChPolicerCountingModeSet(dev, stage, mode);
        UTF_VERIFY_EQUAL1_PARAM_MAC(GT_OK, st, dev);

        /*
            1.2. Call cpssDxChPolicerCountingModeGet with the same stage.
            Expected: GT_OK.
        */
        st = cpssDxChPolicerCountingModeGet(dev, stage, &modeGet);
        UTF_VERIFY_EQUAL1_STRING_MAC(GT_OK, st,
                   "cpssDxChPolicerCountingModeGet: %d", dev);

        /* verifying values */
        UTF_VERIFY_EQUAL1_STRING_MAC(mode, modeGet,
                   "got another mode then was set: %d", dev);

        /*
            1.1. Call function with stage [CPSS_DXCH_POLICER_STAGE_INGRESS_0_E
                                           CPSS_DXCH_POLICER_STAGE_INGRESS_1_E /
                                           CPSS_DXCH_POLICER_STAGE_EGRESS_E],
                         and mode [CPSS_DXCH_POLICER_COUNTING_DISABLE_E,
                                   CPSS_DXCH_POLICER_COUNTING_BILLING_IPFIX_E,
                                   CPSS_DXCH_POLICER_COUNTING_POLICY_E,
                                   CPSS_DXCH_POLICER_COUNTING_VLAN_E].
            Expected: GT_OK.
        */
        if (PRV_CPSS_PP_MAC(dev)->devFamily == CPSS_PP_FAMILY_DXCH_XCAT2_E)
        {
            /* not supported in xCat2 */
            expectedStatus = GT_NOT_APPLICABLE_DEVICE;
        }
        else
        {
            expectedStatus = GT_OK;
        }

        stage = CPSS_DXCH_POLICER_STAGE_INGRESS_0_E;
        mode = CPSS_DXCH_POLICER_COUNTING_VLAN_E;

        st = cpssDxChPolicerCountingModeSet(dev, stage, mode);
        UTF_VERIFY_EQUAL1_PARAM_MAC(expectedStatus, st, dev);

        /*
            1.2. Call cpssDxChPolicerCountingModeGet with the same stage.
            Expected: GT_OK.
        */
        st = cpssDxChPolicerCountingModeGet(dev, stage, &modeGet);
        UTF_VERIFY_EQUAL1_STRING_MAC(GT_OK, st,
                   "cpssDxChPolicerCountingModeGet: %d", dev);

        if (expectedStatus == GT_OK) 
        {
            /* verifying values */
            UTF_VERIFY_EQUAL1_STRING_MAC(mode, modeGet,
                       "got another mode then was set: %d", dev);
        }

        /*
            1.3. Call api with wrong stage [wrong enum values].
            Expected: GT_BAD_PARAM.
        */
        UTF_ENUMS_CHECK_MAC(cpssDxChPolicerCountingModeSet
                            (dev, stage, mode),
                            stage);

        /*
            1.4. Call api with wrong mode [wrong enum values].
            Expected: GT_BAD_PARAM.
        */
        UTF_ENUMS_CHECK_MAC(cpssDxChPolicerCountingModeSet
                            (dev, stage, mode),
                            mode);
    }

    /* restore correct values */
    stage = CPSS_DXCH_POLICER_STAGE_INGRESS_0_E;
    mode = CPSS_DXCH_POLICER_COUNTING_DISABLE_E;

    /* 2. For not-active devices and devices from non-applicable family */
    /* check that function returns GT_BAD_PARAM.                        */

    /* prepare device iterator */
    PRV_TGF_NOT_APPLIC_DEV_RESET_MAC(&dev, UTF_CH1_E | UTF_CH1_DIAMOND_E | UTF_CH2_E | UTF_CH3_E);

    /* go over all non active devices */
    while(GT_OK == prvUtfNextDeviceGet(&dev, GT_FALSE))
    {
        st = cpssDxChPolicerCountingModeSet(dev, stage, mode);
        UTF_VERIFY_EQUAL1_PARAM_MAC(GT_NOT_APPLICABLE_DEVICE, st, dev);
    }

    /* 3. Call function with out of bound value for device id.*/
    dev = PRV_CPSS_MAX_PP_DEVICES_CNS;

    st = cpssDxChPolicerCountingModeSet(dev, stage, mode);
    UTF_VERIFY_EQUAL1_PARAM_MAC(GT_BAD_PARAM, st, dev);
}

/*----------------------------------------------------------------------------*/
/*
GT_STATUS cpssDxChPolicerCountingTriggerByPortEnableGet
(
    IN  GT_U8                             devNum,
    IN  CPSS_DXCH_POLICER_STAGE_TYPE_ENT  stage,
    OUT GT_BOOL                           *enablePtr
)
*/
UTF_TEST_CASE_MAC(cpssDxChPolicerCountingTriggerByPortEnableGet)
{
/*
    ITERATE_DEVICES(Lion and above)
    1.1. Call function with stage [CPSS_DXCH_POLICER_STAGE_INGRESS_0_E
                                   CPSS_DXCH_POLICER_STAGE_INGRESS_1_E /
                                   CPSS_DXCH_POLICER_STAGE_EGRESS_E].
    Expected: GT_OK.
    1.2. Call api with wrong stage [wrong enum values].
    Expected: GT_BAD_PARAM.
    1.3. Call api with wrong enablePtr [NULL].
    Expected: GT_BAD_PTR.
*/
    GT_STATUS st = GT_OK;

    GT_U8                             dev;
    CPSS_DXCH_POLICER_STAGE_TYPE_ENT  stage = CPSS_DXCH_POLICER_STAGE_INGRESS_0_E;
    GT_BOOL                           enable;

    /* prepare device iterator */
    PRV_TGF_NOT_APPLIC_DEV_RESET_MAC(&dev, UTF_CH1_E | UTF_CH1_DIAMOND_E | UTF_CH2_E | UTF_CH3_E | UTF_XCAT_E);

    /* 1. Go over all active devices. */
    while (GT_OK == prvUtfNextDeviceGet(&dev, GT_TRUE))
    {
        /*
            1.1. Call function with stage [CPSS_DXCH_POLICER_STAGE_INGRESS_0_E
                                           CPSS_DXCH_POLICER_STAGE_INGRESS_1_E /
                                           CPSS_DXCH_POLICER_STAGE_EGRESS_E].
            Expected: GT_OK.
        */
        /*call with stage = CPSS_DXCH_POLICER_STAGE_INGRESS_0_E;*/
        stage = CPSS_DXCH_POLICER_STAGE_INGRESS_0_E;

        st = cpssDxChPolicerCountingTriggerByPortEnableGet(dev, stage, &enable);
        UTF_VERIFY_EQUAL1_PARAM_MAC(GT_OK, st, dev);

        /*call with stage = CPSS_DXCH_POLICER_STAGE_INGRESS_1_E;*/
        stage = CPSS_DXCH_POLICER_STAGE_INGRESS_1_E;

        st = cpssDxChPolicerCountingTriggerByPortEnableGet(dev, stage, &enable);
        UTF_VERIFY_EQUAL1_PARAM_MAC(GT_OK, st, dev);

        /*call with stage = CPSS_DXCH_POLICER_STAGE_EGRESS_E;*/
        stage = CPSS_DXCH_POLICER_STAGE_EGRESS_E;

        st = cpssDxChPolicerCountingTriggerByPortEnableGet(dev, stage, &enable);
        UTF_VERIFY_EQUAL1_PARAM_MAC(GT_OK, st, dev);

        /*
            1.2. Call api with wrong stage [wrong enum values].
            Expected: GT_BAD_PARAM.
        */
        UTF_ENUMS_CHECK_MAC(cpssDxChPolicerCountingTriggerByPortEnableGet
                            (dev, stage, &enable),
                            stage);

        /*
            1.3. Call api with wrong enablePtr [NULL].
            Expected: GT_BAD_PTR.
        */
        st = cpssDxChPolicerCountingTriggerByPortEnableGet(dev, stage, NULL);
        UTF_VERIFY_EQUAL1_PARAM_MAC(GT_BAD_PTR, st, dev);
    }

    /* restore correct values */
    stage = CPSS_DXCH_POLICER_STAGE_INGRESS_0_E;

    /* 2. For not-active devices and devices from non-applicable family */
    /* check that function returns GT_BAD_PARAM.                        */

    /* prepare device iterator */
    PRV_TGF_NOT_APPLIC_DEV_RESET_MAC(&dev, UTF_CH1_E | UTF_CH1_DIAMOND_E | UTF_CH2_E | UTF_CH3_E | UTF_XCAT_E);

    /* go over all non active devices */
    while(GT_OK == prvUtfNextDeviceGet(&dev, GT_FALSE))
    {
        st = cpssDxChPolicerCountingTriggerByPortEnableGet(dev, stage, &enable);
        UTF_VERIFY_EQUAL1_PARAM_MAC(GT_NOT_APPLICABLE_DEVICE, st, dev);
    }

    /* 3. Call function with out of bound value for device id.*/
    dev = PRV_CPSS_MAX_PP_DEVICES_CNS;

    st = cpssDxChPolicerCountingTriggerByPortEnableGet(dev, stage, &enable);
    UTF_VERIFY_EQUAL1_PARAM_MAC(GT_BAD_PARAM, st, dev);
}

/*----------------------------------------------------------------------------*/
/*
GT_STATUS cpssDxChPolicerCountingTriggerByPortEnableSet
(
    IN  GT_U8                            devNum,
    IN  CPSS_DXCH_POLICER_STAGE_TYPE_ENT stage,
    IN  GT_BOOL                          enable
)
*/
UTF_TEST_CASE_MAC(cpssDxChPolicerCountingTriggerByPortEnableSet)
{
/*
    ITERATE_DEVICES(Lion and above)
    1.1. Call function with stage [CPSS_DXCH_POLICER_STAGE_INGRESS_0_E
                                   CPSS_DXCH_POLICER_STAGE_INGRESS_1_E /
                                   CPSS_DXCH_POLICER_STAGE_EGRESS_E],
                       and enable [GT_TRUE / GT_FALSE].
    Expected: GT_OK.
    1.2. Call cpssDxChPolicerCountingTriggerByPortEnableGet
         with the same stage.
    Expected: GT_OK.
    1.3. Call api with wrong stage [wrong enum values].
    Expected: GT_BAD_PARAM.
*/
    GT_STATUS st = GT_OK;

    GT_U8                            dev;
    CPSS_DXCH_POLICER_STAGE_TYPE_ENT stage = CPSS_DXCH_POLICER_STAGE_INGRESS_0_E;
    GT_BOOL                          enable = GT_FALSE;
    GT_BOOL                          enableGet;

    /* prepare device iterator */
    PRV_TGF_NOT_APPLIC_DEV_RESET_MAC(&dev, UTF_CH1_E | UTF_CH1_DIAMOND_E | UTF_CH2_E | UTF_CH3_E | UTF_XCAT_E);

    /* 1. Go over all active devices. */
    while (GT_OK == prvUtfNextDeviceGet(&dev, GT_TRUE))
    {
        /*
            1.1. Call function with stage [CPSS_DXCH_POLICER_STAGE_INGRESS_0_E
                                           CPSS_DXCH_POLICER_STAGE_INGRESS_1_E /
                                           CPSS_DXCH_POLICER_STAGE_EGRESS_E],
                               and enable [GT_TRUE / GT_FALSE].
            Expected: GT_OK.
        */
        stage = CPSS_DXCH_POLICER_STAGE_INGRESS_0_E;
        enable = GT_TRUE;

        st = cpssDxChPolicerCountingTriggerByPortEnableSet(dev, stage, enable);
        UTF_VERIFY_EQUAL1_PARAM_MAC(GT_OK, st, dev);

        /*
            1.2. Call cpssDxChPolicerCountingTriggerByPortEnableGet
                 with the same stage.
            Expected: GT_OK.
        */
        st = cpssDxChPolicerCountingTriggerByPortEnableGet(dev, stage, &enableGet);
        UTF_VERIFY_EQUAL1_STRING_MAC(GT_OK, st,
               "cpssDxChPolicerCountingTriggerByPortEnableGet: %d", dev);

        /* verifying values */
        UTF_VERIFY_EQUAL1_STRING_MAC(enable, enableGet,
                   "got another enable then was set: %d", dev);

        /*
            1.1. Call function with stage [CPSS_DXCH_POLICER_STAGE_INGRESS_0_E
                                           CPSS_DXCH_POLICER_STAGE_INGRESS_1_E /
                                           CPSS_DXCH_POLICER_STAGE_EGRESS_E],
                               and enable [GT_TRUE / GT_FALSE].
            Expected: GT_OK.
        */
        stage = CPSS_DXCH_POLICER_STAGE_INGRESS_1_E;
        enable = GT_TRUE;

        st = cpssDxChPolicerCountingTriggerByPortEnableSet(dev, stage, enable);
        UTF_VERIFY_EQUAL1_PARAM_MAC(GT_OK, st, dev);

        /*
            1.2. Call cpssDxChPolicerCountingTriggerByPortEnableGet
                 with the same stage.
            Expected: GT_OK.
        */
        st = cpssDxChPolicerCountingTriggerByPortEnableGet(dev, stage, &enableGet);
        UTF_VERIFY_EQUAL1_STRING_MAC(GT_OK, st,
               "cpssDxChPolicerCountingTriggerByPortEnableGet: %d", dev);

        /* verifying values */
        UTF_VERIFY_EQUAL1_STRING_MAC(enable, enableGet,
                   "got another enable then was set: %d", dev);

        /*
            1.1. Call function with stage [CPSS_DXCH_POLICER_STAGE_INGRESS_0_E
                                           CPSS_DXCH_POLICER_STAGE_INGRESS_1_E /
                                           CPSS_DXCH_POLICER_STAGE_EGRESS_E],
                               and enable [GT_TRUE / GT_FALSE].
            Expected: GT_OK.
        */
        stage = CPSS_DXCH_POLICER_STAGE_EGRESS_E;
        enable = GT_FALSE;

        st = cpssDxChPolicerCountingTriggerByPortEnableSet(dev, stage, enable);
        UTF_VERIFY_EQUAL1_PARAM_MAC(GT_OK, st, dev);

        /*
            1.2. Call cpssDxChPolicerCountingTriggerByPortEnableGet
                 with the same stage.
            Expected: GT_OK.
        */
        st = cpssDxChPolicerCountingTriggerByPortEnableGet(dev, stage, &enableGet);
        UTF_VERIFY_EQUAL1_STRING_MAC(GT_OK, st,
               "cpssDxChPolicerCountingTriggerByPortEnableGet: %d", dev);

        /* verifying values */
        UTF_VERIFY_EQUAL1_STRING_MAC(enable, enableGet,
                   "got another enable then was set: %d", dev);

        /*
            1.3. Call api with wrong stage [wrong enum values].
            Expected: GT_BAD_PARAM.
        */
        UTF_ENUMS_CHECK_MAC(cpssDxChPolicerCountingTriggerByPortEnableSet
                            (dev, stage, enable),
                            stage);
    }

    /* restore correct values */
    stage = CPSS_DXCH_POLICER_STAGE_INGRESS_0_E;
    enable = GT_TRUE;

    /* 2. For not-active devices and devices from non-applicable family */
    /* check that function returns GT_BAD_PARAM.                        */

    /* prepare device iterator */
    PRV_TGF_NOT_APPLIC_DEV_RESET_MAC(&dev, UTF_CH1_E | UTF_CH1_DIAMOND_E | UTF_CH2_E | UTF_CH3_E | UTF_XCAT_E);

    /* go over all non active devices */
    while(GT_OK == prvUtfNextDeviceGet(&dev, GT_FALSE))
    {
        st = cpssDxChPolicerCountingTriggerByPortEnableSet(dev, stage, enable);
        UTF_VERIFY_EQUAL1_PARAM_MAC(GT_NOT_APPLICABLE_DEVICE, st, dev);
    }

    /* 3. Call function with out of bound value for device id.*/
    dev = PRV_CPSS_MAX_PP_DEVICES_CNS;

    st = cpssDxChPolicerCountingTriggerByPortEnableSet(dev, stage, enable);
    UTF_VERIFY_EQUAL1_PARAM_MAC(GT_BAD_PARAM, st, dev);
}

/*----------------------------------------------------------------------------*/
/*
GT_STATUS cpssDxChPolicerCountingWriteBackCacheFlush
(
    IN  GT_U8                            devNum,
    IN  CPSS_DXCH_POLICER_STAGE_TYPE_ENT stage
)
*/
UTF_TEST_CASE_MAC(cpssDxChPolicerCountingWriteBackCacheFlush)
{
/*
    ITERATE_DEVICES(DxChXcat and above)
    1.1. Call function with stage [CPSS_DXCH_POLICER_STAGE_INGRESS_0_E
                                   CPSS_DXCH_POLICER_STAGE_INGRESS_1_E /
                                   CPSS_DXCH_POLICER_STAGE_EGRESS_E],
    Expected: GT_OK.
    1.2. Call api with wrong stage [wrong enum values].
    Expected: GT_BAD_PARAM.
*/
    GT_STATUS st = GT_OK;

    GT_U8                            dev;
    CPSS_DXCH_POLICER_STAGE_TYPE_ENT stage = CPSS_DXCH_POLICER_STAGE_INGRESS_0_E;

    /* prepare device iterator */
    PRV_TGF_NOT_APPLIC_DEV_RESET_MAC(&dev, UTF_CH1_E | UTF_CH1_DIAMOND_E | UTF_CH2_E | UTF_CH3_E);

    /* 1. Go over all active devices. */
    while (GT_OK == prvUtfNextDeviceGet(&dev, GT_TRUE))
    {
        /*
            1.1. Call function with stage [CPSS_DXCH_POLICER_STAGE_INGRESS_0_E
                                           CPSS_DXCH_POLICER_STAGE_INGRESS_1_E /
                                           CPSS_DXCH_POLICER_STAGE_EGRESS_E],
            Expected: GT_OK.
        */
        /*call with stage = CPSS_DXCH_POLICER_STAGE_INGRESS_0_E;*/
        stage = CPSS_DXCH_POLICER_STAGE_INGRESS_0_E;

        st = cpssDxChPolicerCountingWriteBackCacheFlush(dev, stage);
        UTF_VERIFY_EQUAL1_PARAM_MAC(GT_OK, st, dev);

        /*call with stage = CPSS_DXCH_POLICER_STAGE_INGRESS_1_E;*/
        stage = CPSS_DXCH_POLICER_STAGE_INGRESS_1_E;

        st = cpssDxChPolicerCountingWriteBackCacheFlush(dev, stage);
        UTF_VERIFY_EQUAL1_PARAM_MAC(GT_OK, st, dev);

        /*call with stage = CPSS_DXCH_POLICER_STAGE_EGRESS_E;*/
        stage = CPSS_DXCH_POLICER_STAGE_EGRESS_E;

        st = cpssDxChPolicerCountingWriteBackCacheFlush(dev, stage);
        UTF_VERIFY_EQUAL1_PARAM_MAC(GT_OK, st, dev);

        /*
            1.2. Call api with wrong stage [wrong enum values].
            Expected: GT_BAD_PARAM.
        */
        UTF_ENUMS_CHECK_MAC(cpssDxChPolicerCountingWriteBackCacheFlush
                            (dev, stage),
                            stage);
    }

    /* restore correct values */
    stage = CPSS_DXCH_POLICER_STAGE_INGRESS_0_E;

    /* 2. For not-active devices and devices from non-applicable family */
    /* check that function returns GT_BAD_PARAM.                        */

    /* prepare device iterator */
    PRV_TGF_NOT_APPLIC_DEV_RESET_MAC(&dev, UTF_CH1_E | UTF_CH1_DIAMOND_E | UTF_CH2_E | UTF_CH3_E);

    /* go over all non active devices */
    while(GT_OK == prvUtfNextDeviceGet(&dev, GT_FALSE))
    {
        st = cpssDxChPolicerCountingWriteBackCacheFlush(dev, stage);
        UTF_VERIFY_EQUAL1_PARAM_MAC(GT_NOT_APPLICABLE_DEVICE, st, dev);
    }

    /* 3. Call function with out of bound value for device id.*/
    dev = PRV_CPSS_MAX_PP_DEVICES_CNS;

    st = cpssDxChPolicerCountingWriteBackCacheFlush(dev, stage);
    UTF_VERIFY_EQUAL1_PARAM_MAC(GT_BAD_PARAM, st, dev);
}

/*----------------------------------------------------------------------------*/
/*
GT_STATUS cpssDxChPolicerEgressL2RemarkModelGet
(
    IN  GT_U8                                  devNum,
    OUT CPSS_DXCH_POLICER_L2_REMARK_MODEL_ENT  *modelPtr
)
*/
UTF_TEST_CASE_MAC(cpssDxChPolicerEgressL2RemarkModelGet)
{
/*
    ITERATE_DEVICES(DxChXcat and above)
    1.1. Call function with not null pointer.
    Expected: GT_OK.
    1.2. Call api with wrong modelPtr [NULL].
    Expected: GT_BAD_PTR.
*/
    GT_STATUS st = GT_OK;

    GT_U8                                  dev;
    CPSS_DXCH_POLICER_L2_REMARK_MODEL_ENT  model;

    /* prepare device iterator */
    PRV_TGF_NOT_APPLIC_DEV_RESET_MAC(&dev, UTF_CH1_E | UTF_CH1_DIAMOND_E | UTF_CH2_E | UTF_CH3_E);


    /* 1. Go over all active devices. */
    while (GT_OK == prvUtfNextDeviceGet(&dev, GT_TRUE))
    {
        /*
            1.1. Call function with not null pointer.
            Expected: GT_OK.
        */

        /* check that the device is not Lion A0 */
        if ((PRV_CPSS_PP_MAC(dev)->devFamily == CPSS_PP_FAMILY_DXCH_LION_E) &&
            (PRV_CPSS_PP_MAC(dev)->revision < 2))
            return;

        st = cpssDxChPolicerEgressL2RemarkModelGet(dev, &model);
        UTF_VERIFY_EQUAL1_PARAM_MAC(GT_OK, st, dev);

        /*
            1.2. Call api with wrong modelPtr [NULL].
            Expected: GT_BAD_PTR.
        */
        st = cpssDxChPolicerEgressL2RemarkModelGet(dev, NULL);
        UTF_VERIFY_EQUAL1_PARAM_MAC(GT_BAD_PTR, st, dev);
    }

    /* 2. For not-active devices and devices from non-applicable family */
    /* check that function returns GT_BAD_PARAM.                        */

    /* prepare device iterator */
    PRV_TGF_NOT_APPLIC_DEV_RESET_MAC(&dev, UTF_CH1_E | UTF_CH1_DIAMOND_E | UTF_CH2_E | UTF_CH3_E);

    /* go over all non active devices */
    while(GT_OK == prvUtfNextDeviceGet(&dev, GT_FALSE))
    {
        /* check that the device is not Lion A0 */
        if ((PRV_CPSS_PP_MAC(dev)->devFamily == CPSS_PP_FAMILY_DXCH_LION_E) &&
            (PRV_CPSS_PP_MAC(dev)->revision < 2))
            return;

        st = cpssDxChPolicerEgressL2RemarkModelGet(dev, &model);
        UTF_VERIFY_EQUAL1_PARAM_MAC(GT_NOT_APPLICABLE_DEVICE, st, dev);
    }

    /* 3. Call function with out of bound value for device id.*/
    dev = PRV_CPSS_MAX_PP_DEVICES_CNS;

    st = cpssDxChPolicerEgressL2RemarkModelGet(dev, &model);
    UTF_VERIFY_EQUAL1_PARAM_MAC(GT_BAD_PARAM, st, dev);
}

/*----------------------------------------------------------------------------*/
/*
GT_STATUS cpssDxChPolicerEgressL2RemarkModelSet
(
    IN  GT_U8                                 devNum,
    IN  CPSS_DXCH_POLICER_L2_REMARK_MODEL_ENT model
)
*/
UTF_TEST_CASE_MAC(cpssDxChPolicerEgressL2RemarkModelSet)
{
/*
    ITERATE_DEVICES(DxChXcat and above)
    1.1. Call function with model [CPSS_DXCH_POLICER_L2_REMARK_MODEL_TC_E /
                                   CPSS_DXCH_POLICER_L2_REMARK_MODEL_UP_E].
    Expected: GT_OK.
    1.2. Call cpssDxChPolicerEgressL2RemarkModelGet.
    Expected: GT_OK and the same value.
    1.3. Call api with wrong model [wrong enum values].
    Expected: GT_BAD_PARAM.
*/
    GT_STATUS st = GT_OK;

    GT_U8                                 dev;
    CPSS_DXCH_POLICER_L2_REMARK_MODEL_ENT model = CPSS_DXCH_POLICER_L2_REMARK_MODEL_TC_E;
    CPSS_DXCH_POLICER_L2_REMARK_MODEL_ENT modelGet;

    /* prepare device iterator */
    PRV_TGF_NOT_APPLIC_DEV_RESET_MAC(&dev, UTF_CH1_E | UTF_CH1_DIAMOND_E | UTF_CH2_E | UTF_CH3_E);

    /* 1. Go over all active devices. */
    while (GT_OK == prvUtfNextDeviceGet(&dev, GT_TRUE))
    {
        /* check that the device is not Lion A0 */
        if ((PRV_CPSS_PP_MAC(dev)->devFamily == CPSS_PP_FAMILY_DXCH_LION_E) &&
            (PRV_CPSS_PP_MAC(dev)->revision < 2))
            return;

        /*
            1.1. Call function with model [CPSS_DXCH_POLICER_L2_REMARK_MODEL_TC_E /
                                           CPSS_DXCH_POLICER_L2_REMARK_MODEL_UP_E].
            Expected: GT_OK.
        */
        model = CPSS_DXCH_POLICER_L2_REMARK_MODEL_TC_E;

        st = cpssDxChPolicerEgressL2RemarkModelSet(dev, model);
        UTF_VERIFY_EQUAL1_PARAM_MAC(GT_OK, st, dev);

        /*
            1.2. Call cpssDxChPolicerEgressL2RemarkModelGet.
            Expected: GT_OK and the same value.
        */
        st = cpssDxChPolicerEgressL2RemarkModelGet(dev, &modelGet);
        UTF_VERIFY_EQUAL1_STRING_MAC(GT_OK, st,
               "cpssDxChPolicerEgressL2RemarkModelGet: %d", dev);

        /* verifying values */
        UTF_VERIFY_EQUAL1_STRING_MAC(model, modelGet,
                   "got another model then was set: %d", dev);

        /*
            1.1. Call function with model [CPSS_DXCH_POLICER_L2_REMARK_MODEL_TC_E /
                                           CPSS_DXCH_POLICER_L2_REMARK_MODEL_UP_E].
            Expected: GT_OK.
        */
        model = CPSS_DXCH_POLICER_L2_REMARK_MODEL_UP_E;

        st = cpssDxChPolicerEgressL2RemarkModelSet(dev, model);
        UTF_VERIFY_EQUAL1_PARAM_MAC(GT_OK, st, dev);

        /*
            1.2. Call cpssDxChPolicerEgressL2RemarkModelGet.
            Expected: GT_OK and the same value.
        */
        st = cpssDxChPolicerEgressL2RemarkModelGet(dev, &modelGet);
        UTF_VERIFY_EQUAL1_STRING_MAC(GT_OK, st,
               "cpssDxChPolicerEgressL2RemarkModelGet: %d", dev);

        /* verifying values */
        UTF_VERIFY_EQUAL1_STRING_MAC(model, modelGet,
                   "got another model then was set: %d", dev);

        /*
            1.3. Call api with wrong model [wrong enum values].
            Expected: GT_BAD_PARAM.
        */
        UTF_ENUMS_CHECK_MAC(cpssDxChPolicerEgressL2RemarkModelSet
                            (dev, model),
                            model);
    }

    /* restore correct values */
    model = CPSS_DXCH_POLICER_L2_REMARK_MODEL_TC_E;

    /* 2. For not-active devices and devices from non-applicable family */
    /* check that function returns GT_BAD_PARAM.                        */

    /* prepare device iterator */
    PRV_TGF_NOT_APPLIC_DEV_RESET_MAC(&dev, UTF_CH1_E | UTF_CH1_DIAMOND_E | UTF_CH2_E | UTF_CH3_E);

    /* go over all non active devices */
    while(GT_OK == prvUtfNextDeviceGet(&dev, GT_FALSE))
    {
        /* check that the device is not Lion A0 */
        if ((PRV_CPSS_PP_MAC(dev)->devFamily == CPSS_PP_FAMILY_DXCH_LION_E) &&
            (PRV_CPSS_PP_MAC(dev)->revision < 2))
            return;

        st = cpssDxChPolicerEgressL2RemarkModelSet(dev, model);
        UTF_VERIFY_EQUAL1_PARAM_MAC(GT_NOT_APPLICABLE_DEVICE, st, dev);
    }

    /* 3. Call function with out of bound value for device id.*/
    dev = PRV_CPSS_MAX_PP_DEVICES_CNS;

    st = cpssDxChPolicerEgressL2RemarkModelSet(dev, model);
    UTF_VERIFY_EQUAL1_PARAM_MAC(GT_BAD_PARAM, st, dev);
}

/*----------------------------------------------------------------------------*/
/*
GT_STATUS cpssDxChPolicerEgressQosRemarkingEntryGet
(
    IN  GT_U8                                    devNum,
    IN  CPSS_DXCH_POLICER_REMARK_TABLE_TYPE_ENT  remarkTableType,
    IN  GT_U32                                   remarkParamValue,
    IN  CPSS_DP_LEVEL_ENT                        confLevel,
    OUT CPSS_DXCH_POLICER_QOS_PARAM_STC          *qosParamPtr
)
*/
UTF_TEST_CASE_MAC(cpssDxChPolicerEgressQosRemarkingEntryGet)
{
/*
ITERATE_DEVICES(DxChXcat and above)
    1.1. Call function with
            remarkTableType[CPSS_DXCH_POLICER_REMARK_TABLE_TYPE_TC_UP_E /
                            CPSS_DXCH_POLICER_REMARK_TABLE_TYPE_DSCP_E  /
                            CPSS_DXCH_POLICER_REMARK_TABLE_TYPE_XP_E],
            remarkParamValue [0 / 63 / 7],
            and confLevel[CPSS_DP_GREEN_E / CPSS_DP_YELLOW_E  / CPSS_DP_RED_E].
    Expected: GT_OK.
    1.2. Call api with wrong remarkTableType [wrong enum values].
    Expected: GT_BAD_PARAM.
    1.3. Call api with wrong remarkParamValue (out of range).
            remarkTableType[CPSS_DXCH_POLICER_REMARK_TABLE_TYPE_TC_UP_E /
                            CPSS_DXCH_POLICER_REMARK_TABLE_TYPE_DSCP_E  /
                            CPSS_DXCH_POLICER_REMARK_TABLE_TYPE_EXP_E],
            remarkParamValue [8 / 64 / 8], and confLevel[CPSS_DP_GREEN_E].
    Expected: NOT GT_OK.
    1.4. Call api with wrong confLevel [wrong enum values].
    Expected: GT_BAD_PARAM.
    1.5. Call api with wrong qosParamPtr [NULL].
    Expected: GT_BAD_PTR.
*/
    GT_STATUS st = GT_OK;

    GT_U8                                    dev;
    CPSS_DXCH_POLICER_REMARK_TABLE_TYPE_ENT  remarkTableType = CPSS_DXCH_POLICER_REMARK_TABLE_TYPE_TC_UP_E;
    GT_U32                                   remarkParamValue = 0;
    CPSS_DP_LEVEL_ENT                        confLevel = CPSS_DP_GREEN_E;
    CPSS_DXCH_POLICER_QOS_PARAM_STC          qosParam;

    /* zero qosParam first */
    cpssOsBzero((GT_VOID*) &qosParam, sizeof(qosParam));

    /* prepare device iterator */
    PRV_TGF_NOT_APPLIC_DEV_RESET_MAC(&dev, UTF_CH1_E | UTF_CH1_DIAMOND_E | UTF_CH2_E | UTF_CH3_E);

    /* 1. Go over all active devices. */
    while (GT_OK == prvUtfNextDeviceGet(&dev, GT_TRUE))
    {
        if ((PRV_CPSS_PP_MAC(dev)->devFamily == CPSS_PP_FAMILY_DXCH_LION_E) &&
            (PRV_CPSS_PP_MAC(dev)->revision < 2))
            return;

        /*
            1.1. Call function with
                    remarkTableType[CPSS_DXCH_POLICER_REMARK_TABLE_TYPE_TC_UP_E /
                                    CPSS_DXCH_POLICER_REMARK_TABLE_TYPE_DSCP_E  /
                                    CPSS_DXCH_POLICER_REMARK_TABLE_TYPE_XP_E],
                    remarkParamValue [0 / 63 / 7],
                    and confLevel[CPSS_DP_GREEN_E / CPSS_DP_YELLOW_E  / CPSS_DP_RED_E].
            Expected: GT_OK.
        */
        remarkTableType = CPSS_DXCH_POLICER_REMARK_TABLE_TYPE_TC_UP_E;
        remarkParamValue = 0;
        confLevel = CPSS_DP_GREEN_E;

        /* set entry before Get to avoid trash HW values wrong parsing */
        st = cpssDxChPolicerEgressQosRemarkingEntrySet(dev, remarkTableType,
                                    remarkParamValue, confLevel, &qosParam);
        UTF_VERIFY_EQUAL1_PARAM_MAC(GT_OK, st, dev);

        st = cpssDxChPolicerEgressQosRemarkingEntryGet(dev, remarkTableType,
                                    remarkParamValue, confLevel, &qosParam);
        UTF_VERIFY_EQUAL1_PARAM_MAC(GT_OK, st, dev);

        /*
            1.1. Call function with
                    remarkTableType[CPSS_DXCH_POLICER_REMARK_TABLE_TYPE_TC_UP_E /
                                    CPSS_DXCH_POLICER_REMARK_TABLE_TYPE_DSCP_E  /
                                    CPSS_DXCH_POLICER_REMARK_TABLE_TYPE_EXP_E],
                    remarkParamValue [0 / 63 / 7],
                    and confLevel[CPSS_DP_GREEN_E / CPSS_DP_YELLOW_E  / CPSS_DP_RED_E].
            Expected: GT_OK.
        */
        remarkTableType = CPSS_DXCH_POLICER_REMARK_TABLE_TYPE_DSCP_E;
        remarkParamValue = 63;
        confLevel = CPSS_DP_YELLOW_E;
        /* set entry before Get to avoid trash HW values wrong parsing */
        st = cpssDxChPolicerEgressQosRemarkingEntrySet(dev, remarkTableType,
                                    remarkParamValue, confLevel, &qosParam);
        UTF_VERIFY_EQUAL1_PARAM_MAC(GT_OK, st, dev);

        st = cpssDxChPolicerEgressQosRemarkingEntryGet(dev, remarkTableType,
                                    remarkParamValue, confLevel, &qosParam);
        UTF_VERIFY_EQUAL1_PARAM_MAC(GT_OK, st, dev);
        /*
            1.1. Call function with
                    remarkTableType[CPSS_DXCH_POLICER_REMARK_TABLE_TYPE_TC_UP_E /
                                    CPSS_DXCH_POLICER_REMARK_TABLE_TYPE_DSCP_E  /
                                    CPSS_DXCH_POLICER_REMARK_TABLE_TYPE_EXP_E],
                    remarkParamValue [0 / 63 / 7],
                    and confLevel[CPSS_DP_GREEN_E / CPSS_DP_YELLOW_E  / CPSS_DP_RED_E].
            Expected: GT_OK.
        */
        remarkTableType = CPSS_DXCH_POLICER_REMARK_TABLE_TYPE_EXP_E;
        remarkParamValue = 7;
        confLevel = CPSS_DP_RED_E;
        /* set entry before Get to avoid trash HW values wrong parsing */
        st = cpssDxChPolicerEgressQosRemarkingEntrySet(dev, remarkTableType,
                                    remarkParamValue, confLevel, &qosParam);
        UTF_VERIFY_EQUAL1_PARAM_MAC(GT_OK, st, dev);

        st = cpssDxChPolicerEgressQosRemarkingEntryGet(dev, remarkTableType,
                                    remarkParamValue, confLevel, &qosParam);
        UTF_VERIFY_EQUAL1_PARAM_MAC(GT_OK, st, dev);

        /*
            1.2. Call api with wrong remarkTableType [wrong enum values].
            Expected: GT_BAD_PARAM.
        */
        UTF_ENUMS_CHECK_MAC(cpssDxChPolicerEgressQosRemarkingEntryGet
                            (dev, remarkTableType, remarkParamValue,
                            confLevel, &qosParam),
                            remarkTableType);

        /*
            1.3. Call api with wrong remarkParamValue (out of range).
                    remarkTableType[CPSS_DXCH_POLICER_REMARK_TABLE_TYPE_TC_UP_E /
                                    CPSS_DXCH_POLICER_REMARK_TABLE_TYPE_DSCP_E  /
                                    CPSS_DXCH_POLICER_REMARK_TABLE_TYPE_EXP_E],
                    remarkParamValue [8 / 64 / 8], and confLevel[CPSS_DP_GREEN_E].
            Expected: NOT GT_OK.
        */
        remarkTableType = CPSS_DXCH_POLICER_REMARK_TABLE_TYPE_TC_UP_E;
        remarkParamValue = 8;

        st = cpssDxChPolicerEgressQosRemarkingEntryGet(dev, remarkTableType,
                                    remarkParamValue, confLevel, &qosParam);
        UTF_VERIFY_NOT_EQUAL1_PARAM_MAC(GT_OK, st, dev);

        /*
            1.3. Call api with wrong remarkParamValue (out of range).
                    remarkTableType[CPSS_DXCH_POLICER_REMARK_TABLE_TYPE_TC_UP_E /
                                    CPSS_DXCH_POLICER_REMARK_TABLE_TYPE_DSCP_E  /
                                    CPSS_DXCH_POLICER_REMARK_TABLE_TYPE_EXP_E],
                    remarkParamValue [8 / 64 / 8], and confLevel[CPSS_DP_GREEN_E].
            Expected: NOT GT_OK.
        */
        remarkTableType = CPSS_DXCH_POLICER_REMARK_TABLE_TYPE_DSCP_E;
        remarkParamValue = 64;

        st = cpssDxChPolicerEgressQosRemarkingEntryGet(dev, remarkTableType,
                                    remarkParamValue, confLevel, &qosParam);
        UTF_VERIFY_NOT_EQUAL1_PARAM_MAC(GT_OK, st, dev);

        /*
            1.3. Call api with wrong remarkParamValue (out of range).
                    remarkTableType[CPSS_DXCH_POLICER_REMARK_TABLE_TYPE_TC_UP_E /
                                    CPSS_DXCH_POLICER_REMARK_TABLE_TYPE_DSCP_E  /
                                    CPSS_DXCH_POLICER_REMARK_TABLE_TYPE_EXP_E],
                    remarkParamValue [8 / 64 / 8], and confLevel[CPSS_DP_GREEN_E].
            Expected: NOT GT_OK.
        */
        remarkTableType = CPSS_DXCH_POLICER_REMARK_TABLE_TYPE_EXP_E;
        remarkParamValue = 8;

        st = cpssDxChPolicerEgressQosRemarkingEntryGet(dev, remarkTableType,
                                    remarkParamValue, confLevel, &qosParam);
        UTF_VERIFY_NOT_EQUAL1_PARAM_MAC(GT_OK, st, dev);

        remarkParamValue = 0;

        /*
            1.4. Call api with wrong confLevel [wrong enum values].
            Expected: GT_BAD_PARAM.
        */
        UTF_ENUMS_CHECK_MAC(cpssDxChPolicerEgressQosRemarkingEntryGet
                            (dev, remarkTableType, remarkParamValue,
                            confLevel, &qosParam),
                            confLevel);

        /*
            1.5. Call api with wrong qosParamPtr [NULL].
            Expected: GT_BAD_PTR.
        */
        st = cpssDxChPolicerEgressQosRemarkingEntryGet(dev, remarkTableType,
                                    remarkParamValue, confLevel, NULL);
        UTF_VERIFY_EQUAL1_PARAM_MAC(GT_BAD_PTR, st, dev);
    }

    /* restore correct values */
    remarkTableType = CPSS_DXCH_POLICER_REMARK_TABLE_TYPE_TC_UP_E;
    remarkParamValue = 0;
    confLevel = CPSS_DP_GREEN_E;

    /* 2. For not-active devices and devices from non-applicable family */
    /* check that function returns GT_BAD_PARAM.                        */

    /* prepare device iterator */
    PRV_TGF_NOT_APPLIC_DEV_RESET_MAC(&dev, UTF_CH1_E | UTF_CH1_DIAMOND_E | UTF_CH2_E | UTF_CH3_E);

    /* go over all non active devices */
    while(GT_OK == prvUtfNextDeviceGet(&dev, GT_FALSE))
    {
        if ((PRV_CPSS_PP_MAC(dev)->devFamily == CPSS_PP_FAMILY_DXCH_LION_E) &&
            (PRV_CPSS_PP_MAC(dev)->revision < 2))
            return;

        st = cpssDxChPolicerEgressQosRemarkingEntryGet(dev, remarkTableType,
                                    remarkParamValue, confLevel, &qosParam);
        UTF_VERIFY_EQUAL1_PARAM_MAC(GT_NOT_APPLICABLE_DEVICE, st, dev);
    }

    /* 3. Call function with out of bound value for device id.*/
    dev = PRV_CPSS_MAX_PP_DEVICES_CNS;

    st = cpssDxChPolicerEgressQosRemarkingEntryGet(dev, remarkTableType,
                                    remarkParamValue, confLevel, &qosParam);
    UTF_VERIFY_EQUAL1_PARAM_MAC(GT_BAD_PARAM, st, dev);
}

/*----------------------------------------------------------------------------*/
/*
GT_STATUS cpssDxChPolicerEgressQosRemarkingEntrySet
(
    IN  GT_U8                                    devNum,
    IN  CPSS_DXCH_POLICER_REMARK_TABLE_TYPE_ENT  remarkTableType,
    IN  GT_U32                                   remarkParamValue,
    IN  CPSS_DP_LEVEL_ENT                        confLevel,
    IN  CPSS_DXCH_POLICER_QOS_PARAM_STC          *qosParamPtr
)
*/
UTF_TEST_CASE_MAC(cpssDxChPolicerEgressQosRemarkingEntrySet)
{
/*
ITERATE_DEVICES(DxChXcat and above)
    1.1. Call function with
            remarkTableType[CPSS_DXCH_POLICER_REMARK_TABLE_TYPE_TC_UP_E /
                            CPSS_DXCH_POLICER_REMARK_TABLE_TYPE_DSCP_E  /
                            CPSS_DXCH_POLICER_REMARK_TABLE_TYPE_EXP_E],
            remarkParamValue [0 / 63 / 7],
            confLevel[CPSS_DP_GREEN_E / CPSS_DP_YELLOW_E  / CPSS_DP_RED_E],
            qosParam.up   [0 / 3 / 7],
            qosParam.dscp [0 / 33 / 63],
            qosParam.exp  [0 / 3 / 7],
            qosParam.dp   [CPSS_DP_GREEN_E / CPSS_DP_YELLOW_E  / CPSS_DP_RED_E].

    Expected: GT_OK.
    1.2. Call cpssDxChPolicerEgressQosRemarkingEntryGet
         with the same params.
    Expected: GT_OK and the same values.
    1.3. Call api with wrong remarkTableType [wrong enum values].
    Expected: GT_BAD_PARAM.
    1.4. Call api with wrong remarkParamValue (out of range).
            remarkTableType[CPSS_DXCH_POLICER_REMARK_TABLE_TYPE_TC_UP_E /
                            CPSS_DXCH_POLICER_REMARK_TABLE_TYPE_DSCP_E  /
                            CPSS_DXCH_POLICER_REMARK_TABLE_TYPE_EXP_E],
            remarkParamValue [8 / 64 / 8], and confLevel[CPSS_DP_GREEN_E].
    Expected: NOT GT_OK.
    1.5. Call api with wrong confLevel [wrong enum values].
    Expected: GT_BAD_PARAM.
    1.6. Call api with wrong qosParamPtr [NULL].
    Expected: GT_BAD_PTR.
    1.7. Call function with wrong qosParam values (out of range)
            remarkTableType[CPSS_DXCH_POLICER_REMARK_TABLE_TYPE_TC_UP_E /
                            CPSS_DXCH_POLICER_REMARK_TABLE_TYPE_DSCP_E  /
                            CPSS_DXCH_POLICER_REMARK_TABLE_TYPE_EXP_E],
            remarkParamValue [0],
            confLevel[CPSS_DP_GREEN_E],
            qosParam.up   [8 / 0 / 0] (out of range),
            qosParam.dscp [0 / 64 / 0] (out of range),
            qosParam.exp  [0 / 0 / 8] (out of range),
            qosParam.dp   [CPSS_DP_GREEN_E].

    Expected: NOT GT_OK.
*/
    GT_STATUS st = GT_OK;

    GT_U8                                    dev;
    CPSS_DXCH_POLICER_REMARK_TABLE_TYPE_ENT  remarkTableType = CPSS_DXCH_POLICER_REMARK_TABLE_TYPE_TC_UP_E;
    GT_U32                                   remarkParamValue = 0;
    CPSS_DP_LEVEL_ENT                        confLevel = CPSS_DP_GREEN_E;
    CPSS_DXCH_POLICER_QOS_PARAM_STC          qosParam;
    CPSS_DXCH_POLICER_QOS_PARAM_STC          qosParamGet;

    /* zero qosParam first */
    cpssOsBzero((GT_VOID*) &qosParam, sizeof(qosParam));
    cpssOsBzero((GT_VOID*) &qosParamGet, sizeof(qosParamGet));

    /* prepare device iterator */
    PRV_TGF_NOT_APPLIC_DEV_RESET_MAC(&dev, UTF_CH1_E | UTF_CH1_DIAMOND_E | UTF_CH2_E | UTF_CH3_E);

    /* 1. Go over all active devices. */
    while (GT_OK == prvUtfNextDeviceGet(&dev, GT_TRUE))
    {
        if ((PRV_CPSS_PP_MAC(dev)->devFamily == CPSS_PP_FAMILY_DXCH_LION_E) &&
            (PRV_CPSS_PP_MAC(dev)->revision < 2))
            return;

        /*
            1.1. Call function with
                    remarkTableType[CPSS_DXCH_POLICER_REMARK_TABLE_TYPE_TC_UP_E /
                                    CPSS_DXCH_POLICER_REMARK_TABLE_TYPE_DSCP_E  /
                                    CPSS_DXCH_POLICER_REMARK_TABLE_TYPE_EXP_E],
                    remarkParamValue [0 / 63 / 7],
                    confLevel[CPSS_DP_GREEN_E / CPSS_DP_YELLOW_E  / CPSS_DP_RED_E],
                    qosParam.up   [0 / 3 / 7],
                    qosParam.dscp [0 / 33 / 63],
                    qosParam.exp  [0 / 3 / 7],
                    qosParam.dp   [CPSS_DP_GREEN_E / CPSS_DP_YELLOW_E  / CPSS_DP_RED_E].

            Expected: GT_OK.
        */
        remarkTableType = CPSS_DXCH_POLICER_REMARK_TABLE_TYPE_TC_UP_E;
        remarkParamValue = 0;
        confLevel = CPSS_DP_GREEN_E;
        qosParam.up = 0;
        qosParam.dscp = 0;
        qosParam.exp = 0;
        qosParam.dp = CPSS_DP_GREEN_E;

        st = cpssDxChPolicerEgressQosRemarkingEntrySet(dev, remarkTableType,
                                    remarkParamValue, confLevel, &qosParam);
        UTF_VERIFY_EQUAL1_PARAM_MAC(GT_OK, st, dev);

        /*
            1.2. Call cpssDxChPolicerEgressQosRemarkingEntryGet
                 with the same params.
            Expected: GT_OK and the same values.
        */
        st = cpssDxChPolicerEgressQosRemarkingEntryGet(dev, remarkTableType,
                                    remarkParamValue, confLevel, &qosParamGet);
        UTF_VERIFY_EQUAL1_STRING_MAC(GT_OK, st,
               "cpssDxChPolicerEgressQosRemarkingEntryGet: %d", dev);

        /* verifying values */
        UTF_VERIFY_EQUAL1_STRING_MAC(qosParam.up, qosParamGet.up,
                   "got another qosParam.up then was set: %d", dev);
        UTF_VERIFY_EQUAL1_STRING_MAC(qosParam.dscp, qosParamGet.dscp,
                   "got another qosParam.dscp then was set: %d", dev);
        UTF_VERIFY_EQUAL1_STRING_MAC(qosParam.exp, qosParamGet.exp,
                   "got another qosParam.exp then was set: %d", dev);
        UTF_VERIFY_EQUAL1_STRING_MAC(qosParam.dp, qosParamGet.dp,
                   "got another qosParam.dp then was set: %d", dev);

        /*
            1.1. Call function with
                    remarkTableType[CPSS_DXCH_POLICER_REMARK_TABLE_TYPE_TC_UP_E /
                                    CPSS_DXCH_POLICER_REMARK_TABLE_TYPE_DSCP_E  /
                                    CPSS_DXCH_POLICER_REMARK_TABLE_TYPE_EXP_E],
                    remarkParamValue [0 / 63 / 7],
                    confLevel[CPSS_DP_GREEN_E / CPSS_DP_YELLOW_E  / CPSS_DP_RED_E],
                    qosParam.up   [0 / 3 / 7],
                    qosParam.dscp [0 / 33 / 63],
                    qosParam.exp  [0 / 3 / 7],
                    qosParam.dp   [CPSS_DP_GREEN_E / CPSS_DP_YELLOW_E  / CPSS_DP_RED_E].

            Expected: GT_OK.
        */
        remarkTableType = CPSS_DXCH_POLICER_REMARK_TABLE_TYPE_DSCP_E;
        remarkParamValue = 63;
        confLevel = CPSS_DP_YELLOW_E;
        qosParam.up = 3;
        qosParam.dscp = 33;
        qosParam.exp = 3;
        qosParam.dp = CPSS_DP_YELLOW_E;

        st = cpssDxChPolicerEgressQosRemarkingEntrySet(dev, remarkTableType,
                                    remarkParamValue, confLevel, &qosParam);
        UTF_VERIFY_EQUAL1_PARAM_MAC(GT_OK, st, dev);

        /*
            1.2. Call cpssDxChPolicerEgressQosRemarkingEntryGet
                 with the same params.
            Expected: GT_OK and the same values.
        */
        st = cpssDxChPolicerEgressQosRemarkingEntryGet(dev, remarkTableType,
                                    remarkParamValue, confLevel, &qosParamGet);
        UTF_VERIFY_EQUAL1_STRING_MAC(GT_OK, st,
               "cpssDxChPolicerEgressQosRemarkingEntryGet: %d", dev);

        /* verifying values */
        UTF_VERIFY_EQUAL1_STRING_MAC(qosParam.up, qosParamGet.up,
                   "got another qosParam.up then was set: %d", dev);
        UTF_VERIFY_EQUAL1_STRING_MAC(qosParam.dscp, qosParamGet.dscp,
                   "got another qosParam.dscp then was set: %d", dev);
        UTF_VERIFY_EQUAL1_STRING_MAC(qosParam.exp, qosParamGet.exp,
                   "got another qosParam.exp then was set: %d", dev);
        UTF_VERIFY_EQUAL1_STRING_MAC(qosParam.dp, qosParamGet.dp,
                   "got another qosParam.dp then was set: %d", dev);

        /*
            1.1. Call function with
                    remarkTableType[CPSS_DXCH_POLICER_REMARK_TABLE_TYPE_TC_UP_E /
                                    CPSS_DXCH_POLICER_REMARK_TABLE_TYPE_DSCP_E  /
                                    CPSS_DXCH_POLICER_REMARK_TABLE_TYPE_EXP_E],
                    remarkParamValue [0 / 63 / 7],
                    confLevel[CPSS_DP_GREEN_E / CPSS_DP_YELLOW_E  / CPSS_DP_RED_E],
                    qosParam.up   [0 / 3 / 7],
                    qosParam.dscp [0 / 33 / 63],
                    qosParam.exp  [0 / 3 / 7],
                    qosParam.dp   [CPSS_DP_GREEN_E / CPSS_DP_YELLOW_E  / CPSS_DP_RED_E].

            Expected: GT_OK.
        */
        remarkTableType = CPSS_DXCH_POLICER_REMARK_TABLE_TYPE_EXP_E;
        remarkParamValue = 7;
        confLevel = CPSS_DP_RED_E;
        qosParam.up = 7;
        qosParam.dscp = 63;
        qosParam.exp = 7;
        qosParam.dp = CPSS_DP_RED_E;

        st = cpssDxChPolicerEgressQosRemarkingEntrySet(dev, remarkTableType,
                                    remarkParamValue, confLevel, &qosParam);
        UTF_VERIFY_EQUAL1_PARAM_MAC(GT_OK, st, dev);

        /*
            1.2. Call cpssDxChPolicerEgressQosRemarkingEntryGet
                 with the same params.
            Expected: GT_OK and the same values.
        */
        st = cpssDxChPolicerEgressQosRemarkingEntryGet(dev, remarkTableType,
                                    remarkParamValue, confLevel, &qosParamGet);
        UTF_VERIFY_EQUAL1_STRING_MAC(GT_OK, st,
               "cpssDxChPolicerEgressQosRemarkingEntryGet: %d", dev);

        /* verifying values */
        UTF_VERIFY_EQUAL1_STRING_MAC(qosParam.up, qosParamGet.up,
                   "got another qosParam.up then was set: %d", dev);
        UTF_VERIFY_EQUAL1_STRING_MAC(qosParam.dscp, qosParamGet.dscp,
                   "got another qosParam.dscp then was set: %d", dev);
        UTF_VERIFY_EQUAL1_STRING_MAC(qosParam.exp, qosParamGet.exp,
                   "got another qosParam.exp then was set: %d", dev);
        UTF_VERIFY_EQUAL1_STRING_MAC(qosParam.dp, qosParamGet.dp,
                   "got another qosParam.dp then was set: %d", dev);

        /*
            1.3. Call api with wrong remarkTableType [wrong enum values].
            Expected: GT_BAD_PARAM.
        */
        UTF_ENUMS_CHECK_MAC(cpssDxChPolicerEgressQosRemarkingEntrySet
                            (dev, remarkTableType, remarkParamValue,
                            confLevel, &qosParam),
                            remarkTableType);

        /*
            1.4. Call api with wrong remarkParamValue (out of range).
                    remarkTableType[CPSS_DXCH_POLICER_REMARK_TABLE_TYPE_TC_UP_E /
                                    CPSS_DXCH_POLICER_REMARK_TABLE_TYPE_DSCP_E  /
                                    CPSS_DXCH_POLICER_REMARK_TABLE_TYPE_EXP_E],
                    remarkParamValue [8 / 64 / 8], and confLevel[CPSS_DP_GREEN_E].
            Expected: NOT GT_OK.
        */
        remarkTableType = CPSS_DXCH_POLICER_REMARK_TABLE_TYPE_TC_UP_E;
        remarkParamValue = 8;

        st = cpssDxChPolicerEgressQosRemarkingEntrySet(dev, remarkTableType,
                                    remarkParamValue, confLevel, &qosParam);
        UTF_VERIFY_NOT_EQUAL1_PARAM_MAC(GT_OK, st, dev);

        /*
            1.4. Call api with wrong remarkParamValue (out of range).
                    remarkTableType[CPSS_DXCH_POLICER_REMARK_TABLE_TYPE_TC_UP_E /
                                    CPSS_DXCH_POLICER_REMARK_TABLE_TYPE_DSCP_E  /
                                    CPSS_DXCH_POLICER_REMARK_TABLE_TYPE_EXP_E],
                    remarkParamValue [8 / 64 / 8], and confLevel[CPSS_DP_GREEN_E].
            Expected: NOT GT_OK.
        */
        remarkTableType = CPSS_DXCH_POLICER_REMARK_TABLE_TYPE_DSCP_E;
        remarkParamValue = 64;

        st = cpssDxChPolicerEgressQosRemarkingEntrySet(dev, remarkTableType,
                                    remarkParamValue, confLevel, &qosParam);
        UTF_VERIFY_NOT_EQUAL1_PARAM_MAC(GT_OK, st, dev);

        /*
            1.4. Call api with wrong remarkParamValue (out of range).
                    remarkTableType[CPSS_DXCH_POLICER_REMARK_TABLE_TYPE_TC_UP_E /
                                    CPSS_DXCH_POLICER_REMARK_TABLE_TYPE_DSCP_E  /
                                    CPSS_DXCH_POLICER_REMARK_TABLE_TYPE_EXP_E],
                    remarkParamValue [8 / 64 / 8], and confLevel[CPSS_DP_GREEN_E].
            Expected: NOT GT_OK.
        */
        remarkTableType = CPSS_DXCH_POLICER_REMARK_TABLE_TYPE_EXP_E;
        remarkParamValue = 8;

        st = cpssDxChPolicerEgressQosRemarkingEntrySet(dev, remarkTableType,
                                    remarkParamValue, confLevel, &qosParam);
        UTF_VERIFY_NOT_EQUAL1_PARAM_MAC(GT_OK, st, dev);

        remarkParamValue = 0;

        /*
            1.5. Call api with wrong confLevel [wrong enum values].
            Expected: GT_BAD_PARAM.
        */
        UTF_ENUMS_CHECK_MAC(cpssDxChPolicerEgressQosRemarkingEntrySet
                            (dev, remarkTableType, remarkParamValue,
                            confLevel, &qosParam),
                            confLevel);

        /*
            1.6. Call api with wrong qosParamPtr [NULL].
            Expected: GT_BAD_PTR.
        */
        st = cpssDxChPolicerEgressQosRemarkingEntrySet(dev, remarkTableType,
                                    remarkParamValue, confLevel, NULL);
        UTF_VERIFY_EQUAL1_PARAM_MAC(GT_BAD_PTR, st, dev);

        /*
            1.7. Call function with wrong qosParam values (out of range)
                    remarkTableType[CPSS_DXCH_POLICER_REMARK_TABLE_TYPE_TC_UP_E /
                                    CPSS_DXCH_POLICER_REMARK_TABLE_TYPE_DSCP_E  /
                                    CPSS_DXCH_POLICER_REMARK_TABLE_TYPE_EXP_E],
                    remarkParamValue [0],
                    confLevel[CPSS_DP_GREEN_E],
                    qosParam.up   [8 / 0 / 0] (out of range),
                    qosParam.dscp [0 / 64 / 0] (out of range),
                    qosParam.exp  [0 / 0 / 8] (out of range),
                    qosParam.dp   [CPSS_DP_GREEN_E].

            Expected: NOT GT_OK.
        */
        remarkTableType = CPSS_DXCH_POLICER_REMARK_TABLE_TYPE_TC_UP_E;
        remarkParamValue = 0;
        confLevel = CPSS_DP_GREEN_E;
        qosParam.up = 8;
        qosParam.dscp = 0;
        qosParam.exp = 0;
        qosParam.dp = CPSS_DP_GREEN_E;

        st = cpssDxChPolicerEgressQosRemarkingEntrySet(dev, remarkTableType,
                                    remarkParamValue, confLevel, &qosParam);
        UTF_VERIFY_NOT_EQUAL1_PARAM_MAC(GT_OK, st, dev);

        /*
            1.7. Call function with wrong qosParam values (out of range)
                    remarkTableType[CPSS_DXCH_POLICER_REMARK_TABLE_TYPE_TC_UP_E /
                                    CPSS_DXCH_POLICER_REMARK_TABLE_TYPE_DSCP_E  /
                                    CPSS_DXCH_POLICER_REMARK_TABLE_TYPE_EXP_E],
                    remarkParamValue [0],
                    confLevel[CPSS_DP_GREEN_E],
                    qosParam.up   [8 / 0 / 0] (out of range),
                    qosParam.dscp [0 / 64 / 0] (out of range),
                    qosParam.exp  [0 / 0 / 8] (out of range),
                    qosParam.dp   [CPSS_DP_GREEN_E].

            Expected: NOT GT_OK.
        */
        remarkTableType = CPSS_DXCH_POLICER_REMARK_TABLE_TYPE_DSCP_E;
        qosParam.up = 0;
        qosParam.dscp = 64;
        qosParam.exp = 0;

        st = cpssDxChPolicerEgressQosRemarkingEntrySet(dev, remarkTableType,
                                    remarkParamValue, confLevel, &qosParam);
        UTF_VERIFY_NOT_EQUAL1_PARAM_MAC(GT_OK, st, dev);

        /*
            1.7. Call function with wrong qosParam values (out of range)
                    remarkTableType[CPSS_DXCH_POLICER_REMARK_TABLE_TYPE_TC_UP_E /
                                    CPSS_DXCH_POLICER_REMARK_TABLE_TYPE_DSCP_E  /
                                    CPSS_DXCH_POLICER_REMARK_TABLE_TYPE_EXP_E],
                    remarkParamValue [0],
                    confLevel[CPSS_DP_GREEN_E],
                    qosParam.up   [8 / 0 / 0] (out of range),
                    qosParam.dscp [0 / 64 / 0] (out of range),
                    qosParam.exp  [0 / 0 / 8] (out of range),
                    qosParam.dp   [CPSS_DP_GREEN_E].

            Expected: NOT GT_OK.
        */
        remarkTableType = CPSS_DXCH_POLICER_REMARK_TABLE_TYPE_EXP_E;
        qosParam.up = 0;
        qosParam.dscp = 0;
        qosParam.exp = 8;

        st = cpssDxChPolicerEgressQosRemarkingEntrySet(dev, remarkTableType,
                                    remarkParamValue, confLevel, &qosParam);
        UTF_VERIFY_NOT_EQUAL1_PARAM_MAC(GT_OK, st, dev);
    }

    /* restore correct values */
    remarkTableType = CPSS_DXCH_POLICER_REMARK_TABLE_TYPE_TC_UP_E;
    remarkParamValue = 0;
    confLevel = CPSS_DP_GREEN_E;

    /* 2. For not-active devices and devices from non-applicable family */
    /* check that function returns GT_BAD_PARAM.                        */

    /* prepare device iterator */
    PRV_TGF_NOT_APPLIC_DEV_RESET_MAC(&dev, UTF_CH1_E | UTF_CH1_DIAMOND_E | UTF_CH2_E | UTF_CH3_E);

    /* go over all non active devices */
    while(GT_OK == prvUtfNextDeviceGet(&dev, GT_FALSE))
    {
        if ((PRV_CPSS_PP_MAC(dev)->devFamily == CPSS_PP_FAMILY_DXCH_LION_E) &&
            (PRV_CPSS_PP_MAC(dev)->revision < 2))
            return;

        st = cpssDxChPolicerEgressQosRemarkingEntrySet(dev, remarkTableType,
                                    remarkParamValue, confLevel, &qosParam);
        UTF_VERIFY_EQUAL1_PARAM_MAC(GT_NOT_APPLICABLE_DEVICE, st, dev);
    }

    /* 3. Call function with out of bound value for device id.*/
    dev = PRV_CPSS_MAX_PP_DEVICES_CNS;

    st = cpssDxChPolicerEgressQosRemarkingEntrySet(dev, remarkTableType,
                                    remarkParamValue, confLevel, &qosParam);
    UTF_VERIFY_EQUAL1_PARAM_MAC(GT_BAD_PARAM, st, dev);
}

/*----------------------------------------------------------------------------*/
/*
GT_STATUS cpssDxChPolicerEgressQosUpdateEnableGet
(
    IN  GT_U8    devNum,
    OUT GT_BOOL  *enablePtr
)
*/
UTF_TEST_CASE_MAC(cpssDxChPolicerEgressQosUpdateEnableGet)
{
/*
    ITERATE_DEVICES(DxChXcat and above)
    1.1. Call function with not null enablePtr.
    Expected: GT_OK.
    1.1. Call api with wrong enablePtr [NULL].
    Expected: GT_BAD_PTR.
*/
    GT_STATUS st = GT_OK;

    GT_U8    dev;
    GT_BOOL  enable;

    /* prepare device iterator */
    PRV_TGF_NOT_APPLIC_DEV_RESET_MAC(&dev, UTF_CH1_E | UTF_CH1_DIAMOND_E | UTF_CH2_E | UTF_CH3_E);

    /* 1. Go over all active devices. */
    while (GT_OK == prvUtfNextDeviceGet(&dev, GT_TRUE))
    {
        /*
            1.1. Call function with not null enablePtr.
            Expected: GT_OK.
        */
        st = cpssDxChPolicerEgressQosUpdateEnableGet(dev, &enable);
        UTF_VERIFY_EQUAL1_PARAM_MAC(GT_OK, st, dev);

        /*
            1.1. Call api with wrong enablePtr [NULL].
            Expected: GT_BAD_PTR.
        */
        st = cpssDxChPolicerEgressQosUpdateEnableGet(dev, NULL);
        UTF_VERIFY_EQUAL1_PARAM_MAC(GT_BAD_PTR, st, dev);
    }

    /* 2. For not-active devices and devices from non-applicable family */
    /* check that function returns GT_BAD_PARAM.                        */

    /* prepare device iterator */
    PRV_TGF_NOT_APPLIC_DEV_RESET_MAC(&dev, UTF_CH1_E | UTF_CH1_DIAMOND_E | UTF_CH2_E | UTF_CH3_E);

    /* go over all non active devices */
    while(GT_OK == prvUtfNextDeviceGet(&dev, GT_FALSE))
    {
        st = cpssDxChPolicerEgressQosUpdateEnableGet(dev, &enable);
        UTF_VERIFY_EQUAL1_PARAM_MAC(GT_NOT_APPLICABLE_DEVICE, st, dev);
    }

    /* 3. Call function with out of bound value for device id.*/
    dev = PRV_CPSS_MAX_PP_DEVICES_CNS;

    st = cpssDxChPolicerEgressQosUpdateEnableGet(dev, &enable);
    UTF_VERIFY_EQUAL1_PARAM_MAC(GT_BAD_PARAM, st, dev);
}

/*----------------------------------------------------------------------------*/
/*
GT_STATUS cpssDxChPolicerEgressQosUpdateEnableSet
(
    IN  GT_U8   devNum,
    IN  GT_BOOL enable
)
*/
UTF_TEST_CASE_MAC(cpssDxChPolicerEgressQosUpdateEnableSet)
{
/*
    ITERATE_DEVICES(DxChXcat and above)
    1.1. Call function with enable [GT_FALSE / GT_TRUE]
    Expected: GT_OK.
    1.2. Call cpssDxChPolicerEgressQosUpdateEnableGet
    Expected: GT_OK.
*/
    GT_STATUS st = GT_OK;

    GT_U8   dev;
    GT_BOOL enable = GT_FALSE;
    GT_BOOL enableGet;

    /* prepare device iterator */
    PRV_TGF_NOT_APPLIC_DEV_RESET_MAC(&dev, UTF_CH1_E | UTF_CH1_DIAMOND_E | UTF_CH2_E | UTF_CH3_E);

    /* 1. Go over all active devices. */
    while (GT_OK == prvUtfNextDeviceGet(&dev, GT_TRUE))
    {
        /*
            1.1. Call function with enable [GT_FALSE / GT_TRUE]
            Expected: GT_OK.
        */
        /*call with enable = GT_FALSE;*/
        enable = GT_FALSE;
        st = cpssDxChPolicerEgressQosUpdateEnableSet(dev, enable);
        UTF_VERIFY_EQUAL1_PARAM_MAC(GT_OK, st, dev);

        /*
            1.2. Call cpssDxChPolicerEgressQosUpdateEnableGet
            Expected: GT_OK.
        */
        st = cpssDxChPolicerEgressQosUpdateEnableGet(dev, &enableGet);
        UTF_VERIFY_EQUAL1_STRING_MAC(GT_OK, st,
               "cpssDxChPolicerEgressQosUpdateEnableGet: %d", dev);

        /* verifying values */
        UTF_VERIFY_EQUAL1_STRING_MAC(enable, enableGet,
                   "got another enable then was set: %d", dev);

        /*call with enable = GT_TRUE;*/
        enable = GT_TRUE;
        st = cpssDxChPolicerEgressQosUpdateEnableSet(dev, enable);
        UTF_VERIFY_EQUAL1_PARAM_MAC(GT_OK, st, dev);

        /*
            1.2. Call cpssDxChPolicerEgressQosUpdateEnableGet
            Expected: GT_OK.
        */
        st = cpssDxChPolicerEgressQosUpdateEnableGet(dev, &enableGet);
        UTF_VERIFY_EQUAL1_STRING_MAC(GT_OK, st,
               "cpssDxChPolicerEgressQosUpdateEnableGet: %d", dev);

        /* verifying values */
        UTF_VERIFY_EQUAL1_STRING_MAC(enable, enableGet,
                   "got another enable then was set: %d", dev);
    }

    /* restore correct values */
    enable = GT_TRUE;

    /* 2. For not-active devices and devices from non-applicable family */
    /* check that function returns GT_BAD_PARAM.                        */

    /* prepare device iterator */
    PRV_TGF_NOT_APPLIC_DEV_RESET_MAC(&dev, UTF_CH1_E | UTF_CH1_DIAMOND_E | UTF_CH2_E | UTF_CH3_E);

    /* go over all non active devices */
    while(GT_OK == prvUtfNextDeviceGet(&dev, GT_FALSE))
    {
        st = cpssDxChPolicerEgressQosUpdateEnableSet(dev, enable);
        UTF_VERIFY_EQUAL1_PARAM_MAC(GT_NOT_APPLICABLE_DEVICE, st, dev);
    }

    /* 3. Call function with out of bound value for device id.*/
    dev = PRV_CPSS_MAX_PP_DEVICES_CNS;

    st = cpssDxChPolicerEgressQosUpdateEnableSet(dev, enable);
    UTF_VERIFY_EQUAL1_PARAM_MAC(GT_BAD_PARAM, st, dev);
}

/*----------------------------------------------------------------------------*/
/*
GT_STATUS cpssDxChPolicerMemorySizeModeGet
(
    IN  GT_U8                                   devNum,
    OUT CPSS_DXCH_POLICER_MEMORY_CTRL_MODE_ENT  *modePtr
)
*/
UTF_TEST_CASE_MAC(cpssDxChPolicerMemorySizeModeGet)
{
/*
    ITERATE_DEVICES(DxChXcat and above)
    1.1. Call function with not null pointer.
    Expected: GT_OK.
    1.2. Call api with wrong modePtr [NULL].
    Expected: GT_BAD_PTR.
*/
    GT_STATUS st = GT_OK;

    GT_U8                                   dev;
    CPSS_DXCH_POLICER_MEMORY_CTRL_MODE_ENT  mode;

    /* prepare device iterator */
    PRV_TGF_NOT_APPLIC_DEV_RESET_MAC(&dev, UTF_CH1_E | UTF_CH1_DIAMOND_E | UTF_CH2_E | UTF_CH3_E);

    /* 1. Go over all active devices. */
    while (GT_OK == prvUtfNextDeviceGet(&dev, GT_TRUE))
    {
        /*
            1.1. Call function with not null pointer.
            Expected: GT_OK.
        */
        st = cpssDxChPolicerMemorySizeModeGet(dev, &mode);
        UTF_VERIFY_EQUAL1_PARAM_MAC(GT_OK, st, dev);

        /*
            1.2. Call api with wrong modePtr [NULL].
            Expected: GT_BAD_PTR.
        */
        st = cpssDxChPolicerMemorySizeModeGet(dev, NULL);
        UTF_VERIFY_EQUAL1_PARAM_MAC(GT_BAD_PTR, st, dev);
    }

    /* restore correct values */

    /* 2. For not-active devices and devices from non-applicable family */
    /* check that function returns GT_BAD_PARAM.                        */

    /* prepare device iterator */
    PRV_TGF_NOT_APPLIC_DEV_RESET_MAC(&dev, UTF_CH1_E | UTF_CH1_DIAMOND_E | UTF_CH2_E | UTF_CH3_E);

    /* go over all non active devices */
    while(GT_OK == prvUtfNextDeviceGet(&dev, GT_FALSE))
    {
        st = cpssDxChPolicerMemorySizeModeGet(dev, &mode);
        UTF_VERIFY_EQUAL1_PARAM_MAC(GT_NOT_APPLICABLE_DEVICE, st, dev);
    }

    /* 3. Call function with out of bound value for device id.*/
    dev = PRV_CPSS_MAX_PP_DEVICES_CNS;

    st = cpssDxChPolicerMemorySizeModeGet(dev, &mode);
    UTF_VERIFY_EQUAL1_PARAM_MAC(GT_BAD_PARAM, st, dev);
}

/*----------------------------------------------------------------------------*/
/*
GT_STATUS cpssDxChPolicerMemorySizeModeSet
(
    IN  GT_U8                                  devNum,
    IN  CPSS_DXCH_POLICER_MEMORY_CTRL_MODE_ENT mode
)
*/
UTF_TEST_CASE_MAC(cpssDxChPolicerMemorySizeModeSet)
{
/*
    ITERATE_DEVICES(DxChXcat and above)
    1.1. Call function with mode
        [CPSS_DXCH_POLICER_MEMORY_CTRL_MODE_PLR0_UPPER_PLR1_LOWER_E/
         CPSS_DXCH_POLICER_MEMORY_CTRL_MODE_PLR0_UPPER_AND_LOWER_E/
         CPSS_DXCH_POLICER_MEMORY_CTRL_MODE_PLR1_UPPER_AND_LOWER_E/
         CPSS_DXCH_POLICER_MEMORY_CTRL_MODE_PLR1_UPPER_PLR0_LOWER_E]
    Expected: GT_OK.
    1.2. Call cpssDxChPolicerMemorySizeModeGet.
    Expected: GT_OK and the same value.
    1.3. Call api with wrong mode [wrong enum values].
    Expected: GT_BAD_PARAM.
*/
    GT_STATUS st = GT_OK;

    GT_U8                                  dev;
    CPSS_DXCH_POLICER_MEMORY_CTRL_MODE_ENT mode = CPSS_DXCH_POLICER_MEMORY_CTRL_MODE_PLR0_UPPER_PLR1_LOWER_E;
    CPSS_DXCH_POLICER_MEMORY_CTRL_MODE_ENT modeGet;

    /* prepare device iterator */
    PRV_TGF_NOT_APPLIC_DEV_RESET_MAC(&dev, UTF_CH1_E | UTF_CH1_DIAMOND_E | UTF_CH2_E | UTF_CH3_E);

    /* 1. Go over all active devices. */
    while (GT_OK == prvUtfNextDeviceGet(&dev, GT_TRUE))
    {
        /*
            1.1. Call function with mode
                [CPSS_DXCH_POLICER_MEMORY_CTRL_MODE_PLR0_UPPER_PLR1_LOWER_E/
                 CPSS_DXCH_POLICER_MEMORY_CTRL_MODE_PLR0_UPPER_AND_LOWER_E/
                 CPSS_DXCH_POLICER_MEMORY_CTRL_MODE_PLR1_UPPER_AND_LOWER_E/
                 CPSS_DXCH_POLICER_MEMORY_CTRL_MODE_PLR1_UPPER_PLR0_LOWER_E]
            Expected: GT_OK.
        */
        mode = CPSS_DXCH_POLICER_MEMORY_CTRL_MODE_PLR0_UPPER_PLR1_LOWER_E;

        st = cpssDxChPolicerMemorySizeModeSet(dev, mode);
        UTF_VERIFY_EQUAL1_PARAM_MAC(GT_OK, st, dev);

        /*
            1.2. Call cpssDxChPolicerMemorySizeModeGet.
            Expected: GT_OK and the same value.
        */
        st = cpssDxChPolicerMemorySizeModeGet(dev, &modeGet);
        UTF_VERIFY_EQUAL1_STRING_MAC(GT_OK, st,
               "cpssDxChPolicerMemorySizeModeGet: %d", dev);

        /* verifying values */
        UTF_VERIFY_EQUAL1_STRING_MAC(mode, modeGet,
                   "got another mode then was set: %d", dev);

        /*
            1.1. Call function with mode
                [CPSS_DXCH_POLICER_MEMORY_CTRL_MODE_PLR0_UPPER_PLR1_LOWER_E/
                 CPSS_DXCH_POLICER_MEMORY_CTRL_MODE_PLR0_UPPER_AND_LOWER_E/
                 CPSS_DXCH_POLICER_MEMORY_CTRL_MODE_PLR1_UPPER_AND_LOWER_E/
                 CPSS_DXCH_POLICER_MEMORY_CTRL_MODE_PLR1_UPPER_PLR0_LOWER_E]
            Expected: GT_OK.
        */
        mode = CPSS_DXCH_POLICER_MEMORY_CTRL_MODE_PLR0_UPPER_AND_LOWER_E;

        st = cpssDxChPolicerMemorySizeModeSet(dev, mode);
        UTF_VERIFY_EQUAL1_PARAM_MAC(GT_OK, st, dev);

        /*
            1.2. Call cpssDxChPolicerMemorySizeModeGet.
            Expected: GT_OK and the same value.
        */
        st = cpssDxChPolicerMemorySizeModeGet(dev, &modeGet);
        UTF_VERIFY_EQUAL1_STRING_MAC(GT_OK, st,
               "cpssDxChPolicerMemorySizeModeGet: %d", dev);

        /* verifying values */
        UTF_VERIFY_EQUAL1_STRING_MAC(mode, modeGet,
                   "got another mode then was set: %d", dev);

        /*
            1.1. Call function with mode
                [CPSS_DXCH_POLICER_MEMORY_CTRL_MODE_PLR0_UPPER_PLR1_LOWER_E/
                 CPSS_DXCH_POLICER_MEMORY_CTRL_MODE_PLR0_UPPER_AND_LOWER_E/
                 CPSS_DXCH_POLICER_MEMORY_CTRL_MODE_PLR1_UPPER_AND_LOWER_E/
                 CPSS_DXCH_POLICER_MEMORY_CTRL_MODE_PLR1_UPPER_PLR0_LOWER_E]
            Expected: GT_OK.
        */
        mode = CPSS_DXCH_POLICER_MEMORY_CTRL_MODE_PLR1_UPPER_AND_LOWER_E;

        st = cpssDxChPolicerMemorySizeModeSet(dev, mode);
        UTF_VERIFY_EQUAL1_PARAM_MAC(GT_OK, st, dev);

        /*
            1.2. Call cpssDxChPolicerMemorySizeModeGet.
            Expected: GT_OK and the same value.
        */
        st = cpssDxChPolicerMemorySizeModeGet(dev, &modeGet);
        UTF_VERIFY_EQUAL1_STRING_MAC(GT_OK, st,
               "cpssDxChPolicerMemorySizeModeGet: %d", dev);

        /* verifying values */
        UTF_VERIFY_EQUAL1_STRING_MAC(mode, modeGet,
                   "got another mode then was set: %d", dev);

        /*
            1.1. Call function with mode
                [CPSS_DXCH_POLICER_MEMORY_CTRL_MODE_PLR0_UPPER_PLR1_LOWER_E/
                 CPSS_DXCH_POLICER_MEMORY_CTRL_MODE_PLR0_UPPER_AND_LOWER_E/
                 CPSS_DXCH_POLICER_MEMORY_CTRL_MODE_PLR1_UPPER_AND_LOWER_E/
                 CPSS_DXCH_POLICER_MEMORY_CTRL_MODE_PLR1_UPPER_PLR0_LOWER_E]
            Expected: GT_OK.
        */
        mode = CPSS_DXCH_POLICER_MEMORY_CTRL_MODE_PLR1_UPPER_PLR0_LOWER_E;

        st = cpssDxChPolicerMemorySizeModeSet(dev, mode);
        UTF_VERIFY_EQUAL1_PARAM_MAC(GT_OK, st, dev);

        /*
            1.2. Call cpssDxChPolicerMemorySizeModeGet.
            Expected: GT_OK and the same value.
        */
        st = cpssDxChPolicerMemorySizeModeGet(dev, &modeGet);
        UTF_VERIFY_EQUAL1_STRING_MAC(GT_OK, st,
               "cpssDxChPolicerMemorySizeModeGet: %d", dev);

        /* verifying values */
        UTF_VERIFY_EQUAL1_STRING_MAC(mode, modeGet,
                   "got another mode then was set: %d", dev);
        /*
            1.3. Call api with wrong mode [wrong enum values].
            Expected: GT_BAD_PARAM.
        */
        UTF_ENUMS_CHECK_MAC(cpssDxChPolicerMemorySizeModeSet
                            (dev, mode),
                            mode);
    }

    /* restore correct values */
    mode = CPSS_DXCH_POLICER_MEMORY_CTRL_MODE_PLR0_UPPER_PLR1_LOWER_E;

    /* 2. For not-active devices and devices from non-applicable family */
    /* check that function returns GT_BAD_PARAM.                        */

    /* prepare device iterator */
    PRV_TGF_NOT_APPLIC_DEV_RESET_MAC(&dev, UTF_CH1_E | UTF_CH1_DIAMOND_E | UTF_CH2_E | UTF_CH3_E);

    /* go over all non active devices */
    while(GT_OK == prvUtfNextDeviceGet(&dev, GT_FALSE))
    {
        st = cpssDxChPolicerMemorySizeModeSet(dev, mode);
        UTF_VERIFY_EQUAL1_PARAM_MAC(GT_NOT_APPLICABLE_DEVICE, st, dev);
    }

    /* 3. Call function with out of bound value for device id.*/
    dev = PRV_CPSS_MAX_PP_DEVICES_CNS;

    st = cpssDxChPolicerMemorySizeModeSet(dev, mode);
    UTF_VERIFY_EQUAL1_PARAM_MAC(GT_BAD_PARAM, st, dev);
}

/*----------------------------------------------------------------------------*/
/*
GT_STATUS cpssDxChPolicerMeteringCalcMethodGet
(
    IN  GT_U8                                         devNum,
    OUT CPSS_DXCH_POLICER_METERING_CALC_METHOD_ENT    *meteringCalcMethodPtr,
    OUT GT_U32                                        *cirPirAllowedDeviationPtr,
    OUT GT_BOOL                                       *cbsPbsCalcOnFailPtr
)
*/
UTF_TEST_CASE_MAC(cpssDxChPolicerMeteringCalcMethodGet)
{
/*
    ITERATE_DEVICES(DxCh3 and above)
    1.1. Call function with not null pointers.
    Expected: GT_OK.
    1.2. Call api with wrong meteringCalcMethodPtr [NULL].
    Expected: GT_BAD_PTR.
    1.3. Call api with wrong cirPirAllowedDeviationPtr [NULL].
    Expected: GT_BAD_PTR.
    1.4. Call api with wrong cbsPbsCalcOnFailPtr [NULL].
    Expected: GT_BAD_PTR.
*/
    GT_STATUS st = GT_OK;

    GT_U8                                         dev;
    CPSS_DXCH_POLICER_METERING_CALC_METHOD_ENT    meteringCalcMethod;
    GT_U32                                        cirPirAllowedDeviation;
    GT_BOOL                                       cbsPbsCalcOnFail;

    /* prepare device iterator */
    PRV_TGF_NOT_APPLIC_DEV_RESET_MAC(&dev, UTF_CH1_E | UTF_CH1_DIAMOND_E | UTF_CH2_E);

    /* 1. Go over all active devices. */
    while (GT_OK == prvUtfNextDeviceGet(&dev, GT_TRUE))
    {
        /*
            1.1. Call function with not null pointers.
            Expected: GT_OK.
        */

        st = cpssDxChPolicerMeteringCalcMethodGet(dev, &meteringCalcMethod,
                               &cirPirAllowedDeviation, &cbsPbsCalcOnFail);
        UTF_VERIFY_EQUAL1_PARAM_MAC(GT_OK, st, dev);

        /*
            1.2. Call api with wrong meteringCalcMethodPtr [NULL].
            Expected: GT_BAD_PTR.
        */
        st = cpssDxChPolicerMeteringCalcMethodGet(dev, NULL,
                               &cirPirAllowedDeviation, &cbsPbsCalcOnFail);
        UTF_VERIFY_EQUAL1_PARAM_MAC(GT_BAD_PTR, st, dev);


        /* In order to check NULL pointer for cirPirAllowedDeviationPtr and
           cbsPbsCalcOnFailPtr, set meteringCalcMethod to CIR_AND_CBS */

        meteringCalcMethod = CPSS_DXCH_POLICER_METERING_CALC_METHOD_CIR_AND_CBS_E;
        cirPirAllowedDeviation = 10;
        cbsPbsCalcOnFail = GT_TRUE;
        st = cpssDxChPolicerMeteringCalcMethodSet(dev, meteringCalcMethod,
                                                  cirPirAllowedDeviation, cbsPbsCalcOnFail);
        UTF_VERIFY_EQUAL1_PARAM_MAC(GT_OK, st, dev);

        /*
            1.3. Call api with wrong cirPirAllowedDeviationPtr [NULL].
            Expected: GT_BAD_PTR.
        */
        st = cpssDxChPolicerMeteringCalcMethodGet(dev, &meteringCalcMethod,
                               NULL, &cbsPbsCalcOnFail);
        UTF_VERIFY_EQUAL1_PARAM_MAC(GT_BAD_PTR, st, dev);

        /*
            1.4. Call api with wrong cbsPbsCalcOnFailPtr [NULL].
            Expected: GT_BAD_PTR.
        */
        st = cpssDxChPolicerMeteringCalcMethodGet(dev, &meteringCalcMethod,
                               &cirPirAllowedDeviation, NULL);
        UTF_VERIFY_EQUAL1_PARAM_MAC(GT_BAD_PTR, st, dev);
    }

    /* 2. For not-active devices and devices from non-applicable family */
    /* check that function returns GT_BAD_PARAM.                        */

    /* prepare device iterator */
    PRV_TGF_NOT_APPLIC_DEV_RESET_MAC(&dev, UTF_CH1_E | UTF_CH1_DIAMOND_E | UTF_CH2_E);

    /* go over all non active devices */
    while(GT_OK == prvUtfNextDeviceGet(&dev, GT_FALSE))
    {
        st = cpssDxChPolicerMeteringCalcMethodGet(dev, &meteringCalcMethod,
                               &cirPirAllowedDeviation, &cbsPbsCalcOnFail);
        UTF_VERIFY_EQUAL1_PARAM_MAC(GT_NOT_APPLICABLE_DEVICE, st, dev);
    }

    /* 3. Call function with out of bound value for device id.*/
    dev = PRV_CPSS_MAX_PP_DEVICES_CNS;

    st = cpssDxChPolicerMeteringCalcMethodGet(dev, &meteringCalcMethod,
                               &cirPirAllowedDeviation, &cbsPbsCalcOnFail);
    UTF_VERIFY_EQUAL1_PARAM_MAC(GT_BAD_PARAM, st, dev);
}

/*----------------------------------------------------------------------------*/
/*
GT_STATUS cpssDxChPolicerMeteringOnTrappedPktsEnableGet
(
    IN  GT_U8                             devNum,
    IN  CPSS_DXCH_POLICER_STAGE_TYPE_ENT  stage,
    OUT GT_BOOL                           *enablePtr
)
*/
UTF_TEST_CASE_MAC(cpssDxChPolicerMeteringOnTrappedPktsEnableGet)
{
/*
    ITERATE_DEVICES(DxChXcat and above)
    1.1. Call function with stage [CPSS_DXCH_POLICER_STAGE_INGRESS_0_E
                                   CPSS_DXCH_POLICER_STAGE_INGRESS_1_E].
    Expected: GT_OK.
    1.2. Call api with wrong stage [CPSS_DXCH_POLICER_STAGE_EGRESS_E].
    Expected: NOT GT_OK.
    1.3. Call api with wrong stage [wrong enum values].
    Expected: GT_BAD_PARAM.
    1.4. Call api with wrong enablePtr [NULL].
    Expected: GT_BAD_PTR.
*/
    GT_STATUS st = GT_OK;

    GT_U8                             dev;
    CPSS_DXCH_POLICER_STAGE_TYPE_ENT  stage = CPSS_DXCH_POLICER_STAGE_INGRESS_0_E;
    GT_BOOL                           enable;

    /* prepare device iterator */
    PRV_TGF_NOT_APPLIC_DEV_RESET_MAC(&dev, UTF_CH1_E | UTF_CH1_DIAMOND_E | UTF_CH2_E | UTF_CH3_E);

    /* 1. Go over all active devices. */
    while (GT_OK == prvUtfNextDeviceGet(&dev, GT_TRUE))
    {
        if ((PRV_CPSS_PP_MAC(dev)->devFamily == CPSS_PP_FAMILY_DXCH_LION_E) &&
            (PRV_CPSS_PP_MAC(dev)->revision < 2))
            return;

        /*
            1.1. Call function with stage [CPSS_DXCH_POLICER_STAGE_INGRESS_0_E
                                           CPSS_DXCH_POLICER_STAGE_INGRESS_1_E].
            Expected: GT_OK.
        */
        /* call with stage stage = CPSS_DXCH_POLICER_STAGE_INGRESS_0_E; */
        stage = CPSS_DXCH_POLICER_STAGE_INGRESS_0_E;

        st = cpssDxChPolicerMeteringOnTrappedPktsEnableGet(dev, stage, &enable);
        UTF_VERIFY_EQUAL1_PARAM_MAC(GT_OK, st, dev);

        /* call with stage stage = CPSS_DXCH_POLICER_STAGE_INGRESS_1_E; */
        stage = CPSS_DXCH_POLICER_STAGE_INGRESS_1_E;

        st = cpssDxChPolicerMeteringOnTrappedPktsEnableGet(dev, stage, &enable);
        UTF_VERIFY_EQUAL1_PARAM_MAC(GT_OK, st, dev);

        /*
            1.2. Call api with wrong stage [CPSS_DXCH_POLICER_STAGE_EGRESS_E].
            Expected: NOT GT_OK.
        */
        stage = CPSS_DXCH_POLICER_STAGE_EGRESS_E;

        st = cpssDxChPolicerMeteringOnTrappedPktsEnableGet(dev, stage, &enable);
        UTF_VERIFY_NOT_EQUAL1_PARAM_MAC(GT_OK, st, dev);

        stage = CPSS_DXCH_POLICER_STAGE_INGRESS_0_E;

        /*
            1.3. Call api with wrong stage [wrong enum values].
            Expected: GT_BAD_PARAM.
        */
        UTF_ENUMS_CHECK_MAC(cpssDxChPolicerMeteringOnTrappedPktsEnableGet
                            (dev, stage, &enable),
                            stage);

        /*
            1.4. Call api with wrong enablePtr [NULL].
            Expected: GT_BAD_PTR.
        */
        st = cpssDxChPolicerMeteringOnTrappedPktsEnableGet(dev, stage, NULL);
        UTF_VERIFY_EQUAL1_PARAM_MAC(GT_BAD_PTR, st, dev);
    }

    /* restore correct values */
    stage = CPSS_DXCH_POLICER_STAGE_INGRESS_0_E;

    /* 2. For not-active devices and devices from non-applicable family */
    /* check that function returns GT_BAD_PARAM.                        */

    /* prepare device iterator */
    PRV_TGF_NOT_APPLIC_DEV_RESET_MAC(&dev, UTF_CH1_E | UTF_CH1_DIAMOND_E | UTF_CH2_E | UTF_CH3_E);

    /* go over all non active devices */
    while(GT_OK == prvUtfNextDeviceGet(&dev, GT_FALSE))
    {
        if ((PRV_CPSS_PP_MAC(dev)->devFamily == CPSS_PP_FAMILY_DXCH_LION_E) &&
            (PRV_CPSS_PP_MAC(dev)->revision < 2))
            return;

        st = cpssDxChPolicerMeteringOnTrappedPktsEnableGet(dev, stage, &enable);
        UTF_VERIFY_EQUAL1_PARAM_MAC(GT_NOT_APPLICABLE_DEVICE, st, dev);
    }

    /* 3. Call function with out of bound value for device id.*/
    dev = PRV_CPSS_MAX_PP_DEVICES_CNS;

    st = cpssDxChPolicerMeteringOnTrappedPktsEnableGet(dev, stage, &enable);
    UTF_VERIFY_EQUAL1_PARAM_MAC(GT_BAD_PARAM, st, dev);
}

/*----------------------------------------------------------------------------*/
/*
GT_STATUS cpssDxChPolicerMeteringOnTrappedPktsEnableSet
(
    IN  GT_U8                            devNum,
    IN  CPSS_DXCH_POLICER_STAGE_TYPE_ENT stage,
    IN  GT_BOOL                          enable
)
*/
UTF_TEST_CASE_MAC(cpssDxChPolicerMeteringOnTrappedPktsEnableSet)
{
/*
    ITERATE_DEVICES(DxChXcat and above)
    1.1. Call function with stage [CPSS_DXCH_POLICER_STAGE_INGRESS_0_E
                                   CPSS_DXCH_POLICER_STAGE_INGRESS_1_E],
                       and enable [GT_TRUE / GT_FALSE].
    Expected: GT_OK.
    1.2. Call cpssDxChPolicerMeteringOnTrappedPktsEnableGet.
    Expected: GT_OK and the same value.
    1.3. Call api with wrong stage [CPSS_DXCH_POLICER_STAGE_EGRESS_E].
    Expected: NOT GT_OK.
    1.4. Call api with wrong stage [wrong enum values].
    Expected: GT_BAD_PARAM.
*/
    GT_STATUS st = GT_OK;

    GT_U8                            dev;
    CPSS_DXCH_POLICER_STAGE_TYPE_ENT stage = CPSS_DXCH_POLICER_STAGE_INGRESS_0_E;
    GT_BOOL                          enable = GT_FALSE;
    GT_BOOL                          enableGet;

    /* prepare device iterator */
    PRV_TGF_NOT_APPLIC_DEV_RESET_MAC(&dev, UTF_CH1_E | UTF_CH1_DIAMOND_E | UTF_CH2_E | UTF_CH3_E);

    /* 1. Go over all active devices. */
    while (GT_OK == prvUtfNextDeviceGet(&dev, GT_TRUE))
    {
        /*
            1.1. Call function with stage [CPSS_DXCH_POLICER_STAGE_INGRESS_0_E
                                           CPSS_DXCH_POLICER_STAGE_INGRESS_1_E],
                               and enable [GT_TRUE / GT_FALSE].
            Expected: GT_OK.
        */
        stage = CPSS_DXCH_POLICER_STAGE_INGRESS_0_E;
        enable = GT_TRUE;

        st = cpssDxChPolicerMeteringOnTrappedPktsEnableSet(dev, stage, enable);
        UTF_VERIFY_EQUAL1_PARAM_MAC(GT_OK, st, dev);

        /*
            1.2. Call cpssDxChPolicerMeteringOnTrappedPktsEnableGet.
            Expected: GT_OK and the same value.
        */
        st = cpssDxChPolicerMeteringOnTrappedPktsEnableGet(dev, stage, &enableGet);
        UTF_VERIFY_EQUAL1_STRING_MAC(GT_OK, st,
               "cpssDxChPolicerMeteringOnTrappedPktsEnableGet: %d", dev);

        /* verifying values */
        UTF_VERIFY_EQUAL1_STRING_MAC(enable, enableGet,
                   "got another enable then was set: %d", dev);

        /*
            1.1. Call function with stage [CPSS_DXCH_POLICER_STAGE_INGRESS_0_E
                                           CPSS_DXCH_POLICER_STAGE_INGRESS_1_E],
                               and enable [GT_TRUE / GT_FALSE].
            Expected: GT_OK.
        */
        stage = CPSS_DXCH_POLICER_STAGE_INGRESS_1_E;
        enable = GT_FALSE;

        st = cpssDxChPolicerMeteringOnTrappedPktsEnableSet(dev, stage, enable);
        UTF_VERIFY_EQUAL1_PARAM_MAC(GT_OK, st, dev);

        /*
            1.2. Call cpssDxChPolicerMeteringOnTrappedPktsEnableGet.
            Expected: GT_OK and the same value.
        */
        st = cpssDxChPolicerMeteringOnTrappedPktsEnableGet(dev, stage, &enableGet);
        UTF_VERIFY_EQUAL1_STRING_MAC(GT_OK, st,
               "cpssDxChPolicerMeteringOnTrappedPktsEnableGet: %d", dev);

        /* verifying values */
        UTF_VERIFY_EQUAL1_STRING_MAC(enable, enableGet,
                   "got another enable then was set: %d", dev);

        /*
            1.3. Call api with wrong stage [CPSS_DXCH_POLICER_STAGE_EGRESS_E].
            Expected: NOT GT_OK.
        */
        stage = CPSS_DXCH_POLICER_STAGE_EGRESS_E;

        st = cpssDxChPolicerMeteringOnTrappedPktsEnableSet(dev, stage, enable);
        UTF_VERIFY_NOT_EQUAL1_PARAM_MAC(GT_OK, st, dev);

        stage = CPSS_DXCH_POLICER_STAGE_INGRESS_0_E;

        /*
            1.4. Call api with wrong stage [wrong enum values].
            Expected: GT_BAD_PARAM.
        */
        UTF_ENUMS_CHECK_MAC(cpssDxChPolicerMeteringOnTrappedPktsEnableSet
                            (dev, stage, enable),
                            stage);
    }

    /* restore correct values */
    stage = CPSS_DXCH_POLICER_STAGE_INGRESS_0_E;
    enable = GT_TRUE;

    /* 2. For not-active devices and devices from non-applicable family */
    /* check that function returns GT_BAD_PARAM.                        */

    /* prepare device iterator */
    PRV_TGF_NOT_APPLIC_DEV_RESET_MAC(&dev, UTF_CH1_E | UTF_CH1_DIAMOND_E | UTF_CH2_E | UTF_CH3_E);

    /* go over all non active devices */
    while(GT_OK == prvUtfNextDeviceGet(&dev, GT_FALSE))
    {
        st = cpssDxChPolicerMeteringOnTrappedPktsEnableSet(dev, stage, enable);
        UTF_VERIFY_EQUAL1_PARAM_MAC(GT_NOT_APPLICABLE_DEVICE, st, dev);
    }

    /* 3. Call function with out of bound value for device id.*/
    dev = PRV_CPSS_MAX_PP_DEVICES_CNS;

    st = cpssDxChPolicerMeteringOnTrappedPktsEnableSet(dev, stage, enable);
    UTF_VERIFY_EQUAL1_PARAM_MAC(GT_BAD_PARAM, st, dev);
}

/*----------------------------------------------------------------------------*/
/*
GT_STATUS cpssDxChPolicerPolicyCntrGet
(
    IN  GT_U8                             devNum,
    IN  CPSS_DXCH_POLICER_STAGE_TYPE_ENT  stage,
    IN  GT_U32                            index,
    OUT GT_U32                            *cntrValuePtr
)
*/
UTF_TEST_CASE_MAC(cpssDxChPolicerPolicyCntrGet)
{
/*
ITERATE_DEVICES(DxChXcat and above)
    1.1. Call function with stage [CPSS_DXCH_POLICER_STAGE_INGRESS_0_E
                                   CPSS_DXCH_POLICER_STAGE_INGRESS_1_E /
                                   CPSS_DXCH_POLICER_STAGE_EGRESS_E],
                and index [5 / 18 / 21].
    Expected: GT_OK.
    1.2. Call api with wrong stage [wrong enum values].
    Expected: GT_BAD_PARAM.
    1.3. Call api with wrong index [6].
    Expected: NOT GT_OK.
    1.4. Call api with wrong cntrValuePtr [NULL].
    Expected: GT_BAD_PTR.
*/
    GT_STATUS st = GT_OK;

    GT_U8                             dev;
    CPSS_DXCH_POLICER_STAGE_TYPE_ENT  stage = CPSS_DXCH_POLICER_STAGE_INGRESS_0_E;
    GT_U32                            index = 0;
    GT_U32                            cntrValue;

    /* prepare device iterator */
    PRV_TGF_NOT_APPLIC_DEV_RESET_MAC(&dev, UTF_CH1_E | UTF_CH1_DIAMOND_E | UTF_CH2_E | UTF_CH3_E);

    /* 1. Go over all active devices. */
    while (GT_OK == prvUtfNextDeviceGet(&dev, GT_TRUE))
    {
        /*
            1.1. Call function with stage [CPSS_DXCH_POLICER_STAGE_INGRESS_0_E
                                           CPSS_DXCH_POLICER_STAGE_INGRESS_1_E /
                                           CPSS_DXCH_POLICER_STAGE_EGRESS_E],
                        and index [5 / 18 / 21].
            Expected: GT_OK.
        */
        stage = CPSS_DXCH_POLICER_STAGE_INGRESS_0_E;

        if (PRV_CPSS_PP_MAC(dev)->devFamily == CPSS_PP_FAMILY_DXCH_XCAT2_E)
        {
            /* indexes supported are 0,1,2 and 3  (index & 0x7) */
            index = 3;
        }
        else
        {
            index = 5;
        }

        st = cpssDxChPolicerPolicyCntrGet(dev, stage, index, &cntrValue);
        UTF_VERIFY_EQUAL1_PARAM_MAC(GT_OK, st, dev);

        /*
            1.1. Call function with stage [CPSS_DXCH_POLICER_STAGE_INGRESS_0_E
                                           CPSS_DXCH_POLICER_STAGE_INGRESS_1_E /
                                           CPSS_DXCH_POLICER_STAGE_EGRESS_E],
                        and index [5 / 18 / 21].
            Expected: GT_OK.
        */
        stage = CPSS_DXCH_POLICER_STAGE_INGRESS_1_E;
        index = 18;

        st = cpssDxChPolicerPolicyCntrGet(dev, stage, index, &cntrValue);
        UTF_VERIFY_EQUAL1_PARAM_MAC(GT_OK, st, dev);

        /*
            1.1. Call function with stage [CPSS_DXCH_POLICER_STAGE_INGRESS_0_E
                                           CPSS_DXCH_POLICER_STAGE_INGRESS_1_E /
                                           CPSS_DXCH_POLICER_STAGE_EGRESS_E],
                        and index [5 / 18 / 21].
            Expected: GT_OK.
        */
        stage = CPSS_DXCH_POLICER_STAGE_EGRESS_E;

        if (PRV_CPSS_PP_MAC(dev)->devFamily == CPSS_PP_FAMILY_DXCH_XCAT2_E)
        {
            /* indexes supported are 0,1,2 and 3  (index & 0x7) */
            index = 19;
        }
        else
        {
            index = 21;
        }

        st = cpssDxChPolicerPolicyCntrGet(dev, stage, index, &cntrValue);
        UTF_VERIFY_EQUAL1_PARAM_MAC(GT_OK, st, dev);

        /*
            1.2. Call api with wrong stage [wrong enum values].
            Expected: GT_BAD_PARAM.
        */
        UTF_ENUMS_CHECK_MAC(cpssDxChPolicerPolicyCntrGet
                            (dev, stage, index, &cntrValue),
                            stage);

        /*
            1.3. Call api with wrong index [max + 1] (out of range)
            Expected: NOT GT_OK.
        */
        index = (PRV_CPSS_DXCH_PP_MAC(dev)->policer.memSize[(stage)] << 3) + 1;

        st = cpssDxChPolicerPolicyCntrGet(dev, stage, index, &cntrValue);
        UTF_VERIFY_NOT_EQUAL1_PARAM_MAC(GT_OK, st, dev);

        /*
            1.4. Call api with wrong cntrValuePtr [NULL].
            Expected: GT_BAD_PTR.
        */
        st = cpssDxChPolicerPolicyCntrGet(dev, stage, index, NULL);
        UTF_VERIFY_EQUAL1_PARAM_MAC(GT_BAD_PTR, st, dev);
    }

    /* restore correct values */
    stage = CPSS_DXCH_POLICER_STAGE_INGRESS_0_E;
    index = 0;

    /* 2. For not-active devices and devices from non-applicable family */
    /* check that function returns GT_BAD_PARAM.                        */

    /* prepare device iterator */
    PRV_TGF_NOT_APPLIC_DEV_RESET_MAC(&dev, UTF_CH1_E | UTF_CH1_DIAMOND_E | UTF_CH2_E | UTF_CH3_E);

    /* go over all non active devices */
    while(GT_OK == prvUtfNextDeviceGet(&dev, GT_FALSE))
    {
        st = cpssDxChPolicerPolicyCntrGet(dev, stage, index, &cntrValue);
        UTF_VERIFY_EQUAL1_PARAM_MAC(GT_NOT_APPLICABLE_DEVICE, st, dev);
    }

    /* 3. Call function with out of bound value for device id.*/
    dev = PRV_CPSS_MAX_PP_DEVICES_CNS;

    st = cpssDxChPolicerPolicyCntrGet(dev, stage, index, &cntrValue);
    UTF_VERIFY_EQUAL1_PARAM_MAC(GT_BAD_PARAM, st, dev);
}

/*----------------------------------------------------------------------------*/
/*
GT_STATUS cpssDxChPolicerPolicyCntrSet
(
    IN  GT_U8                            devNum,
    IN  CPSS_DXCH_POLICER_STAGE_TYPE_ENT stage,
    IN  GT_U32                           index,
    IN  GT_U32                           cntrValue
)
*/
UTF_TEST_CASE_MAC(cpssDxChPolicerPolicyCntrSet)
{
/*
    ITERATE_DEVICES(DxChXcat and above)
    1.1. Call function with stage [CPSS_DXCH_POLICER_STAGE_INGRESS_0_E
                                   CPSS_DXCH_POLICER_STAGE_INGRESS_1_E /
                                   CPSS_DXCH_POLICER_STAGE_EGRESS_E],
            and index [5 / 18 / 21], cntrValue [0 / 1000 / 3333].
    Expected: GT_OK.
    1.2. Call cpssDxChPolicerPolicyCntrGet with the same params.
    Expected: GT_OK and the same value.
    1.3. Call api with wrong stage [wrong enum values].
    Expected: GT_BAD_PARAM.
    1.4. Call api with wrong index [0].
    Expected: NOT GT_OK.
*/
    GT_STATUS st = GT_OK;

    GT_U8                            dev;
    CPSS_DXCH_POLICER_STAGE_TYPE_ENT stage = CPSS_DXCH_POLICER_STAGE_INGRESS_0_E;
    GT_U32                           index = 0;
    GT_U32                           cntrValue = 0;
    GT_U32                           cntrValueGet;

    /* prepare device iterator */
    PRV_TGF_NOT_APPLIC_DEV_RESET_MAC(&dev, UTF_CH1_E | UTF_CH1_DIAMOND_E | UTF_CH2_E | UTF_CH3_E);

    /* 1. Go over all active devices. */
    while (GT_OK == prvUtfNextDeviceGet(&dev, GT_TRUE))
    {
        /*
            1.1. Call function with stage [CPSS_DXCH_POLICER_STAGE_INGRESS_0_E
                                           CPSS_DXCH_POLICER_STAGE_INGRESS_1_E /
                                           CPSS_DXCH_POLICER_STAGE_EGRESS_E],
                    and index [5 / 18 / 21], cntrValue [0 / 1000 / 3333].
            Expected: GT_OK.
        */
        stage = CPSS_DXCH_POLICER_STAGE_INGRESS_0_E;
        cntrValue = 0;

        if (PRV_CPSS_PP_MAC(dev)->devFamily == CPSS_PP_FAMILY_DXCH_XCAT2_E)
        {
            /* indexes supported are 0,1,2 and 3  (index & 0x7) */
            index = 3;
        }
        else
        {
            index = 5;
        }


        st = cpssDxChPolicerPolicyCntrSet(dev, stage, index, cntrValue);
        UTF_VERIFY_EQUAL1_PARAM_MAC(GT_OK, st, dev);

        /*
            1.2. Call cpssDxChPolicerPolicyCntrGet with the same params.
            Expected: GT_OK and the same value.
        */
        st = cpssDxChPolicerPolicyCntrGet(dev, stage, index, &cntrValueGet);
        UTF_VERIFY_EQUAL1_STRING_MAC(GT_OK, st,
               "cpssDxChPolicerPolicyCntrGet: %d", dev);

        /* verifying values */
        UTF_VERIFY_EQUAL1_STRING_MAC(cntrValue, cntrValueGet,
                   "got another cntrValue then was set: %d", dev);

        /*
            1.1. Call function with stage [CPSS_DXCH_POLICER_STAGE_INGRESS_0_E
                                           CPSS_DXCH_POLICER_STAGE_INGRESS_1_E /
                                           CPSS_DXCH_POLICER_STAGE_EGRESS_E],
                    and index [5 / 18 / 21], cntrValue [0 / 1000 / 3333].
            Expected: GT_OK.
        */
        stage = CPSS_DXCH_POLICER_STAGE_INGRESS_1_E;
        index = 18;
        cntrValue = 1000;

        st = cpssDxChPolicerPolicyCntrSet(dev, stage, index, cntrValue);
        UTF_VERIFY_EQUAL1_PARAM_MAC(GT_OK, st, dev);

        /*
            1.2. Call cpssDxChPolicerPolicyCntrGet with the same params.
            Expected: GT_OK and the same value.
        */
        st = cpssDxChPolicerPolicyCntrGet(dev, stage, index, &cntrValueGet);
        UTF_VERIFY_EQUAL1_STRING_MAC(GT_OK, st,
               "cpssDxChPolicerPolicyCntrGet: %d", dev);

        /* verifying values */
        UTF_VERIFY_EQUAL1_STRING_MAC(cntrValue, cntrValueGet,
                   "got another cntrValue then was set: %d", dev);

        /*
            1.1. Call function with stage [CPSS_DXCH_POLICER_STAGE_INGRESS_0_E
                                           CPSS_DXCH_POLICER_STAGE_INGRESS_1_E /
                                           CPSS_DXCH_POLICER_STAGE_EGRESS_E],
                    and index [5 / 18 / 21], cntrValue [0 / 1000 / 3333].
            Expected: GT_OK.
        */
        stage = CPSS_DXCH_POLICER_STAGE_EGRESS_E;
        cntrValue = 3333;

        if (PRV_CPSS_PP_MAC(dev)->devFamily == CPSS_PP_FAMILY_DXCH_XCAT2_E)
        {
            /* indexes supported are 0,1,2 and 3  (index & 0x7) */
            index = 19;
        }
        else
        {
            index = 21;
        }


        st = cpssDxChPolicerPolicyCntrSet(dev, stage, index, cntrValue);
        UTF_VERIFY_EQUAL1_PARAM_MAC(GT_OK, st, dev);

        /*
            1.2. Call cpssDxChPolicerPolicyCntrGet with the same params.
            Expected: GT_OK and the same value.
        */
        st = cpssDxChPolicerPolicyCntrGet(dev, stage, index, &cntrValueGet);
        UTF_VERIFY_EQUAL1_STRING_MAC(GT_OK, st,
               "cpssDxChPolicerPolicyCntrGet: %d", dev);

        /* verifying values */
        UTF_VERIFY_EQUAL1_STRING_MAC(cntrValue, cntrValueGet,
                   "got another cntrValue then was set: %d", dev);

        /*
            1.3. Call api with wrong stage [wrong enum values].
            Expected: GT_BAD_PARAM.
        */
        UTF_ENUMS_CHECK_MAC(cpssDxChPolicerPolicyCntrSet
                            (dev, stage, index, cntrValue),
                            stage);

        /*
            1.4. Call api with wrong index [max + 1]. (out of range)
            Expected: NOT GT_OK.
        */
        index = (PRV_CPSS_DXCH_PP_MAC(dev)->policer.memSize[(stage)] << 3) + 1;

        st = cpssDxChPolicerPolicyCntrSet(dev, stage, index, cntrValue);
        UTF_VERIFY_NOT_EQUAL1_PARAM_MAC(GT_OK, st, dev);

        index = 0;
    }

    /* restore correct values */
    stage = CPSS_DXCH_POLICER_STAGE_INGRESS_0_E;
    index = 0;
    cntrValue = 0;

    /* 2. For not-active devices and devices from non-applicable family */
    /* check that function returns GT_BAD_PARAM.                        */

    /* prepare device iterator */
    PRV_TGF_NOT_APPLIC_DEV_RESET_MAC(&dev, UTF_CH1_E | UTF_CH1_DIAMOND_E | UTF_CH2_E | UTF_CH3_E);

    /* go over all non active devices */
    while(GT_OK == prvUtfNextDeviceGet(&dev, GT_FALSE))
    {
        st = cpssDxChPolicerPolicyCntrSet(dev, stage, index, cntrValue);
        UTF_VERIFY_EQUAL1_PARAM_MAC(GT_NOT_APPLICABLE_DEVICE, st, dev);
    }

    /* 3. Call function with out of bound value for device id.*/
    dev = PRV_CPSS_MAX_PP_DEVICES_CNS;

    st = cpssDxChPolicerPolicyCntrSet(dev, stage, index, cntrValue);
    UTF_VERIFY_EQUAL1_PARAM_MAC(GT_BAD_PARAM, st, dev);
}


/*----------------------------------------------------------------------------*/
/*
GT_STATUS cpssDxChPolicerPortGroupBillingEntryGet
(
    IN  GT_U8                                 devNum,
    IN  GT_PORT_GROUPS_BMP                    portGroupsBmp,
    IN  CPSS_DXCH_POLICER_STAGE_TYPE_ENT      stage,
    IN  GT_U32                                entryIndex,
    IN  GT_BOOL                               reset,
    OUT CPSS_DXCH3_POLICER_BILLING_ENTRY_STC  *billingCntrPtr
)
*/
UTF_TEST_CASE_MAC(cpssDxChPolicerPortGroupBillingEntryGet)
{
/*
    ITERATE_DEVICES_PORT_GROUPS(DxCh3 and above)
    1.1. Call function with stage [CPSS_DXCH_POLICER_STAGE_INGRESS_0_E
                                   CPSS_DXCH_POLICER_STAGE_INGRESS_1_E /
                                   CPSS_DXCH_POLICER_STAGE_EGRESS_E].
            and entryIndex[0 / 10 / 100], reset [GT_FALSE / GT_TRUE],
    Expected: GT_OK.
    1.2. Call api with wrong stage [wrong enum values].
    Expected: GT_BAD_PARAM.
    1.3. Call api with wrong entryIndex [max + 1].
    Expected: NOT GT_OK.
    1.4. Call api with wrong billingCntrPtr [NULL].
    Expected: GT_BAD_PTR.
*/
    GT_STATUS st = GT_OK;
    GT_U32    portGroupId;

    GT_U8                                 dev;
    GT_PORT_GROUPS_BMP                    portGroupsBmp = 1;
    CPSS_DXCH_POLICER_STAGE_TYPE_ENT      stage = CPSS_DXCH_POLICER_STAGE_INGRESS_0_E;
    GT_U32                                entryIndex = 0;
    GT_BOOL                               reset = GT_FALSE;
    CPSS_DXCH3_POLICER_BILLING_ENTRY_STC  billingCntr;
    CPSS_DXCH3_POLICER_BILLING_ENTRY_STC  billingCntrConfig;

    /* prepare device iterator */
    PRV_TGF_NOT_APPLIC_DEV_RESET_MAC(&dev, UTF_CH1_E | UTF_CH1_DIAMOND_E | UTF_CH2_E);

    /* 1. Go over all active devices. */
    while (GT_OK == prvUtfNextDeviceGet(&dev, GT_TRUE))
    {
        /* initialize port group */
        portGroupsBmp = 1;

        /* 1.1. Go over all active port groups. */
        PRV_CPSS_GEN_PP_START_LOOP_PORT_GROUPS_MAC(dev, portGroupId)
        {
            /*
                1.1. Call function with stage [CPSS_DXCH_POLICER_STAGE_INGRESS_0_E
                                               CPSS_DXCH_POLICER_STAGE_INGRESS_1_E /
                                               CPSS_DXCH_POLICER_STAGE_EGRESS_E].
                        and entryIndex[0 / 10 / 100], reset [GT_FALSE / GT_TRUE],
                Expected: GT_OK.
            */
            stage = CPSS_DXCH_POLICER_STAGE_INGRESS_0_E;
            entryIndex = 0;
            reset = GT_TRUE;
            billingCntrConfig.billingCntrMode = CPSS_DXCH3_POLICER_BILLING_CNTR_1_BYTE_E;
            billingCntrConfig.greenCntr.l[0] = 0;
            billingCntrConfig.greenCntr.l[1] = 0;
            billingCntrConfig.redCntr.l[0] = 0;
            billingCntrConfig.redCntr.l[1] = 0;
            billingCntrConfig.yellowCntr.l[0] = 0;
            billingCntrConfig.yellowCntr.l[1] = 0;

            /* The entry should be configured before read */
            st = cpssDxChPolicerPortGroupBillingEntrySet(dev, portGroupsBmp,
                                              stage, entryIndex, &billingCntrConfig);
            UTF_VERIFY_EQUAL1_PARAM_MAC(GT_OK, st, dev);

            st = cpssDxChPolicerPortGroupBillingEntryGet(dev, portGroupsBmp,
                                  stage, entryIndex, reset, &billingCntr);
            UTF_VERIFY_EQUAL1_PARAM_MAC(GT_OK, st, dev);

            /*
                1.1. Call function with stage [CPSS_DXCH_POLICER_STAGE_INGRESS_0_E
                                               CPSS_DXCH_POLICER_STAGE_INGRESS_1_E /
                                               CPSS_DXCH_POLICER_STAGE_EGRESS_E].
                        and entryIndex[0 / 10 / 100], reset [GT_FALSE / GT_TRUE],
                Expected: GT_OK.
            */
            stage = CPSS_DXCH_POLICER_STAGE_INGRESS_1_E;
            entryIndex = 10;
            reset = GT_TRUE;

            st = cpssDxChPolicerPortGroupBillingEntrySet(dev, portGroupsBmp,
                                        stage, entryIndex, &billingCntrConfig);

            st = cpssDxChPolicerPortGroupBillingEntryGet(dev, portGroupsBmp,
                                  stage, entryIndex, reset, &billingCntr);

            if (PRV_CPSS_DXCH_XCAT_FAMILY_CHECK_MAC(dev) && !IS_STAGE_EXIST_MAC(dev, stage))
            {
                UTF_VERIFY_EQUAL1_PARAM_MAC(GT_BAD_PARAM, st, dev);
            }
            else
                UTF_VERIFY_EQUAL1_PARAM_MAC(GT_OK, st, dev);

            /*
                1.1. Call function with stage [CPSS_DXCH_POLICER_STAGE_INGRESS_0_E
                                               CPSS_DXCH_POLICER_STAGE_INGRESS_1_E /
                                               CPSS_DXCH_POLICER_STAGE_EGRESS_E].
                        and entryIndex[0 / 10 / 100], reset [GT_FALSE / GT_TRUE],
                Expected: GT_OK.
            */
            stage = CPSS_DXCH_POLICER_STAGE_EGRESS_E;
            entryIndex = 100;
            reset = GT_FALSE;

            st = cpssDxChPolicerPortGroupBillingEntrySet(dev, portGroupsBmp,
                                  stage, entryIndex, &billingCntrConfig);

            st = cpssDxChPolicerPortGroupBillingEntryGet(dev, portGroupsBmp,
                                  stage, entryIndex, reset, &billingCntr);
            if (PRV_CPSS_DXCH_XCAT_FAMILY_CHECK_MAC(dev) && !IS_STAGE_EXIST_MAC(dev, stage))
            {
                UTF_VERIFY_EQUAL1_PARAM_MAC(GT_BAD_PARAM, st, dev);
            }
            else
                UTF_VERIFY_EQUAL1_PARAM_MAC(GT_OK, st, dev);


            /*
                1.2. Call api with wrong stage [wrong enum values].
                Expected: GT_BAD_PARAM.
            */
            if (PRV_CPSS_DXCH_XCAT_FAMILY_CHECK_MAC(dev))
            {
                UTF_ENUMS_CHECK_MAC(cpssDxChPolicerPortGroupBillingEntryGet
                                (dev, portGroupsBmp, stage,
                                entryIndex, reset, &billingCntr),
                                stage);
            }
            /*
                1.3. Call api with wrong entryIndex [max + 1].
                Expected: NOT GT_OK.
            */
            stage = CPSS_DXCH_POLICER_STAGE_INGRESS_0_E;
            entryIndex = PRV_CPSS_DXCH_PP_MAC(dev)->policer.memSize[(stage)] + 1;

            st = cpssDxChPolicerPortGroupBillingEntryGet(dev, portGroupsBmp,
                                  stage, entryIndex, reset, &billingCntr);
            UTF_VERIFY_NOT_EQUAL1_PARAM_MAC(GT_OK, st, dev);

            entryIndex = 0;

            /*
                1.4. Call api with wrong billingCntrPtr [NULL].
                Expected: GT_BAD_PTR.
            */
            st = cpssDxChPolicerPortGroupBillingEntryGet(dev, portGroupsBmp,
                                  stage, entryIndex, reset, NULL);
            UTF_VERIFY_EQUAL1_PARAM_MAC(GT_BAD_PTR, st, dev);

            /* set next active port */
            portGroupsBmp = (1 << portGroupId);
        }
        PRV_CPSS_GEN_PP_END_LOOP_PORT_GROUPS_MAC(dev,portGroupId)

        /* restore port group */
        portGroupsBmp = 1;

        /* 1.2. For not-active port groups check that function returns GT_BAD_PARAM. */
        PRV_CPSS_GEN_PP_START_LOOP_NON_ACTIVE_PORT_GROUPS_MAC(dev,portGroupId)
        {
            cpssDxChPolicerPortGroupBillingEntrySet(dev, portGroupsBmp,
                                  stage, entryIndex, &billingCntrConfig);
            st = cpssDxChPolicerPortGroupBillingEntryGet(dev, portGroupsBmp,
                                  stage, entryIndex, reset, &billingCntr);
            if(PRV_CPSS_IS_MULTI_PORT_GROUPS_DEVICE_MAC(dev))
            {
                UTF_VERIFY_EQUAL2_PARAM_MAC(GT_BAD_PARAM, st, dev, portGroupsBmp);
            }
            else
            {
                UTF_VERIFY_EQUAL2_PARAM_MAC(GT_OK, st, dev, portGroupsBmp);
            }

            /* set next non-active port */
            portGroupsBmp = (1 << portGroupId);
        }
        PRV_CPSS_GEN_PP_END_LOOP_NON_ACTIVE_PORT_GROUPS_MAC(dev,portGroupId)
    }

    /* restore correct values */
    portGroupId = 1;
    stage = CPSS_DXCH_POLICER_STAGE_INGRESS_0_E;
    entryIndex = 0;
    reset = GT_TRUE;

    /* 2. For not-active devices and devices from non-applicable family */
    /* check that function returns GT_BAD_PARAM.                        */

    /* prepare device iterator */
    PRV_TGF_NOT_APPLIC_DEV_RESET_MAC(&dev, UTF_CH1_E | UTF_CH1_DIAMOND_E | UTF_CH2_E);

    /* go over all non active devices */
    while(GT_OK == prvUtfNextDeviceGet(&dev, GT_FALSE))
    {
        st = cpssDxChPolicerPortGroupBillingEntryGet(dev, portGroupsBmp,
                                  stage, entryIndex, reset, &billingCntr);
        UTF_VERIFY_EQUAL1_PARAM_MAC(GT_NOT_APPLICABLE_DEVICE, st, dev);
    }

    /* 3. Call function with out of bound value for device id.*/
    dev = PRV_CPSS_MAX_PP_DEVICES_CNS;

    st = cpssDxChPolicerPortGroupBillingEntryGet(dev, portGroupsBmp,
                                  stage, entryIndex, reset, &billingCntr);
    UTF_VERIFY_EQUAL1_PARAM_MAC(GT_BAD_PARAM, st, dev);
}

/*----------------------------------------------------------------------------*/
/*
GT_STATUS cpssDxChPolicerPortGroupBillingEntrySet
(
    IN  GT_U8                                 devNum,
    IN  GT_PORT_GROUPS_BMP                    portGroupsBmp,
    IN  CPSS_DXCH_POLICER_STAGE_TYPE_ENT      stage,
    IN  GT_U32                                entryIndex,
    IN  CPSS_DXCH3_POLICER_BILLING_ENTRY_STC  *billingCntrPtr
)
*/
UTF_TEST_CASE_MAC(cpssDxChPolicerPortGroupBillingEntrySet)
{
/*
    ITERATE_DEVICES_PORT_GROUPS(DxCh3 and above)
    1.1. Call function with stage [CPSS_DXCH_POLICER_STAGE_INGRESS_0_E
                                   CPSS_DXCH_POLICER_STAGE_INGRESS_1_E /
                                   CPSS_DXCH_POLICER_STAGE_EGRESS_E].
    billingCntr.greenCntr.l[0] [0 / 10 / 100];
    billingCntr.greenCntr.l[1] [0 / 10 / 100];
    billingCntr.yellowCntr.l[0][0 / 10 / 100];
    billingCntr.yellowCntr.l[1][0 / 10 / 100];
    billingCntr.redCntr.l[0]   [0 / 10 / 100];
    billingCntr.redCntr.l[1]   [0 / 10 / 100];
    billingCntr.billingCntrMode [CPSS_DXCH3_POLICER_BILLING_CNTR_1_BYTE_E;
                                 CPSS_DXCH3_POLICER_BILLING_CNTR_16_BYTES_E,
                                 CPSS_DXCH3_POLICER_BILLING_CNTR_PACKET_E].
    Expected: GT_OK.
    1.2. Call cpssDxChPolicerPortGroupBillingEntryGet with the same params.
    Expected: GT_OK and the same values.
    1.3. Call api with wrong stage [wrong enum values].
    Expected: GT_BAD_PARAM.
    1.4. Call api with wrong entryIndex [max + 1].
    Expected: NOT GT_OK.
    1.5. Call api with wrong billingCntrPtr [NULL].
    Expected: GT_BAD_PTR.
*/
    GT_STATUS st = GT_OK;
    GT_U32    portGroupId;
    GT_BOOL   isEqual = GT_FALSE;

    GT_U8                                 dev;
    GT_PORT_GROUPS_BMP                    portGroupsBmp = 1;
    CPSS_DXCH_POLICER_STAGE_TYPE_ENT      stage = CPSS_DXCH_POLICER_STAGE_INGRESS_0_E;
    GT_U32                                entryIndex = 0;
    CPSS_DXCH3_POLICER_BILLING_ENTRY_STC  billingCntr;
    CPSS_DXCH3_POLICER_BILLING_ENTRY_STC  billingCntrGet;

    GT_BOOL                               reset = GT_FALSE;

    /* prepare device iterator */
    PRV_TGF_NOT_APPLIC_DEV_RESET_MAC(&dev, UTF_CH1_E | UTF_CH1_DIAMOND_E | UTF_CH2_E);

    /* 1. Go over all active devices. */
    while (GT_OK == prvUtfNextDeviceGet(&dev, GT_TRUE))
    {
        /* initialize port group */
        portGroupsBmp = 1;

        /* 1.1. Go over all active port groups. */
        PRV_CPSS_GEN_PP_START_LOOP_PORT_GROUPS_MAC(dev, portGroupId)
        {
            /*
                1.1. Call function with stage [CPSS_DXCH_POLICER_STAGE_INGRESS_0_E
                                               CPSS_DXCH_POLICER_STAGE_INGRESS_1_E /
                                               CPSS_DXCH_POLICER_STAGE_EGRESS_E].
                billingCntr.greenCntr.l[0] [0 / 10 / 100];
                billingCntr.greenCntr.l[1] [0 / 10 / 100];
                billingCntr.yellowCntr.l[0][0 / 10 / 100];
                billingCntr.yellowCntr.l[1][0 / 10 / 100];
                billingCntr.redCntr.l[0]   [0 / 10 / 100];
                billingCntr.redCntr.l[1]   [0 / 10 / 100];
                billingCntr.billingCntrMode [CPSS_DXCH3_POLICER_BILLING_CNTR_1_BYTE_E;
                                             CPSS_DXCH3_POLICER_BILLING_CNTR_16_BYTES_E,
                                             CPSS_DXCH3_POLICER_BILLING_CNTR_PACKET_E].
                Expected: GT_OK.
            */
            stage = CPSS_DXCH_POLICER_STAGE_INGRESS_0_E;

            if (PRV_CPSS_DXCH_XCAT_FAMILY_CHECK_MAC(dev))
            {
                /* previous tests may disable counting.
                   enable billing counting. */
                st = cpssDxChPolicerCountingModeSet(dev, stage, CPSS_DXCH_POLICER_COUNTING_BILLING_IPFIX_E);
                UTF_VERIFY_EQUAL2_STRING_MAC(GT_OK, st, "cpssDxChPolicerCountingModeSet: %d, %d", dev, plrStage);
            }

            entryIndex = PRV_CPSS_DXCH_PP_MAC(dev)->policer.memSize[(stage)] - 1;

            billingCntr.greenCntr.l[0] = 0;
            billingCntr.greenCntr.l[1] = 0;
            billingCntr.yellowCntr.l[0] = 0;
            billingCntr.yellowCntr.l[1] = 0;
            billingCntr.redCntr.l[0] = 0;
            billingCntr.redCntr.l[1] = 0;
            billingCntr.billingCntrMode = CPSS_DXCH3_POLICER_BILLING_CNTR_1_BYTE_E;

            st = cpssDxChPolicerPortGroupBillingEntrySet(dev, portGroupsBmp,
                                  stage, entryIndex, &billingCntr);
            UTF_VERIFY_EQUAL1_PARAM_MAC(GT_OK, st, dev);

            /*
                1.2. Call cpssDxChPolicerPortGroupBillingEntryGet with the same params.
                Expected: GT_OK and the same values.
            */
            reset = GT_FALSE;

            /* reset MSB and counter mode for non Xcat and above devices */
            if (! PRV_CPSS_DXCH_XCAT_FAMILY_CHECK_MAC(dev))
            {
                billingCntrGet.greenCntr.l[1] = 0;
                billingCntrGet.yellowCntr.l[1] = 0;
                billingCntrGet.redCntr.l[1] = 0;
                billingCntrGet.billingCntrMode = billingCntr.billingCntrMode;
            }
            st = cpssDxChPolicerPortGroupBillingEntryGet(dev, portGroupsBmp,
                                  stage, entryIndex, reset, &billingCntrGet);
            UTF_VERIFY_EQUAL1_STRING_MAC(GT_OK, st,
                   "cpssDxChPolicerPortGroupBillingEntryGet: %d", dev);

            /* verifying values */
            isEqual = (0 == cpssOsMemCmp((GT_VOID*) &billingCntr,
                                         (GT_VOID*) &billingCntrGet,
                                              sizeof(billingCntr))) ? GT_TRUE : GT_FALSE;
            UTF_VERIFY_EQUAL1_STRING_MAC(GT_TRUE, isEqual,
                       "get another billingCntr than was set: %d", dev);

            /*
                1.1. Call function with stage [CPSS_DXCH_POLICER_STAGE_INGRESS_0_E
                                               CPSS_DXCH_POLICER_STAGE_INGRESS_1_E /
                                               CPSS_DXCH_POLICER_STAGE_EGRESS_E].
                billingCntr.greenCntr.l[0] [0 / 10 / 100];
                billingCntr.greenCntr.l[1] [0 / 10 / 100];
                billingCntr.yellowCntr.l[0][0 / 10 / 100];
                billingCntr.yellowCntr.l[1][0 / 10 / 100];
                billingCntr.redCntr.l[0]   [0 / 10 / 100];
                billingCntr.redCntr.l[1]   [0 / 10 / 100];
                billingCntr.billingCntrMode [CPSS_DXCH3_POLICER_BILLING_CNTR_1_BYTE_E;
                                             CPSS_DXCH3_POLICER_BILLING_CNTR_16_BYTES_E,
                                             CPSS_DXCH3_POLICER_BILLING_CNTR_PACKET_E].
                Expected: GT_OK.
            */
            stage = CPSS_DXCH_POLICER_STAGE_INGRESS_1_E;

            if (PRV_CPSS_DXCH_XCAT_FAMILY_CHECK_MAC(dev))
            {
                /* previous tests may disable counting.
                   enable billing counting. */
                st = cpssDxChPolicerCountingModeSet(dev, stage, CPSS_DXCH_POLICER_COUNTING_BILLING_IPFIX_E);
                UTF_VERIFY_EQUAL2_STRING_MAC(GT_OK, st, "cpssDxChPolicerCountingModeSet: %d, %d", dev, plrStage);
            }

            entryIndex = PRV_CPSS_DXCH_PP_MAC(dev)->policer.memSize[(stage)] - 1;

            billingCntr.greenCntr.l[0] = 10;
            billingCntr.greenCntr.l[1] = 10;
            billingCntr.yellowCntr.l[0] = 10;
            billingCntr.yellowCntr.l[1] = 10;
            billingCntr.redCntr.l[0] = 10;
            billingCntr.redCntr.l[1] = 10;
            billingCntr.billingCntrMode = CPSS_DXCH3_POLICER_BILLING_CNTR_16_BYTES_E;

            st = cpssDxChPolicerPortGroupBillingEntrySet(dev, portGroupsBmp,
                                  stage, entryIndex, &billingCntr);
            if (!IS_STAGE_EXIST_MAC(dev, stage))
            {
                UTF_VERIFY_EQUAL1_PARAM_MAC(GT_BAD_PARAM, st, dev);
            }
            else
            {
                UTF_VERIFY_EQUAL1_PARAM_MAC(GT_OK, st, dev);

                /*
                    1.2. Call cpssDxChPolicerPortGroupBillingEntryGet with the same params.
                    Expected: GT_OK and the same values.
                */
                st = cpssDxChPolicerPortGroupBillingEntryGet(dev, portGroupsBmp,
                                      stage, entryIndex, reset, &billingCntrGet);
                UTF_VERIFY_EQUAL1_STRING_MAC(GT_OK, st,
                       "cpssDxChPolicerPortGroupBillingEntryGet: %d", dev);

                /* verifying values */
                isEqual = (0 == cpssOsMemCmp((GT_VOID*) &billingCntr,
                                             (GT_VOID*) &billingCntrGet,
                                                  sizeof(billingCntr))) ? GT_TRUE : GT_FALSE;
                UTF_VERIFY_EQUAL1_STRING_MAC(GT_TRUE, isEqual,
                           "get another billingCntr than was set: %d", dev);
            }
            /*
                1.1. Call function with stage [CPSS_DXCH_POLICER_STAGE_INGRESS_0_E
                                               CPSS_DXCH_POLICER_STAGE_INGRESS_1_E /
                                               CPSS_DXCH_POLICER_STAGE_EGRESS_E].
                billingCntr.greenCntr.l[0] [0 / 10 / 100];
                billingCntr.greenCntr.l[1] [0 / 10 / 100];
                billingCntr.yellowCntr.l[0][0 / 10 / 100];
                billingCntr.yellowCntr.l[1][0 / 10 / 100];
                billingCntr.redCntr.l[0]   [0 / 10 / 100];
                billingCntr.redCntr.l[1]   [0 / 10 / 100];
                billingCntr.billingCntrMode [CPSS_DXCH3_POLICER_BILLING_CNTR_1_BYTE_E;
                                             CPSS_DXCH3_POLICER_BILLING_CNTR_16_BYTES_E,
                                             CPSS_DXCH3_POLICER_BILLING_CNTR_PACKET_E].
                Expected: GT_OK.
            */
            stage = CPSS_DXCH_POLICER_STAGE_EGRESS_E;

            if (PRV_CPSS_DXCH_XCAT_FAMILY_CHECK_MAC(dev))
            {
                /* previous tests may disable counting.
                   enable billing counting. */
                st = cpssDxChPolicerCountingModeSet(dev, stage, CPSS_DXCH_POLICER_COUNTING_BILLING_IPFIX_E);
                UTF_VERIFY_EQUAL2_STRING_MAC(GT_OK, st, "cpssDxChPolicerCountingModeSet: %d, %d", dev, plrStage);
            }

            entryIndex = PRV_CPSS_DXCH_PP_MAC(dev)->policer.memSize[(stage)] - 1;

            billingCntr.greenCntr.l[0] = 100;
            billingCntr.greenCntr.l[1] = 100;
            billingCntr.yellowCntr.l[0] = 100;
            billingCntr.yellowCntr.l[1] = 100;
            billingCntr.redCntr.l[0] = 100;
            billingCntr.redCntr.l[1] = 100;
            billingCntr.billingCntrMode = CPSS_DXCH3_POLICER_BILLING_CNTR_PACKET_E;

            st = cpssDxChPolicerPortGroupBillingEntrySet(dev, portGroupsBmp,
                                  stage, entryIndex, &billingCntr);

            if (!IS_STAGE_EXIST_MAC(dev, stage))
            {
                UTF_VERIFY_EQUAL1_PARAM_MAC(GT_BAD_PARAM, st, dev);
            }
            else
            {
                UTF_VERIFY_EQUAL1_PARAM_MAC(GT_OK, st, dev);

                /*
                    1.2. Call cpssDxChPolicerPortGroupBillingEntryGet with the same params.
                    Expected: GT_OK and the same values.
                */
                reset = GT_FALSE;

                st = cpssDxChPolicerPortGroupBillingEntryGet(dev, portGroupsBmp,
                                      stage, entryIndex, reset, &billingCntrGet);
                UTF_VERIFY_EQUAL1_STRING_MAC(GT_OK, st,
                       "cpssDxChPolicerPortGroupBillingEntryGet: %d", dev);

                /* verifying values */
                isEqual = (0 == cpssOsMemCmp((GT_VOID*) &billingCntr,
                                             (GT_VOID*) &billingCntrGet,
                                                  sizeof(billingCntr))) ? GT_TRUE : GT_FALSE;
                UTF_VERIFY_EQUAL1_STRING_MAC(GT_TRUE, isEqual,
                           "get another billingCntr than was set: %d", dev);
            }
            /*
                1.3. Call api with wrong stage [wrong enum values].
                Expected: GT_BAD_PARAM.
            */
            UTF_ENUMS_CHECK_MAC(cpssDxChPolicerPortGroupBillingEntrySet
                                (dev, portGroupsBmp, stage,
                                entryIndex, &billingCntr),
                                stage);

            /*
                1.4. Call api with wrong entryIndex [max + 1].
                Expected: NOT GT_OK.
            */
            stage = CPSS_DXCH_POLICER_STAGE_INGRESS_0_E;
            entryIndex = PRV_CPSS_DXCH_PP_MAC(dev)->policer.memSize[(stage)] + 1;

            st = cpssDxChPolicerPortGroupBillingEntrySet(dev, portGroupsBmp,
                                  stage, entryIndex, &billingCntr);
            UTF_VERIFY_NOT_EQUAL1_PARAM_MAC(GT_OK, st, dev);

            entryIndex = PRV_CPSS_DXCH_PP_MAC(dev)->policer.memSize[(stage)] - 1;

            /*
                1.5. Call api with wrong billingCntrPtr [NULL].
                Expected: GT_BAD_PTR.
            */
            st = cpssDxChPolicerPortGroupBillingEntrySet(dev, portGroupsBmp,
                                  stage, entryIndex, NULL);
            UTF_VERIFY_EQUAL1_PARAM_MAC(GT_BAD_PTR, st, dev);

            /* set next active port */
            portGroupsBmp = (1 << portGroupId);
        }
        PRV_CPSS_GEN_PP_END_LOOP_PORT_GROUPS_MAC(dev,portGroupId)

        /* restore port group */
        portGroupsBmp = 1;

        /* 1.2. For not-active port groups check that function returns GT_BAD_PARAM. */
        PRV_CPSS_GEN_PP_START_LOOP_NON_ACTIVE_PORT_GROUPS_MAC(dev,portGroupId)
        {
            st = cpssDxChPolicerPortGroupBillingEntrySet(dev, portGroupsBmp,
                                  stage, entryIndex, &billingCntr);
            if(PRV_CPSS_IS_MULTI_PORT_GROUPS_DEVICE_MAC(dev))
            {
                UTF_VERIFY_EQUAL2_PARAM_MAC(GT_BAD_PARAM, st, dev, portGroupsBmp);
            }
            else
            {
                UTF_VERIFY_EQUAL2_PARAM_MAC(GT_OK, st, dev, portGroupsBmp);
            }

            /* set next non-active port */
            portGroupsBmp = (1 << portGroupId);
        }
        PRV_CPSS_GEN_PP_END_LOOP_NON_ACTIVE_PORT_GROUPS_MAC(dev,portGroupId)
    }

    /* restore correct values */
    portGroupId = 1;
    stage = CPSS_DXCH_POLICER_STAGE_INGRESS_0_E;
    entryIndex = 0x0;

    /* 2. For not-active devices and devices from non-applicable family */
    /* check that function returns GT_BAD_PARAM.                        */

    /* prepare device iterator */
    PRV_TGF_NOT_APPLIC_DEV_RESET_MAC(&dev, UTF_CH1_E | UTF_CH1_DIAMOND_E | UTF_CH2_E);

    /* go over all non active devices */
    while(GT_OK == prvUtfNextDeviceGet(&dev, GT_FALSE))
    {
        st = cpssDxChPolicerPortGroupBillingEntrySet(dev, portGroupsBmp,
                                  stage, entryIndex, &billingCntr);
        UTF_VERIFY_EQUAL1_PARAM_MAC(GT_NOT_APPLICABLE_DEVICE, st, dev);
    }

    /* 3. Call function with out of bound value for device id.*/
    dev = PRV_CPSS_MAX_PP_DEVICES_CNS;

    st = cpssDxChPolicerPortGroupBillingEntrySet(dev, portGroupsBmp,
                                  stage, entryIndex, &billingCntr);
    UTF_VERIFY_EQUAL1_PARAM_MAC(GT_BAD_PARAM, st, dev);
}

/*----------------------------------------------------------------------------*/
/*
GT_STATUS cpssDxChPolicerPortGroupErrorCounterGet
(
    IN  GT_U8                             devNum,
    IN  GT_PORT_GROUPS_BMP                portGroupsBmp,
    IN  CPSS_DXCH_POLICER_STAGE_TYPE_ENT  stage,
    OUT GT_U32                            *cntrValuePtr
)
*/
UTF_TEST_CASE_MAC(cpssDxChPolicerPortGroupErrorCounterGet)
{
/*
    ITERATE_DEVICES_PORT_GROUPS(DxCh3 and above)
    1.1. Call function with stage [CPSS_DXCH_POLICER_STAGE_INGRESS_0_E /
                                   CPSS_DXCH_POLICER_STAGE_INGRESS_1_E /
                                   CPSS_DXCH_POLICER_STAGE_EGRESS_E],
    Expected: GT_OK.
    1.2. Call api with wrong stage [wrong enum values].
    Expected: GT_BAD_PARAM.
    1.3. Call api with wrong cntrValuePtr [NULL].
    Expected: GT_BAD_PTR.
*/
    GT_STATUS st = GT_OK;
    GT_U32    portGroupId;

    GT_U8                             dev;
    GT_PORT_GROUPS_BMP                portGroupsBmp = 1;
    CPSS_DXCH_POLICER_STAGE_TYPE_ENT  stage = CPSS_DXCH_POLICER_STAGE_INGRESS_0_E;
    GT_U32                            cntrValue;

    /* prepare device iterator */
    PRV_TGF_NOT_APPLIC_DEV_RESET_MAC(&dev, UTF_CH1_E | UTF_CH1_DIAMOND_E | UTF_CH2_E);

    /* 1. Go over all active devices. */
    while (GT_OK == prvUtfNextDeviceGet(&dev, GT_TRUE))
    {
        /* initialize port group */
        portGroupsBmp = 1;

        /* 1.1. Go over all active port groups. */
        PRV_CPSS_GEN_PP_START_LOOP_PORT_GROUPS_MAC(dev, portGroupId)
        {
            /*
                1.1. Call function with stage [CPSS_DXCH_POLICER_STAGE_INGRESS_0_E
                                               CPSS_DXCH_POLICER_STAGE_INGRESS_1_E /
                                               CPSS_DXCH_POLICER_STAGE_EGRESS_E],
                Expected: GT_OK.
            */
            /*call with stage = CPSS_DXCH_POLICER_STAGE_INGRESS_0_E;*/
            stage = CPSS_DXCH_POLICER_STAGE_INGRESS_0_E;

            st = cpssDxChPolicerPortGroupErrorCounterGet(dev, portGroupsBmp,
                                  stage, &cntrValue);
            UTF_VERIFY_EQUAL1_PARAM_MAC(GT_OK, st, dev);

            /*call with stage = CPSS_DXCH_POLICER_STAGE_INGRESS_1_E;*/
            stage = CPSS_DXCH_POLICER_STAGE_INGRESS_1_E;

            st = cpssDxChPolicerPortGroupErrorCounterGet(dev, portGroupsBmp,
                                  stage, &cntrValue);
            if (PRV_CPSS_DXCH_XCAT_FAMILY_CHECK_MAC(dev) && !IS_STAGE_EXIST_MAC(dev, stage))
            {
                UTF_VERIFY_EQUAL1_PARAM_MAC(GT_BAD_PARAM, st, dev);
            }
            else
                UTF_VERIFY_EQUAL1_PARAM_MAC(GT_OK, st, dev);

            /*call with stage = CPSS_DXCH_POLICER_STAGE_EGRESS_E;*/
            stage = CPSS_DXCH_POLICER_STAGE_EGRESS_E;

            st = cpssDxChPolicerPortGroupErrorCounterGet(dev, portGroupsBmp,
                                  stage, &cntrValue);
            if (PRV_CPSS_DXCH_XCAT_FAMILY_CHECK_MAC(dev) && !IS_STAGE_EXIST_MAC(dev, stage))
            {
                UTF_VERIFY_EQUAL1_PARAM_MAC(GT_BAD_PARAM, st, dev);
            }
            else
                UTF_VERIFY_EQUAL1_PARAM_MAC(GT_OK, st, dev);

            /*
                1.2. Call api with wrong stage [wrong enum values].
                Expected: GT_BAD_PARAM.
            */
            if (PRV_CPSS_DXCH_XCAT_FAMILY_CHECK_MAC(dev))
            {
                UTF_ENUMS_CHECK_MAC(cpssDxChPolicerPortGroupErrorCounterGet
                                (dev, portGroupsBmp, stage, &cntrValue),
                                stage);
            }
            /*
                1.3. Call api with wrong cntrValuePtr [NULL].
                Expected: GT_BAD_PTR.
            */
            stage = CPSS_DXCH_POLICER_STAGE_INGRESS_0_E;
            st = cpssDxChPolicerPortGroupErrorCounterGet(dev, portGroupsBmp,
                                  stage, NULL);
            UTF_VERIFY_EQUAL1_PARAM_MAC(GT_BAD_PTR, st, dev);

            /* set next active port */
            portGroupsBmp = (1 << portGroupId);
        }
        PRV_CPSS_GEN_PP_END_LOOP_PORT_GROUPS_MAC(dev,portGroupId)

        /* restore port group */
        portGroupsBmp = 1;

        /* 1.2. For not-active port groups check that function returns GT_BAD_PARAM. */
        PRV_CPSS_GEN_PP_START_LOOP_NON_ACTIVE_PORT_GROUPS_MAC(dev,portGroupId)
        {
            st = cpssDxChPolicerPortGroupErrorCounterGet(dev, portGroupsBmp,
                                  stage, &cntrValue);
            if(PRV_CPSS_IS_MULTI_PORT_GROUPS_DEVICE_MAC(dev))
            {
                UTF_VERIFY_EQUAL2_PARAM_MAC(GT_BAD_PARAM, st, dev, portGroupsBmp);
            }
            else
            {
                UTF_VERIFY_EQUAL2_PARAM_MAC(GT_OK, st, dev, portGroupsBmp);
            }

            /* set next non-active port */
            portGroupsBmp = (1 << portGroupId);
        }
        PRV_CPSS_GEN_PP_END_LOOP_NON_ACTIVE_PORT_GROUPS_MAC(dev,portGroupId)
    }

    /* restore correct values */
    portGroupId = 1;
    stage = CPSS_DXCH_POLICER_STAGE_INGRESS_0_E;

    /* 2. For not-active devices and devices from non-applicable family */
    /* check that function returns GT_BAD_PARAM.                        */

    /* prepare device iterator */
    PRV_TGF_NOT_APPLIC_DEV_RESET_MAC(&dev, UTF_CH1_E | UTF_CH1_DIAMOND_E | UTF_CH2_E);

    /* go over all non active devices */
    while(GT_OK == prvUtfNextDeviceGet(&dev, GT_FALSE))
    {
        st = cpssDxChPolicerPortGroupErrorCounterGet(dev, portGroupsBmp,
                                  stage, &cntrValue);
        UTF_VERIFY_EQUAL1_PARAM_MAC(GT_NOT_APPLICABLE_DEVICE, st, dev);
    }

    /* 3. Call function with out of bound value for device id.*/
    dev = PRV_CPSS_MAX_PP_DEVICES_CNS;

    st = cpssDxChPolicerPortGroupErrorCounterGet(dev, portGroupsBmp,
                                  stage, &cntrValue);
    UTF_VERIFY_EQUAL1_PARAM_MAC(GT_BAD_PARAM, st, dev);
}

/*----------------------------------------------------------------------------*/
/*
GT_STATUS cpssDxChPolicerPortGroupErrorGet
(
    IN  GT_U8                               devNum,
    IN  GT_PORT_GROUPS_BMP                  portGroupsBmp,
    IN  CPSS_DXCH_POLICER_STAGE_TYPE_ENT    stage,
    OUT CPSS_DXCH3_POLICER_ENTRY_TYPE_ENT   *entryTypePtr,
    OUT GT_U32                              *entryAddrPtr
)
*/
UTF_TEST_CASE_MAC(cpssDxChPolicerPortGroupErrorGet)
{
/*
    ITERATE_DEVICES_PORT_GROUPS(DxCh3 and above)
    1.1. Call function with stage [CPSS_DXCH_POLICER_STAGE_INGRESS_0_E /
                                   CPSS_DXCH_POLICER_STAGE_INGRESS_1_E /
                                   CPSS_DXCH_POLICER_STAGE_EGRESS_E].
    Expected: GT_OK.
    1.2. Call api with wrong stage [wrong enum values].
    Expected: GT_BAD_PARAM.
    1.3. Call api with wrong entryTypePtr [NULL].
    Expected: GT_BAD_PTR.
    1.4. Call api with wrong entryAddrPtr [NULL].
    Expected: GT_BAD_PTR.
*/
    GT_STATUS st = GT_OK;
    GT_U32    portGroupId;

    GT_U8                               dev;
    GT_PORT_GROUPS_BMP                  portGroupsBmp = 1;
    CPSS_DXCH_POLICER_STAGE_TYPE_ENT    stage = CPSS_DXCH_POLICER_STAGE_INGRESS_0_E;
    CPSS_DXCH3_POLICER_ENTRY_TYPE_ENT   entryType;
    GT_U32                              entryAddr;

    /* prepare device iterator */
    PRV_TGF_NOT_APPLIC_DEV_RESET_MAC(&dev, UTF_CH1_E | UTF_CH1_DIAMOND_E | UTF_CH2_E);

    /* 1. Go over all active devices. */
    while (GT_OK == prvUtfNextDeviceGet(&dev, GT_TRUE))
    {
        /* initialize port group */
        portGroupsBmp = 1;

        /* 1.1. Go over all active port groups. */
        PRV_CPSS_GEN_PP_START_LOOP_PORT_GROUPS_MAC(dev, portGroupId)
        {
            /*
                1.1. Call function with stage [CPSS_DXCH_POLICER_STAGE_INGRESS_0_E /
                                               CPSS_DXCH_POLICER_STAGE_INGRESS_1_E /
                                               CPSS_DXCH_POLICER_STAGE_EGRESS_E],
                Expected: GT_OK.
            */
            /*call with stage = CPSS_DXCH_POLICER_STAGE_INGRESS_0_E;*/
            stage = CPSS_DXCH_POLICER_STAGE_INGRESS_0_E;

            st = cpssDxChPolicerPortGroupErrorGet(dev, portGroupsBmp,
                           stage, &entryType, &entryAddr);
            st = (st == GT_EMPTY)? GT_OK : st;
            UTF_VERIFY_EQUAL1_PARAM_MAC(GT_OK, st, dev);

            /*call with stage = CPSS_DXCH_POLICER_STAGE_INGRESS_1_E;*/
            stage = CPSS_DXCH_POLICER_STAGE_INGRESS_1_E;

            st = cpssDxChPolicerPortGroupErrorGet(dev, portGroupsBmp,
                           stage, &entryType, &entryAddr);
            st = (st == GT_EMPTY)? GT_OK : st;
            if (PRV_CPSS_DXCH_XCAT_FAMILY_CHECK_MAC(dev) && !IS_STAGE_EXIST_MAC(dev, stage))
            {
                UTF_VERIFY_EQUAL1_PARAM_MAC(GT_BAD_PARAM, st, dev);
            }
            else
                UTF_VERIFY_EQUAL1_PARAM_MAC(GT_OK, st, dev);


            /*call with stage = CPSS_DXCH_POLICER_STAGE_EGRESS_E;*/
            stage = CPSS_DXCH_POLICER_STAGE_EGRESS_E;

            st = cpssDxChPolicerPortGroupErrorGet(dev, portGroupsBmp,
                           stage, &entryType, &entryAddr);
            st = (st == GT_EMPTY)? GT_OK : st;
            if (PRV_CPSS_DXCH_XCAT_FAMILY_CHECK_MAC(dev) && !IS_STAGE_EXIST_MAC(dev, stage))
            {
                UTF_VERIFY_EQUAL1_PARAM_MAC(GT_BAD_PARAM, st, dev);
            }
            else
                UTF_VERIFY_EQUAL1_PARAM_MAC(GT_OK, st, dev);

            /*
                1.2. Call api with wrong stage [wrong enum values].
                Expected: GT_BAD_PARAM.
            */
            if (PRV_CPSS_DXCH_XCAT_FAMILY_CHECK_MAC(dev))
            {
                UTF_ENUMS_CHECK_MAC(cpssDxChPolicerPortGroupErrorGet
                                (dev, portGroupsBmp, stage,
                                &entryType, &entryAddr),
                                stage);
            }
            /*
                1.3. Call api with wrong entryTypePtr [NULL].
                Expected: GT_BAD_PTR.
            */
            stage = CPSS_DXCH_POLICER_STAGE_INGRESS_0_E;
            st = cpssDxChPolicerPortGroupErrorGet(dev, portGroupsBmp,
                           stage, NULL, &entryAddr);
            UTF_VERIFY_EQUAL1_PARAM_MAC(GT_BAD_PTR, st, dev);

            /*
                1.4. Call api with wrong entryAddrPtr [NULL].
                Expected: GT_BAD_PTR.
            */
            st = cpssDxChPolicerPortGroupErrorGet(dev, portGroupsBmp,
                           stage, &entryType, NULL);
            UTF_VERIFY_EQUAL1_PARAM_MAC(GT_BAD_PTR, st, dev);

            /* set next active port */
            portGroupsBmp = (1 << portGroupId);
        }
        PRV_CPSS_GEN_PP_END_LOOP_PORT_GROUPS_MAC(dev,portGroupId)

        /* restore port group */
        portGroupsBmp = 1;

        /* 1.2. For not-active port groups check that function returns GT_BAD_PARAM. */
        PRV_CPSS_GEN_PP_START_LOOP_NON_ACTIVE_PORT_GROUPS_MAC(dev,portGroupId)
        {
            st = cpssDxChPolicerPortGroupErrorGet(dev, portGroupsBmp,
                           stage, &entryType, &entryAddr);
            st = (st == GT_EMPTY)? GT_OK : st;

            if(PRV_CPSS_IS_MULTI_PORT_GROUPS_DEVICE_MAC(dev))
            {
                UTF_VERIFY_EQUAL2_PARAM_MAC(GT_BAD_PARAM, st, dev, portGroupsBmp);
            }
            else
            {
                UTF_VERIFY_EQUAL2_PARAM_MAC(GT_OK, st, dev, portGroupsBmp);
            }

            /* set next non-active port */
            portGroupsBmp = (1 << portGroupId);
        }
        PRV_CPSS_GEN_PP_END_LOOP_NON_ACTIVE_PORT_GROUPS_MAC(dev,portGroupId)
    }

    /* restore correct values */
    portGroupId = 1;
    stage = CPSS_DXCH_POLICER_STAGE_INGRESS_0_E;

    /* 2. For not-active devices and devices from non-applicable family */
    /* check that function returns GT_BAD_PARAM.                        */

    /* prepare device iterator */
    PRV_TGF_NOT_APPLIC_DEV_RESET_MAC(&dev, UTF_CH1_E | UTF_CH1_DIAMOND_E | UTF_CH2_E);

    /* go over all non active devices */
    while(GT_OK == prvUtfNextDeviceGet(&dev, GT_FALSE))
    {
        st = cpssDxChPolicerPortGroupErrorGet(dev, portGroupsBmp,
                           stage, &entryType, &entryAddr);
        UTF_VERIFY_EQUAL1_PARAM_MAC(GT_NOT_APPLICABLE_DEVICE, st, dev);
    }

    /* 3. Call function with out of bound value for device id.*/
    dev = PRV_CPSS_MAX_PP_DEVICES_CNS;

    st = cpssDxChPolicerPortGroupErrorGet(dev, portGroupsBmp,
                           stage, &entryType, &entryAddr);
    UTF_VERIFY_EQUAL1_PARAM_MAC(GT_BAD_PARAM, st, dev);
}

/*----------------------------------------------------------------------------*/
/*
GT_STATUS cpssDxChPolicerPortGroupManagementCountersGet
(
    IN  GT_U8                                  devNum,
    IN  GT_PORT_GROUPS_BMP                     portGroupsBmp,
    IN  CPSS_DXCH_POLICER_STAGE_TYPE_ENT       stage,
    IN  CPSS_DXCH3_POLICER_MNG_CNTR_SET_ENT    mngCntrSet,
    IN  CPSS_DXCH3_POLICER_MNG_CNTR_TYPE_ENT   mngCntrType,
    OUT CPSS_DXCH3_POLICER_MNG_CNTR_ENTRY_STC  *mngCntrPtr
)
*/
UTF_TEST_CASE_MAC(cpssDxChPolicerPortGroupManagementCountersGet)
{
/*
    ITERATE_DEVICES_PORT_GROUPS(DxCh3 and above)
    1.1. Call function with stage [CPSS_DXCH_POLICER_STAGE_INGRESS_0_E /
                                   CPSS_DXCH_POLICER_STAGE_INGRESS_1_E /
                                   CPSS_DXCH_POLICER_STAGE_EGRESS_E],
        mngCntrSet [CPSS_DXCH3_POLICER_MNG_CNTR_SET0_E,
                    CPSS_DXCH3_POLICER_MNG_CNTR_SET1_E,
                    CPSS_DXCH3_POLICER_MNG_CNTR_SET2_E]
        mngCntrType [CPSS_DXCH3_POLICER_MNG_CNTR_GREEN_E,
                     CPSS_DXCH3_POLICER_MNG_CNTR_RED_E,
                     CPSS_DXCH3_POLICER_MNG_CNTR_DROP_E].
    Expected: GT_OK.
    1.2. Call api with wrong stage [wrong enum values].
    Expected: GT_BAD_PARAM.
    1.3. Call api with wrong mngCntrSet [wrong enum values].
    Expected: GT_BAD_PARAM.
    1.4. Call api with wrong mngCntrType [wrong enum values].
    Expected: GT_BAD_PARAM.
    1.5. Call api with wrong mngCntrPtr [NULL].
    Expected: GT_BAD_PTR.
*/
    GT_STATUS st = GT_OK;
    GT_U32    portGroupId;

    GT_U8                                  dev;
    GT_PORT_GROUPS_BMP                     portGroupsBmp = 1;
    CPSS_DXCH_POLICER_STAGE_TYPE_ENT       stage = CPSS_DXCH_POLICER_STAGE_INGRESS_0_E;
    CPSS_DXCH3_POLICER_MNG_CNTR_SET_ENT    mngCntrSet = CPSS_DXCH3_POLICER_MNG_CNTR_SET0_E;
    CPSS_DXCH3_POLICER_MNG_CNTR_TYPE_ENT   mngCntrType = CPSS_DXCH3_POLICER_MNG_CNTR_GREEN_E;
    CPSS_DXCH3_POLICER_MNG_CNTR_ENTRY_STC  mngCntr;

    /* prepare device iterator */
    PRV_TGF_NOT_APPLIC_DEV_RESET_MAC(&dev, UTF_CH1_E | UTF_CH1_DIAMOND_E | UTF_CH2_E);

    /* 1. Go over all active devices. */
    while (GT_OK == prvUtfNextDeviceGet(&dev, GT_TRUE))
    {
        /* initialize port group */
        portGroupsBmp = 1;

        /* 1.1. Go over all active port groups. */
        PRV_CPSS_GEN_PP_START_LOOP_PORT_GROUPS_MAC(dev, portGroupId)
        {
            /*
                1.1. Call function with stage [CPSS_DXCH_POLICER_STAGE_INGRESS_0_E /
                                               CPSS_DXCH_POLICER_STAGE_INGRESS_1_E /
                                               CPSS_DXCH_POLICER_STAGE_EGRESS_E],
                    mngCntrSet [CPSS_DXCH3_POLICER_MNG_CNTR_SET0_E,
                                CPSS_DXCH3_POLICER_MNG_CNTR_SET1_E,
                                CPSS_DXCH3_POLICER_MNG_CNTR_SET2_E]
                    mngCntrType [CPSS_DXCH3_POLICER_MNG_CNTR_GREEN_E,
                                 CPSS_DXCH3_POLICER_MNG_CNTR_RED_E,
                                 CPSS_DXCH3_POLICER_MNG_CNTR_DROP_E].

                Expected: GT_OK.
            */
            stage = CPSS_DXCH_POLICER_STAGE_INGRESS_0_E;
            mngCntrSet = CPSS_DXCH3_POLICER_MNG_CNTR_SET0_E;
            mngCntrType = CPSS_DXCH3_POLICER_MNG_CNTR_GREEN_E;

            st = cpssDxChPolicerPortGroupManagementCountersGet(dev, portGroupsBmp,
                                        stage, mngCntrSet, mngCntrType, &mngCntr);
            UTF_VERIFY_EQUAL1_PARAM_MAC(GT_OK, st, dev);

            /*
                1.1. Call function with stage [CPSS_DXCH_POLICER_STAGE_INGRESS_0_E /
                                               CPSS_DXCH_POLICER_STAGE_INGRESS_1_E /
                                               CPSS_DXCH_POLICER_STAGE_EGRESS_E],
                    mngCntrSet [CPSS_DXCH3_POLICER_MNG_CNTR_SET0_E,
                                CPSS_DXCH3_POLICER_MNG_CNTR_SET1_E,
                                CPSS_DXCH3_POLICER_MNG_CNTR_SET2_E]
                    mngCntrType [CPSS_DXCH3_POLICER_MNG_CNTR_GREEN_E,
                                 CPSS_DXCH3_POLICER_MNG_CNTR_RED_E,
                                 CPSS_DXCH3_POLICER_MNG_CNTR_DROP_E].

                Expected: GT_OK.
            */
            stage = CPSS_DXCH_POLICER_STAGE_INGRESS_1_E;
            mngCntrSet = CPSS_DXCH3_POLICER_MNG_CNTR_SET1_E;
            mngCntrType = CPSS_DXCH3_POLICER_MNG_CNTR_RED_E;

            st = cpssDxChPolicerPortGroupManagementCountersGet(dev, portGroupsBmp,
                                        stage, mngCntrSet, mngCntrType, &mngCntr);
            if (PRV_CPSS_DXCH_XCAT_FAMILY_CHECK_MAC(dev) && !IS_STAGE_EXIST_MAC(dev, stage))
            {
                UTF_VERIFY_EQUAL1_PARAM_MAC(GT_BAD_PARAM, st, dev);
            }
            else
                UTF_VERIFY_EQUAL1_PARAM_MAC(GT_OK, st, dev);

            /*
                1.1. Call function with stage [CPSS_DXCH_POLICER_STAGE_INGRESS_0_E /
                                               CPSS_DXCH_POLICER_STAGE_INGRESS_1_E /
                                               CPSS_DXCH_POLICER_STAGE_EGRESS_E],
                    mngCntrSet [CPSS_DXCH3_POLICER_MNG_CNTR_SET0_E,
                                CPSS_DXCH3_POLICER_MNG_CNTR_SET1_E,
                                CPSS_DXCH3_POLICER_MNG_CNTR_SET2_E]
                    mngCntrType [CPSS_DXCH3_POLICER_MNG_CNTR_GREEN_E,
                                 CPSS_DXCH3_POLICER_MNG_CNTR_RED_E,
                                 CPSS_DXCH3_POLICER_MNG_CNTR_DROP_E].

                Expected: GT_OK.
            */
            stage = CPSS_DXCH_POLICER_STAGE_EGRESS_E;
            mngCntrSet = CPSS_DXCH3_POLICER_MNG_CNTR_SET2_E;
            mngCntrType = CPSS_DXCH3_POLICER_MNG_CNTR_DROP_E;

            st = cpssDxChPolicerPortGroupManagementCountersGet(dev, portGroupsBmp,
                                        stage, mngCntrSet, mngCntrType, &mngCntr);
            if (PRV_CPSS_DXCH_XCAT_FAMILY_CHECK_MAC(dev) && !IS_STAGE_EXIST_MAC(dev, stage))
            {
                UTF_VERIFY_EQUAL1_PARAM_MAC(GT_BAD_PARAM, st, dev);
            }
            else
                UTF_VERIFY_EQUAL1_PARAM_MAC(GT_OK, st, dev);

            /*
                1.2. Call api with wrong stage [wrong enum values].
                Expected: GT_BAD_PARAM.
            */
            if (PRV_CPSS_DXCH_XCAT_FAMILY_CHECK_MAC(dev))
            {
                UTF_ENUMS_CHECK_MAC(cpssDxChPolicerPortGroupManagementCountersGet
                                (dev, portGroupsBmp, stage,
                                mngCntrSet, mngCntrType, &mngCntr),
                                stage);
            }
            /*
                1.3. Call api with wrong mngCntrSet [wrong enum values].
                Expected: GT_BAD_PARAM.
            */
            UTF_ENUMS_CHECK_MAC(cpssDxChPolicerPortGroupManagementCountersGet
                                (dev, portGroupsBmp, stage,
                                mngCntrSet, mngCntrType, &mngCntr),
                                mngCntrSet);

            /*
                1.4. Call api with wrong mngCntrType [wrong enum values].
                Expected: GT_BAD_PARAM.
            */
            UTF_ENUMS_CHECK_MAC(cpssDxChPolicerPortGroupManagementCountersGet
                                (dev, portGroupsBmp, stage,
                                mngCntrSet, mngCntrType, &mngCntr),
                                mngCntrType);

            /*
                1.5. Call api with wrong mngCntrPtr [NULL].
                Expected: GT_BAD_PTR.
            */
            stage = CPSS_DXCH_POLICER_STAGE_INGRESS_0_E;
            st = cpssDxChPolicerPortGroupManagementCountersGet(dev, portGroupsBmp,
                                        stage, mngCntrSet, mngCntrType, NULL);
            UTF_VERIFY_EQUAL1_PARAM_MAC(GT_BAD_PTR, st, dev);

            /* set next active port */
            portGroupsBmp = (1 << portGroupId);
        }
        PRV_CPSS_GEN_PP_END_LOOP_PORT_GROUPS_MAC(dev,portGroupId)

        /* restore port group */
        portGroupsBmp = 1;

        /* 1.2. For not-active port groups check that function returns GT_BAD_PARAM. */
        PRV_CPSS_GEN_PP_START_LOOP_NON_ACTIVE_PORT_GROUPS_MAC(dev,portGroupId)
        {
            st = cpssDxChPolicerPortGroupManagementCountersGet(dev, portGroupsBmp,
                                        stage, mngCntrSet, mngCntrType, &mngCntr);
            if(PRV_CPSS_IS_MULTI_PORT_GROUPS_DEVICE_MAC(dev))
            {
                UTF_VERIFY_EQUAL2_PARAM_MAC(GT_BAD_PARAM, st, dev, portGroupsBmp);
            }
            else
            {
                UTF_VERIFY_EQUAL2_PARAM_MAC(GT_OK, st, dev, portGroupsBmp);
            }

            /* set next non-active port */
            portGroupsBmp = (1 << portGroupId);
        }
        PRV_CPSS_GEN_PP_END_LOOP_NON_ACTIVE_PORT_GROUPS_MAC(dev,portGroupId)
    }

    /* restore correct values */
    portGroupId = 1;
    stage = CPSS_DXCH_POLICER_STAGE_INGRESS_0_E;
    mngCntrSet = CPSS_DXCH3_POLICER_MNG_CNTR_SET0_E;
    mngCntrType = CPSS_DXCH3_POLICER_MNG_CNTR_GREEN_E;

    /* 2. For not-active devices and devices from non-applicable family */
    /* check that function returns GT_BAD_PARAM.                        */

    /* prepare device iterator */
    PRV_TGF_NOT_APPLIC_DEV_RESET_MAC(&dev, UTF_CH1_E | UTF_CH1_DIAMOND_E | UTF_CH2_E);

    /* go over all non active devices */
    while(GT_OK == prvUtfNextDeviceGet(&dev, GT_FALSE))
    {
        st = cpssDxChPolicerPortGroupManagementCountersGet(dev, portGroupsBmp,
                                        stage, mngCntrSet, mngCntrType, &mngCntr);
        UTF_VERIFY_EQUAL1_PARAM_MAC(GT_NOT_APPLICABLE_DEVICE, st, dev);
    }

    /* 3. Call function with out of bound value for device id.*/
    dev = PRV_CPSS_MAX_PP_DEVICES_CNS;

    st = cpssDxChPolicerPortGroupManagementCountersGet(dev, portGroupsBmp,
                                        stage, mngCntrSet, mngCntrType, &mngCntr);
    UTF_VERIFY_EQUAL1_PARAM_MAC(GT_BAD_PARAM, st, dev);
}

/*----------------------------------------------------------------------------*/
/*
GT_STATUS cpssDxChPolicerPortGroupManagementCountersSet
(
    IN  GT_U8                                  devNum,
    IN  GT_PORT_GROUPS_BMP                     portGroupsBmp,
    IN  CPSS_DXCH_POLICER_STAGE_TYPE_ENT       stage,
    IN  CPSS_DXCH3_POLICER_MNG_CNTR_SET_ENT    mngCntrSet,
    IN  CPSS_DXCH3_POLICER_MNG_CNTR_TYPE_ENT   mngCntrType,
    IN  CPSS_DXCH3_POLICER_MNG_CNTR_ENTRY_STC  *mngCntrPtr
)
*/
UTF_TEST_CASE_MAC(cpssDxChPolicerPortGroupManagementCountersSet)
{
/*
    ITERATE_DEVICES_PORT_GROUPS(DxCh3 and above)
    1.1. Call function with stage [CPSS_DXCH_POLICER_STAGE_INGRESS_0_E /
                                   CPSS_DXCH_POLICER_STAGE_INGRESS_1_E /
                                   CPSS_DXCH_POLICER_STAGE_EGRESS_E],
        mngCntrSet [CPSS_DXCH3_POLICER_MNG_CNTR_SET0_E,
                    CPSS_DXCH3_POLICER_MNG_CNTR_SET1_E,
                    CPSS_DXCH3_POLICER_MNG_CNTR_SET2_E]
        mngCntrType [CPSS_DXCH3_POLICER_MNG_CNTR_GREEN_E,
                     CPSS_DXCH3_POLICER_MNG_CNTR_RED_E,
                     CPSS_DXCH3_POLICER_MNG_CNTR_DROP_E].
        mngCntr.duMngCntr.l[0] [0 / 10 / 100],
        mngCntr.duMngCntr.l[1] [0 / 10 / 100],
        mngCntr.packetMngCntr  [0 / 10 / 100],
    Expected: GT_OK.
    1.2. Call cpssDxChPolicerPortGroupManagementCountersGet
         with the same params.
    Expected: GT_OK and the same values.
    1.3. Call api with wrong stage [wrong enum values].
    Expected: GT_BAD_PARAM.
    1.4. Call api with wrong mngCntrSet [wrong enum values].
    Expected: GT_BAD_PARAM.
    1.5. Call api with wrong mngCntrType [wrong enum values].
    Expected: GT_BAD_PARAM.
    1.6. Call api with wrong mngCntrPtr [NULL].
    Expected: GT_BAD_PTR.
*/
    GT_STATUS st = GT_OK;
    GT_U32    portGroupId;
    GT_BOOL   isEqual = GT_FALSE;

    GT_U8                                  dev;
    GT_PORT_GROUPS_BMP                     portGroupsBmp = 1;
    CPSS_DXCH_POLICER_STAGE_TYPE_ENT       stage = CPSS_DXCH_POLICER_STAGE_INGRESS_0_E;
    CPSS_DXCH3_POLICER_MNG_CNTR_SET_ENT    mngCntrSet = CPSS_DXCH3_POLICER_MNG_CNTR_SET0_E;
    CPSS_DXCH3_POLICER_MNG_CNTR_TYPE_ENT   mngCntrType = CPSS_DXCH3_POLICER_MNG_CNTR_GREEN_E;
    CPSS_DXCH3_POLICER_MNG_CNTR_ENTRY_STC  mngCntr;
    CPSS_DXCH3_POLICER_MNG_CNTR_ENTRY_STC  mngCntrGet;

    /* zero mngCntr first */
    cpssOsBzero((GT_VOID*) &mngCntr, sizeof(mngCntr));
    cpssOsBzero((GT_VOID*) &mngCntrGet, sizeof(mngCntrGet));

    /* prepare device iterator */
    PRV_TGF_NOT_APPLIC_DEV_RESET_MAC(&dev, UTF_CH1_E | UTF_CH1_DIAMOND_E | UTF_CH2_E);

    /* 1. Go over all active devices. */
    while (GT_OK == prvUtfNextDeviceGet(&dev, GT_TRUE))
    {
        /* initialize port group */
        portGroupsBmp = 1;

        /* 1.1. Go over all active port groups. */
        PRV_CPSS_GEN_PP_START_LOOP_PORT_GROUPS_MAC(dev, portGroupId)
        {
            /*
                1.1. Call function with stage [CPSS_DXCH_POLICER_STAGE_INGRESS_0_E /
                                               CPSS_DXCH_POLICER_STAGE_INGRESS_1_E /
                                               CPSS_DXCH_POLICER_STAGE_EGRESS_E],
                    mngCntrSet [CPSS_DXCH3_POLICER_MNG_CNTR_SET0_E,
                                CPSS_DXCH3_POLICER_MNG_CNTR_SET1_E,
                                CPSS_DXCH3_POLICER_MNG_CNTR_SET2_E]
                    mngCntrType [CPSS_DXCH3_POLICER_MNG_CNTR_GREEN_E,
                                 CPSS_DXCH3_POLICER_MNG_CNTR_RED_E,
                                 CPSS_DXCH3_POLICER_MNG_CNTR_DROP_E].
                    mngCntr.duMngCntr.l[0] [0 / 10 / 100],
                    mngCntr.duMngCntr.l[1] [0 / 10 / 100],
                    mngCntr.packetMngCntr  [0 / 10 / 100],
                Expected: GT_OK.
            */
            stage = CPSS_DXCH_POLICER_STAGE_INGRESS_0_E;
            mngCntrSet = CPSS_DXCH3_POLICER_MNG_CNTR_SET0_E;
            mngCntrType = CPSS_DXCH3_POLICER_MNG_CNTR_GREEN_E;
            mngCntr.duMngCntr.l[0] = 0;
            mngCntr.duMngCntr.l[1] = 0;
            mngCntr.packetMngCntr  = 0;

            st = cpssDxChPolicerPortGroupManagementCountersSet(dev, portGroupsBmp,
                                        stage, mngCntrSet, mngCntrType, &mngCntr);
            UTF_VERIFY_EQUAL1_PARAM_MAC(GT_OK, st, dev);

            /*
                1.2. Call cpssDxChPolicerPortGroupManagementCountersGet
                     with the same params.
                Expected: GT_OK and the same values.
            */
            st = cpssDxChPolicerPortGroupManagementCountersGet(dev, portGroupsBmp,
                                        stage, mngCntrSet, mngCntrType, &mngCntrGet);
            UTF_VERIFY_EQUAL1_STRING_MAC(GT_OK, st,
                   "cpssDxChPolicerPortGroupManagementCountersGet: %d", dev);

            /* verifying values */
            isEqual = (0 == cpssOsMemCmp((GT_VOID*) &mngCntr,
                                   (GT_VOID*) &mngCntrGet,
                                    sizeof(mngCntr))) ? GT_TRUE : GT_FALSE;
            UTF_VERIFY_EQUAL1_STRING_MAC(GT_TRUE, isEqual,
                       "get another mngCntr than was set: %d", dev);

            /*
                1.1. Call function with stage [CPSS_DXCH_POLICER_STAGE_INGRESS_0_E /
                                               CPSS_DXCH_POLICER_STAGE_INGRESS_1_E /
                                               CPSS_DXCH_POLICER_STAGE_EGRESS_E],
                    mngCntrSet [CPSS_DXCH3_POLICER_MNG_CNTR_SET0_E,
                                CPSS_DXCH3_POLICER_MNG_CNTR_SET1_E,
                                CPSS_DXCH3_POLICER_MNG_CNTR_SET2_E]
                    mngCntrType [CPSS_DXCH3_POLICER_MNG_CNTR_GREEN_E,
                                 CPSS_DXCH3_POLICER_MNG_CNTR_RED_E,
                                 CPSS_DXCH3_POLICER_MNG_CNTR_DROP_E].
                    mngCntr.duMngCntr.l[0] [0 / 10 / 100],
                    mngCntr.duMngCntr.l[1] [0 / 10 / 100],
                    mngCntr.packetMngCntr  [0 / 10 / 100],
                Expected: GT_OK.
            */
            stage = CPSS_DXCH_POLICER_STAGE_INGRESS_1_E;
            mngCntrSet = CPSS_DXCH3_POLICER_MNG_CNTR_SET1_E;
            mngCntrType = CPSS_DXCH3_POLICER_MNG_CNTR_RED_E;
            mngCntr.duMngCntr.l[0] = 10;
            mngCntr.duMngCntr.l[1] = 10;
            mngCntr.packetMngCntr  = 10;
            /* CH3 has only 32 bit counters */
            if (!PRV_CPSS_DXCH_XCAT_FAMILY_CHECK_MAC(dev))
            {
                mngCntr.duMngCntr.l[1] = 0;
            }

            st = cpssDxChPolicerPortGroupManagementCountersSet(dev, portGroupsBmp,
                                        stage, mngCntrSet, mngCntrType, &mngCntr);
            if (PRV_CPSS_DXCH_XCAT_FAMILY_CHECK_MAC(dev) && !IS_STAGE_EXIST_MAC(dev, stage))
            {
                UTF_VERIFY_EQUAL1_PARAM_MAC(GT_BAD_PARAM, st, dev);
            }
            else
            {
                UTF_VERIFY_EQUAL1_PARAM_MAC(GT_OK, st, dev);
                /*
                    1.2. Call cpssDxChPolicerPortGroupManagementCountersGet
                         with the same params.
                    Expected: GT_OK and the same values.
                */
                st = cpssDxChPolicerPortGroupManagementCountersGet(dev, portGroupsBmp,
                                            stage, mngCntrSet, mngCntrType, &mngCntrGet);
                UTF_VERIFY_EQUAL1_STRING_MAC(GT_OK, st,
                       "cpssDxChPolicerPortGroupManagementCountersGet: %d", dev);

                /* verifying values */
                isEqual = (0 == cpssOsMemCmp((GT_VOID*) &mngCntr,
                                       (GT_VOID*) &mngCntrGet,
                                        sizeof(mngCntr))) ? GT_TRUE : GT_FALSE;
                UTF_VERIFY_EQUAL1_STRING_MAC(GT_TRUE, isEqual,
                           "get another mngCntr than was set: %d", dev);
            }
            /*
                1.1. Call function with stage [CPSS_DXCH_POLICER_STAGE_INGRESS_0_E /
                                               CPSS_DXCH_POLICER_STAGE_INGRESS_1_E /
                                               CPSS_DXCH_POLICER_STAGE_EGRESS_E],
                    mngCntrSet [CPSS_DXCH3_POLICER_MNG_CNTR_SET0_E,
                                CPSS_DXCH3_POLICER_MNG_CNTR_SET1_E,
                                CPSS_DXCH3_POLICER_MNG_CNTR_SET2_E]
                    mngCntrType [CPSS_DXCH3_POLICER_MNG_CNTR_GREEN_E,
                                 CPSS_DXCH3_POLICER_MNG_CNTR_RED_E,
                                 CPSS_DXCH3_POLICER_MNG_CNTR_DROP_E].
                    mngCntr.duMngCntr.l[0] [0 / 10 / 100],
                    mngCntr.duMngCntr.l[1] [0 / 10 / 100],
                    mngCntr.packetMngCntr  [0 / 10 / 100],
                Expected: GT_OK.
            */
            stage = CPSS_DXCH_POLICER_STAGE_EGRESS_E;
            mngCntrSet = CPSS_DXCH3_POLICER_MNG_CNTR_SET2_E;
            mngCntrType = CPSS_DXCH3_POLICER_MNG_CNTR_DROP_E;
            mngCntr.duMngCntr.l[0] = 100;
            mngCntr.duMngCntr.l[1] = 100;
            mngCntr.packetMngCntr  = 100;
            /* CH3 has only 32 bit counters */
            if (!PRV_CPSS_DXCH_XCAT_FAMILY_CHECK_MAC(dev))
            {
                mngCntr.duMngCntr.l[1] = 0;
            }

            st = cpssDxChPolicerPortGroupManagementCountersSet(dev, portGroupsBmp,
                                        stage, mngCntrSet, mngCntrType, &mngCntr);
            if (PRV_CPSS_DXCH_XCAT_FAMILY_CHECK_MAC(dev) && !IS_STAGE_EXIST_MAC(dev, stage))
            {
                UTF_VERIFY_EQUAL1_PARAM_MAC(GT_BAD_PARAM, st, dev);
            }
            else
            {
                UTF_VERIFY_EQUAL1_PARAM_MAC(GT_OK, st, dev);

                /*
                    1.2. Call cpssDxChPolicerPortGroupManagementCountersGet
                         with the same params.
                    Expected: GT_OK and the same values.
                */
                st = cpssDxChPolicerPortGroupManagementCountersGet(dev, portGroupsBmp,
                                            stage, mngCntrSet, mngCntrType, &mngCntrGet);
                UTF_VERIFY_EQUAL1_STRING_MAC(GT_OK, st,
                       "cpssDxChPolicerPortGroupManagementCountersGet: %d", dev);

                /* verifying values */
                isEqual = (0 == cpssOsMemCmp((GT_VOID*) &mngCntr,
                                       (GT_VOID*) &mngCntrGet,
                                        sizeof(mngCntr))) ? GT_TRUE : GT_FALSE;
                UTF_VERIFY_EQUAL1_STRING_MAC(GT_TRUE, isEqual,
                           "get another mngCntr than was set: %d", dev);
            }
            /*
                1.3. Call api with wrong stage [wrong enum values].
                Expected: GT_BAD_PARAM.
            */
            if (PRV_CPSS_DXCH_XCAT_FAMILY_CHECK_MAC(dev))
            {
                UTF_ENUMS_CHECK_MAC(cpssDxChPolicerPortGroupManagementCountersSet
                                (dev, portGroupsBmp, stage,
                                mngCntrSet, mngCntrType, &mngCntr),
                                stage);
            }
            /*
                1.4. Call api with wrong mngCntrSet [wrong enum values].
                Expected: GT_BAD_PARAM.
            */
            stage = CPSS_DXCH_POLICER_STAGE_INGRESS_0_E;
            UTF_ENUMS_CHECK_MAC(cpssDxChPolicerPortGroupManagementCountersSet
                                (dev, portGroupsBmp, stage,
                                mngCntrSet, mngCntrType, &mngCntr),
                                mngCntrSet);

            /*
                1.5. Call api with wrong mngCntrType [wrong enum values].
                Expected: GT_BAD_PARAM.
            */
            UTF_ENUMS_CHECK_MAC(cpssDxChPolicerPortGroupManagementCountersSet
                                (dev, portGroupsBmp, stage,
                                mngCntrSet, mngCntrType, &mngCntr),
                                mngCntrType);

            /*
                1.6. Call api with wrong mngCntrPtr [NULL].
                Expected: GT_BAD_PTR.
            */
            st = cpssDxChPolicerPortGroupManagementCountersSet(dev, portGroupsBmp,
                                        stage, mngCntrSet, mngCntrType, NULL);
            UTF_VERIFY_EQUAL1_PARAM_MAC(GT_BAD_PTR, st, dev);

            /* set next active port */
            portGroupsBmp = (1 << portGroupId);
        }
        PRV_CPSS_GEN_PP_END_LOOP_PORT_GROUPS_MAC(dev,portGroupId)

        /* restore port group */
        portGroupsBmp = 1;

        /* 1.2. For not-active port groups check that function returns GT_BAD_PARAM. */
        PRV_CPSS_GEN_PP_START_LOOP_NON_ACTIVE_PORT_GROUPS_MAC(dev,portGroupId)
        {
            st = cpssDxChPolicerPortGroupManagementCountersSet(dev, portGroupsBmp,
                                        stage, mngCntrSet, mngCntrType, &mngCntr);
            if(PRV_CPSS_IS_MULTI_PORT_GROUPS_DEVICE_MAC(dev))
            {
                UTF_VERIFY_EQUAL2_PARAM_MAC(GT_BAD_PARAM, st, dev, portGroupsBmp);
            }
            else
            {
                UTF_VERIFY_EQUAL2_PARAM_MAC(GT_OK, st, dev, portGroupsBmp);
            }

            /* set next non-active port */
            portGroupsBmp = (1 << portGroupId);
        }
        PRV_CPSS_GEN_PP_END_LOOP_NON_ACTIVE_PORT_GROUPS_MAC(dev,portGroupId)
    }

    /* restore correct values */
    portGroupId = 1;
    stage = CPSS_DXCH_POLICER_STAGE_INGRESS_0_E;
    mngCntrSet = CPSS_DXCH3_POLICER_MNG_CNTR_SET0_E;
    mngCntrType = CPSS_DXCH3_POLICER_MNG_CNTR_GREEN_E;


    /* 2. For not-active devices and devices from non-applicable family */
    /* check that function returns GT_BAD_PARAM.                        */

    /* prepare device iterator */
    PRV_TGF_NOT_APPLIC_DEV_RESET_MAC(&dev, UTF_CH1_E | UTF_CH1_DIAMOND_E | UTF_CH2_E);

    /* go over all non active devices */
    while(GT_OK == prvUtfNextDeviceGet(&dev, GT_FALSE))
    {
        st = cpssDxChPolicerPortGroupManagementCountersSet(dev, portGroupsBmp,
                                        stage, mngCntrSet, mngCntrType, &mngCntr);
        UTF_VERIFY_EQUAL1_PARAM_MAC(GT_NOT_APPLICABLE_DEVICE, st, dev);
    }

    /* 3. Call function with out of bound value for device id.*/
    dev = PRV_CPSS_MAX_PP_DEVICES_CNS;

    st = cpssDxChPolicerPortGroupManagementCountersSet(dev, portGroupsBmp,
                                        stage, mngCntrSet, mngCntrType, &mngCntr);
    UTF_VERIFY_EQUAL1_PARAM_MAC(GT_BAD_PARAM, st, dev);
}

/*----------------------------------------------------------------------------*/
/*
GT_STATUS cpssDxChPolicerPortGroupMeteringEntryGet
(
    IN  GT_U8                                  devNum,
    IN  GT_PORT_GROUPS_BMP                     portGroupsBmp,
    IN  CPSS_DXCH_POLICER_STAGE_TYPE_ENT       stage,
    IN  GT_U32                                 entryIndex,
    OUT CPSS_DXCH3_POLICER_METERING_ENTRY_STC  *entryPtr
)
*/
UTF_TEST_CASE_MAC(cpssDxChPolicerPortGroupMeteringEntryGet)
{
/*
    ITERATE_DEVICES_PORT_GROUPS(DxCh3 and above)
    1.1. Call function with stage [CPSS_DXCH_POLICER_STAGE_INGRESS_0_E /
                                   CPSS_DXCH_POLICER_STAGE_INGRESS_1_E /
                                   CPSS_DXCH_POLICER_STAGE_EGRESS_E],
    Expected: GT_OK.
    1.2. Call api with wrong stage [wrong enum values].
    Expected: GT_BAD_PARAM.
    1.3. Call api with wrong entryIndex [max + 1].
    Expected: NOT GT_OK.
    1.4. Call api with wrong entryPtr [NULL].
    Expected: GT_BAD_PTR.
*/
    GT_STATUS st = GT_OK;
    GT_U32    portGroupId;

    GT_U8                                  dev;
    GT_PORT_GROUPS_BMP                     portGroupsBmp = 1;
    CPSS_DXCH_POLICER_STAGE_TYPE_ENT       stage = CPSS_DXCH_POLICER_STAGE_INGRESS_0_E;
    GT_U32                                 entryIndex = 0;
    CPSS_DXCH3_POLICER_METERING_ENTRY_STC  entry;
    CPSS_DXCH3_POLICER_METERING_ENTRY_STC  entrySet;
    CPSS_DXCH3_POLICER_METER_TB_PARAMS_UNT  tbParams;

    cpssOsBzero((GT_VOID*) &tbParams, sizeof(tbParams));
    cpssOsBzero((GT_VOID*) &entrySet, sizeof(entry));
    entrySet.mngCounterSet      = CPSS_DXCH3_POLICER_MNG_CNTR_SET0_E;
    entrySet.meterColorMode     = CPSS_POLICER_COLOR_BLIND_E;
    entrySet.meterMode          = CPSS_DXCH3_POLICER_METER_MODE_SR_TCM_E;
    entrySet.modifyUp      = CPSS_PACKET_ATTRIBUTE_MODIFY_DISABLE_E;
    entrySet.modifyDscp    = CPSS_PACKET_ATTRIBUTE_MODIFY_DISABLE_E;
    entrySet.modifyDp     = CPSS_PACKET_ATTRIBUTE_MODIFY_DISABLE_E;
    entrySet.yellowPcktCmd = CPSS_DXCH3_POLICER_NON_CONFORM_CMD_NO_CHANGE_E;
    entrySet.redPcktCmd    = CPSS_DXCH3_POLICER_NON_CONFORM_CMD_NO_CHANGE_E;

    /* prepare device iterator */
    PRV_TGF_NOT_APPLIC_DEV_RESET_MAC(&dev, UTF_CH1_E | UTF_CH1_DIAMOND_E | UTF_CH2_E);

    /* 1. Go over all active devices. */
    while (GT_OK == prvUtfNextDeviceGet(&dev, GT_TRUE))
    {
        /* initialize port group */
        portGroupsBmp = 1;

        /* 1.1. Go over all active port groups. */
        PRV_CPSS_GEN_PP_START_LOOP_PORT_GROUPS_MAC(dev, portGroupId)
        {
            /*
                1.1. Call function with stage [CPSS_DXCH_POLICER_STAGE_INGRESS_0_E /
                                               CPSS_DXCH_POLICER_STAGE_INGRESS_1_E /
                                               CPSS_DXCH_POLICER_STAGE_EGRESS_E],
                Expected: GT_OK.
            */
            /*call with stage = CPSS_DXCH_POLICER_STAGE_INGRESS_0_E;*/
            stage = CPSS_DXCH_POLICER_STAGE_INGRESS_0_E;
            entryIndex = PRV_CPSS_DXCH_PP_MAC(dev)->policer.memSize[(stage)] - 1;

            /* set entry before Get to avoid random values in HW */
            st = cpssDxChPolicerPortGroupMeteringEntrySet(dev, portGroupsBmp, 
                                   stage, entryIndex, &entrySet, &tbParams);
            UTF_VERIFY_EQUAL1_PARAM_MAC(GT_OK, st, dev);

            st = cpssDxChPolicerPortGroupMeteringEntryGet(dev, portGroupsBmp,
                                   stage, entryIndex, &entry);
            UTF_VERIFY_EQUAL1_PARAM_MAC(GT_OK, st, dev);

            /*call with stage = CPSS_DXCH_POLICER_STAGE_INGRESS_1_E;*/
            stage = CPSS_DXCH_POLICER_STAGE_INGRESS_1_E;
            entryIndex = PRV_CPSS_DXCH_PP_MAC(dev)->policer.memSize[(stage)] - 1;

            /* set entry before Get to avoid random values in HW */
            st = cpssDxChPolicerPortGroupMeteringEntrySet(dev, portGroupsBmp, 
                                   stage, entryIndex, &entrySet, &tbParams);
            if (!IS_STAGE_EXIST_MAC(dev, stage))
            {
                UTF_VERIFY_EQUAL1_PARAM_MAC(GT_BAD_PARAM, st, dev);
            }
            else
                UTF_VERIFY_EQUAL1_PARAM_MAC(GT_OK, st, dev);


            st = cpssDxChPolicerPortGroupMeteringEntryGet(dev, portGroupsBmp,
                                   stage, entryIndex, &entry);
            if (!IS_STAGE_EXIST_MAC(dev, stage))
            {
                UTF_VERIFY_EQUAL1_PARAM_MAC(GT_BAD_PARAM, st, dev);
            }
            else
                UTF_VERIFY_EQUAL1_PARAM_MAC(GT_OK, st, dev);

            /*call with stage = CPSS_DXCH_POLICER_STAGE_EGRESS_E;*/
            stage = CPSS_DXCH_POLICER_STAGE_EGRESS_E;
            entryIndex = PRV_CPSS_DXCH_PP_MAC(dev)->policer.memSize[(stage)] - 1;

            /* set entry before Get to avoid random values in HW */
            st = cpssDxChPolicerPortGroupMeteringEntrySet(dev, portGroupsBmp, 
                                   stage, entryIndex, &entrySet, &tbParams);
            if (!IS_STAGE_EXIST_MAC(dev, stage))
            {
                UTF_VERIFY_EQUAL1_PARAM_MAC(GT_BAD_PARAM, st, dev);
            }
            else
                UTF_VERIFY_EQUAL1_PARAM_MAC(GT_OK, st, dev);

            st = cpssDxChPolicerPortGroupMeteringEntryGet(dev, portGroupsBmp,
                                   stage, entryIndex, &entry);
            if (!IS_STAGE_EXIST_MAC(dev, stage))
            {
                UTF_VERIFY_EQUAL1_PARAM_MAC(GT_BAD_PARAM, st, dev);
            }
            else
                UTF_VERIFY_EQUAL1_PARAM_MAC(GT_OK, st, dev);

            /*
                1.2. Call api with wrong stage [wrong enum values].
                Expected: GT_BAD_PARAM.
            */
            if (PRV_CPSS_DXCH_XCAT_FAMILY_CHECK_MAC(dev))
            {
                UTF_ENUMS_CHECK_MAC(cpssDxChPolicerPortGroupMeteringEntryGet
                                (dev, portGroupsBmp, stage,
                                entryIndex, &entry),
                                stage);
            }
            /*
                1.3. Call api with wrong entryIndex [max + 1].
                Expected: NOT GT_OK.
            */
            stage = CPSS_DXCH_POLICER_STAGE_INGRESS_0_E;
            entryIndex = PRV_CPSS_DXCH_PP_MAC(dev)->policer.memSize[(stage)] + 1;
            st = cpssDxChPolicerPortGroupMeteringEntryGet(dev, portGroupsBmp,
                                   stage, entryIndex, &entry);
            UTF_VERIFY_NOT_EQUAL1_PARAM_MAC(GT_OK, st, dev);

            entryIndex = PRV_CPSS_DXCH_PP_MAC(dev)->policer.memSize[(stage)] - 1;

            /*
                1.4. Call api with wrong entryPtr [NULL].
                Expected: GT_BAD_PTR.
            */
            st = cpssDxChPolicerPortGroupMeteringEntryGet(dev, portGroupsBmp,
                                   stage, entryIndex, NULL);
            UTF_VERIFY_EQUAL1_PARAM_MAC(GT_BAD_PTR, st, dev);

            /* set next active port */
            portGroupsBmp = (1 << portGroupId);
        }
        PRV_CPSS_GEN_PP_END_LOOP_PORT_GROUPS_MAC(dev,portGroupId)

        /* restore port group */
        portGroupsBmp = 1;

        /* 1.2. For not-active port groups check that function returns GT_BAD_PARAM. */
        PRV_CPSS_GEN_PP_START_LOOP_NON_ACTIVE_PORT_GROUPS_MAC(dev,portGroupId)
        {
            st = cpssDxChPolicerPortGroupMeteringEntryGet(dev, portGroupsBmp,
                                   stage, entryIndex, &entry);
            if(PRV_CPSS_IS_MULTI_PORT_GROUPS_DEVICE_MAC(dev))
            {
                UTF_VERIFY_EQUAL2_PARAM_MAC(GT_BAD_PARAM, st, dev, portGroupsBmp);
            }
            else
            {
                UTF_VERIFY_EQUAL2_PARAM_MAC(GT_OK, st, dev, portGroupsBmp);
            }

            /* set next non-active port */
            portGroupsBmp = (1 << portGroupId);
        }
        PRV_CPSS_GEN_PP_END_LOOP_NON_ACTIVE_PORT_GROUPS_MAC(dev,portGroupId)
    }

    /* restore correct values */
    portGroupId = 1;
    stage = CPSS_DXCH_POLICER_STAGE_INGRESS_0_E;
    entryIndex = 0x0;

    /* 2. For not-active devices and devices from non-applicable family */
    /* check that function returns GT_BAD_PARAM.                        */

    /* prepare device iterator */
    PRV_TGF_NOT_APPLIC_DEV_RESET_MAC(&dev, UTF_CH1_E | UTF_CH1_DIAMOND_E | UTF_CH2_E);

    /* go over all non active devices */
    while(GT_OK == prvUtfNextDeviceGet(&dev, GT_FALSE))
    {
        st = cpssDxChPolicerPortGroupMeteringEntryGet(dev, portGroupsBmp,
                                   stage, entryIndex, &entry);
        UTF_VERIFY_EQUAL1_PARAM_MAC(GT_NOT_APPLICABLE_DEVICE, st, dev);
    }

    /* 3. Call function with out of bound value for device id.*/
    dev = PRV_CPSS_MAX_PP_DEVICES_CNS;

    st = cpssDxChPolicerPortGroupMeteringEntryGet(dev, portGroupsBmp,
                                   stage, entryIndex, &entry);
    UTF_VERIFY_EQUAL1_PARAM_MAC(GT_BAD_PARAM, st, dev);
}

/*----------------------------------------------------------------------------*/
/*
GT_STATUS cpssDxChPolicerPortGroupMeteringEntryRefresh
(
    IN  GT_U8                            devNum,
    IN  GT_PORT_GROUPS_BMP               portGroupsBmp,
    IN  CPSS_DXCH_POLICER_STAGE_TYPE_ENT stage,
    IN  GT_U32                           entryIndex
)
*/
UTF_TEST_CASE_MAC(cpssDxChPolicerPortGroupMeteringEntryRefresh)
{
/*
    ITERATE_DEVICES_PORT_GROUPS(DxCh3 and above)
    1.1. Call function with stage [CPSS_DXCH_POLICER_STAGE_INGRESS_0_E /
                                   CPSS_DXCH_POLICER_STAGE_INGRESS_1_E /
                                   CPSS_DXCH_POLICER_STAGE_EGRESS_E],
    Expected: GT_OK.
    1.2. Call api with wrong stage [wrong enum values].
    Expected: GT_BAD_PARAM.
    1.3. Call api with wrong entryIndex [max].
    Expected: NOT GT_OK.
*/
    GT_STATUS st = GT_OK;
    GT_U32    portGroupId;

    GT_U8                            dev;
    GT_PORT_GROUPS_BMP               portGroupsBmp = 1;
    CPSS_DXCH_POLICER_STAGE_TYPE_ENT stage = CPSS_DXCH_POLICER_STAGE_INGRESS_0_E;
    GT_U32                           entryIndex = 0;


    /* prepare device iterator */
    PRV_TGF_NOT_APPLIC_DEV_RESET_MAC(&dev, UTF_CH1_E | UTF_CH1_DIAMOND_E | UTF_CH2_E);

    /* 1. Go over all active devices. */
    while (GT_OK == prvUtfNextDeviceGet(&dev, GT_TRUE))
    {
        /* initialize port group */
        portGroupsBmp = 1;

        /* 1.1. Go over all active port groups. */
        PRV_CPSS_GEN_PP_START_LOOP_PORT_GROUPS_MAC(dev, portGroupId)
        {
            /*
                1.1. Call function with stage [CPSS_DXCH_POLICER_STAGE_INGRESS_0_E /
                                               CPSS_DXCH_POLICER_STAGE_INGRESS_1_E /
                                               CPSS_DXCH_POLICER_STAGE_EGRESS_E],
                Expected: GT_OK.
            */
            /*call with stage = CPSS_DXCH_POLICER_STAGE_INGRESS_0_E;*/
            stage = CPSS_DXCH_POLICER_STAGE_INGRESS_0_E;
            entryIndex = PRV_CPSS_DXCH_PP_MAC(dev)->policer.memSize[(stage)] - 1;

            st = cpssDxChPolicerPortGroupMeteringEntryRefresh(dev, portGroupsBmp,
                                       stage, entryIndex);
            UTF_VERIFY_EQUAL1_PARAM_MAC(GT_OK, st, dev);

            /*call with stage = CPSS_DXCH_POLICER_STAGE_INGRESS_1_E;*/
            stage = CPSS_DXCH_POLICER_STAGE_INGRESS_1_E;
            entryIndex = PRV_CPSS_DXCH_PP_MAC(dev)->policer.memSize[(stage)] - 1;

            st = cpssDxChPolicerPortGroupMeteringEntryRefresh(dev, portGroupsBmp,
                                       stage, entryIndex);
            if (!IS_STAGE_EXIST_MAC(dev, stage))
            {
                UTF_VERIFY_EQUAL1_PARAM_MAC(GT_BAD_PARAM, st, dev);
            }
            else
                UTF_VERIFY_EQUAL1_PARAM_MAC(GT_OK, st, dev);

            /*call with stage = CPSS_DXCH_POLICER_STAGE_EGRESS_E;*/
            stage = CPSS_DXCH_POLICER_STAGE_EGRESS_E;
            entryIndex = PRV_CPSS_DXCH_PP_MAC(dev)->policer.memSize[(stage)] - 1;

            st = cpssDxChPolicerPortGroupMeteringEntryRefresh(dev, portGroupsBmp,
                                       stage, entryIndex);
            if (!IS_STAGE_EXIST_MAC(dev, stage))
            {
                UTF_VERIFY_EQUAL1_PARAM_MAC(GT_BAD_PARAM, st, dev);
            }
            else
                UTF_VERIFY_EQUAL1_PARAM_MAC(GT_OK, st, dev);

            /*
                1.2. Call api with wrong stage [wrong enum values].
                Expected: GT_BAD_PARAM.
            */
            if (PRV_CPSS_DXCH_XCAT_FAMILY_CHECK_MAC(dev))
            {
                UTF_ENUMS_CHECK_MAC(cpssDxChPolicerPortGroupMeteringEntryRefresh
                                (dev, portGroupsBmp, stage, entryIndex),
                                stage);
            }
            /*
                1.3. Call api with wrong entryIndex [max].
                Expected: NOT GT_OK.
            */
            stage = CPSS_DXCH_POLICER_STAGE_INGRESS_0_E;
            entryIndex = PRV_CPSS_DXCH_PP_MAC(dev)->policer.memSize[(stage)] + 1;

            st = cpssDxChPolicerPortGroupMeteringEntryRefresh(dev, portGroupsBmp,
                                       stage, entryIndex);
            UTF_VERIFY_NOT_EQUAL1_PARAM_MAC(GT_OK, st, dev);

            entryIndex = PRV_CPSS_DXCH_PP_MAC(dev)->policer.memSize[(stage)] - 1;

            /* set next active port */
            portGroupsBmp = (1 << portGroupId);
        }
        PRV_CPSS_GEN_PP_END_LOOP_PORT_GROUPS_MAC(dev,portGroupId)

        /* restore port group */
        portGroupsBmp = 1;

        /* 1.2. For not-active port groups check that function returns GT_BAD_PARAM. */
        PRV_CPSS_GEN_PP_START_LOOP_NON_ACTIVE_PORT_GROUPS_MAC(dev,portGroupId)
        {
            st = cpssDxChPolicerPortGroupMeteringEntryRefresh(dev, portGroupsBmp,
                                       stage, entryIndex);
            if(PRV_CPSS_IS_MULTI_PORT_GROUPS_DEVICE_MAC(dev))
            {
                UTF_VERIFY_EQUAL2_PARAM_MAC(GT_BAD_PARAM, st, dev, portGroupsBmp);
            }
            else
            {
                UTF_VERIFY_EQUAL2_PARAM_MAC(GT_OK, st, dev, portGroupsBmp);
            }

            /* set next non-active port */
            portGroupsBmp = (1 << portGroupId);
        }
        PRV_CPSS_GEN_PP_END_LOOP_NON_ACTIVE_PORT_GROUPS_MAC(dev,portGroupId)
    }

    /* restore correct values */
    portGroupId = 1;
    stage = CPSS_DXCH_POLICER_STAGE_INGRESS_0_E;
    entryIndex = 0x0;

    /* 2. For not-active devices and devices from non-applicable family */
    /* check that function returns GT_BAD_PARAM.                        */

    /* prepare device iterator */
    PRV_TGF_NOT_APPLIC_DEV_RESET_MAC(&dev, UTF_CH1_E | UTF_CH1_DIAMOND_E | UTF_CH2_E);

    /* go over all non active devices */
    while(GT_OK == prvUtfNextDeviceGet(&dev, GT_FALSE))
    {
        st = cpssDxChPolicerPortGroupMeteringEntryRefresh(dev, portGroupsBmp,
                                       stage, entryIndex);
        UTF_VERIFY_EQUAL1_PARAM_MAC(GT_NOT_APPLICABLE_DEVICE, st, dev);
    }

    /* 3. Call function with out of bound value for device id.*/
    dev = PRV_CPSS_MAX_PP_DEVICES_CNS;

    st = cpssDxChPolicerPortGroupMeteringEntryRefresh(dev, portGroupsBmp,
                                       stage, entryIndex);
    UTF_VERIFY_EQUAL1_PARAM_MAC(GT_BAD_PARAM, st, dev);
}

/*----------------------------------------------------------------------------*/
/*
GT_STATUS cpssDxChPolicerPortGroupMeteringEntrySet
(
    IN  GT_U8                                    devNum,
    IN  GT_PORT_GROUPS_BMP                       portGroupsBmp,
    IN  CPSS_DXCH_POLICER_STAGE_TYPE_ENT         stage,
    IN  GT_U32                                   entryIndex,
    IN  CPSS_DXCH3_POLICER_METERING_ENTRY_STC    *entryPtr,
    OUT CPSS_DXCH3_POLICER_METER_TB_PARAMS_UNT   *tbParamsPtr
)
*/
UTF_TEST_CASE_MAC(cpssDxChPolicerPortGroupMeteringEntrySet)
{
/*
    ITERATE_DEVICES_PORT_GROUPS(DxCh3 and above)
    1.1. Call function with correct params.
        stage = CPSS_DXCH_POLICER_STAGE_INGRESS_0_E;
        entryIndex = PRV_CPSS_DXCH_PP_MAC(dev)->policer.memSize[stage] - 1;
        entry.countingEntryIndex = 0;
        entry.mngCounterSet = CPSS_DXCH3_POLICER_MNG_CNTR_SET0_E;
        entry.meterColorMode = CPSS_POLICER_COLOR_BLIND_E;
        entry.meterMode = CPSS_DXCH3_POLICER_METER_MODE_SR_TCM_E;
        entry.tokenBucketParams.srTcmParams.cir = 0;
        entry.tokenBucketParams.srTcmParams.cbs = 0;
        entry.tokenBucketParams.srTcmParams.ebs = 0;
        entry.modifyUp = CPSS_PACKET_ATTRIBUTE_MODIFY_DISABLE_E;
        entry.modifyDscp = CPSS_PACKET_ATTRIBUTE_MODIFY_DISABLE_E;
        entry.modifyDp = CPSS_PACKET_ATTRIBUTE_MODIFY_DISABLE_E;
        entry.yellowPcktCmd = CPSS_DXCH3_POLICER_NON_CONFORM_CMD_NO_CHANGE_E;
        entry.redPcktCmd = CPSS_DXCH3_POLICER_NON_CONFORM_CMD_NO_CHANGE_E;
        entry.qosProfile = 0;
        entry.remarkMode = CPSS_DXCH_POLICER_REMARK_MODE_L2_E;
    Expected: GT_OK.
    1.2. Call cpssDxChPolicerPortGroupMeteringEntryGet
        with the same parameters.
    Expected: GT_OK and the same values.
    1.3. Call function with correct params.
        stage = CPSS_DXCH_POLICER_STAGE_INGRESS_1_E;
        entry.countingEntryIndex = 0;
        entry.mngCounterSet = CPSS_DXCH3_POLICER_MNG_CNTR_SET2_E;
        entry.meterColorMode = CPSS_POLICER_COLOR_AWARE_E;
        entry.meterMode = CPSS_DXCH3_POLICER_METER_MODE_TR_TCM_E;
        entry.tokenBucketParams.trTcmParams.cir = 10;
        entry.tokenBucketParams.trTcmParams.cbs = 20;
        entry.tokenBucketParams.trTcmParams.pir = 30;
        entry.tokenBucketParams.trTcmParams.pbs = 40;
        entry.modifyUp = CPSS_PACKET_ATTRIBUTE_MODIFY_ENABLE_E
        entry.modifyDscp = CPSS_PACKET_ATTRIBUTE_MODIFY_ENABLE_E;
        entry.modifyDp = CPSS_PACKET_ATTRIBUTE_MODIFY_ENABLE_E
        entry.yellowPcktCmd = CPSS_DXCH3_POLICER_NON_CONFORM_CMD_DROP_E;
        entry.redPcktCmd = CPSS_DXCH3_POLICER_NON_CONFORM_CMD_REMARK_E;
        entry.qosProfile = 10;
        entry.remarkMode = CPSS_DXCH_POLICER_REMARK_MODE_L3_E;
    Expected: GT_OK.
    1.2. Call cpssDxChPolicerPortGroupMeteringEntryGet
        with the same parameters.
    Expected: GT_OK and the same values.
    1.4. Call api with wrong stage [wrong enum values].
    Expected: GT_BAD_PARAM.
    1.5. Call api with wrong entryIndex [0].
    Expected: NOT GT_OK.
    1.6. Call api with wrong
         entry.modifyUp [CPSS_PACKET_ATTRIBUTE_MODIFY_KEEP_PREVIOUS_E].
    Expected: NOT GT_OK for xCat and above.
    1.7. Call api with wrong
         entry.modifyDscp [CPSS_PACKET_ATTRIBUTE_MODIFY_KEEP_PREVIOUS_E].
    Expected: NOT GT_OK for xCat and above.
    1.8. Call api with wrong
         entry.modifyDp [CPSS_PACKET_ATTRIBUTE_MODIFY_KEEP_PREVIOUS_E].
    Expected: NOT GT_OK for xCat and above.
    1.9. Call api with wrong
         entry.modifyDp [CPSS_PACKET_ATTRIBUTE_MODIFY_KEEP_PREVIOUS_E].
         and stage [CPSS_DXCH_POLICER_STAGE_INGRESS_0_E].
    Expected: NOT GT_OK (not supported ingress mode).
    1.10. Call api with wrong entry.mngCounterSet [wrong enum values].
    Expected: GT_BAD_PARAM.
    1.11. Call api with wrong entry.meterColorMode [wrong enum values].
    Expected: GT_BAD_PARAM.
    1.12. Call api with wrong entry.meterMode [wrong enum values].
    Expected: GT_BAD_PARAM.
    1.13. Call api with wrong entry.modifyUp [wrong enum values].
    Expected: GT_BAD_PARAM.
    1.14. Call api with wrong entry.modifyDscp [wrong enum values].
    Expected: GT_BAD_PARAM.
    1.15. Call api with wrong entry.modifyDp [wrong enum values].
    Expected: GT_BAD_PARAM for xCat and above and GT_OK for ch3.
    1.16. Call api with wrong entry.yellowPcktCmd [wrong enum values].
    Expected: GT_BAD_PARAM.
    1.17. Call api with wrong entry.redPcktCmd [wrong enum values].
    Expected: GT_BAD_PARAM.
    1.18. Call api with wrong entry.remarkMode [wrong enum values].
    Expected: GT_BAD_PARAM for xCat and above and GT_OK for ch3.
    1.19. Call api with wrong tbParamsPtr [NULL].



    Expected: GT_BAD_PTR.
*/
    GT_STATUS st = GT_OK;
    GT_U32    portGroupId;
    GT_BOOL   isEqual = GT_FALSE;

    GT_U8                                    dev;
    GT_PORT_GROUPS_BMP                       portGroupsBmp = 1;
    CPSS_DXCH_POLICER_STAGE_TYPE_ENT         stage = CPSS_DXCH_POLICER_STAGE_INGRESS_0_E;
    GT_U32                                   entryIndex = 0;
    CPSS_DXCH3_POLICER_METERING_ENTRY_STC    entry;
    CPSS_DXCH3_POLICER_METER_TB_PARAMS_UNT   tbParams;

    CPSS_DXCH3_POLICER_METERING_ENTRY_STC    entryGet;

    cpssOsBzero((GT_VOID*) &entry, sizeof(entry));
    cpssOsBzero((GT_VOID*) &entryGet, sizeof(entryGet));

    /* prepare device iterator */
    PRV_TGF_NOT_APPLIC_DEV_RESET_MAC(&dev, UTF_CH1_E | UTF_CH1_DIAMOND_E | UTF_CH2_E);

    /* 1. Go over all active devices. */
    while (GT_OK == prvUtfNextDeviceGet(&dev, GT_TRUE))
    {
        /* initialize port group */
        portGroupsBmp = 1;

        /* 1.1. Go over all active port groups. */
        PRV_CPSS_GEN_PP_START_LOOP_PORT_GROUPS_MAC(dev, portGroupId)
        {
            /*
                1.1. Call function with correct params.
                    stage = CPSS_DXCH_POLICER_STAGE_INGRESS_0_E;
                    entryIndex = PRV_CPSS_DXCH_PP_MAC(dev)->policer.memSize[stage] - 1;
                    entry.countingEntryIndex = 0;
                    entry.mngCounterSet = CPSS_DXCH3_POLICER_MNG_CNTR_SET0_E;
                    entry.meterColorMode = CPSS_POLICER_COLOR_BLIND_E;
                    entry.meterMode = CPSS_DXCH3_POLICER_METER_MODE_SR_TCM_E;
                    entry.tokenBucketParams.srTcmParams.cir = 0;
                    entry.tokenBucketParams.srTcmParams.cbs = 0;
                    entry.tokenBucketParams.srTcmParams.ebs = 0;
                    entry.modifyUp = CPSS_PACKET_ATTRIBUTE_MODIFY_DISABLE_E;
                    entry.modifyDscp = CPSS_PACKET_ATTRIBUTE_MODIFY_DISABLE_E;
                    entry.modifyDp = CPSS_PACKET_ATTRIBUTE_MODIFY_DISABLE_E;
                    entry.yellowPcktCmd = CPSS_DXCH3_POLICER_NON_CONFORM_CMD_NO_CHANGE_E;
                    entry.redPcktCmd = CPSS_DXCH3_POLICER_NON_CONFORM_CMD_NO_CHANGE_E;
                    entry.qosProfile = 0;
                    entry.remarkMode = CPSS_DXCH_POLICER_REMARK_MODE_L2_E;
                Expected: GT_OK.
            */
            stage = CPSS_DXCH_POLICER_STAGE_INGRESS_0_E;

            entryIndex = PRV_CPSS_DXCH_PP_MAC(dev)->policer.memSize[stage] - 1;

            entry.countingEntryIndex = 0;
            entry.mngCounterSet = CPSS_DXCH3_POLICER_MNG_CNTR_SET0_E;
            entry.meterColorMode = CPSS_POLICER_COLOR_BLIND_E;
            entry.meterMode = CPSS_DXCH3_POLICER_METER_MODE_SR_TCM_E;

            /* set TR parameters because comparing will be done for max structure */
            entry.tokenBucketParams.trTcmParams.pbs = 0;
            entry.tokenBucketParams.trTcmParams.pir = 0;
            entry.tokenBucketParams.trTcmParams.cir = 0;
            entry.tokenBucketParams.trTcmParams.cbs = 0;

            /* set TR Get parameters because comparing will be done for max structure */
            entryGet.tokenBucketParams.trTcmParams.pbs = 0;
            entryGet.tokenBucketParams.trTcmParams.pir = 0;
            entryGet.tokenBucketParams.trTcmParams.cir = 0;
            entryGet.tokenBucketParams.trTcmParams.cbs = 0;

            entry.modifyUp = CPSS_PACKET_ATTRIBUTE_MODIFY_DISABLE_E;
            entry.modifyDscp = CPSS_PACKET_ATTRIBUTE_MODIFY_DISABLE_E;
            /* one of the commands shouls not be NO_CHANGE because of
              CH3 erratum */
            entry.yellowPcktCmd = CPSS_DXCH3_POLICER_NON_CONFORM_CMD_REMARK_E;
            entry.redPcktCmd = CPSS_DXCH3_POLICER_NON_CONFORM_CMD_NO_CHANGE_E;
            entry.qosProfile = 0;
            entry.remarkMode = CPSS_DXCH_POLICER_REMARK_MODE_L2_E;

            /* Not relevant for ingress stage */
            entry.modifyDp = CPSS_PACKET_ATTRIBUTE_MODIFY_KEEP_PREVIOUS_E;

            st = cpssDxChPolicerPortGroupMeteringEntrySet(dev, portGroupsBmp,
                                   stage, entryIndex, &entry, &tbParams);
            UTF_VERIFY_EQUAL1_PARAM_MAC(GT_OK, st, dev);

            /*
                1.2. Call cpssDxChPolicerPortGroupMeteringEntryGet
                    with the same parameters.
                Expected: GT_OK and the same values.
            */
            st = cpssDxChPolicerPortGroupMeteringEntryGet(dev, portGroupsBmp,
                                   stage, entryIndex, &entryGet);
            UTF_VERIFY_EQUAL1_STRING_MAC(GT_OK, st,
                   "cpssDxChPolicerPortGroupMeteringEntryGet: %d", dev);

            /* verifying values */
            isEqual = (0 == cpssOsMemCmp((GT_VOID*) &entry,
                                         (GT_VOID*) &entryGet,
                                         sizeof(entry))) ? GT_TRUE : GT_FALSE;
            UTF_VERIFY_EQUAL1_STRING_MAC(GT_TRUE, isEqual,
                       "get another entry than was set: %d", dev);

            /*
                1.3. Call function with correct params.
                    stage = CPSS_DXCH_POLICER_STAGE_INGRESS_1_E;
                    entry.countingEntryIndex = 0;
                    entry.mngCounterSet = CPSS_DXCH3_POLICER_MNG_CNTR_SET2_E;
                    entry.meterColorMode = CPSS_POLICER_COLOR_AWARE_E;
                    entry.meterMode = CPSS_DXCH3_POLICER_METER_MODE_TR_TCM_E;
                    entry.tokenBucketParams.trTcmParams.cir = 10;
                    entry.tokenBucketParams.trTcmParams.cbs = 20;
                    entry.tokenBucketParams.trTcmParams.pir = 30;
                    entry.tokenBucketParams.trTcmParams.pbs = 40;
                    entry.modifyUp = CPSS_PACKET_ATTRIBUTE_MODIFY_ENABLE_E
                    entry.modifyDscp = CPSS_PACKET_ATTRIBUTE_MODIFY_ENABLE_E;
                    entry.modifyDp = CPSS_PACKET_ATTRIBUTE_MODIFY_ENABLE_E
                    entry.yellowPcktCmd = CPSS_DXCH3_POLICER_NON_CONFORM_CMD_DROP_E;
                    entry.redPcktCmd = CPSS_DXCH3_POLICER_NON_CONFORM_CMD_REMARK_E;
                    entry.qosProfile = 10;
                    entry.remarkMode = CPSS_DXCH_POLICER_REMARK_MODE_L3_E;
                Expected: GT_OK.
            */
            stage = CPSS_DXCH_POLICER_STAGE_INGRESS_1_E;
            entryIndex = PRV_CPSS_DXCH_PP_MAC(dev)->policer.memSize[stage] - 1;

            cpssOsBzero((GT_VOID*) &entry, sizeof(entry));

            entry.countingEntryIndex = 0;
            entry.mngCounterSet = CPSS_DXCH3_POLICER_MNG_CNTR_SET2_E;
            entry.meterColorMode = CPSS_POLICER_COLOR_AWARE_E;
            entry.meterMode = CPSS_DXCH3_POLICER_METER_MODE_TR_TCM_E;

            entry.tokenBucketParams.trTcmParams.cir = 10;
            entry.tokenBucketParams.trTcmParams.cbs = 20;
            entry.tokenBucketParams.trTcmParams.pir = 30;
            entry.tokenBucketParams.trTcmParams.pbs = 40;

            entry.modifyUp = CPSS_PACKET_ATTRIBUTE_MODIFY_ENABLE_E;
            entry.modifyDscp = CPSS_PACKET_ATTRIBUTE_MODIFY_ENABLE_E;

            entry.yellowPcktCmd = CPSS_DXCH3_POLICER_NON_CONFORM_CMD_DROP_E;
            entry.redPcktCmd = CPSS_DXCH3_POLICER_NON_CONFORM_CMD_REMARK_E;
            entry.qosProfile = 10;

            /* Not relevant for ingress stage */
            entry.modifyDp = CPSS_PACKET_ATTRIBUTE_MODIFY_KEEP_PREVIOUS_E;
            entry.remarkMode = CPSS_DXCH_POLICER_REMARK_MODE_L2_E;

            st = cpssDxChPolicerPortGroupMeteringEntrySet(dev, portGroupsBmp,
                                   stage, entryIndex, &entry, &tbParams);
            if (!IS_STAGE_EXIST_MAC(dev, stage))
            {
                UTF_VERIFY_EQUAL1_PARAM_MAC(GT_BAD_PARAM, st, dev);
            }
            else
            {

                UTF_VERIFY_EQUAL1_PARAM_MAC(GT_OK, st, dev);

                /*
                    1.2. Call cpssDxChPolicerPortGroupMeteringEntryGet
                        with the same parameters.
                    Expected: GT_OK and the same values.
                */
                cpssOsBzero((GT_VOID*) &entryGet, sizeof(entryGet));

                st = cpssDxChPolicerPortGroupMeteringEntryGet(dev, portGroupsBmp,
                                       stage, entryIndex, &entryGet);
                UTF_VERIFY_EQUAL1_STRING_MAC(GT_OK, st,
                       "cpssDxChPolicerPortGroupMeteringEntryGet: %d", dev);

                /* verifying values */
                isEqual = (0 == cpssOsMemCmp((GT_VOID*) &entry,
                                             (GT_VOID*) &entryGet,
                                             sizeof(entry))) ? GT_TRUE : GT_FALSE;
                UTF_VERIFY_EQUAL1_STRING_MAC(GT_TRUE, isEqual,
                           "get another entry than was set: %d", dev);
            }
            /*
                1.4. Call api with wrong stage [wrong enum values].
                Expected: GT_BAD_PARAM.
            */
            UTF_ENUMS_CHECK_MAC(cpssDxChPolicerPortGroupMeteringEntrySet
                                (dev, portGroupsBmp, stage,
                                entryIndex, &entry, &tbParams),
                                stage);

            /*
                1.5. Call api with wrong entryIndex [0].
                Expected: NOT GT_OK.
            */
            stage = CPSS_DXCH_POLICER_STAGE_INGRESS_0_E;
            entryIndex = PRV_CPSS_DXCH_PP_MAC(dev)->policer.memSize[(stage)];

            st = cpssDxChPolicerPortGroupMeteringEntrySet(dev, portGroupsBmp,
                                   stage, entryIndex, &entry, &tbParams);
            UTF_VERIFY_NOT_EQUAL1_PARAM_MAC(GT_OK, st, dev);

            entryIndex = PRV_CPSS_DXCH_PP_MAC(dev)->policer.memSize[(stage)] - 1;

            /*
                1.6. Call api with wrong
                     entry.modifyUp [CPSS_PACKET_ATTRIBUTE_MODIFY_KEEP_PREVIOUS_E].
                Expected: NOT GT_OK for xCat and above.
            */
            stage = CPSS_DXCH_POLICER_STAGE_EGRESS_E;
            entryIndex = PRV_CPSS_DXCH_PP_MAC(dev)->policer.memSize[(stage)] - 1;

            entry.modifyUp = CPSS_PACKET_ATTRIBUTE_MODIFY_KEEP_PREVIOUS_E;

            st = cpssDxChPolicerPortGroupMeteringEntrySet(dev, portGroupsBmp,
                                   stage, entryIndex, &entry, &tbParams);
            if(PRV_CPSS_PP_MAC(dev)->devFamily >= CPSS_PP_FAMILY_DXCH_XCAT_E)
            {
                UTF_VERIFY_NOT_EQUAL1_PARAM_MAC(GT_OK, st, dev);
            }
            else
            {
                /* wrong index */
                UTF_VERIFY_EQUAL1_PARAM_MAC(GT_BAD_PARAM, st, dev);
            }

            entry.modifyUp = CPSS_PACKET_ATTRIBUTE_MODIFY_ENABLE_E;

            /*
                1.7. Call api with wrong
                     entry.modifyDscp [CPSS_PACKET_ATTRIBUTE_MODIFY_KEEP_PREVIOUS_E].
                Expected: NOT GT_OK for xCat and above.
            */
            entry.modifyDscp = CPSS_PACKET_ATTRIBUTE_MODIFY_KEEP_PREVIOUS_E;

            st = cpssDxChPolicerPortGroupMeteringEntrySet(dev, portGroupsBmp,
                                   stage, entryIndex, &entry, &tbParams);
            if(PRV_CPSS_PP_MAC(dev)->devFamily >= CPSS_PP_FAMILY_DXCH_XCAT_E)
            {
                UTF_VERIFY_NOT_EQUAL1_PARAM_MAC(GT_OK, st, dev);
            }
            else
            {
                /* wrong index */
                UTF_VERIFY_EQUAL1_PARAM_MAC(GT_BAD_PARAM, st, dev);
            }

            entry.modifyDscp = CPSS_PACKET_ATTRIBUTE_MODIFY_ENABLE_E;

            /*
                1.8. Call api with wrong
                     entry.modifyDp [CPSS_PACKET_ATTRIBUTE_MODIFY_KEEP_PREVIOUS_E].
                Expected: NOT GT_OK for xCat and above.
            */
            entry.modifyDp = CPSS_PACKET_ATTRIBUTE_MODIFY_KEEP_PREVIOUS_E;

            st = cpssDxChPolicerPortGroupMeteringEntrySet(dev, portGroupsBmp,
                                   stage, entryIndex, &entry, &tbParams);
            if(PRV_CPSS_PP_MAC(dev)->devFamily >= CPSS_PP_FAMILY_DXCH_XCAT_E)
            {
                UTF_VERIFY_NOT_EQUAL1_PARAM_MAC(GT_OK, st, dev);
            }
            else
            {
                /* wrong index */
                UTF_VERIFY_EQUAL1_PARAM_MAC(GT_BAD_PARAM, st, dev);
            }

            entry.modifyDp = CPSS_PACKET_ATTRIBUTE_MODIFY_ENABLE_E;

            /*
                1.9. Call api with wrong
                     entry.modifyDp [CPSS_PACKET_ATTRIBUTE_MODIFY_KEEP_PREVIOUS_E].
                     and stage [CPSS_DXCH_POLICER_STAGE_EGRESS_E].
                Expected: NOT GT_OK (not supported egress mode).
            */
            stage = CPSS_DXCH_POLICER_STAGE_EGRESS_E;
            entryIndex = PRV_CPSS_DXCH_PP_MAC(dev)->policer.memSize[(stage)] - 1;

            if(PRV_CPSS_PP_MAC(dev)->devFamily >= CPSS_PP_FAMILY_DXCH_XCAT_E)
            {
                st = cpssDxChPolicerPortGroupMeteringEntrySet(dev, portGroupsBmp,
                                       stage, entryIndex, &entry, &tbParams);
                UTF_VERIFY_NOT_EQUAL1_PARAM_MAC(GT_OK, st, dev);
            }

            stage = CPSS_DXCH_POLICER_STAGE_INGRESS_1_E;
            entryIndex = PRV_CPSS_DXCH_PP_MAC(dev)->policer.memSize[(stage)] - 1;


            /*
                1.10. Call api with wrong entry.mngCounterSet [wrong enum values].
                Expected: GT_BAD_PARAM.
            */
            UTF_ENUMS_CHECK_MAC(cpssDxChPolicerPortGroupMeteringEntrySet
                                (dev, portGroupsBmp, stage,
                                entryIndex, &entry, &tbParams),
                                entry.mngCounterSet);

            /*
                1.11. Call api with wrong entry.meterColorMode [wrong enum values].
                Expected: GT_BAD_PARAM.
            */
            UTF_ENUMS_CHECK_MAC(cpssDxChPolicerPortGroupMeteringEntrySet
                                (dev, portGroupsBmp, stage,
                                entryIndex, &entry, &tbParams),
                                entry.meterColorMode);

            /*
                1.12. Call api with wrong entry.meterMode [wrong enum values].
                Expected: GT_BAD_PARAM.
            */
            UTF_ENUMS_CHECK_MAC(cpssDxChPolicerPortGroupMeteringEntrySet
                                (dev, portGroupsBmp, stage,
                                entryIndex, &entry, &tbParams),
                                entry.meterMode);

            /*
                1.13. Call api with wrong entry.modifyUp [wrong enum values].
                Expected: GT_BAD_PARAM.
            */
            UTF_ENUMS_CHECK_MAC(cpssDxChPolicerPortGroupMeteringEntrySet
                                (dev, portGroupsBmp, stage,
                                entryIndex, &entry, &tbParams),
                                entry.modifyUp);

            /*
                1.14. Call api with wrong entry.modifyDscp [wrong enum values].
                Expected: GT_BAD_PARAM.
            */
            UTF_ENUMS_CHECK_MAC(cpssDxChPolicerPortGroupMeteringEntrySet
                                (dev, portGroupsBmp, stage,
                                entryIndex, &entry, &tbParams),
                                entry.modifyDscp);

            /*
                1.15. Call api with wrong entry.modifyDp [wrong enum values].
                Expected: GT_BAD_PARAM for xCat and above and GT_OK for ch3.
            */
            stage = CPSS_DXCH_POLICER_STAGE_EGRESS_E;
            entryIndex = PRV_CPSS_DXCH_PP_MAC(dev)->policer.memSize[(stage)] - 1;

            if(PRV_CPSS_PP_MAC(dev)->devFamily >= CPSS_PP_FAMILY_DXCH_XCAT_E)
            {
                enumsIndex = 0;
                tempParamValue = entry.modifyDp;

                for(enumsIndex = 0; enumsIndex < utfInvalidEnumArrSize; enumsIndex++)
                {
                    entry.modifyDp = utfInvalidEnumArr[enumsIndex];

                    if(PRV_CPSS_PP_MAC(dev)->devFamily >= CPSS_PP_FAMILY_DXCH_XCAT_E)
                    {
                        st = cpssDxChPolicerPortGroupMeteringEntrySet(dev, portGroupsBmp,
                                                   stage, entryIndex, &entry, &tbParams);

                        UTF_VERIFY_EQUAL1_STRING_MAC(GT_BAD_PARAM, st,
                                        "Fail on wrong enum value: %d", entry.modifyDp);
                    }
                }
                entry.modifyDp = tempParamValue;
            }

            stage = CPSS_DXCH_POLICER_STAGE_INGRESS_1_E;
            entryIndex = PRV_CPSS_DXCH_PP_MAC(dev)->policer.memSize[(stage)] - 1;



            /*
                1.16. Call api with wrong entry.yellowPcktCmd [wrong enum values].
                Expected: GT_BAD_PARAM.
            */
            UTF_ENUMS_CHECK_MAC(cpssDxChPolicerPortGroupMeteringEntrySet
                                (dev, portGroupsBmp, stage,
                                entryIndex, &entry, &tbParams),
                                entry.yellowPcktCmd);

            /*
                1.17. Call api with wrong entry.redPcktCmd [wrong enum values].
                Expected: GT_BAD_PARAM.
            */
            UTF_ENUMS_CHECK_MAC(cpssDxChPolicerPortGroupMeteringEntrySet
                                (dev, portGroupsBmp, stage,
                                entryIndex, &entry, &tbParams),
                                entry.redPcktCmd);

            /*
                1.18. Call api with wrong entry.remarkMode [wrong enum values].
                Expected: GT_BAD_PARAM for xCat and above and GT_OK for ch3.
            */
            stage = CPSS_DXCH_POLICER_STAGE_EGRESS_E;
            entryIndex = PRV_CPSS_DXCH_PP_MAC(dev)->policer.memSize[(stage)] - 1;

            if(PRV_CPSS_PP_MAC(dev)->devFamily >= CPSS_PP_FAMILY_DXCH_XCAT_E)
            {
                enumsIndex = 0;
                tempParamValue = entry.remarkMode;

                for(enumsIndex = 0; enumsIndex < utfInvalidEnumArrSize; enumsIndex++)
                {
                    entry.remarkMode = utfInvalidEnumArr[enumsIndex];

                    if(PRV_CPSS_PP_MAC(dev)->devFamily >= CPSS_PP_FAMILY_DXCH_XCAT_E)
                    {
                        st = cpssDxChPolicerPortGroupMeteringEntrySet(dev, portGroupsBmp,
                                                   stage, entryIndex, &entry, &tbParams);

                        UTF_VERIFY_EQUAL1_STRING_MAC(GT_BAD_PARAM, st,
                                        "Fail on wrong enum value: %d", entry.remarkMode);
                    }
                }


                entry.remarkMode = tempParamValue;
            }

            /*
                1.19. Call api with wrong tbParamsPtr [NULL].
                Expected: GT_BAD_PTR.
            */
            st = cpssDxChPolicerPortGroupMeteringEntrySet(dev, portGroupsBmp,
                                   stage, entryIndex, &entry, NULL);
            UTF_VERIFY_EQUAL1_PARAM_MAC(GT_BAD_PTR, st, dev);

            /* set next active port */
            portGroupsBmp = (1 << portGroupId);
        }
        PRV_CPSS_GEN_PP_END_LOOP_PORT_GROUPS_MAC(dev,portGroupId)

        /* restore port group */
        portGroupsBmp = 1;
        stage = CPSS_DXCH_POLICER_STAGE_INGRESS_0_E;
        entryIndex = PRV_CPSS_DXCH_PP_MAC(dev)->policer.memSize[(stage)] - 1;


        /* 1.2. For not-active port groups check that function returns GT_BAD_PARAM. */
        PRV_CPSS_GEN_PP_START_LOOP_NON_ACTIVE_PORT_GROUPS_MAC(dev,portGroupId)
        {
            st = cpssDxChPolicerPortGroupMeteringEntrySet(dev, portGroupsBmp,
                                   stage, entryIndex, &entry, &tbParams);
            if(PRV_CPSS_IS_MULTI_PORT_GROUPS_DEVICE_MAC(dev))
            {
                UTF_VERIFY_EQUAL2_PARAM_MAC(GT_BAD_PARAM, st, dev, portGroupsBmp);
            }
            else
            {
                UTF_VERIFY_EQUAL2_PARAM_MAC(GT_OK, st, dev, portGroupsBmp);
            }

            /* set next non-active port */
            portGroupsBmp = (1 << portGroupId);
        }
        PRV_CPSS_GEN_PP_END_LOOP_NON_ACTIVE_PORT_GROUPS_MAC(dev,portGroupId)
    }

    /* restore correct values */
    portGroupId = 1;
    stage = CPSS_DXCH_POLICER_STAGE_INGRESS_0_E;
    entryIndex = 0;

    entry.countingEntryIndex = 0;
    entry.mngCounterSet = CPSS_DXCH3_POLICER_MNG_CNTR_SET0_E;
    entry.meterColorMode = CPSS_POLICER_COLOR_BLIND_E;
    entry.meterMode = CPSS_DXCH3_POLICER_METER_MODE_SR_TCM_E;
    entry.modifyUp = CPSS_PACKET_ATTRIBUTE_MODIFY_KEEP_PREVIOUS_E;
    entry.modifyDscp = CPSS_PACKET_ATTRIBUTE_MODIFY_KEEP_PREVIOUS_E;
    entry.modifyDp = CPSS_PACKET_ATTRIBUTE_MODIFY_KEEP_PREVIOUS_E;
    entry.yellowPcktCmd = CPSS_DXCH3_POLICER_NON_CONFORM_CMD_NO_CHANGE_E;
    entry.redPcktCmd = CPSS_DXCH3_POLICER_NON_CONFORM_CMD_NO_CHANGE_E;
    entry.qosProfile = 0;
    entry.remarkMode = CPSS_DXCH_POLICER_REMARK_MODE_L2_E;

    /* 2. For not-active devices and devices from non-applicable family */
    /* check that function returns GT_BAD_PARAM.                        */

    /* prepare device iterator */
    PRV_TGF_NOT_APPLIC_DEV_RESET_MAC(&dev, UTF_CH1_E | UTF_CH1_DIAMOND_E | UTF_CH2_E);

    /* go over all non active devices */
    while(GT_OK == prvUtfNextDeviceGet(&dev, GT_FALSE))
    {
        st = cpssDxChPolicerPortGroupMeteringEntrySet(dev, portGroupsBmp,
                                   stage, entryIndex, &entry, &tbParams);
        UTF_VERIFY_EQUAL1_PARAM_MAC(GT_NOT_APPLICABLE_DEVICE, st, dev);
    }

    /* 3. Call function with out of bound value for device id.*/
    dev = PRV_CPSS_MAX_PP_DEVICES_CNS;

    st = cpssDxChPolicerPortGroupMeteringEntrySet(dev, portGroupsBmp,
                                   stage, entryIndex, &entry, &tbParams);
    UTF_VERIFY_EQUAL1_PARAM_MAC(GT_BAD_PARAM, st, dev);
}

/*----------------------------------------------------------------------------*/
/*
GT_STATUS cpssDxChPolicerPortGroupPolicyCntrGet
(
    IN  GT_U8                             devNum,
    IN  GT_PORT_GROUPS_BMP                portGroupsBmp,
    IN  CPSS_DXCH_POLICER_STAGE_TYPE_ENT  stage,
    IN  GT_U32                            index,
    OUT GT_U32                            *cntrValuePtr
)
*/
UTF_TEST_CASE_MAC(cpssDxChPolicerPortGroupPolicyCntrGet)
{
/*
    ITERATE_DEVICES_PORT_GROUPS(DxChXcat and above)
    1.1. Call function with stage [CPSS_DXCH_POLICER_STAGE_INGRESS_0_E
                                   CPSS_DXCH_POLICER_STAGE_INGRESS_1_E /
                                   CPSS_DXCH_POLICER_STAGE_EGRESS_E],
                and index [5 / 18 / 21].
    Expected: GT_OK.
    1.2. Call api with wrong stage [wrong enum values].
    Expected: GT_BAD_PARAM.
    1.3. Call api with wrong index [6].
    Expected: NOT GT_OK.
    1.4. Call api with wrong cntrValuePtr [NULL].
    Expected: GT_BAD_PTR.
*/
    GT_STATUS st = GT_OK;
    GT_U32    portGroupId;

    GT_U8                             dev;
    GT_PORT_GROUPS_BMP                portGroupsBmp = 1;
    CPSS_DXCH_POLICER_STAGE_TYPE_ENT  stage = CPSS_DXCH_POLICER_STAGE_INGRESS_0_E;
    GT_U32                            index = 0;
    GT_U32                            cntrValue = 0;

    /* prepare device iterator */
    PRV_TGF_NOT_APPLIC_DEV_RESET_MAC(&dev, UTF_CH1_E | UTF_CH1_DIAMOND_E | UTF_CH2_E | UTF_CH3_E);

    /* 1. Go over all active devices. */
    while (GT_OK == prvUtfNextDeviceGet(&dev, GT_TRUE))
    {
        /* initialize port group */
        portGroupsBmp = 1;

        /* 1.1. Go over all active port groups. */
        PRV_CPSS_GEN_PP_START_LOOP_PORT_GROUPS_MAC(dev, portGroupId)
        {
            /*
                1.1. Call function with stage [CPSS_DXCH_POLICER_STAGE_INGRESS_0_E
                                               CPSS_DXCH_POLICER_STAGE_INGRESS_1_E /
                                               CPSS_DXCH_POLICER_STAGE_EGRESS_E],
                            and index [5 / 18 / 21].
                Expected: GT_OK.
            */
            stage = CPSS_DXCH_POLICER_STAGE_INGRESS_0_E;

            if (PRV_CPSS_PP_MAC(dev)->devFamily == CPSS_PP_FAMILY_DXCH_XCAT2_E)
            {
                /* indexes supported are 0,1,2 and 3  (index & 0x7) */
                index = 3;
            }
            else
            {
                index = 5;
            }

            st = cpssDxChPolicerPortGroupPolicyCntrGet(dev, portGroupsBmp,
                                stage, index, &cntrValue);
            UTF_VERIFY_EQUAL1_PARAM_MAC(GT_OK, st, dev);

            /*
                1.1. Call function with stage [CPSS_DXCH_POLICER_STAGE_INGRESS_0_E
                                               CPSS_DXCH_POLICER_STAGE_INGRESS_1_E /
                                               CPSS_DXCH_POLICER_STAGE_EGRESS_E],
                            and index [5 / 18 / 21].
                Expected: GT_OK.
            */
            stage = CPSS_DXCH_POLICER_STAGE_INGRESS_1_E;
            index = 18;

            st = cpssDxChPolicerPortGroupPolicyCntrGet(dev, portGroupsBmp,
                                stage, index, &cntrValue);
            UTF_VERIFY_EQUAL1_PARAM_MAC(GT_OK, st, dev);
            /*
                1.1. Call function with stage [CPSS_DXCH_POLICER_STAGE_INGRESS_0_E
                                               CPSS_DXCH_POLICER_STAGE_INGRESS_1_E /
                                               CPSS_DXCH_POLICER_STAGE_EGRESS_E],
                            and index [5 / 18 / 21].
                Expected: GT_OK.
            */
            stage = CPSS_DXCH_POLICER_STAGE_EGRESS_E;

            if (PRV_CPSS_PP_MAC(dev)->devFamily == CPSS_PP_FAMILY_DXCH_XCAT2_E)
            {
                /* indexes supported are 0,1,2 and 3  (index & 0x7) */
                index = 19;
            }
            else
            {
                index = 21;
            }

            st = cpssDxChPolicerPortGroupPolicyCntrGet(dev, portGroupsBmp,
                                stage, index, &cntrValue);
            UTF_VERIFY_EQUAL1_PARAM_MAC(GT_OK, st, dev);


            /*
                1.2. Call api with wrong stage [wrong enum values].
                Expected: GT_BAD_PARAM.
            */
            UTF_ENUMS_CHECK_MAC(cpssDxChPolicerPortGroupPolicyCntrGet
                                (dev, portGroupsBmp, stage,
                                index, &cntrValue),
                                stage);

            /*
                1.3. Call api with wrong index [6].
                Expected: NOT GT_OK.
            */
            index = 6;
            st = cpssDxChPolicerPortGroupPolicyCntrGet(dev, portGroupsBmp,
                                stage, index, &cntrValue);
            UTF_VERIFY_NOT_EQUAL1_PARAM_MAC(GT_OK, st, dev);

            /*
                1.4. Call api with wrong cntrValuePtr [NULL].
                Expected: GT_BAD_PTR.
            */
            st = cpssDxChPolicerPortGroupPolicyCntrGet(dev, portGroupsBmp,
                                stage, index, NULL);
            UTF_VERIFY_EQUAL1_PARAM_MAC(GT_BAD_PTR, st, dev);

            /* set next active port */
            portGroupsBmp = (1 << portGroupId);
        }
        PRV_CPSS_GEN_PP_END_LOOP_PORT_GROUPS_MAC(dev,portGroupId)

        /* restore port group */
        portGroupsBmp = 1;
        stage = CPSS_DXCH_POLICER_STAGE_INGRESS_0_E;


        if (PRV_CPSS_PP_MAC(dev)->devFamily == CPSS_PP_FAMILY_DXCH_XCAT2_E)
        {
            /* indexes supported are 0,1,2 and 3  (index & 0x7) */
            index = 3;
        }
        else
        {
            index = 5;
        }

        /* 1.2. For not-active port groups check that function returns GT_BAD_PARAM. */
        PRV_CPSS_GEN_PP_START_LOOP_NON_ACTIVE_PORT_GROUPS_MAC(dev,portGroupId)
        {
            st = cpssDxChPolicerPortGroupPolicyCntrGet(dev, portGroupsBmp,
                                stage, index, &cntrValue);
            if(PRV_CPSS_IS_MULTI_PORT_GROUPS_DEVICE_MAC(dev))
            {
                UTF_VERIFY_EQUAL2_PARAM_MAC(GT_BAD_PARAM, st, dev, portGroupsBmp);
            }
            else
            {
                UTF_VERIFY_EQUAL2_PARAM_MAC(GT_OK, st, dev, portGroupsBmp);
            }

            /* set next non-active port */
            portGroupsBmp = (1 << portGroupId);
        }
        PRV_CPSS_GEN_PP_END_LOOP_NON_ACTIVE_PORT_GROUPS_MAC(dev,portGroupId)
    }

    /* restore correct values */
    portGroupId = 1;
    stage = CPSS_DXCH_POLICER_STAGE_INGRESS_0_E;
    index = 0;

    /* 2. For not-active devices and devices from non-applicable family */
    /* check that function returns GT_BAD_PARAM.                        */

    /* prepare device iterator */
    PRV_TGF_NOT_APPLIC_DEV_RESET_MAC(&dev, UTF_CH1_E | UTF_CH1_DIAMOND_E | UTF_CH2_E | UTF_CH3_E);

    /* go over all non active devices */
    while(GT_OK == prvUtfNextDeviceGet(&dev, GT_FALSE))
    {
        st = cpssDxChPolicerPortGroupPolicyCntrGet(dev, portGroupsBmp,
                                stage, index, &cntrValue);
        UTF_VERIFY_EQUAL1_PARAM_MAC(GT_NOT_APPLICABLE_DEVICE, st, dev);
    }

    /* 3. Call function with out of bound value for device id.*/
    dev = PRV_CPSS_MAX_PP_DEVICES_CNS;

    st = cpssDxChPolicerPortGroupPolicyCntrGet(dev, portGroupsBmp,
                                stage, index, &cntrValue);
    UTF_VERIFY_EQUAL1_PARAM_MAC(GT_BAD_PARAM, st, dev);
}


/*----------------------------------------------------------------------------*/
/*
GT_STATUS cpssDxChPolicerPortGroupPolicyCntrSet
(
    IN  GT_U8                            devNum,
    IN  GT_PORT_GROUPS_BMP               portGroupsBmp,
    IN  CPSS_DXCH_POLICER_STAGE_TYPE_ENT stage,
    IN  GT_U32                           index,
    IN  GT_U32                           cntrValue
)
*/
UTF_TEST_CASE_MAC(cpssDxChPolicerPortGroupPolicyCntrSet)
{
/*
    ITERATE_DEVICES_PORT_GROUPS(DxChXcat and above)
    1.1. Call function with stage [CPSS_DXCH_POLICER_STAGE_INGRESS_0_E
                                   CPSS_DXCH_POLICER_STAGE_INGRESS_1_E /
                                   CPSS_DXCH_POLICER_STAGE_EGRESS_E],
                and index [5 / 18 / 21], cntrValue [0 / 10 / 100].
    Expected: GT_OK.
    1.2. Call cpssDxChPolicerPortGroupPolicyCntrGet
         with the same params.
    Expected: GT_OK and the same values.
    1.3. Call api with wrong stage [wrong enum values].
    Expected: GT_BAD_PARAM.
    1.4. Call api with wrong index [6].
    Expected: NOT GT_OK.
*/
    GT_STATUS st = GT_OK;
    GT_U32    portGroupId;

    GT_U8                            dev;
    GT_PORT_GROUPS_BMP               portGroupsBmp = 1;
    CPSS_DXCH_POLICER_STAGE_TYPE_ENT stage = CPSS_DXCH_POLICER_STAGE_INGRESS_0_E;
    GT_U32                           index = 0;
    GT_U32                           cntrValue = 0;
    GT_U32                           cntrValueGet = 0;

    /* prepare device iterator */
    PRV_TGF_NOT_APPLIC_DEV_RESET_MAC(&dev, UTF_CH1_E | UTF_CH1_DIAMOND_E | UTF_CH2_E | UTF_CH3_E);

    /* 1. Go over all active devices. */
    while (GT_OK == prvUtfNextDeviceGet(&dev, GT_TRUE))
    {
        /* initialize port group */
        portGroupsBmp = 1;

        /* 1.1. Go over all active port groups. */
        PRV_CPSS_GEN_PP_START_LOOP_PORT_GROUPS_MAC(dev, portGroupId)
        {
            /*
                1.1. Call function with stage [CPSS_DXCH_POLICER_STAGE_INGRESS_0_E
                                               CPSS_DXCH_POLICER_STAGE_INGRESS_1_E /
                                               CPSS_DXCH_POLICER_STAGE_EGRESS_E],
                            and index [5 / 18 / 21], cntrValue [0 / 10 / 100].
                Expected: GT_OK.
            */
            stage = CPSS_DXCH_POLICER_STAGE_INGRESS_0_E;
            cntrValue = 0;

            if (PRV_CPSS_PP_MAC(dev)->devFamily == CPSS_PP_FAMILY_DXCH_XCAT2_E)
            {
                /* indexes supported are 0,1,2 and 3  (index & 0x7) */
                index = 3;
            }
            else
            {
                index = 5;
            }

            st = cpssDxChPolicerPortGroupPolicyCntrSet(dev, portGroupsBmp,
                                stage, index, cntrValue);
            UTF_VERIFY_EQUAL1_PARAM_MAC(GT_OK, st, dev);

            /*
                1.2. Call cpssDxChPolicerPortGroupPolicyCntrGet
                     with the same params.
                Expected: GT_OK and the same values.
            */
            st = cpssDxChPolicerPortGroupPolicyCntrGet(dev, portGroupsBmp,
                                stage, index, &cntrValueGet);
            UTF_VERIFY_EQUAL1_STRING_MAC(GT_OK, st,
                   "cpssDxChPolicerPortGroupPolicyCntrGet: %d", dev);

            /* verifying values */
            UTF_VERIFY_EQUAL1_STRING_MAC(cntrValue, cntrValueGet,
                       "got another cntrValue then was set: %d", dev);

            /*
                1.1. Call function with stage [CPSS_DXCH_POLICER_STAGE_INGRESS_0_E
                                               CPSS_DXCH_POLICER_STAGE_INGRESS_1_E /
                                               CPSS_DXCH_POLICER_STAGE_EGRESS_E],
                            and index [5 / 18 / 21], cntrValue [0 / 10 / 100].
                Expected: GT_OK.
            */
            stage = CPSS_DXCH_POLICER_STAGE_INGRESS_1_E;
            index = 18;
            cntrValue = 10;

            st = cpssDxChPolicerPortGroupPolicyCntrSet(dev, portGroupsBmp,
                                stage, index, cntrValue);
            UTF_VERIFY_EQUAL1_PARAM_MAC(GT_OK, st, dev);

            /*
                1.2. Call cpssDxChPolicerPortGroupPolicyCntrGet
                     with the same params.
                Expected: GT_OK and the same values.
            */
            st = cpssDxChPolicerPortGroupPolicyCntrGet(dev, portGroupsBmp,
                                stage, index, &cntrValueGet);
            UTF_VERIFY_EQUAL1_STRING_MAC(GT_OK, st,
                   "cpssDxChPolicerPortGroupPolicyCntrGet: %d", dev);

            /* verifying values */
            UTF_VERIFY_EQUAL1_STRING_MAC(cntrValue, cntrValueGet,
                       "got another cntrValue then was set: %d", dev);

            /*
                1.1. Call function with stage [CPSS_DXCH_POLICER_STAGE_INGRESS_0_E
                                               CPSS_DXCH_POLICER_STAGE_INGRESS_1_E /
                                               CPSS_DXCH_POLICER_STAGE_EGRESS_E],
                            and index [5 / 18 / 21], cntrValue [0 / 10 / 100].
                Expected: GT_OK.
            */
            stage = CPSS_DXCH_POLICER_STAGE_EGRESS_E;
            cntrValue = 100;

            if (PRV_CPSS_PP_MAC(dev)->devFamily == CPSS_PP_FAMILY_DXCH_XCAT2_E)
            {
                /* indexes supported are 0,1,2 and 3  (index & 0x7) */
                index = 19;
            }
            else
            {
                index = 21;
            }

            st = cpssDxChPolicerPortGroupPolicyCntrSet(dev, portGroupsBmp,
                                stage, index, cntrValue);
            UTF_VERIFY_EQUAL1_PARAM_MAC(GT_OK, st, dev);

            /*
                1.2. Call cpssDxChPolicerPortGroupPolicyCntrGet
                     with the same params.
                Expected: GT_OK and the same values.
            */
            st = cpssDxChPolicerPortGroupPolicyCntrGet(dev, portGroupsBmp,
                                stage, index, &cntrValueGet);
            UTF_VERIFY_EQUAL1_STRING_MAC(GT_OK, st,
                   "cpssDxChPolicerPortGroupPolicyCntrGet: %d", dev);

            /* verifying values */
            UTF_VERIFY_EQUAL1_STRING_MAC(cntrValue, cntrValueGet,
                       "got another cntrValue then was set: %d", dev);

            /*
                1.3. Call api with wrong stage [wrong enum values].
                Expected: GT_BAD_PARAM.
            */
            UTF_ENUMS_CHECK_MAC(cpssDxChPolicerPortGroupPolicyCntrSet
                                (dev, portGroupsBmp, stage,
                                index, cntrValue),
                                stage);

            /*
                1.4. Call api with wrong index [6].
                Expected: NOT GT_OK.
            */
            index = 6;
            st = cpssDxChPolicerPortGroupPolicyCntrSet(dev, portGroupsBmp,
                                stage, index, cntrValue);
            UTF_VERIFY_NOT_EQUAL1_PARAM_MAC(GT_OK, st, dev);

            /* set next active port */
            portGroupsBmp = (1 << portGroupId);
        }
        PRV_CPSS_GEN_PP_END_LOOP_PORT_GROUPS_MAC(dev,portGroupId)

        /* restore port group */
        portGroupsBmp = 1;
        stage = CPSS_DXCH_POLICER_STAGE_INGRESS_0_E;

        if (PRV_CPSS_PP_MAC(dev)->devFamily == CPSS_PP_FAMILY_DXCH_XCAT2_E)
        {
            /* indexes supported are 0,1,2 and 3  (index & 0x7) */
            index = 3;
        }
        else
        {
            index = 5;
        }

        /* 1.2. For not-active port groups check that function returns GT_BAD_PARAM. */
        PRV_CPSS_GEN_PP_START_LOOP_NON_ACTIVE_PORT_GROUPS_MAC(dev,portGroupId)
        {
            st = cpssDxChPolicerPortGroupPolicyCntrSet(dev, portGroupsBmp,
                                stage, index, cntrValue);
            if(PRV_CPSS_IS_MULTI_PORT_GROUPS_DEVICE_MAC(dev))
            {
                UTF_VERIFY_EQUAL2_PARAM_MAC(GT_BAD_PARAM, st, dev, portGroupsBmp);
            }
            else
            {
                UTF_VERIFY_EQUAL2_PARAM_MAC(GT_OK, st, dev, portGroupsBmp);
            }

            /* set next non-active port */
            portGroupsBmp = (1 << portGroupId);
        }
        PRV_CPSS_GEN_PP_END_LOOP_NON_ACTIVE_PORT_GROUPS_MAC(dev,portGroupId)
    }

    /* restore correct values */
    portGroupId = 1;
    stage = CPSS_DXCH_POLICER_STAGE_INGRESS_0_E;
    index = 0;
    cntrValue = 0;

    /* 2. For not-active devices and devices from non-applicable family */
    /* check that function returns GT_BAD_PARAM.                        */

    /* prepare device iterator */
    PRV_TGF_NOT_APPLIC_DEV_RESET_MAC(&dev, UTF_CH1_E | UTF_CH1_DIAMOND_E | UTF_CH2_E | UTF_CH3_E);

    /* go over all non active devices */
    while(GT_OK == prvUtfNextDeviceGet(&dev, GT_FALSE))
    {
        st = cpssDxChPolicerPortGroupPolicyCntrSet(dev, portGroupsBmp,
                                stage, index, cntrValue);
        UTF_VERIFY_EQUAL1_PARAM_MAC(GT_NOT_APPLICABLE_DEVICE, st, dev);
    }

    /* 3. Call function with out of bound value for device id.*/
    dev = PRV_CPSS_MAX_PP_DEVICES_CNS;

    st = cpssDxChPolicerPortGroupPolicyCntrSet(dev, portGroupsBmp,
                                stage, index, cntrValue);
    UTF_VERIFY_EQUAL1_PARAM_MAC(GT_BAD_PARAM, st, dev);
}

/*----------------------------------------------------------------------------*/
/*
GT_STATUS cpssDxChPolicerPortStormTypeIndexGet
(
    IN  GT_U8                             devNum,
    IN  CPSS_DXCH_POLICER_STAGE_TYPE_ENT  stage,
    IN  GT_U8                             portNum,
    IN  CPSS_DXCH_POLICER_STORM_TYPE_ENT  stormType,
    OUT GT_U32                            *indexPtr
)
*/
UTF_TEST_CASE_MAC(cpssDxChPolicerPortStormTypeIndexGet)
{
/*
    ITERATE_DEVICES_PHY_CPU_PORTS(DxChXcat and above)
    1.1.1. Call function with stage [CPSS_DXCH_POLICER_STAGE_INGRESS_0_E /
                                   CPSS_DXCH_POLICER_STAGE_INGRESS_1_E /
                                   CPSS_DXCH_POLICER_STAGE_EGRESS_E],
            portNum [0 / 4 / CPSS_CPU_PORT_NUM_CNS],
            and stormType [CPSS_DXCH_POLICER_STORM_TYPE_UC_KNOWN_E /
                           CPSS_DXCH_POLICER_STORM_TYPE_MC_REGISTERED_E /
                           CPSS_DXCH_POLICER_STORM_TYPE_TCP_SYN_E]
    Expected: GT_OK.
    1.1.2. Call api with wrong stage [wrong enum values].
    Expected: GT_BAD_PARAM.
    1.1.3. Call api with wrong stormType [wrong enum values].
    Expected: GT_BAD_PARAM.
    1.1.4. Call api with wrong indexPtr [NULL].
    Expected: GT_BAD_PTR.
    1.1.5. Call api with wrong portNum [POLICER_INVALID_PHY_PORT_NUM_CNS].
    Expected: NOT GT_OK.
*/
    GT_STATUS st = GT_OK;

    GT_U8                             dev;
    CPSS_DXCH_POLICER_STAGE_TYPE_ENT  stage = CPSS_DXCH_POLICER_STAGE_INGRESS_0_E;
    GT_U8                             portNum = 0;
    CPSS_DXCH_POLICER_STORM_TYPE_ENT  stormType = CPSS_DXCH_POLICER_STORM_TYPE_UC_KNOWN_E;
    GT_U32                            index;

    /* prepare device iterator */
    PRV_TGF_NOT_APPLIC_DEV_RESET_MAC(&dev, UTF_CH1_E | UTF_CH1_DIAMOND_E | UTF_CH2_E | UTF_CH3_E);

    /* 1. Go over all active devices. */
    while (GT_OK == prvUtfNextDeviceGet(&dev, GT_TRUE))
    {
        st = prvUtfNextPhyPortReset(&portNum, dev);
        UTF_VERIFY_EQUAL0_PARAM_MAC(GT_OK, st);

        /* 1.1. For all active devices go over all available physical ports. */
        while (GT_OK == prvUtfNextPhyPortGet(&portNum, GT_TRUE))
        {
            /*
                1.1.1. Call function with stage [CPSS_DXCH_POLICER_STAGE_INGRESS_0_E /
                                               CPSS_DXCH_POLICER_STAGE_INGRESS_1_E /
                                               CPSS_DXCH_POLICER_STAGE_EGRESS_E],
                        portNum [0 / 4 / CPSS_CPU_PORT_NUM_CNS],
                        and stormType [CPSS_DXCH_POLICER_STORM_TYPE_UC_KNOWN_E /
                                       CPSS_DXCH_POLICER_STORM_TYPE_MC_REGISTERED_E /
                                       CPSS_DXCH_POLICER_STORM_TYPE_TCP_SYN_E]
                Expected: GT_OK.
            */
            stage = CPSS_DXCH_POLICER_STAGE_INGRESS_0_E;

            stormType = CPSS_DXCH_POLICER_STORM_TYPE_UC_KNOWN_E;

            st = cpssDxChPolicerPortStormTypeIndexGet(dev, stage,
                                   portNum, stormType, &index);
            UTF_VERIFY_EQUAL1_PARAM_MAC(GT_OK, st, dev);

            /*
                1.1.1. Call function with stage [CPSS_DXCH_POLICER_STAGE_INGRESS_0_E /
                                               CPSS_DXCH_POLICER_STAGE_INGRESS_1_E /
                                               CPSS_DXCH_POLICER_STAGE_EGRESS_E],
                        portNum [0 / 4 / CPSS_CPU_PORT_NUM_CNS],
                        and stormType [CPSS_DXCH_POLICER_STORM_TYPE_UC_KNOWN_E /
                                       CPSS_DXCH_POLICER_STORM_TYPE_MC_REGISTERED_E /
                                       CPSS_DXCH_POLICER_STORM_TYPE_TCP_SYN_E]
                Expected: GT_OK.
            */
            stage = CPSS_DXCH_POLICER_STAGE_INGRESS_1_E;

            stormType = CPSS_DXCH_POLICER_STORM_TYPE_MC_REGISTERED_E;

            st = cpssDxChPolicerPortStormTypeIndexGet(dev, stage,
                                   portNum, stormType, &index);
            UTF_VERIFY_EQUAL1_PARAM_MAC(GT_OK, st, dev);


            /*
                1.1.2. Call api with wrong stage [wrong enum values].
                Expected: GT_BAD_PARAM.
            */
            UTF_ENUMS_CHECK_MAC(cpssDxChPolicerPortStormTypeIndexGet
                                (dev, stage, portNum,
                                stormType, &index),
                                stage);

            /*
                1.1.3. Call api with wrong stormType [wrong enum values].
                Expected: GT_BAD_PARAM.
            */
            UTF_ENUMS_CHECK_MAC(cpssDxChPolicerPortStormTypeIndexGet
                                (dev, stage, portNum,
                                stormType, &index),
                                stormType);

            /*
                1.1.4. Call api with wrong indexPtr [NULL].
                Expected: GT_BAD_PTR.
            */
            st = cpssDxChPolicerPortStormTypeIndexGet(dev, stage,
                                   portNum, stormType, NULL);
            UTF_VERIFY_EQUAL1_PARAM_MAC(GT_BAD_PTR, st, dev);
        }

        /*
            1.1.5. Call function with stage [CPSS_DXCH_POLICER_STAGE_INGRESS_0_E /
                                           CPSS_DXCH_POLICER_STAGE_INGRESS_1_E /
                                           CPSS_DXCH_POLICER_STAGE_EGRESS_E],
                    portNum [0 / 4 / CPSS_CPU_PORT_NUM_CNS],
                    and stormType [CPSS_DXCH_POLICER_STORM_TYPE_UC_KNOWN_E /
                                   CPSS_DXCH_POLICER_STORM_TYPE_MC_REGISTERED_E /
                                   CPSS_DXCH_POLICER_STORM_TYPE_TCP_SYN_E]
            Expected: GT_OK.
        */
        stage = CPSS_DXCH_POLICER_STAGE_EGRESS_E;
        portNum = CPSS_CPU_PORT_NUM_CNS;
        stormType = CPSS_DXCH_POLICER_STORM_TYPE_TCP_SYN_E;

        st = cpssDxChPolicerPortStormTypeIndexGet(dev, stage,
                               portNum, stormType, &index);
        UTF_VERIFY_EQUAL1_PARAM_MAC(GT_OK, st, dev);

        /*
            1.1.6. Call api with wrong portNum [POLICER_INVALID_PHY_PORT_NUM_CNS].
            Expected: NOT GT_OK.
        */
        portNum = POLICER_INVALID_PHY_PORT_NUM_CNS;
        st = cpssDxChPolicerPortStormTypeIndexGet(dev, stage,
                               portNum, stormType, &index);
        UTF_VERIFY_NOT_EQUAL1_PARAM_MAC(GT_OK, st, dev);

        st = prvUtfNextPhyPortReset(&portNum, dev);
        UTF_VERIFY_EQUAL0_PARAM_MAC(GT_OK, st);

        /*
            1.2. For all active devices go over all non available
            physical ports.
        */
        while (GT_OK == prvUtfNextPhyPortGet(&portNum, GT_FALSE))
        {
            st = cpssDxChPolicerPortStormTypeIndexGet(dev, stage,
                                   portNum, stormType, &index);
            UTF_VERIFY_EQUAL2_PARAM_MAC(GT_BAD_PARAM, st, dev, portNum);
        }

        /* 1.3. For active device check that function returns GT_BAD_PARAM */
        /* for out of bound value for port number.                         */
        portNum = PRV_CPSS_MAX_PP_PORTS_NUM_CNS;

        st = cpssDxChPolicerPortStormTypeIndexGet(dev, stage,
                               portNum, stormType, &index);
        UTF_VERIFY_EQUAL2_PARAM_MAC(GT_BAD_PARAM, st, dev, portNum);

        /* 1.4. For active device check that function returns not GT_BAD_PARAM */
        /* for CPU port number.                                                */
        portNum = CPSS_CPU_PORT_NUM_CNS;

        st = cpssDxChPolicerPortStormTypeIndexGet(dev, stage,
                               portNum, stormType, &index);
        UTF_VERIFY_NOT_EQUAL2_PARAM_MAC(GT_BAD_PARAM, st, dev, portNum);
    }

    /* restore correct values */
    stage = CPSS_DXCH_POLICER_STAGE_INGRESS_0_E;
    portNum = 0;
    stormType = CPSS_DXCH_POLICER_STORM_TYPE_UC_KNOWN_E;

    /* 2. For not-active devices and devices from non-applicable family */
    /* check that function returns GT_BAD_PARAM.                        */

    /* prepare device iterator */
    PRV_TGF_NOT_APPLIC_DEV_RESET_MAC(&dev, UTF_CH1_E | UTF_CH1_DIAMOND_E | UTF_CH2_E | UTF_CH3_E);

    /* go over all non active devices */
    while(GT_OK == prvUtfNextDeviceGet(&dev, GT_FALSE))
    {
        st = cpssDxChPolicerPortStormTypeIndexGet(dev, stage,
                               portNum, stormType, &index);
        UTF_VERIFY_EQUAL1_PARAM_MAC(GT_NOT_APPLICABLE_DEVICE, st, dev);
    }

    /* 3. Call function with out of bound value for device id.*/
    dev = PRV_CPSS_MAX_PP_DEVICES_CNS;

    st = cpssDxChPolicerPortStormTypeIndexGet(dev, stage,
                               portNum, stormType, &index);
    UTF_VERIFY_EQUAL1_PARAM_MAC(GT_BAD_PARAM, st, dev);
}


/*----------------------------------------------------------------------------*/
/*
GT_STATUS cpssDxChPolicerPortStormTypeIndexSet
(
    IN  GT_U8                            devNum,
    IN  CPSS_DXCH_POLICER_STAGE_TYPE_ENT stage,
    IN  GT_U8                            portNum,
    IN  CPSS_DXCH_POLICER_STORM_TYPE_ENT stormType,
    IN  GT_U32                           index
)
*/
UTF_TEST_CASE_MAC(cpssDxChPolicerPortStormTypeIndexSet)
{
/*
    ITERATE_DEVICES_PHY_CPU_PORTS(DxChXcat and above)
    1.1.1. Call function with stage [CPSS_DXCH_POLICER_STAGE_INGRESS_0_E /
                                   CPSS_DXCH_POLICER_STAGE_INGRESS_1_E /
                                   CPSS_DXCH_POLICER_STAGE_EGRESS_E],
            portNum [0 / 4 / CPSS_CPU_PORT_NUM_CNS],
            and stormType [CPSS_DXCH_POLICER_STORM_TYPE_UC_KNOWN_E /
                           CPSS_DXCH_POLICER_STORM_TYPE_MC_REGISTERED_E /
                           CPSS_DXCH_POLICER_STORM_TYPE_TCP_SYN_E]
                           index [0 / 1 / 3].
    Expected: GT_OK.
    1.1.2. Call cpssDxChPolicerPortStormTypeIndexGet with the same params.
    Expected: GT_OK and the same values.
    1.1.3. Call api with wrong stage [wrong enum values].
    Expected: GT_BAD_PARAM.
    1.1.4. Call api with wrong stormType [wrong enum values].
    Expected: GT_BAD_PARAM.
    1.1.5. Call api with wrong index [4].
    Expected: NOT GT_OK.
    1.1.6. Call api with wrong portNum [POLICER_INVALID_PHY_PORT_NUM_CNS].
    Expected: NOT GT_OK.
*/
    GT_STATUS st = GT_OK;

    GT_U8                            dev;
    CPSS_DXCH_POLICER_STAGE_TYPE_ENT stage = CPSS_DXCH_POLICER_STAGE_INGRESS_0_E;
    GT_U8                            portNum = 0;
    CPSS_DXCH_POLICER_STORM_TYPE_ENT stormType = CPSS_DXCH_POLICER_STORM_TYPE_UC_KNOWN_E;
    GT_U32                           index = 0;
    GT_U32                           indexGet = 0;

    /* prepare device iterator */
    PRV_TGF_NOT_APPLIC_DEV_RESET_MAC(&dev, UTF_CH1_E | UTF_CH1_DIAMOND_E | UTF_CH2_E | UTF_CH3_E);

    /* 1. Go over all active devices. */
    while (GT_OK == prvUtfNextDeviceGet(&dev, GT_TRUE))
    {
        st = prvUtfNextPhyPortReset(&portNum, dev);
        UTF_VERIFY_EQUAL0_PARAM_MAC(GT_OK, st);

        /* 1.1. For all active devices go over all available physical ports. */
        while (GT_OK == prvUtfNextPhyPortGet(&portNum, GT_TRUE))
        {
            /*
                1.1.1. Call function with stage [CPSS_DXCH_POLICER_STAGE_INGRESS_0_E /
                                               CPSS_DXCH_POLICER_STAGE_INGRESS_1_E /
                                               CPSS_DXCH_POLICER_STAGE_EGRESS_E],
                        portNum [0 / 4 / CPSS_CPU_PORT_NUM_CNS],
                        and stormType [CPSS_DXCH_POLICER_STORM_TYPE_UC_KNOWN_E /
                                       CPSS_DXCH_POLICER_STORM_TYPE_MC_REGISTERED_E /
                                       CPSS_DXCH_POLICER_STORM_TYPE_TCP_SYN_E]
                                       index [0 / 1 / 3].
                Expected: GT_OK.
            */
            stage = CPSS_DXCH_POLICER_STAGE_INGRESS_0_E;

            stormType = CPSS_DXCH_POLICER_STORM_TYPE_UC_KNOWN_E;
            index = 0;

            st = cpssDxChPolicerPortStormTypeIndexSet(dev, stage,
                                   portNum, stormType, index);
            UTF_VERIFY_EQUAL1_PARAM_MAC(GT_OK, st, dev);

            /*
                1.1.3. Call cpssDxChPolicerPortStormTypeIndexGet with the same params.
                Expected: GT_OK and the same values.
            */
            st = cpssDxChPolicerPortStormTypeIndexGet(dev, stage,
                                   portNum, stormType, &indexGet);
            UTF_VERIFY_EQUAL1_STRING_MAC(GT_OK, st,
                   "cpssDxChPolicerPortStormTypeIndexGet: %d", dev);

            /* verifying values */
            UTF_VERIFY_EQUAL1_STRING_MAC(index, indexGet,
                       "got another index then was set: %d", dev);

            /*
                1.1.4. Call function with stage [CPSS_DXCH_POLICER_STAGE_INGRESS_0_E /
                                               CPSS_DXCH_POLICER_STAGE_INGRESS_1_E /
                                               CPSS_DXCH_POLICER_STAGE_EGRESS_E],
                        portNum [0 / 4 / CPSS_CPU_PORT_NUM_CNS],
                        and stormType [CPSS_DXCH_POLICER_STORM_TYPE_UC_KNOWN_E /
                                       CPSS_DXCH_POLICER_STORM_TYPE_MC_REGISTERED_E /
                                       CPSS_DXCH_POLICER_STORM_TYPE_TCP_SYN_E],
                                       index [0 / 1 / 3].
                Expected: GT_OK.
            */
            stage = CPSS_DXCH_POLICER_STAGE_INGRESS_1_E;

            stormType = CPSS_DXCH_POLICER_STORM_TYPE_MC_REGISTERED_E;
            index = 1;

            st = cpssDxChPolicerPortStormTypeIndexSet(dev, stage,
                                   portNum, stormType, index);
            UTF_VERIFY_EQUAL1_PARAM_MAC(GT_OK, st, dev);

            /*
                1.1.5. Call cpssDxChPolicerPortStormTypeIndexGet with the same params.
                Expected: GT_OK and the same values.
            */
            st = cpssDxChPolicerPortStormTypeIndexGet(dev, stage,
                                   portNum, stormType, &indexGet);
            UTF_VERIFY_EQUAL1_STRING_MAC(GT_OK, st,
                   "cpssDxChPolicerPortStormTypeIndexGet: %d", dev);

            /* verifying values */
            UTF_VERIFY_EQUAL1_STRING_MAC(index, indexGet,
                       "got another index then was set: %d", dev);

            /*
                1.1.6. Call cpssDxChPolicerPortStormTypeIndexGet with the same params.
                Expected: GT_OK and the same values.
            */
            st = cpssDxChPolicerPortStormTypeIndexGet(dev, stage,
                                   portNum, stormType, &indexGet);
            UTF_VERIFY_EQUAL1_STRING_MAC(GT_OK, st,
                   "cpssDxChPolicerPortStormTypeIndexGet: %d", dev);

            /* verifying values */
            UTF_VERIFY_EQUAL1_STRING_MAC(index, indexGet,
                       "got another index then was set: %d", dev);


            /*
                1.1.7. Call api with wrong stage [wrong enum values].
                Expected: GT_BAD_PARAM.
            */
            UTF_ENUMS_CHECK_MAC(cpssDxChPolicerPortStormTypeIndexSet
                                (dev, stage, portNum,
                                stormType, index),
                                stage);

            /*
                1.1.8. Call api with wrong stormType [wrong enum values].
                Expected: GT_BAD_PARAM.
            */
            UTF_ENUMS_CHECK_MAC(cpssDxChPolicerPortStormTypeIndexSet
                                (dev, stage, portNum,
                                stormType, index),
                                stormType);

            /*
                1.1.9. Call api with wrong index [4].
                Expected: NOT GT_OK.
            */
            index = 4;
            st = cpssDxChPolicerPortStormTypeIndexSet(dev, stage,
                                   portNum, stormType, index);
            UTF_VERIFY_NOT_EQUAL1_PARAM_MAC(GT_OK, st, dev);

            index = 0;
        }

        /*
            1.1.10. Call function with stage [CPSS_DXCH_POLICER_STAGE_INGRESS_0_E /
                                           CPSS_DXCH_POLICER_STAGE_INGRESS_1_E /
                                           CPSS_DXCH_POLICER_STAGE_EGRESS_E],
                    portNum [0 / 4 / CPSS_CPU_PORT_NUM_CNS],
                    and stormType [CPSS_DXCH_POLICER_STORM_TYPE_UC_KNOWN_E /
                                   CPSS_DXCH_POLICER_STORM_TYPE_MC_REGISTERED_E /
                                   CPSS_DXCH_POLICER_STORM_TYPE_TCP_SYN_E]
                                   index [0 / 1 / 3].
            Expected: GT_OK.
        */
        stage = CPSS_DXCH_POLICER_STAGE_EGRESS_E;
        portNum = CPSS_CPU_PORT_NUM_CNS;
        stormType = CPSS_DXCH_POLICER_STORM_TYPE_TCP_SYN_E;
        index = 3;

        st = cpssDxChPolicerPortStormTypeIndexSet(dev, stage,
                               portNum, stormType, index);
        UTF_VERIFY_EQUAL1_PARAM_MAC(GT_OK, st, dev);


        /*
            1.1.11. Call api with wrong portNum [POLICER_INVALID_PHY_PORT_NUM_CNS].
            Expected: NOT GT_OK.
        */
        portNum = POLICER_INVALID_PHY_PORT_NUM_CNS;
        st = cpssDxChPolicerPortStormTypeIndexSet(dev, stage,
                               portNum, stormType, index);
        UTF_VERIFY_NOT_EQUAL1_PARAM_MAC(GT_OK, st, dev);

        st = prvUtfNextPhyPortReset(&portNum, dev);
        UTF_VERIFY_EQUAL0_PARAM_MAC(GT_OK, st);

        /*
            1.2. For all active devices go over all non available
            physical ports.
        */
        while (GT_OK == prvUtfNextPhyPortGet(&portNum, GT_FALSE))
        {
            st = cpssDxChPolicerPortStormTypeIndexSet(dev, stage,
                                   portNum, stormType, index);
            UTF_VERIFY_EQUAL2_PARAM_MAC(GT_BAD_PARAM, st, dev, portNum);
        }

        /* 1.3. For active device check that function returns GT_BAD_PARAM */
        /* for out of bound value for port number.                         */
        portNum = PRV_CPSS_MAX_PP_PORTS_NUM_CNS;

        st = cpssDxChPolicerPortStormTypeIndexSet(dev, stage, portNum, stormType, index);
        UTF_VERIFY_EQUAL2_PARAM_MAC(GT_BAD_PARAM, st, dev, portNum);

        /* 1.4. For active device check that function returns not GT_BAD_PARAM */
        /* for CPU port number.                                                */
        portNum = CPSS_CPU_PORT_NUM_CNS;

        st = cpssDxChPolicerPortStormTypeIndexSet(dev, stage, portNum, stormType, index);
        UTF_VERIFY_NOT_EQUAL2_PARAM_MAC(GT_BAD_PARAM, st, dev, portNum);
    }

    /* restore correct values */
    stage = CPSS_DXCH_POLICER_STAGE_INGRESS_0_E;
    portNum = 0;
    stormType = CPSS_DXCH_POLICER_STORM_TYPE_UC_KNOWN_E;
    index = 0;

    /* 2. For not-active devices and devices from non-applicable family */
    /* check that function returns GT_BAD_PARAM.                        */

    /* prepare device iterator */
    PRV_TGF_NOT_APPLIC_DEV_RESET_MAC(&dev, UTF_CH1_E | UTF_CH1_DIAMOND_E | UTF_CH2_E | UTF_CH3_E);

    /* go over all non active devices */
    while(GT_OK == prvUtfNextDeviceGet(&dev, GT_FALSE))
    {
        st = cpssDxChPolicerPortStormTypeIndexSet(dev, stage, portNum, stormType, index);
        UTF_VERIFY_EQUAL1_PARAM_MAC(GT_NOT_APPLICABLE_DEVICE, st, dev);
    }

    /* 3. Call function with out of bound value for device id.*/
    dev = PRV_CPSS_MAX_PP_DEVICES_CNS;

    st = cpssDxChPolicerPortStormTypeIndexSet(dev, stage, portNum, stormType, index);
    UTF_VERIFY_EQUAL1_PARAM_MAC(GT_BAD_PARAM, st, dev);
}


/*----------------------------------------------------------------------------*/
/*
GT_STATUS cpssDxChPolicerStageMeterModeGet
(
    IN  GT_U8                                   devNum,
    IN  CPSS_DXCH_POLICER_STAGE_TYPE_ENT        stage,
    OUT CPSS_DXCH_POLICER_STAGE_METER_MODE_ENT  *modePtr
)
*/
UTF_TEST_CASE_MAC(cpssDxChPolicerStageMeterModeGet)
{
/*
    ITERATE_DEVICES(DxChXcat and above)
    1.1. Call function with stage [CPSS_DXCH_POLICER_STAGE_INGRESS_0_E /
                                   CPSS_DXCH_POLICER_STAGE_INGRESS_1_E /
                                   CPSS_DXCH_POLICER_STAGE_EGRESS_E],
    Expected: GT_OK.
    1.2. Call api with wrong stage [wrong enum values].
    Expected: GT_BAD_PARAM.
    1.3. Call api with wrong modePtr [NULL].
    Expected: GT_BAD_PTR.
*/
    GT_STATUS st = GT_OK;

    GT_U8                                   dev;
    CPSS_DXCH_POLICER_STAGE_TYPE_ENT        stage = CPSS_DXCH_POLICER_STAGE_INGRESS_0_E;
    CPSS_DXCH_POLICER_STAGE_METER_MODE_ENT  mode;

    /* prepare device iterator */
    PRV_TGF_NOT_APPLIC_DEV_RESET_MAC(&dev, UTF_CH1_E | UTF_CH1_DIAMOND_E | UTF_CH2_E | UTF_CH3_E);

    /* 1. Go over all active devices. */
    while (GT_OK == prvUtfNextDeviceGet(&dev, GT_TRUE))
    {
        /*
            1.1. Call function with stage [CPSS_DXCH_POLICER_STAGE_INGRESS_0_E /
                                           CPSS_DXCH_POLICER_STAGE_INGRESS_1_E /
                                           CPSS_DXCH_POLICER_STAGE_EGRESS_E],
            Expected: GT_OK.
        */
        stage = CPSS_DXCH_POLICER_STAGE_INGRESS_0_E;

        st = cpssDxChPolicerStageMeterModeGet(dev, stage, &mode);
        UTF_VERIFY_EQUAL1_PARAM_MAC(GT_OK, st, dev);

        /*
            1.1. Call function with stage [CPSS_DXCH_POLICER_STAGE_INGRESS_0_E /
                                           CPSS_DXCH_POLICER_STAGE_INGRESS_1_E /
                                           CPSS_DXCH_POLICER_STAGE_EGRESS_E],
            Expected: GT_OK.
        */
        stage = CPSS_DXCH_POLICER_STAGE_INGRESS_1_E;

        st = cpssDxChPolicerStageMeterModeGet(dev, stage, &mode);
        UTF_VERIFY_EQUAL1_PARAM_MAC(GT_OK, st, dev);

        /*
            1.1. Call function with stage [CPSS_DXCH_POLICER_STAGE_INGRESS_0_E /
                                           CPSS_DXCH_POLICER_STAGE_INGRESS_1_E /
                                           CPSS_DXCH_POLICER_STAGE_EGRESS_E],
            Expected: GT_OK.
        */
        stage = CPSS_DXCH_POLICER_STAGE_EGRESS_E;

        st = cpssDxChPolicerStageMeterModeGet(dev, stage, &mode);
        UTF_VERIFY_EQUAL1_PARAM_MAC(GT_OK, st, dev);

        /*
            1.2. Call api with wrong stage [wrong enum values].
            Expected: GT_BAD_PARAM.
        */
        UTF_ENUMS_CHECK_MAC(cpssDxChPolicerStageMeterModeGet
                            (dev, stage, &mode),
                            stage);

        /*
            1.3. Call api with wrong modePtr [NULL].
            Expected: GT_BAD_PTR.
        */
        st = cpssDxChPolicerStageMeterModeGet(dev, stage, NULL);
        UTF_VERIFY_EQUAL1_PARAM_MAC(GT_BAD_PTR, st, dev);
    }

    /* restore correct values */
    stage = CPSS_DXCH_POLICER_STAGE_INGRESS_0_E;

    /* 2. For not-active devices and devices from non-applicable family */
    /* check that function returns GT_BAD_PARAM.                        */

    /* prepare device iterator */
    PRV_TGF_NOT_APPLIC_DEV_RESET_MAC(&dev, UTF_CH1_E | UTF_CH1_DIAMOND_E | UTF_CH2_E | UTF_CH3_E);

    /* go over all non active devices */
    while(GT_OK == prvUtfNextDeviceGet(&dev, GT_FALSE))
    {
        st = cpssDxChPolicerStageMeterModeGet(dev, stage, &mode);
        UTF_VERIFY_EQUAL1_PARAM_MAC(GT_NOT_APPLICABLE_DEVICE, st, dev);
    }

    /* 3. Call function with out of bound value for device id.*/
    dev = PRV_CPSS_MAX_PP_DEVICES_CNS;

    st = cpssDxChPolicerStageMeterModeGet(dev, stage, &mode);
    UTF_VERIFY_EQUAL1_PARAM_MAC(GT_BAD_PARAM, st, dev);
}

/*----------------------------------------------------------------------------*/
/*
GT_STATUS cpssDxChPolicerStageMeterModeSet
(
    IN  GT_U8                                  devNum,
    IN  CPSS_DXCH_POLICER_STAGE_TYPE_ENT       stage,
    IN  CPSS_DXCH_POLICER_STAGE_METER_MODE_ENT mode
)
*/
UTF_TEST_CASE_MAC(cpssDxChPolicerStageMeterModeSet)
{
/*
    ITERATE_DEVICES(DxChXcat and above)
    1.1. Call function with stage [CPSS_DXCH_POLICER_STAGE_INGRESS_0_E /
                                   CPSS_DXCH_POLICER_STAGE_INGRESS_1_E /
                                   CPSS_DXCH_POLICER_STAGE_EGRESS_E],
                         and mode [CPSS_DXCH_POLICER_STAGE_METER_MODE_PORT_E /
                                   CPSS_DXCH_POLICER_STAGE_METER_MODE_FLOW_E].
    Expected: GT_OK.
    1.2. Call cpssDxChPolicerStageMeterModeGet with the same params.
    Expected: GT_OK and the same values.
    1.3. Call api with wrong stage [wrong enum values].
    Expected: GT_BAD_PARAM.
    1.4. Call api with wrong mode [wrong enum values].
    Expected: GT_BAD_PARAM.
*/
    GT_STATUS st = GT_OK;

    GT_U8                                  dev;
    CPSS_DXCH_POLICER_STAGE_TYPE_ENT       stage = CPSS_DXCH_POLICER_STAGE_INGRESS_0_E;
    CPSS_DXCH_POLICER_STAGE_METER_MODE_ENT mode = CPSS_DXCH_POLICER_STAGE_METER_MODE_PORT_E;
    CPSS_DXCH_POLICER_STAGE_METER_MODE_ENT modeGet;

    /* prepare device iterator */
    PRV_TGF_NOT_APPLIC_DEV_RESET_MAC(&dev, UTF_CH1_E | UTF_CH1_DIAMOND_E | UTF_CH2_E | UTF_CH3_E);

    /* 1. Go over all active devices. */
    while (GT_OK == prvUtfNextDeviceGet(&dev, GT_TRUE))
    {
        /*
            1.1. Call function with stage [CPSS_DXCH_POLICER_STAGE_INGRESS_0_E /
                                           CPSS_DXCH_POLICER_STAGE_INGRESS_1_E /
                                           CPSS_DXCH_POLICER_STAGE_EGRESS_E],
                                 and mode [CPSS_DXCH_POLICER_STAGE_METER_MODE_PORT_E /
                                           CPSS_DXCH_POLICER_STAGE_METER_MODE_FLOW_E].
            Expected: GT_OK.
        */
        stage = CPSS_DXCH_POLICER_STAGE_INGRESS_0_E;
        mode = CPSS_DXCH_POLICER_STAGE_METER_MODE_PORT_E;

        st = cpssDxChPolicerStageMeterModeSet(dev, stage, mode);
        UTF_VERIFY_EQUAL1_PARAM_MAC(GT_OK, st, dev);

        /*
            1.2. Call cpssDxChPolicerStageMeterModeGet with the same params.
            Expected: GT_OK and the same values.
        */
        st = cpssDxChPolicerStageMeterModeGet(dev, stage, &modeGet);
        UTF_VERIFY_EQUAL1_STRING_MAC(GT_OK, st,
               "cpssDxChPolicerStageMeterModeGet: %d", dev);

        /* verifying values */
        UTF_VERIFY_EQUAL1_STRING_MAC(mode, modeGet,
                   "got another mode then was set: %d", dev);

        /*
            1.1. Call function with stage [CPSS_DXCH_POLICER_STAGE_INGRESS_0_E /
                                           CPSS_DXCH_POLICER_STAGE_INGRESS_1_E /
                                           CPSS_DXCH_POLICER_STAGE_EGRESS_E],
                                 and mode [CPSS_DXCH_POLICER_STAGE_METER_MODE_PORT_E /
                                           CPSS_DXCH_POLICER_STAGE_METER_MODE_FLOW_E].
            Expected: GT_OK.
        */
        stage = CPSS_DXCH_POLICER_STAGE_INGRESS_1_E;

        st = cpssDxChPolicerStageMeterModeSet(dev, stage, mode);
        UTF_VERIFY_EQUAL1_PARAM_MAC(GT_OK, st, dev);

        /*
            1.2. Call cpssDxChPolicerStageMeterModeGet with the same params.
            Expected: GT_OK and the same values.
        */
        st = cpssDxChPolicerStageMeterModeGet(dev, stage, &modeGet);
        UTF_VERIFY_EQUAL1_STRING_MAC(GT_OK, st,
               "cpssDxChPolicerStageMeterModeGet: %d", dev);

        /* verifying values */
        UTF_VERIFY_EQUAL1_STRING_MAC(mode, modeGet,
                   "got another mode then was set: %d", dev);

        /*
            1.1. Call function with stage [CPSS_DXCH_POLICER_STAGE_INGRESS_0_E /
                                           CPSS_DXCH_POLICER_STAGE_INGRESS_1_E /
                                           CPSS_DXCH_POLICER_STAGE_EGRESS_E],
                                 and mode [CPSS_DXCH_POLICER_STAGE_METER_MODE_PORT_E /
                                           CPSS_DXCH_POLICER_STAGE_METER_MODE_FLOW_E].
            Expected: GT_OK.
        */
        stage = CPSS_DXCH_POLICER_STAGE_EGRESS_E;
        mode = CPSS_DXCH_POLICER_STAGE_METER_MODE_FLOW_E;

        st = cpssDxChPolicerStageMeterModeSet(dev, stage, mode);
        UTF_VERIFY_EQUAL1_PARAM_MAC(GT_OK, st, dev);

        /*
            1.2. Call cpssDxChPolicerStageMeterModeGet with the same params.
            Expected: GT_OK and the same values.
        */
        st = cpssDxChPolicerStageMeterModeGet(dev, stage, &modeGet);
        UTF_VERIFY_EQUAL1_STRING_MAC(GT_OK, st,
               "cpssDxChPolicerStageMeterModeGet: %d", dev);

        /* verifying values */
        UTF_VERIFY_EQUAL1_STRING_MAC(mode, modeGet,
                   "got another mode then was set: %d", dev);

        /*
            1.3. Call api with wrong stage [wrong enum values].
            Expected: GT_BAD_PARAM.
        */
        UTF_ENUMS_CHECK_MAC(cpssDxChPolicerStageMeterModeSet
                            (dev, stage, mode),
                            stage);

        /*
            1.4. Call api with wrong mode [wrong enum values].
            Expected: GT_BAD_PARAM.
        */
        UTF_ENUMS_CHECK_MAC(cpssDxChPolicerStageMeterModeSet
                            (dev, stage, mode),
                            mode);
    }

    /* restore correct values */
    stage = CPSS_DXCH_POLICER_STAGE_INGRESS_0_E;
    mode = CPSS_DXCH_POLICER_STAGE_METER_MODE_PORT_E;

    /* 2. For not-active devices and devices from non-applicable family */
    /* check that function returns GT_BAD_PARAM.                        */

    /* prepare device iterator */
    PRV_TGF_NOT_APPLIC_DEV_RESET_MAC(&dev, UTF_CH1_E | UTF_CH1_DIAMOND_E | UTF_CH2_E | UTF_CH3_E);

    /* go over all non active devices */
    while(GT_OK == prvUtfNextDeviceGet(&dev, GT_FALSE))
    {
        st = cpssDxChPolicerStageMeterModeSet(dev, stage, mode);
        UTF_VERIFY_EQUAL1_PARAM_MAC(GT_NOT_APPLICABLE_DEVICE, st, dev);
    }

    /* 3. Call function with out of bound value for device id.*/
    dev = PRV_CPSS_MAX_PP_DEVICES_CNS;

    st = cpssDxChPolicerStageMeterModeSet(dev, stage, mode);
    UTF_VERIFY_EQUAL1_PARAM_MAC(GT_BAD_PARAM, st, dev);
}

/*----------------------------------------------------------------------------*/
/*
GT_STATUS cpssDxChPolicerVlanCntrGet
(
    IN  GT_U8                             devNum,
    IN  CPSS_DXCH_POLICER_STAGE_TYPE_ENT  stage,
    IN  GT_U16                            vid,
    OUT GT_U32                            *cntrValuePtr
)
*/
UTF_TEST_CASE_MAC(cpssDxChPolicerVlanCntrGet)
{
/*
    ITERATE_DEVICES(DxChXcat and above)
    1.1. Call function with stage [CPSS_DXCH_POLICER_STAGE_INGRESS_0_E /
                                   CPSS_DXCH_POLICER_STAGE_INGRESS_1_E /
                                   CPSS_DXCH_POLICER_STAGE_EGRESS_E],
                        and vid [0 / 10 / 100].
    Expected: GT_OK.
    1.2. Call api with wrong stage [wrong enum values].
    Expected: GT_BAD_PARAM.
    1.3. Call api with wrong vid [wrong value].
    Expected: NOT GT_OK.
    1.4. Call api with wrong cntrValuePtr [NULL].
    Expected: GT_BAD_PTR.
*/
    GT_STATUS st = GT_OK;

    GT_U8                             dev;
    CPSS_DXCH_POLICER_STAGE_TYPE_ENT  stage = CPSS_DXCH_POLICER_STAGE_INGRESS_0_E;
    GT_U16                            vid = 0;
    GT_U32                            cntrValue;

    /* prepare device iterator */
    PRV_TGF_NOT_APPLIC_DEV_RESET_MAC(&dev, UTF_CH1_E | UTF_CH1_DIAMOND_E | UTF_CH2_E | UTF_CH3_E | UTF_XCAT2_E);

    /* 1. Go over all active devices. */
    while (GT_OK == prvUtfNextDeviceGet(&dev, GT_TRUE))
    {
        /*
            1.1. Call function with stage [CPSS_DXCH_POLICER_STAGE_INGRESS_0_E /
                                           CPSS_DXCH_POLICER_STAGE_INGRESS_1_E /
                                           CPSS_DXCH_POLICER_STAGE_EGRESS_E],
                                and vid [0 / 10 / 100].
            Expected: GT_OK.
        */
        stage = CPSS_DXCH_POLICER_STAGE_INGRESS_0_E;
        vid = 0;

        st = cpssDxChPolicerVlanCntrGet(dev, stage, vid, &cntrValue);
        UTF_VERIFY_EQUAL1_PARAM_MAC(GT_OK, st, dev);

        /*
            1.1. Call function with stage [CPSS_DXCH_POLICER_STAGE_INGRESS_0_E /
                                           CPSS_DXCH_POLICER_STAGE_INGRESS_1_E /
                                           CPSS_DXCH_POLICER_STAGE_EGRESS_E],
                                and vid [0 / 10 / 100].
            Expected: GT_OK.
        */
        stage = CPSS_DXCH_POLICER_STAGE_INGRESS_1_E;
        vid = 10;

        st = cpssDxChPolicerVlanCntrGet(dev, stage, vid, &cntrValue);
        UTF_VERIFY_EQUAL1_PARAM_MAC(GT_OK, st, dev);

        /*
            1.1. Call function with stage [CPSS_DXCH_POLICER_STAGE_INGRESS_0_E /
                                           CPSS_DXCH_POLICER_STAGE_INGRESS_1_E /
                                           CPSS_DXCH_POLICER_STAGE_EGRESS_E],
                                and vid [0 / 10 / 100].
            Expected: GT_OK.
        */
        stage = CPSS_DXCH_POLICER_STAGE_EGRESS_E;
        vid = 100;

        st = cpssDxChPolicerVlanCntrGet(dev, stage, vid, &cntrValue);
        UTF_VERIFY_EQUAL1_PARAM_MAC(GT_OK, st, dev);

        /*
            1.2. Call api with wrong stage [wrong enum values].
            Expected: GT_BAD_PARAM.
        */
        UTF_ENUMS_CHECK_MAC(cpssDxChPolicerVlanCntrGet
                            (dev, stage, vid, &cntrValue),
                            stage);

        /*
            1.3. Call api with wrong vid [wrong value].
            Expected: NOT GT_OK.
        */
        vid = (GT_U16)(PRV_CPSS_DXCH_PP_MAC(dev)->policer.memSize[stage] << 2);

        st = cpssDxChPolicerVlanCntrGet(dev, stage, vid, &cntrValue);
        UTF_VERIFY_NOT_EQUAL1_PARAM_MAC(GT_OK, st, dev);

        vid = 0;

        /*
            1.4. Call api with wrong cntrValuePtr [NULL].
            Expected: GT_BAD_PTR.
        */
        st = cpssDxChPolicerVlanCntrGet(dev, stage, vid, NULL);
        UTF_VERIFY_EQUAL1_PARAM_MAC(GT_BAD_PTR, st, dev);
    }

    /* restore correct values */
    stage = CPSS_DXCH_POLICER_STAGE_INGRESS_0_E;
    vid = 0;

    /* 2. For not-active devices and devices from non-applicable family */
    /* check that function returns GT_BAD_PARAM.                        */

    /* prepare device iterator */
    PRV_TGF_NOT_APPLIC_DEV_RESET_MAC(&dev, UTF_CH1_E | UTF_CH1_DIAMOND_E | UTF_CH2_E | UTF_CH3_E | UTF_XCAT2_E);

    /* go over all non active devices */
    while(GT_OK == prvUtfNextDeviceGet(&dev, GT_FALSE))
    {
        st = cpssDxChPolicerVlanCntrGet(dev, stage, vid, &cntrValue);
        UTF_VERIFY_EQUAL1_PARAM_MAC(GT_NOT_APPLICABLE_DEVICE, st, dev);
    }

    /* 3. Call function with out of bound value for device id.*/
    dev = PRV_CPSS_MAX_PP_DEVICES_CNS;

    st = cpssDxChPolicerVlanCntrGet(dev, stage, vid, &cntrValue);
    UTF_VERIFY_EQUAL1_PARAM_MAC(GT_BAD_PARAM, st, dev);
}

/*----------------------------------------------------------------------------*/
/*
GT_STATUS cpssDxChPolicerVlanCntrSet
(
    IN  GT_U8                            devNum,
    IN  CPSS_DXCH_POLICER_STAGE_TYPE_ENT stage,
    IN  GT_U16                           vid,
    IN  GT_U32                           cntrValue
)
*/
UTF_TEST_CASE_MAC(cpssDxChPolicerVlanCntrSet)
{
/*
    ITERATE_DEVICES(DxChXcat and above)
    1.1. Call function with stage [CPSS_DXCH_POLICER_STAGE_INGRESS_0_E /
                                   CPSS_DXCH_POLICER_STAGE_INGRESS_1_E /
                                   CPSS_DXCH_POLICER_STAGE_EGRESS_E],
                        and vid [0 / 10 / 100],
                  and cntrValue [0 / 10 / 100].
    Expected: GT_OK.
    1.2. Call cpssDxChPolicerVlanCntrGet with the same params.
    Expected: GT_OK and the same values.
    1.3. Call api with wrong stage [wrong enum values].
    Expected: GT_BAD_PARAM.
    1.4. Call api with wrong vid [wrong value].
    Expected: NOT GT_OK.
*/
    GT_STATUS st = GT_OK;

    GT_U8                            dev;
    CPSS_DXCH_POLICER_STAGE_TYPE_ENT stage = CPSS_DXCH_POLICER_STAGE_INGRESS_0_E;
    GT_U16                           vid = 0;
    GT_U32                           cntrValue = 0;
    GT_U32                           cntrValueGet = 0;

    /* prepare device iterator */
    PRV_TGF_NOT_APPLIC_DEV_RESET_MAC(&dev, UTF_CH1_E | UTF_CH1_DIAMOND_E | UTF_CH2_E | UTF_CH3_E | UTF_XCAT2_E);

    /* 1. Go over all active devices. */
    while (GT_OK == prvUtfNextDeviceGet(&dev, GT_TRUE))
    {
        /*
            1.1. Call function with stage [CPSS_DXCH_POLICER_STAGE_INGRESS_0_E /
                                           CPSS_DXCH_POLICER_STAGE_INGRESS_1_E /
                                           CPSS_DXCH_POLICER_STAGE_EGRESS_E],
                                and vid [0 / 10 / 100],
                          and cntrValue [0 / 10 / 100].
            Expected: GT_OK.
        */
        stage = CPSS_DXCH_POLICER_STAGE_INGRESS_0_E;
        vid = 0;
        cntrValue = 0;

        st = cpssDxChPolicerVlanCntrSet(dev, stage, vid, cntrValue);
        UTF_VERIFY_EQUAL1_PARAM_MAC(GT_OK, st, dev);

        /*
            1.2. Call cpssDxChPolicerVlanCntrGet with the same params.
            Expected: GT_OK and the same values.
        */
        st = cpssDxChPolicerVlanCntrGet(dev, stage, vid, &cntrValueGet);
        UTF_VERIFY_EQUAL1_STRING_MAC(GT_OK, st,
               "cpssDxChPolicerVlanCntrGet: %d", dev);

        /* verifying values */
        UTF_VERIFY_EQUAL1_STRING_MAC(cntrValue, cntrValueGet,
                   "got another cntrValue then was set: %d", dev);

        /*
            1.1. Call function with stage [CPSS_DXCH_POLICER_STAGE_INGRESS_0_E /
                                           CPSS_DXCH_POLICER_STAGE_INGRESS_1_E /
                                           CPSS_DXCH_POLICER_STAGE_EGRESS_E],
                                and vid [0 / 10 / 100],
                          and cntrValue [0 / 10 / 100].
            Expected: GT_OK.
        */
        stage = CPSS_DXCH_POLICER_STAGE_INGRESS_1_E;
        vid = 10;
        cntrValue = 10;

        st = cpssDxChPolicerVlanCntrSet(dev, stage, vid, cntrValue);
        UTF_VERIFY_EQUAL1_PARAM_MAC(GT_OK, st, dev);

        /*
            1.2. Call cpssDxChPolicerVlanCntrGet with the same params.
            Expected: GT_OK and the same values.
        */
        st = cpssDxChPolicerVlanCntrGet(dev, stage, vid, &cntrValueGet);
        UTF_VERIFY_EQUAL1_STRING_MAC(GT_OK, st,
               "cpssDxChPolicerVlanCntrGet: %d", dev);

        /* verifying values */
        UTF_VERIFY_EQUAL1_STRING_MAC(cntrValue, cntrValueGet,
                   "got another cntrValue then was set: %d", dev);

        /*
            1.1. Call function with stage [CPSS_DXCH_POLICER_STAGE_INGRESS_0_E /
                                           CPSS_DXCH_POLICER_STAGE_INGRESS_1_E /
                                           CPSS_DXCH_POLICER_STAGE_EGRESS_E],
                                    vid [0 / 10 / 100],
                          and cntrValue [0 / 10 / 100].
            Expected: GT_OK.
        */
        stage = CPSS_DXCH_POLICER_STAGE_EGRESS_E;
        vid = 100;
        cntrValue = 100;

        st = cpssDxChPolicerVlanCntrSet(dev, stage, vid, cntrValue);
        UTF_VERIFY_EQUAL1_PARAM_MAC(GT_OK, st, dev);

        /*
            1.2. Call cpssDxChPolicerVlanCntrGet with the same params.
            Expected: GT_OK and the same values.
        */
        st = cpssDxChPolicerVlanCntrGet(dev, stage, vid, &cntrValueGet);
        UTF_VERIFY_EQUAL1_STRING_MAC(GT_OK, st,
               "cpssDxChPolicerVlanCntrGet: %d", dev);

        /* verifying values */
        UTF_VERIFY_EQUAL1_STRING_MAC(cntrValue, cntrValueGet,
                   "got another cntrValue then was set: %d", dev);


        /*
            1.3. Call api with wrong stage [wrong enum values].
            Expected: GT_BAD_PARAM.
        */
        UTF_ENUMS_CHECK_MAC(cpssDxChPolicerVlanCntrSet
                            (dev, stage, vid, cntrValue),
                            stage);

        /*
            1.4. Call api with wrong vid [wrong value].
            Expected: NOT GT_OK.
        */
        vid = (GT_U16)(PRV_CPSS_DXCH_PP_MAC(dev)->policer.memSize[stage] << 2);

        st = cpssDxChPolicerVlanCntrSet(dev, stage, vid, cntrValue);
        UTF_VERIFY_NOT_EQUAL1_PARAM_MAC(GT_OK, st, dev);

        vid = 0;
    }

    /* restore correct values */
    stage = CPSS_DXCH_POLICER_STAGE_INGRESS_0_E;
    vid = 0;
    cntrValue = 0;

    /* 2. For not-active devices and devices from non-applicable family */
    /* check that function returns GT_BAD_PARAM.                        */

    /* prepare device iterator */
    PRV_TGF_NOT_APPLIC_DEV_RESET_MAC(&dev, UTF_CH1_E | UTF_CH1_DIAMOND_E | UTF_CH2_E | UTF_CH3_E | UTF_XCAT2_E);

    /* go over all non active devices */
    while(GT_OK == prvUtfNextDeviceGet(&dev, GT_FALSE))
    {
        st = cpssDxChPolicerVlanCntrSet(dev, stage, vid, cntrValue);
        UTF_VERIFY_EQUAL1_PARAM_MAC(GT_NOT_APPLICABLE_DEVICE, st, dev);
    }

    /* 3. Call function with out of bound value for device id.*/
    dev = PRV_CPSS_MAX_PP_DEVICES_CNS;

    st = cpssDxChPolicerVlanCntrSet(dev, stage, vid, cntrValue);
    UTF_VERIFY_EQUAL1_PARAM_MAC(GT_BAD_PARAM, st, dev);
}

/*----------------------------------------------------------------------------*/
/*
GT_STATUS cpssDxChPolicerVlanCountingModeGet
(
    IN  GT_U8                                 devNum,
    IN  CPSS_DXCH_POLICER_STAGE_TYPE_ENT      stage,
    OUT CPSS_DXCH_POLICER_VLAN_CNTR_MODE_ENT  *modePtr
)
*/
UTF_TEST_CASE_MAC(cpssDxChPolicerVlanCountingModeGet)
{
/*
    ITERATE_DEVICES(DxChXcat and above)
    1.1. Call function with stage [CPSS_DXCH_POLICER_STAGE_INGRESS_0_E /
                                   CPSS_DXCH_POLICER_STAGE_INGRESS_1_E /
                                   CPSS_DXCH_POLICER_STAGE_EGRESS_E],
    Expected: GT_OK.
    1.2. Call api with wrong stage [wrong enum values].
    Expected: GT_BAD_PARAM.
    1.3. Call api with wrong modePtr [NULL].
    Expected: GT_BAD_PTR.
*/
    GT_STATUS st = GT_OK;

    GT_U8                                 dev;
    CPSS_DXCH_POLICER_STAGE_TYPE_ENT      stage = CPSS_DXCH_POLICER_STAGE_INGRESS_0_E;
    CPSS_DXCH_POLICER_VLAN_CNTR_MODE_ENT  mode;

    /* prepare device iterator */
    PRV_TGF_NOT_APPLIC_DEV_RESET_MAC(&dev, UTF_CH1_E | UTF_CH1_DIAMOND_E | UTF_CH2_E | UTF_CH3_E | UTF_XCAT2_E);

    /* 1. Go over all active devices. */
    while (GT_OK == prvUtfNextDeviceGet(&dev, GT_TRUE))
    {
        /*
            1.1. Call function with stage [CPSS_DXCH_POLICER_STAGE_INGRESS_0_E /
                                           CPSS_DXCH_POLICER_STAGE_INGRESS_1_E /
                                           CPSS_DXCH_POLICER_STAGE_EGRESS_E],
            Expected: GT_OK.
        */
        stage = CPSS_DXCH_POLICER_STAGE_INGRESS_0_E;

        st = cpssDxChPolicerVlanCountingModeGet(dev, stage, &mode);
        UTF_VERIFY_EQUAL1_PARAM_MAC(GT_OK, st, dev);

        /*
            1.1. Call function with stage [CPSS_DXCH_POLICER_STAGE_INGRESS_0_E /
                                           CPSS_DXCH_POLICER_STAGE_INGRESS_1_E /
                                           CPSS_DXCH_POLICER_STAGE_EGRESS_E],
            Expected: GT_OK.
        */
        stage = CPSS_DXCH_POLICER_STAGE_INGRESS_1_E;

        st = cpssDxChPolicerVlanCountingModeGet(dev, stage, &mode);
        UTF_VERIFY_EQUAL1_PARAM_MAC(GT_OK, st, dev);

        /*
            1.1. Call function with stage [CPSS_DXCH_POLICER_STAGE_INGRESS_0_E /
                                           CPSS_DXCH_POLICER_STAGE_INGRESS_1_E /
                                           CPSS_DXCH_POLICER_STAGE_EGRESS_E],
            Expected: GT_OK.
        */
        stage = CPSS_DXCH_POLICER_STAGE_EGRESS_E;

        st = cpssDxChPolicerVlanCountingModeGet(dev, stage, &mode);
        UTF_VERIFY_EQUAL1_PARAM_MAC(GT_OK, st, dev);

        /*
            1.2. Call api with wrong stage [wrong enum values].
            Expected: GT_BAD_PARAM.
        */
        UTF_ENUMS_CHECK_MAC(cpssDxChPolicerVlanCountingModeGet
                            (dev, stage, &mode),
                            stage);

        /*
            1.3. Call api with wrong modePtr [NULL].
            Expected: GT_BAD_PTR.
        */
        st = cpssDxChPolicerVlanCountingModeGet(dev, stage, NULL);
        UTF_VERIFY_EQUAL1_PARAM_MAC(GT_BAD_PTR, st, dev);
    }

    /* restore correct values */
    stage = CPSS_DXCH_POLICER_STAGE_INGRESS_0_E;

    /* 2. For not-active devices and devices from non-applicable family */
    /* check that function returns GT_BAD_PARAM.                        */

    /* prepare device iterator */
    PRV_TGF_NOT_APPLIC_DEV_RESET_MAC(&dev, UTF_CH1_E | UTF_CH1_DIAMOND_E | UTF_CH2_E | UTF_CH3_E | UTF_XCAT2_E);

    /* go over all non active devices */
    while(GT_OK == prvUtfNextDeviceGet(&dev, GT_FALSE))
    {
        st = cpssDxChPolicerVlanCountingModeGet(dev, stage, &mode);
        UTF_VERIFY_EQUAL1_PARAM_MAC(GT_NOT_APPLICABLE_DEVICE, st, dev);
    }

    /* 3. Call function with out of bound value for device id.*/
    dev = PRV_CPSS_MAX_PP_DEVICES_CNS;

    st = cpssDxChPolicerVlanCountingModeGet(dev, stage, &mode);
    UTF_VERIFY_EQUAL1_PARAM_MAC(GT_BAD_PARAM, st, dev);
}

/*----------------------------------------------------------------------------*/
/*
GT_STATUS cpssDxChPolicerVlanCountingModeSet
(
    IN  GT_U8                                devNum,
    IN  CPSS_DXCH_POLICER_STAGE_TYPE_ENT     stage,
    IN  CPSS_DXCH_POLICER_VLAN_CNTR_MODE_ENT mode
)
*/
UTF_TEST_CASE_MAC(cpssDxChPolicerVlanCountingModeSet)
{
/*
    ITERATE_DEVICES(DxChXcat and above)
    1.1. Call function with stage [CPSS_DXCH_POLICER_STAGE_INGRESS_0_E /
                                   CPSS_DXCH_POLICER_STAGE_INGRESS_1_E /
                                   CPSS_DXCH_POLICER_STAGE_EGRESS_E],
                         and mode [CPSS_DXCH_POLICER_VLAN_CNTR_MODE_BYTES_E /
                                   CPSS_DXCH_POLICER_VLAN_CNTR_MODE_PACKETS_E].
    Expected: GT_OK.
    1.2. Call cpssDxChPolicerVlanCountingModeGet with the same params.
    Expected: GT_OK and the same values.
    1.3. Call api with wrong stage [wrong enum values].
    Expected: GT_BAD_PARAM.
    1.4. Call api with wrong mode [wrong enum values].
    Expected: GT_BAD_PARAM.
*/
    GT_STATUS st = GT_OK;

    GT_U8                                dev;
    CPSS_DXCH_POLICER_STAGE_TYPE_ENT     stage = CPSS_DXCH_POLICER_STAGE_INGRESS_0_E;
    CPSS_DXCH_POLICER_VLAN_CNTR_MODE_ENT mode = CPSS_DXCH_POLICER_VLAN_CNTR_MODE_BYTES_E;
    CPSS_DXCH_POLICER_VLAN_CNTR_MODE_ENT modeGet;

    /* prepare device iterator */
    PRV_TGF_NOT_APPLIC_DEV_RESET_MAC(&dev, UTF_CH1_E | UTF_CH1_DIAMOND_E | UTF_CH2_E | UTF_CH3_E | UTF_XCAT2_E);

    /* 1. Go over all active devices. */
    while (GT_OK == prvUtfNextDeviceGet(&dev, GT_TRUE))
    {
        /*
            1.1. Call function with stage [CPSS_DXCH_POLICER_STAGE_INGRESS_0_E /
                                           CPSS_DXCH_POLICER_STAGE_INGRESS_1_E /
                                           CPSS_DXCH_POLICER_STAGE_EGRESS_E],
                                 and mode [CPSS_DXCH_POLICER_VLAN_CNTR_MODE_BYTES_E /
                                           CPSS_DXCH_POLICER_VLAN_CNTR_MODE_PACKETS_E].
            Expected: GT_OK.
        */
        stage = CPSS_DXCH_POLICER_STAGE_INGRESS_0_E;
        mode = CPSS_DXCH_POLICER_VLAN_CNTR_MODE_BYTES_E;

        st = cpssDxChPolicerVlanCountingModeSet(dev, stage, mode);
        UTF_VERIFY_EQUAL1_PARAM_MAC(GT_OK, st, dev);

        /*
            1.2. Call cpssDxChPolicerVlanCountingModeGet with the same params.
            Expected: GT_OK and the same values.
        */
        st = cpssDxChPolicerVlanCountingModeGet(dev, stage, &modeGet);
        UTF_VERIFY_EQUAL1_STRING_MAC(GT_OK, st,
               "cpssDxChPolicerVlanCountingModeGet: %d", dev);

        /* verifying values */
        UTF_VERIFY_EQUAL1_STRING_MAC(mode, modeGet,
                   "got another mode then was set: %d", dev);

        /*
            1.1. Call function with stage [CPSS_DXCH_POLICER_STAGE_INGRESS_0_E /
                                           CPSS_DXCH_POLICER_STAGE_INGRESS_1_E /
                                           CPSS_DXCH_POLICER_STAGE_EGRESS_E],
                                 and mode [CPSS_DXCH_POLICER_VLAN_CNTR_MODE_BYTES_E /
                                           CPSS_DXCH_POLICER_VLAN_CNTR_MODE_PACKETS_E].
            Expected: GT_OK.
        */
        stage = CPSS_DXCH_POLICER_STAGE_INGRESS_1_E;
        mode = CPSS_DXCH_POLICER_VLAN_CNTR_MODE_BYTES_E;

        st = cpssDxChPolicerVlanCountingModeSet(dev, stage, mode);
        UTF_VERIFY_EQUAL1_PARAM_MAC(GT_OK, st, dev);

        /*
            1.2. Call cpssDxChPolicerVlanCountingModeGet with the same params.
            Expected: GT_OK and the same values.
        */
        st = cpssDxChPolicerVlanCountingModeGet(dev, stage, &modeGet);
        UTF_VERIFY_EQUAL1_STRING_MAC(GT_OK, st,
               "cpssDxChPolicerVlanCountingModeGet: %d", dev);

        /* verifying values */
        UTF_VERIFY_EQUAL1_STRING_MAC(mode, modeGet,
                   "got another mode then was set: %d", dev);

        /*
            1.1. Call function with stage [CPSS_DXCH_POLICER_STAGE_INGRESS_0_E /
                                           CPSS_DXCH_POLICER_STAGE_INGRESS_1_E /
                                           CPSS_DXCH_POLICER_STAGE_EGRESS_E],
                                 and mode [CPSS_DXCH_POLICER_VLAN_CNTR_MODE_BYTES_E /
                                           CPSS_DXCH_POLICER_VLAN_CNTR_MODE_PACKETS_E].
            Expected: GT_OK.
        */
        stage = CPSS_DXCH_POLICER_STAGE_EGRESS_E;
        mode = CPSS_DXCH_POLICER_VLAN_CNTR_MODE_PACKETS_E;

        st = cpssDxChPolicerVlanCountingModeSet(dev, stage, mode);
        UTF_VERIFY_EQUAL1_PARAM_MAC(GT_OK, st, dev);

        /*
            1.2. Call cpssDxChPolicerVlanCountingModeGet with the same params.
            Expected: GT_OK and the same values.
        */
        st = cpssDxChPolicerVlanCountingModeGet(dev, stage, &modeGet);
        UTF_VERIFY_EQUAL1_STRING_MAC(GT_OK, st,
               "cpssDxChPolicerVlanCountingModeGet: %d", dev);

        /* verifying values */
        UTF_VERIFY_EQUAL1_STRING_MAC(mode, modeGet,
                   "got another mode then was set: %d", dev);

        /*
            1.3. Call api with wrong stage [wrong enum values].
            Expected: GT_BAD_PARAM.
        */
        UTF_ENUMS_CHECK_MAC(cpssDxChPolicerVlanCountingModeSet
                            (dev, stage, mode),
                            stage);

        /*
            1.4. Call api with wrong mode [wrong enum values].
            Expected: GT_BAD_PARAM.
        */
        UTF_ENUMS_CHECK_MAC(cpssDxChPolicerVlanCountingModeSet
                            (dev, stage, mode),
                            mode);
    }

    /* restore correct values */
    stage = CPSS_DXCH_POLICER_STAGE_INGRESS_0_E;
    mode = CPSS_DXCH_POLICER_VLAN_CNTR_MODE_BYTES_E;

    /* 2. For not-active devices and devices from non-applicable family */
    /* check that function returns GT_BAD_PARAM.                        */

    /* prepare device iterator */
    PRV_TGF_NOT_APPLIC_DEV_RESET_MAC(&dev, UTF_CH1_E | UTF_CH1_DIAMOND_E | UTF_CH2_E | UTF_CH3_E | UTF_XCAT2_E);

    /* go over all non active devices */
    while(GT_OK == prvUtfNextDeviceGet(&dev, GT_FALSE))
    {
        st = cpssDxChPolicerVlanCountingModeSet(dev, stage, mode);
        UTF_VERIFY_EQUAL1_PARAM_MAC(GT_NOT_APPLICABLE_DEVICE, st, dev);
    }

    /* 3. Call function with out of bound value for device id.*/
    dev = PRV_CPSS_MAX_PP_DEVICES_CNS;

    st = cpssDxChPolicerVlanCountingModeSet(dev, stage, mode);
    UTF_VERIFY_EQUAL1_PARAM_MAC(GT_BAD_PARAM, st, dev);
}

/*----------------------------------------------------------------------------*/
/*
GT_STATUS cpssDxChPolicerVlanCountingPacketCmdTriggerGet
(
    IN  GT_U8                             devNum,
    IN  CPSS_DXCH_POLICER_STAGE_TYPE_ENT  stage,
    IN  CPSS_PACKET_CMD_ENT               cmdTrigger,
    OUT GT_BOOL                           *enablePtr
)
*/
UTF_TEST_CASE_MAC(cpssDxChPolicerVlanCountingPacketCmdTriggerGet)
{
/*
    ITERATE_DEVICES(DxChXcat and above)
    1.1. Call function with stage [CPSS_DXCH_POLICER_STAGE_INGRESS_0_E /
                                   CPSS_DXCH_POLICER_STAGE_INGRESS_1_E /
                        and cmdTrigger [CPSS_PACKET_CMD_FORWARD_E /
                                        CPSS_PACKET_CMD_MIRROR_TO_CPU_E /
                                        CPSS_PACKET_CMD_DROP_SOFT_E].
    Expected: GT_OK.
    1.2. Call function with wrong stage [CPSS_DXCH_POLICER_STAGE_EGRESS_E].
    Expected: NOT GT_OK.
    1.3. Call function with wrong cmdTrigger [CPSS_PACKET_CMD_ROUTE_E].
    Expected: NOT GT_OK.
    1.4. Call api with wrong stage [wrong enum values].
    Expected: GT_BAD_PARAM.
    1.5. Call api with wrong cmdTrigger [wrong enum values].
    Expected: GT_BAD_PARAM.
    1.6. Call api with wrong enablePtr [NULL].
    Expected: GT_BAD_PTR.
*/
    GT_STATUS st = GT_OK;

    GT_U8                             dev;
    CPSS_DXCH_POLICER_STAGE_TYPE_ENT  stage = CPSS_DXCH_POLICER_STAGE_INGRESS_0_E;
    CPSS_PACKET_CMD_ENT               cmdTrigger = CPSS_PACKET_CMD_FORWARD_E;
    GT_BOOL                           enable;

    /* prepare device iterator */
    PRV_TGF_NOT_APPLIC_DEV_RESET_MAC(&dev, UTF_CH1_E | UTF_CH1_DIAMOND_E | UTF_CH2_E | UTF_CH3_E | UTF_XCAT2_E);

    /* 1. Go over all active devices. */
    while (GT_OK == prvUtfNextDeviceGet(&dev, GT_TRUE))
    {
        /*
            1.1. Call function with stage [CPSS_DXCH_POLICER_STAGE_INGRESS_0_E /
                                           CPSS_DXCH_POLICER_STAGE_INGRESS_1_E /
                                and cmdTrigger [CPSS_PACKET_CMD_FORWARD_E /
                                                CPSS_PACKET_CMD_MIRROR_TO_CPU_E /
                                                CPSS_PACKET_CMD_DROP_SOFT_E].
            Expected: GT_OK.
        */
        stage = CPSS_DXCH_POLICER_STAGE_INGRESS_0_E;
        cmdTrigger = CPSS_PACKET_CMD_FORWARD_E;

        st = cpssDxChPolicerVlanCountingPacketCmdTriggerGet(dev, stage,
                                         cmdTrigger, &enable);
        UTF_VERIFY_EQUAL1_PARAM_MAC(GT_OK, st, dev);

        /*
            1.1. Call function with stage [CPSS_DXCH_POLICER_STAGE_INGRESS_0_E /
                                           CPSS_DXCH_POLICER_STAGE_INGRESS_1_E /
                                and cmdTrigger [CPSS_PACKET_CMD_FORWARD_E /
                                                CPSS_PACKET_CMD_MIRROR_TO_CPU_E /
                                                CPSS_PACKET_CMD_DROP_SOFT_E].
            Expected: GT_OK.
        */
        stage = CPSS_DXCH_POLICER_STAGE_INGRESS_1_E;
        cmdTrigger = CPSS_PACKET_CMD_MIRROR_TO_CPU_E;

        st = cpssDxChPolicerVlanCountingPacketCmdTriggerGet(dev, stage,
                                         cmdTrigger, &enable);
        UTF_VERIFY_EQUAL1_PARAM_MAC(GT_OK, st, dev);

        /*
            1.1. Call function with stage [CPSS_DXCH_POLICER_STAGE_INGRESS_0_E /
                                           CPSS_DXCH_POLICER_STAGE_INGRESS_1_E /
                                and cmdTrigger [CPSS_PACKET_CMD_FORWARD_E /
                                                CPSS_PACKET_CMD_MIRROR_TO_CPU_E /
                                                CPSS_PACKET_CMD_DROP_SOFT_E].
            Expected: GT_OK.
        */
        stage = CPSS_DXCH_POLICER_STAGE_INGRESS_0_E;
        cmdTrigger = CPSS_PACKET_CMD_DROP_SOFT_E;

        st = cpssDxChPolicerVlanCountingPacketCmdTriggerGet(dev, stage,
                                         cmdTrigger, &enable);
        UTF_VERIFY_EQUAL1_PARAM_MAC(GT_OK, st, dev);

        /*
            1.2. Call function with wrong stage [CPSS_DXCH_POLICER_STAGE_EGRESS_E].
            Expected: NOT GT_OK.
        */
        stage = CPSS_DXCH_POLICER_STAGE_EGRESS_E;

        st = cpssDxChPolicerVlanCountingPacketCmdTriggerGet(dev, stage,
                                         cmdTrigger, &enable);
        UTF_VERIFY_NOT_EQUAL1_PARAM_MAC(GT_OK, st, dev);

        stage = CPSS_DXCH_POLICER_STAGE_INGRESS_0_E;

        /*
            1.3. Call function with wrong cmdTrigger [CPSS_PACKET_CMD_ROUTE_E].
            Expected: NOT GT_OK.
        */
        cmdTrigger = CPSS_PACKET_CMD_ROUTE_E;

        st = cpssDxChPolicerVlanCountingPacketCmdTriggerGet(dev, stage,
                                         cmdTrigger, &enable);
        UTF_VERIFY_NOT_EQUAL1_PARAM_MAC(GT_OK, st, dev);

        cmdTrigger = CPSS_PACKET_CMD_FORWARD_E;

        /*
            1.4. Call api with wrong stage [wrong enum values].
            Expected: GT_BAD_PARAM.
        */
        UTF_ENUMS_CHECK_MAC(cpssDxChPolicerVlanCountingPacketCmdTriggerGet
                            (dev, stage, cmdTrigger, &enable),
                            stage);

        /*
            1.5. Call api with wrong cmdTrigger [wrong enum values].
            Expected: GT_BAD_PARAM.
        */
        UTF_ENUMS_CHECK_MAC(cpssDxChPolicerVlanCountingPacketCmdTriggerGet
                            (dev, stage, cmdTrigger, &enable),
                            cmdTrigger);

        /*
            1.6. Call api with wrong enablePtr [NULL].
            Expected: GT_BAD_PTR.
        */
        st = cpssDxChPolicerVlanCountingPacketCmdTriggerGet(dev, stage,
                                         cmdTrigger, NULL);
        UTF_VERIFY_EQUAL1_PARAM_MAC(GT_BAD_PTR, st, dev);
    }

    /* restore correct values */
    stage = CPSS_DXCH_POLICER_STAGE_INGRESS_0_E;
    cmdTrigger = CPSS_PACKET_CMD_FORWARD_E;

    /* 2. For not-active devices and devices from non-applicable family */
    /* check that function returns GT_BAD_PARAM.                        */

    /* prepare device iterator */
    PRV_TGF_NOT_APPLIC_DEV_RESET_MAC(&dev, UTF_CH1_E | UTF_CH1_DIAMOND_E | UTF_CH2_E | UTF_CH3_E | UTF_XCAT2_E);

    /* go over all non active devices */
    while(GT_OK == prvUtfNextDeviceGet(&dev, GT_FALSE))
    {
        st = cpssDxChPolicerVlanCountingPacketCmdTriggerGet(dev, stage,
                                         cmdTrigger, &enable);
        UTF_VERIFY_EQUAL1_PARAM_MAC(GT_NOT_APPLICABLE_DEVICE, st, dev);
    }

    /* 3. Call function with out of bound value for device id.*/
    dev = PRV_CPSS_MAX_PP_DEVICES_CNS;

    st = cpssDxChPolicerVlanCountingPacketCmdTriggerGet(dev, stage,
                                         cmdTrigger, &enable);
    UTF_VERIFY_EQUAL1_PARAM_MAC(GT_BAD_PARAM, st, dev);
}

/*----------------------------------------------------------------------------*/
/*
GT_STATUS cpssDxChPolicerVlanCountingPacketCmdTriggerSet
(
    IN  GT_U8                            devNum,
    IN  CPSS_DXCH_POLICER_STAGE_TYPE_ENT stage,
    IN  CPSS_PACKET_CMD_ENT              cmdTrigger,
    IN  GT_BOOL                          enable
)
*/
UTF_TEST_CASE_MAC(cpssDxChPolicerVlanCountingPacketCmdTriggerSet)
{
/*
    ITERATE_DEVICES(DxChXcat and above)
    1.1. Call function with stage [CPSS_DXCH_POLICER_STAGE_INGRESS_0_E /
                                   CPSS_DXCH_POLICER_STAGE_INGRESS_1_E],
                            cmdTrigger [CPSS_PACKET_CMD_FORWARD_E /
                                        CPSS_PACKET_CMD_DROP_SOFT_E],
                        and enable [GT_TRUE / GT_FALSE].
    Expected: GT_OK.
    1.2. Call cpssDxChPolicerVlanCountingPacketCmdTriggerGet
         with the same params.
    Expected: GT_OK and the same values.
    1.3. Call function with wrong stage [CPSS_DXCH_POLICER_STAGE_EGRESS_E].
    Expected: NOT GT_OK.
    1.4. Call function with wrong cmdTrigger [CPSS_PACKET_CMD_ROUTE_E].
    Expected: NOT GT_OK.
    1.5. Call api with wrong stage [wrong enum values].
    Expected: GT_BAD_PARAM.
    1.6. Call api with wrong cmdTrigger [wrong enum values].
    Expected: GT_BAD_PARAM.
*/
    GT_STATUS st = GT_OK;

    GT_U8                            dev;
    CPSS_DXCH_POLICER_STAGE_TYPE_ENT stage = CPSS_DXCH_POLICER_STAGE_INGRESS_0_E;
    CPSS_PACKET_CMD_ENT              cmdTrigger = CPSS_PACKET_CMD_FORWARD_E;
    GT_BOOL                          enable = GT_FALSE;
    GT_BOOL                          enableGet;

    /* prepare device iterator */
    PRV_TGF_NOT_APPLIC_DEV_RESET_MAC(&dev, UTF_CH1_E | UTF_CH1_DIAMOND_E | UTF_CH2_E | UTF_CH3_E | UTF_XCAT2_E);

    /* 1. Go over all active devices. */
    while (GT_OK == prvUtfNextDeviceGet(&dev, GT_TRUE))
    {
        /*
            1.1. Call function with stage [CPSS_DXCH_POLICER_STAGE_INGRESS_0_E /
                                           CPSS_DXCH_POLICER_STAGE_INGRESS_1_E],
                                    cmdTrigger [CPSS_PACKET_CMD_FORWARD_E /
                                                CPSS_PACKET_CMD_DROP_SOFT_E],
                                and enable [GT_TRUE / GT_FALSE].
            Expected: GT_OK.
        */
        stage = CPSS_DXCH_POLICER_STAGE_INGRESS_0_E;
        cmdTrigger = CPSS_PACKET_CMD_FORWARD_E;
        enable = GT_TRUE;

        st = cpssDxChPolicerVlanCountingPacketCmdTriggerSet(dev, stage,
                                         cmdTrigger, enable);
        UTF_VERIFY_EQUAL1_PARAM_MAC(GT_OK, st, dev);

        /*
            1.2. Call cpssDxChPolicerVlanCountingPacketCmdTriggerGet
                 with the same params.
            Expected: GT_OK and the same values.
        */
        st = cpssDxChPolicerVlanCountingPacketCmdTriggerGet(dev, stage,
                                         cmdTrigger, &enableGet);
        UTF_VERIFY_EQUAL1_STRING_MAC(GT_OK, st,
               "cpssDxChPolicerVlanCountingPacketCmdTriggerGet: %d", dev);

        /* verifying values */
        UTF_VERIFY_EQUAL1_STRING_MAC(enable, enableGet,
                   "got another enable then was set: %d", dev);

        /*
            1.1. Call function with stage [CPSS_DXCH_POLICER_STAGE_INGRESS_0_E /
                                           CPSS_DXCH_POLICER_STAGE_INGRESS_1_E],
                                    cmdTrigger [CPSS_PACKET_CMD_FORWARD_E /
                                                CPSS_PACKET_CMD_DROP_SOFT_E],
                                and enable [GT_TRUE / GT_FALSE].
            Expected: GT_OK.
        */
        stage = CPSS_DXCH_POLICER_STAGE_INGRESS_1_E;
        cmdTrigger = CPSS_PACKET_CMD_DROP_SOFT_E;
        enable = GT_FALSE;

        st = cpssDxChPolicerVlanCountingPacketCmdTriggerSet(dev, stage,
                                         cmdTrigger, enable);
        UTF_VERIFY_EQUAL1_PARAM_MAC(GT_OK, st, dev);

        /*
            1.2. Call cpssDxChPolicerVlanCountingPacketCmdTriggerGet
                 with the same params.
            Expected: GT_OK and the same values.
        */
        st = cpssDxChPolicerVlanCountingPacketCmdTriggerGet(dev, stage,
                                         cmdTrigger, &enableGet);
        UTF_VERIFY_EQUAL1_STRING_MAC(GT_OK, st,
               "cpssDxChPolicerVlanCountingPacketCmdTriggerGet: %d", dev);

        /* verifying values */
        UTF_VERIFY_EQUAL1_STRING_MAC(enable, enableGet,
                   "got another enable then was set: %d", dev);

        /*
            1.3. Call function with wrong stage [CPSS_DXCH_POLICER_STAGE_EGRESS_E].
            Expected: NOT GT_OK.
        */
        stage = CPSS_DXCH_POLICER_STAGE_EGRESS_E;

        st = cpssDxChPolicerVlanCountingPacketCmdTriggerSet(dev, stage,
                                         cmdTrigger, enable);
        UTF_VERIFY_NOT_EQUAL1_PARAM_MAC(GT_OK, st, dev);

        stage = CPSS_DXCH_POLICER_STAGE_INGRESS_0_E;

        /*
            1.4. Call function with wrong cmdTrigger [CPSS_PACKET_CMD_ROUTE_E].
            Expected: NOT GT_OK.
        */
        cmdTrigger = CPSS_PACKET_CMD_ROUTE_E;

        st = cpssDxChPolicerVlanCountingPacketCmdTriggerSet(dev, stage,
                                         cmdTrigger, enable);
        UTF_VERIFY_NOT_EQUAL1_PARAM_MAC(GT_OK, st, dev);

        cmdTrigger = CPSS_PACKET_CMD_FORWARD_E;

        /*
            1.5. Call api with wrong stage [wrong enum values].
            Expected: GT_BAD_PARAM.
        */
        UTF_ENUMS_CHECK_MAC(cpssDxChPolicerVlanCountingPacketCmdTriggerSet
                            (dev, stage, cmdTrigger, enable),
                            stage);

        /*
            1.6. Call api with wrong cmdTrigger [wrong enum values].
            Expected: GT_BAD_PARAM.
        */
        UTF_ENUMS_CHECK_MAC(cpssDxChPolicerVlanCountingPacketCmdTriggerSet
                            (dev, stage, cmdTrigger, enable),
                            cmdTrigger);
    }

    /* restore correct values */
    stage = CPSS_DXCH_POLICER_STAGE_INGRESS_0_E;
    cmdTrigger = CPSS_PACKET_CMD_FORWARD_E;
    enable = GT_TRUE;

    /* 2. For not-active devices and devices from non-applicable family */
    /* check that function returns GT_BAD_PARAM.                        */

    /* prepare device iterator */
    PRV_TGF_NOT_APPLIC_DEV_RESET_MAC(&dev, UTF_CH1_E | UTF_CH1_DIAMOND_E | UTF_CH2_E | UTF_CH3_E | UTF_XCAT2_E);

    /* go over all non active devices */
    while(GT_OK == prvUtfNextDeviceGet(&dev, GT_FALSE))
    {
        st = cpssDxChPolicerVlanCountingPacketCmdTriggerSet(dev, stage,
                                         cmdTrigger, enable);
        UTF_VERIFY_EQUAL1_PARAM_MAC(GT_NOT_APPLICABLE_DEVICE, st, dev);
    }

    /* 3. Call function with out of bound value for device id.*/
    dev = PRV_CPSS_MAX_PP_DEVICES_CNS;

    st = cpssDxChPolicerVlanCountingPacketCmdTriggerSet(dev, stage,
                                         cmdTrigger, enable);
    UTF_VERIFY_EQUAL1_PARAM_MAC(GT_BAD_PARAM, st, dev);
}


/*GT_STATUS cpssDxChPolicerPortModeAddressSelectSet
(
    IN  GT_U8                                               devNum,
    IN CPSS_DXCH_POLICER_STAGE_TYPE_ENT                     stage,
    IN CPSS_DXCH_POLICER_PORT_MODE_ADDR_SELECT_TYPE_ENT     type
);*/

UTF_TEST_CASE_MAC(cpssDxChPolicerPortModeAddressSelectSet)
{
/*
    ITERATE_DEVICES(DxChXcat2 and above)
    1.1. Call function with stage [CPSS_DXCH_POLICER_STAGE_INGRESS_0_E /
                                   CPSS_DXCH_POLICER_STAGE_INGRESS_1_E /
                                   CPSS_DXCH_POLICER_STAGE_EGRESS_E],
                            type  [CPSS_DXCH_POLICER_PORT_MODE_ADDR_SELECT_TYPE_FULL_E /
                                        CPSS_DXCH_POLICER_PORT_MODE_ADDR_SELECT_TYPE_COMPRESSED_E]
    Expected: GT_OK.
    1.2. Call cpssDxChPolicerPortModeAddressSelectGet
         with the same params.
    Expected: GT_OK and the same values.
    1.5. Call function with wrong type [wrong enum values].
    Expected: NOT GT_BAD_PARAM.
    1.6. Call api with wrong stage [wrong enum values].
    Expected: GT_BAD_PARAM.
*/
    GT_STATUS st = GT_OK;

    GT_U8                            dev;
    CPSS_DXCH_POLICER_STAGE_TYPE_ENT stage = CPSS_DXCH_POLICER_STAGE_INGRESS_0_E;
    CPSS_DXCH_POLICER_PORT_MODE_ADDR_SELECT_TYPE_ENT    type = CPSS_DXCH_POLICER_PORT_MODE_ADDR_SELECT_TYPE_FULL_E;
    CPSS_DXCH_POLICER_PORT_MODE_ADDR_SELECT_TYPE_ENT    typeGet;

    /* prepare device iterator */
    PRV_TGF_NOT_APPLIC_DEV_RESET_MAC(&dev, UTF_CH1_E | UTF_CH1_DIAMOND_E | UTF_CH2_E | UTF_CH3_E | UTF_XCAT_E | UTF_LION_E);

    /* 1. Go over all active devices. */
    while (GT_OK == prvUtfNextDeviceGet(&dev, GT_TRUE))
    {
        for (stage = CPSS_DXCH_POLICER_STAGE_INGRESS_0_E; 
              stage <= CPSS_DXCH_POLICER_STAGE_EGRESS_E; stage++) 
        {
            /*
                1.1. Call function with stage [CPSS_DXCH_POLICER_STAGE_INGRESS_0_E /
                                               CPSS_DXCH_POLICER_STAGE_INGRESS_1_E /
                                               CPSS_DXCH_POLICER_STAGE_EGRESS_E],
                                    and type [CPSS_DXCH_POLICER_PORT_MODE_ADDR_SELECT_TYPE_FULL_E].
                Expected: GT_OK.
            */
            type = CPSS_DXCH_POLICER_PORT_MODE_ADDR_SELECT_TYPE_FULL_E;

            st = cpssDxChPolicerPortModeAddressSelectSet(dev, stage, type);
            UTF_VERIFY_EQUAL1_PARAM_MAC(GT_OK, st, dev);

            /*
                1.2. Call cpssDxChPolicerPortModeAddressSelectGet
                     with the same params.
                Expected: GT_OK and the same values.
            */
            st = cpssDxChPolicerPortModeAddressSelectGet(dev, stage, &typeGet);
            UTF_VERIFY_EQUAL1_STRING_MAC(GT_OK, st,
                   "cpssDxChPolicerPortModeAddressSelectGet: %d", dev);

            /* verifying values */
            UTF_VERIFY_EQUAL1_STRING_MAC(type, typeGet,
                       "got another type then was set: %d", dev);

            /*
                1.3. Call function with stage [CPSS_DXCH_POLICER_STAGE_INGRESS_0_E /
                                               CPSS_DXCH_POLICER_STAGE_INGRESS_1_E /
                                               CPSS_DXCH_POLICER_STAGE_EGRESS_E],
                                    and type [CPSS_DXCH_POLICER_PORT_MODE_ADDR_SELECT_TYPE_COMPRESSED_E].
                Expected: GT_OK.
            */
            type = CPSS_DXCH_POLICER_PORT_MODE_ADDR_SELECT_TYPE_COMPRESSED_E;

            st = cpssDxChPolicerPortModeAddressSelectSet(dev, stage, type);
            UTF_VERIFY_EQUAL1_PARAM_MAC(GT_OK, st, dev);

            /*
                1.4. Call cpssDxChPolicerPortModeAddressSelectGet
                     with the same params.
                Expected: GT_OK and the same values.
            */
            st = cpssDxChPolicerPortModeAddressSelectGet(dev, stage, &typeGet);
            UTF_VERIFY_EQUAL1_STRING_MAC(GT_OK, st,
                   "cpssDxChPolicerPortModeAddressSelectGet: %d", dev);

            /* verifying values */
            UTF_VERIFY_EQUAL1_STRING_MAC(type, typeGet,
                       "got another type then was set: %d", dev);
        }


        stage = CPSS_DXCH_POLICER_STAGE_INGRESS_0_E;

        /*
            1.5. Call function with wrong type [wrong enum values].
            Expected: GT_BAD_PARAM.
        */
        UTF_ENUMS_CHECK_MAC(cpssDxChPolicerPortModeAddressSelectSet
                            (dev, stage, type),
                            type);

        /*
            1.6. Call function with wrong stage [wrong enum values].
            Expected: GT_BAD_PARAM.
        */
        UTF_ENUMS_CHECK_MAC(cpssDxChPolicerPortModeAddressSelectSet
                            (dev, stage, type),
                            stage);
    }

    /* restore correct values */
    stage = CPSS_DXCH_POLICER_STAGE_INGRESS_0_E;
    type = CPSS_DXCH_POLICER_PORT_MODE_ADDR_SELECT_TYPE_FULL_E;

    /* 2. For not-active devices and devices from non-applicable family */
    /* check that function returns GT_BAD_PARAM.                        */

    /* prepare device iterator */
    PRV_TGF_NOT_APPLIC_DEV_RESET_MAC(&dev, UTF_CH1_E | UTF_CH1_DIAMOND_E | UTF_CH2_E | UTF_CH3_E | UTF_LION_E);

    /* go over all non active devices */
    while(GT_OK == prvUtfNextDeviceGet(&dev, GT_FALSE))
    {
        st = cpssDxChPolicerPortModeAddressSelectSet(dev, stage, type);
        UTF_VERIFY_EQUAL1_PARAM_MAC(GT_NOT_APPLICABLE_DEVICE, st, dev);
    }

    /* 3. Call function with out of bound value for device id.*/
    dev = PRV_CPSS_MAX_PP_DEVICES_CNS;

    st = cpssDxChPolicerPortModeAddressSelectSet(dev, stage, type);
    UTF_VERIFY_EQUAL1_PARAM_MAC(GT_BAD_PARAM, st, dev);
}


/*GT_STATUS cpssDxChPolicerPortModeAddressSelectGet
(
    IN  GT_U8                                               devNum,
    IN  CPSS_DXCH_POLICER_STAGE_TYPE_ENT                    stage,
    OUT CPSS_DXCH_POLICER_PORT_MODE_ADDR_SELECT_TYPE_ENT    *typePtr
);*/

UTF_TEST_CASE_MAC(cpssDxChPolicerPortModeAddressSelectGet)
{
/*
    ITERATE_DEVICES(DxChXcat2 and above)
    1.1. Call function with stage [CPSS_DXCH_POLICER_STAGE_INGRESS_0_E /
                                   CPSS_DXCH_POLICER_STAGE_INGRESS_1_E /
                                   CPSS_DXCH_POLICER_STAGE_EGRESS_E].
    Expected: GT_OK.
    1.2. Call function with wrong stage [wrong enum values].
    Expected: NOT GT_BAD_PARAM.
    1.3. Call api with wrong typePtr [NULL].
    Expected: GT_BAD_PTR.
*/
    GT_STATUS st = GT_OK;

    GT_U8                             dev;
    CPSS_DXCH_POLICER_STAGE_TYPE_ENT  stage = CPSS_DXCH_POLICER_STAGE_INGRESS_0_E;
    CPSS_DXCH_POLICER_PORT_MODE_ADDR_SELECT_TYPE_ENT    type = CPSS_DXCH_POLICER_PORT_MODE_ADDR_SELECT_TYPE_FULL_E;

    /* prepare device iterator */
    PRV_TGF_NOT_APPLIC_DEV_RESET_MAC(&dev, UTF_CH1_E | UTF_CH1_DIAMOND_E | UTF_CH2_E | UTF_CH3_E | UTF_XCAT_E | UTF_LION_E);

    /* 1. Go over all active devices. */
    while (GT_OK == prvUtfNextDeviceGet(&dev, GT_TRUE))
    {
        for (stage = CPSS_DXCH_POLICER_STAGE_INGRESS_0_E; 
              stage <= CPSS_DXCH_POLICER_STAGE_EGRESS_E; stage++) 
        {
            /*
                1.1. Call function with stage [CPSS_DXCH_POLICER_STAGE_INGRESS_0_E /
                                               CPSS_DXCH_POLICER_STAGE_INGRESS_1_E /
                                               CPSS_DXCH_POLICER_STAGE_EGRESS_E].
                Expected: GT_OK.
            */
    
            st = cpssDxChPolicerPortModeAddressSelectGet(dev, stage, &type);
            UTF_VERIFY_EQUAL1_PARAM_MAC(GT_OK, st, dev);
        }


        /*
            1.2. Call api with wrong stage [wrong enum values].
            Expected: GT_BAD_PARAM.
        */
        UTF_ENUMS_CHECK_MAC(cpssDxChPolicerPortModeAddressSelectGet
                            (dev, stage, &type),
                            stage);
        /*
            1.3. Call api with wrong enablePtr [NULL].
            Expected: GT_BAD_PTR.
        */
        st = cpssDxChPolicerPortModeAddressSelectGet(dev, stage, NULL);
        UTF_VERIFY_EQUAL1_PARAM_MAC(GT_BAD_PTR, st, dev);
    }

    /* restore correct values */
    stage = CPSS_DXCH_POLICER_STAGE_INGRESS_0_E;

    /* 2. For not-active devices and devices from non-applicable family */
    /* check that function returns GT_BAD_PARAM.                        */

    /* prepare device iterator */
    PRV_TGF_NOT_APPLIC_DEV_RESET_MAC(&dev, UTF_CH1_E | UTF_CH1_DIAMOND_E | UTF_CH2_E | UTF_CH3_E | UTF_LION_E);

    /* go over all non active devices */
    while(GT_OK == prvUtfNextDeviceGet(&dev, GT_FALSE))
    {
        st = cpssDxChPolicerPortModeAddressSelectGet(dev, stage, &type);
        UTF_VERIFY_EQUAL1_PARAM_MAC(GT_NOT_APPLICABLE_DEVICE, st, dev);
    }

    /* 3. Call function with out of bound value for device id.*/
    dev = PRV_CPSS_MAX_PP_DEVICES_CNS;

    st = cpssDxChPolicerPortModeAddressSelectGet(dev, stage, &type);
    UTF_VERIFY_EQUAL1_PARAM_MAC(GT_BAD_PARAM, st, dev);
}



/*----------------------------------------------------------------------------*/
/*
    Test function to Fill Policer table POLICER METERS
*/
UTF_TEST_CASE_MAC(cpssDxCh3PolicerFillPolicerMetersTable)
{
/*
    ITERATE_DEVICE (DxCh3)
    1.1. Get table Size.
         Call cpssDxChCfgTableNumEntriesGet with table [CPSS_DXCH_CFG_TABLE_POLICER_METERS_E]
                                                 and not NULL numEntriesPtr.
    Expected: GT_OK.
    1.2. Fill all entries in POLICER METERS table.
         Call cpssDxCh3PolicerMeteringEntrySet with entryIndex [0 ... numEntries-1],
                            entryPtr {countingEntryIndex[0],
                                      mngCounterSet[CPSS_DXCH3_POLICER_MNG_CNTR_SET0_E],
                                      meterColorMode[CPSS_POLICER_COLOR_BLIND_E],
                                      meterMode[CPSS_DXCH3_POLICER_METER_MODE_SR_TCM_E],
                                      tokenBucketParams.srTcmParams{cir[0],
                                                                    cbs[0],
                                                                    ebs[0]},
                                      modifyUp[CPSS_PACKET_ATTRIBUTE_MODIFY_KEEP_PREVIOUS_E],
                                      modifyDscp[CPSS_PACKET_ATTRIBUTE_MODIFY_KEEP_PREVIOUS_E],
                                      yellowPcktCmd[CPSS_DXCH3_POLICER_NON_CONFORM_CMD_NO_CHANGE_E],
                                      redPcktCmd[CPSS_DXCH3_POLICER_NON_CONFORM_CMD_NO_CHANGE_E]}
                           and not NULL tbParamsPtr.
    Expected: GT_OK.
    1.3. Try to write entry with index out of range.
         Call cpssDxCh3PolicerMeteringEntrySet with entryIndex [numEntries] and other param from 1.2.
    Expected: NOT GT_OK.
    1.4. Read all entries in POLICER METERS table and compare with original.
         Call cpssDxCh3PolicerMeteringEntryGet with not NULL entryPtr
                                                    and other params from 1.2.
    Expected: GT_OK and the same entryPtr as was set.
    1.5. Try to read entry with index out of range.
         Call cpssDxCh3PolicerMeteringEntryGet with entryIndex [numEntries] not NULL entryPtr
                                                    and other params from 1.2.
    Expected: NOT GT_OK.
*/
    GT_STATUS   st = GT_OK;
    GT_U8       dev;

    GT_U32      numEntries = 0;
    GT_U32      iTemp      = 0;

    CPSS_DXCH3_POLICER_METERING_ENTRY_STC   entry;
    CPSS_DXCH3_POLICER_METERING_ENTRY_STC   entryGet;
    CPSS_DXCH3_POLICER_METER_TB_PARAMS_UNT  tbParams;


    cpssOsBzero((GT_VOID*) &entry, sizeof(entry));
    cpssOsBzero((GT_VOID*) &entryGet, sizeof(entryGet));
    cpssOsBzero((GT_VOID*) &tbParams, sizeof(tbParams));

    /* Fill the entry for POLICER METERS table */
    entry.countingEntryIndex = 0;
    entry.mngCounterSet      = CPSS_DXCH3_POLICER_MNG_CNTR_SET0_E;
    entry.meterColorMode     = CPSS_POLICER_COLOR_BLIND_E;
    entry.meterMode          = CPSS_DXCH3_POLICER_METER_MODE_SR_TCM_E;

    entry.tokenBucketParams.srTcmParams.cir = 0;
    entry.tokenBucketParams.srTcmParams.cbs = 0;
    entry.tokenBucketParams.srTcmParams.ebs = 0;

    entry.modifyUp      = CPSS_PACKET_ATTRIBUTE_MODIFY_KEEP_PREVIOUS_E;
    entry.modifyDscp    = CPSS_PACKET_ATTRIBUTE_MODIFY_KEEP_PREVIOUS_E;
    entry.yellowPcktCmd = CPSS_DXCH3_POLICER_NON_CONFORM_CMD_NO_CHANGE_E;
    entry.redPcktCmd    = CPSS_DXCH3_POLICER_NON_CONFORM_CMD_NO_CHANGE_E;

    /* prepare device iterator */
    PRV_TGF_NOT_APPLIC_DEV_RESET_MAC(&dev, UTF_CH1_E | UTF_CH1_DIAMOND_E | UTF_CH2_E);

    /* 1. Go over all active devices. */
    while (GT_OK == prvUtfNextDeviceGet(&dev, GT_TRUE))
    {
        /* 1.1. Get POLICER METERS Size */
        if (! PRV_CPSS_DXCH_XCAT_FAMILY_CHECK_MAC(dev))
        {
            st = cpssDxChCfgTableNumEntriesGet(dev, CPSS_DXCH_CFG_TABLE_POLICER_METERS_E, &numEntries);
            UTF_VERIFY_EQUAL1_STRING_MAC(GT_OK, st, "cpssDxChCfgTableNumEntriesGet: %d", dev);
        }
        else
        {
            numEntries = PRV_CPSS_DXCH_PP_MAC(dev)->policer.memSize[plrStage];
        }

        /* 1.2. Fill all entries in POLICER METERS table */
        for(iTemp = 0; iTemp < numEntries; ++iTemp)
        {
            st = cpssDxCh3PolicerMeteringEntrySet(dev, plrStage, iTemp, &entry, &tbParams);
            UTF_VERIFY_EQUAL2_STRING_MAC(GT_OK, st, "cpssDxCh3PolicerMeteringEntrySet: %d, %d", dev, iTemp);
        }

        /* 1.3. Try to write entry with index out of range. */
        st = cpssDxCh3PolicerMeteringEntrySet(dev, plrStage, numEntries, &entry, &tbParams);
        UTF_VERIFY_NOT_EQUAL2_STRING_MAC(GT_OK, st, "cpssDxCh3PolicerMeteringEntrySet: %d, %d", dev, numEntries);

        /* 1.4. Read all entries in POLICER METERS table and compare with original */
        for(iTemp = 0; iTemp < numEntries; ++iTemp)
        {
            st = cpssDxCh3PolicerMeteringEntryGet(dev, plrStage, iTemp, &entryGet);
            UTF_VERIFY_EQUAL2_STRING_MAC(GT_OK, st, "cpssDxCh3PolicerMeteringEntryGet: %d, %d", dev, iTemp);

            /* verifying values */
            UTF_VERIFY_EQUAL1_STRING_MAC(entry.countingEntryIndex, entryGet.countingEntryIndex,
                       "got another entryPtr->countingEntryIndex then was set: %d", dev);
            UTF_VERIFY_EQUAL1_STRING_MAC(entry.mngCounterSet, entryGet.mngCounterSet,
                       "got another entryPtr->mngCounterSet then was set: %d", dev);
            UTF_VERIFY_EQUAL1_STRING_MAC(entry.meterColorMode, entryGet.meterColorMode,
                       "got another entryPtr->meterColorMode then was set: %d", dev);
            UTF_VERIFY_EQUAL1_STRING_MAC(entry.meterMode, entryGet.meterMode,
                       "got another entryPtr->meterMode then was set: %d", dev);

            UTF_VERIFY_EQUAL1_STRING_MAC(entry.tokenBucketParams.srTcmParams.cir, entryGet.tokenBucketParams.srTcmParams.cir,
                       "got another entryPtr->tokenBucketParams.srTcmParams.cir then was set: %d", dev);
            UTF_VERIFY_EQUAL1_STRING_MAC(entry.tokenBucketParams.srTcmParams.cbs, entryGet.tokenBucketParams.srTcmParams.cbs,
                       "got another entryPtr->tokenBucketParams.srTcmParams.cbs then was set: %d", dev);
            UTF_VERIFY_EQUAL1_STRING_MAC(entry.tokenBucketParams.srTcmParams.ebs, entryGet.tokenBucketParams.srTcmParams.ebs,
                       "got another entryPtr->tokenBucketParams.srTcmParams.ebs then was set: %d", dev);

            UTF_VERIFY_EQUAL1_STRING_MAC(entry.modifyUp, entryGet.modifyUp,
                       "got another entryPtr->modifyUp then was set: %d", dev);
            UTF_VERIFY_EQUAL1_STRING_MAC(entry.modifyDscp, entryGet.modifyDscp,
                       "got another entryPtr->modifyDscp then was set: %d", dev);
            UTF_VERIFY_EQUAL1_STRING_MAC(entry.yellowPcktCmd, entryGet.yellowPcktCmd,
                       "got another entryPtr->yellowPcktCmd then was set: %d", dev);
            UTF_VERIFY_EQUAL1_STRING_MAC(entry.redPcktCmd, entryGet.redPcktCmd,
                       "got another entryPtr->redPcktCmd then was set: %d", dev);
        }

        /* 1.5. Try to read entry with index out of range. */
        st = cpssDxCh3PolicerMeteringEntryGet(dev, plrStage, numEntries, &entryGet);
        UTF_VERIFY_NOT_EQUAL2_STRING_MAC(GT_OK, st, "cpssDxCh3PolicerMeteringEntryGet: %d, %d", dev, numEntries);
    }
}


/*----------------------------------------------------------------------------*/
/*
    Test function to Fill Policer table POLICER BILLING_COUNTERS
*/
UTF_TEST_CASE_MAC(cpssDxCh3PolicerFillPolicerBillingTable)
{
/*
    ITERATE_DEVICE (DxCh3)
    1.1. Get table Size.
         Call cpssDxChCfgTableNumEntriesGet with table [CPSS_DXCH_CFG_TABLE_POLICER_BILLING_COUNTERS_E]
                                                 and not NULL  numEntriesPtr.
    Expected: GT_OK.
    1.2. Fill all entries in POLICER BILLING_COUNTERS table.
         Call cpssDxCh3PolicerBillingEntrySet with entryIndex [0...numEntries-1],
                   billingCntrPtr{ greenCntr [0],
                                   yellowCntr [0],
                                   redCntr [0]}
    Expected: GT_OK.
    1.3. Try to write entry with index out of range.
         Call cpssDxCh3PolicerBillingEntrySet with entryIndex [numEntries] and other params from 1.2.
    Expected: NOT GT_OK.
    1.4. Read all entries in POLICER BILLING_COUNTERS table and compare with original.
         Call cpssDxCh3PolicerBillingEntryGet with not NULL billingCntrPtr,
                                                    reset [GT_FALSE]
                                                    and other params from 1.2.
    Expected: GT_OK and the same billingCntrPtr as was set
    1.5. Try to read entry with index out of range.
         Call cpssDxCh3PolicerBillingEntryGet with entryIndex [numEntries].
    Expected: NOT GT_OK.
*/
    GT_STATUS   st = GT_OK;
    GT_U8       dev;

    GT_U32      numEntries  = 0;
    GT_U32      iTemp       = 0;

    CPSS_DXCH3_POLICER_BILLING_ENTRY_STC    billingCntr;
    CPSS_DXCH3_POLICER_BILLING_ENTRY_STC    billingCntrGet;
    GT_BOOL                                 reset = GT_FALSE;


    cpssOsBzero((GT_VOID*) &billingCntr, sizeof(billingCntr));
    cpssOsBzero((GT_VOID*) &billingCntrGet, sizeof(billingCntrGet));

    /* Fill the entry for POLICER BILLING_COUNTERS table */
    billingCntr.greenCntr.l[0]  = 0;
    billingCntr.yellowCntr.l[0] = 0;
    billingCntr.redCntr.l[0] = 0;

    /* prepare device iterator */
    PRV_TGF_NOT_APPLIC_DEV_RESET_MAC(&dev, UTF_CH1_E | UTF_CH1_DIAMOND_E | UTF_CH2_E);

    /* 1. Go over all active devices. */
    while (GT_OK == prvUtfNextDeviceGet(&dev, GT_TRUE))
    {
        /* 1.1. Get POLICER BILLING_COUNTERS Size */
        if (! PRV_CPSS_DXCH_XCAT_FAMILY_CHECK_MAC(dev))
        {
            st = cpssDxChCfgTableNumEntriesGet(dev, CPSS_DXCH_CFG_TABLE_POLICER_BILLING_COUNTERS_E, &numEntries);
            UTF_VERIFY_EQUAL1_STRING_MAC(GT_OK, st, "cpssDxChCfgTableNumEntriesGet: %d", dev);
        }
        else
        {
            numEntries = PRV_CPSS_DXCH_PP_MAC(dev)->policer.memSize[plrStage];
        }

        if (PRV_CPSS_DXCH_XCAT_FAMILY_CHECK_MAC(dev))
        {
            /* previous tests may disable counting.
               enable billing counting. */
            st = cpssDxChPolicerCountingModeSet(dev,plrStage, CPSS_DXCH_POLICER_COUNTING_BILLING_IPFIX_E);
            UTF_VERIFY_EQUAL2_STRING_MAC(GT_OK, st, "cpssDxChPolicerCountingModeSet: %d, %d", dev, plrStage);
        }

        /* 1.2. Fill all entries in POLICER BILLING_COUNTERS table */
        for(iTemp = 0; iTemp < numEntries; ++iTemp)
        {
            st = cpssDxCh3PolicerBillingEntrySet(dev, plrStage, iTemp, &billingCntr);
            UTF_VERIFY_EQUAL2_STRING_MAC(GT_OK, st, "cpssDxCh3PolicerBillingEntrySet: %d, %d", dev, iTemp);
        }

        /* 1.3. Try to write entry with index out of range. */
        st = cpssDxCh3PolicerBillingEntrySet(dev, plrStage, numEntries, &billingCntr);
        UTF_VERIFY_NOT_EQUAL2_STRING_MAC(GT_OK, st, "cpssDxCh3PolicerBillingEntrySet: %d, %d", dev, numEntries);

        /* 1.4. Read all entries in POLICER BILLING_COUNTERS table and compare with original */
        for(iTemp = 0; iTemp < numEntries; ++iTemp)
        {
            st = cpssDxCh3PolicerBillingEntryGet(dev, plrStage, iTemp, reset, &billingCntrGet);
            UTF_VERIFY_EQUAL3_STRING_MAC(GT_OK, st, "cpssDxCh3PolicerBillingEntryGet: %d, %d, %d", dev, iTemp, reset);

            /* verifying values */
            UTF_VERIFY_EQUAL1_STRING_MAC(billingCntr.greenCntr.l[0], billingCntrGet.greenCntr.l[0],
                       "got another billingCntrPtr->greenCntr then was set: %d", dev);
            UTF_VERIFY_EQUAL1_STRING_MAC(billingCntr.yellowCntr.l[0], billingCntrGet.yellowCntr.l[0],
                       "got another billingCntrPtr->yellowCntr then was set: %d", dev);
            UTF_VERIFY_EQUAL1_STRING_MAC(billingCntr.redCntr.l[0], billingCntrGet.redCntr.l[0],
                       "got another billingCntrPtr->redCntr then was set: %d", dev);
        }

        /* 1.5. Try to read entry with index out of range. */
        st = cpssDxCh3PolicerBillingEntryGet(dev, plrStage, numEntries, reset,
                                             &billingCntrGet);
        UTF_VERIFY_NOT_EQUAL3_STRING_MAC(GT_OK, st, "cpssDxCh3PolicerBillingEntryGet: %d, %d, %d", dev, numEntries, reset);
    }
}

/*----------------------------------------------------------------------------*/
/*
    Test function to Fill Policer table QOS_PROFILE
*/
UTF_TEST_CASE_MAC(cpssDxCh3PolicerFillQosProfileTable)
{
/*
    ITERATE_DEVICE (DxCh3)
    1.1. Get table Size.
         Call cpssDxChCfgTableNumEntriesGet with table [CPSS_DXCH_CFG_TABLE_QOS_PROFILE_E]
                                                 and not NULL numEntriesPtr.
    Expected: GT_OK.
    1.2. Fill all entries in QOS_PROFILE table.
         Call cpssDxCh3PolicerQosRemarkingEntrySet with qosProfileIndex [0...numEntries-1],
                                                        yellowQosTableRemarkIndex [0],
                                                        redQosTableRemarkIndex [0]
    Expected: GT_OK.
    1.3. Try to write entry with index out of range.
         Call cpssDxCh3PolicerQosRemarkingEntrySet with qosProfileIndex [numEntries] and other params from 1.2.
    Expected: NOT GT_OK.
    1.4. Read all entries in QOS_PROFILE table and compare with original.
         Call cpssDxCh3PolicerQosRemarkingEntryGet  with not NULL yellowQosTableRemarkIndexandPtr
                                                         and not NULL redQosTableRemarkIndexPtr.
    Expected: GT_OK and the same yellowQosTableRemarkIndexand, redQosTableRemarkIndex as was set.
    1.5. Try to read entry with index out of range.
         Call cpssDxCh3PolicerQosRemarkingEntryGet with qosProfileIndex [numEntries]
    Expected: NOT GT_OK.
*/
    GT_STATUS   st = GT_OK;
    GT_U8       dev;

    GT_U32      numEntries  = 0;
    GT_U32      iTemp       = 0;

    GT_U32   yellowQosTableRemarkIndex    = 0;
    GT_U32   redQosTableRemarkIndex       = 0;

    GT_U32   yellowQosTableRemarkIndexGet = 0;
    GT_U32   redQosTableRemarkIndexGet    = 0;


    /* Fill the entry for QOS_PROFILE table */
    yellowQosTableRemarkIndex = 0;
    redQosTableRemarkIndex    = 0;

    /* prepare device iterator */
    PRV_TGF_NOT_APPLIC_DEV_RESET_MAC(&dev, UTF_CH1_E | UTF_CH1_DIAMOND_E | UTF_CH2_E);

    /* 1. Go over all active devices. */
    while (GT_OK == prvUtfNextDeviceGet(&dev, GT_TRUE))
    {
        /* 1.1. Get QOS_PROFILE Size */
        st = cpssDxChCfgTableNumEntriesGet(dev, CPSS_DXCH_CFG_TABLE_QOS_PROFILE_E, &numEntries);
        UTF_VERIFY_EQUAL1_STRING_MAC(GT_OK, st, "cpssDxChCfgTableNumEntriesGet: %d", dev);

        /* 1.2. Fill all entries in QOS_PROFILE table */
        for(iTemp = 0; iTemp < numEntries; ++iTemp)
        {
            st = cpssDxCh3PolicerQosRemarkingEntrySet(dev, plrStage, iTemp,
                                                      yellowQosTableRemarkIndex,
                                                      redQosTableRemarkIndex);
            UTF_VERIFY_EQUAL4_STRING_MAC(GT_OK, st, "cpssDxCh3PolicerQosRemarkingEntrySet: %d, %d, %d, %d",
                                         dev, iTemp, yellowQosTableRemarkIndex, redQosTableRemarkIndex);
        }

        /* 1.3. Try to write entry with index out of range. */
        st = cpssDxCh3PolicerQosRemarkingEntrySet(dev, plrStage, numEntries,
                                                  yellowQosTableRemarkIndex,
                                                  redQosTableRemarkIndex);
        UTF_VERIFY_NOT_EQUAL4_STRING_MAC(GT_OK, st, "cpssDxCh3PolicerQosRemarkingEntrySet: %d, %d, %d, %d",
                                     dev, numEntries, yellowQosTableRemarkIndex, redQosTableRemarkIndex);

        /* 1.4. Read all entries in QOS_PROFILE table and compare with original */
        for(iTemp = 0; iTemp < numEntries; ++iTemp)
        {
            st = cpssDxCh3PolicerQosRemarkingEntryGet(dev, plrStage, iTemp,
                                                      &yellowQosTableRemarkIndexGet,
                                                      &redQosTableRemarkIndexGet);
            UTF_VERIFY_EQUAL2_STRING_MAC(GT_OK, st,
                       "cpssDxCh3PolicerQosRemarkingEntryGet: %d, %d", dev, iTemp);

            /* verifying values */
            UTF_VERIFY_EQUAL1_STRING_MAC(yellowQosTableRemarkIndex, yellowQosTableRemarkIndexGet,
                       "got another yellowQosTableRemarkIndex then was set: %d", dev);
            UTF_VERIFY_EQUAL1_STRING_MAC(redQosTableRemarkIndex, redQosTableRemarkIndexGet,
                       "got another redQosTableRemarkIndex then was set: %d", dev);
        }

        /* 1.5. Try to read entry with index out of range. */
        st = cpssDxCh3PolicerQosRemarkingEntryGet(dev, plrStage, numEntries,
                                                  &yellowQosTableRemarkIndexGet,
                                                  &redQosTableRemarkIndexGet);
        UTF_VERIFY_NOT_EQUAL2_STRING_MAC(GT_OK, st, "cpssDxCh3PolicerQosRemarkingEntryGet: %d, %d", dev, numEntries);
    }
}
/*----------------------------------------------------------------------------*/

typedef struct{
    GT_U32  cirPir;
    GT_U32  cbsPbs;
    GT_U32  ebs;
    GT_U32  cirPirAllowedDeviation;
    GT_BOOL cbsPbsCalcOnFail;
} UTF_DXCH3_POLICER_RATE_TYPE_IMPROVE_REQUEST_STC;

typedef struct{
    GT_STATUS   st;
    GT_U32      cirPir;
    GT_U32      cbsPbs;
    GT_U32      ebs;
} UTF_DXCH3_POLICER_RATE_TYPE_IMPROVE_EXPECTED_RESULT_STC;

/* MRU is used for rate calculation checks */
#define PRV_PLR_RATE_CHECK_MRU_CNS 2048

UTF_DXCH3_POLICER_RATE_TYPE_IMPROVE_REQUEST_STC prvUtfDxXcatPolicerRateTypeImproveRequests[] =
{
    {60000,65535,65535,0,GT_FALSE},
    {100000,65535,65535,0,GT_FALSE},
    {100000,524280,524280,0,GT_FALSE},
    {100000,4194240,4194240,0,GT_FALSE},
    {100000,33553920,33553920,0,GT_FALSE},
    {100000,268431360,268431360,0,GT_FALSE},
    {100000,2147450880,2147450880,0,GT_FALSE},

    {1000000,65535,65535,0,GT_FALSE},
    {10000000,65535,65535,0,GT_FALSE},
    {100000000,65535,65535,0,GT_FALSE},
    {1000000000,65535,65535,0,GT_FALSE},

    {100000,65535,65535,10,GT_FALSE},
    {1000000,65535,65535,10,GT_FALSE},
    {10000000,65535,65535,10,GT_FALSE},
    {100000000,65535,65535,10,GT_FALSE},
    {1000000000,65535,65535,10,GT_FALSE},

    {100000,65535,65535,40,GT_FALSE},
    {1000000,65536,65536,40,GT_FALSE},
    {10000000,65536,65536,40,GT_FALSE},
    {100000000,65536,65536,40,GT_FALSE},
    {1000000000,65536,65536,40,GT_FALSE},

    {10000, 2500, 2500, 0, GT_FALSE},

    {20,400000,100000,0,GT_FALSE},
    {21,400000,100000,0,GT_FALSE},
    {21,400000,100000,0,GT_TRUE},
    {21,400000,100000,3,GT_FALSE},
    {21,400000,100000,3,GT_TRUE},
    {21,400000,100000,5,GT_FALSE},

    {29,400000,100000,0,GT_FALSE},
    {29,400000,100000,0,GT_TRUE},
    {29,400000,100000,3,GT_FALSE},
    {29,400000,100000,3,GT_TRUE},
    {29,400000,100000,5,GT_FALSE},

    {90,600000,100000,0,GT_FALSE},
    {90,600000,100000,0,GT_TRUE},
    {90,600000,100000,11,GT_FALSE},
    {90,600000,100000,11,GT_TRUE},
    {90,600000,100000,12,GT_FALSE},
    {90,600000,100000,12,GT_TRUE},

    {9,1,1,0,GT_FALSE},
    {9,1,1,0,GT_TRUE},
    {9,1,1,20,GT_FALSE},
    {9,1,1,20,GT_TRUE},
    {9,10,1,0,GT_FALSE},
    {9,10,1,0,GT_TRUE},
    {9,10,1,20,GT_FALSE},
    {9,10,1,20,GT_TRUE},

    {9,100000,100000,0,GT_FALSE},
    {9,100000,100000,0,GT_TRUE},
    {9,100000,100000,20,GT_FALSE},
    {9,100000,100000,20,GT_TRUE},
    {9,1000000,100000,0,GT_FALSE},
    {9,1000000,100000,0,GT_TRUE},
    {9,1000000,100000,20,GT_FALSE},
    {9,1000000,100000,20,GT_TRUE},

    {20,1,1,0,GT_FALSE},
    {20,1,1,0,GT_TRUE},
    {21,1,1,0,GT_FALSE},
    {21,1,1,0,GT_TRUE},

    {1023000000,4000,1000,GT_FALSE},
    {1023000000,4000,1000,0,GT_TRUE},
    {1023000000,5000,1000,0,GT_FALSE},
    {1023000000,5000,1000,0,GT_TRUE},
    {1023000000,200000000,1000,0,GT_FALSE},
    {1023000000,200000000,1000,0,GT_TRUE},
    {1023000000,300000000,1000,0,GT_FALSE},
    {1023000000,300000000,1000,0,GT_TRUE},

    {1022990000,300000000,100000,0,GT_FALSE},
    {1022990000,300000000,100000,0,GT_TRUE},
    {1022990000,300000000,100000,1,GT_FALSE},
    {1022990000,300000000,100000,1,GT_TRUE},

    {0xFFFFFFFF,40000,10000,0,GT_FALSE},
    {0xFFFFFFFF,40000,10000,0,GT_TRUE},
    {0xFFFFFFFF,40000,10000,1,GT_FALSE},
    {0xFFFFFFFF,40000,10000,1,GT_TRUE},
    {0xFFFFFFFF,50000,10000,0,GT_FALSE},
    {0xFFFFFFFF,50000,10000,0,GT_TRUE},
    {0xFFFFFFFF,50000,10000,1,GT_FALSE},
    {0xFFFFFFFF,50000,10000,1,GT_TRUE},
    {0xFFFFFFFF,70000,10000,1,GT_FALSE},
    {0xFFFFFFFF,70000,10000,1,GT_TRUE},

    {5,4,2,0,GT_FALSE},
    {5,4,2,10,GT_FALSE},
    {5,4,2,20,GT_FALSE},
    {5,4,2,40,GT_FALSE},

    {1, 1, 1, 0xFFFFFFFF, GT_FALSE},
    {0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, GT_FALSE},
    {0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, GT_TRUE}
};

UTF_DXCH3_POLICER_RATE_TYPE_IMPROVE_EXPECTED_RESULT_STC prvUtfDxXcatPolicerRateTypeImproveExpectedResults[] =
{
    {GT_OK, 60000, 65535, 65535},
    {GT_OK, 100000, 65528, 65528},
    {GT_OK, 100000, 524280, 524280},
    {GT_OK, 100000, 4194240, 4194240},
    {GT_OK, 100000, 33553920, 33553920},
    {GT_OK, 100000, 268431360, 268431360},
    {GT_OK, 100000, 2147450880, 2147450880},

    {GT_OK, 1000000, 65472, 65472},
    {GT_OK, 10000000, 65024, 65024},
    {GT_OK, 100000000, 61440, 61440},
    {GT_OK, 1000000000, 32768, 32768},

    {GT_OK, 100000, 65528, 65528},
    {GT_OK, 1000000, 65472, 65472},
    {GT_OK, 10000000, 65024, 65024},
    {GT_OK, 100000000, 61440, 61440},
    {GT_OK, 1000000000, 32768, 32768},

    {GT_OK, 65535 - PRV_PLR_RATE_CHECK_MRU_CNS, 65535, 65535},
    {GT_OK, (65536 - PRV_PLR_RATE_CHECK_MRU_CNS) * 10, 65536, 65536},
    {GT_OK, (65536 - PRV_PLR_RATE_CHECK_MRU_CNS) * 100, 65536, 65536},
    {GT_OK, (65536 - PRV_PLR_RATE_CHECK_MRU_CNS) * 1000, 65536, 65536},
    {GT_OK, (65536 - PRV_PLR_RATE_CHECK_MRU_CNS) * 10000, 65536, 65536},

    {GT_OK, 10000, 2496, 2496 },

    {GT_OK, 20, 400000, 100000},
    {GT_FAIL, 0, 0, 0},
    {GT_OK, 21, 65535, 65535},
    {GT_FAIL, 0, 0, 0},
    {GT_OK, 21, 65535, 65535},
    {GT_OK, 20, 400000, 100000},

    {GT_FAIL, 0, 0, 0},
    {GT_OK, 29, 65535, 65535},
    {GT_FAIL, 0, 0, 0},
    {GT_OK, 29, 65535, 65535},
    {GT_OK, 30, 400000, 100000},

    {GT_FAIL, 0, 0, 0},
    {GT_OK, 90, 524280, 100000},
    {GT_FAIL, 0, 0, 0},
    {GT_OK, 90, 524280, 100000},
    {GT_OK, 100, 600000, 99968},
    {GT_OK, 100, 600000, 99968},

    {GT_FAIL, 0, 0, 0},
    {GT_OK, 9, 9, 1},
    {GT_FAIL, 0, 0, 0},
    {GT_OK, 9, 9, 1},
    {GT_OK, 9, 10, 1},
    {GT_OK, 9, 10, 1},
    {GT_OK, 9, 10, 1},
    {GT_OK, 9, 10, 1},

    {GT_FAIL, 0, 0, 0},
    {GT_OK, 9, 65535, 65535},
    {GT_OK, 10, 100000, 100000},
    {GT_OK, 10, 100000, 100000},
    {GT_FAIL, 0, 0, 0},
    {GT_OK, 9, 65535, 65535},
    {GT_FAIL, 0, 0, 0},
    {GT_OK, 10, 524280, 100000},

    {GT_FAIL, 0, 0, 0},
    {GT_OK, 20, 20, 1},
    {GT_FAIL, 0, 0, 0},
    {GT_OK, 21, 21, 1},

    {GT_FAIL, 0, 0, 0},
    {GT_OK, 1023000000, 106496, 0},
    {GT_FAIL, 0, 0, 0},
    {GT_OK, 1023000000, 106496, 0},
    {GT_OK, 1023000000, 199999488, 0},
    {GT_OK, 1023000000, 199999488, 0},
    {GT_OK, 1023000000, 299991040, 0},
    {GT_OK, 1023000000, 299991040, 0},

    {GT_FAIL, 0, 0, 0},
    {GT_OK, 1022990000, 268431360, 98304},
    {GT_OK, 1022900000, 299991040, 98304},
    {GT_OK, 1022900000, 299991040, 98304},

    {GT_FAIL, 0, 0, 0},
    {GT_FAIL, 0, 0, 0},
    {GT_FAIL, 0, 0, 0},
    {GT_OK, 0xFFFEF920, 65536, 0},
    {GT_FAIL, 0, 0, 0},
    {GT_FAIL, 0, 0, 0},
    {GT_FAIL, 0, 0, 0},
    {GT_OK, 0xFFFEF920, 65536, 0},
    {GT_OK, 0xFFFEF920, 65536, 0},
    {GT_OK, 0xFFFEF920, 65536, 0},

    {GT_FAIL, 0, 0, 0},
    {GT_FAIL, 0, 0, 0},
    {GT_OK, 4, 4, 2},
    {GT_OK, 4, 4, 2},

    {GT_OK, 1, 1, 1},
    {GT_FAIL, 0, 0, 0},
    {GT_OK, 0xFFFEF920, 2147450880, 2147450880}
};

UTF_DXCH3_POLICER_RATE_TYPE_IMPROVE_REQUEST_STC prvUtfDxCh3PolicerRateTypeImproveRequests[] =
{
    {10000, 2500, 2500, 0, GT_FALSE}
};

UTF_DXCH3_POLICER_RATE_TYPE_IMPROVE_EXPECTED_RESULT_STC prvUtfDxCh3PolicerRateTypeImproveExpectedResults[] =
{
    {GT_OK, 10000, 2496, 2496}
};


/*----------------------------------------------------------------------------*/
/*
    Test function for policer rate type improvements
*/
UTF_TEST_CASE_MAC(cpssDxChPolicerMeteringCalcMethodSet)
{
/*
    ITERATE_DEVICE (DxCh3)
*/
    GT_STATUS   st = GT_OK;
    GT_U8       dev;

    GT_U32      i; /* table line iterator */
    GT_U32      numOfTests;

    CPSS_DXCH_POLICER_METERING_CALC_METHOD_ENT   meteringCalcMethod;
    GT_U32                                       cirPirAllowedDeviation;
    GT_BOOL                                      cbsPbsCalcOnFail;

    CPSS_DXCH3_POLICER_METERING_ENTRY_STC   entry, entryGet;
    CPSS_DXCH3_POLICER_METER_TB_PARAMS_UNT  tbInParams;
    CPSS_DXCH3_POLICER_METER_TB_PARAMS_UNT  tbOutParams;
    UTF_DXCH3_POLICER_RATE_TYPE_IMPROVE_REQUEST_STC         *testRequestPtr;
    UTF_DXCH3_POLICER_RATE_TYPE_IMPROVE_EXPECTED_RESULT_STC *testExpectedResultPtr;

    /* prepare device iterator */
    PRV_TGF_NOT_APPLIC_DEV_RESET_MAC(&dev, UTF_CH1_E | UTF_CH1_DIAMOND_E | UTF_CH2_E);

    /* 1. Go over all active devices. */
    while (GT_OK == prvUtfNextDeviceGet(&dev, GT_TRUE))
    {
        if ( PRV_CPSS_DXCH_XCAT_FAMILY_CHECK_MAC(dev))
        {
            testRequestPtr = &prvUtfDxXcatPolicerRateTypeImproveRequests[0];
            testExpectedResultPtr = &prvUtfDxXcatPolicerRateTypeImproveExpectedResults[0];
            numOfTests = sizeof(prvUtfDxXcatPolicerRateTypeImproveRequests)/
                            sizeof(prvUtfDxXcatPolicerRateTypeImproveRequests[0]);
        }
        else
        {
            testRequestPtr = &prvUtfDxCh3PolicerRateTypeImproveRequests[0];
            testExpectedResultPtr = &prvUtfDxCh3PolicerRateTypeImproveExpectedResults[0];
            numOfTests = sizeof(prvUtfDxCh3PolicerRateTypeImproveRequests)/
                            sizeof(prvUtfDxCh3PolicerRateTypeImproveRequests[0]);
        }

        cpssOsBzero((GT_VOID*) &entry, sizeof(entry));
        cpssOsBzero((GT_VOID*) &entryGet, sizeof(entryGet));
        entry.countingEntryIndex = 0;
        entry.mngCounterSet      = CPSS_DXCH3_POLICER_MNG_CNTR_SET0_E;
        entry.meterColorMode     = CPSS_POLICER_COLOR_BLIND_E;
        entry.meterMode          = CPSS_DXCH3_POLICER_METER_MODE_SR_TCM_E;
        entry.modifyUp           = CPSS_PACKET_ATTRIBUTE_MODIFY_KEEP_PREVIOUS_E;
        entry.modifyDscp         = CPSS_PACKET_ATTRIBUTE_MODIFY_KEEP_PREVIOUS_E;
        entry.yellowPcktCmd      = CPSS_DXCH3_POLICER_NON_CONFORM_CMD_NO_CHANGE_E;
        entry.redPcktCmd         = CPSS_DXCH3_POLICER_NON_CONFORM_CMD_NO_CHANGE_E;

        /* set MRU 2K */
        st = cpssDxCh3PolicerMruSet(dev, plrStage, PRV_PLR_RATE_CHECK_MRU_CNS);
        UTF_VERIFY_EQUAL3_STRING_MAC(GT_OK, st, "cpssDxCh3PolicerMruSet: %d %d %d",
                                                dev,
                                                plrStage,
                                                PRV_PLR_RATE_CHECK_MRU_CNS);

        for( i = 0 ; i < numOfTests ; i++ )
        {
            st = cpssDxChPolicerMeteringCalcMethodSet(dev,
                                                      CPSS_DXCH_POLICER_METERING_CALC_METHOD_CIR_AND_CBS_E,
                                                      testRequestPtr->cirPirAllowedDeviation,
                                                      testRequestPtr->cbsPbsCalcOnFail);

            UTF_VERIFY_EQUAL3_STRING_MAC(GT_OK, st, "cpssDxChPolicerMeteringCalcMethodSet: %d %d %d",
                                                    dev,
                                                    testRequestPtr->cirPirAllowedDeviation,
                                                    testRequestPtr->cbsPbsCalcOnFail);

            st = cpssDxChPolicerMeteringCalcMethodGet(dev,
                                                      &meteringCalcMethod,
                                                      &cirPirAllowedDeviation,
                                                      &cbsPbsCalcOnFail);

            UTF_VERIFY_EQUAL3_STRING_MAC(GT_OK, st, "cpssDxChPolicerMeteringCalcMethodGet: %d %d %d",
                                                    dev,
                                                    testRequestPtr->cirPirAllowedDeviation,
                                                    testRequestPtr->cbsPbsCalcOnFail);

            /* verifying values */
            UTF_VERIFY_EQUAL3_STRING_MAC(CPSS_DXCH_POLICER_METERING_CALC_METHOD_CIR_AND_CBS_E, meteringCalcMethod,
                       "got another meteringCalcMethod then was set: %d %d %d",
                                                                        dev,
                                                                        testRequestPtr->cirPirAllowedDeviation,
                                                                        testRequestPtr->cbsPbsCalcOnFail);
            UTF_VERIFY_EQUAL3_STRING_MAC(testRequestPtr->cirPirAllowedDeviation, cirPirAllowedDeviation,
                       "got another cirPirAllowedDeviation then was set: %d %d %d",
                                                                        dev,
                                                                        testRequestPtr->cirPirAllowedDeviation,
                                                                        testRequestPtr->cbsPbsCalcOnFail);
            UTF_VERIFY_EQUAL3_STRING_MAC(testRequestPtr->cbsPbsCalcOnFail, cbsPbsCalcOnFail,
                       "got another cbsPbsCalcOnFail then was set: %d %d %d",
                                                                    dev,
                                                                    testRequestPtr->cirPirAllowedDeviation,
                                                                    testRequestPtr->cbsPbsCalcOnFail);

            tbInParams.srTcmParams.cir = testRequestPtr->cirPir;
            tbInParams.srTcmParams.cbs = testRequestPtr->cbsPbs;
            tbInParams.srTcmParams.ebs = testRequestPtr->ebs;

            st = cpssDxCh3PolicerEntryMeterParamsCalculate(dev,
                                                           plrStage,
                                                           CPSS_DXCH3_POLICER_METER_MODE_SR_TCM_E,
                                                           &tbInParams,
                                                           &tbOutParams);

            if( GT_FAIL == testExpectedResultPtr->st )
            {
                UTF_VERIFY_EQUAL6_STRING_MAC(GT_FAIL, st, "cpssDxCh3PolicerEntryMeterParamsCalculate: %d %d %d %d %d %d",
                                                          dev,
                                                          tbInParams.srTcmParams.cir,
                                                          tbInParams.srTcmParams.cbs,
                                                          tbInParams.srTcmParams.ebs,
                                                          testRequestPtr->cirPirAllowedDeviation,
                                                          testRequestPtr->cbsPbsCalcOnFail);
            }
            else
            {
                UTF_VERIFY_EQUAL6_STRING_MAC(GT_OK, st, "cpssDxCh3PolicerEntryMeterParamsCalculate: %d %d %d %d %d %d",
                                                        dev,
                                                        tbInParams.srTcmParams.cir,
                                                        tbInParams.srTcmParams.cbs,
                                                        tbInParams.srTcmParams.ebs,
                                                        testRequestPtr->cirPirAllowedDeviation,
                                                        testRequestPtr->cbsPbsCalcOnFail);

                /* verifying values */
                UTF_VERIFY_EQUAL6_STRING_MAC(testExpectedResultPtr->cirPir, tbOutParams.srTcmParams.cir,
                       "got another cirPir then was expected: %d %d %d %d %d %d",
                                                            dev,
                                                            tbInParams.srTcmParams.cir,
                                                            tbInParams.srTcmParams.cbs,
                                                            tbInParams.srTcmParams.ebs,
                                                            testRequestPtr->cirPirAllowedDeviation,
                                                            testRequestPtr->cbsPbsCalcOnFail);
                UTF_VERIFY_EQUAL6_STRING_MAC(testExpectedResultPtr->cbsPbs, tbOutParams.srTcmParams.cbs,
                       "got another cbsPbs then was expected: %d %d %d %d %d %d",
                                                                    dev,
                                                                    tbInParams.srTcmParams.cir,
                                                                    tbInParams.srTcmParams.cbs,
                                                                    tbInParams.srTcmParams.ebs,
                                                                    testRequestPtr->cirPirAllowedDeviation,
                                                                    testRequestPtr->cbsPbsCalcOnFail);
                UTF_VERIFY_EQUAL6_STRING_MAC(testExpectedResultPtr->ebs, tbOutParams.srTcmParams.ebs,
                       "got another ebs then was expected: %d %d %d %d %d %d",
                                                                    dev,
                                                                    tbInParams.srTcmParams.cir,
                                                                    tbInParams.srTcmParams.cbs,
                                                                    tbInParams.srTcmParams.ebs,
                                                                    testRequestPtr->cirPirAllowedDeviation,
                                                                    testRequestPtr->cbsPbsCalcOnFail);
            }

            entry.tokenBucketParams.srTcmParams.cir = tbInParams.srTcmParams.cir;
            entry.tokenBucketParams.srTcmParams.cbs = tbInParams.srTcmParams.cbs;
            entry.tokenBucketParams.srTcmParams.ebs = tbInParams.srTcmParams.ebs;

            st = cpssDxCh3PolicerMeteringEntrySet(dev, plrStage, 0, &entry, &tbOutParams);
            if( GT_FAIL == testExpectedResultPtr->st )
            {
                UTF_VERIFY_EQUAL6_STRING_MAC(GT_FAIL, st, "cpssDxCh3PolicerMeteringEntrySet: %d %d %d %d %d %d",
                                                          dev,
                                                          tbInParams.srTcmParams.cir,
                                                          tbInParams.srTcmParams.cbs,
                                                          tbInParams.srTcmParams.ebs,
                                                          testRequestPtr->cirPirAllowedDeviation,
                                                          testRequestPtr->cbsPbsCalcOnFail);
            }
            else
            {
                UTF_VERIFY_EQUAL6_STRING_MAC(GT_OK, st, "cpssDxCh3PolicerMeteringEntrySet: %d %d %d %d %d %d",
                                                        dev,
                                                        tbInParams.srTcmParams.cir,
                                                        tbInParams.srTcmParams.cbs,
                                                        tbInParams.srTcmParams.ebs,
                                                        testRequestPtr->cirPirAllowedDeviation,
                                                        testRequestPtr->cbsPbsCalcOnFail);

                /* verifying values */
                UTF_VERIFY_EQUAL6_STRING_MAC(testExpectedResultPtr->cirPir, tbOutParams.srTcmParams.cir,
                       "got another entrySet.cirPir then was expected: %d %d %d %d %d %d",
                                                            dev,
                                                            tbInParams.srTcmParams.cir,
                                                            tbInParams.srTcmParams.cbs,
                                                            tbInParams.srTcmParams.ebs,
                                                            testRequestPtr->cirPirAllowedDeviation,
                                                            testRequestPtr->cbsPbsCalcOnFail);
                UTF_VERIFY_EQUAL6_STRING_MAC(testExpectedResultPtr->cbsPbs, tbOutParams.srTcmParams.cbs,
                       "got another entrySet.cbsPbs then was expected: %d %d %d %d %d %d",
                                                                    dev,
                                                                    tbInParams.srTcmParams.cir,
                                                                    tbInParams.srTcmParams.cbs,
                                                                    tbInParams.srTcmParams.ebs,
                                                                    testRequestPtr->cirPirAllowedDeviation,
                                                                    testRequestPtr->cbsPbsCalcOnFail);
                UTF_VERIFY_EQUAL6_STRING_MAC(testExpectedResultPtr->ebs, tbOutParams.srTcmParams.ebs,
                       "got another entrySet.ebs then was expected: %d %d %d %d %d %d",
                                                                    dev,
                                                                    tbInParams.srTcmParams.cir,
                                                                    tbInParams.srTcmParams.cbs,
                                                                    tbInParams.srTcmParams.ebs,
                                                                    testRequestPtr->cirPirAllowedDeviation,
                                                                    testRequestPtr->cbsPbsCalcOnFail);

                st = cpssDxCh3PolicerMeteringEntryGet(dev, plrStage, 0, &entryGet);
                UTF_VERIFY_EQUAL6_STRING_MAC(GT_OK, st, "cpssDxCh3PolicerMeteringEntryGet: %d %d %d %d %d %d",
                                                        dev,
                                                        tbInParams.srTcmParams.cir,
                                                        tbInParams.srTcmParams.cbs,
                                                        tbInParams.srTcmParams.ebs,
                                                        testRequestPtr->cirPirAllowedDeviation,
                                                        testRequestPtr->cbsPbsCalcOnFail);

                /* verifying values */
                UTF_VERIFY_EQUAL6_STRING_MAC(testExpectedResultPtr->cirPir,
                                             entryGet.tokenBucketParams.srTcmParams.cir,
                       "got another entryGet.cirPir then was expected: %d %d %d %d %d %d",
                                                            dev,
                                                            tbInParams.srTcmParams.cir,
                                                            tbInParams.srTcmParams.cbs,
                                                            tbInParams.srTcmParams.ebs,
                                                            testRequestPtr->cirPirAllowedDeviation,
                                                            testRequestPtr->cbsPbsCalcOnFail);
                UTF_VERIFY_EQUAL6_STRING_MAC(testExpectedResultPtr->cbsPbs,
                                             entryGet.tokenBucketParams.srTcmParams.cbs,
                       "got another entryGet.cbsPbs then was expected: %d %d %d %d %d %d",
                                                                    dev,
                                                                    tbInParams.srTcmParams.cir,
                                                                    tbInParams.srTcmParams.cbs,
                                                                    tbInParams.srTcmParams.ebs,
                                                                    testRequestPtr->cirPirAllowedDeviation,
                                                                    testRequestPtr->cbsPbsCalcOnFail);
                UTF_VERIFY_EQUAL6_STRING_MAC(testExpectedResultPtr->ebs,
                                             entryGet.tokenBucketParams.srTcmParams.ebs,
                       "got another entryGet.ebs then was expected: %d %d %d %d %d %d",
                                                                    dev,
                                                                    tbInParams.srTcmParams.cir,
                                                                    tbInParams.srTcmParams.cbs,
                                                                    tbInParams.srTcmParams.ebs,
                                                                    testRequestPtr->cirPirAllowedDeviation,
                                                                    testRequestPtr->cbsPbsCalcOnFail);

            }

            testRequestPtr++;
            testExpectedResultPtr++;
        }
    }
}

/*----------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/
/*
    Configuration of cpssDxCh3Policer suit
*/
UTF_SUIT_BEGIN_TESTS_MAC(cpssDxCh3Policer)
    UTF_SUIT_DECLARE_TEST_MAC(cpssDxCh3PolicerMeteringEnableSet)
    UTF_SUIT_DECLARE_TEST_MAC(cpssDxCh3PolicerMeteringEnableGet)
    UTF_SUIT_DECLARE_TEST_MAC(cpssDxCh3PolicerBillingCountingEnableSet)
    UTF_SUIT_DECLARE_TEST_MAC(cpssDxCh3PolicerBillingCountingEnableGet)
    UTF_SUIT_DECLARE_TEST_MAC(cpssDxCh3PolicerBillingCountersModeSet)
    UTF_SUIT_DECLARE_TEST_MAC(cpssDxCh3PolicerBillingCountersModeGet)
    UTF_SUIT_DECLARE_TEST_MAC(cpssDxCh3PolicerPacketSizeModeSet)
    UTF_SUIT_DECLARE_TEST_MAC(cpssDxCh3PolicerPacketSizeModeGet)
    UTF_SUIT_DECLARE_TEST_MAC(cpssDxCh3PolicerMeterResolutionSet)
    UTF_SUIT_DECLARE_TEST_MAC(cpssDxCh3PolicerMeterResolutionGet)
    UTF_SUIT_DECLARE_TEST_MAC(cpssDxCh3PolicerDropTypeSet)
    UTF_SUIT_DECLARE_TEST_MAC(cpssDxCh3PolicerDropTypeGet)
    UTF_SUIT_DECLARE_TEST_MAC(cpssDxCh3PolicerCountingColorModeSet)
    UTF_SUIT_DECLARE_TEST_MAC(cpssDxCh3PolicerCountingColorModeGet)
    UTF_SUIT_DECLARE_TEST_MAC(cpssDxCh3PolicerManagementCntrsResolutionSet)
    UTF_SUIT_DECLARE_TEST_MAC(cpssDxCh3PolicerManagementCntrsResolutionGet)
    UTF_SUIT_DECLARE_TEST_MAC(cpssDxCh3PolicerPacketSizeModeForTunnelTermSet)
    UTF_SUIT_DECLARE_TEST_MAC(cpssDxCh3PolicerPacketSizeModeForTunnelTermGet)
    UTF_SUIT_DECLARE_TEST_MAC(cpssDxCh3PolicerMeteringAutoRefreshScanEnableSet)
    UTF_SUIT_DECLARE_TEST_MAC(cpssDxCh3PolicerMeteringAutoRefreshScanEnableGet)
    UTF_SUIT_DECLARE_TEST_MAC(cpssDxCh3PolicerMeteringAutoRefreshRangeSet)
    UTF_SUIT_DECLARE_TEST_MAC(cpssDxCh3PolicerMeteringAutoRefreshRangeGet)
    UTF_SUIT_DECLARE_TEST_MAC(cpssDxCh3PolicerMeteringAutoRefreshIntervalSet)
    UTF_SUIT_DECLARE_TEST_MAC(cpssDxCh3PolicerMeteringAutoRefreshIntervalGet)
    UTF_SUIT_DECLARE_TEST_MAC(cpssDxCh3PolicerMeteringEntryRefresh)
    UTF_SUIT_DECLARE_TEST_MAC(cpssDxCh3PolicerPortMeteringEnableSet)
    UTF_SUIT_DECLARE_TEST_MAC(cpssDxCh3PolicerPortMeteringEnableGet)
    UTF_SUIT_DECLARE_TEST_MAC(cpssDxCh3PolicerMruSet)
    UTF_SUIT_DECLARE_TEST_MAC(cpssDxCh3PolicerMruGet)
    UTF_SUIT_DECLARE_TEST_MAC(cpssDxCh3PolicerErrorGet)
    UTF_SUIT_DECLARE_TEST_MAC(cpssDxCh3PolicerErrorCounterGet)
    UTF_SUIT_DECLARE_TEST_MAC(cpssDxCh3PolicerManagementCountersSet)
    UTF_SUIT_DECLARE_TEST_MAC(cpssDxCh3PolicerManagementCountersGet)
    UTF_SUIT_DECLARE_TEST_MAC(cpssDxCh3PolicerMeteringEntrySet)
    UTF_SUIT_DECLARE_TEST_MAC(cpssDxCh3PolicerMeteringEntryGet)
    UTF_SUIT_DECLARE_TEST_MAC(cpssDxCh3PolicerEntryMeterParamsCalculate)
    UTF_SUIT_DECLARE_TEST_MAC(cpssDxCh3PolicerBillingEntrySet)
    UTF_SUIT_DECLARE_TEST_MAC(cpssDxCh3PolicerBillingEntryGet)
    UTF_SUIT_DECLARE_TEST_MAC(cpssDxCh3PolicerQosRemarkingEntrySet)
    UTF_SUIT_DECLARE_TEST_MAC(cpssDxCh3PolicerQosRemarkingEntryGet)
    UTF_SUIT_DECLARE_TEST_MAC(cpssDxChPolicerMeteringCalcMethodSet)

    UTF_SUIT_DECLARE_TEST_MAC(cpssDxChPolicerPortGroupMeteringEntrySet)
    UTF_SUIT_DECLARE_TEST_MAC(cpssDxChPolicerPortGroupPolicyCntrGet)
    UTF_SUIT_DECLARE_TEST_MAC(cpssDxChPolicerPortGroupPolicyCntrSet)
    UTF_SUIT_DECLARE_TEST_MAC(cpssDxChPolicerPortStormTypeIndexGet)
    UTF_SUIT_DECLARE_TEST_MAC(cpssDxChPolicerPortStormTypeIndexSet)
    UTF_SUIT_DECLARE_TEST_MAC(cpssDxChPolicerStageMeterModeGet)
    UTF_SUIT_DECLARE_TEST_MAC(cpssDxChPolicerStageMeterModeSet)
    UTF_SUIT_DECLARE_TEST_MAC(cpssDxChPolicerVlanCntrGet)
    UTF_SUIT_DECLARE_TEST_MAC(cpssDxChPolicerVlanCntrSet)
    UTF_SUIT_DECLARE_TEST_MAC(cpssDxChPolicerVlanCountingModeGet)
    UTF_SUIT_DECLARE_TEST_MAC(cpssDxChPolicerVlanCountingModeSet)
    UTF_SUIT_DECLARE_TEST_MAC(cpssDxChPolicerVlanCountingPacketCmdTriggerGet)
    UTF_SUIT_DECLARE_TEST_MAC(cpssDxChPolicerVlanCountingPacketCmdTriggerSet)

    UTF_SUIT_DECLARE_TEST_MAC(cpssDxChPolicerCountingModeGet)
    UTF_SUIT_DECLARE_TEST_MAC(cpssDxChPolicerCountingModeSet)
    UTF_SUIT_DECLARE_TEST_MAC(cpssDxChPolicerCountingTriggerByPortEnableGet)
    UTF_SUIT_DECLARE_TEST_MAC(cpssDxChPolicerCountingTriggerByPortEnableSet)
    UTF_SUIT_DECLARE_TEST_MAC(cpssDxChPolicerCountingWriteBackCacheFlush)
    UTF_SUIT_DECLARE_TEST_MAC(cpssDxChPolicerEgressL2RemarkModelGet)
    UTF_SUIT_DECLARE_TEST_MAC(cpssDxChPolicerEgressL2RemarkModelSet)
    UTF_SUIT_DECLARE_TEST_MAC(cpssDxChPolicerEgressQosRemarkingEntryGet)
    UTF_SUIT_DECLARE_TEST_MAC(cpssDxChPolicerEgressQosRemarkingEntrySet)
    UTF_SUIT_DECLARE_TEST_MAC(cpssDxChPolicerEgressQosUpdateEnableGet)
    UTF_SUIT_DECLARE_TEST_MAC(cpssDxChPolicerEgressQosUpdateEnableSet)
    UTF_SUIT_DECLARE_TEST_MAC(cpssDxChPolicerMemorySizeModeGet)
    UTF_SUIT_DECLARE_TEST_MAC(cpssDxChPolicerMemorySizeModeSet)
    UTF_SUIT_DECLARE_TEST_MAC(cpssDxChPolicerMeteringCalcMethodGet)
    UTF_SUIT_DECLARE_TEST_MAC(cpssDxChPolicerMeteringOnTrappedPktsEnableGet)
    UTF_SUIT_DECLARE_TEST_MAC(cpssDxChPolicerMeteringOnTrappedPktsEnableSet)
    UTF_SUIT_DECLARE_TEST_MAC(cpssDxChPolicerPolicyCntrGet)
    UTF_SUIT_DECLARE_TEST_MAC(cpssDxChPolicerPolicyCntrSet)
    UTF_SUIT_DECLARE_TEST_MAC(cpssDxChPolicerPortGroupBillingEntryGet)
    UTF_SUIT_DECLARE_TEST_MAC(cpssDxChPolicerPortGroupBillingEntrySet)
    UTF_SUIT_DECLARE_TEST_MAC(cpssDxChPolicerPortGroupErrorCounterGet)
    UTF_SUIT_DECLARE_TEST_MAC(cpssDxChPolicerPortGroupErrorGet)
    UTF_SUIT_DECLARE_TEST_MAC(cpssDxChPolicerPortGroupManagementCountersGet)
    UTF_SUIT_DECLARE_TEST_MAC(cpssDxChPolicerPortGroupManagementCountersSet)
    UTF_SUIT_DECLARE_TEST_MAC(cpssDxChPolicerPortGroupMeteringEntryGet)
    UTF_SUIT_DECLARE_TEST_MAC(cpssDxChPolicerPortGroupMeteringEntryRefresh)
    UTF_SUIT_DECLARE_TEST_MAC(cpssDxChPolicerPortModeAddressSelectSet)
    UTF_SUIT_DECLARE_TEST_MAC(cpssDxChPolicerPortModeAddressSelectGet)


    /* Tests for Tables */
    UTF_SUIT_DECLARE_TEST_MAC(cpssDxCh3PolicerFillPolicerMetersTable)
    UTF_SUIT_DECLARE_TEST_MAC(cpssDxCh3PolicerFillPolicerBillingTable)
    UTF_SUIT_DECLARE_TEST_MAC(cpssDxCh3PolicerFillQosProfileTable)
UTF_SUIT_END_TESTS_MAC(cpssDxCh3Policer)

