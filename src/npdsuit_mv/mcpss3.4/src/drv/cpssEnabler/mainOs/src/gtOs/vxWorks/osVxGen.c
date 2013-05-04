/*******************************************************************************
*              Copyright 2001, GALILEO TECHNOLOGY, LTD.
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
* osVxGen.c
*
* DESCRIPTION:
*       Operating System wrapper general APIs implementation
*
* DEPENDENCIES:
*       None.
*
* FILE REVISION NUMBER:
*       $Revision: 1.1.1.1 $
*******************************************************************************/


/************* Includes *******************************************************/
#include <gtOs/gtOsGen.h>
#include <gtUtil/gtBmPool.h>

/*************** Globals ******************************************************/
static GT_BOOL gtOsWrapperOpen = GT_FALSE;

/************* Functions ******************************************************/
GT_STATUS prvDebugServicesInit(GT_VOID);

/*******************************************************************************
* osWrapperOpen
*
* DESCRIPTION:
*       Initialize OS wrapper facility.
*
* INPUTS:
*       osSpec  - ptr to OS specific init parameter(s)
*
* OUTPUTS:
*       None
*
* RETURNS:
*       GT_OK            - on success
*       GT_FAIL          - on error
*       GT_ALREADY_EXIST - wrapper was already open
*
* COMMENTS:
*       None
*
*******************************************************************************/
GT_STATUS osWrapperOpen(void * osSpec)
{
    GT_STATUS rc;

    if (gtOsWrapperOpen == GT_TRUE)
    {
        return GT_OK;
    }
    else
    {
        /* Must be set to GT_TRUE before use any of OS functions. */
        gtOsWrapperOpen = GT_TRUE;

        gtPoolInit( 500 );

        /* init debug services */
        rc = prvDebugServicesInit();
        if (rc != GT_OK)
        {
            return rc;
        }
    }

    return GT_OK;
}

/*******************************************************************************
* osWrapperClose
*
* DESCRIPTION:
*       Close OS wrapper facility and free all used resources.
*
* INPUTS:
*       None
*
* OUTPUTS:
*       None
*
* RETURNS:
*       GT_OK    - on success
*       GT_FAIL  - on error
*
* COMMENTS:
*       None
*
*******************************************************************************/
GT_STATUS osWrapperClose(void)
{
    IS_WRAPPER_OPEN_STATUS;

    return GT_OK;
}

/*******************************************************************************
* osWrapperIsOpen
*
* DESCRIPTION:
*       Returns GT_TRUE if OS wrapper facility was initialized .
*
* INPUTS:
*       None
*
* OUTPUTS:
*       None
*
* RETURNS:
*       GT_TRUE          - on success
*       GT_FALSE         - on error
*
* COMMENTS:
*       None
*
*******************************************************************************/
GT_INLINE GT_BOOL osWrapperIsOpen(void)
{
    return gtOsWrapperOpen;
}


