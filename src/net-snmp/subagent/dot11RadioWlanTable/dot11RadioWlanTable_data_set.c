/*
 * Note: this file originally auto-generated by mib2c using
 *       version : 1.18 $ of : mfd-data-set.m2c,v $
 *
 * $Id:$
 *
 */
/* standard Net-SNMP includes */
#include <net-snmp/net-snmp-config.h>
#include <net-snmp/net-snmp-includes.h>
#include <net-snmp/agent/net-snmp-agent-includes.h>

/* include our parent header */
#include "dot11RadioWlanTable.h"


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
 *** Table dot11RadioWlanTable
 ***
 **********************************************************************
 **********************************************************************/
/*
 * dot11RadioWlanTable is subid 5 of wtpRadio.
 * Its status is Current.
 * OID: .1.3.6.1.4.1.31656.6.1.1.4.5, length: 12
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
     *                        +==============+
     *       +----------------||  object    ||
     *       |              E ||  lookup    ||
     *       |                +==============+
     *       |                       | S
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
     *                        +--------------+
     *                        |    post      |
     *                        |   request    |
     *                        +--------------+
     *
     */

/**
 * Setup up context with information needed to undo a set request.
 *
 * This function will be called before the individual node undo setup
 * functions are called. If you need to do any undo setup that is not
 * related to a specific column, you can do it here.
 *
 * Note that an individual node's undo_setup function will only be called
 * if that node is being set to a new value.
 *
 * If there is any setup specific to a particular column (e.g. allocating
 * memory for a string), you should do that setup in the node's undo_setup
 * function, so it won't be done unless it is necessary.
 *
 * @param rowreq_ctx
 *        Pointer to the table context (dot11RadioWlanTable_rowreq_ctx)
 *
 * @retval MFD_SUCCESS : success
 * @retval MFD_ERROR   : error. set will fail.
 */
int
dot11RadioWlanTable_undo_setup( dot11RadioWlanTable_rowreq_ctx *rowreq_ctx)
{
    int rc = MFD_SUCCESS;

    DEBUGMSGTL(("verbose:dot11RadioWlanTable:dot11RadioWlanTable_undo_setup","called\n"));

    /** we should have a non-NULL pointer */
    netsnmp_assert( NULL != rowreq_ctx );

    /*
     * TODO:451:M: |-> Setup dot11RadioWlanTable undo.
     * set up dot11RadioWlanTable undo information, in preparation for a set.
     */

    return rc;
} /* dot11RadioWlanTable_undo_setup */

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
 *        Pointer to the table context (dot11RadioWlanTable_rowreq_ctx)
 *
 * @retval MFD_SUCCESS : success
 * @retval MFD_ERROR   : error
 */
int
dot11RadioWlanTable_undo_cleanup( dot11RadioWlanTable_rowreq_ctx *rowreq_ctx)
{
    int rc = MFD_SUCCESS;

    DEBUGMSGTL(("verbose:dot11RadioWlanTable:dot11RadioWlanTable_undo_cleanup","called\n"));

    /** we should have a non-NULL pointer */
    netsnmp_assert( NULL != rowreq_ctx );

    /*
     * TODO:452:M: |-> Cleanup dot11RadioWlanTable undo.
     */

    return rc;
} /* dot11RadioWlanTable_undo_cleanup */

/**
 * commit new values.
 *
 * At this point, you should have done everything you can to ensure that
 * this commit will not fail.
 *
 * Should you need different behavior depending on which columns were
 * set, rowreq_ctx->column_set_flags will indicate which writeable columns were
 * set. The definitions for the FLAG_* bits can be found in
 * dot11RadioWlanTable_oids.h.
 * A new row will have the MFD_ROW_CREATED bit set in rowreq_flags.
 *
 * @param dot11RadioWlanTable_rowreq_ctx
 *        Pointer to the users context.
 *
 * @retval MFD_SUCCESS : success
 * @retval MFD_ERROR   : error
 */
