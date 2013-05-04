/*
 * Note: this file originally auto-generated by mib2c using
 *       version : 1.43.2.3 $ of : mfd-interface.m2c,v $
 *
 * $Id:$
 */
/** @defgroup interface: Routines to interface to Net-SNMP
 *
 * \warning This code should not be modified, called directly,
 *          or used to interpret functionality. It is subject to
 *          change at any time.
 * 
 * @{
 */
/*
 * *********************************************************************
 * *********************************************************************
 * *********************************************************************
 * ***                                                               ***
 * ***  NOTE NOTE NOTE NOTE NOTE NOTE NOTE NOTE NOTE NOTE NOTE NOTE  ***
 * ***                                                               ***
 * ***                                                               ***
 * ***       THIS FILE DOES NOT CONTAIN ANY USER EDITABLE CODE.      ***
 * ***                                                               ***
 * ***                                                               ***
 * ***       THE GENERATED CODE IS INTERNAL IMPLEMENTATION, AND      ***
 * ***                                                               ***
 * ***                                                               ***
 * ***    IS SUBJECT TO CHANGE WITHOUT WARNING IN FUTURE RELEASES.   ***
 * ***                                                               ***
 * ***                                                               ***
 * *********************************************************************
 * *********************************************************************
 * *********************************************************************
 */
#ifndef DOT11RADIOSTATSTABLE_INTERFACE_H
#define DOT11RADIOSTATSTABLE_INTERFACE_H

#ifdef __cplusplus
extern "C" {
#endif


#include "dot11RadioStatsTable.h"

/* ********************************************************************
 * Table declarations
 */

/* PUBLIC interface initialization routine */
void _dot11RadioStatsTable_initialize_interface(dot11RadioStatsTable_registration_ptr user_ctx,
                                    u_long flags);

    dot11RadioStatsTable_rowreq_ctx * dot11RadioStatsTable_allocate_rowreq_ctx(void);
void dot11RadioStatsTable_release_rowreq_ctx(dot11RadioStatsTable_rowreq_ctx *rowreq_ctx);

int dot11RadioStatsTable_index_to_oid(netsnmp_index *oid_idx,
                            dot11RadioStatsTable_mib_index *mib_idx);
int dot11RadioStatsTable_index_from_oid(netsnmp_index *oid_idx,
                              dot11RadioStatsTable_mib_index *mib_idx);

/*
 * access to certain internals. use with caution!
 */
void dot11RadioStatsTable_valid_columns_set(netsnmp_column_info *vc);


#ifdef __cplusplus
}
#endif

#endif /* DOT11RADIOSTATSTABLE_INTERFACE_H */
