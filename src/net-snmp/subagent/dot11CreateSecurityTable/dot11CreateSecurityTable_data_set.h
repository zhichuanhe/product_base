/*
 * Note: this file originally auto-generated by mib2c using
 *       version : 12077 $ of $ 
 *
 * $Id:$
 */
#ifndef DOT11CREATESECURITYTABLE_DATA_SET_H
#define DOT11CREATESECURITYTABLE_DATA_SET_H

#ifdef __cplusplus
extern "C" {
#endif

/* *********************************************************************
 * SET function declarations
 */

/* *********************************************************************
 * SET Table declarations
 */
/**********************************************************************
 **********************************************************************
 ***
 *** Table dot11CreateSecurityTable
 ***
 **********************************************************************
 **********************************************************************/
/*
 * DOT11-AC-MIB::dot11CreateSecurityTable is subid 1 of dot11securityConfig.
 * Its status is Current.
 * OID: .1.3.6.1.4.1.31656.6.1.2.14.1, length: 12
*/


int dot11CreateSecurityTable_undo_setup( dot11CreateSecurityTable_rowreq_ctx *rowreq_ctx);
int dot11CreateSecurityTable_undo_cleanup( dot11CreateSecurityTable_rowreq_ctx *rowreq_ctx);
int dot11CreateSecurityTable_undo( dot11CreateSecurityTable_rowreq_ctx *rowreq_ctx);
int dot11CreateSecurityTable_commit( dot11CreateSecurityTable_rowreq_ctx *rowreq_ctx);
int dot11CreateSecurityTable_undo_commit( dot11CreateSecurityTable_rowreq_ctx *rowreq_ctx);


int securityName_check_value( dot11CreateSecurityTable_rowreq_ctx *rowreq_ctx, char *securityName_val_ptr,  size_t securityName_val_ptr_len);
int securityName_undo_setup( dot11CreateSecurityTable_rowreq_ctx *rowreq_ctx );
int securityName_set( dot11CreateSecurityTable_rowreq_ctx *rowreq_ctx, char *securityName_val_ptr,  size_t securityName_val_ptr_len );
int securityName_undo( dot11CreateSecurityTable_rowreq_ctx *rowreq_ctx );

int securityRowStatus_check_value( dot11CreateSecurityTable_rowreq_ctx *rowreq_ctx, u_long securityRowStatus_val);
int securityRowStatus_undo_setup( dot11CreateSecurityTable_rowreq_ctx *rowreq_ctx );
int securityRowStatus_set( dot11CreateSecurityTable_rowreq_ctx *rowreq_ctx, u_long securityRowStatus_val );
int securityRowStatus_undo( dot11CreateSecurityTable_rowreq_ctx *rowreq_ctx );


int dot11CreateSecurityTable_check_dependencies(dot11CreateSecurityTable_rowreq_ctx *ctx);


#ifdef __cplusplus
}
#endif

#endif /* DOT11CREATESECURITYTABLE_DATA_SET_H */
