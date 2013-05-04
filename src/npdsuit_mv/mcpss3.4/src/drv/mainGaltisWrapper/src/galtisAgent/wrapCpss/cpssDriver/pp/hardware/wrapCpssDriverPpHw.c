/*******************************************************************************
*              (c), Copyright 2001, Marvell International Ltd.                 *
* THIS CODE CONTAINS CONFIDENTIAL INFORMATION OF MARVELL SEMICONDUCTOR, INC.   *
* NO RIGHTS ARE GRANTED HEREIN UNDER ANY PATENT, MASK WORK RIGHT OR COPYRIGHT  *
* OF MARVELL OR ANY THIRD PARTY. MARVELL RESERVES THE RIGHT AT ITS SOLE        *
* DISCRETION TO REQUEST THAT THIS CODE BE IMMEDIATELY RETURNED TO MARVELL.     *
* THIS CODE IS PROVIDED "AS IS". MARVELL MAKES NO WARRANTIES, EXPRESSED,       *
* IMPLIED OR OTHERWISE, REGARDING ITS ACCURACY, COMPLETENESS OR PERFORMANCE.   *
********************************************************************************
* wrapCpssDriverPpHw.c
*
* DESCRIPTION:
*       Wrapper functions for Cpss Hardware
*
* FILE REVISION NUMBER:
*       $Revision: 1.1.1.1 $
*
*******************************************************************************/

/* Common galtis includes */
#include <cmdShell/cmdDb/cmdBase.h>
#include <cmdShell/common/cmdWrapUtils.h>


/* Feature specific includes. */
#include <cpss/generic/cpssTypes.h>
#include <cpssDriver/pp/hardware/cpssDriverPpHw.h>
#include <cpss/extServices/private/prvCpssBindFunc.h>
#include <cpssDriver/pp/config/generic/cpssDrvPpGenDump.h>

#define PRV_CPSS_PORT_GROUP_UNAWARE_MODE_CNS 0xFFFF

extern GT_STATUS prvCpssDrvHwPpMemoryAccessTraceEnableSet
(
    IN GT_BOOL enable
);

/*******************************************************************************
* cpssDrvPpHwRegBitMaskRead
*
* DESCRIPTION:
*       Reads the unmasked bits of a register.
*
* INPUTS:
*       devNum  - PP device number to read from.
*       regAddr - Register address to read from.
*       mask    - Mask for selecting the read bits.
*
* OUTPUTS:
*       dataPtr    - Data read from register.
*
* RETURNS:
*       GT_OK   - on success
*       GT_FAIL - on hardware error
*       GT_NOT_INITIALIZED - if the driver was not initialized
*       GT_HW_ERROR - on hardware error
*
* COMMENTS:
*       The bits in value to be read are the masked bit of 'mask'.
*
*******************************************************************************/
static CMD_STATUS wrCpssDrvPpHwRegBitMaskRead
(
    IN  GT_32 inArgs[CMD_MAX_ARGS],
    IN  GT_32 inFields[CMD_MAX_FIELDS],
    IN  GT_32 numFields,
    OUT GT_8  outArgs[CMD_MAX_BUFFER]
)
{

    GT_STATUS   status;

    GT_U8    devNum;
    GT_U32   regAddr;
    GT_U32   mask;
    GT_U32   data;

    /* check for valid arguments */
    if(!inArgs || !outArgs)
        return CMD_AGENT_ERROR;

    devNum = (GT_U8)inArgs[0];
    regAddr = (GT_U32)inArgs[1];
    mask = (GT_U32)inArgs[2];

    if (mask == 0)
    {
        mask = 0xFFFFFFFF;
    }

    /* call cpss api function */
    status =  cpssDrvPpHwRegBitMaskRead(devNum, CPSS_PORT_GROUP_UNAWARE_MODE_CNS,regAddr, mask, &data);

    /* pack output arguments to galtis string */
    galtisOutput(outArgs, status, "%d", data);

    return CMD_OK;
}

