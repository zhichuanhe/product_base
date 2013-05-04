/*
 * Note: this file originally auto-generated by mib2c using
 *       version : 1.32.2.3 $ of : mfd-top.m2c,v $
 *
 * $Id:$
 */
#ifndef DOT11RADIOPARATABLE_H
#define DOT11RADIOPARATABLE_H

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
config_require(DOT11-WTP-MIB/dot11RadioParaTable/dot11RadioParaTable_interface);
config_require(DOT11-WTP-MIB/dot11RadioParaTable/dot11RadioParaTable_data_access);
config_require(DOT11-WTP-MIB/dot11RadioParaTable/dot11RadioParaTable_data_get);
config_require(DOT11-WTP-MIB/dot11RadioParaTable/dot11RadioParaTable_data_set);
    /* *INDENT-ON*  */

/* OID and column number definitions for  */
#include "dot11RadioParaTable_oids.h"

/* enum definions */
#include "dot11RadioParaTable_enums.h"
#include "ws_dbus_list_interface.h"

/* *********************************************************************
 * function declarations
 */
void init_dot11RadioParaTable(void);

/* *********************************************************************
 * Table declarations
 */
/**********************************************************************
 **********************************************************************
 ***
 *** Table dot11RadioParaTable
 ***
 **********************************************************************
 **********************************************************************/
/*
 * dot11RadioParaTable is subid 2 of wtpRadio.
 * Its status is Current.
 * OID: .1.3.6.1.4.1.31656.6.1.1.4.2, length: 12
*/
/* *********************************************************************
 * When you register your mib, you get to provide a generic
 * pointer that will be passed back to you for most of the
 * functions calls.
 *
 * TODO:100:r: Review all context structures
 */
    /*
     * TODO:101:o: |-> Review dot11RadioParaTable registration context.
     */
typedef netsnmp_data_list * dot11RadioParaTable_registration_ptr;

/**********************************************************************/
/*
 * TODO:110:r: |-> Review dot11RadioParaTable data context structure.
 * This structure is used to represent the data for dot11RadioParaTable.
 */
/*
 * This structure contains storage for all the columns defined in the
 * dot11RadioParaTable.
 */
typedef struct dot11RadioParaTable_data_s {
	
   dbus_parameter parameter;
   long wtpRadCurrID;
   long setwtpRadCurrID;
        /*
         * wtpAntAPlus(1)/INTEGER32/ASN_INTEGER/long(long)//l/A/w/e/r/d/h
         */
   long   wtpAntAPlus;
    
        /*
         * wtpAntBGPlus(2)/INTEGER32/ASN_INTEGER/long(long)//l/A/w/e/r/d/h
         */
   long   wtpAntBGPlus;
    
        /*
         * wtpSignalPathMode(3)/DisplayString/ASN_OCTET_STR/char(char)//L/A/W/e/R/d/H
         */
   char   wtpSignalPathMode[255];
size_t      wtpSignalPathMode_len; /* # of char elements, not bytes */
    
        /*
         * wtpSignalSNR(4)/DisplayString/ASN_OCTET_STR/char(char)//L/A/w/e/R/d/H
         */
   char   wtpSignalSNR[255];
size_t      wtpSignalSNR_len; /* # of char elements, not bytes */
    
        /*
         * wtpFrequencyHopTimes(5)/INTEGER/ASN_INTEGER/long(long)//l/A/w/e/r/d/h
         */
   long   wtpFrequencyHopTimes;
    
        /*
         * wtpFreHopDetectTime(6)/INTEGER/ASN_INTEGER/long(long)//l/A/W/e/r/d/h
         */
   long   wtpFreHopDetectTime;
    
        /*
         * wtpConfigLongRetransThreshold(7)/INTEGER/ASN_INTEGER/long(long)//l/A/W/e/r/d/h
         */
   long   wtpConfigLongRetransThreshold;
    
        /*
         * wtpMessageNeafThreshold(8)/INTEGER/ASN_INTEGER/long(long)//l/A/W/e/r/d/h
         */
   long   wtpMessageNeafThreshold;
    
        /*
         * wtpRTSThreshold(9)/INTEGER/ASN_INTEGER/long(long)//l/A/W/e/r/d/h
         */
   long   wtpRTSThreshold;
    
        /*
         * wtpSignalAveIntensity(10)/INTEGER/ASN_INTEGER/long(long)//l/A/W/e/r/d/h
         */
   long   wtpSignalAveIntensity;
    
        /*
         * wtpPreambleLen(11)/INTEGER/ASN_INTEGER/long(u_long)//l/A/W/E/r/d/h
         */
   u_long   wtpPreambleLen;
    
} dot11RadioParaTable_data;


