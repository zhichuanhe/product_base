/*******************************************************************************
*              (c), Copyright 2006, Marvell International Ltd.                 *
* THIS CODE CONTAINS CONFIDENTIAL INFORMATION OF MARVELL SEMICONDUCTOR, INC.   *
* NO RIGHTS ARE GRANTED HEREIN UNDER ANY PATENT, MASK WORK RIGHT OR COPYRIGHT  *
* OF MARVELL OR ANY THIRD PARTY. MARVELL RESERVES THE RIGHT AT ITS SOLE        *
* DISCRETION TO REQUEST THAT THIS CODE BE IMMEDIATELY RETURNED TO MARVELL.     *
* THIS CODE IS PROVIDED "AS IS". MARVELL MAKES NO WARRANTIES, EXPRESSED,       *
* IMPLIED OR OTHERWISE, REGARDING ITS ACCURACY, COMPLETENESS OR PERFORMANCE.   *
********************************************************************************
* gtDfcdl.h
*
* DESCRIPTION:
*       This file includes functions for Prestera dfcdl table tune.
*
* DEPENDENCIES:
*       None.
*
* FILE REVISION NUMBER:
*       $Revision: 1.1.1.1 $
*
*******************************************************************************/
#ifndef __gtDfcdlh
#define __gtDfcdlh

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

#include <cpss/extServices/os/gtOs/gtGenTypes.h>
#ifdef IMPL_FA
#include <cpssFa/generic/diag/gtFaDiag.h>
#endif

/* To open minimal prints */
#define DFCDL_PRINT

#define MAX_TUNE_PARMA_VALUE 64 /* The maximal value that a DFCDL tune */
                                /* parameter can get (txClk,rxClk,DQS) */

#define DFCDL_MIN_DELAY    0   /* DFCDL Min value */
#define DFCDL_MAX_DELAY    40  /* DFCDL Max value */

#define SDL_PP_SDRAM_SIZE  64  /* DFCDL SDL size of PP Buffer SDRAM */
#define SDL_FOX_SDRAM_SIZE 64  /* DFCDL SDL size of FOX Buffer SDRAM */

/*
 * typedef: enum GT_PORT_SPEED
 *
 * Description: Enumeration of port speeds
 *
 * Enumerations:
 *    GT_SPEED_10    - 10 Mbps
 *    GT_SPEED_100   - 100 Mbps
 *    GT_SPEED_1000  - 1 Gbps
 *    GT_SPEED_10000 - 10 Gbps
 */
typedef enum
{
    GT_SPEED_10,
    GT_SPEED_100,
    GT_SPEED_1000,
    GT_SPEED_10000
}GT_PORT_SPEED;

/*
 * typedef: struct DFCDL_BUFFER_DRAM_SCAN_PARAMS
 *
 * Description: Structure of dram scan parameters.
 *
 * Enumerations:
 *   rxClkStart  - The value of rxClk to start the scanning
 *   rxClkEnd    - The value of rxClk to end the scanning
 *
 *   dqsStart    - The value of dqs to start the scanning
 *   dqsEnd      - The value of dqs to end the scanning
 *
 *   txClkStart  - The value of txClk to start the scanning
 *   txClkEnd    - The value of txClk to end the scanning
 *
 *   testDelay   - The delay between every step in the scan process in
 *                 milisecounds
 *   dqsThreshold - The threshold of dqs to stop the scanning
 *   stepBack     - Step from the limit points
 */
typedef struct
{
    GT_U8  rxClkStart;
    GT_U8  rxClkEnd;
    GT_U8  dqsStart;
    GT_U8  dqsEnd;
    GT_U8  txClkStart;
    GT_U8  txClkEnd;
    GT_U32 testDelay;
    GT_U8  dqsThreshold;
    GT_U8  stepBack;
}DFCDL_BUFFER_DRAM_SCAN_PARAMS;

/*
 * typedef: struct DFCDL_BUFFER_DRAM_PARAMS
 *
 * Description: Structure of dram parameters to set the dfcdl.
 *
 * Enumerations:
 *
 *   dqs         - The value of dqs
 *   clkOut      - The value of clkOut
 *   startBurst  - The value of startBurst.
 *   extTraffic  - GT_TRUE - External traffic generator is used to
 *                               generate the traffic.
 *                 GT_FALSE - The traffic is generated by the CPU.
 *   testDelay   - The delay between every step in the scan process in
 *                 milisecounds
 */
