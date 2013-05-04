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
#include "ws_init_dbus.h"
#include "../mibs_public.h"
#include "wcpss/asd/asd.h"
#include "wcpss/wid/WID.h"
#include "dbus/wcpss/dcli_wid_wtp.h"
#include "dbus/wcpss/dcli_wid_wlan.h"
#include "ws_dcli_wlans.h"
#include "ws_dcli_ac.h"

/* include our parent header */
#include "dot11ConfigTrapSwitchTable.h"


#include "dot11ConfigTrapSwitchTable_data_access.h"

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
 *** Table dot11ConfigTrapSwitchTable
 ***
 **********************************************************************
 **********************************************************************/
/*
 * dot11ConfigTrapSwitchTable is subid 2 of dot11AlarmConfiguration.
 * Its status is Current.
 * OID: .1.3.6.1.4.1.31656.6.1.2.18.2, length: 12
*/

/**
 * initialization for dot11ConfigTrapSwitchTable data access
 *
 * This function is called during startup to allow you to
 * allocate any resources you need for the data table.
 *
 * @param dot11ConfigTrapSwitchTable_reg
 *        Pointer to dot11ConfigTrapSwitchTable_registration
 *
 * @retval MFD_SUCCESS : success.
 * @retval MFD_ERROR   : unrecoverable error.
 */
