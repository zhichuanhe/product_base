/*
 * Note: this file originally auto-generated by mib2c using
 *       version : 1.32.2.3 $ of : mfd-top.m2c,v $
 *
 * $Id:$
 */
#ifndef DOT11WTPWAPICONFIGTABLE_H
#define DOT11WTPWAPICONFIGTABLE_H

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
config_require(DOT11-WTP-MIB/dot11WtpWAPIConfigTable/dot11WtpWAPIConfigTable_interface);
config_require(DOT11-WTP-MIB/dot11WtpWAPIConfigTable/dot11WtpWAPIConfigTable_data_access);
config_require(DOT11-WTP-MIB/dot11WtpWAPIConfigTable/dot11WtpWAPIConfigTable_data_get);
config_require(DOT11-WTP-MIB/dot11WtpWAPIConfigTable/dot11WtpWAPIConfigTable_data_set);
    /* *INDENT-ON*  */

/* OID and column number definitions for  */
#include "dot11WtpWAPIConfigTable_oids.h"

/* enum definions */
#include "dot11WtpWAPIConfigTable_enums.h"
#include "ws_dbus_list_interface.h"

/* *********************************************************************
 * function declarations
 */
void init_dot11WtpWAPIConfigTable(void);

/* *********************************************************************
 * Table declarations
 */
/**********************************************************************
 **********************************************************************
 ***
 *** Table dot11WtpWAPIConfigTable
 ***
 **********************************************************************
 **********************************************************************/
/*
 * dot11WtpWAPIConfigTable is subid 10 of wtpWAPI.
 * Its status is Current.
 * OID: .1.3.6.1.4.1.31656.6.1.1.10.10, length: 12
*/
/* *********************************************************************
 * When you register your mib, you get to provide a generic
 * pointer that will be passed back to you for most of the
 * functions calls.
 *
 * TODO:100:r: Review all context structures
 */
    /*
     * TODO:101:o: |-> Review dot11WtpWAPIConfigTable registration context.
     */
typedef netsnmp_data_list * dot11WtpWAPIConfigTable_registration_ptr;

/**********************************************************************/
/*
 * TODO:110:r: |-> Review dot11WtpWAPIConfigTable data context structure.
 * This structure is used to represent the data for dot11WtpWAPIConfigTable.
 */
/*
 * This structure contains storage for all the columns defined in the
 * dot11WtpWAPIConfigTable.
 */
