/*******************************************************************************
*              (c), Copyright 2001, Marvell International Ltd.                 *
* THIS CODE CONTAINS CONFIDENTIAL INFORMATION OF MARVELL SEMICONDUCTOR, INC.   *
* NO RIGHTS ARE GRANTED HEREIN UNDER ANY PATENT, MASK WORK RIGHT OR COPYRIGHT  *
* OF MARVELL OR ANY THIRD PARTY. MARVELL RESERVES THE RIGHT AT ITS SOLE        *
* DISCRETION TO REQUEST THAT THIS CODE BE IMMEDIATELY RETURNED TO MARVELL.     *
* THIS CODE IS PROVIDED "AS IS". MARVELL MAKES NO WARRANTIES, EXPRESSED,       *
* IMPLIED OR OTHERWISE, REGARDING ITS ACCURACY, COMPLETENESS OR PERFORMANCE.   *
********************************************************************************
* cpssDxChPortTxUT.c
*
* DESCRIPTION:
*       Unit tests for cpssDxChPortTx, that provides
*       CPSS implementation for configuring the Physical Port Tx Traffic Class.
*
* FILE REVISION NUMBER:
*       $Revision: 1.1.1.1 $
*******************************************************************************/
/* includes */

#include <cpss/dxCh/dxChxGen/config/private/prvCpssDxChInfo.h>
#include <cpss/dxCh/dxChxGen/port/cpssDxChPortTx.h>

#include <utf/utfMain.h>
#include <utf/private/prvUtfExtras.h>
#include <common/tgfCommon.h>

/* defines */

/* Default valid value for port id */
#define PORT_TX_VALID_PHY_PORT_CNS  0

#define PORT_TX_INVALID_MIRR_2_ANLY_DESC_MAX_CNS  0xFFFF

/*----------------------------------------------------------------------------*/
/*
GT_STATUS cpssDxChPortTxQueueEnableSet
(
    IN GT_U8        dev,
    IN GT_BOOL      enable
)
*/
UTF_TEST_CASE_MAC(cpssDxChPortTxQueueEnableSet)
{
    /*
    ITERATE_DEVICES
    1.1. Call function with enable [GT_FALSE and GT_TRUE]. Expected: GT_OK for Cheetah devices and GT_BAD_PARAM for others.
    */

    GT_STATUS   st     = GT_OK;

    GT_U8       dev;
    GT_BOOL     enable = GT_TRUE;
    GT_BOOL     enableGet;

    /* prepare device iterator */
    PRV_TGF_NOT_APPLIC_DEV_RESET_MAC(&dev, UTF_NONE_FAMILY_E);
    /* 1. Go over all active devices. */
    while (GT_OK == prvUtfNextDeviceGet(&dev, GT_TRUE))
    {
        /* 1.1. Call with enable [GT_FALSE and GT_TRUE].
           Expected: GT_OK.
        */

        /* Call function with enable = GT_FALSE */
        enable = GT_FALSE;

        st = cpssDxChPortTxQueueEnableSet(dev, enable);
        UTF_VERIFY_EQUAL2_PARAM_MAC(GT_OK, st, dev, enable);

        st = cpssDxChPortTxQueueEnableGet(dev, &enableGet);
        UTF_VERIFY_EQUAL1_PARAM_MAC(GT_OK, st, dev);

        /* verifying values */
        UTF_VERIFY_EQUAL1_STRING_MAC(enable, enableGet,
                   "got another Queue status then was set: %d", dev);

        /* Call function with enable = GT_TRUE */
        enable = GT_TRUE;

        st = cpssDxChPortTxQueueEnableSet(dev, enable);
        UTF_VERIFY_EQUAL2_PARAM_MAC(GT_OK, st, dev, enable);

        st = cpssDxChPortTxQueueEnableGet(dev, &enableGet);
        UTF_VERIFY_EQUAL1_PARAM_MAC(GT_OK, st, dev);

        /* verifying values */
        UTF_VERIFY_EQUAL1_STRING_MAC(enable, enableGet,
                   "got another Queue status then was set: %d", dev);
    }

    /* 2. For not-active devices and devices from non-applicable family */
    /* check that function returns GT_BAD_PARAM.                        */

    /* prepare device iterator */
    PRV_TGF_NOT_APPLIC_DEV_RESET_MAC(&dev, UTF_NONE_FAMILY_E);

    /* go over all non active devices */
    /* enable == GT_TRUE              */
    while(GT_OK == prvUtfNextDeviceGet(&dev, GT_FALSE))
    {
        st = cpssDxChPortTxQueueEnableSet(dev, enable);
        UTF_VERIFY_EQUAL1_PARAM_MAC(GT_NOT_APPLICABLE_DEVICE, st, dev);
    }

    /* 3. Call function with out of bound value for device id.*/

    dev = PRV_CPSS_MAX_PP_DEVICES_CNS;
    /* enable == GT_TRUE */

    st = cpssDxChPortTxQueueEnableSet(dev, enable);
    UTF_VERIFY_EQUAL1_PARAM_MAC(GT_BAD_PARAM, st, dev);
}

/*----------------------------------------------------------------------------*/
/*
GT_STATUS cpssDxChPortTxQueueingEnableSet
(
    IN  GT_U8     devNum,
    IN  GT_U8     portNum,
    IN  GT_U8     tcQueue,
    IN  GT_BOOL   enable
)
*/
UTF_TEST_CASE_MAC(cpssDxChPortTxQueueingEnableSet)
{
    /*
    ITERATE_DEVICES_PHY_CPU_PORTS (DxChx)
    1.1.1. Call with enable [GT_FALSE and GT_TRUE] and tcQueue [0, 4].
    Expected: GT_OK.
    1.1.2. Call with enable [GT_FALSE and GT_TRUE] and out of range tcQueue [CPSS_TC_RANGE_CNS  = 8]
    Expected: GT_BAD_PARAM.
    */

    GT_STATUS   st      = GT_OK;

    GT_U8       dev;
    GT_U8       port    = PORT_TX_VALID_PHY_PORT_CNS;
    GT_U8       tcQueue = 0;
    GT_BOOL     enable  = GT_TRUE;
    GT_BOOL     enableGet;

    /* prepare iterator for go over all active devices */
    PRV_TGF_NOT_APPLIC_DEV_RESET_MAC(&dev, UTF_NONE_FAMILY_E);
    /* 1. Go over all active devices. */
    while (GT_OK == prvUtfNextDeviceGet(&dev, GT_TRUE))
    {
        st = prvUtfNextPhyPortReset(&port, dev);
        UTF_VERIFY_EQUAL0_PARAM_MAC(GT_OK, st);
        /* 1.1. For all active devices go over all available physical ports. */
        while (GT_OK == prvUtfNextPhyPortGet(&port, GT_TRUE))
        {
            /* 1.1.1 Call function for with enable = GT_FALSE and GT_TRUE
               tcQueue [0, 4]
               Expected: GT_OK.
            */

            /* Call function with enable = GT_FALSE, tcQueue = 0*/
            enable = GT_FALSE;
            tcQueue = 0;

            st = cpssDxChPortTxQueueingEnableSet(dev, port, tcQueue, enable);
            UTF_VERIFY_EQUAL4_PARAM_MAC(GT_OK, st, dev, port, tcQueue, enable);

            st = cpssDxChPortTxQueueingEnableGet(dev, port, tcQueue, &enableGet);
            UTF_VERIFY_EQUAL3_PARAM_MAC(GT_OK, st, dev, port, tcQueue);

            /* verifying values */
            UTF_VERIFY_EQUAL1_STRING_MAC(enable, enableGet,
                       "got another Queueing status then was set: %d", dev);

            /* Call function with enable = GT_TRUE, tcQueue = 4 */
            enable = GT_TRUE;
            tcQueue = 4;

            st = cpssDxChPortTxQueueingEnableSet(dev, port, tcQueue, enable);
            UTF_VERIFY_EQUAL4_PARAM_MAC(GT_OK, st, dev, port, tcQueue, enable);

            st = cpssDxChPortTxQueueingEnableGet(dev, port, tcQueue, &enableGet);
            UTF_VERIFY_EQUAL3_PARAM_MAC(GT_OK, st, dev, port, tcQueue);

            /* verifying values */
            UTF_VERIFY_EQUAL1_STRING_MAC(enable, enableGet,
                       "got another Queueing status then was set: %d", dev);

            /* 1.1.2 Call with enable [GT_FALSE and GT_TRUE]
               out of range tcQueue [CPSS_TC_RANGE_CNS = 8]
               Expected: GT_BAD_PARAM.
            */

            /* Call function with enable = GT_FALSE, tcQueue = CPSS_TC_RANGE_CNS */
            enable = GT_FALSE;
            tcQueue = CPSS_TC_RANGE_CNS;

            st = cpssDxChPortTxQueueingEnableSet(dev, port, tcQueue, enable);
            UTF_VERIFY_EQUAL4_PARAM_MAC(GT_BAD_PARAM, st, dev, port, tcQueue, enable);

            /* Call function with enable = GT_TRUE, tcQueue = CPSS_TC_RANGE_CNS*/
            enable = GT_TRUE;
            tcQueue = CPSS_TC_RANGE_CNS;

            st = cpssDxChPortTxQueueingEnableSet(dev, port, tcQueue, enable);
            UTF_VERIFY_EQUAL4_PARAM_MAC(GT_BAD_PARAM, st, dev, port, tcQueue, enable);
        }

        enable = GT_TRUE;
        tcQueue = 2;

        st = prvUtfNextPhyPortReset(&port, dev);
        UTF_VERIFY_EQUAL0_PARAM_MAC(GT_OK, st);
        /* 1.2. For all active devices go over all non available physical ports. */
        while (GT_OK == prvUtfNextPhyPortGet(&port, GT_FALSE))
        {
            /* 1.2.1. Call function for each non-active port */
            /* enable = GT_TRUE, tcQueue = 2                 */
            st = cpssDxChPortTxQueueingEnableSet(dev, port, tcQueue, enable);
            UTF_VERIFY_EQUAL2_PARAM_MAC(GT_BAD_PARAM, st, dev, port);
        }

        /* 1.3. For active device check that function returns GT_BAD_PARAM */
        /* for out of bound value for port number.                         */
        /* enable == GT_TRUE, tcQueue == 2                                 */
        port = PRV_CPSS_MAX_PP_PORTS_NUM_CNS;

        st = cpssDxChPortTxQueueingEnableSet(dev, port, tcQueue, enable);
        UTF_VERIFY_EQUAL2_PARAM_MAC(GT_BAD_PARAM, st, dev, port);

        /* 1.4. For active device check that function returns GT_OK        */
        /* for CPU port number.                                            */
        /* enable == GT_TRUE, tcQueue == 2                                 */
        port = CPSS_CPU_PORT_NUM_CNS;

        st = cpssDxChPortTxQueueingEnableSet(dev, port, tcQueue, enable);
        UTF_VERIFY_EQUAL2_PARAM_MAC(GT_OK, st, dev, port);
    }

    /* 2. For not active devices check that function returns non GT_OK.*/

    port = PORT_TX_VALID_PHY_PORT_CNS;

    /* prepare device iterator */
    PRV_TGF_NOT_APPLIC_DEV_RESET_MAC(&dev, UTF_NONE_FAMILY_E);
    /* Go over all non active devices. */
    while(GT_OK == prvUtfNextDeviceGet(&dev, GT_FALSE))
    {
        st = cpssDxChPortTxQueueingEnableSet(dev, port, tcQueue, enable);
        UTF_VERIFY_EQUAL1_PARAM_MAC(GT_NOT_APPLICABLE_DEVICE, st, dev);
    }

    /* 3.Call function with out of bound value for device id */
    dev = PRV_CPSS_MAX_PP_DEVICES_CNS;
    /* port == 0          */
    /* enable == GT_TRUE  */
    /* tcQueue == 2       */

    st = cpssDxChPortTxQueueingEnableSet(dev, port, tcQueue, enable);
    UTF_VERIFY_EQUAL1_PARAM_MAC(GT_BAD_PARAM, st, dev);
}

/*----------------------------------------------------------------------------*/
/*
GT_STATUS cpssDxChPortTxQueueTxEnableSet
(
    IN  GT_U8     devNum,
    IN  GT_U8     portNum,
    IN  GT_U8     tcQueue,
    IN  GT_BOOL   enable
)
*/
UTF_TEST_CASE_MAC(cpssDxChPortTxQueueTxEnableSet)
{
    /*
    ITERATE_DEVICES_PHY_CPU_PORTS (DxChx)
    1.1.1. Call with enable [GT_FALSE and GT_TRUE] and tcQueue [0, 2]. Expected: GT_OK.
    1.1.2. Call with enable [GT_FALSE and GT_TRUE] and out of range tcQueue [CPSS_TC_RANGE_CNS = 8]
    Expected: GT_BAD_PARAM.
    */

    GT_STATUS   st     = GT_OK;

    GT_U8       dev;
    GT_U8       port     = PORT_TX_VALID_PHY_PORT_CNS;
    GT_U8       tcQueue  = 0;
    GT_BOOL     enable   = GT_TRUE;
    GT_BOOL     enableGet;

    /* prepare iterator for go over all active devices */
    PRV_TGF_NOT_APPLIC_DEV_RESET_MAC(&dev, UTF_NONE_FAMILY_E);

    /* 1. Go over all active devices. */
    while (GT_OK == prvUtfNextDeviceGet(&dev, GT_TRUE))
    {
        st = prvUtfNextPhyPortReset(&port, dev);
        UTF_VERIFY_EQUAL0_PARAM_MAC(GT_OK, st);

        /* 1.1. For all active devices go over all available physical ports. */
        while (GT_OK == prvUtfNextPhyPortGet(&port, GT_TRUE))
        {
            /* 1.1.1 Call function for with enable = GT_FALSE and GT_TRUE
              tcQueue [0, 2]. Expected: GT_OK.
            */

            /* Call function with enable = GT_FALSE, tcQueue = 0 */
            enable = GT_FALSE;
            tcQueue = 0;

            st = cpssDxChPortTxQueueTxEnableSet(dev, port, tcQueue, enable);
            UTF_VERIFY_EQUAL4_PARAM_MAC(GT_OK, st, dev, port, tcQueue, enable);

            st = cpssDxChPortTxQueueTxEnableGet(dev, port, tcQueue, &enableGet);
            UTF_VERIFY_EQUAL3_PARAM_MAC(GT_OK, st, dev, port, tcQueue);

            /* verifying values */
            UTF_VERIFY_EQUAL1_STRING_MAC(enable, enableGet,
                       "got another Tx Queue status then was set: %d", dev);

            /* Call function with enable = GT_TRUE, tcQueue = 2 */
            enable = GT_TRUE;
            tcQueue = 2;

            st = cpssDxChPortTxQueueTxEnableSet(dev, port, tcQueue, enable);
            UTF_VERIFY_EQUAL4_PARAM_MAC(GT_OK, st, dev, port, tcQueue, enable);

            st = cpssDxChPortTxQueueTxEnableGet(dev, port, tcQueue, &enableGet);
            UTF_VERIFY_EQUAL3_PARAM_MAC(GT_OK, st, dev, port, tcQueue);

            /* verifying values */
            UTF_VERIFY_EQUAL1_STRING_MAC(enable, enableGet,
                       "got another Tx Queue status then was set: %d", dev);

            /* 1.1.2 Call with enable [GT_FALSE and GT_TRUE],
               out of range tcQueue [CPSS_TC_RANGE_CNS = 8]
               Expected: GT_BAD_PARAM.
            */

            /* Call function with enable = GT_FALSE, tcQueue = CPSS_TC_RANGE_CNS */
            enable = GT_FALSE;
            tcQueue = CPSS_TC_RANGE_CNS;

            st = cpssDxChPortTxQueueTxEnableSet(dev, port, tcQueue, enable);
            UTF_VERIFY_EQUAL4_PARAM_MAC(GT_BAD_PARAM, st, dev, port, tcQueue, enable);

            /* Call function with enable = GT_TRUE, tcQueue = CPSS_TC_RANGE_CNS */
            enable = GT_TRUE;
            tcQueue = CPSS_TC_RANGE_CNS;

            st = cpssDxChPortTxQueueTxEnableSet(dev, port, tcQueue, enable);
            UTF_VERIFY_EQUAL4_PARAM_MAC(GT_BAD_PARAM, st, dev, port, tcQueue, enable);
        }

        tcQueue = 4;

        st = prvUtfNextPhyPortReset(&port, dev);
        UTF_VERIFY_EQUAL0_PARAM_MAC(GT_OK, st);
        /* 1.2. For all active devices go over all non available physical ports. */
        while (GT_OK == prvUtfNextPhyPortGet(&port, GT_FALSE))
        {
            /* 1.2.1. Call function for each non-active port */
            /* enable == GT_TRUE, tcQueue = 4                */
            st = cpssDxChPortTxQueueTxEnableSet(dev, port, tcQueue, enable);
            UTF_VERIFY_EQUAL2_PARAM_MAC(GT_BAD_PARAM, st, dev, port);
        }

        /* 1.3. For active device check that function returns GT_BAD_PARAM */
        /* for out of bound value for port number.                         */
        /* enable == GT_TRUE, tcQueue == 4 */
        port = PRV_CPSS_MAX_PP_PORTS_NUM_CNS;

        st = cpssDxChPortTxQueueTxEnableSet(dev, port, tcQueue, enable);
        UTF_VERIFY_EQUAL2_PARAM_MAC(GT_BAD_PARAM, st, dev, port);

        /* 1.4. For active device check that function returns GT_OK        */
        /* for CPU port.                                                   */
        /* enable == GT_TRUE, tcQueue == 4 */
        port = CPSS_CPU_PORT_NUM_CNS;

        st = cpssDxChPortTxQueueTxEnableSet(dev, port, tcQueue, enable);
        UTF_VERIFY_EQUAL2_PARAM_MAC(GT_OK, st, dev, port);
    }

    /* 2. For not active devices check that function returns non GT_OK. */
    port = PORT_TX_VALID_PHY_PORT_CNS;

    /* prepare device iterator */
    PRV_TGF_NOT_APPLIC_DEV_RESET_MAC(&dev, UTF_NONE_FAMILY_E);
    /* Go over all non active devices. */
    while(GT_OK == prvUtfNextDeviceGet(&dev, GT_FALSE))
    {
        /* enable == GT_TRUE, tcQueue == 4 */
        st = cpssDxChPortTxQueueTxEnableSet(dev, port, tcQueue, enable);
        UTF_VERIFY_EQUAL1_PARAM_MAC(GT_NOT_APPLICABLE_DEVICE, st, dev);
    }

    /* 3.Call function with out of bound value for device id */
    dev = PRV_CPSS_MAX_PP_DEVICES_CNS;
    /* port == 0, enable == GT_TRUE, tcQueue == 4 */

    st = cpssDxChPortTxQueueTxEnableSet(dev, port, tcQueue,  enable);
    UTF_VERIFY_EQUAL1_PARAM_MAC(GT_BAD_PARAM, st, dev);
}

/*----------------------------------------------------------------------------*/
/*
GT_STATUS cpssDxChPortTxFlushQueuesSet
(
    IN  GT_U8     devNum,
    IN  GT_U8     portNum
)
*/
UTF_TEST_CASE_MAC(cpssDxChPortTxFlushQueuesSet)
{
    /*
    ITERATE_DEVICES_PHY_CPU_PORTS (DxCh)
    */

    GT_STATUS   st   = GT_OK;

    GT_U8       dev;
    GT_U8       port = PORT_TX_VALID_PHY_PORT_CNS;

    /* prepare iterator for go over all active devices */
    PRV_TGF_NOT_APPLIC_DEV_RESET_MAC(&dev, UTF_NONE_FAMILY_E);
    /* 1. Go over all active devices. */
    while (GT_OK == prvUtfNextDeviceGet(&dev, GT_TRUE))
    {
        st = prvUtfNextPhyPortReset(&port, dev);
        UTF_VERIFY_EQUAL0_PARAM_MAC(GT_OK, st);
        /* 1.1. For all active devices go over all available physical ports. */
        while (GT_OK == prvUtfNextPhyPortGet(&port, GT_TRUE))
        {
            st = cpssDxChPortTxFlushQueuesSet(dev, port);
            UTF_VERIFY_EQUAL2_PARAM_MAC(GT_OK, st, dev, port);
        }

        st = prvUtfNextPhyPortReset(&port, dev);
        UTF_VERIFY_EQUAL0_PARAM_MAC(GT_OK, st);
        /* 1.2. For all active devices go over all non available physical ports. */
        while (GT_OK == prvUtfNextPhyPortGet(&port, GT_FALSE))
        {
            st = cpssDxChPortTxFlushQueuesSet(dev, port);
            UTF_VERIFY_EQUAL2_PARAM_MAC(GT_BAD_PARAM, st, dev, port);
        }

        /* 1.3. For active device check that function returns GT_BAD_PARAM */
        /* for out of bound value for port number.                         */
        port = PRV_CPSS_MAX_PP_PORTS_NUM_CNS;

        st = cpssDxChPortTxFlushQueuesSet(dev, port);
        UTF_VERIFY_EQUAL2_PARAM_MAC(GT_BAD_PARAM, st, dev, port);

        /* 1.4. For active device check that function returns GT_OK        */
        /* for CPU port number.                                            */
        port = CPSS_CPU_PORT_NUM_CNS;

        st = cpssDxChPortTxFlushQueuesSet(dev, port);
        UTF_VERIFY_EQUAL2_PARAM_MAC(GT_OK, st, dev, port);
    }

    /* 2. For not active devices check that function returns non GT_OK.*/
    port = PORT_TX_VALID_PHY_PORT_CNS;

    /* prepare device iterator */
    PRV_TGF_NOT_APPLIC_DEV_RESET_MAC(&dev, UTF_NONE_FAMILY_E);

    /* Go over all non active devices. */
    while(GT_OK == prvUtfNextDeviceGet(&dev, GT_FALSE))
    {
        st = cpssDxChPortTxFlushQueuesSet(dev, port);
        UTF_VERIFY_EQUAL1_PARAM_MAC(GT_NOT_APPLICABLE_DEVICE, st, dev);
    }

    /* 3.Call function with out of bound value for device id */
    dev = PRV_CPSS_MAX_PP_DEVICES_CNS;
    /* port == 0 */

    st = cpssDxChPortTxFlushQueuesSet(dev, port);
    UTF_VERIFY_EQUAL1_PARAM_MAC(GT_BAD_PARAM, st, dev);
}

/*----------------------------------------------------------------------------*/
/*
GT_STATUS cpssDxChPortTxShaperEnableSet
(
    IN  GT_U8     devNum,
    IN  GT_U8     portNum,
    IN  GT_BOOL   enable
)
*/
UTF_TEST_CASE_MAC(cpssDxChPortTxShaperEnableSet)
{
/*
    ITERATE_DEVICES_PHY_CPU_PORTS (DxChx)
    1.1.1. Call with enable [GT_FALSE and GT_TRUE].
    Expected: GT_OK.
    1.1.2. Call cpssDxChPortTxShaperEnableGet
    Expected: GT_OK and the same value.
*/
    GT_STATUS   st     = GT_OK;

    GT_U8       dev;
    GT_U8       port   = PORT_TX_VALID_PHY_PORT_CNS;
    GT_BOOL     enable = GT_TRUE;
    GT_BOOL     enableGet;

    /* prepare iterator for go over all active devices */
    PRV_TGF_NOT_APPLIC_DEV_RESET_MAC(&dev, UTF_NONE_FAMILY_E);

    /* 1. Go over all active devices. */
    while (GT_OK == prvUtfNextDeviceGet(&dev, GT_TRUE))
    {
        st = prvUtfNextPhyPortReset(&port, dev);
        UTF_VERIFY_EQUAL0_PARAM_MAC(GT_OK, st);

        /* 1.1. For all active devices go over all available physical ports. */
        while (GT_OK == prvUtfNextPhyPortGet(&port, GT_TRUE))
        {
            /*
                1.1.1. Call with enable [GT_FALSE and GT_TRUE].
                Expected: GT_OK.
            */
            /* Call function with enable = GT_FALSE] */
            enable = GT_FALSE;

            st = cpssDxChPortTxShaperEnableSet(dev, port, enable);
            UTF_VERIFY_EQUAL3_PARAM_MAC(GT_OK, st, dev, port, enable);

            /*
                1.1.2. Call cpssDxChPortTxShaperEnableGet
                Expected: GT_OK and the same value.
            */
            st = cpssDxChPortTxShaperEnableGet(dev, port, &enableGet);
            UTF_VERIFY_EQUAL2_PARAM_MAC(GT_OK, st, dev, port);

            /* Verifying values */
            UTF_VERIFY_EQUAL2_STRING_MAC(enable, enableGet,
                   "get another enable than was set: %d, %d", dev, port);


            /* Call function with enable = GT_TRUE] */
            enable = GT_TRUE;

            st = cpssDxChPortTxShaperEnableSet(dev, port, enable);
            UTF_VERIFY_EQUAL3_PARAM_MAC(GT_OK, st, dev, port, enable);

            /*
                1.1.2. Call cpssDxChPortTxShaperEnableGet
                Expected: GT_OK and the same value.
            */
            st = cpssDxChPortTxShaperEnableGet(dev, port, &enableGet);
            UTF_VERIFY_EQUAL2_PARAM_MAC(GT_OK, st, dev, port);

            /* Verifying values */
            UTF_VERIFY_EQUAL2_STRING_MAC(enable, enableGet,
                   "get another enable than was set: %d, %d", dev, port);
        }

        st = prvUtfNextPhyPortReset(&port, dev);
        UTF_VERIFY_EQUAL0_PARAM_MAC(GT_OK, st);

        /* 1.2. For all active devices go over all non available physical ports. */
        while (GT_OK == prvUtfNextPhyPortGet(&port, GT_FALSE))
        {
            /* 1.2.1. Call function for each non-active port */
            /* enable == GT_TRUE    */
            st = cpssDxChPortTxShaperEnableSet(dev, port, enable);
            UTF_VERIFY_EQUAL2_PARAM_MAC(GT_BAD_PARAM, st, dev, port);
        }

        /* 1.3. For active device check that function returns GT_BAD_PARAM */
        /* for out of bound value for port number.                         */
        /* enable == GT_TRUE */
        port = PRV_CPSS_MAX_PP_PORTS_NUM_CNS;

        st = cpssDxChPortTxShaperEnableSet(dev, port, enable);
        UTF_VERIFY_EQUAL2_PARAM_MAC(GT_BAD_PARAM, st, dev, port);

        /* 1.4. For active device check that function returns GT_OK        */
        /* for CPU port number.                                            */
        /* enable == GT_TRUE */
        port = CPSS_CPU_PORT_NUM_CNS;

        st = cpssDxChPortTxShaperEnableSet(dev, port, enable);
        UTF_VERIFY_EQUAL2_PARAM_MAC(GT_OK, st, dev, port);
    }

    /* 2. For not active devices check that function returns non GT_OK.*/
    port = PORT_TX_VALID_PHY_PORT_CNS;

    /* prepare device iterator */
    PRV_TGF_NOT_APPLIC_DEV_RESET_MAC(&dev, UTF_NONE_FAMILY_E);

    /* Go over all non active devices. */
    while(GT_OK == prvUtfNextDeviceGet(&dev, GT_FALSE))
    {
        st = cpssDxChPortTxShaperEnableSet(dev, port, enable);
        UTF_VERIFY_EQUAL1_PARAM_MAC(GT_NOT_APPLICABLE_DEVICE, st, dev);
    }

    /* 3.Call function with out of bound value for device id */
    dev = PRV_CPSS_MAX_PP_DEVICES_CNS;

    st = cpssDxChPortTxShaperEnableSet(dev, port, enable);
    UTF_VERIFY_EQUAL1_PARAM_MAC(GT_BAD_PARAM, st, dev);
}

/*----------------------------------------------------------------------------*/
/*
GT_STATUS cpssDxChPortTxShaperEnableGet
(
    IN  GT_U8       devNum,
    IN  GT_U8       portNum,
    OUT GT_BOOL     *enablePtr
)
*/
UTF_TEST_CASE_MAC(cpssDxChPortTxShaperEnableGet)
{
/*
    ITERATE_DEVICES_PHY_CPU_PORTS (DxChx)
    1.1. Call function with non-NULL enablePtr.
    Expected: GT_OK.
    1.2. Call function with enablePtr [NULL] .
    Expected: GT_BAD_PTR.
*/
    GT_STATUS   st     = GT_OK;

    GT_U8       dev;
    GT_U8       port;
    GT_BOOL     retEnable;

    /* prepare device iterator */
    PRV_TGF_NOT_APPLIC_DEV_RESET_MAC(&dev, UTF_NONE_FAMILY_E);

    /* 1. Go over all active devices. */
    while(GT_OK == prvUtfNextDeviceGet(&dev, GT_TRUE))
    {
        st = prvUtfNextPhyPortReset(&port, dev);
        UTF_VERIFY_EQUAL0_PARAM_MAC(GT_OK, st);

        /* 1.1. For all active devices go over all available physical ports. */
        while(GT_OK == prvUtfNextPhyPortGet(&port, GT_TRUE))
        {
            /*
               1.1.1. Call function with non-NULL retEnable.
               Expected: GT_OK.
            */
            st = cpssDxChPortTxShaperEnableGet(dev, port, &retEnable);
            UTF_VERIFY_EQUAL2_PARAM_MAC(GT_OK, st, dev, retEnable);

            /*
               1.1.2. Call function with retEnable[NULL].
               Expected: GT_BAD_PTR.
            */
            st = cpssDxChPortTxShaperEnableGet(dev, port, NULL);
            UTF_VERIFY_EQUAL2_STRING_MAC(GT_BAD_PTR, st, "%d, %d, NULL", dev, port);
        }

        /* 1.2. For all active devices go over all non available physical ports. */

        st = prvUtfNextPhyPortReset(&port, dev);
        UTF_VERIFY_EQUAL0_PARAM_MAC(GT_OK, st);

        while (GT_OK == prvUtfNextPhyPortGet(&port, GT_FALSE))
        {
            /* 1.2.1. Call function for each non-active port    */
            /* enable == GT_TRUE                                */
            st = cpssDxChPortTxShaperEnableGet(dev, port, &retEnable);
            UTF_VERIFY_EQUAL2_PARAM_MAC(GT_BAD_PARAM, st, dev, port);
        }

        /* 1.3. For active device check that function returns GT_BAD_PARAM */
        /* for out of bound value for port number.                         */
        /* enable == GT_TRUE */
        port = PRV_CPSS_MAX_PP_PORTS_NUM_CNS;

        st = cpssDxChPortTxShaperEnableGet(dev, port, &retEnable);
        UTF_VERIFY_EQUAL2_PARAM_MAC(GT_BAD_PARAM, st, dev, port);

        /* 1.4. For active device check that function returns GT_OK */
        /* for CPU port number.                                     */
        port = CPSS_CPU_PORT_NUM_CNS;

        st = cpssDxChPortTxShaperEnableGet(dev, port, &retEnable);
        UTF_VERIFY_EQUAL2_PARAM_MAC(GT_OK, st, dev, port);
    }

    port = PORT_TX_VALID_PHY_PORT_CNS;

    /* 2. For not active devices check that function returns non GT_OK.*/
    /* prepare device iterator */
    PRV_TGF_NOT_APPLIC_DEV_RESET_MAC(&dev, UTF_NONE_FAMILY_E);

    /* Go over all non active devices. */
    while(GT_OK == prvUtfNextDeviceGet(&dev, GT_FALSE))
    {
        st = cpssDxChPortTxShaperEnableGet(dev, port, &retEnable);
        UTF_VERIFY_EQUAL1_PARAM_MAC(GT_NOT_APPLICABLE_DEVICE, st, dev);
    }

    /* 3.Call function with out of bound value for device id */
    dev = PRV_CPSS_MAX_PP_DEVICES_CNS;

    st = cpssDxChPortTxShaperEnableGet(dev, port, &retEnable);
    UTF_VERIFY_EQUAL1_PARAM_MAC(GT_BAD_PARAM, st, dev);
}

/*----------------------------------------------------------------------------*/
/*
GT_STATUS cpssDxChPortTxShaperProfileSet
(
    IN    GT_U8     devNum,
    IN    GT_U8     portNum,
    IN    GT_U16    burstSize,
    INOUT GT_U32    *maxRatePtr
)
*/
UTF_TEST_CASE_MAC(cpssDxChPortTxShaperProfileSet)
{
/*
    ITERATE_DEVICES_PHY_CPU_PORTS (DxCh)
    1.1.1. Call with burstSize [0/ 0xFFF] and non-null maxRatePtr.
    Expected: GT_OK and non-null maxRatePtr.
    1.1.2. Call get func with the same params.
    Expected: GT_OK and the same values.
    1.1.3. Call with out of range burstSize [0x2000] and non-null maxRatePtr.
    Expected: NOT GT_OK.
    1.1.4. Call with burstSize [0] and maxRatePtr [NULL].
    Expected: GT_BAD_PTR.
*/

    GT_STATUS   st        = GT_OK;

    GT_U8       dev;
    GT_U8       port      = PORT_TX_VALID_PHY_PORT_CNS;
    GT_U16      burstSize = 0;
    GT_U32      maxRate   = 0xFFFF;
    GT_U16      burstSizeGet = 1;
    GT_U32      maxRateGet   = 0xFFF1;
    GT_BOOL     enableGet    = GT_FALSE;
    GT_U32      minRate,setMaxRate;

    /* prepare iterator for go over all active devices */
    PRV_TGF_NOT_APPLIC_DEV_RESET_MAC(&dev, UTF_NONE_FAMILY_E);

    /* 1. Go over all active devices. */
    while (GT_OK == prvUtfNextDeviceGet(&dev, GT_TRUE))
    {
        st = prvUtfNextPhyPortReset(&port, dev);
        UTF_VERIFY_EQUAL0_PARAM_MAC(GT_OK, st);

        /* 1.1. For all active devices go over all available physical ports. */
        while (GT_OK == prvUtfNextPhyPortGet(&port, GT_TRUE))
        {
            /*
               1.1.1 Call function with burstSize burstSize [0 / 0xFFF],
               non-null maxRatePtr
               Expected: GT_OK and non-null maxRatePtr.
            */

            /* Call function with burstSize = 0, maxRatePtr != NULL */
            burstSize = 0;
            maxRate = 65;

            st = cpssDxChPortTxShaperProfileSet(dev, port, burstSize, &maxRate);
            UTF_VERIFY_EQUAL4_PARAM_MAC(GT_OK, st, dev, port, burstSize, maxRate);

            st = cpssDxChPortTxShaperEnableSet(dev, port, GT_TRUE);
            UTF_VERIFY_EQUAL2_PARAM_MAC(GT_OK, st, dev, port);
            /*
                1.1.2. Call get func with the same params.
                Expected: GT_OK and the same values.
            */

            st = cpssDxChPortTxShaperProfileGet(dev, port, &burstSizeGet,
                                                &maxRateGet, &enableGet);
            UTF_VERIFY_EQUAL2_PARAM_MAC(GT_OK, st, dev, port);

            /* Verifying values */
            UTF_VERIFY_EQUAL2_STRING_MAC(burstSize, burstSizeGet,
                                 "get another burstSize than was set: %d, %d", dev, port);
            UTF_VERIFY_EQUAL2_STRING_MAC(maxRate, maxRateGet,
                                 "get another maxRate than was set: %d, %d", dev, port);


            /* Call function with burstSize == 0xFFF, maxRatePtr != NULL*/
            burstSize = 0xFFF;
            maxRate = 32;

            st = cpssDxChPortTxShaperProfileSet(dev, port, burstSize, &maxRate);
            UTF_VERIFY_EQUAL4_PARAM_MAC(GT_OK, st, dev, port, burstSize, maxRate);

            /*
                1.1.2. Call get func with the same params.
                Expected: GT_OK and the same values.
            */

            st = cpssDxChPortTxShaperProfileGet(dev, port, &burstSizeGet,
                                                &maxRateGet, &enableGet);
            UTF_VERIFY_EQUAL2_PARAM_MAC(GT_OK, st, dev, port);

            /* Verifying values */
            UTF_VERIFY_EQUAL2_STRING_MAC(burstSize, burstSizeGet,
                                 "get another burstSize than was set: %d, %d", dev, port);
            UTF_VERIFY_EQUAL2_STRING_MAC(maxRate, maxRateGet,
                                 "get another maxRate than was set: %d, %d", dev, port);



            /*
               1.1.3. Call with out of range burstSize [0x2000] non-null maxRatePtr.
               Expected: NOT GT_OK.
            */
            burstSize = 0x2000;

            st = cpssDxChPortTxShaperProfileSet(dev, port, burstSize, &maxRate);
            UTF_VERIFY_NOT_EQUAL3_PARAM_MAC(GT_OK, st, dev, port, burstSize);

            /*
               1.1.4. Call with burstSize [0] maxRatePtr [NULL].
               Expected: GT_BAD_PTR.
            */
            burstSize = 0;

            st = cpssDxChPortTxShaperProfileSet(dev, port, burstSize, NULL);
            UTF_VERIFY_EQUAL3_STRING_MAC(GT_BAD_PTR, st, "%d, %d, %d, NULL",
                                         dev, port, burstSize);
            /*
               1.1.5. Find the minimum rate.
                      Loop from rate minimum*100 to minimum rate.
                      Check that requested rate always <= return rate, 
                      except  if rate < minimum rate.
            */
            /* find minimum rate */
            minRate = 0;
            st = cpssDxChPortTxShaperProfileSet(dev, port, burstSize, &minRate);
            UTF_VERIFY_EQUAL3_PARAM_MAC(GT_OK, st, dev, port, burstSize);

            burstSize = 0x200;
            maxRate = minRate*100;
            while(maxRate)
            {
                setMaxRate = maxRate;
                st = cpssDxChPortTxShaperProfileSet(dev, port, burstSize, &maxRate);
                UTF_VERIFY_EQUAL3_PARAM_MAC(GT_OK, st, dev, port, burstSize);

                /* check if minimum */
                if (maxRate > setMaxRate)
                {
                    if (maxRate == minRate)
                    {
                        /*exit loop */
                        break;
                    }
                    else
                    {
                        /*error*/
                        UTF_VERIFY_EQUAL3_PARAM_MAC(maxRate, minRate, dev, port, maxRate);
                    }
                }
                else /* maxRate <= setMaxRate */
                {
                    /* check exact */
                    setMaxRate = maxRate;
                    st = cpssDxChPortTxShaperProfileSet(dev, port, burstSize, &maxRate);
                    UTF_VERIFY_EQUAL3_PARAM_MAC(setMaxRate, maxRate, dev, port, maxRate);
                }
                maxRate--;
            }
        }

        burstSize = 4;

        st = prvUtfNextPhyPortReset(&port, dev);
        UTF_VERIFY_EQUAL0_PARAM_MAC(GT_OK, st);

        /* 1.2. For all active devices go over all non available physical ports.*/
        while (GT_OK == prvUtfNextPhyPortGet(&port, GT_FALSE))
        {
            /* 1.2.1. Call function for each non-active port */
            st = cpssDxChPortTxShaperProfileSet(dev, port, burstSize, &maxRate);
            UTF_VERIFY_EQUAL2_PARAM_MAC(GT_BAD_PARAM, st, dev, port);
        }

        /* 1.3. For active device check that function returns GT_BAD_PARAM */
        /* for out of bound value for port number.                         */
        port = PRV_CPSS_MAX_PP_PORTS_NUM_CNS;

        st = cpssDxChPortTxShaperProfileSet(dev, port, burstSize, &maxRate);
        UTF_VERIFY_EQUAL2_PARAM_MAC(GT_BAD_PARAM, st, dev, port);

        /* 1.4. For active device check that function returns GT_OK        */
        /* for CPU port number.                                            */
        port = CPSS_CPU_PORT_NUM_CNS;

        st = cpssDxChPortTxShaperProfileSet(dev, port, burstSize, &maxRate);
        UTF_VERIFY_EQUAL2_PARAM_MAC(GT_OK, st, dev, port);
    }

    /* 2. For not active devices check that function returns non GT_OK.*/
    port = PORT_TX_VALID_PHY_PORT_CNS;

    /* prepare device iterator */
    PRV_TGF_NOT_APPLIC_DEV_RESET_MAC(&dev, UTF_NONE_FAMILY_E);

    /* Go over all non active devices. */
    while(GT_OK == prvUtfNextDeviceGet(&dev, GT_FALSE))
    {
        st = cpssDxChPortTxShaperProfileSet(dev, port, burstSize, &maxRate);
        UTF_VERIFY_EQUAL1_PARAM_MAC(GT_NOT_APPLICABLE_DEVICE, st, dev);
    }

    /* 3.Call function with out of bound value for device id */
    dev = PRV_CPSS_MAX_PP_DEVICES_CNS;
    /* port == 0, burstSize == 4, maxRatePtr != NULL */

    st = cpssDxChPortTxShaperProfileSet(dev, port, burstSize, &maxRate);
    UTF_VERIFY_EQUAL1_PARAM_MAC(GT_BAD_PARAM, st, dev);
}

/*----------------------------------------------------------------------------*/
/*
GT_STATUS cpssDxChPortTxShaperProfileGet
(
    IN  GT_U8   devNum,
    IN  GT_U8   portNum,
    OUT GT_U16  *burstSizePtr,
    OUT GT_U32  *maxRatePtr,
    OUT GT_BOOL *enablePtr
)
*/
UTF_TEST_CASE_MAC(cpssDxChPortTxShaperProfileGet)
{
/*
    ITERATE_DEVICES_PHY_CPU_PORTS (DxChx)
    1.1.1. Call with tcQueue [2].
    Expected: GT_OK.
    1.1.2. Call with wrong burstSizePtr [NULL].
    Expected: GT_BAD_PTR.
    1.1.3. Call with wrong maxRatePtr [NULL].
    Expected: GT_BAD_PTR.
    1.1.4. Call with wrong enablePtr [NULL].
    Expected: GT_BAD_PTR.
*/

    GT_STATUS   st        = GT_OK;

    GT_U8       dev;
    GT_U8       port      = PORT_TX_VALID_PHY_PORT_CNS;
    GT_U16      burstSizeGet = 1;
    GT_U32      userRateGet  = 0xFFF1;
    GT_BOOL     enableGet    = GT_FALSE;

    /* prepare iterator for go over all active devices */
    PRV_TGF_NOT_APPLIC_DEV_RESET_MAC(&dev, UTF_NONE_FAMILY_E);

    /* 1. Go over all active devices. */
    while (GT_OK == prvUtfNextDeviceGet(&dev, GT_TRUE))
    {
        st = prvUtfNextPhyPortReset(&port, dev);
        UTF_VERIFY_EQUAL0_PARAM_MAC(GT_OK, st);

        /* 1.1. For all active devices go over all available physical ports. */
        while (GT_OK == prvUtfNextPhyPortGet(&port, GT_TRUE))
        {
            /*
               1.1.1. Call with correct params.
               Expected: GT_OK.
            */
            st = cpssDxChPortTxShaperProfileGet(dev, port, &burstSizeGet,
                                                &userRateGet, &enableGet);
            UTF_VERIFY_EQUAL2_PARAM_MAC(GT_OK, st, dev, port);

            /*
                1.1.2. Call with wrong burstSizePtr [NULL].
                Expected: GT_BAD_PTR.
            */
            st = cpssDxChPortTxShaperProfileGet(dev, port, NULL,
                                                            &userRateGet, &enableGet);
            UTF_VERIFY_EQUAL2_PARAM_MAC(GT_BAD_PTR, st, dev, port);

            /*
                1.1.3. Call with wrong userRateGet [NULL].
                Expected: GT_BAD_PTR.
            */
            st = cpssDxChPortTxShaperProfileGet(dev, port, &burstSizeGet,
                                                            NULL, &enableGet);
            UTF_VERIFY_EQUAL2_PARAM_MAC(GT_BAD_PTR, st, dev, port);

            /*
                1.1.4. Call with wrong enableGet [NULL].
                Expected: GT_BAD_PTR.
            */
            st = cpssDxChPortTxShaperProfileGet(dev, port, &burstSizeGet,
                                                            &userRateGet, NULL);
            UTF_VERIFY_EQUAL2_PARAM_MAC(GT_BAD_PTR, st, dev, port);
        }

        st = prvUtfNextPhyPortReset(&port, dev);
        UTF_VERIFY_EQUAL0_PARAM_MAC(GT_OK, st);

        /* 1.2. For all active devices go over all non available physical ports.*/
        while (GT_OK == prvUtfNextPhyPortGet(&port, GT_FALSE))
        {
            /* 1.2.1. Call function for each non-active port */

            st = cpssDxChPortTxShaperProfileGet(dev, port, &burstSizeGet,
                                                            &userRateGet, &enableGet);
            UTF_VERIFY_EQUAL2_PARAM_MAC(GT_BAD_PARAM, st, dev, port);
        }

        /* 1.3. For active device check that function returns GT_BAD_PARAM */
        /* for out of bound value for port number.                         */
        port = PRV_CPSS_MAX_PP_PORTS_NUM_CNS;

        st = cpssDxChPortTxShaperProfileGet(dev, port, &burstSizeGet,
                                                        &userRateGet, &enableGet);
        UTF_VERIFY_EQUAL2_PARAM_MAC(GT_BAD_PARAM, st, dev, port);

        /* 1.4. For active device check that function returns GT_OK        */
        /* for CPU port number.                                            */
        port = CPSS_CPU_PORT_NUM_CNS;

        st = cpssDxChPortTxShaperProfileGet(dev, port, &burstSizeGet,
                                                        &userRateGet, &enableGet);
        UTF_VERIFY_EQUAL2_PARAM_MAC(GT_OK, st, dev, port);
    }

    /* 2. For not active devices check that function returns non GT_OK.*/
    port = PORT_TX_VALID_PHY_PORT_CNS;

    /* prepare device iterator */
    PRV_TGF_NOT_APPLIC_DEV_RESET_MAC(&dev, UTF_NONE_FAMILY_E);

    /* Go over all non active devices. */
    while(GT_OK == prvUtfNextDeviceGet(&dev, GT_FALSE))
    {
        st = cpssDxChPortTxShaperProfileGet(dev, port, &burstSizeGet,
                                                        &userRateGet, &enableGet);
        UTF_VERIFY_EQUAL1_PARAM_MAC(GT_NOT_APPLICABLE_DEVICE, st, dev);
    }

    /* 3.Call function with out of bound value for device id */
    dev = PRV_CPSS_MAX_PP_DEVICES_CNS;

    st = cpssDxChPortTxShaperProfileGet(dev, port, &burstSizeGet,
                                                    &userRateGet, &enableGet);
    UTF_VERIFY_EQUAL1_PARAM_MAC(GT_BAD_PARAM, st, dev);
}

/*----------------------------------------------------------------------------*/
/*
GT_STATUS cpssDxChPortTxQShaperEnableSet
(
    IN  GT_U8     devNum,
    IN  GT_U8     portNum,
    IN  GT_U8     tcQueue,
    IN  GT_BOOL   enable
)
*/
UTF_TEST_CASE_MAC(cpssDxChPortTxQShaperEnableSet)
{
/*
    ITERATE_DEVICES_PHY_CPU_PORTS (DxCh)
    1.1.1. Call with enable [GT_FALSE and GT_TRUE] and tcQueue [0/ 1].
    Expected: GT_OK.
    1.1.2. Call cpssDxChPortTxQShaperEnableGet with the same params.
    Expected: GT_OK and the same enable.
    1.1.3. Call with enable [GT_FALSE and GT_TRUE]
           and out of range tcQueue [CPSS_TC_RANGE_CNS = 8].
    Expected: GT_BAD_PARAM.
*/
    GT_STATUS   st     = GT_OK;

    GT_U8       dev;
    GT_U8       port      = PORT_TX_VALID_PHY_PORT_CNS;
    GT_U8       tcQueue   = 0;
    GT_BOOL     enable    = GT_TRUE;
    GT_BOOL     enableGet = GT_TRUE;

    /* prepare iterator for go over all active devices */
    PRV_TGF_NOT_APPLIC_DEV_RESET_MAC(&dev, UTF_NONE_FAMILY_E);

    /* 1. Go over all active devices. */
    while (GT_OK == prvUtfNextDeviceGet(&dev, GT_TRUE))
    {
        st = prvUtfNextPhyPortReset(&port, dev);
        UTF_VERIFY_EQUAL0_PARAM_MAC(GT_OK, st);

        /* 1.1. For all active devices go over all available physical ports. */
        while (GT_OK == prvUtfNextPhyPortGet(&port, GT_TRUE))
        {
            /*
                1.1.1. Call with enable [GT_FALSE and GT_TRUE] and tcQueue [0/ 1].
                Expected: GT_OK.
            */
            /* Call function with enable = GT_FALSE, tcQueue = 0 */
            enable = GT_FALSE;
            tcQueue = 0;

            st = cpssDxChPortTxQShaperEnableSet(dev, port, tcQueue, enable);
            UTF_VERIFY_EQUAL4_PARAM_MAC(GT_OK, st, dev, port, tcQueue, enable);

            /*
                1.1.2. Call cpssDxChPortTxQShaperEnableGet with the same params.
                Expected: GT_OK and the same enable.
            */
            st = cpssDxChPortTxQShaperEnableGet(dev, port, tcQueue, &enableGet);

            UTF_VERIFY_EQUAL4_PARAM_MAC(GT_OK, st, dev, port, tcQueue, enable);
            /* Verifying values */
            UTF_VERIFY_EQUAL2_STRING_MAC(enable, enableGet,
                         "get another enable than was set: %d, %d", dev, port);


            /* Call function with enable = GT_TRUE, tcQueue = 1 */
            enable = GT_TRUE;
            tcQueue = 1;

            st = cpssDxChPortTxQShaperEnableSet(dev, port, tcQueue, enable);
            UTF_VERIFY_EQUAL4_PARAM_MAC(GT_OK, st, dev, port, tcQueue, enable);

            /*
                1.1.2. Call cpssDxChPortTxQShaperEnableGet with the same params.
                Expected: GT_OK and the same enable.
            */
            st = cpssDxChPortTxQShaperEnableGet(dev, port, tcQueue, &enableGet);

            UTF_VERIFY_EQUAL4_PARAM_MAC(GT_OK, st, dev, port, tcQueue, enable);
            /* Verifying values */
            UTF_VERIFY_EQUAL2_STRING_MAC(enable, enableGet,
                         "get another enable than was set: %d, %d", dev, port);

            /*
                1.1.3. Call with enable [GT_FALSE and GT_TRUE]
                       and out of range tcQueue [CPSS_TC_RANGE_CNS = 8].
                Expected: GT_BAD_PARAM.
            */
            /* Call function with enable = GT_FALSE, tcQueue = CPSS_TC_RANGE_CNS */
            enable = GT_FALSE;
            tcQueue = CPSS_TC_RANGE_CNS;

            st = cpssDxChPortTxQShaperEnableSet(dev, port, tcQueue, enable);
            UTF_VERIFY_EQUAL4_PARAM_MAC(GT_BAD_PARAM, st, dev, port, tcQueue, enable);

            /* Call function with enable = GT_TRUE, tcQueue = CPSS_TC_RANGE_CNS */
            enable = GT_TRUE;
            tcQueue = CPSS_TC_RANGE_CNS;

            st = cpssDxChPortTxQShaperEnableSet(dev, port, tcQueue, enable);
            UTF_VERIFY_EQUAL4_PARAM_MAC(GT_BAD_PARAM, st, dev, port, tcQueue, enable);
        }

        enable = GT_TRUE;
        tcQueue = 4;

        st = prvUtfNextPhyPortReset(&port, dev);
        UTF_VERIFY_EQUAL0_PARAM_MAC(GT_OK, st);

        /* 1.2. For all active devices go over all non available physical ports. */
        while (GT_OK == prvUtfNextPhyPortGet(&port, GT_FALSE))
        {
            /* 1.2.1. Call function for each non-active port */
            /* enable = GT_TRUE, tcQueue = 4 */

            st = cpssDxChPortTxQShaperEnableSet(dev, port, tcQueue, enable);
            UTF_VERIFY_EQUAL2_PARAM_MAC(GT_BAD_PARAM, st, dev, port);
        }

        /* 1.3. For active device check that function returns GT_BAD_PARAM */
        /* for out of bound value for port number.                         */
        /* enable == GT_TRUE, tcQueue == 4 */
        port = PRV_CPSS_MAX_PP_PORTS_NUM_CNS;

        st = cpssDxChPortTxQShaperEnableSet(dev, port, tcQueue, enable);
        UTF_VERIFY_EQUAL2_PARAM_MAC(GT_BAD_PARAM, st, dev, port);

        /* 1.4. For active device check that function returns GT_OK        */
        /* for out CPU port number.                                        */
        /* enable == GT_TRUE, tcQueue == 4 */
        port = CPSS_CPU_PORT_NUM_CNS;

        st = cpssDxChPortTxQShaperEnableSet(dev, port, tcQueue, enable);
        UTF_VERIFY_EQUAL2_PARAM_MAC(GT_OK, st, dev, port);
    }

    /* 2. For not active devices check that function returns non GT_OK.*/
    port = PORT_TX_VALID_PHY_PORT_CNS;

    /* prepare device iterator */
    PRV_TGF_NOT_APPLIC_DEV_RESET_MAC(&dev, UTF_NONE_FAMILY_E);

    /* Go over all non active devices. */
    while(GT_OK == prvUtfNextDeviceGet(&dev, GT_FALSE))
    {
        st = cpssDxChPortTxQShaperEnableSet(dev, port, tcQueue, enable);
        UTF_VERIFY_EQUAL1_PARAM_MAC(GT_NOT_APPLICABLE_DEVICE, st, dev);
    }

    /* 3.Call function with out of bound value for device id */
    dev = PRV_CPSS_MAX_PP_DEVICES_CNS;
    /* port == 0, enable == GT_TRUE, tcQueue == 4 */

    st = cpssDxChPortTxQShaperEnableSet(dev, port, tcQueue,  enable);
    UTF_VERIFY_EQUAL1_PARAM_MAC(GT_BAD_PARAM, st, dev);
}

/*----------------------------------------------------------------------------*/
/*
GT_STATUS cpssDxChPortTxQShaperEnableGet
(
    IN  GT_U8     devNum,
    IN  GT_U8     portNum,
    IN  GT_U8     tcQueue,
    OUT GT_BOOL   &enablePtr
)
*/
UTF_TEST_CASE_MAC(cpssDxChPortTxQShaperEnableGet)
{
/*
    ITERATE_DEVICES_PHY_CPU_PORTS (DxCh)
    1.1.1. Call with tcQueue [0/ 1].
    Expected: GT_OK.
    1.1.2. Call with out of range tcQueue [CPSS_TC_RANGE_CNS = 8].
    Expected: GT_BAD_PARAM.
    1.1.3. Call with wrong enablePtr [NULL].
    Expected: GT_BAD_PTR.
*/
    GT_STATUS   st     = GT_OK;

    GT_U8       dev;
    GT_U8       port      = PORT_TX_VALID_PHY_PORT_CNS;
    GT_U8       tcQueue   = 0;
    GT_BOOL     enableGet = GT_TRUE;

    /* prepare iterator for go over all active devices */
    PRV_TGF_NOT_APPLIC_DEV_RESET_MAC(&dev, UTF_NONE_FAMILY_E);

    /* 1. Go over all active devices. */
    while (GT_OK == prvUtfNextDeviceGet(&dev, GT_TRUE))
    {
        st = prvUtfNextPhyPortReset(&port, dev);
        UTF_VERIFY_EQUAL0_PARAM_MAC(GT_OK, st);

        /* 1.1. For all active devices go over all available physical ports. */
        while (GT_OK == prvUtfNextPhyPortGet(&port, GT_TRUE))
        {
            /*
                1.1.1. Call with tcQueue [0/ 1].
                Expected: GT_OK.
            */
            /* Call function with tcQueue = 0 */
            tcQueue = 0;

            st = cpssDxChPortTxQShaperEnableGet(dev, port, tcQueue, &enableGet);
            UTF_VERIFY_EQUAL3_PARAM_MAC(GT_OK, st, dev, port, tcQueue);

            /* Call function with tcQueue = 1 */
            tcQueue = 1;

            st = cpssDxChPortTxQShaperEnableGet(dev, port, tcQueue, &enableGet);
            UTF_VERIFY_EQUAL3_PARAM_MAC(GT_OK, st, dev, port, tcQueue);

            /*
                1.1.2. Call with out of range tcQueue [CPSS_TC_RANGE_CNS = 8].
                Expected: GT_BAD_PARAM.
            */
            tcQueue = CPSS_TC_RANGE_CNS;

            st = cpssDxChPortTxQShaperEnableGet(dev, port, tcQueue, &enableGet);
            UTF_VERIFY_EQUAL3_PARAM_MAC(GT_BAD_PARAM, st, dev, port, tcQueue);

            tcQueue = 1;

            /*
                1.1.3. Call with wrong enablePtr [NULL].
                Expected: GT_BAD_PTR.
            */
            st = cpssDxChPortTxQShaperEnableGet(dev, port, tcQueue, NULL);
            UTF_VERIFY_EQUAL3_PARAM_MAC(GT_BAD_PTR, st, dev, port, tcQueue);
        }

        tcQueue = 4;

        st = prvUtfNextPhyPortReset(&port, dev);
        UTF_VERIFY_EQUAL0_PARAM_MAC(GT_OK, st);

        /* 1.2. For all active devices go over all non available physical ports. */
        while (GT_OK == prvUtfNextPhyPortGet(&port, GT_FALSE))
        {
            /* 1.2.1. Call function for each non-active port */
            st = cpssDxChPortTxQShaperEnableGet(dev, port, tcQueue, &enableGet);
            UTF_VERIFY_EQUAL2_PARAM_MAC(GT_BAD_PARAM, st, dev, port);
        }

        /* 1.3. For active device check that function returns GT_BAD_PARAM */
        /* for out of bound value for port number.                         */
        port = PRV_CPSS_MAX_PP_PORTS_NUM_CNS;

        st = cpssDxChPortTxQShaperEnableGet(dev, port, tcQueue, &enableGet);
        UTF_VERIFY_EQUAL2_PARAM_MAC(GT_BAD_PARAM, st, dev, port);

        /* 1.4. For active device check that function returns GT_OK        */
        /* for out CPU port number.                                        */
        /* enable == GT_TRUE, tcQueue == 4 */
        port = CPSS_CPU_PORT_NUM_CNS;

        st = cpssDxChPortTxQShaperEnableGet(dev, port, tcQueue, &enableGet);
        UTF_VERIFY_EQUAL2_PARAM_MAC(GT_OK, st, dev, port);
    }

    /* 2. For not active devices check that function returns non GT_OK.*/
    port = PORT_TX_VALID_PHY_PORT_CNS;

    /* prepare device iterator */
    PRV_TGF_NOT_APPLIC_DEV_RESET_MAC(&dev, UTF_NONE_FAMILY_E);

    /* Go over all non active devices. */
    while(GT_OK == prvUtfNextDeviceGet(&dev, GT_FALSE))
    {
        st = cpssDxChPortTxQShaperEnableGet(dev, port, tcQueue, &enableGet);
        UTF_VERIFY_EQUAL1_PARAM_MAC(GT_NOT_APPLICABLE_DEVICE, st, dev);
    }

    /* 3.Call function with out of bound value for device id */
    dev = PRV_CPSS_MAX_PP_DEVICES_CNS;
    /* port == 0, enable == GT_TRUE, tcQueue == 4 */

    st = cpssDxChPortTxQShaperEnableGet(dev, port, tcQueue, &enableGet);
    UTF_VERIFY_EQUAL1_PARAM_MAC(GT_BAD_PARAM, st, dev);
}

/*----------------------------------------------------------------------------*/
/*
GT_STATUS cpssDxChPortTxQShaperProfileSet
(
    IN    GT_U8     devNum,
    IN    GT_U8     portNum,
    IN    GT_U8     tcQueue,
    IN    GT_U16    burstSize,
    INOUT GT_U32    *userRatePtr
)
*/
UTF_TEST_CASE_MAC(cpssDxChPortTxQShaperProfileSet)
{
/*
    ITERATE_DEVICES_PHY_CPU_PORTS (DxChx)
    1.1.1. Call with tcQueue [2], burstSize [0/ 0xFFF] and non-null userRatePtr
    Expected: GT_OK and non-null userRatePtr.
    1.1.2. Call get func with the same params.
    Expected: GT_OK and the same values.
    1.1.3. Call with out of range tcQueue [CPSS_TC_RANGE_CNS = 8], burstSize [17], and non-null userRatePtr.
    Expected: GT_BAD_PARAM.
    1.1.4. Call with tcQueue [4], out of range burstSize [0x2000] and non-null userRatePtr.
    Expected: NOT GT_OK.
    1.1.5. Call with tcQueue [4], burstSize [0] and userRatePtr [NULL].
    Expected: GT_BAD_PTR.
*/

    GT_STATUS   st        = GT_OK;

    GT_U8       dev;
    GT_U8       port      = PORT_TX_VALID_PHY_PORT_CNS;
    GT_U8       tcQueue   = 0;
    GT_U16      burstSize = 0;
    GT_U32      userRate   = 0xFFFF;

    GT_U16      burstSizeGet = 1;
    GT_U32      userRateGet  = 0xFFF1;
    GT_BOOL     enableGet    = GT_FALSE;

    /* prepare iterator for go over all active devices */
    PRV_TGF_NOT_APPLIC_DEV_RESET_MAC(&dev, UTF_NONE_FAMILY_E);

    /* 1. Go over all active devices. */
    while (GT_OK == prvUtfNextDeviceGet(&dev, GT_TRUE))
    {
        st = prvUtfNextPhyPortReset(&port, dev);
        UTF_VERIFY_EQUAL0_PARAM_MAC(GT_OK, st);

        /* 1.1. For all active devices go over all available physical ports. */
        while (GT_OK == prvUtfNextPhyPortGet(&port, GT_TRUE))
        {
            /*
               1.1.1. Call with tcQueue [2], burstSize [0 / 0xFFF] non-null userRatePtr.
               Expected: GT_OK and non-null userRatePtr.
            */

            /* Call function with tcQueue = 2, burstSize = 0, userRatePtr != NULL */
            tcQueue = 2;
            burstSize = 0;
            userRate = 65;

            st = cpssDxChPortTxQShaperProfileSet(dev, port, tcQueue, burstSize, &userRate);
            UTF_VERIFY_EQUAL5_PARAM_MAC(GT_OK, st, dev, port, tcQueue, burstSize, userRate);

            st = cpssDxChPortTxQShaperEnableSet(dev, port, tcQueue, GT_TRUE);
            UTF_VERIFY_EQUAL3_PARAM_MAC(GT_OK, st, dev, port, tcQueue);
            /*
                1.1.2. Call get func with the same params.
                Expected: GT_OK and the same values.
            */

            st = cpssDxChPortTxQShaperProfileGet(dev, port, tcQueue, &burstSizeGet,
                                                            &userRateGet, &enableGet);
            UTF_VERIFY_EQUAL3_PARAM_MAC(GT_OK, st, dev, port, tcQueue);

            /* Verifying values */
            UTF_VERIFY_EQUAL2_STRING_MAC(burstSize, burstSizeGet,
                                 "get another burstSize than was set: %d, %d", dev, port);
            UTF_VERIFY_EQUAL2_STRING_MAC(userRate, userRateGet,
                                 "get another userRate than was set: %d, %d", dev, port);


            /* Call function with tcQueue = 5, burstSize == 0xFFF, userRatePtr != NULL */
            tcQueue = 5;
            burstSize = 0xFFF;
            userRate = 32;

            st = cpssDxChPortTxQShaperProfileSet(dev, port, tcQueue, burstSize, &userRate);
            UTF_VERIFY_EQUAL5_PARAM_MAC(GT_OK, st, dev, port, tcQueue, burstSize, userRate);

            st = cpssDxChPortTxQShaperEnableSet(dev, port, tcQueue, GT_TRUE);
            UTF_VERIFY_EQUAL3_PARAM_MAC(GT_OK, st, dev, port, tcQueue);
            /*
                1.1.2. Call get func with the same params.
                Expected: GT_OK and the same userRatePtr.
            */

            st = cpssDxChPortTxQShaperProfileGet(dev, port, tcQueue, &burstSizeGet,
                                                            &userRateGet, &enableGet);
            UTF_VERIFY_EQUAL3_PARAM_MAC(GT_OK, st, dev, port, tcQueue);

            /* Verifying values */
            UTF_VERIFY_EQUAL2_STRING_MAC(burstSize, burstSizeGet,
                                 "get another burstSize than was set: %d, %d", dev, port);
            UTF_VERIFY_EQUAL2_STRING_MAC(userRate, userRateGet,
                                 "get another userRate than was set: %d, %d", dev, port);


            /*
               1.1.3. Call with out of range tcQueue [CPSS_TC_RANGE_CNS = 8],
               burstSize [17], non-null userRatePtr.
               Expected: GT_BAD_PARAM.
            */
            tcQueue = CPSS_TC_RANGE_CNS;
            burstSize = 17;
            userRate = 50;

            st = cpssDxChPortTxQShaperProfileSet(dev, port, tcQueue, burstSize, &userRate);
            UTF_VERIFY_EQUAL5_PARAM_MAC(GT_BAD_PARAM, st, dev, port, tcQueue, burstSize, userRate);

            /*
               1.1.4. Call with tcQueue [4], out of range burstSize [0x2000]
                                non-null userRatePtr
               Expected: NOT GT_OK.
            */
            tcQueue = 4;
            burstSize = 0x2000;
            userRate = 33;

            st = cpssDxChPortTxQShaperProfileSet(dev, port, tcQueue, burstSize, &userRate);
            UTF_VERIFY_NOT_EQUAL5_PARAM_MAC(GT_OK, st, dev, port, tcQueue, burstSize, userRate);

            /*
               1.1.5. Call with tcQueue [4], burstSize [0] userRatePtr [NULL]
               Expected: GT_BAD_PTR.
            */
            tcQueue = 4;
            burstSize = 0;

            st = cpssDxChPortTxQShaperProfileSet(dev, port, tcQueue, burstSize, NULL);
            UTF_VERIFY_EQUAL4_STRING_MAC(GT_BAD_PTR, st, "%d, %d, %d, %d, NULL",
                                         dev, port, tcQueue, burstSize);
        }

        burstSize = 4;

        st = prvUtfNextPhyPortReset(&port, dev);
        UTF_VERIFY_EQUAL0_PARAM_MAC(GT_OK, st);
        /* 1.2. For all active devices go over all non available physical ports.*/
        while (GT_OK == prvUtfNextPhyPortGet(&port, GT_FALSE))
        {
            /* 1.2.1. Call function for each non-active port */

            st = cpssDxChPortTxQShaperProfileSet(dev, port, tcQueue, burstSize, &userRate);
            UTF_VERIFY_EQUAL2_PARAM_MAC(GT_BAD_PARAM, st, dev, port);
        }

        /* 1.3. For active device check that function returns GT_BAD_PARAM */
        /* for out of bound value for port number.                         */
        port = PRV_CPSS_MAX_PP_PORTS_NUM_CNS;

        st = cpssDxChPortTxQShaperProfileSet(dev, port, tcQueue, burstSize, &userRate);
        UTF_VERIFY_EQUAL2_PARAM_MAC(GT_BAD_PARAM, st, dev, port);

        /* 1.4. For active device check that function returns GT_OK        */
        /* for CPU port number.                                            */
        port = CPSS_CPU_PORT_NUM_CNS;

        st = cpssDxChPortTxQShaperProfileSet(dev, port, tcQueue, burstSize, &userRate);
        UTF_VERIFY_EQUAL2_PARAM_MAC(GT_OK, st, dev, port);
    }

    /* 2. For not active devices check that function returns non GT_OK.*/
    port = PORT_TX_VALID_PHY_PORT_CNS;

    /* prepare device iterator */
    PRV_TGF_NOT_APPLIC_DEV_RESET_MAC(&dev, UTF_NONE_FAMILY_E);
    /* Go over all non active devices. */
    while(GT_OK == prvUtfNextDeviceGet(&dev, GT_FALSE))
    {
        st = cpssDxChPortTxQShaperProfileSet(dev, port, tcQueue, burstSize, &userRate);
        UTF_VERIFY_EQUAL1_PARAM_MAC(GT_NOT_APPLICABLE_DEVICE, st, dev);
    }

    /* 3.Call function with out of bound value for device id */
    dev = PRV_CPSS_MAX_PP_DEVICES_CNS;
    /* port == 0, tcQueue == 4, burstSize == 4, userRatePtr != NULL */

    st = cpssDxChPortTxQShaperProfileSet(dev, port, tcQueue, burstSize, &userRate);
    UTF_VERIFY_EQUAL1_PARAM_MAC(GT_BAD_PARAM, st, dev);
}

/*----------------------------------------------------------------------------*/
/*
GT_STATUS cpssDxChPortTxQShaperProfileGet
(
    IN  GT_U8     devNum,
    IN  GT_U8     portNum,
    IN  GT_U8     tcQueue,
    OUT GT_U16   *burstSizePtr,
    OUT GT_U32   *maxRatePtr,
    OUT GT_BOOL  *enablePtr
)
*/
UTF_TEST_CASE_MAC(cpssDxChPortTxQShaperProfileGet)
{
/*
    ITERATE_DEVICES_PHY_CPU_PORTS (DxChx)
    1.1.1. Call with tcQueue [2].
    Expected: GT_OK.
    1.1.2. Call with out of range tcQueue [CPSS_TC_RANGE_CNS = 8] and non-null pointers.
    Expected: GT_BAD_PARAM.
    1.1.3. Call with wrong burstSizePtr [NULL].
    Expected: GT_BAD_PTR.
    1.1.4. Call with wrong maxRatePtr [NULL].
    Expected: GT_BAD_PTR.
    1.1.5. Call with wrong enablePtr [NULL].
    Expected: GT_BAD_PTR.
*/

    GT_STATUS   st        = GT_OK;

    GT_U8       dev;
    GT_U8       port      = PORT_TX_VALID_PHY_PORT_CNS;
    GT_U8       tcQueue   = 0;
    GT_U16      burstSizeGet = 1;
    GT_U32      userRateGet  = 0xFFF1;
    GT_BOOL     enableGet    = GT_FALSE;

    /* prepare iterator for go over all active devices */
    PRV_TGF_NOT_APPLIC_DEV_RESET_MAC(&dev, UTF_NONE_FAMILY_E);

    /* 1. Go over all active devices. */
    while (GT_OK == prvUtfNextDeviceGet(&dev, GT_TRUE))
    {
        st = prvUtfNextPhyPortReset(&port, dev);
        UTF_VERIFY_EQUAL0_PARAM_MAC(GT_OK, st);

        /* 1.1. For all active devices go over all available physical ports. */
        while (GT_OK == prvUtfNextPhyPortGet(&port, GT_TRUE))
        {
            /*
               1.1.1. Call with tcQueue [2].
               Expected: GT_OK.
            */

            tcQueue = 2;

            st = cpssDxChPortTxQShaperProfileGet(dev, port, tcQueue, &burstSizeGet,
                                                            &userRateGet, &enableGet);
            UTF_VERIFY_EQUAL3_PARAM_MAC(GT_OK, st, dev, port, tcQueue);

            /*
               1.1.2. Call with out of range tcQueue [CPSS_TC_RANGE_CNS = 8].
               Expected: GT_BAD_PARAM.
            */
            tcQueue = CPSS_TC_RANGE_CNS;

            st = cpssDxChPortTxQShaperProfileGet(dev, port, tcQueue, &burstSizeGet,
                                                            &userRateGet, &enableGet);
            UTF_VERIFY_EQUAL3_PARAM_MAC(GT_BAD_PARAM, st, dev, port, tcQueue);

            /*
                1.1.3. Call with wrong burstSizePtr [NULL].
                Expected: GT_BAD_PTR.
            */
            tcQueue = 4;

            st = cpssDxChPortTxQShaperProfileGet(dev, port, tcQueue, NULL,
                                                            &userRateGet, &enableGet);
            UTF_VERIFY_EQUAL3_PARAM_MAC(GT_BAD_PTR, st, dev, port, tcQueue);

            /*
                1.1.4. Call with wrong userRateGet [NULL].
                Expected: GT_BAD_PTR.
            */

            st = cpssDxChPortTxQShaperProfileGet(dev, port, tcQueue, &burstSizeGet,
                                                            NULL, &enableGet);
            UTF_VERIFY_EQUAL3_PARAM_MAC(GT_BAD_PTR, st, dev, port, tcQueue);

            /*
                1.1.5. Call with wrong enableGet [NULL].
                Expected: GT_BAD_PTR.
            */

            st = cpssDxChPortTxQShaperProfileGet(dev, port, tcQueue, &burstSizeGet,
                                                            &userRateGet, NULL);
            UTF_VERIFY_EQUAL3_PARAM_MAC(GT_BAD_PTR, st, dev, port, tcQueue);
        }

        st = prvUtfNextPhyPortReset(&port, dev);
        UTF_VERIFY_EQUAL0_PARAM_MAC(GT_OK, st);

        /* 1.2. For all active devices go over all non available physical ports.*/
        while (GT_OK == prvUtfNextPhyPortGet(&port, GT_FALSE))
        {
            /* 1.2.1. Call function for each non-active port */

            st = cpssDxChPortTxQShaperProfileGet(dev, port, tcQueue, &burstSizeGet,
                                                            &userRateGet, &enableGet);
            UTF_VERIFY_EQUAL2_PARAM_MAC(GT_BAD_PARAM, st, dev, port);
        }

        /* 1.3. For active device check that function returns GT_BAD_PARAM */
        /* for out of bound value for port number.                         */
        port = PRV_CPSS_MAX_PP_PORTS_NUM_CNS;

        st = cpssDxChPortTxQShaperProfileGet(dev, port, tcQueue, &burstSizeGet,
                                                        &userRateGet, &enableGet);
        UTF_VERIFY_EQUAL2_PARAM_MAC(GT_BAD_PARAM, st, dev, port);

        /* 1.4. For active device check that function returns GT_OK        */
        /* for CPU port number.                                            */
        port = CPSS_CPU_PORT_NUM_CNS;

        st = cpssDxChPortTxQShaperProfileGet(dev, port, tcQueue, &burstSizeGet,
                                                        &userRateGet, &enableGet);
        UTF_VERIFY_EQUAL2_PARAM_MAC(GT_OK, st, dev, port);
    }

    /* 2. For not active devices check that function returns non GT_OK.*/
    port = PORT_TX_VALID_PHY_PORT_CNS;

    /* prepare device iterator */
    PRV_TGF_NOT_APPLIC_DEV_RESET_MAC(&dev, UTF_NONE_FAMILY_E);

    /* Go over all non active devices. */
    while(GT_OK == prvUtfNextDeviceGet(&dev, GT_FALSE))
    {
        st = cpssDxChPortTxQShaperProfileGet(dev, port, tcQueue, &burstSizeGet,
                                                        &userRateGet, &enableGet);
        UTF_VERIFY_EQUAL1_PARAM_MAC(GT_NOT_APPLICABLE_DEVICE, st, dev);
    }

    /* 3.Call function with out of bound value for device id */
    dev = PRV_CPSS_MAX_PP_DEVICES_CNS;

    st = cpssDxChPortTxQShaperProfileGet(dev, port, tcQueue, &burstSizeGet,
                                                    &userRateGet, &enableGet);
    UTF_VERIFY_EQUAL1_PARAM_MAC(GT_BAD_PARAM, st, dev);
}


/*----------------------------------------------------------------------------*/
/*
GT_STATUS cpssDxChPortTxBindPortToSchedulerProfileSet
(
    IN  GT_U8                                   devNum,
    IN  GT_U8                                   portNum,
    IN  CPSS_PORT_TX_SCHEDULER_PROFILE_SET_ENT  profileSet
)
*/
UTF_TEST_CASE_MAC(cpssDxChPortTxBindPortToSchedulerProfileSet)
{
    /*
    ITERATE_DEVICES_PHY_CPU_PORTS (DxChx)
    1.1.1. Call with profileSet [CPSS_PORT_TX_SCHEDULER_PROFILE_1_E,
    CPSS_PORT_TX_SCHEDULER_PROFILE_2_E, CPSS_PORT_TX_SCHEDULER_PROFILE_3_E, CPSS_PORT_TX_SCHEDULER_PROFILE_4_E].
    Expected: GT_OK.
    1.1.2. Call cpssDxChPortTxBindPortToSchedulerProfileGet with non-null profileSetPtr.
    Expected: GT_OK and the same profileSetPtr.
    1.1.3. Call with profileSet [CPSS_PORT_TX_SCHEDULER_PROFILE_5_E, CPSS_PORT_TX_SCHEDULER_PROFILE_6_E, CPSS_PORT_TX_SCHEDULER_PROFILE_7_E, CPSS_PORT_TX_SCHEDULER_PROFILE_8_E].
    Expected: GT_OK for Cheetah2 devices and NON GT_OK for others.
    1.1.4. Call with wrong enum values profileSet.
    Expected: GT_BAD_PARAM.
    */

    GT_STATUS   st     = GT_OK;

    GT_U8                                  dev;
    GT_U8                                  port       = PORT_TX_VALID_PHY_PORT_CNS;
    CPSS_PORT_TX_SCHEDULER_PROFILE_SET_ENT profile    = CPSS_PORT_TX_SCHEDULER_PROFILE_1_E;
    CPSS_PORT_TX_SCHEDULER_PROFILE_SET_ENT profileGet = CPSS_PORT_TX_SCHEDULER_PROFILE_1_E;
    CPSS_PP_FAMILY_TYPE_ENT                devFamily  = CPSS_MAX_FAMILY;

    /* prepare iterator for go over all active devices */
    PRV_TGF_NOT_APPLIC_DEV_RESET_MAC(&dev, UTF_NONE_FAMILY_E);

    /* 1. Go over all active devices. */
    while (GT_OK == prvUtfNextDeviceGet(&dev, GT_TRUE))
    {
        st = prvUtfDeviceFamilyGet(dev, &devFamily);
        UTF_VERIFY_EQUAL1_STRING_MAC(GT_OK, st, "prvUtfDeviceFamilyGet: %d", dev);

        st = prvUtfNextPhyPortReset(&port, dev);
        UTF_VERIFY_EQUAL0_PARAM_MAC(GT_OK, st);
        /* 1.1. For all active devices go over all available physical ports. */
        while (GT_OK == prvUtfNextPhyPortGet(&port, GT_TRUE))
        {
            /* 1.1.1. Call with profileSet [CPSS_PORT_TX_SCHEDULER_PROFILE_1_E /
                                            CPSS_PORT_TX_SCHEDULER_PROFILE_2_E /
                                            CPSS_PORT_TX_SCHEDULER_PROFILE_3_E /
                                            CPSS_PORT_TX_SCHEDULER_PROFILE_4_E]
            Expected: GT_OK. */

            /* Call with profileSet [CPSS_PORT_TX_SCHEDULER_PROFILE_1_E] */
            profile = CPSS_PORT_TX_SCHEDULER_PROFILE_1_E;

            st = cpssDxChPortTxBindPortToSchedulerProfileSet(dev, port, profile);
            UTF_VERIFY_EQUAL3_PARAM_MAC(GT_OK, st, dev, port, profile);

            /* Call with profileSet [CPSS_PORT_TX_SCHEDULER_PROFILE_2_E] */
            profile = CPSS_PORT_TX_SCHEDULER_PROFILE_2_E;

            st = cpssDxChPortTxBindPortToSchedulerProfileSet(dev, port, profile);
            UTF_VERIFY_EQUAL3_PARAM_MAC(GT_OK, st, dev, port, profile);

            /* Call with profileSet [CPSS_PORT_TX_SCHEDULER_PROFILE_3_E] */
            profile = CPSS_PORT_TX_SCHEDULER_PROFILE_3_E;

            st = cpssDxChPortTxBindPortToSchedulerProfileSet(dev, port, profile);
            UTF_VERIFY_EQUAL3_PARAM_MAC(GT_OK, st, dev, port, profile);

            /* Call with profileSet [CPSS_PORT_TX_SCHEDULER_PROFILE_4_E] */
            profile = CPSS_PORT_TX_SCHEDULER_PROFILE_4_E;

            st = cpssDxChPortTxBindPortToSchedulerProfileSet(dev, port, profile);
            UTF_VERIFY_EQUAL3_PARAM_MAC(GT_OK, st, dev, port, profile);

            /*
                1.1.2. Call cpssDxChPortTxBindPortToSchedulerProfileGet with non-null profileSetPtr.
                Expected: GT_OK and the same profileSetPtr.
            */

            st = cpssDxChPortTxBindPortToSchedulerProfileGet(dev, port, &profileGet);
            UTF_VERIFY_EQUAL2_STRING_MAC(GT_OK, st,
                                         "cpssDxChPortTxBindPortToSchedulerProfileGet: %d, %d", dev, port);

            /* Verifying values */
            UTF_VERIFY_EQUAL2_STRING_MAC(profile, profileGet,
                                         "get another profileSetPtr than was set: %d, %d", dev, port);

            /* 1.1.3. Call with profileSet [CPSS_PORT_TX_SCHEDULER_PROFILE_5_E,
                                            CPSS_PORT_TX_SCHEDULER_PROFILE_6_E,
                                            CPSS_PORT_TX_SCHEDULER_PROFILE_7_E,
                                            CPSS_PORT_TX_SCHEDULER_PROFILE_8_E].
               Expected: GT_OK for Cheetah2 devices and NON GT_OK for others.
            */

            /* Call with profileSet [CPSS_PORT_TX_SCHEDULER_PROFILE_5_E] */
            profile = CPSS_PORT_TX_SCHEDULER_PROFILE_5_E;

            st = cpssDxChPortTxBindPortToSchedulerProfileSet(dev, port, profile);
            if (CPSS_PP_FAMILY_CHEETAH_E  != devFamily)
            {
                UTF_VERIFY_EQUAL3_PARAM_MAC(GT_OK, st, dev, port, profile);
            }
            else
            {
                UTF_VERIFY_NOT_EQUAL3_PARAM_MAC(GT_OK, st, dev, port, profile);
            }

            /* Call with profileSet [CPSS_PORT_TX_SCHEDULER_PROFILE_6_E] */
            profile = CPSS_PORT_TX_SCHEDULER_PROFILE_6_E;

            st = cpssDxChPortTxBindPortToSchedulerProfileSet(dev, port, profile);
            if (CPSS_PP_FAMILY_CHEETAH_E  != devFamily)
            {
                UTF_VERIFY_EQUAL3_PARAM_MAC(GT_OK, st, dev, port, profile);
            }
            else
            {
                UTF_VERIFY_NOT_EQUAL3_PARAM_MAC(GT_OK, st, dev, port, profile);
            }

            /* Call with profileSet [CPSS_PORT_TX_SCHEDULER_PROFILE_7_E] */
            profile = CPSS_PORT_TX_SCHEDULER_PROFILE_7_E;

            st = cpssDxChPortTxBindPortToSchedulerProfileSet(dev, port, profile);
            if (CPSS_PP_FAMILY_CHEETAH_E  != devFamily)
            {
                UTF_VERIFY_EQUAL3_PARAM_MAC(GT_OK, st, dev, port, profile);
            }
            else
            {
                UTF_VERIFY_NOT_EQUAL3_PARAM_MAC(GT_OK, st, dev, port, profile);
            }

            /* Call with profileSet [CPSS_PORT_TX_SCHEDULER_PROFILE_8_E] */
            profile = CPSS_PORT_TX_SCHEDULER_PROFILE_8_E;

            st = cpssDxChPortTxBindPortToSchedulerProfileSet(dev, port, profile);
            if (CPSS_PP_FAMILY_CHEETAH_E  != devFamily)
            {
                UTF_VERIFY_EQUAL3_PARAM_MAC(GT_OK, st, dev, port, profile);
            }
            else
            {
                UTF_VERIFY_NOT_EQUAL3_PARAM_MAC(GT_OK, st, dev, port, profile);
            }

            /*
               1.1.4. Call with wrong enum values profileSet.
               Expected: GT_BAD_PARAM.
            */
            UTF_ENUMS_CHECK_MAC(cpssDxChPortTxBindPortToSchedulerProfileSet
                                (dev, port, profile),
                                profile);
        }

        profile = CPSS_PORT_TX_SCHEDULER_PROFILE_2_E;

        st = prvUtfNextPhyPortReset(&port, dev);
        UTF_VERIFY_EQUAL0_PARAM_MAC(GT_OK, st);
        /* 1.2. For all active devices go over all non available physical ports. */
        while (GT_OK == prvUtfNextPhyPortGet(&port, GT_FALSE))
        {
            /* 1.2.1. Call function for each non-active port */
            /* profileSet [CPSS_PORT_TX_SCHEDULER_PROFILE_2_E] */

            st = cpssDxChPortTxBindPortToSchedulerProfileSet(dev, port, profile);
            UTF_VERIFY_EQUAL2_PARAM_MAC(GT_BAD_PARAM, st, dev, port);
        }

        /* 1.3. For active device check that function returns GT_BAD_PARAM */
        /* for out of bound value for port number.                         */
        /* profile == CPSS_PORT_TX_SCHEDULER_PROFILE_2_E */
        port = PRV_CPSS_MAX_PP_PORTS_NUM_CNS;

        st = cpssDxChPortTxBindPortToSchedulerProfileSet(dev, port, profile);
        UTF_VERIFY_EQUAL2_PARAM_MAC(GT_BAD_PARAM, st, dev, port);

        /* 1.4. For active device check that function returns GT_OK        */
        /* for CPU port number.                                            */
        /* profile == CPSS_PORT_TX_SCHEDULER_PROFILE_2_E */
        port = CPSS_CPU_PORT_NUM_CNS;

        st = cpssDxChPortTxBindPortToSchedulerProfileSet(dev, port, profile);
        UTF_VERIFY_EQUAL2_PARAM_MAC(GT_OK, st, dev, port);
    }

    /* 2. For not active devices check that function returns non GT_OK.*/

    port = PORT_TX_VALID_PHY_PORT_CNS;

    /* prepare device iterator */
    PRV_TGF_NOT_APPLIC_DEV_RESET_MAC(&dev, UTF_NONE_FAMILY_E);

    /* Go over all non active devices. */
    while(GT_OK == prvUtfNextDeviceGet(&dev, GT_FALSE))
    {
        st = cpssDxChPortTxBindPortToSchedulerProfileSet(dev, port, profile);
        UTF_VERIFY_EQUAL1_PARAM_MAC(GT_NOT_APPLICABLE_DEVICE, st, dev);
    }

    /* 3.Call function with out of bound value for device id */
    dev = PRV_CPSS_MAX_PP_DEVICES_CNS;
    /* port == 0                                         */
    /* profile == CPSS_PORT_TX_SCHEDULER_PROFILE_2_E     */

    st = cpssDxChPortTxBindPortToSchedulerProfileSet(dev, port, profile);
    UTF_VERIFY_EQUAL1_PARAM_MAC(GT_BAD_PARAM, st, dev);
}

/*----------------------------------------------------------------------------*/
/*
GT_STATUS cpssDxChPortTxBindPortToSchedulerProfileGet
(
    IN  GT_U8                                   devNum,
    IN  GT_U8                                   portNum,
    OUT CPSS_PORT_TX_SCHEDULER_PROFILE_SET_ENT  *profileSetPtr
)
*/
UTF_TEST_CASE_MAC(cpssDxChPortTxBindPortToSchedulerProfileGet)
{
/*
    ITERATE_DEVICES_PHY_CPU_PORTS (DxChx)
    1.1.1. Call with non-null profileSetPtr.
    Expected: GT_OK.
    1.1.2. Call with profileSetPtr [NULL]
    Expected: GT_BAD_PTR.
*/

    GT_STATUS   st     = GT_OK;

    GT_U8                                  dev;
    GT_U8                                  port       = PORT_TX_VALID_PHY_PORT_CNS;
    CPSS_PORT_TX_SCHEDULER_PROFILE_SET_ENT profileSet = CPSS_PORT_TX_SCHEDULER_PROFILE_1_E;

    /* prepare iterator for go over all active devices */
    PRV_TGF_NOT_APPLIC_DEV_RESET_MAC(&dev, UTF_NONE_FAMILY_E);

    /* 1. Go over all active devices. */
    while (GT_OK == prvUtfNextDeviceGet(&dev, GT_TRUE))
    {
        st = prvUtfNextPhyPortReset(&port, dev);
        UTF_VERIFY_EQUAL0_PARAM_MAC(GT_OK, st);

        /* 1.1. For all active devices go over all available physical ports. */
        while (GT_OK == prvUtfNextPhyPortGet(&port, GT_TRUE))
        {
            /*
                1.1.1. Call with non-null profileSetPtr.
                Expected: GT_OK.
            */
            st = cpssDxChPortTxBindPortToSchedulerProfileGet(dev, port, &profileSet);
            UTF_VERIFY_EQUAL2_PARAM_MAC(GT_OK, st, dev, port);

            /*
                1.1.2. Call with profileSetPtr [NULL]
                Expected: GT_BAD_PTR.
            */
            st = cpssDxChPortTxBindPortToSchedulerProfileGet(dev, port, NULL);
            UTF_VERIFY_EQUAL2_STRING_MAC(GT_BAD_PTR, st, "%d, %d, NULL", dev, port);
        }

        st = prvUtfNextPhyPortReset(&port, dev);
        UTF_VERIFY_EQUAL0_PARAM_MAC(GT_OK, st);
        /* 1.2. For all active devices go over all non available physical ports. */
        while (GT_OK == prvUtfNextPhyPortGet(&port, GT_FALSE))
        {
            /* 1.2.1. Call function for each non-active port */
            st = cpssDxChPortTxBindPortToSchedulerProfileGet(dev, port, &profileSet);
            UTF_VERIFY_EQUAL2_PARAM_MAC(GT_BAD_PARAM, st, dev, port);
        }

        /* 1.3. For active device check that function returns GT_BAD_PARAM */
        /* for out of bound value for port number.                         */
        port = PRV_CPSS_MAX_PP_PORTS_NUM_CNS;

        st = cpssDxChPortTxBindPortToSchedulerProfileGet(dev, port, &profileSet);
        UTF_VERIFY_EQUAL2_PARAM_MAC(GT_BAD_PARAM, st, dev, port);

        /* 1.4. For active device check that function returns GT_OK        */
        /* for CPU port number.                                            */
        port = CPSS_CPU_PORT_NUM_CNS;

        st = cpssDxChPortTxBindPortToSchedulerProfileGet(dev, port, &profileSet);
        UTF_VERIFY_EQUAL2_PARAM_MAC(GT_OK, st, dev, port);
    }

    /* 2. For not active devices check that function returns non GT_OK.*/

    port = PORT_TX_VALID_PHY_PORT_CNS;

    /* prepare device iterator */
    PRV_TGF_NOT_APPLIC_DEV_RESET_MAC(&dev, UTF_NONE_FAMILY_E);

    /* Go over all non active devices. */
    while(GT_OK == prvUtfNextDeviceGet(&dev, GT_FALSE))
    {
        st = cpssDxChPortTxBindPortToSchedulerProfileGet(dev, port, &profileSet);
        UTF_VERIFY_EQUAL1_PARAM_MAC(GT_NOT_APPLICABLE_DEVICE, st, dev);
    }

    /* 3.Call function with out of bound value for device id */
    dev = PRV_CPSS_MAX_PP_DEVICES_CNS;
    /* port == 0 */

    st = cpssDxChPortTxBindPortToSchedulerProfileGet(dev, port, &profileSet);
    UTF_VERIFY_EQUAL1_PARAM_MAC(GT_BAD_PARAM, st, dev);
}

/*----------------------------------------------------------------------------*/
/*
GT_STATUS cpssDxChPortTx4TcTailDropProfileSet
(
    IN    GT_U8                                   devNum,
    IN    CPSS_PORT_TX_DROP_PROFILE_SET_ENT       profileSet,
    IN    GT_U8                                   trafficClass,
    IN    CPSS_PORT_TX_Q_TAIL_DROP_PROF_TC_PARAMS *tailDropProfileParamsPtr
)
*/
UTF_TEST_CASE_MAC(cpssDxChPortTx4TcTailDropProfileSet)
{
/*
    ITERATE_DEVICES (DxChx)
    1.1. Call with profileSet [CPSS_PORT_TX_DROP_PROFILE_1_E,
                               CPSS_PORT_TX_DROP_PROFILE_4_E],
                   trafficClass [0, 1],
                   and non-null tailDropProfileParamsPtr
    Expected: GT_OK.
    1.2. Call cpssDxChPortTx4TcTailDropProfileGet with non-NULL tailDropProfileParamsGet
              and other parameters from 1.1.
    Expected: GT_OK and same tailDropProfileParamsGet as written
    1.3. Call with profileSet [CPSS_PORT_TX_DROP_PROFILE_5_E,
                                CPSS_PORT_TX_DROP_PROFILE_8_E],
                    trafficClass [0, 7],
                    and non-null tailDropProfileParamsPtr
    Expected: GT_OK for Cheetah2 devices and NON GT_OK for others.
    1.4. Call cpssDxChPortTx4TcTailDropProfileGet with non-NULL tailDropProfileParamsGet
              and other parameters from 1.3.
    Expected: GT_OK and same tailDropProfileParamsGet as written
    1.5. Call with wrong enum values profileSet ,
                    trafficClass [0],
                    and non-null tailDropProfileParamsPtr.
    Expected: GT_BAD_PARAM.
    1.4. Call with profileSet [CPSS_PORT_TX_DROP_PROFILE_1_E],
                    with out of range trafficClass [CPSS_TC_RANGE_CNS = 8],
                    and non-null tailDropProfileParamsPtr.
    Expected: GT_BAD_PARAM.
    1.7. Call with profileSet [CPSS_PORT_TX_DROP_PROFILE_2_E],
                    trafficClass [5],
                    and null tailDropProfileParamsPtr [NULL].
    Expected: GT_BAD_PTR.
    1.8. Call with profileSet [CPSS_PORT_TX_DROP_PROFILE_1_E],
                    trafficClass [0],
                    and non-null tailDropProfileParamsPtr
                    with field dp0MaxBuffNum/ dp1MaxBuffNum/
                    dp2MaxBuffNum/ dp0MaxDescrNum/
                    dp1MaxDescrNum/ dp2MaxDescrNum [0xF000]
    Expected: GT_OK for tailDropProfileParamsPtr->dp1MaxDescrNum,
                        tailDropProfileParamsPtr->dp1MaxBuffNum
                        and NOT GT_OK for others.
*/

    GT_STATUS   st     = GT_OK;

    GT_U8                                   dev;
    CPSS_PORT_TX_DROP_PROFILE_SET_ENT       profile       = 0;
    GT_U8                                   trafficClass  = 0;
    CPSS_PORT_TX_Q_TAIL_DROP_PROF_TC_PARAMS params;
    CPSS_PORT_TX_Q_TAIL_DROP_PROF_TC_PARAMS paramsGet;
    CPSS_PP_FAMILY_TYPE_ENT                 devFamily = CPSS_MAX_FAMILY;
    GT_BOOL                                 isEqual   = GT_FALSE;
    GT_STATUS rc;

    params.dp0MaxBuffNum  = 4;
    params.dp0MaxDescrNum = 8;
    params.dp1MaxBuffNum  = 16;
    params.dp1MaxDescrNum = 32;
    params.dp2MaxBuffNum  = 64;
    params.dp2MaxDescrNum = 128;
    params.tcMaxBuffNum   = 256;
    params.tcMaxDescrNum  = 1024;

    paramsGet.dp0MaxBuffNum  = 4;
    paramsGet.dp0MaxDescrNum = 8;
    paramsGet.dp1MaxBuffNum  = 16;
    paramsGet.dp1MaxDescrNum = 32;
    paramsGet.dp2MaxBuffNum  = 64;
    paramsGet.dp2MaxDescrNum = 128;
    paramsGet.tcMaxBuffNum   = 256;
    paramsGet.tcMaxDescrNum  = 1024;

    /* prepare device iterator */
    PRV_TGF_NOT_APPLIC_DEV_RESET_MAC(&dev, UTF_NONE_FAMILY_E);
    /* 1. Go over all active devices. */
    while (GT_OK == prvUtfNextDeviceGet(&dev, GT_TRUE))
    {
        /* 1.1. Call with profileSet [CPSS_PORT_TX_DROP_PROFILE_1_E,
           CPSS_PORT_TX_DROP_PROFILE_4_E], trafficClass [0, 1],
           non-null tailDropProfileParamsPtr
           Expected: GT_OK.
        */

        /* Call function with profileSet [CPSS_PORT_TX_DROP_PROFILE_1_E] */
        /* trafficClass [0], non-null tailDropProfileParamsPtr */
        profile = CPSS_PORT_TX_DROP_PROFILE_1_E;
        trafficClass = 0;

        st = cpssDxChPortTx4TcTailDropProfileSet(dev, profile, trafficClass, &params);
        UTF_VERIFY_EQUAL3_PARAM_MAC(GT_OK, st, dev, profile, trafficClass);

        /*
            1.2. Call cpssDxChPortTx4TcTailDropProfileGet with non-NULL tailDropProfileParamsGet
                      and other parameters from 1.1.
            Expected: GT_OK and same tailDropProfileParamsGet as written
        */
        st = cpssDxChPortTx4TcTailDropProfileGet(dev, profile, trafficClass, &paramsGet);
        UTF_VERIFY_EQUAL1_STRING_MAC(GT_OK, st, "cpssDxChPortTx4TcTailDropProfileGet: %d", dev);

        /* invalidate variables */
        isEqual = (0 == cpssOsMemCmp(&params, &paramsGet, sizeof(params))) ? GT_TRUE : GT_FALSE;
        UTF_VERIFY_EQUAL1_STRING_MAC(GT_TRUE, isEqual, "got another params than was set: %d", dev);

        /* Call function with profileSet [CPSS_PORT_TX_DROP_PROFILE_4_E] */
        /* trafficClass [1], non-null tailDropProfileParamsPtr */
        profile = CPSS_PORT_TX_DROP_PROFILE_4_E;
        trafficClass = 1;

        st = cpssDxChPortTx4TcTailDropProfileSet(dev, profile, trafficClass, &params);
        UTF_VERIFY_EQUAL3_PARAM_MAC(GT_OK, st, dev, profile, trafficClass);

        /*
            1.2. Call cpssDxChPortTx4TcTailDropProfileGet with non-NULL tailDropProfileParamsGet
                      and other parameters from 1.1.
            Expected: GT_OK and same tailDropProfileParamsGet as written
        */
        st = cpssDxChPortTx4TcTailDropProfileGet(dev, profile, trafficClass, &paramsGet);
        UTF_VERIFY_EQUAL1_STRING_MAC(GT_OK, st, "cpssDxChPortTx4TcTailDropProfileGet: %d", dev);

        /* invalidate variables */
        isEqual = (0 == cpssOsMemCmp(&params, &paramsGet, sizeof(params))) ? GT_TRUE : GT_FALSE;
        UTF_VERIFY_EQUAL1_STRING_MAC(GT_TRUE, isEqual, "got another params than was set: %d", dev);

        /* 1.3. Call with profileSet [CPSS_PORT_TX_DROP_PROFILE_5_E,
                                      CPSS_PORT_TX_DROP_PROFILE_8_E],
                          trafficClass [0, 7],
                          non-null tailDropProfileParamsPtr
           Expected: GT_OK for Cheetah2 devices and NON GT_OK for others.
        */

        st = prvUtfDeviceFamilyGet(dev, &devFamily);
        UTF_VERIFY_EQUAL1_STRING_MAC(GT_OK, st, "prvUtfDeviceFamilyGet: %d", dev);

        /* Call function with profileSet [CPSS_PORT_TX_DROP_PROFILE_5_E] */
        /* trafficClass [0], non-null tailDropProfileParamsPtr */
        profile = CPSS_PORT_TX_DROP_PROFILE_5_E;
        trafficClass = 0;

        st = cpssDxChPortTx4TcTailDropProfileSet(dev, profile, trafficClass, &params);
        if(devFamily != CPSS_PP_FAMILY_CHEETAH_E)
        {
            UTF_VERIFY_EQUAL3_PARAM_MAC(GT_OK, st, dev, profile, trafficClass);

            /*
                1.4. Call cpssDxChPortTx4TcTailDropProfileGet with non-NULL tailDropProfileParamsGet
                          and other parameters from 1.1.
                Expected: GT_OK and same tailDropProfileParamsGet as written
            */
            st = cpssDxChPortTx4TcTailDropProfileGet(dev, profile, trafficClass, &paramsGet);
            UTF_VERIFY_EQUAL1_STRING_MAC(GT_OK, st, "cpssDxChPortTx4TcTailDropProfileGet: %d", dev);

            /* invalidate variables */
            isEqual = (0 == cpssOsMemCmp(&params, &paramsGet, sizeof(params))) ? GT_TRUE : GT_FALSE;
            UTF_VERIFY_EQUAL1_STRING_MAC(GT_TRUE, isEqual, "got another params than was set: %d", dev);
        }
        else
        {
            UTF_VERIFY_NOT_EQUAL3_PARAM_MAC(GT_OK, st, dev, profile, trafficClass);
        }


        /* Call function with profileSet [CPSS_PORT_TX_DROP_PROFILE_8_E] */
        /* trafficClass [7], non-null tailDropProfileParamsPtr */
        profile = CPSS_PORT_TX_DROP_PROFILE_8_E;
        trafficClass = 7;

        st = cpssDxChPortTx4TcTailDropProfileSet(dev, profile, trafficClass, &params);
        if(devFamily != CPSS_PP_FAMILY_CHEETAH_E)
        {
            UTF_VERIFY_EQUAL3_PARAM_MAC(GT_OK, st, dev, profile, trafficClass);

            /*
                1.4. Call cpssDxChPortTx4TcTailDropProfileGet with non-NULL tailDropProfileParamsGet
                          and other parameters from 1.1.
                Expected: GT_OK and same tailDropProfileParamsGet as written
            */
            st = cpssDxChPortTx4TcTailDropProfileGet(dev, profile, trafficClass, &paramsGet);
            UTF_VERIFY_EQUAL1_STRING_MAC(GT_OK, st, "cpssDxChPortTx4TcTailDropProfileGet: %d", dev);
            /* invalidate variables */
            isEqual = (0 == cpssOsMemCmp(&params, &paramsGet, sizeof(params))) ? GT_TRUE : GT_FALSE;
            UTF_VERIFY_EQUAL1_STRING_MAC(GT_TRUE, isEqual, "got another params than was set: %d", dev);
        }
        else
        {
            UTF_VERIFY_NOT_EQUAL3_PARAM_MAC(GT_OK, st, dev, profile, trafficClass);
        }


        /* 1.5. Call with wrong enum values profileSet, trafficClass [0],
                          non-null tailDropProfileParamsPtr
           Expected: GT_BAD_PARAM.
        */
        trafficClass = 0;

        UTF_ENUMS_CHECK_MAC(cpssDxChPortTx4TcTailDropProfileSet
                            (dev, profile, trafficClass, &params),
                            profile);

        /* 1.6. Call with profileSet [CPSS_PORT_TX_DROP_PROFILE_1_E],
                          out of range trafficClass [CPSS_TC_RANGE_CNS = 8],
                          non-null tailDropProfileParamsPtr.
           Expected: GT_BAD_PARAM.
        */
        profile = CPSS_PORT_TX_DROP_PROFILE_1_E;
        trafficClass = CPSS_TC_RANGE_CNS;

        st = cpssDxChPortTx4TcTailDropProfileSet(dev, profile, trafficClass, &params);
        UTF_VERIFY_EQUAL3_PARAM_MAC(GT_BAD_PARAM, st, dev, profile, trafficClass);

        /* 1.7. Call with profileSet [CPSS_PORT_TX_DROP_PROFILE_2_E],
                          trafficClass [5],
                          null tailDropProfileParamsPtr [NULL].
           Expected: GT_BAD_PTR.
        */
        profile = CPSS_PORT_TX_DROP_PROFILE_2_E;
        trafficClass = 5;

        st = cpssDxChPortTx4TcTailDropProfileSet(dev, profile, trafficClass, NULL);
        UTF_VERIFY_EQUAL3_STRING_MAC(GT_BAD_PTR, st, "%d, %d, %d, NULL",
                                     dev, profile, trafficClass);

        /* 1.8. Call with profileSet [CPSS_PORT_TX_DROP_PROFILE_1_E],
           trafficClass [0], non-null tailDropProfileParamsPtr with field
           dp0MaxBuffNum/ dp1MaxBuffNum/ dp2MaxBuffNum/ dp0MaxDescrNum/
           dp1MaxDescrNum/ dp2MaxDescrNum [0xF000]
           Expected: GT_OK for tailDropProfileParamsPtr->dp1MaxDescrNum, tailDropProfileParamsPtr->dp1MaxBuffNum and NOT GT_OK for others.
        */
        profile = CPSS_PORT_TX_DROP_PROFILE_1_E;
        trafficClass = 0;

        params.dp0MaxBuffNum  = 0xF000;
        st = cpssDxChPortTx4TcTailDropProfileSet(dev, profile, trafficClass, &params);
        UTF_VERIFY_NOT_EQUAL4_STRING_MAC(GT_OK, st, "%d, %d, %d, params.dp0MaxBuffNum = %d",
                                     dev, profile, trafficClass, params.dp0MaxBuffNum);
        params.dp0MaxBuffNum  = 4; /* restore */

        params.dp0MaxDescrNum = 0xF000;
        st = cpssDxChPortTx4TcTailDropProfileSet(dev, profile, trafficClass, &params);
        UTF_VERIFY_NOT_EQUAL4_STRING_MAC(GT_OK, st, "%d, %d, %d, params.dp0MaxDescrNum = %d",
                                     dev, profile, trafficClass, params.dp0MaxDescrNum);
        params.dp0MaxDescrNum = 8; /* restore */

        params.dp1MaxBuffNum  = 0xF000;
        st = cpssDxChPortTx4TcTailDropProfileSet(dev, profile, trafficClass, &params);
        if (PRV_CPSS_PP_MAC(prvTgfDevNum)->devFamily == CPSS_PP_FAMILY_DXCH_LION_E)                       \
        {
            rc = GT_OUT_OF_RANGE;
        }
        else
        {
            rc = GT_OK;
        }
        UTF_VERIFY_EQUAL4_STRING_MAC(rc, st, "%d, %d, %d, params.dp1MaxBuffNum = %d",
                                     dev, profile, trafficClass, params.dp1MaxBuffNum);
        params.dp1MaxBuffNum  = 16; /* restore */

        params.dp1MaxDescrNum = 0xF000;
        st = cpssDxChPortTx4TcTailDropProfileSet(dev, profile, trafficClass, &params);
        UTF_VERIFY_EQUAL4_STRING_MAC(rc, st, "%d, %d, %d, params.dp1MaxDescrNum = %d",
                                     dev, profile, trafficClass, params.dp1MaxDescrNum);
        params.dp1MaxDescrNum = 32; /* restore */

        params.dp2MaxBuffNum  = 0xF000;
        st = cpssDxChPortTx4TcTailDropProfileSet(dev, profile, trafficClass, &params);
        UTF_VERIFY_NOT_EQUAL4_STRING_MAC(GT_OK, st, "%d, %d, %d, params.dp2MaxBuffNum = %d",
                                     dev, profile, trafficClass, params.dp2MaxBuffNum);
        params.dp2MaxBuffNum  = 64; /* restore */

        params.dp2MaxDescrNum = 0xF000;
        st = cpssDxChPortTx4TcTailDropProfileSet(dev, profile, trafficClass, &params);
        UTF_VERIFY_NOT_EQUAL4_STRING_MAC(GT_OK, st, "%d, %d, %d, params.dp2MaxDescrNum = %d",
                                     dev, profile, trafficClass, params.dp2MaxDescrNum);
        params.dp2MaxDescrNum = 128; /* restore */
    }

    /* 2. For not-active devices and devices from non-applicable family */
    /* check that function returns GT_BAD_PARAM.                        */

    /* prepare device iterator */
    PRV_TGF_NOT_APPLIC_DEV_RESET_MAC(&dev, UTF_NONE_FAMILY_E);
    /* go over all non active devices */
    while(GT_OK == prvUtfNextDeviceGet(&dev, GT_FALSE))
    {
        st = cpssDxChPortTx4TcTailDropProfileSet(dev, profile, trafficClass, &params);
        UTF_VERIFY_EQUAL1_PARAM_MAC(GT_NOT_APPLICABLE_DEVICE, st, dev);
    }

    /* 3. Call function with out of bound value for device id.*/

    dev = PRV_CPSS_MAX_PP_DEVICES_CNS;
    /* enable == GT_TRUE */

    st = cpssDxChPortTx4TcTailDropProfileSet(dev, profile, trafficClass, &params);
    UTF_VERIFY_EQUAL1_PARAM_MAC(GT_BAD_PARAM, st, dev);
}

/*----------------------------------------------------------------------------*/
/*
GT_STATUS cpssDxChPortTx4TcTailDropProfileGet
(
    IN    GT_U8                                   devNum,
    IN    CPSS_PORT_TX_DROP_PROFILE_SET_ENT       profileSet,
    IN    GT_U8                                   trafficClass,
    OUT   CPSS_PORT_TX_Q_TAIL_DROP_PROF_TC_PARAMS *tailDropProfileParamsPtr
)
*/
UTF_TEST_CASE_MAC(cpssDxChPortTx4TcTailDropProfileGet)
{
/*
    ITERATE_DEVICES (DxChx)
    1.1. Call with profileSet [CPSS_PORT_TX_DROP_PROFILE_1_E /
                               CPSS_PORT_TX_DROP_PROFILE_4_E],
                   trafficClass [0, 1],
                   and non-null tailDropProfileParamsPtr
    Expected: GT_OK.
    1.2. Call with profileSet [CPSS_PORT_TX_DROP_PROFILE_5_E /
                               CPSS_PORT_TX_DROP_PROFILE_8_E],
                   trafficClass [0, 7],
                   and non-null tailDropProfileParamsPtr
    Expected: GT_OK for Cheetah2 devices and NON GT_OK for others.
    1.3. Call with wrong enum values profileSet ,
                   trafficClass [0],
                   and non-null tailDropProfileParamsPtr.
    Expected: GT_BAD_PARAM.
    1.4. Call with profileSet [CPSS_PORT_TX_DROP_PROFILE_1_E],
                   with out of range trafficClass [CPSS_TC_RANGE_CNS = 8],
                   and non-null tailDropProfileParamsPtr.
    Expected: GT_BAD_PARAM.
    1.5. Call with tailDropProfileParamsPtr [NULL]
              and other parameters from 1.1.
    Expected: GT_BAD_PTR
*/
    GT_STATUS   st  = GT_OK;
    GT_U8       dev;

    CPSS_PP_FAMILY_TYPE_ENT                 devFamily    = CPSS_MAX_FAMILY;
    CPSS_PORT_TX_DROP_PROFILE_SET_ENT       profileSet   = CPSS_PORT_TX_DROP_PROFILE_1_E;
    GT_U8                                   trafficClass = 0;

    CPSS_PORT_TX_Q_TAIL_DROP_PROF_TC_PARAMS tailDropProfileParams;

    /* prepare iterator for go over all active devices */
    PRV_TGF_NOT_APPLIC_DEV_RESET_MAC(&dev, UTF_NONE_FAMILY_E);

    /* 1. Go over all active devices. */
    while (GT_OK == prvUtfNextDeviceGet(&dev, GT_TRUE))
    {
        /*
            1.1. Call with profileSet [CPSS_PORT_TX_DROP_PROFILE_1_E /
                                       CPSS_PORT_TX_DROP_PROFILE_4_E],
                           trafficClass [0, 1],
                           and non-null tailDropProfileParamsPtr
            Expected: GT_OK.
        */
        /* call with profileSet = CPSS_PORT_TX_DROP_PROFILE_1_E */
        profileSet   = CPSS_PORT_TX_DROP_PROFILE_1_E;
        trafficClass = 0;

        st = cpssDxChPortTx4TcTailDropProfileGet(dev, profileSet, trafficClass, &tailDropProfileParams);
        UTF_VERIFY_EQUAL3_PARAM_MAC(GT_OK, st, dev, profileSet, trafficClass);

        /* call with profileSet = CPSS_PORT_TX_DROP_PROFILE_4_E */
        profileSet   = CPSS_PORT_TX_DROP_PROFILE_4_E;
        trafficClass = 0;

        st = cpssDxChPortTx4TcTailDropProfileGet(dev, profileSet, trafficClass, &tailDropProfileParams);
        UTF_VERIFY_EQUAL3_PARAM_MAC(GT_OK, st, dev, profileSet, trafficClass);

        /*
            1.2. Call with profileSet [CPSS_PORT_TX_DROP_PROFILE_5_E /
                                       CPSS_PORT_TX_DROP_PROFILE_8_E],
                           trafficClass [0, 7],
                           and non-null tailDropProfileParamsPtr
            Expected: GT_OK for Cheetah2 devices and NON GT_OK for others.
        */
        /* call with profileSet = CPSS_PORT_TX_DROP_PROFILE_5_E */
        st = prvUtfDeviceFamilyGet(dev, &devFamily);
        UTF_VERIFY_EQUAL1_STRING_MAC(GT_OK, st, "prvUtfDeviceFamilyGet: %d", dev);

        profileSet   = CPSS_PORT_TX_DROP_PROFILE_5_E;
        trafficClass = 0;

        st = cpssDxChPortTx4TcTailDropProfileGet(dev, profileSet, trafficClass, &tailDropProfileParams);
        if(devFamily != CPSS_PP_FAMILY_CHEETAH_E)
        {
            UTF_VERIFY_EQUAL3_PARAM_MAC(GT_OK, st, dev, profileSet, trafficClass);
        }
        else
        {
            UTF_VERIFY_NOT_EQUAL3_PARAM_MAC(GT_OK, st, dev, profileSet, trafficClass);
        }

        /* call with profileSet = CPSS_PORT_TX_DROP_PROFILE_8_E */
        st = prvUtfDeviceFamilyGet(dev, &devFamily);
        UTF_VERIFY_EQUAL1_STRING_MAC(GT_OK, st, "prvUtfDeviceFamilyGet: %d", dev);

        profileSet   = CPSS_PORT_TX_DROP_PROFILE_8_E;
        trafficClass = 7;

        st = cpssDxChPortTx4TcTailDropProfileGet(dev, profileSet, trafficClass, &tailDropProfileParams);
        if(devFamily != CPSS_PP_FAMILY_CHEETAH_E)
        {
            UTF_VERIFY_EQUAL3_PARAM_MAC(GT_OK, st, dev, profileSet, trafficClass);
        }
        else
        {
            UTF_VERIFY_NOT_EQUAL3_PARAM_MAC(GT_OK, st, dev, profileSet, trafficClass);
        }

        /*
            1.3. Call with wrong enum values profileSet, trafficClass [0],
                           and non-null tailDropProfileParamsPtr.
            Expected: GT_BAD_PARAM.
        */

        UTF_ENUMS_CHECK_MAC(cpssDxChPortTx4TcTailDropProfileGet
                            (dev, profileSet, trafficClass, &tailDropProfileParams),
                            profileSet);

        /*
            1.4. Call with profileSet [CPSS_PORT_TX_DROP_PROFILE_1_E],
                           with out of range trafficClass [CPSS_TC_RANGE_CNS = 8],
                           and non-null tailDropProfileParamsPtr.
            Expected: GT_BAD_PARAM.
        */
        trafficClass = CPSS_TC_RANGE_CNS;

        st = cpssDxChPortTx4TcTailDropProfileGet(dev, profileSet, trafficClass, &tailDropProfileParams);
        UTF_VERIFY_EQUAL2_STRING_MAC(GT_BAD_PARAM, st, "%d, trafficClass = %d", dev, trafficClass);

        trafficClass = 0;

        /*
            1.5. Call with tailDropProfileParamsPtr [NULL]
                      and other parameters from 1.1.
            Expected: GT_OK
        */
        profileSet   = CPSS_PORT_TX_DROP_PROFILE_1_E;
        trafficClass = 0;

        st = cpssDxChPortTx4TcTailDropProfileGet(dev, profileSet, trafficClass, NULL);
        UTF_VERIFY_EQUAL1_STRING_MAC(GT_BAD_PTR, st, "%d, tailDropProfileParamsPtr = NULL", dev);
    }

    profileSet   = CPSS_PORT_TX_DROP_PROFILE_1_E;
    trafficClass = 0;

    /* 2. For not active devices check that function returns non GT_OK.*/
    /* prepare device iterator */
    PRV_TGF_NOT_APPLIC_DEV_RESET_MAC(&dev, UTF_NONE_FAMILY_E);

    /* Go over all non active devices. */
    while(GT_OK == prvUtfNextDeviceGet(&dev, GT_FALSE))
    {
        st = cpssDxChPortTx4TcTailDropProfileGet(dev, profileSet, trafficClass, &tailDropProfileParams);
        UTF_VERIFY_EQUAL1_PARAM_MAC(GT_NOT_APPLICABLE_DEVICE, st, dev);
    }

    /* 3.Call function with out of bound value for device id */
    dev = PRV_CPSS_MAX_PP_DEVICES_CNS;

    st = cpssDxChPortTx4TcTailDropProfileGet(dev, profileSet, trafficClass, &tailDropProfileParams);
    UTF_VERIFY_EQUAL1_PARAM_MAC(GT_BAD_PARAM, st, dev);
}

/*----------------------------------------------------------------------------*/
/*
GT_STATUS cpssDxChPortTxTailDropProfileSet
(
    IN  GT_U8                               devNum,
    IN  CPSS_PORT_TX_DROP_PROFILE_SET_ENT   profileSet,
    IN  GT_BOOL                             enableSharing,
    IN  GT_U16                              portMaxBuffLimit,
    IN  GT_U16                              portMaxDescrLimit
)
*/
UTF_TEST_CASE_MAC(cpssDxChPortTxTailDropProfileSet)
{
/*
    ITERATE_DEVICES (DxChx)
    1.1. Call with profileSet [CPSS_PORT_TX_DROP_PROFILE_1_E,
                               CPSS_PORT_TX_DROP_PROFILE_4_E],
                   enableSharing [GT_FALSE / GT_TRUE],
                   portMaxBuffLimit [0/35],
                   portMaxDescrLimit [0/32]
    Expected: GT_OK.
    1.2. Call cpssDxChPortTxTailDropProfileGet
              with non-NULL enableSharingPtr,
                            portMaxBuffLimitPtr,
                            portMaxDescrLimitPtr
                            and other parameters from 1.1.
    Expected: GT_OK
    1.3. Call with profileSet [CPSS_PORT_TX_DROP_PROFILE_5_E,
                               CPSS_PORT_TX_DROP_PROFILE_8_E],
                   enableSharing [GT_FALSE / GT_TRUE],
                   portMaxBuffLimit [4/7],
                   portMaxDescrLimit [45/7]
    Expected: GT_OK for Cheetah2 devices and NON GT_OK for others.
    1.4. Call cpssDxChPortTxTailDropProfileGet
              with non-NULL enableSharingPtr,
                            portMaxBuffLimitPtr,
                            portMaxDescrLimitPtr
                            and other parameters from 1.3.
    Expected: GT_OK
    1.5. Call with wrong enum values profileSet ,
                                enableSharing [GT_FALSE and GT_TRUE],
                                portMaxBuffLimit [2],
                                portMaxDescrLimit [1].
    Expected: GT_BAD_PARAM.
    1.6. Call with profileSet [CPSS_PORT_TX_DROP_PROFILE_2_E],
                          enableSharing [GT_FALSE and GT_TRUE],
                          for DxCh and DxCh2 portMaxBuffLimit [4096], for dxCh3 portMaxBuffLimit [16384] (out of range),
                          portMaxDescrLimit [10].
   Expected: NOT GT_OK.
    1.7. Call with profileSet [CPSS_PORT_TX_DROP_PROFILE_1_E],
                   enableSharing [GT_FALSE and GT_TRUE],
                   portMaxBuffLimit [1],
                   for DxCh and DxCh2 portMaxDescrLimit [4096], for dxCh3 portMaxDescrLimit [16384] (out of range).
    Expected: NOT GT_OK.
*/

    GT_STATUS   st     = GT_OK;

    GT_U8                             dev;
    CPSS_PORT_TX_DROP_PROFILE_SET_ENT profile          = 0;
    GT_BOOL                           sharing          = GT_FALSE;
    GT_U16                            MaxBuffLimit     = 0;
    GT_U16                            MaxDescrLimit    = 0;
    CPSS_PP_FAMILY_TYPE_ENT           devFamily        = CPSS_MAX_FAMILY;
    GT_BOOL                           sharingGet       = GT_FALSE;
    GT_U16                            MaxBuffLimitGet  = 0;
    GT_U16                            MaxDescrLimitGet = 0;


    /* prepare device iterator */
    PRV_TGF_NOT_APPLIC_DEV_RESET_MAC(&dev, UTF_NONE_FAMILY_E);

    /* 1. Go over all active devices. */
    while (GT_OK == prvUtfNextDeviceGet(&dev, GT_TRUE))
    {
        /* 1.1. Call with profileSet [CPSS_PORT_TX_DROP_PROFILE_1_E /
                                      CPSS_PORT_TX_DROP_PROFILE_4_E] /
                                      sharing [GT_FALSE and GT_TRUE] /
                                      portMaxBuffLimit [0/35],
                                      portMaxDescrLimit [0/32]
          Expected: GT_OK.
        */


        /* Call function with [enableSharing == GT_FALSE] */
        sharing       = GT_FALSE;
        profile       = CPSS_PORT_TX_DROP_PROFILE_1_E;
        MaxBuffLimit  = 0;
        MaxDescrLimit = 0;

        st = cpssDxChPortTxTailDropProfileSet(dev, profile, sharing, MaxBuffLimit, MaxDescrLimit);
        UTF_VERIFY_EQUAL5_PARAM_MAC(GT_OK, st, dev, profile, sharing, MaxBuffLimit, MaxDescrLimit);

        /*
            1.2. Call cpssDxChPortTxTailDropProfileGet
                      with non-NULL enableSharingPtr,
                                    portMaxBuffLimitPtr,
                                    portMaxDescrLimitPtr
                                    and other parameters from 1.1.
            Expected: GT_OK
        */
        st = cpssDxChPortTxTailDropProfileGet(dev, profile, &sharingGet, &MaxBuffLimitGet, &MaxDescrLimitGet);
        UTF_VERIFY_EQUAL1_STRING_MAC(GT_OK, st, "cpssDxChPortTxTailDropProfileGet: %d", dev);

        /* invalidate variables */
        UTF_VERIFY_EQUAL1_STRING_MAC(sharing, sharingGet, "got another sharing than was set: %d", dev);
        UTF_VERIFY_EQUAL1_STRING_MAC(MaxBuffLimit, MaxBuffLimitGet, "got another MaxBuffLimit than was set: %d", dev);
        UTF_VERIFY_EQUAL1_STRING_MAC(MaxDescrLimit, MaxDescrLimitGet, "got another MaxDescrLimit than was set: %d", dev);


        /* Call function with [enableSharing == GT_FALSE] */
        sharing       = GT_FALSE;
        profile       = CPSS_PORT_TX_DROP_PROFILE_4_E;
        MaxBuffLimit  = 35;
        MaxDescrLimit = 32;

        st = cpssDxChPortTxTailDropProfileSet(dev, profile, sharing, MaxBuffLimit, MaxDescrLimit);
        UTF_VERIFY_EQUAL5_PARAM_MAC(GT_OK, st, dev, profile, sharing, MaxBuffLimit, MaxDescrLimit);

        /*
            1.2. Call cpssDxChPortTxTailDropProfileGet
                      with non-NULL enableSharingPtr,
                                    portMaxBuffLimitPtr,
                                    portMaxDescrLimitPtr
                                    and other parameters from 1.1.
            Expected: GT_OK
        */
        st = cpssDxChPortTxTailDropProfileGet(dev, profile, &sharingGet, &MaxBuffLimitGet, &MaxDescrLimitGet);
        UTF_VERIFY_EQUAL1_STRING_MAC(GT_OK, st, "cpssDxChPortTxTailDropProfileGet: %d", dev);

        /* invalidate variables */
        UTF_VERIFY_EQUAL1_STRING_MAC(sharing, sharingGet, "got another sharing than was set: %d", dev);
        UTF_VERIFY_EQUAL1_STRING_MAC(MaxBuffLimit, MaxBuffLimitGet, "got another MaxBuffLimit than was set: %d", dev);
        UTF_VERIFY_EQUAL1_STRING_MAC(MaxDescrLimit, MaxDescrLimitGet, "got another MaxDescrLimit than was set: %d", dev);


        st = prvUtfDeviceFamilyGet(dev, &devFamily);
        UTF_VERIFY_EQUAL1_STRING_MAC(GT_OK, st, "prvUtfDeviceFamilyGet: %d", dev);
        /*
            1.3. Call with profileSet [CPSS_PORT_TX_DROP_PROFILE_5_E,
                                       CPSS_PORT_TX_DROP_PROFILE_8_E],
                           enableSharing [GT_FALSE / GT_TRUE],
                           portMaxBuffLimit [4/7],
                           portMaxDescrLimit [45/7]
            Expected: GT_OK for Cheetah2 devices and NON GT_OK for others.
        */
        /* Call function with [enableSharing == GT_FALSE] */
        sharing       = GT_FALSE;
        profile       = CPSS_PORT_TX_DROP_PROFILE_5_E;
        MaxBuffLimit  = 4;
        MaxDescrLimit = 45;

        st = cpssDxChPortTxTailDropProfileSet(dev, profile, sharing, MaxBuffLimit, MaxDescrLimit);
        if(devFamily != CPSS_PP_FAMILY_CHEETAH_E)
        {
            UTF_VERIFY_EQUAL5_PARAM_MAC(GT_OK, st, dev, profile, sharing, MaxBuffLimit, MaxDescrLimit);

            /*
                1.4. Call cpssDxChPortTxTailDropProfileGet
                          with non-NULL enableSharingPtr,
                                        portMaxBuffLimitPtr,
                                        portMaxDescrLimitPtr
                                        and other parameters from 1.1.
                Expected: GT_OK
            */
            st = cpssDxChPortTxTailDropProfileGet(dev, profile, &sharingGet, &MaxBuffLimitGet, &MaxDescrLimitGet);
            UTF_VERIFY_EQUAL1_STRING_MAC(GT_OK, st, "cpssDxChPortTxTailDropProfileGet: %d", dev);

            /* invalidate variables */
            UTF_VERIFY_EQUAL1_STRING_MAC(sharing, sharingGet, "got another sharing than was set: %d", dev);
            UTF_VERIFY_EQUAL1_STRING_MAC(MaxBuffLimit, MaxBuffLimitGet, "got another MaxBuffLimit than was set: %d", dev);
            UTF_VERIFY_EQUAL1_STRING_MAC(MaxDescrLimit, MaxDescrLimitGet, "got another MaxDescrLimit than was set: %d", dev);
        }
        else
        {
            UTF_VERIFY_NOT_EQUAL5_PARAM_MAC(GT_OK, st, dev, profile, sharing, MaxBuffLimit, MaxDescrLimit);
        }

        /* Call function with [enableSharing == GT_FALSE] */
        sharing       = GT_FALSE;
        profile       = CPSS_PORT_TX_DROP_PROFILE_8_E;
        MaxBuffLimit  = 7;
        MaxDescrLimit = 7;

        st = cpssDxChPortTxTailDropProfileSet(dev, profile, sharing, MaxBuffLimit, MaxDescrLimit);
        if(devFamily != CPSS_PP_FAMILY_CHEETAH_E)
        {
            UTF_VERIFY_EQUAL5_PARAM_MAC(GT_OK, st, dev, profile, sharing, MaxBuffLimit, MaxDescrLimit);
            /*
                1.4. Call cpssDxChPortTxTailDropProfileGet
                          with non-NULL enableSharingPtr,
                                        portMaxBuffLimitPtr,
                                        portMaxDescrLimitPtr
                                        and other parameters from 1.1.
                Expected: GT_OK
            */
            st = cpssDxChPortTxTailDropProfileGet(dev, profile, &sharingGet, &MaxBuffLimitGet, &MaxDescrLimitGet);
            UTF_VERIFY_EQUAL1_STRING_MAC(GT_OK, st, "cpssDxChPortTxTailDropProfileGet: %d", dev);

            /* invalidate variables */
            UTF_VERIFY_EQUAL1_STRING_MAC(sharing, sharingGet, "got another sharing than was set: %d", dev);
            UTF_VERIFY_EQUAL1_STRING_MAC(MaxBuffLimit, MaxBuffLimitGet, "got another MaxBuffLimit than was set: %d", dev);
            UTF_VERIFY_EQUAL1_STRING_MAC(MaxDescrLimit, MaxDescrLimitGet, "got another MaxDescrLimit than was set: %d", dev);
        }
        else
        {
            UTF_VERIFY_NOT_EQUAL5_PARAM_MAC(GT_OK, st, dev, profile, sharing, MaxBuffLimit, MaxDescrLimit);
        }

        /* 1.5. Call with wrong enum values profileSet ,
                                       enableSharing [GT_FALSE and GT_TRUE],
                                       portMaxBuffLimit [2],
                                       portMaxDescrLimit [1].
           Expected: GT_BAD_PARAM.
        */
        sharing = GT_TRUE;
        MaxBuffLimit = 2;
        MaxDescrLimit = 1;

        UTF_ENUMS_CHECK_MAC(cpssDxChPortTxTailDropProfileSet
                            (dev, profile, sharing, MaxBuffLimit, MaxDescrLimit),
                            profile);

        /* 1.6. Call with profileSet [CPSS_PORT_TX_DROP_PROFILE_2_E],
                          enableSharing [GT_FALSE and GT_TRUE],
                          for DxCh and DxCh2 portMaxBuffLimit [4096], for dxCh3 portMaxBuffLimit [16384] (out of range),
                          portMaxDescrLimit [10].
           Expected: NOT GT_OK.
        */
        profile = CPSS_PORT_TX_DROP_PROFILE_2_E;
        sharing = GT_TRUE;

        if( (PRV_CPSS_PP_MAC(dev)->devFamily == CPSS_PP_FAMILY_CHEETAH_E) ||
            (PRV_CPSS_PP_MAC(dev)->devFamily == CPSS_PP_FAMILY_CHEETAH2_E) )
        {
            MaxBuffLimit = 4096;
        }
        else
        {
            MaxBuffLimit = 16384;
        }

        MaxDescrLimit = 10;

        st = cpssDxChPortTxTailDropProfileSet(dev, profile, sharing, MaxBuffLimit, MaxDescrLimit);
        UTF_VERIFY_NOT_EQUAL5_PARAM_MAC(GT_OK, st, dev, profile, sharing, MaxBuffLimit, MaxDescrLimit);

        /* 1.7. Call with profileSet [CPSS_PORT_TX_DROP_PROFILE_1_E],
                          enableSharing [GT_FALSE and GT_TRUE],
                          portMaxBuffLimit [1],
                          for DxCh and DxCh2 portMaxDescrLimit [4096], for dxCh3 portMaxDescrLimit [16384] (out of range).
          Expected: NOT GT_OK.
        */
        profile = CPSS_PORT_TX_DROP_PROFILE_1_E;
        sharing = GT_TRUE;

        if( (PRV_CPSS_PP_MAC(dev)->devFamily == CPSS_PP_FAMILY_CHEETAH_E) ||
            (PRV_CPSS_PP_MAC(dev)->devFamily == CPSS_PP_FAMILY_CHEETAH2_E) )
        {
            MaxDescrLimit = 4096;
        }
        else
        {
            MaxDescrLimit = 16384;
        }

        st = cpssDxChPortTxTailDropProfileSet(dev, profile, sharing, MaxBuffLimit, MaxDescrLimit);
        UTF_VERIFY_NOT_EQUAL5_PARAM_MAC(GT_OK, st, dev, profile, sharing, MaxBuffLimit, MaxDescrLimit);
    }

    /* 2. For not-active devices and devices from non-applicable family */
    /* check that function returns GT_BAD_PARAM.                        */
    profile = CPSS_PORT_TX_DROP_PROFILE_3_E;
    sharing = GT_TRUE;
    MaxBuffLimit = 10;
    MaxDescrLimit = 8;

    /* prepare device iterator */
    PRV_TGF_NOT_APPLIC_DEV_RESET_MAC(&dev, UTF_NONE_FAMILY_E);

    /* go over all non active devices */
    /* enable == GT_TRUE              */
    while(GT_OK == prvUtfNextDeviceGet(&dev, GT_FALSE))
    {
        /* Call function with profileSet [CPSS_PORT_TX_DROP_PROFILE_3_E]
                              enableSharing [GT_TRUE]
                              portMaxBuffLimit [10],
                              portMaxDescrLimit [8]
           Expected: GT_BAD_PARAM.
        */
        st = cpssDxChPortTxTailDropProfileSet(dev, profile, sharing, MaxBuffLimit, MaxDescrLimit);
        UTF_VERIFY_EQUAL1_PARAM_MAC(GT_NOT_APPLICABLE_DEVICE, st, dev);
    }

    /* 3. Call function with out of bound value for device id.*/

    dev = PRV_CPSS_MAX_PP_DEVICES_CNS;
    /* enable == GT_TRUE */

    st = cpssDxChPortTxTailDropProfileSet(dev, profile, sharing, MaxBuffLimit, MaxDescrLimit);
    UTF_VERIFY_EQUAL1_PARAM_MAC(GT_BAD_PARAM, st, dev);
}

/*----------------------------------------------------------------------------*/
/*
GT_STATUS cpssDxChPortTxTailDropProfileGet
(
    IN  GT_U8                               devNum,
    IN  CPSS_PORT_TX_DROP_PROFILE_SET_ENT   profileSet,
    OUT GT_BOOL                             *enableSharingPtr,
    OUT GT_U16                              *portMaxBuffLimitPtr,
    OUT GT_U16                              *portMaxDescrLimitPtr
)
*/
UTF_TEST_CASE_MAC(cpssDxChPortTxTailDropProfileGet)
{
/*
    ITERATE_DEVICES (DxCh)
    1.1. Call with profileSet [CPSS_PORT_TX_DROP_PROFILE_1_E /
                               CPSS_PORT_TX_DROP_PROFILE_4_E],
                   and non-NULL enableSharing,
                   portMaxBuffLimit,
                   portMaxDescrLimit.
    Expected: GT_OK.
    1.2. Call with profileSet [CPSS_PORT_TX_DROP_PROFILE_5_E /
                               CPSS_PORT_TX_DROP_PROFILE_8_E],
                   and non-NULL enableSharing,
                   portMaxBuffLimit,
                   portMaxDescrLimit.
    Expected: GT_OK for Cheetah2 devices and NON GT_OK for others.
    1.3. Call with wrong enum values profileSet ,
                   and non-NULL enableSharing,
                   portMaxBuffLimit,
                   portMaxDescrLimit.
    Expected: GT_BAD_PARAM.
    1.4. Call with enableSharingPtr [NULL]
                   and other parameters from 1.1.
    Expected: GT_BAD_PTR.
    1.5. Call with portMaxBuffLimitPtr [NULL]
                   and other parameters from 1.1.
    Expected: GT_BAD_PTR.
    1.6. Call with portMaxDescrLimitPtr [NULL]
                   and other parameters from 1.1.
    Expected: GT_BAD_PTR.
*/
    GT_STATUS   st  = GT_OK;
    GT_U8       dev;

    CPSS_PP_FAMILY_TYPE_ENT             devFamily         = CPSS_MAX_FAMILY;
    CPSS_PORT_TX_DROP_PROFILE_SET_ENT   profileSet        = CPSS_PORT_TX_DROP_PROFILE_1_E;
    GT_BOOL                             enableSharing     = GT_FALSE;
    GT_U16                              portMaxBuffLimit  = 0;
    GT_U16                              portMaxDescrLimit = 0;


    /* prepare iterator for go over all active devices */
    PRV_TGF_NOT_APPLIC_DEV_RESET_MAC(&dev, UTF_NONE_FAMILY_E);

    /* 1. Go over all active devices. */
    while (GT_OK == prvUtfNextDeviceGet(&dev, GT_TRUE))
    {
        /*
            1.1. Call with profileSet [CPSS_PORT_TX_DROP_PROFILE_1_E /
                                       CPSS_PORT_TX_DROP_PROFILE_4_E],
                           and non-NULL enableSharing,
                           portMaxBuffLimit,
                           portMaxDescrLimit.
            Expected: GT_OK.
        */
        /* call with profileSet = CPSS_PORT_TX_DROP_PROFILE_1_E */
        profileSet = CPSS_PORT_TX_DROP_PROFILE_1_E;

        st = cpssDxChPortTxTailDropProfileGet(dev, profileSet, &enableSharing,
                                              &portMaxBuffLimit, &portMaxDescrLimit);
        UTF_VERIFY_EQUAL2_PARAM_MAC(GT_OK, st, dev, profileSet);

        /* call with profileSet = CPSS_PORT_TX_DROP_PROFILE_4_E */
        profileSet = CPSS_PORT_TX_DROP_PROFILE_4_E;

        st = cpssDxChPortTxTailDropProfileGet(dev, profileSet, &enableSharing,
                                              &portMaxBuffLimit, &portMaxDescrLimit);
        UTF_VERIFY_EQUAL2_PARAM_MAC(GT_OK, st, dev, profileSet);

        /*
            1.2. Call with profileSet [CPSS_PORT_TX_DROP_PROFILE_5_E /
                                       CPSS_PORT_TX_DROP_PROFILE_8_E],
                           and non-NULL enableSharing,
                           portMaxBuffLimit,
                           portMaxDescrLimit.
            Expected: GT_OK for Cheetah2 devices and NON GT_OK for others.
        */
        st = prvUtfDeviceFamilyGet(dev, &devFamily);
        UTF_VERIFY_EQUAL1_STRING_MAC(GT_OK, st, "prvUtfDeviceFamilyGet: %d", dev);

        /* call with profileSet = CPSS_PORT_TX_DROP_PROFILE_5_E */
        profileSet = CPSS_PORT_TX_DROP_PROFILE_5_E;

        st = cpssDxChPortTxTailDropProfileGet(dev, profileSet, &enableSharing,
                                              &portMaxBuffLimit, &portMaxDescrLimit);
        if(devFamily != CPSS_PP_FAMILY_CHEETAH_E)
        {
            UTF_VERIFY_EQUAL2_PARAM_MAC(GT_OK, st, dev, profileSet);
        }
        else
        {
            UTF_VERIFY_NOT_EQUAL2_PARAM_MAC(GT_OK, st, dev, profileSet);
        }

        /* call with profileSet = CPSS_PORT_TX_DROP_PROFILE_8_E */
        profileSet = CPSS_PORT_TX_DROP_PROFILE_8_E;

        st = cpssDxChPortTxTailDropProfileGet(dev, profileSet, &enableSharing,
                                              &portMaxBuffLimit, &portMaxDescrLimit);
        if(devFamily != CPSS_PP_FAMILY_CHEETAH_E)
        {
            UTF_VERIFY_EQUAL2_PARAM_MAC(GT_OK, st, dev, profileSet);
        }
        else
        {
            UTF_VERIFY_NOT_EQUAL2_PARAM_MAC(GT_OK, st, dev, profileSet);
        }

        /*
            1.3. Call with wrong enum values profileSet ,
                           and non-NULL enableSharing,
                           portMaxBuffLimit,
                           portMaxDescrLimit.
            Expected: GT_BAD_PARAM.
        */
        UTF_ENUMS_CHECK_MAC(cpssDxChPortTxTailDropProfileGet
                            (dev, profileSet, &enableSharing,
                             &portMaxBuffLimit, &portMaxDescrLimit),
                            profileSet);

        /*
            1.4. Call with enableSharingPtr [NULL]
                           and other parameters from 1.1.
            Expected: GT_BAD_PTR.
        */
        st = cpssDxChPortTxTailDropProfileGet(dev, profileSet, NULL,
                                              &portMaxBuffLimit, &portMaxDescrLimit);
        UTF_VERIFY_EQUAL1_STRING_MAC(GT_BAD_PTR, st, "%d, enableSharingPtr = NULL", dev);

        /*
            1.5. Call with portMaxBuffLimitPtr [NULL]
                           and other parameters from 1.1.
            Expected: GT_BAD_PTR.
        */
        st = cpssDxChPortTxTailDropProfileGet(dev, profileSet, &enableSharing,
                                              NULL, &portMaxDescrLimit);
        UTF_VERIFY_EQUAL1_STRING_MAC(GT_BAD_PTR, st, "%d, portMaxBuffLimitPtr = NULL", dev);

        /*
            1.6. Call with portMaxDescrLimitPtr [NULL]
                           and other parameters from 1.1.
            Expected: GT_BAD_PTR.
        */
        st = cpssDxChPortTxTailDropProfileGet(dev, profileSet, &enableSharing,
                                              &portMaxBuffLimit, NULL);
        UTF_VERIFY_EQUAL1_STRING_MAC(GT_BAD_PTR, st, "%d, portMaxDescrLimitPtr = NULL", dev);
    }

    profileSet = CPSS_PORT_TX_DROP_PROFILE_1_E;

    /* 2. For not active devices check that function returns non GT_OK.*/
    /* prepare device iterator */
    PRV_TGF_NOT_APPLIC_DEV_RESET_MAC(&dev, UTF_NONE_FAMILY_E);

    /* Go over all non active devices. */
    while(GT_OK == prvUtfNextDeviceGet(&dev, GT_FALSE))
    {
        st = cpssDxChPortTxTailDropProfileGet(dev, profileSet, &enableSharing,
                                              &portMaxBuffLimit, &portMaxDescrLimit);
        UTF_VERIFY_EQUAL1_PARAM_MAC(GT_NOT_APPLICABLE_DEVICE, st, dev);
    }

    /* 3.Call function with out of bound value for device id */
    dev = PRV_CPSS_MAX_PP_DEVICES_CNS;

    st = cpssDxChPortTxTailDropProfileGet(dev, profileSet, &enableSharing,
                                              &portMaxBuffLimit, &portMaxDescrLimit);
    UTF_VERIFY_EQUAL1_PARAM_MAC(GT_BAD_PARAM, st, dev);
}

/*----------------------------------------------------------------------------*/
/*
GT_STATUS cpssDxChPortTxSniffedPcktDescrLimitSet
(
    IN    GT_U8                  devNum,
    IN    GT_U32                 rxSniffMaxDescNum,
    IN    GT_U32                 txSniffMaxDescNum
)
*/
UTF_TEST_CASE_MAC(cpssDxChPortTxSniffedPcktDescrLimitSet)
{
/*
    ITERATE_DEVICES (DxChx)
    1.1. Call with rxSniffMaxDescNum [0 - 0xFFF], txSniffMaxDescNum [0 - 0xFFF].
    Expected: GT_OK.
    1.2. Call cpssDxChPortTxSniffedPcktDescrLimitGet.
    Expected: GT_OK and the same values.
    1.3. Call with rxSniffMaxDescNum [0xFFF - 0x3FFF],
                   txSniffMaxDescNum [0xFFF - 0x3FFF].
    Expected: GT_OK for dxChx and above and not GT_OK for other.
    1.4. Call with out of range
         rxSniffMaxDescNum [PORT_TX_INVALID_MIRR_2_ANLY_DESC_MAX_CNS]
         and txSniffMaxDescNum[4].
    Expected: NOT GT_OK.
    1.5. Call with rxSniffMaxDescNum [81] and out of range
         txSniffMaxDescNum [PORT_TX_INVALID_MIRR_2_ANLY_DESC_MAX_CNS].
    Expected: NOT GT_OK.
*/
    GT_STATUS st    = GT_OK;

    GT_U8     dev;
    GT_U32    rxNum = 0;
    GT_U32    txNum = 0;

    GT_U32    rxNumGet;
    GT_U32    txNumGet;

    CPSS_PP_FAMILY_TYPE_ENT  devFamily = CPSS_MAX_FAMILY;

    /* prepare device iterator */
    PRV_TGF_NOT_APPLIC_DEV_RESET_MAC(&dev, UTF_NONE_FAMILY_E);

    /* 1. Go over all active devices. */
    while (GT_OK == prvUtfNextDeviceGet(&dev, GT_TRUE))
    {
        st = prvUtfDeviceFamilyGet(dev, &devFamily);
        UTF_VERIFY_EQUAL1_STRING_MAC(GT_OK, st, "prvUtfDeviceFamilyGet: %d", dev);

        /*
           1.1. Call with rxSniffMaxDescNum [0 - 0xFFF],
                          txSniffMaxDescNum [0 - 0xFFF].
           Expected: GT_OK.
        */
        for(rxNum = 0; rxNum < 0xFFF; rxNum += 10)
            for(txNum = 0; txNum < 0xFFF; txNum += 50)
            {
                st = cpssDxChPortTxSniffedPcktDescrLimitSet(dev, rxNum, txNum);
                UTF_VERIFY_EQUAL3_PARAM_MAC(GT_OK, st, dev, rxNum, txNum);

                /*
                    1.2. Call cpssDxChPortTxSniffedPcktDescrLimitGet.
                    Expected: GT_OK and the same values.
                */
                st = cpssDxChPortTxSniffedPcktDescrLimitGet(dev, &rxNumGet, &txNumGet);
                UTF_VERIFY_EQUAL1_PARAM_MAC(GT_OK, st, dev);
            }

        /*
           1.3. Call with rxSniffMaxDescNum [0x1000 - 0x3FFF],
                          txSniffMaxDescNum [0x1000 - 0x3FFF].
           Expected: GT_OK for dxChx and above and not GT_OK for other.
        */
        for(rxNum = 0x1000; rxNum < 0x3FFF; rxNum += 10)
            for(txNum = 0x1000; txNum < 0x3FFF; txNum += 50)
            {
                st = cpssDxChPortTxSniffedPcktDescrLimitSet(dev, rxNum, txNum);

                if( devFamily != CPSS_PP_FAMILY_CHEETAH_E  &&
                    devFamily != CPSS_PP_FAMILY_CHEETAH2_E )
                {
                    UTF_VERIFY_EQUAL3_PARAM_MAC(GT_OK, st, dev, rxNum, txNum);

                    /*
                        1.2. Call cpssDxChPortTxSniffedPcktDescrLimitGet.
                        Expected: GT_OK and the same values.
                    */
                    st = cpssDxChPortTxSniffedPcktDescrLimitGet(dev, &rxNumGet, &txNumGet);
                    UTF_VERIFY_EQUAL1_PARAM_MAC(GT_OK, st, dev);
                }
                else
                {
                    UTF_VERIFY_NOT_EQUAL3_PARAM_MAC(GT_OK, st, dev, rxNum, txNum);
                }

            }

        /*
           1.4. Call with rxSniffMaxDescNum [PORT_TX_INVALID_MIRR_2_ANLY_DESC_MAX_CNS]
                          txSniffMaxDescNum[4].
           Expected: NOT GT_OK.
        */
        if(0 == PRV_CPSS_DXCH_PP_HW_INFO_TXQ_REV_1_OR_ABOVE_MAC(dev))
        {
            /* 14 bits enough for this device */
            rxNum = BIT_14;
        }
        else
        {
            /* 16 bits enough for this device */
            rxNum = BIT_16;
        }
        txNum = 4;

        st = cpssDxChPortTxSniffedPcktDescrLimitSet(dev, rxNum, txNum);
        UTF_VERIFY_NOT_EQUAL3_PARAM_MAC(GT_OK, st, dev, rxNum, txNum);

        /*
           1.5. Call with rxSniffMaxDescNum [81]
                          txSniffMaxDescNum [PORT_TX_INVALID_MIRR_2_ANLY_DESC_MAX_CNS]
           Expected: NOT GT_OK.
        */
        rxNum = 81;
        if(0 == PRV_CPSS_DXCH_PP_HW_INFO_TXQ_REV_1_OR_ABOVE_MAC(dev))
        {
            /* 14 bits enough for this device */
            txNum = BIT_14;
        }
        else
        {
            /* 16 bits enough for this device */
            txNum = BIT_16;
        }

        st = cpssDxChPortTxSniffedPcktDescrLimitSet(dev, rxNum, txNum);
        UTF_VERIFY_NOT_EQUAL3_PARAM_MAC(GT_OK, st, dev, rxNum, txNum);
    }

    /* 2. For not-active devices and devices from non-applicable family */
    /* check that function returns GT_BAD_PARAM.                        */

    rxNum = 96;
    txNum = 8;

    /* prepare device iterator */
    PRV_TGF_NOT_APPLIC_DEV_RESET_MAC(&dev, UTF_NONE_FAMILY_E);

    /* go over all non active devices */
    while(GT_OK == prvUtfNextDeviceGet(&dev, GT_FALSE))
    {
        /* rxSniffMaxDescNum [96], txSniffMaxDescNum[8] */
        st = cpssDxChPortTxSniffedPcktDescrLimitSet(dev, rxNum, txNum);
        UTF_VERIFY_EQUAL1_PARAM_MAC(GT_NOT_APPLICABLE_DEVICE, st, dev);
    }

    /* 3. Call function with out of bound value for device id.*/
    dev = PRV_CPSS_MAX_PP_DEVICES_CNS;
    /* rxSniffMaxDescNum [96], txSniffMaxDescNum[8] */

    st = cpssDxChPortTxSniffedPcktDescrLimitSet(dev, rxNum, txNum);
    UTF_VERIFY_EQUAL1_PARAM_MAC(GT_BAD_PARAM, st, dev);
}

/*----------------------------------------------------------------------------*/
/*
GT_STATUS cpssDxChPortTxSniffedPcktDescrLimitGet
(
    IN    GT_U8                  devNum,
    OUT   GT_U32                *rxSniffMaxDescNumPtr,
    OUT   GT_U32                *txSniffMaxDescNumPtr
)
*/
UTF_TEST_CASE_MAC(cpssDxChPortTxSniffedPcktDescrLimitGet)
{
/*
    ITERATE_DEVICES (DxChx)
    1.1. Call with non-NULL pointers.
    Expected: GT_OK.
    1.2. Call with wrong rxSniffMaxDescNumPtr null pointer.
    Expected: GT_BAD_PTR.
    1.2. Call with wrong txSniffMaxDescNumPtr null pointer.
    Expected: GT_BAD_PTR.
*/
    GT_STATUS   st  = GT_OK;
    GT_U8       dev;
    GT_U32      rxSniffMaxDescNumPtr;
    GT_U32      txSniffMaxDescNumPtr;

    /* prepare iterator for go over all active devices */
    PRV_TGF_NOT_APPLIC_DEV_RESET_MAC(&dev, UTF_NONE_FAMILY_E);

    /* 1. Go over all active devices. */
    while (GT_OK == prvUtfNextDeviceGet(&dev, GT_TRUE))
    {
        /*
            1.1. Call with non-NULL pointers.
            Expected: GT_OK.
        */
        st = cpssDxChPortTxSniffedPcktDescrLimitGet(dev,
                                  &rxSniffMaxDescNumPtr, &txSniffMaxDescNumPtr);
        UTF_VERIFY_EQUAL1_PARAM_MAC(GT_OK, st, dev);

        /*
            1.2. Call with wrong rxSniffMaxDescNumPtr null pointer.
            Expected: GT_BAD_PTR.
        */
        st = cpssDxChPortTxSniffedPcktDescrLimitGet(dev, NULL, &txSniffMaxDescNumPtr);
        UTF_VERIFY_EQUAL1_STRING_MAC(GT_BAD_PTR, st,
                                  "%d, rxSniffMaxDescNumPtr = NULL", dev);

        /*
            1.2. Call with wrong txSniffMaxDescNumPtr null pointer.
            Expected: GT_BAD_PTR.
        */
        st = cpssDxChPortTxSniffedPcktDescrLimitGet(dev, &rxSniffMaxDescNumPtr, NULL);
        UTF_VERIFY_EQUAL1_STRING_MAC(GT_BAD_PTR, st,
                                  "%d, txSniffMaxDescNumPtr = NULL", dev);
    }

    /* 2. For not active devices check that function returns non GT_OK.*/
    /* prepare device iterator */
    PRV_TGF_NOT_APPLIC_DEV_RESET_MAC(&dev, UTF_NONE_FAMILY_E);

    /* Go over all non active devices. */
    while(GT_OK == prvUtfNextDeviceGet(&dev, GT_FALSE))
    {
        st = cpssDxChPortTxSniffedPcktDescrLimitGet(dev,
                                  &rxSniffMaxDescNumPtr, &txSniffMaxDescNumPtr);
        UTF_VERIFY_EQUAL1_PARAM_MAC(GT_NOT_APPLICABLE_DEVICE, st, dev);
    }

    /* 3.Call function with out of bound value for device id */
    dev = PRV_CPSS_MAX_PP_DEVICES_CNS;

    st = cpssDxChPortTxSniffedPcktDescrLimitGet(dev,
                                  &rxSniffMaxDescNumPtr, &txSniffMaxDescNumPtr);
    UTF_VERIFY_EQUAL1_PARAM_MAC(GT_BAD_PARAM, st, dev);
}

/*----------------------------------------------------------------------------*/
/*
GT_STATUS cpssDxChPortTxMcastPcktDescrLimitSet
(
    IN    GT_U8                  devNum,
    IN    GT_U32                 mcastMaxDescNum
)
*/
UTF_TEST_CASE_MAC(cpssDxChPortTxMcastPcktDescrLimitSet)
{
    /*
    ITERATE_DEVICES (DxCh)
    1.1. Call with mcastMaxDescNum [4].
    Expected: GT_OK and the same mcastMaxDescNum.
    1.2. Call with out of range mcastMaxDescNum [32].
    Expected: NOT GT_OK.
    */

    GT_STATUS st          = GT_OK;

    GT_U8     dev;
    GT_U32    mcastNum    = 0;
    GT_U32    mcastNumGet = 0;

    /* prepare device iterator */
    PRV_TGF_NOT_APPLIC_DEV_RESET_MAC(&dev, UTF_NONE_FAMILY_E);

    /* 1. Go over all active devices. */
    while (GT_OK == prvUtfNextDeviceGet(&dev, GT_TRUE))
    {
        /* 1.1. Call with mcastMaxDescNum [4].
           Expected: GT_OK and the same mcastMaxDescNum.
        */
        mcastNum = 4;

        st = cpssDxChPortTxMcastPcktDescrLimitSet(dev, mcastNum);
        UTF_VERIFY_EQUAL2_PARAM_MAC(GT_OK, st, dev, mcastNum);

        /*get value*/

        st = cpssDxChPortTxMcastPcktDescrLimitGet(dev, &mcastNumGet);
        UTF_VERIFY_EQUAL2_PARAM_MAC(GT_OK, st, dev, mcastNum);

        UTF_VERIFY_EQUAL3_STRING_MAC(mcastNum, mcastNumGet,
          "got another mcastNum than was set: %d", dev, mcastNum, mcastNumGet);


        /* 1.2. Call with mcastMaxDescNum [32].
           Expected: NOT GT_OK.
        */
        if(0 == PRV_CPSS_DXCH_PP_HW_INFO_TXQ_REV_1_OR_ABOVE_MAC(dev))
        {
            mcastNum = BIT_5;
        }
        else
        {
            mcastNum = BIT_16;
        }

        st = cpssDxChPortTxMcastPcktDescrLimitSet(dev, mcastNum);
        UTF_VERIFY_NOT_EQUAL2_PARAM_MAC(GT_OK, st, dev, mcastNum);
    }

    mcastNum = 3;

    /* 2. For not-active devices and devices from non-applicable family */
    /* check that function returns GT_BAD_PARAM.                        */

    /* prepare device iterator */
    PRV_TGF_NOT_APPLIC_DEV_RESET_MAC(&dev, UTF_NONE_FAMILY_E);
    /* go over all non active devices */
    while(GT_OK == prvUtfNextDeviceGet(&dev, GT_FALSE))
    {
        /* mcastNum [3] */
        st = cpssDxChPortTxMcastPcktDescrLimitSet(dev, mcastNum);
        UTF_VERIFY_EQUAL1_PARAM_MAC(GT_NOT_APPLICABLE_DEVICE, st, dev);
    }

    /* 3. Call function with out of bound value for device id.*/

    dev = PRV_CPSS_MAX_PP_DEVICES_CNS;
    /* mcastNum [3] */

    st = cpssDxChPortTxMcastPcktDescrLimitSet(dev, mcastNum);
    UTF_VERIFY_EQUAL1_PARAM_MAC(GT_BAD_PARAM, st, dev);
}

/*----------------------------------------------------------------------------*/
/*
GT_STATUS cpssDxChPortTxWrrGlobalParamSet
(
    IN    GT_U8                     devNum,
    IN    CPSS_PORT_TX_WRR_MODE_ENT wrrMode,
    IN    CPSS_PORT_TX_WRR_MTU_ENT  wrrMtu
)
*/
UTF_TEST_CASE_MAC(cpssDxChPortTxWrrGlobalParamSet)
{
    /*
    ITERATE_DEVICES (DxCh)
    1.1. Call with wrrMode [CPSS_PORT_TX_WRR_BYTE_MODE_E, CPSS_PORT_TX_WRR_PACKET_MODE_E]
                and wrrMtu [CPSS_PORT_TX_WRR_MTU_2K_E, CPSS_PORT_TX_WRR_MTU_8K_E].
    Expected: GT_OK.
    1.2. Call with wrong enum values wrrMode  and wrrMtu [CPSS_PORT_TX_WRR_MTU_2K_E].
    Expected: GT_BAD_PARAM.
    1.3. Call with wrrMode [CPSS_PORT_TX_WRR_BYTE_MODE_E] and wrong enum values wrrMtu
    Expected: GT_BAD_PARAM.
    */

    GT_STATUS                 st      = GT_OK;

    GT_U8                     dev;
    CPSS_PORT_TX_WRR_MODE_ENT wrrMode = CPSS_PORT_TX_WRR_BYTE_MODE_E;
    CPSS_PORT_TX_WRR_MTU_ENT  wrrMtu  = CPSS_PORT_TX_WRR_MTU_256_E;

    /* prepare device iterator */
    PRV_TGF_NOT_APPLIC_DEV_RESET_MAC(&dev, UTF_NONE_FAMILY_E);
    /* 1. Go over all active devices. */
    while (GT_OK == prvUtfNextDeviceGet(&dev, GT_TRUE))
    {
        /* 1.1. Call with wrrMode [CPSS_PORT_TX_WRR_BYTE_MODE_E,
                                   CPSS_PORT_TX_WRR_PACKET_MODE_E]
                          wrrMtu  [CPSS_PORT_TX_WRR_MTU_2K_E,
                                   CPSS_PORT_TX_WRR_MTU_8K_E].
           Expected: GT_OK.
        */

        /* Call function with wrrMode [CPSS_PORT_TX_WRR_PACKET_MODE_E] */
        /*                    wrrMtu  [CPSS_PORT_TX_WRR_MTU_2K_E] */
        wrrMode = CPSS_PORT_TX_WRR_PACKET_MODE_E;
        wrrMtu = CPSS_PORT_TX_WRR_MTU_2K_E;

        st = cpssDxChPortTxWrrGlobalParamSet(dev, wrrMode, wrrMtu);
        UTF_VERIFY_EQUAL3_PARAM_MAC(GT_OK, st, dev, wrrMode, wrrMtu);

        /* Call function with wrrMode [CPSS_PORT_TX_WRR_BYTE_MODE_E] */
        /*                    wrrMtu  [CPSS_PORT_TX_WRR_MTU_8K_E] */
        wrrMode = CPSS_PORT_TX_WRR_BYTE_MODE_E;
        wrrMtu = CPSS_PORT_TX_WRR_MTU_8K_E;

        st = cpssDxChPortTxWrrGlobalParamSet(dev, wrrMode, wrrMtu);
        UTF_VERIFY_EQUAL3_PARAM_MAC(GT_OK, st, dev, wrrMode, wrrMtu);

        /* 1.2. Call with wrong enum values wrrMode and wrrMtu [CPSS_PORT_TX_WRR_MTU_2K_E].
           Expected: GT_BAD_PARAM.
        */
        wrrMtu = CPSS_PORT_TX_WRR_MTU_2K_E;

        UTF_ENUMS_CHECK_MAC(cpssDxChPortTxWrrGlobalParamSet
                            (dev, wrrMode, wrrMtu),
                            wrrMode);

        /* 1.3. Call with wrrMode [CPSS_PORT_TX_WRR_BYTE_MODE_E]
                          wrong enum values wrrMtu
           Expected: GT_BAD_PARAM.
        */
        wrrMode = CPSS_PORT_TX_WRR_BYTE_MODE_E;

        UTF_ENUMS_CHECK_MAC(cpssDxChPortTxWrrGlobalParamSet
                            (dev, wrrMode, wrrMtu),
                            wrrMtu);
    }

    wrrMode = CPSS_PORT_TX_WRR_PACKET_MODE_E;
    wrrMtu = CPSS_PORT_TX_WRR_MTU_2K_E;

    /* 2. For not-active devices and devices from non-applicable family */
    /* check that function returns GT_BAD_PARAM.                        */

    /* prepare device iterator */
    PRV_TGF_NOT_APPLIC_DEV_RESET_MAC(&dev, UTF_NONE_FAMILY_E);

    /* go over all non active devices */
    while(GT_OK == prvUtfNextDeviceGet(&dev, GT_FALSE))
    {
        /* wrrMode [CPSS_PORT_TX_WRR_PACKET_MODE_E], wrrMtu [CPSS_PORT_TX_WRR_MTU_2K_E] */
        st = cpssDxChPortTxWrrGlobalParamSet(dev, wrrMode, wrrMtu);
        UTF_VERIFY_EQUAL1_PARAM_MAC(GT_NOT_APPLICABLE_DEVICE, st, dev);
    }

    /* 3. Call function with out of bound value for device id.*/

    dev = PRV_CPSS_MAX_PP_DEVICES_CNS;
    /* wrrMode [CPSS_PORT_TX_WRR_PACKET_MODE_E], wrrMtu [CPSS_PORT_TX_WRR_MTU_2K_E] */

    st = cpssDxChPortTxWrrGlobalParamSet(dev, wrrMode, wrrMtu);
    UTF_VERIFY_EQUAL1_PARAM_MAC(GT_BAD_PARAM, st, dev);
}

/*----------------------------------------------------------------------------*/
/*
GT_STATUS cpssDxChPortTxShaperGlobalParamsSet
(
    IN    GT_U8                   devNum,
    IN    GT_U32                  xgPortsTokensRate,
    IN    GT_U32                  gigPortsTokensRate,
    IN    GT_U32                  gigPortsSlowRateRatio
)
*/
UTF_TEST_CASE_MAC(cpssDxChPortTxShaperGlobalParamsSet)
{
    /*
    ITERATE_DEVICES (DxCh)
    1.1. Call with xgPortsTokensRate [4], gigPortsTokensRate [8],
         gigPortsSlowRateRatio [16].
    Expected: GT_OK and the same values.
    1.2. Call with out of range xgPortsTokensRate [16],
         gigPortsTokensRate [4], and gigPortsSlowRateRatio [8].
    Expected: NOT GT_OK.
    1.3. Call with xgPortsTokensRate [1], out of range
         gigPortsTokensRate [16], gigPortsSlowRateRatio [4].
    Expected: NOT GT_OK.
    1.4. Call with xgPortsTokensRate [8], gigPortsTokensRate [1],
         out of range gigPortsSlowRateRatio [32].
    Expected: NOT GT_OK.
    */
    GT_U8                   dev;
    GT_STATUS               st          = GT_OK;
    GT_U32                  xgRate      = 0;
    GT_U32                  gigRate     = 0;
    GT_U32                  gigRatio    = 0;
    GT_U32                  xgRateGet   = 1;
    GT_U32                  gigRateGet  = 1;
    GT_U32                  gigRatioGet = 1;

    /* prepare device iterator */
    PRV_TGF_NOT_APPLIC_DEV_RESET_MAC(&dev, UTF_LION_E);

    /* 1. Go over all active devices. */
    while (GT_OK == prvUtfNextDeviceGet(&dev, GT_TRUE))
    {
        /* 1.1. Call with xgPortsTokensRate [4],
                          gigPortsTokensRate [8],
                          gigPortsSlowRateRatio [16].
          Expected: GT_OK and the same values.
        */
        xgRate   = 4;
        gigRate  = 8;
        gigRatio = 16;

        st = cpssDxChPortTxShaperGlobalParamsSet(dev, xgRate, gigRate, gigRatio);
        UTF_VERIFY_EQUAL4_PARAM_MAC(GT_OK, st, dev, xgRate, gigRate, gigRatio);

        st = cpssDxChPortTxShaperGlobalParamsGet(dev, &xgRateGet, &gigRateGet,
                                                 &gigRatioGet);
        UTF_VERIFY_EQUAL4_PARAM_MAC(GT_OK, st, dev, xgRate, gigRate, gigRatio);

        /* checking all values */
        UTF_VERIFY_EQUAL1_STRING_MAC(xgRateGet, xgRateGet,
                                     "got another xgRate than was set: %d", dev);
        UTF_VERIFY_EQUAL1_STRING_MAC(gigRate, gigRateGet,
                                     "got another gigRate than was set: %d", dev);
        UTF_VERIFY_EQUAL1_STRING_MAC(gigRatio, gigRatioGet,
                                     "got another gigRatio than was set: %d", dev);

        /* 1.2. Call with out of range xgPortsTokensRate [16],
                          gigPortsTokensRate [4],
                          gigPortsSlowRateRatio [8].
           Expected: NOT GT_OK.
        */
        xgRate   = 16;
        gigRate  = 4;
        gigRatio = 8;

        st = cpssDxChPortTxShaperGlobalParamsSet(dev, xgRate, gigRate, gigRatio);
        UTF_VERIFY_NOT_EQUAL4_PARAM_MAC(GT_OK, st, dev, xgRate, gigRate, gigRatio);

        /* 1.3. Call with xgPortsTokensRate [1],
                          out of range gigPortsTokensRate [16],
                          gigPortsSlowRateRatio [4].
           Expected: NOT GT_OK.
        */
        xgRate   = 1;
        gigRate  = 16;
        gigRatio = 4;

        st = cpssDxChPortTxShaperGlobalParamsSet(dev, xgRate, gigRate, gigRatio);
        UTF_VERIFY_NOT_EQUAL4_PARAM_MAC(GT_OK, st, dev, xgRate, gigRate, gigRatio);

        /* 1.4. Call with xgPortsTokensRate [8],
                          gigPortsTokensRate [1],
                          out of range gigPortsSlowRateRatio [32].
           Expected: NOT GT_OK.
        */
        xgRate   = 8;
        gigRate  = 1;
        gigRatio = 32;

        st = cpssDxChPortTxShaperGlobalParamsSet(dev, xgRate, gigRate, gigRatio);
        UTF_VERIFY_NOT_EQUAL4_PARAM_MAC(GT_OK, st, dev, xgRate, gigRate, gigRatio);
    }

    /* restore correct params */
    xgRate   = 4;
    gigRate  = 8;
    gigRatio = 16;

    /* 2. For not-active devices and devices from non-applicable family */
    /* check that function returns GT_BAD_PARAM.                        */

    /* prepare device iterator */
    PRV_TGF_NOT_APPLIC_DEV_RESET_MAC(&dev, UTF_LION_E);

    /* go over all non active devices */
    while(GT_OK == prvUtfNextDeviceGet(&dev, GT_FALSE))
    {
        st = cpssDxChPortTxShaperGlobalParamsSet(dev, xgRate, gigRate, gigRatio);
        UTF_VERIFY_EQUAL1_PARAM_MAC(GT_NOT_APPLICABLE_DEVICE, st, dev);
    }

    /* 3. Call function with out of bound value for device id.*/
    dev = PRV_CPSS_MAX_PP_DEVICES_CNS;

    st = cpssDxChPortTxShaperGlobalParamsSet(dev, xgRate, gigRate, gigRatio);
    UTF_VERIFY_EQUAL1_PARAM_MAC(GT_BAD_PARAM, st, dev);
}

/*----------------------------------------------------------------------------*/
/*
GT_STATUS cpssDxChPortTxQWrrProfileSet
(
    IN  GT_U8                                   devNum,
    IN  GT_U8                                   tcQueue,
    IN  GT_U8                                   wrrWeight,
    IN  CPSS_PORT_TX_SCHEDULER_PROFILE_SET_ENT  profileSet
)
*/
UTF_TEST_CASE_MAC(cpssDxChPortTxQWrrProfileSet)
{
/*
    ITERATE_DEVICES (DxChx)
    1.1. Call with tcQueue [0, 1, 2, 3], wrrWeight [2, 4, 8, 17]
                        and profileSet [CPSS_PORT_TX_SCHEDULER_PROFILE_1_E,
                                        CPSS_PORT_TX_SCHEDULER_PROFILE_2_E,
                                        CPSS_PORT_TX_SCHEDULER_PROFILE_3_E,
                                        CPSS_PORT_TX_SCHEDULER_PROFILE_4_E].
    Expected: GT_OK.
    1.2. Call cpssDxChPortTxQWrrProfileGet with the same params.
    Expected: GT_OK and the same wrrWeight.
    1.3. Call with tcQueue [4, 5, 6, 7], wrrWeight [65, 128, 255]
                        and profileSet [CPSS_PORT_TX_SCHEDULER_PROFILE_5_E,
                                        CPSS_PORT_TX_SCHEDULER_PROFILE_6_E,
                                        CPSS_PORT_TX_SCHEDULER_PROFILE_7_E,
                                        CPSS_PORT_TX_SCHEDULER_PROFILE_8_E].
    Expected: GT_OK for Cheetah2 and NON GT_OK for other devices.
    1.4. Call with tcQueue [5], wrrWeight [7] and wrong enum values profileSet.
    Expected: GT_BAD_PARAM.
    1.5. Call with out of range tcQueue [CPSS_TC_RANGE_CNS = 8], wrrWeight [65]
            and profileSet [CPSS_PORT_TX_SCHEDULER_PROFILE_1_E].
    Expected: GT_BAD_PARAM.
    1.6. Call with tcQueue [1], wrrWeight [0xFF] (can not see any constraints for it)
            and profileSet [CPSS_PORT_TX_SCHEDULER_PROFILE_1_E].
    Expected: GT_OK.
*/
    GT_STATUS   st     = GT_OK;

    GT_U8                                  dev;
    GT_U8                                  tcQueue   = 0;
    CPSS_PORT_TX_SCHEDULER_PROFILE_SET_ENT profile   = CPSS_PORT_TX_SCHEDULER_PROFILE_1_E;
    GT_U8                                  wrrWeight = 0;
    GT_U8                                  wrrWeightGet;

    CPSS_PP_FAMILY_TYPE_ENT                devFamily = CPSS_MAX_FAMILY;

    /* prepare device iterator */
    PRV_TGF_NOT_APPLIC_DEV_RESET_MAC(&dev, UTF_NONE_FAMILY_E);

    /* 1. Go over all active devices. */
    while (GT_OK == prvUtfNextDeviceGet(&dev, GT_TRUE))
    {
        /*
            1.1. Call with tcQueue [0, 1, 2, 3], wrrWeight [2, 4, 8, 17]
                                and profileSet [CPSS_PORT_TX_SCHEDULER_PROFILE_1_E,
                                                CPSS_PORT_TX_SCHEDULER_PROFILE_2_E,
                                                CPSS_PORT_TX_SCHEDULER_PROFILE_3_E,
                                                CPSS_PORT_TX_SCHEDULER_PROFILE_4_E].
            Expected: GT_OK.
        */

        /* Call function with tcQueue = 0, wrrWeight = 2   */
        /* profileSet = CPSS_PORT_TX_SCHEDULER_PROFILE_1_E */
        tcQueue = 0;
        wrrWeight = 2;
        profile = CPSS_PORT_TX_SCHEDULER_PROFILE_1_E;

        st = cpssDxChPortTxQWrrProfileSet(dev, tcQueue, wrrWeight, profile);
        UTF_VERIFY_EQUAL4_PARAM_MAC(GT_OK, st, dev, tcQueue, wrrWeight, profile);

        /*
            1.2. Call cpssDxChPortTxQWrrProfileGet with the same params.
            Expected: GT_OK and the same wrrWeight.
        */
        st = cpssDxChPortTxQWrrProfileGet(dev, tcQueue, profile, &wrrWeightGet);
        UTF_VERIFY_EQUAL3_PARAM_MAC(GT_OK, st, dev, tcQueue, profile);

        UTF_VERIFY_EQUAL3_STRING_MAC(wrrWeight, wrrWeightGet,
           "got another wrrWeight than was set: %d, %d, %d", dev, tcQueue, profile);


        /* Call function with tcQueue = 1, wrrWeight = 4   */
        /* profileSet = CPSS_PORT_TX_SCHEDULER_PROFILE_2_E */
        tcQueue = 1;
        wrrWeight = 4;
        profile = CPSS_PORT_TX_SCHEDULER_PROFILE_2_E;

        st = cpssDxChPortTxQWrrProfileSet(dev, tcQueue, wrrWeight, profile);
        UTF_VERIFY_EQUAL4_PARAM_MAC(GT_OK, st, dev, tcQueue, wrrWeight, profile);

        /*
            1.2. Call cpssDxChPortTxQWrrProfileGet with the same params.
            Expected: GT_OK and the same wrrWeight.
        */
        st = cpssDxChPortTxQWrrProfileGet(dev, tcQueue, profile, &wrrWeightGet);
        UTF_VERIFY_EQUAL3_PARAM_MAC(GT_OK, st, dev, tcQueue, profile);

        UTF_VERIFY_EQUAL3_STRING_MAC(wrrWeight, wrrWeightGet,
           "got another wrrWeight than was set: %d, %d, %d", dev, tcQueue, profile);


        /* Call function with tcQueue = 2, wrrWeight = 8   */
        /* profileSet = CPSS_PORT_TX_SCHEDULER_PROFILE_3_E */
        tcQueue = 2;
        wrrWeight = 8;
        profile = CPSS_PORT_TX_SCHEDULER_PROFILE_3_E;

        st = cpssDxChPortTxQWrrProfileSet(dev, tcQueue, wrrWeight, profile);
        UTF_VERIFY_EQUAL4_PARAM_MAC(GT_OK, st, dev, tcQueue, wrrWeight, profile);

        /*
            1.2. Call cpssDxChPortTxQWrrProfileGet with the same params.
            Expected: GT_OK and the same wrrWeight.
        */
        st = cpssDxChPortTxQWrrProfileGet(dev, tcQueue, profile, &wrrWeightGet);
        UTF_VERIFY_EQUAL3_PARAM_MAC(GT_OK, st, dev, tcQueue, profile);

        UTF_VERIFY_EQUAL3_STRING_MAC(wrrWeight, wrrWeightGet,
           "got another wrrWeight than was set: %d, %d, %d", dev, tcQueue, profile);


        /* Call function with tcQueue = 3, wrrWeight = 17   */
        /* profileSet = CPSS_PORT_TX_SCHEDULER_PROFILE_4_E */
        tcQueue = 3;
        wrrWeight = 17;
        profile = CPSS_PORT_TX_SCHEDULER_PROFILE_4_E;

        st = cpssDxChPortTxQWrrProfileSet(dev, tcQueue, wrrWeight, profile);
        UTF_VERIFY_EQUAL4_PARAM_MAC(GT_OK, st, dev, tcQueue, wrrWeight, profile);

        /*
            1.2. Call cpssDxChPortTxQWrrProfileGet with the same params.
            Expected: GT_OK and the same wrrWeight.
        */
        st = cpssDxChPortTxQWrrProfileGet(dev, tcQueue, profile, &wrrWeightGet);
        UTF_VERIFY_EQUAL3_PARAM_MAC(GT_OK, st, dev, tcQueue, profile);

        UTF_VERIFY_EQUAL3_STRING_MAC(wrrWeight, wrrWeightGet,
           "got another wrrWeight than was set: %d, %d, %d", dev, tcQueue, profile);

        /*
            1.3. Call with tcQueue [4, 5, 6, 7], wrrWeight [65, 128, 255]
                                and profileSet [CPSS_PORT_TX_SCHEDULER_PROFILE_5_E,
                                                CPSS_PORT_TX_SCHEDULER_PROFILE_6_E,
                                                CPSS_PORT_TX_SCHEDULER_PROFILE_7_E,
                                                CPSS_PORT_TX_SCHEDULER_PROFILE_8_E].
            Expected: GT_OK for Cheetah2 and NON GT_OK for other devices.
        */

        st = prvUtfDeviceFamilyGet(dev, &devFamily);
        UTF_VERIFY_EQUAL1_STRING_MAC(GT_OK, st, "prvUtfDeviceFamilyGet: %d", dev);

        /* Call function with tcQueue = 4, wrrWeight = 65  */
        /* profileSet = CPSS_PORT_TX_SCHEDULER_PROFILE_5_E */
        tcQueue = 4;
        wrrWeight = 65;
        profile = CPSS_PORT_TX_SCHEDULER_PROFILE_5_E;

        st = cpssDxChPortTxQWrrProfileSet(dev, tcQueue, wrrWeight, profile);
        if (CPSS_PP_FAMILY_CHEETAH_E  != devFamily)
        {
            UTF_VERIFY_EQUAL4_PARAM_MAC(GT_OK, st, dev, tcQueue, wrrWeight, profile);

            /*
                1.2. Call cpssDxChPortTxQWrrProfileGet with the same params.
                Expected: GT_OK and the same wrrWeight.
            */
            st = cpssDxChPortTxQWrrProfileGet(dev, tcQueue, profile, &wrrWeightGet);
            UTF_VERIFY_EQUAL3_PARAM_MAC(GT_OK, st, dev, tcQueue, profile);

            UTF_VERIFY_EQUAL3_STRING_MAC(wrrWeight, wrrWeightGet,
               "got another wrrWeight than was set: %d, %d, %d", dev, tcQueue, profile);
        }
        else
        {
            UTF_VERIFY_NOT_EQUAL4_PARAM_MAC(GT_OK, st, dev, tcQueue, wrrWeight, profile);
        }

        /* Call function with tcQueue = 5, wrrWeight = 128 */
        /* profileSet = CPSS_PORT_TX_SCHEDULER_PROFILE_6_E */
        tcQueue = 5;
        wrrWeight = 128;
        profile = CPSS_PORT_TX_SCHEDULER_PROFILE_6_E;

        st = cpssDxChPortTxQWrrProfileSet(dev, tcQueue, wrrWeight, profile);
        if (CPSS_PP_FAMILY_CHEETAH_E  != devFamily)
        {
            UTF_VERIFY_EQUAL4_PARAM_MAC(GT_OK, st, dev, tcQueue, wrrWeight, profile);

            /*
                1.2. Call cpssDxChPortTxQWrrProfileGet with the same params.
                Expected: GT_OK and the same wrrWeight.
            */
            st = cpssDxChPortTxQWrrProfileGet(dev, tcQueue, profile, &wrrWeightGet);
            UTF_VERIFY_EQUAL3_PARAM_MAC(GT_OK, st, dev, tcQueue, profile);

            UTF_VERIFY_EQUAL3_STRING_MAC(wrrWeight, wrrWeightGet,
               "got another wrrWeight than was set: %d, %d, %d", dev, tcQueue, profile);
        }
        else
        {
            UTF_VERIFY_NOT_EQUAL4_PARAM_MAC(GT_OK, st, dev, tcQueue, wrrWeight, profile);
        }

        /* Call function with tcQueue = 6, wrrWeight = 25  */
        /* profileSet = CPSS_PORT_TX_SCHEDULER_PROFILE_7_E */
        tcQueue = 6;
        wrrWeight = 25;
        profile = CPSS_PORT_TX_SCHEDULER_PROFILE_7_E;

        st = cpssDxChPortTxQWrrProfileSet(dev, tcQueue, wrrWeight, profile);
        if (CPSS_PP_FAMILY_CHEETAH_E  != devFamily)
        {
            UTF_VERIFY_EQUAL4_PARAM_MAC(GT_OK, st, dev, tcQueue, wrrWeight, profile);

            /*
                1.2. Call cpssDxChPortTxQWrrProfileGet with the same params.
                Expected: GT_OK and the same wrrWeight.
            */
            st = cpssDxChPortTxQWrrProfileGet(dev, tcQueue, profile, &wrrWeightGet);
            UTF_VERIFY_EQUAL3_PARAM_MAC(GT_OK, st, dev, tcQueue, profile);

            UTF_VERIFY_EQUAL3_STRING_MAC(wrrWeight, wrrWeightGet,
               "got another wrrWeight than was set: %d, %d, %d", dev, tcQueue, profile);
        }
        else
        {
            UTF_VERIFY_NOT_EQUAL4_PARAM_MAC(GT_OK, st, dev, tcQueue, wrrWeight, profile);
        }

        /* Call function with tcQueue = 7, wrrWeight = 55  */
        /* profileSet = CPSS_PORT_TX_SCHEDULER_PROFILE_8_E */
        tcQueue = 7;
        wrrWeight = 55;
        profile = CPSS_PORT_TX_SCHEDULER_PROFILE_8_E;

        st = cpssDxChPortTxQWrrProfileSet(dev, tcQueue, wrrWeight, profile);
        if (CPSS_PP_FAMILY_CHEETAH_E  != devFamily)
        {
            UTF_VERIFY_EQUAL4_PARAM_MAC(GT_OK, st, dev, tcQueue, wrrWeight, profile);

            /*
                1.2. Call cpssDxChPortTxQWrrProfileGet with the same params.
                Expected: GT_OK and the same wrrWeight.
            */
            st = cpssDxChPortTxQWrrProfileGet(dev, tcQueue, profile, &wrrWeightGet);
            UTF_VERIFY_EQUAL3_PARAM_MAC(GT_OK, st, dev, tcQueue, profile);

            UTF_VERIFY_EQUAL3_STRING_MAC(wrrWeight, wrrWeightGet,
               "got another wrrWeight than was set: %d, %d, %d", dev, tcQueue, profile);
        }
        else
        {
            UTF_VERIFY_NOT_EQUAL4_PARAM_MAC(GT_OK, st, dev, tcQueue, wrrWeight, profile);
        }

        /*
            1.4. Call with tcQueue [5], wrrWeight [7] and wrong enum values profileSet.
            Expected: GT_BAD_PARAM.
        */
        tcQueue = 5;
        wrrWeight = 7;

        UTF_ENUMS_CHECK_MAC(cpssDxChPortTxQWrrProfileSet
                            (dev, tcQueue, wrrWeight, profile),
                            profile);

        /*
            1.5. Call with out of range tcQueue [CPSS_TC_RANGE_CNS = 8], wrrWeight [65]
                    and profileSet [CPSS_PORT_TX_SCHEDULER_PROFILE_1_E].
            Expected: GT_BAD_PARAM.
        */
        tcQueue = CPSS_TC_RANGE_CNS;
        wrrWeight = 65;
        profile = CPSS_PORT_TX_SCHEDULER_PROFILE_1_E;

        st = cpssDxChPortTxQWrrProfileSet(dev, tcQueue, wrrWeight, profile);
        UTF_VERIFY_EQUAL4_PARAM_MAC(GT_BAD_PARAM, st, dev, tcQueue, wrrWeight, profile);

        /*
            1.6. Call with tcQueue [1], wrrWeight [0xFF] (can not see any constraints for it)
                    and profileSet [CPSS_PORT_TX_SCHEDULER_PROFILE_1_E].
            Expected: GT_OK.
        */
        tcQueue = 1;
        wrrWeight = 0xFF;
        profile = CPSS_PORT_TX_SCHEDULER_PROFILE_1_E;

        st = cpssDxChPortTxQWrrProfileSet(dev, tcQueue, wrrWeight, profile);
        UTF_VERIFY_EQUAL4_PARAM_MAC(GT_OK, st, dev, tcQueue, wrrWeight, profile);
    }

    tcQueue = 1;
    wrrWeight = 15;
    profile = CPSS_PORT_TX_SCHEDULER_PROFILE_3_E;

    /* 2. For not-active devices and devices from non-applicable family */
    /* check that function returns GT_BAD_PARAM.                        */

    /* prepare device iterator */
    PRV_TGF_NOT_APPLIC_DEV_RESET_MAC(&dev, UTF_NONE_FAMILY_E);

    /* go over all non active devices */
    while(GT_OK == prvUtfNextDeviceGet(&dev, GT_FALSE))
    {
        st = cpssDxChPortTxQWrrProfileSet(dev, tcQueue, wrrWeight, profile);
        UTF_VERIFY_EQUAL1_PARAM_MAC(GT_NOT_APPLICABLE_DEVICE, st, dev);
    }

    /* 3. Call function with out of bound value for device id.*/
    dev = PRV_CPSS_MAX_PP_DEVICES_CNS;

    st = cpssDxChPortTxQWrrProfileSet(dev, tcQueue, wrrWeight, profile);
    UTF_VERIFY_EQUAL1_PARAM_MAC(GT_BAD_PARAM, st, dev);
}

/*----------------------------------------------------------------------------*/
/*
GT_STATUS cpssDxChPortTxQWrrProfileGet
(
    IN  GT_U8                                   devNum,
    IN  GT_U8                                   tcQueue,
    IN  CPSS_PORT_TX_SCHEDULER_PROFILE_SET_ENT  profileSet,
    OUT  GT_U8                                  *wrrWeightPtr
)
*/
UTF_TEST_CASE_MAC(cpssDxChPortTxQWrrProfileGet)
{
/*
    ITERATE_DEVICES (DxChx)
    1.1. Call with tcQueue [0, 1, 2, 3],
                        and profileSet [CPSS_PORT_TX_SCHEDULER_PROFILE_1_E,
                                        CPSS_PORT_TX_SCHEDULER_PROFILE_2_E,
                                        CPSS_PORT_TX_SCHEDULER_PROFILE_3_E,
                                        CPSS_PORT_TX_SCHEDULER_PROFILE_4_E].
    Expected: GT_OK.
    1.2. Call with tcQueue [4, 5, 6, 7],
                        and profileSet [CPSS_PORT_TX_SCHEDULER_PROFILE_5_E,
                                        CPSS_PORT_TX_SCHEDULER_PROFILE_6_E,
                                        CPSS_PORT_TX_SCHEDULER_PROFILE_7_E,
                                        CPSS_PORT_TX_SCHEDULER_PROFILE_8_E].
    Expected: GT_OK for Cheetah2 and NON GT_OK for other devices.
    1.3. Call with tcQueue [5], wrong enum values profileSet.
    Expected: GT_BAD_PARAM.
    1.4. Call with out of range tcQueue [CPSS_TC_RANGE_CNS = 8],
            and profileSet [CPSS_PORT_TX_SCHEDULER_PROFILE_1_E].
    Expected: GT_BAD_PARAM.
*/
    GT_STATUS   st     = GT_OK;

    GT_U8                                  dev;
    GT_U8                                  tcQueue   = 0;
    CPSS_PORT_TX_SCHEDULER_PROFILE_SET_ENT profile   = CPSS_PORT_TX_SCHEDULER_PROFILE_1_E;
    GT_U8                                  wrrWeight;

    CPSS_PP_FAMILY_TYPE_ENT                devFamily = CPSS_MAX_FAMILY;

    /* prepare device iterator */
    PRV_TGF_NOT_APPLIC_DEV_RESET_MAC(&dev, UTF_NONE_FAMILY_E);

    /* 1. Go over all active devices. */
    while (GT_OK == prvUtfNextDeviceGet(&dev, GT_TRUE))
    {
        /*
            1.1. Call with tcQueue [0, 1, 2, 3],
                                and profileSet [CPSS_PORT_TX_SCHEDULER_PROFILE_1_E,
                                                CPSS_PORT_TX_SCHEDULER_PROFILE_2_E,
                                                CPSS_PORT_TX_SCHEDULER_PROFILE_3_E,
                                                CPSS_PORT_TX_SCHEDULER_PROFILE_4_E].
            Expected: GT_OK.
        */

        /* Call function with tcQueue = 0, profileSet = CPSS_PORT_TX_SCHEDULER_PROFILE_1_E */
        tcQueue = 0;
        profile = CPSS_PORT_TX_SCHEDULER_PROFILE_1_E;

        st = cpssDxChPortTxQWrrProfileGet(dev, tcQueue, profile, &wrrWeight);
        UTF_VERIFY_EQUAL3_PARAM_MAC(GT_OK, st, dev, tcQueue, profile);

        /* Call function with tcQueue = 1, profileSet = CPSS_PORT_TX_SCHEDULER_PROFILE_2_E */
        tcQueue = 1;
        profile = CPSS_PORT_TX_SCHEDULER_PROFILE_2_E;

        st = cpssDxChPortTxQWrrProfileGet(dev, tcQueue, profile, &wrrWeight);
        UTF_VERIFY_EQUAL3_PARAM_MAC(GT_OK, st, dev, tcQueue, profile);

        /* Call function with tcQueue = 2, profileSet = CPSS_PORT_TX_SCHEDULER_PROFILE_3_E */
        tcQueue = 2;
        profile = CPSS_PORT_TX_SCHEDULER_PROFILE_3_E;

        st = cpssDxChPortTxQWrrProfileGet(dev, tcQueue, profile, &wrrWeight);
        UTF_VERIFY_EQUAL3_PARAM_MAC(GT_OK, st, dev, tcQueue, profile);

        /* Call function with tcQueue = 3, profileSet = CPSS_PORT_TX_SCHEDULER_PROFILE_4_E */
        tcQueue = 3;
        profile = CPSS_PORT_TX_SCHEDULER_PROFILE_4_E;

        st = cpssDxChPortTxQWrrProfileGet(dev, tcQueue, profile, &wrrWeight);
        UTF_VERIFY_EQUAL3_PARAM_MAC(GT_OK, st, dev, tcQueue, profile);

        /*
            1.2. Call with tcQueue [4, 5, 6, 7],
                                and profileSet [CPSS_PORT_TX_SCHEDULER_PROFILE_5_E,
                                                CPSS_PORT_TX_SCHEDULER_PROFILE_6_E,
                                                CPSS_PORT_TX_SCHEDULER_PROFILE_7_E,
                                                CPSS_PORT_TX_SCHEDULER_PROFILE_8_E].
            Expected: GT_OK for Cheetah2 and NON GT_OK for other devices.
        */
        st = prvUtfDeviceFamilyGet(dev, &devFamily);
        UTF_VERIFY_EQUAL1_STRING_MAC(GT_OK, st, "prvUtfDeviceFamilyGet: %d", dev);

        /* Call function with tcQueue = 4, profileSet = CPSS_PORT_TX_SCHEDULER_PROFILE_5_E */
        tcQueue = 4;
        profile = CPSS_PORT_TX_SCHEDULER_PROFILE_5_E;

        st = cpssDxChPortTxQWrrProfileGet(dev, tcQueue, profile, &wrrWeight);
        if (CPSS_PP_FAMILY_CHEETAH_E  != devFamily)
        {
            UTF_VERIFY_EQUAL3_PARAM_MAC(GT_OK, st, dev, tcQueue, profile);
        }
        else
        {
            UTF_VERIFY_NOT_EQUAL3_PARAM_MAC(GT_OK, st, dev, tcQueue, profile);
        }

        /* Call function with tcQueue = 5, profileSet = CPSS_PORT_TX_SCHEDULER_PROFILE_6_E */
        tcQueue = 5;
        profile = CPSS_PORT_TX_SCHEDULER_PROFILE_6_E;

        st = cpssDxChPortTxQWrrProfileGet(dev, tcQueue, profile, &wrrWeight);

        if (CPSS_PP_FAMILY_CHEETAH_E  != devFamily)
        {
            UTF_VERIFY_EQUAL3_PARAM_MAC(GT_OK, st, dev, tcQueue, profile);
        }
        else
        {
            UTF_VERIFY_NOT_EQUAL3_PARAM_MAC(GT_OK, st, dev, tcQueue, profile);
        }

        /* Call function with tcQueue = 6, profileSet = CPSS_PORT_TX_SCHEDULER_PROFILE_7_E */
        tcQueue = 6;
        profile = CPSS_PORT_TX_SCHEDULER_PROFILE_7_E;

        st = cpssDxChPortTxQWrrProfileGet(dev, tcQueue, profile, &wrrWeight);

        if (CPSS_PP_FAMILY_CHEETAH_E  != devFamily)
        {
            UTF_VERIFY_EQUAL3_PARAM_MAC(GT_OK, st, dev, tcQueue, profile);
        }
        else
        {
            UTF_VERIFY_NOT_EQUAL3_PARAM_MAC(GT_OK, st, dev, tcQueue, profile);
        }

        /* Call function with tcQueue = 7, profileSet = CPSS_PORT_TX_SCHEDULER_PROFILE_8_E */
        tcQueue = 7;
        profile = CPSS_PORT_TX_SCHEDULER_PROFILE_8_E;

        st = cpssDxChPortTxQWrrProfileGet(dev, tcQueue, profile, &wrrWeight);

        if (CPSS_PP_FAMILY_CHEETAH_E  != devFamily)
        {
            UTF_VERIFY_EQUAL3_PARAM_MAC(GT_OK, st, dev, tcQueue, profile);
        }
        else
        {
            UTF_VERIFY_NOT_EQUAL3_PARAM_MAC(GT_OK, st, dev, tcQueue, profile);
        }

        /*
            1.3. Call with tcQueue [5], wrrWeight [7] and wrong enum values profileSet.
            Expected: GT_BAD_PARAM.
        */
        tcQueue = 5;

        UTF_ENUMS_CHECK_MAC(cpssDxChPortTxQWrrProfileGet
                            (dev, tcQueue, profile, &wrrWeight),
                            profile);

        /*
            1.4. Call with out of range tcQueue [CPSS_TC_RANGE_CNS = 8],
                    and profileSet [CPSS_PORT_TX_SCHEDULER_PROFILE_1_E].
            Expected: GT_BAD_PARAM.
        */
        tcQueue = CPSS_TC_RANGE_CNS;
        profile = CPSS_PORT_TX_SCHEDULER_PROFILE_1_E;

        st = cpssDxChPortTxQWrrProfileGet(dev, tcQueue, profile, &wrrWeight);
        UTF_VERIFY_EQUAL3_PARAM_MAC(GT_BAD_PARAM, st, dev, tcQueue, profile);
    }

    tcQueue = 1;
    profile = CPSS_PORT_TX_SCHEDULER_PROFILE_3_E;

    /* 2. For not-active devices and devices from non-applicable family */
    /* check that function returns GT_BAD_PARAM.                        */

    /* prepare device iterator */
    PRV_TGF_NOT_APPLIC_DEV_RESET_MAC(&dev, UTF_NONE_FAMILY_E);

    /* go over all non active devices */
    while(GT_OK == prvUtfNextDeviceGet(&dev, GT_FALSE))
    {
        st = cpssDxChPortTxQWrrProfileGet(dev, tcQueue, profile, &wrrWeight);
        UTF_VERIFY_EQUAL1_PARAM_MAC(GT_NOT_APPLICABLE_DEVICE, st, dev);
    }

    /* 3. Call function with out of bound value for device id.*/
    dev = PRV_CPSS_MAX_PP_DEVICES_CNS;

    st = cpssDxChPortTxQWrrProfileGet(dev, tcQueue, profile, &wrrWeight);
    UTF_VERIFY_EQUAL1_PARAM_MAC(GT_BAD_PARAM, st, dev);
}

/*----------------------------------------------------------------------------*/
/*
GT_STATUS cpssDxChPortTxQArbGroupSet
(
    IN  GT_U8                                   devNum,
    IN  GT_U8                                   tcQueue,
    IN  CPSS_PORT_TX_Q_ARB_GROUP_ENT            arbGroup,
    IN  CPSS_PORT_TX_SCHEDULER_PROFILE_SET_ENT  profileSet
)
*/
UTF_TEST_CASE_MAC(cpssDxChPortTxQArbGroupSet)
{
/*
    ITERATE_DEVICES (DxChx)
    1.1. Call with tcQueue [0, 1, 2, 3], arbGroup [CPSS_PORT_TX_WRR_ARB_GROUP_0_E,
                                                   CPSS_PORT_TX_WRR_ARB_GROUP_1_E,
                                                   CPSS_PORT_TX_SP_ARB_GROUP_E]
                 and profileSet [CPSS_PORT_TX_SCHEDULER_PROFILE_1_E,
                                 CPSS_PORT_TX_SCHEDULER_PROFILE_2_E,
                                 CPSS_PORT_TX_SCHEDULER_PROFILE_3_E,
                                 CPSS_PORT_TX_SCHEDULER_PROFILE_4_E].
    Expected: GT_OK.
    1.2. Call cpssDxChPortTxQArbGroupGet with the same params.
    Expected: GT_OK and the same value.
    1.3. Call with tcQueue [4, 5, 6, 7], arbGroup [CPSS_PORT_TX_WRR_ARB_GROUP_0_E,
                                                   CPSS_PORT_TX_WRR_ARB_GROUP_1_E,
                                                   CPSS_PORT_TX_SP_ARB_GROUP_E]
                                and profileSet [CPSS_PORT_TX_SCHEDULER_PROFILE_5_E,
                                                CPSS_PORT_TX_SCHEDULER_PROFILE_6_E,
                                                CPSS_PORT_TX_SCHEDULER_PROFILE_7_E,
                                                CPSS_PORT_TX_SCHEDULER_PROFILE_8_E]
    Expected: GT_OK for Cheetah and NON GT_OK for other devices.
    1.4. Call with out of range tcQueue [CPSS_TC_RANGE_CNS = 8],
                                arbGroup [CPSS_PORT_TX_WRR_ARB_GROUP_0_E]
                                and profileSet [CPSS_PORT_TX_SCHEDULER_PROFILE_1_E].
    Expected: GT_BAD_PARAM.
    1.5. Call with tcQueue [5], wrong enum values arbGroup
            and profileSet [CPSS_PORT_TX_SCHEDULER_PROFILE_2_E].
    Expected: GT_BAD_PARAM.
    1.6. Call with tcQueue [7], arbGroup [CPSS_PORT_TX_WRR_ARB_GROUP_1_E]
            and wrong enum values profileSet.
    Expected: GT_BAD_PARAM.
*/
    GT_STATUS   st     = GT_OK;

    GT_U8                                  dev;
    GT_U8                                  tcQueue   = 0;
    CPSS_PORT_TX_SCHEDULER_PROFILE_SET_ENT profile   = CPSS_PORT_TX_SCHEDULER_PROFILE_1_E;
    CPSS_PORT_TX_Q_ARB_GROUP_ENT           group     = CPSS_PORT_TX_WRR_ARB_GROUP_0_E;
    CPSS_PORT_TX_Q_ARB_GROUP_ENT           groupGet     = CPSS_PORT_TX_WRR_ARB_GROUP_0_E;

    CPSS_PP_FAMILY_TYPE_ENT                devFamily = CPSS_MAX_FAMILY;

    /* prepare device iterator */
    PRV_TGF_NOT_APPLIC_DEV_RESET_MAC(&dev, UTF_NONE_FAMILY_E);

    /* 1. Go over all active devices. */
    while (GT_OK == prvUtfNextDeviceGet(&dev, GT_TRUE))
    {
        /*
           1.1. Call with tcQueue [0, 1, 2, 3],
                          arbGroup [CPSS_PORT_TX_WRR_ARB_GROUP_0_E /
                                    CPSS_PORT_TX_WRR_ARB_GROUP_1_E /
                                    CPSS_PORT_TX_SP_ARB_GROUP_E],
                          profileSet [CPSS_PORT_TX_SCHEDULER_PROFILE_1_E /
                                      CPSS_PORT_TX_SCHEDULER_PROFILE_2_E /
                                      CPSS_PORT_TX_SCHEDULER_PROFILE_3_E /
                                      CPSS_PORT_TX_SCHEDULER_PROFILE_4_E].
           Expected: GT_OK.
        */

        /* Call function with tcQueue = 0, arbGroup = CPSS_PORT_TX_WRR_ARB_GROUP_0_E */
        /* profileSet = CPSS_PORT_TX_SCHEDULER_PROFILE_1_E */
        tcQueue = 0;
        group = CPSS_PORT_TX_WRR_ARB_GROUP_0_E;
        profile = CPSS_PORT_TX_SCHEDULER_PROFILE_1_E;

        st = cpssDxChPortTxQArbGroupSet(dev, tcQueue, group, profile);
        UTF_VERIFY_EQUAL4_PARAM_MAC(GT_OK, st, dev, tcQueue, group, profile);

        /*
            1.2. Call cpssDxChPortTxQArbGroupGet with the same params.
            Expected: GT_OK and the same value.
        */
        st = cpssDxChPortTxQArbGroupGet(dev, tcQueue, profile, &groupGet);
        UTF_VERIFY_EQUAL3_PARAM_MAC(GT_OK, st, dev, tcQueue, profile);

        UTF_VERIFY_EQUAL3_STRING_MAC(group, groupGet,
           "got another group than was set: %d, %d, %d", dev, tcQueue, profile);


        /* Call function with tcQueue = 1, arbGroup = CPSS_PORT_TX_WRR_ARB_GROUP_1_E */
        /* profileSet = CPSS_PORT_TX_SCHEDULER_PROFILE_2_E */
        tcQueue = 1;
        group = CPSS_PORT_TX_WRR_ARB_GROUP_1_E;
        profile = CPSS_PORT_TX_SCHEDULER_PROFILE_2_E;

        st = cpssDxChPortTxQArbGroupSet(dev, tcQueue, group, profile);
        UTF_VERIFY_EQUAL4_PARAM_MAC(GT_OK, st, dev, tcQueue, group, profile);

        /*
            1.2. Call cpssDxChPortTxQArbGroupGet with the same params.
            Expected: GT_OK and the same value.
        */
        st = cpssDxChPortTxQArbGroupGet(dev, tcQueue, profile, &groupGet);
        UTF_VERIFY_EQUAL3_PARAM_MAC(GT_OK, st, dev, tcQueue, profile);

        UTF_VERIFY_EQUAL3_STRING_MAC(group, groupGet,
           "got another group than was set: %d, %d, %d", dev, tcQueue, profile);


        /* Call function with tcQueue = 2, arbGroup = CPSS_PORT_TX_SP_ARB_GROUP_E */
        /* profileSet = CPSS_PORT_TX_SCHEDULER_PROFILE_3_E */
        tcQueue = 2;
        group = CPSS_PORT_TX_SP_ARB_GROUP_E;
        profile = CPSS_PORT_TX_SCHEDULER_PROFILE_3_E;

        st = cpssDxChPortTxQArbGroupSet(dev, tcQueue, group, profile);
        UTF_VERIFY_EQUAL4_PARAM_MAC(GT_OK, st, dev, tcQueue, group, profile);

        /*
            1.2. Call cpssDxChPortTxQArbGroupGet with the same params.
            Expected: GT_OK and the same value.
        */
        st = cpssDxChPortTxQArbGroupGet(dev, tcQueue, profile, &groupGet);
        UTF_VERIFY_EQUAL3_PARAM_MAC(GT_OK, st, dev, tcQueue, profile);

        UTF_VERIFY_EQUAL3_STRING_MAC(group, groupGet,
           "got another group than was set: %d, %d, %d", dev, tcQueue, profile);


        /* Call function with tcQueue = 3, group = CPSS_PORT_TX_WRR_ARB_GROUP_0_E */
        /* profileSet = CPSS_PORT_TX_SCHEDULER_PROFILE_4_E */
        tcQueue = 3;
        group = CPSS_PORT_TX_WRR_ARB_GROUP_0_E;
        profile = CPSS_PORT_TX_SCHEDULER_PROFILE_4_E;

        st = cpssDxChPortTxQArbGroupSet(dev, tcQueue, group, profile);
        UTF_VERIFY_EQUAL4_PARAM_MAC(GT_OK, st, dev, tcQueue, group, profile);

        /*
            1.2. Call cpssDxChPortTxQArbGroupGet with the same params.
            Expected: GT_OK and the same value.
        */
        st = cpssDxChPortTxQArbGroupGet(dev, tcQueue, profile, &groupGet);
        UTF_VERIFY_EQUAL3_PARAM_MAC(GT_OK, st, dev, tcQueue, profile);

        UTF_VERIFY_EQUAL3_STRING_MAC(group, groupGet,
           "got another group than was set: %d, %d, %d", dev, tcQueue, profile);

        /*
           1.3. Call with tcQueue [4, 5, 6, 7],
                          arbGroup [CPSS_PORT_TX_WRR_ARB_GROUP_0_E /
                                    CPSS_PORT_TX_WRR_ARB_GROUP_1_E /
                                    CPSS_PORT_TX_SP_ARB_GROUP_E],
                          and profileSet [CPSS_PORT_TX_SCHEDULER_PROFILE_5_E /
                                          CPSS_PORT_TX_SCHEDULER_PROFILE_6_E /
                                          CPSS_PORT_TX_SCHEDULER_PROFILE_7_E /
                                          CPSS_PORT_TX_SCHEDULER_PROFILE_8_E]
           Expected: GT_OK for Cheetah and NON GT_OK for other devices.
        */

        st = prvUtfDeviceFamilyGet(dev, &devFamily);
        UTF_VERIFY_EQUAL1_STRING_MAC(GT_OK, st, "prvUtfDeviceFamilyGet: %d", dev);

        /* Call function with tcQueue = 4, group = CPSS_PORT_TX_WRR_ARB_GROUP_0_E */
        /* profileSet = CPSS_PORT_TX_SCHEDULER_PROFILE_5_E */
        tcQueue = 4;
        group = CPSS_PORT_TX_WRR_ARB_GROUP_0_E;
        profile = CPSS_PORT_TX_SCHEDULER_PROFILE_5_E;

        st = cpssDxChPortTxQArbGroupSet(dev, tcQueue, group, profile);
        if (CPSS_PP_FAMILY_CHEETAH_E  != devFamily)
        {
            UTF_VERIFY_EQUAL4_PARAM_MAC(GT_OK, st, dev, tcQueue, group, profile);

            /*
                1.2. Call cpssDxChPortTxQArbGroupGet with the same params.
                Expected: GT_OK and the same value.
            */
            st = cpssDxChPortTxQArbGroupGet(dev, tcQueue, profile, &groupGet);
            UTF_VERIFY_EQUAL3_PARAM_MAC(GT_OK, st, dev, tcQueue, profile);

            UTF_VERIFY_EQUAL3_STRING_MAC(group, groupGet,
               "got another group than was set: %d, %d, %d", dev, tcQueue, profile);
        }
        else
        {
            UTF_VERIFY_NOT_EQUAL4_PARAM_MAC(GT_OK, st, dev, tcQueue, group, profile);
        }

        /* Call function with tcQueue = 5, group = CPSS_PORT_TX_WRR_ARB_GROUP_1_E */
        /* profileSet = CPSS_PORT_TX_SCHEDULER_PROFILE_6_E */
        tcQueue = 5;
        group = CPSS_PORT_TX_WRR_ARB_GROUP_1_E;
        profile = CPSS_PORT_TX_SCHEDULER_PROFILE_6_E;

        st = cpssDxChPortTxQArbGroupSet(dev, tcQueue, group, profile);
        if (CPSS_PP_FAMILY_CHEETAH_E  != devFamily)
        {
            UTF_VERIFY_EQUAL4_PARAM_MAC(GT_OK, st, dev, tcQueue, group, profile);

            /*
                1.2. Call cpssDxChPortTxQArbGroupGet with the same params.
                Expected: GT_OK and the same value.
            */
            st = cpssDxChPortTxQArbGroupGet(dev, tcQueue, profile, &groupGet);
            UTF_VERIFY_EQUAL3_PARAM_MAC(GT_OK, st, dev, tcQueue, profile);

            UTF_VERIFY_EQUAL3_STRING_MAC(group, groupGet,
               "got another group than was set: %d, %d, %d", dev, tcQueue, profile);
        }
        else
        {
            UTF_VERIFY_NOT_EQUAL4_PARAM_MAC(GT_OK, st, dev, tcQueue, group, profile);
        }

        /* Call function with tcQueue = 6, group = CPSS_PORT_TX_SP_ARB_GROUP_E */
        /* profileSet = CPSS_PORT_TX_SCHEDULER_PROFILE_7_E */
        tcQueue = 6;
        group = CPSS_PORT_TX_SP_ARB_GROUP_E;
        profile = CPSS_PORT_TX_SCHEDULER_PROFILE_7_E;

        st = cpssDxChPortTxQArbGroupSet(dev, tcQueue, group, profile);
        if (CPSS_PP_FAMILY_CHEETAH_E  != devFamily)
        {
            UTF_VERIFY_EQUAL4_PARAM_MAC(GT_OK, st, dev, tcQueue, group, profile);

            /*
                1.2. Call cpssDxChPortTxQArbGroupGet with the same params.
                Expected: GT_OK and the same value.
            */
            st = cpssDxChPortTxQArbGroupGet(dev, tcQueue, profile, &groupGet);
            UTF_VERIFY_EQUAL3_PARAM_MAC(GT_OK, st, dev, tcQueue, profile);

            UTF_VERIFY_EQUAL3_STRING_MAC(group, groupGet,
               "got another group than was set: %d, %d, %d", dev, tcQueue, profile);
        }
        else
        {
            UTF_VERIFY_NOT_EQUAL4_PARAM_MAC(GT_OK, st, dev, tcQueue, group, profile);
        }

        /* Call function with tcQueue = 7, group = CPSS_PORT_TX_WRR_ARB_GROUP_0_E */
        /* profileSet = CPSS_PORT_TX_SCHEDULER_PROFILE_8_E */
        tcQueue = 7;
        group = CPSS_PORT_TX_WRR_ARB_GROUP_0_E;
        profile = CPSS_PORT_TX_SCHEDULER_PROFILE_8_E;

        st = cpssDxChPortTxQArbGroupSet(dev, tcQueue, group, profile);
        if (CPSS_PP_FAMILY_CHEETAH_E != devFamily)
        {
            UTF_VERIFY_EQUAL4_PARAM_MAC(GT_OK, st, dev, tcQueue, group, profile);

            /*
                1.2. Call cpssDxChPortTxQArbGroupGet with the same params.
                Expected: GT_OK and the same value.
            */
            st = cpssDxChPortTxQArbGroupGet(dev, tcQueue, profile, &groupGet);
            UTF_VERIFY_EQUAL3_PARAM_MAC(GT_OK, st, dev, tcQueue, profile);

            UTF_VERIFY_EQUAL3_STRING_MAC(group, groupGet,
               "got another group than was set: %d, %d, %d", dev, tcQueue, profile);
        }
        else
        {
            UTF_VERIFY_NOT_EQUAL4_PARAM_MAC(GT_OK, st, dev, tcQueue, group, profile);
        }

        /*
           1.4. Call with out of range tcQueue [CPSS_TC_RANGE_CNS = 8],
                           arbGroup [CPSS_PORT_TX_WRR_ARB_GROUP_0_E],
                           profileSet [CPSS_PORT_TX_SCHEDULER_PROFILE_1_E].
            Expected: GT_BAD_PARAM.
        */
        tcQueue = CPSS_TC_RANGE_CNS;
        group = CPSS_PORT_TX_WRR_ARB_GROUP_0_E;
        profile = CPSS_PORT_TX_SCHEDULER_PROFILE_1_E;

        st = cpssDxChPortTxQArbGroupSet(dev, tcQueue, group, profile);
        UTF_VERIFY_EQUAL4_PARAM_MAC(GT_BAD_PARAM, st, dev, tcQueue, group, profile);

        /*
           1.5. Call with tcQueue [5],
                          wrong enum values arbGroup ,
                          profileSet [CPSS_PORT_TX_SCHEDULER_PROFILE_2_E].
           Expected: GT_BAD_PARAM.
        */
        tcQueue = 5;
        profile = CPSS_PORT_TX_SCHEDULER_PROFILE_2_E;

        UTF_ENUMS_CHECK_MAC(cpssDxChPortTxQArbGroupSet
                            (dev, tcQueue, group, profile),
                            group);

        /*
           1.6. Call with tcQueue [7], arbGroup [CPSS_PORT_TX_WRR_ARB_GROUP_1_E],
                               wrong enum values profileSet.
           Expected: GT_BAD_PARAM.
        */
        tcQueue = 7;
        group = CPSS_PORT_TX_WRR_ARB_GROUP_1_E;

        UTF_ENUMS_CHECK_MAC(cpssDxChPortTxQArbGroupSet
                            (dev, tcQueue, group, profile),
                            profile);
    }

    tcQueue = 7;
    group = CPSS_PORT_TX_WRR_ARB_GROUP_1_E;
    profile = CPSS_PORT_TX_SCHEDULER_PROFILE_2_E;

    /* 2. For not-active devices and devices from non-applicable family */
    /* check that function returns GT_BAD_PARAM.                        */
    /* prepare device iterator */
    PRV_TGF_NOT_APPLIC_DEV_RESET_MAC(&dev, UTF_NONE_FAMILY_E);

    /* go over all non active devices */
    while(GT_OK == prvUtfNextDeviceGet(&dev, GT_FALSE))
    {
        st = cpssDxChPortTxQArbGroupSet(dev, tcQueue, group, profile);
        UTF_VERIFY_EQUAL1_PARAM_MAC(GT_NOT_APPLICABLE_DEVICE, st, dev);
    }

    /* 3. Call function with out of bound value for device id.*/

    dev = PRV_CPSS_MAX_PP_DEVICES_CNS;

    st = cpssDxChPortTxQArbGroupSet(dev, tcQueue, group, profile);
    UTF_VERIFY_EQUAL1_PARAM_MAC(GT_BAD_PARAM, st, dev);
}

/*----------------------------------------------------------------------------*/
/*
GT_STATUS cpssDxChPortTxQArbGroupGet
(
    IN  GT_U8                                   devNum,
    IN  GT_U8                                   tcQueue,
    IN  CPSS_PORT_TX_SCHEDULER_PROFILE_SET_ENT  profileSet,
    OUT CPSS_PORT_TX_Q_ARB_GROUP_ENT            *arbGroupPtr
)
*/
UTF_TEST_CASE_MAC(cpssDxChPortTxQArbGroupGet)
{
/*
    ITERATE_DEVICES (DxChx)
    1.1. Call with tcQueue [0, 1, 2, 3],
                                 arbGroup [CPSS_PORT_TX_WRR_ARB_GROUP_0_E,
                                           CPSS_PORT_TX_WRR_ARB_GROUP_1_E,
                                           CPSS_PORT_TX_SP_ARB_GROUP_E]
                            and profileSet [CPSS_PORT_TX_SCHEDULER_PROFILE_1_E,i
                                            CPSS_PORT_TX_SCHEDULER_PROFILE_2_E,
                                            CPSS_PORT_TX_SCHEDULER_PROFILE_3_E,
                                            CPSS_PORT_TX_SCHEDULER_PROFILE_4_E].
    Expected: GT_OK.
    1.2. Call with tcQueue [4, 5, 6, 7],
                                 arbGroup [CPSS_PORT_TX_WRR_ARB_GROUP_0_E,
                                           CPSS_PORT_TX_WRR_ARB_GROUP_1_E,
                                           CPSS_PORT_TX_SP_ARB_GROUP_E]
                            and profileSet [CPSS_PORT_TX_SCHEDULER_PROFILE_5_E,
                                            CPSS_PORT_TX_SCHEDULER_PROFILE_6_E,
                                            CPSS_PORT_TX_SCHEDULER_PROFILE_7_E,
                                            CPSS_PORT_TX_SCHEDULER_PROFILE_8_E].
    Expected: GT_OK for Cheetah and NON GT_OK for other devices.
    1.3. Call with out of range tcQueue [CPSS_TC_RANGE_CNS = 8],
                                arbGroup [CPSS_PORT_TX_WRR_ARB_GROUP_0_E]
                            and profileSet [CPSS_PORT_TX_SCHEDULER_PROFILE_1_E].
    Expected: GT_BAD_PARAM.
    1.4. Call with tcQueue [7], arbGroup [CPSS_PORT_TX_WRR_ARB_GROUP_1_E]
         and wrong enum values profileSet.
    Expected: GT_BAD_PARAM.
*/
    GT_STATUS   st     = GT_OK;

    GT_U8                                  dev;
    GT_U8                                  tcQueue   = 0;
    CPSS_PORT_TX_SCHEDULER_PROFILE_SET_ENT profile   = CPSS_PORT_TX_SCHEDULER_PROFILE_1_E;
    CPSS_PP_FAMILY_TYPE_ENT                devFamily = CPSS_MAX_FAMILY;
    CPSS_PORT_TX_Q_ARB_GROUP_ENT           group;


    /* prepare device iterator */
    PRV_TGF_NOT_APPLIC_DEV_RESET_MAC(&dev, UTF_NONE_FAMILY_E);

    /* 1. Go over all active devices. */
    while (GT_OK == prvUtfNextDeviceGet(&dev, GT_TRUE))
    {
        /*
           1.1. Call with tcQueue [0, 1, 2, 3],
                          arbGroup [CPSS_PORT_TX_WRR_ARB_GROUP_0_E /
                                    CPSS_PORT_TX_WRR_ARB_GROUP_1_E /
                                    CPSS_PORT_TX_SP_ARB_GROUP_E],
                          profileSet [CPSS_PORT_TX_SCHEDULER_PROFILE_1_E /
                                      CPSS_PORT_TX_SCHEDULER_PROFILE_2_E /
                                      CPSS_PORT_TX_SCHEDULER_PROFILE_3_E /
                                      CPSS_PORT_TX_SCHEDULER_PROFILE_4_E].
           Expected: GT_OK.
        */

        /* Call function with tcQueue = 0, profileSet = CPSS_PORT_TX_SCHEDULER_PROFILE_1_E */
        tcQueue = 0;
        profile = CPSS_PORT_TX_SCHEDULER_PROFILE_1_E;

        st = cpssDxChPortTxQArbGroupGet(dev, tcQueue, profile, &group);
        UTF_VERIFY_EQUAL3_PARAM_MAC(GT_OK, st, dev, tcQueue, profile);

        /* Call function with tcQueue = 1, profileSet = CPSS_PORT_TX_SCHEDULER_PROFILE_2_E */
        tcQueue = 1;
        profile = CPSS_PORT_TX_SCHEDULER_PROFILE_2_E;

        st = cpssDxChPortTxQArbGroupGet(dev, tcQueue, profile, &group);
        UTF_VERIFY_EQUAL3_PARAM_MAC(GT_OK, st, dev, tcQueue, profile);

        /* Call function with tcQueue = 2, profileSet = CPSS_PORT_TX_SCHEDULER_PROFILE_3_E */
        tcQueue = 2;
        profile = CPSS_PORT_TX_SCHEDULER_PROFILE_3_E;

        st = cpssDxChPortTxQArbGroupGet(dev, tcQueue, profile, &group);
        UTF_VERIFY_EQUAL3_PARAM_MAC(GT_OK, st, dev, tcQueue, profile);

        /* Call function with tcQueue = 3, profileSet = CPSS_PORT_TX_SCHEDULER_PROFILE_4_E */
        tcQueue = 3;
        group = CPSS_PORT_TX_WRR_ARB_GROUP_0_E;
        profile = CPSS_PORT_TX_SCHEDULER_PROFILE_4_E;

        st = cpssDxChPortTxQArbGroupGet(dev, tcQueue, profile, &group);
        UTF_VERIFY_EQUAL3_PARAM_MAC(GT_OK, st, dev, tcQueue, profile);

        /*
           1.2. Call with tcQueue [4, 5, 6, 7],
                          arbGroup [CPSS_PORT_TX_WRR_ARB_GROUP_0_E /
                                    CPSS_PORT_TX_WRR_ARB_GROUP_1_E /
                                    CPSS_PORT_TX_SP_ARB_GROUP_E],
                          and profileSet [CPSS_PORT_TX_SCHEDULER_PROFILE_5_E /
                                          CPSS_PORT_TX_SCHEDULER_PROFILE_6_E /
                                          CPSS_PORT_TX_SCHEDULER_PROFILE_7_E /
                                          CPSS_PORT_TX_SCHEDULER_PROFILE_8_E]
           Expected: GT_OK for Cheetah and NON GT_OK for other devices.
        */
        st = prvUtfDeviceFamilyGet(dev, &devFamily);
        UTF_VERIFY_EQUAL1_STRING_MAC(GT_OK, st, "prvUtfDeviceFamilyGet: %d", dev);

        /* Call function with tcQueue = 4, profileSet = CPSS_PORT_TX_SCHEDULER_PROFILE_5_E */
        tcQueue = 4;
        profile = CPSS_PORT_TX_SCHEDULER_PROFILE_5_E;

        st = cpssDxChPortTxQArbGroupGet(dev, tcQueue, profile, &group);
        if (CPSS_PP_FAMILY_CHEETAH_E  != devFamily)
        {
            UTF_VERIFY_EQUAL3_PARAM_MAC(GT_OK, st, dev, tcQueue, profile);
        }
        else
        {
            UTF_VERIFY_NOT_EQUAL3_PARAM_MAC(GT_OK, st, dev, tcQueue, profile);
        }

        /* Call function with tcQueue = 5, profileSet = CPSS_PORT_TX_SCHEDULER_PROFILE_6_E */
        tcQueue = 5;
        profile = CPSS_PORT_TX_SCHEDULER_PROFILE_6_E;

        st = cpssDxChPortTxQArbGroupGet(dev, tcQueue, profile, &group);
        if (CPSS_PP_FAMILY_CHEETAH_E  != devFamily)
        {
            UTF_VERIFY_EQUAL3_PARAM_MAC(GT_OK, st, dev, tcQueue, profile);
        }
        else
        {
            UTF_VERIFY_NOT_EQUAL3_PARAM_MAC(GT_OK, st, dev, tcQueue, profile);
        }

        /* Call function with tcQueue = 6, profileSet = CPSS_PORT_TX_SCHEDULER_PROFILE_7_E */
        tcQueue = 6;
        profile = CPSS_PORT_TX_SCHEDULER_PROFILE_7_E;

        st = cpssDxChPortTxQArbGroupGet(dev, tcQueue, profile, &group);
        if (CPSS_PP_FAMILY_CHEETAH_E  != devFamily)
        {
            UTF_VERIFY_EQUAL3_PARAM_MAC(GT_OK, st, dev, tcQueue, profile);
        }
        else
        {
            UTF_VERIFY_NOT_EQUAL3_PARAM_MAC(GT_OK, st, dev, tcQueue, profile);
        }

        /* Call function with tcQueue = 7, profileSet = CPSS_PORT_TX_SCHEDULER_PROFILE_8_E */
        tcQueue = 7;
        profile = CPSS_PORT_TX_SCHEDULER_PROFILE_8_E;

        st = cpssDxChPortTxQArbGroupGet(dev, tcQueue, profile, &group);
        if (CPSS_PP_FAMILY_CHEETAH_E != devFamily)
        {
            UTF_VERIFY_EQUAL3_PARAM_MAC(GT_OK, st, dev, tcQueue, profile);
        }
        else
        {
            UTF_VERIFY_NOT_EQUAL3_PARAM_MAC(GT_OK, st, dev, tcQueue, profile);
        }

        /*
           1.3. Call with out of range tcQueue [CPSS_TC_RANGE_CNS = 8],
                       profileSet [CPSS_PORT_TX_SCHEDULER_PROFILE_1_E].
            Expected: GT_BAD_PARAM.
        */
        tcQueue = CPSS_TC_RANGE_CNS;
        profile = CPSS_PORT_TX_SCHEDULER_PROFILE_1_E;

        st = cpssDxChPortTxQArbGroupGet(dev, tcQueue, profile, &group);
        UTF_VERIFY_EQUAL3_PARAM_MAC(GT_BAD_PARAM, st, dev, tcQueue, profile);

        /*
           1.4. Call with tcQueue [7], wrong enum values profileSet.
           Expected: GT_BAD_PARAM.
        */
        tcQueue = 7;

        UTF_ENUMS_CHECK_MAC(cpssDxChPortTxQArbGroupGet
                            (dev, tcQueue, profile, &group),
                            profile);
    }

    tcQueue = 7;
    profile = CPSS_PORT_TX_SCHEDULER_PROFILE_2_E;

    /* 2. For not-active devices and devices from non-applicable family */
    /* check that function returns GT_BAD_PARAM.                        */
    /* prepare device iterator */
    PRV_TGF_NOT_APPLIC_DEV_RESET_MAC(&dev, UTF_NONE_FAMILY_E);

    /* go over all non active devices */
    while(GT_OK == prvUtfNextDeviceGet(&dev, GT_FALSE))
    {
        st = cpssDxChPortTxQArbGroupGet(dev, tcQueue, profile, &group);
        UTF_VERIFY_EQUAL1_PARAM_MAC(GT_NOT_APPLICABLE_DEVICE, st, dev);
    }

    /* 3. Call function with out of bound value for device id.*/

    dev = PRV_CPSS_MAX_PP_DEVICES_CNS;

    st = cpssDxChPortTxQArbGroupGet(dev, tcQueue, profile, &group);
    UTF_VERIFY_EQUAL1_PARAM_MAC(GT_BAD_PARAM, st, dev);
}

/*----------------------------------------------------------------------------*/
/*
GT_STATUS cpssDxChPortTxBindPortToDpSet
(
    IN  GT_U8                               devNum,
    IN  GT_U8                               portNum,
    IN  CPSS_PORT_TX_DROP_PROFILE_SET_ENT   profileSet
)
*/
UTF_TEST_CASE_MAC(cpssDxChPortTxBindPortToDpSet)
{
/*
    ITERATE_DEVICES_PHY_CPU_PORTS (DxChx)
    1.1.1. Call with profileSet [CPSS_PORT_TX_DROP_PROFILE_1_E /
                                 CPSS_PORT_TX_DROP_PROFILE_4_E].
    Expected: GT_OK.
    1.1.2. Call cpssDxChPortTxBindPortToDpGet with non-NULL profileSetGet
                and other parameters from 1.1.1
    Expected: GT_OK and same profileSetGet as written
    1.1.3. Call with profileSet [CPSS_PORT_TX_DROP_PROFILE_5_E,
                                  CPSS_PORT_TX_DROP_PROFILE_8_E].
    Expected: GT_OK for Cheetah2 and NON GT_OK for other devices.
    1.1.4. Call cpssDxChPortTxBindPortToDpGet with non-NULL profileSetGet
                and other parameters from 1.1.3
    Expected: GT_OK and same profileSetGet as written
    1.1.5. Call with wrong enum values profileSet.
    Expected: GT_BAD_PARAM.
*/

    GT_STATUS   st     = GT_OK;

    GT_U8                             dev;
    GT_U8                             port       = PORT_TX_VALID_PHY_PORT_CNS;
    CPSS_PORT_TX_DROP_PROFILE_SET_ENT profile    = CPSS_PORT_TX_DROP_PROFILE_1_E;
    CPSS_PORT_TX_DROP_PROFILE_SET_ENT profileGet = CPSS_PORT_TX_DROP_PROFILE_1_E;
    CPSS_PP_FAMILY_TYPE_ENT           devFamily  = CPSS_MAX_FAMILY;

    /* prepare iterator for go over all active devices */
    PRV_TGF_NOT_APPLIC_DEV_RESET_MAC(&dev, UTF_NONE_FAMILY_E);
    /* 1. Go over all active devices. */
    while (GT_OK == prvUtfNextDeviceGet(&dev, GT_TRUE))
    {
        st = prvUtfNextPhyPortReset(&port, dev);
        UTF_VERIFY_EQUAL0_PARAM_MAC(GT_OK, st);

        /* 1.1. For all active devices go over all available physical ports. */
        while (GT_OK == prvUtfNextPhyPortGet(&port, GT_TRUE))
        {
            /* 1.1.1. Call with profileSet [CPSS_PORT_TX_DROP_PROFILE_1_E /
                                            CPSS_PORT_TX_DROP_PROFILE_4_E].
               Expected: GT_OK.
            */

            /* Call with profileSet [CPSS_PORT_TX_DROP_PROFILE_1_E] */
            profile = CPSS_PORT_TX_DROP_PROFILE_1_E;

            st = cpssDxChPortTxBindPortToDpSet(dev, port, profile);
            UTF_VERIFY_EQUAL3_PARAM_MAC(GT_OK, st, dev, port, profile);

            /*
                1.1.2. Call cpssDxChPortTxBindPortToDpGet with non-NULL profileSetGet
                            and other parameters from 1.1.1
                Expected: GT_OK and same profileSetGet as written
            */
            st = cpssDxChPortTxBindPortToDpGet(dev, port, &profileGet);
            UTF_VERIFY_EQUAL2_STRING_MAC(GT_OK, st, "cpssDxChPortTxBindPortToDpGet: %d", dev, port);
            UTF_VERIFY_EQUAL2_STRING_MAC(profile, profileGet, "got another profile than was set: %d", dev, port);

            /* Call with profileSet [CPSS_PORT_TX_DROP_PROFILE_4_E] */
            profile = CPSS_PORT_TX_DROP_PROFILE_4_E;

            st = cpssDxChPortTxBindPortToDpSet(dev, port, profile);
            UTF_VERIFY_EQUAL3_PARAM_MAC(GT_OK, st, dev, port, profile);

            /*
                1.1.2. Call cpssDxChPortTxBindPortToDpGet with non-NULL profileSetGet
                            and other parameters from 1.1.1
                Expected: GT_OK and same profileSetGet as written
            */
            st = cpssDxChPortTxBindPortToDpGet(dev, port, &profileGet);
            UTF_VERIFY_EQUAL2_STRING_MAC(GT_OK, st, "cpssDxChPortTxBindPortToDpGet: %d", dev, port);
            UTF_VERIFY_EQUAL2_STRING_MAC(profile, profileGet, "got another profile than was set: %d", dev, port);

            /* 1.1.3. Call with profileSet [CPSS_PORT_TX_DROP_PROFILE_5_E /
                                            CPSS_PORT_TX_DROP_PROFILE_8_E].
               Expected: GT_OK for Cheetah2 and NON GT_OK for other devices.
            */

            st = prvUtfDeviceFamilyGet(dev, &devFamily);
            UTF_VERIFY_EQUAL1_STRING_MAC(GT_OK, st, "prvUtfDeviceFamilyGet: %d", dev);

            /* Call with profileSet [CPSS_PORT_TX_DROP_PROFILE_5_E] */
            profile = CPSS_PORT_TX_DROP_PROFILE_5_E;

            st = cpssDxChPortTxBindPortToDpSet(dev, port, profile);
            if(devFamily != CPSS_PP_FAMILY_CHEETAH_E)
            {
                UTF_VERIFY_EQUAL3_PARAM_MAC(GT_OK, st, dev, port, profile);

                /*
                    1.1.4. Call cpssDxChPortTxBindPortToDpGet with non-NULL profileSetGet
                                and other parameters from 1.1.1
                    Expected: GT_OK and same profileSetGet as written
                */
                st = cpssDxChPortTxBindPortToDpGet(dev, port, &profileGet);
                UTF_VERIFY_EQUAL2_STRING_MAC(GT_OK, st, "cpssDxChPortTxBindPortToDpGet: %d", dev, port);
                UTF_VERIFY_EQUAL2_STRING_MAC(profile, profileGet, "got another profile than was set: %d", dev, port);
            }
            else
            {
                UTF_VERIFY_NOT_EQUAL3_PARAM_MAC(GT_OK, st, dev, port, profile);
            }

            /* Call with profileSet [CPSS_PORT_TX_DROP_PROFILE_8_E] */
            profile = CPSS_PORT_TX_DROP_PROFILE_8_E;

            st = cpssDxChPortTxBindPortToDpSet(dev, port, profile);
            if(devFamily != CPSS_PP_FAMILY_CHEETAH_E)
            {
                UTF_VERIFY_EQUAL3_PARAM_MAC(GT_OK, st, dev, port, profile);

                /*
                    1.1.4. Call cpssDxChPortTxBindPortToDpGet with non-NULL profileSetGet
                                and other parameters from 1.1.1
                    Expected: GT_OK and same profileSetGet as written
                */
                st = cpssDxChPortTxBindPortToDpGet(dev, port, &profileGet);
                UTF_VERIFY_EQUAL2_STRING_MAC(GT_OK, st, "cpssDxChPortTxBindPortToDpGet: %d", dev, port);
                UTF_VERIFY_EQUAL2_STRING_MAC(profile, profileGet, "got another profile than was set: %d", dev, port);
            }
            else
            {
                UTF_VERIFY_NOT_EQUAL3_PARAM_MAC(GT_OK, st, dev, port, profile);
            }

            /*
               1.1.5. Call with wrong enum values profileSet.
               Expected: GT_BAD_PARAM.
            */
            UTF_ENUMS_CHECK_MAC(cpssDxChPortTxBindPortToDpSet
                                (dev, port, profile),
                                profile);
        }

        profile = CPSS_PORT_TX_DROP_PROFILE_2_E;

        st = prvUtfNextPhyPortReset(&port, dev);
        UTF_VERIFY_EQUAL0_PARAM_MAC(GT_OK, st);
        /* 1.2. For all active devices go over all non available physical ports. */
        while (GT_OK == prvUtfNextPhyPortGet(&port, GT_FALSE))
        {
            /* 1.2.1. Call function for each non-active port */
            /* profileSet [CPSS_PORT_TX_DROP_PROFILE_2_E] */
            st = cpssDxChPortTxBindPortToDpSet(dev, port, profile);
            UTF_VERIFY_EQUAL2_PARAM_MAC(GT_BAD_PARAM, st, dev, port);
        }

        /* 1.3. For active device check that function returns GT_BAD_PARAM */
        /* for out of bound value for port number.                         */
        /* profile == CPSS_PORT_TX_DROP_PROFILE_2_E */
        port = PRV_CPSS_MAX_PP_PORTS_NUM_CNS;

        st = cpssDxChPortTxBindPortToDpSet(dev, port, profile);
        UTF_VERIFY_EQUAL2_PARAM_MAC(GT_BAD_PARAM, st, dev, port);

        /* 1.4. For active device check that function returns GT_OK        */
        /* for CPU port number.                                            */
        /* profile == CPSS_PORT_TX_DROP_PROFILE_2_E */
        port = CPSS_CPU_PORT_NUM_CNS;

        st = cpssDxChPortTxBindPortToDpSet(dev, port, profile);
        UTF_VERIFY_EQUAL2_PARAM_MAC(GT_OK, st, dev, port);
    }

    /* 2. For not active devices check that function returns non GT_OK.*/

    port = PORT_TX_VALID_PHY_PORT_CNS;

    /* prepare device iterator */
    PRV_TGF_NOT_APPLIC_DEV_RESET_MAC(&dev, UTF_NONE_FAMILY_E);
    /* Go over all non active devices. */
    while(GT_OK == prvUtfNextDeviceGet(&dev, GT_FALSE))
    {
        st = cpssDxChPortTxBindPortToDpSet(dev, port, profile);
        UTF_VERIFY_EQUAL1_PARAM_MAC(GT_NOT_APPLICABLE_DEVICE, st, dev);
    }

    /* 3.Call function with out of bound value for device id */
    dev = PRV_CPSS_MAX_PP_DEVICES_CNS;
    /* port == 0                                 */
    /* profile == CPSS_PORT_TX_DROP_PROFILE_2_E  */

    st = cpssDxChPortTxBindPortToDpSet(dev, port, profile);
    UTF_VERIFY_EQUAL1_PARAM_MAC(GT_BAD_PARAM, st, dev);
}

/*----------------------------------------------------------------------------*/
/*
GT_STATUS cpssDxChPortTxBindPortToDpGet
(
    IN  GT_U8                               devNum,
    IN  GT_U8                               portNum,
    OUT CPSS_PORT_TX_DROP_PROFILE_SET_ENT   *profileSetPtr
)
*/
UTF_TEST_CASE_MAC(cpssDxChPortTxBindPortToDpGet)
{
/*
    ITERATE_DEVICES_PHY_CPU_PORTS (DxChx)
    1.1.1. Call with non-NULL profile
    Expected: GT_OK.
    1.1.2. Call with profile [NULL].
    Expected: GT_BAD_PTR.
*/
    GT_STATUS   st  = GT_OK;
    GT_U8       dev;

    GT_U8                             port      = PORT_TX_VALID_PHY_PORT_CNS;
    CPSS_PORT_TX_DROP_PROFILE_SET_ENT profile   = CPSS_PORT_TX_DROP_PROFILE_1_E;


    /* prepare iterator for go over all active devices */
    PRV_TGF_NOT_APPLIC_DEV_RESET_MAC(&dev, UTF_NONE_FAMILY_E);

    /* 1. Go over all active devices. */
    while (GT_OK == prvUtfNextDeviceGet(&dev, GT_TRUE))
    {

        st = prvUtfNextPhyPortReset(&port, dev);
        UTF_VERIFY_EQUAL0_PARAM_MAC(GT_OK, st);

        /* 1.1. For all active devices go over all available physical ports. */
        while (GT_OK == prvUtfNextPhyPortGet(&port, GT_TRUE))
        {
            /*
                1.1.1. Call with non-NULL profile
                Expected: GT_OK.
            */
            st = cpssDxChPortTxBindPortToDpGet(dev, port, &profile);
            UTF_VERIFY_EQUAL2_PARAM_MAC(GT_OK, st, dev, port);
            /*
                1.1.2. Call with profile [NULL].
                Expected: GT_BAD_PTR.
            */
            st = cpssDxChPortTxBindPortToDpGet(dev, port, NULL);
            UTF_VERIFY_EQUAL2_STRING_MAC(GT_BAD_PTR, st, "%d, %d, profileSet = NULL", dev, port);
        }

        profile = CPSS_PORT_TX_DROP_PROFILE_2_E;

        st = prvUtfNextPhyPortReset(&port, dev);
        UTF_VERIFY_EQUAL0_PARAM_MAC(GT_OK, st);
        /* 1.2. For all active devices go over all non available physical ports. */
        while (GT_OK == prvUtfNextPhyPortGet(&port, GT_FALSE))
        {
            /* 1.2.1. Call function for each non-active port */
            st = cpssDxChPortTxBindPortToDpGet(dev, port, &profile);
            UTF_VERIFY_EQUAL2_PARAM_MAC(GT_BAD_PARAM, st, dev, port);
        }

        /* 1.3. For active device check that function returns GT_BAD_PARAM */
        /* for out of bound value for port number.                         */
        port = PRV_CPSS_MAX_PP_PORTS_NUM_CNS;

        st = cpssDxChPortTxBindPortToDpGet(dev, port, &profile);
        UTF_VERIFY_EQUAL2_PARAM_MAC(GT_BAD_PARAM, st, dev, port);

        /* 1.4. For active device check that function returns GT_OK        */
        /* for CPU port number.                                            */
        port = CPSS_CPU_PORT_NUM_CNS;

        st = cpssDxChPortTxBindPortToDpGet(dev, port, &profile);
        UTF_VERIFY_EQUAL2_PARAM_MAC(GT_OK, st, dev, port);
    }

    /* 2. For not active devices check that function returns non GT_OK.*/
    port = PORT_TX_VALID_PHY_PORT_CNS;

    /* prepare device iterator */
    PRV_TGF_NOT_APPLIC_DEV_RESET_MAC(&dev, UTF_NONE_FAMILY_E);

    /* Go over all non active devices. */
    while(GT_OK == prvUtfNextDeviceGet(&dev, GT_FALSE))
    {
        st = cpssDxChPortTxBindPortToDpGet(dev, port, &profile);
        UTF_VERIFY_EQUAL1_PARAM_MAC(GT_NOT_APPLICABLE_DEVICE, st, dev);
    }

    /* 3.Call function with out of bound value for device id */
    dev = PRV_CPSS_MAX_PP_DEVICES_CNS;

    st = cpssDxChPortTxBindPortToDpGet(dev, port, &profile);
    UTF_VERIFY_EQUAL1_PARAM_MAC(GT_BAD_PARAM, st, dev);
}

/*----------------------------------------------------------------------------*/
/*
GT_STATUS cpssDxChPortTxDescNumberGet
(
    IN  GT_U8       devNum,
    IN  GT_U8       portNum,
    OUT GT_U16      *numberPtr
)
*/
UTF_TEST_CASE_MAC(cpssDxChPortTxDescNumberGet)
{
    /*
    ITERATE_DEVICES_PHY_CPU_PORTS (DxChx)
    1.1.1. Call with non-null regsAddrPtr and non-null sizePtr. Expected: GT_OK.
    1.1.2. Call with null regsAddrPtr [NULL] and non-null sizePtr.
    Expected: GT_BAD_PTR.
    1.1.3. Call with non-null regsAddrPtr and null sizePtr [NULL].
    Expected: GT_BAD_PTR.
    */

    GT_STATUS   st     = GT_OK;

    GT_U8  dev;
    GT_U8  port   = PORT_TX_VALID_PHY_PORT_CNS;
    GT_U16 resNum = 0;

    /* prepare iterator for go over all active devices */
    PRV_TGF_NOT_APPLIC_DEV_RESET_MAC(&dev, UTF_NONE_FAMILY_E);

    /* 1. Go over all active devices. */
    while (GT_OK == prvUtfNextDeviceGet(&dev, GT_TRUE))
    {
        st = prvUtfNextPhyPortReset(&port, dev);
        UTF_VERIFY_EQUAL0_PARAM_MAC(GT_OK, st);

        /* 1.1. For all active devices go over all available physical ports. */
        while (GT_OK == prvUtfNextPhyPortGet(&port, GT_TRUE))
        {
            /* 1.1.1. Call with non-null numberPtr.
               Expected: GT_OK.
            */
            st = cpssDxChPortTxDescNumberGet(dev, port, &resNum);
            UTF_VERIFY_EQUAL2_PARAM_MAC(GT_OK, st, dev, port);

            /* 1.1.2. Call with null regsAddrPtr [NULL].
               Expected: GT_BAD_PTR.
            */
            st = cpssDxChPortTxDescNumberGet(dev, port, NULL);
            UTF_VERIFY_EQUAL2_STRING_MAC(GT_BAD_PTR, st, "%d, %d, NULL", dev, port);
        }

        st = prvUtfNextPhyPortReset(&port, dev);
        UTF_VERIFY_EQUAL0_PARAM_MAC(GT_OK, st);
        /* 1.2. For all active devices go over all non available physical ports. */
        while (GT_OK == prvUtfNextPhyPortGet(&port, GT_FALSE))
        {
            /* 1.2.1. Call function for each non-active port */
            st = cpssDxChPortTxDescNumberGet(dev, port, &resNum);
            UTF_VERIFY_EQUAL3_PARAM_MAC(GT_BAD_PARAM, st, dev, port, resNum);
        }

        /* 1.3. For active device check that function returns GT_BAD_PARAM */
        /* for out of bound value for port number.                         */
        port = PRV_CPSS_MAX_PP_PORTS_NUM_CNS;

        st = cpssDxChPortTxDescNumberGet(dev, port, &resNum);
        UTF_VERIFY_EQUAL2_PARAM_MAC(GT_BAD_PARAM, st, dev, port);

        /* 1.4. For active device check that function returns GT_OK        */
        /* for CPU port number.                                            */
        port = CPSS_CPU_PORT_NUM_CNS;

        st = cpssDxChPortTxDescNumberGet(dev, port, &resNum);
        UTF_VERIFY_EQUAL2_PARAM_MAC(GT_OK, st, dev, port);
    }

    /* 2. For not active devices check that function returns non GT_OK.*/

    port = PORT_TX_VALID_PHY_PORT_CNS;

    /* prepare device iterator */
    PRV_TGF_NOT_APPLIC_DEV_RESET_MAC(&dev, UTF_NONE_FAMILY_E);

    /* Go over all non active devices. */
    while(GT_OK == prvUtfNextDeviceGet(&dev, GT_FALSE))
    {
        st = cpssDxChPortTxDescNumberGet(dev, port, &resNum);
        UTF_VERIFY_EQUAL1_PARAM_MAC(GT_NOT_APPLICABLE_DEVICE, st, dev);
    }

    /* 3.Call function with out of bound value for device id */
    dev = PRV_CPSS_MAX_PP_DEVICES_CNS;
    /* port == 0 */

    st = cpssDxChPortTxDescNumberGet(dev, port, &resNum);
    UTF_VERIFY_EQUAL1_PARAM_MAC(GT_BAD_PARAM, st, dev);
}

/*----------------------------------------------------------------------------*/
/*
GT_STATUS cpssDxChPortTxBufNumberGet
(
    IN   GT_U8       devNum,
    IN   GT_U8       portNum,
    OUT  GT_U16      *numPtr
)
*/
UTF_TEST_CASE_MAC(cpssDxChPortTxBufNumberGet)
{
    /*
    ITERATE_DEVICES_PHY_PORTS (DxCh)
    1.1.1. Call with non-null numPtr.
    Expected: GT_OK.
    1.1.2. Call with numPtr [NULL].
    Expected: GT_BAD_PTR.
    */

    GT_STATUS   st     = GT_OK;

    GT_U8  dev;
    GT_U8  port   = PORT_TX_VALID_PHY_PORT_CNS;
    GT_U16 resNum;

    /* prepare iterator for go over all active devices */
    PRV_TGF_NOT_APPLIC_DEV_RESET_MAC(&dev, UTF_NONE_FAMILY_E);
    /* 1. Go over all active devices. */
    while (GT_OK == prvUtfNextDeviceGet(&dev, GT_TRUE))
    {
        st = prvUtfNextPhyPortReset(&port, dev);
        UTF_VERIFY_EQUAL0_PARAM_MAC(GT_OK, st);
        /* 1.1. For all active devices go over all available physical ports. */
        while (GT_OK == prvUtfNextPhyPortGet(&port, GT_TRUE))
        {
            /* 1.1.1. Call with non-null numberPtr.
               Expected: GT_OK.
            */
            st = cpssDxChPortTxBufNumberGet(dev, port, &resNum);
            UTF_VERIFY_EQUAL2_PARAM_MAC(GT_OK, st, dev, port);

            /* 1.1.2. Call with null regsAddrPtr [NULL].
               Expected: GT_BAD_PTR.
            */
            st = cpssDxChPortTxBufNumberGet(dev, port, NULL);
            UTF_VERIFY_EQUAL2_STRING_MAC(GT_BAD_PTR, st, "%d, %d, NULL", dev, port);
        }

        st = prvUtfNextPhyPortReset(&port, dev);
        UTF_VERIFY_EQUAL0_PARAM_MAC(GT_OK, st);
        /* 1.2. For all active devices go over all non available physical ports. */
        while (GT_OK == prvUtfNextPhyPortGet(&port, GT_FALSE))
        {
            /* 1.2.1. Call function for each non-active port */
            st = cpssDxChPortTxBufNumberGet(dev, port, &resNum);
            UTF_VERIFY_EQUAL3_PARAM_MAC(GT_BAD_PARAM, st, dev, port, resNum);
        }

        /* 1.3. For active device check that function returns GT_BAD_PARAM */
        /* for out of bound value for port number.                         */
        port = PRV_CPSS_MAX_PP_PORTS_NUM_CNS;

        st = cpssDxChPortTxBufNumberGet(dev, port, &resNum);
        UTF_VERIFY_EQUAL2_PARAM_MAC(GT_BAD_PARAM, st, dev, port);

        /* 1.4. For active device check that function returns GT_OK        */
        /* for CPU port number.                                            */
        port = CPSS_CPU_PORT_NUM_CNS;

        st = cpssDxChPortTxBufNumberGet(dev, port, &resNum);
        UTF_VERIFY_EQUAL2_PARAM_MAC(GT_OK, st, dev, port);
    }

    /* 2. For not active devices check that function returns non GT_OK.*/
    port = PORT_TX_VALID_PHY_PORT_CNS;

    /* prepare device iterator */
    PRV_TGF_NOT_APPLIC_DEV_RESET_MAC(&dev, UTF_NONE_FAMILY_E);
    /* Go over all non active devices. */
    while(GT_OK == prvUtfNextDeviceGet(&dev, GT_FALSE))
    {
        st = cpssDxChPortTxBufNumberGet(dev, port, &resNum);
        UTF_VERIFY_EQUAL1_PARAM_MAC(GT_NOT_APPLICABLE_DEVICE, st, dev);
    }

    /* 3.Call function with out of bound value for device id */
    dev = PRV_CPSS_MAX_PP_DEVICES_CNS;
    /* port == 0 */

    st = cpssDxChPortTxBufNumberGet(dev, port, &resNum);
    UTF_VERIFY_EQUAL1_PARAM_MAC(GT_BAD_PARAM, st, dev);
}

/*----------------------------------------------------------------------------*/
/*
GT_STATUS cpssDxChPortTx4TcDescNumberGet
(
    IN  GT_U8       devNum,
    IN  GT_U8       portNum,
    IN  GT_U8       trafClass,
    OUT GT_U16      *numberPtr
)
*/
UTF_TEST_CASE_MAC(cpssDxChPortTx4TcDescNumberGet)
{
    /*
    ITERATE_DEVICES_PHY_CPU_PORTS (DxChx)
    1.1.1. Call with trafClass [4], and non-null numberPtr.
    Expected: GT_OK.
    1.1.2. Call with out of range trafClass [CPSS_TC_RANGE_CNS  = 8] and non-null numberPtr.
    Expected: GT_BAD_PARAM.
    1.1.3. Call with trafClass [2], and numberPtr [NULL]
    Expected: GT_BAD_PTR.
    */

    GT_STATUS   st     = GT_OK;

    GT_U8  dev;
    GT_U8  port   = PORT_TX_VALID_PHY_PORT_CNS;
    GT_U8  tc     = 0;
    GT_U16 resNum = 0;

    /* prepare iterator for go over all active devices */
    PRV_TGF_NOT_APPLIC_DEV_RESET_MAC(&dev, UTF_NONE_FAMILY_E);

    /* 1. Go over all active devices. */
    while (GT_OK == prvUtfNextDeviceGet(&dev, GT_TRUE))
    {
        st = prvUtfNextPhyPortReset(&port, dev);
        UTF_VERIFY_EQUAL0_PARAM_MAC(GT_OK, st);

        /* 1.1. For all active devices go over all available physical ports. */
        while (GT_OK == prvUtfNextPhyPortGet(&port, GT_TRUE))
        {
            /* 1.1.1. Call with trafClass [4], and non-null numberPtr.
               Expected: GT_OK.
            */
            tc = 4;

            st = cpssDxChPortTx4TcDescNumberGet(dev, port, tc, &resNum);
            UTF_VERIFY_EQUAL3_PARAM_MAC(GT_OK, st, dev, port, tc);

            /* 1.1.2. Call with out of range trafClass [CPSS_TC_RANGE_CNS  = 8],
                                non-null numberPtr.
               Expected: GT_BAD_PARAM.
            */
            tc = CPSS_TC_RANGE_CNS;

            st = cpssDxChPortTx4TcDescNumberGet(dev, port, tc, &resNum);
            UTF_VERIFY_EQUAL3_PARAM_MAC(GT_BAD_PARAM, st, dev, port, tc);

            /* 1.1.3. Call with trafClass [2],
               numberPtr [NULL]. Expected: GT_BAD_PTR.
            */
            tc = 2;

            st = cpssDxChPortTx4TcDescNumberGet(dev, port, tc, NULL);
            UTF_VERIFY_EQUAL3_STRING_MAC(GT_BAD_PTR, st,
                                         "%d, %d, %d, NULL", dev, port, tc);
        }

        st = prvUtfNextPhyPortReset(&port, dev);
        UTF_VERIFY_EQUAL0_PARAM_MAC(GT_OK, st);
        /* 1.2. For all active devices go over all non available physical ports. */
        while (GT_OK == prvUtfNextPhyPortGet(&port, GT_FALSE))
        {
            /* 1.2.1. Call function for each non-active port */
            st = cpssDxChPortTx4TcDescNumberGet(dev, port, tc, &resNum);
            UTF_VERIFY_EQUAL2_PARAM_MAC(GT_BAD_PARAM, st, dev, port);
        }

        /* 1.3. For active device check that function returns GT_BAD_PARAM */
        /* for out of bound value for port number.                         */
        port = PRV_CPSS_MAX_PP_PORTS_NUM_CNS;

        st = cpssDxChPortTx4TcDescNumberGet(dev, port, tc, &resNum);
        UTF_VERIFY_EQUAL2_PARAM_MAC(GT_BAD_PARAM, st, dev, port);

        /* 1.3. For active device check that function returns GT_OK        */
        /* for CPU port number.                                            */
        port = CPSS_CPU_PORT_NUM_CNS;

        st = cpssDxChPortTx4TcDescNumberGet(dev, port, tc, &resNum);
        UTF_VERIFY_EQUAL2_PARAM_MAC(GT_OK, st, dev, port);
    }

    /* 2. For not active devices check that function returns non GT_OK.*/

    port = PORT_TX_VALID_PHY_PORT_CNS;

    /* prepare device iterator */
    PRV_TGF_NOT_APPLIC_DEV_RESET_MAC(&dev, UTF_NONE_FAMILY_E);

    /* Go over all non active devices. */
    while(GT_OK == prvUtfNextDeviceGet(&dev, GT_FALSE))
    {
        st = cpssDxChPortTx4TcDescNumberGet(dev, port, tc, &resNum);
        UTF_VERIFY_EQUAL1_PARAM_MAC(GT_NOT_APPLICABLE_DEVICE, st, dev);
    }

    /* 3.Call function with out of bound value for device id */
    dev = PRV_CPSS_MAX_PP_DEVICES_CNS;
    /* port == 0, tc == 2 */

    st = cpssDxChPortTx4TcDescNumberGet(dev, port, tc, &resNum);
    UTF_VERIFY_EQUAL1_PARAM_MAC(GT_BAD_PARAM, st, dev);
}

/*----------------------------------------------------------------------------*/
/*
GT_STATUS cpssDxChPortTx4TcBufNumberGet
(
    IN  GT_U8       devNum,
    IN  GT_U8       portNum,
    IN  GT_U8       trafClass,
    OUT GT_U16      *numPtr
)
*/
UTF_TEST_CASE_MAC(cpssDxChPortTx4TcBufNumberGet)
{
    /*
    ITERATE_DEVICES_PHY_CPU_PORTS (DxChx)
    1.1.1. Call with trafClass [5], and non-null numPtr.
    Expected: GT_OK.
    1.1.2. Call with out of range trafClass [CPSS_TC_RANGE_CNS = 8] and non-null numPtr.
    Expected: GT_BAD_PARAM.
    1.1.3. Call with trafClass [1], and numPtr [NULL]
    Expected: GT_BAD_PTR.
    */

    GT_STATUS   st     = GT_OK;

    GT_U8  dev;
    GT_U8  port   = PORT_TX_VALID_PHY_PORT_CNS;
    GT_U8  tc     = 0;
    GT_U16 resNum = 0;

    /* prepare iterator for go over all active devices */
    PRV_TGF_NOT_APPLIC_DEV_RESET_MAC(&dev, UTF_NONE_FAMILY_E);
    /* 1. Go over all active devices. */
    while (GT_OK == prvUtfNextDeviceGet(&dev, GT_TRUE))
    {
        st = prvUtfNextPhyPortReset(&port, dev);
        UTF_VERIFY_EQUAL0_PARAM_MAC(GT_OK, st);
        /* 1.1. For all active devices go over all available physical ports. */
        while (GT_OK == prvUtfNextPhyPortGet(&port, GT_TRUE))
        {
            /* 1.1.1. Call with trafClass [5], and non-null numberPtr.
               Expected: GT_OK.
            */
            tc = 5;

            st = cpssDxChPortTx4TcBufNumberGet(dev, port, tc, &resNum);
            UTF_VERIFY_EQUAL4_PARAM_MAC(GT_OK, st, dev, port, tc, resNum);

            /* 1.1.2. Call with out of range trafClass [CPSS_TC_RANGE_CNS  = 8],
               non-null numberPtr. Expected: GT_BAD_PARAM.
            */
            tc = CPSS_TC_RANGE_CNS;

            st = cpssDxChPortTx4TcBufNumberGet(dev, port, tc, &resNum);
            UTF_VERIFY_EQUAL4_PARAM_MAC(GT_BAD_PARAM, st, dev, port, tc, resNum);

            /* 1.1.3. Call with trafClass [1], numberPtr [NULL].
               Expected: NOT GT_BAD_PTR.
            */
            tc = 1;

            st = cpssDxChPortTx4TcBufNumberGet(dev, port, tc, NULL);
            UTF_VERIFY_EQUAL3_STRING_MAC(GT_BAD_PTR, st, "%d, %d, %d, NULL",
                                         dev, port, tc);
        }

        st = prvUtfNextPhyPortReset(&port, dev);
        UTF_VERIFY_EQUAL0_PARAM_MAC(GT_OK, st);
        /* 1.2. For all active devices go over all non available physical ports. */
        while (GT_OK == prvUtfNextPhyPortGet(&port, GT_FALSE))
        {
            /* 1.2.1. Call function for each non-active port */
            st = cpssDxChPortTx4TcBufNumberGet(dev, port, tc, &resNum);
            UTF_VERIFY_EQUAL4_PARAM_MAC(GT_BAD_PARAM, st, dev, port, tc, resNum);
        }

        /* 1.3. For active device check that function returns GT_BAD_PARAM */
        /* for out of bound value for port number.                         */
        port = PRV_CPSS_MAX_PP_PORTS_NUM_CNS;

        st = cpssDxChPortTx4TcBufNumberGet(dev, port, tc, &resNum);
        UTF_VERIFY_EQUAL2_PARAM_MAC(GT_BAD_PARAM, st, dev, port);

        /* 1.4. For active device check that function returns GT_OK        */
        /* for CPU port number.                                            */
        port = CPSS_CPU_PORT_NUM_CNS;

        st = cpssDxChPortTx4TcBufNumberGet(dev, port, tc, &resNum);
        UTF_VERIFY_EQUAL2_PARAM_MAC(GT_OK, st, dev, port);
    }

    /* 2. For not active devices check that function returns non GT_OK.*/
    port = PORT_TX_VALID_PHY_PORT_CNS;

    /* prepare device iterator */
    PRV_TGF_NOT_APPLIC_DEV_RESET_MAC(&dev, UTF_NONE_FAMILY_E);
    /* Go over all non active devices. */
    while(GT_OK == prvUtfNextDeviceGet(&dev, GT_FALSE))
    {
        st = cpssDxChPortTx4TcBufNumberGet(dev, port, tc, &resNum);
        UTF_VERIFY_EQUAL1_PARAM_MAC(GT_NOT_APPLICABLE_DEVICE, st, dev);
    }

    /* 3.Call function with out of bound value for device id */
    dev = PRV_CPSS_MAX_PP_DEVICES_CNS;
    /* port == 0, tc == 1 */

    st = cpssDxChPortTx4TcBufNumberGet(dev, port, tc, &resNum);
    UTF_VERIFY_EQUAL1_PARAM_MAC(GT_BAD_PARAM, st, dev);
}

/*----------------------------------------------------------------------------*/
/*
GT_STATUS cpssDxChPortTxToCpuShaperModeSet
(
    IN  GT_U8                               devNum,
    IN  CPSS_PORT_TX_DROP_SHAPER_MODE_ENT   mode
)
*/
UTF_TEST_CASE_MAC(cpssDxChPortTxToCpuShaperModeSet)
{
    /*
    ITERATE_DEVICES (DxCh)
    1.1. Call with mode [CPSS_PORT_TX_DROP_SHAPER_BYTE_MODE_E, CPSS_PORT_TX_DROP_SHAPER_PACKET_MODE_E].
    Expected: GT_OK.
    1.2. Call with wrong enum values mode.
    Expected:  GT_BAD_PARAM.
    */

    GT_STATUS   st     = GT_OK;

    GT_U8                             dev;
    CPSS_PORT_TX_DROP_SHAPER_MODE_ENT mode = 0;

    /* prepare device iterator */
    PRV_TGF_NOT_APPLIC_DEV_RESET_MAC(&dev, UTF_LION_E);

    /* 1. Go over all active devices. */
    while (GT_OK == prvUtfNextDeviceGet(&dev, GT_TRUE))
    {
        /* 1.1. Call with mode [CPSS_PORT_TX_DROP_SHAPER_BYTE_MODE_E /
                                CPSS_PORT_TX_DROP_SHAPER_PACKET_MODE_E].
           Expected: GT_OK.
        */

        /* Call function with mode = CPSS_PORT_TX_DROP_SHAPER_BYTE_MODE_E */
        mode = CPSS_PORT_TX_DROP_SHAPER_BYTE_MODE_E;

        st = cpssDxChPortTxToCpuShaperModeSet(dev, mode);
        UTF_VERIFY_EQUAL2_PARAM_MAC(GT_OK, st, dev, mode);

        /* Call function with mode = CPSS_PORT_TX_DROP_SHAPER_PACKET_MODE_E */
        mode = CPSS_PORT_TX_DROP_SHAPER_PACKET_MODE_E;

        st = cpssDxChPortTxToCpuShaperModeSet(dev, mode);
        UTF_VERIFY_EQUAL2_PARAM_MAC(GT_OK, st, dev, mode);

        /*
           1.2. Call with wrong enum values mode.
           Expected:  GT_BAD_PARAM.
        */
        UTF_ENUMS_CHECK_MAC(cpssDxChPortTxToCpuShaperModeSet
                            (dev, mode),
                            mode);
    }

    mode = CPSS_PORT_TX_DROP_SHAPER_BYTE_MODE_E;

    /* 2. For not-active devices and devices from non-applicable family */
    /* check that function returns GT_BAD_PARAM.                        */

    /* prepare device iterator */
    PRV_TGF_NOT_APPLIC_DEV_RESET_MAC(&dev, UTF_LION_E);
    /* go over all non active devices */
    while(GT_OK == prvUtfNextDeviceGet(&dev, GT_FALSE))
    {
        st = cpssDxChPortTxToCpuShaperModeSet(dev, mode);
        UTF_VERIFY_EQUAL1_PARAM_MAC(GT_NOT_APPLICABLE_DEVICE, st, dev);
    }

    /* 3. Call function with out of bound value for device id.*/

    dev = PRV_CPSS_MAX_PP_DEVICES_CNS;
    /* mode == CPSS_PORT_TX_DROP_SHAPER_BYTE_MODE_E */

    st = cpssDxChPortTxToCpuShaperModeSet(dev, mode);
    UTF_VERIFY_EQUAL1_PARAM_MAC(GT_BAD_PARAM, st, dev);
}

/*----------------------------------------------------------------------------*/
/*
GT_STATUS cpssDxChPortTxSharingGlobalResourceEnableSet
(
    IN  GT_U8       devNum,
    IN  GT_BOOL     enable
)
*/
UTF_TEST_CASE_MAC(cpssDxChPortTxSharingGlobalResourceEnableSet)
{
/*
    ITERATE_DEVICES (DxCh2 and above)
    1.1. Call with enable [GT_FALSE and GT_TRUE].
    Expected: GT_OK.
    1.2. Call cpssDxChPortTxSharingGlobalResourceEnableGet with the same params.
    Expected: GT_OK and the same value.
*/
    GT_STATUS   st     = GT_OK;

    GT_U8       dev;
    GT_BOOL     enable = GT_TRUE;
    GT_BOOL     enableGet;

    /* prepare device iterator */
    PRV_TGF_NOT_APPLIC_DEV_RESET_MAC(&dev, UTF_CH1_E | UTF_CH1_DIAMOND_E | UTF_LION_E);

    /* 1. Go over all active devices. */
    while (GT_OK == prvUtfNextDeviceGet(&dev, GT_TRUE))
    {
        /*
            1.1. Call with enable [GT_FALSE and GT_TRUE].
            Expected: GT_OK.
        */
        /* Call function with [enable==GT_FALSE] */
        enable = GT_FALSE;
        st = cpssDxChPortTxSharingGlobalResourceEnableSet(dev, enable);
        UTF_VERIFY_EQUAL2_PARAM_MAC(GT_OK, st, dev, enable);

        /*
            1.2. Call cpssDxChPortTxSharingGlobalResourceEnableGet with the same params.
            Expected: GT_OK and the same value.
        */
        st = cpssDxChPortTxSharingGlobalResourceEnableGet(dev, &enableGet);
        UTF_VERIFY_EQUAL2_PARAM_MAC(GT_OK, st, dev, enable);


        /* Call function with [enable==GT_TRUE] */
        enable = GT_TRUE;
        st = cpssDxChPortTxSharingGlobalResourceEnableSet(dev, enable);
        UTF_VERIFY_EQUAL2_PARAM_MAC(GT_OK, st, dev, enable);

        /*
            1.2. Call cpssDxChPortTxSharingGlobalResourceEnableGet with the same params.
            Expected: GT_OK and the same value.
        */
        st = cpssDxChPortTxSharingGlobalResourceEnableGet(dev, &enableGet);
        UTF_VERIFY_EQUAL2_PARAM_MAC(GT_OK, st, dev, enable);
    }

    /* 2. For not-active devices and devices from non-applicable family */
    /* check that function returns GT_BAD_PARAM.                        */

    /* prepare device iterator */
    PRV_TGF_NOT_APPLIC_DEV_RESET_MAC(&dev, UTF_CH1_E | UTF_CH1_DIAMOND_E | UTF_LION_E);

    /* go over all non active devices */
    /* enable == GT_TRUE              */
    while(GT_OK == prvUtfNextDeviceGet(&dev, GT_FALSE))
    {
        st = cpssDxChPortTxSharingGlobalResourceEnableSet(dev, enable);
        UTF_VERIFY_EQUAL1_PARAM_MAC(GT_NOT_APPLICABLE_DEVICE, st, dev);
    }

    /* 3. Call function with out of bound value for device id.*/
    dev = PRV_CPSS_MAX_PP_DEVICES_CNS;
    /* enable == GT_TRUE */

    st = cpssDxChPortTxSharingGlobalResourceEnableSet(dev, enable);
    UTF_VERIFY_EQUAL1_PARAM_MAC(GT_BAD_PARAM, st, dev);
}

/*----------------------------------------------------------------------------*/
/*
GT_STATUS cpssDxChPortTxSharingGlobalResourceEnableGet
(
    IN  GT_U8    dev,
    OUT GT_BOOL *enablePtr
)
*/
UTF_TEST_CASE_MAC(cpssDxChPortTxSharingGlobalResourceEnableGet)
{
/*
    ITERATE_DEVICES (DxCh2 and above)
    1.1. Call function with correct parameters.
    Expected: GT_OK.
    1.2. For active device check that function returns GT_BAD_PTR.
    Expected: GT_BAD_PTR.
*/
    GT_STATUS   st     = GT_OK;
    GT_U8       dev;
    GT_BOOL     enable;

    /* prepare iterator for go over all active devices */
    PRV_TGF_NOT_APPLIC_DEV_RESET_MAC(&dev, UTF_CH1_E | UTF_CH1_DIAMOND_E | UTF_LION_E);

    /* 1. Go over all active devices. */
    while (GT_OK == prvUtfNextDeviceGet(&dev, GT_TRUE))
    {
        /*
            1.1. Call function with correct parameters.
            Expected: GT_OK.
        */
        st = cpssDxChPortTxSharingGlobalResourceEnableGet(dev, &enable);
        UTF_VERIFY_EQUAL2_PARAM_MAC(GT_OK, st, dev, enable);

        /*
            1.2. For active device check that function returns GT_BAD_PTR.
            Expected: GT_BAD_PTR.
        */
        st = cpssDxChPortTxSharingGlobalResourceEnableGet(dev, NULL);
        UTF_VERIFY_EQUAL1_PARAM_MAC(GT_BAD_PTR, st, dev);
    }

    /* 2. For not active devices check that function returns non GT_OK.*/
    /* prepare device iterator */
    PRV_TGF_NOT_APPLIC_DEV_RESET_MAC(&dev, UTF_CH1_E | UTF_CH1_DIAMOND_E | UTF_LION_E);

    /* Go over all non active devices. */
    while(GT_OK == prvUtfNextDeviceGet(&dev, GT_FALSE))
    {
        st = cpssDxChPortTxSharingGlobalResourceEnableGet(dev, &enable);
        UTF_VERIFY_EQUAL1_PARAM_MAC(GT_NOT_APPLICABLE_DEVICE, st, dev);
    }

    /* 3.Call function with out of bound value for device id */
    dev = PRV_CPSS_MAX_PP_DEVICES_CNS;
    st = cpssDxChPortTxSharingGlobalResourceEnableGet(dev, &enable);
    UTF_VERIFY_EQUAL1_PARAM_MAC(GT_BAD_PARAM, st, dev);
}

/*----------------------------------------------------------------------------*/
/*
GT_STATUS cpssDxChPortTxSharedGlobalResourceLimitsSet
(
    IN  GT_U8   devNum,
    IN  GT_U32  sharedBufLimit,
    IN  GT_U32  sharedDescLimit
)
*/
UTF_TEST_CASE_MAC(cpssDxChPortTxSharedGlobalResourceLimitsSet)
{
/*
    ITERATE_DEVICES (DxCh2 and above)
    1.1. Call with sharedBufLimit [128] and sharedDescLimit [65].
    Expected: GT_OK.
    1.2. Call cpssDxChPortTxSharedGlobalResourceLimitsGet with the same params.
    Expected: GT_OK and the same values.
    1.3. Call with out of range sharedBufLimit [4096] out of range for DxCh2,
                   and sharedDescLimit [1].
    Expected: NOT GT_OK.
    1.4. Call with sharedBufLimit [16381] out of range for DxCh2 and DxCh3,
                   and sharedDescLimit [1].
    Expected: NOT GT_OK.
    1.5. Call with sharedBufLimit [2] and
                   sharedDescLimit [4096] out of range for DxCh2
    Expected: NOT GT_OK.
    1.6. Call with sharedBufLimit [2] and
                   sharedDescLimit [16381] out of range for DxCh2 and DxCh3
    Expected: NOT GT_OK.
*/
    GT_STATUS st    = GT_OK;

    GT_U8     dev;
    GT_U32    bufLimit     = 0;
    GT_U32    bufLimitGet  = 0;
    GT_U32    descLimit    = 0;
    GT_U32    descLimitGet = 0;

    /* prepare device iterator */
    PRV_TGF_NOT_APPLIC_DEV_RESET_MAC(&dev, UTF_CH1_E | UTF_CH1_DIAMOND_E | UTF_LION_E);

    /* 1. Go over all active devices. */
    while (GT_OK == prvUtfNextDeviceGet(&dev, GT_TRUE))
    {
        /*
            1.1. Call with sharedBufLimit [128] and sharedDescLimit [68].
            Expected: GT_OK.
        */
        bufLimit = 128;
/*
*       In DxCh3 and above sharedBufLimit and sharedDescLimit are rounded up
*       to the nearest multiple of 4 since the corresponding fields in the
*       Transmit Queue Resource Sharing register are in 4 buffers\descriptors
*       resolution. */
        descLimit = 68;

        st = cpssDxChPortTxSharedGlobalResourceLimitsSet(dev, bufLimit, descLimit);
        UTF_VERIFY_EQUAL3_PARAM_MAC(GT_OK, st, dev, bufLimit, descLimit);

        /*
            1.2. Call cpssDxChPortTxSharedGlobalResourceLimitsGet with the same params.
            Expected: GT_OK and the same values.
        */
        st = cpssDxChPortTxSharedGlobalResourceLimitsGet(dev, &bufLimitGet, &descLimitGet);
        UTF_VERIFY_EQUAL1_PARAM_MAC(GT_OK, st, dev);

        UTF_VERIFY_EQUAL1_STRING_MAC(bufLimit, bufLimitGet,
                            "got another bufLimit than was set: %d", dev);
        UTF_VERIFY_EQUAL1_STRING_MAC(descLimit, descLimitGet,
                            "got another descLimit than was set: %d", dev);

        /*
            1.3. Call with out of range sharedBufLimit [4096] out of range for DxCh2,
                     and sharedDescLimit [1].
            Expected: NOT GT_OK.
        */
        bufLimit = 4096;
        descLimit = 1;

        st = cpssDxChPortTxSharedGlobalResourceLimitsSet(dev, bufLimit, descLimit);
        /* CH2 */
        if( PRV_CPSS_PP_MAC(dev)->devFamily == CPSS_PP_FAMILY_CHEETAH2_E )
        {
            UTF_VERIFY_NOT_EQUAL3_PARAM_MAC(GT_OK, st, dev, bufLimit, descLimit);
        }
        else /* CH3 and above */
        {
            UTF_VERIFY_EQUAL3_PARAM_MAC(GT_OK, st, dev, bufLimit, descLimit);
        }

        /*
            1.4. Call with sharedBufLimit [16381] out of range for DxCh2 and DxCh3,
                           and sharedDescLimit [1].
            Expected: NOT GT_OK.
        */
        if(0 == PRV_CPSS_DXCH_PP_HW_INFO_TXQ_REV_1_OR_ABOVE_MAC(dev))
        {
            /* 14 bits enough for this device */
            bufLimit = BIT_14;
        }
        else
        {
            /* 16 bits enough for this device */
            bufLimit = BIT_16;
        }

        descLimit = 1;

        st = cpssDxChPortTxSharedGlobalResourceLimitsSet(dev, bufLimit, descLimit);
        UTF_VERIFY_NOT_EQUAL3_PARAM_MAC(GT_OK, st, dev, bufLimit, descLimit);

        /* 1.5. Call with sharedBufLimit [2]
                          sharedDescLimit [4096] out of range for DxCh2
           Expected: GT_OK.
        */
        bufLimit = 2;
        descLimit = 4096;

        st = cpssDxChPortTxSharedGlobalResourceLimitsSet(dev, bufLimit, descLimit);
        /* CH2 */
        if( PRV_CPSS_PP_MAC(dev)->devFamily == CPSS_PP_FAMILY_CHEETAH2_E )
        {
            UTF_VERIFY_NOT_EQUAL3_PARAM_MAC(GT_OK, st, dev, bufLimit, descLimit);
        }
        else /* CH3 and above */
        {
            UTF_VERIFY_EQUAL3_PARAM_MAC(GT_OK, st, dev, bufLimit, descLimit);
        }

        /*
            1.6. Call with sharedBufLimit [2] and
                           sharedDescLimit [16381] out of range for DxCh2 and DxCh3
            Expected: NOT GT_OK.
        */
        bufLimit = 2;
        if(0 == PRV_CPSS_DXCH_PP_HW_INFO_TXQ_REV_1_OR_ABOVE_MAC(dev))
        {
            /* 14 bits enough for this device */
            descLimit = BIT_14;
        }
        else
        {
            /* 16 bits enough for this device */
            descLimit = BIT_16;
        }

        st = cpssDxChPortTxSharedGlobalResourceLimitsSet(dev, bufLimit, descLimit);
        UTF_VERIFY_NOT_EQUAL3_PARAM_MAC(GT_OK, st, dev, bufLimit, descLimit);
    }

    bufLimit = 65;
    descLimit = 27;

    /* 2. For not-active devices and devices from non-applicable family */
    /* check that function returns GT_BAD_PARAM.                        */

    /* prepare device iterator */
    PRV_TGF_NOT_APPLIC_DEV_RESET_MAC(&dev, UTF_CH1_E | UTF_CH1_DIAMOND_E | UTF_LION_E);

    /* go over all non active devices */
    /* sharedBufLimit [65], sharedDescLimit[27] */
    while(GT_OK == prvUtfNextDeviceGet(&dev, GT_FALSE))
    {
        st = cpssDxChPortTxSharedGlobalResourceLimitsSet(dev, bufLimit, descLimit);
        UTF_VERIFY_EQUAL1_PARAM_MAC(GT_NOT_APPLICABLE_DEVICE, st, dev);
    }

    /* 3. Call function with out of bound value for device id.*/

    dev = PRV_CPSS_MAX_PP_DEVICES_CNS;
    /* sharedBufLimit [65], sharedDescLimit[27] */

    st = cpssDxChPortTxSharedGlobalResourceLimitsSet(dev, bufLimit, descLimit);
    UTF_VERIFY_EQUAL1_PARAM_MAC(GT_BAD_PARAM, st, dev);
}

/*----------------------------------------------------------------------------*/
/*
GT_STATUS cpssDxChPortTxSharedGlobalResourceLimitsGet
(
    IN  GT_U8   devNum,
    OUT GT_U32  *sharedBufLimitPtr,
    OUT GT_U32  *sharedDescLimitPtr
)
*/
UTF_TEST_CASE_MAC(cpssDxChPortTxSharedGlobalResourceLimitsGet)
{
/*
    ITERATE_DEVICES (DxCh2 and above)
    1.1. Call with non-NULL pointers.
    Expected: GT_OK.
    1.2. Call with wrong sharedBufLimitPtr null pointer.
    Expected: GT_BAD_PTR.
    1.2. Call with wrong sharedDescLimitPtr null pointer.
    Expected: GT_BAD_PTR.
*/
    GT_STATUS   st  = GT_OK;
    GT_U8       dev;
    GT_U32      sharedBufLimitPtr;
    GT_U32      sharedDescLimitPtr;

    /* prepare iterator for go over all active devices */
    PRV_TGF_NOT_APPLIC_DEV_RESET_MAC(&dev, UTF_CH1_E | UTF_CH1_DIAMOND_E | UTF_LION_E);

    /* 1. Go over all active devices. */
    while (GT_OK == prvUtfNextDeviceGet(&dev, GT_TRUE))
    {
        /*
            1.1. Call with non-NULL pointers.
            Expected: GT_OK.
        */
        st = cpssDxChPortTxSharedGlobalResourceLimitsGet(dev,
                                  &sharedBufLimitPtr, &sharedDescLimitPtr);
        UTF_VERIFY_EQUAL1_PARAM_MAC(GT_OK, st, dev);

        /*
            1.2. Call with wrong sharedBufLimitPtr null pointer.
            Expected: GT_BAD_PTR.
        */
        st = cpssDxChPortTxSharedGlobalResourceLimitsGet(dev, NULL,
                                                  &sharedDescLimitPtr);
        UTF_VERIFY_EQUAL1_STRING_MAC(GT_BAD_PTR, st,
                                  "%d, sharedBufLimitPtr = NULL", dev);

        /*
            1.2. Call with wrong sharedDescLimitPtr null pointer.
            Expected: GT_BAD_PTR.
        */
        st = cpssDxChPortTxSharedGlobalResourceLimitsGet(dev, &sharedBufLimitPtr, NULL);
        UTF_VERIFY_EQUAL1_STRING_MAC(GT_BAD_PTR, st,
                                  "%d, sharedDescLimitPtr = NULL", dev);
    }

    /* 2. For not active devices check that function returns non GT_OK.*/
    /* prepare device iterator */
    PRV_TGF_NOT_APPLIC_DEV_RESET_MAC(&dev, UTF_CH1_E | UTF_CH1_DIAMOND_E | UTF_LION_E);

    /* Go over all non active devices. */
    while(GT_OK == prvUtfNextDeviceGet(&dev, GT_FALSE))
    {
        st = cpssDxChPortTxSharedGlobalResourceLimitsGet(dev,
                                  &sharedBufLimitPtr, &sharedDescLimitPtr);
        UTF_VERIFY_EQUAL1_PARAM_MAC(GT_NOT_APPLICABLE_DEVICE, st, dev);
    }

    /* 3.Call function with out of bound value for device id */
    dev = PRV_CPSS_MAX_PP_DEVICES_CNS;

    st = cpssDxChPortTxSharedGlobalResourceLimitsGet(dev,
                                  &sharedBufLimitPtr, &sharedDescLimitPtr);
    UTF_VERIFY_EQUAL1_PARAM_MAC(GT_BAD_PARAM, st, dev);
}

/*----------------------------------------------------------------------------*/
/*
GT_STATUS cpssDxChPortTxWatchdogGet
(
    IN  GT_U8       devNum,
    IN  GT_U8       portNum,
    OUT GT_BOOL     *enablePtr,
    OUT GT_U16      *timeoutPtr
)
*/
UTF_TEST_CASE_MAC(cpssDxChPortTxWatchdogGet)
{
    /*
    ITERATE_DEVICES_PHY_CPU_PORTS (DxCh)
    1.1.1. Call with non-null enablePtr and non-null timeoutPtr. Expected: GT_OK.
    1.1.2. Call with null enablePtr [NULL] and non-null timeoutPtr.
    Expected: GT_BAD_PTR.
    1.1.3. Call with non-null enablePtr and null timeoutPtr [NULL].
    Expected: GT_BAD_PTR.
    */

    GT_STATUS   st      = GT_OK;

    GT_U8       dev;
    GT_U8       port    = PORT_TX_VALID_PHY_PORT_CNS;
    GT_BOOL     enable;
    GT_U16      timeout;

    /* prepare iterator for go over all active devices */
    PRV_TGF_NOT_APPLIC_DEV_RESET_MAC(&dev, UTF_XCAT_E | UTF_LION_E | UTF_XCAT2_E);
    /* 1. Go over all active devices. */
    while (GT_OK == prvUtfNextDeviceGet(&dev, GT_TRUE))
    {
        st = prvUtfNextPhyPortReset(&port, dev);
        UTF_VERIFY_EQUAL0_PARAM_MAC(GT_OK, st);

        /* 1.1. For all active devices go over all available physical ports. */
        while (GT_OK == prvUtfNextPhyPortGet(&port, GT_TRUE))
        {
            /* 1.1.1. Call with non-null enablePtr,
                                non-null timeoutPtr.
               Expected: GT_OK.
            */
            st = cpssDxChPortTxWatchdogGet(dev, port, &enable, &timeout);
            UTF_VERIFY_EQUAL2_PARAM_MAC(GT_OK, st, dev, port);

            /* 1.1.2. Call with null enablePtr [NULL],
                                non-null timeoutPtr.
               Expected: GT_BAD_PTR.
            */
            st = cpssDxChPortTxWatchdogGet(dev, port, NULL, &timeout);
            UTF_VERIFY_EQUAL2_STRING_MAC(GT_BAD_PTR, st, "%d, %d, NULL", dev, port);

            /* 1.1.3. Call with non-null enablePtr,
                                null timeoutPtr [NULL].
               Expected: GT_BAD_PTR.
            */
            st = cpssDxChPortTxWatchdogGet(dev, port, &enable, NULL);
            UTF_VERIFY_EQUAL2_STRING_MAC(GT_BAD_PTR, st, "%d, %d, timeoutPtr =NULL", dev, port);
        }

        st = prvUtfNextPhyPortReset(&port, dev);
        UTF_VERIFY_EQUAL0_PARAM_MAC(GT_OK, st);
        /* 1.2. For all active devices go over all non available physical ports. */
        while (GT_OK == prvUtfNextPhyPortGet(&port, GT_FALSE))
        {
            st = cpssDxChPortTxWatchdogGet(dev, port, &enable, &timeout);
            UTF_VERIFY_EQUAL4_PARAM_MAC(GT_BAD_PARAM, st, dev, port, &enable, &timeout);
        }

        /* 1.3. For active device check that function returns GT_BAD_PARAM */
        /* for out of bound value for port number.                         */
        port = PRV_CPSS_MAX_PP_PORTS_NUM_CNS;

        st = cpssDxChPortTxWatchdogGet(dev, port, &enable, &timeout);
        UTF_VERIFY_EQUAL2_PARAM_MAC(GT_BAD_PARAM, st, dev, port);

        /* 1.4. For active device check that function returns GT_OK        */
        /* for CPU port number.                                            */
        port = CPSS_CPU_PORT_NUM_CNS;

        st = cpssDxChPortTxWatchdogGet(dev, port, &enable, &timeout);
        UTF_VERIFY_EQUAL2_PARAM_MAC(GT_OK, st, dev, port);
    }

    /* 2. For not active devices check that function returns non GT_OK.*/
    port = PORT_TX_VALID_PHY_PORT_CNS;

    /* prepare device iterator */
    PRV_TGF_NOT_APPLIC_DEV_RESET_MAC(&dev, UTF_XCAT_E | UTF_LION_E | UTF_XCAT2_E);
    /* Go over all non active devices. */
    while(GT_OK == prvUtfNextDeviceGet(&dev, GT_FALSE))
    {
        st = cpssDxChPortTxWatchdogGet(dev, port, &enable, &timeout);
        UTF_VERIFY_EQUAL1_PARAM_MAC(GT_NOT_APPLICABLE_DEVICE, st, dev);
    }

    /* 3.Call function with out of bound value for device id */
    dev = PRV_CPSS_MAX_PP_DEVICES_CNS;

    st = cpssDxChPortTxWatchdogGet(dev, port, &enable, &timeout);
    UTF_VERIFY_EQUAL1_PARAM_MAC(GT_BAD_PARAM, st, dev);
}

/*----------------------------------------------------------------------------*/
/*
GT_STATUS cpssDxChPortTxWatchdogEnableSet
(
    IN  GT_U8     devNum,
    IN  GT_U8     portNum,
    IN  GT_BOOL   enable,
    IN  GT_U16    timeout
)
*/
UTF_TEST_CASE_MAC(cpssDxChPortTxWatchdogEnableSet)
{
    /*
    ITERATE_DEVICES_PHY_CPU_PORTS (DxChx)
    1.1.1. Call with enable [GT_FALSE and GT_TRUE] and timeout [50].
    Expected: GT_OK.
    1.1.2. Call cpssDxChPortTxWatchdogGet
     with non-null GT_TRUE and non-null timeoutPtr.
    Expected: GT_OK and the same enable and timeout.
    1.1.3. Call with enable [GT_TRUE] and out of range timeout [0/ 4097].
    Expected: NOT GT_OK.
    1.1.4. Call with enable [GT_FALSE] and out of range timeout [0/ 4097].
    Expected: GT_OK (timeout is ignored when enable = false).
    */

    GT_STATUS   st      = GT_OK;

    GT_U8       dev;
    GT_U8       port    = PORT_TX_VALID_PHY_PORT_CNS;
    GT_BOOL     enable  = GT_TRUE;
    GT_U16      timeout = 50;

    /* prepare iterator for go over all active devices */
    PRV_TGF_NOT_APPLIC_DEV_RESET_MAC(&dev, UTF_XCAT_E | UTF_LION_E | UTF_XCAT2_E);
    /* 1. Go over all active devices. */
    while (GT_OK == prvUtfNextDeviceGet(&dev, GT_TRUE))
    {
        st = prvUtfNextPhyPortReset(&port, dev);
        UTF_VERIFY_EQUAL0_PARAM_MAC(GT_OK, st);

        /* 1.1. For all active devices go over all available physical ports. */
        while (GT_OK == prvUtfNextPhyPortGet(&port, GT_TRUE))
        {
            /* 1.1.1. Call with enable [GT_FALSE and GT_TRUE]
               timeout [50]. Expected: GT_OK.
            */

            /* Call function with [enable==GT_FALSE] timeout = 50*/
            enable = GT_FALSE;
            timeout = 50;

            st = cpssDxChPortTxWatchdogEnableSet(dev, port, enable, timeout);
            UTF_VERIFY_EQUAL4_PARAM_MAC(GT_OK, st, dev, port, enable, timeout);

            /* Call function with [enable==GT_TRUE] timeout = 25*/
            enable = GT_TRUE;
            timeout = 25;

            st = cpssDxChPortTxWatchdogEnableSet(dev, port, enable, timeout);
            UTF_VERIFY_EQUAL4_PARAM_MAC(GT_OK, st, dev, port, enable, timeout);

            /* 1.1.3. Call with enable [GT_TRUE] out of range timeout [0/ 4097].
               Expected: NOT GT_OK.
            */

            /* Call function with enable==GT_TRUE timeout = 0 */
            enable = GT_TRUE;
            timeout = 0;

            st = cpssDxChPortTxWatchdogEnableSet(dev, port, enable, timeout);
            UTF_VERIFY_NOT_EQUAL4_PARAM_MAC(GT_OK, st, dev, port, enable, timeout);

            /* Call function with enable==GT_TRUE timeout = 4097 */
            timeout = 4097;

            st = cpssDxChPortTxWatchdogEnableSet(dev, port, enable, timeout);
            UTF_VERIFY_NOT_EQUAL4_PARAM_MAC(GT_OK, st, dev, port, enable, timeout);

            /* 1.1.4. Call with enable [GT_FALSE] out of range timeout [0 / 4097].
               Expected: GT_OK (timeout is ignored when enable = false)
            */

            /* Call function with enable = GT_FALSE timeout = 0 */
            enable = GT_FALSE;
            timeout = 0;

            st = cpssDxChPortTxWatchdogEnableSet(dev, port, enable, timeout);
            UTF_VERIFY_EQUAL4_PARAM_MAC(GT_OK, st, dev, port, enable, timeout);

            /* Call function with enable == GT_FALSE timeout = 4097 */
            timeout = 4097;

            st = cpssDxChPortTxWatchdogEnableSet(dev, port, enable, timeout);
            UTF_VERIFY_EQUAL4_PARAM_MAC(GT_OK, st, dev, port, enable, timeout);
        }

        enable = GT_TRUE;
        timeout = 65;

        st = prvUtfNextPhyPortReset(&port, dev);
        UTF_VERIFY_EQUAL0_PARAM_MAC(GT_OK, st);
        /* 1.2. For all active devices go over all non available physical ports. */
        while (GT_OK == prvUtfNextPhyPortGet(&port, GT_FALSE))
        {
            /* 1.2.1. Call function for each non-active port */
            /* enable == GT_TRUE    timeout = 65 */
            st = cpssDxChPortTxWatchdogEnableSet(dev, port, enable, timeout);
            UTF_VERIFY_EQUAL4_PARAM_MAC(GT_BAD_PARAM, st, dev, port, enable, timeout);
        }

        /* 1.3. For active device check that function returns GT_BAD_PARAM */
        /* for out of bound value for port number.                         */
        /* enable == GT_TRUE timeout == 65 */
        port = PRV_CPSS_MAX_PP_PORTS_NUM_CNS;

        st = cpssDxChPortTxWatchdogEnableSet(dev, port, enable, timeout);
        UTF_VERIFY_EQUAL2_PARAM_MAC(GT_BAD_PARAM, st, dev, port);

        /* 1.4. For active device check that function returns GT_OK        */
        /* for CPU port number.                                            */
        /* enable == GT_TRUE timeout == 65 */
        port = CPSS_CPU_PORT_NUM_CNS;

        st = cpssDxChPortTxWatchdogEnableSet(dev, port, enable, timeout);
        UTF_VERIFY_EQUAL2_PARAM_MAC(GT_OK, st, dev, port);
    }

    /* 2. For not active devices check that function returns non GT_OK.*/
    port = PORT_TX_VALID_PHY_PORT_CNS;

    /* prepare device iterator */
    PRV_TGF_NOT_APPLIC_DEV_RESET_MAC(&dev, UTF_XCAT_E | UTF_LION_E | UTF_XCAT2_E);
    /* Go over all non active devices. */
    while(GT_OK == prvUtfNextDeviceGet(&dev, GT_FALSE))
    {
        st = cpssDxChPortTxWatchdogEnableSet(dev, port, enable, timeout);
        UTF_VERIFY_EQUAL1_PARAM_MAC(GT_NOT_APPLICABLE_DEVICE, st, dev);
    }

    /* 3.Call function with out of bound value for device id */
    dev = PRV_CPSS_MAX_PP_DEVICES_CNS;
    /* port == 0         */
    /* enable == GT_TRUE */
    /* timeout == 65     */

    st = cpssDxChPortTxWatchdogEnableSet(dev, port, enable, timeout);
    UTF_VERIFY_EQUAL1_PARAM_MAC(GT_BAD_PARAM, st, dev);
}

/*----------------------------------------------------------------------------*/
/*
GT_STATUS cpssDxChPortTxTailDropUcEnableSet
(
    IN GT_U8   devNum,
    IN GT_BOOL enable
)
*/
UTF_TEST_CASE_MAC(cpssDxChPortTxTailDropUcEnableSet)
{
/*
    ITERATE_DEVICES (DxChx)
    1.1. Call with enable [GT_FALSE / GT_TRUE]
    Expected: GT_OK.
    1.2. Call cpssDxChPortTxTailDropUcEnableGet with non NULL enableGet
    Expected: GT_OK and the same enable.
*/
    GT_STATUS   st  = GT_OK;
    GT_U8       dev;

    GT_BOOL enable    = GT_FALSE;
    GT_BOOL enableGet = GT_FALSE;


    /* prepare iterator for go over all active devices */
    PRV_TGF_NOT_APPLIC_DEV_RESET_MAC(&dev, UTF_NONE_FAMILY_E);

    /* 1. Go over all active devices. */
    while (GT_OK == prvUtfNextDeviceGet(&dev, GT_TRUE))
    {
        /*
            1.1. Call with enable [GT_FALSE / GT_TRUE]
            Expected: GT_OK.
        */
        /* call with enable = GT_FALSE */
        enable = GT_FALSE;

        st = cpssDxChPortTxTailDropUcEnableSet(dev, enable);
        UTF_VERIFY_EQUAL2_PARAM_MAC(GT_OK, st, dev, enable);

        /*
            1.2. Call cpssDxChPortTxTailDropUcEnableGet with non NULL enableGet
            Expected: GT_OK and the same enable.
        */
        st = cpssDxChPortTxTailDropUcEnableGet(dev, &enableGet);
        UTF_VERIFY_EQUAL1_STRING_MAC(GT_OK, st, "cpssDxChPortTxTailDropUcEnableGet: %d", dev);
        UTF_VERIFY_EQUAL1_STRING_MAC(enable, enableGet, "got another enable than was set: %d", dev);

        /* call with enable = GT_TRUE */
        enable = GT_TRUE;

        st = cpssDxChPortTxTailDropUcEnableSet(dev, enable);
        UTF_VERIFY_EQUAL2_PARAM_MAC(GT_OK, st, dev, enable);

        /*
            1.2. Call cpssDxChPortTxTailDropUcEnableGet with non NULL enableGet
            Expected: GT_OK and the same enable.
        */
        st = cpssDxChPortTxTailDropUcEnableGet(dev, &enableGet);
        UTF_VERIFY_EQUAL1_STRING_MAC(GT_OK, st, "cpssDxChPortTxTailDropUcEnableGet: %d", dev);
        UTF_VERIFY_EQUAL1_STRING_MAC(enable, enableGet, "got another enable than was set: %d", dev);
    }

    enable = GT_TRUE;

    /* 2. For not active devices check that function returns non GT_OK.*/
    /* prepare device iterator */
    PRV_TGF_NOT_APPLIC_DEV_RESET_MAC(&dev, UTF_NONE_FAMILY_E);

    /* Go over all non active devices. */
    while(GT_OK == prvUtfNextDeviceGet(&dev, GT_FALSE))
    {
        st = cpssDxChPortTxTailDropUcEnableSet(dev, enable);
        UTF_VERIFY_EQUAL1_PARAM_MAC(GT_NOT_APPLICABLE_DEVICE, st, dev);
    }

    /* 3.Call function with out of bound value for device id */
    dev = PRV_CPSS_MAX_PP_DEVICES_CNS;

    st = cpssDxChPortTxTailDropUcEnableSet(dev, enable);
    UTF_VERIFY_EQUAL1_PARAM_MAC(GT_BAD_PARAM, st, dev);
}

/*----------------------------------------------------------------------------*/
/*
GT_STATUS cpssDxChPortTxTailDropUcEnableGet
(
    IN  GT_U8   devNum,
    OUT GT_BOOL *enablePtr
)
*/
UTF_TEST_CASE_MAC(cpssDxChPortTxTailDropUcEnableGet)
{
/*
    ITERATE_DEVICES (DxChx)
    1.1. Call with non-NULL enable
    Expected: GT_OK.
    1.2. Call with enable [NULL]
    Expected: GT_BAD_PTR.
*/
    GT_STATUS   st  = GT_OK;
    GT_U8       dev;

    GT_BOOL enable = GT_FALSE;


    /* prepare iterator for go over all active devices */
    PRV_TGF_NOT_APPLIC_DEV_RESET_MAC(&dev, UTF_NONE_FAMILY_E);

    /* 1. Go over all active devices. */
    while (GT_OK == prvUtfNextDeviceGet(&dev, GT_TRUE))
    {
        /*
            1.1. Call with non-NULL enable
            Expected: GT_OK.
        */
        st = cpssDxChPortTxTailDropUcEnableGet(dev, &enable);
        UTF_VERIFY_EQUAL1_PARAM_MAC(GT_OK, st, dev);

        /*
            1.2. Call with enable [NULL]
            Expected: GT_BAD_PTR.
        */
        st = cpssDxChPortTxTailDropUcEnableGet(dev, NULL);
        UTF_VERIFY_EQUAL1_STRING_MAC(GT_BAD_PTR, st, "%d, enable = NULL", dev);
    }

    /* 2. For not active devices check that function returns non GT_OK.*/
    /* prepare device iterator */
    PRV_TGF_NOT_APPLIC_DEV_RESET_MAC(&dev, UTF_NONE_FAMILY_E);

    /* Go over all non active devices. */
    while(GT_OK == prvUtfNextDeviceGet(&dev, GT_FALSE))
    {
        st = cpssDxChPortTxTailDropUcEnableGet(dev, &enable);
        UTF_VERIFY_EQUAL1_PARAM_MAC(GT_NOT_APPLICABLE_DEVICE, st, dev);
    }

    /* 3.Call function with out of bound value for device id */
    dev = PRV_CPSS_MAX_PP_DEVICES_CNS;

    st = cpssDxChPortTxTailDropUcEnableGet(dev, &enable);
    UTF_VERIFY_EQUAL1_PARAM_MAC(GT_BAD_PARAM, st, dev);
}

/*----------------------------------------------------------------------------*/
/*
GT_STATUS cpssDxChPortTxBufferTailDropEnableSet
(
    IN GT_U8   devNum,
    IN GT_BOOL enable
)
*/
UTF_TEST_CASE_MAC(cpssDxChPortTxBufferTailDropEnableSet)
{
/*
    ITERATE_DEVICES (DxChx)
    1.1. Call with enable [GT_FALSE / GT_TRUE]
    Expected: GT_OK.
    1.2. Call cpssDxChPortTxBufferTailDropEnableGet with non-NULL enableGet
    Expected: GT_OK and the same enable.
*/
    GT_STATUS   st  = GT_OK;
    GT_U8       dev;

    GT_BOOL enable    = GT_FALSE;
    GT_BOOL enableGet = GT_FALSE;


    /* prepare iterator for go over all active devices */
    PRV_TGF_NOT_APPLIC_DEV_RESET_MAC(&dev, UTF_LION_E);

    /* 1. Go over all active devices. */
    while (GT_OK == prvUtfNextDeviceGet(&dev, GT_TRUE))
    {
        /*
            1.1. Call with enable [GT_FALSE / GT_TRUE]
            Expected: GT_OK.
        */
        /* call with enable = GT_FALSE */
        enable = GT_FALSE;

        st = cpssDxChPortTxBufferTailDropEnableSet(dev, enable);
        UTF_VERIFY_EQUAL2_PARAM_MAC(GT_OK, st, dev, enable);
        /*
            1.2. Call cpssDxChPortTxBufferTailDropEnableGet with non-NULL enableGet
            Expected: GT_OK and the same enable.
        */
        st = cpssDxChPortTxBufferTailDropEnableGet(dev, &enableGet);
        UTF_VERIFY_EQUAL1_STRING_MAC(GT_OK, st, "cpssDxChPortTxBufferTailDropEnableGet: %d", dev);
        UTF_VERIFY_EQUAL1_STRING_MAC(enable, enableGet, "cpssDxChPortTxBufferTailDropEnableGet: %d", dev);

        /* call with enable = GT_TRUE */
        enable = GT_TRUE;

        st = cpssDxChPortTxBufferTailDropEnableSet(dev, enable);
        UTF_VERIFY_EQUAL2_PARAM_MAC(GT_OK, st, dev, enable);

        /*
            1.2. Call cpssDxChPortTxBufferTailDropEnableGet with non-NULL enableGet
            Expected: GT_OK and the same enable.
        */
        st = cpssDxChPortTxBufferTailDropEnableGet(dev, &enableGet);
        UTF_VERIFY_EQUAL1_STRING_MAC(GT_OK, st, "cpssDxChPortTxBufferTailDropEnableGet: %d", dev);
        UTF_VERIFY_EQUAL1_STRING_MAC(enable, enableGet, "cpssDxChPortTxBufferTailDropEnableGet: %d", dev);
    }

    enable = GT_FALSE;

    /* 2. For not active devices check that function returns non GT_OK.*/
    /* prepare device iterator */
    PRV_TGF_NOT_APPLIC_DEV_RESET_MAC(&dev, UTF_LION_E);

    /* Go over all non active devices. */
    while(GT_OK == prvUtfNextDeviceGet(&dev, GT_FALSE))
    {
        st = cpssDxChPortTxBufferTailDropEnableSet(dev, enable);
        UTF_VERIFY_EQUAL1_PARAM_MAC(GT_NOT_APPLICABLE_DEVICE, st, dev);
    }

    /* 3.Call function with out of bound value for device id */
    dev = PRV_CPSS_MAX_PP_DEVICES_CNS;

    st = cpssDxChPortTxBufferTailDropEnableSet(dev, enable);
    UTF_VERIFY_EQUAL1_PARAM_MAC(GT_BAD_PARAM, st, dev);
}

/*----------------------------------------------------------------------------*/
/*
GT_STATUS cpssDxChPortTxBufferTailDropEnableGet
(
    IN  GT_U8   devNum,
    OUT GT_BOOL *enablePtr
)
*/
UTF_TEST_CASE_MAC(cpssDxChPortTxBufferTailDropEnableGet)
{
/*
    ITERATE_DEVICES (DxChx)
    1.1. Call with non-NULL enable
    Expected: GT_OK.
    1.2. Call with enable [NULL]
    Expected: GT_BAD_PTR.
*/
    GT_STATUS   st  = GT_OK;
    GT_U8       dev;

    GT_BOOL enable = GT_FALSE;


    /* prepare iterator for go over all active devices */
    PRV_TGF_NOT_APPLIC_DEV_RESET_MAC(&dev, UTF_LION_E);

    /* 1. Go over all active devices. */
    while (GT_OK == prvUtfNextDeviceGet(&dev, GT_TRUE))
    {
        /*
            1.1. Call with non-NULL enable
            Expected: GT_OK.
        */
        st = cpssDxChPortTxBufferTailDropEnableGet(dev, &enable);
        UTF_VERIFY_EQUAL1_PARAM_MAC(GT_OK, st, dev);

        /*
            1.2. Call with enable [NULL]
            Expected: GT_BAD_PTR.
        */
        st = cpssDxChPortTxBufferTailDropEnableGet(dev, NULL);
        UTF_VERIFY_EQUAL1_STRING_MAC(GT_BAD_PTR, st, "%d, enablePtr = NULL", dev);
    }

    /* 2. For not active devices check that function returns non GT_OK.*/
    /* prepare device iterator */
    PRV_TGF_NOT_APPLIC_DEV_RESET_MAC(&dev, UTF_LION_E);

    /* Go over all non active devices. */
    while(GT_OK == prvUtfNextDeviceGet(&dev, GT_FALSE))
    {
        st = cpssDxChPortTxBufferTailDropEnableGet(dev, &enable);
        UTF_VERIFY_EQUAL1_PARAM_MAC(GT_NOT_APPLICABLE_DEVICE, st, dev);
    }

    /* 3.Call function with out of bound value for device id */
    dev = PRV_CPSS_MAX_PP_DEVICES_CNS;

    st = cpssDxChPortTxBufferTailDropEnableGet(dev, &enable);
    UTF_VERIFY_EQUAL1_PARAM_MAC(GT_BAD_PARAM, st, dev);
}

/*----------------------------------------------------------------------------*/
/*
GT_STATUS cpssDxChPortTxBuffersSharingMaxLimitSet
(
    IN  GT_U8       devNum,
    IN  GT_U32      limit
)
*/
UTF_TEST_CASE_MAC(cpssDxChPortTxBuffersSharingMaxLimitSet)
{
/*
    ITERATE_DEVICES (DxCh2 and above)
    1.1. Call with limit [0 / 0xFFF]
    Expected: GT_OK.
    1.2. Call cpssDxChPortTxBuffersSharingMaxLimitGet with non-NULL limitGet
    Expected: GT_OK and the same limit.
    1.3. For Cheetah2 call with out of range limit [0x1000]
    Expected: NOT GT_OK.
    1.4. For Cheetah3 call with out of range limit [0x4000]
    Expected: NOT GT_OK.
*/
    GT_STATUS   st  = GT_OK;
    GT_U8       dev;

    GT_U32                  limit     = 0;
    GT_U32                  limitGet  = 0;
    CPSS_PP_FAMILY_TYPE_ENT devFamily = CPSS_MAX_FAMILY;


    /* prepare iterator for go over all active devices */
    PRV_TGF_NOT_APPLIC_DEV_RESET_MAC(&dev, UTF_CH1_E | UTF_CH1_DIAMOND_E | UTF_LION_E);

    /* 1. Go over all active devices. */
    while (GT_OK == prvUtfNextDeviceGet(&dev, GT_TRUE))
    {
        st = prvUtfDeviceFamilyGet(dev, &devFamily);
        UTF_VERIFY_EQUAL1_STRING_MAC(GT_OK, st, "prvUtfDeviceFamilyGet: %d", dev);

        /*
            1.1. Call with limit [0 / 0xFFF]
            Expected: GT_OK.
        */
        /* call with limit = 0 */
        limit = 0;

        st = cpssDxChPortTxBuffersSharingMaxLimitSet(dev, limit);
        UTF_VERIFY_EQUAL2_PARAM_MAC(GT_OK, st, dev, limit);
        /*
            1.2. Call cpssDxChPortTxBuffersSharingMaxLimitGet with non-NULL limitGet
            Expected: GT_OK and the same limit.
        */
        st = cpssDxChPortTxBuffersSharingMaxLimitGet(dev, &limitGet);
        UTF_VERIFY_EQUAL1_STRING_MAC(GT_OK, st, "cpssDxChPortTxBuffersSharingMaxLimitGet: %d", dev);
        UTF_VERIFY_EQUAL1_STRING_MAC(limit, limitGet, "got another limit than was set: %d", dev);

        /* call with limit = 0xFFF */
        limit = 0xFFF;

        st = cpssDxChPortTxBuffersSharingMaxLimitSet(dev, limit);
        UTF_VERIFY_EQUAL2_PARAM_MAC(GT_OK, st, dev, limit);

        /*
            1.2. Call cpssDxChPortTxBuffersSharingMaxLimitGet with non-NULL limitGet
            Expected: GT_OK and the same limit.
        */
        st = cpssDxChPortTxBuffersSharingMaxLimitGet(dev, &limitGet);
        UTF_VERIFY_EQUAL1_STRING_MAC(GT_OK, st, "cpssDxChPortTxBuffersSharingMaxLimitGet: %d", dev);
        UTF_VERIFY_EQUAL1_STRING_MAC(limit, limitGet, "got another limit than was set: %d", dev);

        /*
            1.3. For Cheetah2 call with out of range limit [0x1000]
            Expected: NOT GT_OK.
        */
        if (CPSS_PP_FAMILY_CHEETAH2_E  == devFamily)
        {
            limit = 0x1000;

            st = cpssDxChPortTxBuffersSharingMaxLimitSet(dev, limit);
            UTF_VERIFY_NOT_EQUAL2_PARAM_MAC(GT_OK, st, dev, limit);

            limit = 0;
        }

        /*
            1.4. For Cheetah3 call with out of range limit [0x4000]
            Expected: NOT GT_OK.
        */
        if (CPSS_PP_FAMILY_CHEETAH3_E  >= devFamily)
        {
            limit = 0x4000;

            st = cpssDxChPortTxBuffersSharingMaxLimitSet(dev, limit);
            UTF_VERIFY_NOT_EQUAL2_PARAM_MAC(GT_OK, st, dev, limit);
        }
    }

    limit = 0;

    /* 2. For not active devices check that function returns non GT_OK.*/
    /* prepare device iterator */
    PRV_TGF_NOT_APPLIC_DEV_RESET_MAC(&dev, UTF_CH1_E | UTF_CH1_DIAMOND_E | UTF_LION_E);

    /* Go over all non active devices. */
    while(GT_OK == prvUtfNextDeviceGet(&dev, GT_FALSE))
    {
        st = cpssDxChPortTxBuffersSharingMaxLimitSet(dev, limit);
        UTF_VERIFY_EQUAL1_PARAM_MAC(GT_NOT_APPLICABLE_DEVICE, st, dev);
    }

    /* 3.Call function with out of bound value for device id */
    dev = PRV_CPSS_MAX_PP_DEVICES_CNS;

    st = cpssDxChPortTxBuffersSharingMaxLimitSet(dev, limit);
    UTF_VERIFY_EQUAL1_PARAM_MAC(GT_BAD_PARAM, st, dev);
}

/*----------------------------------------------------------------------------*/
/*
GT_STATUS cpssDxChPortTxBuffersSharingMaxLimitGet
(
    IN  GT_U8       devNum,
    OUT GT_U32      *limitPtr
)
*/
UTF_TEST_CASE_MAC(cpssDxChPortTxBuffersSharingMaxLimitGet)
{
/*
    ITERATE_DEVICES (DxCh2 and above)
    1.1. Call with non-NULL limit
    Expected: GT_OK.
    1.2. Call with limit [NULL]
    Expected: GT_BAD_PTR
*/
    GT_STATUS   st  = GT_OK;
    GT_U8       dev;

    GT_U32  limit = 0;

    /* prepare iterator for go over all active devices */
    PRV_TGF_NOT_APPLIC_DEV_RESET_MAC(&dev, UTF_CH1_E | UTF_CH1_DIAMOND_E | UTF_LION_E);

    /* 1. Go over all active devices. */
    while (GT_OK == prvUtfNextDeviceGet(&dev, GT_TRUE))
    {
        /*
            1.1. Call with non-NULL limit
            Expected: GT_OK.
        */
        st = cpssDxChPortTxBuffersSharingMaxLimitGet(dev, &limit);
        UTF_VERIFY_EQUAL1_PARAM_MAC(GT_OK, st, dev);

        /*
            1.2. Call with limit [NULL]
            Expected: GT_BAD_PTR
        */
        st = cpssDxChPortTxBuffersSharingMaxLimitGet(dev, NULL);
        UTF_VERIFY_EQUAL1_STRING_MAC(GT_BAD_PTR, st, "%d, limitPtr = NULL", dev);
    }

    /* 2. For not active devices check that function returns non GT_OK.*/
    /* prepare device iterator */
    PRV_TGF_NOT_APPLIC_DEV_RESET_MAC(&dev, UTF_CH1_E | UTF_CH1_DIAMOND_E | UTF_LION_E);

    /* Go over all non active devices. */
    while(GT_OK == prvUtfNextDeviceGet(&dev, GT_FALSE))
    {
        st = cpssDxChPortTxBuffersSharingMaxLimitGet(dev, &limit);
        UTF_VERIFY_EQUAL1_PARAM_MAC(GT_NOT_APPLICABLE_DEVICE, st, dev);
    }

    /* 3.Call function with out of bound value for device id */
    dev = PRV_CPSS_MAX_PP_DEVICES_CNS;

    st = cpssDxChPortTxBuffersSharingMaxLimitGet(dev, &limit);
    UTF_VERIFY_EQUAL1_PARAM_MAC(GT_BAD_PARAM, st, dev);
}

/*----------------------------------------------------------------------------*/
/*
GT_STATUS cpssDxChPortTxDp1SharedEnableSet
(
    IN  GT_U8       devNum,
    IN  GT_BOOL     enable
)
*/
UTF_TEST_CASE_MAC(cpssDxChPortTxDp1SharedEnableSet)
{
/*
    ITERATE_DEVICES (DxCh3 and above)
    1.1. Call with enable [GT_FALSE / GT_TRUE].
    Expected: GT_OK.
    1.2. Call cpssDxChPortTxDp1SharedEnableGet with non-NULL enablePtr.
    Expected: GT_OK and the same enable.
*/
    GT_STATUS   st = GT_OK;
    GT_U8       dev;

    GT_BOOL enable    = GT_FALSE;
    GT_BOOL enableGet = GT_FALSE;


    /* prepare iterator for go over all active devices */
    PRV_TGF_NOT_APPLIC_DEV_RESET_MAC(&dev, UTF_CH1_E | UTF_CH1_DIAMOND_E | UTF_CH2_E | UTF_LION_E);

    /* 1. Go over all active devices. */
    while (GT_OK == prvUtfNextDeviceGet(&dev, GT_TRUE))
    {
        /*
            1.1. Call with enable [GT_FALSE / GT_TRUE].
            Expected: GT_OK.
        */

        /* Call with enable [GT_FALSE] */
        enable = GT_FALSE;

        st = cpssDxChPortTxDp1SharedEnableSet(dev, enable);
        UTF_VERIFY_EQUAL2_PARAM_MAC(GT_OK, st, dev, enable);

        /*
            1.2. Call cpssDxChPortTxDp1SharedEnableGet with non-NULL enablePtr.
            Expected: GT_OK and the same enable.
        */
        st = cpssDxChPortTxDp1SharedEnableGet(dev, &enableGet);
        UTF_VERIFY_EQUAL1_STRING_MAC(GT_OK, st,
                   "cpssDxChPortTxDp1SharedEnableGet: %d", dev);

        /* Verifying values */
        UTF_VERIFY_EQUAL1_STRING_MAC(enable, enableGet,
                   "cpssDxChPortTxDp1SharedEnableGet: %d", dev);

        /* Call with enable [GT_TRUE] */
        enable = GT_TRUE;

        st = cpssDxChPortTxDp1SharedEnableSet(dev, enable);
        UTF_VERIFY_EQUAL2_PARAM_MAC(GT_OK, st, dev, enable);

        /*
            1.2. Call cpssDxChPortTxDp1SharedEnableGet with non-NULL enablePtr.
            Expected: GT_OK and the same enable.
        */
        st = cpssDxChPortTxDp1SharedEnableGet(dev, &enableGet);
        UTF_VERIFY_EQUAL1_STRING_MAC(GT_OK, st,
                   "cpssDxChPortTxDp1SharedEnableGet: %d", dev);

        /* Verifying values */
        UTF_VERIFY_EQUAL1_STRING_MAC(enable, enableGet,
                   "cpssDxChPortTxDp1SharedEnableGet: %d", dev);
    }

    enable = GT_FALSE;

    /* 2. For not active devices check that function returns non GT_OK.*/
    /* prepare device iterator */
    PRV_TGF_NOT_APPLIC_DEV_RESET_MAC(&dev, UTF_CH1_E | UTF_CH1_DIAMOND_E | UTF_CH2_E | UTF_LION_E);

    /* Go over all non active devices. */
    while(GT_OK == prvUtfNextDeviceGet(&dev, GT_FALSE))
    {
        st = cpssDxChPortTxDp1SharedEnableSet(dev, enable);
        UTF_VERIFY_EQUAL1_PARAM_MAC(GT_NOT_APPLICABLE_DEVICE, st, dev);
    }

    /* 3.Call function with out of bound value for device id */
    dev = PRV_CPSS_MAX_PP_DEVICES_CNS;

    st = cpssDxChPortTxDp1SharedEnableSet(dev, enable);
    UTF_VERIFY_EQUAL1_PARAM_MAC(GT_BAD_PARAM, st, dev);
}

/*----------------------------------------------------------------------------*/
/*
GT_STATUS cpssDxChPortTxDp1SharedEnableGet
(
    IN  GT_U8       devNum,
    OUT GT_BOOL     *enablePtr
)
*/
UTF_TEST_CASE_MAC(cpssDxChPortTxDp1SharedEnableGet)
{
/*
    ITERATE_DEVICES (DxCh3 and above)
    1.1. Call with non-NULL enablePtr.
    Expected: GT_OK.
    1.2. Call with enablePtr [NULL].
    Expected: GT_BAD_PTR.
*/
    GT_STATUS   st = GT_OK;
    GT_U8       dev;

    GT_BOOL enable = GT_FALSE;


    /* prepare iterator for go over all active devices */
    PRV_TGF_NOT_APPLIC_DEV_RESET_MAC(&dev, UTF_CH1_E | UTF_CH1_DIAMOND_E | UTF_CH2_E | UTF_LION_E);

    /* 1. Go over all active devices. */
    while (GT_OK == prvUtfNextDeviceGet(&dev, GT_TRUE))
    {
        /*
            1.1. Call with non-NULL enablePtr.
            Expected: GT_OK.
        */
        st = cpssDxChPortTxDp1SharedEnableGet(dev, &enable);
        UTF_VERIFY_EQUAL1_PARAM_MAC(GT_OK, st, dev);

        /*
            1.2. Call with enablePtr [NULL].
            Expected: GT_BAD_PTR.
        */
        st = cpssDxChPortTxDp1SharedEnableGet(dev, NULL);
        UTF_VERIFY_EQUAL1_STRING_MAC(GT_BAD_PTR, st, "%d, enablePtr = NULL", dev);
    }

    /* 2. For not active devices check that function returns non GT_OK.*/
    /* prepare device iterator */
    PRV_TGF_NOT_APPLIC_DEV_RESET_MAC(&dev, UTF_CH1_E | UTF_CH1_DIAMOND_E | UTF_CH2_E | UTF_LION_E);

    /* Go over all non active devices. */
    while(GT_OK == prvUtfNextDeviceGet(&dev, GT_FALSE))
    {
        st = cpssDxChPortTxDp1SharedEnableGet(dev, &enable);
        UTF_VERIFY_EQUAL1_PARAM_MAC(GT_NOT_APPLICABLE_DEVICE, st, dev);
    }

    /* 3.Call function with out of bound value for device id */
    dev = PRV_CPSS_MAX_PP_DEVICES_CNS;

    st = cpssDxChPortTxDp1SharedEnableGet(dev, &enable);
    UTF_VERIFY_EQUAL1_PARAM_MAC(GT_BAD_PARAM, st, dev);
}


/*----------------------------------------------------------------------------*/
/*
GT_STATUS cpssDxChPortTxTcSharedProfileEnableSet
(
    IN  GT_U8                                   devNum,
    IN  CPSS_PORT_TX_DROP_PROFILE_SET_ENT       pfSet,
    IN  GT_U8                                   tc,
    IN  GT_BOOL                                 enable
)
*/
UTF_TEST_CASE_MAC(cpssDxChPortTxTcSharedProfileEnableSet)
{
/*
    ITERATE_DEVICES (DxCh3 and above)
    1.1. Call with pfSet [CPSS_PORT_TX_DROP_PROFILE_8_E /
                          CPSS_PORT_TX_DROP_PROFILE_1_E],
                   tc [0 / 7]
                   and enable [GT_FALSE / GT_TRUE].
    Expected: GT_OK.
    1.2. Call cpssDxChPortTxTcSharedProfileEnableGet with the same params
                                                          and non-NULL enablePtr
    Expected: GT_OK and the same enable.
    1.3. Call with wrong enum values pfSet
                   and othe params from 1.1.
    Expected:  GT_BAD_PARAM.
    1.4. Call with out of range tc [8]
                   and othe params from 1.1.
    Expected:  GT_BAD_PARAM.
*/
    GT_STATUS   st = GT_OK;
    GT_U8       dev;

    CPSS_PORT_TX_DROP_PROFILE_SET_ENT   pfSet     = CPSS_PORT_TX_DROP_PROFILE_1_E;
    GT_U8                               tc        = 0;
    CPSS_PORT_TX_SHARED_DP_MODE_ENT     enable    = CPSS_PORT_TX_SHARED_DP_MODE_DISABLE_E;
    CPSS_PORT_TX_SHARED_DP_MODE_ENT     enableGet = CPSS_PORT_TX_SHARED_DP_MODE_DISABLE_E;


    /* prepare iterator for go over all active devices */
    PRV_TGF_NOT_APPLIC_DEV_RESET_MAC(&dev, UTF_CH1_E | UTF_CH1_DIAMOND_E | UTF_CH2_E);

    /* 1. Go over all active devices. */
    while (GT_OK == prvUtfNextDeviceGet(&dev, GT_TRUE))
    {
        /*
            1.1. Call with pfSet [CPSS_PORT_TX_DROP_PROFILE_8_E /
                                  CPSS_PORT_TX_DROP_PROFILE_1_E],
                           tc [0 / 7]
                           and enable [GT_FALSE / GT_TRUE].
            Expected: GT_OK.
        */

        /* Call with pfSet [CPSS_PORT_TX_DROP_PROFILE_8_E] */
        pfSet  = CPSS_PORT_TX_DROP_PROFILE_8_E;
        tc     = 0;
        enable = CPSS_PORT_TX_SHARED_DP_MODE_DISABLE_E;

        st = cpssDxChPortTxTcSharedProfileEnableSet(dev, pfSet, tc, enable);
        UTF_VERIFY_EQUAL4_PARAM_MAC(GT_OK, st, dev, pfSet, tc, enable);

        /*
            1.2. Call cpssDxChPortTxTcSharedProfileEnableGet with the same params
                                                                  and non-NULL enablePtr
            Expected: GT_OK and the same enable.
        */
        st = cpssDxChPortTxTcSharedProfileEnableGet(dev, pfSet, tc, &enableGet);
        UTF_VERIFY_EQUAL3_STRING_MAC(GT_OK, st,
                   "cpssDxChPortTxTcSharedProfileEnableGet: %d", dev, pfSet, tc);

        /* Verifying values */
        UTF_VERIFY_EQUAL1_STRING_MAC(enable, enableGet,
                   "cpssDxChPortTxDp1SharedEnableGet: %d", dev);

        /* Call with pfSet [CPSS_PORT_TX_DROP_PROFILE_1_E] */
        pfSet  = CPSS_PORT_TX_DROP_PROFILE_1_E;
        tc     = 7;
        enable = CPSS_PORT_TX_SHARED_DP_MODE_ALL_E;

        st = cpssDxChPortTxTcSharedProfileEnableSet(dev, pfSet, tc, enable);
        UTF_VERIFY_EQUAL4_PARAM_MAC(GT_OK, st, dev, pfSet, tc, enable);

        /*
            1.2. Call cpssDxChPortTxTcSharedProfileEnableGet with the same params
                                                                  and non-NULL enablePtr
            Expected: GT_OK and the same enable.
        */
        st = cpssDxChPortTxTcSharedProfileEnableGet(dev, pfSet, tc, &enableGet);
        UTF_VERIFY_EQUAL3_STRING_MAC(GT_OK, st,
                   "cpssDxChPortTxTcSharedProfileEnableGet: %d", dev, pfSet, tc);

        /* Verifying values */
        UTF_VERIFY_EQUAL1_STRING_MAC(enable, enableGet,
                   "cpssDxChPortTxDp1SharedEnableGet: %d", dev);

        /*
            1.3. Call with wrong enum values pfSet and othe params from 1.1.
            Expected:  GT_BAD_PARAM.
        */
        UTF_ENUMS_CHECK_MAC(cpssDxChPortTxTcSharedProfileEnableSet
                            (dev, pfSet, tc, enable),
                            pfSet);

        /*
            1.4. Call with out of range tc [8]
                           and othe params from 1.1.
            Expected:  GT_BAD_PARAM.
        */
        tc = 8;

        st = cpssDxChPortTxTcSharedProfileEnableSet(dev, pfSet, tc, enable);
        UTF_VERIFY_EQUAL2_STRING_MAC(GT_BAD_PARAM, st, "%d, tc = %d", dev, tc);
    }

    pfSet  = CPSS_PORT_TX_DROP_PROFILE_8_E;
    tc     = 0;
    enable = CPSS_PORT_TX_SHARED_DP_MODE_DISABLE_E;

    /* 2. For not active devices check that function returns non GT_OK.*/
    /* prepare device iterator */
    PRV_TGF_NOT_APPLIC_DEV_RESET_MAC(&dev, UTF_CH1_E | UTF_CH1_DIAMOND_E | UTF_CH2_E);

    /* Go over all non active devices. */
    while(GT_OK == prvUtfNextDeviceGet(&dev, GT_FALSE))
    {
        st = cpssDxChPortTxTcSharedProfileEnableSet(dev, pfSet, tc, enable);
        UTF_VERIFY_EQUAL1_PARAM_MAC(GT_NOT_APPLICABLE_DEVICE, st, dev);
    }

    /* 3.Call function with out of bound value for device id */
    dev = PRV_CPSS_MAX_PP_DEVICES_CNS;

    st = cpssDxChPortTxTcSharedProfileEnableSet(dev, pfSet, tc, enable);
    UTF_VERIFY_EQUAL1_PARAM_MAC(GT_BAD_PARAM, st, dev);
}


/*----------------------------------------------------------------------------*/
/*
GT_STATUS cpssDxChPortTxTcSharedProfileEnableGet
(
    IN  GT_U8                                   devNum,
    IN  CPSS_PORT_TX_DROP_PROFILE_SET_ENT       pfSet,
    IN  GT_U8                                   tc,
    OUT GT_BOOL                                 *enablePtr
)
*/
UTF_TEST_CASE_MAC(cpssDxChPortTxTcSharedProfileEnableGet)
{
/*
    ITERATE_DEVICES (DxCh3 and above)
    1.1. Call with pfSet [CPSS_PORT_TX_DROP_PROFILE_8_E /
                          CPSS_PORT_TX_DROP_PROFILE_1_E],
                   tc [0 / 7]
                   and non-NULL enablePtr.
    Expected: GT_OK.
    1.2. Call with wrong enum values pfSet
                   and othe params from 1.1.
    Expected:  GT_BAD_PARAM.
    1.3. Call with out of range tc [8]
                   and othe params from 1.1.
    Expected:  GT_BAD_PARAM.
    1.4. Call with enablePtr [NULL]
                   and othe params from 1.1.
    Expected: GT_BAD_PTR.
*/
    GT_STATUS   st = GT_OK;
    GT_U8       dev;

    CPSS_PORT_TX_DROP_PROFILE_SET_ENT   pfSet  = CPSS_PORT_TX_DROP_PROFILE_1_E;
    GT_U8                               tc     = 0;
    CPSS_PORT_TX_SHARED_DP_MODE_ENT     enable = CPSS_PORT_TX_SHARED_DP_MODE_DISABLE_E;


    /* prepare iterator for go over all active devices */
    PRV_TGF_NOT_APPLIC_DEV_RESET_MAC(&dev, UTF_CH1_E | UTF_CH1_DIAMOND_E | UTF_CH2_E);

    /* 1. Go over all active devices. */
    while (GT_OK == prvUtfNextDeviceGet(&dev, GT_TRUE))
    {
        /*
            1.1. Call with pfSet [CPSS_PORT_TX_DROP_PROFILE_8_E /
                                  CPSS_PORT_TX_DROP_PROFILE_1_E],
                           tc [0 / 7]
                           and non-NULL enablePtr.
            Expected: GT_OK.
        */

        /* Call with pfSet [CPSS_PORT_TX_DROP_PROFILE_8_E] */
        pfSet = CPSS_PORT_TX_DROP_PROFILE_8_E;
        tc    = 0;

        st = cpssDxChPortTxTcSharedProfileEnableGet(dev, pfSet, tc, &enable);
        UTF_VERIFY_EQUAL3_PARAM_MAC(GT_OK, st, dev, pfSet, tc);

        /* Call with pfSet [CPSS_PORT_TX_DROP_PROFILE_1_E] */
        pfSet = CPSS_PORT_TX_DROP_PROFILE_1_E;
        tc    = 7;

        st = cpssDxChPortTxTcSharedProfileEnableGet(dev, pfSet, tc, &enable);
        UTF_VERIFY_EQUAL3_PARAM_MAC(GT_OK, st, dev, pfSet, tc);

        /*
            1.2. Call with wrong enum values pfSet and othe params from 1.1.
            Expected:  GT_BAD_PARAM.
        */
        UTF_ENUMS_CHECK_MAC(cpssDxChPortTxTcSharedProfileEnableGet
                            (dev, pfSet, tc, &enable),
                            pfSet);

        /*
            1.3. Call with out of range tc [8]
                           and othe params from 1.1.
            Expected:  GT_BAD_PARAM.
        */
        tc = 8;

        st = cpssDxChPortTxTcSharedProfileEnableGet(dev, pfSet, tc, &enable);
        UTF_VERIFY_EQUAL2_STRING_MAC(GT_BAD_PARAM, st, "%d, tc = %d", dev, tc);

        tc = 0;

        /*
            1.4. Call with enablePtr [NULL]
                           and othe params from 1.1.
            Expected: GT_BAD_PTR.
        */
        st = cpssDxChPortTxTcSharedProfileEnableGet(dev, pfSet, tc, NULL);
        UTF_VERIFY_EQUAL1_STRING_MAC(GT_BAD_PTR, st, "%d, enablePtr = NULL", dev);
    }

    pfSet = CPSS_PORT_TX_DROP_PROFILE_8_E;
    tc    = 0;

    /* 2. For not active devices check that function returns non GT_OK.*/
    /* prepare device iterator */
    PRV_TGF_NOT_APPLIC_DEV_RESET_MAC(&dev, UTF_CH1_E | UTF_CH1_DIAMOND_E | UTF_CH2_E);

    /* Go over all non active devices. */
    while(GT_OK == prvUtfNextDeviceGet(&dev, GT_FALSE))
    {
        st = cpssDxChPortTxTcSharedProfileEnableGet(dev, pfSet, tc, &enable);
        UTF_VERIFY_EQUAL1_PARAM_MAC(GT_NOT_APPLICABLE_DEVICE, st, dev);
    }

    /* 3.Call function with out of bound value for device id */
    dev = PRV_CPSS_MAX_PP_DEVICES_CNS;

    st = cpssDxChPortTxTcSharedProfileEnableGet(dev, pfSet, tc, &enable);
    UTF_VERIFY_EQUAL1_PARAM_MAC(GT_BAD_PARAM, st, dev);
}

/*----------------------------------------------------------------------------*/
/*
GT_STATUS cpssDxChPortTxMcastPcktDescrLimitGet
(
    IN  GT_U8   devNum,
    OUT GT_U32  *mcastMaxDescNumPtr
)
*/
UTF_TEST_CASE_MAC(cpssDxChPortTxMcastPcktDescrLimitGet)
{
/*
    ITERATE_DEVICES (DxChx)
    1.1. Call with non-NULL mcastMaxDescNumPtr.
    Expected: GT_OK.
    1.2. Call with mcastMaxDescNumPtr [NULL]
    Expected: GT_BAD_PTR.
*/
    GT_STATUS   st  = GT_OK;
    GT_U8       dev;
    GT_U32      mcastMaxDescNumPtr;

    /* prepare iterator for go over all active devices */
    PRV_TGF_NOT_APPLIC_DEV_RESET_MAC(&dev, UTF_NONE_FAMILY_E);

    /* 1. Go over all active devices. */
    while (GT_OK == prvUtfNextDeviceGet(&dev, GT_TRUE))
    {
        /*
            1.1. Call with non-NULL mcastMaxDescNumPtr
            Expected: GT_OK.
        */
        st = cpssDxChPortTxMcastPcktDescrLimitGet(dev, &mcastMaxDescNumPtr);
        UTF_VERIFY_EQUAL1_PARAM_MAC(GT_OK, st, dev);

        /*
            1.2. Call with mcastMaxDescNumPtr [NULL]
            Expected: GT_BAD_PTR.
        */
        st = cpssDxChPortTxMcastPcktDescrLimitGet(dev, NULL);
        UTF_VERIFY_EQUAL1_STRING_MAC(GT_BAD_PTR, st, "%d, mcastMaxDescNumPtr = NULL", dev);
    }

    /* 2. For not active devices check that function returns non GT_OK.*/
    /* prepare device iterator */
    PRV_TGF_NOT_APPLIC_DEV_RESET_MAC(&dev, UTF_NONE_FAMILY_E);

    /* Go over all non active devices. */
    while(GT_OK == prvUtfNextDeviceGet(&dev, GT_FALSE))
    {
        st = cpssDxChPortTxMcastPcktDescrLimitGet(dev, &mcastMaxDescNumPtr);
        UTF_VERIFY_EQUAL1_PARAM_MAC(GT_NOT_APPLICABLE_DEVICE, st, dev);
    }

    /* 3.Call function with out of bound value for device id */
    dev = PRV_CPSS_MAX_PP_DEVICES_CNS;

    st = cpssDxChPortTxMcastPcktDescrLimitGet(dev, &mcastMaxDescNumPtr);
    UTF_VERIFY_EQUAL1_PARAM_MAC(GT_BAD_PARAM, st, dev);
}

/*----------------------------------------------------------------------------*/
/*
GT_STATUS cpssDxChPortTxRandomTailDropEnableGet
(
    IN  GT_U8   devNum,
    OUT GT_BOOL *enablePtr
)
*/
UTF_TEST_CASE_MAC(cpssDxChPortTxRandomTailDropEnableGet)
{
/*
    ITERATE_DEVICES (DxCh3 and above)
    1.1. Call with non-NULL enablePtr.
    Expected: GT_OK.
    1.2. Call with enablePtr [NULL]
    Expected: GT_BAD_PTR.
*/
    GT_STATUS   st  = GT_OK;
    GT_U8       dev;
    GT_BOOL     enablePtr;

    /* prepare iterator for go over all active devices */
    PRV_TGF_NOT_APPLIC_DEV_RESET_MAC(&dev, UTF_CH1_E | UTF_CH1_DIAMOND_E | UTF_CH2_E | UTF_LION_E);

    /* 1. Go over all active devices. */
    while (GT_OK == prvUtfNextDeviceGet(&dev, GT_TRUE))
    {
        /*
            1.1. Call with non-NULL enablePtr
            Expected: GT_OK.
        */
        st = cpssDxChPortTxRandomTailDropEnableGet(dev, &enablePtr);
        UTF_VERIFY_EQUAL1_PARAM_MAC(GT_OK, st, dev);

        /*
            1.2. Call with enablePtr [NULL]
            Expected: GT_BAD_PTR.
        */
        st = cpssDxChPortTxRandomTailDropEnableGet(dev, NULL);
        UTF_VERIFY_EQUAL1_STRING_MAC(GT_BAD_PTR, st, "%d, enablePtr = NULL", dev);
    }

    /* 2. For not active devices check that function returns non GT_OK.*/
    /* prepare device iterator */
    PRV_TGF_NOT_APPLIC_DEV_RESET_MAC(&dev, UTF_CH1_E | UTF_CH1_DIAMOND_E | UTF_CH2_E | UTF_LION_E);

    /* Go over all non active devices. */
    while(GT_OK == prvUtfNextDeviceGet(&dev, GT_FALSE))
    {
        st = cpssDxChPortTxRandomTailDropEnableGet(dev, &enablePtr);
        UTF_VERIFY_EQUAL1_PARAM_MAC(GT_NOT_APPLICABLE_DEVICE, st, dev);
    }

    /* 3.Call function with out of bound value for device id */
    dev = PRV_CPSS_MAX_PP_DEVICES_CNS;

    st = cpssDxChPortTxRandomTailDropEnableGet(dev, &enablePtr);
    UTF_VERIFY_EQUAL1_PARAM_MAC(GT_BAD_PARAM, st, dev);
}

/*----------------------------------------------------------------------------*/
/*
GT_STATUS cpssDxChPortTxRandomTailDropEnableSet
(
    IN GT_U8   devNum,
    IN GT_BOOL enable
)
*/
UTF_TEST_CASE_MAC(cpssDxChPortTxRandomTailDropEnableSet)
{
/*
    ITERATE_DEVICES (DxCh3 and above)
    1.1. Call with enable [GT_FALSE / GT_TRUE].
    Expected: GT_OK.
    1.2. Call cpssDxChPortTxRandomTailDropEnableGet with non-NULL enablePtr.
    Expected: GT_OK and the same enable.
*/
    GT_STATUS   st        = GT_OK;
    GT_U8       dev;
    GT_BOOL     enable    = GT_FALSE;
    GT_BOOL     enableGet = GT_FALSE;


    /* prepare iterator for go over all active devices */
    PRV_TGF_NOT_APPLIC_DEV_RESET_MAC(&dev, UTF_CH1_E | UTF_CH1_DIAMOND_E | UTF_CH2_E | UTF_LION_E);

    /* 1. Go over all active devices. */
    while (GT_OK == prvUtfNextDeviceGet(&dev, GT_TRUE))
    {
        /*
            1.1. Call with enable [GT_FALSE / GT_TRUE].
            Expected: GT_OK.
        */
        /* Call with enable [GT_FALSE] */
        enable = GT_FALSE;

        st = cpssDxChPortTxRandomTailDropEnableSet(dev, enable);
        UTF_VERIFY_EQUAL2_PARAM_MAC(GT_OK, st, dev, enable);

        /*
            1.2. Call cpssDxChPortTxRandomTailDropEnableGet with non-NULL enablePtr.
            Expected: GT_OK and the same enable.
        */
        st = cpssDxChPortTxRandomTailDropEnableGet(dev, &enableGet);
        UTF_VERIFY_EQUAL1_STRING_MAC(GT_OK, st,
                   "cpssDxChPortTxRandomTailDropEnableGet: %d", dev);

        /* Verifying values */
        UTF_VERIFY_EQUAL1_STRING_MAC(enable, enableGet,
                   "cpssDxChPortTxRandomTailDropEnableGet: %d", dev);

        /* Call with enable [GT_TRUE] */
        enable = GT_TRUE;

        st = cpssDxChPortTxRandomTailDropEnableSet(dev, enable);
        UTF_VERIFY_EQUAL2_PARAM_MAC(GT_OK, st, dev, enable);

        /*
            1.2. Call cpssDxChPortTxRandomTailDropEnableGet with non-NULL enablePtr.
            Expected: GT_OK and the same enable.
        */
        st = cpssDxChPortTxRandomTailDropEnableGet(dev, &enableGet);
        UTF_VERIFY_EQUAL1_STRING_MAC(GT_OK, st,
                   "cpssDxChPortTxRandomTailDropEnableGet: %d", dev);

        /* Verifying values */
        UTF_VERIFY_EQUAL1_STRING_MAC(enable, enableGet,
                   "cpssDxChPortTxRandomTailDropEnableGet: %d", dev);
    }

    enable = GT_FALSE;

    /* 2. For not active devices check that function returns non GT_OK.*/
    /* prepare device iterator */
    PRV_TGF_NOT_APPLIC_DEV_RESET_MAC(&dev, UTF_CH1_E | UTF_CH1_DIAMOND_E | UTF_CH2_E | UTF_LION_E);

    /* Go over all non active devices. */
    while(GT_OK == prvUtfNextDeviceGet(&dev, GT_FALSE))
    {
        st = cpssDxChPortTxRandomTailDropEnableSet(dev, enable);
        UTF_VERIFY_EQUAL1_PARAM_MAC(GT_NOT_APPLICABLE_DEVICE, st, dev);
    }

    /* 3.Call function with out of bound value for device id */
    dev = PRV_CPSS_MAX_PP_DEVICES_CNS;

    st = cpssDxChPortTxRandomTailDropEnableSet(dev, enable);
    UTF_VERIFY_EQUAL1_PARAM_MAC(GT_BAD_PARAM, st, dev);
}

/*----------------------------------------------------------------------------*/
/*
GT_STATUS cpssDxChPortTxShaperGlobalParamsGet
(
    IN    GT_U8          devNum,
    OUT   GT_U32         *xgPortsTokensRatePtr,
    OUT   GT_U32         *gigPortsTokensRatePtr,
    OUT   GT_U32         *gigPortsSlowRateRatioPtr
)
*/
UTF_TEST_CASE_MAC(cpssDxChPortTxShaperGlobalParamsGet)
{
/*
    ITERATE_DEVICES (DxCh3 and above)
    1.1. Call with non-NULL pointers.
    Expected: GT_OK.
    1.2. Call with  null pointers.
    Expected: GT_BAD_PTR.
*/
    GT_STATUS   st  = GT_OK;
    GT_U8       dev;
    GT_U32      xgPortsTokensRatePtr;
    GT_U32      gigPortsTokensRatePtr;
    GT_U32      gigPortsSlowRateRatioPtr;

    /* prepare iterator for go over all active devices */
    PRV_TGF_NOT_APPLIC_DEV_RESET_MAC(&dev, UTF_LION_E);

    /* 1. Go over all active devices. */
    while (GT_OK == prvUtfNextDeviceGet(&dev, GT_TRUE))
    {
        /*
            1.1. Call with non-NULL pointers.
            Expected: GT_OK.
        */
        st = cpssDxChPortTxShaperGlobalParamsGet(dev, &xgPortsTokensRatePtr,
                              &gigPortsTokensRatePtr, &gigPortsSlowRateRatioPtr);
        UTF_VERIFY_EQUAL1_PARAM_MAC(GT_OK, st, dev);

        /*
            1.2. Call with  null pointers.
            Expected: GT_BAD_PTR.
        */
        st = cpssDxChPortTxShaperGlobalParamsGet(dev, NULL,
                              &gigPortsTokensRatePtr, &gigPortsSlowRateRatioPtr);
        UTF_VERIFY_EQUAL1_STRING_MAC(GT_BAD_PTR, st,
                                     "%d, xgPortsTokensRatePtr = NULL", dev);

        st = cpssDxChPortTxShaperGlobalParamsGet(dev, &xgPortsTokensRatePtr,
                                                 NULL, &gigPortsSlowRateRatioPtr);
        UTF_VERIFY_EQUAL1_STRING_MAC(GT_BAD_PTR, st,
                                     "%d, gigPortsTokensRatePtr = NULL", dev);

        st = cpssDxChPortTxShaperGlobalParamsGet(dev, &xgPortsTokensRatePtr,
                                                 &gigPortsTokensRatePtr, NULL);
        UTF_VERIFY_EQUAL1_STRING_MAC(GT_BAD_PTR, st,
                                     "%d, gigPortsSlowRateRatioPtr = NULL", dev);
    }

    /* 2. For not active devices check that function returns non GT_OK.*/
    /* prepare device iterator */
    PRV_TGF_NOT_APPLIC_DEV_RESET_MAC(&dev, UTF_LION_E);

    /* Go over all non active devices. */
    while(GT_OK == prvUtfNextDeviceGet(&dev, GT_FALSE))
    {
        st = cpssDxChPortTxShaperGlobalParamsGet(dev, &xgPortsTokensRatePtr,
                              &gigPortsTokensRatePtr, &gigPortsSlowRateRatioPtr);
        UTF_VERIFY_EQUAL1_PARAM_MAC(GT_NOT_APPLICABLE_DEVICE, st, dev);
    }

    /* 3.Call function with out of bound value for device id */
    dev = PRV_CPSS_MAX_PP_DEVICES_CNS;

    st = cpssDxChPortTxShaperGlobalParamsGet(dev, &xgPortsTokensRatePtr,
                          &gigPortsTokensRatePtr, &gigPortsSlowRateRatioPtr);
    UTF_VERIFY_EQUAL1_PARAM_MAC(GT_BAD_PARAM, st, dev);
}
/*----------------------------------------------------------------------------*/
/*
GT_STATUS cpssDxChPortTxShaperTokenBucketMtuSet
(
    IN GT_U8   devNum,
    IN CPSS_DXCH_PORT_TX_TOKEN_BUCKET_MTU_ENT mtuPtr
)
*/
UTF_TEST_CASE_MAC(cpssDxChPortTxShaperTokenBucketMtuSet)
{
/*
    ITERATE_DEVICES (DxChx)
    1.1. Call with mtuPtr [CPSS_DXCH_PORT_TX_TOKEN_BUCKET_MTU_1_5K_E /
                           CPSS_DXCH_PORT_TX_TOKEN_BUCKET_MTU_10K_E]
    Expected: GT_OK.
    1.2. Call cpssDxChPortTxShaperTokenBucketMtuGet with non NULL mtuPtrGet
    Expected: GT_OK and the same mtuPtr.
*/
    GT_STATUS   st  = GT_OK;
    GT_U8       dev;

    CPSS_DXCH_PORT_TX_TOKEN_BUCKET_MTU_ENT mtuPtr;
    CPSS_DXCH_PORT_TX_TOKEN_BUCKET_MTU_ENT mtuPtrGet;

    /* prepare iterator for go over all active devices */
    PRV_TGF_NOT_APPLIC_DEV_RESET_MAC(&dev, UTF_LION_E);

    /* 1. Go over all active devices. */
    while (GT_OK == prvUtfNextDeviceGet(&dev, GT_TRUE))
    {
        /*
            1.1. Call with mtuPtr [CPSS_DXCH_PORT_TX_TOKEN_BUCKET_MTU_1_5K_E /
                                   CPSS_DXCH_PORT_TX_TOKEN_BUCKET_MTU_10K_E].
            Expected: GT_OK.
        */
        /* call with mtuPtr = CPSS_DXCH_PORT_TX_TOKEN_BUCKET_MTU_1_5K_E */
        mtuPtr = CPSS_DXCH_PORT_TX_TOKEN_BUCKET_MTU_1_5K_E;

        st = cpssDxChPortTxShaperTokenBucketMtuSet(dev, mtuPtr);
        UTF_VERIFY_EQUAL2_PARAM_MAC(GT_OK, st, dev, mtuPtr);

        /*
            1.2. Call cpssDxChPortTxShaperTokenBucketMtuGet with non NULL mtuPtrGet.
            Expected: GT_OK and the same mtuPtr.
        */
        st = cpssDxChPortTxShaperTokenBucketMtuGet(dev, &mtuPtrGet);

        UTF_VERIFY_EQUAL1_STRING_MAC(GT_OK, st,
                       "cpssDxChPortTxShaperTokenBucketMtuGet: %d", dev);
        UTF_VERIFY_EQUAL1_STRING_MAC(mtuPtr, mtuPtrGet,
                       "got another mtuPtr than was set: %d", dev);

        /* call with mtuPtr = CPSS_DXCH_PORT_TX_TOKEN_BUCKET_MTU_10K_E */
        mtuPtr = CPSS_DXCH_PORT_TX_TOKEN_BUCKET_MTU_10K_E;

        st = cpssDxChPortTxShaperTokenBucketMtuSet(dev, mtuPtr);
        UTF_VERIFY_EQUAL2_PARAM_MAC(GT_OK, st, dev, mtuPtr);

        /*
            1.2. Call cpssDxChPortTxShaperTokenBucketMtuGet with non NULL mtuPtrGet
            Expected: GT_OK and the same mtuPtr.
        */
        st = cpssDxChPortTxShaperTokenBucketMtuGet(dev, &mtuPtrGet);

        UTF_VERIFY_EQUAL1_STRING_MAC(GT_OK, st,
                  "cpssDxChPortTxShaperTokenBucketMtuGet: %d", dev);
        UTF_VERIFY_EQUAL1_STRING_MAC(mtuPtr, mtuPtrGet,
                  "got another mtuPtr than was set: %d", dev);
    }

    mtuPtr = CPSS_DXCH_PORT_TX_TOKEN_BUCKET_MTU_10K_E;

    /* 2. For not active devices check that function returns non GT_OK.*/

    /* prepare device iterator */
    PRV_TGF_NOT_APPLIC_DEV_RESET_MAC(&dev, UTF_LION_E);

    /* Go over all non active devices. */
    while(GT_OK == prvUtfNextDeviceGet(&dev, GT_FALSE))
    {
        st = cpssDxChPortTxShaperTokenBucketMtuSet(dev, mtuPtr);
        UTF_VERIFY_EQUAL1_PARAM_MAC(GT_NOT_APPLICABLE_DEVICE, st, dev);
    }

    /* 3.Call function with out of bound value for device id */
    dev = PRV_CPSS_MAX_PP_DEVICES_CNS;

    st = cpssDxChPortTxShaperTokenBucketMtuSet(dev, mtuPtr);
    UTF_VERIFY_EQUAL1_PARAM_MAC(GT_BAD_PARAM, st, dev);
}

/*----------------------------------------------------------------------------*/
/*
GT_STATUS cpssDxChPortTxShaperTokenBucketMtuGet
(
    IN  GT_U8   devNum,
    OUT CPSS_DXCH_PORT_TX_TOKEN_BUCKET_MTU_ENT *mtuPtrPtr
)
*/
UTF_TEST_CASE_MAC(cpssDxChPortTxShaperTokenBucketMtuGet)
{
/*
    ITERATE_DEVICES (DxChx)
    1.1. Call with non-NULL mtuPtr
    Expected: GT_OK.
    1.2. Call with mtuPtr [NULL]
    Expected: GT_BAD_PTR.
*/
    GT_STATUS   st  = GT_OK;
    GT_U8       dev;

    CPSS_DXCH_PORT_TX_TOKEN_BUCKET_MTU_ENT mtuPtr;

    /* prepare iterator for go over all active devices */
    PRV_TGF_NOT_APPLIC_DEV_RESET_MAC(&dev, UTF_LION_E);

    /* 1. Go over all active devices. */
    while (GT_OK == prvUtfNextDeviceGet(&dev, GT_TRUE))
    {
        /*
            1.1. Call with non-NULL mtuPtr
            Expected: GT_OK.
        */
        st = cpssDxChPortTxShaperTokenBucketMtuGet(dev, &mtuPtr);
        UTF_VERIFY_EQUAL1_PARAM_MAC(GT_OK, st, dev);

        /*
            1.2. Call with mtuPtr [NULL]
            Expected: GT_BAD_PTR.
        */
        st = cpssDxChPortTxShaperTokenBucketMtuGet(dev, NULL);
        UTF_VERIFY_EQUAL1_STRING_MAC(GT_BAD_PTR, st,
                                     "%d, mtuPtr = NULL", dev);
    }

    /* 2. For not active devices check that function returns non GT_OK.*/

    /* prepare device iterator */
    PRV_TGF_NOT_APPLIC_DEV_RESET_MAC(&dev, UTF_LION_E);

    /* Go over all non active devices. */
    while(GT_OK == prvUtfNextDeviceGet(&dev, GT_FALSE))
    {
        st = cpssDxChPortTxShaperTokenBucketMtuGet(dev, &mtuPtr);
        UTF_VERIFY_EQUAL1_PARAM_MAC(GT_NOT_APPLICABLE_DEVICE, st, dev);
    }

    /* 3.Call function with out of bound value for device id */
    dev = PRV_CPSS_MAX_PP_DEVICES_CNS;

    st = cpssDxChPortTxShaperTokenBucketMtuGet(dev, &mtuPtr);
    UTF_VERIFY_EQUAL1_PARAM_MAC(GT_BAD_PARAM, st, dev);
}

/*----------------------------------------------------------------------------*/
/*
GT_STATUS cpssDxChTxPortAllShapersDisable
(
    IN  GT_U8    devNum
)
*/
UTF_TEST_CASE_MAC(cpssDxChTxPortAllShapersDisable)
{
/*
    ITERATE_DEVICES (DxChx)
    1.1. Call with correct devNum.
    Expected: GT_OK.
*/
    GT_STATUS   st  = GT_OK;
    GT_U8       dev;

    /* prepare iterator for go over all active devices */
    PRV_TGF_NOT_APPLIC_DEV_RESET_MAC(&dev, UTF_NONE_FAMILY_E);

    /* 1. Go over all active devices. */
    while (GT_OK == prvUtfNextDeviceGet(&dev, GT_TRUE))
    {
        /*
            1.1. Call with correct devNum.
            Expected: GT_OK.
        */
        st = cpssDxChTxPortAllShapersDisable(dev);
        UTF_VERIFY_EQUAL1_PARAM_MAC(GT_OK, st, dev);
    }

    /* 2. For not active devices check that function returns non GT_OK.*/

    /* prepare device iterator */
    PRV_TGF_NOT_APPLIC_DEV_RESET_MAC(&dev, UTF_NONE_FAMILY_E);

    /* Go over all non active devices. */
    while(GT_OK == prvUtfNextDeviceGet(&dev, GT_FALSE))
    {
        st = cpssDxChTxPortAllShapersDisable(dev);
        UTF_VERIFY_EQUAL1_PARAM_MAC(GT_NOT_APPLICABLE_DEVICE, st, dev);
    }

    /* 3.Call function with out of bound value for device id */
    dev = PRV_CPSS_MAX_PP_DEVICES_CNS;

    st = cpssDxChTxPortAllShapersDisable(dev);
    UTF_VERIFY_EQUAL1_PARAM_MAC(GT_BAD_PARAM, st, dev);
}
/*----------------------------------------------------------------------------*/
/*
GT_STATUS cpssDxChTxPortShapersDisable
(
    IN  GT_U8     devNum,
    IN  GT_U8     portNum
)
*/
UTF_TEST_CASE_MAC(cpssDxChTxPortShapersDisable)
{
/*
    ITERATE_DEVICES_PHY_CPU_PORTS (DxCh)
*/

    GT_STATUS   st   = GT_OK;
    GT_U8       dev;
    GT_U8       port = PORT_TX_VALID_PHY_PORT_CNS;

    /* prepare iterator for go over all active devices */
    PRV_TGF_NOT_APPLIC_DEV_RESET_MAC(&dev, UTF_NONE_FAMILY_E);

    /* 1. Go over all active devices. */
    while (GT_OK == prvUtfNextDeviceGet(&dev, GT_TRUE))
    {
        st = prvUtfNextPhyPortReset(&port, dev);
        UTF_VERIFY_EQUAL0_PARAM_MAC(GT_OK, st);

        /* 1.1. For all active devices go over all available physical ports. */
        while (GT_OK == prvUtfNextPhyPortGet(&port, GT_TRUE))
        {
            st = cpssDxChTxPortShapersDisable(dev, port);
            UTF_VERIFY_EQUAL2_PARAM_MAC(GT_OK, st, dev, port);
        }

        st = prvUtfNextPhyPortReset(&port, dev);
        UTF_VERIFY_EQUAL0_PARAM_MAC(GT_OK, st);

        /* 1.2. For all active devices go over all non available physical ports. */
        while (GT_OK == prvUtfNextPhyPortGet(&port, GT_FALSE))
        {
            st = cpssDxChTxPortShapersDisable(dev, port);
            UTF_VERIFY_EQUAL2_PARAM_MAC(GT_BAD_PARAM, st, dev, port);
        }

        /* 1.3. For active device check that function returns GT_BAD_PARAM */
        /* for out of bound value for port number.                         */
        port = PRV_CPSS_MAX_PP_PORTS_NUM_CNS;

        st = cpssDxChTxPortShapersDisable(dev, port);
        UTF_VERIFY_EQUAL2_PARAM_MAC(GT_BAD_PARAM, st, dev, port);

        /* 1.4. For active device check that function returns GT_OK        */
        /* for CPU port number.                                            */
        port = CPSS_CPU_PORT_NUM_CNS;

        st = cpssDxChTxPortShapersDisable(dev, port);
        UTF_VERIFY_EQUAL2_PARAM_MAC(GT_OK, st, dev, port);
    }

    /* 2. For not active devices check that function returns non GT_OK.*/
    port = PORT_TX_VALID_PHY_PORT_CNS;

    /* prepare device iterator */
    PRV_TGF_NOT_APPLIC_DEV_RESET_MAC(&dev, UTF_NONE_FAMILY_E);

    /* Go over all non active devices. */
    while(GT_OK == prvUtfNextDeviceGet(&dev, GT_FALSE))
    {
        st = cpssDxChTxPortShapersDisable(dev, port);
        UTF_VERIFY_EQUAL1_PARAM_MAC(GT_NOT_APPLICABLE_DEVICE, st, dev);
    }

    /* 3.Call function with out of bound value for device id */
    dev = PRV_CPSS_MAX_PP_DEVICES_CNS;
    /* port == 0 */

    st = cpssDxChTxPortShapersDisable(dev, port);
    UTF_VERIFY_EQUAL1_PARAM_MAC(GT_BAD_PARAM, st, dev);
}

/*----------------------------------------------------------------------------*/
/*
GT_STATUS cpssDxChPortTxInit
(
    IN GT_U8    devNum,
)
*/
UTF_TEST_CASE_MAC(cpssDxChPortTxInit)
{
/*
    ITERATE_DEVICES (DxChx)
    1.1. Call with not null dev.
    Expected: GT_OK.
*/

    GT_STATUS   st     = GT_OK;
    GT_U8       dev;

    /* prepare iterator for go over all active devices */
    PRV_TGF_NOT_APPLIC_DEV_RESET_MAC(&dev, UTF_NONE_FAMILY_E);

    /* 1. Go over all active devices. */
    while (GT_OK == prvUtfNextDeviceGet(&dev, GT_TRUE))
    {
        /*
            1.1. Call with not null dev.
            Expected: GT_OK.
        */
        st = cpssDxChPortTxInit(dev);
        UTF_VERIFY_EQUAL1_PARAM_MAC(GT_OK, st, dev);
    }

    /* 2. For not active devices check that function returns non GT_OK.*/

    /* prepare device iterator */
    PRV_TGF_NOT_APPLIC_DEV_RESET_MAC(&dev, UTF_NONE_FAMILY_E);

    /* Go over all non active devices. */
    while(GT_OK == prvUtfNextDeviceGet(&dev, GT_FALSE))
    {
        st = cpssDxChPortTxInit(dev);
        UTF_VERIFY_EQUAL1_PARAM_MAC(GT_NOT_APPLICABLE_DEVICE, st, dev);
    }

    /* 3.Call function with out of bound value for device id */
    dev = PRV_CPSS_MAX_PP_DEVICES_CNS;

    st = cpssDxChPortTxInit(dev);
    UTF_VERIFY_EQUAL1_PARAM_MAC(GT_BAD_PARAM, st, dev);
}

/*----------------------------------------------------------------------------*/
/*
GT_STATUS cpssDxChPortTxSharedPolicySet
(
    IN  GT_U8                                devNum,
    IN  CPSS_DXCH_PORT_TX_SHARED_POLICY_ENT  policy
)
*/
UTF_TEST_CASE_MAC(cpssDxChPortTxSharedPolicySet)
{
/*
    ITERATE_DEVICES (xCat and above)
    1.1. Call with policy[CPSS_DXCH_PORT_TX_SHARED_POLICY_UNCONSTRAINED_E /
                          CPSS_DXCH_PORT_TX_SHARED_POLICY_CONSTRAINED_E].
    Expected: GT_OK.
    1.2. Call get func with the same params.
    Expected: GT_OK and the same value.
    1.3. Call with wrong enum values policy.
    Expected: GT_BAD_PARAM.
*/

    GT_STATUS   st     = GT_OK;
    GT_U8       dev;
    CPSS_DXCH_PORT_TX_SHARED_POLICY_ENT  policy = 0;
    CPSS_DXCH_PORT_TX_SHARED_POLICY_ENT  policyGet = 1;

    /* prepare iterator for go over all active devices */
    PRV_TGF_NOT_APPLIC_DEV_RESET_MAC(&dev, UTF_CH1_E | UTF_CH1_DIAMOND_E | UTF_CH2_E | UTF_CH3_E);

    /* 1. Go over all active devices. */
    while (GT_OK == prvUtfNextDeviceGet(&dev, GT_TRUE))
    {
        /*
            1.1. Call with policy[CPSS_DXCH_PORT_TX_SHARED_POLICY_UNCONSTRAINED_E /
                                  CPSS_DXCH_PORT_TX_SHARED_POLICY_CONSTRAINED_E].
            Expected: GT_OK.
        */
        /*call with policy = CPSS_DXCH_PORT_TX_SHARED_POLICY_UNCONSTRAINED_E;*/
        policy = CPSS_DXCH_PORT_TX_SHARED_POLICY_UNCONSTRAINED_E;

        st = cpssDxChPortTxSharedPolicySet(dev, policy);
        UTF_VERIFY_EQUAL1_PARAM_MAC(GT_OK, st, dev);

        /*
            1.2. Call get func with the same params.
            Expected: GT_OK and the same value.
        */
        st = cpssDxChPortTxSharedPolicyGet(dev, &policyGet);

        UTF_VERIFY_EQUAL1_PARAM_MAC(GT_OK, st, dev);
        UTF_VERIFY_EQUAL1_STRING_MAC(policy, policyGet,
                         "get another policy than was set: %d", dev);


        /*call with policy = CPSS_DXCH_PORT_TX_SHARED_POLICY_CONSTRAINED_E;*/
        policy = CPSS_DXCH_PORT_TX_SHARED_POLICY_CONSTRAINED_E;

        st = cpssDxChPortTxSharedPolicySet(dev, policy);
        UTF_VERIFY_EQUAL1_PARAM_MAC(GT_OK, st, dev);

        /*
            1.2. Call get func with the same params.
            Expected: GT_OK and the same value.
        */
        st = cpssDxChPortTxSharedPolicyGet(dev, &policyGet);

        UTF_VERIFY_EQUAL1_PARAM_MAC(GT_OK, st, dev);
        UTF_VERIFY_EQUAL1_STRING_MAC(policy, policyGet,
                         "get another policy than was set: %d", dev);

        /*
            1.3. Call with wrong enum values policy.
            Expected: GT_BAD_PARAM.
        */
        UTF_ENUMS_CHECK_MAC(cpssDxChPortTxSharedPolicySet
                            (dev, policy),
                            policy);
    }

    /* 2. For not active devices check that function returns non GT_OK.*/

    /* prepare device iterator */
    PRV_TGF_NOT_APPLIC_DEV_RESET_MAC(&dev, UTF_CH1_E | UTF_CH1_DIAMOND_E | UTF_CH2_E | UTF_CH3_E);

    /* Go over all non active devices. */
    while(GT_OK == prvUtfNextDeviceGet(&dev, GT_FALSE))
    {
        st = cpssDxChPortTxSharedPolicySet(dev, policy);
        UTF_VERIFY_EQUAL1_PARAM_MAC(GT_NOT_APPLICABLE_DEVICE, st, dev);
    }

    /* 3.Call function with out of bound value for device id */
    dev = PRV_CPSS_MAX_PP_DEVICES_CNS;

    st = cpssDxChPortTxSharedPolicySet(dev, policy);
    UTF_VERIFY_EQUAL1_PARAM_MAC(GT_BAD_PARAM, st, dev);
}

/*----------------------------------------------------------------------------*/
/*
GT_STATUS cpssDxChPortTxSharedPolicyGet
(
    IN  GT_U8                                devNum,
    OUT CPSS_DXCH_PORT_TX_SHARED_POLICY_ENT  *policyPtr
)
*/
UTF_TEST_CASE_MAC(cpssDxChPortTxSharedPolicyGet)
{
/*
    ITERATE_DEVICES (xCat and above)
    1.1. Call with not null policy.
    Expected: GT_OK.
    1.2. Call with wrong policy[NULL].
    Expected: GT_BAD_PTR.
*/

    GT_STATUS   st     = GT_OK;
    GT_U8       dev;
    CPSS_DXCH_PORT_TX_SHARED_POLICY_ENT  policyGet = 1;

    /* prepare iterator for go over all active devices */
    PRV_TGF_NOT_APPLIC_DEV_RESET_MAC(&dev, UTF_CH1_E | UTF_CH1_DIAMOND_E | UTF_CH2_E | UTF_CH3_E);

    /* 1. Go over all active devices. */
    while (GT_OK == prvUtfNextDeviceGet(&dev, GT_TRUE))
    {
        /*
            1.1. Call with not null policy.
            Expected: GT_OK.
        */

        st = cpssDxChPortTxSharedPolicyGet(dev, &policyGet);

        UTF_VERIFY_EQUAL1_PARAM_MAC(GT_OK, st, dev);

        /*
            1.2. Call with wrong policy[NULL].
            Expected: GT_BAD_PTR.
        */

        st = cpssDxChPortTxSharedPolicyGet(dev, NULL);
        UTF_VERIFY_EQUAL1_PARAM_MAC(GT_BAD_PTR, st, dev);
    }

    /* 2. For not active devices check that function returns non GT_OK.*/

    /* prepare device iterator */
    PRV_TGF_NOT_APPLIC_DEV_RESET_MAC(&dev, UTF_CH1_E | UTF_CH1_DIAMOND_E | UTF_CH2_E | UTF_CH3_E);

    /* Go over all non active devices. */
    while(GT_OK == prvUtfNextDeviceGet(&dev, GT_FALSE))
    {
        st = cpssDxChPortTxSharedPolicyGet(dev, &policyGet);
        UTF_VERIFY_EQUAL1_PARAM_MAC(GT_NOT_APPLICABLE_DEVICE, st, dev);
    }

    /* 3.Call function with out of bound value for device id */
    dev = PRV_CPSS_MAX_PP_DEVICES_CNS;

    st = cpssDxChPortTxSharedPolicyGet(dev, &policyGet);
    UTF_VERIFY_EQUAL1_PARAM_MAC(GT_BAD_PARAM, st, dev);
}

/*----------------------------------------------------------------------------*/
/*
GT_STATUS cpssDxChPortTxShaperModeSet
(
    IN  GT_U8    devNum,
    IN  GT_U8    portNum,
    IN  CPSS_PORT_TX_DROP_SHAPER_MODE_ENT   mode
)
*/
UTF_TEST_CASE_MAC(cpssDxChPortTxShaperModeSet)
{
/*
    ITERATE_DEVICES_PHY_CPU_PORTS (Lion)
    1.1.1. Call with mode [CPSS_PORT_TX_DROP_SHAPER_BYTE_MODE_E /
                           CPSS_PORT_TX_DROP_SHAPER_PACKET_MODE_E].
    Expected: GT_OK.
    1.1.2. Call cpssDxChPortTxShaperModeGet with non-NULL pointers,
                                                 other params same as in 1.1.1.
    Expected: GT_OK and the same values.
    1.1.3. Call with out of range mode [wrong enum values],
                     other params same as in 1.1.1.
    Expected: GT_BAD_PARAM.
*/
    GT_STATUS   st   = GT_OK;
    GT_U8       dev;
    GT_U8       port = PORT_TX_VALID_PHY_PORT_CNS;

    CPSS_PORT_TX_DROP_SHAPER_MODE_ENT   mode    = CPSS_PORT_TX_DROP_SHAPER_BYTE_MODE_E;
    CPSS_PORT_TX_DROP_SHAPER_MODE_ENT   modeGet = CPSS_PORT_TX_DROP_SHAPER_BYTE_MODE_E;


    /* prepare iterator for go over all active devices */
    PRV_TGF_NOT_APPLIC_DEV_RESET_MAC(&dev, UTF_CH1_E | UTF_CH1_DIAMOND_E | UTF_CH2_E | UTF_CH3_E | UTF_XCAT_E | UTF_XCAT2_E);

    /* 1. Go over all active devices. */
    while (GT_OK == prvUtfNextDeviceGet(&dev, GT_TRUE))
    {
        st = prvUtfNextPhyPortReset(&port, dev);
        UTF_VERIFY_EQUAL0_PARAM_MAC(GT_OK, st);

        /* 1.1. For all active devices go over all available physical ports. */
        while (GT_OK == prvUtfNextPhyPortGet(&port, GT_TRUE))
        {
            /*
                1.1.1. Call with mode [CPSS_PORT_TX_DROP_SHAPER_BYTE_MODE_E /
                                       CPSS_PORT_TX_DROP_SHAPER_PACKET_MODE_E].
                Expected: GT_OK.
            */
            mode = CPSS_PORT_TX_DROP_SHAPER_BYTE_MODE_E;

            st = cpssDxChPortTxShaperModeSet(dev, port, mode);
            UTF_VERIFY_EQUAL3_PARAM_MAC(GT_OK, st, dev, port, mode);

            /*
                1.1.2. Call cpssDxChPortTxShaperModeGet with non-NULL pointers,
                                                             other params same as in 1.1.1.
                Expected: GT_OK and the same values.
            */
            st = cpssDxChPortTxShaperModeGet(dev, port, &modeGet);
            UTF_VERIFY_EQUAL1_STRING_MAC(GT_OK, st,
                       "cpssDxChPortTxShaperModeGet: %d",  dev);

            /* Verifying values */
            UTF_VERIFY_EQUAL1_STRING_MAC(mode, modeGet,
                       "get another mode than was set: %d", dev);

            /*
                1.1.1. Call with mode [CPSS_PORT_TX_DROP_SHAPER_BYTE_MODE_E /
                                       CPSS_PORT_TX_DROP_SHAPER_PACKET_MODE_E].
                Expected: GT_OK.
            */
            mode = CPSS_PORT_TX_DROP_SHAPER_PACKET_MODE_E;

            st = cpssDxChPortTxShaperModeSet(dev, port, mode);
            UTF_VERIFY_EQUAL3_PARAM_MAC(GT_OK, st, dev, port, mode);

            /*
                1.1.2. Call cpssDxChPortTxShaperModeGet with non-NULL pointers,
                                                             other params same as in 1.1.1.
                Expected: GT_OK and the same values.
            */
            st = cpssDxChPortTxShaperModeGet(dev, port, &modeGet);
            UTF_VERIFY_EQUAL1_STRING_MAC(GT_OK, st,
                       "cpssDxChPortTxShaperModeGet: %d",  dev);

            /* Verifying values */
            UTF_VERIFY_EQUAL1_STRING_MAC(mode, modeGet,
                       "get another mode than was set: %d", dev);

            /*
                1.1.3. Call with out of range mode [wrong enum values],
                                 other params same as in 1.1.1.
                Expected: GT_BAD_PARAM.
            */
            UTF_ENUMS_CHECK_MAC(cpssDxChPortTxShaperModeSet
                                (dev, port, mode),
                                mode);
        }

        mode = CPSS_PORT_TX_DROP_SHAPER_BYTE_MODE_E;

        st = prvUtfNextPhyPortReset(&port, dev);
        UTF_VERIFY_EQUAL0_PARAM_MAC(GT_OK, st);

        /* 1.2. For all active devices go over all non available physical ports. */
        while (GT_OK == prvUtfNextPhyPortGet(&port, GT_FALSE))
        {
            /* 1.2.1. Call function for each non-active port */
            st = cpssDxChPortTxShaperModeSet(dev, port, mode);
            UTF_VERIFY_EQUAL2_PARAM_MAC(GT_BAD_PARAM, st, dev, port);
        }

        /* 1.3. For active device check that function returns GT_BAD_PARAM */
        /* for out of bound value for port number.                         */
        port = PRV_CPSS_MAX_PP_PORTS_NUM_CNS;

        st = cpssDxChPortTxShaperModeSet(dev, port, mode);
        UTF_VERIFY_EQUAL2_PARAM_MAC(GT_BAD_PARAM, st, dev, port);

        /* 1.4. For active device check CPU port number */
        port = CPSS_CPU_PORT_NUM_CNS;
        /*
            1.4.1. Call with mode [CPSS_PORT_TX_DROP_SHAPER_BYTE_MODE_E /
                                   CPSS_PORT_TX_DROP_SHAPER_PACKET_MODE_E].
            Expected: GT_OK.
        */
        mode = CPSS_PORT_TX_DROP_SHAPER_BYTE_MODE_E;

        st = cpssDxChPortTxShaperModeSet(dev, port, mode);
        UTF_VERIFY_EQUAL3_PARAM_MAC(GT_OK, st, dev, port, mode);

        /*
            1.4.2. Call cpssDxChPortTxShaperModeGet with non-NULL pointers,
                                                         other params same as in 1.1.1.
            Expected: GT_OK and the same values.
        */
        st = cpssDxChPortTxShaperModeGet(dev, port, &modeGet);
        UTF_VERIFY_EQUAL1_STRING_MAC(GT_OK, st,
                   "cpssDxChPortTxShaperModeGet: %d",  dev);

        /* Verifying values */
        UTF_VERIFY_EQUAL1_STRING_MAC(mode, modeGet,
                   "get another mode than was set: %d", dev);

        /*
            1.4.1. Call with mode [CPSS_PORT_TX_DROP_SHAPER_BYTE_MODE_E /
                                   CPSS_PORT_TX_DROP_SHAPER_PACKET_MODE_E].
            Expected: GT_OK.
        */
        mode = CPSS_PORT_TX_DROP_SHAPER_PACKET_MODE_E;

        st = cpssDxChPortTxShaperModeSet(dev, port, mode);
        UTF_VERIFY_EQUAL3_PARAM_MAC(GT_OK, st, dev, port, mode);

        /*
            1.4.2. Call cpssDxChPortTxShaperModeGet with non-NULL pointers,
                                                         other params same as in 1.1.1.
            Expected: GT_OK and the same values.
        */
        st = cpssDxChPortTxShaperModeGet(dev, port, &modeGet);
        UTF_VERIFY_EQUAL1_STRING_MAC(GT_OK, st,
                   "cpssDxChPortTxShaperModeGet: %d",  dev);

        /* Verifying values */
        UTF_VERIFY_EQUAL1_STRING_MAC(mode, modeGet,
                   "get another mode than was set: %d", dev);

        /*
            1.4.3. Call with out of range mode [wrong enum values],
                             other params same as in 1.1.1.
            Expected: GT_BAD_PARAM.
        */
        UTF_ENUMS_CHECK_MAC(cpssDxChPortTxShaperModeSet
                            (dev, port, mode),
                            mode);
    }

    mode = CPSS_PORT_TX_DROP_SHAPER_BYTE_MODE_E;

    /* 2. For not active devices check that function returns non GT_OK.*/
    port = PORT_TX_VALID_PHY_PORT_CNS;

    /* prepare device iterator */
    PRV_TGF_NOT_APPLIC_DEV_RESET_MAC(&dev, UTF_CH1_E | UTF_CH1_DIAMOND_E | UTF_CH2_E | UTF_CH3_E | UTF_XCAT_E | UTF_XCAT2_E);

    /* Go over all non active devices. */
    while(GT_OK == prvUtfNextDeviceGet(&dev, GT_FALSE))
    {
        st = cpssDxChPortTxShaperModeSet(dev, port, mode);
        UTF_VERIFY_EQUAL1_PARAM_MAC(GT_NOT_APPLICABLE_DEVICE, st, dev);
    }

    /* 3.Call function with out of bound value for device id */
    dev = PRV_CPSS_MAX_PP_DEVICES_CNS;

    st = cpssDxChPortTxShaperModeSet(dev, port, mode);
    UTF_VERIFY_EQUAL1_PARAM_MAC(GT_BAD_PARAM, st, dev);
}

/*----------------------------------------------------------------------------*/
/*
GT_STATUS cpssDxChPortTxShaperModeGet
(
    IN  GT_U8    devNum,
    IN  GT_U8    portNum,
    OUT CPSS_PORT_TX_DROP_SHAPER_MODE_ENT   *modePtr
*/
UTF_TEST_CASE_MAC(cpssDxChPortTxShaperModeGet)
{
/*
    ITERATE_DEVICES_PHY_CPU_PORTS (Lion)
    1.1.1. Call with non NULL modePtr.
    Expected: GT_OK.
    1.1.2. Call with modePtr [NULL],
                     other params same as in 1.1.1.
    Expected: GT_BAD_PTR.
*/
    GT_STATUS   st   = GT_OK;
    GT_U8       dev;
    GT_U8       port = PORT_TX_VALID_PHY_PORT_CNS;

    CPSS_PORT_TX_DROP_SHAPER_MODE_ENT   mode = CPSS_PORT_TX_DROP_SHAPER_BYTE_MODE_E;


    /* prepare iterator for go over all active devices */
    PRV_TGF_NOT_APPLIC_DEV_RESET_MAC(&dev, UTF_CH1_E | UTF_CH1_DIAMOND_E | UTF_CH2_E | UTF_CH3_E | UTF_XCAT_E | UTF_XCAT2_E);

    /* 1. Go over all active devices. */
    while (GT_OK == prvUtfNextDeviceGet(&dev, GT_TRUE))
    {
        st = prvUtfNextPhyPortReset(&port, dev);
        UTF_VERIFY_EQUAL0_PARAM_MAC(GT_OK, st);

        /* 1.1. For all active devices go over all available physical ports. */
        while (GT_OK == prvUtfNextPhyPortGet(&port, GT_TRUE))
        {
            /*
                1.1.1. Call with non NULL modePtr.
                Expected: GT_OK.
            */
            st = cpssDxChPortTxShaperModeGet(dev, port, &mode);
            UTF_VERIFY_EQUAL2_PARAM_MAC(GT_OK, st, dev, port);

            /*
                1.1.2. Call with modePtr [NULL],
                                 other params same as in 1.1.1.
                Expected: GT_BAD_PTR.
            */
            st = cpssDxChPortTxShaperModeGet(dev, port, NULL);
            UTF_VERIFY_EQUAL2_PARAM_MAC(GT_BAD_PTR, st, "%d, modePtr = NULL", dev);
        }

        st = prvUtfNextPhyPortReset(&port, dev);
        UTF_VERIFY_EQUAL0_PARAM_MAC(GT_OK, st);

        /* 1.2. For all active devices go over all non available physical ports. */
        while (GT_OK == prvUtfNextPhyPortGet(&port, GT_FALSE))
        {
            /* 1.2.1. Call function for each non-active port */
            st = cpssDxChPortTxShaperModeGet(dev, port, &mode);
            UTF_VERIFY_EQUAL2_PARAM_MAC(GT_BAD_PARAM, st, dev, port);
        }

        /* 1.3. For active device check that function returns GT_BAD_PARAM */
        /* for out of bound value for port number.                         */
        port = PRV_CPSS_MAX_PP_PORTS_NUM_CNS;

        st = cpssDxChPortTxShaperModeGet(dev, port, &mode);
        UTF_VERIFY_EQUAL2_PARAM_MAC(GT_BAD_PARAM, st, dev, port);

        /* 1.4. For active device check that function returns GT_OK        */
        /* for CPU port number.                                            */
        port = CPSS_CPU_PORT_NUM_CNS;

        st = cpssDxChPortTxShaperModeGet(dev, port, &mode);
        UTF_VERIFY_EQUAL2_PARAM_MAC(GT_OK, st, dev, port);
    }

    /* 2. For not active devices check that function returns non GT_OK.*/
    port = PORT_TX_VALID_PHY_PORT_CNS;

    /* prepare device iterator */
    PRV_TGF_NOT_APPLIC_DEV_RESET_MAC(&dev, UTF_CH1_E | UTF_CH1_DIAMOND_E | UTF_CH2_E | UTF_CH3_E | UTF_XCAT_E | UTF_XCAT2_E);

    /* Go over all non active devices. */
    while(GT_OK == prvUtfNextDeviceGet(&dev, GT_FALSE))
    {
        st = cpssDxChPortTxShaperModeGet(dev, port, &mode);
        UTF_VERIFY_EQUAL1_PARAM_MAC(GT_NOT_APPLICABLE_DEVICE, st, dev);
    }

    /* 3.Call function with out of bound value for device id */
    dev = PRV_CPSS_MAX_PP_DEVICES_CNS;

    st = cpssDxChPortTxShaperModeGet(dev, port, &mode);
    UTF_VERIFY_EQUAL1_PARAM_MAC(GT_BAD_PARAM, st, dev);
}

/*----------------------------------------------------------------------------*/
/*
GT_STATUS cpssDxChPortTxShaperConfigurationSet
(
    IN  GT_U8                   devNum,
    IN  CPSS_DXCH_PORT_TX_SHAPER_CONFIG_STC *configsPtr
)
*/
UTF_TEST_CASE_MAC(cpssDxChPortTxShaperConfigurationSet)
{
/*
    ITERATE_DEVICES(Lion)
    1.1. Call with configsPtr{tokensRate [1 / 15],
                              slowRateRatio [1 / 16],
                              tokensRateGran[CPSS_DXCH_PORT_TX_SHAPER_GRANULARITY_64_CORE_CLOCKS_E /
                                             CPSS_DXCH_PORT_TX_SHAPER_GRANULARITY_1024_CORE_CLOCKS_E],
                              portsPacketLength [0 / 0xFFFFFF],
                              cpuPacketLength [0 / 0xFFFFFF]}
    Expected: GT_OK.
    1.2. Call cpssDxChPortTxShaperConfigurationGet with non-NULL pointers
                                                        other params same as in 1.1.
    Expected: GT_OK and the same values.
    1.3. Call with out of range configsPtr->tokensRate [0 / 16],
                   other params same as in 1.1.
    Expected: NOT GT_OK.
    1.4. Call with out of range configsPtr->slowRateRatio [0 / 17],
                   other params same as in 1.1.
    Expected: NOT GT_OK.
    1.5. Call with out of range configsPtr->tokensRateGran [wrong enum values],
                   other params same as in 1.1.1.
    Expected: GT_BAD_PARAM.
    1.6. Call with out of range configsPtr->portsPacketLength [0xFFFFFF + 1],
                   other params same as in 1.1.
    Expected: NOT GT_OK.
    1.7. Call with out of range configsPtr->cpuPacketLength [0xFFFFFF + 1],
                   other params same as in 1.1.
    Expected: NOT GT_OK.
    1.8. Call with configsPtr [NULL],
                   other params same as in 1.1.
    Expected: GT_BAD_PTR.
*/
    GT_STATUS   st = GT_OK;
    GT_U8       dev;

    CPSS_DXCH_PORT_TX_SHAPER_CONFIG_STC configs    = {0};
    CPSS_DXCH_PORT_TX_SHAPER_CONFIG_STC configsGet = {0};


    /* prepare iterator for go over all active devices */
    PRV_TGF_NOT_APPLIC_DEV_RESET_MAC(&dev, UTF_CH1_E | UTF_CH1_DIAMOND_E | UTF_CH2_E | UTF_CH3_E | UTF_XCAT_E | UTF_XCAT2_E);

    /* 1. Go over all active devices. */
    while (GT_OK == prvUtfNextDeviceGet(&dev, GT_TRUE))
    {
        /*
            1.1. Call with configsPtr{tokensRate [1 / 15],
                                      slowRateRatio [1 / 16],
                                      tokensRateGran[CPSS_DXCH_PORT_TX_SHAPER_GRANULARITY_64_CORE_CLOCKS_E /
                                                     CPSS_DXCH_PORT_TX_SHAPER_GRANULARITY_1024_CORE_CLOCKS_E],
                                      portsPacketLength [0 / 0xFFFFFF],
                                      cpuPacketLength [0 / 0xFFFFFF]}
            Expected: GT_OK.
        */
        configs.tokensRate        = 1;
        configs.slowRateRatio     = 1;
        configs.tokensRateGran    = CPSS_DXCH_PORT_TX_SHAPER_GRANULARITY_64_CORE_CLOCKS_E;
        configs.portsPacketLength = 0;
        configs.cpuPacketLength   = 0;

        st = cpssDxChPortTxShaperConfigurationSet(dev, &configs);
        UTF_VERIFY_EQUAL1_PARAM_MAC(GT_OK, st, dev);

        /*
            1.2. Call cpssDxChPortTxShaperConfigurationGet with non-NULL pointers
                                                                other params same as in 1.1.
            Expected: GT_OK and the same values.
        */
        st = cpssDxChPortTxShaperConfigurationGet(dev, &configsGet);
        UTF_VERIFY_EQUAL1_STRING_MAC(GT_OK, st,
                   "cpssDxChPortTxShaperConfigurationGet: %d", dev);

        /* verifying values */
        UTF_VERIFY_EQUAL1_STRING_MAC(configs.tokensRate, configsGet.tokensRate,
                           "got another configsPtr->tokensRate then was set: %d", dev);
        UTF_VERIFY_EQUAL1_STRING_MAC(configs.slowRateRatio, configsGet.slowRateRatio,
                           "got another configsPtr->slowRateRatio then was set: %d", dev);
        UTF_VERIFY_EQUAL1_STRING_MAC(configs.tokensRateGran, configsGet.tokensRateGran,
                           "got another configsPtr->tokensRateGran then was set: %d", dev);
        UTF_VERIFY_EQUAL1_STRING_MAC(configs.portsPacketLength, configsGet.portsPacketLength,
                           "got another configsPtr->portsPacketLength then was set: %d", dev);
        UTF_VERIFY_EQUAL1_STRING_MAC(configs.cpuPacketLength, configsGet.cpuPacketLength,
                           "got another configsPtr->cpuPacketLength then was set: %d", dev);

        /*
            1.1. Call with configsPtr{tokensRate [1 / 15],
                                      slowRateRatio [1 / 16],
                                      tokensRateGran[CPSS_DXCH_PORT_TX_SHAPER_GRANULARITY_64_CORE_CLOCKS_E /
                                                     CPSS_DXCH_PORT_TX_SHAPER_GRANULARITY_1024_CORE_CLOCKS_E],
                                      portsPacketLength [0 / 0xFFFFFF],
                                      cpuPacketLength [0 / 0xFFFFFF]}
            Expected: GT_OK.
        */
        configs.tokensRate        = 15;
        configs.slowRateRatio     = 16;
        configs.tokensRateGran    = CPSS_DXCH_PORT_TX_SHAPER_GRANULARITY_1024_CORE_CLOCKS_E;
        configs.portsPacketLength = 0xFFFFFF;
        configs.cpuPacketLength   = 0xFFFFFF;

        st = cpssDxChPortTxShaperConfigurationSet(dev, &configs);
        UTF_VERIFY_EQUAL1_PARAM_MAC(GT_OK, st, dev);

        /*
            1.2. Call cpssDxChPortTxShaperConfigurationGet with non-NULL pointers
                                                                other params same as in 1.1.
            Expected: GT_OK and the same values.
        */
        st = cpssDxChPortTxShaperConfigurationGet(dev, &configsGet);
        UTF_VERIFY_EQUAL1_STRING_MAC(GT_OK, st,
                   "cpssDxChPortTxShaperConfigurationGet: %d", dev);

        /* verifying values */
        UTF_VERIFY_EQUAL1_STRING_MAC(configs.tokensRate, configsGet.tokensRate,
                           "got another configsPtr->tokensRate then was set: %d", dev);
        UTF_VERIFY_EQUAL1_STRING_MAC(configs.slowRateRatio, configsGet.slowRateRatio,
                           "got another configsPtr->slowRateRatio then was set: %d", dev);
        UTF_VERIFY_EQUAL1_STRING_MAC(configs.tokensRateGran, configsGet.tokensRateGran,
                           "got another configsPtr->tokensRateGran then was set: %d", dev);
        UTF_VERIFY_EQUAL1_STRING_MAC(configs.portsPacketLength, configsGet.portsPacketLength,
                           "got another configsPtr->portsPacketLength then was set: %d", dev);
        UTF_VERIFY_EQUAL1_STRING_MAC(configs.cpuPacketLength, configsGet.cpuPacketLength,
                           "got another configsPtr->cpuPacketLength then was set: %d", dev);

        /*
            1.3. Call with out of range configsPtr->tokensRate [0 / 16],
                           other params same as in 1.1.
            Expected: NOT GT_OK.
        */
        /* call with configsPtr->tokensRate = 0 */
        configs.tokensRate = 0;

        st = cpssDxChPortTxShaperConfigurationSet(dev, &configs);
        UTF_VERIFY_NOT_EQUAL2_STRING_MAC(GT_OK, st, "%d, ->tokensRate = %f",
                                         dev, configs.tokensRate);

        /* call with configsPtr->tokensRate = 16 */
        configs.tokensRate = 16;

        st = cpssDxChPortTxShaperConfigurationSet(dev, &configs);
        UTF_VERIFY_NOT_EQUAL2_STRING_MAC(GT_OK, st, "%d, ->tokensRate = %f",
                                         dev, configs.tokensRate);

        configs.tokensRate = 1;

        /*
            1.4. Call with out of range configsPtr->slowRateRatio [0 / 17],
                           other params same as in 1.1.
            Expected: NOT GT_OK.
        */
        /* call with configsPtr->slowRateRatio = 0 */
        configs.slowRateRatio = 0;

        st = cpssDxChPortTxShaperConfigurationSet(dev, &configs);
        UTF_VERIFY_NOT_EQUAL2_STRING_MAC(GT_OK, st, "%d, ->slowRateRatio = %f",
                                         dev, configs.slowRateRatio);

        /* call with configsPtr->slowRateRatio = 16 */
        configs.slowRateRatio = 17;

        st = cpssDxChPortTxShaperConfigurationSet(dev, &configs);
        UTF_VERIFY_NOT_EQUAL2_STRING_MAC(GT_OK, st, "%d, ->slowRateRatio = %f",
                                         dev, configs.slowRateRatio);

        configs.slowRateRatio = 1;

        /*
            1.5. Call with out of range configsPtr->tokensRateGran [wrong enum values],
                           other params same as in 1.1.1.
            Expected: GT_BAD_PARAM.
        */
        UTF_ENUMS_CHECK_MAC(cpssDxChPortTxShaperConfigurationSet
                            (dev, &configs),
                            configs.tokensRateGran);

        /*
            1.6. Call with out of range configsPtr->portsPacketLength [0xFFFFFF + 1],
                           other params same as in 1.1.
            Expected: NOT GT_OK.
        */
        configs.portsPacketLength = 0xFFFFFF + 1;

        st = cpssDxChPortTxShaperConfigurationSet(dev, &configs);
        UTF_VERIFY_NOT_EQUAL2_STRING_MAC(GT_OK, st, "%d, ->portsPacketLength = %f",
                                         dev, configs.portsPacketLength);

        configs.portsPacketLength = 0;

        /*
            1.7. Call with out of range configsPtr->cpuPacketLength [0xFFFFFF + 1],
                           other params same as in 1.1.
            Expected: NOT GT_OK.
        */
        configs.cpuPacketLength = 0xFFFFFF + 1;

        st = cpssDxChPortTxShaperConfigurationSet(dev, &configs);
        UTF_VERIFY_NOT_EQUAL2_STRING_MAC(GT_OK, st, "%d, ->cpuPacketLength = %f",
                                         dev, configs.cpuPacketLength);

        configs.cpuPacketLength = 0;

        /*
            1.8. Call with configsPtr [NULL],
                           other params same as in 1.1.
            Expected: GT_BAD_PTR.
        */
        st = cpssDxChPortTxShaperConfigurationSet(dev, NULL);
        UTF_VERIFY_EQUAL1_STRING_MAC(GT_BAD_PTR, st, "%d, configsPtr = NULL", dev);
    }

    configs.tokensRate        = 1;
    configs.slowRateRatio     = 1;
    configs.tokensRateGran    = CPSS_DXCH_PORT_TX_SHAPER_GRANULARITY_64_CORE_CLOCKS_E;
    configs.portsPacketLength = 0;
    configs.cpuPacketLength   = 0;

    /* 2. For not active devices check that function returns non GT_OK.*/
    /* prepare device iterator */
    PRV_TGF_NOT_APPLIC_DEV_RESET_MAC(&dev, UTF_CH1_E | UTF_CH1_DIAMOND_E | UTF_CH2_E | UTF_CH3_E | UTF_XCAT_E | UTF_XCAT2_E);

    /* Go over all non active devices. */
    while(GT_OK == prvUtfNextDeviceGet(&dev, GT_FALSE))
    {
        st = cpssDxChPortTxShaperConfigurationSet(dev, &configs);
        UTF_VERIFY_EQUAL1_PARAM_MAC(GT_NOT_APPLICABLE_DEVICE, st, dev);
    }

    /* 3.Call function with out of bound value for device id */
    dev = PRV_CPSS_MAX_PP_DEVICES_CNS;

    st = cpssDxChPortTxShaperConfigurationSet(dev, &configs);
    UTF_VERIFY_EQUAL1_PARAM_MAC(GT_BAD_PARAM, st, dev);
}

/*----------------------------------------------------------------------------*/
/*
GT_STATUS cpssDxChPortTxShaperConfigurationGet
(
    IN  GT_U8                   devNum,
    OUT CPSS_DXCH_PORT_TX_SHAPER_CONFIG_STC *configsPtr
)
*/
UTF_TEST_CASE_MAC(cpssDxChPortTxShaperConfigurationGet)
{
/*
    ITERATE_DEVICES(Lion)
    1.1. Call with non NULL configsPtr.
    Expected: GT_OK.
    1.2. Call with configsPtr [NULL],
                   other params same as in 1.1.
    Expected: GT_BAD_PTR.
*/
    GT_STATUS   st = GT_OK;
    GT_U8       dev;

    CPSS_DXCH_PORT_TX_SHAPER_CONFIG_STC configs = {0};


    /* prepare iterator for go over all active devices */
    PRV_TGF_NOT_APPLIC_DEV_RESET_MAC(&dev, UTF_CH1_E | UTF_CH1_DIAMOND_E | UTF_CH2_E | UTF_CH3_E | UTF_XCAT_E | UTF_XCAT2_E);

    /* 1. Go over all active devices. */
    while (GT_OK == prvUtfNextDeviceGet(&dev, GT_TRUE))
    {
        /*
            1.1. Call with non NULL configsPtr.
            Expected: GT_OK.
        */
        st = cpssDxChPortTxShaperConfigurationGet(dev, &configs);
        UTF_VERIFY_EQUAL1_PARAM_MAC(GT_OK, st, dev);

        /*
            1.8. Call with configsPtr [NULL],
                           other params same as in 1.1.1.
            Expected: GT_BAD_PTR.
        */
        st = cpssDxChPortTxShaperConfigurationGet(dev, NULL);
        UTF_VERIFY_EQUAL1_STRING_MAC(GT_BAD_PTR, st, "%d, configsPtr = NULL", dev);
    }

    /* 2. For not active devices check that function returns non GT_OK.*/
    /* prepare device iterator */
    PRV_TGF_NOT_APPLIC_DEV_RESET_MAC(&dev, UTF_CH1_E | UTF_CH1_DIAMOND_E | UTF_CH2_E | UTF_CH3_E | UTF_XCAT_E | UTF_XCAT2_E);

    /* Go over all non active devices. */
    while(GT_OK == prvUtfNextDeviceGet(&dev, GT_FALSE))
    {
        st = cpssDxChPortTxShaperConfigurationGet(dev, &configs);
        UTF_VERIFY_EQUAL1_PARAM_MAC(GT_NOT_APPLICABLE_DEVICE, st, dev);
    }

    /* 3.Call function with out of bound value for device id */
    dev = PRV_CPSS_MAX_PP_DEVICES_CNS;

    st = cpssDxChPortTxShaperConfigurationGet(dev, &configs);
    UTF_VERIFY_EQUAL1_PARAM_MAC(GT_BAD_PARAM, st, dev);
}

/*----------------------------------------------------------------------------*/
/*
GT_STATUS cpssDxChPortTxShaperBaselineSet
(
    IN GT_U8    devNum,
    IN GT_U32   baseline
)
*/
UTF_TEST_CASE_MAC(cpssDxChPortTxShaperBaselineSet)
{
/*
    ITERATE_DEVICES(Lion)
    1.1. Call with baseline [0 / 1000 / 0xFFFFFF]
    Expected: GT_OK.
    1.2. Call cpssDxChPortTxShaperBaselineGet with non-NULL pointers
                                                   other params same as in 1.1.
    Expected: GT_OK and the same values.
    1.3. Call with out of range baseline [0xFFFFFF + 1],
                   other params same as in 1.1.
    Expected: NOT GT_OK.
*/
    GT_STATUS   st = GT_OK;
    GT_U8       dev;

    GT_U32      baseline    = 0;
    GT_U32      baselineGet = 0;


    /* prepare iterator for go over all active devices */
    PRV_TGF_NOT_APPLIC_DEV_RESET_MAC(&dev, UTF_CH1_E | UTF_CH1_DIAMOND_E | UTF_CH2_E | UTF_CH3_E | UTF_XCAT_E | UTF_XCAT2_E);

    /* 1. Go over all active devices. */
    while (GT_OK == prvUtfNextDeviceGet(&dev, GT_TRUE))
    {
        /*
            1.1. Call with baseline [0 / 1000 / 0xFFFFFF]
            Expected: GT_OK.
        */
        baseline = 0;

        st = cpssDxChPortTxShaperBaselineSet(dev, baseline);
        UTF_VERIFY_EQUAL2_PARAM_MAC(GT_OK, st, dev, baseline);

        /*
            1.2. Call cpssDxChPortTxShaperBaselineGet with non-NULL pointers
                                                           other params same as in 1.1.
            Expected: GT_OK and the same values.
        */
        st = cpssDxChPortTxShaperBaselineGet(dev, &baselineGet);
        UTF_VERIFY_EQUAL1_STRING_MAC(GT_OK, st,
                   "cpssDxChPortTxShaperBaselineGet: %d", dev);

        /* verifying values */
        UTF_VERIFY_EQUAL1_STRING_MAC(baseline, baselineGet,
                   "got another baseline then was set: %d", dev);

        /*
            1.1. Call with baseline [0 / 1000 / 0xFFFFFF]
            Expected: GT_OK.
        */
        baseline = 1000;

        st = cpssDxChPortTxShaperBaselineSet(dev, baseline);
        UTF_VERIFY_EQUAL2_PARAM_MAC(GT_OK, st, dev, baseline);

        /*
            1.2. Call cpssDxChPortTxShaperBaselineGet with non-NULL pointers
                                                           other params same as in 1.1.
            Expected: GT_OK and the same values.
        */
        st = cpssDxChPortTxShaperBaselineGet(dev, &baselineGet);
        UTF_VERIFY_EQUAL1_STRING_MAC(GT_OK, st,
                   "cpssDxChPortTxShaperBaselineGet: %d", dev);

        /* verifying values */
        UTF_VERIFY_EQUAL1_STRING_MAC(baseline, baselineGet,
                   "got another baseline then was set: %d", dev);

        /*
            1.1. Call with baseline [0 / 1000 / 0xFFFFFF]
            Expected: GT_OK.
        */
        baseline = 0xFFFFFF;

        st = cpssDxChPortTxShaperBaselineSet(dev, baseline);
        UTF_VERIFY_EQUAL2_PARAM_MAC(GT_OK, st, dev, baseline);

        /*
            1.2. Call cpssDxChPortTxShaperBaselineGet with non-NULL pointers
                                                           other params same as in 1.1.
            Expected: GT_OK and the same values.
        */
        st = cpssDxChPortTxShaperBaselineGet(dev, &baselineGet);
        UTF_VERIFY_EQUAL1_STRING_MAC(GT_OK, st,
                   "cpssDxChPortTxShaperBaselineGet: %d", dev);

        /* verifying values */
        UTF_VERIFY_EQUAL1_STRING_MAC(baseline, baselineGet,
                   "got another baseline then was set: %d", dev);

        /*
            1.3. Call with out of range baseline [0xFFFFFF + 1],
                           other params same as in 1.1.
            Expected: NOT GT_OK.
        */
        baseline = 0xFFFFFF + 1;

        st = cpssDxChPortTxShaperBaselineSet(dev, baseline);
        UTF_VERIFY_NOT_EQUAL2_PARAM_MAC(GT_OK, st, dev, baseline);
    }

    baseline = 0;

    /* 2. For not active devices check that function returns non GT_OK.*/
    /* prepare device iterator */
    PRV_TGF_NOT_APPLIC_DEV_RESET_MAC(&dev, UTF_CH1_E | UTF_CH1_DIAMOND_E | UTF_CH2_E | UTF_CH3_E | UTF_XCAT_E | UTF_XCAT2_E);

    /* Go over all non active devices. */
    while(GT_OK == prvUtfNextDeviceGet(&dev, GT_FALSE))
    {
        st = cpssDxChPortTxShaperBaselineSet(dev, baseline);
        UTF_VERIFY_EQUAL1_PARAM_MAC(GT_NOT_APPLICABLE_DEVICE, st, dev);
    }

    /* 3.Call function with out of bound value for device id */
    dev = PRV_CPSS_MAX_PP_DEVICES_CNS;

    st = cpssDxChPortTxShaperBaselineSet(dev, baseline);
    UTF_VERIFY_EQUAL1_PARAM_MAC(GT_BAD_PARAM, st, dev);
}

/*----------------------------------------------------------------------------*/
/*
GT_STATUS cpssDxChPortTxShaperBaselineGet
(
    IN  GT_U8    devNum,
    OUT GT_U32   *baselinePtr
)
*/
UTF_TEST_CASE_MAC(cpssDxChPortTxShaperBaselineGet)
{
/*
    ITERATE_DEVICES(Lion)
    1.1. Call with non NULL baselinePtr.
    Expected: GT_OK.
    1.2. Call with baselinePtr [NULL],
                   other params same as in 1.1.
    Expected: GT_BAD_PTR.
*/
    GT_STATUS   st = GT_OK;
    GT_U8       dev;

    GT_U32      baseline = 0;


    /* prepare iterator for go over all active devices */
    PRV_TGF_NOT_APPLIC_DEV_RESET_MAC(&dev, UTF_CH1_E | UTF_CH1_DIAMOND_E | UTF_CH2_E | UTF_CH3_E | UTF_XCAT_E | UTF_XCAT2_E);

    /* 1. Go over all active devices. */
    while (GT_OK == prvUtfNextDeviceGet(&dev, GT_TRUE))
    {
        /*
            1.1. Call with non NULL baselinePtr.
            Expected: GT_OK.
        */
        st = cpssDxChPortTxShaperBaselineGet(dev, &baseline);
        UTF_VERIFY_EQUAL1_PARAM_MAC(GT_OK, st, dev);

        /*
            1.2. Call with baselinePtr [NULL],
                           other params same as in 1.1.
            Expected: GT_BAD_PTR.
        */
        st = cpssDxChPortTxShaperBaselineGet(dev, NULL);
        UTF_VERIFY_EQUAL2_PARAM_MAC(GT_BAD_PTR, st, "%d, baselinePtr = NULL", dev);
    }

    /* 2. For not active devices check that function returns non GT_OK.*/
    /* prepare device iterator */
    PRV_TGF_NOT_APPLIC_DEV_RESET_MAC(&dev, UTF_CH1_E | UTF_CH1_DIAMOND_E | UTF_CH2_E | UTF_CH3_E | UTF_XCAT_E | UTF_XCAT2_E);

    /* Go over all non active devices. */
    while(GT_OK == prvUtfNextDeviceGet(&dev, GT_FALSE))
    {
        st = cpssDxChPortTxShaperBaselineGet(dev, &baseline);
        UTF_VERIFY_EQUAL1_PARAM_MAC(GT_NOT_APPLICABLE_DEVICE, st, dev);
    }

    /* 3.Call function with out of bound value for device id */
    dev = PRV_CPSS_MAX_PP_DEVICES_CNS;

    st = cpssDxChPortTxShaperBaselineGet(dev, &baseline);
    UTF_VERIFY_EQUAL1_PARAM_MAC(GT_BAD_PARAM, st, dev);
}

/*----------------------------------------------------------------------------*/
/*
GT_STATUS cpssDxChPortTxSchedulerDeficitModeEnableSet
(
    IN  GT_U8     devNum,
    IN  GT_BOOL   enable
)
*/
UTF_TEST_CASE_MAC(cpssDxChPortTxSchedulerDeficitModeEnableSet)
{
/*
    ITERATE_DEVICES(Lion)
    1.1. Call with enable [GT_FALSE and GT_TRUE].
    Expected: GT_OK.
    1.2. Call cpssDxChPortTxSchedulerDeficitModeEnableGet with non-NULL pointers
                                                               other params same as in 1.1.
    Expected: GT_OK and the same values.
*/
    GT_STATUS   st = GT_OK;
    GT_U8       dev;

    GT_BOOL     enable    = GT_FALSE;
    GT_BOOL     enableGet = GT_FALSE;


    /* prepare device iterator */
    PRV_TGF_NOT_APPLIC_DEV_RESET_MAC(&dev, UTF_CH1_E | UTF_CH1_DIAMOND_E | UTF_CH2_E | UTF_CH3_E | UTF_XCAT_E | UTF_XCAT2_E);

    /* 1. Go over all active devices. */
    while(GT_OK == prvUtfNextDeviceGet(&dev, GT_TRUE))
    {
        /*
            1.1. Call with enable [GT_FALSE and GT_TRUE].
            Expected: GT_OK.
        */
        enable = GT_FALSE;

        st = cpssDxChPortTxSchedulerDeficitModeEnableSet(dev, enable);
        UTF_VERIFY_EQUAL2_PARAM_MAC(GT_OK, st, dev, enable);

        /*
            1.2. Call cpssDxChPortTxSchedulerDeficitModeEnableGet with non-NULL pointers
                                                                       other params same as in 1.1.
            Expected: GT_OK and the same values.
        */
        st = cpssDxChPortTxSchedulerDeficitModeEnableGet(dev, &enableGet);
        UTF_VERIFY_EQUAL1_STRING_MAC(GT_OK, st,
                   "cpssDxChPortTxSchedulerDeficitModeEnableGet: %d", dev);

        /* Verifying values */
        UTF_VERIFY_EQUAL1_STRING_MAC(enable, enableGet,
                   "get another enable than was set: %d", dev);

        /* Call with enable [GT_TRUE] */
        enable = GT_TRUE;

        st = cpssDxChPortTxSchedulerDeficitModeEnableSet(dev, enable);
        UTF_VERIFY_EQUAL2_PARAM_MAC(GT_OK, st, dev, enable);

        /*
            1.2. Call cpssDxChPortTxSchedulerDeficitModeEnableGet with non-NULL pointers
                                                                       other params same as in 1.1.
            Expected: GT_OK and the same values.
        */
        st = cpssDxChPortTxSchedulerDeficitModeEnableGet(dev, &enableGet);
        UTF_VERIFY_EQUAL1_STRING_MAC(GT_OK, st,
                   "cpssDxChPortTxSchedulerDeficitModeEnableGet: %d", dev);

        /* Verifying values */
        UTF_VERIFY_EQUAL1_STRING_MAC(enable, enableGet,
                   "get another enable than was set: %d", dev);
    }

    enable = GT_FALSE;

    /* 2. For not-active devices and devices from non-applicable family */
    /* check that function returns GT_BAD_PARAM.                        */

    /* prepare device iterator */
    PRV_TGF_NOT_APPLIC_DEV_RESET_MAC(&dev, UTF_CH1_E | UTF_CH1_DIAMOND_E | UTF_CH2_E | UTF_CH3_E | UTF_XCAT_E | UTF_XCAT2_E);

    /* go over all non active devices */
    while(GT_OK == prvUtfNextDeviceGet(&dev, GT_FALSE))
    {
        st = cpssDxChPortTxSchedulerDeficitModeEnableSet(dev, enable);
        UTF_VERIFY_EQUAL1_PARAM_MAC(GT_NOT_APPLICABLE_DEVICE, st, dev);
    }

    /* 3. Call function with out of bound value for device id.*/
    dev = PRV_CPSS_MAX_PP_DEVICES_CNS;

    st = cpssDxChPortTxSchedulerDeficitModeEnableSet(dev, enable);
    UTF_VERIFY_EQUAL1_PARAM_MAC(GT_BAD_PARAM, st, dev);
}

/*----------------------------------------------------------------------------*/
/*
GT_STATUS cpssDxChPortTxSchedulerDeficitModeEnableGet
(
    IN  GT_U8     devNum,
    OUT GT_BOOL   *enablePtr
)
*/
UTF_TEST_CASE_MAC(cpssDxChPortTxSchedulerDeficitModeEnableGet)
{
/*
    ITERATE_DEVICES(Lion)
    1.1. Call with non NUILL enablePtr.
    Expected: GT_OK.
    1.2. Call with enablePtr [NULL],
                   other params same as in 1.1.
    Expected: GT_BAD_PTR.
*/
    GT_STATUS   st = GT_OK;
    GT_U8       dev;

    GT_BOOL     enable = GT_FALSE;


    /* prepare device iterator */
    PRV_TGF_NOT_APPLIC_DEV_RESET_MAC(&dev, UTF_CH1_E | UTF_CH1_DIAMOND_E | UTF_CH2_E | UTF_CH3_E | UTF_XCAT_E | UTF_XCAT2_E);

    /* 1. Go over all active devices. */
    while(GT_OK == prvUtfNextDeviceGet(&dev, GT_TRUE))
    {
        /*
            1.1. Call with non NUILL enablePtr.
            Expected: GT_OK.
        */
        st = cpssDxChPortTxSchedulerDeficitModeEnableGet(dev, &enable);
        UTF_VERIFY_EQUAL1_PARAM_MAC(GT_OK, st, dev);

        /*
            1.2. Call with enablePtr [NULL],
                           other params same as in 1.1.
            Expected: GT_BAD_PTR.
        */
        st = cpssDxChPortTxSchedulerDeficitModeEnableGet(dev, NULL);
        UTF_VERIFY_EQUAL1_STRING_MAC(GT_BAD_PTR, st, "%d, enablePtr = NULL", dev);
    }

    enable = GT_FALSE;

    /* 2. For not-active devices and devices from non-applicable family */
    /* check that function returns GT_BAD_PARAM.                        */

    /* prepare device iterator */
    PRV_TGF_NOT_APPLIC_DEV_RESET_MAC(&dev, UTF_CH1_E | UTF_CH1_DIAMOND_E | UTF_CH2_E | UTF_CH3_E | UTF_XCAT_E | UTF_XCAT2_E);

    /* go over all non active devices */
    while(GT_OK == prvUtfNextDeviceGet(&dev, GT_FALSE))
    {
        st = cpssDxChPortTxSchedulerDeficitModeEnableGet(dev, &enable);
        UTF_VERIFY_EQUAL1_PARAM_MAC(GT_NOT_APPLICABLE_DEVICE, st, dev);
    }

    /* 3. Call function with out of bound value for device id.*/
    dev = PRV_CPSS_MAX_PP_DEVICES_CNS;

    st = cpssDxChPortTxSchedulerDeficitModeEnableGet(dev, &enable);
    UTF_VERIFY_EQUAL1_PARAM_MAC(GT_BAD_PARAM, st, dev);
}

/*----------------------------------------------------------------------------*/
/*
GT_STATUS cpssDxChPortTxRequestMaskSet
(
    IN GT_U8    devNum,
    IN GT_U32   maskIndex,
    IN GT_U32   maskValue
)
*/
UTF_TEST_CASE_MAC(cpssDxChPortTxRequestMaskSet)
{
/*
    ITERATE_DEVICES (Lion)
    1.1. Call with maskIndex [0 / 1 / 3],
                   maskValue [3 / 100 / 255].
    Expected: GT_OK.
    1.2. Call cpssDxChPortTxRequestMaskGet with non-NULL pointers
                                                other params same as in 1.1.
    Expected: GT_OK and the same values.
    1.3. Call with out of range maskIndex [4],
                   other params same as in 1.1.
    Expected: NOT GT_OK.
    1.4. Call with out of range maskValue [0 / 256],
                   other params same as in 1.1.
    Expected: NOT GT_OK.
*/
    GT_STATUS   st = GT_OK;
    GT_U8       dev;

    GT_U32      maskIndex    = 0;
    GT_U32      maskValue    = 0;
    GT_U32      maskValueGet = 0;


    /* prepare iterator for go over all active devices */
    PRV_TGF_NOT_APPLIC_DEV_RESET_MAC(&dev, UTF_CH1_E | UTF_CH1_DIAMOND_E | UTF_CH2_E | UTF_CH3_E | UTF_XCAT_E | UTF_XCAT2_E);

    /* 1. Go over all active devices. */
    while (GT_OK == prvUtfNextDeviceGet(&dev, GT_TRUE))
    {
        /*
            1.1. Call with maskIndex [0 / 1 / 3],
                           maskValue [3 / 100 / 255].
            Expected: GT_OK.
        */
        maskIndex = 0;
        maskValue = 3;

        st = cpssDxChPortTxRequestMaskSet(dev, maskIndex, maskValue);
        UTF_VERIFY_EQUAL3_PARAM_MAC(GT_OK, st, dev, maskIndex, maskValue);

        /*
            1.2. Call cpssDxChPortTxRequestMaskGet with non-NULL pointers
                                                        other params same as in 1.1.
            Expected: GT_OK and the same values.
        */
        st = cpssDxChPortTxRequestMaskGet(dev, maskIndex, &maskValueGet);
        UTF_VERIFY_EQUAL1_STRING_MAC(GT_OK, st,
                   "cpssDxChPortTxRequestMaskGet: %d", dev);

        /* verifying values */
        UTF_VERIFY_EQUAL1_STRING_MAC(maskValue, maskValueGet,
                   "got another maskValue then was set: %d", dev);

        /*
            1.1. Call with maskIndex [0 / 1 / 3],
                           maskValue [3 / 100 / 255].
            Expected: GT_OK.
        */
        maskIndex = 1;
        maskValue = 100;

        st = cpssDxChPortTxRequestMaskSet(dev, maskIndex, maskValue);
        UTF_VERIFY_EQUAL3_PARAM_MAC(GT_OK, st, dev, maskIndex, maskValue);

        /*
            1.2. Call cpssDxChPortTxRequestMaskGet with non-NULL pointers
                                                        other params same as in 1.1.
            Expected: GT_OK and the same values.
        */
        st = cpssDxChPortTxRequestMaskGet(dev, maskIndex, &maskValueGet);
        UTF_VERIFY_EQUAL1_STRING_MAC(GT_OK, st,
                   "cpssDxChPortTxRequestMaskGet: %d", dev);

        /* verifying values */
        UTF_VERIFY_EQUAL1_STRING_MAC(maskValue, maskValueGet,
                   "got another maskValue then was set: %d", dev);

        /*
            1.1. Call with maskIndex [0 / 1 / 3],
                           maskValue [3 / 100 / 255].
            Expected: GT_OK.
        */
        maskIndex = 3;
        maskValue = 255;

        st = cpssDxChPortTxRequestMaskSet(dev, maskIndex, maskValue);
        UTF_VERIFY_EQUAL3_PARAM_MAC(GT_OK, st, dev, maskIndex, maskValue);

        /*
            1.2. Call cpssDxChPortTxRequestMaskGet with non-NULL pointers
                                                        other params same as in 1.1.
            Expected: GT_OK and the same values.
        */
        st = cpssDxChPortTxRequestMaskGet(dev, maskIndex, &maskValueGet);
        UTF_VERIFY_EQUAL1_STRING_MAC(GT_OK, st,
                   "cpssDxChPortTxRequestMaskGet: %d", dev);

        /* verifying values */
        UTF_VERIFY_EQUAL1_STRING_MAC(maskValue, maskValueGet,
                   "got another maskValue then was set: %d", dev);

        /*
            1.3. Call with out of range maskIndex [4],
                           other params same as in 1.1.
            Expected: NOT GT_OK.
        */
        maskIndex = 4;

        st = cpssDxChPortTxRequestMaskSet(dev, maskIndex, maskValue);
        UTF_VERIFY_NOT_EQUAL2_PARAM_MAC(GT_OK, st, dev, maskIndex);

        maskIndex = 0;

        /*
            1.4. Call with out of range maskValue [0 / 256],
                           other params same as in 1.1.
            Expected: NOT GT_OK.
        */

        /* call with maskValue = 0 */
        maskValue = 0;

        st = cpssDxChPortTxRequestMaskSet(dev, maskIndex, maskValue);
        UTF_VERIFY_NOT_EQUAL2_PARAM_MAC(GT_OK, st, dev, maskValue);

        /* call with maskValue = 256 */
        maskValue = 256;

        st = cpssDxChPortTxRequestMaskSet(dev, maskIndex, maskValue);
        UTF_VERIFY_NOT_EQUAL2_PARAM_MAC(GT_OK, st, dev, maskValue);
    }

    maskIndex = 0;
    maskValue = 3;

    /* 2. For not active devices check that function returns non GT_OK.*/
    /* prepare device iterator */
    PRV_TGF_NOT_APPLIC_DEV_RESET_MAC(&dev, UTF_CH1_E | UTF_CH1_DIAMOND_E | UTF_CH2_E | UTF_CH3_E | UTF_XCAT_E | UTF_XCAT2_E);

    /* Go over all non active devices. */
    while(GT_OK == prvUtfNextDeviceGet(&dev, GT_FALSE))
    {
        st = cpssDxChPortTxRequestMaskSet(dev, maskIndex, maskValue);
        UTF_VERIFY_EQUAL1_PARAM_MAC(GT_NOT_APPLICABLE_DEVICE, st, dev);
    }

    /* 3.Call function with out of bound value for device id */
    dev = PRV_CPSS_MAX_PP_DEVICES_CNS;

    st = cpssDxChPortTxRequestMaskSet(dev, maskIndex, maskValue);
    UTF_VERIFY_EQUAL1_PARAM_MAC(GT_BAD_PARAM, st, dev);
}

/*----------------------------------------------------------------------------*/
/*
GT_STATUS cpssDxChPortTxRequestMaskGet
(
    IN  GT_U8    devNum,
    IN  GT_U32   maskIndex,
    OUT GT_U32  *maskValuePtr
)
*/
UTF_TEST_CASE_MAC(cpssDxChPortTxRequestMaskGet)
{
/*
    ITERATE_DEVICES(Lion)
    1.1. Call with maskIndex [0..3],
                   non NULL maskValuePtr.
    Expected: GT_OK.
    1.2. Call with out of range maskIndex [4],
                   other params same as in 1.1.
    Expected: NOT GT_OK.
    1.3. Call with maskValuePtr [NULL],
                   other params same as in 1.1.
    Expected: GT_BAD_PTR.
*/
    GT_STATUS   st = GT_OK;
    GT_U8       dev;

    GT_U32      maskIndex = 0;
    GT_U32      maskValue = 0;


    /* prepare iterator for go over all active devices */
    PRV_TGF_NOT_APPLIC_DEV_RESET_MAC(&dev, UTF_CH1_E | UTF_CH1_DIAMOND_E | UTF_CH2_E | UTF_CH3_E | UTF_XCAT_E | UTF_XCAT2_E);

    /* 1. Go over all active devices. */
    while (GT_OK == prvUtfNextDeviceGet(&dev, GT_TRUE))
    {
        /*
            1.1. Call with maskIndex [0..3],
                           non NULL maskValuePtr.
            Expected: GT_OK.
        */

        for (maskIndex = 0; maskIndex <= 3; maskIndex++)
        {
            st = cpssDxChPortTxRequestMaskGet(dev, maskIndex, &maskValue);
            UTF_VERIFY_EQUAL2_PARAM_MAC(GT_OK, st, dev, maskIndex);
        }

        /*
            1.2. Call with out of range maskIndex [4],
                           other params same as in 1.1.
            Expected: NOT GT_OK.
        */
        maskIndex = 4;

        st = cpssDxChPortTxRequestMaskGet(dev, maskIndex, &maskValue);
        UTF_VERIFY_NOT_EQUAL2_PARAM_MAC(GT_OK, st, dev, maskIndex);

        maskIndex = 0;

        /*
            1.3. Call with maskValuePtr [NULL],
                           other params same as in 1.1.
            Expected: GT_BAD_PTR.
        */
        st = cpssDxChPortTxRequestMaskGet(dev, maskIndex, NULL);
        UTF_VERIFY_EQUAL1_STRING_MAC(GT_BAD_PTR, st, "%d, maskValuePtr = NULL", dev);
    }

    maskIndex = 0;

    /* 2. For not active devices check that function returns non GT_OK.*/
    /* prepare device iterator */
    PRV_TGF_NOT_APPLIC_DEV_RESET_MAC(&dev, UTF_CH1_E | UTF_CH1_DIAMOND_E | UTF_CH2_E | UTF_CH3_E | UTF_XCAT_E | UTF_XCAT2_E);

    /* Go over all non active devices. */
    while(GT_OK == prvUtfNextDeviceGet(&dev, GT_FALSE))
    {
        st = cpssDxChPortTxRequestMaskGet(dev, maskIndex, &maskValue);
        UTF_VERIFY_EQUAL1_PARAM_MAC(GT_NOT_APPLICABLE_DEVICE, st, dev);
    }

    /* 3.Call function with out of bound value for device id */
    dev = PRV_CPSS_MAX_PP_DEVICES_CNS;

    st = cpssDxChPortTxRequestMaskGet(dev, maskIndex, &maskValue);
    UTF_VERIFY_EQUAL1_PARAM_MAC(GT_BAD_PARAM, st, dev);
}

/*----------------------------------------------------------------------------*/
/*
GT_STATUS cpssDxChPortTxRequestMaskIndexSet
(
    IN GT_U8    devNum,
    IN GT_U8    portNum,
    IN GT_U32   maskIndex
)
*/
UTF_TEST_CASE_MAC(cpssDxChPortTxRequestMaskIndexSet)
{
/*
    ITERATE_DEVICES_PHY_PORTS (Lion)
    1.1.1. Call with maskIndex [0..3].
    Expected: GT_OK.
    1.1.2. Call cpssDxChPortTxRequestMaskIndexGet with non-NULL pointers
                                                       other params same as in 1.1.1.
    Expected: GT_OK and the same values.
    1.1.3. Call with out of range maskIndex [4],
                     other params same as in 1.1.1.
    Expected: NOT GT_OK.
*/
    GT_STATUS   st   = GT_OK;
    GT_U8       dev;
    GT_U8       port = PORT_TX_VALID_PHY_PORT_CNS;

    GT_U32      maskIndex    = 0;
    GT_U32      maskIndexGet = 0;


    /* prepare iterator for go over all active devices */
    PRV_TGF_NOT_APPLIC_DEV_RESET_MAC(&dev, UTF_CH1_E | UTF_CH1_DIAMOND_E | UTF_CH2_E | UTF_CH3_E | UTF_XCAT_E | UTF_XCAT2_E);

    /* 1. Go over all active devices. */
    while (GT_OK == prvUtfNextDeviceGet(&dev, GT_TRUE))
    {
        st = prvUtfNextPhyPortReset(&port, dev);
        UTF_VERIFY_EQUAL0_PARAM_MAC(GT_OK, st);

        /* 1.1. For all active devices go over all available physical ports. */
        while (GT_OK == prvUtfNextPhyPortGet(&port, GT_TRUE))
        {
            /*
                1.1.1. Call with maskIndex [0..3].
                Expected: GT_OK.
            */
            for (maskIndex = 0; maskIndex <= 3; maskIndex++)
            {
                st = cpssDxChPortTxRequestMaskIndexSet(dev, port, maskIndex);
                UTF_VERIFY_EQUAL3_PARAM_MAC(GT_OK, st, dev, port, maskIndex);

                /*
                    1.1.2. Call cpssDxChPortTxRequestMaskIndexGet with non-NULL pointers
                                                                       other params same as in 1.1.1.
                    Expected: GT_OK and the same values.
                */
                st = cpssDxChPortTxRequestMaskIndexGet(dev, port, &maskIndexGet);
                UTF_VERIFY_EQUAL1_STRING_MAC(GT_OK, st,
                           "cpssDxChPortTxRequestMaskIndexGet: %d", dev);

                /* verifying values */
                UTF_VERIFY_EQUAL1_STRING_MAC(maskIndex, maskIndexGet,
                           "got another maskIndex then was set: %d", dev);
            }

            /*
                1.1.3. Call with out of range maskIndex [4],
                                 other params same as in 1.1.1.
                Expected: NOT GT_OK.
            */
            maskIndex = 4;

            st = cpssDxChPortTxRequestMaskIndexSet(dev, port, maskIndex);
            UTF_VERIFY_NOT_EQUAL2_PARAM_MAC(GT_OK, st, dev, maskIndex);
        }

        maskIndex = 0;

        st = prvUtfNextPhyPortReset(&port, dev);
        UTF_VERIFY_EQUAL0_PARAM_MAC(GT_OK, st);

        /* 1.2. For all active devices go over all non available physical ports. */
        while (GT_OK == prvUtfNextPhyPortGet(&port, GT_FALSE))
        {
            /* 1.2.1. Call function for each non-active port */
            st = cpssDxChPortTxRequestMaskIndexSet(dev, port, maskIndex);
            UTF_VERIFY_EQUAL2_PARAM_MAC(GT_BAD_PARAM, st, dev, port);
        }

        /* 1.3. For active device check that function returns GT_BAD_PARAM */
        /* for out of bound value for port number.                         */
        port = PRV_CPSS_MAX_PP_PORTS_NUM_CNS;

        st = cpssDxChPortTxRequestMaskIndexSet(dev, port, maskIndex);
        UTF_VERIFY_EQUAL2_PARAM_MAC(GT_BAD_PARAM, st, dev, port);

        /* 1.4. For active device check that function returns GT_BAD_PARAM */
        /* for CPU port number.                                            */
        port = CPSS_CPU_PORT_NUM_CNS;

        st = cpssDxChPortTxRequestMaskIndexSet(dev, port, maskIndex);
        UTF_VERIFY_EQUAL2_PARAM_MAC(GT_BAD_PARAM, st, dev, port);
    }

    maskIndex = 0;

    /* 2. For not active devices check that function returns non GT_OK.*/
    port = PORT_TX_VALID_PHY_PORT_CNS;

    /* prepare device iterator */
    PRV_TGF_NOT_APPLIC_DEV_RESET_MAC(&dev, UTF_CH1_E | UTF_CH1_DIAMOND_E | UTF_CH2_E | UTF_CH3_E | UTF_XCAT_E | UTF_XCAT2_E);

    /* Go over all non active devices. */
    while(GT_OK == prvUtfNextDeviceGet(&dev, GT_FALSE))
    {
        st = cpssDxChPortTxRequestMaskIndexSet(dev, port, maskIndex);
        UTF_VERIFY_EQUAL1_PARAM_MAC(GT_NOT_APPLICABLE_DEVICE, st, dev);
    }

    /* 3.Call function with out of bound value for device id */
    dev = PRV_CPSS_MAX_PP_DEVICES_CNS;

    st = cpssDxChPortTxRequestMaskIndexSet(dev, port, maskIndex);
    UTF_VERIFY_EQUAL1_PARAM_MAC(GT_BAD_PARAM, st, dev);
}

/*----------------------------------------------------------------------------*/
/*
GT_STATUS cpssDxChPortTxRequestMaskIndexGet
(
    IN  GT_U8    devNum,
    IN  GT_U8    portNum,
    OUT GT_U32   *maskIndexPtr
)
*/
UTF_TEST_CASE_MAC(cpssDxChPortTxRequestMaskIndexGet)
{
/*
    ITERATE_DEVICES_PHY_PORTS (Lion)
    1.1.1. Call with non NULL maskIndexPtr.
    Expected: GT_OK.
    1.1.2. Call with maskIndexPtr [NULL],
                     other params same as in 1.1.
    Expected: GT_BAD_PTR.
*/
    GT_STATUS   st   = GT_OK;
    GT_U8       dev;
    GT_U8       port = PORT_TX_VALID_PHY_PORT_CNS;

    GT_U32      maskIndex = 0;


    /* prepare iterator for go over all active devices */
    PRV_TGF_NOT_APPLIC_DEV_RESET_MAC(&dev, UTF_CH1_E | UTF_CH1_DIAMOND_E | UTF_CH2_E | UTF_CH3_E | UTF_XCAT_E | UTF_XCAT2_E);

    /* 1. Go over all active devices. */
    while (GT_OK == prvUtfNextDeviceGet(&dev, GT_TRUE))
    {
        st = prvUtfNextPhyPortReset(&port, dev);
        UTF_VERIFY_EQUAL0_PARAM_MAC(GT_OK, st);

        /* 1.1. For all active devices go over all available physical ports. */
        while (GT_OK == prvUtfNextPhyPortGet(&port, GT_TRUE))
        {
            /*
                1.1.1. Call with non NULL maskIndexPtr.
                Expected: GT_OK.
            */
            st = cpssDxChPortTxRequestMaskIndexGet(dev, port, &maskIndex);
            UTF_VERIFY_EQUAL2_PARAM_MAC(GT_OK, st, dev, port);

            /*
                1.1.2. Call with maskIndexPtr [NULL],
                                 other params same as in 1.1.
                Expected: GT_BAD_PTR.
            */
            st = cpssDxChPortTxRequestMaskIndexGet(dev, port, NULL);
            UTF_VERIFY_EQUAL2_PARAM_MAC(GT_BAD_PTR, st, "%d, maskIndexPtr = NULL", dev);
        }

        st = prvUtfNextPhyPortReset(&port, dev);
        UTF_VERIFY_EQUAL0_PARAM_MAC(GT_OK, st);

        /* 1.2. For all active devices go over all non available physical ports. */
        while (GT_OK == prvUtfNextPhyPortGet(&port, GT_FALSE))
        {
            /* 1.2.1. Call function for each non-active port */
            st = cpssDxChPortTxRequestMaskIndexGet(dev, port, &maskIndex);
            UTF_VERIFY_EQUAL2_PARAM_MAC(GT_BAD_PARAM, st, dev, port);
        }

        /* 1.3. For active device check that function returns GT_BAD_PARAM */
        /* for out of bound value for port number.                         */
        port = PRV_CPSS_MAX_PP_PORTS_NUM_CNS;

        st = cpssDxChPortTxRequestMaskIndexGet(dev, port, &maskIndex);
        UTF_VERIFY_EQUAL2_PARAM_MAC(GT_BAD_PARAM, st, dev, port);

        /* 1.4. For active device check that function returns GT_BAD_PARAM */
        /* for CPU port number.                                            */
        port = CPSS_CPU_PORT_NUM_CNS;

        st = cpssDxChPortTxRequestMaskIndexGet(dev, port, &maskIndex);
        UTF_VERIFY_EQUAL2_PARAM_MAC(GT_BAD_PARAM, st, dev, port);
    }

    /* 2. For not active devices check that function returns non GT_OK.*/
    port = PORT_TX_VALID_PHY_PORT_CNS;

    /* prepare device iterator */
    PRV_TGF_NOT_APPLIC_DEV_RESET_MAC(&dev, UTF_CH1_E | UTF_CH1_DIAMOND_E | UTF_CH2_E | UTF_CH3_E | UTF_XCAT_E | UTF_XCAT2_E);

    /* Go over all non active devices. */
    while(GT_OK == prvUtfNextDeviceGet(&dev, GT_FALSE))
    {
        st = cpssDxChPortTxRequestMaskIndexGet(dev, port, &maskIndex);
        UTF_VERIFY_EQUAL1_PARAM_MAC(GT_NOT_APPLICABLE_DEVICE, st, dev);
    }

    /* 3.Call function with out of bound value for device id */
    dev = PRV_CPSS_MAX_PP_DEVICES_CNS;

    st = cpssDxChPortTxRequestMaskIndexGet(dev, port, &maskIndex);
    UTF_VERIFY_EQUAL1_PARAM_MAC(GT_BAD_PARAM, st, dev);
}

/*----------------------------------------------------------------------------*/
/*
GT_STATUS cpssDxChPortTxGlobalBufNumberGet
(
    IN  GT_U8   dev,
    OUT GT_U32  *numberPtr
)
*/
UTF_TEST_CASE_MAC(cpssDxChPortTxGlobalBufNumberGet)
{
/*
    ITERATE_DEVICES(DxCh3 and above)
    1.1. Call with not null numberPtr.
    Expected: GT_OK.
    1.2. Call api with wrong numberPtr [NULL].
    Expected: GT_BAD_PTR.
*/
    GT_STATUS st = GT_OK;

    GT_U8   dev;
    GT_U32  number;

    /* prepare device iterator */
    PRV_TGF_NOT_APPLIC_DEV_RESET_MAC(&dev, UTF_CH1_E | UTF_CH1_DIAMOND_E | UTF_CH2_E);

    /* 1. Go over all active devices. */
    while (GT_OK == prvUtfNextDeviceGet(&dev, GT_TRUE))
    {
        /*
            1.1. Call with not null numberPtr.
            Expected: GT_OK.
        */
        st = cpssDxChPortTxGlobalBufNumberGet(dev, &number);
        UTF_VERIFY_EQUAL1_PARAM_MAC(GT_OK, st, dev);

        /*
            1.2. Call api with wrong numberPtr [NULL].
            Expected: GT_BAD_PTR.
        */
        st = cpssDxChPortTxGlobalBufNumberGet(dev, NULL);
        UTF_VERIFY_EQUAL1_STRING_MAC(GT_BAD_PTR, st, "numberPtr is NULL", dev);
    }

    /* 2. For not-active devices and devices from non-applicable family */
    /* check that function returns GT_BAD_PARAM.                        */

    /* prepare device iterator */
    PRV_TGF_NOT_APPLIC_DEV_RESET_MAC(&dev, UTF_CH1_E | UTF_CH1_DIAMOND_E | UTF_CH2_E);

    /* go over all non active devices */
    while(GT_OK == prvUtfNextDeviceGet(&dev, GT_FALSE))
    {
        st = cpssDxChPortTxGlobalBufNumberGet(dev, &number);
        UTF_VERIFY_EQUAL1_PARAM_MAC(GT_NOT_APPLICABLE_DEVICE, st, dev);
    }

    /* 3. Call function with out of bound value for device id.*/
    dev = PRV_CPSS_MAX_PP_DEVICES_CNS;

    st = cpssDxChPortTxGlobalBufNumberGet(dev, &number);
    UTF_VERIFY_EQUAL1_PARAM_MAC(GT_BAD_PARAM, st, dev);
}

/*----------------------------------------------------------------------------*/
/*
GT_STATUS cpssDxChPortTxGlobalDescNumberGet
(
    IN  GT_U8   dev,
    OUT GT_U32  *numberPtr
)
*/
UTF_TEST_CASE_MAC(cpssDxChPortTxGlobalDescNumberGet)
{
/*
    ITERATE_DEVICES(DxCh3 and above)
    1.1. Call with not null numberPtr.
    Expected: GT_OK.
    1.2. Call api with wrong numberPtr [NULL].
    Expected: GT_BAD_PTR.
*/
    GT_STATUS st = GT_OK;

    GT_U8   dev;
    GT_U32  number;

    /* prepare device iterator */
    PRV_TGF_NOT_APPLIC_DEV_RESET_MAC(&dev, UTF_CH1_E | UTF_CH1_DIAMOND_E | UTF_CH2_E);

    /* 1. Go over all active devices. */
    while (GT_OK == prvUtfNextDeviceGet(&dev, GT_TRUE))
    {
        /*
            1.1. Call with not null numberPtr.
            Expected: GT_OK.
        */
        st = cpssDxChPortTxGlobalDescNumberGet(dev, &number);
        UTF_VERIFY_EQUAL1_PARAM_MAC(GT_OK, st, dev);

        /*
            1.2. Call api with wrong numberPtr [NULL].
            Expected: GT_BAD_PTR.
        */
        st = cpssDxChPortTxGlobalDescNumberGet(dev, NULL);
        UTF_VERIFY_EQUAL1_STRING_MAC(GT_BAD_PTR, st, "numberPtr is NULL", dev);
    }

    /* 2. For not-active devices and devices from non-applicable family */
    /* check that function returns GT_BAD_PARAM.                        */

    /* prepare device iterator */
    PRV_TGF_NOT_APPLIC_DEV_RESET_MAC(&dev, UTF_CH1_E | UTF_CH1_DIAMOND_E | UTF_CH2_E);

    /* go over all non active devices */
    while(GT_OK == prvUtfNextDeviceGet(&dev, GT_FALSE))
    {
        st = cpssDxChPortTxGlobalDescNumberGet(dev, &number);
        UTF_VERIFY_EQUAL1_PARAM_MAC(GT_NOT_APPLICABLE_DEVICE, st, dev);
    }

    /* 3. Call function with out of bound value for device id.*/
    dev = PRV_CPSS_MAX_PP_DEVICES_CNS;

    st = cpssDxChPortTxGlobalDescNumberGet(dev, &number);
    UTF_VERIFY_EQUAL1_PARAM_MAC(GT_BAD_PARAM, st, dev);
}

/*----------------------------------------------------------------------------*/
/*
GT_STATUS cpssDxChPortTxMcastBufNumberGet
(
    IN  GT_U8   dev,
    OUT GT_U32  *numberPtr
)
*/
UTF_TEST_CASE_MAC(cpssDxChPortTxMcastBufNumberGet)
{
/*
    ITERATE_DEVICES(Lion)
    1.1. Call with not null numberPtr.
    Expected: GT_OK.
    1.2. Call api with wrong numberPtr [NULL].
    Expected: GT_BAD_PTR.
*/
    GT_STATUS st = GT_OK;

    GT_U8   dev;
    GT_U32  number;

    /* prepare device iterator */
    PRV_TGF_NOT_APPLIC_DEV_RESET_MAC(&dev, UTF_CH1_E | UTF_CH1_DIAMOND_E | UTF_CH2_E | UTF_CH3_E | UTF_XCAT_E | UTF_XCAT2_E);

    /* 1. Go over all active devices. */
    while (GT_OK == prvUtfNextDeviceGet(&dev, GT_TRUE))
    {
        /*
            1.1. Call with not null numberPtr.
            Expected: GT_OK.
        */
        st = cpssDxChPortTxMcastBufNumberGet(dev, &number);
        UTF_VERIFY_EQUAL1_PARAM_MAC(GT_OK, st, dev);

        /*
            1.2. Call api with wrong numberPtr [NULL].
            Expected: GT_BAD_PTR.
        */
        st = cpssDxChPortTxMcastBufNumberGet(dev, NULL);
        UTF_VERIFY_EQUAL1_STRING_MAC(GT_BAD_PTR, st, "numberPtr is NULL", dev);
    }

    /* 2. For not-active devices and devices from non-applicable family */
    /* check that function returns GT_BAD_PARAM.                        */

    /* prepare device iterator */
    PRV_TGF_NOT_APPLIC_DEV_RESET_MAC(&dev, UTF_CH1_E | UTF_CH1_DIAMOND_E | UTF_CH2_E | UTF_CH3_E | UTF_XCAT_E | UTF_XCAT2_E);

    /* go over all non active devices */
    while(GT_OK == prvUtfNextDeviceGet(&dev, GT_FALSE))
    {
        st = cpssDxChPortTxMcastBufNumberGet(dev, &number);
        UTF_VERIFY_EQUAL1_PARAM_MAC(GT_NOT_APPLICABLE_DEVICE, st, dev);
    }

    /* 3. Call function with out of bound value for device id.*/
    dev = PRV_CPSS_MAX_PP_DEVICES_CNS;

    st = cpssDxChPortTxMcastBufNumberGet(dev, &number);
    UTF_VERIFY_EQUAL1_PARAM_MAC(GT_BAD_PARAM, st, dev);
}

/*----------------------------------------------------------------------------*/
/*
GT_STATUS cpssDxChPortTxMcastBuffersLimitGet
(
    IN  GT_U8   dev,
    OUT GT_U32  *mcastMaxBufNumPtr
)
*/
UTF_TEST_CASE_MAC(cpssDxChPortTxMcastBuffersLimitGet)
{
/*
    ITERATE_DEVICES(Lion)
    1.1. Call with not null mcastMaxBufNumPtr.
    Expected: GT_OK.
    1.2. Call api with wrong mcastMaxBufNumPtr [NULL].
    Expected: GT_BAD_PTR.
*/
    GT_STATUS st = GT_OK;

    GT_U8   dev;
    GT_U32  mcastMaxBufNum;

    /* prepare device iterator */
    PRV_TGF_NOT_APPLIC_DEV_RESET_MAC(&dev, UTF_CH1_E | UTF_CH1_DIAMOND_E | UTF_CH2_E | UTF_CH3_E | UTF_XCAT_E | UTF_XCAT2_E);

    /* 1. Go over all active devices. */
    while (GT_OK == prvUtfNextDeviceGet(&dev, GT_TRUE))
    {
        /*
            1.1. Call with not null mcastMaxBufNumPtr.
            Expected: GT_OK.
        */
        st = cpssDxChPortTxMcastBuffersLimitGet(dev, &mcastMaxBufNum);
        UTF_VERIFY_EQUAL1_PARAM_MAC(GT_OK, st, dev);

        /*
            1.2. Call api with wrong mcastMaxBufNumPtr [NULL].
            Expected: GT_BAD_PTR.
        */
        st = cpssDxChPortTxMcastBuffersLimitGet(dev, NULL);
        UTF_VERIFY_EQUAL1_STRING_MAC(GT_BAD_PTR, st, "mcastMaxBufNumPtr is NULL", dev);
    }

    /* 2. For not-active devices and devices from non-applicable family */
    /* check that function returns GT_BAD_PARAM.                        */

    /* prepare device iterator */
    PRV_TGF_NOT_APPLIC_DEV_RESET_MAC(&dev, UTF_CH1_E | UTF_CH1_DIAMOND_E | UTF_CH2_E | UTF_CH3_E | UTF_XCAT_E | UTF_XCAT2_E);

    /* go over all non active devices */
    while(GT_OK == prvUtfNextDeviceGet(&dev, GT_FALSE))
    {
        st = cpssDxChPortTxMcastBuffersLimitGet(dev, &mcastMaxBufNum);
        UTF_VERIFY_EQUAL1_PARAM_MAC(GT_NOT_APPLICABLE_DEVICE, st, dev);
    }

    /* 3. Call function with out of bound value for device id.*/
    dev = PRV_CPSS_MAX_PP_DEVICES_CNS;

    st = cpssDxChPortTxMcastBuffersLimitGet(dev, &mcastMaxBufNum);
    UTF_VERIFY_EQUAL1_PARAM_MAC(GT_BAD_PARAM, st, dev);
}

/*----------------------------------------------------------------------------*/
/*
GT_STATUS cpssDxChPortTxMcastBuffersLimitSet
(
    IN  GT_U8  dev,
    IN  GT_U32 mcastMaxBufNum
)
*/
UTF_TEST_CASE_MAC(cpssDxChPortTxMcastBuffersLimitSet)
{
/*
    ITERATE_DEVICES(Lion)
    1.1. Call with mcastMaxBufNum[0 / 100 / 0xFFFF],
    Expected: GT_OK.
    1.2. Call cpssDxChPortTxMcastBuffersLimitGet
           with the same parameters.
    Expected: GT_OK and the same values than was set.
    1.3. Call api with wrong mcastMaxBufNum [0xFFFF+1].
    Expected: NOT GT_OK.
*/
    GT_STATUS st = GT_OK;

    GT_U8  dev;
    GT_U32 mcastMaxBufNum = 0;
    GT_U32 mcastMaxBufNumGet = 0;

    /* prepare device iterator */
    PRV_TGF_NOT_APPLIC_DEV_RESET_MAC(&dev, UTF_CH1_E | UTF_CH1_DIAMOND_E | UTF_CH2_E | UTF_CH3_E | UTF_XCAT_E | UTF_XCAT2_E);

    /* 1. Go over all active devices. */
    while (GT_OK == prvUtfNextDeviceGet(&dev, GT_TRUE))
    {
        /*
            1.1. Call with mcastMaxBufNum[0 / 100 / 0xFFFF],
            Expected: GT_OK.
        */
        /* call with mcastMaxBufNum[0] */
        mcastMaxBufNum = 0;

        st = cpssDxChPortTxMcastBuffersLimitSet(dev, mcastMaxBufNum);
        UTF_VERIFY_EQUAL1_PARAM_MAC(GT_OK, st, dev);

        /*
            1.2. Call cpssDxChPortTxMcastBuffersLimitGet
                   with the same parameters.
            Expected: GT_OK and the same values than was set.
        */
        st = cpssDxChPortTxMcastBuffersLimitGet(dev, &mcastMaxBufNumGet);
        UTF_VERIFY_EQUAL1_STRING_MAC(GT_OK, st,
                  "cpssDxChPortTxMcastBuffersLimitGet: %d ", dev);

        /* Verifying values */
        UTF_VERIFY_EQUAL1_STRING_MAC(mcastMaxBufNum, mcastMaxBufNumGet,
                       "got another mcastMaxBufNum then was set: %d", dev);

        /* call with mcastMaxBufNum[100] */
        mcastMaxBufNum = 100;

        st = cpssDxChPortTxMcastBuffersLimitSet(dev, mcastMaxBufNum);
        UTF_VERIFY_EQUAL1_PARAM_MAC(GT_OK, st, dev);

        /*
            1.2. Call cpssDxChPortTxMcastBuffersLimitGet
                   with the same parameters.
            Expected: GT_OK and the same values than was set.
        */
        st = cpssDxChPortTxMcastBuffersLimitGet(dev, &mcastMaxBufNumGet);
        UTF_VERIFY_EQUAL1_STRING_MAC(GT_OK, st,
                  "cpssDxChPortTxMcastBuffersLimitGet: %d ", dev);

        /* Verifying values */
        UTF_VERIFY_EQUAL1_STRING_MAC(mcastMaxBufNum, mcastMaxBufNumGet,
                       "got another mcastMaxBufNum then was set: %d", dev);

        /* call with mcastMaxBufNum[0xFFFF] */
        mcastMaxBufNum = 0xFFFF;

        st = cpssDxChPortTxMcastBuffersLimitSet(dev, mcastMaxBufNum);
        UTF_VERIFY_EQUAL1_PARAM_MAC(GT_OK, st, dev);

        /*
            1.2. Call cpssDxChPortTxMcastBuffersLimitGet
                   with the same parameters.
            Expected: GT_OK and the same values than was set.
        */
        st = cpssDxChPortTxMcastBuffersLimitGet(dev, &mcastMaxBufNumGet);
        UTF_VERIFY_EQUAL1_STRING_MAC(GT_OK, st,
                  "cpssDxChPortTxMcastBuffersLimitGet: %d ", dev);

        /* Verifying values */
        UTF_VERIFY_EQUAL1_STRING_MAC(mcastMaxBufNum, mcastMaxBufNumGet,
                       "got another mcastMaxBufNum then was set: %d", dev);

        /*
            1.3. Call api with wrong mcastMaxBufNum [0xFFFF + 1].
            Expected: NOT GT_OK.
        */
        mcastMaxBufNum = 0xFFFF + 1;

        st = cpssDxChPortTxMcastBuffersLimitSet(dev, mcastMaxBufNum);
        UTF_VERIFY_NOT_EQUAL1_PARAM_MAC(GT_OK, st, dev);

        mcastMaxBufNum = 0;
    }

    /* restore correct values */
    mcastMaxBufNum = 0;

    /* 2. For not-active devices and devices from non-applicable family */
    /* check that function returns GT_BAD_PARAM.                        */

    /* prepare device iterator */
    PRV_TGF_NOT_APPLIC_DEV_RESET_MAC(&dev, UTF_CH1_E | UTF_CH1_DIAMOND_E | UTF_CH2_E | UTF_CH3_E | UTF_XCAT_E | UTF_XCAT2_E);

    /* go over all non active devices */
    while(GT_OK == prvUtfNextDeviceGet(&dev, GT_FALSE))
    {
        st = cpssDxChPortTxMcastBuffersLimitSet(dev, mcastMaxBufNum);
        UTF_VERIFY_EQUAL1_PARAM_MAC(GT_NOT_APPLICABLE_DEVICE, st, dev);
    }

    /* 3. Call function with out of bound value for device id.*/
    dev = PRV_CPSS_MAX_PP_DEVICES_CNS;

    st = cpssDxChPortTxMcastBuffersLimitSet(dev, mcastMaxBufNum);
    UTF_VERIFY_EQUAL1_PARAM_MAC(GT_BAD_PARAM, st, dev);
}

/*----------------------------------------------------------------------------*/
/*
GT_STATUS cpssDxChPortTxMcastDescNumberGet
(
    IN  GT_U8   dev,
    OUT GT_U32  *numberPtr
)
*/
UTF_TEST_CASE_MAC(cpssDxChPortTxMcastDescNumberGet)
{
/*
    ITERATE_DEVICES(DxCh3 and above)
    1.1. Call with not null numberPtr.
    Expected: GT_OK.
    1.2. Call api with wrong numberPtr [NULL].
    Expected: GT_BAD_PTR.
*/
    GT_STATUS st = GT_OK;

    GT_U8   dev;
    GT_U32  number;

    /* prepare device iterator */
    PRV_TGF_NOT_APPLIC_DEV_RESET_MAC(&dev, UTF_CH1_E | UTF_CH1_DIAMOND_E | UTF_CH2_E);

    /* 1. Go over all active devices. */
    while (GT_OK == prvUtfNextDeviceGet(&dev, GT_TRUE))
    {
        /*
            1.1. Call with not null numberPtr.
            Expected: GT_OK.
        */
        st = cpssDxChPortTxMcastDescNumberGet(dev, &number);
        UTF_VERIFY_EQUAL1_PARAM_MAC(GT_OK, st, dev);

        /*
            1.2. Call api with wrong numberPtr [NULL].
            Expected: GT_BAD_PTR.
        */
        st = cpssDxChPortTxMcastDescNumberGet(dev, NULL);
        UTF_VERIFY_EQUAL1_STRING_MAC(GT_BAD_PTR, st, "numberPtr is NULL", dev);
    }

    /* 2. For not-active devices and devices from non-applicable family */
    /* check that function returns GT_BAD_PARAM.                        */

    /* prepare device iterator */
    PRV_TGF_NOT_APPLIC_DEV_RESET_MAC(&dev, UTF_CH1_E | UTF_CH1_DIAMOND_E | UTF_CH2_E);

    /* go over all non active devices */
    while(GT_OK == prvUtfNextDeviceGet(&dev, GT_FALSE))
    {
        st = cpssDxChPortTxMcastDescNumberGet(dev, &number);
        UTF_VERIFY_EQUAL1_PARAM_MAC(GT_NOT_APPLICABLE_DEVICE, st, dev);
    }

    /* 3. Call function with out of bound value for device id.*/
    dev = PRV_CPSS_MAX_PP_DEVICES_CNS;

    st = cpssDxChPortTxMcastDescNumberGet(dev, &number);
    UTF_VERIFY_EQUAL1_PARAM_MAC(GT_BAD_PARAM, st, dev);
}

/*----------------------------------------------------------------------------*/
/*
GT_STATUS cpssDxChPortTxProfileWeightedRandomTailDropEnableGet
(
    IN  GT_U8                                     dev,
    IN  CPSS_PORT_TX_DROP_PROFILE_SET_ENT         pfSet,
    IN  GT_U32                                    dp,
    OUT CPSS_PORT_TX_TAIL_DROP_WRTD_ENABLERS_STC  *enablersPtr
)
*/
UTF_TEST_CASE_MAC(cpssDxChPortTxProfileWeightedRandomTailDropEnableGet)
{
/*
    ITERATE_DEVICES(Lion,xCat2)
    1.1. Call with pfSet[CPSS_PORT_TX_DROP_PROFILE_1_E /
                         CPSS_PORT_TX_DROP_PROFILE_2_E /
                         CPSS_PORT_TX_DROP_PROFILE_3_E /
                         CPSS_PORT_TX_DROP_PROFILE_4_E /
                         CPSS_PORT_TX_DROP_PROFILE_5_E],
                   dp[0 / 1 / 2 / 0 / 0],
                   tc[0 / 1 / 2/ 3 / 7]
    Expected: GT_OK.
    1.2. Call api with wrong pfSet [wrong enum values].
    Expected: GT_BAD_PARAM.
    1.3. Call api with wrong dp [3].
    Expected: NOT GT_OK.
    1.4. Call api with wrong enablersPtr [NULL].
    Expected: GT_BAD_PTR.
    1.5. Call api with wrong tc [8].
    Expected XCAT2: GT_BAD_PARAM.
    Expected LION: GT_OK.
*/
    GT_STATUS st = GT_OK;

    GT_U8                                     dev;
    CPSS_PORT_TX_DROP_PROFILE_SET_ENT         pfSet = CPSS_PORT_TX_DROP_PROFILE_1_E;
    GT_U32                                    dp = 0;
    GT_U8                                     tc = 0;
    CPSS_PORT_TX_TAIL_DROP_WRTD_ENABLERS_STC  enablers;

    cpssOsBzero((GT_VOID*) &enablers, sizeof(enablers));

    /* prepare device iterator */
    PRV_TGF_NOT_APPLIC_DEV_RESET_MAC(&dev, UTF_CH1_E | UTF_CH1_DIAMOND_E | UTF_CH2_E | UTF_CH3_E | UTF_XCAT_E);

    /* 1. Go over all active devices. */
    while (GT_OK == prvUtfNextDeviceGet(&dev, GT_TRUE))
    {
        /*
            1.1. Call with pfSet[CPSS_PORT_TX_DROP_PROFILE_1_E /
                                 CPSS_PORT_TX_DROP_PROFILE_2_E /
                                 CPSS_PORT_TX_DROP_PROFILE_3_E /
                                 CPSS_PORT_TX_DROP_PROFILE_4_E /
                                 CPSS_PORT_TX_DROP_PROFILE_5_E],
                           dp[0 / 1 / 2 / 0 / 0],
                           tc[0 / 1 / 2/ 3 / 7]
            Expected: GT_OK.
        */

        /* call with pfSet[CPSS_PORT_TX_DROP_PROFILE_1_E], dp[0], tc[0] */
        pfSet = CPSS_PORT_TX_DROP_PROFILE_1_E;
        dp = 0;
        tc = 0;

        st = cpssDxChPortTxProfileWeightedRandomTailDropEnableGet(dev,
                            pfSet, dp, tc, &enablers);
        UTF_VERIFY_EQUAL1_PARAM_MAC(GT_OK, st, dev);

        /* call with pfSet[CPSS_PORT_TX_DROP_PROFILE_2_E], dp[1], tc[1] */
        pfSet = CPSS_PORT_TX_DROP_PROFILE_2_E;
        dp = 1;
        tc = 1;
        st = cpssDxChPortTxProfileWeightedRandomTailDropEnableGet(dev,
                            pfSet, dp,  tc, &enablers);
        UTF_VERIFY_EQUAL1_PARAM_MAC(GT_OK, st, dev);

        /* call with pfSet[CPSS_PORT_TX_DROP_PROFILE_3_E], dp[2], tc[2] */
        pfSet = CPSS_PORT_TX_DROP_PROFILE_3_E;
        dp = 2;
        tc = 2;

        st = cpssDxChPortTxProfileWeightedRandomTailDropEnableGet(dev,
                            pfSet, dp, tc, &enablers);
        if (PRV_CPSS_PP_MAC(prvTgfDevNum)->devFamily == CPSS_PP_FAMILY_DXCH_LION_E)
        {
            UTF_VERIFY_EQUAL1_PARAM_MAC(GT_OK, st, dev);
        }
        else
        {
            UTF_VERIFY_EQUAL1_PARAM_MAC(GT_BAD_PARAM, st, dev);
        }

        /* call with pfSet[CPSS_PORT_TX_DROP_PROFILE_4_E], dp[0], tc[3] */
        pfSet = CPSS_PORT_TX_DROP_PROFILE_4_E;
        dp = 0;
        tc = 3;

        st = cpssDxChPortTxProfileWeightedRandomTailDropEnableGet(dev,
                            pfSet, dp, tc,  &enablers);
        UTF_VERIFY_EQUAL1_PARAM_MAC(GT_OK, st, dev);

        /* call with pfSet[CPSS_PORT_TX_DROP_PROFILE_5_E], dp[0], tc[7] */
        pfSet = CPSS_PORT_TX_DROP_PROFILE_5_E;
        dp = 0;
        tc = 7;
        st = cpssDxChPortTxProfileWeightedRandomTailDropEnableGet(dev,
                            pfSet, dp,  tc, &enablers);
        UTF_VERIFY_EQUAL1_PARAM_MAC(GT_OK, st, dev);

        /*
            1.2. Call api with wrong pfSet [wrong enum values].
            Expected: GT_BAD_PARAM.
        */
        UTF_ENUMS_CHECK_MAC(cpssDxChPortTxProfileWeightedRandomTailDropEnableGet
                            (dev, pfSet, dp, tc, &enablers),
                            pfSet);

        /*
            1.3. Call api with wrong dp [3].
            Expected: NOT GT_OK.
        */
        dp = 3;

        st = cpssDxChPortTxProfileWeightedRandomTailDropEnableGet(dev,
                            pfSet, dp, tc, &enablers);
        UTF_VERIFY_NOT_EQUAL1_PARAM_MAC(GT_OK, st, dev);

        dp = 0;

        /*
            1.4. Call api with wrong enablersPtr [NULL].
            Expected: GT_BAD_PTR.
        */
        st = cpssDxChPortTxProfileWeightedRandomTailDropEnableGet(dev,
                            pfSet, dp,  tc, NULL);
        UTF_VERIFY_EQUAL1_STRING_MAC(GT_BAD_PTR, st, "enablersPtr is NULL", dev);

        /*
            1.5. Call api with wrong tc [8].
            Expected XCAT2: GT_BAD_PARAM.
            Expected LION: GT_OK.
        */
        tc = 8;
        st = cpssDxChPortTxProfileWeightedRandomTailDropEnableGet(dev,
                            pfSet, dp, tc, &enablers);
        if (PRV_CPSS_PP_MAC(prvTgfDevNum)->devFamily == CPSS_PP_FAMILY_DXCH_LION_E)
        {
            UTF_VERIFY_EQUAL1_PARAM_MAC(GT_OK, st, dev);
        }
        else
        {
            UTF_VERIFY_EQUAL1_PARAM_MAC(GT_BAD_PARAM, st, dev);
        }
    }

    /* restore correct values */
    pfSet = CPSS_PORT_TX_DROP_PROFILE_1_E;
    dp = 0;
    tc = 0;

    /* 2. For not-active devices and devices from non-applicable family */
    /* check that function returns GT_BAD_PARAM.                        */

    /* prepare device iterator */
    PRV_TGF_NOT_APPLIC_DEV_RESET_MAC(&dev, UTF_CH1_E | UTF_CH1_DIAMOND_E | UTF_CH2_E | UTF_CH3_E | UTF_XCAT_E);

    /* go over all non active devices */
    while(GT_OK == prvUtfNextDeviceGet(&dev, GT_FALSE))
    {
        st = cpssDxChPortTxProfileWeightedRandomTailDropEnableGet(dev,
                            pfSet, dp,  tc, &enablers);
        UTF_VERIFY_EQUAL1_PARAM_MAC(GT_NOT_APPLICABLE_DEVICE, st, dev);
    }

    /* 3. Call function with out of bound value for device id.*/
    dev = PRV_CPSS_MAX_PP_DEVICES_CNS;

    st = cpssDxChPortTxProfileWeightedRandomTailDropEnableGet(dev,
                            pfSet, dp,  tc, &enablers);
    UTF_VERIFY_EQUAL1_PARAM_MAC(GT_BAD_PARAM, st, dev);
}

/*----------------------------------------------------------------------------*/
/*
GT_STATUS cpssDxChPortTxProfileWeightedRandomTailDropEnableSet
(
    IN GT_U8                                    devNum,
    IN CPSS_PORT_TX_DROP_PROFILE_SET_ENT        pfSet,
    IN GT_U32                                   dp,
    IN GT_U8                                    tc,
    IN CPSS_PORT_TX_TAIL_DROP_WRTD_ENABLERS_STC *enablersPtr
)
*/
UTF_TEST_CASE_MAC(cpssDxChPortTxProfileWeightedRandomTailDropEnableSet)
{
/*
    ITERATE_DEVICES(Lion, xCat2)
    1.1. Call with pfSet[CPSS_PORT_TX_DROP_PROFILE_1_E /
                         CPSS_PORT_TX_DROP_PROFILE_2_E /
                         CPSS_PORT_TX_DROP_PROFILE_3_E /
                         CPSS_PORT_TX_DROP_PROFILE_4_E]
                   dp[0 / 1 / 2 / 0 ],
                   tc[0 / 1 / 3 / 7 ],
           enablers.tcDpLimit       [GT_TRUE / GT_FALSE],
           enablers.portLimit       [GT_TRUE / GT_FALSE],
           enablers.tcLimit         [GT_TRUE / GT_FALSE],
           enablers.sharedPoolLimit [GT_TRUE / GT_FALSE],

    Expected: GT_OK.
    1.2. Call cpssDxChPortTxProfileWeightedRandomTailDropEnableGet
           with the same parameters.
    Expected: GT_OK and the same values than was set.
    1.3. Call api with wrong pfSet [wrong enum values].
    Expected: GT_BAD_PARAM.
    1.4. Call api with wrong dp [3].
    Expected: NOT GT_OK.
    1.5. Call api with wrong enablers [NULL].
    Expected: GT_BAD_PTR.
    1.6. Call api with wrong tc [8].
    Expected XCAT2: GT_BAD_PARAM.
    Expected LION: GT_OK.
*/
    GT_STATUS st = GT_OK;
    GT_BOOL   isEqual = GT_FALSE;

    GT_U8                                     dev;
    CPSS_PORT_TX_DROP_PROFILE_SET_ENT         pfSet = CPSS_PORT_TX_DROP_PROFILE_1_E;
    GT_U32                                    dp = 0;
    GT_U8                                     tc = 0;
    CPSS_PORT_TX_TAIL_DROP_WRTD_ENABLERS_STC  enablers;
    CPSS_PORT_TX_TAIL_DROP_WRTD_ENABLERS_STC  enablersGet;

    cpssOsBzero((GT_VOID*) &enablers, sizeof(enablers));
    cpssOsBzero((GT_VOID*) &enablersGet, sizeof(enablersGet));

    /* prepare device iterator */
    PRV_TGF_NOT_APPLIC_DEV_RESET_MAC(&dev, UTF_CH1_E | UTF_CH1_DIAMOND_E | UTF_CH2_E | UTF_CH3_E | UTF_XCAT_E);

    /* 1. Go over all active devices. */
    while (GT_OK == prvUtfNextDeviceGet(&dev, GT_TRUE))
    {
        /*
            1.1. Call with pfSet[CPSS_PORT_TX_DROP_PROFILE_1_E /
                                 CPSS_PORT_TX_DROP_PROFILE_2_E /
                                 CPSS_PORT_TX_DROP_PROFILE_3_E /
                                 CPSS_PORT_TX_DROP_PROFILE_4_E]
                           dp[0 / 1 / 2 / 0 ],
                           tc[0 / 1 / 3 / 7 ],
                   enablers.tcDpLimit       [GT_TRUE / GT_FALSE],
                   enablers.portLimit       [GT_TRUE / GT_FALSE],
                   enablers.tcLimit         [GT_TRUE / GT_FALSE],
                   enablers.sharedPoolLimit [GT_TRUE / GT_FALSE],

            Expected: GT_OK.
        */
        /* call with pfSet[CPSS_PORT_TX_DROP_PROFILE_1_E], dp[0], tc[0]
                       enablers.tcDpLimit       = GT_TRUE;
                       enablers.portLimit       = GT_TRUE;
                       enablers.tcLimit         = GT_TRUE;
                       enablers.sharedPoolLimit = GT_TRUE;*/
        pfSet = CPSS_PORT_TX_DROP_PROFILE_1_E;
        dp = 0;
        tc = 0;
        enablers.tcDpLimit       = GT_TRUE;
        enablers.portLimit       = GT_TRUE;
        enablers.tcLimit         = GT_TRUE;
        enablers.sharedPoolLimit = GT_TRUE;

        st = cpssDxChPortTxProfileWeightedRandomTailDropEnableSet(dev,
                            pfSet, dp, tc, &enablers);
        UTF_VERIFY_EQUAL1_PARAM_MAC(GT_OK, st, dev);

        /*
            1.2. Call cpssDxChPortTxProfileWeightedRandomTailDropEnableGet
                   with the same parameters.
            Expected: GT_OK and the same values than was set.
        */
        st = cpssDxChPortTxProfileWeightedRandomTailDropEnableGet(dev,
                            pfSet, dp, tc, &enablersGet);
        UTF_VERIFY_EQUAL1_STRING_MAC(GT_OK, st,
                  "cpssDxChPortTxProfileWeightedRandomTailDropEnableGet: %d ", dev);

        /* Verifying values */
        if (PRV_CPSS_PP_MAC(prvTgfDevNum)->devFamily == CPSS_PP_FAMILY_DXCH_LION_E)
        {
            isEqual = (0 == cpssOsMemCmp((GT_VOID*)&enablers, (GT_VOID*)&enablersGet,
                                         sizeof (enablers))) ? GT_TRUE : GT_FALSE;
        }
        else
        {
            isEqual = ((enablers.portLimit == enablersGet.portLimit) &&
                       (enablers.sharedPoolLimit == enablersGet.sharedPoolLimit) &&
                       (enablers.tcDpLimit == enablersGet.tcDpLimit)) ? GT_TRUE :
                                                                        GT_FALSE;
        }


        UTF_VERIFY_EQUAL1_STRING_MAC(GT_TRUE, isEqual,
                   "get another enablers than was set: %d", dev);


        /* call with pfSet[CPSS_PORT_TX_DROP_PROFILE_2_E], dp[1], tc[0]
                       enablers.tcDpLimit       = GT_FALSE;
                       enablers.portLimit       = GT_FALSE;
                       enablers.tcLimit         = GT_FALSE;
                       enablers.sharedPoolLimit = GT_FALSE;*/
        pfSet = CPSS_PORT_TX_DROP_PROFILE_2_E;
        dp = 1;
        tc = 1;
        enablers.tcDpLimit       = GT_FALSE;
        enablers.portLimit       = GT_FALSE;
        enablers.tcLimit         = GT_FALSE;
        enablers.sharedPoolLimit = GT_FALSE;

        st = cpssDxChPortTxProfileWeightedRandomTailDropEnableSet(dev,
                            pfSet, dp, tc, &enablers);
        UTF_VERIFY_EQUAL1_PARAM_MAC(GT_OK, st, dev);

        /*
            1.2. Call cpssDxChPortTxProfileWeightedRandomTailDropEnableGet
                   with the same parameters.
            Expected: GT_OK and the same values than was set.
        */
        st = cpssDxChPortTxProfileWeightedRandomTailDropEnableGet(dev,
                            pfSet, dp, tc,&enablersGet);
        UTF_VERIFY_EQUAL1_STRING_MAC(GT_OK, st,
                  "cpssDxChPortTxProfileWeightedRandomTailDropEnableGet: %d ", dev);

        /* Verifying values */
        if (PRV_CPSS_PP_MAC(prvTgfDevNum)->devFamily == CPSS_PP_FAMILY_DXCH_LION_E)
        {
            isEqual = (0 == cpssOsMemCmp((GT_VOID*)&enablers, (GT_VOID*)&enablersGet,
                                         sizeof (enablers))) ? GT_TRUE : GT_FALSE;
        }
        else
        {
            isEqual = ((enablers.portLimit == enablersGet.portLimit) &&
                       (enablers.sharedPoolLimit == enablersGet.sharedPoolLimit) &&
                       (enablers.tcDpLimit == enablersGet.tcDpLimit)) ? GT_TRUE :
                                                                        GT_FALSE;
        }

        UTF_VERIFY_EQUAL1_STRING_MAC(GT_TRUE, isEqual,
                   "get another enablers than was set: %d", dev);

        /* call with pfSet[CPSS_PORT_TX_DROP_PROFILE_3_E], dp[2], tc[3]
                       enablers.tcDpLimit       = GT_FALSE;
                       enablers.portLimit       = GT_TRUE;
                       enablers.tcLimit         = GT_FALSE;
                       enablers.sharedPoolLimit = GT_TRUE;*/
        pfSet = CPSS_PORT_TX_DROP_PROFILE_3_E;
        dp = 2;
        tc = 3;
        enablers.tcDpLimit       = GT_FALSE;
        enablers.portLimit       = GT_TRUE;
        enablers.tcLimit         = GT_FALSE;
        enablers.sharedPoolLimit = GT_TRUE;

        st = cpssDxChPortTxProfileWeightedRandomTailDropEnableSet(dev,
                            pfSet, dp, tc, &enablers);

        if (PRV_CPSS_PP_MAC(prvTgfDevNum)->devFamily == CPSS_PP_FAMILY_DXCH_LION_E)
        {
            UTF_VERIFY_EQUAL1_PARAM_MAC(GT_OK, st, dev);
    
            /*
                1.2. Call cpssDxChPortTxProfileWeightedRandomTailDropEnableGet
                       with the same parameters.
                Expected: GT_OK and the same values than was set.
            */
            st = cpssDxChPortTxProfileWeightedRandomTailDropEnableGet(dev,
                                pfSet, dp, tc, &enablersGet);
            UTF_VERIFY_EQUAL1_STRING_MAC(GT_OK, st,
                      "cpssDxChPortTxProfileWeightedRandomTailDropEnableGet: %d ", dev);
    
            /* Verifying values */
            isEqual = (0 == cpssOsMemCmp((GT_VOID*)&enablers, (GT_VOID*)&enablersGet,
                                         sizeof (enablers))) ? GT_TRUE : GT_FALSE;

            UTF_VERIFY_EQUAL1_STRING_MAC(GT_TRUE, isEqual,
                       "get another enablers than was set: %d", dev);
        }
        else /* XCAT2 */
        {
            UTF_VERIFY_EQUAL1_PARAM_MAC(GT_BAD_PARAM, st, dev);
        }


        /* call with pfSet[CPSS_PORT_TX_DROP_PROFILE_4_E], dp[0], tc[7]
                       enablers.tcDpLimit       = GT_TRUE;
                       enablers.portLimit       = GT_FALSE;
                       enablers.tcLimit         = GT_TRUE;
                       enablers.sharedPoolLimit = GT_FALSE;*/
        pfSet = CPSS_PORT_TX_DROP_PROFILE_4_E;
        dp = 0;
        tc = 7;
        enablers.tcDpLimit       = GT_TRUE;
        enablers.portLimit       = GT_FALSE;
        enablers.tcLimit         = GT_TRUE;
        enablers.sharedPoolLimit = GT_FALSE;

        st = cpssDxChPortTxProfileWeightedRandomTailDropEnableSet(dev,
                            pfSet, dp, tc, &enablers);
        UTF_VERIFY_EQUAL1_PARAM_MAC(GT_OK, st, dev);

        /*
            1.2. Call cpssDxChPortTxProfileWeightedRandomTailDropEnableGet
                   with the same parameters.
            Expected: GT_OK and the same values than was set.
        */
        st = cpssDxChPortTxProfileWeightedRandomTailDropEnableGet(dev,
                            pfSet, dp, tc, &enablersGet);
        UTF_VERIFY_EQUAL1_STRING_MAC(GT_OK, st,
                  "cpssDxChPortTxProfileWeightedRandomTailDropEnableGet: %d ", dev);

        /* Verifying values */
        if (PRV_CPSS_PP_MAC(prvTgfDevNum)->devFamily == CPSS_PP_FAMILY_DXCH_LION_E)
        {
            isEqual = (0 == cpssOsMemCmp((GT_VOID*)&enablers, (GT_VOID*)&enablersGet,
                                         sizeof (enablers))) ? GT_TRUE : GT_FALSE;
        }
        else
        {
            isEqual = ((enablers.portLimit == enablersGet.portLimit) &&
                       (enablers.sharedPoolLimit == enablersGet.sharedPoolLimit) &&
                       (enablers.tcDpLimit == enablersGet.tcDpLimit)) ? GT_TRUE :
                                                                        GT_FALSE;
        }

        UTF_VERIFY_EQUAL1_STRING_MAC(GT_TRUE, isEqual,
                   "get another enablers than was set: %d", dev);

        /*
            1.3. Call api with wrong pfSet [wrong enum values].
            Expected: GT_BAD_PARAM.
        */
        UTF_ENUMS_CHECK_MAC(cpssDxChPortTxProfileWeightedRandomTailDropEnableSet
                            (dev, pfSet, dp, tc, &enablers),
                            pfSet);

        /*
            1.4. Call api with wrong dp [3].
            Expected: NOT GT_OK.
        */
        dp = 3;
        st = cpssDxChPortTxProfileWeightedRandomTailDropEnableSet(dev,
                            pfSet, dp, tc, &enablers);
        UTF_VERIFY_NOT_EQUAL1_PARAM_MAC(GT_OK, st, dev);

        dp = 0;

        /*
            1.5. Call api with wrong enablersPtr [NULL].
            Expected: GT_BAD_PTR.
        */
        st = cpssDxChPortTxProfileWeightedRandomTailDropEnableSet(dev,
                            pfSet, dp, tc, NULL);
        UTF_VERIFY_EQUAL1_STRING_MAC(GT_BAD_PTR, st, "enablersPtr is NULL", dev);

        /*
            1.6. Call api with wrong tc [8].
            Expected XCAT2: GT_BAD_PARAM.
            Expected LION: GT_OK.
        */
        tc = 8;
        st = cpssDxChPortTxProfileWeightedRandomTailDropEnableSet(dev,
                            pfSet, dp, tc, &enablers);
        if (PRV_CPSS_PP_MAC(prvTgfDevNum)->devFamily == CPSS_PP_FAMILY_DXCH_LION_E)
        {
            UTF_VERIFY_EQUAL1_PARAM_MAC(GT_OK, st, dev);
        }
        else
        {
            UTF_VERIFY_EQUAL1_PARAM_MAC(GT_BAD_PARAM, st, dev);
        }
        
    }

    /* restore correct values */
    pfSet = CPSS_PORT_TX_DROP_PROFILE_1_E;
    dp = 0;
    tc = 0;
    enablers.tcDpLimit = GT_TRUE;
    enablers.portLimit = GT_TRUE;
    enablers.tcLimit = GT_TRUE;
    enablers.sharedPoolLimit = GT_TRUE;

    /* 2. For not-active devices and devices from non-applicable family */
    /* check that function returns GT_BAD_PARAM.                        */

    /* prepare device iterator */
    PRV_TGF_NOT_APPLIC_DEV_RESET_MAC(&dev, UTF_CH1_E | UTF_CH1_DIAMOND_E | UTF_CH2_E | UTF_CH3_E | UTF_XCAT_E);

    /* go over all non active devices */
    while(GT_OK == prvUtfNextDeviceGet(&dev, GT_FALSE))
    {
        st = cpssDxChPortTxProfileWeightedRandomTailDropEnableSet(dev,
                            pfSet, dp, tc, &enablers);
        UTF_VERIFY_EQUAL1_PARAM_MAC(GT_NOT_APPLICABLE_DEVICE, st, dev);
    }

    /* 3. Call function with out of bound value for device id.*/
    dev = PRV_CPSS_MAX_PP_DEVICES_CNS;

    st = cpssDxChPortTxProfileWeightedRandomTailDropEnableSet(dev,
                            pfSet, dp, tc, &enablers);
    UTF_VERIFY_EQUAL1_PARAM_MAC(GT_BAD_PARAM, st, dev);
}

/*----------------------------------------------------------------------------*/
/*
GT_STATUS cpssDxChPortTxResourceHistogramCounterGet
(
    IN  GT_U8   dev,
    IN  GT_U32  cntrNum,
    OUT GT_U32  *cntrPtr
)
*/
UTF_TEST_CASE_MAC(cpssDxChPortTxResourceHistogramCounterGet)
{
/*
    ITERATE_DEVICES(Lion)
    1.1. Call with cntrNum[0 / 1 / 2 / 3],
    Expected: GT_OK.
    1.2. Call api with wrong cntrNum [4].
    Expected: NOT GT_OK.
    1.3. Call api with wrong cntrPtr [NULL].
    Expected: GT_BAD_PTR.
*/
    GT_STATUS st = GT_OK;

    GT_U8   dev;
    GT_U32  cntrNum = 0;
    GT_U32  cntr;

    /* prepare device iterator */
    PRV_TGF_NOT_APPLIC_DEV_RESET_MAC(&dev, UTF_CH1_E | UTF_CH1_DIAMOND_E | UTF_CH2_E | UTF_CH3_E | UTF_XCAT_E | UTF_XCAT2_E);

    /* 1. Go over all active devices. */
    while (GT_OK == prvUtfNextDeviceGet(&dev, GT_TRUE))
    {
        /*
            1.1. Call with cntrNum[0 / 1 / 2 / 3],
            Expected: GT_OK.
        */
        /* call with cntrNum[0] */
        cntrNum = 0;

        st = cpssDxChPortTxResourceHistogramCounterGet(dev, cntrNum, &cntr);
        UTF_VERIFY_EQUAL1_PARAM_MAC(GT_OK, st, dev);

        /* call with cntrNum[1] */
        cntrNum = 1;

        st = cpssDxChPortTxResourceHistogramCounterGet(dev, cntrNum, &cntr);
        UTF_VERIFY_EQUAL1_PARAM_MAC(GT_OK, st, dev);

        /* call with cntrNum[2] */
        cntrNum = 2;

        st = cpssDxChPortTxResourceHistogramCounterGet(dev, cntrNum, &cntr);
        UTF_VERIFY_EQUAL1_PARAM_MAC(GT_OK, st, dev);

        /* call with cntrNum[3] */
        cntrNum = 3;

        st = cpssDxChPortTxResourceHistogramCounterGet(dev, cntrNum, &cntr);
        UTF_VERIFY_EQUAL1_PARAM_MAC(GT_OK, st, dev);

        /*
            1.2. Call api with wrong cntrNum [4].
            Expected: NOT GT_OK.
        */
        cntrNum = 4;

        st = cpssDxChPortTxResourceHistogramCounterGet(dev, cntrNum, &cntr);
        UTF_VERIFY_NOT_EQUAL1_PARAM_MAC(GT_OK, st, dev);

        cntrNum = 0;

        /*
            1.3. Call api with wrong cntrPtr [NULL].
            Expected: GT_BAD_PTR.
        */
        st = cpssDxChPortTxResourceHistogramCounterGet(dev, cntrNum, NULL);
        UTF_VERIFY_EQUAL1_STRING_MAC(GT_BAD_PTR, st, "cntrPtr is NULL", dev);
    }

    /* restore correct values */
    cntrNum = 0;

    /* 2. For not-active devices and devices from non-applicable family */
    /* check that function returns GT_BAD_PARAM.                        */

    /* prepare device iterator */
    PRV_TGF_NOT_APPLIC_DEV_RESET_MAC(&dev, UTF_CH1_E | UTF_CH1_DIAMOND_E | UTF_CH2_E | UTF_CH3_E | UTF_XCAT_E | UTF_XCAT2_E);

    /* go over all non active devices */
    while(GT_OK == prvUtfNextDeviceGet(&dev, GT_FALSE))
    {
        st = cpssDxChPortTxResourceHistogramCounterGet(dev, cntrNum, &cntr);
        UTF_VERIFY_EQUAL1_PARAM_MAC(GT_NOT_APPLICABLE_DEVICE, st, dev);
    }

    /* 3. Call function with out of bound value for device id.*/
    dev = PRV_CPSS_MAX_PP_DEVICES_CNS;

    st = cpssDxChPortTxResourceHistogramCounterGet(dev, cntrNum, &cntr);
    UTF_VERIFY_EQUAL1_PARAM_MAC(GT_BAD_PARAM, st, dev);
}

/*----------------------------------------------------------------------------*/
/*
GT_STATUS cpssDxChPortTxResourceHistogramThresholdGet
(
    IN  GT_U8   dev,
    IN  GT_U32  cntrNum,
    OUT GT_U32  *thresholdPtr
)
*/
UTF_TEST_CASE_MAC(cpssDxChPortTxResourceHistogramThresholdGet)
{
/*
    ITERATE_DEVICES(Lion)
    1.1. Call with cntrNum[0 / 1 / 2 / 3],
    Expected: GT_OK.
    1.2. Call api with wrong cntrNum [4].
    Expected: NOT GT_OK.
    1.3. Call api with wrong thresholdPtr [NULL].
    Expected: GT_BAD_PTR.
*/
    GT_STATUS st = GT_OK;

    GT_U8   dev;
    GT_U32  cntrNum = 0;
    GT_U32  threshold;

    /* prepare device iterator */
    PRV_TGF_NOT_APPLIC_DEV_RESET_MAC(&dev, UTF_CH1_E | UTF_CH1_DIAMOND_E | UTF_CH2_E | UTF_CH3_E | UTF_XCAT_E | UTF_XCAT2_E);

    /* 1. Go over all active devices. */
    while (GT_OK == prvUtfNextDeviceGet(&dev, GT_TRUE))
    {
        /*
            1.1. Call with cntrNum[0 / 1 / 2 / 3],
            Expected: GT_OK.
        */
        /* call with cntrNum[0] */
        cntrNum = 0;

        st = cpssDxChPortTxResourceHistogramThresholdGet(dev, cntrNum, &threshold);
        UTF_VERIFY_EQUAL1_PARAM_MAC(GT_OK, st, dev);

        /* call with cntrNum[1] */
        cntrNum = 1;

        st = cpssDxChPortTxResourceHistogramThresholdGet(dev, cntrNum, &threshold);
        UTF_VERIFY_EQUAL1_PARAM_MAC(GT_OK, st, dev);

        /* call with cntrNum[2] */
        cntrNum = 2;

        st = cpssDxChPortTxResourceHistogramThresholdGet(dev, cntrNum, &threshold);
        UTF_VERIFY_EQUAL1_PARAM_MAC(GT_OK, st, dev);

        /* call with cntrNum[3] */
        cntrNum = 3;

        st = cpssDxChPortTxResourceHistogramThresholdGet(dev, cntrNum, &threshold);
        UTF_VERIFY_EQUAL1_PARAM_MAC(GT_OK, st, dev);

        /*
            1.2. Call api with wrong cntrNum [4].
            Expected: NOT GT_OK.
        */
        cntrNum = 4;

        st = cpssDxChPortTxResourceHistogramThresholdGet(dev, cntrNum, &threshold);
        UTF_VERIFY_NOT_EQUAL1_PARAM_MAC(GT_OK, st, dev);

        cntrNum = 0;

        /*
            1.3. Call api with wrong thresholdPtr [NULL].
            Expected: GT_BAD_PTR.
        */
        st = cpssDxChPortTxResourceHistogramThresholdGet(dev, cntrNum, NULL);
        UTF_VERIFY_EQUAL1_STRING_MAC(GT_BAD_PTR, st, "thresholdPtr is NULL", dev);
    }

    /* restore correct values */
    cntrNum = 0;

    /* 2. For not-active devices and devices from non-applicable family */
    /* check that function returns GT_BAD_PARAM.                        */

    /* prepare device iterator */
    PRV_TGF_NOT_APPLIC_DEV_RESET_MAC(&dev, UTF_CH1_E | UTF_CH1_DIAMOND_E | UTF_CH2_E | UTF_CH3_E | UTF_XCAT_E | UTF_XCAT2_E);

    /* go over all non active devices */
    while(GT_OK == prvUtfNextDeviceGet(&dev, GT_FALSE))
    {
        st = cpssDxChPortTxResourceHistogramThresholdGet(dev, cntrNum, &threshold);
        UTF_VERIFY_EQUAL1_PARAM_MAC(GT_NOT_APPLICABLE_DEVICE, st, dev);
    }

    /* 3. Call function with out of bound value for device id.*/
    dev = PRV_CPSS_MAX_PP_DEVICES_CNS;

    st = cpssDxChPortTxResourceHistogramThresholdGet(dev, cntrNum, &threshold);
    UTF_VERIFY_EQUAL1_PARAM_MAC(GT_BAD_PARAM, st, dev);
}

/*----------------------------------------------------------------------------*/
/*
GT_STATUS cpssDxChPortTxResourceHistogramThresholdSet
(
    IN  GT_U8  dev,
    IN  GT_U32 cntrNum,
    IN  GT_U32 threshold
)
*/
UTF_TEST_CASE_MAC(cpssDxChPortTxResourceHistogramThresholdSet)
{
/*
    ITERATE_DEVICES(Lion)
    1.1. Call with cntrNum[0 / 1 / 2 / 3],
                 threshold[0 / 1 / 0x2FFF / 0x3FFF].
    Expected: GT_OK.
    1.2. Call cpssDxChPortTxResourceHistogramThresholdGet
           with the same parameters.
    Expected: GT_OK and the same values than was set.
    1.3. Call api with wrong cntrNum [4].
    Expected: NOT GT_OK.
    1.4. Call api with wrong threshold [0x4000].
    Expected: NOT GT_OK.
*/
    GT_STATUS st = GT_OK;

    GT_U8  dev;
    GT_U32 cntrNum = 0;
    GT_U32 threshold = 0;
    GT_U32 thresholdGet = 0;

    /* prepare device iterator */
    PRV_TGF_NOT_APPLIC_DEV_RESET_MAC(&dev, UTF_CH1_E | UTF_CH1_DIAMOND_E | UTF_CH2_E | UTF_CH3_E | UTF_XCAT_E | UTF_XCAT2_E);

    /* 1. Go over all active devices. */
    while (GT_OK == prvUtfNextDeviceGet(&dev, GT_TRUE))
    {
        /*
            1.1. Call with cntrNum[0 / 1 / 2 / 3],
                         threshold[0 / 1 / 0x2FFF / 0x3FFF].
            Expected: GT_OK.
        */

        /* call with cntrNum[0], threshold[0] */
        cntrNum = 0;
        threshold = 0;

        st = cpssDxChPortTxResourceHistogramThresholdSet(dev, cntrNum, threshold);
        UTF_VERIFY_EQUAL1_PARAM_MAC(GT_OK, st, dev);

        /*
            1.2. Call cpssDxChPortTxResourceHistogramThresholdGet
                   with the same parameters.
            Expected: GT_OK and the same values than was set.
        */
        st = cpssDxChPortTxResourceHistogramThresholdGet(dev, cntrNum, &thresholdGet);
        UTF_VERIFY_EQUAL1_STRING_MAC(GT_OK, st,
                  "cpssDxChPortTxResourceHistogramThresholdGet: %d ", dev);

        /* Verifying values */
        UTF_VERIFY_EQUAL1_STRING_MAC(threshold, thresholdGet,
                       "got another threshold then was set: %d", dev);

        /* call with cntrNum[1], threshold[1] */
        cntrNum = 1;
        threshold = 1;

        st = cpssDxChPortTxResourceHistogramThresholdSet(dev, cntrNum, threshold);
        UTF_VERIFY_EQUAL1_PARAM_MAC(GT_OK, st, dev);

        /*
            1.2. Call cpssDxChPortTxResourceHistogramThresholdGet
                   with the same parameters.
            Expected: GT_OK and the same values than was set.
        */
        st = cpssDxChPortTxResourceHistogramThresholdGet(dev, cntrNum, &thresholdGet);
        UTF_VERIFY_EQUAL1_STRING_MAC(GT_OK, st,
                  "cpssDxChPortTxResourceHistogramThresholdGet: %d ", dev);

        /* Verifying values */
        UTF_VERIFY_EQUAL1_STRING_MAC(threshold, thresholdGet,
                       "got another threshold then was set: %d", dev);

        /* call with cntrNum[2], threshold[0x2FFF] */
        cntrNum = 2;
        threshold = 0x2FFF;

        st = cpssDxChPortTxResourceHistogramThresholdSet(dev, cntrNum, threshold);
        UTF_VERIFY_EQUAL1_PARAM_MAC(GT_OK, st, dev);

        /*
            1.2. Call cpssDxChPortTxResourceHistogramThresholdGet
                   with the same parameters.
            Expected: GT_OK and the same values than was set.
        */
        st = cpssDxChPortTxResourceHistogramThresholdGet(dev, cntrNum, &thresholdGet);
        UTF_VERIFY_EQUAL1_STRING_MAC(GT_OK, st,
                  "cpssDxChPortTxResourceHistogramThresholdGet: %d ", dev);

        /* Verifying values */
        UTF_VERIFY_EQUAL1_STRING_MAC(threshold, thresholdGet,
                       "got another threshold then was set: %d", dev);

        /* call with cntrNum[3], threshold[0x3FFF] */
        cntrNum = 3;
        threshold = 0x3FFF;

        st = cpssDxChPortTxResourceHistogramThresholdSet(dev, cntrNum, threshold);
        UTF_VERIFY_EQUAL1_PARAM_MAC(GT_OK, st, dev);

        /*
            1.2. Call cpssDxChPortTxResourceHistogramThresholdGet
                   with the same parameters.
            Expected: GT_OK and the same values than was set.
        */
        st = cpssDxChPortTxResourceHistogramThresholdGet(dev, cntrNum, &thresholdGet);
        UTF_VERIFY_EQUAL1_STRING_MAC(GT_OK, st,
                  "cpssDxChPortTxResourceHistogramThresholdGet: %d ", dev);

        /* Verifying values */
        UTF_VERIFY_EQUAL1_STRING_MAC(threshold, thresholdGet,
                       "got another threshold then was set: %d", dev);

        /*
            1.3. Call api with wrong cntrNum [4].
            Expected: NOT GT_OK.
        */
        cntrNum = 4;

        st = cpssDxChPortTxResourceHistogramThresholdSet(dev, cntrNum, threshold);
        UTF_VERIFY_NOT_EQUAL1_PARAM_MAC(GT_OK, st, dev);

        cntrNum = 0;

        /*
            1.4. Call api with wrong threshold [0x4000].
            Expected: NOT GT_OK.
        */
        threshold = 0x4000;

        st = cpssDxChPortTxResourceHistogramThresholdSet(dev, cntrNum, threshold);
        UTF_VERIFY_NOT_EQUAL1_PARAM_MAC(GT_OK, st, dev);

        threshold = 0;
    }

    /* restore correct values */
    cntrNum = 0;
    threshold = 0;

    /* 2. For not-active devices and devices from non-applicable family */
    /* check that function returns GT_BAD_PARAM.                        */

    /* prepare device iterator */
    PRV_TGF_NOT_APPLIC_DEV_RESET_MAC(&dev, UTF_CH1_E | UTF_CH1_DIAMOND_E | UTF_CH2_E | UTF_CH3_E | UTF_XCAT_E | UTF_XCAT2_E);

    /* go over all non active devices */
    while(GT_OK == prvUtfNextDeviceGet(&dev, GT_FALSE))
    {
        st = cpssDxChPortTxResourceHistogramThresholdSet(dev, cntrNum, threshold);
        UTF_VERIFY_EQUAL1_PARAM_MAC(GT_NOT_APPLICABLE_DEVICE, st, dev);
    }

    /* 3. Call function with out of bound value for device id.*/
    dev = PRV_CPSS_MAX_PP_DEVICES_CNS;

    st = cpssDxChPortTxResourceHistogramThresholdSet(dev, cntrNum, threshold);
    UTF_VERIFY_EQUAL1_PARAM_MAC(GT_BAD_PARAM, st, dev);
}

/*----------------------------------------------------------------------------*/
/*
GT_STATUS cpssDxChPortTxSharedPoolLimitsGet
(
    IN  GT_U8    dev,
    IN  GT_U32   poolNum,
    OUT GT_U32   *maxBufNumPtr,
    OUT GT_U32   *maxDescNumPtr
)
*/
UTF_TEST_CASE_MAC(cpssDxChPortTxSharedPoolLimitsGet)
{
/*
    ITERATE_DEVICES(Lion)
    1.1. Call with poolNum[0 / 7],
    Expected: GT_OK.
    1.2. Call api with wrong poolNum [8].
    Expected: NOT GT_OK.
    1.3. Call api with wrong maxBufNumPtr [NULL].
    Expected: GT_BAD_PTR.
    1.4. Call api with wrong maxDescNumPtr [NULL].
    Expected: GT_BAD_PTR.
*/
    GT_STATUS st = GT_OK;

    GT_U8    dev;
    GT_U32   poolNum = 0;
    GT_U32   maxBufNum;
    GT_U32   maxDescNum;

    /* prepare device iterator */
    PRV_TGF_NOT_APPLIC_DEV_RESET_MAC(&dev, UTF_CH1_E | UTF_CH1_DIAMOND_E | UTF_CH2_E | UTF_CH3_E | UTF_XCAT_E | UTF_XCAT2_E);

    /* 1. Go over all active devices. */
    while (GT_OK == prvUtfNextDeviceGet(&dev, GT_TRUE))
    {
        /*
            1.1. Call with poolNum[0 / 7],
            Expected: GT_OK.
        */

        /* call with poolNum[0] */
        poolNum = 0;

        st = cpssDxChPortTxSharedPoolLimitsGet(dev, poolNum, &maxBufNum, &maxDescNum);
        UTF_VERIFY_EQUAL1_PARAM_MAC(GT_OK, st, dev);

        /* call with poolNum[7] */
        poolNum = 7;

        st = cpssDxChPortTxSharedPoolLimitsGet(dev, poolNum, &maxBufNum, &maxDescNum);
        UTF_VERIFY_EQUAL1_PARAM_MAC(GT_OK, st, dev);

        /*
            1.2. Call api with wrong poolNum [8].
            Expected: NOT GT_OK.
        */
        poolNum = 8;

        st = cpssDxChPortTxSharedPoolLimitsGet(dev, poolNum, &maxBufNum, &maxDescNum);
        UTF_VERIFY_NOT_EQUAL1_PARAM_MAC(GT_OK, st, dev);

        poolNum = 0;

        /*
            1.3. Call api with wrong maxBufNumPtr [NULL].
            Expected: GT_BAD_PTR.
        */
        st = cpssDxChPortTxSharedPoolLimitsGet(dev, poolNum, NULL, &maxDescNum);
        UTF_VERIFY_EQUAL1_STRING_MAC(GT_BAD_PTR, st, "maxBufNumPtr is NULL", dev);

        /*
            1.4. Call api with wrong maxDescNumPtr [NULL].
            Expected: GT_BAD_PTR.
        */
        st = cpssDxChPortTxSharedPoolLimitsGet(dev, poolNum, &maxBufNum, NULL);
        UTF_VERIFY_EQUAL1_STRING_MAC(GT_BAD_PTR, st, "maxDescNumPtr is NULL", dev);
    }

    /* restore correct values */
    poolNum = 0;

    /* 2. For not-active devices and devices from non-applicable family */
    /* check that function returns GT_BAD_PARAM.                        */

    /* prepare device iterator */
    PRV_TGF_NOT_APPLIC_DEV_RESET_MAC(&dev, UTF_CH1_E | UTF_CH1_DIAMOND_E | UTF_CH2_E | UTF_CH3_E | UTF_XCAT_E | UTF_XCAT2_E);

    /* go over all non active devices */
    while(GT_OK == prvUtfNextDeviceGet(&dev, GT_FALSE))
    {
        st = cpssDxChPortTxSharedPoolLimitsGet(dev, poolNum, &maxBufNum, &maxDescNum);
        UTF_VERIFY_EQUAL1_PARAM_MAC(GT_NOT_APPLICABLE_DEVICE, st, dev);
    }

    /* 3. Call function with out of bound value for device id.*/
    dev = PRV_CPSS_MAX_PP_DEVICES_CNS;

    st = cpssDxChPortTxSharedPoolLimitsGet(dev, poolNum, &maxBufNum, &maxDescNum);
    UTF_VERIFY_EQUAL1_PARAM_MAC(GT_BAD_PARAM, st, dev);
}

/*----------------------------------------------------------------------------*/
/*
GT_STATUS cpssDxChPortTxSharedPoolLimitsSet
(
    IN  GT_U8  dev,
    IN  GT_U32 poolNum,
    IN  GT_U32 maxBufNum,
    IN  GT_U32 maxDescNum
)
*/
UTF_TEST_CASE_MAC(cpssDxChPortTxSharedPoolLimitsSet)
{
/*
ITERATE_DEVICES(Lion)
1.1. Call with poolNum[0 / 3 / 7],
               maxBufNum[0 / 3 / 0x3FFF],
               maxDescNum[0 / 3 / 0x3FFF],
Expected: GT_OK.
1.3. Call api with wrong poolNum [8].
Expected: NOT GT_OK.
1.4. Call api with wrong maxBufNum [0x4000].
Expected: NOT GT_OK.
1.5. Call api with wrong maxDescNum [0x4000].
Expected: NOT GT_OK.
*/
    GT_STATUS st = GT_OK;

    GT_U8  dev;
    GT_U32 poolNum = 0;
    GT_U32 maxBufNum = 0;
    GT_U32 maxDescNum = 0;
    GT_U32 maxBufNumGet = 0;
    GT_U32 maxDescNumGet = 0;

    /* prepare device iterator */
    PRV_TGF_NOT_APPLIC_DEV_RESET_MAC(&dev, UTF_CH1_E | UTF_CH1_DIAMOND_E | UTF_CH2_E | UTF_CH3_E | UTF_XCAT_E | UTF_XCAT2_E);

    /* 1. Go over all active devices. */
    while (GT_OK == prvUtfNextDeviceGet(&dev, GT_TRUE))
    {
        /*
            1.1. Call with poolNum[0 / 3 / 7], maxBufNum[0 / 3 / 0x3FFF],
                           maxDescNum[0 / 3 / 0x3FFF],
            Expected: GT_OK.
        */

        /* call with poolNum[0], maxBufNum[0], maxDescNum[0] */
        poolNum = 0;
        maxBufNum = 0;
        maxDescNum = 0;

        st = cpssDxChPortTxSharedPoolLimitsSet(dev, poolNum, maxBufNum, maxDescNum);
        UTF_VERIFY_EQUAL1_PARAM_MAC(GT_OK, st, dev);

        /*
            1.2. Call cpssDxChPortTxSharedPoolLimitsGet with the same parameters.
            Expected: GT_OK and the same values than was set.
        */
        st = cpssDxChPortTxSharedPoolLimitsGet(dev, poolNum, &maxBufNumGet, &maxDescNumGet);
        UTF_VERIFY_EQUAL1_STRING_MAC(GT_OK, st,
                  "cpssDxChPortTxSharedPoolLimitsGet: %d ", dev);

        /* Verifying values */
        UTF_VERIFY_EQUAL1_STRING_MAC(maxBufNum, maxBufNumGet,
                       "got another maxBufNum then was set: %d", dev);
        UTF_VERIFY_EQUAL1_STRING_MAC(maxDescNum, maxDescNumGet,
                       "got another maxDescNum then was set: %d", dev);

        /* call with poolNum[3], maxBufNum[3], maxDescNum[3] */
        poolNum = 3;
        maxBufNum = 3;
        maxDescNum = 3;

        st = cpssDxChPortTxSharedPoolLimitsSet(dev, poolNum, maxBufNum, maxDescNum);
        UTF_VERIFY_EQUAL1_PARAM_MAC(GT_OK, st, dev);

        /*
            1.2. Call cpssDxChPortTxSharedPoolLimitsGet with the same parameters.
            Expected: GT_OK and the same values than was set.
        */
        st = cpssDxChPortTxSharedPoolLimitsGet(dev, poolNum, &maxBufNumGet, &maxDescNumGet);
        UTF_VERIFY_EQUAL1_STRING_MAC(GT_OK, st,
                  "cpssDxChPortTxSharedPoolLimitsGet: %d ", dev);

        /* Verifying values */
        UTF_VERIFY_EQUAL1_STRING_MAC(maxBufNum, maxBufNumGet,
                       "got another maxBufNum then was set: %d", dev);
        UTF_VERIFY_EQUAL1_STRING_MAC(maxDescNum, maxDescNumGet,
                       "got another maxDescNum then was set: %d", dev);

        /* call with poolNum[7], maxBufNum[0x3FFF], maxDescNum[0x3FFF] */
        poolNum = 7;
        maxBufNum = 0x3FFF;
        maxDescNum = 0x3FFF;

        st = cpssDxChPortTxSharedPoolLimitsSet(dev, poolNum, maxBufNum, maxDescNum);
        UTF_VERIFY_EQUAL1_PARAM_MAC(GT_OK, st, dev);

        /*
            1.2. Call cpssDxChPortTxSharedPoolLimitsGet with the same parameters.
            Expected: GT_OK and the same values than was set.
        */
        st = cpssDxChPortTxSharedPoolLimitsGet(dev, poolNum, &maxBufNumGet, &maxDescNumGet);
        UTF_VERIFY_EQUAL1_STRING_MAC(GT_OK, st,
                  "cpssDxChPortTxSharedPoolLimitsGet: %d ", dev);

        /* Verifying values */
        UTF_VERIFY_EQUAL1_STRING_MAC(maxBufNum, maxBufNumGet,
                       "got another maxBufNum then was set: %d", dev);
        UTF_VERIFY_EQUAL1_STRING_MAC(maxDescNum, maxDescNumGet,
                       "got another maxDescNum then was set: %d", dev);

        /*
            1.3. Call api with wrong poolNum [8].
            Expected: NOT GT_OK.
        */
        poolNum = 8;

        st = cpssDxChPortTxSharedPoolLimitsSet(dev, poolNum, maxBufNum, maxDescNum);
        UTF_VERIFY_NOT_EQUAL1_PARAM_MAC(GT_OK, st, dev);

        poolNum = 0;

        /*
            1.4. Call api with wrong maxBufNum [0x4000].
            Expected: NOT GT_OK.
        */
        maxBufNum = 0x4000;

        st = cpssDxChPortTxSharedPoolLimitsSet(dev, poolNum, maxBufNum, maxDescNum);
        UTF_VERIFY_NOT_EQUAL1_PARAM_MAC(GT_OK, st, dev);

        maxBufNum = 0;

        /*
            1.5. Call api with wrong maxDescNum [0x4000].
            Expected: NOT GT_OK.
        */
        maxDescNum = 0x4000;

        st = cpssDxChPortTxSharedPoolLimitsSet(dev, poolNum, maxBufNum, maxDescNum);
        UTF_VERIFY_NOT_EQUAL1_PARAM_MAC(GT_OK, st, dev);

        maxDescNum = 0;
    }

    /* restore correct values */
    poolNum = 0;
    maxBufNum = 0;
    maxDescNum = 0;

    /* 2. For not-active devices and devices from non-applicable family */
    /* check that function returns GT_BAD_PARAM.                        */

    /* prepare device iterator */
    PRV_TGF_NOT_APPLIC_DEV_RESET_MAC(&dev, UTF_CH1_E | UTF_CH1_DIAMOND_E | UTF_CH2_E | UTF_CH3_E | UTF_XCAT_E | UTF_XCAT2_E);

    /* go over all non active devices */
    while(GT_OK == prvUtfNextDeviceGet(&dev, GT_FALSE))
    {
        st = cpssDxChPortTxSharedPoolLimitsSet(dev, poolNum, maxBufNum, maxDescNum);
        UTF_VERIFY_EQUAL1_PARAM_MAC(GT_NOT_APPLICABLE_DEVICE, st, dev);
    }

    /* 3. Call function with out of bound value for device id.*/
    dev = PRV_CPSS_MAX_PP_DEVICES_CNS;

    st = cpssDxChPortTxSharedPoolLimitsSet(dev, poolNum, maxBufNum, maxDescNum);
    UTF_VERIFY_EQUAL1_PARAM_MAC(GT_BAD_PARAM, st, dev);
}

/*----------------------------------------------------------------------------*/
/*
GT_STATUS cpssDxChPortTxSharedResourceBufNumberGet
(
    IN  GT_U8   dev,
    IN  GT_U32  poolNum,
    OUT GT_U32  *numberPtr
)
*/
UTF_TEST_CASE_MAC(cpssDxChPortTxSharedResourceBufNumberGet)
{
/*
    ITERATE_DEVICES(Lion)
    1.1. Call with poolNum[0 / 3 / 7],
    Expected: GT_OK.
    1.2. Call api with wrong poolNum [8].
    Expected: NOT GT_OK.
    1.3. Call api with wrong numberPtr [NULL].
    Expected: GT_BAD_PTR.
*/
    GT_STATUS st = GT_OK;

    GT_U8   dev;
    GT_U32  poolNum = 0;
    GT_U32  number;

    /* prepare device iterator */
    PRV_TGF_NOT_APPLIC_DEV_RESET_MAC(&dev, UTF_CH1_E | UTF_CH1_DIAMOND_E | UTF_CH2_E | UTF_CH3_E | UTF_XCAT_E | UTF_XCAT2_E);

    /* 1. Go over all active devices. */
    while (GT_OK == prvUtfNextDeviceGet(&dev, GT_TRUE))
    {
        /*
            1.1. Call with poolNum[0 / 3 / 7],
            Expected: GT_OK.
        */
        /* call with poolNum[0] */
        poolNum = 0;

        st = cpssDxChPortTxSharedResourceBufNumberGet(dev, poolNum, &number);
        UTF_VERIFY_EQUAL1_PARAM_MAC(GT_OK, st, dev);

        /* call with poolNum[3] */
        poolNum = 3;

        st = cpssDxChPortTxSharedResourceBufNumberGet(dev, poolNum, &number);
        UTF_VERIFY_EQUAL1_PARAM_MAC(GT_OK, st, dev);

        /* call with poolNum[7] */
        poolNum = 7;

        st = cpssDxChPortTxSharedResourceBufNumberGet(dev, poolNum, &number);
        UTF_VERIFY_EQUAL1_PARAM_MAC(GT_OK, st, dev);

        /*
            1.2. Call api with wrong poolNum [8].
            Expected: NOT GT_OK.
        */
        poolNum = 8;

        st = cpssDxChPortTxSharedResourceBufNumberGet(dev, poolNum, &number);
        UTF_VERIFY_NOT_EQUAL1_PARAM_MAC(GT_OK, st, dev);

        poolNum = 0;

        /*
            1.3. Call api with wrong numberPtr [NULL].
            Expected: GT_BAD_PTR.
        */
        st = cpssDxChPortTxSharedResourceBufNumberGet(dev, poolNum, NULL);
        UTF_VERIFY_EQUAL1_STRING_MAC(GT_BAD_PTR, st, "numberPtr is NULL", dev);
    }

    /* restore correct values */
    poolNum = 0;

    /* 2. For not-active devices and devices from non-applicable family */
    /* check that function returns GT_BAD_PARAM.                        */

    /* prepare device iterator */
    PRV_TGF_NOT_APPLIC_DEV_RESET_MAC(&dev, UTF_CH1_E | UTF_CH1_DIAMOND_E | UTF_CH2_E | UTF_CH3_E | UTF_XCAT_E | UTF_XCAT2_E);

    /* go over all non active devices */
    while(GT_OK == prvUtfNextDeviceGet(&dev, GT_FALSE))
    {
        st = cpssDxChPortTxSharedResourceBufNumberGet(dev, poolNum, &number);
        UTF_VERIFY_EQUAL1_PARAM_MAC(GT_NOT_APPLICABLE_DEVICE, st, dev);
    }

    /* 3. Call function with out of bound value for device id.*/
    dev = PRV_CPSS_MAX_PP_DEVICES_CNS;

    st = cpssDxChPortTxSharedResourceBufNumberGet(dev, poolNum, &number);
    UTF_VERIFY_EQUAL1_PARAM_MAC(GT_BAD_PARAM, st, dev);
}

/*----------------------------------------------------------------------------*/
/*
GT_STATUS cpssDxChPortTxSharedResourceDescNumberGet
(
    IN  GT_U8   dev,
    IN  GT_U32  poolNum,
    OUT GT_U32  *numberPtr
)
*/
UTF_TEST_CASE_MAC(cpssDxChPortTxSharedResourceDescNumberGet)
{
/*
    ITERATE_DEVICES(Lion)
    1.1. Call with poolNum[0 / 3 / 7],
    Expected: GT_OK.
    1.2. Call api with wrong poolNum [8].
    Expected: NOT GT_OK.
    1.3. Call api with wrong numberPtr [NULL].
    Expected: GT_BAD_PTR.
*/
    GT_STATUS st = GT_OK;

    GT_U8   dev;
    GT_U32  poolNum = 0;
    GT_U32  number;

    /* prepare device iterator */
    PRV_TGF_NOT_APPLIC_DEV_RESET_MAC(&dev, UTF_CH1_E | UTF_CH1_DIAMOND_E | UTF_CH2_E | UTF_CH3_E | UTF_XCAT_E | UTF_XCAT2_E);

    /* 1. Go over all active devices. */
    while (GT_OK == prvUtfNextDeviceGet(&dev, GT_TRUE))
    {
        /*
            1.1. Call with poolNum[0 / 3 / 7],
            Expected: GT_OK.
        */
        /* call with poolNum[0] */
        poolNum = 0;

        st = cpssDxChPortTxSharedResourceDescNumberGet(dev, poolNum, &number);
        UTF_VERIFY_EQUAL1_PARAM_MAC(GT_OK, st, dev);

        /* call with poolNum[3] */
        poolNum = 3;

        st = cpssDxChPortTxSharedResourceDescNumberGet(dev, poolNum, &number);
        UTF_VERIFY_EQUAL1_PARAM_MAC(GT_OK, st, dev);

        /* call with poolNum[7] */
        poolNum = 7;

        st = cpssDxChPortTxSharedResourceDescNumberGet(dev, poolNum, &number);
        UTF_VERIFY_EQUAL1_PARAM_MAC(GT_OK, st, dev);

        /*
            1.2. Call api with wrong poolNum [8].
            Expected: NOT GT_OK.
        */
        poolNum = 8;

        st = cpssDxChPortTxSharedResourceDescNumberGet(dev, poolNum, &number);
        UTF_VERIFY_NOT_EQUAL1_PARAM_MAC(GT_OK, st, dev);

        poolNum = 0;

        /*
            1.3. Call api with wrong numberPtr [NULL].
            Expected: GT_BAD_PTR.
        */
        st = cpssDxChPortTxSharedResourceDescNumberGet(dev, poolNum, NULL);
        UTF_VERIFY_EQUAL1_STRING_MAC(GT_BAD_PTR, st, "numberPtr is NULL", dev);
    }

    /* restore correct values */
    poolNum = 0;

    /* 2. For not-active devices and devices from non-applicable family */
    /* check that function returns GT_BAD_PARAM.                        */

    /* prepare device iterator */
    PRV_TGF_NOT_APPLIC_DEV_RESET_MAC(&dev, UTF_CH1_E | UTF_CH1_DIAMOND_E | UTF_CH2_E | UTF_CH3_E | UTF_XCAT_E | UTF_XCAT2_E);

    /* go over all non active devices */
    while(GT_OK == prvUtfNextDeviceGet(&dev, GT_FALSE))
    {
        st = cpssDxChPortTxSharedResourceDescNumberGet(dev, poolNum, &number);
        UTF_VERIFY_EQUAL1_PARAM_MAC(GT_NOT_APPLICABLE_DEVICE, st, dev);
    }

    /* 3. Call function with out of bound value for device id.*/
    dev = PRV_CPSS_MAX_PP_DEVICES_CNS;

    st = cpssDxChPortTxSharedResourceDescNumberGet(dev, poolNum, &number);
    UTF_VERIFY_EQUAL1_PARAM_MAC(GT_BAD_PARAM, st, dev);
}

/*----------------------------------------------------------------------------*/
/*
GT_STATUS cpssDxChPortTxSniffedBufNumberGet
(
    IN  GT_U8    dev,
    OUT GT_U32   *rxNumberPtr,
    OUT GT_U32   *txNumberPtr
)
*/
UTF_TEST_CASE_MAC(cpssDxChPortTxSniffedBufNumberGet)
{
/*
    ITERATE_DEVICES(Lion)
    1.1. Call with not null rxNumberPtr.
    Expected: GT_OK.
    1.2. Call api with wrong rxNumberPtr [NULL].
    Expected: GT_BAD_PTR.
    1.3. Call api with wrong txNumberPtr [NULL].
    Expected: GT_BAD_PTR.
*/
    GT_STATUS st = GT_OK;

    GT_U8    dev;
    GT_U32   rxNumber;
    GT_U32   txNumber;

    /* prepare device iterator */
    PRV_TGF_NOT_APPLIC_DEV_RESET_MAC(&dev, UTF_CH1_E | UTF_CH1_DIAMOND_E | UTF_CH2_E | UTF_CH3_E | UTF_XCAT_E | UTF_XCAT2_E);

    /* 1. Go over all active devices. */
    while (GT_OK == prvUtfNextDeviceGet(&dev, GT_TRUE))
    {
        /*
            1.1. Call with not null rxNumberPtr.
            Expected: GT_OK.
        */
        st = cpssDxChPortTxSniffedBufNumberGet(dev, &rxNumber, &txNumber);
        UTF_VERIFY_EQUAL1_PARAM_MAC(GT_OK, st, dev);

        /*
            1.2. Call api with wrong rxNumberPtr [NULL].
            Expected: GT_BAD_PTR.
        */
        st = cpssDxChPortTxSniffedBufNumberGet(dev, NULL, &txNumber);
        UTF_VERIFY_EQUAL1_STRING_MAC(GT_BAD_PTR, st, "rxNumberPtr is NULL", dev);

        /*
            1.3. Call api with wrong txNumberPtr [NULL].
            Expected: GT_BAD_PTR.
        */
        st = cpssDxChPortTxSniffedBufNumberGet(dev, &rxNumber, NULL);
        UTF_VERIFY_EQUAL1_STRING_MAC(GT_BAD_PTR, st, "txNumberPtr is NULL", dev);
    }

    /* restore correct values */

    /* 2. For not-active devices and devices from non-applicable family */
    /* check that function returns GT_BAD_PARAM.                        */

    /* prepare device iterator */
    PRV_TGF_NOT_APPLIC_DEV_RESET_MAC(&dev, UTF_CH1_E | UTF_CH1_DIAMOND_E | UTF_CH2_E | UTF_CH3_E | UTF_XCAT_E | UTF_XCAT2_E);

    /* go over all non active devices */
    while(GT_OK == prvUtfNextDeviceGet(&dev, GT_FALSE))
    {
        st = cpssDxChPortTxSniffedBufNumberGet(dev, &rxNumber, &txNumber);
        UTF_VERIFY_EQUAL1_PARAM_MAC(GT_NOT_APPLICABLE_DEVICE, st, dev);
    }

    /* 3. Call function with out of bound value for device id.*/
    dev = PRV_CPSS_MAX_PP_DEVICES_CNS;

    st = cpssDxChPortTxSniffedBufNumberGet(dev, &rxNumber, &txNumber);
    UTF_VERIFY_EQUAL1_PARAM_MAC(GT_BAD_PARAM, st, dev);
}

/*----------------------------------------------------------------------------*/
/*
GT_STATUS cpssDxChPortTxSniffedBuffersLimitGet
(
    IN  GT_U8    dev,
    OUT GT_U32   *rxSniffMaxBufNumPtr,
    OUT GT_U32   *txSniffMaxBufNumPtr
)
*/
UTF_TEST_CASE_MAC(cpssDxChPortTxSniffedBuffersLimitGet)
{
/*
    ITERATE_DEVICES(Lion)
    1.1. Call with not null rxSniffMaxBufNumPtr.
    Expected: GT_OK.
    1.2. Call api with wrong rxSniffMaxBufNumPtr [NULL].
    Expected: GT_BAD_PTR.
    1.3. Call api with wrong txSniffMaxBufNumPtr [NULL].
    Expected: GT_BAD_PTR.
*/
    GT_STATUS st = GT_OK;

    GT_U8    dev;
    GT_U32   rxSniffMaxBufNum;
    GT_U32   txSniffMaxBufNum;

    /* prepare device iterator */
    PRV_TGF_NOT_APPLIC_DEV_RESET_MAC(&dev, UTF_CH1_E | UTF_CH1_DIAMOND_E | UTF_CH2_E | UTF_CH3_E | UTF_XCAT_E | UTF_XCAT2_E);

    /* 1. Go over all active devices. */
    while (GT_OK == prvUtfNextDeviceGet(&dev, GT_TRUE))
    {
        /*
            1.1. Call with not null rxSniffMaxBufNumPtr.
            Expected: GT_OK.
        */
        st = cpssDxChPortTxSniffedBuffersLimitGet(dev, &rxSniffMaxBufNum, &txSniffMaxBufNum);
        UTF_VERIFY_EQUAL1_PARAM_MAC(GT_OK, st, dev);

        /*
            1.2. Call api with wrong rxSniffMaxBufNumPtr [NULL].
            Expected: GT_BAD_PTR.
        */
        st = cpssDxChPortTxSniffedBuffersLimitGet(dev, NULL, &txSniffMaxBufNum);
        UTF_VERIFY_EQUAL1_STRING_MAC(GT_BAD_PTR, st, "rxSniffMaxBufNumPtr is NULL", dev);

        /*
            1.3. Call api with wrong txSniffMaxBufNumPtr [NULL].
            Expected: GT_BAD_PTR.
        */
        st = cpssDxChPortTxSniffedBuffersLimitGet(dev, &rxSniffMaxBufNum, NULL);
        UTF_VERIFY_EQUAL1_STRING_MAC(GT_BAD_PTR, st, "txSniffMaxBufNumPtr is NULL", dev);
    }

    /* restore correct values */

    /* 2. For not-active devices and devices from non-applicable family */
    /* check that function returns GT_BAD_PARAM.                        */

    /* prepare device iterator */
    PRV_TGF_NOT_APPLIC_DEV_RESET_MAC(&dev, UTF_CH1_E | UTF_CH1_DIAMOND_E | UTF_CH2_E | UTF_CH3_E | UTF_XCAT_E | UTF_XCAT2_E);

    /* go over all non active devices */
    while(GT_OK == prvUtfNextDeviceGet(&dev, GT_FALSE))
    {
        st = cpssDxChPortTxSniffedBuffersLimitGet(dev, &rxSniffMaxBufNum, &txSniffMaxBufNum);
        UTF_VERIFY_EQUAL1_PARAM_MAC(GT_NOT_APPLICABLE_DEVICE, st, dev);
    }

    /* 3. Call function with out of bound value for device id.*/
    dev = PRV_CPSS_MAX_PP_DEVICES_CNS;

    st = cpssDxChPortTxSniffedBuffersLimitGet(dev, &rxSniffMaxBufNum, &txSniffMaxBufNum);
    UTF_VERIFY_EQUAL1_PARAM_MAC(GT_BAD_PARAM, st, dev);
}

/*----------------------------------------------------------------------------*/
/*
GT_STATUS cpssDxChPortTxSniffedBuffersLimitSet
(
    IN  GT_U8  dev,
    IN  GT_U32 rxSniffMaxBufNum,
    IN  GT_U32 txSniffMaxBufNum
)
*/
UTF_TEST_CASE_MAC(cpssDxChPortTxSniffedBuffersLimitSet)
{
/*
    ITERATE_DEVICES(Lion)
    1.1. Call with rxSniffMaxBufNum[0 / 3 / 0xFFFF],
                   txSniffMaxBufNum[0 / 3 / 0xFFFF],
    Expected: GT_OK.
    1.2. Call cpssDxChPortTxSniffedBuffersLimitGet
           with the same parameters.
    Expected: GT_OK and the same values than was set.
    1.3. Call api with wrong rxSniffMaxBufNum [0xFFFF + 1].
    Expected: NOT GT_OK.
    1.4. Call api with wrong txSniffMaxBufNum [0xFFFF + 1].
    Expected: NOT GT_OK.
*/
    GT_STATUS st = GT_OK;

    GT_U8  dev;
    GT_U32 rxSniffMaxBufNum = 0;
    GT_U32 txSniffMaxBufNum = 0;
    GT_U32 rxSniffMaxBufNumGet = 0;
    GT_U32 txSniffMaxBufNumGet = 0;

    /* prepare device iterator */
    PRV_TGF_NOT_APPLIC_DEV_RESET_MAC(&dev, UTF_CH1_E | UTF_CH1_DIAMOND_E | UTF_CH2_E | UTF_CH3_E | UTF_XCAT_E | UTF_XCAT2_E);

    /* 1. Go over all active devices. */
    while (GT_OK == prvUtfNextDeviceGet(&dev, GT_TRUE))
    {
        /*
            1.1. Call with rxSniffMaxBufNum[0 / 3 / 0xFFFF],
                           txSniffMaxBufNum[0 / 3 / 0xFFFF],
            Expected: GT_OK.
        */

        /* call with rxSniffMaxBufNum[0], txSniffMaxBufNum[0] */
        rxSniffMaxBufNum = 0;
        txSniffMaxBufNum = 0;

        st = cpssDxChPortTxSniffedBuffersLimitSet(dev, rxSniffMaxBufNum, txSniffMaxBufNum);
        UTF_VERIFY_EQUAL1_PARAM_MAC(GT_OK, st, dev);

        /*
            1.2. Call cpssDxChPortTxSniffedBuffersLimitGet
                   with the same parameters.
            Expected: GT_OK and the same values than was set.
        */
        st = cpssDxChPortTxSniffedBuffersLimitGet(dev, &rxSniffMaxBufNumGet, &txSniffMaxBufNumGet);
        UTF_VERIFY_EQUAL1_STRING_MAC(GT_OK, st,
                  "cpssDxChPortTxSniffedBuffersLimitGet: %d ", dev);

        /* Verifying values */
        UTF_VERIFY_EQUAL1_STRING_MAC(rxSniffMaxBufNum, rxSniffMaxBufNumGet,
                       "got another rxSniffMaxBufNum then was set: %d", dev);
        UTF_VERIFY_EQUAL1_STRING_MAC(txSniffMaxBufNum, txSniffMaxBufNumGet,
                       "got another txSniffMaxBufNum then was set: %d", dev);

        /* call with rxSniffMaxBufNum[3], txSniffMaxBufNum[3] */
        rxSniffMaxBufNum = 3;
        txSniffMaxBufNum = 3;

        st = cpssDxChPortTxSniffedBuffersLimitSet(dev, rxSniffMaxBufNum, txSniffMaxBufNum);
        UTF_VERIFY_EQUAL1_PARAM_MAC(GT_OK, st, dev);

        /*
            1.2. Call cpssDxChPortTxSniffedBuffersLimitGet
                   with the same parameters.
            Expected: GT_OK and the same values than was set.
        */
        st = cpssDxChPortTxSniffedBuffersLimitGet(dev, &rxSniffMaxBufNumGet, &txSniffMaxBufNumGet);
        UTF_VERIFY_EQUAL1_STRING_MAC(GT_OK, st,
                  "cpssDxChPortTxSniffedBuffersLimitGet: %d ", dev);

        /* Verifying values */
        UTF_VERIFY_EQUAL1_STRING_MAC(rxSniffMaxBufNum, rxSniffMaxBufNumGet,
                       "got another rxSniffMaxBufNum then was set: %d", dev);
        UTF_VERIFY_EQUAL1_STRING_MAC(txSniffMaxBufNum, txSniffMaxBufNumGet,
                       "got another txSniffMaxBufNum then was set: %d", dev);


        /* call with rxSniffMaxBufNum[0xFFFF], txSniffMaxBufNum[0xFFFF] */
        rxSniffMaxBufNum = 0xFFFF;
        txSniffMaxBufNum = 0xFFFF;

        st = cpssDxChPortTxSniffedBuffersLimitSet(dev, rxSniffMaxBufNumGet, txSniffMaxBufNum);
        UTF_VERIFY_EQUAL1_PARAM_MAC(GT_OK, st, dev);

        /*
            1.2. Call cpssDxChPortTxSniffedBuffersLimitGet
                   with the same parameters.
            Expected: GT_OK and the same values than was set.
        */
        st = cpssDxChPortTxSniffedBuffersLimitGet(dev, &rxSniffMaxBufNum, &txSniffMaxBufNumGet);
        UTF_VERIFY_EQUAL1_STRING_MAC(GT_OK, st,
                  "cpssDxChPortTxSniffedBuffersLimitGet: %d ", dev);

        /* Verifying values */
        UTF_VERIFY_EQUAL1_STRING_MAC(rxSniffMaxBufNum, rxSniffMaxBufNumGet,
                       "got another rxSniffMaxBufNum then was set: %d", dev);
        UTF_VERIFY_EQUAL1_STRING_MAC(txSniffMaxBufNum, txSniffMaxBufNumGet,
                       "got another txSniffMaxBufNum then was set: %d", dev);

        /*
            1.3. Call api with wrong rxSniffMaxBufNum [0xFFFF + 1].
            Expected: NOT GT_OK.
        */
        rxSniffMaxBufNum = 0xFFFF + 1;

        st = cpssDxChPortTxSniffedBuffersLimitSet(dev, rxSniffMaxBufNum, txSniffMaxBufNum);
        UTF_VERIFY_NOT_EQUAL1_PARAM_MAC(GT_OK, st, dev);

        rxSniffMaxBufNum = 0;

        /*
            1.4. Call api with wrong txSniffMaxBufNum [0xFFFF + 1].
            Expected: NOT GT_OK.
        */
        txSniffMaxBufNum = 0xFFFF + 1;

        st = cpssDxChPortTxSniffedBuffersLimitSet(dev, rxSniffMaxBufNum, txSniffMaxBufNum);
        UTF_VERIFY_NOT_EQUAL1_PARAM_MAC(GT_OK, st, dev);

        txSniffMaxBufNum = 0;
    }

    /* restore correct values */
    rxSniffMaxBufNum = 0;
    txSniffMaxBufNum = 0;

    /* 2. For not-active devices and devices from non-applicable family */
    /* check that function returns GT_BAD_PARAM.                        */

    /* prepare device iterator */
    PRV_TGF_NOT_APPLIC_DEV_RESET_MAC(&dev, UTF_CH1_E | UTF_CH1_DIAMOND_E | UTF_CH2_E | UTF_CH3_E | UTF_XCAT_E | UTF_XCAT2_E);

    /* go over all non active devices */
    while(GT_OK == prvUtfNextDeviceGet(&dev, GT_FALSE))
    {
        st = cpssDxChPortTxSniffedBuffersLimitSet(dev, rxSniffMaxBufNum, txSniffMaxBufNum);
        UTF_VERIFY_EQUAL1_PARAM_MAC(GT_NOT_APPLICABLE_DEVICE, st, dev);
    }

    /* 3. Call function with out of bound value for device id.*/
    dev = PRV_CPSS_MAX_PP_DEVICES_CNS;

    st = cpssDxChPortTxSniffedBuffersLimitSet(dev, rxSniffMaxBufNum, txSniffMaxBufNum);
    UTF_VERIFY_EQUAL1_PARAM_MAC(GT_BAD_PARAM, st, dev);
}

/*----------------------------------------------------------------------------*/
/*
GT_STATUS cpssDxChPortTxSniffedDescNumberGet
(
    IN  GT_U8    dev,
    OUT GT_U32   *rxNumberPtr,
    OUT GT_U32   *txNumberPtr
)
*/
UTF_TEST_CASE_MAC(cpssDxChPortTxSniffedDescNumberGet)
{
/*
    ITERATE_DEVICES(DxCh3 and above)
    1.1. Call with not null rxNumberPtr.
    Expected: GT_OK.
    1.2. Call api with wrong rxNumberPtr [NULL].
    Expected: GT_BAD_PTR.
    1.3. Call api with wrong txNumberPtr [NULL].
    Expected: GT_BAD_PTR.
*/
    GT_STATUS st = GT_OK;

    GT_U8    dev;
    GT_U32   rxNumber;
    GT_U32   txNumber;

    /* prepare device iterator */
    PRV_TGF_NOT_APPLIC_DEV_RESET_MAC(&dev, UTF_CH1_E | UTF_CH1_DIAMOND_E | UTF_CH2_E);

    /* 1. Go over all active devices. */
    while (GT_OK == prvUtfNextDeviceGet(&dev, GT_TRUE))
    {
        /*
            1.1. Call with not null rxNumberPtr.
            Expected: GT_OK.
        */
        st = cpssDxChPortTxSniffedDescNumberGet(dev, &rxNumber, &txNumber);
        UTF_VERIFY_EQUAL1_PARAM_MAC(GT_OK, st, dev);

        /*
            1.2. Call api with wrong rxNumberPtr [NULL].
            Expected: GT_BAD_PTR.
        */
        st = cpssDxChPortTxSniffedDescNumberGet(dev, NULL, &txNumber);
        UTF_VERIFY_EQUAL1_STRING_MAC(GT_BAD_PTR, st, "rxNumberPtr is NULL", dev);

        /*
            1.3. Call api with wrong txNumberPtr [NULL].
            Expected: GT_BAD_PTR.
        */
        st = cpssDxChPortTxSniffedDescNumberGet(dev, &rxNumber, NULL);
        UTF_VERIFY_EQUAL1_STRING_MAC(GT_BAD_PTR, st, "txNumberPtr is NULL", dev);
    }

    /* restore correct values */

    /* 2. For not-active devices and devices from non-applicable family */
    /* check that function returns GT_BAD_PARAM.                        */

    /* prepare device iterator */
    PRV_TGF_NOT_APPLIC_DEV_RESET_MAC(&dev, UTF_CH1_E | UTF_CH1_DIAMOND_E | UTF_CH2_E);

    /* go over all non active devices */
    while(GT_OK == prvUtfNextDeviceGet(&dev, GT_FALSE))
    {
        st = cpssDxChPortTxSniffedDescNumberGet(dev, &rxNumber, &txNumber);
        UTF_VERIFY_EQUAL1_PARAM_MAC(GT_NOT_APPLICABLE_DEVICE, st, dev);
    }

    /* 3. Call function with out of bound value for device id.*/
    dev = PRV_CPSS_MAX_PP_DEVICES_CNS;

    st = cpssDxChPortTxSniffedDescNumberGet(dev, &rxNumber, &txNumber);
    UTF_VERIFY_EQUAL1_PARAM_MAC(GT_BAD_PARAM, st, dev);
}

/*----------------------------------------------------------------------------*/
/*
GT_STATUS cpssDxChPortTxTailDropBufferConsumptionModeGet
(
    IN  GT_U8                                                     dev,
    OUT CPSS_DXCH_PORT_TX_TAIL_DROP_BUFFER_CONSUMPTION_MODE_ENT   *modePtr,
    OUT GT_U32                                                    *lengthPtr
)
*/
UTF_TEST_CASE_MAC(cpssDxChPortTxTailDropBufferConsumptionModeGet)
{
/*
    ITERATE_DEVICES(Lion)
    1.1. Call with not null modePtr.
    Expected: GT_OK.
    1.2. Call api with wrong modePtr [NULL].
    Expected: GT_BAD_PTR.
    1.3. Call api with wrong lengthPtr [NULL].
    Expected: GT_BAD_PTR.
*/
    GT_STATUS st = GT_OK;

    GT_U8                                                     dev;
    CPSS_DXCH_PORT_TX_TAIL_DROP_BUFFER_CONSUMPTION_MODE_ENT   mode;
    GT_U32                                                    length;

    /* prepare device iterator */
    PRV_TGF_NOT_APPLIC_DEV_RESET_MAC(&dev, UTF_CH1_E | UTF_CH1_DIAMOND_E | UTF_CH2_E | UTF_CH3_E | UTF_XCAT_E | UTF_XCAT2_E);

    /* 1. Go over all active devices. */
    while (GT_OK == prvUtfNextDeviceGet(&dev, GT_TRUE))
    {
        /*
            1.1. Call with not null modePtr.
            Expected: GT_OK.
        */
        st = cpssDxChPortTxTailDropBufferConsumptionModeGet(dev,
                                     &mode, &length);
        UTF_VERIFY_EQUAL1_PARAM_MAC(GT_OK, st, dev);

        /*
            1.2. Call api with wrong modePtr [NULL].
            Expected: GT_BAD_PTR.
        */
        st = cpssDxChPortTxTailDropBufferConsumptionModeGet(dev, NULL, &length);
        UTF_VERIFY_EQUAL1_STRING_MAC(GT_BAD_PTR, st, "modePtr is NULL", dev);

        /*
            1.3. Call api with wrong lengthPtr [NULL].
            Expected: GT_BAD_PTR.
        */
        st = cpssDxChPortTxTailDropBufferConsumptionModeGet(dev, &mode, NULL);
        UTF_VERIFY_EQUAL1_STRING_MAC(GT_BAD_PTR, st, "lengthPtr is NULL", dev);
    }

    /* restore correct values */

    /* 2. For not-active devices and devices from non-applicable family */
    /* check that function returns GT_BAD_PARAM.                        */

    /* prepare device iterator */
    PRV_TGF_NOT_APPLIC_DEV_RESET_MAC(&dev, UTF_CH1_E | UTF_CH1_DIAMOND_E | UTF_CH2_E | UTF_CH3_E | UTF_XCAT_E | UTF_XCAT2_E);

    /* go over all non active devices */
    while(GT_OK == prvUtfNextDeviceGet(&dev, GT_FALSE))
    {
        st = cpssDxChPortTxTailDropBufferConsumptionModeGet(dev,
                                     &mode, &length);
        UTF_VERIFY_EQUAL1_PARAM_MAC(GT_NOT_APPLICABLE_DEVICE, st, dev);
    }

    /* 3. Call function with out of bound value for device id.*/
    dev = PRV_CPSS_MAX_PP_DEVICES_CNS;

    st = cpssDxChPortTxTailDropBufferConsumptionModeGet(dev,
                                     &mode, &length);
    UTF_VERIFY_EQUAL1_PARAM_MAC(GT_BAD_PARAM, st, dev);
}

/*----------------------------------------------------------------------------*/
/*
GT_STATUS cpssDxChPortTxTailDropBufferConsumptionModeSet
(
    IN  GT_U8                                                   dev,
    IN  CPSS_DXCH_PORT_TX_TAIL_DROP_BUFFER_CONSUMPTION_MODE_ENT mode,
    IN  GT_U32                                                  length
)
*/
UTF_TEST_CASE_MAC(cpssDxChPortTxTailDropBufferConsumptionModeSet)
{
/*
    ITERATE_DEVICES(Lion)
    1.1. Call with mode[CPSS_DXCH_PORT_TX_TAIL_DROP_BUFFER_CONSUMPTION_MODE_BYTE_E /
                        CPSS_DXCH_PORT_TX_TAIL_DROP_BUFFER_CONSUMPTION_MODE_PACKET_E],
                   length[0 / 63],
    Expected: GT_OK.
    1.2. Call cpssDxChPortTxTailDropBufferConsumptionModeGet
           with the same parameters.
    Expected: GT_OK and the same values than was set.
    1.3. Call api with wrong mode [wrong enum values].
    Expected: GT_BAD_PARAM.
    1.4. Call api with wrong length [64].
    mode = CPSS_DXCH_PORT_TX_TAIL_DROP_BUFFER_CONSUMPTION_MODE_PACKET_E;
    Expected: NOT GT_OK.
    1.5. Call api with wrong length [64] (not relevant).
    mode = CPSS_DXCH_PORT_TX_TAIL_DROP_BUFFER_CONSUMPTION_MODE_BYTE_E;
    Expected: GT_OK.
*/
    GT_STATUS st = GT_OK;

    GT_U8                                                   dev;
    CPSS_DXCH_PORT_TX_TAIL_DROP_BUFFER_CONSUMPTION_MODE_ENT mode =
                CPSS_DXCH_PORT_TX_TAIL_DROP_BUFFER_CONSUMPTION_MODE_BYTE_E;
    GT_U32                                                  length = 0;

    CPSS_DXCH_PORT_TX_TAIL_DROP_BUFFER_CONSUMPTION_MODE_ENT modeGet =
                CPSS_DXCH_PORT_TX_TAIL_DROP_BUFFER_CONSUMPTION_MODE_BYTE_E;
    GT_U32                                                  lengthGet = 0;

    /* prepare device iterator */
    PRV_TGF_NOT_APPLIC_DEV_RESET_MAC(&dev, UTF_CH1_E | UTF_CH1_DIAMOND_E | UTF_CH2_E | UTF_CH3_E | UTF_XCAT_E | UTF_XCAT2_E);

    /* 1. Go over all active devices. */
    while (GT_OK == prvUtfNextDeviceGet(&dev, GT_TRUE))
    {
        /*
            1.1. Call with mode[CPSS_DXCH_PORT_TX_TAIL_DROP_BUFFER_CONSUMPTION_MODE_BYTE_E /
                                CPSS_DXCH_PORT_TX_TAIL_DROP_BUFFER_CONSUMPTION_MODE_PACKET_E],
                           length[0 / 63],
            Expected: GT_OK.
        */
        /* call with mode[CPSS_DXCH_PORT_TX_TAIL_DROP_BUFFER_CONSUMPTION_MODE_BYTE_E],
                     length[0] */
        mode = CPSS_DXCH_PORT_TX_TAIL_DROP_BUFFER_CONSUMPTION_MODE_BYTE_E;
        length = 0;

        st = cpssDxChPortTxTailDropBufferConsumptionModeSet(dev, mode, length);
        UTF_VERIFY_EQUAL1_PARAM_MAC(GT_OK, st, dev);

        /*
            1.2. Call cpssDxChPortTxTailDropBufferConsumptionModeGet
                   with the same parameters.
            Expected: GT_OK and the same values than was set.
        */
        st = cpssDxChPortTxTailDropBufferConsumptionModeGet(dev,
                                     &modeGet, &lengthGet);
        UTF_VERIFY_EQUAL1_STRING_MAC(GT_OK, st,
                  "cpssDxChPortTxTailDropBufferConsumptionModeGet: %d ", dev);

        /* Verifying values */
        UTF_VERIFY_EQUAL1_STRING_MAC(mode, modeGet,
                       "got another mode then was set: %d", dev);

        /* call with mode[CPSS_DXCH_PORT_TX_TAIL_DROP_BUFFER_CONSUMPTION_MODE_PACKET_E],
                     length[63] */
        mode = CPSS_DXCH_PORT_TX_TAIL_DROP_BUFFER_CONSUMPTION_MODE_PACKET_E;
        length = 63;

        st = cpssDxChPortTxTailDropBufferConsumptionModeSet(dev, mode, length);
        UTF_VERIFY_EQUAL1_PARAM_MAC(GT_OK, st, dev);

        /*
            1.2. Call cpssDxChPortTxTailDropBufferConsumptionModeGet
                   with the same parameters.
            Expected: GT_OK and the same values than was set.
        */
        st = cpssDxChPortTxTailDropBufferConsumptionModeGet(dev, &modeGet, &lengthGet);
        UTF_VERIFY_EQUAL1_STRING_MAC(GT_OK, st,
                  "cpssDxChPortTxTailDropBufferConsumptionModeGet: %d ", dev);

        /* Verifying values */
        UTF_VERIFY_EQUAL1_STRING_MAC(mode, modeGet,
                       "got another mode then was set: %d", dev);
        UTF_VERIFY_EQUAL1_STRING_MAC(length, lengthGet,
                       "got another length then was set: %d", dev);

        /*
            1.3. Call api with wrong mode [wrong enum values].
            Expected: GT_BAD_PARAM.
        */
        UTF_ENUMS_CHECK_MAC(cpssDxChPortTxTailDropBufferConsumptionModeSet
                            (dev, mode, length),
                            mode);

        /*
            1.4. Call api with wrong length [64].
            mode = CPSS_DXCH_PORT_TX_TAIL_DROP_BUFFER_CONSUMPTION_MODE_PACKET_E;
            Expected: NOT GT_OK.
        */
        mode = CPSS_DXCH_PORT_TX_TAIL_DROP_BUFFER_CONSUMPTION_MODE_PACKET_E;
        length = 64;

        st = cpssDxChPortTxTailDropBufferConsumptionModeSet(dev, mode, length);
        UTF_VERIFY_NOT_EQUAL1_PARAM_MAC(GT_OK, st, dev);

        length = 0;

        /*
            1.5. Call api with wrong length [64] (not relevant).
            mode = CPSS_DXCH_PORT_TX_TAIL_DROP_BUFFER_CONSUMPTION_MODE_BYTE_E;
            Expected: GT_OK.
        */
        mode = CPSS_DXCH_PORT_TX_TAIL_DROP_BUFFER_CONSUMPTION_MODE_BYTE_E;
        length = 64;

        st = cpssDxChPortTxTailDropBufferConsumptionModeSet(dev, mode, length);
        UTF_VERIFY_EQUAL1_PARAM_MAC(GT_OK, st, dev);

        length = 0;
    }

    /* restore correct values */
    mode = CPSS_DXCH_PORT_TX_TAIL_DROP_BUFFER_CONSUMPTION_MODE_BYTE_E;
    length = 0;

    /* 2. For not-active devices and devices from non-applicable family */
    /* check that function returns GT_BAD_PARAM.                        */

    /* prepare device iterator */
    PRV_TGF_NOT_APPLIC_DEV_RESET_MAC(&dev, UTF_CH1_E | UTF_CH1_DIAMOND_E | UTF_CH2_E | UTF_CH3_E | UTF_XCAT_E | UTF_XCAT2_E);

    /* go over all non active devices */
    while(GT_OK == prvUtfNextDeviceGet(&dev, GT_FALSE))
    {
        st = cpssDxChPortTxTailDropBufferConsumptionModeSet(dev,
                                     mode, length);
        UTF_VERIFY_EQUAL1_PARAM_MAC(GT_NOT_APPLICABLE_DEVICE, st, dev);
    }

    /* 3. Call function with out of bound value for device id.*/
    dev = PRV_CPSS_MAX_PP_DEVICES_CNS;

    st = cpssDxChPortTxTailDropBufferConsumptionModeSet(dev,
                                     mode, length);
    UTF_VERIFY_EQUAL1_PARAM_MAC(GT_BAD_PARAM, st, dev);
}

/*----------------------------------------------------------------------------*/
/*
GT_STATUS cpssDxChPortTxTailDropWrtdMasksGet
(
    IN  GT_U8                       dev,
    OUT CPSS_PORT_TX_WRTD_MASK_LSB  *maskLsbPtr
)
*/
UTF_TEST_CASE_MAC(cpssDxChPortTxTailDropWrtdMasksGet)
{
/*
    ITERATE_DEVICES(Lion,xCat2)
    1.1. Call with not null maskLsbPtr.
    Expected: GT_OK.
    1.2. Call api with wrong maskLsbPtr [NULL].
    Expected: GT_BAD_PTR.
*/
    GT_STATUS st = GT_OK;

    GT_U8                          dev;
    CPSS_PORT_TX_WRTD_MASK_LSB_STC maskLsb;

    /* prepare device iterator */
    PRV_TGF_NOT_APPLIC_DEV_RESET_MAC(&dev, UTF_CH1_E | UTF_CH1_DIAMOND_E | UTF_CH2_E | UTF_CH3_E | UTF_XCAT_E);

    /* 1. Go over all active devices. */
    while (GT_OK == prvUtfNextDeviceGet(&dev, GT_TRUE))
    {
        /*
            1.1. Call with not null maskLsbPtr.
            Expected: GT_OK.
        */
        st = cpssDxChPortTxTailDropWrtdMasksGet(dev, &maskLsb);
        UTF_VERIFY_EQUAL1_PARAM_MAC(GT_OK, st, dev);

        /*
            1.2. Call api with wrong maskLsbPtr [NULL].
            Expected: GT_BAD_PTR.
        */
        st = cpssDxChPortTxTailDropWrtdMasksGet(dev, NULL);
        UTF_VERIFY_EQUAL1_STRING_MAC(GT_BAD_PTR, st, "maskLsbPtr is NULL", dev);
    }

    /* restore correct values */

    /* 2. For not-active devices and devices from non-applicable family */
    /* check that function returns GT_BAD_PARAM.                        */

    /* prepare device iterator */
    PRV_TGF_NOT_APPLIC_DEV_RESET_MAC(&dev, UTF_CH1_E | UTF_CH1_DIAMOND_E | UTF_CH2_E | UTF_CH3_E | UTF_XCAT_E);

    /* go over all non active devices */
    while(GT_OK == prvUtfNextDeviceGet(&dev, GT_FALSE))
    {
        st = cpssDxChPortTxTailDropWrtdMasksGet(dev, &maskLsb);
        UTF_VERIFY_EQUAL1_PARAM_MAC(GT_NOT_APPLICABLE_DEVICE, st, dev);
    }

    /* 3. Call function with out of bound value for device id.*/
    dev = PRV_CPSS_MAX_PP_DEVICES_CNS;

    st = cpssDxChPortTxTailDropWrtdMasksGet(dev, &maskLsb);
    UTF_VERIFY_EQUAL1_PARAM_MAC(GT_BAD_PARAM, st, dev);
}

/*----------------------------------------------------------------------------*/
/*
GT_STATUS cpssDxChPortTxTailDropWrtdMasksSet
(
    IN  GT_U8                       dev,
    IN  CPSS_PORT_TX_WRTD_MASK_LSB  *maskLsbPtr
)
*/
UTF_TEST_CASE_MAC(cpssDxChPortTxTailDropWrtdMasksSet)
{
/*
    ITERATE_DEVICES(Lion,xCat2)
    1.1. Call with not null maskLsbPtr.
            maskLsb.tcDp [0 / 3 / 9],
            maskLsb.port [0 / 3 / 9],
            maskLsb.tc   [0 / 3 / 9],
            maskLsb.pool [0 / 3 / 9]
    Expected: GT_OK.
    1.2. Call with not null maskLsbPtr.
            maskLsb.tcDp [10],
            maskLsb.port [11],
            maskLsb.tc   [12],
            maskLsb.pool [13]
    Expected XCAT2: GT_OK.
    Expected LION: GT_OUT_OF_RANGE.
    1.3. Call cpssDxChPortTxTailDropWrtdMasksGet
           with the same parameters.
    Expected: GT_OK and the same values than was set.
    1.4. Call api with wrong maskLsbPtr [NULL].
    Expected: GT_BAD_PTR.
*/
    GT_STATUS st = GT_OK;

    GT_U8                       dev;
    CPSS_PORT_TX_WRTD_MASK_LSB_STC  maskLsb;
    CPSS_PORT_TX_WRTD_MASK_LSB_STC  maskLsbGet;

    /* prepare device iterator */
    PRV_TGF_NOT_APPLIC_DEV_RESET_MAC(&dev, UTF_CH1_E | UTF_CH1_DIAMOND_E | UTF_CH2_E | UTF_CH3_E | UTF_XCAT_E);

    /* 1. Go over all active devices. */
    while (GT_OK == prvUtfNextDeviceGet(&dev, GT_TRUE))
    {
        /*
            1.1. Call with not null maskLsbPtr.
                    maskLsb.tcDp [0 / 3 / 9],
                    maskLsb.port [0 / 3 / 9],
                    maskLsb.tc   [0 / 3 / 9],
                    maskLsb.pool [0 / 3 / 9]
            Expected: GT_OK.
        */
        maskLsb.tcDp = 0;
        maskLsb.port = 0;
        maskLsb.tc   = 0;
        maskLsb.pool = 0;
        st = cpssDxChPortTxTailDropWrtdMasksSet(dev, &maskLsb);
        UTF_VERIFY_EQUAL1_PARAM_MAC(GT_OK, st, dev);

        /*
            1.2. Call cpssDxChPortTxTailDropWrtdMasksGet
                   with the same parameters.
            Expected: GT_OK and the same values than was set.
        */
        st = cpssDxChPortTxTailDropWrtdMasksGet(dev, &maskLsbGet);
        UTF_VERIFY_EQUAL1_STRING_MAC(GT_OK, st,
                  "cpssDxChPortTxTailDropWrtdMasksGet: %d ", dev);

        /* Verifying values */
        UTF_VERIFY_EQUAL1_STRING_MAC(maskLsb.tcDp, maskLsbGet.tcDp,
                "got another maskLsb.tcDp then was set: %d", dev);
        UTF_VERIFY_EQUAL1_STRING_MAC(maskLsb.port, maskLsbGet.port,
                "got another maskLsb.port then was set: %d", dev);

        if (PRV_CPSS_PP_MAC(prvTgfDevNum)->devFamily == CPSS_PP_FAMILY_DXCH_LION_E)
        {
            UTF_VERIFY_EQUAL1_STRING_MAC(maskLsb.tc, maskLsbGet.tc,
                "got another maskLsb.tc then was set: %d", dev);
        }

        UTF_VERIFY_EQUAL1_STRING_MAC(maskLsb.pool, maskLsbGet.pool,
                "got another maskLsb.pool then was set: %d", dev);


        maskLsb.tcDp = 3;
        maskLsb.port = 3;
        maskLsb.tc   = 3;
        maskLsb.pool = 3;
        st = cpssDxChPortTxTailDropWrtdMasksSet(dev, &maskLsb);
        UTF_VERIFY_EQUAL1_PARAM_MAC(GT_OK, st, dev);

        /*
            1.2. Call cpssDxChPortTxTailDropWrtdMasksGet
                   with the same parameters.
            Expected: GT_OK and the same values than was set.
        */
        st = cpssDxChPortTxTailDropWrtdMasksGet(dev, &maskLsbGet);
        UTF_VERIFY_EQUAL1_STRING_MAC(GT_OK, st,
                  "cpssDxChPortTxTailDropWrtdMasksGet: %d ", dev);

        /* Verifying values */
        UTF_VERIFY_EQUAL1_STRING_MAC(maskLsb.tcDp, maskLsbGet.tcDp,
                "got another maskLsb.tcDp then was set: %d", dev);
        UTF_VERIFY_EQUAL1_STRING_MAC(maskLsb.port, maskLsbGet.port,
                "got another maskLsb.port then was set: %d", dev);
        if (PRV_CPSS_PP_MAC(prvTgfDevNum)->devFamily == CPSS_PP_FAMILY_DXCH_LION_E)
        {
            UTF_VERIFY_EQUAL1_STRING_MAC(maskLsb.tc, maskLsbGet.tc,
                "got another maskLsb.tc then was set: %d", dev);
        }
        UTF_VERIFY_EQUAL1_STRING_MAC(maskLsb.pool, maskLsbGet.pool,
                "got another maskLsb.pool then was set: %d", dev);


        maskLsb.tcDp = 9;
        maskLsb.port = 9;
        maskLsb.tc   = 9;
        maskLsb.pool = 9;
        st = cpssDxChPortTxTailDropWrtdMasksSet(dev, &maskLsb);
        UTF_VERIFY_EQUAL1_PARAM_MAC(GT_OK, st, dev);

        /*
            1.2. Call cpssDxChPortTxTailDropWrtdMasksGet
                   with the same parameters.
            Expected: GT_OK and the same values than was set.
        */
        st = cpssDxChPortTxTailDropWrtdMasksGet(dev, &maskLsbGet);
        UTF_VERIFY_EQUAL1_STRING_MAC(GT_OK, st,
                  "cpssDxChPortTxTailDropWrtdMasksGet: %d ", dev);

        /* Verifying values */
        UTF_VERIFY_EQUAL1_STRING_MAC(maskLsb.tcDp, maskLsbGet.tcDp,
                "got another maskLsb.tcDp then was set: %d", dev);
        UTF_VERIFY_EQUAL1_STRING_MAC(maskLsb.port, maskLsbGet.port,
                "got another maskLsb.port then was set: %d", dev);
        if (PRV_CPSS_PP_MAC(prvTgfDevNum)->devFamily == CPSS_PP_FAMILY_DXCH_LION_E)
        {
            UTF_VERIFY_EQUAL1_STRING_MAC(maskLsb.tc, maskLsbGet.tc,
                "got another maskLsb.tc then was set: %d", dev);
        }
        UTF_VERIFY_EQUAL1_STRING_MAC(maskLsb.pool, maskLsbGet.pool,
                "got another maskLsb.pool then was set: %d", dev);


        maskLsb.tcDp = 10;
        maskLsb.port = 11;
        maskLsb.tc   = 12;
        maskLsb.pool = 13;
        st = cpssDxChPortTxTailDropWrtdMasksSet(dev, &maskLsb);
        if (PRV_CPSS_PP_MAC(prvTgfDevNum)->devFamily == CPSS_PP_FAMILY_DXCH_LION_E)
        {
            UTF_VERIFY_EQUAL1_PARAM_MAC(GT_OUT_OF_RANGE, st, dev);
        }
        else
        {
            /*
                1.2. Call cpssDxChPortTxTailDropWrtdMasksGet
                       with the same parameters.
                Expected: GT_OK and the same values than was set.
            */
            st = cpssDxChPortTxTailDropWrtdMasksGet(dev, &maskLsbGet);
            UTF_VERIFY_EQUAL1_STRING_MAC(GT_OK, st,
                      "cpssDxChPortTxTailDropWrtdMasksGet: %d ", dev);
    
            /* Verifying values */
            UTF_VERIFY_EQUAL1_STRING_MAC(maskLsb.tcDp, maskLsbGet.tcDp,
                    "got another maskLsb.tcDp then was set: %d", dev);
            UTF_VERIFY_EQUAL1_STRING_MAC(maskLsb.port, maskLsbGet.port,
                    "got another maskLsb.port then was set: %d", dev);
            UTF_VERIFY_EQUAL1_STRING_MAC(maskLsb.pool, maskLsbGet.pool,
                    "got another maskLsb.pool then was set: %d", dev);
        }


        /*
            1.3. Call api with wrong maskLsbPtr [NULL].
            Expected: GT_BAD_PTR.
        */
        st = cpssDxChPortTxTailDropWrtdMasksSet(dev, NULL);
        UTF_VERIFY_EQUAL1_STRING_MAC(GT_BAD_PTR, st, "maskLsbPtr is NULL", dev);
    }

    /* 2. For not-active devices and devices from non-applicable family */
    /* check that function returns GT_BAD_PARAM.                        */

    /* prepare device iterator */
    PRV_TGF_NOT_APPLIC_DEV_RESET_MAC(&dev, UTF_CH1_E | UTF_CH1_DIAMOND_E | UTF_CH2_E | UTF_CH3_E | UTF_XCAT_E);

    /* go over all non active devices */
    while(GT_OK == prvUtfNextDeviceGet(&dev, GT_FALSE))
    {
        st = cpssDxChPortTxTailDropWrtdMasksSet(dev, &maskLsb);
        UTF_VERIFY_EQUAL1_PARAM_MAC(GT_NOT_APPLICABLE_DEVICE, st, dev);
    }

    /* 3. Call function with out of bound value for device id.*/
    dev = PRV_CPSS_MAX_PP_DEVICES_CNS;

    st = cpssDxChPortTxTailDropWrtdMasksSet(dev, &maskLsb);
    UTF_VERIFY_EQUAL1_PARAM_MAC(GT_BAD_PARAM, st, dev);
}


/*----------------------------------------------------------------------------*/
/*
GT_STATUS cpssDxChPortTxTcProfileSharedPoolGet
(
    IN  GT_U8                              dev,
    IN  CPSS_PORT_TX_DROP_PROFILE_SET_ENT  pfSet,
    IN  GT_U8                              tc,
    OUT GT_U32                             *poolNumPtr
)
*/
UTF_TEST_CASE_MAC(cpssDxChPortTxTcProfileSharedPoolGet)
{
/*
    ITERATE_DEVICES(Lion)
    1.1. Call with pfSet[CPSS_PORT_TX_DROP_PROFILE_1_E /
                         CPSS_PORT_TX_DROP_PROFILE_2_E /
                         CPSS_PORT_TX_DROP_PROFILE_3_E /
                         CPSS_PORT_TX_DROP_PROFILE_4_E /
                         CPSS_PORT_TX_DROP_PROFILE_5_E],
                   tc[0 / 1 / 2 / 3 / 7],
    Expected: GT_OK.
    1.2. Call api with wrong pfSet [wrong enum values].
    Expected: GT_BAD_PARAM.
    1.3. Call api with wrong tc [8].
    Expected: NOT GT_OK.
    1.4. Call api with wrong poolNumPtr [NULL].
    Expected: GT_BAD_PTR.
*/
    GT_STATUS st = GT_OK;

    GT_U8                              dev;
    CPSS_PORT_TX_DROP_PROFILE_SET_ENT  pfSet = CPSS_PORT_TX_DROP_PROFILE_1_E;
    GT_U8                              tc = 0;
    GT_U32                             poolNum;

    /* prepare device iterator */
    PRV_TGF_NOT_APPLIC_DEV_RESET_MAC(&dev, UTF_CH1_E | UTF_CH1_DIAMOND_E | UTF_CH2_E | UTF_CH3_E | UTF_XCAT_E | UTF_XCAT2_E);

    /* 1. Go over all active devices. */
    while (GT_OK == prvUtfNextDeviceGet(&dev, GT_TRUE))
    {
        /*
            1.1. Call with pfSet[CPSS_PORT_TX_DROP_PROFILE_1_E /
                                 CPSS_PORT_TX_DROP_PROFILE_2_E /
                                 CPSS_PORT_TX_DROP_PROFILE_3_E /
                                 CPSS_PORT_TX_DROP_PROFILE_4_E /
                                 CPSS_PORT_TX_DROP_PROFILE_5_E],
                           tc[0 / 1 / 2 / 3 / 7],
            Expected: GT_OK.
        */
        /* call with pfSet[CPSS_PORT_TX_DROP_PROFILE_1_E], tc[0] */
        pfSet = CPSS_PORT_TX_DROP_PROFILE_1_E;
        tc = 0;

        st = cpssDxChPortTxTcProfileSharedPoolGet(dev, pfSet, tc, &poolNum);
        UTF_VERIFY_EQUAL1_PARAM_MAC(GT_OK, st, dev);

        /* call with pfSet[CPSS_PORT_TX_DROP_PROFILE_2_E], tc[1] */
        pfSet = CPSS_PORT_TX_DROP_PROFILE_2_E;
        tc = 1;

        st = cpssDxChPortTxTcProfileSharedPoolGet(dev, pfSet, tc, &poolNum);
        UTF_VERIFY_EQUAL1_PARAM_MAC(GT_OK, st, dev);

        /* call with pfSet[CPSS_PORT_TX_DROP_PROFILE_3_E], tc[2] */
        pfSet = CPSS_PORT_TX_DROP_PROFILE_3_E;
        tc = 2;

        st = cpssDxChPortTxTcProfileSharedPoolGet(dev, pfSet, tc, &poolNum);
        UTF_VERIFY_EQUAL1_PARAM_MAC(GT_OK, st, dev);

        /* call with pfSet[CPSS_PORT_TX_DROP_PROFILE_4_E], tc[3] */
        pfSet = CPSS_PORT_TX_DROP_PROFILE_4_E;
        tc = 3;

        st = cpssDxChPortTxTcProfileSharedPoolGet(dev, pfSet, tc, &poolNum);
        UTF_VERIFY_EQUAL1_PARAM_MAC(GT_OK, st, dev);

        /* call with pfSet[CPSS_PORT_TX_DROP_PROFILE_5_E], tc[7] */
        pfSet = CPSS_PORT_TX_DROP_PROFILE_5_E;
        tc = 7;

        st = cpssDxChPortTxTcProfileSharedPoolGet(dev, pfSet, tc, &poolNum);
        UTF_VERIFY_EQUAL1_PARAM_MAC(GT_OK, st, dev);

        /*
            1.2. Call api with wrong pfSet [wrong enum values].
            Expected: GT_BAD_PARAM.
        */
        UTF_ENUMS_CHECK_MAC(cpssDxChPortTxTcProfileSharedPoolGet
                            (dev, pfSet, tc, &poolNum),
                            pfSet);

        /*
            1.3. Call api with wrong tc [8].
            Expected: NOT GT_OK.
        */
        tc = 8;

        st = cpssDxChPortTxTcProfileSharedPoolGet(dev, pfSet, tc, &poolNum);
        UTF_VERIFY_NOT_EQUAL1_PARAM_MAC(GT_OK, st, dev);

        tc = 0;

        /*
            1.4. Call api with wrong poolNumPtr [NULL].
            Expected: GT_BAD_PTR.
        */
        st = cpssDxChPortTxTcProfileSharedPoolGet(dev, pfSet, tc, NULL);
        UTF_VERIFY_EQUAL1_STRING_MAC(GT_BAD_PTR, st, "poolNumPtr is NULL", dev);
    }

    /* restore correct values */
    pfSet = CPSS_PORT_TX_DROP_PROFILE_1_E;
    tc = 0;

    /* 2. For not-active devices and devices from non-applicable family */
    /* check that function returns GT_BAD_PARAM.                        */

    /* prepare device iterator */
    PRV_TGF_NOT_APPLIC_DEV_RESET_MAC(&dev, UTF_CH1_E | UTF_CH1_DIAMOND_E | UTF_CH2_E | UTF_CH3_E | UTF_XCAT_E | UTF_XCAT2_E);

    /* go over all non active devices */
    while(GT_OK == prvUtfNextDeviceGet(&dev, GT_FALSE))
    {
        st = cpssDxChPortTxTcProfileSharedPoolGet(dev, pfSet, tc, &poolNum);
        UTF_VERIFY_EQUAL1_PARAM_MAC(GT_NOT_APPLICABLE_DEVICE, st, dev);
    }

    /* 3. Call function with out of bound value for device id.*/
    dev = PRV_CPSS_MAX_PP_DEVICES_CNS;

    st = cpssDxChPortTxTcProfileSharedPoolGet(dev, pfSet, tc, &poolNum);
    UTF_VERIFY_EQUAL1_PARAM_MAC(GT_BAD_PARAM, st, dev);
}

/*----------------------------------------------------------------------------*/
/*
GT_STATUS cpssDxChPortTxTcProfileSharedPoolSet
(
    IN  GT_U8                             dev,
    IN  CPSS_PORT_TX_DROP_PROFILE_SET_ENT pfSet,
    IN  GT_U8                             tc,
    IN  GT_U32                            poolNum
)
*/
UTF_TEST_CASE_MAC(cpssDxChPortTxTcProfileSharedPoolSet)
{
/*
    ITERATE_DEVICES(Lion)
    1.1. Call with pfSet[CPSS_PORT_TX_DROP_PROFILE_1_E /
                         CPSS_PORT_TX_DROP_PROFILE_2_E /
                         CPSS_PORT_TX_DROP_PROFILE_3_E /
                         CPSS_PORT_TX_DROP_PROFILE_4_E /
                         CPSS_PORT_TX_DROP_PROFILE_5_E],
                   tc[0 / 1 / 2 / 3 / 7],
                   poolNum[0 / 1 / 2 / 3 / 7],
    Expected: GT_OK.
    1.2. Call cpssDxChPortTxTcProfileSharedPoolGet
           with the same parameters.
    Expected: GT_OK and the same values than was set.
    1.3. Call api with wrong pfSet [wrong enum values].
    Expected: GT_BAD_PARAM.
    1.4. Call api with wrong tc [8].
    Expected: NOT GT_OK.
    1.5. Call api with wrong poolNum [8].
    Expected: NOT GT_OK.
*/
    GT_STATUS st = GT_OK;

    GT_U8                             dev;
    CPSS_PORT_TX_DROP_PROFILE_SET_ENT pfSet = CPSS_PORT_TX_DROP_PROFILE_1_E;
    GT_U8                             tc = 0;
    GT_U32                            poolNum = 0;
    GT_U32                            poolNumGet = 0;

    /* prepare device iterator */
    PRV_TGF_NOT_APPLIC_DEV_RESET_MAC(&dev, UTF_CH1_E | UTF_CH1_DIAMOND_E | UTF_CH2_E | UTF_CH3_E | UTF_XCAT_E | UTF_XCAT2_E);

    /* 1. Go over all active devices. */
    while (GT_OK == prvUtfNextDeviceGet(&dev, GT_TRUE))
    {
        /*
            1.1. Call with pfSet[CPSS_PORT_TX_DROP_PROFILE_1_E /
                                 CPSS_PORT_TX_DROP_PROFILE_2_E /
                                 CPSS_PORT_TX_DROP_PROFILE_3_E /
                                 CPSS_PORT_TX_DROP_PROFILE_4_E /
                                 CPSS_PORT_TX_DROP_PROFILE_5_E],
                           tc[0 / 1 / 2 / 3 / 7],
                           poolNum[0 / 1 / 2 / 3 / 7],
            Expected: GT_OK.
        */
        /* call with pfSet[CPSS_PORT_TX_DROP_PROFILE_1_E], tc[0], poolNum[0] */
        pfSet = CPSS_PORT_TX_DROP_PROFILE_1_E;
        tc = 0;
        poolNum = 0;

        st = cpssDxChPortTxTcProfileSharedPoolSet(dev, pfSet, tc, poolNum);
        UTF_VERIFY_EQUAL1_PARAM_MAC(GT_OK, st, dev);

        /*
            1.2. Call cpssDxChPortTxTcProfileSharedPoolGet
                   with the same parameters.
            Expected: GT_OK and the same values than was set.
        */
        st = cpssDxChPortTxTcProfileSharedPoolGet(dev, pfSet, tc, &poolNumGet);
        UTF_VERIFY_EQUAL1_STRING_MAC(GT_OK, st,
                  "cpssDxChPortTxTcProfileSharedPoolGet: %d ", dev);

        /* Verifying values */
        UTF_VERIFY_EQUAL1_STRING_MAC(poolNum, poolNumGet,
                       "got another poolNum then was set: %d", dev);

        /* call with pfSet[CPSS_PORT_TX_DROP_PROFILE_2_E], tc[1], poolNum[1] */
        pfSet = CPSS_PORT_TX_DROP_PROFILE_2_E;
        tc = 1;
        poolNum = 1;

        st = cpssDxChPortTxTcProfileSharedPoolSet(dev, pfSet, tc, poolNum);
        UTF_VERIFY_EQUAL1_PARAM_MAC(GT_OK, st, dev);

        /*
            1.2. Call cpssDxChPortTxTcProfileSharedPoolGet
                   with the same parameters.
            Expected: GT_OK and the same values than was set.
        */
        st = cpssDxChPortTxTcProfileSharedPoolGet(dev, pfSet, tc, &poolNumGet);
        UTF_VERIFY_EQUAL1_STRING_MAC(GT_OK, st,
                  "cpssDxChPortTxTcProfileSharedPoolGet: %d ", dev);

        /* Verifying values */
        UTF_VERIFY_EQUAL1_STRING_MAC(poolNum, poolNumGet,
                       "got another poolNum then was set: %d", dev);

        /* call with pfSet[CPSS_PORT_TX_DROP_PROFILE_3_E], tc[2], poolNum[2] */
        pfSet = CPSS_PORT_TX_DROP_PROFILE_3_E;
        tc = 2;
        poolNum = 2;

        st = cpssDxChPortTxTcProfileSharedPoolSet(dev, pfSet, tc, poolNum);
        UTF_VERIFY_EQUAL1_PARAM_MAC(GT_OK, st, dev);

        /*
            1.2. Call cpssDxChPortTxTcProfileSharedPoolGet
                   with the same parameters.
            Expected: GT_OK and the same values than was set.
        */
        st = cpssDxChPortTxTcProfileSharedPoolGet(dev, pfSet, tc, &poolNumGet);
        UTF_VERIFY_EQUAL1_STRING_MAC(GT_OK, st,
                  "cpssDxChPortTxTcProfileSharedPoolGet: %d ", dev);

        /* Verifying values */
        UTF_VERIFY_EQUAL1_STRING_MAC(poolNum, poolNumGet,
                       "got another poolNum then was set: %d", dev);

        /* call with pfSet[CPSS_PORT_TX_DROP_PROFILE_4_E], tc[3], poolNum[3] */
        pfSet = CPSS_PORT_TX_DROP_PROFILE_4_E;
        tc = 3;
        poolNum = 3;

        st = cpssDxChPortTxTcProfileSharedPoolSet(dev, pfSet, tc, poolNum);
        UTF_VERIFY_EQUAL1_PARAM_MAC(GT_OK, st, dev);

        /*
            1.2. Call cpssDxChPortTxTcProfileSharedPoolGet
                   with the same parameters.
            Expected: GT_OK and the same values than was set.
        */
        st = cpssDxChPortTxTcProfileSharedPoolGet(dev, pfSet, tc, &poolNumGet);
        UTF_VERIFY_EQUAL1_STRING_MAC(GT_OK, st,
                  "cpssDxChPortTxTcProfileSharedPoolGet: %d ", dev);

        /* Verifying values */
        UTF_VERIFY_EQUAL1_STRING_MAC(poolNum, poolNumGet,
                       "got another poolNum then was set: %d", dev);

        /* call with pfSet[CPSS_PORT_TX_DROP_PROFILE_5_E], tc[7], poolNum[7] */
        pfSet = CPSS_PORT_TX_DROP_PROFILE_5_E;
        tc = 7;
        poolNum = 7;

        st = cpssDxChPortTxTcProfileSharedPoolSet(dev, pfSet, tc, poolNum);
        UTF_VERIFY_EQUAL1_PARAM_MAC(GT_OK, st, dev);

        /*
            1.2. Call cpssDxChPortTxTcProfileSharedPoolGet
                   with the same parameters.
            Expected: GT_OK and the same values than was set.
        */
        st = cpssDxChPortTxTcProfileSharedPoolGet(dev, pfSet, tc, &poolNumGet);
        UTF_VERIFY_EQUAL1_STRING_MAC(GT_OK, st,
                  "cpssDxChPortTxTcProfileSharedPoolGet: %d ", dev);

        /* Verifying values */
        UTF_VERIFY_EQUAL1_STRING_MAC(poolNum, poolNumGet,
                       "got another poolNum then was set: %d", dev);

        /*
            1.3. Call api with wrong pfSet [wrong enum values].
            Expected: GT_BAD_PARAM.
        */
        UTF_ENUMS_CHECK_MAC(cpssDxChPortTxTcProfileSharedPoolSet
                            (dev, pfSet, tc, poolNum),
                            pfSet);

        /*
            1.4. Call api with wrong tc [8].
            Expected: NOT GT_OK.
        */
        tc = 8;

        st = cpssDxChPortTxTcProfileSharedPoolSet(dev, pfSet, tc, poolNum);
        UTF_VERIFY_NOT_EQUAL1_PARAM_MAC(GT_OK, st, dev);

        tc = 0;

        /*
            1.5. Call api with wrong poolNum [8].
            Expected: NOT GT_OK.
        */
        poolNum = 8;

        st = cpssDxChPortTxTcProfileSharedPoolSet(dev, pfSet, tc, poolNum);
        UTF_VERIFY_NOT_EQUAL1_PARAM_MAC(GT_OK, st, dev);

        poolNum = 0;
    }

    /* restore correct values */
    pfSet = CPSS_PORT_TX_DROP_PROFILE_1_E;
    tc = 0;
    poolNum = 0;

    /* 2. For not-active devices and devices from non-applicable family */
    /* check that function returns GT_BAD_PARAM.                        */

    /* prepare device iterator */
    PRV_TGF_NOT_APPLIC_DEV_RESET_MAC(&dev, UTF_CH1_E | UTF_CH1_DIAMOND_E | UTF_CH2_E | UTF_CH3_E | UTF_XCAT_E | UTF_XCAT2_E);

    /* go over all non active devices */
    while(GT_OK == prvUtfNextDeviceGet(&dev, GT_FALSE))
    {
        st = cpssDxChPortTxTcProfileSharedPoolSet(dev, pfSet, tc, poolNum);
        UTF_VERIFY_EQUAL1_PARAM_MAC(GT_NOT_APPLICABLE_DEVICE, st, dev);
    }

    /* 3. Call function with out of bound value for device id.*/
    dev = PRV_CPSS_MAX_PP_DEVICES_CNS;

    st = cpssDxChPortTxTcProfileSharedPoolSet(dev, pfSet, tc, poolNum);
    UTF_VERIFY_EQUAL1_PARAM_MAC(GT_BAD_PARAM, st, dev);
}

/*----------------------------------------------------------------------------*/
/*
GT_STATUS cpssDxChPortTxByteCountChangeEnableGet
(
    IN  GT_U8                                   dev,
    OUT CPSS_DXCH_PORT_TX_BC_CHANGE_ENABLE_ENT  *bcChangeEnablePtr
)
*/
UTF_TEST_CASE_MAC(cpssDxChPortTxByteCountChangeEnableGet)
{
/*
    ITERATE_DEVICES(DxChXcat)
    1.1. Call with not null bcChangeEnablePtr.
    Expected: GT_OK.
    1.2. Call api with wrong bcChangeEnablePtr [NULL].
    Expected: GT_BAD_PTR.
*/
    GT_STATUS st = GT_OK;

    GT_U8                                   dev;
    CPSS_DXCH_PORT_TX_BC_CHANGE_ENABLE_ENT  bcChangeEnable;

    /* prepare device iterator */
    PRV_TGF_NOT_APPLIC_DEV_RESET_MAC(&dev, UTF_CH1_E | UTF_CH1_DIAMOND_E | UTF_CH2_E | UTF_CH3_E | UTF_LION_E);
    
    /* 1. Go over all active devices. */
    while (GT_OK == prvUtfNextDeviceGet(&dev, GT_TRUE))
    {
        /*
            1.1. Call with not null bcChangeEnablePtr.
            Expected: GT_OK.
        */
        st = cpssDxChPortTxByteCountChangeEnableGet(dev, &bcChangeEnable);
        if ((PRV_CPSS_PP_MAC(prvTgfDevNum)->devFamily == CPSS_PP_FAMILY_DXCH_XCAT_E) && 
            (GT_TRUE != PRV_CPSS_DXCH_XCAT_A3_ONLY_CHECK_MAC(prvTgfDevNum)))
            UTF_VERIFY_EQUAL1_PARAM_MAC(GT_NOT_APPLICABLE_DEVICE, st, dev);
        else
            UTF_VERIFY_EQUAL1_PARAM_MAC(GT_OK, st, dev);

        /*
            1.2. Call api with wrong bcChangeEnablePtr [NULL].
            Expected: GT_BAD_PTR.
        */
        st = cpssDxChPortTxByteCountChangeEnableGet(dev, NULL);
        if ((PRV_CPSS_PP_MAC(prvTgfDevNum)->devFamily == CPSS_PP_FAMILY_DXCH_XCAT_E) && 
            (GT_TRUE != PRV_CPSS_DXCH_XCAT_A3_ONLY_CHECK_MAC(prvTgfDevNum)))
            UTF_VERIFY_EQUAL1_PARAM_MAC(GT_NOT_APPLICABLE_DEVICE, st, dev);
        else
            UTF_VERIFY_EQUAL1_STRING_MAC(GT_BAD_PTR, st, 
                                       "%d, bcChangeEnablePtr = NULL", dev);
    }
     
    /* 2. For not-active devices and devices from non-applicable family */
    /* check that function returns GT_BAD_PARAM.                        */

    /* prepare device iterator */
    PRV_TGF_NOT_APPLIC_DEV_RESET_MAC(&dev, UTF_CH1_E | UTF_CH1_DIAMOND_E | UTF_CH2_E | UTF_CH3_E | UTF_LION_E);

    /* go over all non active devices */
    while(GT_OK == prvUtfNextDeviceGet(&dev, GT_FALSE))
    {
        st = cpssDxChPortTxByteCountChangeEnableGet(dev, &bcChangeEnable);
        UTF_VERIFY_EQUAL1_PARAM_MAC(GT_NOT_APPLICABLE_DEVICE, st, dev);
    }
     
    /* 3. Call function with out of bound value for device id.*/
    dev = PRV_CPSS_MAX_PP_DEVICES_CNS;

    st = cpssDxChPortTxByteCountChangeEnableGet(dev, &bcChangeEnable);
    UTF_VERIFY_EQUAL1_PARAM_MAC(GT_BAD_PARAM, st, dev);
}

/*----------------------------------------------------------------------------*/
/*
GT_STATUS cpssDxChPortTxByteCountChangeEnableSet
(
    IN  GT_U8                                  dev,
    IN  CPSS_DXCH_PORT_TX_BC_CHANGE_ENABLE_ENT bcChangeEnable
)
*/
UTF_TEST_CASE_MAC(cpssDxChPortTxByteCountChangeEnableSet)
{
/*
    ITERATE_DEVICES(DxChXcat)
    1.1. Call with bcChangeEnable[CPSS_DXCH_PORT_TX_BC_CHANGE_DISABLE_ALL_E /
                                  CPSS_DXCH_PORT_TX_BC_CHANGE_ENABLE_SHAPER_ONLY_E /
                                  CPSS_DXCH_PORT_TX_BC_CHANGE_ENABLE_SCHEDULER_ONLY_E /
                                  CPSS_DXCH_PORT_TX_BC_CHANGE_ENABLE_SHAPER_AND_SCHEDULER_E],
    Expected: GT_OK.
    1.2. Call cpssDxChPortTxByteCountChangeEnableGet
           with the same parameters.
    Expected: GT_OK and the same values than was set.
    1.3. Call api with wrong bcChangeEnable [wrong enum values].
    Expected: GT_BAD_PARAM.
*/
    GT_STATUS st = GT_OK;

    GT_U8                                  dev;
    CPSS_DXCH_PORT_TX_BC_CHANGE_ENABLE_ENT bcChangeEnable = CPSS_DXCH_PORT_TX_BC_CHANGE_DISABLE_ALL_E;
    CPSS_DXCH_PORT_TX_BC_CHANGE_ENABLE_ENT bcChangeEnableGet = CPSS_DXCH_PORT_TX_BC_CHANGE_DISABLE_ALL_E;

    /* prepare device iterator */
    PRV_TGF_NOT_APPLIC_DEV_RESET_MAC(&dev, UTF_CH1_E | UTF_CH1_DIAMOND_E | UTF_CH2_E | UTF_CH3_E | UTF_LION_E);
    
    /* 1. Go over all active devices. */
    while (GT_OK == prvUtfNextDeviceGet(&dev, GT_TRUE))
    {
        /*
            1.1. Call with bcChangeEnable[CPSS_DXCH_PORT_TX_BC_CHANGE_DISABLE_ALL_E /
                                          CPSS_DXCH_PORT_TX_BC_CHANGE_ENABLE_SHAPER_ONLY_E /
                                          CPSS_DXCH_PORT_TX_BC_CHANGE_ENABLE_SCHEDULER_ONLY_E /
                                          CPSS_DXCH_PORT_TX_BC_CHANGE_ENABLE_SHAPER_AND_SCHEDULER_E],
            Expected: GT_OK.
        */

        /* call with bcChangeEnable[CPSS_DXCH_PORT_TX_BC_CHANGE_DISABLE_ALL_E] */
        bcChangeEnable = CPSS_DXCH_PORT_TX_BC_CHANGE_DISABLE_ALL_E;

        st = cpssDxChPortTxByteCountChangeEnableSet(dev, bcChangeEnable);
        if ((PRV_CPSS_PP_MAC(prvTgfDevNum)->devFamily == CPSS_PP_FAMILY_DXCH_XCAT_E) && 
            (GT_TRUE != PRV_CPSS_DXCH_XCAT_A3_ONLY_CHECK_MAC(prvTgfDevNum)))
            UTF_VERIFY_EQUAL1_PARAM_MAC(GT_NOT_APPLICABLE_DEVICE, st, dev);
        else
            UTF_VERIFY_EQUAL1_PARAM_MAC(GT_OK, st, dev);

        /*
            1.2. Call cpssDxChPortTxByteCountChangeEnableGet
                   with the same parameters.
            Expected: GT_OK and the same values than was set.
        */
        st = cpssDxChPortTxByteCountChangeEnableGet(dev, &bcChangeEnableGet);
        if ((PRV_CPSS_PP_MAC(prvTgfDevNum)->devFamily == CPSS_PP_FAMILY_DXCH_XCAT_E) && 
            (GT_TRUE != PRV_CPSS_DXCH_XCAT_A3_ONLY_CHECK_MAC(prvTgfDevNum)))
            UTF_VERIFY_EQUAL1_PARAM_MAC(GT_NOT_APPLICABLE_DEVICE, st, dev);
        else
            UTF_VERIFY_EQUAL1_STRING_MAC(GT_OK, st,
                      "cpssDxChPortTxByteCountChangeEnableGet: %d ", dev);

        if (GT_OK == st)
        {
            /* Verifying values */
            UTF_VERIFY_EQUAL1_STRING_MAC(bcChangeEnable, bcChangeEnableGet, 
                           "got another bcChangeEnable then was set: %d", dev);
        }

        /* call with bcChangeEnable[CPSS_DXCH_PORT_TX_BC_CHANGE_ENABLE_SHAPER_ONLY_E] */
        bcChangeEnable = CPSS_DXCH_PORT_TX_BC_CHANGE_ENABLE_SHAPER_ONLY_E;

        st = cpssDxChPortTxByteCountChangeEnableSet(dev, bcChangeEnable);
        if ((PRV_CPSS_PP_MAC(prvTgfDevNum)->devFamily == CPSS_PP_FAMILY_DXCH_XCAT_E) && 
            (GT_TRUE != PRV_CPSS_DXCH_XCAT_A3_ONLY_CHECK_MAC(prvTgfDevNum)))
            UTF_VERIFY_EQUAL1_PARAM_MAC(GT_NOT_APPLICABLE_DEVICE, st, dev);
        else
            UTF_VERIFY_EQUAL1_PARAM_MAC(GT_OK, st, dev);

        /* call get func again, 1.2. */
        st = cpssDxChPortTxByteCountChangeEnableGet(dev, &bcChangeEnableGet);
        if ((PRV_CPSS_PP_MAC(prvTgfDevNum)->devFamily == CPSS_PP_FAMILY_DXCH_XCAT_E) && 
            (GT_TRUE != PRV_CPSS_DXCH_XCAT_A3_ONLY_CHECK_MAC(prvTgfDevNum)))
            UTF_VERIFY_EQUAL1_PARAM_MAC(GT_NOT_APPLICABLE_DEVICE, st, dev);
        else
            UTF_VERIFY_EQUAL1_STRING_MAC(GT_OK, st,
                      "cpssDxChPortTxByteCountChangeEnableGet: %d ", dev);

        if (GT_OK == st)
        {
            /* Verifying values */
            UTF_VERIFY_EQUAL1_STRING_MAC(bcChangeEnable, bcChangeEnableGet, 
                           "got another bcChangeEnable then was set: %d", dev);
        }

        /* call with bcChangeEnable[CPSS_DXCH_PORT_TX_BC_CHANGE_ENABLE_SCHEDULER_ONLY_E] */
        bcChangeEnable = CPSS_DXCH_PORT_TX_BC_CHANGE_ENABLE_SCHEDULER_ONLY_E;

        st = cpssDxChPortTxByteCountChangeEnableSet(dev, bcChangeEnable);
        if ((PRV_CPSS_PP_MAC(prvTgfDevNum)->devFamily == CPSS_PP_FAMILY_DXCH_XCAT_E) && 
            (GT_TRUE != PRV_CPSS_DXCH_XCAT_A3_ONLY_CHECK_MAC(prvTgfDevNum)))
            UTF_VERIFY_EQUAL1_PARAM_MAC(GT_NOT_APPLICABLE_DEVICE, st, dev);
        else
            UTF_VERIFY_EQUAL1_PARAM_MAC(GT_OK, st, dev);

        /* call get func again, 1.2. */
        st = cpssDxChPortTxByteCountChangeEnableGet(dev, &bcChangeEnableGet);
        if ((PRV_CPSS_PP_MAC(prvTgfDevNum)->devFamily == CPSS_PP_FAMILY_DXCH_XCAT_E) && 
            (GT_TRUE != PRV_CPSS_DXCH_XCAT_A3_ONLY_CHECK_MAC(prvTgfDevNum)))
            UTF_VERIFY_EQUAL1_PARAM_MAC(GT_NOT_APPLICABLE_DEVICE, st, dev);
        else
            UTF_VERIFY_EQUAL1_STRING_MAC(GT_OK, st,
                      "cpssDxChPortTxByteCountChangeEnableGet: %d ", dev);

        if (GT_OK == st)
        {
            /* Verifying values */
            UTF_VERIFY_EQUAL1_STRING_MAC(bcChangeEnable, bcChangeEnableGet, 
                           "got another bcChangeEnable then was set: %d", dev);
        }

        /* call with bcChangeEnable[CPSS_DXCH_PORT_TX_BC_CHANGE_ENABLE_SHAPER_AND_SCHEDULER_E] */
        bcChangeEnable = CPSS_DXCH_PORT_TX_BC_CHANGE_ENABLE_SHAPER_AND_SCHEDULER_E;

        st = cpssDxChPortTxByteCountChangeEnableSet(dev, bcChangeEnable);
        if ((PRV_CPSS_PP_MAC(prvTgfDevNum)->devFamily == CPSS_PP_FAMILY_DXCH_XCAT_E) && 
            (GT_TRUE != PRV_CPSS_DXCH_XCAT_A3_ONLY_CHECK_MAC(prvTgfDevNum)))
            UTF_VERIFY_EQUAL1_PARAM_MAC(GT_NOT_APPLICABLE_DEVICE, st, dev);
        else
            UTF_VERIFY_EQUAL1_PARAM_MAC(GT_OK, st, dev);

        /* call get func again, 1.2. */
        st = cpssDxChPortTxByteCountChangeEnableGet(dev, &bcChangeEnableGet);
        if ((PRV_CPSS_PP_MAC(prvTgfDevNum)->devFamily == CPSS_PP_FAMILY_DXCH_XCAT_E) && 
            (GT_TRUE != PRV_CPSS_DXCH_XCAT_A3_ONLY_CHECK_MAC(prvTgfDevNum)))
            UTF_VERIFY_EQUAL1_PARAM_MAC(GT_NOT_APPLICABLE_DEVICE, st, dev);
        else
            UTF_VERIFY_EQUAL1_STRING_MAC(GT_OK, st,
                      "cpssDxChPortTxByteCountChangeEnableGet: %d ", dev);

        if (GT_OK == st)
        {
            /* Verifying values */
            UTF_VERIFY_EQUAL1_STRING_MAC(bcChangeEnable, bcChangeEnableGet, 
                           "got another bcChangeEnable then was set: %d", dev);
        }

        /*
            1.3. Call api with wrong bcChangeEnable [wrong enum values].
            Expected: GT_BAD_PARAM.
        */
        if ((PRV_CPSS_PP_MAC(prvTgfDevNum)->devFamily != CPSS_PP_FAMILY_DXCH_XCAT_E) || 
            (GT_TRUE == PRV_CPSS_DXCH_XCAT_A3_ONLY_CHECK_MAC(prvTgfDevNum)))
            UTF_ENUMS_CHECK_MAC(cpssDxChPortTxByteCountChangeEnableSet
                                (dev, bcChangeEnable),
                                bcChangeEnable);
    }

    /* restore correct values */
    bcChangeEnable = CPSS_DXCH_PORT_TX_BC_CHANGE_DISABLE_ALL_E;
     
    /* 2. For not-active devices and devices from non-applicable family */
    /* check that function returns GT_BAD_PARAM.                        */

    /* prepare device iterator */
    PRV_TGF_NOT_APPLIC_DEV_RESET_MAC(&dev, UTF_CH1_E | UTF_CH1_DIAMOND_E | UTF_CH2_E | UTF_CH3_E | UTF_LION_E);

    /* go over all non active devices */
    while(GT_OK == prvUtfNextDeviceGet(&dev, GT_FALSE))
    {
        st = cpssDxChPortTxByteCountChangeEnableSet(dev, bcChangeEnable);
        UTF_VERIFY_EQUAL1_PARAM_MAC(GT_NOT_APPLICABLE_DEVICE, st, dev);
    }
     
    /* 3. Call function with out of bound value for device id.*/
    dev = PRV_CPSS_MAX_PP_DEVICES_CNS;

    st = cpssDxChPortTxByteCountChangeEnableSet(dev, bcChangeEnable);
    UTF_VERIFY_EQUAL1_PARAM_MAC(GT_BAD_PARAM, st, dev);
}

/*----------------------------------------------------------------------------*/
/*
GT_STATUS cpssDxChPortTxByteCountChangeValueSet
(
    IN GT_U8    devNum,
    IN GT_U8    portNum,
    IN GT_32    bcValue
)
*/
UTF_TEST_CASE_MAC(cpssDxChPortTxByteCountChangeValueSet)
{
/*
    ITERATE_DEVICES_PHY_PORTS (xCat)
    1.1.1. Call with bcValue [-63...63].
    Expected: GT_OK.
    1.1.2. Call cpssDxChPortTxByteCountChangeValueGet with non-NULL pointers
                                   other params same as in 1.1.1.
    Expected: GT_OK and the same values.
    1.1.3. Call with out of range bcValue [-64 / 64],
                     other params same as in 1.1.1.
    Expected: NOT GT_OK.
*/
    GT_STATUS st   = GT_OK;
    GT_U8     dev;
    GT_U8     port = PORT_TX_VALID_PHY_PORT_CNS;

    GT_32     bcValue    = 0;
    GT_32     bcValueGet = 0;

    /* prepare iterator for go over all active devices */
    PRV_TGF_NOT_APPLIC_DEV_RESET_MAC(&dev, UTF_CH1_E | UTF_CH1_DIAMOND_E | UTF_CH2_E | UTF_CH3_E | UTF_LION_E);

    /* 1. Go over all active devices. */
    while (GT_OK == prvUtfNextDeviceGet(&dev, GT_TRUE))
    {
        st = prvUtfNextPhyPortReset(&port, dev);
        UTF_VERIFY_EQUAL0_PARAM_MAC(GT_OK, st);

        /* 1.1. For all active devices go over all available physical ports. */
        while (GT_OK == prvUtfNextPhyPortGet(&port, GT_TRUE))
        {
            /*
                1.1.1. Call with bcValue [-63...63].
                Expected: GT_OK.
            */
            for (bcValue = -63; bcValue < 64; bcValue++)
            {
                st = cpssDxChPortTxByteCountChangeValueSet(dev, port, bcValue);
                if ((PRV_CPSS_PP_MAC(prvTgfDevNum)->devFamily == CPSS_PP_FAMILY_DXCH_XCAT_E) && 
                    (GT_TRUE != PRV_CPSS_DXCH_XCAT_A3_ONLY_CHECK_MAC(prvTgfDevNum)))
                    UTF_VERIFY_EQUAL1_PARAM_MAC(GT_NOT_APPLICABLE_DEVICE, st, dev);
                else
                    UTF_VERIFY_EQUAL3_PARAM_MAC(GT_OK, st, dev, port, bcValue);
        
                if (GT_OK == st)
                {
                    /*
                        1.1.2. Call cpssDxChPortTxByteCountChangeValueGet with non-NULL pointers
                                                                  other params same as in 1.1.1.
                        Expected: GT_OK and the same values.
                    */
                    st = cpssDxChPortTxByteCountChangeValueGet(dev, port, &bcValueGet);
                    UTF_VERIFY_EQUAL1_STRING_MAC(GT_OK, st,
                               "cpssDxChPortTxByteCountChangeValueGet: %d", dev);
            
                    /* verifying values */
                    UTF_VERIFY_EQUAL1_STRING_MAC(bcValue, bcValueGet,
                               "got another bcValue then was set: %d", dev);
                }
            }
    
            /*
                1.1.3. Call with out of range bcValue [-64 / 64],
                                 other params same as in 1.1.1.
                Expected: NOT GT_OK.
            */
            /*call with bcValue = 64;*/
            bcValue = 64;
            
            st = cpssDxChPortTxByteCountChangeValueSet(dev, port, bcValue);
            if ((PRV_CPSS_PP_MAC(prvTgfDevNum)->devFamily == CPSS_PP_FAMILY_DXCH_XCAT_E) && 
                (GT_TRUE != PRV_CPSS_DXCH_XCAT_A3_ONLY_CHECK_MAC(prvTgfDevNum)))
                UTF_VERIFY_EQUAL1_PARAM_MAC(GT_NOT_APPLICABLE_DEVICE, st, dev);
            else
                UTF_VERIFY_NOT_EQUAL2_PARAM_MAC(GT_OK, st, dev, bcValue);

            /*call with bcValue = -64;*/
            bcValue = -64;
            
            st = cpssDxChPortTxByteCountChangeValueSet(dev, port, bcValue);
            if ((PRV_CPSS_PP_MAC(prvTgfDevNum)->devFamily == CPSS_PP_FAMILY_DXCH_XCAT_E) && 
                (GT_TRUE != PRV_CPSS_DXCH_XCAT_A3_ONLY_CHECK_MAC(prvTgfDevNum)))
                UTF_VERIFY_EQUAL1_PARAM_MAC(GT_NOT_APPLICABLE_DEVICE, st, dev);
            else
                UTF_VERIFY_NOT_EQUAL2_PARAM_MAC(GT_OK, st, dev, bcValue);
        }

        bcValue = 0;

        st = prvUtfNextPhyPortReset(&port, dev);
        UTF_VERIFY_EQUAL0_PARAM_MAC(GT_OK, st);

        /* 1.2. For all active devices go over all non available physical ports. */
        while (GT_OK == prvUtfNextPhyPortGet(&port, GT_FALSE))
        {
            /* 1.2.1. Call function for each non-active port */
            st = cpssDxChPortTxByteCountChangeValueSet(dev, port, bcValue);
            UTF_VERIFY_EQUAL2_PARAM_MAC(GT_BAD_PARAM, st, dev, port);
        }

        /* 1.3. For active device check that function returns GT_BAD_PARAM */
        /* for out of bound value for port number.                         */
        port = PRV_CPSS_MAX_PP_PORTS_NUM_CNS;

        st = cpssDxChPortTxByteCountChangeValueSet(dev, port, bcValue);
        if ((PRV_CPSS_PP_MAC(prvTgfDevNum)->devFamily == CPSS_PP_FAMILY_DXCH_XCAT_E) && 
            (GT_TRUE != PRV_CPSS_DXCH_XCAT_A3_ONLY_CHECK_MAC(prvTgfDevNum)))
            UTF_VERIFY_EQUAL1_PARAM_MAC(GT_NOT_APPLICABLE_DEVICE, st, dev);
        else
            UTF_VERIFY_EQUAL2_PARAM_MAC(GT_BAD_PARAM, st, dev, port);

        /* 1.4. For active device check that function returns GT_OK */
        /* for CPU port number.                                     */
        port = CPSS_CPU_PORT_NUM_CNS;

        st = cpssDxChPortTxByteCountChangeValueSet(dev, port, bcValue);
        if ((PRV_CPSS_PP_MAC(prvTgfDevNum)->devFamily == CPSS_PP_FAMILY_DXCH_XCAT_E) && 
            (GT_TRUE != PRV_CPSS_DXCH_XCAT_A3_ONLY_CHECK_MAC(prvTgfDevNum)))
            UTF_VERIFY_EQUAL1_PARAM_MAC(GT_NOT_APPLICABLE_DEVICE, st, dev);
        else
            UTF_VERIFY_EQUAL2_PARAM_MAC(GT_OK, st, dev, port);
    }

    bcValue = 0;

    /* 2. For not active devices check that function returns non GT_OK.*/
    port = PORT_TX_VALID_PHY_PORT_CNS;

    /* prepare device iterator */
    PRV_TGF_NOT_APPLIC_DEV_RESET_MAC(&dev, UTF_CH1_E | UTF_CH1_DIAMOND_E | UTF_CH2_E | UTF_CH3_E | UTF_LION_E);

    /* Go over all non active devices. */
    while(GT_OK == prvUtfNextDeviceGet(&dev, GT_FALSE))
    {
        st = cpssDxChPortTxByteCountChangeValueSet(dev, port, bcValue);
        UTF_VERIFY_EQUAL1_PARAM_MAC(GT_NOT_APPLICABLE_DEVICE, st, dev);
    }

    /* 3.Call function with out of bound value for device id */
    dev = PRV_CPSS_MAX_PP_DEVICES_CNS;

    st = cpssDxChPortTxByteCountChangeValueSet(dev, port, bcValue);
    UTF_VERIFY_EQUAL1_PARAM_MAC(GT_BAD_PARAM, st, dev);
}

/*----------------------------------------------------------------------------*/
/*
GT_STATUS cpssDxChPortTxByteCountChangeValueGet
(
    IN  GT_U8    devNum,
    IN  GT_U8    portNum,
    OUT GT_32    *bcValuePtr
)
*/
UTF_TEST_CASE_MAC(cpssDxChPortTxByteCountChangeValueGet)
{
/*
    ITERATE_DEVICES_PHY_PORTS (xCat)
    1.1.1. Call with non NULL bcValuePtr.
    Expected: GT_OK.
    1.1.2. Call with bcValuePtr [NULL], other params same as in 1.1.
    Expected: GT_BAD_PTR.
*/
    GT_STATUS   st   = GT_OK;
    GT_U8       dev;
    GT_U8       port = PORT_TX_VALID_PHY_PORT_CNS;
    GT_32       bcValue = 0;
    
    /* prepare iterator for go over all active devices */
    PRV_TGF_NOT_APPLIC_DEV_RESET_MAC(&dev, UTF_CH1_E | UTF_CH1_DIAMOND_E | UTF_CH2_E | UTF_CH3_E | UTF_LION_E);

    /* 1. Go over all active devices. */
    while (GT_OK == prvUtfNextDeviceGet(&dev, GT_TRUE))
    {
        st = prvUtfNextPhyPortReset(&port, dev);
        UTF_VERIFY_EQUAL0_PARAM_MAC(GT_OK, st);

        /* 1.1. For all active devices go over all available physical ports. */
        while (GT_OK == prvUtfNextPhyPortGet(&port, GT_TRUE))
        {
            /*
                1.1.1. Call with non NULL bcValuePtr.
                Expected: GT_OK.
            */
            st = cpssDxChPortTxByteCountChangeValueGet(dev, port, &bcValue);
            if ((PRV_CPSS_PP_MAC(prvTgfDevNum)->devFamily == CPSS_PP_FAMILY_DXCH_XCAT_E) && 
                (GT_TRUE != PRV_CPSS_DXCH_XCAT_A3_ONLY_CHECK_MAC(prvTgfDevNum)))
                UTF_VERIFY_EQUAL1_PARAM_MAC(GT_NOT_APPLICABLE_DEVICE, st, dev);
            else
                UTF_VERIFY_EQUAL2_PARAM_MAC(GT_OK, st, dev, port);

            /*
                1.1.2. Call with bcValuePtr [NULL], other params same as in 1.1.
                Expected: GT_BAD_PTR.
            */
            st = cpssDxChPortTxByteCountChangeValueGet(dev, port, NULL);
            if ((PRV_CPSS_PP_MAC(prvTgfDevNum)->devFamily == CPSS_PP_FAMILY_DXCH_XCAT_E) && 
                (GT_TRUE != PRV_CPSS_DXCH_XCAT_A3_ONLY_CHECK_MAC(prvTgfDevNum)))
                UTF_VERIFY_EQUAL1_PARAM_MAC(GT_NOT_APPLICABLE_DEVICE, st, dev);
            else
                UTF_VERIFY_EQUAL2_PARAM_MAC(GT_BAD_PTR, st, "%d, bcValuePtr = NULL", dev);
        }

        st = prvUtfNextPhyPortReset(&port, dev);
        UTF_VERIFY_EQUAL0_PARAM_MAC(GT_OK, st);

        /* 1.2. For all active devices go over all non available physical ports. */
        while (GT_OK == prvUtfNextPhyPortGet(&port, GT_FALSE))
        {
            /* 1.2.1. Call function for each non-active port */
            st = cpssDxChPortTxByteCountChangeValueGet(dev, port, &bcValue);
            UTF_VERIFY_EQUAL2_PARAM_MAC(GT_BAD_PARAM, st, dev, port);
        }

        /* 1.3. For active device check that function returns GT_BAD_PARAM */
        /* for out of bound value for port number.                         */
        port = PRV_CPSS_MAX_PP_PORTS_NUM_CNS;

        st = cpssDxChPortTxByteCountChangeValueGet(dev, port, &bcValue);
        if ((PRV_CPSS_PP_MAC(prvTgfDevNum)->devFamily == CPSS_PP_FAMILY_DXCH_XCAT_E) && 
            (GT_TRUE != PRV_CPSS_DXCH_XCAT_A3_ONLY_CHECK_MAC(prvTgfDevNum)))
            UTF_VERIFY_EQUAL1_PARAM_MAC(GT_NOT_APPLICABLE_DEVICE, st, dev);
        else
            UTF_VERIFY_EQUAL2_PARAM_MAC(GT_BAD_PARAM, st, dev, port);

        /* 1.4. For active device check that function returns GT_OK */
        /* for CPU port number.                                     */
        port = CPSS_CPU_PORT_NUM_CNS;

        st = cpssDxChPortTxByteCountChangeValueGet(dev, port, &bcValue);
        if ((PRV_CPSS_PP_MAC(prvTgfDevNum)->devFamily == CPSS_PP_FAMILY_DXCH_XCAT_E) && 
            (GT_TRUE != PRV_CPSS_DXCH_XCAT_A3_ONLY_CHECK_MAC(prvTgfDevNum)))
            UTF_VERIFY_EQUAL1_PARAM_MAC(GT_NOT_APPLICABLE_DEVICE, st, dev);
        else
            UTF_VERIFY_EQUAL2_PARAM_MAC(GT_OK, st, dev, port);
    }

    /* 2. For not active devices check that function returns non GT_OK.*/
    port = PORT_TX_VALID_PHY_PORT_CNS;

    /* prepare device iterator */
    PRV_TGF_NOT_APPLIC_DEV_RESET_MAC(&dev, UTF_CH1_E | UTF_CH1_DIAMOND_E | UTF_CH2_E | UTF_CH3_E | UTF_LION_E);

    /* Go over all non active devices. */
    while(GT_OK == prvUtfNextDeviceGet(&dev, GT_FALSE))
    {
        st = cpssDxChPortTxByteCountChangeValueGet(dev, port, &bcValue);
        UTF_VERIFY_EQUAL1_PARAM_MAC(GT_NOT_APPLICABLE_DEVICE, st, dev);
    }

    /* 3.Call function with out of bound value for device id */
    dev = PRV_CPSS_MAX_PP_DEVICES_CNS;

    st = cpssDxChPortTxByteCountChangeValueGet(dev, port, &bcValue);
    UTF_VERIFY_EQUAL1_PARAM_MAC(GT_BAD_PARAM, st, dev);
}

/*----------------------------------------------------------------------------*/
/*
GT_STATUS cpssDxChPortTxQueueEnableGet
(
    IN  GT_U8    dev,
    OUT GT_BOOL  *enablePtr
)
*/
UTF_TEST_CASE_MAC(cpssDxChPortTxQueueEnableGet)
{
/*
    ITERATE_DEVICES(All DxCh Devices)
    1.1. Call with not null enablePtr.
    Expected: GT_OK.
    1.2. Call api with wrong enablePtr [NULL].
    Expected: GT_BAD_PTR.
*/
    GT_STATUS st = GT_OK;

    GT_U8    dev;
    GT_BOOL  enable;

    /* prepare device iterator */
    PRV_TGF_NOT_APPLIC_DEV_RESET_MAC(&dev, UTF_NONE_FAMILY_E);
    
    /* 1. Go over all active devices. */
    while (GT_OK == prvUtfNextDeviceGet(&dev, GT_TRUE))
    {
        /*
            1.1. Call with not null enablePtr.
            Expected: GT_OK.
        */
        st = cpssDxChPortTxQueueEnableGet(dev, &enable);
        UTF_VERIFY_EQUAL1_PARAM_MAC(GT_OK, st, dev);

        /*
            1.2. Call api with wrong enablePtr [NULL].
            Expected: GT_BAD_PTR.
        */
        st = cpssDxChPortTxQueueEnableGet(dev, NULL);
        UTF_VERIFY_EQUAL1_STRING_MAC(GT_BAD_PTR, st, 
                                   "%d, enablePtr = NULL", dev);
    }
     
    /* 2. For not-active devices and devices from non-applicable family */
    /* check that function returns GT_BAD_PARAM.                        */

    /* prepare device iterator */
    PRV_TGF_NOT_APPLIC_DEV_RESET_MAC(&dev, UTF_NONE_FAMILY_E);

    /* go over all non active devices */
    while(GT_OK == prvUtfNextDeviceGet(&dev, GT_FALSE))
    {
        st = cpssDxChPortTxQueueEnableGet(dev, &enable);
        UTF_VERIFY_EQUAL1_PARAM_MAC(GT_NOT_APPLICABLE_DEVICE, st, dev);
    }
     
    /* 3. Call function with out of bound value for device id.*/
    dev = PRV_CPSS_MAX_PP_DEVICES_CNS;

    st = cpssDxChPortTxQueueEnableGet(dev, &enable);
    UTF_VERIFY_EQUAL1_PARAM_MAC(GT_BAD_PARAM, st, dev);
}

/*----------------------------------------------------------------------------*/
/*
GT_STATUS cpssDxChPortTxQueueTxEnableGet
(
    IN  GT_U8     devNum,
    IN  GT_U8     portNum,
    IN  GT_U8     tcQueue,
    OUT GT_BOOL   *enablePtr
)
*/
UTF_TEST_CASE_MAC(cpssDxChPortTxQueueTxEnableGet)
{
/*
    ITERATE_DEVICES_PHY_CPU_PORTS (DxCh)
    1.1.1. Call with tcQueue [0/ 1].
    Expected: GT_OK.
    1.1.2. Call with out of range tcQueue [CPSS_TC_RANGE_CNS = 8].
    Expected: GT_BAD_PARAM.
    1.1.3. Call with wrong enablePtr [NULL].
    Expected: GT_BAD_PTR.
*/
    GT_STATUS   st     = GT_OK;

    GT_U8       dev;
    GT_U8       port      = PORT_TX_VALID_PHY_PORT_CNS;
    GT_U8       tcQueue   = 0;
    GT_BOOL     enableGet = GT_TRUE;

    /* prepare iterator for go over all active devices */
    PRV_TGF_NOT_APPLIC_DEV_RESET_MAC(&dev, UTF_NONE_FAMILY_E);

    /* 1. Go over all active devices. */
    while (GT_OK == prvUtfNextDeviceGet(&dev, GT_TRUE))
    {
        st = prvUtfNextPhyPortReset(&port, dev);
        UTF_VERIFY_EQUAL0_PARAM_MAC(GT_OK, st);

        /* 1.1. For all active devices go over all available physical ports. */
        while (GT_OK == prvUtfNextPhyPortGet(&port, GT_TRUE))
        {
            /*
                1.1.1. Call with tcQueue [0/ 1].
                Expected: GT_OK.
            */
            /* Call function with tcQueue = 0 */
            tcQueue = 0;

            st = cpssDxChPortTxQueueTxEnableGet(dev, port, tcQueue, &enableGet);
            UTF_VERIFY_EQUAL3_PARAM_MAC(GT_OK, st, dev, port, tcQueue);

            /* Call function with tcQueue = 1 */
            tcQueue = 1;

            st = cpssDxChPortTxQueueTxEnableGet(dev, port, tcQueue, &enableGet);
            UTF_VERIFY_EQUAL3_PARAM_MAC(GT_OK, st, dev, port, tcQueue);

            /*
                1.1.2. Call with out of range tcQueue [CPSS_TC_RANGE_CNS = 8].
                Expected: GT_BAD_PARAM.
            */
            tcQueue = CPSS_TC_RANGE_CNS;

            st = cpssDxChPortTxQueueTxEnableGet(dev, port, tcQueue, &enableGet);
            UTF_VERIFY_EQUAL3_PARAM_MAC(GT_BAD_PARAM, st, dev, port, tcQueue);

            tcQueue = 1;

            /*
                1.1.3. Call with wrong enablePtr [NULL].
                Expected: GT_BAD_PTR.
            */
            st = cpssDxChPortTxQueueTxEnableGet(dev, port, tcQueue, NULL);
            UTF_VERIFY_EQUAL3_PARAM_MAC(GT_BAD_PTR, st, dev, port, tcQueue);
        }

        tcQueue = 4;

        st = prvUtfNextPhyPortReset(&port, dev);
        UTF_VERIFY_EQUAL0_PARAM_MAC(GT_OK, st);

        /* 1.2. For all active devices go over all non available physical ports. */
        while (GT_OK == prvUtfNextPhyPortGet(&port, GT_FALSE))
        {
            /* 1.2.1. Call function for each non-active port */
            st = cpssDxChPortTxQueueTxEnableGet(dev, port, tcQueue, &enableGet);
            UTF_VERIFY_EQUAL2_PARAM_MAC(GT_BAD_PARAM, st, dev, port);
        }

        /* 1.3. For active device check that function returns GT_BAD_PARAM */
        /* for out of bound value for port number.                         */
        port = PRV_CPSS_MAX_PP_PORTS_NUM_CNS;

        st = cpssDxChPortTxQueueTxEnableGet(dev, port, tcQueue, &enableGet);
        UTF_VERIFY_EQUAL2_PARAM_MAC(GT_BAD_PARAM, st, dev, port);

        /* 1.4. For active device check that function returns GT_OK        */
        /* for out CPU port number.                                        */
        /* enable == GT_TRUE, tcQueue == 4 */
        port = CPSS_CPU_PORT_NUM_CNS;

        st = cpssDxChPortTxQueueTxEnableGet(dev, port, tcQueue, &enableGet);
        UTF_VERIFY_EQUAL2_PARAM_MAC(GT_OK, st, dev, port);
    }

    /* 2. For not active devices check that function returns non GT_OK.*/
    port = PORT_TX_VALID_PHY_PORT_CNS;

    /* prepare device iterator */
    PRV_TGF_NOT_APPLIC_DEV_RESET_MAC(&dev, UTF_NONE_FAMILY_E);

    /* Go over all non active devices. */
    while(GT_OK == prvUtfNextDeviceGet(&dev, GT_FALSE))
    {
        st = cpssDxChPortTxQueueTxEnableGet(dev, port, tcQueue, &enableGet);
        UTF_VERIFY_EQUAL1_PARAM_MAC(GT_NOT_APPLICABLE_DEVICE, st, dev);
    }

    /* 3.Call function with out of bound value for device id */
    dev = PRV_CPSS_MAX_PP_DEVICES_CNS;
    /* port == 0, enable == GT_TRUE, tcQueue == 4 */

    st = cpssDxChPortTxQueueTxEnableGet(dev, port, tcQueue, &enableGet);
    UTF_VERIFY_EQUAL1_PARAM_MAC(GT_BAD_PARAM, st, dev);
}

/*----------------------------------------------------------------------------*/
/*
GT_STATUS cpssDxChPortTxQueueingEnableGet
(
    IN  GT_U8     devNum,
    IN  GT_U8     portNum,
    IN  GT_U8     tcQueue,
    OUT GT_BOOL   *enablePtr
)
*/
UTF_TEST_CASE_MAC(cpssDxChPortTxQueueingEnableGet)
{
/*
    ITERATE_DEVICES_PHY_CPU_PORTS (DxCh)
    1.1.1. Call with tcQueue [0/ 1].
    Expected: GT_OK.
    1.1.2. Call with out of range tcQueue [CPSS_TC_RANGE_CNS = 8].
    Expected: GT_BAD_PARAM.
    1.1.3. Call with wrong enablePtr [NULL].
    Expected: GT_BAD_PTR.
*/
    GT_STATUS   st     = GT_OK;

    GT_U8       dev;
    GT_U8       port      = PORT_TX_VALID_PHY_PORT_CNS;
    GT_U8       tcQueue   = 0;
    GT_BOOL     enableGet = GT_TRUE;

    /* prepare iterator for go over all active devices */
    PRV_TGF_NOT_APPLIC_DEV_RESET_MAC(&dev, UTF_NONE_FAMILY_E);

    /* 1. Go over all active devices. */
    while (GT_OK == prvUtfNextDeviceGet(&dev, GT_TRUE))
    {
        st = prvUtfNextPhyPortReset(&port, dev);
        UTF_VERIFY_EQUAL0_PARAM_MAC(GT_OK, st);

        /* 1.1. For all active devices go over all available physical ports. */
        while (GT_OK == prvUtfNextPhyPortGet(&port, GT_TRUE))
        {
            /*
                1.1.1. Call with tcQueue [0/ 1].
                Expected: GT_OK.
            */
            /* Call function with tcQueue = 0 */
            tcQueue = 0;

            st = cpssDxChPortTxQueueingEnableGet(dev, port, tcQueue, &enableGet);
            UTF_VERIFY_EQUAL3_PARAM_MAC(GT_OK, st, dev, port, tcQueue);

            /* Call function with tcQueue = 1 */
            tcQueue = 1;

            st = cpssDxChPortTxQueueingEnableGet(dev, port, tcQueue, &enableGet);
            UTF_VERIFY_EQUAL3_PARAM_MAC(GT_OK, st, dev, port, tcQueue);

            /*
                1.1.2. Call with out of range tcQueue [CPSS_TC_RANGE_CNS = 8].
                Expected: GT_BAD_PARAM.
            */
            tcQueue = CPSS_TC_RANGE_CNS;

            st = cpssDxChPortTxQueueingEnableGet(dev, port, tcQueue, &enableGet);
            UTF_VERIFY_EQUAL3_PARAM_MAC(GT_BAD_PARAM, st, dev, port, tcQueue);

            tcQueue = 1;

            /*
                1.1.3. Call with wrong enablePtr [NULL].
                Expected: GT_BAD_PTR.
            */
            st = cpssDxChPortTxQueueingEnableGet(dev, port, tcQueue, NULL);
            UTF_VERIFY_EQUAL3_PARAM_MAC(GT_BAD_PTR, st, dev, port, tcQueue);
        }

        tcQueue = 4;

        st = prvUtfNextPhyPortReset(&port, dev);
        UTF_VERIFY_EQUAL0_PARAM_MAC(GT_OK, st);

        /* 1.2. For all active devices go over all non available physical ports. */
        while (GT_OK == prvUtfNextPhyPortGet(&port, GT_FALSE))
        {
            /* 1.2.1. Call function for each non-active port */
            st = cpssDxChPortTxQueueingEnableGet(dev, port, tcQueue, &enableGet);
            UTF_VERIFY_EQUAL2_PARAM_MAC(GT_BAD_PARAM, st, dev, port);
        }

        /* 1.3. For active device check that function returns GT_BAD_PARAM */
        /* for out of bound value for port number.                         */
        port = PRV_CPSS_MAX_PP_PORTS_NUM_CNS;

        st = cpssDxChPortTxQueueingEnableGet(dev, port, tcQueue, &enableGet);
        UTF_VERIFY_EQUAL2_PARAM_MAC(GT_BAD_PARAM, st, dev, port);

        /* 1.4. For active device check that function returns GT_OK        */
        /* for out CPU port number.                                        */
        /* enable == GT_TRUE, tcQueue == 4 */
        port = CPSS_CPU_PORT_NUM_CNS;

        st = cpssDxChPortTxQueueingEnableGet(dev, port, tcQueue, &enableGet);
        UTF_VERIFY_EQUAL2_PARAM_MAC(GT_OK, st, dev, port);
    }

    /* 2. For not active devices check that function returns non GT_OK.*/
    port = PORT_TX_VALID_PHY_PORT_CNS;

    /* prepare device iterator */
    PRV_TGF_NOT_APPLIC_DEV_RESET_MAC(&dev, UTF_NONE_FAMILY_E);

    /* Go over all non active devices. */
    while(GT_OK == prvUtfNextDeviceGet(&dev, GT_FALSE))
    {
        st = cpssDxChPortTxQueueingEnableGet(dev, port, tcQueue, &enableGet);
        UTF_VERIFY_EQUAL1_PARAM_MAC(GT_NOT_APPLICABLE_DEVICE, st, dev);
    }

    /* 3.Call function with out of bound value for device id */
    dev = PRV_CPSS_MAX_PP_DEVICES_CNS;
    /* port == 0, enable == GT_TRUE, tcQueue == 4 */

    st = cpssDxChPortTxQueueingEnableGet(dev, port, tcQueue, &enableGet);
    UTF_VERIFY_EQUAL1_PARAM_MAC(GT_BAD_PARAM, st, dev);
}

/*----------------------------------------------------------------------------*/
/*
GT_STATUS cpssDxChPortTxShaperOnStackAsGigEnableGet
(
    IN  GT_U8    dev,
    OUT GT_BOOL  *enablePtr
)
*/
UTF_TEST_CASE_MAC(cpssDxChPortTxShaperOnStackAsGigEnableGet)
{
/*
    ITERATE_DEVICES(DxChXcat)
    1.1. Call with not null enablePtr.
    Expected: GT_OK.
    1.2. Call api with wrong enablePtr [NULL].
    Expected: GT_BAD_PTR.
*/
    GT_STATUS st = GT_OK;

    GT_U8    dev;
    GT_BOOL  enable;

    /* prepare device iterator */
    PRV_TGF_NOT_APPLIC_DEV_RESET_MAC(&dev, UTF_CH1_E | UTF_CH1_DIAMOND_E | UTF_CH2_E | UTF_CH3_E | UTF_LION_E);
    
    /* 1. Go over all active devices. */
    while (GT_OK == prvUtfNextDeviceGet(&dev, GT_TRUE))
    {
        /*
            1.1. Call with not null enablePtr.
            Expected: GT_OK.
        */
        st = cpssDxChPortTxShaperOnStackAsGigEnableGet(dev, &enable);
        if ((PRV_CPSS_PP_MAC(prvTgfDevNum)->devFamily == CPSS_PP_FAMILY_DXCH_XCAT_E) && 
            (GT_TRUE != PRV_CPSS_DXCH_XCAT_A3_ONLY_CHECK_MAC(prvTgfDevNum)))
            UTF_VERIFY_EQUAL1_PARAM_MAC(GT_NOT_APPLICABLE_DEVICE, st, dev);
        else
            UTF_VERIFY_EQUAL1_PARAM_MAC(GT_OK, st, dev);

        /*
            1.2. Call api with wrong enablePtr [NULL].
            Expected: GT_BAD_PTR.
        */
        st = cpssDxChPortTxShaperOnStackAsGigEnableGet(dev, NULL);
        if ((PRV_CPSS_PP_MAC(prvTgfDevNum)->devFamily == CPSS_PP_FAMILY_DXCH_XCAT_E) && 
            (GT_TRUE != PRV_CPSS_DXCH_XCAT_A3_ONLY_CHECK_MAC(prvTgfDevNum)))
            UTF_VERIFY_EQUAL1_PARAM_MAC(GT_NOT_APPLICABLE_DEVICE, st, dev);
        else
            UTF_VERIFY_EQUAL1_STRING_MAC(GT_BAD_PTR, st, 
                                       "%d, enablePtr = NULL", dev);
    }
     
    /* 2. For not-active devices and devices from non-applicable family */
    /* check that function returns GT_BAD_PARAM.                        */

    /* prepare device iterator */
    PRV_TGF_NOT_APPLIC_DEV_RESET_MAC(&dev, UTF_CH1_E | UTF_CH1_DIAMOND_E | UTF_CH2_E | UTF_CH3_E | UTF_LION_E);

    /* go over all non active devices */
    while(GT_OK == prvUtfNextDeviceGet(&dev, GT_FALSE))
    {
        st = cpssDxChPortTxShaperOnStackAsGigEnableGet(dev, &enable);
        UTF_VERIFY_EQUAL1_PARAM_MAC(GT_NOT_APPLICABLE_DEVICE, st, dev);
    }
     
    /* 3. Call function with out of bound value for device id.*/
    dev = PRV_CPSS_MAX_PP_DEVICES_CNS;

    st = cpssDxChPortTxShaperOnStackAsGigEnableGet(dev, &enable);
    UTF_VERIFY_EQUAL1_PARAM_MAC(GT_BAD_PARAM, st, dev);
}

/*----------------------------------------------------------------------------*/
/*
GT_STATUS cpssDxChPortTxShaperOnStackAsGigEnableSet
(
    IN  GT_U8   dev,
    IN  GT_BOOL enable
)
*/
UTF_TEST_CASE_MAC(cpssDxChPortTxShaperOnStackAsGigEnableSet)
{
/*
    ITERATE_DEVICES(DxChXcat)
    1.1. Call with enable[GT_TRUE / GT_FALSE],
    Expected: GT_OK.
    1.2. Call cpssDxChPortTxShaperOnStackAsGigEnableGet
           with the same parameters.
    Expected: GT_OK and the same values than was set.
*/
    GT_STATUS st = GT_OK;

    GT_U8   dev;
    GT_BOOL enable = GT_FALSE;
    GT_BOOL enableGet = GT_FALSE;

    /* prepare device iterator */
    PRV_TGF_NOT_APPLIC_DEV_RESET_MAC(&dev, UTF_CH1_E | UTF_CH1_DIAMOND_E | UTF_CH2_E | UTF_CH3_E | UTF_LION_E);
    
    /* 1. Go over all active devices. */
    while (GT_OK == prvUtfNextDeviceGet(&dev, GT_TRUE))
    {
        /*
            1.1. Call with enable[GT_TRUE / GT_FALSE],
            Expected: GT_OK.
        */
        /* call with enable[GT_TRUE] */
        enable = GT_TRUE;

        st = cpssDxChPortTxShaperOnStackAsGigEnableSet(dev, enable);
        if ((PRV_CPSS_PP_MAC(prvTgfDevNum)->devFamily == CPSS_PP_FAMILY_DXCH_XCAT_E) && 
            (GT_TRUE != PRV_CPSS_DXCH_XCAT_A3_ONLY_CHECK_MAC(prvTgfDevNum)))
            UTF_VERIFY_EQUAL1_PARAM_MAC(GT_NOT_APPLICABLE_DEVICE, st, dev);
        else
            UTF_VERIFY_EQUAL1_PARAM_MAC(GT_OK, st, dev);

        /*
            1.2. Call cpssDxChPortTxShaperOnStackAsGigEnableGet
                   with the same parameters.
            Expected: GT_OK and the same values than was set.
        */
        st = cpssDxChPortTxShaperOnStackAsGigEnableGet(dev, &enableGet);
        if ((PRV_CPSS_PP_MAC(prvTgfDevNum)->devFamily == CPSS_PP_FAMILY_DXCH_XCAT_E) && 
            (GT_TRUE != PRV_CPSS_DXCH_XCAT_A3_ONLY_CHECK_MAC(prvTgfDevNum)))
            UTF_VERIFY_EQUAL1_PARAM_MAC(GT_NOT_APPLICABLE_DEVICE, st, dev);
        else
            UTF_VERIFY_EQUAL1_STRING_MAC(GT_OK, st,
                      "cpssDxChPortTxShaperOnStackAsGigEnableGet: %d ", dev);

        if (GT_OK == st)
        {
            /* Verifying values */
            UTF_VERIFY_EQUAL1_STRING_MAC(enable, enableGet, 
                           "got another enable then was set: %d", dev);
        }

        /* call with enable[GT_FALSE] */
        enable = GT_FALSE;

        st = cpssDxChPortTxShaperOnStackAsGigEnableSet(dev, enable);
        if ((PRV_CPSS_PP_MAC(prvTgfDevNum)->devFamily == CPSS_PP_FAMILY_DXCH_XCAT_E) && 
            (GT_TRUE != PRV_CPSS_DXCH_XCAT_A3_ONLY_CHECK_MAC(prvTgfDevNum)))
            UTF_VERIFY_EQUAL1_PARAM_MAC(GT_NOT_APPLICABLE_DEVICE, st, dev);
        else
            UTF_VERIFY_EQUAL1_PARAM_MAC(GT_OK, st, dev);

        if (GT_OK == st)
        {
            /* call get func again, 1.2. */
            st = cpssDxChPortTxShaperOnStackAsGigEnableGet(dev, &enableGet);
            UTF_VERIFY_EQUAL1_STRING_MAC(GT_OK, st,
                      "cpssDxChPortTxShaperOnStackAsGigEnableGet: %d ", dev);

            /* Verifying values */
            UTF_VERIFY_EQUAL1_STRING_MAC(enable, enableGet, 
                           "got another enable then was set: %d", dev);
        }
    }

    /* restore correct values */
    enable = GT_TRUE;
     
    /* 2. For not-active devices and devices from non-applicable family */
    /* check that function returns GT_BAD_PARAM.                        */

    /* prepare device iterator */
    PRV_TGF_NOT_APPLIC_DEV_RESET_MAC(&dev, UTF_CH1_E | UTF_CH1_DIAMOND_E | UTF_CH2_E | UTF_CH3_E | UTF_LION_E);

    /* go over all non active devices */
    while(GT_OK == prvUtfNextDeviceGet(&dev, GT_FALSE))
    {
        st = cpssDxChPortTxShaperOnStackAsGigEnableSet(dev, enable);
        UTF_VERIFY_EQUAL1_PARAM_MAC(GT_NOT_APPLICABLE_DEVICE, st, dev);
    }
     
    /* 3. Call function with out of bound value for device id.*/
    dev = PRV_CPSS_MAX_PP_DEVICES_CNS;

    st = cpssDxChPortTxShaperOnStackAsGigEnableSet(dev, enable);
    UTF_VERIFY_EQUAL1_PARAM_MAC(GT_BAD_PARAM, st, dev);
}

/*----------------------------------------------------------------------------*/
/*
 * Configuration of cpssDxChPortTx suit
 */
UTF_SUIT_BEGIN_TESTS_MAC(cpssDxChPortTx)
    UTF_SUIT_DECLARE_TEST_MAC(cpssDxChPortTxQueueEnableSet)
    UTF_SUIT_DECLARE_TEST_MAC(cpssDxChPortTxQueueingEnableSet)
    UTF_SUIT_DECLARE_TEST_MAC(cpssDxChPortTxQueueTxEnableSet)
    UTF_SUIT_DECLARE_TEST_MAC(cpssDxChPortTxFlushQueuesSet)
    UTF_SUIT_DECLARE_TEST_MAC(cpssDxChPortTxShaperEnableSet)
    UTF_SUIT_DECLARE_TEST_MAC(cpssDxChPortTxShaperEnableGet)
    UTF_SUIT_DECLARE_TEST_MAC(cpssDxChPortTxShaperProfileSet)
    UTF_SUIT_DECLARE_TEST_MAC(cpssDxChPortTxShaperProfileGet)
    UTF_SUIT_DECLARE_TEST_MAC(cpssDxChPortTxQShaperEnableSet)
    UTF_SUIT_DECLARE_TEST_MAC(cpssDxChPortTxQShaperEnableGet)
    UTF_SUIT_DECLARE_TEST_MAC(cpssDxChPortTxQShaperProfileSet)
    UTF_SUIT_DECLARE_TEST_MAC(cpssDxChPortTxQShaperProfileGet)
    UTF_SUIT_DECLARE_TEST_MAC(cpssDxChPortTxBindPortToSchedulerProfileSet)
    UTF_SUIT_DECLARE_TEST_MAC(cpssDxChPortTxBindPortToSchedulerProfileGet)
    UTF_SUIT_DECLARE_TEST_MAC(cpssDxChPortTx4TcTailDropProfileSet)
    UTF_SUIT_DECLARE_TEST_MAC(cpssDxChPortTx4TcTailDropProfileGet)
    UTF_SUIT_DECLARE_TEST_MAC(cpssDxChPortTxTailDropProfileSet)
    UTF_SUIT_DECLARE_TEST_MAC(cpssDxChPortTxTailDropProfileGet)
    UTF_SUIT_DECLARE_TEST_MAC(cpssDxChPortTxSniffedPcktDescrLimitSet)
    UTF_SUIT_DECLARE_TEST_MAC(cpssDxChPortTxSniffedPcktDescrLimitGet)
    UTF_SUIT_DECLARE_TEST_MAC(cpssDxChPortTxMcastPcktDescrLimitSet)
    UTF_SUIT_DECLARE_TEST_MAC(cpssDxChPortTxWrrGlobalParamSet)
    UTF_SUIT_DECLARE_TEST_MAC(cpssDxChPortTxShaperGlobalParamsSet)
    UTF_SUIT_DECLARE_TEST_MAC(cpssDxChPortTxQWrrProfileSet)
    UTF_SUIT_DECLARE_TEST_MAC(cpssDxChPortTxQWrrProfileGet)
    UTF_SUIT_DECLARE_TEST_MAC(cpssDxChPortTxQArbGroupSet)
    UTF_SUIT_DECLARE_TEST_MAC(cpssDxChPortTxQArbGroupGet)
    UTF_SUIT_DECLARE_TEST_MAC(cpssDxChPortTxBindPortToDpSet)
    UTF_SUIT_DECLARE_TEST_MAC(cpssDxChPortTxBindPortToDpGet)
    UTF_SUIT_DECLARE_TEST_MAC(cpssDxChPortTxDescNumberGet)
    UTF_SUIT_DECLARE_TEST_MAC(cpssDxChPortTxBufNumberGet)
    UTF_SUIT_DECLARE_TEST_MAC(cpssDxChPortTx4TcDescNumberGet)
    UTF_SUIT_DECLARE_TEST_MAC(cpssDxChPortTx4TcBufNumberGet)
    UTF_SUIT_DECLARE_TEST_MAC(cpssDxChPortTxToCpuShaperModeSet)
    UTF_SUIT_DECLARE_TEST_MAC(cpssDxChPortTxSharingGlobalResourceEnableSet)
    UTF_SUIT_DECLARE_TEST_MAC(cpssDxChPortTxSharingGlobalResourceEnableGet)
    UTF_SUIT_DECLARE_TEST_MAC(cpssDxChPortTxSharedGlobalResourceLimitsSet)
    UTF_SUIT_DECLARE_TEST_MAC(cpssDxChPortTxSharedGlobalResourceLimitsGet)
    UTF_SUIT_DECLARE_TEST_MAC(cpssDxChPortTxWatchdogGet)
    UTF_SUIT_DECLARE_TEST_MAC(cpssDxChPortTxWatchdogEnableSet)
    UTF_SUIT_DECLARE_TEST_MAC(cpssDxChPortTxTailDropUcEnableSet)
    UTF_SUIT_DECLARE_TEST_MAC(cpssDxChPortTxTailDropUcEnableGet)
    UTF_SUIT_DECLARE_TEST_MAC(cpssDxChPortTxBufferTailDropEnableSet)
    UTF_SUIT_DECLARE_TEST_MAC(cpssDxChPortTxBufferTailDropEnableGet)
    UTF_SUIT_DECLARE_TEST_MAC(cpssDxChPortTxBuffersSharingMaxLimitSet)
    UTF_SUIT_DECLARE_TEST_MAC(cpssDxChPortTxBuffersSharingMaxLimitGet)
    UTF_SUIT_DECLARE_TEST_MAC(cpssDxChPortTxDp1SharedEnableSet)
    UTF_SUIT_DECLARE_TEST_MAC(cpssDxChPortTxDp1SharedEnableGet)
    UTF_SUIT_DECLARE_TEST_MAC(cpssDxChPortTxTcSharedProfileEnableSet)
    UTF_SUIT_DECLARE_TEST_MAC(cpssDxChPortTxTcSharedProfileEnableGet)
    UTF_SUIT_DECLARE_TEST_MAC(cpssDxChPortTxMcastPcktDescrLimitGet)
    UTF_SUIT_DECLARE_TEST_MAC(cpssDxChPortTxRandomTailDropEnableGet)
    UTF_SUIT_DECLARE_TEST_MAC(cpssDxChPortTxRandomTailDropEnableSet)
    UTF_SUIT_DECLARE_TEST_MAC(cpssDxChPortTxShaperGlobalParamsGet)
    UTF_SUIT_DECLARE_TEST_MAC(cpssDxChPortTxShaperTokenBucketMtuSet)
    UTF_SUIT_DECLARE_TEST_MAC(cpssDxChPortTxShaperTokenBucketMtuGet)
    UTF_SUIT_DECLARE_TEST_MAC(cpssDxChTxPortAllShapersDisable)
    UTF_SUIT_DECLARE_TEST_MAC(cpssDxChTxPortShapersDisable)
    UTF_SUIT_DECLARE_TEST_MAC(cpssDxChPortTxInit)
    UTF_SUIT_DECLARE_TEST_MAC(cpssDxChPortTxSharedPolicySet)
    UTF_SUIT_DECLARE_TEST_MAC(cpssDxChPortTxSharedPolicyGet)
    UTF_SUIT_DECLARE_TEST_MAC(cpssDxChPortTxShaperModeSet)
    UTF_SUIT_DECLARE_TEST_MAC(cpssDxChPortTxShaperModeGet)
    UTF_SUIT_DECLARE_TEST_MAC(cpssDxChPortTxShaperConfigurationSet)
    UTF_SUIT_DECLARE_TEST_MAC(cpssDxChPortTxShaperConfigurationGet)
    UTF_SUIT_DECLARE_TEST_MAC(cpssDxChPortTxShaperBaselineSet)
    UTF_SUIT_DECLARE_TEST_MAC(cpssDxChPortTxShaperBaselineGet)
    UTF_SUIT_DECLARE_TEST_MAC(cpssDxChPortTxSchedulerDeficitModeEnableSet)
    UTF_SUIT_DECLARE_TEST_MAC(cpssDxChPortTxSchedulerDeficitModeEnableGet)
    UTF_SUIT_DECLARE_TEST_MAC(cpssDxChPortTxRequestMaskSet)
    UTF_SUIT_DECLARE_TEST_MAC(cpssDxChPortTxRequestMaskGet)
    UTF_SUIT_DECLARE_TEST_MAC(cpssDxChPortTxRequestMaskIndexSet)
    UTF_SUIT_DECLARE_TEST_MAC(cpssDxChPortTxRequestMaskIndexGet)
    UTF_SUIT_DECLARE_TEST_MAC(cpssDxChPortTxGlobalBufNumberGet)
    UTF_SUIT_DECLARE_TEST_MAC(cpssDxChPortTxGlobalDescNumberGet)
    UTF_SUIT_DECLARE_TEST_MAC(cpssDxChPortTxMcastBufNumberGet)
    UTF_SUIT_DECLARE_TEST_MAC(cpssDxChPortTxMcastBuffersLimitGet)
    UTF_SUIT_DECLARE_TEST_MAC(cpssDxChPortTxMcastBuffersLimitSet)
    UTF_SUIT_DECLARE_TEST_MAC(cpssDxChPortTxMcastDescNumberGet)
    UTF_SUIT_DECLARE_TEST_MAC(cpssDxChPortTxProfileWeightedRandomTailDropEnableGet)
    UTF_SUIT_DECLARE_TEST_MAC(cpssDxChPortTxProfileWeightedRandomTailDropEnableSet)
    UTF_SUIT_DECLARE_TEST_MAC(cpssDxChPortTxResourceHistogramCounterGet)
    UTF_SUIT_DECLARE_TEST_MAC(cpssDxChPortTxResourceHistogramThresholdGet)
    UTF_SUIT_DECLARE_TEST_MAC(cpssDxChPortTxResourceHistogramThresholdSet)
    UTF_SUIT_DECLARE_TEST_MAC(cpssDxChPortTxSharedPoolLimitsGet)
    UTF_SUIT_DECLARE_TEST_MAC(cpssDxChPortTxSharedPoolLimitsSet)
    UTF_SUIT_DECLARE_TEST_MAC(cpssDxChPortTxSharedResourceBufNumberGet)
    UTF_SUIT_DECLARE_TEST_MAC(cpssDxChPortTxSharedResourceDescNumberGet)
    UTF_SUIT_DECLARE_TEST_MAC(cpssDxChPortTxSniffedBufNumberGet)
    UTF_SUIT_DECLARE_TEST_MAC(cpssDxChPortTxSniffedBuffersLimitGet)
    UTF_SUIT_DECLARE_TEST_MAC(cpssDxChPortTxSniffedBuffersLimitSet)
    UTF_SUIT_DECLARE_TEST_MAC(cpssDxChPortTxSniffedDescNumberGet)
    UTF_SUIT_DECLARE_TEST_MAC(cpssDxChPortTxTailDropBufferConsumptionModeGet)
    UTF_SUIT_DECLARE_TEST_MAC(cpssDxChPortTxTailDropBufferConsumptionModeSet)
    UTF_SUIT_DECLARE_TEST_MAC(cpssDxChPortTxTailDropWrtdMasksGet)
    UTF_SUIT_DECLARE_TEST_MAC(cpssDxChPortTxTailDropWrtdMasksSet)
    UTF_SUIT_DECLARE_TEST_MAC(cpssDxChPortTxTcProfileSharedPoolGet)
    UTF_SUIT_DECLARE_TEST_MAC(cpssDxChPortTxTcProfileSharedPoolSet)

    UTF_SUIT_DECLARE_TEST_MAC(cpssDxChPortTxByteCountChangeEnableGet)
    UTF_SUIT_DECLARE_TEST_MAC(cpssDxChPortTxByteCountChangeEnableSet)
    UTF_SUIT_DECLARE_TEST_MAC(cpssDxChPortTxByteCountChangeValueGet)
    UTF_SUIT_DECLARE_TEST_MAC(cpssDxChPortTxByteCountChangeValueSet)
    UTF_SUIT_DECLARE_TEST_MAC(cpssDxChPortTxQueueEnableGet)
    UTF_SUIT_DECLARE_TEST_MAC(cpssDxChPortTxQueueTxEnableGet)
    UTF_SUIT_DECLARE_TEST_MAC(cpssDxChPortTxQueueingEnableGet)
    UTF_SUIT_DECLARE_TEST_MAC(cpssDxChPortTxShaperOnStackAsGigEnableGet)
    UTF_SUIT_DECLARE_TEST_MAC(cpssDxChPortTxShaperOnStackAsGigEnableSet)

UTF_SUIT_END_TESTS_MAC(cpssDxChPortTx)

