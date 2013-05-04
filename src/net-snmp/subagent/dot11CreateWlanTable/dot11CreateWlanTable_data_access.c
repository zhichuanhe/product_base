/*
 * Note: this file originally auto-generated by mib2c using
 *       version : 14170 $ of $ 
 *
 * $Id:$
 */
/* standard Net-SNMP includes */
#include <net-snmp/net-snmp-config.h>
#include <net-snmp/net-snmp-includes.h>
#include <net-snmp/agent/net-snmp-agent-includes.h>

/* include our parent header */
#include "dot11CreateWlanTable.h"


#include "dot11CreateWlanTable_data_access.h"
#include "wcpss/asd/asd.h"
#include "wcpss/wid/WID.h"
#include "dbus/wcpss/dcli_wid_wtp.h"
#include "dbus/wcpss/dcli_wid_wlan.h"
#include "ws_dcli_wlans.h"
#include "autelanWtpGroup.h"
#include "ws_init_dbus.h"
#include "ws_dbus_list_interface.h"

/** @ingroup interface
 * @addtogroup data_access data_access: Routines to access data
 *
 * These routines are used to locate the data used to satisfy
 * requests.
 * 
 * @{
 */
/**********************************************************************
 **********************************************************************
 ***
 *** Table dot11CreateWlanTable
 ***
 **********************************************************************
 **********************************************************************/
/*
 * DOT11-AC-MIB::dot11CreateWlanTable is subid 1 of dot11wlanConfig.
 * Its status is Current.
 * OID: .1.3.6.1.4.1.31656.6.1.2.13.1, length: 12
*/

/**
 * initialization for dot11CreateWlanTable data access
 *
 * This function is called during startup to allow you to
 * allocate any resources you need for the data table.
 *
 * @param dot11CreateWlanTable_reg
 *        Pointer to dot11CreateWlanTable_registration
 *
 * @retval MFD_SUCCESS : success.
 * @retval MFD_ERROR   : unrecoverable error.
 */
int
dot11CreateWlanTable_init_data(dot11CreateWlanTable_registration * dot11CreateWlanTable_reg)
{
    DEBUGMSGTL(("verbose:dot11CreateWlanTable:dot11CreateWlanTable_init_data","called\n"));

    /*
     * TODO:303:o: Initialize dot11CreateWlanTable data.
     */
    /*
    ***************************************************
    ***             START EXAMPLE CODE              ***
    ***---------------------------------------------***/
    /*
     * if you are the sole writer for the file, you could
     * open it here. However, as stated earlier, we are assuming
     * the worst case, which in this case means that the file is
     * written to by someone else, and might not even exist when
     * we start up. So we can't do anything here.
     */
    /*
    ***---------------------------------------------***
    ***              END  EXAMPLE CODE              ***
    ***************************************************/

    return MFD_SUCCESS;
} /* dot11CreateWlanTable_init_data */

/**
 * container overview
 *
 */

/**
 * container initialization
 *
 * @param container_ptr_ptr A pointer to a container pointer. If you
 *        create a custom container, use this parameter to return it
 *        to the MFD helper. If set to NULL, the MFD helper will
 *        allocate a container for you.
 * @param  cache A pointer to a cache structure. You can set the timeout
 *         and other cache flags using this pointer.
 *
 *  This function is called at startup to allow you to customize certain
 *  aspects of the access method. For the most part, it is for advanced
 *  users. The default code should suffice for most cases. If no custom
 *  container is allocated, the MFD code will create one for your.
 *
 *  This is also the place to set up cache behavior. The default, to
 *  simply set the cache timeout, will work well with the default
 *  container. If you are using a custom container, you may want to
 *  look at the cache helper documentation to see if there are any
 *  flags you want to set.
 *
 * @remark
 *  This would also be a good place to do any initialization needed
 *  for you data source. For example, opening a connection to another
 *  process that will supply the data, opening a database, etc.
 */