/*******************************************************************************
* cpssDrvPpHwRegBitMaskWrite
*
* DESCRIPTION:
*       Writes the unmasked bits of a register.
*
* INPUTS:
*       devNum  - PP device number to write to.
*       regAddr - Register address to write to.
*       mask    - Mask for selecting the written bits.
*       value   - Data to be written to register.
*
* OUTPUTS:
*       None.
*
* RETURNS:
*       GT_OK   - on success
*       GT_FAIL - on hardware error
*       GT_NOT_INITIALIZED - if the driver was not initialized
*       GT_HW_ERROR - on hardware error
*
* COMMENTS:
*       The bits in value to be written are the masked bit of 'mask'.
*
*******************************************************************************/
static CMD_STATUS wrCpssDrvPpHwRegBitMaskWrite
(
    IN  GT_32 inArgs[CMD_MAX_ARGS],
    IN  GT_32 inFields[CMD_MAX_FIELDS],
    IN  GT_32 numFields,
    OUT GT_8  outArgs[CMD_MAX_BUFFER]
)
{

    GT_STATUS   status;

    GT_U8    devNum;
    GT_U32   regAddr;
    GT_U32   mask;
    GT_U32   data;
    /* check for valid arguments */
    if(!inArgs || !outArgs)
        return CMD_AGENT_ERROR;

    devNum = (GT_U8)inArgs[0];
    regAddr = (GT_U32)inArgs[1];
    mask = (GT_U32)inArgs[2];
    data = (GT_U32)inArgs[3];

    if (mask == 0)
    {
        mask = 0xFFFFFFFF;
    }

    /* call cpss api function */
    status =  cpssDrvPpHwRegBitMaskWrite(devNum, CPSS_PORT_GROUP_UNAWARE_MODE_CNS,regAddr, mask, data);

    /* pack output arguments to galtis string */
    galtisOutput(outArgs, status, "");

    return CMD_OK;
}

/*******************************************************************************
* wrCpssDrvPpHwPortGroupRegBitMaskRead
*
* DESCRIPTION:
*       Reads the unmasked bits of a register.
*
* INPUTS:
*       devNum  - PP device number to read from.
*       portGroupId  - The port group Id. relevant only to 'multi-port-groups' devices.
*                 supports value CPSS_PORT_GROUP_UNAWARE_MODE_CNS
*       regAddr - Register address to read from.
*       mask    - Mask for selecting the read bits.
*
* OUTPUTS:
*       dataPtr    - Data read from register.
*
* RETURNS:
*       GT_OK   - on success
*       GT_FAIL - on hardware error
*       GT_NOT_INITIALIZED - if the driver was not initialized
*       GT_HW_ERROR - on hardware error
*
* COMMENTS:
*       The bits in value to be read are the masked bit of 'mask'.
*
*******************************************************************************/
static CMD_STATUS wrCpssDrvPpHwPortGroupRegBitMaskRead
(
    IN  GT_32 inArgs[CMD_MAX_ARGS],
    IN  GT_32 inFields[CMD_MAX_FIELDS],
    IN  GT_32 numFields,
    OUT GT_8  outArgs[CMD_MAX_BUFFER]
)
{

    GT_STATUS   status;

    GT_U8    devNum;
    GT_U32   portGroupId;
    GT_U32   regAddr;
    GT_U32   mask;
    GT_U32   data;

    /* check for valid arguments */
    if(!inArgs || !outArgs)
        return CMD_AGENT_ERROR;

    devNum = (GT_U8)inArgs[0];
    portGroupId = (GT_U32)inArgs[1];
    regAddr = (GT_U32)inArgs[2];
    mask = (GT_U32)inArgs[3];

    if (mask == 0)
    {
        mask = 0xFFFFFFFF;
    }

    /* call cpss api function */
    status =  cpssDrvPpHwRegBitMaskRead(
        devNum, portGroupId,regAddr, mask, &data);

    /* pack output arguments to galtis string */
    galtisOutput(outArgs, status, "%d", data);

    return CMD_OK;
}