typedef struct dot11WtpWAPIConfigTable_data_s {
   dbus_parameter parameter;
   long localWlanID;
   long SecurityID;
   
        /*
         * wtpWapiConfigVersion(2)/INTEGER32/ASN_INTEGER/long(long)//l/A/w/e/r/d/h
         */
   long   wtpWapiConfigVersion;
    
        /*
         * wtpWapiControlledAuthenControlenabled(3)/TruthValue/ASN_INTEGER/long(u_long)//l/A/w/E/r/d/h
         */
   u_long   wtpWapiControlledAuthenControlenabled;
    
        /*
         * wtpWapiControlledPortControl(4)/DisplayString/ASN_OCTET_STR/char(char)//L/A/w/e/R/d/H
         */
   char   wtpWapiControlledPortControl[255];
size_t      wtpWapiControlledPortControl_len; /* # of char elements, not bytes */
    
        /*
         * wtpWapiWPIOptionImplement(5)/TruthValue/ASN_INTEGER/long(u_long)//l/A/w/E/r/d/h
         */
   u_long   wtpWapiWPIOptionImplement;
    
        /*
         * wtpWapiWPIPreauthImplemented(6)/TruthValue/ASN_INTEGER/long(u_long)//l/A/w/E/r/d/h
         */
   u_long   wtpWapiWPIPreauthImplemented;
    
        /*
         * wtpWapiEnabled(7)/TruthValue/ASN_INTEGER/long(u_long)//l/A/W/E/r/d/h
         */
   u_long   wtpWapiEnabled;
    
        /*
         * wtpWapiPreauthEnabled(8)/TruthValue/ASN_INTEGER/long(u_long)//l/A/W/E/r/d/h
         */
   u_long   wtpWapiPreauthEnabled;
    
        /*
         * wtpWapiUnicastKeysSupported(9)/UNSIGNED32/ASN_UNSIGNED/u_long(u_long)//l/A/w/e/r/d/h
         */
   u_long   wtpWapiUnicastKeysSupported;
    
        /*
         * wtpWapiUnicastRekeyMethod(10)/INTEGER/ASN_INTEGER/long(u_long)//l/A/W/E/r/d/h
         */
   u_long   wtpWapiUnicastRekeyMethod;
    
        /*
         * wtpWapiUnicastRekeyTime(11)/UNSIGNED32/ASN_UNSIGNED/u_long(u_long)//l/A/W/e/r/d/h
         */
   u_long   wtpWapiUnicastRekeyTime;
    
        /*
         * wtpWapiUnicastRekeyPackets(12)/UNSIGNED32/ASN_UNSIGNED/u_long(u_long)//l/A/W/e/r/d/h
         */
   u_long   wtpWapiUnicastRekeyPackets;
    
        /*
         * wtpWapiMulticastCipher(13)/DisplayString/ASN_OCTET_STR/char(char)//L/A/W/e/R/d/H
         */
   char   wtpWapiMulticastCipher[255];
size_t      wtpWapiMulticastCipher_len; /* # of char elements, not bytes */
    
        /*
         * wtpWapiMulticastRekeyMethod(14)/INTEGER/ASN_INTEGER/long(long)//l/A/W/e/r/d/h
         */
   long   wtpWapiMulticastRekeyMethod;
    
        /*
         * wtpWapiMulticastRekeyTime(15)/UNSIGNED32/ASN_UNSIGNED/u_long(u_long)//l/A/W/e/r/d/h
         */
   u_long   wtpWapiMulticastRekeyTime;
    
        /*
         * wtpWapiMulticastRekeyPackets(16)/UNSIGNED32/ASN_UNSIGNED/u_long(u_long)//l/A/W/e/r/d/h
         */
   u_long   wtpWapiMulticastRekeyPackets;
    
        /*
         * wtpWapiMulticastRekeyStrict(17)/TruthValue/ASN_INTEGER/long(u_long)//l/A/W/E/r/d/h
         */
   u_long   wtpWapiMulticastRekeyStrict;
    
        /*
         * wtpWapiPSKValue(18)/DisplayString/ASN_OCTET_STR/char(char)//L/A/W/e/R/d/H
         */
   char   wtpWapiPSKValue[255];
size_t      wtpWapiPSKValue_len; /* # of char elements, not bytes */
    
        /*
         * wtpWapiPSKPassPhrase(19)/DisplayString/ASN_OCTET_STR/char(char)//L/A/W/e/R/d/H
         */
   char   wtpWapiPSKPassPhrase[255];
size_t      wtpWapiPSKPassPhrase_len; /* # of char elements, not bytes */
    
        /*
         * wtpWapiCertificateUpdateCount(20)/UNSIGNED32/ASN_UNSIGNED/u_long(u_long)//l/A/W/e/r/d/h
         */
   u_long   wtpWapiCertificateUpdateCount;
    
        /*
         * wtpWapiMulticastUpdateCount(21)/UNSIGNED32/ASN_UNSIGNED/u_long(u_long)//l/A/W/e/r/d/h
         */
   u_long   wtpWapiMulticastUpdateCount;
    
        /*
         * wtpWapiUnicastUpdateCount(22)/UNSIGNED32/ASN_UNSIGNED/u_long(u_long)//l/A/W/e/r/d/h
         */
   u_long   wtpWapiUnicastUpdateCount;
    
        /*
         * wtpWapiMulticastCipherSize(23)/UNSIGNED32/ASN_UNSIGNED/u_long(u_long)//l/A/w/e/r/d/h
         */
   u_long   wtpWapiMulticastCipherSize;
    
        /*
         * wtpWapiBKLifetime(24)/UNSIGNED32/ASN_UNSIGNED/u_long(u_long)//l/A/W/e/r/d/h
         */
   u_long   wtpWapiBKLifetime;
    
        /*
         * wtpWapiBKReauthThreshold(25)/UNSIGNED32/ASN_UNSIGNED/u_long(u_long)//l/A/W/e/r/d/h
         */
   u_long   wtpWapiBKReauthThreshold;
    
        /*
         * wtpWapiSATimeout(26)/UNSIGNED32/ASN_UNSIGNED/u_long(u_long)//l/A/W/e/r/d/h
         */
   u_long   wtpWapiSATimeout;
    
        /*
         * wtpWapiAuthSuiteSelected(27)/DisplayString/ASN_OCTET_STR/char(char)//L/A/w/e/R/d/H
         */
   char   wtpWapiAuthSuiteSelected[255];
size_t      wtpWapiAuthSuiteSelected_len; /* # of char elements, not bytes */
    
        /*
         * wtpWapiUnicastCipherSelected(28)/DisplayString/ASN_OCTET_STR/char(char)//L/A/w/e/R/d/H
         */
   char   wtpWapiUnicastCipherSelected[255];
size_t      wtpWapiUnicastCipherSelected_len; /* # of char elements, not bytes */
    
        /*
         * wtpWapiMulticastCipherSelected(29)/DisplayString/ASN_OCTET_STR/char(char)//L/A/w/e/R/d/H
         */
   char   wtpWapiMulticastCipherSelected[255];
size_t      wtpWapiMulticastCipherSelected_len; /* # of char elements, not bytes */
    
        /*
         * wtpWapiBKIDUsed(30)/DisplayString/ASN_OCTET_STR/char(char)//L/A/w/e/R/d/H
         */
   char   wtpWapiBKIDUsed[255];
size_t      wtpWapiBKIDUsed_len; /* # of char elements, not bytes */
    
        /*
         * wtpWapiAuthSuiteRequested(31)/DisplayString/ASN_OCTET_STR/char(char)//L/A/w/e/R/d/H
         */
   char   wtpWapiAuthSuiteRequested[255];
size_t      wtpWapiAuthSuiteRequested_len; /* # of char elements, not bytes */
    
        /*
         * wtpWapiUnicastCipherRequested(32)/DisplayString/ASN_OCTET_STR/char(char)//L/A/w/e/R/d/H
         */
   char   wtpWapiUnicastCipherRequested[255];
size_t      wtpWapiUnicastCipherRequested_len; /* # of char elements, not bytes */
    
        /*
         * wtpWapiMulticastCipherRequested(33)/DisplayString/ASN_OCTET_STR/char(char)//L/A/w/e/R/d/H
         */
   char   wtpWapiMulticastCipherRequested[255];
size_t      wtpWapiMulticastCipherRequested_len; /* # of char elements, not bytes */
    
} dot11WtpWAPIConfigTable_data;


