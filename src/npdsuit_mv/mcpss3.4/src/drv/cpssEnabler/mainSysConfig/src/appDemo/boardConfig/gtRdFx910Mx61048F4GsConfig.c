/*******************************************************************************
*              Copyright 2002, GALILEO TECHNOLOGY, LTD.
*
* THIS CODE CONTAINS CONFIDENTIAL INFORMATION OF MARVELL. NO RIGHTS ARE GRANTED
* HEREIN UNDER ANY PATENT, MASK WORK RIGHT OR COPYRIGHT OF MARVELL OR ANY THIRD
* PARTY. MARVELL RESERVES THE RIGHT AT ITS SOLE DISCRETION TO REQUEST THAT THIS
* CODE BE IMMEDIATELY RETURNED TO MARVELL. THIS CODE IS PROVIDED "AS IS".
* MARVELL MAKES NO WARRANTIES, EXPRESS, IMPLIED OR OTHERWISE, REGARDING ITS
* ACCURACY, COMPLETENESS OR PERFORMANCE. MARVELL COMPRISES MARVELL TECHNOLOGY
* GROUP LTD. (MTGL) AND ITS SUBSIDIARIES, MARVELL INTERNATIONAL LTD. (MIL),
* MARVELL TECHNOLOGY, INC. (MTI), MARVELL SEMICONDUCTOR, INC. (MSI), MARVELL
* ASIA PTE LTD. (MAPL), MARVELL JAPAN K.K. (MJKK), GALILEO TECHNOLOGY LTD. (GTL)
* AND GALILEO TECHNOLOGY, INC. (GTI).
********************************************************************************
* gtRd-fx910Mx61048F4GsConfig.c
*
* DESCRIPTION:
*       Initialization functions for the RD-Fx910-MX610-48F4GS boards.
*
* DEPENDENCIES:
*       None.
*
* FILE REVISION NUMBER:
*       $Revision: 1.1.1.1 $
*
*******************************************************************************/

#include <appDemo/os/appOs.h>
#include <appDemo/boardConfig/appDemoBoardConfig.h>
#include <appDemo/boardConfig/appDemoCfgMisc.h>
#include <appDemo/sysHwConfig/gtAppDemoSysConfigDefaults.h>

#include <appDemo/sysHwConfig/gtAppDemoXbarConfig.h>
#include <appDemo/sysHwConfig/gtAppDemoFaConfig.h>

#include <cpss/generic/config/private/prvCpssConfigTypes.h>
#include <cpss/exMx/exMxGen/bridge/cpssExMxBrgFdb.h>


/*******************************************************************************
 * External variables
 ******************************************************************************/
extern APP_DEMO_PP_CONFIG    appDemoPpConfigList[PRV_CPSS_MAX_PP_DEVICES_CNS];


/*******************************************************************************
* getBoardInfo
*
* DESCRIPTION:
*       Return the board configuration info.
*
* INPUTS:
*       boardRevId  - The board revision Id.
*
* OUTPUTS:
*       isB2bSystem - GT_TRUE, the system is a B2B system.
*       numOfPp     - Number of Packet processors in system.
*       numOfFa     - Number of Fabric Adaptors in system.
*       numOfXbar   - Number of Crossbar devices in system.
*
* RETURNS:
*       GT_OK   - on success,
*       GT_FAIL - otherwise.
*
* COMMENTS:
*       None.
*
*******************************************************************************/
static GT_STATUS getBoardInfo
(
    IN  GT_U8   boardRevId,
    OUT GT_U8   *numOfPp,
    OUT GT_U8   *numOfFa,
    OUT GT_U8   *numOfXbar,
    OUT GT_BOOL *isB2bSystem
)
{
    GT_STATUS   retVal;                 /* Function return value.       */
    GT_U8       ppCounter;              /* Number of Packet processors. */
    GT_U8       i;                      /* Loop index.                  */

    retVal = appDemoSysGetPciInfo();
    if(retVal != GT_OK)
    {
        return retVal;
    }

    ppCounter = 0;
    for(i = 0; i < PRV_CPSS_MAX_PP_DEVICES_CNS; i++)
    {
        if(appDemoPpConfigList[i].valid == GT_TRUE)
        {
            ppCounter++;
        }
    }

    *numOfPp    = ppCounter;
    *numOfFa    = ppCounter;
    *numOfXbar  = ppCounter;


    *isB2bSystem = GT_FALSE;

    return GT_OK;
}