typedef struct
{
    GT_U8                dqs;
    GT_U8                clkOut;
    GT_U8                startBurst;
    GT_BOOL              extTraffic;
    GT_U32               testDelay;
}DFCDL_BUFFER_DRAM_PARAMS;

/*
 * typedef: struct DFCDL_BUFFER_DRAM_TUNE_PARAMS
 *
 * Description:
 *      Contains the parameters that are used to tune the DFCDLs
 *
 * Fields:
 *      rxClk - rxClk.
 *      txClk - txClk.
 *      dqs   - dqs
 *
 * Comment:
 */
typedef struct
{
    GT_U8 rxClk;
    GT_U8 txClk;
    GT_U8 dqs;

}DFCDL_BUFFER_DRAM_TUNE_PARAMS;

/*
 * typedef: struct RXCLK_VS_DQS_WINDOW
 *
 * Description:
 *  Defines a window of DQS values for a specific RXClK value
 *
 * Fields:
 *      rxClkValue      - The value of the RxClk.
 *      dqsValues       - Array of all the DQS values.
 *      dqsValuesLength - Number of entries in the array.
 *
 * Comment:
 */
typedef struct
{
    GT_U8  dqsValues[MAX_TUNE_PARMA_VALUE];
    GT_U8  dqsValuesLength;

}RXCLK_VS_DQS_WINDOW;

/*
 * typedef: enum GT_PCK_PATTERN_TYPE
 *
 * Description:
 *      The pattern type of the packet trnasmited via the CPU.
 *
 * Fields:
 *      GT_PATTERN_TYPE_SSO     - SSO type of the pattern in the packet.
 *      GT_PATTERN_TYPE_RANDOM  - random pattern type.
 *      GT_PATTERN_TYPE_AA55    - AA55 pattern type.
 *      GT_PATTERN_TYPE_SS32    - SS32 type of the pattern in the packet.
 *      GT_PATTERN_TYPE_ZERO    - all zeroes
 *
 * Comment:
 */
typedef enum
{
    GT_PATTERN_TYPE_SSO     = 0,
    GT_PATTERN_TYPE_RANDOM  = 1,
    GT_PATTERN_TYPE_AA55    = 2,
    GT_PATTERN_TYPE_SS32    = 3,
    GT_PATTERN_TYPE_ZERO    = 4,

    GT_PCKT_TYPE_LAST

}GT_PCK_PATTERN_TYPE;

/*
 * typedef: enum GT_DFCDL_DEVICE_TYPE
 *
 * Description: Enumeration of each device participate in dfcdl tuning
 *
 * Enumerations:
 *    GT_DFCDL_PP_E - In the scan process increment every step
 *    GT_DFCDL_FA_E - In the scan process decrement every step
 */
typedef enum
{
    GT_DFCDL_PP  = 0,
    GT_DFCDL_FA

} GT_DFCDL_DEVICE_TYPE;

#ifndef IMPL_FA
/*
 * typedef: enum GT_MEM_TEST_PROFILE
 *
 * Description: memory test profile.
 *
 * Enumerations:
 *      GT_INCREMENTAL - write incremental values and verify.
 *      GT_BIT_TOGGLE  - set bit and verify.
 *      GT_AA_55       - set 0xAAAAAAAA/0x55555555 and verify.
 */
typedef enum
{
    GT_INCREMENTAL,
    GT_BIT_TOGGLE,
    GT_AA_55
}GT_MEM_TEST_PROFILE;
#endif

/*******************************************************************************
* gtDiagDFCDLBufferDramAutoTune
*
* DESCRIPTION:
*       High level function to run Marvell Auto Tuner utility.
*
*       The most parameters are taken by default values. If it's needed
*       specify parameters differ from defaults user should configure and
*       run others functions.
*
* INPUTS:
*       devNum          - The device number to do the scaning.
*       devType         - The device type
*       pattType        - pattern type of traffic
*       externalSB      - GT_TRUE - external S.B., GT_FALSE - internal S.B.
*       rxHPOffsExt     - the offset taken from RxClk hang point in External
*                         S.B. mode.
*       rxHPOffsInt     - the offset taken from RxClk hang point in Internal
*                         S.B. mode.
*       printScanData   - GT_TRUE to print scan data
*
* OUTPUTS:
*       None.
*
* RETURNS :
*       GT_OK   - on success
*       GT_FAIL - on error
*
* COMMENTS:
*
*
*******************************************************************************/
GT_STATUS gtDiagDFCDLBufferDramAutoTune
(
    IN GT_U8                   devNum,
    IN GT_DFCDL_DEVICE_TYPE    devType,
    IN GT_PCK_PATTERN_TYPE     pattType,
    IN GT_BOOL                 externalSB,
    IN GT_U32                  rxHPOffsExt,
    IN GT_U32                  rxHPOffsInt
);

