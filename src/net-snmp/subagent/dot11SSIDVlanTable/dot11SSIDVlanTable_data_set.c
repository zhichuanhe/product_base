/*
 * Note: this file originally auto-generated by mib2c using
 *       version : 12077 $ of $
 *
 * $Id:$
 *
 */
/* standard Net-SNMP includes */
#include <net-snmp/net-snmp-config.h>
#include <net-snmp/net-snmp-includes.h>
#include <net-snmp/agent/net-snmp-agent-includes.h>

/* include our parent header */
#include "dot11SSIDVlanTable.h"
#include "autelanWtpGroup.h"
#include "ws_init_dbus.h"
#include "ws_dbus_list_interface.h"


/** @defgroup data_set data_set: Routines to set data
 *
 * These routines are used to set the value for individual objects. The
 * row context is passed, along with the new value.
 * 
 * @{
 */
/**********************************************************************
 **********************************************************************
 ***
 *** Table dot11SSIDVlanTable
 ***
 **********************************************************************
 **********************************************************************/
/*
 * DOT11-AC-MIB::dot11SSIDVlanTable is subid 3 of dot11wtpConfig.
 * Its status is Current.
 * OID: .1.3.6.1.4.1.31656.6.1.2.12.3, length: 12
*/
    /*
     * NOTE: if you update this chart, please update the versions in
     *       local/mib2c-conf.d/parent-set.m2i
     *       agent/mibgroup/helpers/baby_steps.c
     * while you're at it.
     */
    /*
     ***********************************************************************
     * Baby Steps Flow Chart (2004.06.05)                                  *
     *                                                                     *
     * +--------------+    +================+    U = unconditional path    *
     * |optional state|    ||required state||    S = path for success      *
     * +--------------+    +================+    E = path for error        *
     ***********************************************************************
     *
     *                        +--------------+
     *                        |     pre      |
     *                        |   request    |
     *                        +--------------+
     *                               | U
     * +-------------+        +==============+
     * |    row    |f|<-------||  object    ||
     * |  create   |1|      E ||  lookup    ||
     * +-------------+        +==============+
     *     E |   | S                 | S
     *       |   +------------------>|
     *       |                +==============+
     *       |              E ||   check    ||
     *       |<---------------||   values   ||
     *       |                +==============+
     *       |                       | S
     *       |                +==============+
     *       |       +<-------||   undo     ||
     *       |       |      E ||   setup    ||
     *       |       |        +==============+
     *       |       |               | S
     *       |       |        +==============+
     *       |       |        ||    set     ||-------------------------->+
     *       |       |        ||   value    || E                         |
     *       |       |        +==============+                           |
     *       |       |               | S                                 |
     *       |       |        +--------------+                           |
     *       |       |        |    check     |-------------------------->|
     *       |       |        |  consistency | E                         |
     *       |       |        +--------------+                           |
     *       |       |               | S                                 |
     *       |       |        +==============+         +==============+  |
     *       |       |        ||   commit   ||-------->||     undo   ||  |
     *       |       |        ||            || E       ||    commit  ||  |
     *       |       |        +==============+         +==============+  |
     *       |       |               | S                     U |<--------+
     *       |       |        +--------------+         +==============+
     *       |       |        | irreversible |         ||    undo    ||
     *       |       |        |    commit    |         ||     set    ||
     *       |       |        +--------------+         +==============+
     *       |       |               | U                     U |
     *       |       +-------------->|<------------------------+
     *       |                +==============+
     *       |                ||   undo     ||
     *       |                ||  cleanup   ||
     *       |                +==============+
     *       +---------------------->| U
     *                               |
     *                          (err && f1)------------------->+
     *                               |                         |
     *                        +--------------+         +--------------+
     *                        |    post      |<--------|      row     |
     *                        |   request    |       U |    release   |
     *                        +--------------+         +--------------+
     *
     */

/**
 * Setup up context with information needed to undo a set request.
 *
 * This function will be called before the individual node undo setup
 * functions are called. If you need to do any undo setup that is not
 * related to a specific column, you can do it here.
 *
 * Note that the undo context has been allocated with
 * dot11SSIDVlanTable_allocate_data(), but may need extra
 * initialization similar to what you may have done in
 * dot11SSIDVlanTable_rowreq_ctx_init().
 * Note that an individual node's undo_setup function will only be called
 * if that node is being set to a new value.
 *
 * If there is any setup specific to a particular column (e.g. allocating
 * memory for a string), you should do that setup in the node's undo_setup
 * function, so it won't be done unless it is necessary.
 *
 * @param rowreq_ctx
 *        Pointer to the table context (dot11SSIDVlanTable_rowreq_ctx)
 *
 * @retval MFD_SUCCESS : success
 * @retval MFD_ERROR   : error. set will fail.
 */
