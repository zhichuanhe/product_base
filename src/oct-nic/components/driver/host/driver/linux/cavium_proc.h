/*
 *
 * OCTEON SDK
 *
 * Copyright (c) 2011 Cavium Networks. All rights reserved.
 *
 * This file, which is part of the OCTEON SDK which also includes the
 * OCTEON SDK Package from Cavium Networks, contains proprietary and
 * confidential information of Cavium Networks and in some cases its
 * suppliers. 
 *
 * Any licensed reproduction, distribution, modification, or other use of
 * this file or the confidential information or patented inventions
 * embodied in this file is subject to your license agreement with Cavium
 * Networks. Unless you and Cavium Networks have agreed otherwise in
 * writing, the applicable license terms "OCTEON SDK License Type 5" can be found 
 * under the directory: $OCTEON_ROOT/components/driver/licenses/
 *
 * All other use and disclosure is prohibited.
 *
 * Contact Cavium Networks at info@caviumnetworks.com for more information.
 *
 */

/*! \file cavium_proc.h
    \brief Host Driver: Routines to process read/write operations on
                        /proc entries exported by the Octeon driver.
*/

#ifndef __CAVIUM_PROC_H__
#define __CAVIUM_PROC_H__

/** Host: Routine to create /proc files for Octeon driver.
  * @param octeon_dev - Octeon device pointer.
  */
int
cavium_init_proc(octeon_device_t    *octeon_dev);

/** Host: Routine to delete /proc files for Octeon driver.
  * @param octeon_dev - Octeon device pointer.
  */
void
cavium_delete_proc(octeon_device_t  *octeon_dev);


#endif /*   __CAVIUM_PROC_H__  */


/* $Id: cavium_proc.h 42868 2009-05-19 23:57:52Z panicker $ */
