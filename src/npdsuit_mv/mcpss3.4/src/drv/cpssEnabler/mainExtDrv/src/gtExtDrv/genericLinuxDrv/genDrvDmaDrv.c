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
* genDrvDmaDrv.c
*
* DESCRIPTION:
*       Includes DMA functions wrappers implementation.
*
* DEPENDENCIES:
*       None.
*
* FILE REVISION NUMBER:
*       $Revision: 1.1.1.1 $
*
*******************************************************************************/
#ifdef GDA8548_DRV
 #include <gtExtDrv/drivers/gtCacheMng.h>
 #include <gtExtDrv/drivers/gtDmaDrv.h>
 #include <string.h>
#else
 #define KERNEL_DMA_RW
 #include <gtExtDrv/drivers/gtCacheMng.h>
 #include <gtExtDrv/drivers/gtDmaDrv.h>
 #if 0
  #include <coreDrv/gtDma.h>
 #else
  #include <string.h>
 #endif
#endif

#ifdef KERNEL_DMA_RW
#include <sys/ioctl.h>              /* ioctl        */
#include "kerneldrv/include/presteraGlob.h"
extern GT_32 gtPpFd;                /* pp file descriptor           */
#endif

/*******************************************************************************
* extDrvDmaWrite
*
* DESCRIPTION:
*       Write a given buffer to the given address using the Dma.
*
* INPUTS:
*       address     - The destination address to write to.
*       buffer      - The buffer to be written.
*       length      - Length of buffer in words.
*       burstLimit  - Number of words to be written on each burst.
*
* OUTPUTS:
*       None.
*
* RETURNS:
*       GT_OK   - on success,
*       GT_FAIL - othersise.
*
* COMMENTS:
*       1.  The given buffer is allways 4 bytes aligned, any further allignment
*           requirements should be handled internally by this function.
*       2.  The given buffer may be allocated from an uncached memory space, and
*           it's to the function to handle the cache flushing.
*       3.  The Prestera Driver assumes that the implementation of the DMA is
*           blocking, otherwise the Driver functionality might be damaged.
*
*******************************************************************************/
GT_STATUS extDrvDmaWrite
(
    IN  GT_U32  address,
    IN  GT_U32  *buffer,
    IN  GT_U32  length,
    IN  GT_U32  burstLimit
)
{
#ifdef  KERNEL_DMA_RW
    int rc;
    GT_DmaReadWrite_STC params;

    params.address = address;
    params.length = length;
    params.burstLimit = burstLimit;
    params.buffer = buffer;

    rc = ioctl(gtPpFd, PRESTERA_IOC_DMAWRITE, &params);
    return (rc == 0) ? GT_OK : GT_FAIL;
#else /* !KERNEL_DMA_RW */
#if 0
    GT_U32  sourceAddr;     /* The source address to begin the transfer from*/
    GT_U32  destAddr;       /* Destination address for the data to be       */
                            /* transfered.                                  */
    GT_U32  command;        /* Dma command parameter.                       */
    DMA_ENGINE  dmaEngine;  /* The Dma engine to perform the burst through. */
    DMA_STATUS  dmaStatus;
    GT_U32  i;

    /* Set the dma function parameters.     */
    dmaEngine   = DMA_ENG_0;
    sourceAddr  = (GT_U32)buffer;
    destAddr    = address;
    command     = (DMA_NON_CHAIN_MODE | DMA_BLOCK_TRANSFER_MODE);

    switch(burstLimit)
    {
        case(2):
            command |= (DMA_DEST_DTL_8BYTES | DMA_DTL_8BYTES);
            break;
       case(4):
            command |= (DMA_DEST_DTL_16BYTES | DMA_DTL_16BYTES);
            break;

        default:
            return GT_FAIL;
    }

    /* Flush the buffer data    */
    extDrvMgmtCacheFlush(GT_MGMT_DATA_CACHE_E,buffer,length * sizeof(GT_U32));

    for(i = 0; i < length; i += burstLimit)
    {
        /* Wait until the Dma is Idle.          */
        while(gtDmaIsChannelActive(dmaEngine) != DMA_CHANNEL_IDLE);

        dmaStatus = gtDmaTransfer(dmaEngine,sourceAddr + 4 * i,destAddr + 4 * i,
                                  burstLimit * sizeof(GT_U32),command,NULL);
        if(dmaStatus != DMA_OK)
        {
            return GT_FAIL;
        }
    }

    /* Wait until the Dma is Idle.          */
    while(gtDmaIsChannelActive(dmaEngine) != DMA_CHANNEL_IDLE);
#else
    memcpy((void *)address, (void *)buffer, length * 4);
#endif
    return GT_OK;
#endif /* KERNEL_DMA_RW */
}