int
dot11SSIDVlanTable_undo_setup( dot11SSIDVlanTable_rowreq_ctx *rowreq_ctx)
{
    int rc = MFD_SUCCESS;

    DEBUGMSGTL(("verbose:dot11SSIDVlanTable:dot11SSIDVlanTable_undo_setup","called\n"));

    /** we should have a non-NULL pointer */
    netsnmp_assert( NULL != rowreq_ctx );

    /*
     * TODO:451:M: |-> Setup dot11SSIDVlanTable undo.
     * set up dot11SSIDVlanTable undo information, in preparation for a set.
     * Undo storage is in (* wlanSSID_val_ptr_ptr )*
     */

    return rc;
} /* dot11SSIDVlanTable_undo_setup */

/**
 * Undo a set request.
 *
 * This function will be called before the individual node undo
 * functions are called. If you need to do any undo that is not
 * related to a specific column, you can do it here.
 *
 * Note that an individual node's undo function will only be called
 * if that node is being set to a new value.
 *
 * If there is anything  specific to a particular column (e.g. releasing
 * memory for a string), you should do that setup in the node's undo
 * function, so it won't be done unless it is necessary.
 *
 * @param rowreq_ctx
 *        Pointer to the table context (dot11SSIDVlanTable_rowreq_ctx)
 *
 * @retval MFD_SUCCESS : success
 * @retval MFD_ERROR   : error. set will fail.
 */
int
dot11SSIDVlanTable_undo( dot11SSIDVlanTable_rowreq_ctx *rowreq_ctx)
{
    int rc = MFD_SUCCESS;

    DEBUGMSGTL(("verbose:dot11SSIDVlanTable:dot11SSIDVlanTable_undo","called\n"));

    /** we should have a non-NULL pointer */
    netsnmp_assert( NULL != rowreq_ctx );

    /*
     * TODO:451:M: |-> dot11SSIDVlanTable undo.
     * dot11SSIDVlanTable undo information, in response to a failed set.
     * Undo storage is in (* wlanSSID_val_ptr_ptr )*
     */

    return rc;
} /* dot11SSIDVlanTable_undo_setup */

/**
 * Cleanup up context undo information.
 *
 * This function will be called after set/commit processing. If you
 * allocated any resources in undo_setup, this is the place to release
 * those resources.
 *
 * This function is called regardless of the success or failure of the set
 * request. If you need to perform different steps for cleanup depending
 * on success or failure, you can add a flag to the rowreq_ctx.
 *
 * @param rowreq_ctx
 *        Pointer to the table context (dot11SSIDVlanTable_rowreq_ctx)
 *
 * @retval MFD_SUCCESS : success
 * @retval MFD_ERROR   : error
 */
int
dot11SSIDVlanTable_undo_cleanup( dot11SSIDVlanTable_rowreq_ctx *rowreq_ctx)
{
    int rc = MFD_SUCCESS;

    DEBUGMSGTL(("verbose:dot11SSIDVlanTable:dot11SSIDVlanTable_undo_cleanup","called\n"));

    /** we should have a non-NULL pointer */
    netsnmp_assert( NULL != rowreq_ctx );

    /*
     * TODO:452:M: |-> Cleanup dot11SSIDVlanTable undo.
     * Undo storage is in (* wlanSSID_val_ptr_ptr )*
     */

    return rc;
} /* dot11SSIDVlanTable_undo_cleanup */

/**
 * commit new values.
 *
 * At this point, you should have done everything you can to ensure that
 * this commit will not fail.
 *
 * Should you need different behavior depending on which columns were
 * set, rowreq_ctx->column_set_flags will indicate which writeable columns were
 * set. The definitions for the COLUMN_*_FLAG bits can be found in
 * dot11SSIDVlanTable_oids.h.
 * A new row will have the MFD_ROW_CREATED bit set in rowreq_flags.
 *
 * @param dot11SSIDVlanTable_rowreq_ctx
 *        Pointer to the users context.
 *
 * @retval MFD_SUCCESS : success
 * @retval MFD_ERROR   : error
 */