#ifdef IMPL_XBAR
/*******************************************************************************
* getXbarCfgParams
*
* DESCRIPTION:
*       Return the parameters for xbarSysConfig().
*
* INPUTS:
*       boardRevId      - The board revision Id.
*
* OUTPUTS:
*      xbarCoreSysCfg   - xbar sys config phase1 parameters struct.
*
* RETURNS:
*       None.
*
* COMMENTS:
*       None.
*
*******************************************************************************/
static GT_STATUS getXbarCfgParams
(
    IN   GT_U8                   boardRevId,
    OUT  GT_XBAR_CORE_SYS_CFG    *xbarCoreSysCfg
)
{
    GT_XBAR_CORE_SYS_CFG  localXbarCfgParams = GT_XBAR_SYS_CFG_DEF;

    osMemCpy(xbarCoreSysCfg,&localXbarCfgParams,
             sizeof(GT_XBAR_CORE_SYS_CFG));

    return GT_OK;
}
#endif

/*******************************************************************************
* getPpPhase1Config
*
* DESCRIPTION:
*       Returns the configuration parameters for corePpHwPhase1Init().
*
* INPUTS:
*       boardRevId      - The board revision Id.
*       devIdx          - The Pp Index to get the parameters for.
*
* OUTPUTS:
*       ppPhase1Params  - Phase1 parameters struct.
*
* RETURNS:
*       GT_OK   - on success,
*       GT_FAIL - otherwise.
*
* COMMENTS:
*       None.
*
*******************************************************************************/
static GT_STATUS getPpPhase1Config
(
    IN  GT_U8                       boardRevId,
    IN  GT_U8                       devIdx,
    OUT CPSS_PP_PHASE1_INIT_PARAMS  *phase1Params
)
{
    CPSS_PP_PHASE1_INIT_PARAMS    localPpPh1Config = CPSS_PP_PHASE1_DEFAULT;
    void    *intVecNum;

    localPpPh1Config.devNum             = devIdx;
    localPpPh1Config.baseAddr           =
        appDemoPpConfigList[devIdx].pciInfo.pciBaseAddr;
    localPpPh1Config.internalPciBase    =
        appDemoPpConfigList[devIdx].pciInfo.internalPciBase;
    localPpPh1Config.deviceId           =
        ((appDemoPpConfigList[devIdx].pciInfo.pciDevVendorId.devId << 16) |
         (appDemoPpConfigList[devIdx].pciInfo.pciDevVendorId.vendorId));

    extDrvGetPciIntVec(GT_PCI_INT_D,&intVecNum);
    localPpPh1Config.intVecNum  = (GT_U32)intVecNum;
    extDrvGetIntMask(GT_PCI_INT_D,&(localPpPh1Config.intMask));


    switch(boardRevId)
    {
        case 1:
        case 2:
            localPpPh1Config.coreClk = 133;
            break;
        case 3:
        case 4:
        case 5:
        case 6:
            localPpPh1Config.coreClk = 166;
            break;
        default:
            return GT_FAIL;
    }


    localPpPh1Config.uplinkCfg = CPSS_PP_UPLINK_FA_E;

    /* External buffer Dram configurations  */
    localPpPh1Config.bufferDram = CPSS_BUFFER_DRAM_USE_DEFAULT_DRAM_CONF_E;
    localPpPh1Config.bufferDramCfg.overwriteDef             = GT_TRUE;
    localPpPh1Config.bufferDramCfg.mrs                      = 0x51;
    localPpPh1Config.bufferDramCfg.emrs                     = 0x0;
    localPpPh1Config.bufferDramCfg.dramTiming.tdqss         = 0x0;
    localPpPh1Config.bufferDramCfg.dramTiming.trcd          = 0x5;
    localPpPh1Config.bufferDramCfg.dramTiming.trp           = 0x2;
    localPpPh1Config.bufferDramCfg.dramTiming.twr           = 0x2;
    localPpPh1Config.bufferDramCfg.dramTiming.twtr          = 0x3;
    localPpPh1Config.bufferDramCfg.dramTiming.tras          = 0xA;
    localPpPh1Config.bufferDramCfg.dramTiming.trrd          = 0x1;
    localPpPh1Config.bufferDramCfg.dramTiming.tmrd          = 0x0;

    localPpPh1Config.bufferDramCfg.dramTiming.trfc          = 0x9;
    localPpPh1Config.bufferDramCfg.dramTiming.rd2rdDelay    = 0x0;
    localPpPh1Config.bufferDramCfg.dramTiming.rd2wrDelay    = 0x0;

    localPpPh1Config.bufferDramCfg.dramTiming.writeCount    = 0x3;
    localPpPh1Config.bufferDramCfg.dramTiming.readCount     = 0x3;

    localPpPh1Config.bufferDramCfg.operation                = 0x0;
    localPpPh1Config.bufferDramCfg.trCdWr                   = 0x0;

    /* Set the refresh count according to the BD frequency. */
    localPpPh1Config.bufferDramCfg.dramFrequency = CPSS_DRAM_FREQ_300_MHZ_E;
    switch(localPpPh1Config.bufferDramCfg.dramFrequency)
    {
        case(CPSS_DRAM_FREQ_200_MHZ_E):
            localPpPh1Config.bufferDramCfg.refreshCounter           = 0x550;
            break;
        case(CPSS_DRAM_FREQ_250_MHZ_E):
            localPpPh1Config.bufferDramCfg.refreshCounter           = 0x6D0;
            break;
        case(CPSS_DRAM_FREQ_300_MHZ_E):
            localPpPh1Config.bufferDramCfg.refreshCounter           = 0x850;
            break;
        default:
            localPpPh1Config.bufferDramCfg.refreshCounter           = 0xE00;
    }

    localPpPh1Config.coreClk = 166;
    localPpPh1Config.mngInterfaceType                     = CPSS_CHANNEL_PCI_E;
    osMemCpy(phase1Params,&localPpPh1Config,sizeof(CPSS_PP_PHASE1_INIT_PARAMS));
    return GT_OK;
}

