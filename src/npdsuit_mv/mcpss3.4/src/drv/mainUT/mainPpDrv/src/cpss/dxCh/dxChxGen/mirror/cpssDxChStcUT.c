/*******************************************************************************H
*              (c), Copyright 2001, Marvell International Ltd.                 *
* THIS CODE CONTAINS CONFIDENTIAL INFORMATION OF MARVELL SEMICONDUCTOR, INC.   *
* NO RIGHTS ARE GRANTED HEREIN UNDER ANY PATENT, MASK WORK RIGHT OR COPYRIGHT  *
* OF MARVELL OR ANY THIRD PARTY. MARVELL RESERVES THE RIGHT AT ITS SOLE        *
* DISCRETION TO REQUEST THAT THIS CODE BE IMMEDIATELY RETURNED TO MARVELL.     *
* THIS CODE IS PROVIDED "AS IS". MARVELL MAKES NO WARRANTIES, EXPRESSED,       *
* IMPLIED OR OTHERWISE, REGARDING ITS ACCURACY, COMPLETENESS OR PERFORMANCE.   *
********************************************************************************
* cpssDxChStcUT.c
*
* DESCRIPTION:
*       Unit tests for cpssDxChStc, that provides
*       CPSS DxCh Sampling To CPU (STC) APIs
*
*
* FILE REVISION NUMBER:
*       $Revision: 1.1.1.1 $
*******************************************************************************/

/* includes */
#include <cpss/dxCh/dxChxGen/config/private/prvCpssDxChInfo.h>
#include <cpss/dxCh/dxChxGen/mirror/cpssDxChStc.h>

#include <utf/utfMain.h>
#include <utf/private/prvUtfExtras.h>

/* defines */

/* Default valid value for port id */
#define MIRROR_VALID_PHY_PORT_CNS       0

/*----------------------------------------------------------------------------*/
/*
GT_STATUS cpssDxChStcIngressCountModeSet
(
    IN  GT_U8                           devNum,
    IN  CPSS_DXCH_STC_COUNT_MODE_ENT    mode
)
*/
UTF_TEST_CASE_MAC(cpssDxChStcIngressCountModeSet)
{
/*
    ITERATE_DEVICES (DxChx)
    1.1. Call function with mode[CPSS_DXCH_STC_COUNT_ALL_PACKETS_E / 
                                CPSS_DXCH_STC_COUNT_NON_DROPPED_PACKETS_E]
    Expected: GT_OK.
    1.2. Call cpssDxChStcIngressCountModeGet with non-NULL modeGet
              and other parameter as in 1.1.
    Expected: GT_OK and same mode as written
    1.3. Call function with wrong enum values mode.
    Expected: GT_BAD_PARAM. 
*/
    GT_STATUS   st       = GT_OK;
    GT_U8       dev;

    CPSS_DXCH_STC_COUNT_MODE_ENT    mode    = CPSS_DXCH_STC_COUNT_ALL_PACKETS_E;
    CPSS_DXCH_STC_COUNT_MODE_ENT    modeGet = CPSS_DXCH_STC_COUNT_ALL_PACKETS_E;


    /* prepare iterator for go over all active devices */
    PRV_TGF_NOT_APPLIC_DEV_RESET_MAC(&dev, UTF_NONE_FAMILY_E);

    /* 1. Go over all active devices. */
    while (GT_OK == prvUtfNextDeviceGet(&dev, GT_TRUE))
    {
        /*
            1.1. Call function with mode[CPSS_DXCH_STC_COUNT_ALL_PACKETS_E / 
                                        CPSS_DXCH_STC_COUNT_NON_DROPPED_PACKETS_E]
            Expected: GT_OK.
        */
        /* call with mode = CPSS_DXCH_STC_COUNT_ALL_PACKETS_E */
        mode = CPSS_DXCH_STC_COUNT_ALL_PACKETS_E;

        st = cpssDxChStcIngressCountModeSet(dev, mode);
        UTF_VERIFY_EQUAL2_PARAM_MAC(GT_OK, st, dev, mode);

        /*
            1.2. Call cpssDxChStcIngressCountModeGet with non-NULL modeGet
                      and other parameter as in 1.1.
            Expected: GT_OK and same mode as written
        */
        st = cpssDxChStcIngressCountModeGet(dev, &modeGet);
        UTF_VERIFY_EQUAL1_STRING_MAC(GT_OK, st, "cpssDxChStcIngressCountModeGet: %d", dev);
        UTF_VERIFY_EQUAL1_STRING_MAC(mode, modeGet, "got another mode then was set: %d", dev);

        /* call with mode = CPSS_DXCH_STC_COUNT_NON_DROPPED_PACKETS_E */
        mode = CPSS_DXCH_STC_COUNT_NON_DROPPED_PACKETS_E;

        st = cpssDxChStcIngressCountModeSet(dev, mode);
        UTF_VERIFY_EQUAL2_PARAM_MAC(GT_OK, st, dev, mode);

        /*
            1.2. Call cpssDxChStcIngressCountModeGet with non-NULL modeGet
                      and other parameter as in 1.1.
            Expected: GT_OK and same mode as written
        */
        st = cpssDxChStcIngressCountModeGet(dev, &modeGet);
        UTF_VERIFY_EQUAL1_STRING_MAC(GT_OK, st, "cpssDxChStcIngressCountModeGet: %d", dev);
        UTF_VERIFY_EQUAL1_STRING_MAC(mode, modeGet, "got another mode then was set: %d", dev);

        /*
            1.3. Call function with wrong enum values mode.
            Expected: GT_BAD_PARAM. 
        */
        UTF_ENUMS_CHECK_MAC(cpssDxChStcIngressCountModeSet
                            (dev, mode),
                            mode);
    }

    mode = CPSS_DXCH_STC_COUNT_ALL_PACKETS_E;

    /* 2. For not active devices check that function returns non GT_OK.*/
    /* prepare device iterator */
    PRV_TGF_NOT_APPLIC_DEV_RESET_MAC(&dev, UTF_NONE_FAMILY_E);

    /* Go over all non active devices. */
    while(GT_OK == prvUtfNextDeviceGet(&dev, GT_FALSE))
    {
        st = cpssDxChStcIngressCountModeSet(dev, mode);
        UTF_VERIFY_EQUAL1_PARAM_MAC(GT_NOT_APPLICABLE_DEVICE, st, dev);
    }

    /* 3.Call function with out of bound value for device id */
    dev = PRV_CPSS_MAX_PP_DEVICES_CNS;

    st = cpssDxChStcIngressCountModeSet(dev, mode);
    UTF_VERIFY_EQUAL1_PARAM_MAC(GT_BAD_PARAM, st, dev);
}

/*----------------------------------------------------------------------------*/
/*
GT_STATUS cpssDxChStcIngressCountModeGet
(
    IN  GT_U8                           devNum,
    OUT CPSS_DXCH_STC_COUNT_MODE_ENT    *modePtr
)
*/
UTF_TEST_CASE_MAC(cpssDxChStcIngressCountModeGet)
{
/*
    ITERATE_DEVICES (DxChx)
    1.1. Call cpssDxChStcIngressCountModeGet with non-NULL modePtr
    Expected: GT_OK
    1.2. Call function with modePtr [NULL].
    Expected: GT_BAD_PTR.
*/
    GT_STATUS   st       = GT_OK;
    GT_U8       dev;

    CPSS_DXCH_STC_COUNT_MODE_ENT    mode    = CPSS_DXCH_STC_COUNT_ALL_PACKETS_E;


    /* prepare iterator for go over all active devices */
    PRV_TGF_NOT_APPLIC_DEV_RESET_MAC(&dev, UTF_NONE_FAMILY_E);

    /* 1. Go over all active devices. */
    while (GT_OK == prvUtfNextDeviceGet(&dev, GT_TRUE))
    {
        /*
            1.1. Call cpssDxChStcIngressCountModeGet with non-NULL modePtr
            Expected: GT_OK
        */
        st = cpssDxChStcIngressCountModeGet(dev, &mode);
        UTF_VERIFY_EQUAL1_PARAM_MAC(GT_OK, st, dev);

        /*
            1.2. Call function with modePtr [NULL].
            Expected: GT_BAD_PTR.
        */
        st = cpssDxChStcIngressCountModeGet(dev, NULL);
        UTF_VERIFY_EQUAL1_STRING_MAC(GT_BAD_PTR, st, "%d, modePtr = NULL", dev);
    }

    mode = CPSS_DXCH_STC_COUNT_ALL_PACKETS_E;

    /* 2. For not active devices check that function returns non GT_OK.*/
    /* prepare device iterator */
    PRV_TGF_NOT_APPLIC_DEV_RESET_MAC(&dev, UTF_NONE_FAMILY_E);

    /* Go over all non active devices. */
    while(GT_OK == prvUtfNextDeviceGet(&dev, GT_FALSE))
    {
        st = cpssDxChStcIngressCountModeGet(dev, &mode);
        UTF_VERIFY_EQUAL1_PARAM_MAC(GT_NOT_APPLICABLE_DEVICE, st, dev);
    }

    /* 3.Call function with out of bound value for device id */
    dev = PRV_CPSS_MAX_PP_DEVICES_CNS;

    st = cpssDxChStcIngressCountModeGet(dev, &mode);
    UTF_VERIFY_EQUAL1_PARAM_MAC(GT_BAD_PARAM, st, dev);
}