/*******************************************************************************
* extDrvDmaRead
*
* DESCRIPTION:
*       Read a memory block from a given address.
*
* INPUTS:
*       address     - The address to read from.
*       length      - Length of the memory block to read (in words).
*       burstLimit  - Number of words to be read on each burst.
*
* OUTPUTS:
*       buffer  - The read data.
*
* RETURNS:
*       GT_OK   - on success,
*       GT_FAIL - othersise.
*
* COMMENTS:
*       1.  The given buffer is allways 4 bytes aligned, any further allignment
*           requirements should be handled internally by this function.
*       2.  The given buffer may be allocated from an uncached memory space, and
*           it's to the function to handle the cache flushing.
*       3.  The Prestera Driver assumes that the implementation of the DMA is
*           blocking, otherwise the Driver functionality might be damaged.
*
*******************************************************************************/
GT_STATUS extDrvDmaRead
(
    IN  GT_U32  address,
    IN  GT_U32  length,
    IN  GT_U32  burstLimit,
    OUT GT_U32  *buffer
)
{
#ifdef  KERNEL_DMA_RW
    int rc;
    GT_DmaReadWrite_STC params;

    params.address = address;
    params.length = length;
    params.burstLimit = burstLimit;
    params.buffer = buffer;

    rc = ioctl(gtPpFd, PRESTERA_IOC_DMAREAD, &params);
    return (rc == 0) ? GT_OK : GT_FAIL;
#else /* !KERNEL_DMA_RW */
#if 0
    GT_U32  sourceAddr;     /* The source address to begin the transfer from*/
    GT_U32  destAddr;       /* Destination address for the data to be       */
                            /* transfered.                                  */
    GT_U32  command;        /* Dma command parameter.                       */
    DMA_ENGINE  dmaEngine;  /* The Dma engine to perform the burst through. */
    DMA_STATUS  dmaStatus;

    /* Set the dma function parameters.     */
    dmaEngine   = DMA_ENG_1;
    sourceAddr  = address;
    destAddr    = (GT_U32)buffer;
    command     = (DMA_NON_CHAIN_MODE | DMA_BLOCK_TRANSFER_MODE);

    switch(burstLimit)
    {
        case(2):
            command |= (DMA_DEST_DTL_8BYTES | DMA_DTL_8BYTES);
            break;
        case(4):
            command |= (DMA_DEST_DTL_16BYTES | DMA_DTL_16BYTES);
            break;

        default:
            return GT_FAIL;
    }

    /*osPrintf("src = 0x%x, dst = 0x%x, length = %d.\n",sourceAddr,destAddr,
             length);*/

    /* Wait until the Dma is Idle.          */
    while(gtDmaIsChannelActive(dmaEngine) != DMA_CHANNEL_IDLE);

    extDrvMgmtCacheFlush(GT_MGMT_DATA_CACHE_E,buffer,sizeof(GT_U32) * length);
    extDrvMgmtCacheInvalidate(GT_MGMT_DATA_CACHE_E,buffer,sizeof(GT_U32) * length);

    dmaStatus = gtDmaTransfer(dmaEngine,sourceAddr,destAddr,
                              length * sizeof(GT_U32),command,NULL);
    if(dmaStatus != DMA_OK)
    {
        return GT_FAIL;
    }

    /* Wait until the Dma is Idle.          */
    while(gtDmaIsChannelActive(dmaEngine) != DMA_CHANNEL_IDLE);
#else
    memcpy((void *)buffer, (void *)address, length * 4);
#endif
    return GT_OK;
#endif /* KERNEL_DMA_RW */
}