int
dot11SSIDVlanTable_commit( dot11SSIDVlanTable_rowreq_ctx *rowreq_ctx)
{
    int rc = MFD_SUCCESS;
    int             save_flags;

    DEBUGMSGTL(("verbose:dot11SSIDVlanTable:dot11SSIDVlanTable_commit","called\n"));

    /** we should have a non-NULL pointer */
    netsnmp_assert( NULL != rowreq_ctx );

    /*
     * save flags, then clear until we actually do something
     */
    save_flags = rowreq_ctx->column_set_flags;
    rowreq_ctx->column_set_flags = 0;

	if(RS_NOTREADY == rowreq_ctx->data.VlanRowStatus) {
        rowreq_ctx->data.VlanRowStatus = RS_NOTINSERVICE;
    }
    /*
     * commit dot11SSIDVlanTable data
     * 1) check the column's flag in save_flags to see if it was set.
     * 2) clear the flag when you handle that column
     * 3) set the column's flag in column_set_flags if it needs undo
     *    processing in case of a failure.
     */
    if (save_flags & COLUMN_VLANID_FLAG) {
       save_flags &= ~COLUMN_VLANID_FLAG; /* clear VlanId */
       /*
        * TODO:482:o: |-> commit column VlanId.
        */
       rc = MFD_SUCCESS;
       if(-1 == rc) {
           snmp_log(LOG_ERR,"dot11SSIDVlanTable column VlanId commit failed\n");
       }
       else {
            /*
             * set flag, in case we need to undo VlanId
             */
            rowreq_ctx->column_set_flags |= COLUMN_VLANID_FLAG;
       }
    }

    if (save_flags & COLUMN_VLANROWSTATUS_FLAG) {
       save_flags &= ~COLUMN_VLANROWSTATUS_FLAG; /* clear VlanRowStatus */
	   switch(rowreq_ctx->data.VlanRowStatus) {
            case RS_ACTIVE:
                {
					rc = -1;
					if(0 == rowreq_ctx->data.is_modify)
					{			
						rowreq_ctx->data.is_modify = 1;
						
	                    void *connection = NULL;
	                    if(SNMPD_DBUS_SUCCESS == get_instance_dbus_connection(rowreq_ctx->data.parameter, &connection, SNMPD_INSTANCE_MASTER_V3)) 
						{
							int ret = 0, ret1=0,ret2=0;
							char vlanid[10];
							ret = wtp_apply_wlan(rowreq_ctx->data.parameter, connection, rowreq_ctx->data.localWtpId, rowreq_ctx->data.localWlanID);
							if(ret ==1)
							{
								ret1=config_wlan_service(rowreq_ctx->data.parameter, connection, rowreq_ctx->data.localWlanID, "disable");
								if(ret1==1)
								{
									snprintf(vlanid,sizeof(vlanid)-1,"%d",rowreq_ctx->data.VlanId);
									ret2=set_wlan_vlan_id(rowreq_ctx->data.parameter, connection, rowreq_ctx->data.localWlanID, vlanid);
									snmp_log(LOG_DEBUG, "dot11SSIDVlanTable_commit: set_wlan_vlan_id: ret2 = %d\n", ret2);
									if(1 == ret2)
									{
										rc = MFD_SUCCESS;
									}
									else
									{
										rowreq_ctx->data.is_modify = 0;
										rc = -1;								
										if(SNMPD_CONNECTION_ERROR == ret2) {
	                                    	close_slot_dbus_connection(rowreq_ctx->data.parameter.slot_id);
										}
										break;
	                        	    }
									config_wlan_service(rowreq_ctx->data.parameter, connection, rowreq_ctx->data.localWlanID, "enable");
								}
								else	
	                            {
	                            	rowreq_ctx->data.is_modify = 0;
									rc = -1;
	                                if(SNMPD_CONNECTION_ERROR == ret1) {
	                                    close_slot_dbus_connection(rowreq_ctx->data.parameter.slot_id);
	                        	    }
	                            }
							}
	                    }				
					}
					else
					{
						rc = MFD_SUCCESS;
						break;
					}
                }
                break;

            case RS_CREATEANDWAIT:
                rowreq_ctx->data.VlanRowStatus = RS_NOTREADY;
                rc = MFD_SUCCESS;
                break;
                
            case RS_DESTROY:  
                {
					rc = -1;
                    int ret = 0;
                    void *connection = NULL;
                    if(SNMPD_DBUS_SUCCESS == get_instance_dbus_connection(rowreq_ctx->data.parameter, &connection, SNMPD_INSTANCE_MASTER_V3)) 
					{
						int ret1 =1,ret2 = 0;
						ret1=config_wlan_service(rowreq_ctx->data.parameter, connection, rowreq_ctx->data.localWlanID, "disable");
						if(ret1 ==1 )
						{
							ret2 = undo_wlan_vlan_cmd(rowreq_ctx->data.parameter, connection, rowreq_ctx->data.localWlanID);
							snmp_log(LOG_DEBUG, "dot11SSIDVlanTable_commit: undo_wlan_vlan_cmd: ret2 = %d\n", ret2);							
							if(1 == ret2)
							{
								rc = MFD_SUCCESS;
							}
							else
							{
								rc = -1;
								if(SNMPD_CONNECTION_ERROR == ret2){
	                            	close_slot_dbus_connection(rowreq_ctx->data.parameter.slot_id);
								}
								break;
	                        }  
							config_wlan_service(rowreq_ctx->data.parameter, connection, rowreq_ctx->data.localWlanID, "enable");
							wtp_delete_wlan(rowreq_ctx->data.parameter, connection, rowreq_ctx->data.localWtpId, rowreq_ctx->data.localWlanID);
						}
						else
						{
							rc = -1;
							if(SNMPD_CONNECTION_ERROR == ret1) {
	                        	close_slot_dbus_connection(rowreq_ctx->data.parameter.slot_id);
							}
							break;
	            	    }
                    }
                }
				if(MFD_SUCCESS != rc)
				{
					rowreq_ctx->rowreq_flags &= ~MFD_ROW_DELETED;
				}
                break;
                
            default:
                break;
        }
       /*
        * TODO:482:o: |-> commit column VlanRowStatus.
        */
       if(-1 == rc) {
           snmp_log(LOG_ERR,"dot11SSIDVlanTable column VlanRowStatus commit failed\n");
       }
       else {
            /*
             * set flag, in case we need to undo VlanRowStatus
             */
            rowreq_ctx->column_set_flags |= COLUMN_VLANROWSTATUS_FLAG;
       }
    }

    /*
     * if we successfully commited this row, set the dirty flag.
     */
    if (MFD_SUCCESS == rc) {
        rowreq_ctx->rowreq_flags |= MFD_ROW_DIRTY;
    }

    if (save_flags) {
       snmp_log(LOG_ERR, "unhandled columns (0x%x) in commit\n", save_flags);
       return MFD_ERROR;
    }

    return rc;
} /* dot11SSIDVlanTable_commit */