/*----------------------------------------------------------------------------*/
/*
GT_STATUS cpssDxChStcReloadModeSet
(
    IN  GT_U8                                   devNum,
    IN  CPSS_DXCH_STC_TYPE_ENT                  stcType,
    IN  CPSS_DXCH_STC_COUNT_RELOAD_MODE_ENT     mode
)
*/
UTF_TEST_CASE_MAC(cpssDxChStcReloadModeSet)
{
/*
    ITERATE_DEVICES (DxChx)
    1.1. Call function with stcType [CPSS_DXCH_STC_INGRESS_E /
                                     CPSS_DXCH_STC_EGRESS_E],
                            mode [CPSS_DXCH_STC_COUNT_RELOAD_CONTINUOUS_E / 
                                  CPSS_DXCH_STC_COUNT_RELOAD_TRIGGERED_E].
    Expected: GT_OK.
    1.2. Call cpssDxChStcReloadModeGet with not NULL modePtr
              and other parameters as in 1.1.
    Expected: GT_OK and same mode as written
    1.3. Call with wrong enum values stcType  
              and other parameters as in 1.1.
    Expected: GT_BAD_PARAM.
    1.4. Call with wrong enum values mode  
              and other parameters as in 1.1.
    Expected: GT_BAD_PARAM.
*/
    GT_STATUS   st       = GT_OK;
    GT_U8       dev;

    CPSS_DXCH_STC_TYPE_ENT                  stcType = CPSS_DXCH_STC_INGRESS_E;
    CPSS_DXCH_STC_COUNT_RELOAD_MODE_ENT     mode    = CPSS_DXCH_STC_COUNT_RELOAD_CONTINUOUS_E;
    CPSS_DXCH_STC_COUNT_RELOAD_MODE_ENT     modeGet = CPSS_DXCH_STC_COUNT_RELOAD_CONTINUOUS_E;


    /* prepare iterator for go over all active devices */
    PRV_TGF_NOT_APPLIC_DEV_RESET_MAC(&dev, UTF_NONE_FAMILY_E);

    /* 1. Go over all active devices. */
    while (GT_OK == prvUtfNextDeviceGet(&dev, GT_TRUE))
    {
        /*
            1.1. Call function with stcType [CPSS_DXCH_STC_INGRESS_E / 
                                             CPSS_DXCH_STC_EGRESS_E],
                                    mode [CPSS_DXCH_STC_COUNT_RELOAD_CONTINUOUS_E / 
                                          CPSS_DXCH_STC_COUNT_RELOAD_TRIGGERED_E].
            Expected: GT_OK.
        */
        /* call with stcType = CPSS_DXCH_STC_INGRESS_E */
        stcType = CPSS_DXCH_STC_INGRESS_E;
        mode    = CPSS_DXCH_STC_COUNT_RELOAD_CONTINUOUS_E;

        st = cpssDxChStcReloadModeSet(dev, stcType, mode);
        UTF_VERIFY_EQUAL3_PARAM_MAC(GT_OK, st, dev, stcType, mode);

        /*
            1.2. Call cpssDxChStcReloadModeGet with not NULL modePtr
                      and other parameters as in 1.1.
            Expected: GT_OK and same mode as written
        */
        st = cpssDxChStcReloadModeGet(dev, stcType, &modeGet);
        UTF_VERIFY_EQUAL1_STRING_MAC(GT_OK, st, "cpssDxChStcReloadModeGet: %d", dev);
        UTF_VERIFY_EQUAL1_STRING_MAC(mode, modeGet, "got another mode then was set: %d", dev);

        /* call with stcType = CPSS_DXCH_STC_EGRESS_E */
        stcType = CPSS_DXCH_STC_EGRESS_E;
        mode    = CPSS_DXCH_STC_COUNT_RELOAD_TRIGGERED_E;

        st = cpssDxChStcReloadModeSet(dev, stcType, mode);
        UTF_VERIFY_EQUAL3_PARAM_MAC(GT_OK, st, dev, stcType, mode);

        /*
            1.2. Call cpssDxChStcReloadModeGet with not NULL modePtr
                      and other parameters as in 1.1.
            Expected: GT_OK and same mode as written
        */
        st = cpssDxChStcReloadModeGet(dev, stcType, &modeGet);
        UTF_VERIFY_EQUAL1_STRING_MAC(GT_OK, st, "cpssDxChStcReloadModeGet: %d", dev);
        UTF_VERIFY_EQUAL1_STRING_MAC(mode, modeGet, "got another mode then was set: %d", dev);

        /*
            1.3. Call with wrong enum values stcType and other parameters as in 1.1.
            Expected: GT_BAD_PARAM.
        */
        UTF_ENUMS_CHECK_MAC(cpssDxChStcReloadModeSet
                            (dev, stcType, mode),
                            stcType);

        /*
            1.4. Call with wrong enum values mode and other parameters as in 1.1.
            Expected: GT_BAD_PARAM.
        */
        UTF_ENUMS_CHECK_MAC(cpssDxChStcReloadModeSet
                            (dev, stcType, mode),
                            mode);
    }

    stcType = CPSS_DXCH_STC_INGRESS_E;
    mode    = CPSS_DXCH_STC_COUNT_ALL_PACKETS_E;

    /* 2. For not active devices check that function returns non GT_OK.*/
    /* prepare device iterator */
    PRV_TGF_NOT_APPLIC_DEV_RESET_MAC(&dev, UTF_NONE_FAMILY_E);

    /* Go over all non active devices. */
    while(GT_OK == prvUtfNextDeviceGet(&dev, GT_FALSE))
    {
        st = cpssDxChStcReloadModeSet(dev, stcType, mode);
        UTF_VERIFY_EQUAL1_PARAM_MAC(GT_NOT_APPLICABLE_DEVICE, st, dev);
    }

    /* 3.Call function with out of bound value for device id */
    dev = PRV_CPSS_MAX_PP_DEVICES_CNS;

    st = cpssDxChStcReloadModeSet(dev, stcType, mode);
    UTF_VERIFY_EQUAL1_PARAM_MAC(GT_BAD_PARAM, st, dev);    
}

