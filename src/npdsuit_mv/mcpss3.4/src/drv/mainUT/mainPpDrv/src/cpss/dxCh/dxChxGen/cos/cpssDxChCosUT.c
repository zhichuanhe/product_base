/*******************************************************************************
*              (c), Copyright 2001, Marvell International Ltd.                 *
* THIS CODE CONTAINS CONFIDENTIAL INFORMATION OF MARVELL SEMICONDUCTOR, INC.   *
* NO RIGHTS ARE GRANTED HEREIN UNDER ANY PATENT, MASK WORK RIGHT OR COPYRIGHT  *
* OF MARVELL OR ANY THIRD PARTY. MARVELL RESERVES THE RIGHT AT ITS SOLE        *
* DISCRETION TO REQUEST THAT THIS CODE BE IMMEDIATELY RETURNED TO MARVELL.     *
* THIS CODE IS PROVIDED "AS IS". MARVELL MAKES NO WARRANTIES, EXPRESSED,       *
* IMPLIED OR OTHERWISE, REGARDING ITS ACCURACY, COMPLETENESS OR PERFORMANCE.   *
********************************************************************************
* cpssDxChCosUT.c
*
* DESCRIPTION:
*       Unit tests for cpssDxChCos, that provides
*       Function implementation for Profile Qos configuration.
*
* FILE REVISION NUMBER:
*       $Revision: 1.1.1.1 $
*******************************************************************************/
/* includes */

#include <cpss/dxCh/dxChxGen/cos/cpssDxChCos.h>
#include <cpss/dxCh/dxChxGen/config/private/prvCpssDxChInfo.h>

#include <utf/utfMain.h>
#include <utf/private/prvUtfExtras.h>

/* defines */

/* Default valid values for port id */
#define COS_VALID_VIRT_PORT_CNS 0

#define COS_VALID_PHY_PORT_CNS  0

/*----------------------------------------------------------------------------*/
/*
GT_STATUS cpssDxChCosProfileEntrySet
(
     IN  GT_U8                      dev,
     IN  GT_U8                      profileIndex,
     IN  CPSS_DXCH_COS_PROFILE_STC  *cosPtr
)
*/
UTF_TEST_CASE_MAC(cpssDxChCosProfileEntrySet)
{
/*
    ITERATE_DEVICES (DxChx)
    1.1. Call function with profileIndex [1],
                            cosPtr { CPSS_DP_GREEN_E , 1, 1, 1, 1}
    Expected: GT_OK.
    1.2. Call cpssDxChCosProfileEntryGet with same profileIndex.
    Expected: GT_OK and the same cosPtr.
    1.3. Call function with out of range profileIndex [PRV_CPSS_DXCH_QOS_PROFILE_NUM_MAX_CNS = 72],
                            dscp [32],
                            cosPtr { CPSS_DP_GREEN_E , 1, 1, 1, 1}
    Expected: NOT GT_OK for Cheetah and GT_OK for Cheetah-2,3.
    1.4. Call function with out of range profileIndex [PRV_CPSS_DXCH2_QOS_PROFILE_NUM_MAX_CNS = 128]
                            and valid other parameters.
    Expected: NON GT_OK.
    1.5. Call function with profileIndex [2],
                            out of range cosPtr->dscp
                            cosPtr { CPSS_DP_GREEN_E , 1, 1, CPSS_DSCP_RANGE_CNS =64, 1}
    Expected: NOT GT_OK.
    1.6. Call function with profileIndex [3],and null cosPtr [NULL]
    Expected: GT_BAD_PTR.
    1.7. Call function with profileIndex [4],
                            out of range cosPtr->dropPrecedence
                            cosPtr {wrong enum values, 1, 1, 32, 1}
    Expected: GT_BAD_PARAM.
    1.8. Call function with profileIndex [5],
                            out of range cosPtr->userPriority
                            cosPtr {CPSS_DP_YELLOW_E, 8, 2, 32, 1}
    Expected: NOT GT_OK.
    1.9. Call function with profileIndex [6],
                            dscp [25],
                            out of range cosPtr->trafficClass
                            cosPtr { CPSS_DP_YELLOW_E, 5, CPSS_TC_RANGE_CNS = 8, 32, 1}
    Expected: GT_BAD_PARAM.
    1.10. Call function with profileIndex [5],
                             out of range cosPtr->exp
                             cosPtr {CPSS_DP_YELLOW_E, 8, 2, 32, 8}
    Expected: NOT GT_OK for Cheetah-2,3 and GT_OK for others.
*/
    GT_STATUS                 st      = GT_OK;

    GT_U8                     dev;
    GT_U32                    profile = 0;
    CPSS_DXCH_COS_PROFILE_STC cos;
    CPSS_DXCH_COS_PROFILE_STC cosGet;
    CPSS_PP_FAMILY_TYPE_ENT   devFamily;

    /* prepare device iterator */
    PRV_TGF_NOT_APPLIC_DEV_RESET_MAC(&dev, UTF_NONE_FAMILY_E);

    /* 1. Go over all active devices. */
    while (GT_OK == prvUtfNextDeviceGet(&dev, GT_TRUE))
    {
        /* Getting device family */
        st = prvUtfDeviceFamilyGet(dev, &devFamily);
        UTF_VERIFY_EQUAL1_STRING_MAC(GT_OK, st, "prvUtfDeviceFamilyGet: %d", dev);

        /*
            1.1. Call function with profileIndex [1],
                                    cosPtr { CPSS_DP_GREEN_E , 1, 1, 1, 1}
            Expected: GT_OK.
        */

        profile = 1;
        cos.dropPrecedence = CPSS_DP_GREEN_E;
        cos.userPriority   = 1;
        cos.trafficClass   = 1;
        cos.dscp           = 1;
        cos.exp            = 1;

        st = cpssDxChCosProfileEntrySet(dev, profile, &cos);
        UTF_VERIFY_EQUAL2_PARAM_MAC(GT_OK, st, dev, profile);

        /*
                1.2. Call cpssDxChCosProfileEntryGet with same profileIndex.
                Expected: GT_OK and the same cosPtr.
        */
        st = cpssDxChCosProfileEntryGet(dev, profile, &cosGet);
        UTF_VERIFY_EQUAL2_STRING_MAC(GT_OK, st,
                       "cpssDxChCosProfileEntryGet: %d, %d", dev, profile);

        /* Checking cos == cosGet */
        UTF_VERIFY_EQUAL1_STRING_MAC(cos.dropPrecedence, cosGet.dropPrecedence,
                       "get another cosPtr->dropPrecedence than was set: %d", dev);
        UTF_VERIFY_EQUAL1_STRING_MAC(cos.userPriority, cosGet.userPriority,
                       "get another cosPtr->userPriority than was set: %d", dev);
        UTF_VERIFY_EQUAL1_STRING_MAC(cos.trafficClass, cosGet.trafficClass,
                       "get another cosPtr->trafficClass than was set: %d", dev);
        UTF_VERIFY_EQUAL1_STRING_MAC(cos.dscp, cosGet.dscp,
                       "get another cosPtr->dscp than was set: %d", dev);
        if(devFamily != CPSS_PP_FAMILY_CHEETAH_E)
        {
            UTF_VERIFY_EQUAL1_STRING_MAC(cos.exp, cosGet.exp,
                           "get another cosPtr->exp than was set: %d", dev);
        }

        /*
            1.3. Call function with out of range profileIndex [PRV_CPSS_DXCH_QOS_PROFILE_NUM_MAX_CNS = 72],
                                    cosPtr { CPSS_DP_GREEN_E , 1, 1, 1, 1}
            Expected: NOT GT_OK for Cheetah and GT_OK for Cheetah-2,3.
        */
        profile = PRV_CPSS_DXCH_QOS_PROFILE_NUM_MAX_CNS;

        st = cpssDxChCosProfileEntrySet(dev, profile, &cos);
        if (devFamily != CPSS_PP_FAMILY_CHEETAH_E)
        {
            UTF_VERIFY_EQUAL2_PARAM_MAC(GT_OK, st, dev, profile);
        }
        else
        {
            UTF_VERIFY_NOT_EQUAL2_PARAM_MAC(GT_OK, st, dev, profile);
        }

        /*
            1.4. Call function with out of range profileIndex [PRV_CPSS_DXCH2_QOS_PROFILE_NUM_MAX_CNS = 128]
                            and valid other parameters.
            Expected: NON GT_OK.
        */
        profile = PRV_CPSS_DXCH2_QOS_PROFILE_NUM_MAX_CNS;

        st = cpssDxChCosProfileEntrySet(dev, profile, &cos);
        UTF_VERIFY_NOT_EQUAL2_PARAM_MAC(GT_OK, st, dev, profile);

        /*
            1.5. Call function with profileIndex [2],
                            out of range cosPtr->dscp
                            cosPtr { CPSS_DP_GREEN_E , 1, 1, CPSS_DSCP_RANGE_CNS =64, 1}
            Expected: NOT GT_OK.
        */
        profile = 2;
        cos.dscp = CPSS_DSCP_RANGE_CNS;

        st = cpssDxChCosProfileEntrySet(dev, profile, &cos);
        UTF_VERIFY_NOT_EQUAL2_STRING_MAC(GT_OK, st, "%d, cos->dscp= %d",
                                         dev, cos.dscp);

        cos.dscp = 32;
        /*
            1.6. Call function with profileIndex [3],
                                    and null cosPtr [NULL]
            Expected: GT_BAD_PTR.
        */
        profile = 3;

        st = cpssDxChCosProfileEntrySet(dev, profile, NULL);
        UTF_VERIFY_EQUAL2_STRING_MAC(GT_BAD_PTR, st, "%d, %d, NULL",
                                     dev, profile);

        /*
            1.7. Call function with profileIndex [4],
                            out of range cosPtr->dropPrecedence
                            cosPtr {wrong enum values, 1, 1, 32, 1}
            Expected: GT_BAD_PARAM.
        */
        profile = 4;

        UTF_ENUMS_CHECK_MAC(cpssDxChCosProfileEntrySet
                            (dev, profile, &cos),
                            cos.dropPrecedence);

        /*
            1.8. Call function with profileIndex [5],
                                    out of range cosPtr->userPriority
                                    cosPtr {CPSS_DP_YELLOW_E, 8, 2, 32, 1}
            Expected: NOT GT_OK.
        */
        profile            = 5;
        cos.dropPrecedence = CPSS_DP_RED_E;
        cos.userPriority   = CPSS_USER_PRIORITY_RANGE_CNS;
        cos.trafficClass   = 2;

        st = cpssDxChCosProfileEntrySet(dev, profile, &cos);
        UTF_VERIFY_NOT_EQUAL2_STRING_MAC(GT_OK, st,
            "%d, cosPtr->userPriority = %d", dev, cos.userPriority);

        /*
            1.9. Call function with profileIndex [6],
                                    out of range cosPtr->trafficClass
                                    cosPtr { CPSS_DP_YELLOW_E, 5, CPSS_TC_RANGE_CNS = 8, 32, 1}
            Expected: GT_BAD_PARAM.
        */
        profile          = 6;
        cos.userPriority = 5;
        cos.trafficClass = CPSS_TC_RANGE_CNS;

        st = cpssDxChCosProfileEntrySet(dev, profile, &cos);
        UTF_VERIFY_EQUAL2_STRING_MAC(GT_BAD_PARAM, st,
            "%d, cos->trafficClass = %d", dev, cos.trafficClass);

        /*
            1.10. Call function with profileIndex [5],
                                     out of range cosPtr->exp
                                     cosPtr {CPSS_DP_YELLOW_E, 5, 2, 32, 8}
            Expected: NOT GT_OK for Cheetah-2,3 and GT_OK for others.
        */
        profile          = 5;
        cos.trafficClass = 2;
        cos.exp = CPSS_EXP_RANGE_CNS;

        st = cpssDxChCosProfileEntrySet(dev, profile, &cos);
        if (devFamily != CPSS_PP_FAMILY_CHEETAH_E)
        {
            UTF_VERIFY_NOT_EQUAL2_PARAM_MAC(GT_OK, st, dev, cos.exp);
        }
        else
        {
            UTF_VERIFY_EQUAL2_PARAM_MAC(GT_OK, st, dev, cos.exp);
        }

        /*
            1.11. Call function with DP==CPSS_DP_YELLOW_E
            Expected: GT_OK.
        */

        profile = 1;
        cos.dropPrecedence = CPSS_DP_YELLOW_E;
        cos.userPriority   = 1;
        cos.trafficClass   = 1;
        cos.dscp           = 1;
        cos.exp            = 1;

        st = cpssDxChCosProfileEntrySet(dev, profile, &cos);
        if (0 == PRV_CPSS_DXCH3_FAMILY_CHECK_MAC(dev))
        {
            UTF_VERIFY_NOT_EQUAL2_PARAM_MAC(GT_OK, st, dev, profile);
        }
        else
        {
            UTF_VERIFY_EQUAL2_PARAM_MAC(GT_OK, st, dev, profile);
        }

        if (st == GT_OK)
        {
            st = cpssDxChCosProfileEntryGet(dev, profile, &cosGet);
            UTF_VERIFY_EQUAL2_STRING_MAC(GT_OK, st,
                           "cpssDxChCosProfileEntryGet: %d, %d", dev, profile);

            /* Checking cos == cosGet */
            UTF_VERIFY_EQUAL1_STRING_MAC(cos.dropPrecedence, cosGet.dropPrecedence,
                                         "get another cosPtr->dropPrecedence than was set: %d", dev);
            UTF_VERIFY_EQUAL1_STRING_MAC(cos.userPriority, cosGet.userPriority,
                           "get another cosPtr->userPriority than was set: %d", dev);
            UTF_VERIFY_EQUAL1_STRING_MAC(cos.trafficClass, cosGet.trafficClass,
                           "get another cosPtr->trafficClass than was set: %d", dev);
            UTF_VERIFY_EQUAL1_STRING_MAC(cos.dscp, cosGet.dscp,
                           "get another cosPtr->dscp than was set: %d", dev);
            UTF_VERIFY_EQUAL1_STRING_MAC(cos.exp, cosGet.exp,
                           "get another cosPtr->exp than was set: %d", dev);
        }
    }

    profile = 1;
    cos.dropPrecedence = CPSS_DP_GREEN_E;
    cos.userPriority   = 1;
    cos.trafficClass   = 1;
    cos.dscp           = 1;
    cos.exp            = 1;

    /* 2. For not-active devices and devices from non-applicable family */
    /* check that function returns GT_BAD_PARAM.                        */

    /* prepare device iterator */
    PRV_TGF_NOT_APPLIC_DEV_RESET_MAC(&dev, UTF_NONE_FAMILY_E);
    /* go over all non active devices */
    while(GT_OK == prvUtfNextDeviceGet(&dev, GT_FALSE))
    {
        st = cpssDxChCosProfileEntrySet(dev, profile, &cos);
        UTF_VERIFY_EQUAL1_PARAM_MAC(GT_NOT_APPLICABLE_DEVICE, st, dev);
    }

    /* 3. Call function with out of bound value for device id.*/

    dev = PRV_CPSS_MAX_PP_DEVICES_CNS;

    st = cpssDxChCosProfileEntrySet(dev, profile, &cos);
    UTF_VERIFY_EQUAL1_PARAM_MAC(GT_BAD_PARAM, st, dev);
}

/*----------------------------------------------------------------------------*/
/*
GT_STATUS cpssDxChCosProfileEntryGet
(
     IN  GT_U8                      dev,
     IN  GT_U8                      profileIndex,
     OUT CPSS_DXCH_COS_PROFILE_STC  *cosPtr
)
*/
UTF_TEST_CASE_MAC(cpssDxChCosProfileEntryGet)
{
/*
    ITERATE_DEVICES (DxChx)
    1.1. Call function with profileIndex [1],
                            and non-null cosPtr.
    Expected: GT_OK.
    1.2. Call function with out of range profileIndex [PRV_CPSS_DXCH_QOS_PROFILE_NUM_MAX_CNS = 72],
                            and non-null cosPtr.
    Expected: NOT GT_OK for Cheetah and GT_OK for Cheetah-2,3.
    1.3. Call function with out of range profileIndex [PRV_CPSS_DXCH2_QOS_PROFILE_NUM_MAX_CNS = 128]
                            and valid other parameters.
    Expected: NON GT_OK.
    1.4. Call function with profileIndex [3],
                            and null cosPtr[NULL].
    Expected: GT_BAD_PTR.
*/
    GT_STATUS                 st      = GT_OK;

    GT_U8                     dev;
    GT_U32                    profile = 0;
    CPSS_DXCH_COS_PROFILE_STC cos;
    CPSS_PP_FAMILY_TYPE_ENT   devFamily;

    /* prepare device iterator */
    PRV_TGF_NOT_APPLIC_DEV_RESET_MAC(&dev, UTF_NONE_FAMILY_E);

    /* 1. Go over all active devices. */
    while (GT_OK == prvUtfNextDeviceGet(&dev, GT_TRUE))
    {
        /*
            1.1. Call function with profileIndex [1],
                            and non-null cosPtr.
            Expected: GT_OK.
        */
        profile = 1;

        st = cpssDxChCosProfileEntryGet(dev, profile, &cos);
        UTF_VERIFY_EQUAL2_PARAM_MAC(GT_OK, st, dev, profile);

        /* Getting device family */
        st = prvUtfDeviceFamilyGet(dev, &devFamily);
        UTF_VERIFY_EQUAL1_STRING_MAC(GT_OK, st, "prvUtfDeviceFamilyGet: %d", dev);

        /*
            1.2. Call function with out of range profileIndex [PRV_CPSS_DXCH_QOS_PROFILE_NUM_MAX_CNS = 72],
                                    and non-null cosPtr.
            Expected: NOT GT_OK for Cheetah and GT_OK for Cheetah-2.
        */
        profile = PRV_CPSS_DXCH_QOS_PROFILE_NUM_MAX_CNS;

        st = cpssDxChCosProfileEntryGet(dev, profile, &cos);
        if (devFamily != CPSS_PP_FAMILY_CHEETAH_E)
        {
            UTF_VERIFY_EQUAL2_PARAM_MAC(GT_OK, st, dev, profile);
        }
        else
        {
            UTF_VERIFY_NOT_EQUAL2_PARAM_MAC(GT_OK, st, dev, profile);
        }

        /*
            1.3. Call function with out of range profileIndex [PRV_CPSS_DXCH2_QOS_PROFILE_NUM_MAX_CNS = 128]
                                   and valid other parameters.
            Expected: NON GT_OK.
        */
        profile = PRV_CPSS_DXCH2_QOS_PROFILE_NUM_MAX_CNS;

        st = cpssDxChCosProfileEntryGet(dev, profile, &cos);
        UTF_VERIFY_NOT_EQUAL2_PARAM_MAC(GT_OK, st, dev, profile);

        /*
            1.4. Call function with profileIndex [3],
                                    and null cosPtr[NULL].
            Expected: GT_BAD_PTR.
        */
        profile = 3;

        st = cpssDxChCosProfileEntryGet(dev, profile, NULL);
        UTF_VERIFY_EQUAL1_STRING_MAC(GT_BAD_PTR, st, "%d, cosPtr = NULL", dev);
    }

    profile = 1;

    /* 2. For not-active devices and devices from non-applicable family */
    /* check that function returns GT_BAD_PARAM.                        */

    /* prepare device iterator */
    PRV_TGF_NOT_APPLIC_DEV_RESET_MAC(&dev, UTF_NONE_FAMILY_E);

    /* go over all non active devices */
    while(GT_OK == prvUtfNextDeviceGet(&dev, GT_FALSE))
    {
        st = cpssDxChCosProfileEntryGet(dev, profile, &cos);
        UTF_VERIFY_EQUAL1_PARAM_MAC(GT_NOT_APPLICABLE_DEVICE, st, dev);
    }

    /* 3. Call function with out of bound value for device id.*/

    dev = PRV_CPSS_MAX_PP_DEVICES_CNS;

    st = cpssDxChCosProfileEntryGet(dev, profile, &cos);
    UTF_VERIFY_EQUAL1_PARAM_MAC(GT_BAD_PARAM, st, dev);
}

/*----------------------------------------------------------------------------*/
/*
GT_STATUS cpssDxChCosDscpToProfileMapSet
(
    IN GT_U8    dev,
    IN GT_U8    dscp,
    IN GT_U8    profileIndex
)
*/
UTF_TEST_CASE_MAC(cpssDxChCosDscpToProfileMapSet)
{
/*
    ITERATE_DEVICES (DxChx)
    1.1. Call function with dscp [32],
                            profileIndex [1].
    Expected: GT_OK.
    1.2. Call cpssDxChCosDscpToProfileMapGet with same dscp.
    Expected: GT_OK and the same profileIndex.
    1.3. Call function with out of range profileIndex [PRV_CPSS_DXCH_QOS_PROFILE_NUM_MAX_CNS = 72],
                            dscp [32].
    Expected: NOT GT_OK for Cheetah and GT_OK for Cheetah-2,3.
    1.4. Call function with out of range profileIndex [PRV_CPSS_DXCH2_QOS_PROFILE_NUM_MAX_CNS = 128]
                            and valid other parameters.
    Expected: NON GT_OK.
    1.5. Call function with profileIndex [2],
                            out of range dscp [CPSS_DSCP_RANGE_CNS =64].
    Expected: NOT GT_OK.
*/
    GT_STATUS               st      = GT_OK;

    GT_U8                   dev;
    GT_U8                   dscp    = 0;
    GT_U32                  profile = 0;
    GT_U32                  pfGet   = 0;
    CPSS_PP_FAMILY_TYPE_ENT devFamily;

    /* prepare device iterator */
    PRV_TGF_NOT_APPLIC_DEV_RESET_MAC(&dev, UTF_NONE_FAMILY_E);

    /* 1. Go over all active devices. */
    while (GT_OK == prvUtfNextDeviceGet(&dev, GT_TRUE))
    {
        /*
           1.1. Call function with dscp [32],
                                   profileIndex [1].
           Expected: GT_OK.
        */
        dscp = 32;
        profile = 1;

        st = cpssDxChCosDscpToProfileMapSet(dev, dscp, profile);
        UTF_VERIFY_EQUAL3_PARAM_MAC(GT_OK, st, dev, dscp, profile);

        /*
            1.2. Call cpssDxChCosDscpToProfileMapGet with same dscp.
            Expected: GT_OK and the same profileIndex.
        */
        st = cpssDxChCosDscpToProfileMapGet(dev, dscp, &pfGet);
        UTF_VERIFY_EQUAL2_STRING_MAC(GT_OK, st,
                       "cpssDxChCosDscpToProfileMapGet: %d, %d", dev, dscp);
        UTF_VERIFY_EQUAL2_STRING_MAC(profile, pfGet,
                       "get another profile than was set: %d, %d", dev, dscp);

        /* Getting device family */
        st = prvUtfDeviceFamilyGet(dev, &devFamily);
        UTF_VERIFY_EQUAL1_STRING_MAC(GT_OK, st, "prvUtfDeviceFamilyGet: %d", dev);

        /*
            1.3. Call function with out of range profileIndex [PRV_CPSS_DXCH_QOS_PROFILE_NUM_MAX_CNS = 72],
                                    dscp [32].
            Expected: NOT GT_OK for Cheetah and GT_OK for Cheetah-2,3.
        */
        dscp    = 32;
        profile = PRV_CPSS_DXCH_QOS_PROFILE_NUM_MAX_CNS;

        st = cpssDxChCosDscpToProfileMapSet(dev, dscp, profile);
        if (devFamily != CPSS_PP_FAMILY_CHEETAH_E)
        {
            UTF_VERIFY_EQUAL3_PARAM_MAC(GT_OK, st, dev, dscp, profile);
        }
        else
        {
            UTF_VERIFY_NOT_EQUAL2_STRING_MAC(GT_OK, st, "%d, profileIndex = %d",
                                             dev, profile);
        }

        /*
            1.4. Call function with out of range profileIndex [PRV_CPSS_DXCH2_QOS_PROFILE_NUM_MAX_CNS = 128]
                                    and valid other parameters.
            Expected: NON GT_OK.
        */
        profile = PRV_CPSS_DXCH2_QOS_PROFILE_NUM_MAX_CNS;

        st = cpssDxChCosDscpToProfileMapSet(dev, dscp, profile);
        UTF_VERIFY_NOT_EQUAL2_STRING_MAC(GT_OK, st, "%d, profileIndex = %d",
                                         dev, profile);

        /*
            1.5. Call function with profileIndex [2],
                                    out of range dscp [CPSS_DSCP_RANGE_CNS =64].
            Expected: NOT GT_OK.
        */
        dscp = CPSS_DSCP_RANGE_CNS;
        profile = 2;

        st = cpssDxChCosDscpToProfileMapSet(dev, dscp, profile);
        UTF_VERIFY_NOT_EQUAL2_PARAM_MAC(GT_OK, st, dev, dscp);
    }

    dscp = 32;
    profile = 1;

    /* 2. For not-active devices and devices from non-applicable family */
    /* check that function returns GT_BAD_PARAM.                        */

    /* prepare device iterator */
    PRV_TGF_NOT_APPLIC_DEV_RESET_MAC(&dev, UTF_NONE_FAMILY_E);

    /* go over all non active devices */
    while(GT_OK == prvUtfNextDeviceGet(&dev, GT_FALSE))
    {
        st = cpssDxChCosDscpToProfileMapSet(dev, dscp, profile);
        UTF_VERIFY_EQUAL1_PARAM_MAC(GT_NOT_APPLICABLE_DEVICE, st, dev);
    }

    /* 3. Call function with out of bound value for device id.*/

    dev = PRV_CPSS_MAX_PP_DEVICES_CNS;

    st = cpssDxChCosDscpToProfileMapSet(dev, dscp, profile);
    UTF_VERIFY_EQUAL1_PARAM_MAC(GT_BAD_PARAM, st, dev);
}

/*----------------------------------------------------------------------------*/
/*
GT_STATUS cpssDxChCosDscpToProfileMapGet
(
    IN GT_U8    dev,
    IN GT_U8    dscp,
    IN GT_U8   *profileIndexPtr
)
*/
UTF_TEST_CASE_MAC(cpssDxChCosDscpToProfileMapGet)
{
/*
    ITERATE_DEVICES (DxChx)
    1.1. Call function with dscp [24],
                            non-null profileIndexPtr.
    Expected: GT_OK.
    1.2. Call function with out of range dscp [CPSS_DSCP_RANGE_CNS=64]
                            and non-null profileIndexPtr.
    Expected: NOT GT_OK.
    1.3. Call function with dscp [12],
                            null profileIndexPtr [NULL].
    Expected: GT_BAD_PTR.
*/
    GT_STATUS    st      = GT_OK;

    GT_U8        dev;
    GT_U8        dscp    = 0;
    GT_U32       profile = 0;

    /* prepare device iterator */
    PRV_TGF_NOT_APPLIC_DEV_RESET_MAC(&dev, UTF_NONE_FAMILY_E);

    /* 1. Go over all active devices. */
    while (GT_OK == prvUtfNextDeviceGet(&dev, GT_TRUE))
    {
        /*
            1.1. Call function with dscp [24],
                                    non-null profileIndexPtr.
            Expected: GT_OK.
        */
        dscp = 24;

        st = cpssDxChCosDscpToProfileMapGet(dev, dscp, &profile);
        UTF_VERIFY_EQUAL2_PARAM_MAC(GT_OK, st, dev, dscp);

        /*
            1.2. Call function with out of range dscp [CPSS_DSCP_RANGE_CNS=64]
                                    and non-null profileIndexPtr.
            Expected: NOT GT_OK.
        */
        dscp = CPSS_DSCP_RANGE_CNS;

        st = cpssDxChCosDscpToProfileMapGet(dev, dscp, &profile);
        UTF_VERIFY_NOT_EQUAL2_PARAM_MAC(GT_OK, st, dev, dscp);

        /*
            1.3. Call function with dscp [12],
                                    null profileIndexPtr [NULL].
            Expected: GT_BAD_PTR.
        */
        dscp = 12;

        st = cpssDxChCosDscpToProfileMapGet(dev, dscp, NULL);
        UTF_VERIFY_EQUAL1_STRING_MAC(GT_BAD_PTR, st,
                                     "%d, profileIndexPtr = NULL", dev);
    }

    dscp = 24;

    /* 2. For not-active devices and devices from non-applicable family */
    /* check that function returns GT_BAD_PARAM.                        */

    /* prepare device iterator */
    PRV_TGF_NOT_APPLIC_DEV_RESET_MAC(&dev, UTF_NONE_FAMILY_E);

    /* go over all non active devices */
    while(GT_OK == prvUtfNextDeviceGet(&dev, GT_FALSE))
    {
        st = cpssDxChCosDscpToProfileMapGet(dev, dscp, &profile);
        UTF_VERIFY_EQUAL1_PARAM_MAC(GT_NOT_APPLICABLE_DEVICE, st, dev);
    }

    /* 3. Call function with out of bound value for device id.*/

    dev = PRV_CPSS_MAX_PP_DEVICES_CNS;

    st = cpssDxChCosDscpToProfileMapGet(dev, dscp, &profile);
    UTF_VERIFY_EQUAL1_PARAM_MAC(GT_BAD_PARAM, st, dev);
}

/*----------------------------------------------------------------------------*/
/*
GT_STATUS cpssDxChCosDscpMutationEntrySet
(
    IN GT_U8   dev,
    IN GT_U8   dscp,
    IN GT_U8   newDscp
)
*/
UTF_TEST_CASE_MAC(cpssDxChCosDscpMutationEntrySet)
{
/*
    ITERATE_DEVICES (DxChx)
    1.1. Call function with dscp [32],
                            newDscp [1].
    Expected: GT_OK.
    1.2. Call cpssDxChCosDscpMutationEntryGet with same dscp.
    Expected: GT_OK and the same newDscpPtr.
    1.3. Call function with out of range dscp [CPSS_DSCP_RANGE_CNS=64],
                            newDscp [2].
    Expected: NOT GT_OK.
    1.4. Call function with dscp [50],
                            out of range newDscp [CPSS_DSCP_RANGE_CNS=64].
    Expected: NOT GT_OK.
*/
    GT_STATUS st      = GT_OK;

    GT_U8     dev;
    GT_U8     dscp    = 0;
    GT_U8     newDscp = 0;
    GT_U8     dscpGet = 0;

    /* prepare device iterator */
    PRV_TGF_NOT_APPLIC_DEV_RESET_MAC(&dev, UTF_NONE_FAMILY_E);

    /* 1. Go over all active devices. */
    while (GT_OK == prvUtfNextDeviceGet(&dev, GT_TRUE))
    {
        /*
           1.1. Call function with dscp [32],
                                   newDscp [1].
           Expected: GT_OK.
        */
        dscp = 32;
        newDscp = 1;

        st = cpssDxChCosDscpMutationEntrySet(dev, dscp, newDscp);
        UTF_VERIFY_EQUAL3_PARAM_MAC(GT_OK, st, dev, dscp, newDscp);

        /*
            1.2. Call cpssDxChCosDscpMutationEntryGet with same dscp.
            Expected: GT_OK and the same newDscpPtr.
        */
        st = cpssDxChCosDscpMutationEntryGet(dev, dscp, &dscpGet);
        UTF_VERIFY_EQUAL2_STRING_MAC(GT_OK, st,
                       "cpssDxChCosDscpMutationEntryGet: %d, %d", dev, dscp);
        UTF_VERIFY_EQUAL2_STRING_MAC(newDscp, dscpGet,
                       "get another newDscp than was set: %d, %d", dev, dscp);

        /*
            1.3. Call function with out of range dscp [CPSS_DSCP_RANGE_CNS=64],
                                    newDscp [2].
            Expected: NOT GT_OK.
        */
        dscp = CPSS_DSCP_RANGE_CNS;
        newDscp = 2;

        st = cpssDxChCosDscpMutationEntrySet(dev, dscp, newDscp);
        UTF_VERIFY_NOT_EQUAL2_PARAM_MAC(GT_OK, st, dev, dscp);

        /*
            1.4. Call function with dscp [50],
                                    out of range newDscp [CPSS_DSCP_RANGE_CNS=64].
            Expected: NOT GT_OK.
        */
        dscp = 50;
        newDscp = CPSS_DSCP_RANGE_CNS;

        st = cpssDxChCosDscpMutationEntrySet(dev, dscp, newDscp);
        UTF_VERIFY_NOT_EQUAL2_STRING_MAC(GT_OK, st, "%d, newDscp = %d",
                                        dev, newDscp);
    }

    dscp = 32;
    newDscp = 1;

    /* 2. For not-active devices and devices from non-applicable family */
    /* check that function returns GT_BAD_PARAM.                        */

    /* prepare device iterator */
    PRV_TGF_NOT_APPLIC_DEV_RESET_MAC(&dev, UTF_NONE_FAMILY_E);

    /* go over all non active devices */
    while(GT_OK == prvUtfNextDeviceGet(&dev, GT_FALSE))
    {
        st = cpssDxChCosDscpMutationEntrySet(dev, dscp, newDscp);
        UTF_VERIFY_EQUAL1_PARAM_MAC(GT_NOT_APPLICABLE_DEVICE, st, dev);
    }

    /* 3. Call function with out of bound value for device id.*/

    dev = PRV_CPSS_MAX_PP_DEVICES_CNS;

    st = cpssDxChCosDscpMutationEntrySet(dev, dscp, newDscp);
    UTF_VERIFY_EQUAL1_PARAM_MAC(GT_BAD_PARAM, st, dev);
}