/* *********************************************************************
 * TODO:115:o: |-> Review dot11WtpWAPIConfigTable undo context.
 * We're just going to use the same data structure for our
 * undo_context. If you want to do something more efficent,
 * define your typedef here.
 */
typedef dot11WtpWAPIConfigTable_data dot11WtpWAPIConfigTable_undo_data;

/*
 * TODO:120:r: |-> Review dot11WtpWAPIConfigTable mib index.
 * This structure is used to represent the index for dot11WtpWAPIConfigTable.
 */
typedef struct dot11WtpWAPIConfigTable_mib_index_s {

        /*
         * WapiWlanID(1)/INTEGER/ASN_INTEGER/long(long)//l/A/w/e/r/d/h
         */
   long   WapiWlanID;


} dot11WtpWAPIConfigTable_mib_index;

    /*
     * TODO:121:r: |   |-> Review dot11WtpWAPIConfigTable max index length.
     * If you KNOW that your indexes will never exceed a certain
     * length, update this macro to that length.
*/
#define MAX_dot11WtpWAPIConfigTable_IDX_LEN     1


/* *********************************************************************
 * TODO:130:o: |-> Review dot11WtpWAPIConfigTable Row request (rowreq) context.
 * When your functions are called, you will be passed a
 * dot11WtpWAPIConfigTable_rowreq_ctx pointer.
 */
typedef struct dot11WtpWAPIConfigTable_rowreq_ctx_s {

    /** this must be first for container compare to work */
    netsnmp_index        oid_idx;
    oid                  oid_tmp[MAX_dot11WtpWAPIConfigTable_IDX_LEN];
    
    dot11WtpWAPIConfigTable_mib_index        tbl_idx;
    
    dot11WtpWAPIConfigTable_data              data;
    dot11WtpWAPIConfigTable_undo_data       * undo;
    unsigned int                column_set_flags; /* flags for set columns */


    /*
     * flags per row. Currently, the first (lower) 8 bits are reserved
     * for the user. See mfd.h for other flags.
     */
    u_int                       rowreq_flags;

    /*
     * implementor's context pointer (provided during registration)
     */
    dot11WtpWAPIConfigTable_registration_ptr dot11WtpWAPIConfigTable_reg;

    /*
     * TODO:131:o: |   |-> Add useful data to dot11WtpWAPIConfigTable rowreq context.
     */
    
    /*
     * storage for future expansion
     */
    netsnmp_data_list             *dot11WtpWAPIConfigTable_data_list;

} dot11WtpWAPIConfigTable_rowreq_ctx;

typedef struct dot11WtpWAPIConfigTable_ref_rowreq_ctx_s {
    dot11WtpWAPIConfigTable_rowreq_ctx *rowreq_ctx;
} dot11WtpWAPIConfigTable_ref_rowreq_ctx;

/* *********************************************************************
 * function prototypes
 */
    int dot11WtpWAPIConfigTable_pre_request(dot11WtpWAPIConfigTable_registration_ptr user_context);
    int dot11WtpWAPIConfigTable_post_request(dot11WtpWAPIConfigTable_registration_ptr user_context);

    int dot11WtpWAPIConfigTable_check_dependencies(dot11WtpWAPIConfigTable_rowreq_ctx * rowreq_ctx); 
    int dot11WtpWAPIConfigTable_commit(dot11WtpWAPIConfigTable_rowreq_ctx * rowreq_ctx);

extern oid dot11WtpWAPIConfigTable_oid[];
extern int dot11WtpWAPIConfigTable_oid_size;


#include "dot11WtpWAPIConfigTable_interface.h"
#include "dot11WtpWAPIConfigTable_data_access.h"
#include "dot11WtpWAPIConfigTable_data_get.h"
#include "dot11WtpWAPIConfigTable_data_set.h"

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

#endif /* DOT11WTPWAPICONFIGTABLE_H */