/*----------------------------------------------------------------------------*/
/*
GT_STATUS cpssDxChStcReloadModeGet
(
    IN  GT_U8                                   devNum,
    IN  CPSS_DXCH_STC_TYPE_ENT                  stcType,
    OUT CPSS_DXCH_STC_COUNT_RELOAD_MODE_ENT     *modePtr
)
*/
UTF_TEST_CASE_MAC(cpssDxChStcReloadModeGet)
{
/*
    ITERATE_DEVICES (DxChx)
    1.1. Call function with stcType [CPSS_DXCH_STC_INGRESS_E / CPSS_DXCH_STC_EGRESS_E],
                            and not-NULL modePtr.
    Expected: GT_OK.
    1.2. Call with wrong enum values stcType 
              and other parameters as in 1.1.
    Expected: GT_BAD_PARAM.
    1.3. Call with modePtr [NULL] 
              and other parameters as in 1.1.
    Expected: GT_BAD_PTR.
*/
    GT_STATUS   st       = GT_OK;
    GT_U8       dev;

    CPSS_DXCH_STC_TYPE_ENT                  stcType = CPSS_DXCH_STC_INGRESS_E;
    CPSS_DXCH_STC_COUNT_RELOAD_MODE_ENT     mode    = CPSS_DXCH_STC_COUNT_RELOAD_CONTINUOUS_E;


    /* prepare iterator for go over all active devices */
    PRV_TGF_NOT_APPLIC_DEV_RESET_MAC(&dev, UTF_NONE_FAMILY_E);

    /* 1. Go over all active devices. */
    while (GT_OK == prvUtfNextDeviceGet(&dev, GT_TRUE))
    {
        /*
            1.1. Call function with stcType [CPSS_DXCH_STC_INGRESS_E / CPSS_DXCH_STC_EGRESS_E],
                                    and not-NULL modePtr.
            Expected: GT_OK.
        */
        /* call with stcType = CPSS_DXCH_STC_INGRESS_E */
        stcType = CPSS_DXCH_STC_INGRESS_E;

        st = cpssDxChStcReloadModeGet(dev, stcType, &mode);
        UTF_VERIFY_EQUAL2_PARAM_MAC(GT_OK, st, dev, stcType);

        /* call with stcType = CPSS_DXCH_STC_EGRESS_E */
        stcType = CPSS_DXCH_STC_EGRESS_E;

        st = cpssDxChStcReloadModeGet(dev, stcType, &mode);
        UTF_VERIFY_EQUAL2_PARAM_MAC(GT_OK, st, dev, stcType);

        /*
            1.2. Call with wrong enum values stcType and other parameters as in 1.1.
            Expected: GT_BAD_PARAM.
        */
        UTF_ENUMS_CHECK_MAC(cpssDxChStcReloadModeGet
                            (dev, stcType, &mode),
                            stcType);

        /*
            1.3. Call with modePtr [NULL] 
                      and other parameters as in 1.1.
            Expected: GT_BAD_PTR.
        */
        st = cpssDxChStcReloadModeGet(dev, stcType, NULL);
        UTF_VERIFY_EQUAL1_STRING_MAC(GT_BAD_PTR, st, "%d, modePtr = NULL", dev);
    }

    stcType = CPSS_DXCH_STC_INGRESS_E;

    /* 2. For not active devices check that function returns non GT_OK.*/
    /* prepare device iterator */
    PRV_TGF_NOT_APPLIC_DEV_RESET_MAC(&dev, UTF_NONE_FAMILY_E);

    /* Go over all non active devices. */
    while(GT_OK == prvUtfNextDeviceGet(&dev, GT_FALSE))
    {
        st = cpssDxChStcReloadModeGet(dev, stcType, &mode);
        UTF_VERIFY_EQUAL1_PARAM_MAC(GT_NOT_APPLICABLE_DEVICE, st, dev);
    }

    /* 3.Call function with out of bound value for device id */
    dev = PRV_CPSS_MAX_PP_DEVICES_CNS;

    st = cpssDxChStcReloadModeGet(dev, stcType, &mode);
    UTF_VERIFY_EQUAL1_PARAM_MAC(GT_BAD_PARAM, st, dev); 
}

/*----------------------------------------------------------------------------*/
/*
GT_STATUS cpssDxChStcEnableSet
(
    IN  GT_U8                       devNum,
    IN  CPSS_DXCH_STC_TYPE_ENT      stcType,
    IN  GT_BOOL                     enable
)
*/
UTF_TEST_CASE_MAC(cpssDxChStcEnableSet)
{
/*
    ITERATE_DEVICES (DxChx)
    1.1. Call function with stcType [CPSS_DXCH_STC_INGRESS_E / 
                                     CPSS_DXCH_STC_EGRESS_E], 
                            enable [GT_TRUE / GT_FALSE]
    Expected: GT_OK.
    1.2. Call cpssDxChStcEnableGet with non-NULL enableGet 
              and other parameters as in 1.1
    Expected: GT_OK and same enable as written
    1.3. Call with wrong enum values stcType 
              and other parameters as in 1.1.
    Expected: GT_BAD_PARAM
*/
    GT_STATUS   st       = GT_OK;
    GT_U8       dev;

    CPSS_DXCH_STC_TYPE_ENT  stcType   = CPSS_DXCH_STC_INGRESS_E;
    GT_BOOL                 enable    = GT_FALSE;
    GT_BOOL                 enableGet = GT_FALSE;


    /* prepare iterator for go over all active devices */
    PRV_TGF_NOT_APPLIC_DEV_RESET_MAC(&dev, UTF_NONE_FAMILY_E);

    /* 1. Go over all active devices. */
    while (GT_OK == prvUtfNextDeviceGet(&dev, GT_TRUE))
    {
        /*
            1.1. Call function with stcType [CPSS_DXCH_STC_INGRESS_E / 
                                             CPSS_DXCH_STC_EGRESS_E], 
                                    enable [GT_TRUE / GT_FALSE]
            Expected: GT_OK.
        */
        /* call with stcType = CPSS_DXCH_STC_INGRESS_E */
        stcType = CPSS_DXCH_STC_INGRESS_E;
        enable  = GT_TRUE;

        st = cpssDxChStcEnableSet(dev, stcType, enable);
        UTF_VERIFY_EQUAL3_PARAM_MAC(GT_OK, st, dev, stcType, enable);

        /*
            1.2. Call cpssDxChStcEnableGet with non-NULL enableGet 
                      and other parameters as in 1.1
            Expected: GT_OK and same enable as written
        */
        st = cpssDxChStcEnableGet(dev, stcType, &enableGet);
        UTF_VERIFY_EQUAL1_STRING_MAC(GT_OK, st, "cpssDxChStcEnableGet: %d", dev);
        UTF_VERIFY_EQUAL1_STRING_MAC(enable, enableGet, "got another enable then was set: %d", dev);

        /* call with stcType = CPSS_DXCH_STC_EGRESS_E */
        stcType = CPSS_DXCH_STC_EGRESS_E;
        enable  = GT_FALSE;

        st = cpssDxChStcEnableSet(dev, stcType, enable);
        UTF_VERIFY_EQUAL3_PARAM_MAC(GT_OK, st, dev, stcType, enable);

        /*
            1.2. Call cpssDxChStcEnableGet with non-NULL enableGet 
                      and other parameters as in 1.1
            Expected: GT_OK and same enable as written
        */
        st = cpssDxChStcEnableGet(dev, stcType, &enableGet);
        UTF_VERIFY_EQUAL1_STRING_MAC(GT_OK, st, "cpssDxChStcEnableGet: %d", dev);
        UTF_VERIFY_EQUAL1_STRING_MAC(enable, enableGet, "got another enable then was set: %d", dev);
        
        /*
            1.3. Call with wrong enum values stcType and other parameters as in 1.1.
            Expected: GT_BAD_PARAM
        */
        UTF_ENUMS_CHECK_MAC(cpssDxChStcEnableSet
                            (dev, stcType, enable),
                            stcType);
    }

    stcType = CPSS_DXCH_STC_INGRESS_E;
    enable  = GT_TRUE;

    /* 2. For not active devices check that function returns non GT_OK.*/
    /* prepare device iterator */
    PRV_TGF_NOT_APPLIC_DEV_RESET_MAC(&dev, UTF_NONE_FAMILY_E);

    /* Go over all non active devices. */
    while(GT_OK == prvUtfNextDeviceGet(&dev, GT_FALSE))
    {
        st = cpssDxChStcEnableSet(dev, stcType, enable);
        UTF_VERIFY_EQUAL1_PARAM_MAC(GT_NOT_APPLICABLE_DEVICE, st, dev);
    }

    /* 3.Call function with out of bound value for device id */
    dev = PRV_CPSS_MAX_PP_DEVICES_CNS;

    st = cpssDxChStcEnableSet(dev, stcType, enable);
    UTF_VERIFY_EQUAL1_PARAM_MAC(GT_BAD_PARAM, st, dev);
}