/*******************************************************************************
* gtDfcdlScanConfig
*
* DESCRIPTION:
*       Config the DFCDL scan parameters and initilze the library.
*
* INPUTS:
*       devNum          - The device number.
*       firstPortNumber - The first port number in the configured ports.
*       lastPortNumber  - The last port number in the configured ports.
*       extTraffic      - GT_TRUE - External traffic generator is used to
*                                   generate the traffic.
*                         GT_FALSE - The traffic is generated by the CPU.
*       init            - GT_TRUE -  Initilaize the library.
*                         GT_FALSE - Close the library.
*       externalSB      - GT_TRUE - use external S.B. as default.
*       internalSB      - GT_TRUE - use external S.B. as default.
*
* OUTPUTS:
*       None.
*
* RETURNS :
*       GT_OK   - on success
*       GT_FAIL - on error
*
* COMMENTS:
*
*
*******************************************************************************/
GT_STATUS gtDfcdlScanConfig
(
    IN GT_U8    devNum,
    IN GT_U8    firstPortNum,
    IN GT_U8    lastPortNum,
    IN GT_BOOL  extTraffic,
    IN GT_BOOL  init,
    IN GT_BOOL  externalSB
);

/*******************************************************************************
* gtDfcdlPpAutoScan
*
* DESCRIPTION:
*   This function automatically find and set the configuration for the PP DFCDL.
*   The function stores the DFCDLs in the SRAM.
*
* INPUTS:
*       devNum          - The device number to do the scaning.
*       firstPortNumber - The first port number in the configured ports.
*       lastPortNumber  - The last port number in the configured ports.
*       extTraffic      - GT_TRUE - External traffic generator is used to
*                                   generate the traffic.
*                         GT_FALSE - The traffic is generated by the CPU.
*       scanParams      - The scaning parameters.
*
* OUTPUTS:
*       None.
*
* RETURNS :
*       GT_OK   - on success
*       GT_FAIL - on error
*
* COMMENTS:
*
*
*******************************************************************************/
GT_STATUS gtDfcdlPpAutoScan
(
    IN GT_U8                            devNum,
    IN DFCDL_BUFFER_DRAM_SCAN_PARAMS    *scanParams
);

/*******************************************************************************
* gtDfcdlFaAutoScan
*
* DESCRIPTION:
*   This function automatically find and set the configuration for the Fa DFCDL.
*   The function stores the DFCDLs in the SRAM.
*
* INPUTS:
*       devNum          - The device number to do the scaning.
*       scanParams      - The scaning parameters.
*
* OUTPUTS:
*       None.
*
* RETURNS :
*       GT_OK   - on success
*       GT_FAIL - on error
*
* COMMENTS:
*
*
*******************************************************************************/
GT_STATUS gtDfcdlFaAutoScan
(
    IN GT_U8                            devNum,
    IN DFCDL_BUFFER_DRAM_SCAN_PARAMS    *scanParams
);

/*******************************************************************************
* gtDfcdlConfigInternalTraffic
*
* DESCRIPTION:
*   Configure the internal traffic parameters.
*
* INPUTS:
*       devNum     - Device number.
*       pattType   - Pattern type.
*       pcktLength - The packet size
*
* OUTPUTS:
*       None.
*
* RETURNS :
*
*
*
* COMMENTS:
*
*
*******************************************************************************/
GT_STATUS gtDfcdlConfigInternalTraffic
(
    IN GT_U8                devNum,
    IN GT_PCK_PATTERN_TYPE  pattType,
    IN GT_U32               pcktLength
);