#ifdef IMPL_FA
/*******************************************************************************
* getFaPhase1Config
*
* DESCRIPTION:
*       Returns the configuration parameters for coreFaHwPhase1Init().
*
* INPUTS:
*       boardRevId      - The board revision Id.
*       devIdx          - The Fa Index to get the parameters for.
*
* OUTPUTS:
*       faPhase1Params  - Phase1 parameters struct.
*
* RETURNS:
*       GT_OK   - on success,
*       GT_FAIL - otherwise.
*
* COMMENTS:
*       None.
*
*******************************************************************************/
static GT_STATUS getFaPhase1Config
(
    IN  GT_U8                       boardRevId,
    IN  GT_U8                       devIdx,
    OUT GT_FA_PHASE1_INIT_PARAMS    *phase1Params
)
{
    GT_FA_PHASE1_INIT_PARAMS    localFaPh1Config = FA_PHASE1_DEFAULT;


    localFaPh1Config.devNum                 = devIdx;

    localFaPh1Config.intConfig.intType      = GT_INT_2_UPLINK_GPP;
    localFaPh1Config.intConfig.intVecNum    = devIdx;
    localFaPh1Config.bufferDramCfg.overwriteDef = GT_FALSE;

    osMemCpy(phase1Params,&localFaPh1Config,sizeof(GT_FA_PHASE1_INIT_PARAMS));
    return GT_OK;
}
#endif