/**
 * undo commit new values.
 *
 * Should you need different behavior depending on which columns were
 * set, rowreq_ctx->column_set_flags will indicate which writeable columns were
 * set. The definitions for the COLUMN_*_FLAG bits can be found in
 * dot11SSIDVlanTable_oids.h.
 * A new row will have the MFD_ROW_CREATED bit set in rowreq_flags.
 *
 * @param dot11SSIDVlanTable_rowreq_ctx
 *        Pointer to the users context.
 *
 * @retval MFD_SUCCESS : success
 * @retval MFD_ERROR   : error
 */
int
dot11SSIDVlanTable_undo_commit( dot11SSIDVlanTable_rowreq_ctx *rowreq_ctx)
{
    int rc = MFD_SUCCESS;

    DEBUGMSGTL(("verbose:dot11SSIDVlanTable:dot11SSIDVlanTable_undo_commit","called\n"));

    /** we should have a non-NULL pointer */
    netsnmp_assert( NULL != rowreq_ctx );

    /*
     * TODO:485:M: |-> Undo dot11SSIDVlanTable commit.
     * check the column's flag in rowreq_ctx->column_set_flags to see
     * if it was set during commit, then undo it.
     *
     * eg: if (rowreq_ctx->column_set_flags & COLUMN__FLAG) {}
     */

    
    /*
     * if we successfully un-commited this row, clear the dirty flag.
     */
    if (MFD_SUCCESS == rc) {
        rowreq_ctx->rowreq_flags &= ~MFD_ROW_DIRTY;
    }

    return rc;
} /* dot11SSIDVlanTable_undo_commit */

/*
 * TODO:440:M: Implement dot11SSIDVlanTable node value checks.
 * TODO:450:M: Implement dot11SSIDVlanTable undo functions.
 * TODO:460:M: Implement dot11SSIDVlanTable set functions.
 * TODO:480:M: Implement dot11SSIDVlanTable commit functions.
 */
/*---------------------------------------------------------------------
 * DOT11-AC-MIB::dot11SSIDVlanEntry.VlanId
 * VlanId is subid 3 of dot11SSIDVlanEntry.
 * Its status is Current, and its access level is ReadWrite.
 * OID: .1.3.6.1.4.1.31656.6.1.2.12.3.1.3
 * Description:
Set Vlan Id.
 *
 * Attributes:
 *   accessible 1     isscalar 0     enums  0      hasdefval 0
 *   readable   1     iscolumn 1     ranges 0      hashint   0
 *   settable   1
 *
 *
 * Its syntax is INTEGER (based on perltype INTEGER)
 * The net-snmp type is ASN_INTEGER. The C type decl is long (long)
 */
