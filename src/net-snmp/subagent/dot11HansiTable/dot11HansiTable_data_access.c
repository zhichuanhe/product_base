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
#include "dot11HansiTable.h"

#include "mibs_public.h"
#include "autelanWtpGroup.h"
#include "dot11HansiTable_data_access.h"
#include "ws_dbus_def.h"
#include "hmd/hmdpub.h"
#include "ws_local_hansi.h"
#include "dbus/hmd/HmdDbusDef.h"
#include "ws_local_hansi.h"

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
 *** Table dot11HansiTable
 ***
 **********************************************************************
 **********************************************************************/
/*
 * dot11HansiTable is subid 1 of dot11Hansi.
 * Its status is Current.
 * OID: .1.3.6.1.4.1.31656.6.1.2.29.1, length: 12
*/

/**
 * initialization for dot11HansiTable data access
 *
 * This function is called during startup to allow you to
 * allocate any resources you need for the data table.
 *
 * @param dot11HansiTable_reg
 *        Pointer to dot11HansiTable_registration
 *
 * @retval MFD_SUCCESS : success.
 * @retval MFD_ERROR   : unrecoverable error.
 */

static void 
ipaddr_int_to_str(int ip, char *ipaddr) {
    
    char temp_ip[10] = { 0 };
    snprintf(temp_ip, sizeof(temp_ip) - 1, "%d",(ip & 0xFF000000) >> 24);
    strcat(ipaddr, temp_ip);
    
    memset(temp_ip, 0, sizeof(temp_ip));
    snprintf(temp_ip, sizeof(temp_ip) - 1, ".%d",(ip & 0xFF0000) >> 16);
    strcat(ipaddr, temp_ip);
    
    memset(temp_ip, 0, sizeof(temp_ip));
    snprintf(temp_ip, sizeof(temp_ip) - 1, ".%d",(ip & 0xFF00) >> 8);
    strcat(ipaddr, temp_ip);
    
    memset(temp_ip, 0, sizeof(temp_ip));
    snprintf(temp_ip, sizeof(temp_ip) - 1, ".%d",(ip & 0xFF));
    strcat(ipaddr, temp_ip);  
}
 