#if defined (IMPL_FA) || defined (IMPL_XBAR)
/*******************************************************************************
* getXbarPhase1Config
*
* DESCRIPTION:
*       Returns the configuration parameters for coreFaHwPhase1Init().
*
* INPUTS:
*       boardRevId      - The board revision Id.
*       devIdx          - The Fa Index to get the parameters for.
*
* OUTPUTS:
*       faPhase1Params  - Phase1 parameters struct.
*
* RETURNS:
*       GT_OK   - on success,
*       GT_FAIL - otherwise.
*
* COMMENTS:
*       None.
*
*******************************************************************************/
static GT_STATUS getXbarPhase1Config
(
    IN  GT_U8                       boardRevId,
    IN  GT_U8                       devIdx,
    OUT GT_XBAR_PHASE1_INIT_PARAMS  *phase1Params
)
{
    GT_XBAR_PHASE1_INIT_PARAMS  localXbarPh1Config =
                            GT_XBAR_FOX_PHASE1_DEF_PARAMS;
    GT_XBAR_DEVICE              xbarDeviceType = GT_XBAR_DEF_DEV_TYPE;
    void    *intVecNum;

    switch(boardRevId)
    {
        case 1:
        case 3:
        case 5:
            localXbarPh1Config.hgParamsListLen = 4;
            localXbarPh1Config.hgParamsList =
                osMalloc(localXbarPh1Config.hgParamsListLen *
                         sizeof(GT_HG_INIT_PARAMS));

            if(localXbarPh1Config.hgParamsList == NULL)
            {
                return GT_FAIL;
            }

            appDemoHgGetDefInitParams(xbarDeviceType,
                                      &localXbarPh1Config.hgParamsListLen,
                                      localXbarPh1Config.hgParamsList);

            localXbarPh1Config.hgParamsList[2].swapLaneOrder = GT_TRUE;
            localXbarPh1Config.devNum = devIdx;

            break;

        case 2:
        case 4:
        case 6:
            localXbarPh1Config.hgParamsListLen = 4;
            localXbarPh1Config.hgParamsList =
                osMalloc(localXbarPh1Config.hgParamsListLen *
                         sizeof(GT_HG_INIT_PARAMS));

            if(localXbarPh1Config.hgParamsList == NULL)
            {
                return GT_FAIL;
            }
            xbarDeviceType = GT_XBAR_98FX900A;

            appDemoHgGetDefInitParams(xbarDeviceType,
                                      &localXbarPh1Config.hgParamsListLen,
                                      localXbarPh1Config.hgParamsList);

            localXbarPh1Config.devNum = devIdx;
            break;

        default:
            return GT_FAIL;
    }

    if(xbarDeviceType == GT_XBAR_98FX9010A)
    {
        localXbarPh1Config.intConfig.intType = GT_INT_2_PCI_D;
        extDrvGetPciIntVec(GT_PCI_INT_D,&intVecNum);
        localXbarPh1Config.intConfig.intVecNum  = (GT_U32)intVecNum;
        extDrvGetIntMask(GT_PCI_INT_D,&(localXbarPh1Config.intConfig.intMask));
    }
    osMemCpy(phase1Params,&localXbarPh1Config,
             sizeof(GT_XBAR_PHASE1_INIT_PARAMS));
    return GT_OK;
}
#endif



/*******************************************************************************
* configBoardAfterPhase1
*
* DESCRIPTION:
*       This function performs all needed configurations that should be done
*       after phase1.
*
* INPUTS:
*       boardRevId      - The board revision Id.
*
* OUTPUTS:
*       None.
*
* RETURNS:
*       GT_OK   - on success,
*       GT_FAIL - otherwise.
*
* COMMENTS:
*       None.
*
*******************************************************************************/
static GT_STATUS configBoardAfterPhase1
(
    IN  GT_U8   boardRevId
)
{
    return GT_OK;
}