/*******************************************************************************
* wrCpssDrvPpHwPortGroupRegBitMaskWrite
*
* DESCRIPTION:
*       Writes the unmasked bits of a register.
*
* INPUTS:
*       devNum  - PP device number to write to.
*       portGroupId  - The port group Id. relevant only to 'multi-port-groups' devices.
*                 supports value CPSS_PORT_GROUP_UNAWARE_MODE_CNS
*       regAddr - Register address to write to.
*       mask    - Mask for selecting the written bits.
*       value   - Data to be written to register.
*
* OUTPUTS:
*       None.
*
* RETURNS:
*       GT_OK   - on success
*       GT_FAIL - on hardware error
*       GT_NOT_INITIALIZED - if the driver was not initialized
*       GT_HW_ERROR - on hardware error
*
* COMMENTS:
*       The bits in value to be written are the masked bit of 'mask'.
*
*******************************************************************************/
static CMD_STATUS wrCpssDrvPpHwPortGroupRegBitMaskWrite
(
    IN  GT_32 inArgs[CMD_MAX_ARGS],
    IN  GT_32 inFields[CMD_MAX_FIELDS],
    IN  GT_32 numFields,
    OUT GT_8  outArgs[CMD_MAX_BUFFER]
)
{

    GT_STATUS   status;

    GT_U8    devNum;
    GT_U32   portGroupId;
    GT_U32   regAddr;
    GT_U32   mask;
    GT_U32   data;
    /* check for valid arguments */
    if(!inArgs || !outArgs)
        return CMD_AGENT_ERROR;

    devNum = (GT_U8)inArgs[0];
    portGroupId =  (GT_U32)inArgs[1];
    regAddr = (GT_U32)inArgs[2];
    mask = (GT_U32)inArgs[3];
    data = (GT_U32)inArgs[4];

    if (mask == 0)
    {
        mask = 0xFFFFFFFF;
    }

    /* call cpss api function */
    status =  cpssDrvPpHwRegBitMaskWrite(devNum, portGroupId,regAddr, mask, data);

    /* pack output arguments to galtis string */
    galtisOutput(outArgs, status, "");

    return CMD_OK;
}

/* Dump functions */
GT_STATUS cpssDrvDumPciReg
(
    IN GT_U8   devNum,
    IN GT_U32  regAddr
)
{
    GT_U32 regData;
    GT_STATUS rc;

    rc = cpssDrvPpHwInternalPciRegRead(devNum,CPSS_PORT_GROUP_UNAWARE_MODE_CNS,regAddr,&regData);


    cmdOsPrintf("Device %d PCI/PEX register address 0x%08X value = 0x%08X\n",
             devNum,regAddr,regData);

    return rc;
}

/*******************************************************************************
* cpssPpDumpRegisters
*
* DESCRIPTION:
*       This routine print dump of PP registers.
*
* INPUTS:
*       devNum - PP's device number .
*
* OUTPUTS:
*       None
*
* RETURNS:
*       GT_OK           - on success  (all Registers dumped successfully)
*       GT_FAIL         - on failure. (at least one register read error)
*       GT_BAD_PARAM    - invalid device number
*
* COMMENTS:
*
*******************************************************************************/
static CMD_STATUS wrCpssPpDumpRegisters
(
    IN  GT_32 inArgs[CMD_MAX_ARGS],
    IN  GT_32 inFields[CMD_MAX_FIELDS],
    IN  GT_32 numFields,
    OUT GT_8  outArgs[CMD_MAX_BUFFER]
)
{
    GT_STATUS   status;

    IN GT_U8    devNum;


    /* check for valid arguments */
    if(!inArgs || !outArgs)
        return CMD_AGENT_ERROR;

    devNum = (GT_U8)inArgs[0];

    /* call cpss api function */
    status =  cpssPpDumpRegisters(devNum);

    /* pack output arguments to galtis string */
    galtisOutput(outArgs, status, "");

    return CMD_OK;

}