/*----------------------------------------------------------------------------*/
/*
GT_STATUS cpssDxChStcEnableGet
(
    IN  GT_U8                       devNum,
    IN  CPSS_DXCH_STC_TYPE_ENT      stcType,
    OUT GT_BOOL                     *enablePtr
)
*/
UTF_TEST_CASE_MAC(cpssDxChStcEnableGet)
{
/*
    ITERATE_DEVICES (DxChx)
    1.1. Call function with stcType [CPSS_DXCH_STC_INGRESS_E / CPSS_DXCH_STC_EGRESS_E], 
                            and non-NULL enablePtr
    Expected: GT_OK.
    1.2. Call with wrong enum values stcType 
              and other parameters as in 1.1.
    Expected: GT_BAD_PARAM
    1.3. Call with out of range enablePtr [NULL]
              and other parameters as in 1.1.
    Expected: GT_BAD_PTR
*/
    GT_STATUS   st       = GT_OK;
    GT_U8       dev;

    CPSS_DXCH_STC_TYPE_ENT  stcType = CPSS_DXCH_STC_INGRESS_E;
    GT_BOOL                 enable  = GT_FALSE;


    /* prepare iterator for go over all active devices */
    PRV_TGF_NOT_APPLIC_DEV_RESET_MAC(&dev, UTF_NONE_FAMILY_E);

    /* 1. Go over all active devices. */
    while (GT_OK == prvUtfNextDeviceGet(&dev, GT_TRUE))
    {
        /*
            1.1. Call function with stcType [CPSS_DXCH_STC_INGRESS_E / CPSS_DXCH_STC_EGRESS_E], 
                                    and non-NULL enablePtr
            Expected: GT_OK.
        */
        /* Call with stcType = CPSS_DXCH_STC_INGRESS_E */
        stcType = CPSS_DXCH_STC_INGRESS_E;

        st = cpssDxChStcEnableGet(dev, stcType, &enable);
        UTF_VERIFY_EQUAL2_PARAM_MAC(GT_OK, st, dev, stcType);

        /* Call with stcType = CPSS_DXCH_STC_EGRESS_E */
        stcType = CPSS_DXCH_STC_EGRESS_E;

        st = cpssDxChStcEnableGet(dev, stcType, &enable);
        UTF_VERIFY_EQUAL2_PARAM_MAC(GT_OK, st, dev, stcType);

        /*
            1.2. Call with wrong enum values stcType and other parameters as in 1.1.
            Expected: GT_BAD_PARAM
        */
        UTF_ENUMS_CHECK_MAC(cpssDxChStcEnableGet
                            (dev, stcType, &enable),
                            stcType);

        /*
            1.3. Call with out of range enablePtr [NULL]
                      and other parameters as in 1.1.
            Expected: GT_BAD_PTR
        */
        st = cpssDxChStcEnableGet(dev, stcType, NULL);
        UTF_VERIFY_EQUAL1_STRING_MAC(GT_BAD_PTR, st, "%d, enablePtr = NULL", dev);
    }

    /* 2. For not active devices check that function returns non GT_OK.*/
    /* prepare device iterator */
    PRV_TGF_NOT_APPLIC_DEV_RESET_MAC(&dev, UTF_NONE_FAMILY_E);

    /* Go over all non active devices. */
    while(GT_OK == prvUtfNextDeviceGet(&dev, GT_FALSE))
    {
        st = cpssDxChStcEnableGet(dev, stcType, &enable);
        UTF_VERIFY_EQUAL1_PARAM_MAC(GT_NOT_APPLICABLE_DEVICE, st, dev);
    }

    /* 3.Call function with out of bound value for device id */
    dev = PRV_CPSS_MAX_PP_DEVICES_CNS;

    st = cpssDxChStcEnableGet(dev, stcType, &enable);
    UTF_VERIFY_EQUAL1_PARAM_MAC(GT_BAD_PARAM, st, dev);
}

/*----------------------------------------------------------------------------*/
/*
GT_STATUS cpssDxChStcPortLimitSet
(
    IN  GT_U8                       devNum,
    IN  GT_U8                       port,
    IN  CPSS_DXCH_STC_TYPE_ENT      stcType,
    IN  GT_U32                      limit
)
*/
UTF_TEST_CASE_MAC(cpssDxChStcPortLimitSet)
{
/*
    ITERATE_DEVICES_CPU_PORTS (DxChx)
    1.1.1. Call function with stcType [CPSS_DXCH_STC_INGRESS_E / CPSS_DXCH_STC_EGRESS_E],
                         and with limit [0 / 0x3FFFFFFF]
    Expected: GT_OK 
    1.1.2. Call cpssDxChStcPortLimitGet with not-NULL limitPtr
                and other parameters as in 1.1.1
    Expected: GT_OK and same limit thne was written
    1.1.3. Call with wrong enum values stcType 
              and other parameters as in 1.1.1.
    Expected: GT_BAD_PARAM
    1.1.4. Call with out of range limit [0x40000000]
              and other parameters as in 1.1.1.
    Expected: NOT GT_OK
*/
    GT_STATUS   st    = GT_OK;
    GT_U8       port  = MIRROR_VALID_PHY_PORT_CNS;
    GT_U8       dev;

    CPSS_DXCH_STC_TYPE_ENT  stcType  = CPSS_DXCH_STC_INGRESS_E;
    GT_U32                  limit    = 0;
    GT_U32                  limitGet = 0;


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
                1.1.1. Call function with stcType [CPSS_DXCH_STC_INGRESS_E / CPSS_DXCH_STC_EGRESS_E],
                                     and with limit [0 / 0x3FFFFFFF]
                Expected: GT_OK    
            */
            /* call with stcType = CPSS_DXCH_STC_INGRESS_E */
            stcType = CPSS_DXCH_STC_INGRESS_E;
            limit   = 0;

            st = cpssDxChStcPortLimitSet(dev, port, stcType, limit);
            UTF_VERIFY_EQUAL4_PARAM_MAC(GT_OK, st, dev, port, stcType, limit);

            /*
                1.1.2. Call cpssDxChStcPortLimitGet with not-NULL limitPtr
                            and other parameters as in 1.1.1
                Expected: GT_OK and same limit thne was written
            */
            st = cpssDxChStcPortLimitGet(dev, port, stcType, &limitGet);
            UTF_VERIFY_EQUAL2_STRING_MAC(GT_OK, st, "cpssDxChStcPortLimitGet: %d, %d", dev, port);
            UTF_VERIFY_EQUAL2_STRING_MAC(limit, limitGet, "got another limit then was set: %d, %d", dev, port);

            /* call with stcType = CPSS_DXCH_STC_EGRESS_E */
            stcType = CPSS_DXCH_STC_EGRESS_E;
            limit   = 0x3FFFFFFF;

            st = cpssDxChStcPortLimitSet(dev, port, stcType, limit);
            UTF_VERIFY_EQUAL4_PARAM_MAC(GT_OK, st, dev, port, stcType, limit);
            
            /*
                1.1.2. Call cpssDxChStcPortLimitGet with not-NULL limitPtr
                            and other parameters as in 1.1.1
                Expected: GT_OK and same limit thne was written
            */
            st = cpssDxChStcPortLimitGet(dev, port, stcType, &limitGet);
            UTF_VERIFY_EQUAL2_STRING_MAC(GT_OK, st, "cpssDxChStcPortLimitGet: %d, %d", dev, port);
            UTF_VERIFY_EQUAL2_STRING_MAC(limit, limitGet, "got another limit then was set: %d, %d", dev, port);

            /*
                1.1.3. Call with wrong enum values stcType and other parameters as in 1.1.1.
                Expected: GT_BAD_PARAM
            */
            UTF_ENUMS_CHECK_MAC(cpssDxChStcPortLimitSet
                                (dev, port, stcType, limit),
                                stcType);

            /*
                1.1.4. Call with out of range limit [0x40000000]
                          and other parameters as in 1.1.1.
                Expected: NOT GT_OK
            */
            limit = 0x40000000;

            st = cpssDxChStcPortLimitSet(dev, port, stcType, limit);
            UTF_VERIFY_NOT_EQUAL3_STRING_MAC(GT_OK, st, "%d, %d, limit = %d", dev, port, limit);
        }

        stcType = CPSS_DXCH_STC_INGRESS_E;
        limit   = 0;

        st = prvUtfNextPhyPortReset(&port, dev);
        UTF_VERIFY_EQUAL0_PARAM_MAC(GT_OK, st);

        /* 1.2. For all active devices go over all non available physical ports. */
        while (GT_OK == prvUtfNextPhyPortGet(&port, GT_FALSE))
        {
            /* 1.2.1. Call function for each non-active port */
            st = cpssDxChStcPortLimitSet(dev, port, stcType, limit);
            UTF_VERIFY_EQUAL2_PARAM_MAC(GT_BAD_PARAM, st, dev, port);
        }

        /* 1.3. For active device check that function returns GT_BAD_PARAM */
        /* for out of bound value for port number.                         */

        port = PRV_CPSS_MAX_PP_PORTS_NUM_CNS;

        st = cpssDxChStcPortLimitSet(dev, port, stcType, limit);
        UTF_VERIFY_EQUAL2_PARAM_MAC(GT_BAD_PARAM, st, dev, port);

        /* 1.4. For active device check that function returns GT_BAD_PARAM */
        /* for CPU port number.                                            */
        port = CPSS_CPU_PORT_NUM_CNS;

        st = cpssDxChStcPortLimitSet(dev, port, stcType, limit);
        UTF_VERIFY_EQUAL2_PARAM_MAC(GT_BAD_PARAM, st, dev, port);
    }

    stcType = CPSS_DXCH_STC_INGRESS_E;
    limit   = 0;

    /* 2. For not active devices check that function returns non GT_OK.*/
    port = MIRROR_VALID_PHY_PORT_CNS;

    /* prepare device iterator */
    PRV_TGF_NOT_APPLIC_DEV_RESET_MAC(&dev, UTF_NONE_FAMILY_E);

    /* Go over all non active devices. */
    while(GT_OK == prvUtfNextDeviceGet(&dev, GT_FALSE))
    {
        st = cpssDxChStcPortLimitSet(dev, port, stcType, limit);
        UTF_VERIFY_EQUAL1_PARAM_MAC(GT_NOT_APPLICABLE_DEVICE, st, dev);
    }

    /* 3.Call function with out of bound value for device id */
    dev = PRV_CPSS_MAX_PP_DEVICES_CNS;

    st = cpssDxChStcPortLimitSet(dev, port, stcType, limit);
    UTF_VERIFY_EQUAL1_PARAM_MAC(GT_BAD_PARAM, st, dev);
}