/*******************************************************************************
* getPpPhase2Config
*
* DESCRIPTION:
*       Returns the configuration parameters for corePpHwPhase2Init().
*
* INPUTS:
*       boardRevId      - The board revision Id.
*       oldDevNum       - The old Pp device number to get the parameters for.
*
* OUTPUTS:
*       ppPhase2Params  - Phase2 parameters struct.
*
* RETURNS:
*       GT_OK   - on success,
*       GT_FAIL - otherwise.
*
* COMMENTS:
*       None.
*
*******************************************************************************/
static GT_STATUS getPpPhase2Config
(
    IN  GT_U8                       boardRevId,
    IN  GT_U8                       oldDevNum,
    OUT CPSS_PP_PHASE2_INIT_PARAMS  *phase2Params
)
{
    CPSS_PP_PHASE2_INIT_PARAMS    localPpPh2Config = CPSS_PP_PHASE2_DEFAULT;
    GT_STATUS   retVal;

    localPpPh2Config.devNum     = oldDevNum;
    localPpPh2Config.deviceId   = CPSS_98EX115D_CNS /*GT_98EX120B*/;
    localPpPh2Config.baseAddr   =
        appDemoPpConfigList[oldDevNum].pciInfo.pciBaseAddr;

    retVal = appDemoAllocateDmaMem(localPpPh2Config.deviceId,RX_DESC_NUM_DEF,
                                   RX_BUFF_SIZE_DEF,RX_BUFF_ALLIGN_DEF,
                                   TX_DESC_NUM_DEF,AU_DESC_NUM_DEF,
                                   &localPpPh2Config);

    osMemCpy(phase2Params,&localPpPh2Config,sizeof(CPSS_PP_PHASE2_INIT_PARAMS));
    return retVal;
}


/*******************************************************************************
* configBoardAfterPhase2
*
* DESCRIPTION:
*       This function performs all needed configurations that should be done
*       after phase2.
*
* INPUTS:
*       boardRevId      - The board revision Id.
*
* OUTPUTS:
*       None.
*
* RETURNS:
*       GT_OK   - on success,
*       GT_FAIL - otherwise.
*
* COMMENTS:
*       None.
*
*******************************************************************************/
static GT_STATUS configBoardAfterPhase2
(
    IN  GT_U8   boardRevId
)
{
    GT_U8   i;


#ifdef IMPL_CM
    GT_STATUS status;
    /* all ommited features will be CORE_API_CM_PORT_VIRT_AS_IS_E */
    static GT_CM_FEATURE_SUPPORT_STC defSwitchFeatureStatus[] =
    {
        {GT_FEATURE_IPV6_E,        GT_FEATURE_NOT_SUPPORTED_IGNORE_E} ,
        {GT_FEATURE_IPV4_TUNNEL_E, GT_FEATURE_NOT_SUPPORTED_IGNORE_E} ,
        {GT_FEATURE_TUNNEL_E,      GT_FEATURE_NOT_SUPPORTED_IGNORE_E}
    };


    status = coreCmSetDevFeatureSupport(unitDevList[0], /* twist device number = 0*/
                                        3,
                                        defSwitchFeatureStatus);

    if (GT_OK != status)
        return (status);
#endif

    for(i = 0; i < PRV_CPSS_MAX_PP_DEVICES_CNS; i++)
    {
        if(appDemoPpConfigList[i].valid == GT_TRUE)
        {
            appDemoSetPortPhyAddr(appDemoPpConfigList[i].devNum,6,0x1e);
            appDemoSetPortPhyAddr(appDemoPpConfigList[i].devNum,7,0x1f);
            appDemoSetPortPhyAddr(appDemoPpConfigList[i].devNum,8,0x10);
            appDemoSetPortPhyAddr(appDemoPpConfigList[i].devNum,9,0x11);
            appDemoSetPortPhyAddr(appDemoPpConfigList[i].devNum,10,0x12);
            appDemoSetPortPhyAddr(appDemoPpConfigList[i].devNum,11,0x13);

            appDemoSetPortPhyAddr(appDemoPpConfigList[i].devNum,48,0x8);
            appDemoSetPortPhyAddr(appDemoPpConfigList[i].devNum,49,0x9);
            appDemoSetPortPhyAddr(appDemoPpConfigList[i].devNum,50,0xA);
            appDemoSetPortPhyAddr(appDemoPpConfigList[i].devNum,51,0xB);
        }
    }

    return GT_OK;
}