int
dot11RadioWlanTable_commit( dot11RadioWlanTable_rowreq_ctx *rowreq_ctx)
{
    int rc = MFD_SUCCESS;
    int             save_flags;

    DEBUGMSGTL(("verbose:dot11RadioWlanTable:dot11RadioWlanTable_commit","called\n"));

    /** we should have a non-NULL pointer */
    netsnmp_assert( NULL != rowreq_ctx );

    /*
     * save flags, then clear until we actually do something
     */
    save_flags = rowreq_ctx->column_set_flags;
    rowreq_ctx->column_set_flags = 0;

    /*
     * commit dot11RadioWlanTable data
     * 1) check the column's flag in save_flags to see if it was set.
     * 2) clear the flag when you handle that column
     * 3) set the column's flag in column_set_flags if it needs undo
     *    processing in case of a failure.
     */
    if (save_flags & FLAG_TRAFFICLIMITSTATE) {
       save_flags &= ~FLAG_TRAFFICLIMITSTATE; /* clear TrafficLimitState */
       /*
        * TODO:482:o: |-> commit column TrafficLimitState.
        */
       //rc = -1;
       if(-1 == rc) {
           snmp_log(LOG_ERR,"dot11RadioWlanTable column TrafficLimitState commit failed\n");
       }
       else {
            /*
             * set flag, in case we need to undo TrafficLimitState
             */
            rowreq_ctx->column_set_flags |= FLAG_TRAFFICLIMITSTATE;
       }
    }

    if (save_flags & FLAG_TRAFFICLIMITVALUE) {
       save_flags &= ~FLAG_TRAFFICLIMITVALUE; /* clear TrafficLimitValue */
       /*
        * TODO:482:o: |-> commit column TrafficLimitValue.
        */
       //rc = -1;
       if(-1 == rc) {
           snmp_log(LOG_ERR,"dot11RadioWlanTable column TrafficLimitValue commit failed\n");
       }
       else {
            /*
             * set flag, in case we need to undo TrafficLimitValue
             */
            rowreq_ctx->column_set_flags |= FLAG_TRAFFICLIMITVALUE;
       }
    }

    if (save_flags & FLAG_ISONLYALLOW11NSTAACCESS) {
       save_flags &= ~FLAG_ISONLYALLOW11NSTAACCESS; /* clear IsOnlyAllow11nStaAccess */
       /*
        * TODO:482:o: |-> commit column IsOnlyAllow11nStaAccess.
        */
       //rc = -1;
       if(-1 == rc) {
           snmp_log(LOG_ERR,"dot11RadioWlanTable column IsOnlyAllow11nStaAccess commit failed\n");
       }
       else {
            /*
             * set flag, in case we need to undo IsOnlyAllow11nStaAccess
             */
            rowreq_ctx->column_set_flags |= FLAG_ISONLYALLOW11NSTAACCESS;
       }
    }

    if (save_flags) {
       snmp_log(LOG_ERR, "unhandled columns (0x%x) in commit\n", save_flags);
       return MFD_ERROR;
    }

    return rc;
} /* dot11RadioWlanTable_commit */

/**
 * undo commit new values.
 *
 * Should you need different behavior depending on which columns were
 * set, rowreq_ctx->column_set_flags will indicate which writeable columns were
 * set. The definitions for the FLAG_* bits can be found in
 * dot11RadioWlanTable_oids.h.
 * A new row will have the MFD_ROW_CREATED bit set in rowreq_flags.
 *
 * @param dot11RadioWlanTable_rowreq_ctx
 *        Pointer to the users context.
 *
 * @retval MFD_SUCCESS : success
 * @retval MFD_ERROR   : error
 */
int
dot11RadioWlanTable_undo_commit( dot11RadioWlanTable_rowreq_ctx *rowreq_ctx)
{
    int rc = MFD_SUCCESS;

    DEBUGMSGTL(("verbose:dot11RadioWlanTable:dot11RadioWlanTable_undo_commit","called\n"));

    /** we should have a non-NULL pointer */
    netsnmp_assert( NULL != rowreq_ctx );

    /*
     * TODO:485:M: |-> Undo dot11RadioWlanTable commit.
     * check the column's flag in rowreq_ctx->column_set_flags to see
     * if it was set during commit, then undo it.
     *
     * eg: if (rowreq_ctx->column_set_flags & FLAG_) {}
     */

    return rc;
} /* dot11RadioWlanTable_undo_commit */