/*----------------------------------------------------------------------------*/
/*
GT_STATUS cpssDxChCosDscpMutationEntryGet
(
    IN GT_U8   dev,
    IN GT_U8   dscp,
    IN GT_U8  *newDscpPtr
)
*/
UTF_TEST_CASE_MAC(cpssDxChCosDscpMutationEntryGet)
{
/*
    ITERATE_DEVICES (DxChx)
    1.1. Call function with dscp [24], non-null newDscpPtr.
    Expected: GT_OK.
    1.2. Call function with out of range dscp [CPSS_DSCP_RANGE_CNS =64]
                            and non-null newDscpPtr.
    Expected: NOT GT_OK.
    1.3. Call function with dscp [12], null newDscpPtr [NULL].
    Expected: GT_BAD_PTR.
*/
    GT_STATUS    st      = GT_OK;

    GT_U8        dev;
    GT_U8        dscp    = 0;
    GT_U8        newDscp = 0;

    /* prepare device iterator */
    PRV_TGF_NOT_APPLIC_DEV_RESET_MAC(&dev, UTF_NONE_FAMILY_E);

    /* 1. Go over all active devices. */
    while (GT_OK == prvUtfNextDeviceGet(&dev, GT_TRUE))
    {
        /*
            1.1. Call function with dscp [24],
                                    non-null newDscpPtr.
            Expected: GT_OK.
        */
        dscp = 24;

        st = cpssDxChCosDscpMutationEntryGet(dev, dscp, &newDscp);
        UTF_VERIFY_EQUAL2_PARAM_MAC(GT_OK, st, dev, dscp);

        /*
            1.2. Call function with out of range dscp [CPSS_DSCP_RANGE_CNS=64]
                                    and non-null newDscpPtr.
            Expected: NOT GT_OK.
        */
        dscp = CPSS_DSCP_RANGE_CNS;

        st = cpssDxChCosDscpMutationEntryGet(dev, dscp, &newDscp);
        UTF_VERIFY_NOT_EQUAL2_PARAM_MAC(GT_OK, st, dev, dscp);

        /*
            1.3. Call function with dscp [12],
                                    null newDscpPtr [NULL].
            Expected: GT_BAD_PTR.
        */
        dscp = 12;

        st = cpssDxChCosDscpToProfileMapGet(dev, dscp, NULL);
        UTF_VERIFY_EQUAL1_STRING_MAC(GT_BAD_PTR, st, "%d, newDscpPtr = NULL", dev);
    }

    dscp = 24;

    /* 2. For not-active devices and devices from non-applicable family */
    /* check that function returns GT_BAD_PARAM.                        */

    /* prepare device iterator */
    PRV_TGF_NOT_APPLIC_DEV_RESET_MAC(&dev, UTF_NONE_FAMILY_E);

    /* go over all non active devices */
    while(GT_OK == prvUtfNextDeviceGet(&dev, GT_FALSE))
    {
        st = cpssDxChCosDscpMutationEntryGet(dev, dscp, &newDscp);
        UTF_VERIFY_EQUAL1_PARAM_MAC(GT_NOT_APPLICABLE_DEVICE, st, dev);
    }

    /* 3. Call function with out of bound value for device id.*/

    dev = PRV_CPSS_MAX_PP_DEVICES_CNS;

    st = cpssDxChCosDscpMutationEntryGet(dev, dscp, &newDscp);
    UTF_VERIFY_EQUAL1_PARAM_MAC(GT_BAD_PARAM, st, dev);
}

/*----------------------------------------------------------------------------*/
/*
cpssDxChCosUpCfiDeiToProfileMapSet
(
    IN GT_U8    dev,
    IN GT_U32   upProfileIndex,
    IN GT_U8    up,
    IN GT_U8    cfiDeiBit,
    IN GT_U8    profileIndex
)
*/
UTF_TEST_CASE_MAC(cpssDxChCosUpCfiDeiToProfileMapSet)
{
/*
    ITERATE_DEVICES (DxCh3 and above)
    1.1. Call with up [0 / 7],
                   cfiDeiBit [0 / 1]
                   and profileIndex [0 / 127].
    Expected: GT_OK.
    1.2. Call cpssDxChCosUpCfiDeiToProfileMapGet with same up, cfiDeiBit and non-NULL profileIndexPtr.
    Expected: GT_OK and the same profileIndex.
    1.3. Call with out of range up [CPSS_USER_PRIORITY_RANGE_CNS = 8]
                   and other parameters from 1.1.
    Expected: NOT GT_OK.
    1.4. Call with out of range cfiDeiBit [2]
                   and other parameters from 1.1.
    Expected: NOT GT_OK.
    1.5. Call with out of range profileIndex [128]
                   and other parameters from 1.1.
    Expected: NOT GT_OK.
*/
    GT_STATUS   st = GT_OK;
    GT_U8       dev;

    GT_U8    up              = 0;
    GT_U8    cfiDeiBit       = 0;
    GT_U32   profileIndex    = 0;
    GT_U32   profileIndexGet = 0;
    GT_U32   upProfileIndex;
    CPSS_PP_FAMILY_TYPE_ENT   devFamily;


    /* prepare device iterator */
    PRV_TGF_NOT_APPLIC_DEV_RESET_MAC(&dev, UTF_NONE_FAMILY_E);

    /* 1. Go over all active devices. */
    while(GT_OK == prvUtfNextDeviceGet(&dev, GT_TRUE))
    {
        /* Getting device family */
        st = prvUtfDeviceFamilyGet(dev, &devFamily);
        UTF_VERIFY_EQUAL1_STRING_MAC(GT_OK, st, "prvUtfDeviceFamilyGet: %d", dev);

        for(upProfileIndex = 0 ; upProfileIndex < 2 ; upProfileIndex++)
        {
            /*
                1.1. Call with up [0 / 7],
                               cfiDeiBit [0 / 1]
                               and profileIndex [0 / 127].
                Expected: GT_OK.
            */

            /* Call with up [0] */
            up           = 0;
            cfiDeiBit    = 0;
            profileIndex = 0;

            st = cpssDxChCosUpCfiDeiToProfileMapSet(dev, upProfileIndex, up, cfiDeiBit, profileIndex);
            UTF_VERIFY_EQUAL5_PARAM_MAC(GT_OK, st, dev, upProfileIndex, up, cfiDeiBit, profileIndex);

            /*
                1.2. Call cpssDxChCosUpCfiDeiToProfileMapGet with same up, cfiDeiBit and non-NULL profileIndexPtr.
                Expected: GT_OK and the same profileIndex.
            */
            st = cpssDxChCosUpCfiDeiToProfileMapGet(dev, upProfileIndex, up, cfiDeiBit, &profileIndexGet);
            UTF_VERIFY_EQUAL4_STRING_MAC(GT_OK, st,
                       "cpssDxChCosUpCfiDeiToProfileMapGet: %d, %d, %d, %d", dev, upProfileIndex, up, cfiDeiBit);

            /* Verifying values */
            UTF_VERIFY_EQUAL1_STRING_MAC(profileIndex, profileIndexGet,
                       "get another profileIndex than was set: %d", dev);

            /* Call with up [7] */
            up           = 7;
            cfiDeiBit    = 1;

            profileIndex = PRV_CPSS_DXCH_QOS_PROFILE_MAX_MAC(dev) - 1;/*127 ch2 and above or 71 ch1*/

            st = cpssDxChCosUpCfiDeiToProfileMapSet(dev, upProfileIndex, up, cfiDeiBit, profileIndex);
            UTF_VERIFY_EQUAL5_PARAM_MAC(GT_OK, st, dev, upProfileIndex, up, cfiDeiBit, profileIndex);

            /*
                1.2. Call cpssDxChCosUpCfiDeiToProfileMapGet with same up, cfiDeiBit and non-NULL profileIndexPtr.
                Expected: GT_OK and the same profileIndex.
            */
            st = cpssDxChCosUpCfiDeiToProfileMapGet(dev, upProfileIndex, up, cfiDeiBit, &profileIndexGet);
            UTF_VERIFY_EQUAL4_STRING_MAC(GT_OK, st,
                       "cpssDxChCosUpCfiDeiToProfileMapGet: %d, %d, %d , %d", dev, upProfileIndex, up, cfiDeiBit);

            /* Verifying values */
            UTF_VERIFY_EQUAL1_STRING_MAC(profileIndex, profileIndexGet,
                       "get another profileIndex than was set: %d", dev);

            /*
                1.3. Call with out of range up [CPSS_USER_PRIORITY_RANGE_CNS = 8]
                               and other parameters from 1.1.
                Expected: NOT GT_OK.
            */
            up = CPSS_USER_PRIORITY_RANGE_CNS;

            st = cpssDxChCosUpCfiDeiToProfileMapSet(dev, upProfileIndex, up, cfiDeiBit, profileIndex);
            UTF_VERIFY_NOT_EQUAL3_PARAM_MAC(GT_OK, st, dev, upProfileIndex, up);

            up = 0;

            /*
                1.4. Call with out of range cfiDeiBit [2]
                               and other parameters from 1.1.
                Expected: NOT GT_OK.
            */
            cfiDeiBit = 2;

            st = cpssDxChCosUpCfiDeiToProfileMapSet(dev, upProfileIndex, up, cfiDeiBit, profileIndex);
            if(devFamily >= CPSS_PP_FAMILY_CHEETAH3_E)
            {
                UTF_VERIFY_EQUAL2_STRING_MAC(GT_BAD_PARAM, st, "%d, cfiDeiBit = %d", dev, cfiDeiBit);
            }
            else
            {
                /* the parameter cfiDeiBit need to be ignored */
                UTF_VERIFY_EQUAL5_PARAM_MAC(GT_OK, st, dev, upProfileIndex, up, cfiDeiBit, profileIndex);
            }

            cfiDeiBit = 0;

            /*
                1.5. Call with out of range profileIndex [128]
                               and other parameters from 1.1.
                Expected: NOT GT_OK.
            */
            profileIndex = PRV_CPSS_DXCH_QOS_PROFILE_MAX_MAC(dev);/*128 ch2 and above or 72 ch1*/

            st = cpssDxChCosUpCfiDeiToProfileMapSet(dev, upProfileIndex, up, cfiDeiBit, profileIndex);
            UTF_VERIFY_NOT_EQUAL2_STRING_MAC(GT_OK, st, "%d, profileIndex = %d", dev, profileIndex);
        }/* for upProfileIndex */


        up           = 0;
        cfiDeiBit    = 0;
        profileIndex = 0;
        upProfileIndex = 2; /* bad parameter */

        st = cpssDxChCosUpCfiDeiToProfileMapSet(dev, upProfileIndex, up, cfiDeiBit, profileIndex);
        if (PRV_CPSS_DXCH_LION_FAMILY_CHECK_MAC(dev))
        {
            UTF_VERIFY_EQUAL5_PARAM_MAC(GT_BAD_PARAM, st, dev, upProfileIndex, up, cfiDeiBit, profileIndex);
        }
        else
        {
            /* the parameter upProfileIndex need to be ignored */
            UTF_VERIFY_EQUAL5_PARAM_MAC(GT_OK, st, dev, upProfileIndex, up, cfiDeiBit, profileIndex);
        }

    }/*while*/

    up           = 0;
    cfiDeiBit    = 0;
    profileIndex = 0;
    upProfileIndex = 0;

    /* 2. For not-active devices and devices from non-applicable family */
    /* check that function returns GT_BAD_PARAM.                        */

    /* prepare device iterator */
    PRV_TGF_NOT_APPLIC_DEV_RESET_MAC(&dev, UTF_NONE_FAMILY_E);

    /* go over all non active devices */
    while(GT_OK == prvUtfNextDeviceGet(&dev, GT_FALSE))
    {
        st = cpssDxChCosUpCfiDeiToProfileMapSet(dev, upProfileIndex, up, cfiDeiBit, profileIndex);
        UTF_VERIFY_EQUAL1_PARAM_MAC(GT_NOT_APPLICABLE_DEVICE, st, dev);
    }

    /* 3. Call function with out of bound value for device id.*/
    dev = PRV_CPSS_MAX_PP_DEVICES_CNS;

    st = cpssDxChCosUpCfiDeiToProfileMapSet(dev, upProfileIndex, up, cfiDeiBit, profileIndex);
    UTF_VERIFY_EQUAL1_PARAM_MAC(GT_BAD_PARAM, st, dev);
}

/*----------------------------------------------------------------------------*/
/*
cpssDxChCosUpCfiDeiToProfileMapGet
(
    IN  GT_U8   dev,
    IN GT_U32   upProfileIndex,
    IN  GT_U8   up,
    IN  GT_U8   cfiDeiBit,
    OUT GT_U8   *profileIndexPtr
)
*/
UTF_TEST_CASE_MAC(cpssDxChCosUpCfiDeiToProfileMapGet)
{
/*
    ITERATE_DEVICES (DxCh3 and above)
    1.1. Call with up [0 / 7],
                   cfiDeiBit [0 / 1]
                   and non-NULL profileIndexPtr.
    Expected: GT_OK.
    1.2. Call with out of range up [CPSS_USER_PRIORITY_RANGE_CNS= 8]
                   and other parameters from 1.1.
    Expected: NOT GT_OK.
    1.3. Call with out of range cfiDeiBit [2]
                   and other parameters from 1.1.
    Expected: NOT GT_OK.
    1.4. Call with profileIndexPtr [NULL]
                   and other parameters from 1.1.
    Expected: GT_BAD_PTR.
*/
    GT_STATUS   st = GT_OK;
    GT_U8       dev;

    GT_U8    up           = 0;
    GT_U8    cfiDeiBit    = 0;
    GT_U32   profileIndex = 0;
    GT_U32   upProfileIndex  ;
    CPSS_PP_FAMILY_TYPE_ENT   devFamily;


    /* prepare device iterator */
    PRV_TGF_NOT_APPLIC_DEV_RESET_MAC(&dev, UTF_NONE_FAMILY_E);

    /* 1. Go over all active devices. */
    while(GT_OK == prvUtfNextDeviceGet(&dev, GT_TRUE))
    {
        /* Getting device family */
        st = prvUtfDeviceFamilyGet(dev, &devFamily);
        UTF_VERIFY_EQUAL1_STRING_MAC(GT_OK, st, "prvUtfDeviceFamilyGet: %d", dev);

        for(upProfileIndex = 0 ; upProfileIndex < 2 ; upProfileIndex++)
        {
            /*
                1.1. Call with up [0 / 7],
                               cfiDeiBit [0 / 1]
                               and non-NULL profileIndexPtr.
                Expected: GT_OK.
            */

            /* Call with up [0] */
            up        = 0;
            cfiDeiBit = 0;

            st = cpssDxChCosUpCfiDeiToProfileMapGet(dev, upProfileIndex, up, cfiDeiBit, &profileIndex);
            UTF_VERIFY_EQUAL4_PARAM_MAC(GT_OK, st, dev, upProfileIndex, up, cfiDeiBit);

            /* Call with up [7] */
            up        = 7;
            cfiDeiBit = 1;

            st = cpssDxChCosUpCfiDeiToProfileMapGet(dev, upProfileIndex, up, cfiDeiBit, &profileIndex);
            UTF_VERIFY_EQUAL4_PARAM_MAC(GT_OK, st, dev, upProfileIndex, up, cfiDeiBit);

            /*
                1.2. Call with out of range up [CPSS_USER_PRIORITY_RANGE_CNS = 8]
                               and other parameters from 1.1.
                Expected: NOT GT_OK.
            */
            up = CPSS_USER_PRIORITY_RANGE_CNS;

            st = cpssDxChCosUpCfiDeiToProfileMapGet(dev, upProfileIndex, up, cfiDeiBit, &profileIndex);
            UTF_VERIFY_NOT_EQUAL3_PARAM_MAC(GT_OK, st, dev, upProfileIndex, up);

            up = 0;

            /*
                1.3. Call with out of range cfiDeiBit [2]
                               and other parameters from 1.1.
                Expected: NOT GT_OK.
            */
            cfiDeiBit = 2;

            st = cpssDxChCosUpCfiDeiToProfileMapGet(dev, upProfileIndex, up, cfiDeiBit, &profileIndex);
            if(devFamily >= CPSS_PP_FAMILY_CHEETAH3_E)
            {
                UTF_VERIFY_NOT_EQUAL2_STRING_MAC(GT_OK, st, "%d, cfiDeiBit = %d", dev, cfiDeiBit);
            }
            else
            {
                /* the parameter cfiDeiBit need to be ignored */
                UTF_VERIFY_EQUAL4_PARAM_MAC(GT_OK, st, dev, upProfileIndex, up, cfiDeiBit);
            }

            cfiDeiBit = 0;

            /*
                1.4. Call with profileIndexPtr [NULL]
                               and other parameters from 1.1.
                Expected: GT_BAD_PTR.
            */
            st = cpssDxChCosUpCfiDeiToProfileMapGet(dev, upProfileIndex, up, cfiDeiBit, NULL);
            UTF_VERIFY_EQUAL1_STRING_MAC(GT_BAD_PTR, st, "%d, profileIndexPtr = NULL", dev);
        }/* for upProfileIndex */


        up           = 0;
        cfiDeiBit    = 0;
        upProfileIndex = 2; /* bad parameter */

        st = cpssDxChCosUpCfiDeiToProfileMapGet(dev, upProfileIndex, up, cfiDeiBit, &profileIndex);
        if (PRV_CPSS_DXCH_LION_FAMILY_CHECK_MAC(dev))
        {
            UTF_VERIFY_EQUAL4_PARAM_MAC(GT_BAD_PARAM, st, dev, upProfileIndex, up, cfiDeiBit);
        }
        else
        {
            /* the parameter upProfileIndex need to be ignored */
            UTF_VERIFY_EQUAL4_PARAM_MAC(GT_OK, st, dev, upProfileIndex, up, cfiDeiBit);
        }

    }/*while*/

    up        = 0;
    cfiDeiBit = 0;
    upProfileIndex = 0;

    /* 2. For not-active devices and devices from non-applicable family */
    /* check that function returns GT_BAD_PARAM.                        */

    /* prepare device iterator */
    PRV_TGF_NOT_APPLIC_DEV_RESET_MAC(&dev, UTF_NONE_FAMILY_E);

    /* go over all non active devices */
    while(GT_OK == prvUtfNextDeviceGet(&dev, GT_FALSE))
    {
        st = cpssDxChCosUpCfiDeiToProfileMapGet(dev, upProfileIndex, up, cfiDeiBit, &profileIndex);
        UTF_VERIFY_EQUAL1_PARAM_MAC(GT_NOT_APPLICABLE_DEVICE, st, dev);
    }

    /* 3. Call function with out of bound value for device id.*/
    dev = PRV_CPSS_MAX_PP_DEVICES_CNS;

    st = cpssDxChCosUpCfiDeiToProfileMapGet(dev, upProfileIndex, up, cfiDeiBit, &profileIndex);
    UTF_VERIFY_EQUAL1_PARAM_MAC(GT_BAD_PARAM, st, dev);
}

/*----------------------------------------------------------------------------*/
/*
cpssDxChCosPortDpToCfiDeiMapEnableSet
(
    IN GT_U8    dev,
    IN GT_U8    port,
    IN GT_BOOL  enable
)
*/
UTF_TEST_CASE_MAC(cpssDxChCosPortDpToCfiDeiMapEnableSet)
{
/*
    ITERATE_DEVICES_PHY_CPU_PORTS (DxCh3 and above)
    1.1.1. Call with enable [GT_FALSE / GT_TRUE].
    Expected: GT_OK.
    1.1.2. Call cpssDxChCosPortDpToCfiDeiMapEnableGet with non-null enablePtr.
    Expected: GT_OK and the same enable.
*/
    GT_STATUS   st   = GT_OK;
    GT_U8       dev;
    GT_U8       port = COS_VALID_PHY_PORT_CNS;

    GT_BOOL   enable    = GT_FALSE;
    GT_BOOL   enableGet = GT_FALSE;


    /* prepare iterator for go over all active devices */
    PRV_TGF_NOT_APPLIC_DEV_RESET_MAC(&dev, UTF_CH1_E | UTF_CH1_DIAMOND_E | UTF_CH2_E);

    /* 1. Go over all active devices. */
    while(GT_OK == prvUtfNextDeviceGet(&dev, GT_TRUE))
    {
        st = prvUtfNextPhyPortReset(&port, dev);
        UTF_VERIFY_EQUAL0_PARAM_MAC(GT_OK, st);

        /* 1.1. For all active devices go over all available physical ports. */
        while(GT_OK == prvUtfNextPhyPortGet(&port, GT_TRUE))
        {
            /*
                1.1.1. Call with enable [GT_FALSE and GT_TRUE].
                Expected: GT_OK.
            */

            /* Call with enable [GT_FALSE] */
            enable = GT_FALSE;

            st = cpssDxChCosPortDpToCfiDeiMapEnableSet(dev, port, enable);
            UTF_VERIFY_EQUAL3_PARAM_MAC(GT_OK, st, dev, port, enable);

            /*
                1.1.2. Call cpssDxChCosPortDpToCfiDeiMapEnableGet with non-null enablePtr.
                Expected: GT_OK and the same enable.
            */
            st = cpssDxChCosPortDpToCfiDeiMapEnableGet(dev, port, &enableGet);
            UTF_VERIFY_EQUAL2_STRING_MAC(GT_OK, st,
                       "cpssDxChCosPortDpToCfiDeiMapEnableGet: %d, %d", dev, port);

            /* Verifying values */
            UTF_VERIFY_EQUAL2_STRING_MAC(enable, enableGet,
                       "get another enable than was set: %d", dev, port);

            /* Call with enable [GT_TRUE] */
            enable = GT_TRUE;

            st = cpssDxChCosPortDpToCfiDeiMapEnableSet(dev, port, enable);
            UTF_VERIFY_EQUAL3_PARAM_MAC(GT_OK, st, dev, port, enable);

            /*
                1.1.2. Call cpssDxChCosPortDpToCfiDeiMapEnableGet with non-null enablePtr.
                Expected: GT_OK and the same enable.
            */
            st = cpssDxChCosPortDpToCfiDeiMapEnableGet(dev, port, &enableGet);
            UTF_VERIFY_EQUAL2_STRING_MAC(GT_OK, st,
                       "cpssDxChCosPortDpToCfiDeiMapEnableGet: %d, %d", dev, port);

            /* Verifying values */
            UTF_VERIFY_EQUAL2_STRING_MAC(enable, enableGet,
                       "get another enable than was set: %d", dev, port);
        }

        enable = GT_FALSE;

        st = prvUtfNextPhyPortReset(&port, dev);
        UTF_VERIFY_EQUAL0_PARAM_MAC(GT_OK, st);

        /* 1.2. For all active devices go over all non available physical ports. */
        while(GT_OK == prvUtfNextPhyPortGet(&port, GT_FALSE))
        {
            /* 1.2.1. Call function for each non-active port */
            st = cpssDxChCosPortDpToCfiDeiMapEnableSet(dev, port, enable);
            UTF_VERIFY_EQUAL2_PARAM_MAC(GT_BAD_PARAM, st, dev, port);
        }

        /* 1.3. For active device check that function returns GT_BAD_PARAM */
        /* for out of bound value for port number.                         */
        port = PRV_CPSS_MAX_PP_PORTS_NUM_CNS;

        st = cpssDxChCosPortDpToCfiDeiMapEnableSet(dev, port, enable);
        UTF_VERIFY_EQUAL2_PARAM_MAC(GT_BAD_PARAM, st, dev, port);

        /* 1.4. For active device check that function returns GT_OK        */
        /* for CPU port number.                                            */
        port = CPSS_CPU_PORT_NUM_CNS;

        st = cpssDxChCosPortDpToCfiDeiMapEnableSet(dev, port, enable);
        UTF_VERIFY_EQUAL2_PARAM_MAC(GT_OK, st, dev, port);
    }

    enable = GT_FALSE;

    /* 2. For not active devices check that function returns non GT_OK.*/
    port = COS_VALID_PHY_PORT_CNS;

    /* prepare device iterator */
    PRV_TGF_NOT_APPLIC_DEV_RESET_MAC(&dev, UTF_CH1_E | UTF_CH1_DIAMOND_E | UTF_CH2_E);

    /* Go over all non active devices. */
    while(GT_OK == prvUtfNextDeviceGet(&dev, GT_FALSE))
    {
        st = cpssDxChCosPortDpToCfiDeiMapEnableSet(dev, port, enable);
        UTF_VERIFY_EQUAL1_PARAM_MAC(GT_NOT_APPLICABLE_DEVICE, st, dev);
    }

    /* 3.Call function with out of bound value for device id */
    dev = PRV_CPSS_MAX_PP_DEVICES_CNS;

    st = cpssDxChCosPortDpToCfiDeiMapEnableSet(dev, port, enable);
    UTF_VERIFY_EQUAL1_PARAM_MAC(GT_BAD_PARAM, st, dev);
}

/*----------------------------------------------------------------------------*/
/*
cpssDxChCosPortDpToCfiDeiMapEnableGet
(
    IN  GT_U8   dev,
    IN  GT_U8   port,
    OUT GT_BOOL *enablePtr
)
*/
UTF_TEST_CASE_MAC(cpssDxChCosPortDpToCfiDeiMapEnableGet)
{
/*
    ITERATE_DEVICES_PHY_CPU_PORTS (DxCh3 and above)
    1.1.1. Call with non-NULL enablePtr.
    Expected: GT_OK.
    1.1.2. Call with enablePtr [NULL].
    Expected: GT_BAD_PTR.
*/
    GT_STATUS   st   = GT_OK;
    GT_U8       dev;
    GT_U8       port = COS_VALID_PHY_PORT_CNS;

    GT_BOOL   enable = GT_FALSE;


    /* prepare iterator for go over all active devices */
    PRV_TGF_NOT_APPLIC_DEV_RESET_MAC(&dev, UTF_CH1_E | UTF_CH1_DIAMOND_E | UTF_CH2_E);

    /* 1. Go over all active devices. */
    while(GT_OK == prvUtfNextDeviceGet(&dev, GT_TRUE))
    {
        st = prvUtfNextPhyPortReset(&port, dev);
        UTF_VERIFY_EQUAL0_PARAM_MAC(GT_OK, st);

        /* 1.1. For all active devices go over all available physical ports. */
        while(GT_OK == prvUtfNextPhyPortGet(&port, GT_TRUE))
        {
            /*
                1.1.1. Call with non-NULL enablePtr.
                Expected: GT_OK.
            */
            st = cpssDxChCosPortDpToCfiDeiMapEnableGet(dev, port, &enable);
            UTF_VERIFY_EQUAL2_PARAM_MAC(GT_OK, st, dev, port);

            /*
                1.1.2. Call with enablePtr [NULL].
                Expected: GT_BAD_PTR.
            */
            st = cpssDxChCosPortDpToCfiDeiMapEnableGet(dev, port, NULL);
            UTF_VERIFY_EQUAL2_STRING_MAC(GT_BAD_PTR, st, "%d, %d, enablePtr = NULL", dev, port);
        }

        st = prvUtfNextPhyPortReset(&port, dev);
        UTF_VERIFY_EQUAL0_PARAM_MAC(GT_OK, st);

        /* 1.2. For all active devices go over all non available physical ports. */
        while(GT_OK == prvUtfNextPhyPortGet(&port, GT_FALSE))
        {
            /* 1.2.1. Call function for each non-active port */
            st = cpssDxChCosPortDpToCfiDeiMapEnableGet(dev, port, &enable);
            UTF_VERIFY_EQUAL2_PARAM_MAC(GT_BAD_PARAM, st, dev, port);
        }

        /* 1.3. For active device check that function returns GT_BAD_PARAM */
        /* for out of bound value for port number.                         */
        port = PRV_CPSS_MAX_PP_PORTS_NUM_CNS;

        st = cpssDxChCosPortDpToCfiDeiMapEnableGet(dev, port, &enable);
        UTF_VERIFY_EQUAL2_PARAM_MAC(GT_BAD_PARAM, st, dev, port);

        /* 1.4. For active device check that function returns GT_OK        */
        /* for CPU port number.                                            */
        port = CPSS_CPU_PORT_NUM_CNS;

        st = cpssDxChCosPortDpToCfiDeiMapEnableGet(dev, port, &enable);
        UTF_VERIFY_EQUAL2_PARAM_MAC(GT_OK, st, dev, port);
    }

    /* 2. For not active devices check that function returns non GT_OK.*/
    port = COS_VALID_PHY_PORT_CNS;

    /* prepare device iterator */
    PRV_TGF_NOT_APPLIC_DEV_RESET_MAC(&dev, UTF_CH1_E | UTF_CH1_DIAMOND_E | UTF_CH2_E);

    /* Go over all non active devices. */
    while(GT_OK == prvUtfNextDeviceGet(&dev, GT_FALSE))
    {
        st = cpssDxChCosPortDpToCfiDeiMapEnableGet(dev, port, &enable);
        UTF_VERIFY_EQUAL1_PARAM_MAC(GT_NOT_APPLICABLE_DEVICE, st, dev);
    }

    /* 3.Call function with out of bound value for device id */
    dev = PRV_CPSS_MAX_PP_DEVICES_CNS;

    st = cpssDxChCosPortDpToCfiDeiMapEnableGet(dev, port, &enable);
    UTF_VERIFY_EQUAL1_PARAM_MAC(GT_BAD_PARAM, st, dev);
}