/*******************************************************************************
* cpssPpDumpMemory
*
* DESCRIPTION:
*       This routine print dump of PP memory.
*
* INPUTS:
*       devNum      - PP's device number .
*       startAddr   - dump start address (will be aligned to 16 bytes)
*       dumpType    - dump line format (16 bytes / 8 shorts / or 4 words)
*       dumpLength  - dump length in bytes
*
* OUTPUTS:
*       None
*
* RETURNS:
*       GT_OK           - on success  (memory dumped successfully)
*       GT_FAIL         - on failure. (memory read error)
*       GT_BAD_PARAM    - invalid device number
*
* COMMENTS:
*       4 lower bits of startAddr will be zeroed (ignored) for alignment
*
*******************************************************************************/
static CMD_STATUS wrCpssPpDumpMemory
(
    IN  GT_32 inArgs[CMD_MAX_ARGS],
    IN  GT_32 inFields[CMD_MAX_FIELDS],
    IN  GT_32 numFields,
    OUT GT_8  outArgs[CMD_MAX_BUFFER]
)
{
    GT_STATUS   status;

    IN GT_U8                        devNum;
    IN GT_U32                       startAddr;
    IN CPSS_MEMORY_DUMP_TYPE_ENT    dumpType;
    IN GT_U32                       dumpLength;

    /* check for valid arguments */
    if(!inArgs || !outArgs)
        return CMD_AGENT_ERROR;

    devNum     = (GT_U8)inArgs[0];
    startAddr  = (GT_U32)inArgs[1];
    dumpType   = (CPSS_MEMORY_DUMP_TYPE_ENT)inArgs[2];
    dumpLength = (GT_U32)inArgs[3];

    /* call cpss api function */
    status =  cpssPpDumpMemory(devNum,startAddr,dumpType,dumpLength);

    /* pack output arguments to galtis string */
    galtisOutput(outArgs, status, "");

    return CMD_OK;

}

/*******************************************************************************
* wrCpssHwPortGroupSetMemDumpTblEntry (table command)
*
* DESCRIPTION:
*     set data to memory
*
* INPUTS:
*     [0] GT_U8 lPpId -
*          PP number
*
*     [1] GT_U32 portGroupId -
*          portGroupId
*
*     [2] GT_U32 lStartAddr -
*          the start address of the data block
*
*     [3] GT_U32 lLength -
*          number of words (4 bytes) to read from memory
*
*
* OUTPUTS:
*     none
*
* RETURNS:
*     CMD_OK            - on success.
*     CMD_AGENT_ERROR   - on failure.
*     CMD_FIELD_UNDERFLOW - not enough field arguments.
*     CMD_FIELD_OVERFLOW  - too many field arguments.
*
* COMMENTS:
*     none
*
*******************************************************************************/
static CMD_STATUS wrCpssHwPortGroupSetMemDumpTblEntry
(
    IN  GT_32 inArgs[CMD_MAX_ARGS],
    IN  GT_32 inFields[CMD_MAX_FIELDS],
    IN  GT_32 numFields,
    OUT GT_8  outArgs[CMD_MAX_BUFFER]
)
{
    GT_U8 devNum;
    GT_U32 portGroupId;
    GT_U32 regAddr;
    GT_STATUS status;
    GT_U32 data[4];

    /* check for valid arguments */
    if(!inArgs || !outArgs)
        return CMD_AGENT_ERROR;

    if(numFields < 5)
        return CMD_FIELD_UNDERFLOW;
    if(numFields > 5)
        return CMD_FIELD_OVERFLOW;

    devNum  = (GT_U8)inArgs[0];
    portGroupId = inArgs[1];
    regAddr = (GT_U32)inFields[0];

    data[0] = (GT_U32)inFields[1];
    data[1] = (GT_U32)inFields[2];
    data[2] = (GT_U32)inFields[3];
    data[3] = (GT_U32)inFields[4];

    status = cpssDrvPpHwRamWrite(devNum,portGroupId,regAddr,4,data);
    galtisOutput(outArgs, status, "");

    return CMD_OK;
}

/********************************************************************/

static GT_U32 currentPortGroupAddress;
static GT_U32 endPortGroupAddress;