/**
 * Check that the proposed new value is potentially valid.
 *
 * @param rowreq_ctx
 *        Pointer to the row request context.
 * @param VlanId_val
 *        A long containing the new value.
 *
 * @retval MFD_SUCCESS        : incoming value is legal
 * @retval MFD_NOT_VALID_NOW  : incoming value is not valid now
 * @retval MFD_NOT_VALID_EVER : incoming value is never valid
 *
 * This is the place to check for requirements that are not
 * expressed in the mib syntax (for example, a requirement that
 * is detailed in the description for an object).
 *
 * You should check that the requested change between the undo value and the
 * new value is legal (ie, the transistion from one value to another
 * is legal).
 *      
 *@note
 * This check is only to determine if the new value
 * is \b potentially valid. This is the first check of many, and
 * is one of the simplest ones.
 * 
 *@note
 * this is not the place to do any checks for values
 * which depend on some other value in the mib. Those
 * types of checks should be done in the
 * dot11SSIDVlanTable_check_dependencies() function.
 *
 * The following checks have already been done for you:
 *    The syntax is ASN_INTEGER
 *
 * If there a no other checks you need to do, simply return MFD_SUCCESS.
 *
 */
int
VlanId_check_value( dot11SSIDVlanTable_rowreq_ctx *rowreq_ctx, long VlanId_val)
{
    DEBUGMSGTL(("verbose:dot11SSIDVlanTable:VlanId_check_value","called\n"));

    /** should never get a NULL pointer */
    netsnmp_assert(NULL != rowreq_ctx);

    /*
     * TODO:441:o: |-> Check for valid VlanId value.
     */

	if(RS_NOTINSERVICE == rowreq_ctx->data.VlanRowStatus || RS_NOTREADY == rowreq_ctx->data.VlanRowStatus)
	    return MFD_SUCCESS; 
	else     
        return MFD_ERROR; /* VlanId value not illegal */
} /* VlanId_check_value */

/**
 * Save old value information
 *
 * @param rowreq_ctx
 *        Pointer to the table context (dot11SSIDVlanTable_rowreq_ctx)
 *
 * @retval MFD_SUCCESS : success
 * @retval MFD_ERROR   : error. set will fail.
 *
 * This function will be called after the table level undo setup function
 * dot11SSIDVlanTable_undo_setup has been called.
 *
 *@note
 * this function will only be called if a new value is set for this column.
 *
 * If there is any setup specific to a particular column (e.g. allocating
 * memory for a string), you should do that setup in this function, so it
 * won't be done unless it is necessary.
 */
int
VlanId_undo_setup( dot11SSIDVlanTable_rowreq_ctx *rowreq_ctx)
{
    DEBUGMSGTL(("verbose:dot11SSIDVlanTable:VlanId_undo_setup","called\n"));

    /** should never get a NULL pointer */
    netsnmp_assert(NULL != rowreq_ctx);

    /*
     * TODO:455:o: |-> Setup VlanId undo.
     */
    /*
     * copy VlanId data
     * set rowreq_ctx->undo->VlanId from rowreq_ctx->data.VlanId
     */
    rowreq_ctx->undo->VlanId = rowreq_ctx->data.VlanId;


    return MFD_SUCCESS;
} /* VlanId_undo_setup */

/**
 * Set the new value.
 *
 * @param rowreq_ctx
 *        Pointer to the users context. You should know how to
 *        manipulate the value from this object.
 * @param VlanId_val
 *        A long containing the new value.
 */