void
dot11CreateWlanTable_container_init(netsnmp_container **container_ptr_ptr,
                             netsnmp_cache *cache)
{
    DEBUGMSGTL(("verbose:dot11CreateWlanTable:dot11CreateWlanTable_container_init","called\n"));
    
    if (NULL == container_ptr_ptr) {
        snmp_log(LOG_ERR,"bad container param to dot11CreateWlanTable_container_init\n");
        return;
    }

    /*
     * For advanced users, you can use a custom container. If you
     * do not create one, one will be created for you.
     */
    *container_ptr_ptr = NULL;

    if (NULL == cache) {
        snmp_log(LOG_ERR,"bad cache param to dot11CreateWlanTable_container_init\n");
        return;
    }

    /*
     * TODO:345:A: Set up dot11CreateWlanTable cache properties.
     *
     * Also for advanced users, you can set parameters for the
     * cache. Do not change the magic pointer, as it is used
     * by the MFD helper. To completely disable caching, set
     * cache->enabled to 0.
     */
    cache->timeout = DOT11CREATEWLANTABLE_CACHE_TIMEOUT; /* seconds */
} /* dot11CreateWlanTable_container_init */

/**
 * container shutdown
 *
 * @param container_ptr A pointer to the container.
 *
 *  This function is called at shutdown to allow you to customize certain
 *  aspects of the access method. For the most part, it is for advanced
 *  users. The default code should suffice for most cases.
 *
 *  This function is called before dot11CreateWlanTable_container_free().
 *
 * @remark
 *  This would also be a good place to do any cleanup needed
 *  for you data source. For example, closing a connection to another
 *  process that supplied the data, closing a database, etc.
 */
void
dot11CreateWlanTable_container_shutdown(netsnmp_container *container_ptr)
{
    DEBUGMSGTL(("verbose:dot11CreateWlanTable:dot11CreateWlanTable_container_shutdown","called\n"));
    
    if (NULL == container_ptr) {
        snmp_log(LOG_ERR,"bad params to dot11CreateWlanTable_container_shutdown\n");
        return;
    }

} /* dot11CreateWlanTable_container_shutdown */

/**
 * load initial data
 *
 * TODO:350:M: Implement dot11CreateWlanTable data load
 * This function will also be called by the cache helper to load
 * the container again (after the container free function has been
 * called to free the previous contents).
 *
 * @param container container to which items should be inserted
 *
 * @retval MFD_SUCCESS              : success.
 * @retval MFD_RESOURCE_UNAVAILABLE : Can't access data source
 * @retval MFD_ERROR                : other error.
 *
 *  This function is called to load the index(es) (and data, optionally)
 *  for the every row in the data set.
 *
 * @remark
 *  While loading the data, the only important thing is the indexes.
 *  If access to your data is cheap/fast (e.g. you have a pointer to a
 *  structure in memory), it would make sense to update the data here.
 *  If, however, the accessing the data invovles more work (e.g. parsing
 *  some other existing data, or peforming calculations to derive the data),
 *  then you can limit yourself to setting the indexes and saving any
 *  information you will need later. Then use the saved information in
 *  dot11CreateWlanTable_row_prep() for populating data.
 *
 * @note
 *  If you need consistency between rows (like you want statistics
 *  for each row to be from the same time frame), you should set all
 *  data here.
 *
 */