#ifdef IMPL_FA
/*******************************************************************************
* getFaPhase2Config
*
* DESCRIPTION:
*       Returns the configuration parameters for coreFaHwPhase2Init().
*
* INPUTS:
*       boardRevId      - The board revision Id.
*       oldDevNum       - The old Pp device number to get the parameters for.
*
* OUTPUTS:
*       ppPhase2Params  - Phase2 parameters struct.
*
* RETURNS:
*       GT_OK   - on success,
*       GT_FAIL - otherwise.
*
* COMMENTS:
*       None.
*
*******************************************************************************/
static GT_STATUS getFaPhase2Config
(
    IN  GT_U8                       boardRevId,
    IN  GT_U8                       oldDevNum,
    OUT GT_FA_PHASE2_INIT_PARAMS    *phase2Params
)
{


    phase2Params->devNum    =  oldDevNum;
    phase2Params->oldDevNum =  oldDevNum;
    return GT_OK;
}
#endif

#if defined (IMPL_FA) || defined (IMPL_XBAR)
/*******************************************************************************
* getXbarPhase2Config
*
* DESCRIPTION:
*       Returns the configuration parameters for coreFaHwPhase2Init().
*
* INPUTS:
*       boardRevId      - The board revision Id.
*       oldDevNum       - The old Pp device number to get the parameters for.
*
* OUTPUTS:
*       ppPhase2Params  - Phase2 parameters struct.
*
* RETURNS:
*       GT_OK   - on success,
*       GT_FAIL - otherwise.
*
* COMMENTS:
*       None.
*
*******************************************************************************/
static GT_STATUS getXbarPhase2Config
(
    IN  GT_U8                       boardRevId,
    IN  GT_U8                       oldDevNum,
    OUT GT_XBAR_PHASE2_INIT_PARAMS  *phase2Params
)
{


    phase2Params->devNum    =  oldDevNum;
    phase2Params->oldDevNum =  oldDevNum;
    return GT_OK;
}
#endif

/*******************************************************************************
* getPpLogicalInitParams
*
* DESCRIPTION:
*       Returns the parameters needed for sysPpLogicalInit() function.
*
* INPUTS:
*       boardRevId      - The board revision Id.
*       devNum          - The Pp device number to get the parameters for.
*
* OUTPUTS:
*       ppLogInitParams - Pp logical init parameters struct.
*
* RETURNS:
*       GT_OK   - on success,
*       GT_FAIL - otherwise.
*
* COMMENTS:
*       None.
*
*******************************************************************************/
static GT_STATUS getPpLogicalInitParams
(
    IN  GT_U8                      boardRevId,
    IN  GT_U8                      devNum,
    OUT CPSS_PP_CONFIG_INIT_STC   *ppLogInitParams
)
{
    CPSS_PP_CONFIG_INIT_STC localPpCfgParams = PP_LOGICAL_CONFIG_DEFAULT;

    osMemCpy(ppLogInitParams, &localPpCfgParams, sizeof(CPSS_PP_CONFIG_INIT_STC));

    return GT_OK;
}


/*******************************************************************************
* getTapiLibInitParams
*
* DESCRIPTION:
*       Returns Tapi library initialization parameters.
*
* INPUTS:
*       boardRevId      - The board revision Id.
*       devNum          - The Pp device number to get the parameters for.
*
* OUTPUTS:
*       libInitParams   - Library initialization parameters.
*
* RETURNS:
*       GT_OK   - on success,
*       GT_FAIL - otherwise.
*
* COMMENTS:
*       None.
*
*******************************************************************************/
static GT_STATUS getTapiLibInitParams
(
    IN  GT_U8                       boardRevId,
    IN  GT_U8                       devNum,
    OUT APP_DEMO_LIB_INIT_PARAMS   *libInitParams
)
{
    APP_DEMO_LIB_INIT_PARAMS  localLibInitParams = LIB_INIT_PARAMS_DEFAULT;

    osMemCpy(libInitParams,&localLibInitParams,
             sizeof(APP_DEMO_LIB_INIT_PARAMS));
    return GT_OK;
}

