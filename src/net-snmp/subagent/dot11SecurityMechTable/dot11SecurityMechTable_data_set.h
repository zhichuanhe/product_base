/*
 * Note: this file originally auto-generated by mib2c using
 *       version : 1.18 $ of : mfd-data-set.m2c,v $ 
 *
 * $Id:$
 */
#ifndef DOT11SECURITYMECHTABLE_DATA_SET_H
#define DOT11SECURITYMECHTABLE_DATA_SET_H

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
 *** Table dot11SecurityMechTable
 ***
 **********************************************************************
 **********************************************************************/
/*
 * dot11SecurityMechTable is subid 1 of wtpSecurity.
 * Its status is Current.
 * OID: .1.3.6.1.4.1.31656.6.1.1.9.1, length: 12
*/
    int dot11SecurityMechTable_wtpMacAddr_check_index( dot11SecurityMechTable_rowreq_ctx *rowreq_ctx ); /* external */
    int dot11SecurityMechTable_wlanCurrID_check_index( dot11SecurityMechTable_rowreq_ctx *rowreq_ctx ); /* external */


int dot11SecurityMechTable_undo_setup( dot11SecurityMechTable_rowreq_ctx *rowreq_ctx);
int dot11SecurityMechTable_undo_cleanup( dot11SecurityMechTable_rowreq_ctx *rowreq_ctx);
int dot11SecurityMechTable_commit( dot11SecurityMechTable_rowreq_ctx *rowreq_ctx);
int dot11SecurityMechTable_undo_commit( dot11SecurityMechTable_rowreq_ctx *rowreq_ctx);


int wtpWirelessSecurMechName_check_value( dot11SecurityMechTable_rowreq_ctx *rowreq_ctx, char *wtpWirelessSecurMechName_val_ptr,  size_t wtpWirelessSecurMechName_val_ptr_len);
int wtpWirelessSecurMechName_undo_setup( dot11SecurityMechTable_rowreq_ctx *rowreq_ctx );
int wtpWirelessSecurMechName_set( dot11SecurityMechTable_rowreq_ctx *rowreq_ctx, char *wtpWirelessSecurMechName_val_ptr,  size_t wtpWirelessSecurMechName_val_ptr_len );
int wtpWirelessSecurMechName_undo( dot11SecurityMechTable_rowreq_ctx *rowreq_ctx );

int wtpWirelessSecurMechID_check_value( dot11SecurityMechTable_rowreq_ctx *rowreq_ctx, long wtpWirelessSecurMechID_val);
int wtpWirelessSecurMechID_undo_setup( dot11SecurityMechTable_rowreq_ctx *rowreq_ctx );
int wtpWirelessSecurMechID_set( dot11SecurityMechTable_rowreq_ctx *rowreq_ctx, long wtpWirelessSecurMechID_val );
int wtpWirelessSecurMechID_undo( dot11SecurityMechTable_rowreq_ctx *rowreq_ctx );

int wtpWirelessSecurMechType_check_value( dot11SecurityMechTable_rowreq_ctx *rowreq_ctx, char *wtpWirelessSecurMechType_val_ptr,  size_t wtpWirelessSecurMechType_val_ptr_len);
int wtpWirelessSecurMechType_undo_setup( dot11SecurityMechTable_rowreq_ctx *rowreq_ctx );
int wtpWirelessSecurMechType_set( dot11SecurityMechTable_rowreq_ctx *rowreq_ctx, char *wtpWirelessSecurMechType_val_ptr,  size_t wtpWirelessSecurMechType_val_ptr_len );
int wtpWirelessSecurMechType_undo( dot11SecurityMechTable_rowreq_ctx *rowreq_ctx );

int wtpWirelessSecurMechEncryType_check_value( dot11SecurityMechTable_rowreq_ctx *rowreq_ctx, u_long wtpWirelessSecurMechEncryType_val);
int wtpWirelessSecurMechEncryType_undo_setup( dot11SecurityMechTable_rowreq_ctx *rowreq_ctx );
int wtpWirelessSecurMechEncryType_set( dot11SecurityMechTable_rowreq_ctx *rowreq_ctx, u_long wtpWirelessSecurMechEncryType_val );
int wtpWirelessSecurMechEncryType_undo( dot11SecurityMechTable_rowreq_ctx *rowreq_ctx );

int wtpWirelessSecurMechSecurPolicyKey_check_value( dot11SecurityMechTable_rowreq_ctx *rowreq_ctx, char *wtpWirelessSecurMechSecurPolicyKey_val_ptr,  size_t wtpWirelessSecurMechSecurPolicyKey_val_ptr_len);
int wtpWirelessSecurMechSecurPolicyKey_undo_setup( dot11SecurityMechTable_rowreq_ctx *rowreq_ctx );
int wtpWirelessSecurMechSecurPolicyKey_set( dot11SecurityMechTable_rowreq_ctx *rowreq_ctx, char *wtpWirelessSecurMechSecurPolicyKey_val_ptr,  size_t wtpWirelessSecurMechSecurPolicyKey_val_ptr_len );
int wtpWirelessSecurMechSecurPolicyKey_undo( dot11SecurityMechTable_rowreq_ctx *rowreq_ctx );

int wtpWirelessSecurMechKeyInputType_check_value( dot11SecurityMechTable_rowreq_ctx *rowreq_ctx, char *wtpWirelessSecurMechKeyInputType_val_ptr,  size_t wtpWirelessSecurMechKeyInputType_val_ptr_len);
int wtpWirelessSecurMechKeyInputType_undo_setup( dot11SecurityMechTable_rowreq_ctx *rowreq_ctx );
int wtpWirelessSecurMechKeyInputType_set( dot11SecurityMechTable_rowreq_ctx *rowreq_ctx, char *wtpWirelessSecurMechKeyInputType_val_ptr,  size_t wtpWirelessSecurMechKeyInputType_val_ptr_len );
int wtpWirelessSecurMechKeyInputType_undo( dot11SecurityMechTable_rowreq_ctx *rowreq_ctx );


int dot11SecurityMechTable_check_dependencies(dot11SecurityMechTable_rowreq_ctx *ctx);


#ifdef __cplusplus
}
#endif

#endif /* DOT11SECURITYMECHTABLE_DATA_SET_H */