int
VlanId_set( dot11SSIDVlanTable_rowreq_ctx *rowreq_ctx, long VlanId_val )
{

    DEBUGMSGTL(("verbose:dot11SSIDVlanTable:VlanId_set","called\n"));

    /** should never get a NULL pointer */
    netsnmp_assert(NULL != rowreq_ctx);

    /*
     * TODO:461:M: |-> Set VlanId value.
     * set VlanId value in rowreq_ctx->data
     */
	int rc = MFD_ERROR;
	if(RS_NOTINSERVICE == rowreq_ctx->data.VlanRowStatus && rowreq_ctx->data.is_modify) /*modify*/
	{
		void *connection = NULL;
        if(SNMPD_DBUS_SUCCESS == get_instance_dbus_connection(rowreq_ctx->data.parameter, &connection, SNMPD_INSTANCE_MASTER_V3)) 
        {
			int ret1=0,ret2=0;
			char vlanid[10] = { 0 };
			memset(vlanid,0,10);
			ret1=config_wlan_service(rowreq_ctx->data.parameter, connection, rowreq_ctx->data.localWlanID, "disable");
			if(ret1==1)
			{
				snprintf(vlanid,sizeof(vlanid)-1,"%d",VlanId_val);
				ret2=set_wlan_vlan_id(rowreq_ctx->data.parameter, connection,rowreq_ctx->data.localWlanID,vlanid);
				if(1 == ret2)
				{
					rowreq_ctx->data.VlanId = VlanId_val;
					rc = MFD_SUCCESS;
				}
				else if(SNMPD_CONNECTION_ERROR == ret2){
					close_slot_dbus_connection(rowreq_ctx->data.parameter.slot_id);
				}
				
				config_wlan_service(rowreq_ctx->data.parameter, connection,rowreq_ctx->data.localWlanID,"enable");
			}
			else if(SNMPD_CONNECTION_ERROR == ret1) {
				close_slot_dbus_connection(rowreq_ctx->data.parameter.slot_id);
			}
        }
	}
	else if(RS_NOTREADY == rowreq_ctx->data.VlanRowStatus)/*creat and wait*/
	{
		rowreq_ctx->data.VlanId = VlanId_val;
		rc = MFD_SUCCESS;
	}
    return rc;  
} /* VlanId_set */

/**
 * undo the previous set.
 *
 * @param rowreq_ctx
 *        Pointer to the users context.
 */
int
VlanId_undo( dot11SSIDVlanTable_rowreq_ctx *rowreq_ctx)
{

    DEBUGMSGTL(("verbose:dot11SSIDVlanTable:VlanId_undo","called\n"));

    netsnmp_assert(NULL != rowreq_ctx);

    /*
     * TODO:456:o: |-> Clean up VlanId undo.
     */
    /*
     * copy VlanId data
     * set rowreq_ctx->data.VlanId from rowreq_ctx->undo->VlanId
     */
    rowreq_ctx->data.VlanId = rowreq_ctx->undo->VlanId;

    
    return MFD_SUCCESS;
} /* VlanId_undo */

/*---------------------------------------------------------------------
 * DOT11-AC-MIB::dot11SSIDVlanEntry.VlanRowStatus
 * VlanRowStatus is subid 4 of dot11SSIDVlanEntry.
 * Its status is Current, and its access level is Create.
 * OID: .1.3.6.1.4.1.31656.6.1.2.12.3.1.4
 * Description:
This variable is used to create, modify, and/or delete a row
		        	 in this table.
 *
 * Attributes:
 *   accessible 1     isscalar 0     enums  1      hasdefval 0
 *   readable   1     iscolumn 1     ranges 0      hashint   0
 *   settable   1
 *
 * Enum range: 3/8. Values:  active(1), notInService(2), notReady(3), createAndGo(4), createAndWait(5), destroy(6)
 *
 * Its syntax is RowStatus (based on perltype INTEGER)
 * The net-snmp type is ASN_INTEGER. The C type decl is long (u_long)
 */
/**
 * Check that the proposed new value is potentially valid.
 *
 * @param rowreq_ctx
 *        Pointer to the row request context.
 * @param VlanRowStatus_val
 *        A long containing the new value.
 *
 * @retval MFD_SUCCESS        : incoming value is legal
 * @retval MFD_NOT_VALID_NOW  : incoming value is not valid now
 * @retval MFD_NOT_VALID_EVER : incoming value is never valid
 *
 * This is the place to check for requirements that are not
 * expressed in the mib syntax (for example, a requirement that
 * is detailed in the description for an object).
 *
 * You should check that the requested change between the undo value and the
 * new value is legal (ie, the transistion from one value to another
 * is legal).
 *      
 *@note
 * This check is only to determine if the new value
 * is \b potentially valid. This is the first check of many, and
 * is one of the simplest ones.
 * 
 *@note
 * this is not the place to do any checks for values
 * which depend on some other value in the mib. Those
 * types of checks should be done in the
 * dot11SSIDVlanTable_check_dependencies() function.
 *
 * The following checks have already been done for you:
 *    The syntax is ASN_INTEGER
 *    The value is one of  active(1), notInService(2), notReady(3), createAndGo(4), createAndWait(5), destroy(6)
 *
 * If there a no other checks you need to do, simply return MFD_SUCCESS.
 *
 */