/*
 * TODO:420:r: Implement dot11RadioWlanTable index validation.
 */
/*---------------------------------------------------------------------
 * DOT11-WTP-MIB::dot11RadioStatsEntry.wtpRadCurrID
 * wtpRadCurrID is subid 1 of dot11RadioStatsEntry.
 * Its status is Current, and its access level is ReadOnly.
 * OID: .1.3.6.1.4.1.31656.6.1.1.4.1.1.1
 * Description:
Current radio ID.
 *
 * Attributes:
 *   accessible 1     isscalar 0     enums  0      hasdefval 0
 *   readable   1     iscolumn 1     ranges 0      hashint   0
 *   settable   0
 *
 *
 * Its syntax is INTEGER (based on perltype INTEGER)
 * The net-snmp type is ASN_INTEGER. The C type decl is long (long)
 */
/*
 * TODO:440:M: Implement dot11RadioWlanTable node value checks.
 * TODO:450:M: Implement dot11RadioWlanTable undo functions.
 * TODO:460:M: Implement dot11RadioWlanTable set functions.
 * TODO:480:M: Implement dot11RadioWlanTable commit functions.
 */
/*---------------------------------------------------------------------
 * DOT11-WTP-MIB::dot11RadioWlanEntry.TrafficLimitState
 * TrafficLimitState is subid 2 of dot11RadioWlanEntry.
 * Its status is Current, and its access level is ReadWrite.
 * OID: .1.3.6.1.4.1.31656.6.1.1.4.5.1.2
 * Description:
Traffic limit state.
 *
 * Attributes:
 *   accessible 1     isscalar 0     enums  1      hasdefval 0
 *   readable   1     iscolumn 1     ranges 0      hashint   0
 *   settable   1
 *
 * Enum range: 2/8. Values:  enable(1), disable(2)
 *
 * Its syntax is INTEGER (based on perltype INTEGER)
 * The net-snmp type is ASN_INTEGER. The C type decl is long (u_long)
 */
/**
 * Check that the proposed new value is potentially valid.
 *
 * @param rowreq_ctx
 *        Pointer to the row request context.
 * @param TrafficLimitState_val
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
 * dot11RadioWlanTable_check_dependencies() function.
 *
 * The following checks have already been done for you:
 *    The syntax is ASN_INTEGER
 *    The value is one of  enable(1), disable(2)
 *
 * If there a no other checks you need to do, simply return MFD_SUCCESS.
 *
 */
int
TrafficLimitState_check_value( dot11RadioWlanTable_rowreq_ctx *rowreq_ctx, u_long TrafficLimitState_val)
{
    DEBUGMSGTL(("verbose:dot11RadioWlanTable:TrafficLimitState_check_value","called\n"));

    /** should never get a NULL pointer */
    netsnmp_assert(NULL != rowreq_ctx);

    /*
     * TODO:441:o: |-> Check for valid TrafficLimitState value.
     */

    return MFD_SUCCESS; /* TrafficLimitState value not illegal */
} /* TrafficLimitState_check_value */

/**
 * Save old value information
 *
 * @param rowreq_ctx
 *        Pointer to the table context (dot11RadioWlanTable_rowreq_ctx)
 *
 * @retval MFD_SUCCESS : success
 * @retval MFD_ERROR   : error. set will fail.
 *
 * This function will be called after the table level undo setup function
 * dot11RadioWlanTable_undo_setup has been called.
 *
 *@note
 * this function will only be called if a new value is set for this column.
 *
 * If there is any setup specific to a particular column (e.g. allocating
 * memory for a string), you should do that setup in this function, so it
 * won't be done unless it is necessary.
 */