/*----------------------------------------------------------------------------*/
/*
cpssDxChCosDpToCfiDeiMapSet
(
    IN GT_U8                dev,
    IN CPSS_DP_LEVEL_ENT    dp,
    IN GT_U8                cfiDeiBit
)
*/
UTF_TEST_CASE_MAC(cpssDxChCosDpToCfiDeiMapSet)
{
/*
    ITERATE_DEVICES (DxCh3 and above)
    1.1. Call with dp [CPSS_DP_GREEN_E /
                       CPSS_DP_RED_E]
                   and cfiDeiBit [0 / 1].
    Expected: GT_OK.
    1.2. Call cpssDxChCosDpToCfiDeiMapGet with same dp and non-NULL cfiDeiBitPtr.
    Expected: GT_OK and the same cfiDeiBit.
    1.3. Call with wrong enum values dp
                   and other parameters from 1.1.
    Expected: GT_BAD_PARAM.
    1.4. Call with out of range cfiDeiBit [2]
                   and other parameters from 1.1.
    Expected: NOT GT_OK.
    1.5. Call with out of range dp [CPSS_DP_YELLOW_E],
                   and other parameters from 1.1.
    Expected: NOT GT_OK.
*/
    GT_STATUS   st = GT_OK;
    GT_U8       dev;

    CPSS_DP_LEVEL_ENT    dp           = CPSS_DP_GREEN_E;
    GT_U8                cfiDeiBit    = 0;
    GT_U8                cfiDeiBitGet = 0;


    /* prepare device iterator */
    PRV_TGF_NOT_APPLIC_DEV_RESET_MAC(&dev, UTF_CH1_E | UTF_CH1_DIAMOND_E | UTF_CH2_E);

    /* 1. Go over all active devices. */
    while(GT_OK == prvUtfNextDeviceGet(&dev, GT_TRUE))
    {
        /*
            1.1. Call with dp [CPSS_DP_GREEN_E /
                               CPSS_DP_RED_E]
                           and cfiDeiBit [0 / 1].
            Expected: GT_OK.
        */

        /* Call with dp [CPSS_DP_GREEN_E] */
        dp        = CPSS_DP_GREEN_E;
        cfiDeiBit = 0;

        st = cpssDxChCosDpToCfiDeiMapSet(dev, dp, cfiDeiBit);
        UTF_VERIFY_EQUAL3_PARAM_MAC(GT_OK, st, dev, dp, cfiDeiBit);

        /*
            1.2. Call cpssDxChCosDpToCfiDeiMapGet with same dp and non-NULL cfiDeiBitPtr.
            Expected: GT_OK and the same cfiDeiBit.
        */
        st = cpssDxChCosDpToCfiDeiMapGet(dev, dp, &cfiDeiBitGet);
        UTF_VERIFY_EQUAL2_STRING_MAC(GT_OK, st,
                   "cpssDxChCosDpToCfiDeiMapGet: %d, %d", dev, dp);

        /* Verifying values */
        UTF_VERIFY_EQUAL1_STRING_MAC(cfiDeiBit, cfiDeiBitGet,
                   "get another cfiDeiBit than was set: %d", dev);

        /* Call with dp [CPSS_DP_RED_E] */
        dp        = CPSS_DP_RED_E;
        cfiDeiBit = 1;

        st = cpssDxChCosDpToCfiDeiMapSet(dev, dp, cfiDeiBit);
        UTF_VERIFY_EQUAL3_PARAM_MAC(GT_OK, st, dev, dp, cfiDeiBit);

        /*
            1.2. Call cpssDxChCosDpToCfiDeiMapGet with same dp and non-NULL cfiDeiBitPtr.
            Expected: GT_OK and the same cfiDeiBit.
        */
        st = cpssDxChCosDpToCfiDeiMapGet(dev, dp, &cfiDeiBitGet);
        UTF_VERIFY_EQUAL2_STRING_MAC(GT_OK, st,
                   "cpssDxChCosDpToCfiDeiMapGet: %d, %d", dev, dp);

        /* Verifying values */
        UTF_VERIFY_EQUAL1_STRING_MAC(cfiDeiBit, cfiDeiBitGet,
                   "get another cfiDeiBit than was set: %d", dev);

        /*
            1.3. Call with wrong enum values dp and other parameters from 1.1.
            Expected: GT_BAD_PARAM.
        */
        UTF_ENUMS_CHECK_MAC(cpssDxChCosDpToCfiDeiMapSet
                            (dev, dp, cfiDeiBit),
                            dp);

        /*
            1.4. Call with out of range cfiDeiBit [2]
                           and other parameters from 1.1.
            Expected: NOT GT_OK.
        */
        cfiDeiBit = 2;

        st = cpssDxChCosDpToCfiDeiMapSet(dev, dp, cfiDeiBit);
        UTF_VERIFY_NOT_EQUAL2_STRING_MAC(GT_OK, st, "%d, cfiDeiBit = %d", dev, cfiDeiBit);

        cfiDeiBit = 1;

        /*
            1.5. Call with out of range dp [CPSS_DP_YELLOW_E],
                           and other parameters from 1.1.
            Expected: NOT GT_OK.
        */
        dp = CPSS_DP_YELLOW_E;

        st = cpssDxChCosDpToCfiDeiMapSet(dev, dp, cfiDeiBit);
        if (0 == PRV_CPSS_DXCH_PP_HW_INFO_TXQ_REV_1_OR_ABOVE_MAC(dev))
        {
            UTF_VERIFY_NOT_EQUAL3_PARAM_MAC(GT_OK, st, dev, dp, cfiDeiBit);
        }
        else
        {
            UTF_VERIFY_EQUAL3_PARAM_MAC(GT_OK, st, dev, dp, cfiDeiBit);
        }

        st = cpssDxChCosDpToCfiDeiMapGet(dev, dp, &cfiDeiBitGet);
        if (0 == PRV_CPSS_DXCH_PP_HW_INFO_TXQ_REV_1_OR_ABOVE_MAC(dev))
        {
            UTF_VERIFY_NOT_EQUAL3_PARAM_MAC(GT_OK, st, dev, dp, cfiDeiBit);
        }
        else
        {
            UTF_VERIFY_EQUAL3_PARAM_MAC(GT_OK, st, dev, dp, cfiDeiBit);
            UTF_VERIFY_EQUAL3_PARAM_MAC(
                cfiDeiBitGet, cfiDeiBit, dev, dp, cfiDeiBit);
        }
    }

    dp        = CPSS_DP_GREEN_E;
    cfiDeiBit = 0;

    /* 2. For not-active devices and devices from non-applicable family */
    /* check that function returns GT_BAD_PARAM.                        */

    /* prepare device iterator */
    PRV_TGF_NOT_APPLIC_DEV_RESET_MAC(&dev, UTF_CH1_E | UTF_CH1_DIAMOND_E | UTF_CH2_E);

    /* go over all non active devices */
    while(GT_OK == prvUtfNextDeviceGet(&dev, GT_FALSE))
    {
        st = cpssDxChCosDpToCfiDeiMapSet(dev, dp, cfiDeiBit);
        UTF_VERIFY_EQUAL1_PARAM_MAC(GT_NOT_APPLICABLE_DEVICE, st, dev);
    }

    /* 3. Call function with out of bound value for device id.*/
    dev = PRV_CPSS_MAX_PP_DEVICES_CNS;

    st = cpssDxChCosDpToCfiDeiMapSet(dev, dp, cfiDeiBit);
    UTF_VERIFY_EQUAL1_PARAM_MAC(GT_BAD_PARAM, st, dev);
}

/*----------------------------------------------------------------------------*/
/*
cpssDxChCosDpToCfiDeiMapGet
(
    IN  GT_U8               dev,
    IN  CPSS_DP_LEVEL_ENT   dp,
    OUT GT_U8               *cfiDeiBitPtr
)
*/
UTF_TEST_CASE_MAC(cpssDxChCosDpToCfiDeiMapGet)
{
/*
    ITERATE_DEVICES (DxCh3 and above)
    1.1. Call with dp [CPSS_DP_GREEN_E /
                       CPSS_DP_RED_E]
                   and non-NULL cfiDeiBitPtr.
    Expected: GT_OK.
    1.2. Call with wrong enum values dp
                   and other parameters from 1.1.
    Expected: GT_BAD_PARAM.
    1.3. Call with out of range dp [CPSS_DP_YELLOW_E],
                   and other parameters from 1.1.
    Expected: NOT GT_OK.
    1.4. Call with cfiDeiBitPtr [NULL]
                   and other parameters from 1.1.
    Expected: GT_BAD_PTR.
*/
    GT_STATUS   st = GT_OK;
    GT_U8       dev;

    CPSS_DP_LEVEL_ENT    dp        = CPSS_DP_GREEN_E;
    GT_U8                cfiDeiBit = 0;


    /* prepare device iterator */
    PRV_TGF_NOT_APPLIC_DEV_RESET_MAC(&dev, UTF_CH1_E | UTF_CH1_DIAMOND_E | UTF_CH2_E);

    /* 1. Go over all active devices. */
    while(GT_OK == prvUtfNextDeviceGet(&dev, GT_TRUE))
    {
        /*
            1.1. Call with dp [CPSS_DP_GREEN_E /
                               CPSS_DP_RED_E]
                           and non-NULL cfiDeiBitPtr.
            Expected: GT_OK.
        */

        /* Call with dp [CPSS_DP_GREEN_E] */
        dp = CPSS_DP_GREEN_E;

        st = cpssDxChCosDpToCfiDeiMapGet(dev, dp, &cfiDeiBit);
        UTF_VERIFY_EQUAL2_PARAM_MAC(GT_OK, st, dev, dp);

        /* Call with dp [CPSS_DP_RED_E] */
        dp = CPSS_DP_RED_E;

        st = cpssDxChCosDpToCfiDeiMapGet(dev, dp, &cfiDeiBit);
        UTF_VERIFY_EQUAL2_PARAM_MAC(GT_OK, st, dev, dp);

        /*
            1.2. Call with wrong enum values dp and other parameters from 1.1.
            Expected: GT_BAD_PARAM.
        */
        UTF_ENUMS_CHECK_MAC(cpssDxChCosDpToCfiDeiMapGet
                            (dev, dp, &cfiDeiBit),
                            dp);

        /*
            1.3. Call with out of range dp [CPSS_DP_YELLOW_E],
                       and other parameters from 1.1.
            Expected: NOT GT_OK.
        */
        dp = CPSS_DP_YELLOW_E;

        st = cpssDxChCosDpToCfiDeiMapGet(dev, dp, &cfiDeiBit);
        if (0 == PRV_CPSS_DXCH_PP_HW_INFO_TXQ_REV_1_OR_ABOVE_MAC(dev))
        {
            UTF_VERIFY_NOT_EQUAL2_PARAM_MAC(GT_OK, st, dev, dp);
        }
        else
        {
            UTF_VERIFY_EQUAL2_PARAM_MAC(GT_OK, st, dev, dp);
        }

        dp = CPSS_DP_GREEN_E;

        /*
            1.4. Call with cfiDeiBitPtr [NULL]
                           and other parameters from 1.1.
            Expected: GT_BAD_PTR.
        */
        st = cpssDxChCosDpToCfiDeiMapGet(dev, dp, NULL);
        UTF_VERIFY_EQUAL1_STRING_MAC(GT_BAD_PTR, st, "%d, cfiDeiBitPtr = NULL", dev);
    }

    dp = CPSS_DP_GREEN_E;

    /* 2. For not-active devices and devices from non-applicable family */
    /* check that function returns GT_BAD_PARAM.                        */

    /* prepare device iterator */
    PRV_TGF_NOT_APPLIC_DEV_RESET_MAC(&dev, UTF_CH1_E | UTF_CH1_DIAMOND_E | UTF_CH2_E);

    /* go over all non active devices */
    while(GT_OK == prvUtfNextDeviceGet(&dev, GT_FALSE))
    {
        st = cpssDxChCosDpToCfiDeiMapGet(dev, dp, &cfiDeiBit);
        UTF_VERIFY_EQUAL1_PARAM_MAC(GT_NOT_APPLICABLE_DEVICE, st, dev);
    }

    /* 3. Call function with out of bound value for device id.*/
    dev = PRV_CPSS_MAX_PP_DEVICES_CNS;

    st = cpssDxChCosDpToCfiDeiMapGet(dev, dp, &cfiDeiBit);
    UTF_VERIFY_EQUAL1_PARAM_MAC(GT_BAD_PARAM, st, dev);
}

/*----------------------------------------------------------------------------*/
/*
GT_STATUS cpssDxChCosPortQosConfigSet
(
    IN GT_U8                dev,
    IN GT_U8                port,
    IN CPSS_QOS_ENTRY_STC  *portQosCfg_PTR
)
*/
UTF_TEST_CASE_MAC(cpssDxChCosPortQosConfigSet)
{
/*
    ITERATE_DEVICES_VIRT_PORTS (DxChx)
    1.1.1. Call with valid portQosGfgPtr {1,
                                          CPSS_PACKET_ATTRIBUTE_ASSIGN_PRECEDENCE_SOFT_E,
                                          CPSS_PACKET_ATTRIBUTE_MODIFY_ENABLE_E,
                                          CPSS_PACKET_ATTRIBUTE_MODIFY_DISABLE_E}.
    Expected: GT_OK.
    1.1.2. Call cpssDxChCosPortQosConfigGet.
    Expected: GT_OK and the same portQosGfgPtr.
    1.1.3. Call with portQosGfgPtr [NULL]
    Expected: GT_BAD_PTR.
    1.1.4. Call with out of range portQosGfgPtr->qosProfileId
                     portQosGfgPtr {PRV_CPSS_DXCH_QOS_PROFILE_NUM_MAX_CNS = 72,
                                    CPSS_PACKET_ATTRIBUTE_ASSIGN_PRECEDENCE_SOFT_E,
                                          CPSS_PACKET_ATTRIBUTE_MODIFY_ENABLE_E,
                                          CPSS_PACKET_ATTRIBUTE_MODIFY_DISABLE_E}.
    Expected: NOT GT_OK for Cheetah and GT_OK for Cheetah-2,3.
    1.1.5. Call function with out of range portQosGfgPtr->qosProfileId
                     portQosGfgPtr [PRV_CPSS_DXCH2_QOS_PROFILE_NUM_MAX_CNS = 128]
                     and valid other parameters.
    Expected: NOT GT_OK.
    1.1.6. Call with out of range portQosGfgPtr->assignPrecedence {2,
                                                                   wrong enum values,
                                                                   CPSS_PACKET_ATTRIBUTE_MODIFY_ENABLE_E,
                                                                   CPSS_PACKET_ATTRIBUTE_MODIFY_DISABLE_E}.
    Expected: GT_BAD_PARAM.
    1.1.7. Call with out of range portQosGfgPtr->enableModifyUp {2,
                                                                 CPSS_PACKET_ATTRIBUTE_ASSIGN_PRECEDENCE_HARD_E,
                                                                 wrong enum values,
                                                                 CPSS_PACKET_ATTRIBUTE_MODIFY_DISABLE_E}.
    Expected: GT_BAD_PARAM.
    1.1.8. Call with out of range portQosGfgPtr->enableModifyDscp {3,
                                                                   CPSS_PACKET_ATTRIBUTE_ASSIGN_PRECEDENCE_HARD_E,
                                                                   CPSS_PACKET_ATTRIBUTE_MODIFY_ENABLE_E,
                                                                   wrong enum values}.
    Expected: GT_BAD_PARAM.
    1.2. Call with port [CPSS_CPU_PORT_NUM_CNS = 63] and valid other parameters.
    Expected: GT_OK.
*/

    GT_STATUS               st     = GT_OK;

    GT_U8                   dev;
    GT_U8                   port   = COS_VALID_VIRT_PORT_CNS;
    CPSS_QOS_ENTRY_STC      portCfg;
    CPSS_QOS_ENTRY_STC      portCfgGet;
    CPSS_PP_FAMILY_TYPE_ENT devFamily;

    /* prepare iterator for go over all active devices */
    PRV_TGF_NOT_APPLIC_DEV_RESET_MAC(&dev, UTF_NONE_FAMILY_E);

    /* 1. Go over all active devices. */
    while (GT_OK == prvUtfNextDeviceGet(&dev, GT_TRUE))
    {
        st = prvUtfNextVirtPortReset(&port, dev);
        UTF_VERIFY_EQUAL0_PARAM_MAC(GT_OK, st);

        /* 1.1. For all active devices go over all available virtual ports. */
        while (GT_OK == prvUtfNextVirtPortGet(&port, GT_TRUE))
        {
            /*
                1.1.1. Call with valid portQosGfgPtr {1,
                                                      CPSS_PACKET_ATTRIBUTE_ASSIGN_PRECEDENCE_SOFT_E,
                                                      CPSS_PACKET_ATTRIBUTE_MODIFY_KEEP_PREVIOUS_E,
                                                      CPSS_PACKET_ATTRIBUTE_MODIFY_KEEP_PREVIOUS_E}.
                Expected: GT_OK.
            */

            portCfg.qosProfileId     = 1;
            portCfg.assignPrecedence = CPSS_PACKET_ATTRIBUTE_ASSIGN_PRECEDENCE_SOFT_E;
            portCfg.enableModifyUp   = CPSS_PACKET_ATTRIBUTE_MODIFY_ENABLE_E;
            portCfg.enableModifyDscp = CPSS_PACKET_ATTRIBUTE_MODIFY_DISABLE_E;

            st = cpssDxChCosPortQosConfigSet(dev, port, &portCfg);
            UTF_VERIFY_EQUAL2_PARAM_MAC(GT_OK, st, dev, port);

            /*
                1.1.2. Call cpssDxChCosPortQosConfigGet.
                Expected: GT_OK and the same portQosGfgPtr.
            */
            st = cpssDxChCosPortQosConfigGet(dev, port, &portCfgGet);
            UTF_VERIFY_EQUAL2_STRING_MAC(GT_OK, st,
                       "cpssDxChCosPortQosConfigGet: %d, %d", dev, port);

            /* Verifying struct fields */
            UTF_VERIFY_EQUAL2_STRING_MAC(portCfg.qosProfileId, portCfgGet.qosProfileId,
                       "get another portQosCfg_PTR->qosProfileId than was set: %d, %d", dev, port);
            UTF_VERIFY_EQUAL2_STRING_MAC(portCfg.assignPrecedence, portCfgGet.assignPrecedence,
                       "get another portQosCfg_PTR->assignPrecedence than was set: %d, %d", dev, port);
            UTF_VERIFY_EQUAL2_STRING_MAC(portCfg.enableModifyUp, portCfgGet.enableModifyUp,
                       "get another portQosCfg_PTR->enableModifyUp than was set: %d, %d", dev, port);
            UTF_VERIFY_EQUAL2_STRING_MAC(portCfg.enableModifyDscp, portCfgGet.enableModifyDscp,
                       "get another portQosCfg_PTR->enableModifyDscp than was set: %d, %d", dev, port);

            /*
                1.1.3. Call with portQosGfgPtr [NULL]
                Expected: GT_BAD_PTR.
            */
            st = cpssDxChCosPortQosConfigSet(dev, port, NULL);
            UTF_VERIFY_EQUAL2_STRING_MAC(GT_BAD_PTR, st, "%d, %d, NULL", dev, port);

            /* Getting device family */
            st = prvUtfDeviceFamilyGet(dev, &devFamily);
            UTF_VERIFY_EQUAL1_STRING_MAC(GT_OK, st, "prvUtfDeviceFamilyGet: %d", dev);

            /*
                1.1.4. Call with out of range portQosGfgPtr->qosProfileId
                     portQosGfgPtr {PRV_CPSS_DXCH_QOS_PROFILE_NUM_MAX_CNS = 72,
                                    CPSS_PACKET_ATTRIBUTE_ASSIGN_PRECEDENCE_SOFT_E,
                                    CPSS_PACKET_ATTRIBUTE_MODIFY_KEEP_PREVIOUS_E,
                                    CPSS_PACKET_ATTRIBUTE_MODIFY_KEEP_PREVIOUS_E}.
                Expected: NOT GT_OK for Cheetah and GT_OK for Cheetah-2.
            */
            portCfg.qosProfileId = PRV_CPSS_DXCH_QOS_PROFILE_NUM_MAX_CNS;

            st = cpssDxChCosPortQosConfigSet(dev, port, &portCfg);
            if (devFamily != CPSS_PP_FAMILY_CHEETAH_E)
            {
                UTF_VERIFY_EQUAL3_STRING_MAC(GT_OK, st,
                           "%d, %d, portQosGfgPtr->qosProfileId = %d", dev, port, portCfg.qosProfileId);
            }
            else
            {
                UTF_VERIFY_NOT_EQUAL3_STRING_MAC(GT_OK, st,
                           "%d, %d, portQosGfgPtr->qosProfileId = %d", dev, port, portCfg.qosProfileId);
            }

            /*
                1.1.5. Call function with out of range portQosGfgPtr->qosProfileId
                     portQosGfgPtr [PRV_CPSS_DXCH2_QOS_PROFILE_NUM_MAX_CNS = 128]
                     and valid other parameters.
                Expected: NOT GT_OK.
            */
            portCfg.qosProfileId = PRV_CPSS_DXCH2_QOS_PROFILE_NUM_MAX_CNS;

            st = cpssDxChCosPortQosConfigSet(dev, port, &portCfg);
            UTF_VERIFY_NOT_EQUAL3_STRING_MAC(GT_OK, st,
                           "%d, %d, portQosGfgPtr->qosProfileId = %d", dev, port, portCfg.qosProfileId);

            /*
                1.1.6. Call with out of range portQosGfgPtr->assignPrecedence {2,
                               wrong enum values,
                               CPSS_PACKET_ATTRIBUTE_MODIFY_KEEP_PREVIOUS_E,
                               CPSS_PACKET_ATTRIBUTE_MODIFY_KEEP_PREVIOUS_E}.
                Expected: GT_BAD_PARAM.
            */
            portCfg.qosProfileId     = 2;

            UTF_ENUMS_CHECK_MAC(cpssDxChCosPortQosConfigSet
                                (dev, port, &portCfg),
                                portCfg.assignPrecedence);

            /*
                1.1.7. Call with out of range portQosGfgPtr->enableModifyUp {2,
                               CPSS_PACKET_ATTRIBUTE_ASSIGN_PRECEDENCE_HARD_E,
                               wrong enum values,
                               CPSS_PACKET_ATTRIBUTE_MODIFY_KEEP_PREVIOUS_E}.
                Expected: GT_BAD_PARAM.
            */
            portCfg.assignPrecedence = CPSS_PACKET_ATTRIBUTE_ASSIGN_PRECEDENCE_HARD_E;

            UTF_ENUMS_CHECK_MAC(cpssDxChCosPortQosConfigSet
                                (dev, port, &portCfg),
                                portCfg.enableModifyUp);

            /*
                1.1.8. Call with out of range portQosGfgPtr->enableModifyDscp {3,
                                 CPSS_PACKET_ATTRIBUTE_ASSIGN_PRECEDENCE_HARD_E,
                                 CPSS_PACKET_ATTRIBUTE_MODIFY_ENABLE_E,
                                 wrong enum values}.
                Expected: GT_BAD_PARAM.
            */
            portCfg.enableModifyUp   = CPSS_PACKET_ATTRIBUTE_MODIFY_ENABLE_E;

            UTF_ENUMS_CHECK_MAC(cpssDxChCosPortQosConfigSet
                                (dev, port, &portCfg),
                                portCfg.enableModifyDscp);
        }

        portCfg.qosProfileId     = 1;
        portCfg.assignPrecedence = CPSS_PACKET_ATTRIBUTE_ASSIGN_PRECEDENCE_SOFT_E;
        portCfg.enableModifyUp   = CPSS_PACKET_ATTRIBUTE_MODIFY_ENABLE_E;
        portCfg.enableModifyDscp = CPSS_PACKET_ATTRIBUTE_MODIFY_DISABLE_E;

        st = prvUtfNextVirtPortReset(&port, dev);
        UTF_VERIFY_EQUAL0_PARAM_MAC(GT_OK, st);
        /* 1.2. For all active devices go over all non available virtual ports. */
        while (GT_OK == prvUtfNextVirtPortGet(&port, GT_FALSE))
        {
            /* 1.2.1. Call function for each non-active port */
            st = cpssDxChCosPortQosConfigSet(dev, port, &portCfg);
            UTF_VERIFY_EQUAL2_PARAM_MAC(GT_BAD_PARAM, st, dev, port);
        }

        /* 1.3. For active device check that function returns GT_BAD_PARAM */
        /* for out of bound value for port number.                         */
        port = PRV_CPSS_MAX_PP_PORTS_NUM_CNS;

        st = cpssDxChCosPortQosConfigSet(dev, port, &portCfg);
        UTF_VERIFY_EQUAL2_PARAM_MAC(GT_BAD_PARAM, st, dev, port);

        /* 1.4. For active device check that function returns GT_OK        */
        /* for CPU port number.                                            */
        port = CPSS_CPU_PORT_NUM_CNS;

        st = cpssDxChCosPortQosConfigSet(dev, port, &portCfg);
        UTF_VERIFY_EQUAL2_PARAM_MAC(GT_OK, st, dev, port);
    }

    portCfg.qosProfileId     = 1;
    portCfg.assignPrecedence = CPSS_PACKET_ATTRIBUTE_ASSIGN_PRECEDENCE_SOFT_E;
    portCfg.enableModifyUp   = CPSS_PACKET_ATTRIBUTE_MODIFY_KEEP_PREVIOUS_E;
    portCfg.enableModifyDscp = CPSS_PACKET_ATTRIBUTE_MODIFY_KEEP_PREVIOUS_E;

    /* 2. For not active devices check that function returns non GT_OK.*/
    port = COS_VALID_VIRT_PORT_CNS;

    /* prepare device iterator */
    PRV_TGF_NOT_APPLIC_DEV_RESET_MAC(&dev, UTF_NONE_FAMILY_E);

    /* Go over all non active devices. */
    while(GT_OK == prvUtfNextDeviceGet(&dev, GT_FALSE))
    {
        st = cpssDxChCosPortQosConfigSet(dev, port, &portCfg);
        UTF_VERIFY_EQUAL1_PARAM_MAC(GT_NOT_APPLICABLE_DEVICE, st, dev);
    }

    /* 3.Call function with out of bound value for device id */
    dev = PRV_CPSS_MAX_PP_DEVICES_CNS;

    st = cpssDxChCosPortQosConfigSet(dev, port, &portCfg);
    UTF_VERIFY_EQUAL1_PARAM_MAC(GT_BAD_PARAM, st, dev);
}

/*----------------------------------------------------------------------------*/
/*
GT_STATUS cpssDxChCosPortQosConfigGet
(
    IN GT_U8                dev,
    IN GT_U8                port,
    IN CPSS_QOS_ENTRY_STC  *portQosCfg_PTR
)
*/
UTF_TEST_CASE_MAC(cpssDxChCosPortQosConfigGet)
{
/*
    ITERATE_DEVICES_VIRT_PORTS (DxChx)
    1.1.1. Call with non-null portQosCfg_PTR.
    Expected: GT_OK.
    1.1.2. Call with portQosCfg_PTR [NULL].
    Expected: GT_BAD_PTR.
    1.2. Call with port [CPSS_CPU_PORT_NUM_CNS = 63] and valid other parameters.
    Expected: GT_OK.
*/
    GT_STATUS          st      = GT_OK;

    GT_U8              dev;
    GT_U8              port    = COS_VALID_VIRT_PORT_CNS;
    CPSS_QOS_ENTRY_STC portCfg;

    /* prepare iterator for go over all active devices */
    PRV_TGF_NOT_APPLIC_DEV_RESET_MAC(&dev, UTF_NONE_FAMILY_E);

    /* 1. Go over all active devices. */
    while (GT_OK == prvUtfNextDeviceGet(&dev, GT_TRUE))
    {
        st = prvUtfNextVirtPortReset(&port, dev);
        UTF_VERIFY_EQUAL0_PARAM_MAC(GT_OK, st);

        /* 1.1. For all active devices go over all available virtual ports. */
        while (GT_OK == prvUtfNextVirtPortGet(&port, GT_TRUE))
        {
            /*
                1.1.1. Call with non-null portQosCfg_PTR.
                Expected: GT_OK.
            */
            st = cpssDxChCosPortQosConfigGet(dev, port, &portCfg);
            UTF_VERIFY_EQUAL2_PARAM_MAC(GT_OK, st, dev, port);

            /*
                1.1.2. Call with portQosCfg_PTR [NULL].
                Expected: GT_BAD_PTR.
            */
            st = cpssDxChCosPortQosConfigGet(dev, port, NULL);
            UTF_VERIFY_EQUAL2_STRING_MAC(GT_BAD_PTR, st, "%d, %d, NULL", dev, port);
        }

        st = prvUtfNextVirtPortReset(&port, dev);
        UTF_VERIFY_EQUAL0_PARAM_MAC(GT_OK, st);

        /* 1.2. For all active devices go over all non available
           virtual ports.
        */
        while (GT_OK == prvUtfNextVirtPortGet(&port, GT_FALSE))
        {
            st = cpssDxChCosPortQosConfigGet(dev, port, &portCfg);
            UTF_VERIFY_EQUAL2_PARAM_MAC(GT_BAD_PARAM, st, dev, port);
        }

        /* 1.3. For active device check that function returns GT_BAD_PARAM */
        /* for out of bound value for port number.                         */
        port = PRV_CPSS_MAX_PP_PORTS_NUM_CNS;

        st = cpssDxChCosPortQosConfigGet(dev, port, &portCfg);
        UTF_VERIFY_EQUAL2_PARAM_MAC(GT_BAD_PARAM, st, dev, port);

        /* 1.4. For active device check that function returns GT_OK        */
        /* for CPU port number.                                            */
        port = CPSS_CPU_PORT_NUM_CNS;

        st = cpssDxChCosPortQosConfigGet(dev, port, &portCfg);
        UTF_VERIFY_EQUAL2_PARAM_MAC(GT_OK, st, dev, port);
    }

    /* 2. For not active devices check that function returns non GT_OK.*/

    port = COS_VALID_VIRT_PORT_CNS;

    /* prepare device iterator */
    PRV_TGF_NOT_APPLIC_DEV_RESET_MAC(&dev, UTF_NONE_FAMILY_E);

    /* Go over all non active devices. */
    while(GT_OK == prvUtfNextDeviceGet(&dev, GT_FALSE))
    {
        st = cpssDxChCosPortQosConfigGet(dev, port, &portCfg);
        UTF_VERIFY_EQUAL1_PARAM_MAC(GT_NOT_APPLICABLE_DEVICE, st, dev);
    }

    /* 3.Call function with out of bound value for device id */
    dev = PRV_CPSS_MAX_PP_DEVICES_CNS;

    st = cpssDxChCosPortQosConfigGet(dev, port, &portCfg);
    UTF_VERIFY_EQUAL1_PARAM_MAC(GT_BAD_PARAM, st, dev);
}