/*******************************************************************************
* afterInitBoardConfig
*
* DESCRIPTION:
*       PSS Board specific configurations that should be done after board
*       initialization.
*
* INPUTS:
*       boardRevId      - The board revision Id.
*
* OUTPUTS:
*       None.
*
* RETURNS:
*       GT_OK   - on success,
*       GT_FAIL - otherwise.
*
* COMMENTS:
*
*******************************************************************************/
static GT_STATUS afterInitBoardConfig
(
    IN  GT_U8                       boardRevId
)
{
    GT_STATUS   rc = GT_OK;


    if (boardRevId > 4)
    {
         GT_U8       i;

        /* go over all devices and virtual ports per device */
        for (i = 0; (i < PRV_CPSS_MAX_PP_DEVICES_CNS) && (GT_OK == rc); i++)
        {
            if (GT_TRUE == appDemoPpConfigList[i].valid)
            {
                GT_U8 dev;
                GT_U8 port;
                GT_U8 numOfPorts;

                /* get device id and number of virtual ports per device */
                dev = appDemoPpConfigList[i].devNum;
                numOfPorts = PRV_CPSS_PP_MAC(dev)->numOfVirtPorts;

                for (port = 0; (port < numOfPorts) && (GT_OK == rc); port++)
                {
                    rc = cpssExMxBrgFdbPortAuToCpuSet(dev, port, CPSS_MAC_AU_MESSAGE_ALL_E, GT_TRUE);
                }
            }
        }
    }
    return GT_OK;
}

/*******************************************************************************
* gtDbMx61048F4GsBoardReg
*
* DESCRIPTION:
*       Registeration function for the DB-Fx910-MX610-48F4GS board.
*
* INPUTS:
*       boardRevId      - The board revision Id.
*
* OUTPUTS:
*       boardCfgFuncs   - Board configuration functions.
*
* RETURNS:
*       GT_OK   - on success,
*       GT_FAIL - otherwise.
*
* COMMENTS:
*       None.
*
*******************************************************************************/
GT_STATUS gtDbFx910Mx61048F4GsBoardReg
(
    IN  GT_U8                   boardRevId,
    OUT GT_BOARD_CONFIG_FUNCS   *boardCfgFuncs
)
{
    if(boardCfgFuncs == NULL)
    {
        return GT_FAIL;
    }

    boardCfgFuncs->boardGetInfo                 = getBoardInfo;
#ifdef IMPL_XBAR
    boardCfgFuncs->boardGetXbarCfgParams        = getXbarCfgParams;
#endif
    boardCfgFuncs->boardGetPpPh1Params          = getPpPhase1Config;
#ifdef IMPL_FA
    boardCfgFuncs->boardGetFaPh1Params          = getFaPhase1Config;
    boardCfgFuncs->boardGetFaPh2Params          = getFaPhase2Config;
#endif
#if defined (IMPL_FA) || defined (IMPL_XBAR)
    boardCfgFuncs->boardGetXbarPh1Params        = getXbarPhase1Config;
    boardCfgFuncs->boardGetXbarPh2Params        = getXbarPhase2Config;
#endif
    boardCfgFuncs->boardAfterPhase1Config       = configBoardAfterPhase1;
    boardCfgFuncs->boardGetPpPh2Params          = getPpPhase2Config;
    boardCfgFuncs->boardAfterPhase2Config       = configBoardAfterPhase2;
    boardCfgFuncs->boardGetPpLogInitParams      = getPpLogicalInitParams;
    boardCfgFuncs->boardGetLibInitParams        = getTapiLibInitParams;
    boardCfgFuncs->boardAfterInitConfig         = afterInitBoardConfig;

    return GT_OK;
}
