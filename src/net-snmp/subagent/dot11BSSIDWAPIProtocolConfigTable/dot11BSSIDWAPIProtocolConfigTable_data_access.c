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
#include "dot11BSSIDWAPIProtocolConfigTable.h"


#include "dot11BSSIDWAPIProtocolConfigTable_data_access.h"
#include "wcpss/asd/asd.h"
#include "ws_sta.h"
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
 *** Table dot11BSSIDWAPIProtocolConfigTable
 ***
 **********************************************************************
 **********************************************************************/
/*
 * dot11BSSIDWAPIProtocolConfigTable is subid 2 of wtpWAPI.
 * Its status is Current.
 * OID: .1.3.6.1.4.1.31656.6.1.1.10.2, length: 12
*/

/**
 * initialization for dot11BSSIDWAPIProtocolConfigTable data access
 *
 * This function is called during startup to allow you to
 * allocate any resources you need for the data table.
 *
 * @param dot11BSSIDWAPIProtocolConfigTable_reg
 *        Pointer to dot11BSSIDWAPIProtocolConfigTable_registration
 *
 * @retval MFD_SUCCESS : success.
 * @retval MFD_ERROR   : unrecoverable error.
 */
int
dot11BSSIDWAPIProtocolConfigTable_init_data(dot11BSSIDWAPIProtocolConfigTable_registration_ptr dot11BSSIDWAPIProtocolConfigTable_reg)
{
    DEBUGMSGTL(("verbose:dot11BSSIDWAPIProtocolConfigTable:dot11BSSIDWAPIProtocolConfigTable_init_data","called\n"));

    /*
     * TODO:303:o: Initialize dot11BSSIDWAPIProtocolConfigTable data.
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
} /* dot11BSSIDWAPIProtocolConfigTable_init_data */

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
dot11BSSIDWAPIProtocolConfigTable_container_init(netsnmp_container **container_ptr_ptr,
                        netsnmp_cache *cache)
{
    DEBUGMSGTL(("verbose:dot11BSSIDWAPIProtocolConfigTable:dot11BSSIDWAPIProtocolConfigTable_container_init","called\n"));
    
    if((NULL == cache) || (NULL == container_ptr_ptr)) {
        snmp_log(LOG_ERR,"bad params to dot11BSSIDWAPIProtocolConfigTable_container_init\n");
        return;
    }

    /*
     * For advanced users, you can use a custom container. If you
     * do not create one, one will be created for you.
     */
    *container_ptr_ptr = NULL;

    /*
     * TODO:345:A: Set up dot11BSSIDWAPIProtocolConfigTable cache properties.
     *
     * Also for advanced users, you can set parameters for the
     * cache. Do not change the magic pointer, as it is used
     * by the MFD helper. To completely disable caching, set
     * cache->enabled to 0.
     */
    cache->timeout = DOT11BSSIDWAPIPROTOCOLCONFIGTABLE_CACHE_TIMEOUT; /* seconds */
} /* dot11BSSIDWAPIProtocolConfigTable_container_init */

/**
 * load cache data
 *
 * TODO:350:M: Implement dot11BSSIDWAPIProtocolConfigTable cache load
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
 *  dot11BSSIDWAPIProtocolConfigTable_row_prep() for populating data.
 *
 * @note
 *  If you need consistency between rows (like you want statistics
 *  for each row to be from the same time frame), you should set all
 *  data here.
 *
 */
int
dot11BSSIDWAPIProtocolConfigTable_cache_load(netsnmp_container *container)
{
	snmp_log(LOG_DEBUG, "enter dot11BSSIDWAPIProtocolConfigTable_cache_load\n");

    dot11BSSIDWAPIProtocolConfigTable_rowreq_ctx *rowreq_ctx;
    size_t                 count = 0;
    
    /*
     * this example code is based on a data source that is a
     * text file to be read and parsed.
     */
    //FILE *filep;
    //char line[MAX_LINE_SIZE];
    /*
     * temporary storage for index values
     */
        /*
         * wtpMacAddr(1)/DisplayString/ASN_OCTET_STR/char(char)//L/A/w/e/R/d/H
         */
        /** 128 - 1(entry) - 1(col) - 1(other indexes) = 111 */
   char   wtpMacAddr[20] = { 0 };
   size_t      wtpMacAddr_len = 0;
        /*
         * wtpBssCurrID(1)/DisplayString/ASN_OCTET_STR/char(char)//L/A/w/e/R/d/H
         */
        /** 128 - 1(entry) - 1(col) - 1(other indexes) = 111 */
   char   wtpBssCurrID[20] = { 0 };
   size_t      wtpBssCurrID_len = 0;

    DEBUGMSGTL(("verbose:dot11BSSIDWAPIProtocolConfigTable:dot11BSSIDWAPIProtocolConfigTable_cache_load","called\n"));

    /*
    ***************************************************
    ***             START EXAMPLE CODE              ***
    ***---------------------------------------------***/
    /*
     * open our data file.
     */
    //filep = fopen("/etc/dummy.conf", "r");
    //if(NULL ==  filep) {
        //return MFD_RESOURCE_UNAVAILABLE;
    //}

    /*
    ***---------------------------------------------***
    ***              END  EXAMPLE CODE              ***
    ***************************************************/
    /*
     * TODO:351:M: |-> Load/update data in the dot11BSSIDWAPIProtocolConfigTable container.
     * loop over your dot11BSSIDWAPIProtocolConfigTable data, allocate a rowreq context,
     * set the index(es) [and data, optionally] and insert into
     * the container.
     */
    instance_parameter *para_head = NULL, *para_node = NULL;
    list_instance_parameter(&para_head, SNMPD_INSTANCE_MASTER);	
    for(para_node = para_head; NULL != para_node; para_node = para_node->next)
    {
		int ret = 0;	
		struct dcli_wtp_info *wtp = NULL;
		struct dcli_wtp_info *tmp = NULL;
		struct dcli_bss_info *bss = NULL;
		int i = 0;
		
		snmp_log(LOG_DEBUG, "enter show_wapi_mib_info_of_all_wtp_cmd\n");
		ret = show_wapi_mib_info_of_all_wtp_cmd(para_node->parameter, para_node->connection,&wtp);	
		snmp_log(LOG_DEBUG, "exit show_wapi_mib_info_of_all_wtp_cmd,ret=%d\n", ret);
		if(ret == 1)
		{
			for(tmp = wtp; (NULL != tmp); tmp = tmp->next)
			{
				memset(wtpMacAddr,0,sizeof(wtpMacAddr));
				snprintf(wtpMacAddr,sizeof(wtpMacAddr)-1,
						 "%02X:%02X:%02X:%02X:%02X:%02X",
						 tmp->mac[0],tmp->mac[1],
						 tmp->mac[2],tmp->mac[3],
						 tmp->mac[4],tmp->mac[5]);

				for(i=0,bss = tmp->bss_list; 
					((i<tmp->num_bss)&&(NULL != bss)); 
					i++,bss = bss->next)
				{
					memset(wtpBssCurrID,0,sizeof(wtpBssCurrID));
					snprintf(wtpBssCurrID,sizeof(wtpBssCurrID)-1,
							 "%02X:%02X:%02X:%02X:%02X:%02X",
							 bss->bssid[0],bss->bssid[1],bss->bssid[2],
							 bss->bssid[3],bss->bssid[4],bss->bssid[5]);
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
	    //if(NULL == filep)
	      //  break;

	    /*
	     * parse line into variables
	     */
	    /*
	    ***---------------------------------------------***
	    ***              END  EXAMPLE CODE              ***
	    ***************************************************/

	        /*
	         * TODO:352:M: |   |-> set indexes in new dot11BSSIDWAPIProtocolConfigTable rowreq context.
	         */
	        rowreq_ctx = dot11BSSIDWAPIProtocolConfigTable_allocate_rowreq_ctx();
	        if (NULL == rowreq_ctx) {
	            snmp_log(LOG_ERR, "memory allocation failed\n");
				Free_show_wapi_mib_info_of_all_wtp_cmd(wtp);
				free_instance_parameter_list(&para_head);
	            return MFD_RESOURCE_UNAVAILABLE;
	        }
			
			wtpMacAddr_len = strlen(wtpMacAddr);		
			wtpBssCurrID_len = strlen(wtpBssCurrID);
	        if(MFD_SUCCESS != dot11BSSIDWAPIProtocolConfigTable_indexes_set(rowreq_ctx
	                               , wtpMacAddr, wtpMacAddr_len
	                               , wtpBssCurrID, wtpBssCurrID_len
	               )) {
	            snmp_log(LOG_ERR,"error setting index while loading "
	                     "dot11BSSIDWAPIProtocolConfigTable cache.\n");
	            dot11BSSIDWAPIProtocolConfigTable_release_rowreq_ctx(rowreq_ctx);
	            continue;
	        }

	        /*
	         * TODO:352:r: |   |-> populate dot11BSSIDWAPIProtocolConfigTable data context.
	         * Populate data context here. (optionally, delay until row prep)
	         */
	    /*
	     * TRANSIENT or semi-TRANSIENT data:
	     * copy data or save any info needed to do it in row_prep.
	     */
	    /*
	     * setup/save data for wapiControlledAuthControl
	     * wapiControlledAuthControl(1)/TruthValue/ASN_INTEGER/long(u_long)//l/A/w/E/r/d/h
	     */
	    /*
	     * TODO:246:r: |-> Define wapiControlledAuthControl mapping.
	     * Map values between raw/native values and MIB values
	     *
	    * enums usually need mapping.
	    */   
	    
		memcpy(&(rowreq_ctx->data.parameter), &(para_node->parameter), sizeof(dbus_parameter));
			
	    /*if(MFD_SUCCESS !=
	       wapiControlledAuthControl_map(&rowreq_ctx->data.wapiControlledAuthControl, wapiControlledAuthControl )) {
	        return MFD_ERROR;
	    }*/
	    if((bss->wapiEnabled == 1)&&(bss->ControlledAuthControl == 1))
			rowreq_ctx->data.wapiControlledAuthControl = 1;
		else
			rowreq_ctx->data.wapiControlledAuthControl = 2;
	    
	    /*
	     * setup/save data for wapiControlledPortControl
	     * wapiControlledPortControl(2)/INTEGER/ASN_INTEGER/long(u_long)//l/A/w/E/r/d/h
	     */
	    /*
	     * TODO:246:r: |-> Define wapiControlledPortControl mapping.
	     * Map values between raw/native values and MIB values
	     *
	    * enums usually need mapping.
	    */
	    /*if(MFD_SUCCESS !=
	       wapiControlledPortControl_map(&rowreq_ctx->data.wapiControlledPortControl, wapiControlledPortControl )) {
	        return MFD_ERROR;
	    }*/
	    rowreq_ctx->data.wapiControlledPortControl = bss->ControlledPortControl;
	    
	    /*
	     * setup/save data for wapiEnabled
	     * wapiEnabled(3)/TruthValue/ASN_INTEGER/long(u_long)//l/A/W/E/r/d/h
	     */
	    /*
	     * TODO:246:r: |-> Define wapiEnabled mapping.
	     * Map values between raw/native values and MIB values
	     *
	    * enums usually need mapping.
	    */
	    /*if(MFD_SUCCESS !=
	       wapiEnabled_map(&rowreq_ctx->data.wapiEnabled, wapiEnabled )) {
	        return MFD_ERROR;
	    }*/
	    if(bss->wapiEnabled == 1)
	    	rowreq_ctx->data.wapiEnabled = 1;
		else
			rowreq_ctx->data.wapiEnabled = 2;
	    
	    /*
	     * setup/save data for wapiCertificateUpdateCount
	     * wapiCertificateUpdateCount(4)/UNSIGNED32/ASN_UNSIGNED/u_long(u_long)//l/A/W/e/r/d/h
	     */
	    /*
	     * TODO:246:r: |-> Define wapiCertificateUpdateCount mapping.
	     * Map values between raw/native values and MIB values
	     *
	     * Integer based value can usually just do a direct copy.
	     */
	    rowreq_ctx->data.wapiCertificateUpdateCount = bss->CertificateUpdateCount;
	    
	    /*
	     * setup/save data for wapiMulticastUpdateCount
	     * wapiMulticastUpdateCount(5)/UNSIGNED32/ASN_UNSIGNED/u_long(u_long)//l/A/W/e/r/d/h
	     */
	    /*
	     * TODO:246:r: |-> Define wapiMulticastUpdateCount mapping.
	     * Map values between raw/native values and MIB values
	     *
	     * Integer based value can usually just do a direct copy.
	     */
	    rowreq_ctx->data.wapiMulticastUpdateCount = bss->MulticastUpdateCount;
	    
	    /*
	     * setup/save data for wapiUnicastUpdateCount
	     * wapiUnicastUpdateCount(6)/UNSIGNED32/ASN_UNSIGNED/u_long(u_long)//l/A/W/e/r/d/h
	     */
	    /*
	     * TODO:246:r: |-> Define wapiUnicastUpdateCount mapping.
	     * Map values between raw/native values and MIB values
	     *
	     * Integer based value can usually just do a direct copy.
	     */
	    rowreq_ctx->data.wapiUnicastUpdateCount = bss->UnicastUpdateCount;
	    
	    /*
	     * setup/save data for wapiAuthSuiteSelected
	     * wapiAuthSuiteSelected(7)/INTEGER/ASN_INTEGER/long(long)//l/A/w/e/r/d/h
	     */
	    /*
	     * TODO:246:r: |-> Define wapiAuthSuiteSelected mapping.
	     * Map values between raw/native values and MIB values
	     *
	     * Integer based value can usually just do a direct copy.
	     */
	    rowreq_ctx->data.wapiAuthSuiteSelected = bss->AuthSuiteSelected;
	    
	    /*
	     * setup/save data for wapiAuthSuiteRequested
	     * wapiAuthSuiteRequested(8)/INTEGER/ASN_INTEGER/long(long)//l/A/w/e/r/d/h
	     */
	    /*
	     * TODO:246:r: |-> Define wapiAuthSuiteRequested mapping.
	     * Map values between raw/native values and MIB values
	     *
	     * Integer based value can usually just do a direct copy.
	     */
	    rowreq_ctx->data.wapiAuthSuiteRequested = bss->AuthenticationSuite;
	    
	        
	        /*
	         * insert into table container
	         */
	        if(CONTAINER_INSERT(container, rowreq_ctx))		
			{
				dot11BSSIDWAPIProtocolConfigTable_release_rowreq_ctx(rowreq_ctx);
			}
	        ++count;
	    		}
			}
		}
		Free_show_wapi_mib_info_of_all_wtp_cmd(wtp);
	}
	free_instance_parameter_list(&para_head);


    /*
    ***************************************************
    ***             START EXAMPLE CODE              ***
    ***---------------------------------------------***/
    //if(NULL != filep)
      //  fclose(filep);
    /*
    ***---------------------------------------------***
    ***              END  EXAMPLE CODE              ***
    ***************************************************/

    DEBUGMSGT(("verbose:dot11BSSIDWAPIProtocolConfigTable:dot11BSSIDWAPIProtocolConfigTable_cache_load",
               "inserted %d records\n", count));

	snmp_log(LOG_DEBUG, "exit dot11BSSIDWAPIProtocolConfigTable_cache_load\n");
    return MFD_SUCCESS;
} /* dot11BSSIDWAPIProtocolConfigTable_cache_load */

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
dot11BSSIDWAPIProtocolConfigTable_cache_free(netsnmp_container *container)
{
    DEBUGMSGTL(("verbose:dot11BSSIDWAPIProtocolConfigTable:dot11BSSIDWAPIProtocolConfigTable_cache_free","called\n"));

    /*
     * TODO:380:M: Free dot11BSSIDWAPIProtocolConfigTable cache.
     */
} /* dot11BSSIDWAPIProtocolConfigTable_cache_free */

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
dot11BSSIDWAPIProtocolConfigTable_row_prep( dot11BSSIDWAPIProtocolConfigTable_rowreq_ctx *rowreq_ctx)
{
    DEBUGMSGTL(("verbose:dot11BSSIDWAPIProtocolConfigTable:dot11BSSIDWAPIProtocolConfigTable_row_prep","called\n"));

    netsnmp_assert(NULL != rowreq_ctx);

    /*
     * TODO:390:o: Prepare row for request.
     * If populating row data was delayed, this is the place to
     * fill in the row for this request.
     */

    return MFD_SUCCESS;
} /* dot11BSSIDWAPIProtocolConfigTable_row_prep */

/** @} */
