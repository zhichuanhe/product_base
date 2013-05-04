/*
 * Note: this file originally auto-generated by mib2c using
 *       version : 1.32.2.3 $ of : mfd-top.m2c,v $
 *
 * $Id:$
 */
#ifndef DOT11CONFIGWLANTABLE_H
#define DOT11CONFIGWLANTABLE_H

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
config_require(DOT11-AC-MIB/dot11ConfigWlanTable/dot11ConfigWlanTable_interface);
config_require(DOT11-AC-MIB/dot11ConfigWlanTable/dot11ConfigWlanTable_data_access);
config_require(DOT11-AC-MIB/dot11ConfigWlanTable/dot11ConfigWlanTable_data_get);
config_require(DOT11-AC-MIB/dot11ConfigWlanTable/dot11ConfigWlanTable_data_set);
    /* *INDENT-ON*  */

/* OID and column number definitions for  */
#include "dot11ConfigWlanTable_oids.h"

/* enum definions */
#include "dot11ConfigWlanTable_enums.h"
#include "ws_dbus_list_interface.h"

/* *********************************************************************
 * function declarations
 */
void init_dot11ConfigWlanTable(void);

/* *********************************************************************
 * Table declarations
 */
/**********************************************************************
 **********************************************************************
 ***
 *** Table dot11ConfigWlanTable
 ***
 **********************************************************************
 **********************************************************************/
/*
 * dot11ConfigWlanTable is subid 2 of dot11wlanConfig.
 * Its status is Current.
 * OID: .1.3.6.1.4.1.31656.6.1.2.13.2, length: 12
*/
/* *********************************************************************
 * When you register your mib, you get to provide a generic
 * pointer that will be passed back to you for most of the
 * functions calls.
 *
 * TODO:100:r: Review all context structures
 */
    /*
     * TODO:101:o: |-> Review dot11ConfigWlanTable registration context.
     */
typedef netsnmp_data_list * dot11ConfigWlanTable_registration_ptr;

/**********************************************************************/
/*
 * TODO:110:r: |-> Review dot11ConfigWlanTable data context structure.
 * This structure is used to represent the data for dot11ConfigWlanTable.
 */
/*
 * This structure contains storage for all the columns defined in the
 * dot11ConfigWlanTable.
 */
typedef struct dot11ConfigWlanTable_data_s {

   dbus_parameter parameter;

   u_long local_wlanID;
    
        /*
         * wlanBindSecurity(1)/INTEGER/ASN_INTEGER/long(long)//l/A/W/e/r/d/h
         */
   long   wlanBindSecurity;
    
        /*
         * wlanBindIf(2)/DisplayString/ASN_OCTET_STR/char(char)//L/A/W/e/R/d/H
         */
   char   wlanBindIf[255];
size_t      wlanBindIf_len; /* # of char elements, not bytes */
    
        /*
         * wlanHideEssid(3)/INTEGER/ASN_INTEGER/long(u_long)//l/A/W/E/r/d/h
         */
   u_long   wlanHideEssid;
    
        /*
         * wlanServiceEnable(4)/INTEGER/ASN_INTEGER/long(u_long)//l/A/W/E/r/d/h
         */
   u_long   wlanServiceEnable;
    
        /*
         * wlanMaxConnectUsr(5)/INTEGER/ASN_INTEGER/long(long)//l/A/W/e/R/d/h
         */
   long   wlanMaxConnectUsr;
    
        /*
         * wlanLoadBalanceStatus(6)/INTEGER/ASN_INTEGER/long(u_long)//l/A/W/E/r/d/h
         */
   u_long   wlanLoadBalanceStatus;
    
        /*
         * wlanLoadBalanceStatusBaseOnFlow(7)/INTEGER/ASN_INTEGER/long(u_long)//l/A/W/E/r/d/h
         */
   u_long   wlanLoadBalanceStatusBaseOnFlow;
    
        /*
         * wlanLoadBalanceStatusBaseOnUsr(8)/INTEGER/ASN_INTEGER/long(u_long)//l/A/W/E/r/d/h
         */
   u_long   wlanLoadBalanceStatusBaseOnUsr;
    
        /*
         * wlanLoadBalanceTrafficDiffThreshhd(9)/INTEGER/ASN_INTEGER/long(long)//l/A/W/e/r/d/h
         */
   long   wlanLoadBalanceTrafficDiffThreshhd;
    
        /*
         * wlanLoadBalanceUsersDiffThreshhd(10)/INTEGER/ASN_INTEGER/long(long)//l/A/W/e/r/d/h
         */
   long   wlanLoadBalanceUsersDiffThreshhd;
    
        /*
         * wlanStaOnlineNum(11)/INTEGER/ASN_INTEGER/long(long)//l/A/w/e/r/d/h
         */
   long   wlanStaOnlineNum;
    
        /*
         * wlanUsrWirelessResoUseRate(12)/DisplayString/ASN_OCTET_STR/char(char)//L/A/w/e/R/d/H
         */
   char   wlanUsrWirelessResoUseRate[255];
size_t      wlanUsrWirelessResoUseRate_len; /* # of char elements, not bytes */
    
        /*
         * wlanBindSecType(13)/INTEGER/ASN_INTEGER/long(u_long)//l/A/w/E/r/d/h
         */
   u_long   wlanBindSecType;
    
        /*
         * wlanBindEncryType(14)/INTEGER/ASN_INTEGER/long(u_long)//l/A/w/E/r/d/h
         */
   u_long   wlanBindEncryType;

   
   char   wlanInterfaceIPAddr[255];
   size_t  wlanInterfaceIPAddr_len; 
} dot11ConfigWlanTable_data;