/*----------------------------------------------------------------------------*/
/*
GT_STATUS cpssDxChCosMacQosEntrySet
(
    IN GT_U8               dev,
    IN GT_U32              entryIdx,
    IN CPSS_QOS_ENTRY_STC *macQosCfg_PTR
)
*/
UTF_TEST_CASE_MAC(cpssDxChCosMacQosEntrySet)
{
/*
    ITERATE_DEVICES (DxChx)
    1.1. Call with entryIdx [1],
                   valid macQosCfg_PTR {1,
                                        CPSS_PACKET_ATTRIBUTE_ASSIGN_PRECEDENCE_SOFT_E,
                                        CPSS_PACKET_ATTRIBUTE_MODIFY_KEEP_PREVIOUS_E,
                                        CPSS_PACKET_ATTRIBUTE_MODIFY_KEEP_PREVIOUS_E}.
    Expected: GT_OK.
    1.1.2. Call cpssDxChCosMacQosEntryGet with the same entryIdx.
    Expected: GT_OK and the same macQosCfg_PTR.
    1.1.3. Call with out of range entryIdx [0/ 8]
                     and valid the same as 1.1. macQosCfg_PTR.
    Expected: NOT GT_OK.
    1.1.4. Call with entryIdx [2],
                     macQosCfg_PTR [NULL]
    Expected: GT_BAD_PTR.
    1.1.5. Call with entryIdx [3],
                     out of range macQosCfg_PTR->qosProfileId {PRV_CPSS_DXCH_QOS_PROFILE_NUM_MAX_CNS = 72,
                                                               CPSS_PACKET_ATTRIBUTE_ASSIGN_PRECEDENCE_SOFT_E,
                                                               CPSS_PACKET_ATTRIBUTE_MODIFY_KEEP_PREVIOUS_E,
                                                               CPSS_PACKET_ATTRIBUTE_MODIFY_KEEP_PREVIOUS_E}.
    Expected: NOT GT_OK for Cheetah and GT_OK for Cheetah-2,3.
    1.1.6. Call function with out of range portQosGfgPtr->qosProfileId
                            portQosGfgPtr [PRV_CPSS_DXCH2_QOS_PROFILE_NUM_MAX_CNS = 128]
                            and valid other parameters.
    Expected: NOT GT_OK.
    1.1.7. Call with entryIdx [4],
                     out of range macQosCfg_PTR->assignPrecedence {2,
                                                                   wrong enum values,
                                                                   CPSS_PACKET_ATTRIBUTE_MODIFY_KEEP_PREVIOUS_E,
                                                                   CPSS_PACKET_ATTRIBUTE_MODIFY_KEEP_PREVIOUS_E}.
    Expected: GT_BAD_PARAM.
    1.1.8. Call with entryIdx [4],
                     out of range macQosCfg_PTR->enableModifyUp {2,
                                                                 CPSS_PACKET_ATTRIBUTE_ASSIGN_PRECEDENCE_HARD_E,
                                                                 wrong enum values,
                                                                 CPSS_PACKET_ATTRIBUTE_MODIFY_KEEP_PREVIOUS_E}.
    Expected: GT_BAD_PARAM.
    1.1.9. Call with entryIdx [5],
                     out of range macQosCfg_PTR->enableModifyDscp {3,
                                                                   CPSS_PACKET_ATTRIBUTE_ASSIGN_PRECEDENCE_HARD_E,
                                                                   CPSS_PACKET_ATTRIBUTE_MODIFY_ENABLE_E,
                                                                   wrong enum values}.
    Expected: GT_BAD_PARAM.
*/
    GT_STATUS               st    = GT_OK;

    GT_U8                   dev;
    GT_U32                  index = 0;
    CPSS_QOS_ENTRY_STC      macCfg;
    CPSS_QOS_ENTRY_STC      macCfgGet;
    CPSS_PP_FAMILY_TYPE_ENT devFamily;

    /* prepare device iterator */
    PRV_TGF_NOT_APPLIC_DEV_RESET_MAC(&dev, UTF_NONE_FAMILY_E);

    /* 1. Go over all active devices. */
    while (GT_OK == prvUtfNextDeviceGet(&dev, GT_TRUE))
    {
        /*
            1.1. Call with entryIdx [1],
                           valid macQosCfg_PTR {1,
                                                CPSS_PACKET_ATTRIBUTE_ASSIGN_PRECEDENCE_SOFT_E,
                                                CPSS_PACKET_ATTRIBUTE_MODIFY_KEEP_PREVIOUS_E,
                                                CPSS_PACKET_ATTRIBUTE_MODIFY_KEEP_PREVIOUS_E}.
            Expected: GT_OK.
        */

        index = 1;
        macCfg.qosProfileId     = 1;
        macCfg.assignPrecedence = CPSS_PACKET_ATTRIBUTE_ASSIGN_PRECEDENCE_SOFT_E;
        macCfg.enableModifyUp   = CPSS_PACKET_ATTRIBUTE_MODIFY_KEEP_PREVIOUS_E;
        macCfg.enableModifyDscp = CPSS_PACKET_ATTRIBUTE_MODIFY_KEEP_PREVIOUS_E;

        st = cpssDxChCosMacQosEntrySet(dev, index, &macCfg);
        UTF_VERIFY_EQUAL2_PARAM_MAC(GT_OK, st, dev, index);

        /*
            1.1.2. Call cpssDxChCosMacQosEntryGet with the same entryIdx.
            Expected: GT_OK and the same macQosCfg_PTR.
        */
        st = cpssDxChCosMacQosEntryGet(dev, index, &macCfgGet);
        UTF_VERIFY_EQUAL2_STRING_MAC(GT_OK, st,
                       "cpssDxChCosMacQosEntryGet: %d, %d", dev, index);

        /* Verifying macQosCfg_PTR */
        UTF_VERIFY_EQUAL2_STRING_MAC(macCfg.qosProfileId, macCfgGet.qosProfileId,
            "get another macQosCfg_PTR->qosProfileId than was set: %d, %d", dev, index);
        UTF_VERIFY_EQUAL2_STRING_MAC(macCfg.assignPrecedence, macCfgGet.assignPrecedence,
            "get another macQosCfg_PTR->assignPrecedence than was set: %d, %d", dev, index);
        UTF_VERIFY_EQUAL2_STRING_MAC(macCfg.enableModifyUp, macCfgGet.enableModifyUp,
            "get another macQosCfg_PTR->enableModifyUp than was set: %d, %d", dev, index);
        UTF_VERIFY_EQUAL2_STRING_MAC(macCfg.enableModifyDscp, macCfgGet.enableModifyDscp,
            "get another macQosCfg_PTR->enableModifyDscp than was set: %d, %d", dev, index);

        /*
            1.1.3. Call with out of range entryIdx [0/ 8]
                             and valid the same as 1.1. macQosCfg_PTR.
            Expected: NOT GT_OK.
        */
        index = 0;

        st = cpssDxChCosMacQosEntrySet(dev, index, &macCfg);
        UTF_VERIFY_NOT_EQUAL2_PARAM_MAC(GT_OK, st, dev, index);

        index = 8;

        st = cpssDxChCosMacQosEntrySet(dev, index, &macCfg);
        UTF_VERIFY_NOT_EQUAL2_PARAM_MAC(GT_OK, st, dev, index);

        /*
            1.1.4. Call with entryIdx [2],
                             macQosCfg_PTR [NULL]
            Expected: GT_BAD_PTR.
        */
        index = 2;

        st = cpssDxChCosMacQosEntrySet(dev, index, NULL);
        UTF_VERIFY_EQUAL1_STRING_MAC(GT_BAD_PTR, st, "%d, macQosCfg_PTR = NULL",
                                     dev);

        /* Getting device family */
        st = prvUtfDeviceFamilyGet(dev, &devFamily);
        UTF_VERIFY_EQUAL1_STRING_MAC(GT_OK, st, "prvUtfDeviceFamilyGet: %d", dev);

        /*
            1.1.5. Call with entryIdx [3],
                             out of range macQosCfg_PTR->qosProfileId {PRV_CPSS_DXCH_QOS_PROFILE_NUM_MAX_CNS = 72,
                                                                       CPSS_PACKET_ATTRIBUTE_ASSIGN_PRECEDENCE_SOFT_E,
                                                                       CPSS_PACKET_ATTRIBUTE_MODIFY_KEEP_PREVIOUS_E,
                                                                       CPSS_PACKET_ATTRIBUTE_MODIFY_KEEP_PREVIOUS_E}.
            Expected: NOT GT_OK for Cheetah and GT_OK for Cheetah-2.
        */
        index = 3;
        macCfg.qosProfileId = PRV_CPSS_DXCH_QOS_PROFILE_NUM_MAX_CNS;

        st = cpssDxChCosMacQosEntrySet(dev, index, &macCfg);
        if (devFamily != CPSS_PP_FAMILY_CHEETAH_E)
        {
            UTF_VERIFY_EQUAL3_STRING_MAC(GT_OK, st,
                "%d, %d, macQosCfg_PTR->qosProfileId = %d", dev, index, macCfg.qosProfileId);
        }
        else
        {
            UTF_VERIFY_NOT_EQUAL2_STRING_MAC(GT_OK, st,
                "%d, macQosCfg_PTR->qosProfileId = %d", dev, macCfg.qosProfileId);
        }


        /*
            1.1.6. Call function with out of range portQosGfgPtr->qosProfileId
                            portQosGfgPtr [PRV_CPSS_DXCH2_QOS_PROFILE_NUM_MAX_CNS = 128]
                            and valid other parameters.
            Expected: NOT GT_OK.
        */
        macCfg.qosProfileId = PRV_CPSS_DXCH2_QOS_PROFILE_NUM_MAX_CNS;

        st = cpssDxChCosMacQosEntrySet(dev, index, &macCfg);
        UTF_VERIFY_NOT_EQUAL2_STRING_MAC(GT_OK, st,
            "%d, macQosCfg_PTR->qosProfileId = %d", dev, macCfg.qosProfileId);

        /*
            1.1.7. Call with entryIdx [4],
                             out of range macQosCfg_PTR->assignPrecedence {2,
                                    wrong enum values,
                                    CPSS_PACKET_ATTRIBUTE_MODIFY_KEEP_PREVIOUS_E,
                                    CPSS_PACKET_ATTRIBUTE_MODIFY_KEEP_PREVIOUS_E}.
            Expected: GT_BAD_PARAM.
        */
        index = 4;
        macCfg.qosProfileId = 2;

        UTF_ENUMS_CHECK_MAC(cpssDxChCosMacQosEntrySet
                            (dev, index, &macCfg),
                            macCfg.assignPrecedence);

        /*
            1.1.8. Call with entryIdx [4],
                             out of range macQosCfg_PTR->enableModifyUp {2,
                                    CPSS_PACKET_ATTRIBUTE_ASSIGN_PRECEDENCE_HARD_E,
                                    wrong enum values,
                                    CPSS_PACKET_ATTRIBUTE_MODIFY_KEEP_PREVIOUS_E}.
            Expected: GT_BAD_PARAM.
        */
        macCfg.assignPrecedence = CPSS_PACKET_ATTRIBUTE_ASSIGN_PRECEDENCE_HARD_E;

        UTF_ENUMS_CHECK_MAC(cpssDxChCosMacQosEntrySet
                            (dev, index, &macCfg),
                            macCfg.enableModifyUp);

        /*
            1.1.9. Call with entryIdx [5],
                   out of range macQosCfg_PTR->enableModifyDscp {3,
                            CPSS_PACKET_ATTRIBUTE_ASSIGN_PRECEDENCE_HARD_E,
                            CPSS_PACKET_ATTRIBUTE_MODIFY_ENABLE_E,
                            wrong enum values}.
            Expected: GT_BAD_PARAM.
        */
        macCfg.enableModifyUp = CPSS_PACKET_ATTRIBUTE_MODIFY_ENABLE_E;

        UTF_ENUMS_CHECK_MAC(cpssDxChCosMacQosEntrySet
                            (dev, index, &macCfg),
                            macCfg.enableModifyDscp);
    }

    index = 1;
    macCfg.qosProfileId     = 1;
    macCfg.assignPrecedence = CPSS_PACKET_ATTRIBUTE_ASSIGN_PRECEDENCE_SOFT_E;
    macCfg.enableModifyUp   = CPSS_PACKET_ATTRIBUTE_MODIFY_KEEP_PREVIOUS_E;
    macCfg.enableModifyDscp = CPSS_PACKET_ATTRIBUTE_MODIFY_KEEP_PREVIOUS_E;

    /* 2. For not-active devices and devices from non-applicable family */
    /* check that function returns GT_BAD_PARAM.                        */

    /* prepare device iterator */
    PRV_TGF_NOT_APPLIC_DEV_RESET_MAC(&dev, UTF_NONE_FAMILY_E);
    /* go over all non active devices */
    while(GT_OK == prvUtfNextDeviceGet(&dev, GT_FALSE))
    {
        st = cpssDxChCosMacQosEntrySet(dev, index, &macCfg);
        UTF_VERIFY_EQUAL1_PARAM_MAC(GT_NOT_APPLICABLE_DEVICE, st, dev);
    }

    /* 3. Call function with out of bound value for device id.*/

    dev = PRV_CPSS_MAX_PP_DEVICES_CNS;

    st = cpssDxChCosMacQosEntrySet(dev, index, &macCfg);
    UTF_VERIFY_EQUAL1_PARAM_MAC(GT_BAD_PARAM, st, dev);
}

/*----------------------------------------------------------------------------*/
/*
GT_STATUS cpssDxChCosMacQosEntryGet
(
    IN  GT_U8               dev,
    IN  GT_U32              entryIdx,
    OUT CPSS_QOS_ENTRY_STC *macQosCfg_PTR
)
*/
UTF_TEST_CASE_MAC(cpssDxChCosMacQosEntryGet)
{
/*
    ITERATE_DEVICES (DxChx)
    1.1. Call function with entryIdx [4],
                            non-null macQosCfg_PTR.
    Expected: GT_OK.
    1.2. Call function with out of range entryIdx [0/ 8]
                            and non-null macQosCfg_PTR.
    Expected: NOT GT_OK.
    1.3. Call function with entryIdx [2],
                            null macQosCfg_PTR [NULL].
    Expected: GT_BAD_PTR.
*/
    GT_STATUS          st    = GT_OK;

    GT_U8              dev;
    GT_U8              index = 0;
    CPSS_QOS_ENTRY_STC macCfg;

    /* prepare device iterator */
    PRV_TGF_NOT_APPLIC_DEV_RESET_MAC(&dev, UTF_NONE_FAMILY_E);

    /* 1. Go over all active devices. */
    while (GT_OK == prvUtfNextDeviceGet(&dev, GT_TRUE))
    {
        /*
            1.1. Call function with entryIdx  [4],
                                    non-null macQosCfg_PTR.
            Expected: GT_OK.
        */
        index = 4;

        st = cpssDxChCosMacQosEntryGet(dev, index, &macCfg);
        UTF_VERIFY_EQUAL2_PARAM_MAC(GT_OK, st, dev, index);

        /*
            1.2. Call function with out of range entryIdx [0/ 8]
                                    and non-null macQosCfg_PTR.
            Expected: NOT GT_OK.
        */
        index = 0;

        st = cpssDxChCosMacQosEntryGet(dev, index, &macCfg);
        UTF_VERIFY_NOT_EQUAL2_PARAM_MAC(GT_OK, st, dev, index);

        index = 8;

        st = cpssDxChCosMacQosEntryGet(dev, index, &macCfg);
        UTF_VERIFY_NOT_EQUAL2_PARAM_MAC(GT_OK, st, dev, index);

        /*
            1.3. Call function with entryIdx [2],
                                    null macQosCfg_PTR [NULL].
            Expected: GT_BAD_PTR.
        */
        index = 2;

        st = cpssDxChCosMacQosEntryGet(dev, index, NULL);
        UTF_VERIFY_EQUAL1_STRING_MAC(GT_BAD_PTR, st, "%d, macQosCfg_PTR = NULL",
                                     dev);
    }

    index = 4;

    /* 2. For not-active devices and devices from non-applicable family */
    /* check that function returns GT_BAD_PARAM.                        */

    /* prepare device iterator */
    PRV_TGF_NOT_APPLIC_DEV_RESET_MAC(&dev, UTF_NONE_FAMILY_E);

    /* go over all non active devices */
    while(GT_OK == prvUtfNextDeviceGet(&dev, GT_FALSE))
    {
        st = cpssDxChCosMacQosEntryGet(dev, index, &macCfg);
        UTF_VERIFY_EQUAL1_PARAM_MAC(GT_NOT_APPLICABLE_DEVICE, st, dev);
    }

    /* 3. Call function with out of bound value for device id.*/

    dev = PRV_CPSS_MAX_PP_DEVICES_CNS;

    st = cpssDxChCosMacQosEntryGet(dev, index, &macCfg);
    UTF_VERIFY_EQUAL1_PARAM_MAC(GT_BAD_PARAM, st, dev);
}

/*----------------------------------------------------------------------------*/
/*
GT_STATUS cpssDxChCosMacQosConflictResolutionSet
(
    IN GT_U8                        dev,
    IN CPSS_MAC_QOS_RESOLVE_ENT     macQosResolvType
)
*/
UTF_TEST_CASE_MAC(cpssDxChCosMacQosConflictResolutionSet)
{
/*
    ITERATE_DEVICES (DxChx)
    1.1. Call function with macQosResolvType [CPSS_MAC_QOS_GET_FROM_DA_E /
                                              CPSS_MAC_QOS_GET_FROM_SA_E].
    Expected: GT_OK.
    1.2. Call cpssDxChCosMacQosConflictResolutionGet.
    Expected: GT_OK and the same macQosResolvType.
    1.3. Call function with wrong enum values macQosResolvType .
    Expected: GT_BAD_PARAM.
*/
    GT_STATUS                st      = GT_OK;

    GT_U8                    dev;
    CPSS_MAC_QOS_RESOLVE_ENT macType = CPSS_MAC_QOS_GET_FROM_DA_E;
    CPSS_MAC_QOS_RESOLVE_ENT macGet  = CPSS_MAC_QOS_GET_FROM_DA_E;

    /* prepare device iterator */
    PRV_TGF_NOT_APPLIC_DEV_RESET_MAC(&dev, UTF_NONE_FAMILY_E);

    /* 1. Go over all active devices. */
    while (GT_OK == prvUtfNextDeviceGet(&dev, GT_TRUE))
    {
        /*
           1.1. Call function with macQosResolvType [CPSS_MAC_QOS_GET_FROM_DA_E /
                                                     CPSS_MAC_QOS_GET_FROM_SA_E].
           Expected: GT_OK.
        */

        /*
            1.2. Call cpssDxChCosMacQosConflictResolutionGet.
            Expected: GT_OK and the same macQosResolvType.
        */

        /* 1.1. CAll with macQosResolvType [CPSS_MAC_QOS_GET_FROM_DA_E] */
        macType = CPSS_MAC_QOS_GET_FROM_DA_E;

        st = cpssDxChCosMacQosConflictResolutionSet(dev, macType);
        UTF_VERIFY_EQUAL2_PARAM_MAC(GT_OK, st, dev, macType);

        /* 1.2. CAll with macQosResolvType [CPSS_MAC_QOS_GET_FROM_DA_E] */
        st = cpssDxChCosMacQosConflictResolutionGet(dev, &macGet);
        UTF_VERIFY_EQUAL1_STRING_MAC(GT_OK, st,
                       "cpssDxChPortCpuRxBufLimitGet: %d", dev);
        UTF_VERIFY_EQUAL1_STRING_MAC(macType, macGet,
                       "get another macQosResolvType than was set: %d", dev);

        /* 1.1. CAll with macQosResolvType [CPSS_MAC_QOS_GET_FROM_SA_E] */
        macType = CPSS_MAC_QOS_GET_FROM_SA_E;

        st = cpssDxChCosMacQosConflictResolutionSet(dev, macType);
        UTF_VERIFY_EQUAL2_PARAM_MAC(GT_OK, st, dev, macType);

        /* 1.2. CAll with macQosResolvType [CPSS_MAC_QOS_GET_FROM_SA_E] */
        st = cpssDxChCosMacQosConflictResolutionGet(dev, &macGet);
        UTF_VERIFY_EQUAL1_STRING_MAC(GT_OK, st,
                       "cpssDxChPortCpuRxBufLimitGet: %d", dev);
        UTF_VERIFY_EQUAL1_STRING_MAC(macType, macGet,
                       "get another macQosResolvType than was set: %d", dev);

        /*
            1.3. Call function with wrong enum values macQosResolvType .
            Expected: GT_BAD_PARAM.
        */
        UTF_ENUMS_CHECK_MAC(cpssDxChCosMacQosConflictResolutionSet
                            (dev, macType),
                            macType);
    }

    macType = CPSS_MAC_QOS_GET_FROM_DA_E;

    /* 2. For not-active devices and devices from non-applicable family */
    /* check that function returns GT_BAD_PARAM.                        */

    /* prepare device iterator */
    PRV_TGF_NOT_APPLIC_DEV_RESET_MAC(&dev, UTF_NONE_FAMILY_E);

    /* go over all non active devices */
    while(GT_OK == prvUtfNextDeviceGet(&dev, GT_FALSE))
    {
        st = cpssDxChCosMacQosConflictResolutionSet(dev, macType);
        UTF_VERIFY_EQUAL1_PARAM_MAC(GT_NOT_APPLICABLE_DEVICE, st, dev);
    }

    /* 3. Call function with out of bound value for device id.*/
    dev = PRV_CPSS_MAX_PP_DEVICES_CNS;

    st = cpssDxChCosMacQosConflictResolutionSet(dev, macType);
    UTF_VERIFY_EQUAL1_PARAM_MAC(GT_BAD_PARAM, st, dev);
}

/*----------------------------------------------------------------------------*/
/*
GT_STATUS cpssDxChCosMacQosConflictResolutionGet
(
    IN GT_U8                        dev,
    IN CPSS_MAC_QOS_RESOLVE_ENT    *macQosResolvTypePtr
)
*/
UTF_TEST_CASE_MAC(cpssDxChCosMacQosConflictResolutionGet)
{
/*
    ITERATE_DEVICES (DxChx)
    1.1. Call function with non-null macQosResolvTypePtr.
    Expected: GT_OK.
    1.2. Call function with null macQosResolvTypePtr [NULL].
    Expected: GT_BAD_PTR.
*/
    GT_STATUS                st      = GT_OK;

    GT_U8                    dev;
    CPSS_MAC_QOS_RESOLVE_ENT macType = CPSS_MAC_QOS_GET_FROM_DA_E;

    /* prepare device iterator */
    PRV_TGF_NOT_APPLIC_DEV_RESET_MAC(&dev, UTF_NONE_FAMILY_E);

    /* 1. Go over all active devices. */
    while (GT_OK == prvUtfNextDeviceGet(&dev, GT_TRUE))
    {
        /*
            1.1. Call function with non-null macQosResolvTypePtr.
            Expected: GT_OK.
        */
        st = cpssDxChCosMacQosConflictResolutionGet(dev, &macType);
        UTF_VERIFY_EQUAL1_PARAM_MAC(GT_OK, st, dev);

        /*
            1.2. Call function with null macQosResolvTypePtr [NULL].
            Expected: GT_BAD_PTR.
        */
        st = cpssDxChCosMacQosConflictResolutionGet(dev, NULL);
        UTF_VERIFY_EQUAL1_STRING_MAC(GT_BAD_PTR, st, "%d, NULL", dev);
    }

    /* 2. For not-active devices and devices from non-applicable family */
    /* check that function returns GT_BAD_PARAM.                        */

    /* prepare device iterator */
    PRV_TGF_NOT_APPLIC_DEV_RESET_MAC(&dev, UTF_NONE_FAMILY_E);

    /* go over all non active devices */
    while(GT_OK == prvUtfNextDeviceGet(&dev, GT_FALSE))
    {
        st = cpssDxChCosMacQosConflictResolutionGet(dev, &macType);
        UTF_VERIFY_EQUAL1_PARAM_MAC(GT_NOT_APPLICABLE_DEVICE, st, dev);
    }

    /* 3. Call function with out of bound value for device id.*/

    dev = PRV_CPSS_MAX_PP_DEVICES_CNS;

    st = cpssDxChCosMacQosConflictResolutionGet(dev, &macType);
    UTF_VERIFY_EQUAL1_PARAM_MAC(GT_BAD_PARAM, st, dev);
}

/*----------------------------------------------------------------------------*/
/*
GT_STATUS cpssDxChCosRemarkTblEntrySet
(
    IN GT_U8    dev,
    IN GT_U32   profileIndex,
    IN GT_U32   newProfileIndex
)
*/
UTF_TEST_CASE_MAC(cpssDxChCosRemarkTblEntrySet)
{
/*
    ITERATE_DEVICES (DxCh1, DxCh2)
    1.1. Call function with profileIndex [1],
                            newProfileIndex[2].
    Expected: GT_OK.
    1.2. Call cpssDxChCosRemarkTblEntryGet with same profile profileIndex
    Expected: GT_OK and the same newProfileIndex.
    1.3. Call function with out of range profileIndex [PRV_CPSS_DXCH_QOS_PROFILE_NUM_MAX_CNS = 72],
                            newProfileIndex[5].
    Expected: NOT GT_OK for Cheetah and GT_OK for Cheetah-2.
    1.4. Call function with out of range newprofileIndex [PRV_CPSS_DXCH2_QOS_PROFILE_NUM_MAX_CNS = 128]
                            and valid other parameters.
    Expected: NOT GT_OK.
    1.5. Call function with profileIndex [7],
                            out of range newProfileIndex[PRV_CPSS_DXCH_QOS_PROFILE_NUM_MAX_CNS = 72].
    Expected: NOT GT_OK for Cheetah and GT_OK for Cheetah-2.
    1.6. Call function with out of range profileIndex [PRV_CPSS_DXCH2_QOS_PROFILE_NUM_MAX_CNS = 128]
                            and valid other parameters.
    Expected: NOT GT_OK.
*/
    GT_STATUS               st         = GT_OK;

    GT_U8                   dev;
    GT_U32                  profile    = 0;
    GT_U32                  newProfile = 0;
    GT_U32                  pfGet      = 0;
    CPSS_PP_FAMILY_TYPE_ENT devFamily;

    /* prepare device iterator */
    PRV_TGF_NOT_APPLIC_DEV_RESET_MAC(&dev, UTF_CH3_E | UTF_XCAT_E | UTF_LION_E | UTF_XCAT2_E);

    /* 1. Go over all active devices. */
    while (GT_OK == prvUtfNextDeviceGet(&dev, GT_TRUE))
    {
        /*
           1.1. Call function with profileIndex [1],
                                   newProfileIndex[2].
            Expected: GT_OK.
        */
        profile    = 1;
        newProfile = 2;

        st = cpssDxChCosRemarkTblEntrySet(dev, profile, newProfile);
        UTF_VERIFY_EQUAL3_PARAM_MAC(GT_OK, st, dev, profile, newProfile);

        /*
            1.2. Call cpssDxChCosRemarkTblEntryGet with same profile profileIndex
            Expected: GT_OK and the same newProfileIndex.
        */
        st = cpssDxChCosRemarkTblEntryGet(dev, profile, &pfGet);
        UTF_VERIFY_EQUAL2_STRING_MAC(GT_OK, st,
                       "cpssDxChCosRemarkTblEntryGet: %d, %d", dev, profile);
        UTF_VERIFY_EQUAL1_STRING_MAC(newProfile, pfGet,
                       "get another newProfileIndex than was set: %d", dev);

        /* Getting device family */
        st = prvUtfDeviceFamilyGet(dev, &devFamily);
        UTF_VERIFY_EQUAL1_STRING_MAC(GT_OK, st, "prvUtfDeviceFamilyGet: %d", dev);

        /*
            1.3. Call function with out of range profileIndex [PRV_CPSS_DXCH_QOS_PROFILE_NUM_MAX_CNS = 72],
                                    newProfileIndex[5].
            Expected: NOT GT_OK for Cheetah and GT_OK for Cheetah-2.
        */
        profile    = PRV_CPSS_DXCH_QOS_PROFILE_NUM_MAX_CNS;
        newProfile = 5;

        st = cpssDxChCosRemarkTblEntrySet(dev, profile, newProfile);
        if(devFamily != CPSS_PP_FAMILY_CHEETAH_E)
        {
            UTF_VERIFY_EQUAL3_PARAM_MAC(GT_OK, st, dev, profile, newProfile);
        }
        else
        {
            UTF_VERIFY_NOT_EQUAL2_PARAM_MAC(GT_OK, st, dev, profile);
        }

        /*
            1.4. Call function with out of range newprofileIndex [PRV_CPSS_DXCH2_QOS_PROFILE_NUM_MAX_CNS = 128]
                                    and valid other parameters.
            Expected: NOT GT_OK.
        */
        profile    = 1;
        newProfile = PRV_CPSS_DXCH2_QOS_PROFILE_NUM_MAX_CNS;

        st = cpssDxChCosRemarkTblEntrySet(dev, profile, newProfile);
        UTF_VERIFY_NOT_EQUAL2_STRING_MAC(GT_OK, st, "%d, newprofileIndex = %d",
                                         dev, newProfile);

        /*
            1.5. Call function with profileIndex [7],
                            out of range newProfileIndex[PRV_CPSS_DXCH_QOS_PROFILE_NUM_MAX_CNS = 72].
            Expected: NOT GT_OK for Cheetah and GT_OK for Cheetah-2.
        */
        profile    = 7;
        newProfile = PRV_CPSS_DXCH_QOS_PROFILE_NUM_MAX_CNS;

        st = cpssDxChCosRemarkTblEntrySet(dev, profile, newProfile);
        if(devFamily != CPSS_PP_FAMILY_CHEETAH_E)
        {
            UTF_VERIFY_EQUAL3_PARAM_MAC(GT_OK, st, dev, profile, newProfile);
        }
        else
        {
            UTF_VERIFY_NOT_EQUAL2_STRING_MAC(GT_OK, st, "%d, newProfileIndex = %d",
                                             dev, newProfile);
        }

        /*
            1.6. Call function with out of range profileIndex [PRV_CPSS_DXCH2_QOS_PROFILE_NUM_MAX_CNS = 128]
                                    and valid other parameters.
            Expected: NOT GT_OK.
        */
        profile    = PRV_CPSS_DXCH2_QOS_PROFILE_NUM_MAX_CNS;
        newProfile = 2;

        st = cpssDxChCosRemarkTblEntrySet(dev, profile, newProfile);
        UTF_VERIFY_NOT_EQUAL2_PARAM_MAC(GT_OK, st, dev, profile);
    }

    profile    = 1;
    newProfile = 2;

    /* 2. For not-active devices and devices from non-applicable family */
    /* check that function returns GT_BAD_PARAM.                        */

    /* prepare device iterator */
    PRV_TGF_NOT_APPLIC_DEV_RESET_MAC(&dev, UTF_CH3_E | UTF_XCAT_E | UTF_LION_E | UTF_XCAT2_E);

    /* go over all non active devices */
    while(GT_OK == prvUtfNextDeviceGet(&dev, GT_FALSE))
    {
        st = cpssDxChCosRemarkTblEntrySet(dev, profile, newProfile);
        UTF_VERIFY_EQUAL1_PARAM_MAC(GT_NOT_APPLICABLE_DEVICE, st, dev);
    }

    /* 3. Call function with out of bound value for device id.*/

    dev = PRV_CPSS_MAX_PP_DEVICES_CNS;

    st = cpssDxChCosRemarkTblEntrySet(dev, profile, newProfile);
    UTF_VERIFY_EQUAL1_PARAM_MAC(GT_BAD_PARAM, st, dev);
}