int
TrafficLimitState_undo_setup( dot11RadioWlanTable_rowreq_ctx *rowreq_ctx)
{
    DEBUGMSGTL(("verbose:dot11RadioWlanTable:TrafficLimitState_undo_setup","called\n"));

    /** should never get a NULL pointer */
    netsnmp_assert(NULL != rowreq_ctx);

    /*
     * TODO:455:o: |-> Setup TrafficLimitState undo.
     */
    /*
     * copy TrafficLimitState data
     * set rowreq_ctx->undo->TrafficLimitState from rowreq_ctx->data.TrafficLimitState
     */
    rowreq_ctx->undo->TrafficLimitState = rowreq_ctx->data.TrafficLimitState;


    return MFD_SUCCESS;
} /* TrafficLimitState_undo_setup */

/**
 * Set the new value.
 *
 * @param rowreq_ctx
 *        Pointer to the users context. You should know how to
 *        manipulate the value from this object.
 * @param TrafficLimitState_val
 *        A long containing the new value.
 */
int
TrafficLimitState_set( dot11RadioWlanTable_rowreq_ctx *rowreq_ctx, u_long TrafficLimitState_val )
{

    DEBUGMSGTL(("verbose:dot11RadioWlanTable:TrafficLimitState_set","called\n"));

    /** should never get a NULL pointer */
    netsnmp_assert(NULL != rowreq_ctx);

/*
 * TODO:245:o: |-> Implement TrafficLimitState reverse mapping.
 * If the values for your data type don't exactly match the
 * possible values defined by the mib, you should map them here.
 */
    /*
    ***************************************************
    ***             START EXAMPLE CODE              ***
    ***---------------------------------------------***/
    switch(TrafficLimitState_val) {
        case TRAFFICLIMITSTATE_ENABLE:
             TrafficLimitState_val = INTERNAL_TRAFFICLIMITSTATE_ENABLE;
             break;

        case TRAFFICLIMITSTATE_DISABLE:
             TrafficLimitState_val = INTERNAL_TRAFFICLIMITSTATE_DISABLE;
             break;

             default:
                 snmp_log(LOG_ERR, "couldn't reverse map value %ld for TrafficLimitState\n", TrafficLimitState_val );
                 return SNMP_ERR_GENERR;
    }
    /*
    ***---------------------------------------------***
    ***              END  EXAMPLE CODE              ***
    ***************************************************/

    /*
     * TODO:461:M: |-> Set TrafficLimitState value.
     * set TrafficLimitState value in rowreq_ctx->data
     */    
	void *connection = NULL;
	if(SNMPD_DBUS_ERROR == get_instance_dbus_connection(rowreq_ctx->data.parameter, &connection, SNMPD_INSTANCE_MASTER_V3))
		return MFD_ERROR;
			
    int rc = MFD_ERROR;
	int ret = 0;
	char wlanid[10] = { 0 };
	memset(wlanid,0,10);
	char state[10] = { 0 };
	memset(state,0,10);
	snprintf(wlanid,sizeof(wlanid)-1,"%d",rowreq_ctx->data.localBindWlanID);
	int flag = 1;

	if(TrafficLimitState_val == TRAFFICLIMITSTATE_ENABLE)
	{
		memset(state,0,10);
		strncpy(state,"enable",sizeof(state)-1);
	}
	else if(TrafficLimitState_val == TRAFFICLIMITSTATE_DISABLE)
	{
		memset(state,0,10);
		strncpy(state,"disable",sizeof(state)-1);
	}
	else
	{	
		flag = 0;
		rc = MFD_ERROR;		
	}
		
    if(flag == 1)
    {
		ret=radio_bss_traffic_limit_cmd(rowreq_ctx->data.parameter,connection,rowreq_ctx->data.localwtpRadCurrID,wlanid,state);
		if(ret==1)
		{
			rowreq_ctx->data.TrafficLimitState = TrafficLimitState_val;			
			rc = MFD_SUCCESS;			
		}
		else
		{
			if(SNMPD_CONNECTION_ERROR == ret) {
				close_slot_dbus_connection(rowreq_ctx->data.parameter.slot_id);
			}
			
			rc = MFD_ERROR;		
		}
    }	

    return MFD_SUCCESS;
} /* TrafficLimitState_set */

/**
 * undo the previous set.
 *
 * @param rowreq_ctx
 *        Pointer to the users context.
 */
