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
#include "dot11ConfigRadioTable.h"


#include "dot11ConfigRadioTable_data_access.h"
#include "wcpss/asd/asd.h"
#include "wcpss/wid/WID.h"
#include "dbus/wcpss/ACDbusDef1.h"
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
 *** Table dot11ConfigRadioTable
 ***
 **********************************************************************
 **********************************************************************/
/*
 * dot11ConfigRadioTable is subid 3 of wtpRadio.
 * Its status is Current.
 * OID: .1.3.6.1.4.1.31656.6.1.1.4.3, length: 12
*/

/**
 * initialization for dot11ConfigRadioTable data access
 *
 * This function is called during startup to allow you to
 * allocate any resources you need for the data table.
 *
 * @param dot11ConfigRadioTable_reg
 *        Pointer to dot11ConfigRadioTable_registration
 *
 * @retval MFD_SUCCESS : success.
 * @retval MFD_ERROR   : unrecoverable error.
 */
int
dot11ConfigRadioTable_init_data(dot11ConfigRadioTable_registration_ptr dot11ConfigRadioTable_reg)
{
    DEBUGMSGTL(("verbose:dot11ConfigRadioTable:dot11ConfigRadioTable_init_data","called\n"));

    /*
     * TODO:303:o: Initialize dot11ConfigRadioTable data.
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
} /* dot11ConfigRadioTable_init_data */

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
dot11ConfigRadioTable_container_init(netsnmp_container **container_ptr_ptr,
                        netsnmp_cache *cache)
{
    DEBUGMSGTL(("verbose:dot11ConfigRadioTable:dot11ConfigRadioTable_container_init","called\n"));
    
    if((NULL == cache) || (NULL == container_ptr_ptr)) {
        snmp_log(LOG_ERR,"bad params to dot11ConfigRadioTable_container_init\n");
        return;
    }

    /*
     * For advanced users, you can use a custom container. If you
     * do not create one, one will be created for you.
     */
    *container_ptr_ptr = NULL;

    /*
     * TODO:345:A: Set up dot11ConfigRadioTable cache properties.
     *
     * Also for advanced users, you can set parameters for the
     * cache. Do not change the magic pointer, as it is used
     * by the MFD helper. To completely disable caching, set
     * cache->enabled to 0.
     */
    cache->timeout = DOT11CONFIGRADIOTABLE_CACHE_TIMEOUT; /* seconds */
} /* dot11ConfigRadioTable_container_init */

/**
 * load cache data
 *
 * TODO:350:M: Implement dot11ConfigRadioTable cache load
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
 *  dot11ConfigRadioTable_row_prep() for populating data.
 *
 * @note
 *  If you need consistency between rows (like you want statistics
 *  for each row to be from the same time frame), you should set all
 *  data here.
 *
 */
int
dot11ConfigRadioTable_cache_load(netsnmp_container *container)
{
	snmp_log(LOG_DEBUG, "enter dot11ConfigRadioTable_cache_load\n");

    dot11ConfigRadioTable_rowreq_ctx *rowreq_ctx;
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
         * wtpMacAddr(1)/Dot11BaseWtpIdTC/ASN_OCTET_STR/char(char)//L/A/w/e/r/d/h
         */
        /** 128 - 1(entry) - 1(col) - 1(other indexes) = 111 */
   char   wtpMacAddr[20] = { 0 };
   size_t      wtpMacAddr_len = 0;
        /*
         * wtpWirelessIfIndex(1)/INTEGER/ASN_INTEGER/long(long)//l/A/w/e/r/d/h
         */

    DEBUGMSGTL(("verbose:dot11ConfigRadioTable:dot11ConfigRadioTable_cache_load","called\n"));

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
     * TODO:351:M: |-> Load/update data in the dot11ConfigRadioTable container.
     * loop over your dot11ConfigRadioTable data, allocate a rowreq context,
     * set the index(es) [and data, optionally] and insert into
     * the container.
     */
	
	int radioLeadCode = 0;	
    char tembwid[5] = { 0 };	
	char radiowlan[255] = { 0 };
	char radioBindWlan[255] = { 0 };
	size_t radioBindWlan_len = 0;
	char radioDelbindWlan[255] = { 0 };
	size_t radioDelbindWlan_len = 0;
	char radioBindQos[255] = { 0 };
	size_t radioBindQos_len = 0;
	char radioDelbindQos[255] = { 0 };
	size_t radioDelbindQos_len = 0;
	int radioAmpduSwitch = 0;
//	int radioCwmode = 0;
	int radioShortGISwitch = 0;
	int radioAmsduSwitch = 0;
	char temmcs[5] = { 0 };	
	char radiomcs[255] = { 0 };
//	char radioMCS[255] = { 0 };
//	size_t radioMCS_len = 0;
	int radio11nSwitch = 0;

    snmpd_dbus_message *messageHead = NULL, *messageNode = NULL;
    
    snmp_log(LOG_DEBUG, "enter list_connection_call_dbus_method:show_all_wtp_radio_config_information_cmd\n");
    messageHead = list_connection_call_dbus_method(show_all_wtp_radio_config_information_cmd, SHOW_ALL_WTP_TABLE_METHOD);
	snmp_log(LOG_DEBUG, "exit list_connection_call_dbus_method:show_all_wtp_radio_config_information_cmd,messageHead=%p\n", messageHead);
	
	if(messageHead)
	{
		for(messageNode = messageHead; NULL != messageNode; messageNode = messageNode->next)
		{
		    struct WtpConfigRadioInfo *WtpHead = messageNode->message;
            void *connection = NULL;
            if(SNMPD_DBUS_ERROR == get_slot_dbus_connection(messageNode->parameter.slot_id, &connection, SNMPD_INSTANCE_MASTER_V3))
                continue;
                
		    if(WtpHead)
		    {
		        struct WtpConfigRadioInfo *q = NULL;
        		for(q = WtpHead->WtpConfigRadioInfo_list; q != NULL; q = q->next)
        		{
        			memset(wtpMacAddr,0,20);
					if(q->wtpMacAddr)
					{
						snprintf(wtpMacAddr,sizeof(wtpMacAddr)-1,
								 "%02X:%02X:%02X:%02X:%02X:%02X",
								 q->wtpMacAddr[0],q->wtpMacAddr[1],
								 q->wtpMacAddr[2],q->wtpMacAddr[3],
								 q->wtpMacAddr[4],q->wtpMacAddr[5]);
					}

                    int i = 0;
        			struct Sub_WtpConfigRadioInfo *sub_radio = NULL;
        			for(i=0,sub_radio = q->Sub_WtpConfigRadioInfo_head;((i<q->wtp_radio_num)&&(sub_radio));i++,sub_radio = sub_radio->next)
        			{
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
    //} while (filep && (line[0] == '\n'));

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
         * TODO:352:M: |   |-> set indexes in new dot11ConfigRadioTable rowreq context.
         */
        rowreq_ctx = dot11ConfigRadioTable_allocate_rowreq_ctx();
        if (NULL == rowreq_ctx) {
            snmp_log(LOG_ERR, "memory allocation failed\n");
            free_dbus_message_list(&messageHead, Free_show_all_wtp_radio_config_information_cmd);
            return MFD_RESOURCE_UNAVAILABLE;
        }
		
		wtpMacAddr_len = strlen(wtpMacAddr);
        if(MFD_SUCCESS != dot11ConfigRadioTable_indexes_set(rowreq_ctx
                               , wtpMacAddr, wtpMacAddr_len
                               , sub_radio->wtpRadLocalID+1
               )) {
            snmp_log(LOG_ERR,"error setting index while loading "
                     "dot11ConfigRadioTable cache.\n");
            dot11ConfigRadioTable_release_rowreq_ctx(rowreq_ctx);
            continue;
        }

        /*
         * TODO:352:r: |   |-> populate dot11ConfigRadioTable data context.
         * Populate data context here. (optionally, delay until row prep)
         */
    /*
     * TRANSIENT or semi-TRANSIENT data:
     * copy data or save any info needed to do it in row_prep.
     */
    /*
     * setup/save data for radioChannel
     * radioChannel(1)/INTEGER/ASN_INTEGER/long(long)//l/A/W/e/r/d/h
     */
    /*
     * TODO:246:r: |-> Define radioChannel mapping.
     * Map values between raw/native values and MIB values
     *
     * Integer based value can usually just do a direct copy.
     */
    memcpy(&(rowreq_ctx->data.parameter), &(messageNode->parameter), sizeof(dbus_parameter));
	rowreq_ctx->data.wtpRadCurrID = sub_radio->wtpRadCurrID ;
	rowreq_ctx->data.setwtpRadCurrID = sub_radio->wtpRadCurrID;
		
    rowreq_ctx->data.radioChannel = sub_radio->radioChannel;
    
    /*
     * setup/save data for radioTxPower
     * radioTxPower(2)/INTEGER/ASN_INTEGER/long(long)//l/A/W/e/R/d/h
     */
    /*
     * TODO:246:r: |-> Define radioTxPower mapping.
     * Map values between raw/native values and MIB values
     *
     * Integer based value can usually just do a direct copy.
     */
    rowreq_ctx->data.radioTxPower = sub_radio->radioTxPower;
    
    /*
     * setup/save data for radioType
     * radioType(3)/INTEGER/ASN_INTEGER/long(u_long)//l/A/W/E/r/d/h
     */
    /*
     * TODO:246:r: |-> Define radioType mapping.
     * Map values between raw/native values and MIB values
     *
    * enums usually need mapping.
    */
    /*if(MFD_SUCCESS !=
       radioType_map(&rowreq_ctx->data.radioType, sub_radio->radioType )) {
        return MFD_ERROR;
    }*/
    unsigned long radioType = 0;
    unsigned int flag_11n = 0;
    
    if(sub_radio->radioType & IEEE80211_11N){
        flag_11n = 1;
    }
    
    if(sub_radio->radioType & IEEE80211_11A) {
#if 0
        radioType |= 0x1;           //a           
        if(flag_11n) {
            radioType |= 0x1 << 3;      //an
        }
#else    
        if(flag_11n) {
            if(sub_radio->radioType & IEEE80211_11AN) {
                radioType |= 0x1;           //a        
            }
            radioType |= 0x1 << 3;      //an
        }
        else {
            radioType |= 0x1;           //a        
        }
#endif    
    }

    if(sub_radio->radioType & IEEE80211_11B) {
        radioType |= 0x1 << 1;      //b        
    }

    if(sub_radio->radioType & IEEE80211_11G) {
#if 0
        radioType |= 0x1 << 2;      //g        
        if(flag_11n) {
            radioType |= 0x1 << 4;      //gn
        }
#else
        if(flag_11n) {
            if((sub_radio->radioType & IEEE80211_11GN)
			|| (radioType & (0x1 << 1))) {                
                radioType |= 0x1 << 2;      //g        
            }
            radioType |= 0x1 << 4;      //gn
        }
        else {
            radioType |= 0x1 << 2;      //g        
        }
#endif        
    }
    
    rowreq_ctx->data.radioType = radioType;
    /*
     * setup/save data for radioSpeed
     * radioSpeed(4)/INTEGER/ASN_INTEGER/long(long)//l/A/W/e/r/d/h
     */
    /*
     * TODO:246:r: |-> Define radioSpeed mapping.
     * Map values between raw/native values and MIB values
     *
     * Integer based value can usually just do a direct copy.
     */
    rowreq_ctx->data.radioSpeed = sub_radio->radioMaxSpeed;
    
    /*
     * setup/save data for radioBeacon
     * radioBeacon(5)/INTEGER/ASN_INTEGER/long(long)//l/A/W/e/R/d/h
     */
    /*
     * TODO:246:r: |-> Define radioBeacon mapping.
     * Map values between raw/native values and MIB values
     *
     * Integer based value can usually just do a direct copy.
     */
    rowreq_ctx->data.radioBeacon = sub_radio->radioBeacon;
    
    /*
     * setup/save data for radioFragThreshold
     * radioFragThreshold(6)/INTEGER/ASN_INTEGER/long(long)//l/A/W/e/R/d/h
     */
    /*
     * TODO:246:r: |-> Define radioFragThreshold mapping.
     * Map values between raw/native values and MIB values
     *
     * Integer based value can usually just do a direct copy.
     */
    rowreq_ctx->data.radioFragThreshold = sub_radio->radioBeaf;
    
    /*
     * setup/save data for radioDtim
     * radioDtim(7)/INTEGER/ASN_INTEGER/long(long)//l/A/W/e/R/d/h
     */
    /*
     * TODO:246:r: |-> Define radioDtim mapping.
     * Map values between raw/native values and MIB values
     *
     * Integer based value can usually just do a direct copy.
     */
    rowreq_ctx->data.radioDtim = sub_radio->radioDtim;
    
    /*
     * setup/save data for radioRTS
     * radioRTS(8)/INTEGER/ASN_INTEGER/long(long)//l/A/W/e/R/d/h
     */
    /*
     * TODO:246:r: |-> Define radioRTS mapping.
     * Map values between raw/native values and MIB values
     *
     * Integer based value can usually just do a direct copy.
     */
    rowreq_ctx->data.radioRTS = sub_radio->radioRTS;
    
    /*
     * setup/save data for radioLeadCode
     * radioLeadCode(9)/INTEGER/ASN_INTEGER/long(u_long)//l/A/W/E/r/d/h
     */
    /*
     * TODO:246:r: |-> Define radioLeadCode mapping.
     * Map values between raw/native values and MIB values
     *
    * enums usually need mapping.
    */
    
    if(sub_radio->radioLeadCode==1)
	{
		radioLeadCode=1;
	}
	else
	{
		radioLeadCode=2;
	}
	
    /*if(MFD_SUCCESS !=
       radioLeadCode_map(&rowreq_ctx->data.radioLeadCode, radioLeadCode )) {
        return MFD_ERROR;
    }*/
    rowreq_ctx->data.radioLeadCode = radioLeadCode;
    
    /*
     * setup/save data for radioShortRetry
     * radioShortRetry(10)/INTEGER/ASN_INTEGER/long(long)//l/A/W/e/R/d/h
     */
    /*
     * TODO:246:r: |-> Define radioShortRetry mapping.
     * Map values between raw/native values and MIB values
     *
     * Integer based value can usually just do a direct copy.
     */
    rowreq_ctx->data.radioShortRetry = sub_radio->radioShortRetry;
    
    /*
     * setup/save data for radioLongRetry
     * radioLongRetry(11)/INTEGER/ASN_INTEGER/long(long)//l/A/W/e/R/d/h
     */
    /*
     * TODO:246:r: |-> Define radioLongRetry mapping.
     * Map values between raw/native values and MIB values
     *
     * Integer based value can usually just do a direct copy.
     */
    rowreq_ctx->data.radioLongRetry = sub_radio->radioLongRetry;
    
    /*
     * setup/save data for radioService
     * radioService(12)/INTEGER/ASN_INTEGER/long(u_long)//l/A/W/E/r/d/h
     */
    /*
     * TODO:246:r: |-> Define radioService mapping.
     * Map values between raw/native values and MIB values
     *
    * enums usually need mapping.
    */
    /*if(MFD_SUCCESS !=
       radioService_map(&rowreq_ctx->data.radioService, sub_radio->radioService )) {
        return MFD_ERROR;
    }*/
    rowreq_ctx->data.radioService = sub_radio->radioService;
    
    /*
     * setup/save data for radioMaxSpeed
     * radioMaxSpeed(13)/INTEGER/ASN_INTEGER/long(long)//l/A/W/e/r/d/h
     */
    /*
     * TODO:246:r: |-> Define radioMaxSpeed mapping.
     * Map values between raw/native values and MIB values
     *
     * Integer based value can usually just do a direct copy.
     */
    rowreq_ctx->data.radioMaxSpeed = sub_radio->radioMaxSpeed;
    
    /*
     * setup/save data for radioMaxFlow
     * radioMaxFlow(14)/INTEGER/ASN_INTEGER/long(long)//l/A/W/e/R/d/h
     */
    /*
     * TODO:246:r: |-> Define radioMaxFlow mapping.
     * Map values between raw/native values and MIB values
     *
     * Integer based value can usually just do a direct copy.
     */
    rowreq_ctx->data.radioMaxFlow = sub_radio->radioMaxFlow;
    
    /*
     * setup/save data for radioBindWlan
     * radioBindWlan(15)/DisplayString/ASN_OCTET_STR/char(char)//L/A/W/e/R/d/H
     */
    /*
     * TODO:246:r: |-> Define radioBindWlan mapping.
     * Map values between raw/native values and MIB values
     *
     * if(MFD_SUCCESS !=
     *    radioBindWlan_map(&rowreq_ctx->data.radioBindWlan, &rowreq_ctx->data.radioBindWlan_len,
     *                radioBindWlan, radioBindWlan_len, 0)) {
     *    return MFD_ERROR;
     * }
     */
    /*
     * make sure there is enough space for radioBindWlan data
     */
    /*if ((NULL == rowreq_ctx->data.radioBindWlan) ||
        (rowreq_ctx->data.radioBindWlan_len < (radioBindWlan_len * sizeof(rowreq_ctx->data.radioBindWlan[0])))) {
        snmp_log(LOG_ERR,"not enough space for value\n");
        return MFD_ERROR;
    }*/	
    int j = 0;
	memset(radiowlan, 0, 255);	
	for(j = 0; j < sub_radio->radioWlanNum; j++)
	{
		memset(tembwid,0,5);
		if(j == sub_radio->radioWlanNum-1)
		{
			snprintf(tembwid,sizeof(tembwid)-1,"%d",sub_radio->radioBindWlan[j]);
		}
		else
		{
			snprintf(tembwid,sizeof(tembwid)-1,"%d,",sub_radio->radioBindWlan[j]);
		}
		strncat(radiowlan,tembwid,sizeof(radiowlan)-strlen(radiowlan)-1);
	}

    radioBindWlan_len = MIN(strlen(radiowlan),sizeof(radioBindWlan)-1);
	memset(radioBindWlan,0,sizeof(radioBindWlan));
	memcpy(radioBindWlan,radiowlan,radioBindWlan_len);
				
    rowreq_ctx->data.radioBindWlan_len = radioBindWlan_len * sizeof(rowreq_ctx->data.radioBindWlan[0]);
    memcpy( rowreq_ctx->data.radioBindWlan, radioBindWlan, rowreq_ctx->data.radioBindWlan_len );
    
    /*
     * setup/save data for radioDelbindWlan
     * radioDelbindWlan(16)/DisplayString/ASN_OCTET_STR/char(char)//L/A/W/e/R/d/H
     */
    /*
     * TODO:246:r: |-> Define radioDelbindWlan mapping.
     * Map values between raw/native values and MIB values
     *
     * if(MFD_SUCCESS !=
     *    radioDelbindWlan_map(&rowreq_ctx->data.radioDelbindWlan, &rowreq_ctx->data.radioDelbindWlan_len,
     *                radioDelbindWlan, radioDelbindWlan_len, 0)) {
     *    return MFD_ERROR;
     * }
     */
    /*
     * make sure there is enough space for radioDelbindWlan data
     */
    /*if ((NULL == rowreq_ctx->data.radioDelbindWlan) ||
        (rowreq_ctx->data.radioDelbindWlan_len < (radioDelbindWlan_len * sizeof(rowreq_ctx->data.radioDelbindWlan[0])))) {
        snmp_log(LOG_ERR,"not enough space for value\n");
        return MFD_ERROR;
    }*/
    radioDelbindWlan_len = MIN(strlen(radiowlan),sizeof(radioDelbindWlan)-1);
	memset(radioDelbindWlan,0,sizeof(radioDelbindWlan));
	memcpy(radioDelbindWlan,radiowlan,radioDelbindWlan_len);
	
    rowreq_ctx->data.radioDelbindWlan_len = radioDelbindWlan_len * sizeof(rowreq_ctx->data.radioDelbindWlan[0]);
    memcpy( rowreq_ctx->data.radioDelbindWlan, radioDelbindWlan, rowreq_ctx->data.radioDelbindWlan_len );
    
    /*
     * setup/save data for radioBindQos
     * radioBindQos(17)/DisplayString/ASN_OCTET_STR/char(char)//L/A/W/e/R/d/H
     */
    /*
     * TODO:246:r: |-> Define radioBindQos mapping.
     * Map values between raw/native values and MIB values
     *
     * if(MFD_SUCCESS !=
     *    radioBindQos_map(&rowreq_ctx->data.radioBindQos, &rowreq_ctx->data.radioBindQos_len,
     *                radioBindQos, radioBindQos_len, 0)) {
     *    return MFD_ERROR;
     * }
     */
    /*
     * make sure there is enough space for radioBindQos data
     */
    /*if ((NULL == rowreq_ctx->data.radioBindQos) ||
        (rowreq_ctx->data.radioBindQos_len < (radioBindQos_len * sizeof(rowreq_ctx->data.radioBindQos[0])))) {
        snmp_log(LOG_ERR,"not enough space for value\n");
        return MFD_ERROR;
    }*/
	memset(radioBindQos,0,sizeof(radioBindQos));
	snprintf(radioBindQos,sizeof(radioBindQos)-1,"%d",sub_radio->radioBindQos);	
    radioBindQos_len = MIN(strlen(radioBindQos),sizeof(radioBindQos)-1);
	
    rowreq_ctx->data.radioBindQos_len = radioBindQos_len * sizeof(rowreq_ctx->data.radioBindQos[0]);
    memcpy( rowreq_ctx->data.radioBindQos, radioBindQos, rowreq_ctx->data.radioBindQos_len );
    
    /*
     * setup/save data for radioDelbindQos
     * radioDelbindQos(18)/DisplayString/ASN_OCTET_STR/char(char)//L/A/W/e/R/d/H
     */
    /*
     * TODO:246:r: |-> Define radioDelbindQos mapping.
     * Map values between raw/native values and MIB values
     *
     * if(MFD_SUCCESS !=
     *    radioDelbindQos_map(&rowreq_ctx->data.radioDelbindQos, &rowreq_ctx->data.radioDelbindQos_len,
     *                radioDelbindQos, radioDelbindQos_len, 0)) {
     *    return MFD_ERROR;
     * }
     */
    /*
     * make sure there is enough space for radioDelbindQos data
     */
    /*if ((NULL == rowreq_ctx->data.radioDelbindQos) ||
        (rowreq_ctx->data.radioDelbindQos_len < (radioDelbindQos_len * sizeof(rowreq_ctx->data.radioDelbindQos[0])))) {
        snmp_log(LOG_ERR,"not enough space for value\n");
        return MFD_ERROR;
    }*/
    memset(radioDelbindQos,0,sizeof(radioDelbindQos));
	snprintf(radioDelbindQos,sizeof(radioDelbindQos)-1,"%d",sub_radio->radioDelbindQos);	
    radioDelbindQos_len = MIN(strlen(radioDelbindQos),sizeof(radioDelbindQos)-1);
	
    rowreq_ctx->data.radioDelbindQos_len = radioDelbindQos_len * sizeof(rowreq_ctx->data.radioDelbindQos[0]);
    memcpy( rowreq_ctx->data.radioDelbindQos, radioDelbindQos, rowreq_ctx->data.radioDelbindQos_len );
    
    /*
     * setup/save data for radioAmpduSwitch
     * radioAmpduSwitch(19)/INTEGER/ASN_INTEGER/long(u_long)//l/A/W/E/r/d/h
     */
    /*
     * TODO:246:r: |-> Define radioAmpduSwitch mapping.
     * Map values between raw/native values and MIB values
     *
    * enums usually need mapping.
    */
    /*if(MFD_SUCCESS !=
       radioAmpduSwitch_map(&rowreq_ctx->data.radioAmpduSwitch, radioAmpduSwitch )) {
        return MFD_ERROR;
    }*/
    radioAmpduSwitch = 0;
//	radioCwmode = 0;
	radioShortGISwitch = 0;
	radioAmsduSwitch = 0;
	memset(radiomcs, 0, 255);	
	radio11nSwitch = 0;
	
	int OnlyAllow11nStaAccess = 2;

	if((sub_radio->Radio_Type&IEEE80211_11N) > 0)/*radio type include 'n'*/
	{
		if(sub_radio->AmpduAble == 1)
		{
			radioAmpduSwitch = 1;
		}
		else
		{
			radioAmpduSwitch = 0;
		}
		if(sub_radio->guardinterval == 1)
		{
			radioShortGISwitch = 1;
		}
		else
		{
			radioShortGISwitch = 0;
		}
		if(sub_radio->AmsduAble == 1)
		{
			radioAmsduSwitch = 1;
		}
		else
		{
			radioAmsduSwitch = 0;
		}
		for(j = 0; j < sub_radio->mcs_count; j++)
		{
			memset(temmcs,0,sizeof(temmcs));
			if(j == sub_radio->mcs_count-1)
			{
				snprintf(temmcs,sizeof(temmcs)-1,"%d",sub_radio->mcs_list[j]);
			}
			else
			{
				snprintf(temmcs,sizeof(temmcs)-1,"%d,",sub_radio->mcs_list[j]);
			}
			strncat(radiomcs,temmcs,sizeof(radiomcs)-strlen(radiomcs)-1);
		}
		if(sub_radio->Mixed_Greenfield == 1)
		{
			OnlyAllow11nStaAccess = 1;
		}
		else
		{
			OnlyAllow11nStaAccess = 2;
		}

		radio11nSwitch = 1;
	}
	else
	{
		radio11nSwitch = 0;
	}

    rowreq_ctx->data.radioAmpduSwitch = radioAmpduSwitch;
    
    rowreq_ctx->data.radioCwmode = sub_radio->cwmode;
    
    rowreq_ctx->data.radioShortGISwitch = radioShortGISwitch;
	
    rowreq_ctx->data.radioAmsduSwitch = radioAmsduSwitch;
    
    /*
     * setup/save data for radioMCS
     * radioMCS(23)/DisplayString/ASN_OCTET_STR/char(char)//L/A/W/e/R/d/H
     */
    /*
     * TODO:246:r: |-> Define radioMCS mapping.
     * Map values between raw/native values and MIB values
     *
     * if(MFD_SUCCESS !=
     *    radioMCS_map(&rowreq_ctx->data.radioMCS, &rowreq_ctx->data.radioMCS_len,
     *                radioMCS, radioMCS_len, 0)) {
     *    return MFD_ERROR;
     * }
     */
    /*
     * make sure there is enough space for radioMCS data
     */
    /*if ((NULL == rowreq_ctx->data.radioMCS) ||
        (rowreq_ctx->data.radioMCS_len < (radioMCS_len * sizeof(rowreq_ctx->data.radioMCS[0])))) {
        snmp_log(LOG_ERR,"not enough space for value\n");
        return MFD_ERROR;
    }*/
/*	radioMCS_len = MIN(strlen(radiomcs),sizeof(radioMCS)-1);
	memset(radioMCS,0,sizeof(radioMCS));
	memcpy(radioMCS,radiomcs,radioMCS_len);
				
    rowreq_ctx->data.radioMCS_len = radioMCS_len * sizeof(rowreq_ctx->data.radioMCS[0]);
    memcpy( rowreq_ctx->data.radioMCS, radioMCS, rowreq_ctx->data.radioMCS_len );*/

	rowreq_ctx->data.radioMCS_len = MIN(strlen(radiomcs),sizeof(rowreq_ctx->data.radioMCS)-1);
    memcpy( rowreq_ctx->data.radioMCS, radiomcs, rowreq_ctx->data.radioMCS_len );

    rowreq_ctx->data.radio11nSwitch = radio11nSwitch;
    
    /*
     * setup/save data for OnlyAllow11nStaAccess
     * OnlyAllow11nStaAccess(25)/TruthValue/ASN_INTEGER/long(u_long)//l/A/W/E/r/d/h
     */
    /*
     * TODO:246:r: |-> Define OnlyAllow11nStaAccess mapping.
     * Map values between raw/native values and MIB values
     *
    * enums usually need mapping.
    */

    if(MFD_SUCCESS !=
       OnlyAllow11nStaAccess_map(&rowreq_ctx->data.OnlyAllow11nStaAccess, OnlyAllow11nStaAccess )) {
        return MFD_ERROR;
    }


        
        /*
         * insert into table container
         */
    			        if(CONTAINER_INSERT(container, rowreq_ctx))
    			        {
    						dot11ConfigRadioTable_release_rowreq_ctx(rowreq_ctx);
    			        }
    			        ++count;		
    				}
    			}
    		}
    	}
	    free_dbus_message_list(&messageHead, Free_show_all_wtp_radio_config_information_cmd);
	}


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

    DEBUGMSGT(("verbose:dot11ConfigRadioTable:dot11ConfigRadioTable_cache_load",
               "inserted %d records\n", count));

	snmp_log(LOG_DEBUG, "exit dot11ConfigRadioTable_cache_load\n");
    return MFD_SUCCESS;
} /* dot11ConfigRadioTable_cache_load */

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
dot11ConfigRadioTable_cache_free(netsnmp_container *container)
{
    DEBUGMSGTL(("verbose:dot11ConfigRadioTable:dot11ConfigRadioTable_cache_free","called\n"));

    /*
     * TODO:380:M: Free dot11ConfigRadioTable cache.
     */
} /* dot11ConfigRadioTable_cache_free */

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
dot11ConfigRadioTable_row_prep( dot11ConfigRadioTable_rowreq_ctx *rowreq_ctx)
{
    DEBUGMSGTL(("verbose:dot11ConfigRadioTable:dot11ConfigRadioTable_row_prep","called\n"));

    netsnmp_assert(NULL != rowreq_ctx);

    /*
     * TODO:390:o: Prepare row for request.
     * If populating row data was delayed, this is the place to
     * fill in the row for this request.
     */

    return MFD_SUCCESS;
} /* dot11ConfigRadioTable_row_prep */

/** @} */