/*----------------------------------------------------------------------------*/
/*
GT_STATUS cpssDxChCosRemarkTblEntryGet
(
    IN GT_U8    dev,
    IN GT_U32   profileIndex,
    IN GT_U32  *newProfileIndexPtr
)
*/
UTF_TEST_CASE_MAC(cpssDxChCosRemarkTblEntryGet)
{
/*
    ITERATE_DEVICES (DxCh1, DxCh2)
    1.1. Call function with profileIndex [4],
                            non-null profileIndexPtr.
    Expected: GT_OK.
    1.2. Call function with out of range profileIndex [72]
                            and non-null profileIndexPtr.
    Expected: NOT GT_OK for Cheetah and GT_OK for Cheetah-2.
    1.3. Call function with out of range profileIndex [PRV_CPSS_DXCH2_QOS_PROFILE_NUM_MAX_CNS = 128]
                            and valid other parameters.
    Expected: NOT GT_OK.
    1.4. Call function with profileIndex [2],
                            null profileIndexPtr [NULL].
    Expected: GT_BAD_PTR.
*/
    GT_STATUS               st         = GT_OK;

    GT_U8                   dev;
    GT_U32                  profile    = 0;
    GT_U32                  newProfile = 0;
    CPSS_PP_FAMILY_TYPE_ENT devFamily;

    /* prepare device iterator */
    PRV_TGF_NOT_APPLIC_DEV_RESET_MAC(&dev, UTF_CH3_E | UTF_XCAT_E | UTF_LION_E | UTF_XCAT2_E);

    /* 1. Go over all active devices. */
    while (GT_OK == prvUtfNextDeviceGet(&dev, GT_TRUE))
    {
        /*
            1.1. Call function with profileIndex [4],
                                    non-null profileIndexPtr.
            Expected: GT_OK.
        */
        profile = 4;

        st = cpssDxChCosRemarkTblEntryGet(dev, profile, &newProfile);
        UTF_VERIFY_EQUAL2_PARAM_MAC(GT_OK, st, dev, profile);

        /* Getting device family */
        st = prvUtfDeviceFamilyGet(dev, &devFamily);
        UTF_VERIFY_EQUAL1_STRING_MAC(GT_OK, st, "prvUtfDeviceFamilyGet: %d", dev);

        /*
            1.2. Call function with out of range profileIndex [72]
                            and non-null profileIndexPtr.
            Expected: NOT GT_OK for Cheetah and GT_OK for Cheetah-2.
        */
        profile = PRV_CPSS_DXCH_QOS_PROFILE_NUM_MAX_CNS;

        st = cpssDxChCosRemarkTblEntryGet(dev, profile, &newProfile);
        if(devFamily != CPSS_PP_FAMILY_CHEETAH_E)
        {
            UTF_VERIFY_EQUAL2_PARAM_MAC(GT_OK, st, dev, profile);
        }
        else
        {
            UTF_VERIFY_NOT_EQUAL2_PARAM_MAC(GT_OK, st, dev, profile);
        }

        /*
            1.3. Call function with out of range profileIndex [PRV_CPSS_DXCH2_QOS_PROFILE_NUM_MAX_CNS = 128]
                                                 and valid other parameters.
            Expected: NOT GT_OK.
        */
        profile = PRV_CPSS_DXCH2_QOS_PROFILE_NUM_MAX_CNS;

        st = cpssDxChCosRemarkTblEntryGet(dev, profile, &newProfile);
        UTF_VERIFY_NOT_EQUAL2_PARAM_MAC(GT_OK, st, dev, profile);

        /*
            1.4. Call function with profileIndex [2],
                                    null profileIndexPtr [NULL].
            Expected: GT_BAD_PTR.
        */
        profile    = 2;

        st = cpssDxChCosRemarkTblEntryGet(dev, profile, NULL);
        UTF_VERIFY_EQUAL1_STRING_MAC(GT_BAD_PTR, st, "%d, newProfileIndexPtr = NULL", dev);
    }

    profile    = 1;

    /* 2. For not-active devices and devices from non-applicable family */
    /* check that function returns GT_BAD_PARAM.                        */

    /* prepare device iterator */
    PRV_TGF_NOT_APPLIC_DEV_RESET_MAC(&dev, UTF_CH3_E | UTF_XCAT_E | UTF_LION_E | UTF_XCAT2_E);

    /* go over all non active devices */
    while(GT_OK == prvUtfNextDeviceGet(&dev, GT_FALSE))
    {
        st = cpssDxChCosRemarkTblEntryGet(dev, profile, &newProfile);
        UTF_VERIFY_EQUAL1_PARAM_MAC(GT_NOT_APPLICABLE_DEVICE, st, dev);
    }

    /* 3. Call function with out of bound value for device id.*/

    dev = PRV_CPSS_MAX_PP_DEVICES_CNS;

    st = cpssDxChCosRemarkTblEntryGet(dev, profile, &newProfile);
    UTF_VERIFY_EQUAL1_PARAM_MAC(GT_BAD_PARAM, st, dev);
}

/*----------------------------------------------------------------------------*/
/*
GT_STATUS cpssDxChPortDefaultUPSet
(
    IN GT_U8    dev,
    IN GT_U8    port,
    IN GT_U8    defaultUserPrio
)
*/
UTF_TEST_CASE_MAC(cpssDxChPortDefaultUPSet)
{
/*
    ITERATE_DEVICES_VIRT_PORTS (DxChx)
    1.1.1. Call with defaultUserPrio [1].
    Expected: GT_OK.
    1.1.2. Call cpssDxChPortDefaultUPGet
    Expected: GT_OK and the same defaultUserPrioPtr.
    1.1.3. Call with out of range defaultUserPrio [CPSS_USER_PRIORITY_RANGE_CNS = 8]
    Expected: NOT GT_OK.
    1.2. Call with port [CPSS_CPU_PORT_NUM_CNS = 63] and valid other parameters.
    Expected: GT_OK.
*/
    GT_STATUS st       = GT_OK;

    GT_U8     dev;
    GT_U8     port     = COS_VALID_VIRT_PORT_CNS;
    GT_U8     userPrio = 0;
    GT_U8     prioGet  = 0;

    /* prepare iterator for go over all active devices */
    PRV_TGF_NOT_APPLIC_DEV_RESET_MAC(&dev, UTF_NONE_FAMILY_E);

    /* 1. Go over all active devices. */
    while (GT_OK == prvUtfNextDeviceGet(&dev, GT_TRUE))
    {
        st = prvUtfNextVirtPortReset(&port, dev);
        UTF_VERIFY_EQUAL0_PARAM_MAC(GT_OK, st);

        /* 1.1. For all active devices go over all available virtual ports. */
        while (GT_OK == prvUtfNextVirtPortGet(&port, GT_TRUE))
        {
            /*
                1.1.1. Call with defaultUserPrio [1].
                Expected: GT_OK.
            */
            userPrio = 1;

            st = cpssDxChPortDefaultUPSet(dev, port, userPrio);
            UTF_VERIFY_EQUAL3_PARAM_MAC(GT_OK, st, dev, port, userPrio);

            /*
                1.1.2. Call cpssDxChPortDefaultUPGet
                Expected: GT_OK and the same defaultUserPrioPtr.
            */
            st = cpssDxChPortDefaultUPGet(dev, port, &prioGet);
            UTF_VERIFY_EQUAL2_STRING_MAC(GT_OK, st,
                       "cpssDxChPortDefaultUPGet: %d, %d", dev, port);

            UTF_VERIFY_EQUAL2_STRING_MAC(userPrio, prioGet,
                       "get another defaultUserPrio than was set: %d, %d", dev, port);

            /*
                1.1.3. Call with out of range defaultUserPrio [CPSS_USER_PRIORITY_RANGE_CNS = 8]
                Expected: NOT GT_OK.
            */
            userPrio = CPSS_USER_PRIORITY_RANGE_CNS;

            st = cpssDxChPortDefaultUPSet(dev, port, userPrio);
            UTF_VERIFY_NOT_EQUAL3_PARAM_MAC(GT_OK, st, dev, port, userPrio);
        }

        userPrio = 1;

        st = prvUtfNextVirtPortReset(&port, dev);
        UTF_VERIFY_EQUAL0_PARAM_MAC(GT_OK, st);
        /* 1.2. For all active devices go over all non available virtual ports. */
        while (GT_OK == prvUtfNextVirtPortGet(&port, GT_FALSE))
        {
            /* 1.2.1. Call function for each non-active port */
            st = cpssDxChPortDefaultUPSet(dev, port, userPrio);
            UTF_VERIFY_EQUAL2_PARAM_MAC(GT_BAD_PARAM, st, dev, port);
        }

        /* 1.3. For active device check that function returns GT_BAD_PARAM */
        /* for out of bound value for port number.                         */
        port = PRV_CPSS_MAX_PP_PORTS_NUM_CNS;

        st = cpssDxChPortDefaultUPSet(dev, port, userPrio);
        UTF_VERIFY_EQUAL2_PARAM_MAC(GT_BAD_PARAM, st, dev, port);

        /* 1.4. For active device check that function returns GT_OK        */
        /* for CPU port number.                                            */
        port = CPSS_CPU_PORT_NUM_CNS;

        st = cpssDxChPortDefaultUPSet(dev, port, userPrio);
        UTF_VERIFY_EQUAL2_PARAM_MAC(GT_OK, st, dev, port);
    }

    userPrio = 1;

    /* 2. For not active devices check that function returns non GT_OK.*/
    port = COS_VALID_VIRT_PORT_CNS;

    /* prepare device iterator */
    PRV_TGF_NOT_APPLIC_DEV_RESET_MAC(&dev, UTF_NONE_FAMILY_E);

    /* Go over all non active devices. */
    while(GT_OK == prvUtfNextDeviceGet(&dev, GT_FALSE))
    {
        st = cpssDxChPortDefaultUPSet(dev, port, userPrio);
        UTF_VERIFY_EQUAL1_PARAM_MAC(GT_NOT_APPLICABLE_DEVICE, st, dev);
    }

    /* 3.Call function with out of bound value for device id */
    dev = PRV_CPSS_MAX_PP_DEVICES_CNS;

    st = cpssDxChPortDefaultUPSet(dev, port, userPrio);
    UTF_VERIFY_EQUAL1_PARAM_MAC(GT_BAD_PARAM, st, dev);
}

/*----------------------------------------------------------------------------*/
/*
GT_STATUS cpssDxChPortDefaultUPGet
(
    IN GT_U8    dev,
    IN GT_U8    port,
    IN GT_U8    *defaultUserPrioPtr
)
*/
UTF_TEST_CASE_MAC(cpssDxChPortDefaultUPGet)
{
/*
    ITERATE_DEVICES_VIRT_PORTS (DxChx)
    1.1.1. Call with non-null defaultUserPrioPtr.
    Expected: GT_OK.
    1.1.2. Call with defaultUserPrioPtr [NULL].
    Expected: GT_BAD_PTR.
    1.2. Call with port [CPSS_CPU_PORT_NUM_CNS = 63] and valid other parameters.
    Expected: GT_OK.
*/
    GT_STATUS   st       = GT_OK;

    GT_U8       dev;
    GT_U8       port     = COS_VALID_VIRT_PORT_CNS;
    GT_U8       userPrio = 0;

    /* prepare iterator for go over all active devices */
    PRV_TGF_NOT_APPLIC_DEV_RESET_MAC(&dev, UTF_NONE_FAMILY_E);

    /* 1. Go over all active devices. */
    while (GT_OK == prvUtfNextDeviceGet(&dev, GT_TRUE))
    {
        st = prvUtfNextVirtPortReset(&port, dev);
        UTF_VERIFY_EQUAL0_PARAM_MAC(GT_OK, st);

        /* 1.1. For all active devices go over all available virtual ports. */
        while (GT_OK == prvUtfNextVirtPortGet(&port, GT_TRUE))
        {
            /*
                1.1.1. Call with non-null defaultUserPrioPtr.
                Expected: GT_OK.
            */
            st = cpssDxChPortDefaultUPGet(dev, port, &userPrio);
            UTF_VERIFY_EQUAL2_PARAM_MAC(GT_OK, st, dev, port);

            /*
                1.1.2. Call with defaultUserPrioPtr [NULL].
                Expected: GT_BAD_PTR.
            */
            st = cpssDxChPortDefaultUPGet(dev, port, NULL);
            UTF_VERIFY_EQUAL2_STRING_MAC(GT_BAD_PTR, st, "%d, %d, NULL", dev, port);
        }

        st = prvUtfNextVirtPortReset(&port, dev);
        UTF_VERIFY_EQUAL0_PARAM_MAC(GT_OK, st);

        /* 1.2. For all active devices go over all non available
           virtual ports.
        */
        while (GT_OK == prvUtfNextVirtPortGet(&port, GT_FALSE))
        {
            st = cpssDxChPortDefaultUPGet(dev, port, &userPrio);
            UTF_VERIFY_EQUAL2_PARAM_MAC(GT_BAD_PARAM, st, dev, port);
        }

        /* 1.3. For active device check that function returns GT_BAD_PARAM */
        /* for out of bound value for port number.                         */
        port = PRV_CPSS_MAX_PP_PORTS_NUM_CNS;

        st = cpssDxChPortDefaultUPGet(dev, port, &userPrio);
        UTF_VERIFY_EQUAL2_PARAM_MAC(GT_BAD_PARAM, st, dev, port);

        /* 1.4. For active device check that function returns GT_OK        */
        /* for CPU port number.                                            */
        port = CPSS_CPU_PORT_NUM_CNS;

        st = cpssDxChPortDefaultUPGet(dev, port, &userPrio);
        UTF_VERIFY_EQUAL2_PARAM_MAC(GT_OK, st, dev, port);
    }

    /* 2. For not active devices check that function returns non GT_OK.*/

    port = COS_VALID_VIRT_PORT_CNS;

    /* prepare device iterator */
    PRV_TGF_NOT_APPLIC_DEV_RESET_MAC(&dev, UTF_NONE_FAMILY_E);

    /* Go over all non active devices. */
    while(GT_OK == prvUtfNextDeviceGet(&dev, GT_FALSE))
    {
        st = cpssDxChPortDefaultUPGet(dev, port, &userPrio);
        UTF_VERIFY_EQUAL1_PARAM_MAC(GT_NOT_APPLICABLE_DEVICE, st, dev);
    }

    /* 3.Call function with out of bound value for device id */
    dev = PRV_CPSS_MAX_PP_DEVICES_CNS;

    st = cpssDxChPortDefaultUPGet(dev, port, &userPrio);
    UTF_VERIFY_EQUAL1_PARAM_MAC(GT_BAD_PARAM, st, dev);
}

/*----------------------------------------------------------------------------*/
/*
GT_STATUS cpssDxChPortModifyUPSet
(
    IN GT_U8   dev,
    IN GT_U8   port,
    IN GT_BOOL upOverrideEnable
)
*/
UTF_TEST_CASE_MAC(cpssDxChPortModifyUPSet)
{
/*
    ITERATE_DEVICES_VIRT_PORTS (DxChx)
    1.1.1. Call with upOverrideEnable [GT_FALSE, GT_TRUE].
    Expected: GT_OK.
    1.1.2. Call cpssDxChPortModifyUPGet
    Expected: GT_OK and the same upOverrideEnablePtr.
    1.2. Call with port [CPSS_CPU_PORT_NUM_CNS = 63] and valid other parameters.
    Expected: GT_OK.
*/
    GT_STATUS st       = GT_OK;

    GT_U8     dev;
    GT_U8     port     = COS_VALID_VIRT_PORT_CNS;
    GT_BOOL   state    = GT_FALSE;
    GT_BOOL   stateGet = GT_FALSE;

    /* prepare iterator for go over all active devices */
    PRV_TGF_NOT_APPLIC_DEV_RESET_MAC(&dev, UTF_NONE_FAMILY_E);

    /* 1. Go over all active devices. */
    while (GT_OK == prvUtfNextDeviceGet(&dev, GT_TRUE))
    {
        st = prvUtfNextVirtPortReset(&port, dev);
        UTF_VERIFY_EQUAL0_PARAM_MAC(GT_OK, st);

        /* 1.1. For all active devices go over all available virtual ports. */
        while (GT_OK == prvUtfNextVirtPortGet(&port, GT_TRUE))
        {
            /*
                1.1.1. Call with upOverrideEnable [GT_FALSE, GT_TRUE].
                Expected: GT_OK.
            */

            /* upOverrideEnable = GT_FALSE*/
            state = GT_FALSE;

            st = cpssDxChPortModifyUPSet(dev, port, state);
            UTF_VERIFY_EQUAL3_PARAM_MAC(GT_OK, st, dev, port, state);

            /*
                1.1.2. Call cpssDxChPortModifyUPGet
                Expected: GT_OK and the same upOverrideEnablePtr.
            */
            st = cpssDxChPortModifyUPGet(dev, port, &stateGet);
            UTF_VERIFY_EQUAL2_STRING_MAC(GT_OK, st,
                       "cpssDxChPortModifyUPGet: %d, %d", dev, port);

            UTF_VERIFY_EQUAL2_STRING_MAC(state, stateGet,
                       "get another upOverrideEnable than was set: %d, %d", dev, port);

            /* upOverrideEnable = GT_TRUE*/
            state = GT_TRUE;

            st = cpssDxChPortModifyUPSet(dev, port, state);
            UTF_VERIFY_EQUAL3_PARAM_MAC(GT_OK, st, dev, port, state);

            /*
                1.1.2. Call cpssDxChPortModifyUPGet
                Expected: GT_OK and the same upOverrideEnablePtr.
            */
            st = cpssDxChPortModifyUPGet(dev, port, &stateGet);
            UTF_VERIFY_EQUAL2_STRING_MAC(GT_OK, st,
                       "cpssDxChPortModifyUPGet: %d, %d", dev, port);

            UTF_VERIFY_EQUAL2_STRING_MAC(state, stateGet,
                       "get another upOverrideEnable than was set: %d, %d", dev, port);
        }

        state = GT_TRUE;

        st = prvUtfNextVirtPortReset(&port, dev);
        UTF_VERIFY_EQUAL0_PARAM_MAC(GT_OK, st);
        /* 1.2. For all active devices go over all non available virtual ports. */
        while (GT_OK == prvUtfNextVirtPortGet(&port, GT_FALSE))
        {
            /* 1.2.1. Call function for each non-active port */
            st = cpssDxChPortModifyUPSet(dev, port, state);
            UTF_VERIFY_EQUAL2_PARAM_MAC(GT_BAD_PARAM, st, dev, port);
        }

        /* 1.3. For active device check that function returns GT_BAD_PARAM */
        /* for out of bound value for port number.                         */
        port = PRV_CPSS_MAX_PP_PORTS_NUM_CNS;

        st = cpssDxChPortModifyUPSet(dev, port, state);
        UTF_VERIFY_EQUAL2_PARAM_MAC(GT_BAD_PARAM, st, dev, port);

        /* 1.4. For active device check that function returns GT_OK        */
        /* for CPU port number.                                            */
        port = CPSS_CPU_PORT_NUM_CNS;

        st = cpssDxChPortModifyUPSet(dev, port, state);
        UTF_VERIFY_EQUAL2_PARAM_MAC(GT_OK, st, dev, port);
    }

    state = GT_TRUE;

    /* 2. For not active devices check that function returns non GT_OK.*/
    port = COS_VALID_VIRT_PORT_CNS;

    /* prepare device iterator */
    PRV_TGF_NOT_APPLIC_DEV_RESET_MAC(&dev, UTF_NONE_FAMILY_E);

    /* Go over all non active devices. */
    while(GT_OK == prvUtfNextDeviceGet(&dev, GT_FALSE))
    {
        st = cpssDxChPortModifyUPSet(dev, port, state);
        UTF_VERIFY_EQUAL1_PARAM_MAC(GT_NOT_APPLICABLE_DEVICE, st, dev);
    }

    /* 3.Call function with out of bound value for device id */
    dev = PRV_CPSS_MAX_PP_DEVICES_CNS;

    st = cpssDxChPortModifyUPSet(dev, port, state);
    UTF_VERIFY_EQUAL1_PARAM_MAC(GT_BAD_PARAM, st, dev);
}

/*----------------------------------------------------------------------------*/
/*
GT_STATUS cpssDxChPortModifyUPGet
(
    IN GT_U8    dev,
    IN GT_U8    port,
    IN GT_BOOL *upOverrideEnablePtr
)
*/
UTF_TEST_CASE_MAC(cpssDxChPortModifyUPGet)
{
/*
    ITERATE_DEVICES_VIRT_PORTS (DxChx)
    1.1.1. Call with non-null upOverrideEnablePtr.
    Expected: GT_OK.
    1.1.2. Call with upOverrideEnablePtr [NULL].
    Expected: GT_BAD_PTR.
    1.2. Call with port [CPSS_CPU_PORT_NUM_CNS = 63] and valid other parameters.
    Expected: GT_OK.
*/
    GT_STATUS   st     = GT_OK;

    GT_U8       dev;
    GT_U8       port   = COS_VALID_VIRT_PORT_CNS;
    GT_BOOL     enable = GT_FALSE;

    /* prepare iterator for go over all active devices */
    PRV_TGF_NOT_APPLIC_DEV_RESET_MAC(&dev, UTF_NONE_FAMILY_E);

    /* 1. Go over all active devices. */
    while (GT_OK == prvUtfNextDeviceGet(&dev, GT_TRUE))
    {
        st = prvUtfNextVirtPortReset(&port, dev);
        UTF_VERIFY_EQUAL0_PARAM_MAC(GT_OK, st);

        /* 1.1. For all active devices go over all available virtual ports. */
        while (GT_OK == prvUtfNextVirtPortGet(&port, GT_TRUE))
        {
            /*
                1.1.1. Call with non-null upOverrideEnablePtr.
                Expected: GT_OK.
            */
            st = cpssDxChPortModifyUPGet(dev, port, &enable);
            UTF_VERIFY_EQUAL2_PARAM_MAC(GT_OK, st, dev, port);

            /*
                1.1.2. Call with upOverrideEnablePtr [NULL].
                Expected: GT_BAD_PTR.
            */
            st = cpssDxChPortModifyUPGet(dev, port, NULL);
            UTF_VERIFY_EQUAL2_STRING_MAC(GT_BAD_PTR, st, "%d, %d, NULL", dev, port);
        }

        st = prvUtfNextVirtPortReset(&port, dev);
        UTF_VERIFY_EQUAL0_PARAM_MAC(GT_OK, st);

        /* 1.2. For all active devices go over all non available
           virtual ports.
        */
        while (GT_OK == prvUtfNextVirtPortGet(&port, GT_FALSE))
        {
            st = cpssDxChPortModifyUPGet(dev, port, &enable);
            UTF_VERIFY_EQUAL2_PARAM_MAC(GT_BAD_PARAM, st, dev, port);
        }

        /* 1.3. For active device check that function returns GT_BAD_PARAM */
        /* for out of bound value for port number.                         */
        port = PRV_CPSS_MAX_PP_PORTS_NUM_CNS;

        st = cpssDxChPortModifyUPGet(dev, port, &enable);
        UTF_VERIFY_EQUAL2_PARAM_MAC(GT_BAD_PARAM, st, dev, port);

        /* 1.4. For active device check that function returns GT_OK        */
        /* for CPU port number.                                            */
        port = CPSS_CPU_PORT_NUM_CNS;

        st = cpssDxChPortModifyUPGet(dev, port, &enable);
        UTF_VERIFY_EQUAL2_PARAM_MAC(GT_OK, st, dev, port);
    }

    /* 2. For not active devices check that function returns non GT_OK.*/

    port = COS_VALID_VIRT_PORT_CNS;

    /* prepare device iterator */
    PRV_TGF_NOT_APPLIC_DEV_RESET_MAC(&dev, UTF_NONE_FAMILY_E);

    /* Go over all non active devices. */
    while(GT_OK == prvUtfNextDeviceGet(&dev, GT_FALSE))
    {
        st = cpssDxChPortModifyUPGet(dev, port, &enable);
        UTF_VERIFY_EQUAL1_PARAM_MAC(GT_NOT_APPLICABLE_DEVICE, st, dev);
    }

    /* 3.Call function with out of bound value for device id */
    dev = PRV_CPSS_MAX_PP_DEVICES_CNS;

    st = cpssDxChPortModifyUPGet(dev, port, &enable);
    UTF_VERIFY_EQUAL1_PARAM_MAC(GT_BAD_PARAM, st, dev);
}

/*----------------------------------------------------------------------------*/
/*
GT_STATUS cpssDxChCosPortQosTrustModeSet
(
    IN GT_U8                         dev,
    IN GT_U8                         port,
    IN CPSS_QOS_PORT_TRUST_MODE_ENT  portQosTrustMode
)
*/
UTF_TEST_CASE_MAC(cpssDxChCosPortQosTrustModeSet)
{
/*
    ITERATE_DEVICES_VIRT_PORTS (DxChx)
    1.1.1. Call with portQosTrustMode [CPSS_QOS_PORT_NO_TRUST_E /
                                       CPSS_QOS_PORT_TRUST_L2_E /
                                       CPSS_QOS_PORT_TRUST_L3_E /
                                       CPSS_QOS_PORT_TRUST_L2_L3_E].
    Expected: GT_OK.
    1.1.2. Call cpssDxChCosPortQosTrustModeGet
    Expected: GT_OK and the same portQosTrustModePtr.
    1.1.3. Call with wrong enum values portQosTrustMode
    Expected: GT_BAD_PARAM.
    1.2. Call with port [CPSS_CPU_PORT_NUM_CNS = 63] and valid other parameters.
    Expected: GT_OK.
*/
    GT_STATUS                     st      = GT_OK;

    GT_U8                         dev;
    GT_U8                         port    = COS_VALID_VIRT_PORT_CNS;
    CPSS_QOS_PORT_TRUST_MODE_ENT  mode    = CPSS_QOS_PORT_NO_TRUST_E;
    CPSS_QOS_PORT_TRUST_MODE_ENT  modeGet = CPSS_QOS_PORT_NO_TRUST_E;

    /* prepare iterator for go over all active devices */
    PRV_TGF_NOT_APPLIC_DEV_RESET_MAC(&dev, UTF_NONE_FAMILY_E);

    /* 1. Go over all active devices. */
    while (GT_OK == prvUtfNextDeviceGet(&dev, GT_TRUE))
    {
        st = prvUtfNextVirtPortReset(&port, dev);
        UTF_VERIFY_EQUAL0_PARAM_MAC(GT_OK, st);

        /* 1.1. For all active devices go over all available virtual ports. */
        while (GT_OK == prvUtfNextVirtPortGet(&port, GT_TRUE))
        {
            /*
                1.1.1. Call with portQosTrustMode [CPSS_QOS_PORT_NO_TRUST_E /
                                       CPSS_QOS_PORT_TRUST_L2_E /
                                       CPSS_QOS_PORT_TRUST_L3_E /
                                       CPSS_QOS_PORT_TRUST_L2_L3_E].
                Expected: GT_OK.
            */

            /* Call with portQosTrustMode [CPSS_QOS_PORT_NO_TRUST_E] */
            mode = CPSS_QOS_PORT_NO_TRUST_E;

            st = cpssDxChCosPortQosTrustModeSet(dev, port, mode);
            UTF_VERIFY_EQUAL3_PARAM_MAC(GT_OK, st, dev, port, mode);

            /*
                1.1.2. Call cpssDxChCosPortQosTrustModeGet
                Expected: GT_OK and the same portQosTrustModePtr.
            */
            st = cpssDxChCosPortQosTrustModeGet(dev, port, &modeGet);
            UTF_VERIFY_EQUAL2_STRING_MAC(GT_OK, st,
                       "cpssDxChCosPortQosTrustModeGet: %d, %d", dev, port);
            UTF_VERIFY_EQUAL2_STRING_MAC(mode, modeGet,
                       "get another portQosTrustMode than was set: %d, %d", dev, port);

            /* Call with portQosTrustMode [CPSS_QOS_PORT_TRUST_L2_E] */
            mode = CPSS_QOS_PORT_TRUST_L2_E;

            st = cpssDxChCosPortQosTrustModeSet(dev, port, mode);
            UTF_VERIFY_EQUAL3_PARAM_MAC(GT_OK, st, dev, port, mode);

            /*
                1.1.2. Call cpssDxChCosPortQosTrustModeGet
                Expected: GT_OK and the same portQosTrustModePtr.
            */
            st = cpssDxChCosPortQosTrustModeGet(dev, port, &modeGet);
            UTF_VERIFY_EQUAL2_STRING_MAC(GT_OK, st,
                       "cpssDxChCosPortQosTrustModeGet: %d, %d", dev, port);
            UTF_VERIFY_EQUAL2_STRING_MAC(mode, modeGet,
                       "get another portQosTrustMode than was set: %d, %d", dev, port);

            /* Call with portQosTrustMode [CPSS_QOS_PORT_TRUST_L3_E] */
            mode = CPSS_QOS_PORT_TRUST_L3_E;

            st = cpssDxChCosPortQosTrustModeSet(dev, port, mode);
            UTF_VERIFY_EQUAL3_PARAM_MAC(GT_OK, st, dev, port, mode);

            /*
                1.1.2. Call cpssDxChCosPortQosTrustModeGet
                Expected: GT_OK and the same portQosTrustModePtr.
            */
            st = cpssDxChCosPortQosTrustModeGet(dev, port, &modeGet);
            UTF_VERIFY_EQUAL2_STRING_MAC(GT_OK, st,
                       "cpssDxChCosPortQosTrustModeGet: %d, %d", dev, port);
            UTF_VERIFY_EQUAL2_STRING_MAC(mode, modeGet,
                       "get another portQosTrustMode than was set: %d, %d", dev, port);

            /* Call with portQosTrustMode [CPSS_QOS_PORT_TRUST_L2_L3_E] */
            mode = CPSS_QOS_PORT_TRUST_L2_L3_E;

            st = cpssDxChCosPortQosTrustModeSet(dev, port, mode);
            UTF_VERIFY_EQUAL3_PARAM_MAC(GT_OK, st, dev, port, mode);

            /*
                1.1.2. Call cpssDxChCosPortQosTrustModeGet
                Expected: GT_OK and the same portQosTrustModePtr.
            */
            st = cpssDxChCosPortQosTrustModeGet(dev, port, &modeGet);
            UTF_VERIFY_EQUAL2_STRING_MAC(GT_OK, st,
                       "cpssDxChCosPortQosTrustModeGet: %d, %d", dev, port);
            UTF_VERIFY_EQUAL2_STRING_MAC(mode, modeGet,
                       "get another portQosTrustMode than was set: %d, %d", dev, port);

            /*
                1.1.3. Call with wrong enum values portQosTrustMode
                Expected: GT_BAD_PARAM.
            */
            UTF_ENUMS_CHECK_MAC(cpssDxChCosPortQosTrustModeSet
                                (dev, port, mode),
                                mode);
        }

        mode = CPSS_QOS_PORT_NO_TRUST_E;

        st = prvUtfNextVirtPortReset(&port, dev);
        UTF_VERIFY_EQUAL0_PARAM_MAC(GT_OK, st);
        /* 1.2. For all active devices go over all non available virtual ports. */
        while (GT_OK == prvUtfNextVirtPortGet(&port, GT_FALSE))
        {
            /* 1.2.1. Call function for each non-active port */
            st = cpssDxChCosPortQosTrustModeSet(dev, port, mode);
            UTF_VERIFY_EQUAL2_PARAM_MAC(GT_BAD_PARAM, st, dev, port);
        }

        /* 1.3. For active device check that function returns GT_BAD_PARAM */
        /* for out of bound value for port number.                         */
        port = PRV_CPSS_MAX_PP_PORTS_NUM_CNS;

        st = cpssDxChCosPortQosTrustModeSet(dev, port, mode);
        UTF_VERIFY_EQUAL2_PARAM_MAC(GT_BAD_PARAM, st, dev, port);

        /* 1.4. For active device check that function returns GT_OK        */
        /* for CPU port number.                                            */
        port = CPSS_CPU_PORT_NUM_CNS;

        st = cpssDxChCosPortQosTrustModeSet(dev, port, mode);
        UTF_VERIFY_EQUAL2_PARAM_MAC(GT_OK, st, dev, port);
    }

    mode = CPSS_QOS_PORT_NO_TRUST_E;

    /* 2. For not active devices check that function returns non GT_OK.*/
    port = COS_VALID_VIRT_PORT_CNS;

    /* prepare device iterator */
    PRV_TGF_NOT_APPLIC_DEV_RESET_MAC(&dev, UTF_NONE_FAMILY_E);

    /* Go over all non active devices. */
    while(GT_OK == prvUtfNextDeviceGet(&dev, GT_FALSE))
    {
        st = cpssDxChCosPortQosTrustModeSet(dev, port, mode);
        UTF_VERIFY_EQUAL1_PARAM_MAC(GT_NOT_APPLICABLE_DEVICE, st, dev);
    }

    /* 3.Call function with out of bound value for device id */
    dev = PRV_CPSS_MAX_PP_DEVICES_CNS;

    st = cpssDxChCosPortQosTrustModeSet(dev, port, mode);
    UTF_VERIFY_EQUAL1_PARAM_MAC(GT_BAD_PARAM, st, dev);
}