int
TrafficLimitState_undo( dot11RadioWlanTable_rowreq_ctx *rowreq_ctx)
{

    DEBUGMSGTL(("verbose:dot11RadioWlanTable:TrafficLimitState_undo","called\n"));

    netsnmp_assert(NULL != rowreq_ctx);

    /*
     * TODO:456:o: |-> Clean up TrafficLimitState undo.
     */
    /*
     * copy TrafficLimitState data
     * set rowreq_ctx->data.TrafficLimitState from rowreq_ctx->undo->TrafficLimitState
     */
    rowreq_ctx->data.TrafficLimitState = rowreq_ctx->undo->TrafficLimitState;

    
    return MFD_SUCCESS;
} /* TrafficLimitState_undo */

/*---------------------------------------------------------------------
 * DOT11-WTP-MIB::dot11RadioWlanEntry.TrafficLimitValue
 * TrafficLimitValue is subid 3 of dot11RadioWlanEntry.
 * Its status is Current, and its access level is ReadWrite.
 * OID: .1.3.6.1.4.1.31656.6.1.1.4.5.1.3
 * Description:
Traffic limit value.
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
 * @param TrafficLimitValue_val
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
 * dot11RadioWlanTable_check_dependencies() function.
 *
 * The following checks have already been done for you:
 *    The syntax is ASN_INTEGER
 *
 * If there a no other checks you need to do, simply return MFD_SUCCESS.
 *
 */
int
TrafficLimitValue_check_value( dot11RadioWlanTable_rowreq_ctx *rowreq_ctx, long TrafficLimitValue_val)
{
    DEBUGMSGTL(("verbose:dot11RadioWlanTable:TrafficLimitValue_check_value","called\n"));

    /** should never get a NULL pointer */
    netsnmp_assert(NULL != rowreq_ctx);

    /*
     * TODO:441:o: |-> Check for valid TrafficLimitValue value.
     */

    return MFD_SUCCESS; /* TrafficLimitValue value not illegal */
} /* TrafficLimitValue_check_value */

/**
 * Save old value information
 *
 * @param rowreq_ctx
 *        Pointer to the table context (dot11RadioWlanTable_rowreq_ctx)
 *
 * @retval MFD_SUCCESS : success
 * @retval MFD_ERROR   : error. set will fail.
 *
 * This function will be called after the table level undo setup function
 * dot11RadioWlanTable_undo_setup has been called.
 *
 *@note
 * this function will only be called if a new value is set for this column.
 *
 * If there is any setup specific to a particular column (e.g. allocating
 * memory for a string), you should do that setup in this function, so it
 * won't be done unless it is necessary.
 */
int
TrafficLimitValue_undo_setup( dot11RadioWlanTable_rowreq_ctx *rowreq_ctx)
{
    DEBUGMSGTL(("verbose:dot11RadioWlanTable:TrafficLimitValue_undo_setup","called\n"));

    /** should never get a NULL pointer */
    netsnmp_assert(NULL != rowreq_ctx);

    /*
     * TODO:455:o: |-> Setup TrafficLimitValue undo.
     */
    /*
     * copy TrafficLimitValue data
     * set rowreq_ctx->undo->TrafficLimitValue from rowreq_ctx->data.TrafficLimitValue
     */
    rowreq_ctx->undo->TrafficLimitValue = rowreq_ctx->data.TrafficLimitValue;


    return MFD_SUCCESS;
} /* TrafficLimitValue_undo_setup */

/**
 * Set the new value.
 *
 * @param rowreq_ctx
 *        Pointer to the users context. You should know how to
 *        manipulate the value from this object.
 * @param TrafficLimitValue_val
 *        A long containing the new value.
 */