/*----------------------------------------------------------------------------*/
/*
GT_STATUS cpssDxChStcPortLimitGet
(
    IN  GT_U8                       devNum,
    IN  GT_U8                       port,
    IN  CPSS_DXCH_STC_TYPE_ENT      stcType,
    OUT GT_U32                      *limitPtr
)
*/
UTF_TEST_CASE_MAC(cpssDxChStcPortLimitGet)
{
/*
    ITERATE_DEVICES_CPU_PORTS (DxChx)
    1.1.1. Call function with stcType [CPSS_DXCH_STC_INGRESS_E / CPSS_DXCH_STC_EGRESS_E],
                         and not-NULL limitPtr
    1.1.2. Call with wrong enum values stcType 
              and other parameters as in 1.1.1.
    Expected: GT_BAD_PARAM
    1.1.3. Call with limitPtr [NULL]
              and other parameters as in 1.1.1.
    Expected: GT_BAD_PTR
*/
    GT_STATUS   st    = GT_OK;
    GT_U8       port  = MIRROR_VALID_PHY_PORT_CNS;
    GT_U8       dev;

    CPSS_DXCH_STC_TYPE_ENT  stcType  = CPSS_DXCH_STC_INGRESS_E;
    GT_U32                  limit    = 0;


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
                1.1.1. Call function with stcType [CPSS_DXCH_STC_INGRESS_E / 
                                                   CPSS_DXCH_STC_EGRESS_E],
                                     and not-NULL limitPtr
                Expected: GT_OK
            */
            /* call with stcType = CPSS_DXCH_STC_INGRESS_E */
            stcType = CPSS_DXCH_STC_INGRESS_E;

            st = cpssDxChStcPortLimitGet(dev, port, stcType, &limit);
            UTF_VERIFY_EQUAL3_PARAM_MAC(GT_OK, st, dev, port, stcType);

            /* call with stcType = CPSS_DXCH_STC_EGRESS_E */
            stcType = CPSS_DXCH_STC_EGRESS_E;

            st = cpssDxChStcPortLimitGet(dev, port, stcType, &limit);
            UTF_VERIFY_EQUAL3_PARAM_MAC(GT_OK, st, dev, port, stcType);

            /*
                1.1.2. Call with wrong enum values stcType and other parameters as in 1.1.1.
                Expected: GT_BAD_PARAM
            */
            UTF_ENUMS_CHECK_MAC(cpssDxChStcPortLimitGet
                                (dev, port, stcType, &limit),
                                stcType);

            /*
                1.1.3. Call with limitPtr [NULL]
                          and other parameters as in 1.1.1.
                Expected: GT_BAD_PTR
            */
            st = cpssDxChStcPortLimitGet(dev, port, stcType, NULL);
            UTF_VERIFY_EQUAL2_STRING_MAC(GT_BAD_PTR, st, "%d, %d, limitPtr = NULL", dev, port);
        }

        stcType = CPSS_DXCH_STC_INGRESS_E;

        st = prvUtfNextPhyPortReset(&port, dev);
        UTF_VERIFY_EQUAL0_PARAM_MAC(GT_OK, st);

        /* 1.2. For all active devices go over all non available physical ports. */
        while (GT_OK == prvUtfNextPhyPortGet(&port, GT_FALSE))
        {
            /* 1.2.1. Call function for each non-active port */
            st = cpssDxChStcPortLimitGet(dev, port, stcType, &limit);
            UTF_VERIFY_EQUAL2_PARAM_MAC(GT_BAD_PARAM, st, dev, port);
        }

        /* 1.3. For active device check that function returns GT_BAD_PARAM */
        /* for out of bound value for port number.                         */

        port = PRV_CPSS_MAX_PP_PORTS_NUM_CNS;

        st = cpssDxChStcPortLimitGet(dev, port, stcType, &limit);
        UTF_VERIFY_EQUAL2_PARAM_MAC(GT_BAD_PARAM, st, dev, port);

        /* 1.4. For active device check that function returns GT_BAD_PARAM */
        /* for CPU port number.                                            */
        port = CPSS_CPU_PORT_NUM_CNS;

        st = cpssDxChStcPortLimitGet(dev, port, stcType, &limit);
        UTF_VERIFY_EQUAL2_PARAM_MAC(GT_BAD_PARAM, st, dev, port);
    }

    stcType = CPSS_DXCH_STC_INGRESS_E;

    /* 2. For not active devices check that function returns non GT_OK.*/
    port = MIRROR_VALID_PHY_PORT_CNS;

    /* prepare device iterator */
    PRV_TGF_NOT_APPLIC_DEV_RESET_MAC(&dev, UTF_NONE_FAMILY_E);

    /* Go over all non active devices. */
    while(GT_OK == prvUtfNextDeviceGet(&dev, GT_FALSE))
    {
        st = cpssDxChStcPortLimitGet(dev, port, stcType, &limit);
        UTF_VERIFY_EQUAL1_PARAM_MAC(GT_NOT_APPLICABLE_DEVICE, st, dev);
    }

    /* 3.Call function with out of bound value for device id */
    dev = PRV_CPSS_MAX_PP_DEVICES_CNS;

    st = cpssDxChStcPortLimitGet(dev, port, stcType, &limit);
    UTF_VERIFY_EQUAL1_PARAM_MAC(GT_BAD_PARAM, st, dev);
}

