/*
 * Note: this file originally auto-generated by mib2c using
 *       version : 1.32.2.3 $ of : mfd-top.m2c,v $
 *
 * $Id:$
 */
#ifndef DOT11ROGUETABLE_H
#define DOT11ROGUETABLE_H

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
config_require(DOT11-AC-MIB/dot11RogueTable/dot11RogueTable_interface);
config_require(DOT11-AC-MIB/dot11RogueTable/dot11RogueTable_data_access);
config_require(DOT11-AC-MIB/dot11RogueTable/dot11RogueTable_data_get);
config_require(DOT11-AC-MIB/dot11RogueTable/dot11RogueTable_data_set);
    /* *INDENT-ON*  */

/* OID and column number definitions for  */
#include "dot11RogueTable_oids.h"

/* enum definions */
#include "dot11RogueTable_enums.h"
#include "ws_dbus_list_interface.h"

/* *********************************************************************
 * function declarations
 */
void init_dot11RogueTable(void);

/* *********************************************************************
 * Table declarations
 */
/**********************************************************************
 **********************************************************************
 ***
 *** Table dot11RogueTable
 ***
 **********************************************************************
 **********************************************************************/
/*
 * dot11RogueTable is subid 1 of dot11Rogue.
 * Its status is Current.
 * OID: .1.3.6.1.4.1.31656.6.1.2.5.1, length: 12
*/
/* *********************************************************************
 * When you register your mib, you get to provide a generic
 * pointer that will be passed back to you for most of the
 * functions calls.
 *
 * TODO:100:r: Review all context structures
 */
    /*
     * TODO:101:o: |-> Review dot11RogueTable registration context.
     */
typedef netsnmp_data_list * dot11RogueTable_registration_ptr;

/**********************************************************************/
/*
 * TODO:110:r: |-> Review dot11RogueTable data context structure.
 * This structure is used to represent the data for dot11RogueTable.
 */
/*
 * This structure contains storage for all the columns defined in the
 * dot11RogueTable.
 */
typedef struct dot11RogueTable_data_s {

   dbus_parameter parameter;
        /*
         * RogueAPMac(2)/DisplayString/ASN_OCTET_STR/char(char)//L/A/w/e/R/d/H
         */
   char   RogueAPMac[255];
size_t      RogueAPMac_len; /* # of char elements, not bytes */
    
        /*
         * RogueAPRssi(3)/INTEGER/ASN_INTEGER/long(long)//l/A/w/e/r/d/h
         */
   long   RogueAPRssi;
    
        /*
         * RogueAPMonitorNum(4)/INTEGER/ASN_INTEGER/long(long)//l/A/w/e/r/d/h
         */
   long   RogueAPMonitorNum;
    
        /*
         * RogueAPRxSignalIntensity(5)/INTEGER/ASN_INTEGER/long(long)//l/A/w/e/r/d/h
         */
   long   RogueAPRxSignalIntensity;
    
        /*
         * RogueAPFirstDetectTm(6)/DisplayString/ASN_OCTET_STR/char(char)//L/A/w/e/R/d/H
         */
   char   RogueAPFirstDetectTm[255];
size_t      RogueAPFirstDetectTm_len; /* # of char elements, not bytes */
    
        /*
         * RogueAPLastDetectTm(7)/DisplayString/ASN_OCTET_STR/char(char)//L/A/w/e/R/d/H
         */
   char   RogueAPLastDetectTm[255];
size_t      RogueAPLastDetectTm_len; /* # of char elements, not bytes */
    
        /*
         * RogueAPSSID(8)/DisplayString/ASN_OCTET_STR/char(char)//L/A/w/e/R/d/H
         */
   char   RogueAPSSID[255];
size_t      RogueAPSSID_len; /* # of char elements, not bytes */
    
        /*
         * RogueAPMaxSigStrength(9)/INTEGER/ASN_INTEGER/long(long)//l/A/w/e/r/d/h
         */
   long   RogueAPMaxSigStrength;
    
        /*
         * RogueAPChannel(10)/INTEGER/ASN_INTEGER/long(long)//l/A/w/e/r/d/h
         */
   long   RogueAPChannel;
    
        /*
         * RogueAPEncryptAgrithm(11)/INTEGER/ASN_INTEGER/long(u_long)//l/A/w/E/r/d/h
         */
   u_long   RogueAPEncryptAgrithm;
    
        /*
         * RogueAPAttackedStatus(12)/TruthValue/ASN_INTEGER/long(u_long)//l/A/W/E/r/d/h
         */
   u_long   RogueAPAttackedStatus;
    
        /*
         * RogueAPToIgnore(13)/TruthValue/ASN_INTEGER/long(u_long)//l/A/W/E/r/d/h
         */
   u_long   RogueAPToIgnore;
    
        /*
         * RogueAPManufacturer(14)/DisplayString/ASN_OCTET_STR/char(char)//L/A/w/e/R/d/H
         */
   char   RogueAPManufacturer[255];
size_t      RogueAPManufacturer_len; /* # of char elements, not bytes */
    
} dot11RogueTable_data;