int
TrafficLimitValue_set( dot11RadioWlanTable_rowreq_ctx *rowreq_ctx, long TrafficLimitValue_val )
{

    DEBUGMSGTL(("verbose:dot11RadioWlanTable:TrafficLimitValue_set","called\n"));

    /** should never get a NULL pointer */
    netsnmp_assert(NULL != rowreq_ctx);

/*
 * TODO:245:o: |-> Implement TrafficLimitValue reverse mapping.
 * If the values for your data type don't exactly match the
 * possible values defined by the mib, you should map them here.
 */
    /*
     * TODO:461:M: |-> Set TrafficLimitValue value.
     * set TrafficLimitValue value in rowreq_ctx->data
     */
    void *connection = NULL;
	if(SNMPD_DBUS_ERROR == get_instance_dbus_connection(rowreq_ctx->data.parameter, &connection, SNMPD_INSTANCE_MASTER_V3))
		return MFD_ERROR;
	
    int rc = MFD_ERROR;
	int ret = 0;
	char wlanid[10] = { 0 };
	memset(wlanid,0,10);
	char value[10] = { 0 };
	memset(value,0,10);
	snprintf(wlanid,sizeof(wlanid)-1,"%d",rowreq_ctx->data.localBindWlanID);
	snprintf(value,sizeof(value)-1,"%d",TrafficLimitValue_val);
		
	ret=radio_bss_traffic_limit_send_value_cmd(rowreq_ctx->data.parameter,connection,rowreq_ctx->data.localwtpRadCurrID,wlanid,value);
	if(ret==1)
	{
		rowreq_ctx->data.TrafficLimitValue = TrafficLimitValue_val;
		rc = MFD_SUCCESS;			
	}
	else
	{
		if(SNMPD_CONNECTION_ERROR == ret) {
				close_slot_dbus_connection(rowreq_ctx->data.parameter.slot_id);
			}
			
		rc = MFD_ERROR;		
	}

    return MFD_SUCCESS;
} /* TrafficLimitValue_set */

/**
 * undo the previous set.
 *
 * @param rowreq_ctx
 *        Pointer to the users context.
 */
int
TrafficLimitValue_undo( dot11RadioWlanTable_rowreq_ctx *rowreq_ctx)
{

    DEBUGMSGTL(("verbose:dot11RadioWlanTable:TrafficLimitValue_undo","called\n"));

    netsnmp_assert(NULL != rowreq_ctx);

    /*
     * TODO:456:o: |-> Clean up TrafficLimitValue undo.
     */
    /*
     * copy TrafficLimitValue data
     * set rowreq_ctx->data.TrafficLimitValue from rowreq_ctx->undo->TrafficLimitValue
     */
    rowreq_ctx->data.TrafficLimitValue = rowreq_ctx->undo->TrafficLimitValue;

    
    return MFD_SUCCESS;
} /* TrafficLimitValue_undo */

/*---------------------------------------------------------------------
 * DOT11-WTP-MIB::dot11RadioWlanEntry.IsOnlyAllow11nStaAccess
 * IsOnlyAllow11nStaAccess is subid 4 of dot11RadioWlanEntry.
 * Its status is Current, and its access level is ReadWrite.
 * OID: .1.3.6.1.4.1.31656.6.1.1.4.5.1.4
 * Description:
Whether only allows 11N terminal access.
 *
 * Attributes:
 *   accessible 1     isscalar 0     enums  1      hasdefval 0
 *   readable   1     iscolumn 1     ranges 0      hashint   0
 *   settable   1
 *
 * Enum range: 2/8. Values:  true(1), false(2)
 *
 * Its syntax is TruthValue (based on perltype INTEGER)
 * The net-snmp type is ASN_INTEGER. The C type decl is long (u_long)
 */
/**
 * Check that the proposed new value is potentially valid.
 *
 * @param rowreq_ctx
 *        Pointer to the row request context.
 * @param IsOnlyAllow11nStaAccess_val
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
 * dot11RadioWlanTable_check_dependencies() function.
 *
 * The following checks have already been done for you:
 *    The syntax is ASN_INTEGER
 *    The value is one of  true(1), false(2)
 *
 * If there a no other checks you need to do, simply return MFD_SUCCESS.
 *
 */