/* *********************************************************************
 * TODO:115:o: |-> Review dot11ConfigWlanTable undo context.
 * We're just going to use the same data structure for our
 * undo_context. If you want to do something more efficent,
 * define your typedef here.
 */
typedef dot11ConfigWlanTable_data dot11ConfigWlanTable_undo_data;

/*
 * TODO:120:r: |-> Review dot11ConfigWlanTable mib index.
 * This structure is used to represent the index for dot11ConfigWlanTable.
 */
typedef struct dot11ConfigWlanTable_mib_index_s {

        /*
         * wlanID(1)/INTEGER/ASN_INTEGER/long(long)//l/A/W/e/r/d/h
         */
   long   wlanID;


} dot11ConfigWlanTable_mib_index;

    /*
     * TODO:121:r: |   |-> Review dot11ConfigWlanTable max index length.
     * If you KNOW that your indexes will never exceed a certain
     * length, update this macro to that length.
*/
#define MAX_dot11ConfigWlanTable_IDX_LEN     1


/* *********************************************************************
 * TODO:130:o: |-> Review dot11ConfigWlanTable Row request (rowreq) context.
 * When your functions are called, you will be passed a
 * dot11ConfigWlanTable_rowreq_ctx pointer.
 */
typedef struct dot11ConfigWlanTable_rowreq_ctx_s {

    /** this must be first for container compare to work */
    netsnmp_index        oid_idx;
    oid                  oid_tmp[MAX_dot11ConfigWlanTable_IDX_LEN];
    
    dot11ConfigWlanTable_mib_index        tbl_idx;
    
    dot11ConfigWlanTable_data              data;
    dot11ConfigWlanTable_undo_data       * undo;
    unsigned int                column_set_flags; /* flags for set columns */


    /*
     * flags per row. Currently, the first (lower) 8 bits are reserved
     * for the user. See mfd.h for other flags.
     */
    u_int                       rowreq_flags;

    /*
     * implementor's context pointer (provided during registration)
     */
    dot11ConfigWlanTable_registration_ptr dot11ConfigWlanTable_reg;

    /*
     * TODO:131:o: |   |-> Add useful data to dot11ConfigWlanTable rowreq context.
     */
    
    /*
     * storage for future expansion
     */
    netsnmp_data_list             *dot11ConfigWlanTable_data_list;

} dot11ConfigWlanTable_rowreq_ctx;

typedef struct dot11ConfigWlanTable_ref_rowreq_ctx_s {
    dot11ConfigWlanTable_rowreq_ctx *rowreq_ctx;
} dot11ConfigWlanTable_ref_rowreq_ctx;

/* *********************************************************************
 * function prototypes
 */
    int dot11ConfigWlanTable_pre_request(dot11ConfigWlanTable_registration_ptr user_context);
    int dot11ConfigWlanTable_post_request(dot11ConfigWlanTable_registration_ptr user_context);

    int dot11ConfigWlanTable_check_dependencies(dot11ConfigWlanTable_rowreq_ctx * rowreq_ctx); 
    int dot11ConfigWlanTable_commit(dot11ConfigWlanTable_rowreq_ctx * rowreq_ctx);

extern oid dot11ConfigWlanTable_oid[];
extern int dot11ConfigWlanTable_oid_size;


#include "dot11ConfigWlanTable_interface.h"
#include "dot11ConfigWlanTable_data_access.h"
#include "dot11ConfigWlanTable_data_get.h"
#include "dot11ConfigWlanTable_data_set.h"

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

#endif /* DOT11CONFIGWLANTABLE_H */