int
VlanRowStatus_check_value( dot11SSIDVlanTable_rowreq_ctx *rowreq_ctx, u_long VlanRowStatus_val)
{
    DEBUGMSGTL(("verbose:dot11SSIDVlanTable:VlanRowStatus_check_value","called\n"));

    /** should never get a NULL pointer */
    netsnmp_assert(NULL != rowreq_ctx);

    /*
     * TODO:441:o: |-> Check for valid VlanRowStatus value.
     */

    return MFD_SUCCESS; /* VlanRowStatus value not illegal */
} /* VlanRowStatus_check_value */

/**
 * Save old value information
 *
 * @param rowreq_ctx
 *        Pointer to the table context (dot11SSIDVlanTable_rowreq_ctx)
 *
 * @retval MFD_SUCCESS : success
 * @retval MFD_ERROR   : error. set will fail.
 *
 * This function will be called after the table level undo setup function
 * dot11SSIDVlanTable_undo_setup has been called.
 *
 *@note
 * this function will only be called if a new value is set for this column.
 *
 * If there is any setup specific to a particular column (e.g. allocating
 * memory for a string), you should do that setup in this function, so it
 * won't be done unless it is necessary.
 */
int
VlanRowStatus_undo_setup( dot11SSIDVlanTable_rowreq_ctx *rowreq_ctx)
{
    DEBUGMSGTL(("verbose:dot11SSIDVlanTable:VlanRowStatus_undo_setup","called\n"));

    /** should never get a NULL pointer */
    netsnmp_assert(NULL != rowreq_ctx);

    /*
     * TODO:455:o: |-> Setup VlanRowStatus undo.
     */
    /*
     * copy VlanRowStatus data
     * set rowreq_ctx->undo->VlanRowStatus from rowreq_ctx->data.VlanRowStatus
     */
    rowreq_ctx->undo->VlanRowStatus = rowreq_ctx->data.VlanRowStatus;


    return MFD_SUCCESS;
} /* VlanRowStatus_undo_setup */

/**
 * Set the new value.
 *
 * @param rowreq_ctx
 *        Pointer to the users context. You should know how to
 *        manipulate the value from this object.
 * @param VlanRowStatus_val
 *        A long containing the new value.
 */
int
VlanRowStatus_set( dot11SSIDVlanTable_rowreq_ctx *rowreq_ctx, u_long VlanRowStatus_val )
{

    DEBUGMSGTL(("verbose:dot11SSIDVlanTable:VlanRowStatus_set","called\n"));

    /** should never get a NULL pointer */
    netsnmp_assert(NULL != rowreq_ctx);

    /*
     * TODO:461:M: |-> Set VlanRowStatus value.
     * set VlanRowStatus value in rowreq_ctx->data
     */
    int ret = MFD_SUCCESS;
    
    switch(VlanRowStatus_val) {

		case RS_NOTINSERVICE:
        case RS_ACTIVE:         
        case RS_DESTROY:
            rowreq_ctx->data.VlanRowStatus = VlanRowStatus_val;
            ret = MFD_SUCCESS;
            break;
    
        case RS_NOTREADY:
        case RS_CREATEANDGO:
            ret = MFD_ERROR;
            break;
    
        case RS_CREATEANDWAIT:
            if(RS_NONEXISTENT == rowreq_ctx->data.VlanRowStatus) {
                rowreq_ctx->data.localWtpId = global_to_local_ID(&(rowreq_ctx->data.parameter), 
                                                                    rowreq_ctx->tbl_idx.wtpId, 
                                                                    WIRELESS_MAX_NUM);
				
				rowreq_ctx->data.localWlanID = global_to_local_ID(&(rowreq_ctx->data.parameter), 
                                                                    rowreq_ctx->tbl_idx.wlanId, 
                                                                    WIRELESS_MAX_NUM);
				
                rowreq_ctx->data.VlanRowStatus = VlanRowStatus_val;
                ret = MFD_SUCCESS;
            }
            else {
                ret = MFD_ERROR;
            }
            break;
    
        default:
            snmp_log(LOG_WARNING, "unknow %s in VlanRowStatus_set", VlanRowStatus_val);
            ret = MFD_ERROR;
            break;
    }

    return ret;
} /* VlanRowStatus_set */

/**
 * undo the previous set.
 *
 * @param rowreq_ctx
 *        Pointer to the users context.
 */