/*----------------------------------------------------------------------------*/
/*
GT_STATUS cpssDxChStcPortReadyForNewLimitGet
(
    IN  GT_U8                       devNum,
    IN  GT_U8                       port,
    IN  CPSS_DXCH_STC_TYPE_ENT      stcType,
    OUT GT_BOOL                     *isReadyPtr
)
*/
UTF_TEST_CASE_MAC(cpssDxChStcPortReadyForNewLimitGet)
{
/*
    ITERATE_DEVICES_CPU_PORTS (DxChx)
    1.1.1. Call function with stcType [CPSS_DXCH_STC_INGRESS_E / CPSS_DXCH_STC_EGRESS_E],
                         and not-NULL isReadyPtr
    1.1.2. Call with wrong enum values stcType 
              and other parameters as in 1.1.1.
    Expected: GT_BAD_PARAM
    1.1.3. Call with isReadyPtr [NULL]
              and other parameters as in 1.1.1.
    Expected: GT_BAD_PTR
*/
    GT_STATUS   st    = GT_OK;
    GT_U8       port  = MIRROR_VALID_PHY_PORT_CNS;
    GT_U8       dev;

    CPSS_DXCH_STC_TYPE_ENT  stcType = CPSS_DXCH_STC_INGRESS_E;
    GT_BOOL                 isReady = GT_FALSE;


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
                1.1.1. Call function with stcType [CPSS_DXCH_STC_INGRESS_E /
                                                   CPSS_DXCH_STC_EGRESS_E],
                                     and not-NULL isReadyPtr
                Expected: GT_OK
            */
            /* Call with stcType = CPSS_DXCH_STC_INGRESS_E */
            stcType = CPSS_DXCH_STC_INGRESS_E;

            st = cpssDxChStcPortReadyForNewLimitGet(dev, port, stcType, &isReady);
            UTF_VERIFY_EQUAL3_PARAM_MAC(GT_OK, st, dev, port, stcType);

            /* Call with stcType = CPSS_DXCH_STC_EGRESS_E */
            stcType = CPSS_DXCH_STC_EGRESS_E;

            st = cpssDxChStcPortReadyForNewLimitGet(dev, port, stcType, &isReady);
            UTF_VERIFY_EQUAL3_PARAM_MAC(GT_OK, st, dev, port, stcType);

            /*
                1.1.2. Call with wrong enum values stcType and other parameters as in 1.1.1.
                Expected: GT_BAD_PARAM
            */
            UTF_ENUMS_CHECK_MAC(cpssDxChStcPortReadyForNewLimitGet
                                (dev, port, stcType, &isReady),
                                stcType);

            /*
                1.1.3. Call with isReadyPtr [NULL]
                          and other parameters as in 1.1.1.
                Expected: GT_BAD_PTR
            */
            st = cpssDxChStcPortReadyForNewLimitGet(dev, port, stcType, NULL);
            UTF_VERIFY_EQUAL2_STRING_MAC(GT_BAD_PTR, st, "%d, %d, isReadyPtr = NULL", dev, port);
        }

        stcType = CPSS_DXCH_STC_INGRESS_E;

        st = prvUtfNextPhyPortReset(&port, dev);
        UTF_VERIFY_EQUAL0_PARAM_MAC(GT_OK, st);

        /* 1.2. For all active devices go over all non available physical ports. */
        while (GT_OK == prvUtfNextPhyPortGet(&port, GT_FALSE))
        {
            /* 1.2.1. Call function for each non-active port */
            st = cpssDxChStcPortReadyForNewLimitGet(dev, port, stcType, &isReady);
            UTF_VERIFY_EQUAL2_PARAM_MAC(GT_BAD_PARAM, st, dev, port);
        }

        /* 1.3. For active device check that function returns GT_BAD_PARAM */
        /* for out of bound value for port number.                         */

        port = PRV_CPSS_MAX_PP_PORTS_NUM_CNS;

        st = cpssDxChStcPortReadyForNewLimitGet(dev, port, stcType, &isReady);
        UTF_VERIFY_EQUAL2_PARAM_MAC(GT_BAD_PARAM, st, dev, port);

        /* 1.4. For active device check that function returns GT_BAD_PARAM */
        /* for CPU port number.                                            */
        port = CPSS_CPU_PORT_NUM_CNS;

        st = cpssDxChStcPortReadyForNewLimitGet(dev, port, stcType, &isReady);
        UTF_VERIFY_EQUAL2_PARAM_MAC(GT_BAD_PARAM, st, dev, port);
    }

    stcType = CPSS_DXCH_STC_INGRESS_E;

    /* 2. For not active devices check that function returns non GT_OK.*/
    port = MIRROR_VALID_PHY_PORT_CNS;

    /* prepare device iterator */
    PRV_TGF_NOT_APPLIC_DEV_RESET_MAC(&dev, UTF_NONE_FAMILY_E);

    /* Go over all non active devices. */
    while(GT_OK == prvUtfNextDeviceGet(&dev, GT_FALSE))
    {
        st = cpssDxChStcPortReadyForNewLimitGet(dev, port, stcType, &isReady);
        UTF_VERIFY_EQUAL1_PARAM_MAC(GT_NOT_APPLICABLE_DEVICE, st, dev);
    }

    /* 3.Call function with out of bound value for device id */
    dev = PRV_CPSS_MAX_PP_DEVICES_CNS;

    st = cpssDxChStcPortReadyForNewLimitGet(dev, port, stcType, &isReady);
    UTF_VERIFY_EQUAL1_PARAM_MAC(GT_BAD_PARAM, st, dev);
}

/*----------------------------------------------------------------------------*/
/*
GT_STATUS cpssDxChStcPortCountdownCntrGet
(
    IN  GT_U8                       devNum,
    IN  GT_U8                       port,
    IN  CPSS_DXCH_STC_TYPE_ENT      stcType,
    OUT GT_U32                      *cntrPtr
)
*/
UTF_TEST_CASE_MAC(cpssDxChStcPortCountdownCntrGet)
{
/*
    ITERATE_DEVICES_CPU_PORTS (DxChx)
    1.1.1. Call function with stcType [CPSS_DXCH_STC_INGRESS_E / CPSS_DXCH_STC_EGRESS_E],
                         and not-NULL cntrPtr
    1.1.2. Call with wrong enum values stcType 
              and other parameters as in 1.1.1.
    Expected: GT_BAD_PARAM
    1.1.3. Call with cntrPtr [NULL]
              and other parameters as in 1.1.1.
    Expected: GT_BAD_PTR
*/
    GT_STATUS   st    = GT_OK;
    GT_U8       port  = MIRROR_VALID_PHY_PORT_CNS;
    GT_U8       dev;

    CPSS_DXCH_STC_TYPE_ENT  stcType = CPSS_DXCH_STC_INGRESS_E;
    GT_U32                  cntr    = 0;


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
                1.1.1. Call function with stcType [CPSS_DXCH_STC_INGRESS_E / 
                                                   CPSS_DXCH_STC_EGRESS_E],
                                     and not-NULL cntrPtr
                Expected: GT_OK
            */
            /* call with stcType = CPSS_DXCH_STC_INGRESS_E */
            stcType = CPSS_DXCH_STC_INGRESS_E;

            st = cpssDxChStcPortCountdownCntrGet(dev, port, stcType, &cntr);
            UTF_VERIFY_EQUAL3_PARAM_MAC(GT_OK, st, dev, port, stcType);

            /* call with stcType = CPSS_DXCH_STC_EGRESS_E */
            stcType = CPSS_DXCH_STC_EGRESS_E;

            st = cpssDxChStcPortCountdownCntrGet(dev, port, stcType, &cntr);
            UTF_VERIFY_EQUAL3_PARAM_MAC(GT_OK, st, dev, port, stcType);

            /*
                1.1.2. Call with wrong enum values stcType and other parameters as in 1.1.1.
                Expected: GT_BAD_PARAM
            */
            UTF_ENUMS_CHECK_MAC(cpssDxChStcPortCountdownCntrGet
                                (dev, port, stcType, &cntr),
                                stcType);

            /*
                1.1.3. Call with cntrPtr [NULL]
                          and other parameters as in 1.1.1.
                Expected: GT_BAD_PTR
            */
            st = cpssDxChStcPortCountdownCntrGet(dev, port, stcType, NULL);
            UTF_VERIFY_EQUAL3_PARAM_MAC(GT_BAD_PTR, st, "%d, %d, cntrPtr = NULL", dev, port);
        }

        stcType = CPSS_DXCH_STC_INGRESS_E;

        st = prvUtfNextPhyPortReset(&port, dev);
        UTF_VERIFY_EQUAL0_PARAM_MAC(GT_OK, st);

        /* 1.2. For all active devices go over all non available physical ports. */
        while (GT_OK == prvUtfNextPhyPortGet(&port, GT_FALSE))
        {
            /* 1.2.1. Call function for each non-active port */
            st = cpssDxChStcPortCountdownCntrGet(dev, port, stcType, &cntr);
            UTF_VERIFY_EQUAL2_PARAM_MAC(GT_BAD_PARAM, st, dev, port);
        }

        /* 1.3. For active device check that function returns GT_BAD_PARAM */
        /* for out of bound value for port number.                         */

        port = PRV_CPSS_MAX_PP_PORTS_NUM_CNS;

        st = cpssDxChStcPortCountdownCntrGet(dev, port, stcType, &cntr);
        UTF_VERIFY_EQUAL2_PARAM_MAC(GT_BAD_PARAM, st, dev, port);

        /* 1.4. For active device check that function returns GT_BAD_PARAM */
        /* for CPU port number.                                            */
        port = CPSS_CPU_PORT_NUM_CNS;

        st = cpssDxChStcPortCountdownCntrGet(dev, port, stcType, &cntr);
        UTF_VERIFY_EQUAL2_PARAM_MAC(GT_BAD_PARAM, st, dev, port);
    }

    stcType = CPSS_DXCH_STC_INGRESS_E;

    /* 2. For not active devices check that function returns non GT_OK.*/
    port = MIRROR_VALID_PHY_PORT_CNS;

    /* prepare device iterator */
    PRV_TGF_NOT_APPLIC_DEV_RESET_MAC(&dev, UTF_NONE_FAMILY_E);

    /* Go over all non active devices. */
    while(GT_OK == prvUtfNextDeviceGet(&dev, GT_FALSE))
    {
        st = cpssDxChStcPortCountdownCntrGet(dev, port, stcType, &cntr);
        UTF_VERIFY_EQUAL1_PARAM_MAC(GT_NOT_APPLICABLE_DEVICE, st, dev);
    }

    /* 3.Call function with out of bound value for device id */
    dev = PRV_CPSS_MAX_PP_DEVICES_CNS;

    st = cpssDxChStcPortCountdownCntrGet(dev, port, stcType, &cntr);
    UTF_VERIFY_EQUAL1_PARAM_MAC(GT_BAD_PARAM, st, dev);
}

