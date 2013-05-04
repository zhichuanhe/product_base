/*
 * Note: this file originally auto-generated by mib2c using
 *       version : 14170 $ of $ 
 *
 * $Id:$
 */
/** \page MFD helper for dot11PermitBSSIDTable
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
#include "dot11PermitBSSIDTable.h"

#include <net-snmp/agent/mib_modules.h>

#include "dot11PermitBSSIDTable_interface.h"
#include "autelanWtpGroup.h"

#define DOT11PERMITBSSIDTABLE "2.28"

oid dot11PermitBSSIDTable_oid[128] = { 0 };
int dot11PermitBSSIDTable_oid_size = 0;

    dot11PermitBSSIDTable_registration  dot11PermitBSSIDTable_user_context;

void initialize_table_dot11PermitBSSIDTable(void);
void shutdown_table_dot11PermitBSSIDTable(void);


/**
 * Initializes the dot11PermitBSSIDTable module
 */
void
init_dot11PermitBSSIDTable(void)
{
    DEBUGMSGTL(("verbose:dot11PermitBSSIDTable:init_dot11PermitBSSIDTable","called\n"));

    /*
     * TODO:300:o: Perform dot11PermitBSSIDTable one-time module initialization.
     */
     
    /*
     * here we initialize all the tables we're planning on supporting
     */
    if (should_init("dot11PermitBSSIDTable"))
        initialize_table_dot11PermitBSSIDTable();

} /* init_dot11PermitBSSIDTable */

/**
 * Shut-down the dot11PermitBSSIDTable module (agent is exiting)
 */
void
shutdown_dot11PermitBSSIDTable(void)
{
    if (should_init("dot11PermitBSSIDTable"))
        shutdown_table_dot11PermitBSSIDTable();

}

/**
 * Initialize the table dot11PermitBSSIDTable 
 *    (Define its contents and how it's structured)
 */
void
initialize_table_dot11PermitBSSIDTable(void)
{
    dot11PermitBSSIDTable_registration * user_context;
    u_long flags;

    DEBUGMSGTL(("verbose:dot11PermitBSSIDTable:initialize_table_dot11PermitBSSIDTable","called\n"));

    /*
     * TODO:301:o: Perform dot11PermitBSSIDTable one-time table initialization.
     */

    /*
     * TODO:302:o: |->Initialize dot11PermitBSSIDTable user context
     * if you'd like to pass in a pointer to some data for this
     * table, allocate or set it up here.
     */
    /*
     * a netsnmp_data_list is a simple way to store void pointers. A simple
     * string token is used to add, find or remove pointers.
     */
    user_context = netsnmp_create_data_list("dot11PermitBSSIDTable", NULL, NULL);
    
    /*
     * No support for any flags yet, but in the future you would
     * set any flags here.
     */
    flags = 0;
    
    mad_dev_oid(dot11PermitBSSIDTable_oid, DOT11PERMITBSSIDTABLE, &dot11PermitBSSIDTable_oid_size, enterprise_pvivate_oid);
    /*
     * call interface initialization code
     */
    _dot11PermitBSSIDTable_initialize_interface(user_context, flags);
} /* initialize_table_dot11PermitBSSIDTable */

/**
 * Shutdown the table dot11PermitBSSIDTable 
 */
void
shutdown_table_dot11PermitBSSIDTable(void)
{
    /*
     * call interface shutdown code
     */
    _dot11PermitBSSIDTable_shutdown_interface(&dot11PermitBSSIDTable_user_context);
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
dot11PermitBSSIDTable_rowreq_ctx_init(dot11PermitBSSIDTable_rowreq_ctx *rowreq_ctx,
                           void *user_init_ctx)
{
    DEBUGMSGTL(("verbose:dot11PermitBSSIDTable:dot11PermitBSSIDTable_rowreq_ctx_init","called\n"));

    netsnmp_assert(NULL != rowreq_ctx);
    
    /*
     * TODO:210:o: |-> Perform extra dot11PermitBSSIDTable rowreq initialization. (eg DEFVALS)
     */

    return MFD_SUCCESS;
} /* dot11PermitBSSIDTable_rowreq_ctx_init */

/**
 * extra context cleanup
 *
 */
void dot11PermitBSSIDTable_rowreq_ctx_cleanup(dot11PermitBSSIDTable_rowreq_ctx *rowreq_ctx)
{
    DEBUGMSGTL(("verbose:dot11PermitBSSIDTable:dot11PermitBSSIDTable_rowreq_ctx_cleanup","called\n"));

    netsnmp_assert(NULL != rowreq_ctx);
    
    /*
     * TODO:211:o: |-> Perform extra dot11PermitBSSIDTable rowreq cleanup.
     */
} /* dot11PermitBSSIDTable_rowreq_ctx_cleanup */

/************************************************************
 * the *_should_save routine is called to determine if a row
 * should be stored persistently.
 *
 * Note that this is not a 'dirty' check (i.e. if a row has changed),
 * but a check for volatile rows that should not be saved between
 * restarts.
 *
 * return 1 if the row should be stored
 * return 0 if the row should not be stored
 */
int
dot11PermitBSSIDTable_container_should_save(dot11PermitBSSIDTable_rowreq_ctx * rowreq_ctx)
{
    
    return 1; /* save the row */
}

/**
 * pre-request callback
 *
 *
 * @retval MFD_SUCCESS              : success.
 * @retval MFD_ERROR                : other error
 */
int
dot11PermitBSSIDTable_pre_request(dot11PermitBSSIDTable_registration * user_context)
{
    DEBUGMSGTL(("verbose:dot11PermitBSSIDTable:dot11PermitBSSIDTable_pre_request","called\n"));

    /*
     * TODO:510:o: Perform dot11PermitBSSIDTable pre-request actions.
     */

    return MFD_SUCCESS;
} /* dot11PermitBSSIDTable_pre_request */

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
dot11PermitBSSIDTable_post_request(dot11PermitBSSIDTable_registration * user_context, int rc)
{
    DEBUGMSGTL(("verbose:dot11PermitBSSIDTable:dot11PermitBSSIDTable_post_request","called\n"));

    /*
     * TODO:511:o: Perform dot11PermitBSSIDTable post-request actions.
     */

    /*
     * check to set if any rows were changed.
     */
    if (dot11PermitBSSIDTable_dirty_get()) {
        /*
         * check if request was successful. If so, this would be
         * a good place to save data to its persistent store.
         */
        if (MFD_SUCCESS == rc) {
            /*
             * save changed rows, if you haven't already
             */
            snmp_store(netsnmp_ds_get_string(NETSNMP_DS_LIBRARY_ID,
                                             NETSNMP_DS_LIB_APPTYPE));
        }
        
        dot11PermitBSSIDTable_dirty_set(0); /* clear table dirty flag */
    }

    return MFD_SUCCESS;
} /* dot11PermitBSSIDTable_post_request */


/** @{ */