int
dot11CreateWlanTable_container_load(netsnmp_container *container)
{
	snmp_log(LOG_DEBUG, "enter dot11CreateWlanTable_container_load\n");
	
    dot11CreateWlanTable_rowreq_ctx *rowreq_ctx;
    size_t                 count = 0;

    /*
     * temporary storage for index values
     */
        /*
         * wlanID(1)/INTEGER/ASN_INTEGER/long(long)//l/A/W/e/r/d/h
         */
   long   wlanID;

    
    /*
     * this example code is based on a data source that is a
     * text file to be read and parsed.
     */
    //FILE *filep;
    //char line[MAX_LINE_SIZE];

    DEBUGMSGTL(("verbose:dot11CreateWlanTable:dot11CreateWlanTable_container_load","called\n"));

    /*
    ***************************************************
    ***             START EXAMPLE CODE              ***
    ***---------------------------------------------***/
    /*
     * open our data file.
     */
    //filep = fopen("/etc/dummy.conf", "r");
    //if(NULL ==  filep) {
      //  return MFD_RESOURCE_UNAVAILABLE;
    //}

    /*
    ***---------------------------------------------***
    ***              END  EXAMPLE CODE              ***
    ***************************************************/
    /*
     * TODO:351:M: |-> Load/update data in the dot11CreateWlanTable container.
     * loop over your dot11CreateWlanTable data, allocate a rowreq context,
     * set the index(es) [and data, optionally] and insert into
     * the container.
     */
    snmpd_dbus_message *messageHead = NULL, *messageNode = NULL;
    snmp_log(LOG_DEBUG, "enter list_connection_call_dbus_method:show_wlan_list\n");
    messageHead = list_connection_call_dbus_method(show_wlan_list, SHOW_ALL_WTP_TABLE_METHOD);
	snmp_log(LOG_DEBUG, "exit list_connection_call_dbus_method:show_wlan_list,messageHead=%p\n", messageHead);

	if(messageHead)
	{
		for(messageNode = messageHead; NULL != messageNode; messageNode = messageNode->next)
		{
		    DCLI_WLAN_API_GROUP *WLANINFO = messageNode->message;			
			char WlanName[255] = { 0 };
			char ESSID[255] = { 0 };
		    if(WLANINFO)
		    {
		        int i = 0;
		        for(i = 0; i < WLANINFO->wlan_num; i++)
        		{        			
					if(WLANINFO->WLAN[i])
				    {
						wlanID = local_to_global_ID(messageNode->parameter, WLANINFO->WLAN[i]->WlanID, WIRELESS_MAX_NUM);

						memset(WlanName,0,sizeof(WlanName));
						if(WLANINFO->WLAN[i]->WlanName)
						{
							strncpy(WlanName,WLANINFO->WLAN[i]->WlanName,sizeof(WlanName)-1);
						}
						
						memset(ESSID,0,sizeof(ESSID));
						if(WLANINFO->WLAN[i]->ESSID)
						{
							strncpy(ESSID,WLANINFO->WLAN[i]->ESSID,sizeof(ESSID)-1);
						}
				    /*
				    ***************************************************
				    ***             START EXAMPLE CODE              ***
				    ***---------------------------------------------***/
				    /*
				     * get a line (skip blank lines)
				     */
				    //do {
				      //  if (!fgets(line, sizeof(line), filep)) {
				            /* we're done */
				        //    fclose(filep);
				          //  filep = NULL;
				        //}
				   // } while (filep && (line[0] == '\n'));

				    /*
				     * check for end of data
				     */
				   // if(NULL == filep)
				     //   break;

				    /*
				     * parse line into variables
				     */
				    /*
				    ***---------------------------------------------***
				    ***              END  EXAMPLE CODE              ***
				    ***************************************************/

				        /*
				         * TODO:352:M: |   |-> set indexes in new dot11CreateWlanTable rowreq context.
				         * data context will be set from the param (unless NULL,
				         *      in which case a new data context will be allocated)
				         */
				        rowreq_ctx = dot11CreateWlanTable_allocate_rowreq_ctx(NULL);
				        if (NULL == rowreq_ctx) {
				            snmp_log(LOG_ERR, "memory allocation failed\n");
							free_dbus_message_list(&messageHead, Free_wlan_head);
				            return MFD_RESOURCE_UNAVAILABLE;
				        }
				        if(MFD_SUCCESS != dot11CreateWlanTable_indexes_set(rowreq_ctx
				                               , wlanID
				               )) {
				            snmp_log(LOG_ERR,"error setting index while loading "
				                     "dot11CreateWlanTable data.\n");
				            dot11CreateWlanTable_release_rowreq_ctx(rowreq_ctx);
				            continue;
				        }

						rowreq_ctx->data.localWlanID = WLANINFO->WLAN[i]->WlanID;
						
						memcpy(&(rowreq_ctx->data.parameter), &(messageNode->parameter), sizeof(rowreq_ctx->data.parameter));

				        /*
				         * TODO:352:r: |   |-> populate dot11CreateWlanTable data context.
				         * Populate data context here. (optionally, delay until row prep)
				         */
				    /*
				     * TRANSIENT or semi-TRANSIENT data:
				     * copy data or save any info needed to do it in row_prep.
				     */
				    /*
				     * setup/save data for wlanName
				     * wlanName(1)/DisplayString/ASN_OCTET_STR/char(char)//L/A/W/e/R/d/H
				     */
				    /** no mapping */
				    /*
				     * make sure there is enough space for wlanName data
				     */
				    /*if ((NULL == rowreq_ctx->data.wlanName) ||
				        (rowreq_ctx->data.wlanName_len <
				         (wlanName_len* sizeof(wlanName[0])))) {
				        snmp_log(LOG_ERR,"not enough space for value\n");
				        return MFD_ERROR;
				    }*/
				    rowreq_ctx->data.wlanName_len = MIN(sizeof(rowreq_ctx->data.wlanName) - 1, strlen(WlanName));
				    memcpy( rowreq_ctx->data.wlanName, WlanName, rowreq_ctx->data.wlanName_len );
				    
				    /*
				     * setup/save data for wlanESSID
				     * wlanESSID(2)/DisplayString/ASN_OCTET_STR/char(char)//L/A/W/e/R/d/H
				     */
				    /** no mapping */
				    /*
				     * make sure there is enough space for wlanESSID data
				     */
				    /*if ((NULL == rowreq_ctx->data.wlanESSID) ||
				        (rowreq_ctx->data.wlanESSID_len <
				         (wlanESSID_len* sizeof(wlanESSID[0])))) {
				        snmp_log(LOG_ERR,"not enough space for value\n");
				        return MFD_ERROR;
				    }*/
				    rowreq_ctx->data.wlanESSID_len = MIN(sizeof(rowreq_ctx->data.wlanESSID) - 1, strlen(ESSID));
				    memcpy( rowreq_ctx->data.wlanESSID, ESSID, rowreq_ctx->data.wlanESSID_len );
				    
				    /*
				     * setup/save data for wlanRowStatus
				     * wlanRowStatus(3)/RowStatus/ASN_INTEGER/long(u_long)//l/A/W/E/r/d/h
				     */
				    /** no mapping */
				    rowreq_ctx->data.wlanRowStatus = RS_ACTIVE;
				    
				        
				        /*
				         * insert into table container
				         */
				        if(CONTAINER_INSERT(container, rowreq_ctx))
				        {
				        	dot11CreateWlanTable_release_rowreq_ctx(rowreq_ctx);
				        }
				        ++count;
				    }			
				}
		    }
		}
		free_dbus_message_list(&messageHead, Free_wlan_head);
    }		


    /*
    ***************************************************
    ***             START EXAMPLE CODE              ***
    ***---------------------------------------------***/
    //if(NULL != filep)
       // fclose(filep);
    /*
    ***---------------------------------------------***
    ***              END  EXAMPLE CODE              ***
    ***************************************************/

    DEBUGMSGT(("verbose:dot11CreateWlanTable:dot11CreateWlanTable_container_load",
               "inserted %d records\n", count));

	snmp_log(LOG_DEBUG, "exit dot11CreateWlanTable_container_load\n");
	
    return MFD_SUCCESS;
} /* dot11CreateWlanTable_container_load */