/*----------------------------------------------------------------------------*/
/*
GT_STATUS cpssDxChCosPortQosTrustModeGet
(
    IN GT_U8                         dev,
    IN GT_U8                         port,
    IN CPSS_QOS_PORT_TRUST_MODE_ENT  *portQosTrustModePtr
)
*/
UTF_TEST_CASE_MAC(cpssDxChCosPortQosTrustModeGet)
{
/*
    ITERATE_DEVICES_VIRT_PORTS (DxChx)
    1.1.1. Call with non-null portQosTrustModePtr.
    Expected: GT_OK.
    1.1.2. Call with portQosTrustModePtr [NULL].
    Expected: GT_BAD_PTR.
    1.2. Call with port [CPSS_CPU_PORT_NUM_CNS = 63] and valid other parameters.
    Expected: GT_OK.
*/
    GT_STATUS                     st      = GT_OK;

    GT_U8                         dev;
    GT_U8                         port    = COS_VALID_VIRT_PORT_CNS;
    CPSS_QOS_PORT_TRUST_MODE_ENT  mode    = CPSS_QOS_PORT_NO_TRUST_E;

    /* prepare iterator for go over all active devices */
    PRV_TGF_NOT_APPLIC_DEV_RESET_MAC(&dev, UTF_NONE_FAMILY_E);

    /* 1. Go over all active devices. */
    while (GT_OK == prvUtfNextDeviceGet(&dev, GT_TRUE))
    {
        st = prvUtfNextVirtPortReset(&port, dev);
        UTF_VERIFY_EQUAL0_PARAM_MAC(GT_OK, st);

        /* 1.1. For all active devices go over all available virtual ports. */
        while (GT_OK == prvUtfNextVirtPortGet(&port, GT_TRUE))
        {
            /*
                1.1.1. Call with non-null portQosTrustModePtr.
                Expected: GT_OK.
            */
            st = cpssDxChCosPortQosTrustModeGet(dev, port, &mode);
            UTF_VERIFY_EQUAL2_PARAM_MAC(GT_OK, st, dev, port);

            /*
                1.1.2. Call with portQosTrustModePtr [NULL].
                Expected: GT_BAD_PTR.
            */
            st = cpssDxChCosPortQosTrustModeGet(dev, port, NULL);
            UTF_VERIFY_EQUAL2_STRING_MAC(GT_BAD_PTR, st, "%d, %d, NULL", dev, port);
        }

        st = prvUtfNextVirtPortReset(&port, dev);
        UTF_VERIFY_EQUAL0_PARAM_MAC(GT_OK, st);

        /* 1.2. For all active devices go over all non available
           virtual ports.
        */
        while (GT_OK == prvUtfNextVirtPortGet(&port, GT_FALSE))
        {
            st = cpssDxChCosPortQosTrustModeGet(dev, port, &mode);
            UTF_VERIFY_EQUAL2_PARAM_MAC(GT_BAD_PARAM, st, dev, port);
        }

        /* 1.3. For active device check that function returns GT_BAD_PARAM */
        /* for out of bound value for port number.                         */
        port = PRV_CPSS_MAX_PP_PORTS_NUM_CNS;

        st = cpssDxChCosPortQosTrustModeGet(dev, port, &mode);
        UTF_VERIFY_EQUAL2_PARAM_MAC(GT_BAD_PARAM, st, dev, port);

        /* 1.4. For active device check that function returns GT_OK        */
        /* for CPU port number.                                            */
        port = CPSS_CPU_PORT_NUM_CNS;

        st = cpssDxChCosPortQosTrustModeGet(dev, port, &mode);
        UTF_VERIFY_EQUAL2_PARAM_MAC(GT_OK, st, dev, port);
    }

    /* 2. For not active devices check that function returns non GT_OK.*/

    port = COS_VALID_VIRT_PORT_CNS;

    /* prepare device iterator */
    PRV_TGF_NOT_APPLIC_DEV_RESET_MAC(&dev, UTF_NONE_FAMILY_E);

    /* Go over all non active devices. */
    while(GT_OK == prvUtfNextDeviceGet(&dev, GT_FALSE))
    {
        st = cpssDxChCosPortQosTrustModeGet(dev, port, &mode);
        UTF_VERIFY_EQUAL1_PARAM_MAC(GT_NOT_APPLICABLE_DEVICE, st, dev);
    }

    /* 3.Call function with out of bound value for device id */
    dev = PRV_CPSS_MAX_PP_DEVICES_CNS;

    st = cpssDxChCosPortQosTrustModeGet(dev, port, &mode);
    UTF_VERIFY_EQUAL1_PARAM_MAC(GT_BAD_PARAM, st, dev);
}

/*----------------------------------------------------------------------------*/
/*
GT_STATUS cpssDxChCosPortReMapDSCPSet
(
    IN GT_U8                        dev,
    IN GT_U8                        port,
    IN GT_BOOL                      enableDscpMutation
)
*/
UTF_TEST_CASE_MAC(cpssDxChCosPortReMapDSCPSet)
{
/*
    ITERATE_DEVICES_VIRT_PORTS (DxChx)
    1.1.1. Call with enableDscpMutation [GT_FALSE, GT_TRUE].
    Expected: GT_OK.
    1.1.2. Call cpssDxChCosPortReMapDSCPGet
    Expected: GT_OK and the same enableDscpMutation.
    1.2. Call with port [CPSS_CPU_PORT_NUM_CNS = 63] and valid other parameters.
    Expected: GT_OK.
*/
    GT_STATUS st       = GT_OK;

    GT_U8     dev;
    GT_U8     port     = COS_VALID_VIRT_PORT_CNS;
    GT_BOOL   state    = GT_FALSE;
    GT_BOOL   stateGet = GT_FALSE;

    /* prepare iterator for go over all active devices */
    PRV_TGF_NOT_APPLIC_DEV_RESET_MAC(&dev, UTF_NONE_FAMILY_E);

    /* 1. Go over all active devices. */
    while (GT_OK == prvUtfNextDeviceGet(&dev, GT_TRUE))
    {
        st = prvUtfNextVirtPortReset(&port, dev);
        UTF_VERIFY_EQUAL0_PARAM_MAC(GT_OK, st);

        /* 1.1. For all active devices go over all available virtual ports. */
        while (GT_OK == prvUtfNextVirtPortGet(&port, GT_TRUE))
        {
            /*
                1.1.1. Call with enableDscpMutation [GT_FALSE, GT_TRUE].
                Expected: GT_OK.
            */

            /* upOverrideEnable = GT_FALSE*/
            state = GT_FALSE;

            st = cpssDxChCosPortReMapDSCPSet(dev, port, state);
            UTF_VERIFY_EQUAL3_PARAM_MAC(GT_OK, st, dev, port, state);

            /*
                1.1.2. Call cpssDxChCosPortReMapDSCPGet
                Expected: GT_OK and the same enableDscpMutation.
            */
            st = cpssDxChCosPortReMapDSCPGet(dev, port, &stateGet);
            UTF_VERIFY_EQUAL2_STRING_MAC(GT_OK, st,
                       "cpssDxChCosPortReMapDSCPGet: %d, %d", dev, port);
            UTF_VERIFY_EQUAL2_STRING_MAC(state, stateGet,
                       "get another enableDscpMutation than was set: %d, %d", dev, port);

            /* upOverrideEnable = GT_TRUE*/
            state = GT_TRUE;

            st = cpssDxChCosPortReMapDSCPSet(dev, port, state);
            UTF_VERIFY_EQUAL3_PARAM_MAC(GT_OK, st, dev, port, state);

            /*
                1.1.2. Call cpssDxChCosPortReMapDSCPGet
                Expected: GT_OK and the same enableDscpMutation.
            */
            st = cpssDxChCosPortReMapDSCPGet(dev, port, &stateGet);
            UTF_VERIFY_EQUAL2_STRING_MAC(GT_OK, st,
                       "cpssDxChCosPortReMapDSCPGet: %d, %d", dev, port);
            UTF_VERIFY_EQUAL2_STRING_MAC(state, stateGet,
                       "get another enableDscpMutation than was set: %d, %d", dev, port);
        }

        state = GT_TRUE;

        st = prvUtfNextVirtPortReset(&port, dev);
        UTF_VERIFY_EQUAL0_PARAM_MAC(GT_OK, st);
        /* 1.2. For all active devices go over all non available virtual ports. */
        while (GT_OK == prvUtfNextVirtPortGet(&port, GT_FALSE))
        {
            /* 1.2.1. Call function for each non-active port */
            st = cpssDxChCosPortReMapDSCPSet(dev, port, state);
            UTF_VERIFY_EQUAL2_PARAM_MAC(GT_BAD_PARAM, st, dev, port);
        }

        /* 1.3. For active device check that function returns GT_BAD_PARAM */
        /* for out of bound value for port number.                         */
        port = PRV_CPSS_MAX_PP_PORTS_NUM_CNS;

        st = cpssDxChCosPortReMapDSCPSet(dev, port, state);
        UTF_VERIFY_EQUAL2_PARAM_MAC(GT_BAD_PARAM, st, dev, port);

        /* 1.4. For active device check that function returns GT_OK        */
        /* for CPU port number.                                            */
        port = CPSS_CPU_PORT_NUM_CNS;

        st = cpssDxChCosPortReMapDSCPSet(dev, port, state);
        UTF_VERIFY_EQUAL2_PARAM_MAC(GT_OK, st, dev, port);
    }

    state = GT_TRUE;

    /* 2. For not active devices check that function returns non GT_OK.*/
    port = COS_VALID_VIRT_PORT_CNS;

    /* prepare device iterator */
    PRV_TGF_NOT_APPLIC_DEV_RESET_MAC(&dev, UTF_NONE_FAMILY_E);

    /* Go over all non active devices. */
    while(GT_OK == prvUtfNextDeviceGet(&dev, GT_FALSE))
    {
        st = cpssDxChCosPortReMapDSCPSet(dev, port, state);
        UTF_VERIFY_EQUAL1_PARAM_MAC(GT_NOT_APPLICABLE_DEVICE, st, dev);
    }

    /* 3.Call function with out of bound value for device id */
    dev = PRV_CPSS_MAX_PP_DEVICES_CNS;

    st = cpssDxChCosPortReMapDSCPSet(dev, port, state);
    UTF_VERIFY_EQUAL1_PARAM_MAC(GT_BAD_PARAM, st, dev);
}

/*----------------------------------------------------------------------------*/
/*
GT_STATUS cpssDxChCosPortReMapDSCPGet
(
    IN GT_U8      dev,
    IN GT_U8      port,
    IN GT_BOOL   *enableDscpMutationPtr
)
*/
UTF_TEST_CASE_MAC(cpssDxChCosPortReMapDSCPGet)
{
/*
    ITERATE_DEVICES_VIRT_PORTS (DxChx)
    1.1.1. Call with non-null enableDscpMutationPtr.
    Expected: GT_OK.
    1.1.2. Call with enableDscpMutationPtr [NULL].
    Expected: GT_BAD_PTR.
    1.2. Call with port [CPSS_CPU_PORT_NUM_CNS = 63] and valid other parameters.
    Expected: GT_OK.
*/
    GT_STATUS   st     = GT_OK;

    GT_U8       dev;
    GT_U8       port   = COS_VALID_VIRT_PORT_CNS;
    GT_BOOL     enable = GT_FALSE;

    /* prepare iterator for go over all active devices */
    PRV_TGF_NOT_APPLIC_DEV_RESET_MAC(&dev, UTF_NONE_FAMILY_E);

    /* 1. Go over all active devices. */
    while (GT_OK == prvUtfNextDeviceGet(&dev, GT_TRUE))
    {
        st = prvUtfNextVirtPortReset(&port, dev);
        UTF_VERIFY_EQUAL0_PARAM_MAC(GT_OK, st);

        /* 1.1. For all active devices go over all available virtual ports. */
        while (GT_OK == prvUtfNextVirtPortGet(&port, GT_TRUE))
        {
            /*
                1.1.1. Call with non-null enableDscpMutationPtr.
                Expected: GT_OK.
            */
            st = cpssDxChCosPortReMapDSCPGet(dev, port, &enable);
            UTF_VERIFY_EQUAL2_PARAM_MAC(GT_OK, st, dev, port);

            /*
                1.1.2. Call with enableDscpMutationPtr [NULL].
                Expected: GT_BAD_PTR.
            */
            st = cpssDxChCosPortReMapDSCPGet(dev, port, NULL);
            UTF_VERIFY_EQUAL2_STRING_MAC(GT_BAD_PTR, st, "%d, %d, NULL", dev, port);
        }

        st = prvUtfNextVirtPortReset(&port, dev);
        UTF_VERIFY_EQUAL0_PARAM_MAC(GT_OK, st);

        /* 1.2. For all active devices go over all non available
           virtual ports.
        */
        while (GT_OK == prvUtfNextVirtPortGet(&port, GT_FALSE))
        {
            st = cpssDxChCosPortReMapDSCPGet(dev, port, &enable);
            UTF_VERIFY_EQUAL2_PARAM_MAC(GT_BAD_PARAM, st, dev, port);
        }

        /* 1.3. For active device check that function returns GT_BAD_PARAM */
        /* for out of bound value for port number.                         */
        port = PRV_CPSS_MAX_PP_PORTS_NUM_CNS;

        st = cpssDxChCosPortReMapDSCPGet(dev, port, &enable);
        UTF_VERIFY_EQUAL2_PARAM_MAC(GT_BAD_PARAM, st, dev, port);

        /* 1.4. For active device check that function returns  GT_OK       */
        /* for CPU port number.                                            */
        port = CPSS_CPU_PORT_NUM_CNS;

        st = cpssDxChCosPortReMapDSCPGet(dev, port, &enable);
        UTF_VERIFY_EQUAL2_PARAM_MAC(GT_OK, st, dev, port);
    }

    /* 2. For not active devices check that function returns non GT_OK.*/

    port = COS_VALID_VIRT_PORT_CNS;

    /* prepare device iterator */
    PRV_TGF_NOT_APPLIC_DEV_RESET_MAC(&dev, UTF_NONE_FAMILY_E);

    /* Go over all non active devices. */
    while(GT_OK == prvUtfNextDeviceGet(&dev, GT_FALSE))
    {
        st = cpssDxChCosPortReMapDSCPGet(dev, port, &enable);
        UTF_VERIFY_EQUAL1_PARAM_MAC(GT_NOT_APPLICABLE_DEVICE, st, dev);
    }

    /* 3.Call function with out of bound value for device id */
    dev = PRV_CPSS_MAX_PP_DEVICES_CNS;

    st = cpssDxChCosPortReMapDSCPGet(dev, port, &enable);
    UTF_VERIFY_EQUAL1_PARAM_MAC(GT_BAD_PARAM, st, dev);
}

/*----------------------------------------------------------------------------*/
/*
GT_STATUS cpssDxChCosPortVlanQoSCfgEntryGet
(
    IN    GT_U8     dev,
    INOUT GT_U32    port,
    OUT   GT_U32   *entryPtr
)
*/
UTF_TEST_CASE_MAC(cpssDxChCosPortVlanQoSCfgEntryGet)
{
/*
1.1. Call with port [0/ 10] and non-null entryPtr.
Expected: GT_OK.
1.2. Call with entryPtr [NULL].
Expected: GT_BAD_PTR.
1.3. Call with port [CPSS_CPU_PORT_NUM_CNS = 63] and valid other parameters.
Expected:  GT_OK.
1.4. Call with port [PRV_CPSS_MAX_PP_PORTS_NUM_CNS].
Expected: GT_OK.
*/
    GT_STATUS   st    = GT_OK;

    GT_U8       dev;
    GT_U8       port  = COS_VALID_VIRT_PORT_CNS;
    GT_U32      entry[4] = {0,0,0,0};

    /* prepare iterator for go over all active devices */
    PRV_TGF_NOT_APPLIC_DEV_RESET_MAC(&dev, UTF_NONE_FAMILY_E);

    /* 1. Go over all active devices. */
    while (GT_OK == prvUtfNextDeviceGet(&dev, GT_TRUE))
    {
        /*
            1.1. Call with port [0/ 10] and non-null entryPtr.
            Expected: GT_OK.
        */
        port = 0;
        st = cpssDxChCosPortVlanQoSCfgEntryGet(dev, port, entry);
        UTF_VERIFY_EQUAL2_PARAM_MAC(GT_OK, st, dev, port);

        port = 9;/* fixed from 10 to 9 to support diamond-cut with ports 0..9 */
        st = cpssDxChCosPortVlanQoSCfgEntryGet(dev, port, entry);
        UTF_VERIFY_EQUAL2_PARAM_MAC(GT_OK, st, dev, port);
        /*
            1.2. Call with entryPtr [NULL].
            Expected: GT_BAD_PTR.
        */
        port = 0;
        st = cpssDxChCosPortVlanQoSCfgEntryGet(dev, port, NULL);
        UTF_VERIFY_EQUAL2_STRING_MAC(GT_BAD_PTR, st, "%d, %d, NULL", dev, port);

        /*
            1.4. Call with port [PRV_CPSS_MAX_PP_PORTS_NUM_CNS].
            Expected: NOT GT_OK.
        */
        port = PRV_CPSS_MAX_PP_PORTS_NUM_CNS;

        st = cpssDxChCosPortVlanQoSCfgEntryGet(dev, port, entry);
        UTF_VERIFY_NOT_EQUAL2_PARAM_MAC(GT_OK, st, dev, port);

        /* 1.4. For active device check that function returns  GT_OK        */
        /* for CPU port number.                                             */
        port = CPSS_CPU_PORT_NUM_CNS;

        st = cpssDxChCosPortVlanQoSCfgEntryGet(dev, port, entry);
        UTF_VERIFY_EQUAL2_PARAM_MAC(GT_OK, st, dev, port);
    }

    /* 2. For not active devices check that function returns non GT_OK.*/

    port = COS_VALID_VIRT_PORT_CNS;

    /* prepare device iterator */
    PRV_TGF_NOT_APPLIC_DEV_RESET_MAC(&dev, UTF_NONE_FAMILY_E);

    /* Go over all non active devices. */
    while(GT_OK == prvUtfNextDeviceGet(&dev, GT_FALSE))
    {
        st = cpssDxChCosPortVlanQoSCfgEntryGet(dev, port, entry);
        UTF_VERIFY_EQUAL1_PARAM_MAC(GT_NOT_APPLICABLE_DEVICE, st, dev);
    }

    /* 3.Call function with out of bound value for device id */
    dev = PRV_CPSS_MAX_PP_DEVICES_CNS;

    st = cpssDxChCosPortVlanQoSCfgEntryGet(dev, port, entry);
    UTF_VERIFY_EQUAL1_PARAM_MAC(GT_BAD_PARAM, st, dev);
}

/*----------------------------------------------------------------------------*/
/*
GT_STATUS cpssDxChCosRemarkEntryGet
(
    IN    GT_U8     dev,
    INOUT GT_U32    index,
    OUT   GT_U32   *entryPtr
)
*/
UTF_TEST_CASE_MAC(cpssDxChCosRemarkEntryGet)
{
/*
    ITERATE_DEVICES (DxCh1, DxCh2)
    1.1. Call function with index [4],
                            non-null entryPtr.
    Expected: GT_OK.
    1.2. Call function with index [72]
                            and non-null entryPtr.
    Expected: GT_OK for Cheetah2 and NON GT_OK for Cheetah.
    1.3. Call function with index [128]
                            and non-null entryPtr.
    Expected: NON GT_OK.
    1.4. Call function with index [2],
                            null entryPtr [NULL].
    Expected: GT_BAD_PTR.
*/
    GT_STATUS    st    = GT_OK;

    GT_U8        dev;
    GT_U8        index = 0;
    GT_U32       entry[1] = { 0 };

    CPSS_PP_FAMILY_TYPE_ENT   devFamily;

    /* prepare device iterator */
    PRV_TGF_NOT_APPLIC_DEV_RESET_MAC(&dev, UTF_CH3_E | UTF_XCAT_E | UTF_LION_E | UTF_XCAT2_E);

    /* 1. Go over all active devices. */
    while (GT_OK == prvUtfNextDeviceGet(&dev, GT_TRUE))
    {
        /* Getting device family */
        st = prvUtfDeviceFamilyGet(dev, &devFamily);
        UTF_VERIFY_EQUAL1_STRING_MAC(GT_OK, st, "prvUtfDeviceFamilyGet: %d", dev);

        /*
            1.1. Call function with index [4],
                                    non-null entryPtr.
            Expected: GT_OK.
        */
        index = 4;

        st = cpssDxChCosRemarkEntryGet(dev, index, entry);
        UTF_VERIFY_EQUAL2_PARAM_MAC(GT_OK, st, dev, index);

        /*
            1.2. Call function with index [72]
                                    and non-null entryPtr.
            Expected: GT_OK for DxCh2,3 and NON GT_OK for Ch.
        */
        index = PRV_CPSS_DXCH_QOS_PROFILE_NUM_MAX_CNS;

        st = cpssDxChCosRemarkEntryGet(dev, index, entry);
        if(devFamily != CPSS_PP_FAMILY_CHEETAH_E)
        {
            UTF_VERIFY_EQUAL2_PARAM_MAC(GT_OK, st, dev, index);
        }
        else
        {
            UTF_VERIFY_NOT_EQUAL2_PARAM_MAC(GT_OK, st, dev, index);
        }

        /*
            1.3. Call function with index [128]
                                    and non-null entryPtr.
            Expected: NON GT_OK.
        */
        index = PRV_CPSS_DXCH2_QOS_PROFILE_NUM_MAX_CNS;

        st = cpssDxChCosRemarkEntryGet(dev, index, entry);
        UTF_VERIFY_NOT_EQUAL2_PARAM_MAC(GT_OK, st, dev, index);

        /*
            1.4. Call function with index [2],
                                    null entryPtr [NULL].
            Expected: GT_BAD_PTR.
        */
        index = 2;

        st = cpssDxChCosRemarkEntryGet(dev, index, NULL);
        UTF_VERIFY_EQUAL1_STRING_MAC(GT_BAD_PTR, st, "%d, entryPtr = NULL",
                                     dev);
    }

    index = 4;

    /* 2. For not-active devices and devices from non-applicable family */
    /* check that function returns GT_BAD_PARAM.                        */

    /* prepare device iterator */
    PRV_TGF_NOT_APPLIC_DEV_RESET_MAC(&dev, UTF_CH3_E | UTF_XCAT_E | UTF_LION_E | UTF_XCAT2_E);

    /* go over all non active devices */
    while(GT_OK == prvUtfNextDeviceGet(&dev, GT_FALSE))
    {
        st = cpssDxChCosRemarkEntryGet(dev, index, entry);
        UTF_VERIFY_EQUAL1_PARAM_MAC(GT_NOT_APPLICABLE_DEVICE, st, dev);
    }

    /* 3. Call function with out of bound value for device id.*/

    dev = PRV_CPSS_MAX_PP_DEVICES_CNS;

    st = cpssDxChCosRemarkEntryGet(dev, index, entry);
    UTF_VERIFY_EQUAL1_PARAM_MAC(GT_BAD_PARAM, st, dev);
}

/*----------------------------------------------------------------------------*/
/*
GT_STATUS cpssDxChCosQoSProfileEntryGet
(
    IN    GT_U8     dev,
    INOUT GT_U32    index,
    OUT   GT_U32   *entryPtr
)
*/
UTF_TEST_CASE_MAC(cpssDxChCosQoSProfileEntryGet)
{
/*
    ITERATE_DEVICES (DxChx)
    1.1. Call function with index [4],
                            non-null entryPtr.
    Expected: GT_OK.
    1.2. Call function with index [72]
                            and non-null entryPtr.
    Expected: GT_OK for DxCh2,3 and NON GT_OK for Ch.
    1.2. Call function with index [128]
                            and non-null entryPtr.
    Expected: NON GT_OK.
    1.3. Call function with index [2], null entryPtr [NULL].
    Expected: GT_BAD_PTR.
*/
    GT_STATUS    st    = GT_OK;

    GT_U8        dev;
    GT_U8        index = 0;
    GT_U32       entry[1] = { 0 };

    CPSS_PP_FAMILY_TYPE_ENT   devFamily;

    /* prepare device iterator */
    PRV_TGF_NOT_APPLIC_DEV_RESET_MAC(&dev, UTF_NONE_FAMILY_E);

    /* 1. Go over all active devices. */
    while (GT_OK == prvUtfNextDeviceGet(&dev, GT_TRUE))
    {
        /* Getting device family */
        st = prvUtfDeviceFamilyGet(dev, &devFamily);
        UTF_VERIFY_EQUAL1_STRING_MAC(GT_OK, st, "prvUtfDeviceFamilyGet: %d", dev);

        /*
            1.1. Call function with index [4],
                                    non-null entryPtr.
            Expected: GT_OK.
        */
        index = 4;

        st = cpssDxChCosQoSProfileEntryGet(dev, index, entry);
        UTF_VERIFY_EQUAL2_PARAM_MAC(GT_OK, st, dev, index);

        /*
            1.2. Call function with index [72]
                                    and non-null entryPtr.
            Expected: GT_OK for Ch2 and NON GT_OK for Ch.
        */
        index = PRV_CPSS_DXCH_QOS_PROFILE_NUM_MAX_CNS;

        st = cpssDxChCosQoSProfileEntryGet(dev, index, entry);
        if (devFamily != CPSS_PP_FAMILY_CHEETAH_E)
        {
            UTF_VERIFY_EQUAL2_PARAM_MAC(GT_OK, st, dev, index);
        }
        else
        {
            UTF_VERIFY_NOT_EQUAL2_PARAM_MAC(GT_OK, st, dev, index);
        }

        /*
            1.3. Call function with index [128]
                                    and non-null entryPtr.
            Expected: NON GT_OK.
        */
        index = PRV_CPSS_DXCH2_QOS_PROFILE_NUM_MAX_CNS;

        st = cpssDxChCosQoSProfileEntryGet(dev, index, entry);
        UTF_VERIFY_NOT_EQUAL2_PARAM_MAC(GT_OK, st, dev, index);

        /*
            1.3. Call function with index [2],
                                    null entryPtr [NULL].
            Expected: GT_BAD_PTR.
        */
        index = 2;

        st = cpssDxChCosQoSProfileEntryGet(dev, index, NULL);
        UTF_VERIFY_EQUAL1_STRING_MAC(GT_BAD_PTR, st, "%d, entryPtr = NULL",
                                     dev);
    }

    index = 4;

    /* 2. For not-active devices and devices from non-applicable family */
    /* check that function returns GT_BAD_PARAM.                        */

    /* prepare device iterator */
    PRV_TGF_NOT_APPLIC_DEV_RESET_MAC(&dev, UTF_NONE_FAMILY_E);

    /* go over all non active devices */
    while(GT_OK == prvUtfNextDeviceGet(&dev, GT_FALSE))
    {
        st = cpssDxChCosQoSProfileEntryGet(dev, index, entry);
        UTF_VERIFY_EQUAL1_PARAM_MAC(GT_NOT_APPLICABLE_DEVICE, st, dev);
    }

    /* 3. Call function with out of bound value for device id.*/

    dev = PRV_CPSS_MAX_PP_DEVICES_CNS;

    st = cpssDxChCosQoSProfileEntryGet(dev, index, entry);
    UTF_VERIFY_EQUAL1_PARAM_MAC(GT_BAD_PARAM, st, dev);
}

/*----------------------------------------------------------------------------*/
/*
GT_STATUS cpssDxChCosExpToProfileMapSet
(
    IN  GT_U8   dev,
    IN  GT_U32  exp,
    IN  GT_U32  profileIndex
)
*/
UTF_TEST_CASE_MAC(cpssDxChCosExpToProfileMapSet)
{
/*
    ITERATE_DEVICES (DxCh2 and above)
    1.1. Call function with profileIndex [4], exp [1].
    Expected: GT_OK.
    1.2. Call cpssDxChCosExpToProfileMapGet with all valid parameters
                                            and the same exp.
    Expected: GT_OK and the same profileIndex.
    1.3. Call function with out of range
         profileIndex [PRV_CPSS_DXCH2_QOS_PROFILE_NUM_MAX_CNS = 128] and exp [1].
    Expected: NOT GT_OK.
    1.4. Call function with profileIndex [2]
                       and out of range exp [CPSS_EXP_RANGE_CNS = 8].
    Expected: NON GT_OK.
*/
    GT_STATUS  st      = GT_OK;

    GT_U8      dev;
    GT_U32     exp     = 0;
    GT_U32     profile = 0;
    GT_U32     pfGet   = 0;

    /* prepare device iterator */
    PRV_TGF_NOT_APPLIC_DEV_RESET_MAC(&dev, UTF_CH1_E | UTF_CH1_DIAMOND_E);

    /* 1. Go over all active devices. */
    while (GT_OK == prvUtfNextDeviceGet(&dev, GT_TRUE))
    {
        /*
            1.1. Call function with profileIndex [4], exp [1].
            Expected: GT_OK.
        */
        exp     = 1;
        profile = 4;

        st = cpssDxChCosExpToProfileMapSet(dev, exp, profile);
        UTF_VERIFY_EQUAL3_PARAM_MAC(GT_OK, st, dev, exp, profile);

        /*
            1.2. Call cpssDxChCosExpToProfileMapGet with all valid parameters
                                            and the same exp.
            Expected: GT_OK and the same profileIndex.
        */
        st = cpssDxChCosExpToProfileMapGet(dev, exp, &pfGet);
        UTF_VERIFY_EQUAL2_STRING_MAC(GT_OK, st,
                       "cpssDxChCosExpToProfileMapGet: %d, %d", dev, exp);
        UTF_VERIFY_EQUAL1_STRING_MAC(profile, pfGet,
                       "get another profileIndex than was set: %d", dev);

        /*
            1.3. Call function with out of range
                 profileIndex [PRV_CPSS_DXCH2_QOS_PROFILE_NUM_MAX_CNS = 128] and exp [1].
            Expected: NOT GT_OK.
        */
        profile = PRV_CPSS_DXCH2_QOS_PROFILE_NUM_MAX_CNS;

        st = cpssDxChCosExpToProfileMapSet(dev, exp, profile);
        UTF_VERIFY_NOT_EQUAL2_STRING_MAC(GT_OK, st, "%d, profileIndex = %d",
                                         dev, profile);

        /*
            1.4. Call function with profileIndex [2]
                      and out of range exp [CPSS_EXP_RANGE_CNS = 8].
            Expected: NON GT_OK.
        */
        exp = CPSS_EXP_RANGE_CNS;
        profile = 2;

        st = cpssDxChCosExpToProfileMapSet(dev, exp, profile);
        UTF_VERIFY_NOT_EQUAL2_PARAM_MAC(GT_OK, st, dev, exp);
    }

    exp     = 1;
    profile = 4;

    /* 2. For not-active devices and devices from non-applicable family */
    /* check that function returns GT_BAD_PARAM.                        */

    /* prepare device iterator */
    PRV_TGF_NOT_APPLIC_DEV_RESET_MAC(&dev, UTF_CH1_E | UTF_CH1_DIAMOND_E);

    /* go over all non active devices */
    while(GT_OK == prvUtfNextDeviceGet(&dev, GT_FALSE))
    {
        st = cpssDxChCosExpToProfileMapSet(dev, exp, profile);
        UTF_VERIFY_EQUAL1_PARAM_MAC(GT_NOT_APPLICABLE_DEVICE, st, dev);
    }

    /* 3. Call function with out of bound value for device id.*/

    dev = PRV_CPSS_MAX_PP_DEVICES_CNS;

    st = cpssDxChCosExpToProfileMapSet(dev, exp, profile);
    UTF_VERIFY_EQUAL1_PARAM_MAC(GT_BAD_PARAM, st, dev);
}