/*******************************************************************************
* gtDfcdlPrintWindows
*
* DESCRIPTION:
*   The function prints the valid TxClk  and rxClk DQS windows.
*
* INPUTS:
*       None.
*
* OUTPUTS:
*       None.
*
* RETURNS :
*
*
*
* COMMENTS:
*   The values that will be printed are the last values of the
*   scan. If PP auto scan was invoked then the values will be the PP TxClk
*   values. If the FA auto scan was invoked then the values will be the FA
*   TxClk values.
*
*******************************************************************************/
GT_STATUS gtDfcdlPrintWindows
(
    GT_VOID
);

/*******************************************************************************
* gtDfcdlGetValidTxClkValues
*
* DESCRIPTION:
*   The function returns the valid TxClk values found in the scan. The function
*   also returns the RxClk and DQS values for the TxClk entry.
*
* INPUTS:
*       None.
*
* OUTPUTS:
*       txClkValidValues - Arrays of valid txClk values.
*       arrayLen         - The number of txClk values in the array.
*
* RETURNS :
*
*
*
* COMMENTS:
*       The memory for the array should be allocated before calling the
*       function. The values that will be returned are the last values of the
*       scan. If PP auto scan was invoked then the values will be the PP TxClk
*       values. If the FA auto scan was invoked then the values will be the FA
*       TxClk values.
*
*******************************************************************************/
GT_STATUS gtDfcdlGetValidTxClkValues
(
    OUT GT_U8   txClkValidValues[DFCDL_MAX_DELAY],
    OUT GT_U8   *arrayLen
);

/*******************************************************************************
* gtDfcdlPrintValidTxClkValues
*
* DESCRIPTION:
*   The function prints the valid TxClk values found in the scan. The function
*   also prints the RxClk and DQS values for the TxClk entry.
*
* INPUTS:
*       None.
*
* OUTPUTS:
*       None.
*
* RETURNS :
*
*
*
* COMMENTS:
*   The values that will be printed are the last values of the
*   scan. If PP auto scan was invoked then the values will be the PP TxClk
*   values. If the FA auto scan was invoked then the values will be the FA
*   TxClk values.
*
*******************************************************************************/
GT_STATUS gtDfcdlPrintValidTxClkValues
(
    GT_VOID
);

/*******************************************************************************
* gtDfcdlGetValidRxClkValues
*
* DESCRIPTION:
*   The function returns the valid RxClk value. For each RxClk value it returns
*   the list of dqs valid values.
*
* INPUTS:
*       None.
*
* OUTPUTS:
*       rxClkValidValues - Arrays of valid rxClk values.
*
* RETURNS :
*
*
*
* COMMENTS:
*       The memory for the array should be allocated before calling the
*       function. The values that will be returned are the last values of the
*       scan. If PP auto scan was invoked then the values will be the PP RxClk
*       values. If the FA auto scan was invoked then the values will be the FA
*       RxClk values. The index of the array is the rxClk value. If the
*       dqsValuesLength = 0 then the rxClk value is not valid
*
*******************************************************************************/
GT_STATUS gtDfcdlGetValidRxClkValues
(
    OUT RXCLK_VS_DQS_WINDOW rxClkValidValues[DFCDL_MAX_DELAY]
);

/*******************************************************************************
* gtDfcdlPrintValidRxClkValues
*
* DESCRIPTION:
*   The function prints the valid RxClk values found in the scan. The function
*   also prints the valid dqs valid for each rxClk value.
*
* INPUTS:
*       None.
*
* OUTPUTS:
*       None.
*
* RETURNS :
*
*
*
* COMMENTS:
*   The values that will be printed are the last values of the
*   scan. If PP auto scan was invoked then the values will be the PP RxClk
*   values. If the FA auto scan was invoked then the values will be the FA
*   RxClk values.
*
*******************************************************************************/
GT_STATUS gtDfcdlPrintValidRxClkValues
(
    GT_VOID
);

/*******************************************************************************
* gtDfcdlGetValues
*
* DESCRIPTION:
*   Get the DFCDL values.
*
* INPUTS:
*       None.
* OUTPUTS:
*       dfcdlValuesArray    - Array containing the DFCDL values.
*
* RETURNS :
*
*
*
* COMMENTS:
*       The memory for the array should be allocated before calling the
*       function. The values that will be returned are the last values of the
*       scan. If PP auto scan was invoked then the values will be the PP DFCDL
*       values. If the FA auto scan was invoked then the values will be the FA
*       DFCDL values.
*
*
*******************************************************************************/
GT_STATUS gtDfcdlGetValues
(
    OUT GT_U32  dfcdlValuesArray[SDL_PP_SDRAM_SIZE]
);

