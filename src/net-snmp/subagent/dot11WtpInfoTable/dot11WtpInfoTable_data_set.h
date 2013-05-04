/*
 * Note: this file originally auto-generated by mib2c using
 *       version : 1.18 $ of : mfd-data-set.m2c,v $ 
 *
 * $Id:$
 */
#ifndef DOT11WTPINFOTABLE_DATA_SET_H
#define DOT11WTPINFOTABLE_DATA_SET_H

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
 *** Table dot11WtpInfoTable
 ***
 **********************************************************************
 **********************************************************************/
/*
 * dot11WtpInfoTable is subid 1 of wtpGeneralInfo.
 * Its status is Current.
 * OID: .1.3.6.1.4.1.31656.6.1.1.1.1, length: 12
*/
int wtpMacAddr_check_index( dot11WtpInfoTable_rowreq_ctx *rowreq_ctx ); /* internal */


int dot11WtpInfoTable_undo_setup( dot11WtpInfoTable_rowreq_ctx *rowreq_ctx);
int dot11WtpInfoTable_undo_cleanup( dot11WtpInfoTable_rowreq_ctx *rowreq_ctx);
int dot11WtpInfoTable_commit( dot11WtpInfoTable_rowreq_ctx *rowreq_ctx);
int dot11WtpInfoTable_undo_commit( dot11WtpInfoTable_rowreq_ctx *rowreq_ctx);


int wtpDevName_check_value( dot11WtpInfoTable_rowreq_ctx *rowreq_ctx, char *wtpDevName_val_ptr,  size_t wtpDevName_val_ptr_len);
int wtpDevName_undo_setup( dot11WtpInfoTable_rowreq_ctx *rowreq_ctx );
int wtpDevName_set( dot11WtpInfoTable_rowreq_ctx *rowreq_ctx, char *wtpDevName_val_ptr,  size_t wtpDevName_val_ptr_len );
int wtpDevName_undo( dot11WtpInfoTable_rowreq_ctx *rowreq_ctx );

int wtpPosInfo_check_value( dot11WtpInfoTable_rowreq_ctx *rowreq_ctx, char *wtpPosInfo_val_ptr,  size_t wtpPosInfo_val_ptr_len);
int wtpPosInfo_undo_setup( dot11WtpInfoTable_rowreq_ctx *rowreq_ctx );
int wtpPosInfo_set( dot11WtpInfoTable_rowreq_ctx *rowreq_ctx, char *wtpPosInfo_val_ptr,  size_t wtpPosInfo_val_ptr_len );
int wtpPosInfo_undo( dot11WtpInfoTable_rowreq_ctx *rowreq_ctx );

int wtpProduct_check_value( dot11WtpInfoTable_rowreq_ctx *rowreq_ctx, char *wtpProduct_val_ptr,  size_t wtpProduct_val_ptr_len);
int wtpProduct_undo_setup( dot11WtpInfoTable_rowreq_ctx *rowreq_ctx );
int wtpProduct_set( dot11WtpInfoTable_rowreq_ctx *rowreq_ctx, char *wtpProduct_val_ptr,  size_t wtpProduct_val_ptr_len );
int wtpProduct_undo( dot11WtpInfoTable_rowreq_ctx *rowreq_ctx );

int wtpDevTypeNum_check_value( dot11WtpInfoTable_rowreq_ctx *rowreq_ctx, char *wtpDevTypeNum_val_ptr,  size_t wtpDevTypeNum_val_ptr_len);
int wtpDevTypeNum_undo_setup( dot11WtpInfoTable_rowreq_ctx *rowreq_ctx );
int wtpDevTypeNum_set( dot11WtpInfoTable_rowreq_ctx *rowreq_ctx, char *wtpDevTypeNum_val_ptr,  size_t wtpDevTypeNum_val_ptr_len );
int wtpDevTypeNum_undo( dot11WtpInfoTable_rowreq_ctx *rowreq_ctx );

int wtpSoftVersionInfo_check_value( dot11WtpInfoTable_rowreq_ctx *rowreq_ctx, char *wtpSoftVersionInfo_val_ptr,  size_t wtpSoftVersionInfo_val_ptr_len);
int wtpSoftVersionInfo_undo_setup( dot11WtpInfoTable_rowreq_ctx *rowreq_ctx );
int wtpSoftVersionInfo_set( dot11WtpInfoTable_rowreq_ctx *rowreq_ctx, char *wtpSoftVersionInfo_val_ptr,  size_t wtpSoftVersionInfo_val_ptr_len );
int wtpSoftVersionInfo_undo( dot11WtpInfoTable_rowreq_ctx *rowreq_ctx );

int wtpUpTime_check_value( dot11WtpInfoTable_rowreq_ctx *rowreq_ctx, u_long wtpUpTime_val);
int wtpUpTime_undo_setup( dot11WtpInfoTable_rowreq_ctx *rowreq_ctx );
int wtpUpTime_set( dot11WtpInfoTable_rowreq_ctx *rowreq_ctx, u_long wtpUpTime_val );
int wtpUpTime_undo( dot11WtpInfoTable_rowreq_ctx *rowreq_ctx );