int
dot11HansiTable_init_data(dot11HansiTable_registration_ptr dot11HansiTable_reg)
{
    DEBUGMSGTL(("verbose:dot11HansiTable:dot11HansiTable_init_data","called\n"));

    /*
     * TODO:303:o: Initialize dot11HansiTable data.
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
} /* dot11HansiTable_init_data */

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
dot11HansiTable_container_init(netsnmp_container **container_ptr_ptr,
                        netsnmp_cache *cache)
{
    DEBUGMSGTL(("verbose:dot11HansiTable:dot11HansiTable_container_init","called\n"));
    
    if((NULL == cache) || (NULL == container_ptr_ptr)) {
        snmp_log(LOG_ERR,"bad params to dot11HansiTable_container_init\n");
        return;
    }

    /*
     * For advanced users, you can use a custom container. If you
     * do not create one, one will be created for you.
     */
    *container_ptr_ptr = NULL;

    /*
     * TODO:345:A: Set up dot11HansiTable cache properties.
     *
     * Also for advanced users, you can set parameters for the
     * cache. Do not change the magic pointer, as it is used
     * by the MFD helper. To completely disable caching, set
     * cache->enabled to 0.
     */
    cache->timeout = DOT11HANSITABLE_CACHE_TIMEOUT; /* seconds */
} /* dot11HansiTable_container_init */

/**
 * load cache data
 *
 * TODO:350:M: Implement dot11HansiTable cache load
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
 *  dot11HansiTable_row_prep() for populating data.
 *
 * @note
 *  If you need consistency between rows (like you want statistics
 *  for each row to be from the same time frame), you should set all
 *  data here.
 *
 */
int
dot11HansiTable_cache_load(netsnmp_container *container)
{
	
	snmp_log(LOG_DEBUG, "enter dot11HansiTable_cache_load\n");
	
    dot11HansiTable_rowreq_ctx *rowreq_ctx;
    size_t                 count = 0;
    

    DEBUGMSGTL(("verbose:dot11HansiTable:dot11HansiTable_cache_load","called\n"));

    int ret = 0;;
    struct Hmd_Board_Info_show *instHead = NULL, *instNode = NULL;
    
    snmp_log(LOG_DEBUG, "enter show_broad_instance_info\n");
    ret = show_broad_instance_info(ccgi_dbus_connection, &instHead);
    snmp_log(LOG_DEBUG, "exit show_broad_instance_info,ret=%d\n", ret);

		
	if(1 == ret)
    {
        
        for(instNode = instHead->hmd_board_list; NULL != instNode; instNode = instNode->next)
        {
            int i = 0;
            int index = 0;
            char instance_state[32] = { 0 };
            for(i = 0; i < instNode->InstNum; i++){
                index = (instNode->Hmd_Inst[i]->slot_no - 1) * (INSTANCE_NUM * VRRP_TYPE_NUM) + instNode->Hmd_Inst[i]->Inst_ID;
                
                memset(instance_state, 0, sizeof(instance_state));
                if(1 == instNode->Hmd_Inst[i]->isActive)
                    strncpy(instance_state, "Master", sizeof(instance_state) - 1);
                else if (0 == instNode->Hmd_Inst[i]->isActive)
                    strncpy(instance_state, "Back", sizeof(instance_state) - 1);
      
    	        rowreq_ctx = dot11HansiTable_allocate_rowreq_ctx();
    	        if (NULL == rowreq_ctx) {
    	            snmp_log(LOG_ERR, "memory allocation failed\n");
    	            free_broad_instance_info(&instHead);
    	            return MFD_RESOURCE_UNAVAILABLE;
    	        }
    	        if(MFD_SUCCESS != dot11HansiTable_indexes_set(rowreq_ctx
    	                               , index
    	               )) {
    	            snmp_log(LOG_ERR,"error setting index while loading "
    	                     "dot11HansiTable cache.\n");
    	            dot11HansiTable_release_rowreq_ctx(rowreq_ctx);
    	            continue;
    	        }

    			int HansiState_len = 0;
    			HansiState_len = MIN(strlen(instance_state),sizeof(rowreq_ctx->data.HansiState));
    			rowreq_ctx->data.HansiState_len = HansiState_len * sizeof(rowreq_ctx->data.HansiState[0]);
    			
    			if ((NULL == rowreq_ctx->data.HansiState) ||
    				(rowreq_ctx->data.HansiState_len < (HansiState_len * sizeof(rowreq_ctx->data.HansiState[0])))) {
    				snmp_log(LOG_ERR,"not enough space for value\n");
    				return MFD_ERROR;
    			}
    			memcpy( rowreq_ctx->data.HansiState, instance_state, rowreq_ctx->data.HansiState_len );
    		    
    	        
                int j = 0;
                char uplink_ip[256] = { 0 };

    			for(j = 0; j < instNode->Hmd_Inst[i]->Inst_UNum; j++){

    			    if(0 != j) {
    			        strcat(uplink_ip, " ");
    			    }    
    			    ipaddr_int_to_str(instNode->Hmd_Inst[i]->Inst_Uplink[j].vir_ip, uplink_ip);
    			}

    			int HansiUplinkIP_len = 0;
    			
    			HansiUplinkIP_len = MIN(strlen(uplink_ip),sizeof(rowreq_ctx->data.HansiUplinkIP));
    			
    			rowreq_ctx->data.HansiUplinkIP_len = HansiUplinkIP_len * sizeof(rowreq_ctx->data.HansiUplinkIP[0]);
    			if ((NULL == rowreq_ctx->data.HansiUplinkIP) ||
    		        (rowreq_ctx->data.HansiUplinkIP_len < (HansiUplinkIP_len * sizeof(rowreq_ctx->data.HansiUplinkIP[0])))) {
    		        snmp_log(LOG_ERR,"not enough space for value\n");
    		        return MFD_ERROR;
    		    }
    			memcpy( rowreq_ctx->data.HansiUplinkIP, uplink_ip, rowreq_ctx->data.HansiUplinkIP_len );

                
                char downlink_ip[256] = { 0 };

    			for(j = 0; j < instNode->Hmd_Inst[i]->Inst_DNum; j++){
    			
    			    if(0 != j) {
    			        strcat(downlink_ip, " ");
    			    }    
    			    ipaddr_int_to_str(instNode->Hmd_Inst[i]->Inst_Downlink[j].vir_ip, downlink_ip);

    			}

    		    int HansiDownlinkIP_len = 0;
    			HansiDownlinkIP_len = MIN(strlen(downlink_ip),sizeof(rowreq_ctx->data.HansiDownlinkIP));
    		    rowreq_ctx->data.HansiDownlinkIP_len = HansiDownlinkIP_len * sizeof(rowreq_ctx->data.HansiDownlinkIP[0]);

    			if ((NULL == rowreq_ctx->data.HansiDownlinkIP) ||
    		        (rowreq_ctx->data.HansiDownlinkIP_len < (HansiDownlinkIP_len * sizeof(rowreq_ctx->data.HansiDownlinkIP[0])))) {
    		        snmp_log(LOG_ERR,"not enough space for value\n");
    		        return MFD_ERROR;
    		    }
    		    memcpy( rowreq_ctx->data.HansiDownlinkIP, downlink_ip, rowreq_ctx->data.HansiDownlinkIP_len );

                
                char gateway_ip[256] = { 0 };

    			for(j = 0; j < instNode->Hmd_Inst[i]->Inst_GNum; j++){

    			    if(0 != j) {
    			        strcat(gateway_ip, " ");
    			    }    
    			    ipaddr_int_to_str(instNode->Hmd_Inst[i]->Inst_Gateway[j].vir_ip, gateway_ip);

    			}

    		    int HansiHblinkIP_len = 0;
    			HansiHblinkIP_len = MIN(strlen(gateway_ip),sizeof(rowreq_ctx->data.HansiHblinkIP));
    			rowreq_ctx->data.HansiHblinkIP_len = HansiHblinkIP_len * sizeof(rowreq_ctx->data.HansiHblinkIP[0]);
    			if ((NULL == rowreq_ctx->data.HansiHblinkIP) ||
    		        (rowreq_ctx->data.HansiHblinkIP_len < (HansiHblinkIP_len * sizeof(rowreq_ctx->data.HansiHblinkIP[0])))) {
    		        snmp_log(LOG_ERR,"not enough space for value\n");
    		        return MFD_ERROR;
    		    }	    	    
    		    memcpy( rowreq_ctx->data.HansiHblinkIP, gateway_ip, rowreq_ctx->data.HansiHblinkIP_len );     			    
    	        
    	        if(CONTAINER_INSERT(container, rowreq_ctx))
    	        {
    				dot11HansiTable_release_rowreq_ctx(rowreq_ctx);
    	        }
    	        ++count;
            }

            for(i = 0; i < instNode->LocalInstNum; i++) {

                index = (instNode->Hmd_Local_Inst[i]->slot_no - 1) * (INSTANCE_NUM * VRRP_TYPE_NUM) + instNode->Hmd_Local_Inst[i]->Inst_ID + INSTANCE_NUM;
                
                memset(instance_state, 0, sizeof(instance_state));
                if(1 == instNode->Hmd_Local_Inst[i]->isActive)
                    strncpy(instance_state, "Master", sizeof(instance_state) - 1);
                else if (0 == instNode->Hmd_Local_Inst[i]->isActive)
                    strncpy(instance_state, "Back", sizeof(instance_state) - 1);

    	        rowreq_ctx = dot11HansiTable_allocate_rowreq_ctx();
    	        if (NULL == rowreq_ctx) {
    	            snmp_log(LOG_ERR, "memory allocation failed\n");
    	            free_broad_instance_info(&instHead);
    	            return MFD_RESOURCE_UNAVAILABLE;
    	        }
    	        if(MFD_SUCCESS != dot11HansiTable_indexes_set(rowreq_ctx
    	                               , index
    	               )) {
    	            snmp_log(LOG_ERR,"error setting index while loading "
    	                     "dot11HansiTable cache.\n");
    	            dot11HansiTable_release_rowreq_ctx(rowreq_ctx);
    	            continue;
    	        }

    			int HansiState_len = 0;
    			HansiState_len = MIN(strlen(instance_state),sizeof(rowreq_ctx->data.HansiState));
    			rowreq_ctx->data.HansiState_len = HansiState_len * sizeof(rowreq_ctx->data.HansiState[0]);
    			
    			if ((NULL == rowreq_ctx->data.HansiState) ||
    				(rowreq_ctx->data.HansiState_len < (HansiState_len * sizeof(rowreq_ctx->data.HansiState[0])))) {
    				snmp_log(LOG_ERR,"not enough space for value\n");
    				return MFD_ERROR;
    			}
    			memcpy( rowreq_ctx->data.HansiState, instance_state, rowreq_ctx->data.HansiState_len );


                char uplink_ip[256] = { 0 };

    			int j = 0;
    			for(j = 0; j < instNode->Hmd_Local_Inst[i]->Inst_UNum; j++){

    			    if(0 != j) {
    			        strcat(uplink_ip, " ");
    			    }    
    			    ipaddr_int_to_str(instNode->Hmd_Local_Inst[i]->Inst_Uplink[j].vir_ip, uplink_ip);
    			}
        
   	
    			int HansiUplinkIP_len = 0;
    			
    			HansiUplinkIP_len = MIN(strlen(uplink_ip),sizeof(rowreq_ctx->data.HansiUplinkIP));
    			
    			rowreq_ctx->data.HansiUplinkIP_len = HansiUplinkIP_len * sizeof(rowreq_ctx->data.HansiUplinkIP[0]);
    			if ((NULL == rowreq_ctx->data.HansiUplinkIP) ||
    		        (rowreq_ctx->data.HansiUplinkIP_len < (HansiUplinkIP_len * sizeof(rowreq_ctx->data.HansiUplinkIP[0])))) {
    		        snmp_log(LOG_ERR,"not enough space for value\n");
    		        return MFD_ERROR;
    		    }
    			memcpy( rowreq_ctx->data.HansiUplinkIP, uplink_ip, rowreq_ctx->data.HansiUplinkIP_len );

    			
                char downlink_ip[256] = { 0 };

    			for(j = 0; j < instNode->Hmd_Local_Inst[i]->Inst_DNum; j++){

    			    if(0 != j) {
    			        strcat(downlink_ip, " ");
    			    }    
    			    ipaddr_int_to_str(instNode->Hmd_Local_Inst[i]->Inst_Downlink[j].vir_ip, downlink_ip);

    			}
  		    
    		    int HansiDownlinkIP_len = 0;
    			HansiDownlinkIP_len = MIN(strlen(downlink_ip),sizeof(rowreq_ctx->data.HansiDownlinkIP));
    		    rowreq_ctx->data.HansiDownlinkIP_len = HansiDownlinkIP_len * sizeof(rowreq_ctx->data.HansiDownlinkIP[0]);

    			if ((NULL == rowreq_ctx->data.HansiDownlinkIP) ||
    		        (rowreq_ctx->data.HansiDownlinkIP_len < (HansiDownlinkIP_len * sizeof(rowreq_ctx->data.HansiDownlinkIP[0])))) {
    		        snmp_log(LOG_ERR,"not enough space for value\n");
    		        return MFD_ERROR;
    		    }
    		    memcpy( rowreq_ctx->data.HansiDownlinkIP, downlink_ip, rowreq_ctx->data.HansiDownlinkIP_len );

                char gateway_ip[256] = { 0 };

    			for(j = 0; j < instNode->Hmd_Local_Inst[i]->Inst_GNum; j++){
    			    if(0 != j) {
    			        strcat(gateway_ip, " ");
    			    }    
    			    ipaddr_int_to_str(instNode->Hmd_Local_Inst[i]->Inst_Gateway[j].vir_ip, gateway_ip);

    			}
    				    
    		    int HansiHblinkIP_len = 0;
    			HansiHblinkIP_len = MIN(strlen(gateway_ip),sizeof(rowreq_ctx->data.HansiHblinkIP));
    			rowreq_ctx->data.HansiHblinkIP_len = HansiHblinkIP_len * sizeof(rowreq_ctx->data.HansiHblinkIP[0]);
    			/*if ((NULL == rowreq_ctx->data.HansiHblinkIP) ||
    		        (rowreq_ctx->data.HansiHblinkIP_len < (HansiHblinkIP_len * sizeof(rowreq_ctx->data.HansiHblinkIP[0])))) {
    		        snmp_log(LOG_ERR,"not enough space for value\n");
    		        return MFD_ERROR;
    		    }*/	    	    
    		    memcpy( rowreq_ctx->data.HansiHblinkIP, gateway_ip, rowreq_ctx->data.HansiHblinkIP_len );   			    
    	        
    	        if(CONTAINER_INSERT(container, rowreq_ctx))
    	        {
    				dot11HansiTable_release_rowreq_ctx(rowreq_ctx);
    	        }
    	        ++count;
            }
	    }
    }	
    
    free_broad_instance_info(&instHead);
    

    DEBUGMSGT(("verbose:dot11HansiTable:dot11HansiTable_cache_load",
               "inserted %d records\n", count));

	snmp_log(LOG_DEBUG, "exit dot11HansiTable_cache_load\n");
    return MFD_SUCCESS;
} /* dot11HansiTable_cache_load */

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
dot11HansiTable_cache_free(netsnmp_container *container)
{
    DEBUGMSGTL(("verbose:dot11HansiTable:dot11HansiTable_cache_free","called\n"));

    /*
     * TODO:380:M: Free dot11HansiTable cache.
     */
} /* dot11HansiTable_cache_free */

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
dot11HansiTable_row_prep( dot11HansiTable_rowreq_ctx *rowreq_ctx)
{
    DEBUGMSGTL(("verbose:dot11HansiTable:dot11HansiTable_row_prep","called\n"));

    netsnmp_assert(NULL != rowreq_ctx);

    /*
     * TODO:390:o: Prepare row for request.
     * If populating row data was delayed, this is the place to
     * fill in the row for this request.
     */

    return MFD_SUCCESS;
} /* dot11HansiTable_row_prep */

/** @} */