/**
 * container clean up
 *
 * @param container container with all current items
 *
 *  This optional callback is called prior to all
 *  item's being removed from the container. If you
 *  need to do any processing before that, do it here.
 *
 * @note
 *  The MFD helper will take care of releasing all the row contexts.
 *
 */
void
dot11CreateWlanTable_container_free(netsnmp_container *container)
{
    DEBUGMSGTL(("verbose:dot11CreateWlanTable:dot11CreateWlanTable_container_free","called\n"));

    /*
     * TODO:380:M: Free dot11CreateWlanTable container data.
     */
} /* dot11CreateWlanTable_container_free */

/**
 * prepare row for processing.
 *
 *  When the agent has located the row for a request, this function is
 *  called to prepare the row for processing. If you fully populated
 *  the data context during the index setup phase, you may not need to
 *  do anything.
 *
 * @param rowreq_ctx pointer to a context.
 *
 * @retval MFD_SUCCESS     : success.
 * @retval MFD_ERROR       : other error.
 */
int
dot11CreateWlanTable_row_prep( dot11CreateWlanTable_rowreq_ctx *rowreq_ctx)
{
    DEBUGMSGTL(("verbose:dot11CreateWlanTable:dot11CreateWlanTable_row_prep","called\n"));

    netsnmp_assert(NULL != rowreq_ctx);

    /*
     * TODO:390:o: Prepare row for request.
     * If populating row data was delayed, this is the place to
     * fill in the row for this request.
     */

    return MFD_SUCCESS;
} /* dot11CreateWlanTable_row_prep */