int
VlanRowStatus_undo( dot11SSIDVlanTable_rowreq_ctx *rowreq_ctx)
{

    DEBUGMSGTL(("verbose:dot11SSIDVlanTable:VlanRowStatus_undo","called\n"));

    netsnmp_assert(NULL != rowreq_ctx);

    /*
     * TODO:456:o: |-> Clean up VlanRowStatus undo.
     */
    /*
     * copy VlanRowStatus data
     * set rowreq_ctx->data.VlanRowStatus from rowreq_ctx->undo->VlanRowStatus
     */
    rowreq_ctx->data.VlanRowStatus = rowreq_ctx->undo->VlanRowStatus;

    
    return MFD_SUCCESS;
} /* VlanRowStatus_undo */

/**
 * check dependencies
 *
 * This is useful for for tables which have dependencies between columns
 * (or rows, or tables). For example, two columns allocating a percentage
 * of something add up 100%.
 *
 * Should you need different behavior depending on which columns were
 * set, rowreq_ctx->column_set_flags will indicate which writeable columns were
 * set. The definitions for the COLUMN_*_FLAG bits can be found in
 * dot11SSIDVlanTable_oids.h.
 * A new row will have the MFD_ROW_CREATED bit set in rowreq_flags.
 *
 * @retval MFD_SUCCESS all the changes to the row are legal
 * @retval MFD_ERROR   one or more changes are not legal
 *
 * (see README-table-dot11SSIDVlanTable if you don't have dependencies)
 */
int
dot11SSIDVlanTable_check_dependencies(dot11SSIDVlanTable_rowreq_ctx *rowreq_ctx)
{
    int rc = MFD_SUCCESS;
    
    DEBUGMSGTL(("internal:dot11SSIDVlanTable:dot11SSIDVlanTable_check_dependencies","called\n"));

    netsnmp_assert(NULL != rowreq_ctx);

    /*
     * TODO:470:o: Check dot11SSIDVlanTable row dependencies.
     * check that all new value are legal and consistent with each other
     */
    /*
     * check RowStatus dependencies
     */
    if (rowreq_ctx->column_set_flags & COLUMN_VLANROWSTATUS_FLAG) {
        /*
         * check for valid RowStatus transition (old, new)
         * (Note: move transition check to 
         *  to catch errors earlier)
         */
        rc = check_rowstatus_transition( rowreq_ctx->undo->VlanRowStatus,
                                         rowreq_ctx->data.VlanRowStatus );
        if (MFD_SUCCESS != rc)
            return rc;
        
        /*
         * row creation requirements
         */
        if (rowreq_ctx->rowreq_flags & MFD_ROW_CREATED) {
            if (ROWSTATUS_DESTROY ==  rowreq_ctx->data.VlanRowStatus) {
                rowreq_ctx->rowreq_flags |= MFD_ROW_DELETED;
            }
            else if (ROWSTATUS_CREATEANDGO ==  rowreq_ctx->data.VlanRowStatus) {
                if ((rowreq_ctx->column_set_flags & DOT11SSIDVLANTABLE_REQUIRED_COLS)
                    != DOT11SSIDVLANTABLE_REQUIRED_COLS) {
                    DEBUGMSGTL(("dot11SSIDVlanTable",
                                "required columns missing (0x%0x != 0x%0x)\n",
                                rowreq_ctx->column_set_flags, DOT11SSIDVLANTABLE_REQUIRED_COLS));
                    return MFD_CANNOT_CREATE_NOW;
                }
                rowreq_ctx->data.VlanRowStatus = ROWSTATUS_ACTIVE;
            }
        } /* row creation */
        else {
            /*
             * row change requirements
             */
            /*
             * don't allow a destroy if any other value was changed, since
             * that might call data access routines with bad info.
             *
             * you may or may not require the row be notInService before it
             * can be destroyed.
             */
            if (ROWSTATUS_DESTROY == rowreq_ctx->data.VlanRowStatus) {
                if (rowreq_ctx->column_set_flags & ~COLUMN_VLANROWSTATUS_FLAG) {
                    DEBUGMSGTL(("dot11SSIDVlanTable",
                                "destroy must be only varbind for row\n"));
                    return MFD_NOT_VALID_NOW;
                }
                rowreq_ctx->rowreq_flags |= MFD_ROW_DELETED;

            } /* row destroy */
        } /* row change */
    }
    else {
        /*
         * must have row status to create a row
         */
        if (rowreq_ctx->rowreq_flags & MFD_ROW_CREATED) {
            DEBUGMSGTL(("dot11SSIDVlanTable",
                        "must use RowStatus to create rows\n"));
            return MFD_CANNOT_CREATE_NOW;
        }
    } /* row status not set */

    if ( MFD_SUCCESS != rc )
        return rc;

    return rc;
} /* dot11SSIDVlanTable_check_dependencies */

/** @} */
