/*
 * Note: this file originally auto-generated by mib2c using
 *       version : 1.32.2.3 $ of : mfd-top.m2c,v $ 
 *
 * $Id:$
 */
/** \mainpage MFD helper for dot11BssWAPIPerformanceStatsTable
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
#include "dot11BssWAPIPerformanceStatsTable.h"

#include <net-snmp/agent/mib_modules.h>

#include "dot11BssWAPIPerformanceStatsTable_interface.h"
#include "autelanWtpGroup.h"

#define BSSWAPIPSOID	"1.10.5"

oid dot11BssWAPIPerformanceStatsTable_oid[128] = { 0 };
int dot11BssWAPIPerformanceStatsTable_oid_size = 0;

void initialize_table_dot11BssWAPIPerformanceStatsTable(void);


/**
 * Initializes the dot11BssWAPIPerformanceStatsTable module
 */
void
init_dot11BssWAPIPerformanceStatsTable(void)
{
    DEBUGMSGTL(("verbose:dot11BssWAPIPerformanceStatsTable:init_dot11BssWAPIPerformanceStatsTable","called\n"));

    /*
     * TODO:300:o: Perform dot11BssWAPIPerformanceStatsTable one-time module initialization.
     */
     
    /*
     * here we initialize all the tables we're planning on supporting
     */
    if (should_init("dot11BssWAPIPerformanceStatsTable"))
        initialize_table_dot11BssWAPIPerformanceStatsTable();

} /* init_dot11BssWAPIPerformanceStatsTable */

/**
 * Initialize the table dot11BssWAPIPerformanceStatsTable 
 *    (Define its contents and how it's structured)
 */
void
initialize_table_dot11BssWAPIPerformanceStatsTable(void)
{
    dot11BssWAPIPerformanceStatsTable_registration_ptr user_context;
    u_long flags;

    DEBUGMSGTL(("verbose:dot11BssWAPIPerformanceStatsTable:initialize_table_dot11BssWAPIPerformanceStatsTable","called\n"));

    /*
     * TODO:301:o: Perform dot11BssWAPIPerformanceStatsTable one-time table initialization.
     */

    /*
     * TODO:302:o: |->Initialize dot11BssWAPIPerformanceStatsTable user context
     * if you'd like to pass in a pointer to some data for this
     * table, allocate or set it up here.
     */
    /*
     * a netsnmp_data_list is a simple way to store void pointers. A simple
     * string token is used to add, find or remove pointers.
     */
    user_context = netsnmp_create_data_list("dot11BssWAPIPerformanceStatsTable", NULL, NULL);
    
    /*
     * No support for any flags yet, but in the future you would
     * set any flags here.
     */
    flags = 0;
    
    /*
     * call interface initialization code
     */
    mad_dev_oid(dot11BssWAPIPerformanceStatsTable_oid,BSSWAPIPSOID,&dot11BssWAPIPerformanceStatsTable_oid_size,enterprise_pvivate_oid);
    _dot11BssWAPIPerformanceStatsTable_initialize_interface(user_context, flags);
} /* initialize_table_dot11BssWAPIPerformanceStatsTable */

/**
 * pre-request callback
 *
 *
 * @retval MFD_SUCCESS              : success.
 * @retval MFD_ERROR                : other error
 */
int
dot11BssWAPIPerformanceStatsTable_pre_request(dot11BssWAPIPerformanceStatsTable_registration_ptr user_context)
{
    DEBUGMSGTL(("verbose:dot11BssWAPIPerformanceStatsTable:dot11BssWAPIPerformanceStatsTable_pre_request","called\n"));

    /*
     * TODO:510:o: Perform dot11BssWAPIPerformanceStatsTable pre-request actions.
     */

    return MFD_SUCCESS;
} /* dot11BssWAPIPerformanceStatsTable_pre_request */

/**
 * post-request callback
 *
 *
 * @retval MFD_SUCCESS : success.
 * @retval MFD_ERROR   : other error (ignored)
 */
int
dot11BssWAPIPerformanceStatsTable_post_request(dot11BssWAPIPerformanceStatsTable_registration_ptr user_context)
{
    DEBUGMSGTL(("verbose:dot11BssWAPIPerformanceStatsTable:dot11BssWAPIPerformanceStatsTable_post_request","called\n"));

    /*
     * TODO:511:o: Perform dot11BssWAPIPerformanceStatsTable pos-request actions.
     */

    return MFD_SUCCESS;
} /* dot11BssWAPIPerformanceStatsTable_post_request */


/** @{ */
