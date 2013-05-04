/*
 * Note: this file originally auto-generated by mib2c using
 *       version : 12077 $ of $ 
 *
 * $Id:$
 */
#ifndef DOT11PORTALNOAUTHENTICATEDTABLE_DATA_SET_H
#define DOT11PORTALNOAUTHENTICATEDTABLE_DATA_SET_H

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
 *** Table dot11PortalNoAuthenticatedTable
 ***
 **********************************************************************
 **********************************************************************/
/*
 * DOT11-AC-MIB::dot11PortalNoAuthenticatedTable is subid 5 of dot11ConfigPortalServerGroup.
 * Its status is Current.
 * OID: .1.3.6.1.4.1.31656.6.1.2.23.5, length: 12
*/


int dot11PortalNoAuthenticatedTable_undo_setup( dot11PortalNoAuthenticatedTable_rowreq_ctx *rowreq_ctx);
int dot11PortalNoAuthenticatedTable_undo_cleanup( dot11PortalNoAuthenticatedTable_rowreq_ctx *rowreq_ctx);
int dot11PortalNoAuthenticatedTable_undo( dot11PortalNoAuthenticatedTable_rowreq_ctx *rowreq_ctx);
int dot11PortalNoAuthenticatedTable_commit( dot11PortalNoAuthenticatedTable_rowreq_ctx *rowreq_ctx);
int dot11PortalNoAuthenticatedTable_undo_commit( dot11PortalNoAuthenticatedTable_rowreq_ctx *rowreq_ctx);


int NoAuthRowStatus_check_value( dot11PortalNoAuthenticatedTable_rowreq_ctx *rowreq_ctx, u_long NoAuthRowStatus_val);
int NoAuthRowStatus_undo_setup( dot11PortalNoAuthenticatedTable_rowreq_ctx *rowreq_ctx );
int NoAuthRowStatus_set( dot11PortalNoAuthenticatedTable_rowreq_ctx *rowreq_ctx, u_long NoAuthRowStatus_val );
int NoAuthRowStatus_undo( dot11PortalNoAuthenticatedTable_rowreq_ctx *rowreq_ctx );


int dot11PortalNoAuthenticatedTable_check_dependencies(dot11PortalNoAuthenticatedTable_rowreq_ctx *ctx);


#ifdef __cplusplus
}
#endif

#endif /* DOT11PORTALNOAUTHENTICATEDTABLE_DATA_SET_H */