/*******************************************************************************
* wrCpssHwPortGroupGetMemDumpTblNextEntry (table command)
*
* DESCRIPTION:
*     Get next data from memory
*
* INPUTS:
*     [0] GT_U8 lPpId -
*          PP number
*
*     [1] GT_U32 portGroupId -
*          portGroupId
*
*     [2] GT_U32 lStartAddr -
*          the start address of the data block
*
*     [3] GT_U32 lLength -
*          number of words (4 bytes) to read from memory
*
*
* OUTPUTS:
*      Table fields as describe in the table description are written into
*      outArgs parameter.
*
* RETURNS:
*     CMD_OK            - on success.
*     CMD_AGENT_ERROR   - on failure.
*     CMD_FIELD_UNDERFLOW - not enough field arguments.
*     CMD_FIELD_OVERFLOW  - too many field arguments.
*
* COMMENTS:
*     none
*
*******************************************************************************/
static CMD_STATUS wrCpssHwPortGroupGetMemDumpTblNextEntry
(
    IN  GT_32 inArgs[CMD_MAX_ARGS],
    IN  GT_32 inFields[CMD_MAX_FIELDS],
    IN  GT_32 numFields,
    OUT GT_8  outArgs[CMD_MAX_BUFFER]
)
{
    GT_STATUS status;
    GT_U8 devNum;
    GT_U32 portGroupId;
    GT_U32 data[4];

    /* check for valid arguments */
    if(!inArgs || !outArgs)
        return CMD_AGENT_ERROR;

    if (currentPortGroupAddress > endPortGroupAddress)
    {
        galtisOutput(outArgs, GT_OK, "%d", -1);
        return CMD_OK;
    }

    devNum = (GT_U8)inArgs[0];
    portGroupId = (inArgs[1] == PRV_CPSS_PORT_GROUP_UNAWARE_MODE_CNS)?
        CPSS_PORT_GROUP_UNAWARE_MODE_CNS:(GT_U32)inArgs[1];


    status = cpssDrvPpHwRamRead(devNum,portGroupId,currentPortGroupAddress,4,data);
    if(status != GT_OK)
    {
        galtisOutput(outArgs, status, "");
        return CMD_OK;
    }

    inFields[0] = currentPortGroupAddress;
    inFields[1] = data[0];
    inFields[2] = data[1];
    inFields[3] = data[2];
    inFields[4] = data[3];
    currentPortGroupAddress += 16;

    /* pack and output table fields */
    fieldOutput("%d%d%d%d%d", inFields[0], inFields[1], inFields[2],
                inFields[3], inFields[4]);
    galtisOutput(outArgs, status, "%f");

    return CMD_OK;
}

/*******************************************************************************
* wrCpssHwPortGroupGetMemDumpTblFirstEntry (table command)
*
* DESCRIPTION:
*     Get first data from memory
*
* INPUTS:
*     [0] GT_U8 lPpId -
*          PP number
*
*     [1] GT_U32 portGroupId -
*          portGroupId
*
*     [2] GT_U32 lStartAddr -
*          the start address of the data block
*
*     [3] GT_U32 lLength -
*          number of words (4 bytes) to read from memory
*
*
* OUTPUTS:
*      Table fields as describe in the table description are written into
*      outArgs parameter.
*
* RETURNS:
*     CMD_OK            - on success.
*     CMD_AGENT_ERROR   - on failure.
*     CMD_FIELD_UNDERFLOW - not enough field arguments.
*     CMD_FIELD_OVERFLOW  - too many field arguments.
*
* COMMENTS:
*     none
*
*******************************************************************************/
static CMD_STATUS wrCpssHwPortGroupGetMemDumpTblFirstEntry
(
    IN  GT_32 inArgs[CMD_MAX_ARGS],
    IN  GT_32 inFields[CMD_MAX_FIELDS],
    IN  GT_32 numFields,
    OUT GT_8  outArgs[CMD_MAX_BUFFER]
)
{
    GT_U32 numOfWords;

    /* check for valid arguments */
    if(!inArgs || !outArgs)
        return CMD_AGENT_ERROR;

    currentPortGroupAddress = (GT_U32)inArgs[2];
    numOfWords         = (GT_U32)inArgs[3];
    endPortGroupAddress     = currentPortGroupAddress+4*numOfWords-4;

    return wrCpssHwPortGroupGetMemDumpTblNextEntry(
        inArgs,inFields,numFields,outArgs);
}