/*
 * TODO:420:r: Implement dot11CreateWlanTable index validation.
 */
/*---------------------------------------------------------------------
 * DOT11-AC-MIB::dot11VlanConfigEntry.wlanID
 * wlanID is subid 1 of dot11VlanConfigEntry.
 * Its status is Current, and its access level is ReadWrite.
 * OID: .1.3.6.1.4.1.31656.6.1.2.7.1.1
 * Description:
Vlan ID.
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
 * check validity of wlanID external index portion
 *
 * NOTE: this is not the place to do any checks for the sanity
 *       of multiple indexes. Those types of checks should be done in the
 *       dot11CreateWlanTable_validate_index() function.
 *
 * @retval MFD_SUCCESS   : the incoming value is legal
 * @retval MFD_ERROR     : the incoming value is NOT legal
 */
int
dot11CreateWlanTable_wlanID_check_index( dot11CreateWlanTable_rowreq_ctx *rowreq_ctx )
{
    DEBUGMSGTL(("verbose:dot11CreateWlanTable:dot11CreateWlanTable_wlanID_check_index","called\n"));

    netsnmp_assert(NULL != rowreq_ctx);

    /*
     * TODO:424:M: |-> Check dot11CreateWlanTable external index wlanID.
     * check that index value in the table context (rowreq_ctx)
     * for the external index wlanID is legal.
     */

    return MFD_SUCCESS; /*  external index wlanID ok */
} /* dot11CreateWlanTable_wlanID_check_index */

/**
 * verify specified index is valid.
 *
 * This check is independent of whether or not the values specified for
 * the columns of the new row are valid. Column values and row consistency
 * will be checked later. At this point, only the index values should be
 * checked.
 *
 * All of the individual index validation functions have been called, so this
 * is the place to make sure they are valid as a whole when combined. If
 * you only have one index, then you probably don't need to do anything else
 * here.
 * 
 * @note Keep in mind that if the indexes refer to a row in this or
 *       some other table, you can't check for that row here to make
 *       decisions, since that row might not be created yet, but may
 *       be created during the processing this request. If you have
 *       such checks, they should be done in the check_dependencies
 *       function, because any new/deleted/changed rows should be
 *       available then.
 *
 *
 * @param dot11CreateWlanTable_reg
 *        Pointer to the user registration data
 * @param dot11CreateWlanTable_rowreq_ctx
 *        Pointer to the users context.
 * @retval MFD_SUCCESS            : success
 * @retval MFD_CANNOT_CREATE_NOW  : index not valid right now
 * @retval MFD_CANNOT_CREATE_EVER : index never valid
 */
int
dot11CreateWlanTable_validate_index( dot11CreateWlanTable_registration * dot11CreateWlanTable_reg,
                           dot11CreateWlanTable_rowreq_ctx *rowreq_ctx)
{
    int rc = MFD_SUCCESS;

    DEBUGMSGTL(("verbose:dot11CreateWlanTable:dot11CreateWlanTable_validate_index","called\n"));

    /** we should have a non-NULL pointer */
    netsnmp_assert( NULL != rowreq_ctx );

    /*
     * TODO:430:M: |-> Validate potential dot11CreateWlanTable index.
     */
    if(0) {
        snmp_log(LOG_WARNING,"invalid index for a new row in the "
                 "dot11CreateWlanTable table.\n");
        /*
         * determine failure type.
         *
         * If the index could not ever be created, return MFD_NOT_EVER
         * If the index can not be created under the present circumstances
         * (even though it could be created under other circumstances),
         * return MFD_NOT_NOW.
         */
        if(0) {
            return MFD_CANNOT_CREATE_EVER;
        }
        else {
            return MFD_CANNOT_CREATE_NOW;
        }
    }

    return rc;
} /* dot11CreateWlanTable_validate_index */

/** @} */