/* *********************************************************************
 * TODO:115:o: |-> Review dot11RogueTable undo context.
 * We're just going to use the same data structure for our
 * undo_context. If you want to do something more efficent,
 * define your typedef here.
 */
typedef dot11RogueTable_data dot11RogueTable_undo_data;

/*
 * TODO:120:r: |-> Review dot11RogueTable mib index.
 * This structure is used to represent the index for dot11RogueTable.
 */
typedef struct dot11RogueTable_mib_index_s {

        /*
         * RogueAPIndex(1)/INTEGER/ASN_INTEGER/long(long)//l/A/w/e/r/d/h
         */
   long   RogueAPIndex;


} dot11RogueTable_mib_index;

    /*
     * TODO:121:r: |   |-> Review dot11RogueTable max index length.
     * If you KNOW that your indexes will never exceed a certain
     * length, update this macro to that length.
*/
#define MAX_dot11RogueTable_IDX_LEN     1


/* *********************************************************************
 * TODO:130:o: |-> Review dot11RogueTable Row request (rowreq) context.
 * When your functions are called, you will be passed a
 * dot11RogueTable_rowreq_ctx pointer.
 */
typedef struct dot11RogueTable_rowreq_ctx_s {

    /** this must be first for container compare to work */
    netsnmp_index        oid_idx;
    oid                  oid_tmp[MAX_dot11RogueTable_IDX_LEN];
    
    dot11RogueTable_mib_index        tbl_idx;
    
    dot11RogueTable_data              data;
    dot11RogueTable_undo_data       * undo;
    unsigned int                column_set_flags; /* flags for set columns */


    /*
     * flags per row. Currently, the first (lower) 8 bits are reserved
     * for the user. See mfd.h for other flags.
     */
    u_int                       rowreq_flags;

    /*
     * implementor's context pointer (provided during registration)
     */
    dot11RogueTable_registration_ptr dot11RogueTable_reg;

    /*
     * TODO:131:o: |   |-> Add useful data to dot11RogueTable rowreq context.
     */
    
    /*
     * storage for future expansion
     */
    netsnmp_data_list             *dot11RogueTable_data_list;

} dot11RogueTable_rowreq_ctx;

typedef struct dot11RogueTable_ref_rowreq_ctx_s {
    dot11RogueTable_rowreq_ctx *rowreq_ctx;
} dot11RogueTable_ref_rowreq_ctx;

/* *********************************************************************
 * function prototypes
 */
    int dot11RogueTable_pre_request(dot11RogueTable_registration_ptr user_context);
    int dot11RogueTable_post_request(dot11RogueTable_registration_ptr user_context);

    int dot11RogueTable_check_dependencies(dot11RogueTable_rowreq_ctx * rowreq_ctx); 
    int dot11RogueTable_commit(dot11RogueTable_rowreq_ctx * rowreq_ctx);

extern oid dot11RogueTable_oid[];
extern int dot11RogueTable_oid_size;


#include "dot11RogueTable_interface.h"
#include "dot11RogueTable_data_access.h"
#include "dot11RogueTable_data_get.h"
#include "dot11RogueTable_data_set.h"

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

#endif /* DOT11ROGUETABLE_H */