int wtpOnlineTime_check_value( dot11WtpInfoTable_rowreq_ctx *rowreq_ctx, u_long wtpOnlineTime_val);
int wtpOnlineTime_undo_setup( dot11WtpInfoTable_rowreq_ctx *rowreq_ctx );
int wtpOnlineTime_set( dot11WtpInfoTable_rowreq_ctx *rowreq_ctx, u_long wtpOnlineTime_val );
int wtpOnlineTime_undo( dot11WtpInfoTable_rowreq_ctx *rowreq_ctx );

int wtpSysSoftName_check_value( dot11WtpInfoTable_rowreq_ctx *rowreq_ctx, char *wtpSysSoftName_val_ptr,  size_t wtpSysSoftName_val_ptr_len);
int wtpSysSoftName_undo_setup( dot11WtpInfoTable_rowreq_ctx *rowreq_ctx );
int wtpSysSoftName_set( dot11WtpInfoTable_rowreq_ctx *rowreq_ctx, char *wtpSysSoftName_val_ptr,  size_t wtpSysSoftName_val_ptr_len );
int wtpSysSoftName_undo( dot11WtpInfoTable_rowreq_ctx *rowreq_ctx );

int wtpSysHWVersion_check_value( dot11WtpInfoTable_rowreq_ctx *rowreq_ctx, char *wtpSysHWVersion_val_ptr,  size_t wtpSysHWVersion_val_ptr_len);
int wtpSysHWVersion_undo_setup( dot11WtpInfoTable_rowreq_ctx *rowreq_ctx );
int wtpSysHWVersion_set( dot11WtpInfoTable_rowreq_ctx *rowreq_ctx, char *wtpSysHWVersion_val_ptr,  size_t wtpSysHWVersion_val_ptr_len );
int wtpSysHWVersion_undo( dot11WtpInfoTable_rowreq_ctx *rowreq_ctx );

int wtpSN_check_value( dot11WtpInfoTable_rowreq_ctx *rowreq_ctx, char *wtpSN_val_ptr,  size_t wtpSN_val_ptr_len);
int wtpSN_undo_setup( dot11WtpInfoTable_rowreq_ctx *rowreq_ctx );
int wtpSN_set( dot11WtpInfoTable_rowreq_ctx *rowreq_ctx, char *wtpSN_val_ptr,  size_t wtpSN_val_ptr_len );
int wtpSN_undo( dot11WtpInfoTable_rowreq_ctx *rowreq_ctx );

int wtpSysSoftProductor_check_value( dot11WtpInfoTable_rowreq_ctx *rowreq_ctx, char *wtpSysSoftProductor_val_ptr,  size_t wtpSysSoftProductor_val_ptr_len);
int wtpSysSoftProductor_undo_setup( dot11WtpInfoTable_rowreq_ctx *rowreq_ctx );
int wtpSysSoftProductor_set( dot11WtpInfoTable_rowreq_ctx *rowreq_ctx, char *wtpSysSoftProductor_val_ptr,  size_t wtpSysSoftProductor_val_ptr_len );
int wtpSysSoftProductor_undo( dot11WtpInfoTable_rowreq_ctx *rowreq_ctx );

int wtpDomain_check_value( dot11WtpInfoTable_rowreq_ctx *rowreq_ctx, char *wtpDomain_val_ptr,  size_t wtpDomain_val_ptr_len);
int wtpDomain_undo_setup( dot11WtpInfoTable_rowreq_ctx *rowreq_ctx );
int wtpDomain_set( dot11WtpInfoTable_rowreq_ctx *rowreq_ctx, char *wtpDomain_val_ptr,  size_t wtpDomain_val_ptr_len );
int wtpDomain_undo( dot11WtpInfoTable_rowreq_ctx *rowreq_ctx );

int wtpSysRestart_check_value( dot11WtpInfoTable_rowreq_ctx *rowreq_ctx, u_long wtpSysRestart_val);
int wtpSysRestart_undo_setup( dot11WtpInfoTable_rowreq_ctx *rowreq_ctx );
int wtpSysRestart_set( dot11WtpInfoTable_rowreq_ctx *rowreq_ctx, u_long wtpSysRestart_val );
int wtpSysRestart_undo( dot11WtpInfoTable_rowreq_ctx *rowreq_ctx );

int wtpSysReset_check_value( dot11WtpInfoTable_rowreq_ctx *rowreq_ctx, u_long wtpSysReset_val);
int wtpSysReset_undo_setup( dot11WtpInfoTable_rowreq_ctx *rowreq_ctx );
int wtpSysReset_set( dot11WtpInfoTable_rowreq_ctx *rowreq_ctx, u_long wtpSysReset_val );
int wtpSysReset_undo( dot11WtpInfoTable_rowreq_ctx *rowreq_ctx );

int wtpColdReboot_check_value( dot11WtpInfoTable_rowreq_ctx *rowreq_ctx, u_long wtpColdReboot_val);
int wtpColdReboot_undo_setup( dot11WtpInfoTable_rowreq_ctx *rowreq_ctx );
int wtpColdReboot_set( dot11WtpInfoTable_rowreq_ctx *rowreq_ctx, u_long wtpColdReboot_val );
int wtpColdReboot_undo( dot11WtpInfoTable_rowreq_ctx *rowreq_ctx );


int dot11WtpInfoTable_check_dependencies(dot11WtpInfoTable_rowreq_ctx *ctx);


#ifdef __cplusplus
}
#endif

#endif /* DOT11WTPINFOTABLE_DATA_SET_H */