/*******************************************************************************
* gtDfcdlPrintValues
*
* DESCRIPTION:
*   Print the DFCDL values.
*
* INPUTS:
*       None.
*
* OUTPUTS:
*       None.
*
* RETURNS :
*
*
* COMMENTS:
*       The values that will be printed are the last values of the
*       scan. If PP auto scan was invoked then the values will be the PP DFCDL
*       values. If the FA auto scan was invoked then the values will be the FA
*       DFCDL values.
*
*
*******************************************************************************/
GT_STATUS gtDfcdlPrintValues
(
    GT_VOID
);

/*******************************************************************************
* gtDfcdlPpScan
*
* DESCRIPTION:
*   The function set the dfcdl table with the given values and check the MIB
*   counter value.
*
* INPUTS:
*       devNum          - The device number to do the scaning.
*       scanParams      - The scaning parameters.
*       isFirst         - is first entry to scan.
*       isLast          - is this the last entry to scan.
*
* OUTPUTS:
*       errorCnt        - MIB error counter value.
*
* RETURNS :
*       GT_OK   - on success
*       GT_FAIL - on error
*
* COMMENTS:
*
*
*******************************************************************************/
GT_STATUS gtDfcdlPpScan
(
    IN  GT_U8                       devNum,
    IN  DFCDL_BUFFER_DRAM_PARAMS    *scanParam,
    IN  GT_BOOL                     isFirst,
    IN  GT_BOOL                     isLast,
    OUT GT_U64                      *errorCnt
);

/*******************************************************************************
* gtDfcdlScanSetConfig
*
* DESCRIPTION:
*   The function set the dfcdl configuration before and after the dfcdl scan.
*
* INPUTS:
*       devNum          - The device number to do the scaning.
*       isStart         - start / stop the configuration.
*       isExternal      - external / internal traffic.
*       isFaLoop        - enable fa loop.
*       packetType      - packet type to transmit.
*
* OUTPUTS:
*       None.
*
* RETURNS :
*       GT_OK   - on success
*       GT_FAIL - on error
*
* COMMENTS:
*
*
*******************************************************************************/
GT_STATUS gtDfcdlScanSetConfig
(
    IN  GT_U8                       devNum,
    IN  GT_BOOL                     isStart,
    IN  GT_BOOL                     isExternal,
    IN  GT_BOOL                     isFaLoop,
    IN  GT_PCK_PATTERN_TYPE         packetType
);

GT_STATUS PHY_88E1043_PATCH
(
    IN GT_U8 devNum
);

/*******************************************************************************
* gtDfcdlHelp
*
* DESCRIPTION:
*   Print help for usage DFCDL Auto tuner.
*
* INPUTS:
*       None.
*
* OUTPUTS:
*       None.
*
* RETURNS :
*       GT_OK   - on success
*       GT_FAIL - on error
*
* COMMENTS:
*
*
*******************************************************************************/
GT_STATUS   gtDfcdlHelp
(
    GT_VOID
);

/******************************************************************************/

/*
* Typedef: enum Pp2FAEnG4Loop_IN_Enable
*
* Description: Enumeration of parameter Enable of galtis
*       table / command Pp2FAEnG4Loop
*
*/
typedef enum
{
    Pp2FAEnG4Loop_IN_Enable_FALSE = 0,
    Pp2FAEnG4Loop_IN_Enable_TRUE = 1
} Pp2FAEnG4Loop_IN_Enable;


/*
* Typedef: enum Pp2FAEnG4Loop_IN_isTagged
*
* Description: Enumeration of parameter isTagged of galtis
*       table / command Pp2FAEnG4Loop
*
*/
typedef enum
{
    Pp2FAEnG4Loop_IN_isTagged_noTagging = 0,
    Pp2FAEnG4Loop_IN_isTagged_Tagging = 1
} Pp2FAEnG4Loop_IN_isTagged;