/*******************************************************************************
* hwSetMemDumpTblEntry (table command)
*
* DESCRIPTION:
*     set data to memory
*
* INPUTS:
*     [0] GT_U8 lPpId -
*          PP number
*
*     [1] GT_U32 lStartAddr -
*          the start address of the data block
*
*     [2] GT_U32 lLength -
*          number of words (4 bytes) to read from memory
*
*
* OUTPUTS:
*     none
*
* RETURNS:
*     CMD_OK            - on success.
*     CMD_AGENT_ERROR   - on failure.
*     CMD_FIELD_UNDERFLOW - not enough field arguments.
*     CMD_FIELD_OVERFLOW  - too many field arguments.
*
* COMMENTS:
*     none
*
*******************************************************************************/
static CMD_STATUS wrCpssHwSetMemDumpTblEntry
(
    IN  GT_32 inArgs[CMD_MAX_ARGS],
    IN  GT_32 inFields[CMD_MAX_FIELDS],
    IN  GT_32 numFields,
    OUT GT_8  outArgs[CMD_MAX_BUFFER]
)
{
    /* check for valid arguments */
    if(!inArgs || !outArgs)
        return CMD_AGENT_ERROR;

    inArgs[3] = inArgs[2];
    inArgs[2] = inArgs[1];
    inArgs[1] = PRV_CPSS_PORT_GROUP_UNAWARE_MODE_CNS;

    return wrCpssHwPortGroupSetMemDumpTblEntry(
        inArgs,inFields,numFields,outArgs);
}

/*******************************************************************************
* hwGetMemDumpTblFirstEntry (table command)
*
* DESCRIPTION:
*     Get first data from memory
*
* INPUTS:
*     [0] GT_U8 lPpId -
*          PP number
*
*     [1] GT_U32 lStartAddr -
*          the start address of the data block
*
*     [2] GT_U32 lLength -
*          number of words (4 bytes) to read from memory
*
*
* OUTPUTS:
*      Table fields as describe in the table description are written into
*      outArgs parameter.
*
* RETURNS:
*     CMD_OK            - on success.
*     CMD_AGENT_ERROR   - on failure.
*     CMD_FIELD_UNDERFLOW - not enough field arguments.
*     CMD_FIELD_OVERFLOW  - too many field arguments.
*
* COMMENTS:
*     none
*
*******************************************************************************/
static CMD_STATUS wrCpssHwGetMemDumpTblFirstEntry
(
    IN  GT_32 inArgs[CMD_MAX_ARGS],
    IN  GT_32 inFields[CMD_MAX_FIELDS],
    IN  GT_32 numFields,
    OUT GT_8  outArgs[CMD_MAX_BUFFER]
)
{
    /* check for valid arguments */
    if(!inArgs || !outArgs)
        return CMD_AGENT_ERROR;
    inArgs[3] = inArgs[2];
    inArgs[2] = inArgs[1];
    inArgs[1] = PRV_CPSS_PORT_GROUP_UNAWARE_MODE_CNS;

    return wrCpssHwPortGroupGetMemDumpTblFirstEntry(inArgs,inFields,numFields,outArgs);

}
/*******************************************************************************
* hwGetMemDumpTblNextEntry (table command)
*
* DESCRIPTION:
*     Get next data from memory
*
* INPUTS:
*     [0] GT_U8 lPpId -
*          PP number
*
*     [1] GT_U32 lStartAddr -
*          the start address of the data block
*
*     [2] GT_U32 lLength -
*          number of words (4 bytes) to read from memory
*
*
* OUTPUTS:
*      Table fields as describe in the table description are written into
*      outArgs parameter.
*
* RETURNS:
*     CMD_OK            - on success.
*     CMD_AGENT_ERROR   - on failure.
*     CMD_FIELD_UNDERFLOW - not enough field arguments.
*     CMD_FIELD_OVERFLOW  - too many field arguments.
*
* COMMENTS:
*     none
*
*******************************************************************************/
static CMD_STATUS wrCpssHwGetMemDumpTblNextEntry
(
    IN  GT_32 inArgs[CMD_MAX_ARGS],
    IN  GT_32 inFields[CMD_MAX_FIELDS],
    IN  GT_32 numFields,
    OUT GT_8  outArgs[CMD_MAX_BUFFER]
)
{
    /* check for valid arguments */
    if(!inArgs || !outArgs)
        return CMD_AGENT_ERROR;

    inArgs[3] = inArgs[2];
    inArgs[2] = inArgs[1];
    inArgs[1] = PRV_CPSS_PORT_GROUP_UNAWARE_MODE_CNS;

    return wrCpssHwPortGroupGetMemDumpTblNextEntry(
        inArgs,inFields,numFields,outArgs);
}

