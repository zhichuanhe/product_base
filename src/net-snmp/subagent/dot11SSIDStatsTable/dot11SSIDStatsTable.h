/*
 * Note: this file originally auto-generated by mib2c using
 *       version : 1.32.2.3 $ of : mfd-top.m2c,v $
 *
 * $Id:$
 */
#ifndef DOT11SSIDSTATSTABLE_H
#define DOT11SSIDSTATSTABLE_H

#ifdef __cplusplus
extern "C" {
#endif


/** @defgroup misc misc: Miscelaneous routines
 *
 * @{
 */
#include <net-snmp/library/asn1.h>

/* other required module components */
    /* *INDENT-OFF*  */
config_require(DOT11-WTP-MIB/dot11SSIDStatsTable/dot11SSIDStatsTable_interface);
config_require(DOT11-WTP-MIB/dot11SSIDStatsTable/dot11SSIDStatsTable_data_access);
config_require(DOT11-WTP-MIB/dot11SSIDStatsTable/dot11SSIDStatsTable_data_get);
config_require(DOT11-WTP-MIB/dot11SSIDStatsTable/dot11SSIDStatsTable_data_set);
    /* *INDENT-ON*  */

/* OID and column number definitions for  */
#include "dot11SSIDStatsTable_oids.h"

/* enum definions */
#include "dot11SSIDStatsTable_enums.h"
#include "ws_dbus_list_interface.h"

/* *********************************************************************
 * function declarations
 */
void init_dot11SSIDStatsTable(void);

/* *********************************************************************
 * Table declarations
 */
/**********************************************************************
 **********************************************************************
 ***
 *** Table dot11SSIDStatsTable
 ***
 **********************************************************************
 **********************************************************************/
/*
 * dot11SSIDStatsTable is subid 6 of wtpWlan.
 * Its status is Current.
 * OID: .1.3.6.1.4.1.33050.6.1.1.5.6, length: 12
*/
/* *********************************************************************
 * When you register your mib, you get to provide a generic
 * pointer that will be passed back to you for most of the
 * functions calls.
 *
 * TODO:100:r: Review all context structures
 */
    /*
     * TODO:101:o: |-> Review dot11SSIDStatsTable registration context.
     */
typedef netsnmp_data_list * dot11SSIDStatsTable_registration_ptr;

/**********************************************************************/
/*
 * TODO:110:r: |-> Review dot11SSIDStatsTable data context structure.
 * This structure is used to represent the data for dot11SSIDStatsTable.
 */
/*
 * This structure contains storage for all the columns defined in the
 * dot11SSIDStatsTable.
 */
typedef struct dot11SSIDStatsTable_data_s {
    
        /*
         * SSIDTxSignalPkts(1)/COUNTER/ASN_COUNTER/u_long(u_long)//l/A/w/e/r/d/h
         */
   u_long   SSIDTxSignalPkts;
    
        /*
         * SSIDRxSignalPkts(2)/COUNTER/ASN_COUNTER/u_long(u_long)//l/A/w/e/r/d/h
         */
   u_long   SSIDRxSignalPkts;
    
        /*
         * SSIDTxDataPkts(3)/COUNTER/ASN_COUNTER/u_long(u_long)//l/A/w/e/r/d/h
         */
   u_long   SSIDTxDataPkts;
    
        /*
         * SSIDRxDataPkts(4)/COUNTER/ASN_COUNTER/u_long(u_long)//l/A/w/e/r/d/h
         */
   u_long   SSIDRxDataPkts;
    
        /*
         * SSIDUplinkDataOctets(5)/COUNTER64/ASN_COUNTER64/U64(U64)//l/A/w/e/r/d/h
         */
   unsigned long long   SSIDUplinkDataOctets;
    
        /*
         * SSIDDwlinkDataOctets(6)/COUNTER64/ASN_COUNTER64/U64(U64)//l/A/w/e/r/d/h
         */
   unsigned long long   SSIDDwlinkDataOctets;
    
        /*
         * SSIDChStatsDwlinkTotRetryPkts(7)/COUNTER/ASN_COUNTER/u_long(u_long)//l/A/w/e/r/d/h
         */
   u_long   SSIDChStatsDwlinkTotRetryPkts;
    
        /*
         * SSIDChStatsUplinkUniFrameCnt(8)/INTEGER/ASN_INTEGER/long(long)//l/A/w/e/r/d/h
         */
   u_long   SSIDChStatsUplinkUniFrameCnt;
    
        /*
         * SSIDChStatsDwlinkUniFrameCnt(9)/INTEGER/ASN_INTEGER/long(long)//l/A/w/e/r/d/h
         */
   u_long   SSIDChStatsDwlinkUniFrameCnt;
    
        /*
         * SSIDUpChStatsFrameNonUniFrameCnt(10)/INTEGER/ASN_INTEGER/long(long)//l/A/w/e/r/d/h
         */
   u_long   SSIDUpChStatsFrameNonUniFrameCnt;
    
        /*
         * SSIDDownChStatsFrameNonUniFrameCnt(11)/INTEGER/ASN_INTEGER/long(long)//l/A/w/e/r/d/h
         */
   u_long   SSIDDownChStatsFrameNonUniFrameCnt;
    
        /*
         * SSIDApChStatsFrameFragRate(12)/INTEGER/ASN_INTEGER/long(long)//l/A/w/e/r/d/h
         */
   u_long   SSIDApChStatsFrameFragRate;
    
        /*
         * SSIDRxCtrlFrameCnt(13)/COUNTER/ASN_COUNTER/u_long(u_long)//l/A/w/e/r/d/h
         */
   u_long   SSIDRxCtrlFrameCnt;
    
        /*
         * SSIDRxDataFrameCnt(14)/COUNTER/ASN_COUNTER/u_long(u_long)//l/A/w/e/r/d/h
         */
   u_long   SSIDRxDataFrameCnt;
    
        /*
         * SSIDRxAuthenFrameCnt(15)/COUNTER/ASN_COUNTER/u_long(u_long)//l/A/w/e/r/d/h
         */
   u_long   SSIDRxAuthenFrameCnt;
    
        /*
         * SSIDRxAssociateFrameCnt(16)/COUNTER/ASN_COUNTER/u_long(u_long)//l/A/w/e/r/d/h
         */
   u_long   SSIDRxAssociateFrameCnt;
    
        /*
         * SSIDTxCtrlFrameCnt(17)/COUNTER/ASN_COUNTER/u_long(u_long)//l/A/w/e/r/d/h
         */
   u_long   SSIDTxCtrlFrameCnt;
    
        /*
         * SSIDTxDataFrameCnt(18)/COUNTER/ASN_COUNTER/u_long(u_long)//l/A/w/e/r/d/h
         */
   u_long   SSIDTxDataFrameCnt;
    
        /*
         * SSIDTxAuthenFrameCnt(19)/COUNTER/ASN_COUNTER/u_long(u_long)//l/A/w/e/r/d/h
         */
   u_long   SSIDTxAuthenFrameCnt;
    
        /*
         * SSIDTxAssociateFrameCnt(20)/COUNTER/ASN_COUNTER/u_long(u_long)//l/A/w/e/r/d/h
         */
   u_long   SSIDTxAssociateFrameCnt;
    
        /*
         * SSIDApChStatsNumStations(21)/INTEGER/ASN_INTEGER/long(long)//l/A/w/e/r/d/h
         */
   u_long   SSIDApChStatsNumStations;
    
        /*
         * SSIDDwlinkTotFrameCnt(22)/COUNTER/ASN_COUNTER/u_long(u_long)//l/A/w/e/r/d/h
         */
   u_long   SSIDDwlinkTotFrameCnt;
    
        /*
         * SSIDUplinkTotFrameCnt(23)/COUNTER/ASN_COUNTER/u_long(u_long)//l/A/w/e/r/d/h
         */
   u_long   SSIDUplinkTotFrameCnt;

		/*
         * SSIDAssociatedUserNum(24)/COUNTER/ASN_COUNTER/u_long(u_long)//l/A/w/e/r/d/h
         */
   u_long   SSIDAssociatedUserNum;
    
} dot11SSIDStatsTable_data;


/*
 * TODO:120:r: |-> Review dot11SSIDStatsTable mib index.
 * This structure is used to represent the index for dot11SSIDStatsTable.
 */
typedef struct dot11SSIDStatsTable_mib_index_s {

        /*
         * wtpMacAddr(1)/Dot11BaseWtpIdTC/ASN_OCTET_STR/char(char)//L/A/w/e/r/d/h
         */
        /** 128 - 1(other indexes) - oid length(14) = 112 */
   char   wtpMacAddr[112];
   size_t      wtpMacAddr_len;

        /*
         * wlanCurrID(1)/INTEGER/ASN_INTEGER/long(long)//l/A/w/e/r/d/h
         */
   long   wlanCurrID;


} dot11SSIDStatsTable_mib_index;

    /*
     * TODO:121:r: |   |-> Review dot11SSIDStatsTable max index length.
     * If you KNOW that your indexes will never exceed a certain
     * length, update this macro to that length.
     *
     * BE VERY CAREFUL TO TAKE INTO ACCOUNT THE MAXIMUM
     * POSSIBLE LENGHT FOR EVERY VARIABLE LENGTH INDEX!
     * Guessing 128 - col/entry(2)  - oid len(12)
*/
#define MAX_dot11SSIDStatsTable_IDX_LEN     114


/* *********************************************************************
 * TODO:130:o: |-> Review dot11SSIDStatsTable Row request (rowreq) context.
 * When your functions are called, you will be passed a
 * dot11SSIDStatsTable_rowreq_ctx pointer.
 */
typedef struct dot11SSIDStatsTable_rowreq_ctx_s {

    /** this must be first for container compare to work */
    netsnmp_index        oid_idx;
    oid                  oid_tmp[MAX_dot11SSIDStatsTable_IDX_LEN];
    
    dot11SSIDStatsTable_mib_index        tbl_idx;
    
    dot11SSIDStatsTable_data              data;

    /*
     * flags per row. Currently, the first (lower) 8 bits are reserved
     * for the user. See mfd.h for other flags.
     */
    u_int                       rowreq_flags;

    /*
     * implementor's context pointer (provided during registration)
     */
    dot11SSIDStatsTable_registration_ptr dot11SSIDStatsTable_reg;

    /*
     * TODO:131:o: |   |-> Add useful data to dot11SSIDStatsTable rowreq context.
     */
    
    /*
     * storage for future expansion
     */
    netsnmp_data_list             *dot11SSIDStatsTable_data_list;

} dot11SSIDStatsTable_rowreq_ctx;

typedef struct dot11SSIDStatsTable_ref_rowreq_ctx_s {
    dot11SSIDStatsTable_rowreq_ctx *rowreq_ctx;
} dot11SSIDStatsTable_ref_rowreq_ctx;

/* *********************************************************************
 * function prototypes
 */
    int dot11SSIDStatsTable_pre_request(dot11SSIDStatsTable_registration_ptr user_context);
    int dot11SSIDStatsTable_post_request(dot11SSIDStatsTable_registration_ptr user_context);


extern oid dot11SSIDStatsTable_oid[];
extern int dot11SSIDStatsTable_oid_size;


#include "dot11SSIDStatsTable_interface.h"
#include "dot11SSIDStatsTable_data_access.h"
#include "dot11SSIDStatsTable_data_get.h"
#include "dot11SSIDStatsTable_data_set.h"

/*
 * DUMMY markers, ignore
 *
 * TODO:099:x: *************************************************************
 * TODO:199:x: *************************************************************
 * TODO:299:x: *************************************************************
 * TODO:399:x: *************************************************************
 * TODO:499:x: *************************************************************
 */

#ifdef __cplusplus
}
#endif

#endif /* DOT11SSIDSTATSTABLE_H */
