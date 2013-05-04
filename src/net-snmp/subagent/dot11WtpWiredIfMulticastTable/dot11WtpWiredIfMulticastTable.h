/*
 * Note: this file originally auto-generated by mib2c using
 *       version : 1.32.2.3 $ of : mfd-top.m2c,v $
 *
 * $Id:$
 */
#ifndef DOT11WTPWIREDIFMULTICASTTABLE_H
#define DOT11WTPWIREDIFMULTICASTTABLE_H

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
config_require(DOT11-WTP-MIB/dot11WtpWiredIfMulticastTable/dot11WtpWiredIfMulticastTable_interface);
config_require(DOT11-WTP-MIB/dot11WtpWiredIfMulticastTable/dot11WtpWiredIfMulticastTable_data_access);
config_require(DOT11-WTP-MIB/dot11WtpWiredIfMulticastTable/dot11WtpWiredIfMulticastTable_data_get);
config_require(DOT11-WTP-MIB/dot11WtpWiredIfMulticastTable/dot11WtpWiredIfMulticastTable_data_set);
    /* *INDENT-ON*  */

/* OID and column number definitions for  */
#include "dot11WtpWiredIfMulticastTable_oids.h"

/* enum definions */
#include "dot11WtpWiredIfMulticastTable_enums.h"
#include "ws_dbus_list_interface.h"

/* *********************************************************************
 * function declarations
 */
void init_dot11WtpWiredIfMulticastTable(void);

/* *********************************************************************
 * Table declarations
 */
/**********************************************************************
 **********************************************************************
 ***
 *** Table dot11WtpWiredIfMulticastTable
 ***
 **********************************************************************
 **********************************************************************/
/*
 * dot11WtpWiredIfMulticastTable is subid 6 of wtpInterface.
 * Its status is Current.
 * OID: .1.3.6.1.4.1.31656.6.1.1.3.6, length: 12
*/
/* *********************************************************************
 * When you register your mib, you get to provide a generic
 * pointer that will be passed back to you for most of the
 * functions calls.
 *
 * TODO:100:r: Review all context structures
 */
    /*
     * TODO:101:o: |-> Review dot11WtpWiredIfMulticastTable registration context.
     */
typedef netsnmp_data_list * dot11WtpWiredIfMulticastTable_registration_ptr;

/**********************************************************************/
/*
 * TODO:110:r: |-> Review dot11WtpWiredIfMulticastTable data context structure.
 * This structure is used to represent the data for dot11WtpWiredIfMulticastTable.
 */
/*
 * This structure contains storage for all the columns defined in the
 * dot11WtpWiredIfMulticastTable.
 */
typedef struct dot11WtpWiredIfMulticastTable_data_s {
    
        /*
         * wtpWiredIfInMulticastPkts(1)/COUNTER/ASN_COUNTER/u_long(u_long)//l/A/w/e/r/d/h
         */
   u_long   wtpWiredIfInMulticastPkts;
    
        /*
         * wtpWiredIfOutMulticastPkts(2)/COUNTER/ASN_COUNTER/u_long(u_long)//l/A/w/e/r/d/h
         */
   u_long   wtpWiredIfOutMulticastPkts;
    
} dot11WtpWiredIfMulticastTable_data;


/*
 * TODO:120:r: |-> Review dot11WtpWiredIfMulticastTable mib index.
 * This structure is used to represent the index for dot11WtpWiredIfMulticastTable.
 */
typedef struct dot11WtpWiredIfMulticastTable_mib_index_s {

        /*
         * wtpMacAddr(1)/Dot11BaseWtpIdTC/ASN_OCTET_STR/char(char)//L/A/w/e/r/d/h
         */
        /** 128 - 1(other indexes) - oid length(14) = 112 */
   char   wtpMacAddr[112];
   size_t      wtpMacAddr_len;

        /*
         * wtpIfIndex(1)/INTEGER/ASN_INTEGER/long(long)//l/A/w/e/r/d/h
         */
   long   wtpIfIndex;


} dot11WtpWiredIfMulticastTable_mib_index;

    /*
     * TODO:121:r: |   |-> Review dot11WtpWiredIfMulticastTable max index length.
     * If you KNOW that your indexes will never exceed a certain
     * length, update this macro to that length.
     *
     * BE VERY CAREFUL TO TAKE INTO ACCOUNT THE MAXIMUM
     * POSSIBLE LENGHT FOR EVERY VARIABLE LENGTH INDEX!
     * Guessing 128 - col/entry(2)  - oid len(12)
*/
#define MAX_dot11WtpWiredIfMulticastTable_IDX_LEN     114


/* *********************************************************************
 * TODO:130:o: |-> Review dot11WtpWiredIfMulticastTable Row request (rowreq) context.
 * When your functions are called, you will be passed a
 * dot11WtpWiredIfMulticastTable_rowreq_ctx pointer.
 */
typedef struct dot11WtpWiredIfMulticastTable_rowreq_ctx_s {

    /** this must be first for container compare to work */
    netsnmp_index        oid_idx;
    oid                  oid_tmp[MAX_dot11WtpWiredIfMulticastTable_IDX_LEN];
    
    dot11WtpWiredIfMulticastTable_mib_index        tbl_idx;
    
    dot11WtpWiredIfMulticastTable_data              data;

    /*
     * flags per row. Currently, the first (lower) 8 bits are reserved
     * for the user. See mfd.h for other flags.
     */
    u_int                       rowreq_flags;

    /*
     * implementor's context pointer (provided during registration)
     */
    dot11WtpWiredIfMulticastTable_registration_ptr dot11WtpWiredIfMulticastTable_reg;

    /*
     * TODO:131:o: |   |-> Add useful data to dot11WtpWiredIfMulticastTable rowreq context.
     */
    
    /*
     * storage for future expansion
     */
    netsnmp_data_list             *dot11WtpWiredIfMulticastTable_data_list;

} dot11WtpWiredIfMulticastTable_rowreq_ctx;

typedef struct dot11WtpWiredIfMulticastTable_ref_rowreq_ctx_s {
    dot11WtpWiredIfMulticastTable_rowreq_ctx *rowreq_ctx;
} dot11WtpWiredIfMulticastTable_ref_rowreq_ctx;

/* *********************************************************************
 * function prototypes
 */
    int dot11WtpWiredIfMulticastTable_pre_request(dot11WtpWiredIfMulticastTable_registration_ptr user_context);
    int dot11WtpWiredIfMulticastTable_post_request(dot11WtpWiredIfMulticastTable_registration_ptr user_context);


extern oid dot11WtpWiredIfMulticastTable_oid[];
extern int dot11WtpWiredIfMulticastTable_oid_size;


#include "dot11WtpWiredIfMulticastTable_interface.h"
#include "dot11WtpWiredIfMulticastTable_data_access.h"
#include "dot11WtpWiredIfMulticastTable_data_get.h"
#include "dot11WtpWiredIfMulticastTable_data_set.h"

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

#endif /* DOT11WTPWIREDIFMULTICASTTABLE_H */
