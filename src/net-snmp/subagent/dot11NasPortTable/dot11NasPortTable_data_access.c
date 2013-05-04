/*
 * Note: this file originally auto-generated by mib2c using
 *       version : 1.12 $ of : mfd-data-access.m2c,v $ 
 *
 * $Id:$
 */
/* standard Net-SNMP includes */
#include <net-snmp/net-snmp-config.h>
#include <net-snmp/net-snmp-includes.h>
#include <net-snmp/agent/net-snmp-agent-includes.h>
#include <stdio.h>   
#include <string.h>  

/* include our parent header */
#include "dot11NasPortTable.h"


#include "dot11NasPortTable_data_access.h"
#include "wcpss/wid/WID.h"
#include "dbus/wcpss/dcli_wid_wtp.h"
#include "dbus/wcpss/dcli_wid_wlan.h"
#include "nm/app/eag/eag_errcode.h"
#include "nm/app/eag/eag_conf.h"
#include "nm/app/eag/eag_interface.h"
#include "ws_dcli_wlans.h"
#include "../mibs_public.h"
#include "ws_init_dbus.h"
#include "ws_eag_conf.h"




/** @defgroup data_access data_access: Routines to access data
 *
 * These routines are used to locate the data used to satisfy
 * requests.
 * 
 * @{
 */
/**********************************************************************
 **********************************************************************
 ***
 *** Table dot11NasPortTable
 ***
 **********************************************************************
 **********************************************************************/
/*
 * dot11NasPortTable is subid 2 of dot11ConfigNasGroup.
 * Its status is Current.
 * OID: .1.3.6.1.4.1.31656.6.1.2.21.2, length: 12
*/

/**
 * initialization for dot11NasPortTable data access
 *
 * This function is called during startup to allow you to
 * allocate any resources you need for the data table.
 *
 * @param dot11NasPortTable_reg
 *        Pointer to dot11NasPortTable_registration
 *
 * @retval MFD_SUCCESS : success.
 * @retval MFD_ERROR   : unrecoverable error.
 */