/*----------------------------------------------------------------------------*/
/*
GT_STATUS cpssDxChCosExpToProfileMapGet
(
    IN  GT_U8   dev,
    IN  GT_U32  exp,
    OUT GT_U32  *profileIndexPtr
)
*/
UTF_TEST_CASE_MAC(cpssDxChCosExpToProfileMapGet)
{
/*
    ITERATE_DEVICES (DxCh2 and above)
    1.1. Call function with non-NULL profileIndexPtr, exp [1].
    Expected: GT_OK.
    1.2. Call function with non-NULL profileIndexPtr and
         out of range exp [CPSS_EXP_RANGE_CNS = 8].
    Expected: NON GT_OK.
    1.3. Call function profileIndexPtr [NULL] and exp [1].
    Expected: GT_BAD_PTR.
*/
    GT_STATUS  st      = GT_OK;

    GT_U8      dev;
    GT_U32     exp     = 0;
    GT_U32     profile = 0;

    /* prepare device iterator */
    PRV_TGF_NOT_APPLIC_DEV_RESET_MAC(&dev, UTF_CH1_E | UTF_CH1_DIAMOND_E);

    /* 1. Go over all active devices. */
    while (GT_OK == prvUtfNextDeviceGet(&dev, GT_TRUE))
    {
        /*
            1.1. Call function with non-NULL profileIndexPtr, exp [1].
            Expected: GT_OK.
        */
        exp = 1;

        st = cpssDxChCosExpToProfileMapGet(dev, exp, &profile);
        UTF_VERIFY_EQUAL2_PARAM_MAC(GT_OK, st, dev, exp);

        /*
            1.2. Call function with non-NULL profileIndexPtr
                      and out of range exp [CPSS_EXP_RANGE_CNS = 8].
            Expected: NON GT_OK.
        */
        exp = CPSS_EXP_RANGE_CNS;

        st = cpssDxChCosExpToProfileMapGet(dev, exp, &profile);
        UTF_VERIFY_NOT_EQUAL2_PARAM_MAC(GT_OK, st, dev, exp);

        /*
            1.3. Call function profileIndexPtr [NULL]
                               and exp [1].
            Expected: GT_BAD_PTR.
        */
        exp = 1;

        st = cpssDxChCosExpToProfileMapGet(dev, exp, NULL);
        UTF_VERIFY_NOT_EQUAL1_STRING_MAC(GT_OK, st,
                        "%d, profileIndexPtr = NULL", dev);
    }

    exp = 1;

    /* 2. For not-active devices and devices from non-applicable family */
    /* check that function returns GT_BAD_PARAM.                        */

    /* prepare device iterator */
    PRV_TGF_NOT_APPLIC_DEV_RESET_MAC(&dev, UTF_CH1_E | UTF_CH1_DIAMOND_E);

    /* go over all non active devices */
    while(GT_OK == prvUtfNextDeviceGet(&dev, GT_FALSE))
    {
        st = cpssDxChCosExpToProfileMapGet(dev, exp, &profile);
        UTF_VERIFY_EQUAL1_PARAM_MAC(GT_NOT_APPLICABLE_DEVICE, st, dev);
    }

    /* 3. Call function with out of bound value for device id.*/

    dev = PRV_CPSS_MAX_PP_DEVICES_CNS;

    st = cpssDxChCosExpToProfileMapGet(dev, exp, &profile);
    UTF_VERIFY_EQUAL1_PARAM_MAC(GT_BAD_PARAM, st, dev);
}

/*----------------------------------------------------------------------------*/
/*
GT_STATUS cpssDxChCosTrustDsaTagQosModeSet
(
    IN GT_U8      dev,
    IN GT_U8      port,
    IN GT_BOOL    enable
)
*/

UTF_TEST_CASE_MAC(cpssDxChCosTrustDsaTagQosModeSet)
{
/*
ITERATE_DEVICES_PHY_CPU_PORTS (DxChx)
1.1. Call function with enable [GT_FALSE\ GT_TRUE].
Expected: GT_OK.
1.2. Call cpssDxChCosTrustDsaTagQosModeGet with non-NULL enable Ptr.
Expected: GT_OK and enable the same as was set.
*/
    GT_STATUS          st      = GT_OK;

    GT_U8              dev;
    GT_U8              port;
    GT_BOOL            enable;
    GT_BOOL            enableGet;

    /* prepare iterator for go over all active devices */
    PRV_TGF_NOT_APPLIC_DEV_RESET_MAC(&dev, UTF_NONE_FAMILY_E);
    /* 1. Go over all active devices. */
    while (GT_OK == prvUtfNextDeviceGet(&dev, GT_TRUE))
    {
        st = prvUtfNextPhyPortReset(&port, dev);
        UTF_VERIFY_EQUAL0_PARAM_MAC(GT_OK, st);
        /* 1.1. For all active devices go over all available Physical ports. */
        while (GT_OK == prvUtfNextPhyPortGet(&port, GT_TRUE))
        {
            /* 1.1. Call function with enable [GT_FALSE\ GT_TRUE].
            Expected: GT_OK. */
            /* 1.2. Call cpssDxChCosTrustDsaTagQosModeGet with non-NULL enable Ptr.
            Expected: GT_OK and enable the same as was set. */

            /* 1.1. enable == GT_FALSE */
            enable = GT_FALSE;

            st = cpssDxChCosTrustDsaTagQosModeSet(dev, port, enable);
            UTF_VERIFY_EQUAL3_PARAM_MAC(GT_OK, st, dev, port, enable);

            /* 1.2. enable == GT_FALSE */
            st = cpssDxChCosTrustDsaTagQosModeGet(dev, port, &enableGet);
            UTF_VERIFY_EQUAL2_STRING_MAC(GT_OK, st,
                "cpssDxChCosTrustDsaTagQosModeGet: %d, %d", dev, port);
            UTF_VERIFY_EQUAL2_STRING_MAC(enable, enableGet,
                "get another enable value than was set: %d, %d", dev, port);

            /* 1.1. enable == GT_TRUE */
            enable = GT_TRUE;

            st = cpssDxChCosTrustDsaTagQosModeSet(dev, port, enable);
            UTF_VERIFY_EQUAL3_PARAM_MAC(GT_OK, st, dev, port, enable);

            /* 1.2. enable == GT_TRUE */
            st = cpssDxChCosTrustDsaTagQosModeGet(dev, port, &enableGet);
            UTF_VERIFY_EQUAL2_STRING_MAC(GT_OK, st,
                "cpssDxChCosTrustDsaTagQosModeGet: %d, %d", dev, port);
            UTF_VERIFY_EQUAL2_STRING_MAC(enable, enableGet,
                "get another enable value than was set: %d, %d", dev, port);
        }

        enable = GT_FALSE;

        st = prvUtfNextPhyPortReset(&port, dev);
        UTF_VERIFY_EQUAL0_PARAM_MAC(GT_OK, st);
        /* 1.2. For all active devices go over all non available
           physical ports.
        */
        while (GT_OK == prvUtfNextPhyPortGet(&port, GT_FALSE))
        {
            st = cpssDxChCosTrustDsaTagQosModeSet(dev, port, enable);
            UTF_VERIFY_EQUAL2_PARAM_MAC(GT_BAD_PARAM, st, dev, port);
        }

        /* 1.3. For active device check that function returns GT_BAD_PARAM */
        /* for out of bound value for port number.                         */
        port = PRV_CPSS_MAX_PP_PORTS_NUM_CNS;

        st = cpssDxChCosTrustDsaTagQosModeSet(dev, port, enable);
        UTF_VERIFY_EQUAL2_PARAM_MAC(GT_BAD_PARAM, st, dev, port);

        /* 1.4. For active device check that function returns GT_OK */
        /* for CPU port number.                                     */
        port = CPSS_CPU_PORT_NUM_CNS;

        st = cpssDxChCosTrustDsaTagQosModeSet(dev, port, enable);
        UTF_VERIFY_EQUAL2_PARAM_MAC(GT_OK, st, dev, port);
    }

    enable = GT_FALSE;
    port = 0;

    /* 2. For not active devices check that function returns GT_BAD_PARAM.*/
    /* prepare device iterator */
    PRV_TGF_NOT_APPLIC_DEV_RESET_MAC(&dev, UTF_NONE_FAMILY_E);
    /* Go over all non active devices. */
    while(GT_OK == prvUtfNextDeviceGet(&dev, GT_FALSE))
    {
        st = cpssDxChCosTrustDsaTagQosModeSet(dev, port, enable);
        UTF_VERIFY_EQUAL1_PARAM_MAC(GT_NOT_APPLICABLE_DEVICE, st, dev);
    }

    /* 3. Call function with out of bound value for device id */
    dev = PRV_CPSS_MAX_PP_DEVICES_CNS;

    st = cpssDxChCosTrustDsaTagQosModeSet(dev, port, enable);
    UTF_VERIFY_EQUAL1_PARAM_MAC(GT_BAD_PARAM, st, dev);
}

/*----------------------------------------------------------------------------*/
/*
GT_STATUS cpssDxChCosTrustDsaTagQosModeGet
(
    IN GT_U8      dev,
    IN GT_U8      port,
    IN GT_BOOL    *enablePtr
)
*/

UTF_TEST_CASE_MAC(cpssDxChCosTrustDsaTagQosModeGet)
{
/*
ITERATE_DEVICES_PHY_CPU_PORTS (DxChx)
1.1. Call function with non-NULL enablePtr.
Expected: GT_OK.
1.2. Call with enbalePtr [NULL] .
Expected: GT_BAD_PTR.
*/
    GT_STATUS          st      = GT_OK;

    GT_U8              dev;
    GT_U8              port;
    GT_BOOL            enableGet;

    /* prepare iterator for go over all active devices */
    PRV_TGF_NOT_APPLIC_DEV_RESET_MAC(&dev, UTF_NONE_FAMILY_E);
    /* 1. Go over all active devices. */
    while (GT_OK == prvUtfNextDeviceGet(&dev, GT_TRUE))
    {
        st = prvUtfNextPhyPortReset(&port, dev);
        UTF_VERIFY_EQUAL0_PARAM_MAC(GT_OK, st);
        /* 1.1. For all active devices go over all available Physical ports. */
        while (GT_OK == prvUtfNextPhyPortGet(&port, GT_TRUE))
        {
            /* 1.1. Call function with non-NULL enablePtr.
            Expected: GT_OK. */
            st = cpssDxChCosTrustDsaTagQosModeGet(dev, port, &enableGet);
            UTF_VERIFY_EQUAL2_PARAM_MAC(GT_OK, st, dev, port);

            /* 1.2. Call with enbalePtr [NULL] .
            Expected: GT_BAD_PTR. */
            st = cpssDxChCosTrustDsaTagQosModeGet(dev, port, NULL);
            UTF_VERIFY_EQUAL2_STRING_MAC(GT_BAD_PTR, st, "%d, %d, NULL", dev, port);
        }

        st = prvUtfNextPhyPortReset(&port, dev);
        UTF_VERIFY_EQUAL0_PARAM_MAC(GT_OK, st);
        /* 1.2. For all active devices go over all non available
           Physical ports.
        */
        while (GT_OK == prvUtfNextPhyPortGet(&port, GT_FALSE))
        {
            st = cpssDxChCosTrustDsaTagQosModeGet(dev, port, &enableGet);
            UTF_VERIFY_EQUAL2_PARAM_MAC(GT_BAD_PARAM, st, dev, port);
        }

        /* 1.3. For active device check that function returns GT_BAD_PARAM */
        /* for out of bound value for port number.                         */
        port = PRV_CPSS_MAX_PP_PORTS_NUM_CNS;

        st = cpssDxChCosTrustDsaTagQosModeGet(dev, port, &enableGet);
        UTF_VERIFY_EQUAL2_PARAM_MAC(GT_BAD_PARAM, st, dev, port);

        /* 1.4. For active device check that function returns GT_OK */
        /* for CPU port number.                                     */
        port = CPSS_CPU_PORT_NUM_CNS;

        st = cpssDxChCosTrustDsaTagQosModeGet(dev, port, &enableGet);
        UTF_VERIFY_EQUAL2_PARAM_MAC(GT_OK, st, dev, port);
    }

    port = 0;

    /* 2. For not active devices check that function returns GT_BAD_PARAM.*/
    /* prepare device iterator */
    PRV_TGF_NOT_APPLIC_DEV_RESET_MAC(&dev, UTF_NONE_FAMILY_E);
    /* Go over all non active devices. */
    while(GT_OK == prvUtfNextDeviceGet(&dev, GT_FALSE))
    {
        st = cpssDxChCosTrustDsaTagQosModeGet(dev, port, &enableGet);
        UTF_VERIFY_EQUAL1_PARAM_MAC(GT_NOT_APPLICABLE_DEVICE, st, dev);
    }

    /* 3. Call function with out of bound value for device id */
    dev = PRV_CPSS_MAX_PP_DEVICES_CNS;

    st = cpssDxChCosTrustDsaTagQosModeGet(dev, port, &enableGet);
    UTF_VERIFY_EQUAL1_PARAM_MAC(GT_BAD_PARAM, st, dev);
}

/*----------------------------------------------------------------------------*/
/*
cpssDxChCosTrustExpModeSet
(
    IN GT_U8    dev,
    IN GT_U8    port,
    IN GT_BOOL  enable
)
*/
UTF_TEST_CASE_MAC(cpssDxChCosTrustExpModeSet)
{
/*
    ITERATE_DEVICES_PHY_CPU_PORTS (xCat and above)
    1.1.1. Call with enable [GT_FALSE / GT_TRUE].
    Expected: GT_OK.
    1.1.2. Call cpssDxChCosTrustExpModeGet with non-null enablePtr.
    Expected: GT_OK and the same enable.
*/
    GT_STATUS   st   = GT_OK;
    GT_U8       dev;
    GT_U8       port = COS_VALID_PHY_PORT_CNS;

    GT_BOOL   enable    = GT_FALSE;
    GT_BOOL   enableGet = GT_FALSE;


    /* prepare iterator for go over all active devices */
    PRV_TGF_NOT_APPLIC_DEV_RESET_MAC(&dev, UTF_CH1_E | UTF_CH1_DIAMOND_E | UTF_CH2_E | UTF_CH3_E);

    /* 1. Go over all active devices. */
    while(GT_OK == prvUtfNextDeviceGet(&dev, GT_TRUE))
    {
        st = prvUtfNextPhyPortReset(&port, dev);
        UTF_VERIFY_EQUAL0_PARAM_MAC(GT_OK, st);

        /* 1.1. For all active devices go over all available physical ports. */
        while(GT_OK == prvUtfNextPhyPortGet(&port, GT_TRUE))
        {
            /*
                1.1.1. Call with enable [GT_FALSE and GT_TRUE].
                Expected: GT_OK.
            */

            /* Call with enable [GT_FALSE] */
            enable = GT_FALSE;

            st = cpssDxChCosTrustExpModeSet(dev, port, enable);
            UTF_VERIFY_EQUAL3_PARAM_MAC(GT_OK, st, dev, port, enable);

            /*
                1.1.2. Call cpssDxChCosTrustExpModeGet with non-null enablePtr.
                Expected: GT_OK and the same enable.
            */
            st = cpssDxChCosTrustExpModeGet(dev, port, &enableGet);
            UTF_VERIFY_EQUAL2_STRING_MAC(GT_OK, st,
                       "cpssDxChCosTrustExpModeGet: %d, %d", dev, port);

            /* Verifying values */
            UTF_VERIFY_EQUAL2_STRING_MAC(enable, enableGet,
                       "get another enable than was set: %d", dev, port);

            /* Call with enable [GT_TRUE] */
            enable = GT_TRUE;

            st = cpssDxChCosTrustExpModeSet(dev, port, enable);
            UTF_VERIFY_EQUAL3_PARAM_MAC(GT_OK, st, dev, port, enable);

            /*
                1.1.2. Call cpssDxChCosTrustExpModeGet with non-null enablePtr.
                Expected: GT_OK and the same enable.
            */
            st = cpssDxChCosTrustExpModeGet(dev, port, &enableGet);
            UTF_VERIFY_EQUAL2_STRING_MAC(GT_OK, st,
                       "cpssDxChCosTrustExpModeGet: %d, %d", dev, port);

            /* Verifying values */
            UTF_VERIFY_EQUAL2_STRING_MAC(enable, enableGet,
                       "get another enable than was set: %d", dev, port);
        }

        enable = GT_FALSE;

        st = prvUtfNextPhyPortReset(&port, dev);
        UTF_VERIFY_EQUAL0_PARAM_MAC(GT_OK, st);

        /* 1.2. For all active devices go over all non available physical ports. */
        while(GT_OK == prvUtfNextPhyPortGet(&port, GT_FALSE))
        {
            /* 1.2.1. Call function for each non-active port */
            st = cpssDxChCosTrustExpModeSet(dev, port, enable);
            UTF_VERIFY_EQUAL2_PARAM_MAC(GT_BAD_PARAM, st, dev, port);
        }

        /* 1.3. For active device check that function returns GT_BAD_PARAM */
        /* for out of bound value for port number.                         */
        port = PRV_CPSS_MAX_PP_PORTS_NUM_CNS;

        st = cpssDxChCosTrustExpModeSet(dev, port, enable);
        UTF_VERIFY_EQUAL2_PARAM_MAC(GT_BAD_PARAM, st, dev, port);

        /* 1.4. For active device check that function returns GT_OK        */
        /* for CPU port number.                                            */
        port = CPSS_CPU_PORT_NUM_CNS;

        st = cpssDxChCosTrustExpModeSet(dev, port, enable);
        UTF_VERIFY_EQUAL2_PARAM_MAC(GT_OK, st, dev, port);
    }

    enable = GT_FALSE;

    /* 2. For not active devices check that function returns non GT_OK.*/
    port = COS_VALID_PHY_PORT_CNS;

    /* prepare device iterator */
    PRV_TGF_NOT_APPLIC_DEV_RESET_MAC(&dev, UTF_CH1_E | UTF_CH1_DIAMOND_E | UTF_CH2_E | UTF_CH3_E);

    /* Go over all non active devices. */
    while(GT_OK == prvUtfNextDeviceGet(&dev, GT_FALSE))
    {
        st = cpssDxChCosTrustExpModeSet(dev, port, enable);
        UTF_VERIFY_EQUAL1_PARAM_MAC(GT_NOT_APPLICABLE_DEVICE, st, dev);
    }

    /* 3.Call function with out of bound value for device id */
    dev = PRV_CPSS_MAX_PP_DEVICES_CNS;

    st = cpssDxChCosTrustExpModeSet(dev, port, enable);
    UTF_VERIFY_EQUAL1_PARAM_MAC(GT_BAD_PARAM, st, dev);
}

/*----------------------------------------------------------------------------*/
/*
cpssDxChCosTrustExpModeGet
(
    IN  GT_U8   dev,
    IN  GT_U8   port,
    OUT GT_BOOL *enablePtr
)
*/
UTF_TEST_CASE_MAC(cpssDxChCosTrustExpModeGet)
{
/*
    ITERATE_DEVICES_PHY_CPU_PORTS (DxCh3 and above)
    1.1.1. Call with non-NULL enablePtr.
    Expected: GT_OK.
    1.1.2. Call with enablePtr [NULL].
    Expected: GT_BAD_PTR.
*/
    GT_STATUS   st   = GT_OK;
    GT_U8       dev;
    GT_U8       port = COS_VALID_PHY_PORT_CNS;

    GT_BOOL   enable = GT_FALSE;


    /* prepare iterator for go over all active devices */
    PRV_TGF_NOT_APPLIC_DEV_RESET_MAC(&dev, UTF_CH1_E | UTF_CH1_DIAMOND_E | UTF_CH2_E | UTF_CH3_E);

    /* 1. Go over all active devices. */
    while(GT_OK == prvUtfNextDeviceGet(&dev, GT_TRUE))
    {
        st = prvUtfNextPhyPortReset(&port, dev);
        UTF_VERIFY_EQUAL0_PARAM_MAC(GT_OK, st);

        /* 1.1. For all active devices go over all available physical ports. */
        while(GT_OK == prvUtfNextPhyPortGet(&port, GT_TRUE))
        {
            /*
                1.1.1. Call with non-NULL enablePtr.
                Expected: GT_OK.
            */
            st = cpssDxChCosTrustExpModeGet(dev, port, &enable);
            UTF_VERIFY_EQUAL2_PARAM_MAC(GT_OK, st, dev, port);

            /*
                1.1.2. Call with enablePtr [NULL].
                Expected: GT_BAD_PTR.
            */
            st = cpssDxChCosTrustExpModeGet(dev, port, NULL);
            UTF_VERIFY_EQUAL2_STRING_MAC(GT_BAD_PTR, st, "%d, %d, enablePtr = NULL", dev, port);
        }

        st = prvUtfNextPhyPortReset(&port, dev);
        UTF_VERIFY_EQUAL0_PARAM_MAC(GT_OK, st);

        /* 1.2. For all active devices go over all non available physical ports. */
        while(GT_OK == prvUtfNextPhyPortGet(&port, GT_FALSE))
        {
            /* 1.2.1. Call function for each non-active port */
            st = cpssDxChCosTrustExpModeGet(dev, port, &enable);
            UTF_VERIFY_EQUAL2_PARAM_MAC(GT_BAD_PARAM, st, dev, port);
        }

        /* 1.3. For active device check that function returns GT_BAD_PARAM */
        /* for out of bound value for port number.                         */
        port = PRV_CPSS_MAX_PP_PORTS_NUM_CNS;

        st = cpssDxChCosTrustExpModeGet(dev, port, &enable);
        UTF_VERIFY_EQUAL2_PARAM_MAC(GT_BAD_PARAM, st, dev, port);

        /* 1.4. For active device check that function returns GT_OK        */
        /* for CPU port number.                                            */
        port = CPSS_CPU_PORT_NUM_CNS;

        st = cpssDxChCosTrustExpModeGet(dev, port, &enable);
        UTF_VERIFY_EQUAL2_PARAM_MAC(GT_OK, st, dev, port);
    }

    /* 2. For not active devices check that function returns non GT_OK.*/
    port = COS_VALID_PHY_PORT_CNS;

    /* prepare device iterator */
    PRV_TGF_NOT_APPLIC_DEV_RESET_MAC(&dev, UTF_CH1_E | UTF_CH1_DIAMOND_E | UTF_CH2_E | UTF_CH3_E);

    /* Go over all non active devices. */
    while(GT_OK == prvUtfNextDeviceGet(&dev, GT_FALSE))
    {
        st = cpssDxChCosTrustExpModeGet(dev, port, &enable);
        UTF_VERIFY_EQUAL1_PARAM_MAC(GT_NOT_APPLICABLE_DEVICE, st, dev);
    }

    /* 3.Call function with out of bound value for device id */
    dev = PRV_CPSS_MAX_PP_DEVICES_CNS;

    st = cpssDxChCosTrustExpModeGet(dev, port, &enable);
    UTF_VERIFY_EQUAL1_PARAM_MAC(GT_BAD_PARAM, st, dev);
}

/*----------------------------------------------------------------------------*/
/*
cpssDxChCosL2TrustModeVlanTagSelectSet
(
    IN GT_U8                     devNum,
    IN GT_U8                     port,
    IN CPSS_VLAN_TAG_TYPE_ENT    vlanTagType
)
*/
UTF_TEST_CASE_MAC(cpssDxChCosL2TrustModeVlanTagSelectSet)
{
/*
    ITERATE_DEVICES_PHY_CPU_PORTS (xCat and above)
    1.1.1. Call with vlanTagType [CPSS_VLAN_TAG0_E / CPSS_VLAN_TAG1_E].
    Expected: GT_OK.
    1.1.2. Call cpssDxChCosL2TrustModeVlanTagSelectGet with non-null vlanTagTypePtr.
    Expected: GT_OK and the same vlanTagType.
    1.1.3. Call with wrong enum values vlanTagType .
    Expected: GT_BAD_PARAM.
*/
    GT_STATUS   st   = GT_OK;
    GT_U8       dev;
    GT_U8       port = COS_VALID_PHY_PORT_CNS;

    CPSS_VLAN_TAG_TYPE_ENT vlanTagType = CPSS_VLAN_TAG0_E;
    CPSS_VLAN_TAG_TYPE_ENT vlanTagTypeGet = CPSS_VLAN_TAG0_E;

    /* prepare iterator for go over all active devices */
    PRV_TGF_NOT_APPLIC_DEV_RESET_MAC(&dev, UTF_CH1_E | UTF_CH1_DIAMOND_E | UTF_CH2_E | UTF_CH3_E);

    /* 1. Go over all active devices. */
    while(GT_OK == prvUtfNextDeviceGet(&dev, GT_TRUE))
    {
        st = prvUtfNextPhyPortReset(&port, dev);
        UTF_VERIFY_EQUAL0_PARAM_MAC(GT_OK, st);

        /* 1.1. For all active devices go over all available physical ports. */
        while(GT_OK == prvUtfNextPhyPortGet(&port, GT_TRUE))
        {
            /*
                1.1.1. Call with vlanTagType [CPSS_VLAN_TAG0_E / CPSS_VLAN_TAG1_E].
                Expected: GT_OK.
            */

            /* Call with vlanTagType [CPSS_VLAN_TAG0_E] */
            vlanTagType = CPSS_VLAN_TAG0_E;

            st = cpssDxChCosL2TrustModeVlanTagSelectSet(dev, port, vlanTagType);
            UTF_VERIFY_EQUAL3_PARAM_MAC(GT_OK, st, dev, port, vlanTagType);

            /*
                1.1.2. Call cpssDxChCosL2TrustModeVlanTagSelectGet with non-null vlanTagTypePtr.
                Expected: GT_OK and the same vlanTagType.
            */
            st = cpssDxChCosL2TrustModeVlanTagSelectGet(dev, port, &vlanTagTypeGet);
            UTF_VERIFY_EQUAL2_STRING_MAC(GT_OK, st,
                       "cpssDxChCosL2TrustModeVlanTagSelectGet: %d, %d", dev, port);

            /* Verifying values */
            UTF_VERIFY_EQUAL2_STRING_MAC(vlanTagType, vlanTagTypeGet,
                       "get another vlanTagType than was set: %d", dev, port);


            /* Call with vlanTagType [CPSS_VLAN_TAG1_E] */
            vlanTagType = CPSS_VLAN_TAG1_E;

            st = cpssDxChCosL2TrustModeVlanTagSelectSet(dev, port, vlanTagType);
            UTF_VERIFY_EQUAL3_PARAM_MAC(GT_OK, st, dev, port, vlanTagType);

            /*
                1.1.2. Call cpssDxChCosL2TrustModeVlanTagSelectGet with non-null vlanTagTypePtr.
                Expected: GT_OK and the same vlanTagType.
            */
            st = cpssDxChCosL2TrustModeVlanTagSelectGet(dev, port, &vlanTagTypeGet);
            UTF_VERIFY_EQUAL2_STRING_MAC(GT_OK, st,
                       "cpssDxChCosL2TrustModeVlanTagSelectGet: %d, %d", dev, port);

            /* Verifying values */
            UTF_VERIFY_EQUAL2_STRING_MAC(vlanTagType, vlanTagTypeGet,
                       "get another vlanTagType than was set: %d", dev, port);

            /*
                1.1.3. Call with wrong enum values vlanTagType.
                Expected: GT_BAD_PARAM.
            */
            UTF_ENUMS_CHECK_MAC(cpssDxChCosL2TrustModeVlanTagSelectSet
                                (dev, port, vlanTagType),
                                vlanTagType);
        }

        st = prvUtfNextPhyPortReset(&port, dev);
        UTF_VERIFY_EQUAL0_PARAM_MAC(GT_OK, st);

        /* 1.2. For all active devices go over all non available physical ports. */
        while(GT_OK == prvUtfNextPhyPortGet(&port, GT_FALSE))
        {
            /* 1.2.1. Call function for each non-active port */
            st = cpssDxChCosL2TrustModeVlanTagSelectSet(dev, port, vlanTagType);
            UTF_VERIFY_EQUAL2_PARAM_MAC(GT_BAD_PARAM, st, dev, port);
        }

        /* 1.3. For active device check that function returns GT_BAD_PARAM */
        /* for out of bound value for port number.                         */
        port = PRV_CPSS_MAX_PP_PORTS_NUM_CNS;

        st = cpssDxChCosL2TrustModeVlanTagSelectSet(dev, port, vlanTagType);
        UTF_VERIFY_EQUAL2_PARAM_MAC(GT_BAD_PARAM, st, dev, port);

        /* 1.4. For active device check that function returns GT_OK        */
        /* for CPU port number.                                            */
        port = CPSS_CPU_PORT_NUM_CNS;

        st = cpssDxChCosL2TrustModeVlanTagSelectSet(dev, port, vlanTagType);
        UTF_VERIFY_EQUAL2_PARAM_MAC(GT_OK, st, dev, port);
    }

    vlanTagType = GT_FALSE;

    /* 2. For not active devices check that function returns non GT_OK.*/
    port = COS_VALID_PHY_PORT_CNS;

    /* prepare device iterator */
    PRV_TGF_NOT_APPLIC_DEV_RESET_MAC(&dev, UTF_CH1_E | UTF_CH1_DIAMOND_E | UTF_CH2_E | UTF_CH3_E);

    /* Go over all non active devices. */
    while(GT_OK == prvUtfNextDeviceGet(&dev, GT_FALSE))
    {
        st = cpssDxChCosL2TrustModeVlanTagSelectSet(dev, port, vlanTagType);
        UTF_VERIFY_EQUAL1_PARAM_MAC(GT_NOT_APPLICABLE_DEVICE, st, dev);
    }

    /* 3.Call function with out of bound value for device id */
    dev = PRV_CPSS_MAX_PP_DEVICES_CNS;

    st = cpssDxChCosL2TrustModeVlanTagSelectSet(dev, port, vlanTagType);
    UTF_VERIFY_EQUAL1_PARAM_MAC(GT_BAD_PARAM, st, dev);
}

/*----------------------------------------------------------------------------*/
/*
cpssDxChCosL2TrustModeVlanTagSelectGet
(
    IN  GT_U8                     devNum,
    IN  GT_U8                     port,
    OUT CPSS_VLAN_TAG_TYPE_ENT    *vlanTagTypePtr
)
*/
UTF_TEST_CASE_MAC(cpssDxChCosL2TrustModeVlanTagSelectGet)
{
/*
    ITERATE_DEVICES_PHY_CPU_PORTS (xCat and above)
    1.1.1. Call with non-NULL vlanTagTypePtr.
    Expected: GT_OK.
    1.1.2. Call with vlanTagTypePtr [NULL].
    Expected: GT_BAD_PTR.
*/
    GT_STATUS   st   = GT_OK;
    GT_U8       dev;
    GT_U8       port = COS_VALID_PHY_PORT_CNS;

    CPSS_VLAN_TAG_TYPE_ENT vlanTagType;


    /* prepare iterator for go over all active devices */
    PRV_TGF_NOT_APPLIC_DEV_RESET_MAC(&dev, UTF_CH1_E | UTF_CH1_DIAMOND_E | UTF_CH2_E | UTF_CH3_E);

    /* 1. Go over all active devices. */
    while(GT_OK == prvUtfNextDeviceGet(&dev, GT_TRUE))
    {
        st = prvUtfNextPhyPortReset(&port, dev);
        UTF_VERIFY_EQUAL0_PARAM_MAC(GT_OK, st);

        /* 1.1. For all active devices go over all available physical ports. */
        while(GT_OK == prvUtfNextPhyPortGet(&port, GT_TRUE))
        {
            /*
                1.1.1. Call with non-NULL vlanTagTypePtr.
                Expected: GT_OK.
            */
            st = cpssDxChCosL2TrustModeVlanTagSelectGet(dev, port, &vlanTagType);
            UTF_VERIFY_EQUAL2_PARAM_MAC(GT_OK, st, dev, port);

            /*
                1.1.2. Call with vlanTagTypePtr [NULL].
                Expected: GT_BAD_PTR.
            */
            st = cpssDxChCosL2TrustModeVlanTagSelectGet(dev, port, NULL);
            UTF_VERIFY_EQUAL2_STRING_MAC(GT_BAD_PTR, st,
                             "%d, %d, vlanTagTypePtr = NULL", dev, port);
        }

        st = prvUtfNextPhyPortReset(&port, dev);
        UTF_VERIFY_EQUAL0_PARAM_MAC(GT_OK, st);

        /* 1.2. For all active devices go over all non available physical ports. */
        while(GT_OK == prvUtfNextPhyPortGet(&port, GT_FALSE))
        {
            /* 1.2.1. Call function for each non-active port */
            st = cpssDxChCosL2TrustModeVlanTagSelectGet(dev, port, &vlanTagType);
            UTF_VERIFY_EQUAL2_PARAM_MAC(GT_BAD_PARAM, st, dev, port);
        }

        /* 1.3. For active device check that function returns GT_BAD_PARAM */
        /* for out of bound value for port number.                         */
        port = PRV_CPSS_MAX_PP_PORTS_NUM_CNS;

        st = cpssDxChCosL2TrustModeVlanTagSelectGet(dev, port, &vlanTagType);
        UTF_VERIFY_EQUAL2_PARAM_MAC(GT_BAD_PARAM, st, dev, port);

        /* 1.4. For active device check that function returns GT_OK        */
        /* for CPU port number.                                            */
        port = CPSS_CPU_PORT_NUM_CNS;

        st = cpssDxChCosL2TrustModeVlanTagSelectGet(dev, port, &vlanTagType);
        UTF_VERIFY_EQUAL2_PARAM_MAC(GT_OK, st, dev, port);
    }

    /* 2. For not active devices check that function returns non GT_OK.*/
    port = COS_VALID_PHY_PORT_CNS;

    /* prepare device iterator */
    PRV_TGF_NOT_APPLIC_DEV_RESET_MAC(&dev, UTF_CH1_E | UTF_CH1_DIAMOND_E | UTF_CH2_E | UTF_CH3_E);

    /* Go over all non active devices. */
    while(GT_OK == prvUtfNextDeviceGet(&dev, GT_FALSE))
    {
        st = cpssDxChCosL2TrustModeVlanTagSelectGet(dev, port, &vlanTagType);
        UTF_VERIFY_EQUAL1_PARAM_MAC(GT_NOT_APPLICABLE_DEVICE, st, dev);
    }

    /* 3.Call function with out of bound value for device id */
    dev = PRV_CPSS_MAX_PP_DEVICES_CNS;

    st = cpssDxChCosL2TrustModeVlanTagSelectGet(dev, port, &vlanTagType);
    UTF_VERIFY_EQUAL1_PARAM_MAC(GT_BAD_PARAM, st, dev);
}