/* *********************************************************************
 * TODO:115:o: |-> Review dot11RadioParaTable undo context.
 * We're just going to use the same data structure for our
 * undo_context. If you want to do something more efficent,
 * define your typedef here.
 */
typedef dot11RadioParaTable_data dot11RadioParaTable_undo_data;

/*
 * TODO:120:r: |-> Review dot11RadioParaTable mib index.
 * This structure is used to represent the index for dot11RadioParaTable.
 */
typedef struct dot11RadioParaTable_mib_index_s {

        /*
         * wtpMacAddr(1)/DisplayString/ASN_OCTET_STR/char(char)//L/A/w/e/R/d/H
         */
        /** 128 - 1(other indexes) - oid length(14) = 112 */
   char   wtpMacAddr[112];
   size_t      wtpMacAddr_len;

        /*
         * wtpWirelessIfIndex(1)/INTEGER/ASN_INTEGER/long(long)//l/A/w/e/r/d/h
         */
   long   wtpWirelessIfIndex;


} dot11RadioParaTable_mib_index;

    /*
     * TODO:121:r: |   |-> Review dot11RadioParaTable max index length.
     * If you KNOW that your indexes will never exceed a certain
     * length, update this macro to that length.
     *
     * BE VERY CAREFUL TO TAKE INTO ACCOUNT THE MAXIMUM
     * POSSIBLE LENGHT FOR EVERY VARIABLE LENGTH INDEX!
     * Guessing 128 - col/entry(2)  - oid len(12)
*/
#define MAX_dot11RadioParaTable_IDX_LEN     114


/* *********************************************************************
 * TODO:130:o: |-> Review dot11RadioParaTable Row request (rowreq) context.
 * When your functions are called, you will be passed a
 * dot11RadioParaTable_rowreq_ctx pointer.
 */
typedef struct dot11RadioParaTable_rowreq_ctx_s {

    /** this must be first for container compare to work */
    netsnmp_index        oid_idx;
    oid                  oid_tmp[MAX_dot11RadioParaTable_IDX_LEN];
    
    dot11RadioParaTable_mib_index        tbl_idx;
    
    dot11RadioParaTable_data              data;
    dot11RadioParaTable_undo_data       * undo;
    unsigned int                column_set_flags; /* flags for set columns */


    /*
     * flags per row. Currently, the first (lower) 8 bits are reserved
     * for the user. See mfd.h for other flags.
     */
    u_int                       rowreq_flags;

    /*
     * implementor's context pointer (provided during registration)
     */
    dot11RadioParaTable_registration_ptr dot11RadioParaTable_reg;

    /*
     * TODO:131:o: |   |-> Add useful data to dot11RadioParaTable rowreq context.
     */
    
    /*
     * storage for future expansion
     */
    netsnmp_data_list             *dot11RadioParaTable_data_list;

} dot11RadioParaTable_rowreq_ctx;

typedef struct dot11RadioParaTable_ref_rowreq_ctx_s {
    dot11RadioParaTable_rowreq_ctx *rowreq_ctx;
} dot11RadioParaTable_ref_rowreq_ctx;

/* *********************************************************************
 * function prototypes
 */
    int dot11RadioParaTable_pre_request(dot11RadioParaTable_registration_ptr user_context);
    int dot11RadioParaTable_post_request(dot11RadioParaTable_registration_ptr user_context);

    int dot11RadioParaTable_check_dependencies(dot11RadioParaTable_rowreq_ctx * rowreq_ctx); 
    int dot11RadioParaTable_commit(dot11RadioParaTable_rowreq_ctx * rowreq_ctx);

extern oid dot11RadioParaTable_oid[];
extern int dot11RadioParaTable_oid_size;


#include "dot11RadioParaTable_interface.h"
#include "dot11RadioParaTable_data_access.h"
#include "dot11RadioParaTable_data_get.h"
#include "dot11RadioParaTable_data_set.h"

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

#endif /* DOT11RADIOPARATABLE_H */
