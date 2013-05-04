/*
 * Note: this file originally auto-generated by mib2c using
 *       version : 1.32.2.3 $ of : mfd-top.m2c,v $
 *
 * $Id:$
 */
#ifndef DOT11PORTALUSERINFOTABLE_H
#define DOT11PORTALUSERINFOTABLE_H

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
config_require(DOT11-WTP-MIB/dot11PortalUserInfoTable/dot11PortalUserInfoTable_interface);
config_require(DOT11-WTP-MIB/dot11PortalUserInfoTable/dot11PortalUserInfoTable_data_access);
config_require(DOT11-WTP-MIB/dot11PortalUserInfoTable/dot11PortalUserInfoTable_data_get);
config_require(DOT11-WTP-MIB/dot11PortalUserInfoTable/dot11PortalUserInfoTable_data_set);
    /* *INDENT-ON*  */

/* OID and column number definitions for  */
#include "dot11PortalUserInfoTable_oids.h"

/* enum definions */
#include "dot11PortalUserInfoTable_enums.h"

/* *********************************************************************
 * function declarations
 */
void init_dot11PortalUserInfoTable(void);

/* *********************************************************************
 * Table declarations
 */
/**********************************************************************
 **********************************************************************
 ***
 *** Table dot11PortalUserInfoTable
 ***
 **********************************************************************
 **********************************************************************/
/*
 * dot11PortalUserInfoTable is subid 2 of wlanPortal.
 * Its status is Current.
 * OID: .1.3.6.1.4.1.31656.6.1.1.17.2, length: 12
*/
/* *********************************************************************
 * When you register your mib, you get to provide a generic
 * pointer that will be passed back to you for most of the
 * functions calls.
 *
 * TODO:100:r: Review all context structures
 */
    /*
     * TODO:101:o: |-> Review dot11PortalUserInfoTable registration context.
     */
typedef netsnmp_data_list * dot11PortalUserInfoTable_registration_ptr;

/**********************************************************************/
/*
 * TODO:110:r: |-> Review dot11PortalUserInfoTable data context structure.
 * This structure is used to represent the data for dot11PortalUserInfoTable.
 */
/*
 * This structure contains storage for all the columns defined in the
 * dot11PortalUserInfoTable.
 */
typedef struct dot11PortalUserInfoTable_data_s {
    
        /*
         * portalUserip(1)/IPADDR/ASN_IPADDRESS/u_long(u_long)//l/A/w/e/r/d/h
         */
   u_long   portalUserip;
    
        /*
         * portalUsername(2)/DisplayString/ASN_OCTET_STR/char(char)//L/A/w/e/R/d/H
         */
   char   portalUsername[255];
size_t      portalUsername_len; /* # of char elements, not bytes */
    
        /*
         * portalUserUptime(3)/DisplayString/ASN_OCTET_STR/char(char)//L/A/w/e/R/d/H
         */
   char   portalUserUptime[255];
size_t      portalUserUptime_len; /* # of char elements, not bytes */
    
        /*
         * portalUseronline(4)/TICKS/ASN_TIMETICKS/u_long(u_long)//l/A/w/e/r/d/h
         */
   u_long   portalUseronline;

   u_long   userAuthType;
    
} dot11PortalUserInfoTable_data;


/*
 * TODO:120:r: |-> Review dot11PortalUserInfoTable mib index.
 * This structure is used to represent the index for dot11PortalUserInfoTable.
 */
typedef struct dot11PortalUserInfoTable_mib_index_s {

        /*
         * wtpMacAddr(1)/DisplayString/ASN_OCTET_STR/char(char)//L/A/w/e/R/d/H
         */
        /** 128 - 1(other indexes) - oid length(14) = 112 */
   char   wtpMacAddr[112];
   size_t      wtpMacAddr_len;

        /*
         * wtpStaMacAddr(1)/DisplayString/ASN_OCTET_STR/char(char)//L/A/w/e/R/d/H
         */
        /** 128 - 1(other indexes) - oid length(14) = 112 */
   char   wtpStaMacAddr[112];
   size_t      wtpStaMacAddr_len;


} dot11PortalUserInfoTable_mib_index;

    /*
     * TODO:121:r: |   |-> Review dot11PortalUserInfoTable max index length.
     * If you KNOW that your indexes will never exceed a certain
     * length, update this macro to that length.
     *
     * BE VERY CAREFUL TO TAKE INTO ACCOUNT THE MAXIMUM
     * POSSIBLE LENGHT FOR EVERY VARIABLE LENGTH INDEX!
     * Guessing 128 - col/entry(2)  - oid len(12)
*/
#define MAX_dot11PortalUserInfoTable_IDX_LEN     114


/* *********************************************************************
 * TODO:130:o: |-> Review dot11PortalUserInfoTable Row request (rowreq) context.
 * When your functions are called, you will be passed a
 * dot11PortalUserInfoTable_rowreq_ctx pointer.
 */
typedef struct dot11PortalUserInfoTable_rowreq_ctx_s {

    /** this must be first for container compare to work */
    netsnmp_index        oid_idx;
    oid                  oid_tmp[MAX_dot11PortalUserInfoTable_IDX_LEN];
    
    dot11PortalUserInfoTable_mib_index        tbl_idx;
    
    dot11PortalUserInfoTable_data              data;

    /*
     * flags per row. Currently, the first (lower) 8 bits are reserved
     * for the user. See mfd.h for other flags.
     */
    u_int                       rowreq_flags;

    /*
     * implementor's context pointer (provided during registration)
     */
    dot11PortalUserInfoTable_registration_ptr dot11PortalUserInfoTable_reg;

    /*
     * TODO:131:o: |   |-> Add useful data to dot11PortalUserInfoTable rowreq context.
     */
    
    /*
     * storage for future expansion
     */
    netsnmp_data_list             *dot11PortalUserInfoTable_data_list;

} dot11PortalUserInfoTable_rowreq_ctx;

typedef struct dot11PortalUserInfoTable_ref_rowreq_ctx_s {
    dot11PortalUserInfoTable_rowreq_ctx *rowreq_ctx;
} dot11PortalUserInfoTable_ref_rowreq_ctx;

/* *********************************************************************
 * function prototypes
 */
    int dot11PortalUserInfoTable_pre_request(dot11PortalUserInfoTable_registration_ptr user_context);
    int dot11PortalUserInfoTable_post_request(dot11PortalUserInfoTable_registration_ptr user_context);


extern oid dot11PortalUserInfoTable_oid[];
extern int dot11PortalUserInfoTable_oid_size;


#include "dot11PortalUserInfoTable_interface.h"
#include "dot11PortalUserInfoTable_data_access.h"
#include "dot11PortalUserInfoTable_data_get.h"
#include "dot11PortalUserInfoTable_data_set.h"

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

#endif /* DOT11PORTALUSERINFOTABLE_H */
