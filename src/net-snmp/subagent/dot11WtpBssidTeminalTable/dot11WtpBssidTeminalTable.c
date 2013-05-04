/*
 * Note: this file originally auto-generated by mib2c using
 *       version : 1.32.2.3 $ of : mfd-top.m2c,v $ 
 *
 * $Id:$
 */
/** \mainpage MFD helper for dot11WtpBssidTeminalTable
 *
 * \section intro Introduction
 * Introductory text.
 *
 */
/* standard Net-SNMP includes */
#include <net-snmp/net-snmp-config.h>
#include <net-snmp/net-snmp-includes.h>
#include <net-snmp/agent/net-snmp-agent-includes.h>

#include "mibs_public.h"
/* include our parent header */
#include "dot11WtpBssidTeminalTable.h"

#include <net-snmp/agent/mib_modules.h>

#include "dot11WtpBssidTeminalTable_interface.h"

#define WTPBSSIDTABLE "1.17.3"

oid dot11WtpBssidTeminalTable_oid[128] = { 0 };
int dot11WtpBssidTeminalTable_oid_size = 0;

void initialize_table_dot11WtpBssidTeminalTable(void);


/**
 * Initializes the dot11WtpBssidTeminalTable module
 */
void
init_dot11WtpBssidTeminalTable(void)
{
    DEBUGMSGTL(("verbose:dot11WtpBssidTeminalTable:init_dot11WtpBssidTeminalTable","called\n"));

    /*
     * TODO:300:o: Perform dot11WtpBssidTeminalTable one-time module initialization.
     */
    /*
     * here we initialize all the tables we're planning on supporting
     */
    if (should_init("dot11WtpBssidTeminalTable"))
        initialize_table_dot11WtpBssidTeminalTable();

} /* init_dot11WtpBssidTeminalTable */

/**
 * Initialize the table dot11WtpBssidTeminalTable 
 *    (Define its contents and how it's structured)
 */
void
initialize_table_dot11WtpBssidTeminalTable(void)
{
    dot11WtpBssidTeminalTable_registration_ptr user_context;
    u_long flags;

    DEBUGMSGTL(("verbose:dot11WtpBssidTeminalTable:initialize_table_dot11WtpBssidTeminalTable","called\n"));

    /*
     * TODO:301:o: Perform dot11WtpBssidTeminalTable one-time table initialization.
     */

    /*
     * TODO:302:o: |->Initialize dot11WtpBssidTeminalTable user context
     * if you'd like to pass in a pointer to some data for this
     * table, allocate or set it up here.
     */
    /*
     * a netsnmp_data_list is a simple way to store void pointers. A simple
     * string token is used to add, find or remove pointers.
     */
    user_context = netsnmp_create_data_list("dot11WtpBssidTeminalTable", NULL, NULL);
    
    /*
     * No support for any flags yet, but in the future you would
     * set any flags here.
     */
    flags = 0;
    mad_dev_oid(dot11WtpBssidTeminalTable_oid,WTPBSSIDTABLE,&dot11WtpBssidTeminalTable_oid_size,enterprise_pvivate_oid);
    /*
     * call interface initialization code
     */
    _dot11WtpBssidTeminalTable_initialize_interface(user_context, flags);
} /* initialize_table_dot11WtpBssidTeminalTable */

/**
 * pre-request callback
 *
 *
 * @retval MFD_SUCCESS              : success.
 * @retval MFD_ERROR                : other error
 */
int
dot11WtpBssidTeminalTable_pre_request(dot11WtpBssidTeminalTable_registration_ptr user_context)
{
    DEBUGMSGTL(("verbose:dot11WtpBssidTeminalTable:dot11WtpBssidTeminalTable_pre_request","called\n"));

    /*
     * TODO:510:o: Perform dot11WtpBssidTeminalTable pre-request actions.
     */

    return MFD_SUCCESS;
} /* dot11WtpBssidTeminalTable_pre_request */

/**
 * post-request callback
 *
 *
 * @retval MFD_SUCCESS : success.
 * @retval MFD_ERROR   : other error (ignored)
 */
int
dot11WtpBssidTeminalTable_post_request(dot11WtpBssidTeminalTable_registration_ptr user_context)
{
    DEBUGMSGTL(("verbose:dot11WtpBssidTeminalTable:dot11WtpBssidTeminalTable_post_request","called\n"));

    /*
     * TODO:511:o: Perform dot11WtpBssidTeminalTable pos-request actions.
     */

    return MFD_SUCCESS;
} /* dot11WtpBssidTeminalTable_post_request */


/** @{ */