int
dot11ConfigTrapSwitchTable_init_data(dot11ConfigTrapSwitchTable_registration_ptr dot11ConfigTrapSwitchTable_reg)
{
    DEBUGMSGTL(("verbose:dot11ConfigTrapSwitchTable:dot11ConfigTrapSwitchTable_init_data","called\n"));

    /*
     * TODO:303:o: Initialize dot11ConfigTrapSwitchTable data.
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
} /* dot11ConfigTrapSwitchTable_init_data */

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
dot11ConfigTrapSwitchTable_container_init(netsnmp_container **container_ptr_ptr,
                        netsnmp_cache *cache)
{
    DEBUGMSGTL(("verbose:dot11ConfigTrapSwitchTable:dot11ConfigTrapSwitchTable_container_init","called\n"));
    
    if((NULL == cache) || (NULL == container_ptr_ptr)) {
        snmp_log(LOG_ERR,"bad params to dot11ConfigTrapSwitchTable_container_init\n");
        return;
    }

    /*
     * For advanced users, you can use a custom container. If you
     * do not create one, one will be created for you.
     */
    *container_ptr_ptr = NULL;

    /*
     * TODO:345:A: Set up dot11ConfigTrapSwitchTable cache properties.
     *
     * Also for advanced users, you can set parameters for the
     * cache. Do not change the magic pointer, as it is used
     * by the MFD helper. To completely disable caching, set
     * cache->enabled to 0.
     */
    cache->timeout = DOT11CONFIGTRAPSWITCHTABLE_CACHE_TIMEOUT; /* seconds */
} /* dot11ConfigTrapSwitchTable_container_init */

/**
 * load cache data
 *
 * TODO:350:M: Implement dot11ConfigTrapSwitchTable cache load
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
 *  dot11ConfigTrapSwitchTable_row_prep() for populating data.
 *
 * @note
 *  If you need consistency between rows (like you want statistics
 *  for each row to be from the same time frame), you should set all
 *  data here.
 *
 */
int
dot11ConfigTrapSwitchTable_cache_load(netsnmp_container *container)
{
	snmp_log(LOG_DEBUG, "enter dot11ConfigTrapSwitchTable_cache_load\n");

    dot11ConfigTrapSwitchTable_rowreq_ctx *rowreq_ctx;
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
        /** 128 - 1(entry) - 1(col) - 0(other indexes) = 112 */
   char   wtpMacAddr[20];
   size_t      wtpMacAddr_len;

    DEBUGMSGTL(("verbose:dot11ConfigTrapSwitchTable:dot11ConfigTrapSwitchTable_cache_load","called\n"));

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
     * TODO:351:M: |-> Load/update data in the dot11ConfigTrapSwitchTable container.
     * loop over your dot11ConfigTrapSwitchTable data, allocate a rowreq context,
     * set the index(es) [and data, optionally] and insert into
     * the container.
     */

    
    snmpd_dbus_message *messageHead = NULL, *messageNode = NULL;
    snmp_log(LOG_DEBUG, "enter list_connection_call_dbus_method:show_wtp_list_by_mac_cmd_func\n");
    messageHead = list_connection_call_dbus_method(show_wtp_list_by_mac_cmd_func, SHOW_ALL_WTP_TABLE_METHOD);
    snmp_log(LOG_DEBUG, "exit list_connection_call_dbus_method:show_wtp_list_by_mac_cmd_func,messageHead=%p\n", messageHead);
	
	if(messageHead)
	{
	    for(messageNode = messageHead; NULL != messageNode; messageNode = messageNode->next)
        {
            DCLI_WTP_API_GROUP_ONE *wtp_node = messageNode->message;

    		if((wtp_node)&&(wtp_node->WTP_INFO)&&(wtp_node->WTP_INFO->WTP_LIST))
    		{
        		WID_WTP *q = NULL;
        		for(q = wtp_node->WTP_INFO->WTP_LIST; NULL != q; q = q->next)
        		{
        			memset(wtpMacAddr,0,20);
					if(q->WTPMAC)
					{
						snprintf(wtpMacAddr,sizeof(wtpMacAddr)-1,"%02X:%02X:%02X:%02X:%02X:%02X",q->WTPMAC[0],q->WTPMAC[1],q->WTPMAC[2],q->WTPMAC[3],q->WTPMAC[4],q->WTPMAC[5]);
					}
                    
        			int ret_threshold = 0;
        			WID_TRAP_THRESHOLD *show_node = NULL;
        			
        			void *connection = NULL;
        			if(SNMPD_DBUS_ERROR == get_instance_dbus_connection(messageNode->parameter, &connection, SNMPD_INSTANCE_MASTER_V3))
                        continue;
        			
        			ret_threshold = show_ap_trap_rogue_ap_ter_cpu_mem_threshold_cmd(messageNode->parameter, connection, q->WTPID, &show_node);
        			if( 1 == ret_threshold)
        			{
			/*
			***************************************************
			*** 			START EXAMPLE CODE				***
			***---------------------------------------------***/
			/*
			 * get a line (skip blank lines)
			 */
					//	  do {
					//		  if (!fgets(line, sizeof(line), filep)) {
					//			  /* we're done */
					//			  fclose(filep);
					//			  filep = NULL;
					//		  }
					//	  } while (filep && (line[0] == '\n'));
		
			/*
			 * check for end of data
			 */
					//	  if(NULL == filep)
					//		  break;
		
			/*
			 * parse line into variables
			 */
			/*
			***---------------------------------------------***
			*** 			 END  EXAMPLE CODE				***
			***************************************************/
		
				/*
				 * TODO:352:M: |   |-> set indexes in new dot11ConfigTrapSwitchTable rowreq context.
				 */
				rowreq_ctx = dot11ConfigTrapSwitchTable_allocate_rowreq_ctx();
				if (NULL == rowreq_ctx) {
					snmp_log(LOG_ERR, "memory allocation failed\n");
					FREE_OBJECT(q->WTPMAC);
					Free_show_ap_trap_threshold(show_node);
                    free_dbus_message_list(&messageHead, Free_wtp_list_by_mac_head);
					return MFD_RESOURCE_UNAVAILABLE;
				}
				wtpMacAddr_len=strlen(wtpMacAddr);
				if(MFD_SUCCESS != dot11ConfigTrapSwitchTable_indexes_set(rowreq_ctx
									   , wtpMacAddr, wtpMacAddr_len
					   )) {
					snmp_log(LOG_ERR,"error setting index while loading "
							 "dot11ConfigTrapSwitchTable cache.\n");
					dot11ConfigTrapSwitchTable_release_rowreq_ctx(rowreq_ctx);
					continue;
				}
		
				/*
				 * TODO:352:r: |   |-> populate dot11ConfigTrapSwitchTable data context.
				 * Populate data context here. (optionally, delay until row prep)
				 */
			/*
			 * TRANSIENT or semi-TRANSIENT data:
			 * copy data or save any info needed to do it in row_prep.
			 */
			/*
			 * setup/save data for APInterfThreshhd
			 * APInterfThreshhd(2)/INTEGER/ASN_INTEGER/long(long)//l/A/W/e/r/d/h
			 */
			/*
			 * TODO:246:r: |-> Define APInterfThreshhd mapping.
			 * Map values between raw/native values and MIB values
			 *
			 * Integer based value can usually just do a direct copy.
			 */
			memcpy(&(rowreq_ctx->data.parameter), &(messageNode->parameter), sizeof(dbus_parameter));
			rowreq_ctx->data.wtpCurrID = q->WTPID;
			rowreq_ctx->data.APInterfThreshhd = show_node->rogue_ap_threshold;
			
			/*
			 * setup/save data for StaInterfThreshhd
			 * StaInterfThreshhd(3)/INTEGER/ASN_INTEGER/long(long)//l/A/W/e/r/d/h
			 */
			/*
			 * TODO:246:r: |-> Define StaInterfThreshhd mapping.
			 * Map values between raw/native values and MIB values
			 *
			 * Integer based value can usually just do a direct copy.
			 */
			rowreq_ctx->data.StaInterfThreshhd = show_node->rogue_termi_threshold;
			
			/*
			 * setup/save data for CPUusageThreshhd
			 * CPUusageThreshhd(4)/INTEGER/ASN_INTEGER/long(long)//l/A/W/e/R/d/h
			 */
			/*
			 * TODO:246:r: |-> Define CPUusageThreshhd mapping.
			 * Map values between raw/native values and MIB values
			 *
			 * Integer based value can usually just do a direct copy.
			 */
			rowreq_ctx->data.CPUusageThreshhd = show_node->cpu;
			
			/*
			 * setup/save data for MemUsageThreshhd
			 * MemUsageThreshhd(5)/INTEGER/ASN_INTEGER/long(long)//l/A/W/e/R/d/h
			 */
			/*
			 * TODO:246:r: |-> Define MemUsageThreshhd mapping.
			 * Map values between raw/native values and MIB values
			 *
			 * Integer based value can usually just do a direct copy.
			 */
			rowreq_ctx->data.MemUsageThreshhd = show_node->memoryuse;
			
			/*
			 * setup/save data for CoChAPInterfThreshhd
			 * CoChAPInterfThreshhd(6)/INTEGER/ASN_INTEGER/long(long)//l/A/W/e/R/d/h
			 */
			/*
			 * TODO:246:r: |-> Define CoChAPInterfThreshhd mapping.
			 * Map values between raw/native values and MIB values
			 *
			 * Integer based value can usually just do a direct copy.
			 */
			rowreq_ctx->data.CoChAPInterfThreshhd = show_node->samechannelrssi_theshold;
			
			/*
			 * setup/save data for AdjacentChAPInterfThreshhd
			 * AdjacentChAPInterfThreshhd(7)/INTEGER/ASN_INTEGER/long(long)//l/A/W/e/R/d/h
			 */
			/*
			 * TODO:246:r: |-> Define AdjacentChAPInterfThreshhd mapping.
			 * Map values between raw/native values and MIB values
			 *
			 * Integer based value can usually just do a direct copy.
			 */
			rowreq_ctx->data.AdjacentChAPInterfThreshhd = show_node->neighborchannelrssi_theshold;
			
				
				/*
				 * insert into table container
				 */
    				    if(CONTAINER_INSERT(container, rowreq_ctx))
        				{
        					dot11ConfigTrapSwitchTable_release_rowreq_ctx(rowreq_ctx);
        				}
        				++count;
        				
        				Free_show_ap_trap_threshold(show_node);
        			}
					FREE_OBJECT(q->WTPMAC);
        		}
        	}
    	}
        free_dbus_message_list(&messageHead, Free_wtp_list_by_mac_head);
	}


    /*
    ***************************************************
    ***             START EXAMPLE CODE              ***
    ***---------------------------------------------***/
//    if(NULL != filep)
//        fclose(filep);
    /*
    ***---------------------------------------------***
    ***              END  EXAMPLE CODE              ***
    ***************************************************/

    DEBUGMSGT(("verbose:dot11ConfigTrapSwitchTable:dot11ConfigTrapSwitchTable_cache_load",
               "inserted %d records\n", count));

	snmp_log(LOG_DEBUG, "exit dot11ConfigTrapSwitchTable_cache_load\n");
    return MFD_SUCCESS;
} /* dot11ConfigTrapSwitchTable_cache_load */

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
dot11ConfigTrapSwitchTable_cache_free(netsnmp_container *container)
{
    DEBUGMSGTL(("verbose:dot11ConfigTrapSwitchTable:dot11ConfigTrapSwitchTable_cache_free","called\n"));

    /*
     * TODO:380:M: Free dot11ConfigTrapSwitchTable cache.
     */
} /* dot11ConfigTrapSwitchTable_cache_free */

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
dot11ConfigTrapSwitchTable_row_prep( dot11ConfigTrapSwitchTable_rowreq_ctx *rowreq_ctx)
{
    DEBUGMSGTL(("verbose:dot11ConfigTrapSwitchTable:dot11ConfigTrapSwitchTable_row_prep","called\n"));

    netsnmp_assert(NULL != rowreq_ctx);

    /*
     * TODO:390:o: Prepare row for request.
     * If populating row data was delayed, this is the place to
     * fill in the row for this request.
     */

    return MFD_SUCCESS;
} /* dot11ConfigTrapSwitchTable_row_prep */

/** @} */