/*----------------------------------------------------------------------------*/
/*
GT_STATUS cpssDxChCosPortUpProfileIndexSet
(
    IN  GT_U8  dev,
    IN  GT_U8  portNum,
    IN  GT_U8  up,
    IN  GT_U32 upProfileIndex
)
*/
UTF_TEST_CASE_MAC(cpssDxChCosPortUpProfileIndexSet)
{
/*
    ITERATE_DEVICES_PHY_CPU_PORTS(Lion and above)
    1.1. Call with up[0 / 3 / 7], upProfileIndex[0 / 1 / 1],
    Expected: GT_OK.
    1.2. Call cpssDxChCosPortUpProfileIndexGet with the same parameters.
    Expected: GT_OK and the same values than was set.
    1.3. Call api with wrong up [8].
    Expected: NOT GT_OK.
    1.4. Call api with wrong upProfileIndex [2].
    Expected: NOT GT_OK.
*/
    GT_STATUS st = GT_OK;

    GT_U8  dev;
    GT_U8  portNum = COS_VALID_PHY_PORT_CNS;
    GT_U8  up = 0;
    GT_U32 upProfileIndex = 0;
    GT_U32 upProfileIndexGet = 0;

    /* prepare device iterator */
    PRV_TGF_NOT_APPLIC_DEV_RESET_MAC(&dev, UTF_CH1_E | UTF_CH1_DIAMOND_E | UTF_CH2_E | UTF_CH3_E | UTF_XCAT_E);

    /* 1. Go over all active devices. */
    while (GT_OK == prvUtfNextDeviceGet(&dev, GT_TRUE))
    {
        st = prvUtfNextPhyPortReset(&portNum, dev);
        UTF_VERIFY_EQUAL0_PARAM_MAC(GT_OK, st);

        /* 1.1. For all active devices go over all available physical ports. */
        while(GT_OK == prvUtfNextPhyPortGet(&portNum, GT_TRUE))
        {

            /*
                1.1. Call with up[0 / 3 / 7], upProfileIndex[0 / 1 / 1],
                Expected: GT_OK.
            */
            /* call with up[0], upProfileIndex[0] */
            up = 0;
            upProfileIndex = 0;

            st = cpssDxChCosPortUpProfileIndexSet(dev, portNum, up, upProfileIndex);
            UTF_VERIFY_EQUAL1_PARAM_MAC(GT_OK, st, dev);

            /*
                1.2. Call cpssDxChCosPortUpProfileIndexGet with the same parameters.
                Expected: GT_OK and the same values than was set.
            */
            st = cpssDxChCosPortUpProfileIndexGet(dev, portNum, up, &upProfileIndexGet);
            UTF_VERIFY_EQUAL1_STRING_MAC(GT_OK, st,
                      "cpssDxChCosPortUpProfileIndexGet: %d ", dev);

            /* Verifying values */
            UTF_VERIFY_EQUAL1_STRING_MAC(upProfileIndex, upProfileIndexGet,
                           "got another upProfileIndex then was set: %d", dev);

            /* call with up[3], upProfileIndex[1] */
            up = 3;
            upProfileIndex = 1;

            st = cpssDxChCosPortUpProfileIndexSet(dev, portNum, up, upProfileIndex);
            UTF_VERIFY_EQUAL1_PARAM_MAC(GT_OK, st, dev);

            /* call get func again, 1.2. */
            st = cpssDxChCosPortUpProfileIndexGet(dev, portNum, up, &upProfileIndexGet);
            UTF_VERIFY_EQUAL1_STRING_MAC(GT_OK, st,
                      "cpssDxChCosPortUpProfileIndexGet: %d ", dev);

            /* Verifying values */
            UTF_VERIFY_EQUAL1_STRING_MAC(upProfileIndex, upProfileIndexGet,
                           "got another upProfileIndex then was set: %d", dev);

            /* call with up[7], upProfileIndex[1] */
            up = 7;
            upProfileIndex = 1;

            st = cpssDxChCosPortUpProfileIndexSet(dev, portNum, up, upProfileIndex);
            UTF_VERIFY_EQUAL1_PARAM_MAC(GT_OK, st, dev);

            /* call get func again, 1.2. */
            st = cpssDxChCosPortUpProfileIndexGet(dev, portNum, up, &upProfileIndexGet);
            UTF_VERIFY_EQUAL1_STRING_MAC(GT_OK, st,
                      "cpssDxChCosPortUpProfileIndexGet: %d ", dev);

            /* Verifying values */
            UTF_VERIFY_EQUAL1_STRING_MAC(upProfileIndex, upProfileIndexGet,
                           "got another upProfileIndex then was set: %d", dev);

            /*
                1.3. Call api with wrong up [8].
                Expected: NOT GT_OK.
            */
            up = 8;

            st = cpssDxChCosPortUpProfileIndexSet(dev, portNum, up, upProfileIndex);
            UTF_VERIFY_NOT_EQUAL1_PARAM_MAC(GT_OK, st, dev);

            up = 0;

            /*
                1.4. Call api with wrong upProfileIndex [2].
                Expected: NOT GT_OK.
            */
            upProfileIndex = 2;

            st = cpssDxChCosPortUpProfileIndexSet(dev, portNum, up, upProfileIndex);
            UTF_VERIFY_NOT_EQUAL1_PARAM_MAC(GT_OK, st, dev);

            upProfileIndex = 0;
        }

        st = prvUtfNextPhyPortReset(&portNum, dev);
        UTF_VERIFY_EQUAL0_PARAM_MAC(GT_OK, st);

        /* 1.2. For all active devices go over all non available physical ports. */
        while(GT_OK == prvUtfNextPhyPortGet(&portNum, GT_FALSE))
        {
            /* 1.2.1. Call function for each non-active portNum */
            st = cpssDxChCosPortUpProfileIndexSet(dev, portNum, up, upProfileIndex);
            UTF_VERIFY_EQUAL2_PARAM_MAC(GT_BAD_PARAM, st, dev, portNum);
        }

        /* 1.3. For active device check that function returns GT_BAD_PARAM */
        /* for out of bound value for portNum number.                         */
        portNum = PRV_CPSS_MAX_PP_PORTS_NUM_CNS;

        st = cpssDxChCosPortUpProfileIndexSet(dev, portNum, up, upProfileIndex);
        UTF_VERIFY_EQUAL2_PARAM_MAC(GT_BAD_PARAM, st, dev, portNum);

        /* 1.4. For active device check that function returns GT_OK        */
        /* for CPU portNum number.                                            */
        portNum = CPSS_CPU_PORT_NUM_CNS;

        st = cpssDxChCosPortUpProfileIndexSet(dev, portNum, up, upProfileIndex);
        UTF_VERIFY_EQUAL2_PARAM_MAC(GT_OK, st, dev, portNum);
    }

    /* restore correct values */
    portNum = COS_VALID_PHY_PORT_CNS;
    up = 0;
    upProfileIndex = 0;

    /* 2. For not-active devices and devices from non-applicable family */
    /* check that function returns GT_BAD_PARAM.                        */

    /* prepare device iterator */
    PRV_TGF_NOT_APPLIC_DEV_RESET_MAC(&dev, UTF_CH1_E | UTF_CH1_DIAMOND_E | UTF_CH2_E | UTF_CH3_E | UTF_XCAT_E);

    /* go over all non active devices */
    while(GT_OK == prvUtfNextDeviceGet(&dev, GT_FALSE))
    {
        st = cpssDxChCosPortUpProfileIndexSet(dev, portNum, up, upProfileIndex);
        UTF_VERIFY_EQUAL1_PARAM_MAC(GT_NOT_APPLICABLE_DEVICE, st, dev);
    }

    /* 3. Call function with out of bound value for device id.*/
    dev = PRV_CPSS_MAX_PP_DEVICES_CNS;

    st = cpssDxChCosPortUpProfileIndexSet(dev, portNum, up, upProfileIndex);
    UTF_VERIFY_EQUAL1_PARAM_MAC(GT_BAD_PARAM, st, dev);
}

/*----------------------------------------------------------------------------*/
/*
GT_STATUS cpssDxChCosPortUpProfileIndexGet
(
    IN  GT_U8   dev,
    IN  GT_U8   portNum,
    IN  GT_U8   up,
    OUT GT_U32  *upProfileIndexPtr
)
*/
UTF_TEST_CASE_MAC(cpssDxChCosPortUpProfileIndexGet)
{
/*
    ITERATE_DEVICES_PHY_CPU_PORTS(Lion and above)
    1.1. Call with up[0 / 3 / 7].
    Expected: GT_OK.
    1.2. Call api with wrong up [8].
    Expected: NOT GT_OK.
    1.3. Call api with wrong upProfileIndexPtr [NULL].
    Expected: GT_BAD_PTR.
*/
    GT_STATUS st = GT_OK;

    GT_U8   dev;
    GT_U8   portNum = COS_VALID_PHY_PORT_CNS;
    GT_U8   up = 0;
    GT_U32  upProfileIndex;

    /* prepare device iterator */
    PRV_TGF_NOT_APPLIC_DEV_RESET_MAC(&dev, UTF_CH1_E | UTF_CH1_DIAMOND_E | UTF_CH2_E | UTF_CH3_E | UTF_XCAT_E);

    /* 1. Go over all active devices. */
    while (GT_OK == prvUtfNextDeviceGet(&dev, GT_TRUE))
    {
        st = prvUtfNextPhyPortReset(&portNum, dev);
        UTF_VERIFY_EQUAL0_PARAM_MAC(GT_OK, st);

        /* 1.1. For all active devices go over all available physical ports. */
        while(GT_OK == prvUtfNextPhyPortGet(&portNum, GT_TRUE))
        {
            /*
                1.1. Call with up[0 / 3 / 7].
                Expected: GT_OK.
            */
            /* call with up[0] */
            up = 0;

            st = cpssDxChCosPortUpProfileIndexGet(dev, portNum, up, &upProfileIndex);
            UTF_VERIFY_EQUAL1_PARAM_MAC(GT_OK, st, dev);

            /* call with up[3] */
            up = 3;

            st = cpssDxChCosPortUpProfileIndexGet(dev, portNum, up, &upProfileIndex);
            UTF_VERIFY_EQUAL1_PARAM_MAC(GT_OK, st, dev);

            /* call with up[7] */
            up = 7;

            st = cpssDxChCosPortUpProfileIndexGet(dev, portNum, up, &upProfileIndex);
            UTF_VERIFY_EQUAL1_PARAM_MAC(GT_OK, st, dev);


            /*
                1.2. Call api with wrong up [8].
                Expected: NOT GT_OK.
            */
            up = 8;

            st = cpssDxChCosPortUpProfileIndexGet(dev, portNum, up, &upProfileIndex);
            UTF_VERIFY_NOT_EQUAL1_PARAM_MAC(GT_OK, st, dev);

            up = 0;

            /*
                1.3. Call api with wrong upProfileIndexPtr [NULL].
                Expected: GT_BAD_PTR.
            */
            st = cpssDxChCosPortUpProfileIndexGet(dev, portNum, up, NULL);
            UTF_VERIFY_EQUAL1_STRING_MAC(GT_BAD_PTR, st,
                                       "%d, upProfileIndexPtr = NULL", dev);
        }

        st = prvUtfNextPhyPortReset(&portNum, dev);
        UTF_VERIFY_EQUAL0_PARAM_MAC(GT_OK, st);

        /* 1.2. For all active devices go over all non available physical ports. */
        while(GT_OK == prvUtfNextPhyPortGet(&portNum, GT_FALSE))
        {
            /* 1.2.1. Call function for each non-active portNum */
            st = cpssDxChCosPortUpProfileIndexGet(dev, portNum, up, &upProfileIndex);
            UTF_VERIFY_EQUAL2_PARAM_MAC(GT_BAD_PARAM, st, dev, portNum);
        }

        /* 1.3. For active device check that function returns GT_BAD_PARAM */
        /* for out of bound value for portNum number.                         */
        portNum = PRV_CPSS_MAX_PP_PORTS_NUM_CNS;

        st = cpssDxChCosPortUpProfileIndexGet(dev, portNum, up, &upProfileIndex);
        UTF_VERIFY_EQUAL2_PARAM_MAC(GT_BAD_PARAM, st, dev, portNum);

        /* 1.4. For active device check that function returns GT_OK        */
        /* for CPU portNum number.                                            */
        portNum = CPSS_CPU_PORT_NUM_CNS;

        st = cpssDxChCosPortUpProfileIndexGet(dev, portNum, up, &upProfileIndex);
        UTF_VERIFY_EQUAL2_PARAM_MAC(GT_OK, st, dev, portNum);
    }

    /* restore correct values */
    portNum = 0;
    up = 0;

    /* 2. For not-active devices and devices from non-applicable family */
    /* check that function returns GT_BAD_PARAM.                        */

    /* prepare device iterator */
    PRV_TGF_NOT_APPLIC_DEV_RESET_MAC(&dev, UTF_CH1_E | UTF_CH1_DIAMOND_E | UTF_CH2_E | UTF_CH3_E | UTF_XCAT_E);

    /* go over all non active devices */
    while(GT_OK == prvUtfNextDeviceGet(&dev, GT_FALSE))
    {
        st = cpssDxChCosPortUpProfileIndexGet(dev, portNum, up, &upProfileIndex);
        UTF_VERIFY_EQUAL1_PARAM_MAC(GT_NOT_APPLICABLE_DEVICE, st, dev);
    }

    /* 3. Call function with out of bound value for device id.*/
    dev = PRV_CPSS_MAX_PP_DEVICES_CNS;

    st = cpssDxChCosPortUpProfileIndexGet(dev, portNum, up, &upProfileIndex);
    UTF_VERIFY_EQUAL1_PARAM_MAC(GT_BAD_PARAM, st, dev);
}


/*----------------------------------------------------------------------------*/
/*
    Test function to Fill Profile table.
*/
UTF_TEST_CASE_MAC(cpssDxChCosFillProfileTable)
{
/*
    ITERATE_DEVICE (DxChx)
    1.1. Get table Size.
         Call cpssDxChCfgTableNumEntriesGet with table [CPSS_DXCH_CFG_TABLE_QOS_PROFILE_E]
                                                 and non-NULL numEntriesPtr.
    Expected: GT_OK.
    1.2. Fill all entries in Profile table.
         Call cpssDxChCosProfileEntrySet with profileIndex [0..numEntries-1],
                                         cosPtr { CPSS_DP_GREEN_E , 1, 1, 1, 1}
    Expected: GT_OK.
    1.3. Try to write entry with index out of range.
         Call cpssDxChCosProfileEntrySet with profileIndex [numEntries],
                                         cosPtr { CPSS_DP_GREEN_E , 1, 1, 1, 1}
    Expected: NOT GT_OK.
    1.4. Read all entries in Profile table and compare with original.
         Call cpssDxChCosProfileEntryGet with same profileIndex.
    Expected: GT_OK and the same cosPtr.
    1.5. Try to read entry with index out of range.
         Call cpssDxChCosProfileEntryGet with profileIndex [numEntries],
    Expected: NOT GT_OK.
*/
    GT_STATUS   st = GT_OK;
    GT_U8       dev;

    GT_U32      numEntries = 0;
    GT_U8       iTemp      = 0;

    CPSS_DXCH_COS_PROFILE_STC cos;
    CPSS_DXCH_COS_PROFILE_STC cosGet;

    CPSS_PP_FAMILY_TYPE_ENT   devFamily;


    cpssOsBzero((GT_VOID*) &cos, sizeof(cos));
    cpssOsBzero((GT_VOID*) &cos, sizeof(cos));

    /* Fill the entry for Profile table */
    cos.dropPrecedence = CPSS_DP_GREEN_E;
    cos.userPriority   = 1;
    cos.trafficClass   = 1;
    cos.dscp           = 1;
    cos.exp            = 1;

    /* prepare device iterator */
    PRV_TGF_NOT_APPLIC_DEV_RESET_MAC(&dev, UTF_NONE_FAMILY_E);

    /* 1. Go over all active devices. */
    while (GT_OK == prvUtfNextDeviceGet(&dev, GT_TRUE))
    {
        /* Getting device family */
        st = prvUtfDeviceFamilyGet(dev, &devFamily);
        UTF_VERIFY_EQUAL1_STRING_MAC(GT_OK, st, "prvUtfDeviceFamilyGet: %d", dev);

        /* 1.1. Get table Size */
        st = cpssDxChCfgTableNumEntriesGet(dev, CPSS_DXCH_CFG_TABLE_QOS_PROFILE_E, &numEntries);
        UTF_VERIFY_EQUAL1_STRING_MAC(GT_OK, st, "cpssDxChCfgTableNumEntriesGet: %d", dev);

        /* 1.2. Fill all entries in Profile table */
        for(iTemp = 0; iTemp < numEntries; ++iTemp)
        {
            /* make every entry unique */
            cos.userPriority   = iTemp % 7;
            cos.trafficClass   = iTemp % 7;
            cos.dscp           = iTemp % 63;
            cos.exp            = iTemp % 7;

            st = cpssDxChCosProfileEntrySet(dev, iTemp, &cos);
            UTF_VERIFY_EQUAL2_STRING_MAC(GT_OK, st, "cpssDxChCosProfileEntrySet: %d, %d", dev, iTemp);
        }

        /* 1.3. Try to write entry with index out of range. */
        st = cpssDxChCosProfileEntrySet(dev, (GT_U8)numEntries, &cos);
        UTF_VERIFY_NOT_EQUAL2_STRING_MAC(GT_OK, st, "cpssDxChCosProfileEntrySet: %d, %d", dev, numEntries);

        /* 1.4. Read all entries in Profile table and compare with original */
        for(iTemp = 0; iTemp < numEntries; ++iTemp)
        {
            /* restore unique entry before compare */
            cos.userPriority   = iTemp % 7;
            cos.trafficClass   = iTemp % 7;
            cos.dscp           = iTemp % 63;
            cos.exp            = iTemp % 7;

            st = cpssDxChCosProfileEntryGet(dev, iTemp, &cosGet);
            UTF_VERIFY_EQUAL2_STRING_MAC(GT_OK, st, "cpssDxChCosProfileEntryGet: %d, %d", dev, iTemp);

            /* Checking cos == cosGet */
            UTF_VERIFY_EQUAL1_STRING_MAC(cos.dropPrecedence, cosGet.dropPrecedence,
                           "get another cosPtr->dropPrecedence than was set: %d", dev);
            UTF_VERIFY_EQUAL1_STRING_MAC(cos.userPriority, cosGet.userPriority,
                           "get another cosPtr->userPriority than was set: %d", dev);
            UTF_VERIFY_EQUAL1_STRING_MAC(cos.trafficClass, cosGet.trafficClass,
                           "get another cosPtr->trafficClass than was set: %d", dev);
            UTF_VERIFY_EQUAL1_STRING_MAC(cos.dscp, cosGet.dscp,
                           "get another cosPtr->dscp than was set: %d", dev);
            if(devFamily != CPSS_PP_FAMILY_CHEETAH_E)
            {
                UTF_VERIFY_EQUAL1_STRING_MAC(cos.exp, cosGet.exp,
                               "get another cosPtr->exp than was set: %d", dev);
            }
        }

        /* 1.5. Try to read entry with index out of range. */
        st = cpssDxChCosProfileEntryGet(dev, (GT_U8)numEntries, &cosGet);
        UTF_VERIFY_NOT_EQUAL2_STRING_MAC(GT_OK, st, "cpssDxChCosProfileEntryGet: %d, %d", dev, numEntries);
    }
}

/*----------------------------------------------------------------------------*/
/*
    Test function to Fill MAC QoS Table.
*/
UTF_TEST_CASE_MAC(cpssDxChCosFillMacQosTable)
{
/*
    ITERATE_DEVICE (DxChx)
    1.1. Get table Size. numEntries [8].
    1.2. Fill all entries in MAC QoS table.
         Call cpssDxChCosMacQosEntrySet with entryIdx [1 ...numEntries-1],
                                        valid macQosCfg_PTR {1,
                                                             CPSS_PACKET_ATTRIBUTE_ASSIGN_PRECEDENCE_SOFT_E,
                                                             CPSS_PACKET_ATTRIBUTE_MODIFY_KEEP_PREVIOUS_E,
                                                             CPSS_PACKET_ATTRIBUTE_MODIFY_KEEP_PREVIOUS_E}.
    Expected: GT_OK.
    1.3. Try to write entry with index out of range.
         Call cpssDxChCosMacQosEntrySet with entryIdx [numEntries],
                                        valid macQosCfg_PTR {1,
                                                             CPSS_PACKET_ATTRIBUTE_ASSIGN_PRECEDENCE_SOFT_E,
                                                             CPSS_PACKET_ATTRIBUTE_MODIFY_KEEP_PREVIOUS_E,
                                                             CPSS_PACKET_ATTRIBUTE_MODIFY_KEEP_PREVIOUS_E}.
    Expected: NOT GT_OK.
    1.4. Read all entries in MAC QoS table and compare with original.
         Call cpssDxChCosMacQosEntryGet with the same entryIdx.
    Expected: GT_OK and the same macQosCfg_PTR.
    1.5. Try to read entry with index out of range.
         Call cpssDxChCosMacQosEntryGet with entryIdx [numEntries].
    Expected: NOT GT_OK.
*/
    GT_STATUS   st = GT_OK;
    GT_U8       dev;

    GT_U32      numEntries = 0;
    GT_U32      iTemp      = 0;

    CPSS_QOS_ENTRY_STC      macCfg;
    CPSS_QOS_ENTRY_STC      macCfgGet;


    cpssOsBzero((GT_VOID*) &macCfg, sizeof(macCfg));
    cpssOsBzero((GT_VOID*) &macCfg, sizeof(macCfg));

    /* Fill the entry for MAC QoS table */
    macCfg.qosProfileId     = 1;
    macCfg.assignPrecedence = CPSS_PACKET_ATTRIBUTE_ASSIGN_PRECEDENCE_SOFT_E;
    macCfg.enableModifyUp   = CPSS_PACKET_ATTRIBUTE_MODIFY_KEEP_PREVIOUS_E;
    macCfg.enableModifyDscp = CPSS_PACKET_ATTRIBUTE_MODIFY_KEEP_PREVIOUS_E;

    /* prepare device iterator */
    PRV_TGF_NOT_APPLIC_DEV_RESET_MAC(&dev, UTF_NONE_FAMILY_E);

    /* 1. Go over all active devices. */
    while (GT_OK == prvUtfNextDeviceGet(&dev, GT_TRUE))
    {
        /* 1.1. Get table Size */
        numEntries = 8;

        /* 1.2. Fill all entries in MAC QoS table */
        for(iTemp = 1; iTemp < numEntries; ++iTemp)
        {
            st = cpssDxChCosMacQosEntrySet(dev, iTemp, &macCfg);
            UTF_VERIFY_EQUAL2_STRING_MAC(GT_OK, st, "cpssDxChCosMacQosEntrySet: %d, %d", dev, iTemp);
        }

        /* 1.3. Try to write entry with index out of range. */
        st = cpssDxChCosMacQosEntrySet(dev, numEntries, &macCfg);
        UTF_VERIFY_NOT_EQUAL2_STRING_MAC(GT_OK, st, "cpssDxChCosMacQosEntrySet: %d, %d", dev, numEntries);

        /* 1.4. Read all entries in MAC QoS table and compare with original */
        for(iTemp = 1; iTemp < numEntries; ++iTemp)
        {
            st = cpssDxChCosMacQosEntryGet(dev, iTemp, &macCfgGet);
            UTF_VERIFY_EQUAL2_STRING_MAC(GT_OK, st, "cpssDxChCosMacQosEntryGet: %d, %d", dev, iTemp);

            /* Verifying macQosCfg_PTR */
            UTF_VERIFY_EQUAL2_STRING_MAC(macCfg.qosProfileId, macCfgGet.qosProfileId,
                "get another macQosCfg_PTR->qosProfileId than was set: %d, %d", dev, iTemp);
            UTF_VERIFY_EQUAL2_STRING_MAC(macCfg.assignPrecedence, macCfgGet.assignPrecedence,
                "get another macQosCfg_PTR->assignPrecedence than was set: %d, %d", dev, iTemp);
            UTF_VERIFY_EQUAL2_STRING_MAC(macCfg.enableModifyUp, macCfgGet.enableModifyUp,
                "get another macQosCfg_PTR->enableModifyUp than was set: %d, %d", dev, iTemp);
            UTF_VERIFY_EQUAL2_STRING_MAC(macCfg.enableModifyDscp, macCfgGet.enableModifyDscp,
                "get another macQosCfg_PTR->enableModifyDscp than was set: %d, %d", dev, iTemp);
        }

        /* 1.5. Try to read entry with index out of range. */
        st = cpssDxChCosMacQosEntryGet(dev, numEntries, &macCfgGet);
        UTF_VERIFY_NOT_EQUAL2_STRING_MAC(GT_OK, st, "cpssDxChCosMacQosEntryGet: %d, %d", dev, numEntries);
    }
}

/*----------------------------------------------------------------------------*/
/*
 * Configuration of cpssDxChCos suit
 */
UTF_SUIT_BEGIN_TESTS_MAC(cpssDxChCos)
    UTF_SUIT_DECLARE_TEST_MAC(cpssDxChCosProfileEntrySet)
    UTF_SUIT_DECLARE_TEST_MAC(cpssDxChCosProfileEntryGet)
    UTF_SUIT_DECLARE_TEST_MAC(cpssDxChCosDscpToProfileMapSet)
    UTF_SUIT_DECLARE_TEST_MAC(cpssDxChCosDscpToProfileMapGet)
    UTF_SUIT_DECLARE_TEST_MAC(cpssDxChCosDscpMutationEntrySet)
    UTF_SUIT_DECLARE_TEST_MAC(cpssDxChCosDscpMutationEntryGet)
    UTF_SUIT_DECLARE_TEST_MAC(cpssDxChCosUpCfiDeiToProfileMapSet)
    UTF_SUIT_DECLARE_TEST_MAC(cpssDxChCosUpCfiDeiToProfileMapGet)
    UTF_SUIT_DECLARE_TEST_MAC(cpssDxChCosPortDpToCfiDeiMapEnableSet)
    UTF_SUIT_DECLARE_TEST_MAC(cpssDxChCosPortDpToCfiDeiMapEnableGet)
    UTF_SUIT_DECLARE_TEST_MAC(cpssDxChCosDpToCfiDeiMapSet)
    UTF_SUIT_DECLARE_TEST_MAC(cpssDxChCosDpToCfiDeiMapGet)
    UTF_SUIT_DECLARE_TEST_MAC(cpssDxChCosPortQosConfigSet)
    UTF_SUIT_DECLARE_TEST_MAC(cpssDxChCosPortQosConfigGet)
    UTF_SUIT_DECLARE_TEST_MAC(cpssDxChCosMacQosEntrySet)
    UTF_SUIT_DECLARE_TEST_MAC(cpssDxChCosMacQosEntryGet)
    UTF_SUIT_DECLARE_TEST_MAC(cpssDxChCosMacQosConflictResolutionSet)
    UTF_SUIT_DECLARE_TEST_MAC(cpssDxChCosMacQosConflictResolutionGet)
    UTF_SUIT_DECLARE_TEST_MAC(cpssDxChCosRemarkTblEntrySet)
    UTF_SUIT_DECLARE_TEST_MAC(cpssDxChCosRemarkTblEntryGet)
    UTF_SUIT_DECLARE_TEST_MAC(cpssDxChPortDefaultUPSet)
    UTF_SUIT_DECLARE_TEST_MAC(cpssDxChPortDefaultUPGet)
    UTF_SUIT_DECLARE_TEST_MAC(cpssDxChPortModifyUPSet)
    UTF_SUIT_DECLARE_TEST_MAC(cpssDxChPortModifyUPGet)
    UTF_SUIT_DECLARE_TEST_MAC(cpssDxChCosPortQosTrustModeSet)
    UTF_SUIT_DECLARE_TEST_MAC(cpssDxChCosPortQosTrustModeGet)
    UTF_SUIT_DECLARE_TEST_MAC(cpssDxChCosPortReMapDSCPSet)
    UTF_SUIT_DECLARE_TEST_MAC(cpssDxChCosPortReMapDSCPGet)
    UTF_SUIT_DECLARE_TEST_MAC(cpssDxChCosPortVlanQoSCfgEntryGet)
    UTF_SUIT_DECLARE_TEST_MAC(cpssDxChCosRemarkEntryGet)
    UTF_SUIT_DECLARE_TEST_MAC(cpssDxChCosQoSProfileEntryGet)
    UTF_SUIT_DECLARE_TEST_MAC(cpssDxChCosExpToProfileMapSet)
    UTF_SUIT_DECLARE_TEST_MAC(cpssDxChCosExpToProfileMapGet)
    UTF_SUIT_DECLARE_TEST_MAC(cpssDxChCosTrustDsaTagQosModeSet)
    UTF_SUIT_DECLARE_TEST_MAC(cpssDxChCosTrustDsaTagQosModeGet)
    UTF_SUIT_DECLARE_TEST_MAC(cpssDxChCosTrustExpModeSet)
    UTF_SUIT_DECLARE_TEST_MAC(cpssDxChCosTrustExpModeGet)
    UTF_SUIT_DECLARE_TEST_MAC(cpssDxChCosL2TrustModeVlanTagSelectSet)
    UTF_SUIT_DECLARE_TEST_MAC(cpssDxChCosL2TrustModeVlanTagSelectGet)
    UTF_SUIT_DECLARE_TEST_MAC(cpssDxChCosPortUpProfileIndexSet)
    UTF_SUIT_DECLARE_TEST_MAC(cpssDxChCosPortUpProfileIndexGet)

    /* Test filling Table */
    UTF_SUIT_DECLARE_TEST_MAC(cpssDxChCosFillProfileTable)
    UTF_SUIT_DECLARE_TEST_MAC(cpssDxChCosFillMacQosTable)
UTF_SUIT_END_TESTS_MAC(cpssDxChCos)

