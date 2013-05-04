/*
 * Note: this file originally auto-generated by mib2c using
 *       version : 14170 $ of $ 
 *
 * $Id:$
 */
/** \page MFD helper for dot11BssStaInfoTable
 *
 * \section intro Introduction
 * Introductory text.
 *
 */
/* standard Net-SNMP includes */
#include <net-snmp/net-snmp-config.h>
#include <net-snmp/net-snmp-includes.h>
#include <net-snmp/agent/net-snmp-agent-includes.h>

/* include our parent header */
#include "dot11BssStaInfoTable.h"

#include <net-snmp/agent/mib_modules.h>

#include "dot11BssStaInfoTable_interface.h"
#include "autelanWtpGroup.h"

#define BSSSTAINFOTABLE "1.8.3"

oid dot11BssStaInfoTable_oid[128] = { 0 };
int dot11BssStaInfoTable_oid_size = 0;

    dot11BssStaInfoTable_registration  dot11BssStaInfoTable_user_context;

void initialize_table_dot11BssStaInfoTable(void);
void shutdown_table_dot11BssStaInfoTable(void);


/**
 * Initializes the dot11BssStaInfoTable module
 */
void
init_dot11BssStaInfoTable(void)
{
    DEBUGMSGTL(("verbose:dot11BssStaInfoTable:init_dot11BssStaInfoTable","called\n"));

    /*
     * TODO:300:o: Perform dot11BssStaInfoTable one-time module initialization.
     */
     
    /*
     * here we initialize all the tables we're planning on supporting
     */
    if (should_init("dot11BssStaInfoTable"))
        initialize_table_dot11BssStaInfoTable();

} /* init_dot11BssStaInfoTable */

/**
 * Shut-down the dot11BssStaInfoTable module (agent is exiting)
 */
void
shutdown_dot11BssStaInfoTable(void)
{
    if (should_init("dot11BssStaInfoTable"))
        shutdown_table_dot11BssStaInfoTable();

}

/**
 * Initialize the table dot11BssStaInfoTable 
 *    (Define its contents and how it's structured)
 */
void
initialize_table_dot11BssStaInfoTable(void)
{
    dot11BssStaInfoTable_registration * user_context;
    u_long flags;

    DEBUGMSGTL(("verbose:dot11BssStaInfoTable:initialize_table_dot11BssStaInfoTable","called\n"));

    /*
     * TODO:301:o: Perform dot11BssStaInfoTable one-time table initialization.
     */

    /*
     * TODO:302:o: |->Initialize dot11BssStaInfoTable user context
     * if you'd like to pass in a pointer to some data for this
     * table, allocate or set it up here.
     */
    /*
     * a netsnmp_data_list is a simple way to store void pointers. A simple
     * string token is used to add, find or remove pointers.
     */
    user_context = netsnmp_create_data_list("dot11BssStaInfoTable", NULL, NULL);
    
    /*
     * No support for any flags yet, but in the future you would
     * set any flags here.
     */
    flags = 0;

    mad_dev_oid(dot11BssStaInfoTable_oid,BSSSTAINFOTABLE,&dot11BssStaInfoTable_oid_size,enterprise_pvivate_oid);
    /*
     * call interface initialization code
     */
    _dot11BssStaInfoTable_initialize_interface(user_context, flags);
} /* initialize_table_dot11BssStaInfoTable */

/**
 * Shutdown the table dot11BssStaInfoTable 
 */
void
shutdown_table_dot11BssStaInfoTable(void)
{
    /*
     * call interface shutdown code
     */
    _dot11BssStaInfoTable_shutdown_interface(&dot11BssStaInfoTable_user_context);
}

/**
 * extra context initialization (eg default values)
 *
 * @param rowreq_ctx    : row request context
 * @param user_init_ctx : void pointer for user (parameter to rowreq_ctx_allocate)
 *
 * @retval MFD_SUCCESS  : no errors
 * @retval MFD_ERROR    : error (context allocate will fail)
 */
int
dot11BssStaInfoTable_rowreq_ctx_init(dot11BssStaInfoTable_rowreq_ctx *rowreq_ctx,
                           void *user_init_ctx)
{
    DEBUGMSGTL(("verbose:dot11BssStaInfoTable:dot11BssStaInfoTable_rowreq_ctx_init","called\n"));

    netsnmp_assert(NULL != rowreq_ctx);
    
    /*
     * TODO:210:o: |-> Perform extra dot11BssStaInfoTable rowreq initialization. (eg DEFVALS)
     */

    return MFD_SUCCESS;
} /* dot11BssStaInfoTable_rowreq_ctx_init */

/**
 * extra context cleanup
 *
 */
void dot11BssStaInfoTable_rowreq_ctx_cleanup(dot11BssStaInfoTable_rowreq_ctx *rowreq_ctx)
{
    DEBUGMSGTL(("verbose:dot11BssStaInfoTable:dot11BssStaInfoTable_rowreq_ctx_cleanup","called\n"));

    netsnmp_assert(NULL != rowreq_ctx);
    
    /*
     * TODO:211:o: |-> Perform extra dot11BssStaInfoTable rowreq cleanup.
     */
} /* dot11BssStaInfoTable_rowreq_ctx_cleanup */

/**
 * pre-request callback
 *
 *
 * @retval MFD_SUCCESS              : success.
 * @retval MFD_ERROR                : other error
 */
int
dot11BssStaInfoTable_pre_request(dot11BssStaInfoTable_registration * user_context)
{
    DEBUGMSGTL(("verbose:dot11BssStaInfoTable:dot11BssStaInfoTable_pre_request","called\n"));

    /*
     * TODO:510:o: Perform dot11BssStaInfoTable pre-request actions.
     */

    return MFD_SUCCESS;
} /* dot11BssStaInfoTable_pre_request */

/**
 * post-request callback
 *
 * Note:
 *   New rows have been inserted into the container, and
 *   deleted rows have been removed from the container and
 *   released.
 *
 * @param user_context
 * @param rc : MFD_SUCCESS if all requests succeeded
 *
 * @retval MFD_SUCCESS : success.
 * @retval MFD_ERROR   : other error (ignored)
 */
int
dot11BssStaInfoTable_post_request(dot11BssStaInfoTable_registration * user_context, int rc)
{
    DEBUGMSGTL(("verbose:dot11BssStaInfoTable:dot11BssStaInfoTable_post_request","called\n"));

    /*
     * TODO:511:o: Perform dot11BssStaInfoTable post-request actions.
     */

    return MFD_SUCCESS;
} /* dot11BssStaInfoTable_post_request */


/** @{ */
