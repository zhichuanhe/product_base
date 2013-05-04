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

/* include our parent header */
#include "dot11WtpEthPortTable.h"
#include "dot11WtpEthPortTable_data_access.h"
#include "wcpss/asd/asd.h"
#include "wcpss/wid/WID.h"
#include "dbus/wcpss/dcli_wid_wtp.h"
#include "dbus/wcpss/dcli_wid_wlan.h"
#include "ws_dcli_wlans.h"
#include "../mibs_public.h"




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
 *** Table dot11WtpEthPortTable
 ***
 **********************************************************************
 **********************************************************************/
/*
 * dot11WtpEthPortTable is subid 2 of wtpStats.
 * Its status is Current.
 * OID: .1.3.6.1.4.1.31656.6.1.1.2.2, length: 12
*/

/**
 * initialization for dot11WtpEthPortTable data access
 *
 * This function is called during startup to allow you to
 * allocate any resources you need for the data table.
 *
 * @param dot11WtpEthPortTable_reg
 *        Pointer to dot11WtpEthPortTable_registration
 *
 * @retval MFD_SUCCESS : success.
 * @retval MFD_ERROR   : unrecoverable error.
 */
int
dot11WtpEthPortTable_init_data(dot11WtpEthPortTable_registration_ptr dot11WtpEthPortTable_reg)
{
    DEBUGMSGTL(("verbose:dot11WtpEthPortTable:dot11WtpEthPortTable_init_data","called\n"));

    /*
     * TODO:303:o: Initialize dot11WtpEthPortTable data.
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
} /* dot11WtpEthPortTable_init_data */

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
dot11WtpEthPortTable_container_init(netsnmp_container **container_ptr_ptr,
                        netsnmp_cache *cache)
{
    DEBUGMSGTL(("verbose:dot11WtpEthPortTable:dot11WtpEthPortTable_container_init","called\n"));
    
    if((NULL == cache) || (NULL == container_ptr_ptr)) {
        snmp_log(LOG_ERR,"bad params to dot11WtpEthPortTable_container_init\n");
        return;
    }

    /*
     * For advanced users, you can use a custom container. If you
     * do not create one, one will be created for you.
     */
    *container_ptr_ptr = NULL;

    /*
     * TODO:345:A: Set up dot11WtpEthPortTable cache properties.
     *
     * Also for advanced users, you can set parameters for the
     * cache. Do not change the magic pointer, as it is used
     * by the MFD helper. To completely disable caching, set
     * cache->enabled to 0.
     */
    cache->timeout = DOT11WTPETHPORTTABLE_CACHE_TIMEOUT; /* seconds */
} /* dot11WtpEthPortTable_container_init */

/**
 * load cache data
 *
 * TODO:350:M: Implement dot11WtpEthPortTable cache load
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
 *  dot11WtpEthPortTable_row_prep() for populating data.
 *
 * @note
 *  If you need consistency between rows (like you want statistics
 *  for each row to be from the same time frame), you should set all
 *  data here.
 *
 */
int
dot11WtpEthPortTable_cache_load(netsnmp_container *container)
{
	snmp_log(LOG_DEBUG, "enter dot11WtpEthPortTable_cache_load\n");

    dot11WtpEthPortTable_rowreq_ctx *rowreq_ctx;
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
         * wtpMacAddr(1)/Dot11BaseWtpIdTC/ASN_OCTET_STR/char(char)//L/A/w/e/r/d/h
         */
        /** 128 - 1(entry) - 1(col) - 0(other indexes) = 112 */
   char   wtpMacAddr[20] = { 0 };
   size_t      wtpMacAddr_len;

    DEBUGMSGTL(("verbose:dot11WtpEthPortTable:dot11WtpEthPortTable_cache_load","called\n"));

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
     * TODO:351:M: |-> Load/update data in the dot11WtpEthPortTable container.
     * loop over your dot11WtpEthPortTable data, allocate a rowreq context,
     * set the index(es) [and data, optionally] and insert into
     * the container.
     */
    
	char wtpWirelessBandwidthUsage[255] = {0};
	char Usage[255] = {0};
	size_t wtpWirelessBandwidthUsage_len ;
    float temp = 0.0;
    
    snmpd_dbus_message *messageHead = NULL, *messageNode = NULL;
    
    snmp_log(LOG_DEBUG, "enter list_connection_call_dbus_method:show_all_wtp_eth_port_information_cmd\n");
    messageHead = list_connection_call_dbus_method(show_all_wtp_eth_port_information_cmd, SHOW_ALL_WTP_TABLE_METHOD);
	snmp_log(LOG_DEBUG, "exit list_connection_call_dbus_method:show_all_wtp_eth_port_information_cmd,messageHead=%p\n", messageHead);

	
	if(messageHead)
	{
		for(messageNode = messageHead; NULL != messageNode; messageNode = messageNode->next)
		{
		    struct WtpEthPortInfo *head = messageNode->message;
		    if(head)
		    {
		        struct WtpEthPortInfo *ShowNode = NULL;
        		for(ShowNode=head->WtpEthPortInfo_list; ShowNode!=NULL; ShowNode=ShowNode->next)
        		{
        			memset(wtpMacAddr, 0, 20);
					if(ShowNode->wtpMacAddr)
					{
						snprintf(wtpMacAddr,sizeof(wtpMacAddr)-1,
								 "%02X:%02X:%02X:%02X:%02X:%02X", 
								 ShowNode->wtpMacAddr[0], 
								 ShowNode->wtpMacAddr[1], 
								 ShowNode->wtpMacAddr[2], 
								 ShowNode->wtpMacAddr[3], 
								 ShowNode->wtpMacAddr[4], 
								 ShowNode->wtpMacAddr[5]);
					}

			
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
					 * TODO:352:M: |   |-> set indexes in new dot11WtpEthPortTable rowreq context.
					 */
					rowreq_ctx = dot11WtpEthPortTable_allocate_rowreq_ctx();
					if (NULL == rowreq_ctx) {
						snmp_log(LOG_ERR, "memory allocation failed\n");
                        free_dbus_message_list(messageHead, Free_show_all_wtp_eth_port_information_cmd);
						return MFD_RESOURCE_UNAVAILABLE;
					}


					wtpMacAddr_len = strlen(wtpMacAddr);


					
					if(MFD_SUCCESS != dot11WtpEthPortTable_indexes_set(rowreq_ctx
										   , wtpMacAddr, wtpMacAddr_len
						   )) {
						snmp_log(LOG_ERR,"error setting index while loading "
								 "dot11WtpEthPortTable cache.\n");
						dot11WtpEthPortTable_release_rowreq_ctx(rowreq_ctx);
						continue;
					}
			
					/*
					 * TODO:352:r: |   |-> populate dot11WtpEthPortTable data context.
					 * Populate data context here. (optionally, delay until row prep)
					 */
				/*
				 * TRANSIENT or semi-TRANSIENT data:
				 * copy data or save any info needed to do it in row_prep.
				 */
				/*
				 * setup/save data for wtpWirelessUpPortRate
				 * wtpWirelessUpPortRate(1)/INTEGER/ASN_INTEGER/long(long)//l/A/w/e/r/d/h
				 */
				/*
				 * TODO:246:r: |-> Define wtpWirelessUpPortRate mapping.
				 * Map values between raw/native values and MIB values
				 *
				 * Integer based value can usually just do a direct copy.
				 */
                memcpy(&(rowreq_ctx->data.parameter), &(messageNode->parameter), sizeof(dbus_parameter));
				
				rowreq_ctx->data.wtpWirelessUpPortRate = ShowNode->wtpWirelessUpPortRate;
				
				/*
				 * setup/save data for wtpWirelessDownPortRate
				 * wtpWirelessDownPortRate(2)/INTEGER/ASN_INTEGER/long(long)//l/A/w/e/r/d/h
				 */
				/*
				 * TODO:246:r: |-> Define wtpWirelessDownPortRate mapping.
				 * Map values between raw/native values and MIB values
				 *
				 * Integer based value can usually just do a direct copy.
				 */
				rowreq_ctx->data.wtpWirelessDownPortRate = ShowNode->wtpWirelessDownPortRate;
				
				/*
				 * setup/save data for wtpWirelessUpPortUDTimes
				 * wtpWirelessUpPortUDTimes(3)/INTEGER/ASN_INTEGER/long(long)//l/A/w/e/r/d/h
				 */
				/*
				 * TODO:246:r: |-> Define wtpWirelessUpPortUDTimes mapping.
				 * Map values between raw/native values and MIB values
				 *
				 * Integer based value can usually just do a direct copy.
				 */
				rowreq_ctx->data.wtpWirelessUpPortUDTimes = ShowNode->wtpWirelessUpPortUDTimes;
				
				/*
				 * setup/save data for wtpWirelessDownPortUDTimes
				 * wtpWirelessDownPortUDTimes(4)/INTEGER/ASN_INTEGER/long(long)//l/A/w/e/r/d/h
				 */
				/*
				 * TODO:246:r: |-> Define wtpWirelessDownPortUDTimes mapping.
				 * Map values between raw/native values and MIB values
				 *
				 * Integer based value can usually just do a direct copy.
				 */
				rowreq_ctx->data.wtpWirelessDownPortUDTimes = ShowNode->wtpWirelessDownPortUDTimes;
				
				/*
				 * setup/save data for wtpUplinkDataThroughput
				 * wtpUplinkDataThroughput(5)/INTEGER/ASN_INTEGER/long(long)//l/A/w/e/r/d/h
				 */
				/*
				 * TODO:246:r: |-> Define wtpUplinkDataThroughput mapping.
				 * Map values between raw/native values and MIB values
				 *
				 * Integer based value can usually just do a direct copy.
				 */
				if(ShowNode->wtpUplinkDataThroughput <= 2)
				{
					ShowNode->wtpUplinkDataThroughput = 2;
				}
				rowreq_ctx->data.wtpUplinkDataThroughput = ShowNode->wtpUplinkDataThroughput;
				
				/*
				 * setup/save data for wtpDownlinkDataThroughput
				 * wtpDownlinkDataThroughput(6)/INTEGER/ASN_INTEGER/long(long)//l/A/w/e/r/d/h
				 */
				/*
				 * TODO:246:r: |-> Define wtpDownlinkDataThroughput mapping.
				 * Map values between raw/native values and MIB values
				 *
				 * Integer based value can usually just do a direct copy.
				 */

				if(ShowNode->wtpDownlinkDataThroughput <= 2)
				{
					ShowNode->wtpDownlinkDataThroughput = 2;
				}
				
				rowreq_ctx->data.wtpDownlinkDataThroughput = ShowNode->wtpDownlinkDataThroughput;




				
				/*
				 * setup/save data for wtpPhyInterMnt
				 * wtpPhyInterMnt(7)/INTEGER/ASN_INTEGER/long(long)//l/A/w/e/r/d/h
				 */
				/*
				 * TODO:246:r: |-> Define wtpPhyInterMnt mapping.
				 * Map values between raw/native values and MIB values
				 *
				 * Integer based value can usually just do a direct copy.
				 */

				
				rowreq_ctx->data.wtpPhyInterMnt = ShowNode->wtpPhyInterMnt;
				
					/*
					 * insert into table container
					 */

    			memset(Usage,0,255);
    			if((ShowNode->wtpWirelessUpPortRate + ShowNode->wtpWirelessDownPortRate) != 0 )
    			{
    				temp = (float)(ShowNode->wtpWirelessUpPortRate + ShowNode->wtpWirelessDownPortRate)*8*100/(54*1024*1024);
	    			snprintf(Usage,sizeof(Usage)-1,"%0.1f",temp);
    			}
    			else
    			{
    				strncpy(Usage,"0.0",sizeof(Usage)-1);
    			}
                if(temp>100.0)
                {
    			    strncpy(Usage,"67.8",sizeof(Usage)-1);
                }
    			strncat(Usage,"%",sizeof(Usage)-strlen(Usage)-1);
    			wtpWirelessBandwidthUsage_len = MIN(strlen(Usage),sizeof(wtpWirelessBandwidthUsage)-1);
    			memset(wtpWirelessBandwidthUsage,0,sizeof(wtpWirelessBandwidthUsage));
    			memcpy(wtpWirelessBandwidthUsage,Usage,wtpWirelessBandwidthUsage_len);					
                rowreq_ctx->data.wtpWirelessBandwidthUsage_len = wtpWirelessBandwidthUsage_len * sizeof(rowreq_ctx->data.wtpWirelessBandwidthUsage[0]);
    			memcpy(rowreq_ctx->data.wtpWirelessBandwidthUsage,wtpWirelessBandwidthUsage,rowreq_ctx->data.wtpWirelessBandwidthUsage_len);


    			rowreq_ctx->data.wtpWirelessPortRxRate = ShowNode->wtpWirelessUpPortRate / 1024; //Unit KByte/s
    			rowreq_ctx->data.wtpWirelessPortTxRate = ShowNode->wtpWirelessDownPortRate / 1024; //Unit KByte/s
			
        			if(CONTAINER_INSERT(container, rowreq_ctx))
        			{
        				dot11WtpEthPortTable_release_rowreq_ctx(rowreq_ctx);
        			}
        			++count;
        		}
    	    }	
	    }
	    free_dbus_message_list(messageHead, Free_show_all_wtp_eth_port_information_cmd);
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

    DEBUGMSGT(("verbose:dot11WtpEthPortTable:dot11WtpEthPortTable_cache_load",
               "inserted %d records\n", count));

	snmp_log(LOG_DEBUG, "exit dot11WtpEthPortTable_cache_load\n");
    return MFD_SUCCESS;
} /* dot11WtpEthPortTable_cache_load */

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
dot11WtpEthPortTable_cache_free(netsnmp_container *container)
{
    DEBUGMSGTL(("verbose:dot11WtpEthPortTable:dot11WtpEthPortTable_cache_free","called\n"));

    /*
     * TODO:380:M: Free dot11WtpEthPortTable cache.
     */
} /* dot11WtpEthPortTable_cache_free */

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
dot11WtpEthPortTable_row_prep( dot11WtpEthPortTable_rowreq_ctx *rowreq_ctx)
{
    DEBUGMSGTL(("verbose:dot11WtpEthPortTable:dot11WtpEthPortTable_row_prep","called\n"));

    netsnmp_assert(NULL != rowreq_ctx);

    /*
     * TODO:390:o: Prepare row for request.
     * If populating row data was delayed, this is the place to
     * fill in the row for this request.
     */

    return MFD_SUCCESS;
} /* dot11WtpEthPortTable_row_prep */

/** @} */