/*----------------------------------------------------------------------------*/
/*
GT_STATUS cpssDxChStcPortSampledPacketsCntrSet 
(
    IN  GT_U8                       devNum,
    IN  GT_U8                       port,
    IN  CPSS_DXCH_STC_TYPE_ENT      stcType,
    IN  GT_U32                      cntr
)
*/
UTF_TEST_CASE_MAC(cpssDxChStcPortSampledPacketsCntrSet)
{
/*
    ITERATE_DEVICES_CPU_PORTS (DxChx)
    1.1.1. Call function with stcType [CPSS_DXCH_STC_INGRESS_E / CPSS_DXCH_STC_EGRESS_E],
                         and cntr [0 / 0xFFFF]
    Expected: GT_OK
    1.1.2. Call cpssDxChStcPortSampledPacketsCntrGet with not-NULL cntrGet
                and other parameters as in 1.1.1
    Expected: GT_OK and same cntr thne was written
    1.1.3. Call with wrong enum values stcType 
              and other parameters as in 1.1.1.
    Expected: GT_BAD_PARAM
    1.1.4. Call with out of range cntr [0x10000]
              and other parameters as in 1.1.1.
    Expected: NOT GT_OK
*/
    GT_STATUS   st    = GT_OK;
    GT_U8       port  = MIRROR_VALID_PHY_PORT_CNS;
    GT_U8       dev;

    CPSS_DXCH_STC_TYPE_ENT  stcType = CPSS_DXCH_STC_INGRESS_E;
    GT_U32                  cntr    = 0;
    GT_U32                  cntrGet = 0;


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
                1.1.1. Call function with stcType [CPSS_DXCH_STC_INGRESS_E / 
                                                   CPSS_DXCH_STC_EGRESS_E],
                                     and cntr [0 / 0xFFFF]
                Expected: GT_OK
            */
            /* call with stcType = CPSS_DXCH_STC_INGRESS_E */
            stcType = CPSS_DXCH_STC_INGRESS_E;
            cntr    = 0;

            st = cpssDxChStcPortSampledPacketsCntrSet(dev, port, stcType, cntr);
            UTF_VERIFY_EQUAL4_PARAM_MAC(GT_OK, st, dev, port, stcType, cntr);

            /*
                1.1.2. Call cpssDxChStcPortSampledPacketsCntrGet with not-NULL cntrGet
                            and other parameters as in 1.1.1
                Expected: GT_OK and same cntr thne was written
            */
            st = cpssDxChStcPortSampledPacketsCntrGet(dev, port, stcType, &cntrGet);
            UTF_VERIFY_EQUAL2_STRING_MAC(GT_OK, st, "cpssDxChStcPortSampledPacketsCntrGet: %d, %d", dev, port);
            UTF_VERIFY_EQUAL2_STRING_MAC(cntr, cntrGet, "got another cntr then was set: %d, %d", dev, port);

            /* call with stcType = CPSS_DXCH_STC_EGRESS_E */
            stcType = CPSS_DXCH_STC_EGRESS_E;
            cntr    = 0xFFFF;

            st = cpssDxChStcPortSampledPacketsCntrSet(dev, port, stcType, cntr);
            UTF_VERIFY_EQUAL4_PARAM_MAC(GT_OK, st, dev, port, stcType, cntr);

            /*
                1.1.2. Call cpssDxChStcPortSampledPacketsCntrGet with not-NULL cntrGet
                            and other parameters as in 1.1.1
                Expected: GT_OK and same cntr thne was written
            */
            st = cpssDxChStcPortSampledPacketsCntrGet(dev, port, stcType, &cntrGet);
            UTF_VERIFY_EQUAL2_STRING_MAC(GT_OK, st, "cpssDxChStcPortSampledPacketsCntrGet: %d, %d", dev, port);
            UTF_VERIFY_EQUAL2_STRING_MAC(cntr, cntrGet, "got another cntr then was set: %d, %d", dev, port);

            /*
                1.1.3. Call with wrong enum values stcType and other parameters as in 1.1.1.
                Expected: GT_BAD_PARAM
            */
            UTF_ENUMS_CHECK_MAC(cpssDxChStcPortSampledPacketsCntrSet
                                (dev, port, stcType, cntr),
                                stcType);

            /*
                1.1.4. Call with out of range cntr [0x10000]
                          and other parameters as in 1.1.1.
                Expected: NOT GT_OK
            */
            cntr = 0x10000;

            st = cpssDxChStcPortSampledPacketsCntrSet(dev, port, stcType, cntr);
            UTF_VERIFY_NOT_EQUAL3_STRING_MAC(GT_OK, st, "%d, %d, cntr = %d", dev, port, cntr);
        }   

        stcType = CPSS_DXCH_STC_INGRESS_E;
        cntr    = 0;

        st = prvUtfNextPhyPortReset(&port, dev);
        UTF_VERIFY_EQUAL0_PARAM_MAC(GT_OK, st);

        /* 1.2. For all active devices go over all non available physical ports. */
        while (GT_OK == prvUtfNextPhyPortGet(&port, GT_FALSE))
        {
            /* 1.2.1. Call function for each non-active port */
            st = cpssDxChStcPortSampledPacketsCntrSet(dev, port, stcType, cntr);
            UTF_VERIFY_EQUAL2_PARAM_MAC(GT_BAD_PARAM, st, dev, port);
        }

        /* 1.3. For active device check that function returns GT_BAD_PARAM */
        /* for out of bound value for port number.                         */

        port = PRV_CPSS_MAX_PP_PORTS_NUM_CNS;

        st = cpssDxChStcPortSampledPacketsCntrSet(dev, port, stcType, cntr);
        UTF_VERIFY_EQUAL2_PARAM_MAC(GT_BAD_PARAM, st, dev, port);

        /* 1.4. For active device check that function returns GT_BAD_PARAM */
        /* for CPU port number.                                            */
        port = CPSS_CPU_PORT_NUM_CNS;

        st = cpssDxChStcPortSampledPacketsCntrSet(dev, port, stcType, cntr);
        UTF_VERIFY_EQUAL2_PARAM_MAC(GT_BAD_PARAM, st, dev, port);
    }

    stcType = CPSS_DXCH_STC_INGRESS_E;
    cntr    = 0;

    /* 2. For not active devices check that function returns non GT_OK.*/
    port = MIRROR_VALID_PHY_PORT_CNS;

    /* prepare device iterator */
    PRV_TGF_NOT_APPLIC_DEV_RESET_MAC(&dev, UTF_NONE_FAMILY_E);

    /* Go over all non active devices. */
    while(GT_OK == prvUtfNextDeviceGet(&dev, GT_FALSE))
    {
        st = cpssDxChStcPortSampledPacketsCntrSet(dev, port, stcType, cntr);
        UTF_VERIFY_EQUAL1_PARAM_MAC(GT_NOT_APPLICABLE_DEVICE, st, dev);
    }

    /* 3.Call function with out of bound value for device id */
    dev = PRV_CPSS_MAX_PP_DEVICES_CNS;

    st = cpssDxChStcPortSampledPacketsCntrSet(dev, port, stcType, cntr);
    UTF_VERIFY_EQUAL1_PARAM_MAC(GT_BAD_PARAM, st, dev);
}