/*******************************************************************************
* gtPp2FAEnG4Loop
*
* DESCRIPTION:
*       Configure the PP & FA to perform a loopback from an input port to FA
*       and back to the input port.
*
*   INPUTS:
*       GT_U32 DevNum -
*       Dev Num
*
*       Pp2FAEnG4Loop_IN_Enable Enable -
*       No description available.
*
*       Pp2FAEnG4Loop_IN_isTagged isTagged -
*       No description available.
*
*   OUTPUTS:
*       None.
*
*
*   COMMENTS:
*       None.
*
*   RETURNS:
*       GT_OK - on success
*       G_BAD_PARAM - bad input value
*       GT_FAIL - on error
*
*******************************************************************************/
GT_STATUS gtPp2FAEnG4Loop
(
    IN   GT_U8 DevNum,
    IN   Pp2FAEnG4Loop_IN_Enable    Enable,
    IN   Pp2FAEnG4Loop_IN_isTagged  isTagged
);

/*******************************************************************************
* PHY_88E1043_C_PATCH
*
* DESCRIPTION:
*       PHY 1043, revision C, errata fix
*
* INPUTS:
*       devNum          - PP number
*
* OUTPUTS:
*       None.
*
* RETURNS:
*       GT_OK       - on success
*       GT_FAIL     - on error.
*       GT_HW_ERROR - on hardware error
*
* COMMENTS:
*
*******************************************************************************/
GT_STATUS PHY_88E1043_C_PATCH
(
    IN GT_U8 devNum
);

/*******************************************************************************
* dfcdlDiag
*
* DESCRIPTION: Asserts diagnostic search for Hang point.
*
*
* INPUTS:
*        size   : Packet size.
*        profile: Inc/Bit Tuggle.
*        txClk  : the TX value to configure.
*        dqs    : the DQS value to configure.
*
*
* OUTPUTS:
*   None
*
* RETURNS :
*   GT_TRUE - if pass criteria.
*   GT_FLASE - otherwise.
*
*
* COMMENTS:
*
*
*******************************************************************************/
GT_STATUS   dfcdlDiag
(
    IN GT_U8                   devNum,
    IN GT_DFCDL_DEVICE_TYPE    devType,
    IN GT_U32                  size,
    IN GT_MEM_TEST_PROFILE     profile,
    IN GT_U8                   txClk,
    IN GT_U8                   dqs

);

/*******************************************************************************
* madLoop
*
* DESCRIPTION:
*   Test function. Loop the Dfcdl Auto tuner.
*
* INPUTS:
*       devNum          - The device number to do the scaning.
*       devType         - The device type
*       pattType        - pattern type of traffic
*       externalSB      - GT_TRUE - external S.B., GT_FALSE - internal S.B.
*       rxHPOffsExt     - the offset taken from RxClk hang point in External
*                         S.B. mode.
*       rxHPOffsInt     - the offset taken from RxClk hang point in Internal
*                         S.B. mode.
* OUTPUTS:
*       None.
*
* RETURNS :
*
* COMMENTS:
*
*
*******************************************************************************/
GT_STATUS   madLoop
(
    IN GT_U32                  n_times,
    IN GT_U8                   devNum,
    IN GT_DFCDL_DEVICE_TYPE    devType,
    IN GT_PCK_PATTERN_TYPE     pattType,
    IN GT_BOOL                 externalSB,
    IN GT_U32                  rxHPOffsExt,
    IN GT_U32                  rxHPOffsInt
);

/*******************************************************************************
* dfcdlDebugLoop
*
* DESCRIPTION:
*   Test function. Performs a loop which reset and read the CRC counters after
*   Configuring the Dfcdl table. The input values are being set to fill the
*   DFCDL table.
*
* INPUTS:
*  GT_U8                 devNum.
*  GT_DFCDL_DEVICE_TYPE  devType    - PP of FA.
*  GT_U8                 dqs        - dqs value to set.
*  GT_U8                 rx         - rx value to set.
*  GT_U8                 tx         - tx value to set.
*  GT_U32                wait       - Wait val (msec) between reset to read
*  GT_U32                loopNum    - Iteration number.
*  GT_U32                writeAgain - Weather to write table at each iteration
*
* OUTPUTS:
*       None.
*
* RETURNS :
*       GT_OK   - on success
*       GT_FAIL - on error
*
* COMMENTS:
*
*
*******************************************************************************/
GT_STATUS dfcdlDebugLoop
(
    IN  GT_U8                   devNum,
    IN  GT_DFCDL_DEVICE_TYPE    devType,
    IN  GT_U8                   dqs,
    IN  GT_U8                   rx,
    IN  GT_U8                   tx,
    IN  GT_U32                  wait,
    IN  GT_U32                  loopNum,
    IN  GT_U32                  writeAgain
);

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* __gtDfcdlh */

