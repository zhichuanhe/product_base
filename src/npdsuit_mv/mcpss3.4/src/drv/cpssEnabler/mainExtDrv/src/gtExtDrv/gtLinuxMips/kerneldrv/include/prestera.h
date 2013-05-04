/*******************************************************************************
Copyright (C) Marvell International Ltd. and its affiliates

This software file (the "File") is owned and distributed by Marvell 
International Ltd. and/or its affiliates ("Marvell") under the following
alternative licensing terms.  Once you have made an election to distribute the
File under one of the following license alternatives, please (i) delete this
introductory statement regarding license alternatives, (ii) delete the two
license alternatives that you have not elected to use and (iii) preserve the
Marvell copyright notice above.

********************************************************************************
Marvell Commercial License Option

If you received this File from Marvell and you have entered into a commercial
license agreement (a "Commercial License") with Marvell, the File is licensed
to you under the terms of the applicable Commercial License.

********************************************************************************
Marvell GPL License Option

If you received this File from Marvell, you may opt to use, redistribute and/or 
modify this File in accordance with the terms and conditions of the General 
Public License Version 2, June 1991 (the "GPL License"), a copy of which is 
available along with the File in the license.txt file or by writing to the Free 
Software Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA 02111-1307 or 
on the worldwide web at http://www.gnu.org/licenses/gpl.txt. 

THE FILE IS DISTRIBUTED AS-IS, WITHOUT WARRANTY OF ANY KIND, AND THE IMPLIED 
WARRANTIES OF MERCHANTABILITY OR FITNESS FOR A PARTICULAR PURPOSE ARE EXPRESSLY 
DISCLAIMED.  The GPL License provides additional details about this warranty 
disclaimer.
********************************************************************************
Marvell BSD License Option

If you received this File from Marvell, you may opt to use, redistribute and/or 
modify this File under the following licensing terms. 
Redistribution and use in source and binary forms, with or without modification, 
are permitted provided that the following conditions are met:

    *   Redistributions of source code must retain the above copyright notice,
	    this list of conditions and the following disclaimer. 

    *   Redistributions in binary form must reproduce the above copyright
        notice, this list of conditions and the following disclaimer in the
        documentation and/or other materials provided with the distribution. 

    *   Neither the name of Marvell nor the names of its contributors may be 
        used to endorse or promote products derived from this software without 
        specific prior written permission. 
    
THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND 
ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED 
WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE 
DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR CONTRIBUTORS BE LIABLE FOR 
ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES 
(INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; 
LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON 
ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT 
(INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS 
SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
********************************************************************************
* prestera.h
*
* DESCRIPTION:
*       Includes defines and structures needed by the PP device driver
*
* DEPENDENCIES:
*       None.
*
* FILE REVISION NUMBER:
*       $Revision: 1.1.1.1 $
*
*******************************************************************************/
#ifndef __PRESTERA__
#define __PRESTERA__

#include <linux/version.h>
#include "prestera_glob.h"

/* only allow 2.6.12 and higher */
#if LINUX_VERSION_CODE < KERNEL_VERSION(2,6,12)
# error "This kernel is too old: not supported by this file"
#endif

#ifndef PRESTERA_MAJOR
#define PRESTERA_MAJOR 252   /* major number */
#endif 

#undef  GT_SYNC
#define GT_SYNC

struct Prestera_Dev
{
    struct Prestera_Dev     *next;          /* pointer to next dev          */
    unsigned long           magic;          /**/
    unsigned long           flags;          /**/
    unsigned int            dev_num;        /**/
    char                    *wr_p;          /* where to write               */
    char                    *rd_p;          /* where to read                */
    unsigned long           irq;            /**/
    unsigned long           pci_address;    /**/
    struct semaphore        sem;            /* Mutual exclusion semaphore   */
    loff_t                  size;           /* prestera mem size            */
};

struct Mem_Region
{
    unsigned long base;
    unsigned long size;
};

struct Dma_Region
{
	unsigned long base;			/* DMA base address used by CPU */
	unsigned long device_base;  /* DMA base address used by device */
};

struct PP_Dev
{
    struct Mem_Region config; /* Configuration space */
    struct Mem_Region ppregs; /* PP registers space */
	struct Dma_Region ppdma;  /* PP dma region remapped */
};

#define CPSS_DEV_END  0x7FFFFFFF

#define CPSS_DEV_TYPES       \
 PRV_CPSS_DX_DEVICES_MAC,    \
 PRV_CPSS_EXMX_DEVICES_MAC,  \
 PRV_CPSS_PUMA_DEVICES_MAC,  \
 CPSS_DEV_END


#endif /* __PRESTERA__ */