/*----------------------------------------------------------------------------*/
/*
GT_STATUS cpssDxChStcPortSampledPacketsCntrGet
(
    IN  GT_U8                       devNum,
    IN  GT_U8                       port,
    IN  CPSS_DXCH_STC_TYPE_ENT      stcType,
    OUT GT_U32                      *cntrPtr
)
*/
UTF_TEST_CASE_MAC(cpssDxChStcPortSampledPacketsCntrGet)
{
/*
    ITERATE_DEVICES_CPU_PORTS (DxChx)
    1.1.1. Call function with stcType [CPSS_DXCH_STC_INGRESS_E / CPSS_DXCH_STC_EGRESS_E],
                         and non-NULL cntrPtr
    1.1.2. Call with wrong enum values stcType 
              and other parameters as in 1.1.1.
    Expected: GT_BAD_PARAM
    1.1.3. Call with cntrPtr [NULL]
              and other parameters as in 1.1.1.
    Expected: GT_BAD_PTR
*/
    GT_STATUS   st    = GT_OK;
    GT_U8       port  = MIRROR_VALID_PHY_PORT_CNS;
    GT_U8       dev;

    CPSS_DXCH_STC_TYPE_ENT  stcType = CPSS_DXCH_STC_INGRESS_E;
    GT_U32                  cntr    = 0;


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
                1.1.1. Call function with stcType [CPSS_DXCH_STC_INGRESS_E / 
                                                   CPSS_DXCH_STC_EGRESS_E],
                                     and non-NULL cntrPtr
                Expected: GT_OK
            */
            /* call with stcType = CPSS_DXCH_STC_INGRESS_E */
            stcType = CPSS_DXCH_STC_INGRESS_E;

            st = cpssDxChStcPortSampledPacketsCntrGet(dev, port, stcType, &cntr);
            UTF_VERIFY_EQUAL3_PARAM_MAC(GT_OK, st, dev, port, stcType);

            /* call with stcType = CPSS_DXCH_STC_EGRESS_E */
            stcType = CPSS_DXCH_STC_EGRESS_E;

            st = cpssDxChStcPortSampledPacketsCntrGet(dev, port, stcType, &cntr);
            UTF_VERIFY_EQUAL3_PARAM_MAC(GT_OK, st, dev, port, stcType);

            /*
                1.1.2. Call with wrong enum values stcType and other parameters as in 1.1.1.
                Expected: GT_BAD_PARAM
            */
            UTF_ENUMS_CHECK_MAC(cpssDxChStcPortSampledPacketsCntrGet
                                (dev, port, stcType, &cntr),
                                stcType);

            /*
                1.1.3. Call with cntrPtr [NULL]
                          and other parameters as in 1.1.1.
                Expected: GT_BAD_PTR
            */
            st = cpssDxChStcPortSampledPacketsCntrGet(dev, port, stcType, NULL);
            UTF_VERIFY_EQUAL2_STRING_MAC(GT_BAD_PTR, st, "%d, %d, cntrPtr = NULL", dev, port);
        }   

        stcType = CPSS_DXCH_STC_INGRESS_E;

        st = prvUtfNextPhyPortReset(&port, dev);
        UTF_VERIFY_EQUAL0_PARAM_MAC(GT_OK, st);

        /* 1.2. For all active devices go over all non available physical ports. */
        while (GT_OK == prvUtfNextPhyPortGet(&port, GT_FALSE))
        {
            /* 1.2.1. Call function for each non-active port */
            st = cpssDxChStcPortSampledPacketsCntrGet(dev, port, stcType, &cntr);
            UTF_VERIFY_EQUAL2_PARAM_MAC(GT_BAD_PARAM, st, dev, port);
        }

        /* 1.3. For active device check that function returns GT_BAD_PARAM */
        /* for out of bound value for port number.                         */

        port = PRV_CPSS_MAX_PP_PORTS_NUM_CNS;

        st = cpssDxChStcPortSampledPacketsCntrGet(dev, port, stcType, &cntr);
        UTF_VERIFY_EQUAL2_PARAM_MAC(GT_BAD_PARAM, st, dev, port);

        /* 1.4. For active device check that function returns GT_BAD_PARAM */
        /* for CPU port number.                                            */
        port = CPSS_CPU_PORT_NUM_CNS;

        st = cpssDxChStcPortSampledPacketsCntrGet(dev, port, stcType, &cntr);
        UTF_VERIFY_EQUAL2_PARAM_MAC(GT_BAD_PARAM, st, dev, port);
    }

    stcType = CPSS_DXCH_STC_INGRESS_E;

    /* 2. For not active devices check that function returns non GT_OK.*/
    port = MIRROR_VALID_PHY_PORT_CNS;

    /* prepare device iterator */
    PRV_TGF_NOT_APPLIC_DEV_RESET_MAC(&dev, UTF_NONE_FAMILY_E);

    /* Go over all non active devices. */
    while(GT_OK == prvUtfNextDeviceGet(&dev, GT_FALSE))
    {
        st = cpssDxChStcPortSampledPacketsCntrGet(dev, port, stcType, &cntr);
        UTF_VERIFY_EQUAL1_PARAM_MAC(GT_NOT_APPLICABLE_DEVICE, st, dev);
    }

    /* 3.Call function with out of bound value for device id */
    dev = PRV_CPSS_MAX_PP_DEVICES_CNS;

    st = cpssDxChStcPortSampledPacketsCntrGet(dev, port, stcType, &cntr);
    UTF_VERIFY_EQUAL1_PARAM_MAC(GT_BAD_PARAM, st, dev);
}


/*----------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*
 * Configuration of cpssDxChStc suit
 */
UTF_SUIT_BEGIN_TESTS_MAC(cpssDxChStc)
    UTF_SUIT_DECLARE_TEST_MAC(cpssDxChStcIngressCountModeSet)
    UTF_SUIT_DECLARE_TEST_MAC(cpssDxChStcIngressCountModeGet)
    UTF_SUIT_DECLARE_TEST_MAC(cpssDxChStcReloadModeSet)
    UTF_SUIT_DECLARE_TEST_MAC(cpssDxChStcReloadModeGet)
    UTF_SUIT_DECLARE_TEST_MAC(cpssDxChStcEnableSet)
    UTF_SUIT_DECLARE_TEST_MAC(cpssDxChStcEnableGet)
    UTF_SUIT_DECLARE_TEST_MAC(cpssDxChStcPortLimitSet)
    UTF_SUIT_DECLARE_TEST_MAC(cpssDxChStcPortLimitGet)
    UTF_SUIT_DECLARE_TEST_MAC(cpssDxChStcPortReadyForNewLimitGet)
    UTF_SUIT_DECLARE_TEST_MAC(cpssDxChStcPortCountdownCntrGet)
    UTF_SUIT_DECLARE_TEST_MAC(cpssDxChStcPortSampledPacketsCntrSet)
    UTF_SUIT_DECLARE_TEST_MAC(cpssDxChStcPortSampledPacketsCntrGet)
UTF_SUIT_END_TESTS_MAC(cpssDxChStc)