int
IsOnlyAllow11nStaAccess_check_value( dot11RadioWlanTable_rowreq_ctx *rowreq_ctx, u_long IsOnlyAllow11nStaAccess_val)
{
    DEBUGMSGTL(("verbose:dot11RadioWlanTable:IsOnlyAllow11nStaAccess_check_value","called\n"));

    /** should never get a NULL pointer */
    netsnmp_assert(NULL != rowreq_ctx);

    /*
     * TODO:441:o: |-> Check for valid IsOnlyAllow11nStaAccess value.
     */

    return MFD_SUCCESS; /* IsOnlyAllow11nStaAccess value not illegal */
} /* IsOnlyAllow11nStaAccess_check_value */

/**
 * Save old value information
 *
 * @param rowreq_ctx
 *        Pointer to the table context (dot11RadioWlanTable_rowreq_ctx)
 *
 * @retval MFD_SUCCESS : success
 * @retval MFD_ERROR   : error. set will fail.
 *
 * This function will be called after the table level undo setup function
 * dot11RadioWlanTable_undo_setup has been called.
 *
 *@note
 * this function will only be called if a new value is set for this column.
 *
 * If there is any setup specific to a particular column (e.g. allocating
 * memory for a string), you should do that setup in this function, so it
 * won't be done unless it is necessary.
 */
int
IsOnlyAllow11nStaAccess_undo_setup( dot11RadioWlanTable_rowreq_ctx *rowreq_ctx)
{
    DEBUGMSGTL(("verbose:dot11RadioWlanTable:IsOnlyAllow11nStaAccess_undo_setup","called\n"));

    /** should never get a NULL pointer */
    netsnmp_assert(NULL != rowreq_ctx);

    /*
     * TODO:455:o: |-> Setup IsOnlyAllow11nStaAccess undo.
     */
    /*
     * copy IsOnlyAllow11nStaAccess data
     * set rowreq_ctx->undo->IsOnlyAllow11nStaAccess from rowreq_ctx->data.IsOnlyAllow11nStaAccess
     */
    rowreq_ctx->undo->IsOnlyAllow11nStaAccess = rowreq_ctx->data.IsOnlyAllow11nStaAccess;


    return MFD_SUCCESS;
} /* IsOnlyAllow11nStaAccess_undo_setup */

/**
 * Set the new value.
 *
 * @param rowreq_ctx
 *        Pointer to the users context. You should know how to
 *        manipulate the value from this object.
 * @param IsOnlyAllow11nStaAccess_val
 *        A long containing the new value.
 */
int
IsOnlyAllow11nStaAccess_set( dot11RadioWlanTable_rowreq_ctx *rowreq_ctx, u_long IsOnlyAllow11nStaAccess_val )
{

    DEBUGMSGTL(("verbose:dot11RadioWlanTable:IsOnlyAllow11nStaAccess_set","called\n"));

    /** should never get a NULL pointer */
    netsnmp_assert(NULL != rowreq_ctx);

/*
 * TODO:245:o: |-> Implement IsOnlyAllow11nStaAccess reverse mapping.
 * If the values for your data type don't exactly match the
 * possible values defined by the mib, you should map them here.
 */
    /*
    ***************************************************
    ***             START EXAMPLE CODE              ***
    ***---------------------------------------------***/
    switch(IsOnlyAllow11nStaAccess_val) {
        case TRUTHVALUE_TRUE:
             IsOnlyAllow11nStaAccess_val = INTERNAL_ISONLYALLOW11NSTAACCESS_TRUE;
             break;

        case TRUTHVALUE_FALSE:
             IsOnlyAllow11nStaAccess_val = INTERNAL_ISONLYALLOW11NSTAACCESS_FALSE;
             break;

             default:
                 snmp_log(LOG_ERR, "couldn't reverse map value %ld for IsOnlyAllow11nStaAccess\n", IsOnlyAllow11nStaAccess_val );
                 return SNMP_ERR_GENERR;
    }
    /*
    ***---------------------------------------------***
    ***              END  EXAMPLE CODE              ***
    ***************************************************/

    /*
     * TODO:461:M: |-> Set IsOnlyAllow11nStaAccess value.
     * set IsOnlyAllow11nStaAccess value in rowreq_ctx->data
     */    
    void *connection = NULL;
	if(SNMPD_DBUS_ERROR == get_instance_dbus_connection(rowreq_ctx->data.parameter, &connection, SNMPD_INSTANCE_MASTER_V3))
		return MFD_ERROR;
	
	int rc = MFD_ERROR;
	int ret = 0;
	char wlanid[10] = { 0 };
	memset(wlanid,0,10);
	char workmode[10] = { 0 };
	snprintf(wlanid,sizeof(wlanid)-1,"%d",rowreq_ctx->data.localBindWlanID);
	int flag = 1;

	memset(workmode,0,10);
	if(IsOnlyAllow11nStaAccess_val == INTERNAL_TRAFFICLIMITSTATE_ENABLE)
	{
		strncpy(workmode,"puren",sizeof(workmode)-1);
	}
	else if(IsOnlyAllow11nStaAccess_val == INTERNAL_ISONLYALLOW11NSTAACCESS_FALSE)
	{
		strncpy(workmode,"mixed",sizeof(workmode)-1);
	}
	else
	{	
		flag = 0;
		rc = MFD_ERROR; 	
	}
		
	if(flag == 1)
	{
		ret = set_radio_11n_puren_mixed_cmd(rowreq_ctx->data.parameter,connection,rowreq_ctx->data.localwtpRadCurrID,wlanid,workmode);
		if(ret==1)
		{
			rowreq_ctx->data.IsOnlyAllow11nStaAccess = IsOnlyAllow11nStaAccess_val;
			rc = MFD_SUCCESS;			
		}
		else
		{
			if(SNMPD_CONNECTION_ERROR == ret) {
				close_slot_dbus_connection(rowreq_ctx->data.parameter.slot_id);
			}
			
			rc = MFD_ERROR; 	
		}
	}	

    return MFD_SUCCESS;
} /* IsOnlyAllow11nStaAccess_set */