/*******************************************************************************
* wrCpssDrvHwPpMemoryAccessTraceOnOff (command)
*
* DESCRIPTION:
*     On/Off memory access trace
*
* INPUTS:
*      isMemTraceOn         - GT_FALSE - memory trace is OFF
*                           - GT_TRUE - memory trace is ON
* OUTPUTS:
*      None
*
* RETURNS:
*     CMD_OK               - on success.
*
* COMMENTS:
*     None
*
*******************************************************************************/
static CMD_STATUS wrCpssDrvHwPpMemoryAccessTraceOnOff
(
    IN  GT_32 inArgs[CMD_MAX_ARGS],
    IN  GT_32 inFields[CMD_MAX_FIELDS],
    IN  GT_32 numFields,
    OUT GT_8  outArgs[CMD_MAX_BUFFER]
)
{
    GT_STATUS   status;
    GT_U8 enable;
    /* check for valid arguments */
    if(!inArgs || !outArgs)
        return CMD_AGENT_ERROR;
    if ((GT_U8)inArgs[0] == 0)
    {
        enable = GT_FALSE;
    }
    else
    {
        enable = GT_TRUE;
    }
    /* call cpss api function */
    status =  prvCpssDrvHwPpMemoryAccessTraceEnableSet(enable);

    /* pack output arguments to galtis string */
    galtisOutput(outArgs, status, "");

    return CMD_OK;
}



/**** database initialization **************************************/

static CMD_COMMAND dbCommands[] =
{
    {"cpssDrvPpHwRegBitMaskRead",
        &wrCpssDrvPpHwRegBitMaskRead,
        3, 0},

    {"cpssDrvPpHwRegBitMaskWrite",
        &wrCpssDrvPpHwRegBitMaskWrite,
        4, 0},

    {"cpssDrvPpHwPortGroupRegBitMaskRead",
        &wrCpssDrvPpHwPortGroupRegBitMaskRead,
        4, 0},

    {"cpssDrvPpHwPortGroupRegBitMaskWrite",
        &wrCpssDrvPpHwPortGroupRegBitMaskWrite,
        5, 0},

    {"cpssPpDumpRegisters ",
        &wrCpssPpDumpRegisters,
        4, 0},

    {"cpssPpDumpMemory",
        &wrCpssPpDumpMemory,
        1, 0},

    {"cpssPpPortGroupRamDumpMemorySet",
        &wrCpssHwPortGroupSetMemDumpTblEntry,
        2, 5},
    {"cpssPpPortGroupRamDumpMemoryGetFirst",
        &wrCpssHwPortGroupGetMemDumpTblFirstEntry,
        4, 0},
    {"cpssPpPortGroupRamDumpMemoryGetNext",
        &wrCpssHwPortGroupGetMemDumpTblNextEntry,
        4, 0},

    {"cpssPpRamDumpMemorySet",
        &wrCpssHwSetMemDumpTblEntry,
        1, 5},
    {"cpssPpRamDumpMemoryGetFirst",
        &wrCpssHwGetMemDumpTblFirstEntry,
        3, 0},
    {"cpssPpRamDumpMemoryGetNext",
        &wrCpssHwGetMemDumpTblNextEntry,
        3, 0},
    {"cpssDrvHwPpMemoryAccessTraceOnOff",
        &wrCpssDrvHwPpMemoryAccessTraceOnOff,
        1, 0},
};

#define numCommands (sizeof(dbCommands) / sizeof(CMD_COMMAND))

/*******************************************************************************
* cmdLibInitCpssDriverPpHw
*
* DESCRIPTION:
*     Library database initialization function.
*
* INPUTS:
*     none
*
* OUTPUTS:
*     none
*
* RETURNS:
*     GT_OK   - on success.
*     GT_FAIL - on failure.
*
* COMMENTS:
*     none
*
*******************************************************************************/
GT_STATUS cmdLibInitCpssDriverPpHw
(
    GT_VOID
)
{
    return cmdInitLibrary(dbCommands, numCommands);
}