int
dot11NasPortTable_init_data(dot11NasPortTable_registration_ptr dot11NasPortTable_reg)
{
    DEBUGMSGTL(("verbose:dot11NasPortTable:dot11NasPortTable_init_data","called\n"));

    /*
     * TODO:303:o: Initialize dot11NasPortTable data.
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
} /* dot11NasPortTable_init_data */

/**
 * container-cached overview
 *
 */

/***********************************************************************
 *
 * cache
 *
 ***********************************************************************/
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
dot11NasPortTable_container_init(netsnmp_container **container_ptr_ptr,
                        netsnmp_cache *cache)
{
    DEBUGMSGTL(("verbose:dot11NasPortTable:dot11NasPortTable_container_init","called\n"));
    
    if((NULL == cache) || (NULL == container_ptr_ptr)) {
        snmp_log(LOG_ERR,"bad params to dot11NasPortTable_container_init\n");
        return;
    }

    /*
     * For advanced users, you can use a custom container. If you
     * do not create one, one will be created for you.
     */
    *container_ptr_ptr = NULL;

    /*
     * TODO:345:A: Set up dot11NasPortTable cache properties.
     *
     * Also for advanced users, you can set parameters for the
     * cache. Do not change the magic pointer, as it is used
     * by the MFD helper. To completely disable caching, set
     * cache->enabled to 0.
     */
    cache->timeout = DOT11NASPORTTABLE_CACHE_TIMEOUT; /* seconds */
} /* dot11NasPortTable_container_init */

/**
 * load cache data
 *
 * TODO:350:M: Implement dot11NasPortTable cache load
 *
 * @param container container to which items should be inserted
 *
 * @retval MFD_SUCCESS              : success.
 * @retval MFD_RESOURCE_UNAVAILABLE : Can't access data source
 * @retval MFD_ERROR                : other error.
 *
 *  This function is called to cache the index(es) (and data, optionally)
 *  for the every row in the data set.
 *
 * @remark
 *  While loading the cache, the only important thing is the indexes.
 *  If access to your data is cheap/fast (e.g. you have a pointer to a
 *  structure in memory), it would make sense to update the data here.
 *  If, however, the accessing the data invovles more work (e.g. parsing
 *  some other existing data, or peforming calculations to derive the data),
 *  then you can limit yourself to setting the indexes and saving any
 *  information you will need later. Then use the saved information in
 *  dot11NasPortTable_row_prep() for populating data.
 *
 * @note
 *  If you need consistency between rows (like you want statistics
 *  for each row to be from the same time frame), you should set all
 *  data here.
 *
 */
int
dot11NasPortTable_cache_load(netsnmp_container *container)
{
	snmp_log(LOG_DEBUG, "enter dot11NasPortTable_cache_load\n");

    dot11NasPortTable_rowreq_ctx *rowreq_ctx;
    size_t                 count = 0;
    
    /*
     * this example code is based on a data source that is a
     * text file to be read and parsed.
     */
//    FILE *filep;
//    char line[MAX_LINE_SIZE];
    /*
     * temporary storage for index values
     */
        /*
         * wtpMacAddr(1)/DisplayString/ASN_OCTET_STR/char(char)//L/A/w/e/R/d/H
         */
        /** 128 - 1(entry) - 1(col) - 1(other indexes) = 111 */
   char   wtpMacAddr[20];
   size_t      wtpMacAddr_len;
        /*
         * wlanID(1)/INTEGER/ASN_INTEGER/long(long)//l/A/w/e/r/d/h
         */
   long   wlanID;

    DEBUGMSGTL(("verbose:dot11NasPortTable:dot11NasPortTable_cache_load","called\n"));

    /*
    ***************************************************
    ***             START EXAMPLE CODE              ***
    ***---------------------------------------------***/
    /*
     * open our data file.
     */
//    filep = fopen("/etc/dummy.conf", "r");
//    if(NULL ==  filep) {
//        return MFD_RESOURCE_UNAVAILABLE;
//    }

    /*
    ***---------------------------------------------***
    ***              END  EXAMPLE CODE              ***
    ***************************************************/
    /*
     * TODO:351:M: |-> Load/update data in the dot11NasPortTable container.
     * loop over your dot11NasPortTable data, allocate a rowreq context,
     * set the index(es) [and data, optionally] and insert into
     * the container.
     */


	size_t NasPortID_len;
	char NasPortID[11] = { 0 };	
	int ret = -1;
	int i=0;		
	struct nasportid_conf nasportid;	

    snmpd_dbus_message *messageHead = NULL, *messageNode = NULL;
    
    snmp_log(LOG_DEBUG, "enter list_connection_call_dbus_method:show_all_wlan_ssid_stats_information_cmd\n");
    messageHead = list_connection_call_dbus_method(show_all_wlan_ssid_stats_information_cmd, SHOW_ALL_WTP_TABLE_METHOD);
	snmp_log(LOG_DEBUG, "exit list_connection_call_dbus_method:show_all_wlan_ssid_stats_information_cmd,messageHead=%p\n", messageHead);
	
	if(messageHead)
	{
		for(messageNode = messageHead; NULL != messageNode; messageNode = messageNode->next)
		{
			void *connection = NULL;
			ret = -1;
            if(SNMPD_DBUS_SUCCESS == get_instance_dbus_connection(messageNode->parameter, &connection, SNMPD_INSTANCE_MASTER_V3))
            {
				memset (&nasportid,0,sizeof(nasportid));					
				ret=eag_get_nasportid(connection, messageNode->parameter.local_id, messageNode->parameter.instance_id, &nasportid);
            }		
			
		    struct SSIDStatsInfo *head = messageNode->message;
		    if(head)
		    {
		        struct SSIDStatsInfo *wtp_show = NULL;
        	    for(wtp_show = head->next; NULL != wtp_show; wtp_show = wtp_show->next)
        	    {
        			memset(wtpMacAddr,0,20);
					if(wtp_show->wtpMacAddr) 
					{
						snprintf(wtpMacAddr,sizeof(wtpMacAddr)-1,"%02X:%02X:%02X:%02X:%02X:%02X",wtp_show->wtpMacAddr[0],
																		   wtp_show->wtpMacAddr[1],
																		   wtp_show->wtpMacAddr[2],
																		   wtp_show->wtpMacAddr[3],
																		   wtp_show->wtpMacAddr[4],
																		   wtp_show->wtpMacAddr[5]);
					}
                   
                  // struct SSIDStatsInfo_Radioid_info *radio_Show = NULL;
                   struct SSIDStatsInfo_sub_wlan *wlan_Show = NULL;
				   if(wtp_show->SSIDStatsInfo_Radioid_info_head)
				   {
					   for(wlan_Show = wtp_show->SSIDStatsInfo_Radioid_info_head->SSIDStatsInfo_sub_wlan_head; NULL != wlan_Show; wlan_Show = wlan_Show->next)
	        		   {
	        	          wlanID = local_to_global_ID(messageNode->parameter, wlan_Show->wlanCurrID, WIRELESS_MAX_NUM);
			   /*
			   ***************************************************
			   ***			   START EXAMPLE CODE			   ***
			   ***---------------------------------------------***/
			   /*
				* get a line (skip blank lines)
				*/
			   //do {
			   //	 if (!fgets(line, sizeof(line), filep)) {
			   //		 /* we're done */
			   //		 fclose(filep);
			   //		 filep = NULL;
			   //	 }
			   //} while (filep && (line[0] == '\n'));
		   
			   /*
				* check for end of data
				*/
			   //if(NULL == filep)
			   //	 break;
		   
			   /*
				* parse line into variables
				*/
			   /*
			   ***---------------------------------------------***
			   ***				END  EXAMPLE CODE			   ***
			   ***************************************************/
		   
				   /*
					* TODO:352:M: |   |-> set indexes in new dot11NasPortTable rowreq context.
					*/
				   rowreq_ctx = dot11NasPortTable_allocate_rowreq_ctx();
				   if (NULL == rowreq_ctx) {
					   snmp_log(LOG_ERR, "memory allocation failed\n");
	                   free_dbus_message_list(&messageHead, Free_show_all_wlan_ssid_stats_information_cmd);
					   return MFD_RESOURCE_UNAVAILABLE;
				   }
				   wtpMacAddr_len = strlen(wtpMacAddr);
				   if(MFD_SUCCESS != dot11NasPortTable_indexes_set(rowreq_ctx
										  , wtpMacAddr, wtpMacAddr_len
										  , wlanID
						  )) {
					   snmp_log(LOG_ERR,"error setting index while loading "
								"dot11NasPortTable cache.\n");
					   dot11NasPortTable_release_rowreq_ctx(rowreq_ctx);
					   continue;
				   }
		   
				   /*
					* TODO:352:r: |   |-> populate dot11NasPortTable data context.
					* Populate data context here. (optionally, delay until row prep)
					*/
			   /*
				* TRANSIENT or semi-TRANSIENT data:
				* copy data or save any info needed to do it in row_prep.
				*/
			   /*
				* setup/save data for NasPortID
				* NasPortID(1)/DisplayString/ASN_OCTET_STR/char(char)//L/A/w/e/R/d/H
				*/
			   /*
				* TODO:246:r: |-> Define NasPortID mapping.
				* Map values between raw/native values and MIB values
				*
				* if(MFD_SUCCESS !=
				*	 NasPortID_map(&rowreq_ctx->data.NasPortID, &rowreq_ctx->data.NasPortID_len,
				*				 NasPortID, NasPortID_len, 0)) {
				*	 return MFD_ERROR;
				* }
				*/
			   /*
				* make sure there is enough space for NasPortID data
				*/
					  //if ((NULL == rowreq_ctx->data.NasPortID) ||
					  //  (rowreq_ctx->data.NasPortID_len < (NasPortID_len * sizeof(rowreq_ctx->data.NasPortID[0])))) {
					  //  snmp_log(LOG_ERR,"not enough space for value\n");
					  //  return MFD_ERROR;
					  //}
						//start :lixiang add for nasportid 2011-4-11 
					    int vlanid = 0;
						if(EAG_RETURN_OK == ret)
						{
						   for (i=0;i<nasportid.current_num;i++)
						   {
							   if (NASPORTID_KEYTYPE_WLAN_WTP == nasportid.nasportid_map[i].key_type) {
								   int wtpstart = nasportid.nasportid_map[i].key.wlan_wtp.wtpid_begin;
								   int wtpend = nasportid.nasportid_map[i].key.wlan_wtp.wtpid_end;
								   int wlanstart = nasportid.nasportid_map[i].key.wlan_wtp.wlanid_begin;
								   int wlanend = nasportid.nasportid_map[i].key.wlan_wtp.wlanid_end;
								   int wwnasportid = nasportid.nasportid_map[i].nasportid;
						   
								   if(wtp_show->wtpCurrID >= wtpstart && wtp_show->wtpCurrID <= wtpend && 
									  wlan_Show->wlanCurrID >= wlanstart && wlan_Show->wlanCurrID <= wlanend)
								   {
									   vlanid = wwnasportid;
									   break;
								   }
							   }
						   }
						}						

						char NasPortID_buff[12];					  
						snprintf(NasPortID_buff, sizeof(NasPortID_buff) - 1, "%010d", vlanid);


						NasPortID_len = MIN(strlen(NasPortID_buff),sizeof(NasPortID)-1);

						memset(NasPortID,0,sizeof(NasPortID));

						memcpy(NasPortID,NasPortID_buff,NasPortID_len);
						rowreq_ctx->data.NasPortID_len = NasPortID_len * sizeof(rowreq_ctx->data.NasPortID[0]);
						memcpy( rowreq_ctx->data.NasPortID, NasPortID, rowreq_ctx->data.NasPortID_len );

				   
				   /*
					* insert into table container
					*/
					   if(CONTAINER_INSERT(container, rowreq_ctx))
	    				   {
	    				   		dot11NasPortTable_release_rowreq_ctx(rowreq_ctx);
	    				   }
	    				   ++count;
	    			    }				   }
  
    	        }
    	    }
    	}
        free_dbus_message_list(&messageHead, Free_show_all_wlan_ssid_stats_information_cmd);
	}
    /*
    ***************************************************
    ***             START EXAMPLE CODE              ***
    ***---------------------------------------------***/
   // if(NULL != filep)
   //     fclose(filep);
    /*
    ***---------------------------------------------***
    ***              END  EXAMPLE CODE              ***
    ***************************************************/

    DEBUGMSGT(("verbose:dot11NasPortTable:dot11NasPortTable_cache_load",
               "inserted %d records\n", count));

	snmp_log(LOG_DEBUG, "exit dot11NasPortTable_cache_load\n");
    return MFD_SUCCESS;
} /* dot11NasPortTable_cache_load */

/**
 * cache clean up
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
dot11NasPortTable_cache_free(netsnmp_container *container)
{
    DEBUGMSGTL(("verbose:dot11NasPortTable:dot11NasPortTable_cache_free","called\n"));

    /*
     * TODO:380:M: Free dot11NasPortTable cache.
     */
} /* dot11NasPortTable_cache_free */

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
dot11NasPortTable_row_prep( dot11NasPortTable_rowreq_ctx *rowreq_ctx)
{
    DEBUGMSGTL(("verbose:dot11NasPortTable:dot11NasPortTable_row_prep","called\n"));

    netsnmp_assert(NULL != rowreq_ctx);

    /*
     * TODO:390:o: Prepare row for request.
     * If populating row data was delayed, this is the place to
     * fill in the row for this request.
     */

    return MFD_SUCCESS;
} /* dot11NasPortTable_row_prep */

/** @} */