/**
 * undo the previous set.
 *
 * @param rowreq_ctx
 *        Pointer to the users context.
 */
int
IsOnlyAllow11nStaAccess_undo( dot11RadioWlanTable_rowreq_ctx *rowreq_ctx)
{

    DEBUGMSGTL(("verbose:dot11RadioWlanTable:IsOnlyAllow11nStaAccess_undo","called\n"));

    netsnmp_assert(NULL != rowreq_ctx);

    /*
     * TODO:456:o: |-> Clean up IsOnlyAllow11nStaAccess undo.
     */
    /*
     * copy IsOnlyAllow11nStaAccess data
     * set rowreq_ctx->data.IsOnlyAllow11nStaAccess from rowreq_ctx->undo->IsOnlyAllow11nStaAccess
     */
    rowreq_ctx->data.IsOnlyAllow11nStaAccess = rowreq_ctx->undo->IsOnlyAllow11nStaAccess;

    
    return MFD_SUCCESS;
} /* IsOnlyAllow11nStaAccess_undo */

/**
 * check dependencies
 *
 * This is useful for for tables which have dependencies between columns
 * (or rows, or tables). For example, two columns allocating a percentage
 * of something add up 100%.
 *
 * Should you need different behavior depending on which columns were
 * set, rowreq_ctx->column_set_flags will indicate which writeable columns were
 * set. The definitions for the FLAG_* bits can be found in
 * dot11RadioWlanTable_oids.h.
 * A new row will have the MFD_ROW_CREATED bit set in rowreq_flags.
 *
 * @retval MFD_SUCCESS all the changes to the row are legal
 * @retval MFD_ERROR   one or more changes are not legal
 *
 * (see README-table-dot11RadioWlanTable if you don't have dependencies)
 */
int
dot11RadioWlanTable_check_dependencies(dot11RadioWlanTable_rowreq_ctx *rowreq_ctx)
{
    int rc = MFD_SUCCESS;
    
    DEBUGMSGTL(("internal:dot11RadioWlanTable:dot11RadioWlanTable_check_dependencies","called\n"));

    netsnmp_assert(NULL != rowreq_ctx);

    /*
     * TODO:470:o: Check dot11RadioWlanTable row dependencies.
     * check that all new value are legal and consistent with each other
     */
    return rc;
} /* dot11RadioWlanTable_check_dependencies */

/** @} */
