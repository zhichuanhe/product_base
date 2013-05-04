/*
 * Note: this file originally auto-generated by mib2c using
 *       version : 1.18.2.1 $ of : mfd-data-get.m2c,v $ 
 *
 * $Id:$
 */
/* standard Net-SNMP includes */
#include <net-snmp/net-snmp-config.h>
#include <net-snmp/net-snmp-includes.h>
#include <net-snmp/agent/net-snmp-agent-includes.h>

/* include our parent header */
#include "dot11PortalUserInfoTable.h"


/** @defgroup data_get data_get: Routines to get data
 *
 * TODO:230:M: Implement dot11PortalUserInfoTable get routines.
 * TODO:240:M: Implement dot11PortalUserInfoTable mapping routines (if any).
 *
 * These routine are used to get the value for individual objects. The
 * row context is passed, along with a pointer to the memory where the
 * value should be copied.
 *
 * @{
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

/* ---------------------------------------------------------------------
 * TODO:200:r: Implement dot11PortalUserInfoTable data context functions.
 */

/*---------------------------------------------------------------------
 * DOT11-AC-MIB::dot11ConfigTrapSwitchEntry.wtpMacAddr
 * wtpMacAddr is subid 1 of dot11ConfigTrapSwitchEntry.
 * Its status is Current, and its access level is ReadOnly.
 * OID: .1.3.6.1.4.1.31656.6.1.2.18.2.1.1
 * Description:
AP Mac  Address Group.
 *
 * Attributes:
 *   accessible 1     isscalar 0     enums  0      hasdefval 0
 *   readable   1     iscolumn 1     ranges 1      hashint   1
 *   settable   0
 *   hint: 255a
 *
 * Ranges:  0 - 255;
 *
 * Its syntax is DisplayString (based on perltype OCTETSTR)
 * The net-snmp type is ASN_OCTET_STR. The C type decl is char (char)
 * This data type requires a length.  (Max 255)
 */
/**
 * map a value from its original native format to the MIB format.
 *
 * @retval MFD_SUCCESS         : success
 * @retval MFD_ERROR           : Any other error
 *
 * @note parameters follow the memset convention (dest, src).
 *
 * @note generation and use of this function can be turned off by re-running
 * mib2c after adding the following line to the file
 * default-node-wtpMacAddr.m2d :
 *   @eval $m2c_node_skip_mapping = 1@
 *
 * @remark
 *  If the values for your data type don't exactly match the
 *  possible values defined by the mib, you should map them here.
 *  Otherwise, just do a direct copy.
 */
int
wtpMacAddr_dot11PortalUserInfoTable_map(char **mib_wtpMacAddr_val_ptr_ptr, size_t *mib_wtpMacAddr_val_ptr_len_ptr, char *raw_wtpMacAddr_val_ptr, size_t raw_wtpMacAddr_val_ptr_len, int allow_realloc)
{
    int converted_len;

    netsnmp_assert(NULL != raw_wtpMacAddr_val_ptr);
    netsnmp_assert((NULL != mib_wtpMacAddr_val_ptr_ptr) && (NULL != mib_wtpMacAddr_val_ptr_len_ptr));
    
    DEBUGMSGTL(("verbose:dot11PortalUserInfoTable:wtpMacAddr_map","called\n"));
    
    /*
     * TODO:241:r: |-> Implement wtpMacAddr non-integer mapping
     * it is hard to autogenerate code for mapping types that are not simple
     * integers, so here is an idea of what you might need to do. It will
     * probably need some tweaking to get right.
     */
    /*
     * if the length of the raw data doesn't directly correspond with
     * the length of the mib data, set converted_len to the
     * space required.
     */
    converted_len = raw_wtpMacAddr_val_ptr_len; /* assume equal */
    if((NULL == *mib_wtpMacAddr_val_ptr_ptr) || (*mib_wtpMacAddr_val_ptr_len_ptr < converted_len)) {
        if(! allow_realloc) {
            snmp_log(LOG_ERR,"not enough space for value mapping\n");
            return SNMP_ERR_GENERR;
        }
        *mib_wtpMacAddr_val_ptr_ptr = realloc( *mib_wtpMacAddr_val_ptr_ptr, converted_len * sizeof(**mib_wtpMacAddr_val_ptr_ptr));
        if(NULL == *mib_wtpMacAddr_val_ptr_ptr) {
            snmp_log(LOG_ERR,"could not allocate memory\n");
            return SNMP_ERR_GENERR;
        }
    }
    *mib_wtpMacAddr_val_ptr_len_ptr = converted_len;
    memcpy( *mib_wtpMacAddr_val_ptr_ptr, raw_wtpMacAddr_val_ptr, converted_len );

    return MFD_SUCCESS;
} /* wtpMacAddr_map */

/*---------------------------------------------------------------------
 * DOT11-WTP-MIB::dot11WtpStaEntry.wtpStaMacAddr
 * wtpStaMacAddr is subid 1 of dot11WtpStaEntry.
 * Its status is Current, and its access level is ReadOnly.
 * OID: .1.3.6.1.4.1.31656.6.1.1.8.1.1.1
 * Description:
The terminal MAC address.
 *
 * Attributes:
 *   accessible 1     isscalar 0     enums  0      hasdefval 0
 *   readable   1     iscolumn 1     ranges 1      hashint   1
 *   settable   0
 *   hint: 255a
 *
 * Ranges:  0 - 255;
 *
 * Its syntax is DisplayString (based on perltype OCTETSTR)
 * The net-snmp type is ASN_OCTET_STR. The C type decl is char (char)
 * This data type requires a length.  (Max 255)
 */
/**
 * map a value from its original native format to the MIB format.
 *
 * @retval MFD_SUCCESS         : success
 * @retval MFD_ERROR           : Any other error
 *
 * @note parameters follow the memset convention (dest, src).
 *
 * @note generation and use of this function can be turned off by re-running
 * mib2c after adding the following line to the file
 * default-node-wtpStaMacAddr.m2d :
 *   @eval $m2c_node_skip_mapping = 1@
 *
 * @remark
 *  If the values for your data type don't exactly match the
 *  possible values defined by the mib, you should map them here.
 *  Otherwise, just do a direct copy.
 */
int
wtpStaMacAddr_dot11PortalUserInfoTable_map(char **mib_wtpStaMacAddr_val_ptr_ptr, size_t *mib_wtpStaMacAddr_val_ptr_len_ptr, char *raw_wtpStaMacAddr_val_ptr, size_t raw_wtpStaMacAddr_val_ptr_len, int allow_realloc)
{
    int converted_len;

    netsnmp_assert(NULL != raw_wtpStaMacAddr_val_ptr);
    netsnmp_assert((NULL != mib_wtpStaMacAddr_val_ptr_ptr) && (NULL != mib_wtpStaMacAddr_val_ptr_len_ptr));
    
    DEBUGMSGTL(("verbose:dot11PortalUserInfoTable:wtpStaMacAddr_map","called\n"));
    
    /*
     * TODO:241:r: |-> Implement wtpStaMacAddr non-integer mapping
     * it is hard to autogenerate code for mapping types that are not simple
     * integers, so here is an idea of what you might need to do. It will
     * probably need some tweaking to get right.
     */
    /*
     * if the length of the raw data doesn't directly correspond with
     * the length of the mib data, set converted_len to the
     * space required.
     */
    converted_len = raw_wtpStaMacAddr_val_ptr_len; /* assume equal */
    if((NULL == *mib_wtpStaMacAddr_val_ptr_ptr) || (*mib_wtpStaMacAddr_val_ptr_len_ptr < converted_len)) {
        if(! allow_realloc) {
            snmp_log(LOG_ERR,"not enough space for value mapping\n");
            return SNMP_ERR_GENERR;
        }
        *mib_wtpStaMacAddr_val_ptr_ptr = realloc( *mib_wtpStaMacAddr_val_ptr_ptr, converted_len * sizeof(**mib_wtpStaMacAddr_val_ptr_ptr));
        if(NULL == *mib_wtpStaMacAddr_val_ptr_ptr) {
            snmp_log(LOG_ERR,"could not allocate memory\n");
            return SNMP_ERR_GENERR;
        }
    }
    *mib_wtpStaMacAddr_val_ptr_len_ptr = converted_len;
    memcpy( *mib_wtpStaMacAddr_val_ptr_ptr, raw_wtpStaMacAddr_val_ptr, converted_len );

    return MFD_SUCCESS;
} /* wtpStaMacAddr_map */


/**
 * set mib index(es)
 *
 * @param tbl_idx mib index structure
 *
 * @retval MFD_SUCCESS     : success.
 * @retval MFD_ERROR       : other error.
 *
 * @remark
 *  This convenience function is useful for setting all the MIB index
 *  components with a single function call. It is assume that the C values
 *  have already been mapped from their native/rawformat to the MIB format.
 */
int
dot11PortalUserInfoTable_indexes_set_tbl_idx(dot11PortalUserInfoTable_mib_index *tbl_idx, char *wtpMacAddr_val_ptr,  size_t wtpMacAddr_val_ptr_len, char *wtpStaMacAddr_val_ptr,  size_t wtpStaMacAddr_val_ptr_len)
{
    DEBUGMSGTL(("verbose:dot11PortalUserInfoTable:dot11PortalUserInfoTable_indexes_set_tbl_idx","called\n"));

    /* wtpMacAddr(1)/DisplayString/ASN_OCTET_STR/char(char)//L/A/w/e/R/d/H */
     tbl_idx->wtpMacAddr_len = sizeof(tbl_idx->wtpMacAddr);
    /*
     * make sure there is enough space for wtpMacAddr data
     */
    if ((NULL == tbl_idx->wtpMacAddr) ||
        (tbl_idx->wtpMacAddr_len < (wtpMacAddr_val_ptr_len * sizeof(tbl_idx->wtpMacAddr[0])))) {
        snmp_log(LOG_ERR,"not enough space for value\n");
        return MFD_ERROR;
    }
    tbl_idx->wtpMacAddr_len = wtpMacAddr_val_ptr_len * sizeof(tbl_idx->wtpMacAddr[0]);
    memcpy( tbl_idx->wtpMacAddr, wtpMacAddr_val_ptr, tbl_idx->wtpMacAddr_len );
    
    /* wtpStaMacAddr(1)/DisplayString/ASN_OCTET_STR/char(char)//L/A/w/e/R/d/H */
     tbl_idx->wtpStaMacAddr_len = sizeof(tbl_idx->wtpStaMacAddr);
    /*
     * make sure there is enough space for wtpStaMacAddr data
     */
    if ((NULL == tbl_idx->wtpStaMacAddr) ||
        (tbl_idx->wtpStaMacAddr_len < (wtpStaMacAddr_val_ptr_len * sizeof(tbl_idx->wtpStaMacAddr[0])))) {
        snmp_log(LOG_ERR,"not enough space for value\n");
        return MFD_ERROR;
    }
    tbl_idx->wtpStaMacAddr_len = wtpStaMacAddr_val_ptr_len * sizeof(tbl_idx->wtpStaMacAddr[0]);
    memcpy( tbl_idx->wtpStaMacAddr, wtpStaMacAddr_val_ptr, tbl_idx->wtpStaMacAddr_len );
    

    return MFD_SUCCESS;
} /* dot11PortalUserInfoTable_indexes_set_tbl_idx */

/**
 * @internal
 * set row context indexes
 *
 * @param reqreq_ctx the row context that needs updated indexes
 *
 * @retval MFD_SUCCESS     : success.
 * @retval MFD_ERROR       : other error.
 *
 * @remark
 *  This function sets the mib indexs, then updates the oid indexs
 *  from the mib index.
 */
int
dot11PortalUserInfoTable_indexes_set(dot11PortalUserInfoTable_rowreq_ctx *rowreq_ctx, char *wtpMacAddr_val_ptr,  size_t wtpMacAddr_val_ptr_len, char *wtpStaMacAddr_val_ptr,  size_t wtpStaMacAddr_val_ptr_len)
{
    DEBUGMSGTL(("verbose:dot11PortalUserInfoTable:dot11PortalUserInfoTable_indexes_set","called\n"));

    if(MFD_SUCCESS != dot11PortalUserInfoTable_indexes_set_tbl_idx(&rowreq_ctx->tbl_idx
                                   , wtpMacAddr_val_ptr, wtpMacAddr_val_ptr_len
                                   , wtpStaMacAddr_val_ptr, wtpStaMacAddr_val_ptr_len
           ))
        return MFD_ERROR;

    /*
     * convert mib index to oid index
     */
    rowreq_ctx->oid_idx.len = sizeof(rowreq_ctx->oid_tmp) / sizeof(oid);
    if(0 != dot11PortalUserInfoTable_index_to_oid(&rowreq_ctx->oid_idx,
                                    &rowreq_ctx->tbl_idx)) {
        return MFD_ERROR;
    }

    return MFD_SUCCESS;
} /* dot11PortalUserInfoTable_indexes_set */


/*---------------------------------------------------------------------
 * DOT11-WTP-MIB::dot11PortalUserInfoEntry.portalUserip
 * portalUserip is subid 1 of dot11PortalUserInfoEntry.
 * Its status is Current, and its access level is ReadOnly.
 * OID: .1.3.6.1.4.1.31656.6.1.1.17.2.1.1
 * Description:
The  portal user ip address).
 *
 * Attributes:
 *   accessible 1     isscalar 0     enums  0      hasdefval 0
 *   readable   1     iscolumn 1     ranges 0      hashint   0
 *   settable   0
 *
 *
 * Its syntax is IPADDR (based on perltype IPADDR)
 * The net-snmp type is ASN_IPADDRESS. The C type decl is u_long (u_long)
 */
/**
 * map a value from its original native format to the MIB format.
 *
 * @retval MFD_SUCCESS         : success
 * @retval MFD_ERROR           : Any other error
 *
 * @note parameters follow the memset convention (dest, src).
 *
 * @note generation and use of this function can be turned off by re-running
 * mib2c after adding the following line to the file
 * default-node-portalUserip.m2d :
 *   @eval $m2c_node_skip_mapping = 1@
 *
 * @remark
 *  If the values for your data type don't exactly match the
 *  possible values defined by the mib, you should map them here.
 *  Otherwise, just do a direct copy.
 */
int
portalUserip_map(u_long *mib_portalUserip_val_ptr, u_long raw_portalUserip_val)
{
    netsnmp_assert(NULL != mib_portalUserip_val_ptr);
    
    DEBUGMSGTL(("verbose:dot11PortalUserInfoTable:portalUserip_map","called\n"));
    
    /*
     * TODO:241:o: |-> Implement portalUserip mapping.
     * If the values for your data type don't exactly match the
     * possible values defined by the mib, you should map them here.
     */
    (*mib_portalUserip_val_ptr) = raw_portalUserip_val;

    return MFD_SUCCESS;
} /* portalUserip_map */

/**
 * Extract the current value of the portalUserip data.
 *
 * Set a value using the data context for the row.
 *
 * @param rowreq_ctx
 *        Pointer to the row request context.
 * @param portalUserip_val_ptr
 *        Pointer to storage for a u_long variable
 *
 * @retval MFD_SUCCESS         : success
 * @retval MFD_SKIP            : skip this node (no value for now)
 * @retval MFD_ERROR           : Any other error
 */
int
portalUserip_get( dot11PortalUserInfoTable_rowreq_ctx *rowreq_ctx, u_long * portalUserip_val_ptr )
{
   /** we should have a non-NULL pointer */
   netsnmp_assert( NULL != portalUserip_val_ptr );


    DEBUGMSGTL(("verbose:dot11PortalUserInfoTable:portalUserip_get","called\n"));

    netsnmp_assert(NULL != rowreq_ctx);

/*
 * TODO:231:o: |-> Extract the current value of the portalUserip data.
 * set (* portalUserip_val_ptr ) from rowreq_ctx->data
 */
    (* portalUserip_val_ptr ) = rowreq_ctx->data.portalUserip;

    return MFD_SUCCESS;
} /* portalUserip_get */

/*---------------------------------------------------------------------
 * DOT11-WTP-MIB::dot11PortalUserInfoEntry.portalUsername
 * portalUsername is subid 2 of dot11PortalUserInfoEntry.
 * Its status is Current, and its access level is ReadOnly.
 * OID: .1.3.6.1.4.1.31656.6.1.1.17.2.1.2
 * Description:
The  portal user name).
 *
 * Attributes:
 *   accessible 1     isscalar 0     enums  0      hasdefval 0
 *   readable   1     iscolumn 1     ranges 1      hashint   1
 *   settable   0
 *   hint: 255a
 *
 * Ranges:  0 - 255;
 *
 * Its syntax is DisplayString (based on perltype OCTETSTR)
 * The net-snmp type is ASN_OCTET_STR. The C type decl is char (char)
 * This data type requires a length.  (Max 255)
 */
/**
 * map a value from its original native format to the MIB format.
 *
 * @retval MFD_SUCCESS         : success
 * @retval MFD_ERROR           : Any other error
 *
 * @note parameters follow the memset convention (dest, src).
 *
 * @note generation and use of this function can be turned off by re-running
 * mib2c after adding the following line to the file
 * default-node-portalUsername.m2d :
 *   @eval $m2c_node_skip_mapping = 1@
 *
 * @remark
 *  If the values for your data type don't exactly match the
 *  possible values defined by the mib, you should map them here.
 *  Otherwise, just do a direct copy.
 */
int
portalUsername_map(char **mib_portalUsername_val_ptr_ptr, size_t *mib_portalUsername_val_ptr_len_ptr, char *raw_portalUsername_val_ptr, size_t raw_portalUsername_val_ptr_len, int allow_realloc)
{
    int converted_len;

    netsnmp_assert(NULL != raw_portalUsername_val_ptr);
    netsnmp_assert((NULL != mib_portalUsername_val_ptr_ptr) && (NULL != mib_portalUsername_val_ptr_len_ptr));
    
    DEBUGMSGTL(("verbose:dot11PortalUserInfoTable:portalUsername_map","called\n"));
    
    /*
     * TODO:241:r: |-> Implement portalUsername non-integer mapping
     * it is hard to autogenerate code for mapping types that are not simple
     * integers, so here is an idea of what you might need to do. It will
     * probably need some tweaking to get right.
     */
    /*
     * if the length of the raw data doesn't directly correspond with
     * the length of the mib data, set converted_len to the
     * space required.
     */
    converted_len = raw_portalUsername_val_ptr_len; /* assume equal */
    if((NULL == *mib_portalUsername_val_ptr_ptr) || (*mib_portalUsername_val_ptr_len_ptr < converted_len)) {
        if(! allow_realloc) {
            snmp_log(LOG_ERR,"not enough space for value mapping\n");
            return SNMP_ERR_GENERR;
        }
        *mib_portalUsername_val_ptr_ptr = realloc( *mib_portalUsername_val_ptr_ptr, converted_len * sizeof(**mib_portalUsername_val_ptr_ptr));
        if(NULL == *mib_portalUsername_val_ptr_ptr) {
            snmp_log(LOG_ERR,"could not allocate memory\n");
            return SNMP_ERR_GENERR;
        }
    }
    *mib_portalUsername_val_ptr_len_ptr = converted_len;
    memcpy( *mib_portalUsername_val_ptr_ptr, raw_portalUsername_val_ptr, converted_len );

    return MFD_SUCCESS;
} /* portalUsername_map */

/**
 * Extract the current value of the portalUsername data.
 *
 * Set a value using the data context for the row.
 *
 * @param rowreq_ctx
 *        Pointer to the row request context.
 * @param portalUsername_val_ptr_ptr
 *        Pointer to storage for a char variable
 * @param portalUsername_val_ptr_len_ptr
 *        Pointer to a size_t. On entry, it will contain the size (in bytes)
 *        pointed to by portalUsername.
 *        On exit, this value should contain the data size (in bytes).
 *
 * @retval MFD_SUCCESS         : success
 * @retval MFD_SKIP            : skip this node (no value for now)
 * @retval MFD_ERROR           : Any other error
*
 * @note If you need more than (*portalUsername_val_ptr_len_ptr) bytes of memory,
 *       allocate it using malloc() and update portalUsername_val_ptr_ptr.
 *       <b>DO NOT</b> free the previous pointer.
 *       The MFD helper will release the memory you allocate.
 *
 * @remark If you call this function yourself, you are responsible
 *         for checking if the pointer changed, and freeing any
 *         previously allocated memory. (Not necessary if you pass
 *         in a pointer to static memory, obviously.)
 */
int
portalUsername_get( dot11PortalUserInfoTable_rowreq_ctx *rowreq_ctx, char **portalUsername_val_ptr_ptr, size_t *portalUsername_val_ptr_len_ptr )
{
   /** we should have a non-NULL pointer and enough storage */
   netsnmp_assert( (NULL != portalUsername_val_ptr_ptr) && (NULL != *portalUsername_val_ptr_ptr));
   netsnmp_assert( NULL != portalUsername_val_ptr_len_ptr );


    DEBUGMSGTL(("verbose:dot11PortalUserInfoTable:portalUsername_get","called\n"));

    netsnmp_assert(NULL != rowreq_ctx);

/*
 * TODO:231:o: |-> Extract the current value of the portalUsername data.
 * set (* portalUsername_val_ptr_ptr ) and (* portalUsername_val_ptr_len_ptr ) from rowreq_ctx->data
 */
    /*
     * make sure there is enough space for portalUsername data
     */
    if ((NULL == (* portalUsername_val_ptr_ptr )) ||
        ((* portalUsername_val_ptr_len_ptr ) < (rowreq_ctx->data.portalUsername_len * sizeof((* portalUsername_val_ptr_ptr )[0])))) {
        /*
         * allocate space for portalUsername data
         */
        (* portalUsername_val_ptr_ptr ) = malloc(rowreq_ctx->data.portalUsername_len * sizeof((* portalUsername_val_ptr_ptr )[0]));
        if(NULL == (* portalUsername_val_ptr_ptr )) {
            snmp_log(LOG_ERR,"could not allocate memory\n");
            return MFD_ERROR;
        }
    }
    (* portalUsername_val_ptr_len_ptr ) = rowreq_ctx->data.portalUsername_len * sizeof((* portalUsername_val_ptr_ptr )[0]);
    memcpy( (* portalUsername_val_ptr_ptr ), rowreq_ctx->data.portalUsername, (* portalUsername_val_ptr_len_ptr ) );

    return MFD_SUCCESS;
} /* portalUsername_get */

/*---------------------------------------------------------------------
 * DOT11-WTP-MIB::dot11PortalUserInfoEntry.portalUserUptime
 * portalUserUptime is subid 3 of dot11PortalUserInfoEntry.
 * Its status is Current, and its access level is ReadOnly.
 * OID: .1.3.6.1.4.1.31656.6.1.1.17.2.1.3
 * Description:
The  portal user login time).
 *
 * Attributes:
 *   accessible 1     isscalar 0     enums  0      hasdefval 0
 *   readable   1     iscolumn 1     ranges 1      hashint   1
 *   settable   0
 *   hint: 255a
 *
 * Ranges:  0 - 255;
 *
 * Its syntax is DisplayString (based on perltype OCTETSTR)
 * The net-snmp type is ASN_OCTET_STR. The C type decl is char (char)
 * This data type requires a length.  (Max 255)
 */
/**
 * map a value from its original native format to the MIB format.
 *
 * @retval MFD_SUCCESS         : success
 * @retval MFD_ERROR           : Any other error
 *
 * @note parameters follow the memset convention (dest, src).
 *
 * @note generation and use of this function can be turned off by re-running
 * mib2c after adding the following line to the file
 * default-node-portalUserUptime.m2d :
 *   @eval $m2c_node_skip_mapping = 1@
 *
 * @remark
 *  If the values for your data type don't exactly match the
 *  possible values defined by the mib, you should map them here.
 *  Otherwise, just do a direct copy.
 */
int
portalUserUptime_map(char **mib_portalUserUptime_val_ptr_ptr, size_t *mib_portalUserUptime_val_ptr_len_ptr, char *raw_portalUserUptime_val_ptr, size_t raw_portalUserUptime_val_ptr_len, int allow_realloc)
{
    int converted_len;

    netsnmp_assert(NULL != raw_portalUserUptime_val_ptr);
    netsnmp_assert((NULL != mib_portalUserUptime_val_ptr_ptr) && (NULL != mib_portalUserUptime_val_ptr_len_ptr));
    
    DEBUGMSGTL(("verbose:dot11PortalUserInfoTable:portalUserUptime_map","called\n"));
    
    /*
     * TODO:241:r: |-> Implement portalUserUptime non-integer mapping
     * it is hard to autogenerate code for mapping types that are not simple
     * integers, so here is an idea of what you might need to do. It will
     * probably need some tweaking to get right.
     */
    /*
     * if the length of the raw data doesn't directly correspond with
     * the length of the mib data, set converted_len to the
     * space required.
     */
    converted_len = raw_portalUserUptime_val_ptr_len; /* assume equal */
    if((NULL == *mib_portalUserUptime_val_ptr_ptr) || (*mib_portalUserUptime_val_ptr_len_ptr < converted_len)) {
        if(! allow_realloc) {
            snmp_log(LOG_ERR,"not enough space for value mapping\n");
            return SNMP_ERR_GENERR;
        }
        *mib_portalUserUptime_val_ptr_ptr = realloc( *mib_portalUserUptime_val_ptr_ptr, converted_len * sizeof(**mib_portalUserUptime_val_ptr_ptr));
        if(NULL == *mib_portalUserUptime_val_ptr_ptr) {
            snmp_log(LOG_ERR,"could not allocate memory\n");
            return SNMP_ERR_GENERR;
        }
    }
    *mib_portalUserUptime_val_ptr_len_ptr = converted_len;
    memcpy( *mib_portalUserUptime_val_ptr_ptr, raw_portalUserUptime_val_ptr, converted_len );

    return MFD_SUCCESS;
} /* portalUserUptime_map */

/**
 * Extract the current value of the portalUserUptime data.
 *
 * Set a value using the data context for the row.
 *
 * @param rowreq_ctx
 *        Pointer to the row request context.
 * @param portalUserUptime_val_ptr_ptr
 *        Pointer to storage for a char variable
 * @param portalUserUptime_val_ptr_len_ptr
 *        Pointer to a size_t. On entry, it will contain the size (in bytes)
 *        pointed to by portalUserUptime.
 *        On exit, this value should contain the data size (in bytes).
 *
 * @retval MFD_SUCCESS         : success
 * @retval MFD_SKIP            : skip this node (no value for now)
 * @retval MFD_ERROR           : Any other error
*
 * @note If you need more than (*portalUserUptime_val_ptr_len_ptr) bytes of memory,
 *       allocate it using malloc() and update portalUserUptime_val_ptr_ptr.
 *       <b>DO NOT</b> free the previous pointer.
 *       The MFD helper will release the memory you allocate.
 *
 * @remark If you call this function yourself, you are responsible
 *         for checking if the pointer changed, and freeing any
 *         previously allocated memory. (Not necessary if you pass
 *         in a pointer to static memory, obviously.)
 */
int
portalUserUptime_get( dot11PortalUserInfoTable_rowreq_ctx *rowreq_ctx, char **portalUserUptime_val_ptr_ptr, size_t *portalUserUptime_val_ptr_len_ptr )
{
   /** we should have a non-NULL pointer and enough storage */
   netsnmp_assert( (NULL != portalUserUptime_val_ptr_ptr) && (NULL != *portalUserUptime_val_ptr_ptr));
   netsnmp_assert( NULL != portalUserUptime_val_ptr_len_ptr );


    DEBUGMSGTL(("verbose:dot11PortalUserInfoTable:portalUserUptime_get","called\n"));

    netsnmp_assert(NULL != rowreq_ctx);

/*
 * TODO:231:o: |-> Extract the current value of the portalUserUptime data.
 * set (* portalUserUptime_val_ptr_ptr ) and (* portalUserUptime_val_ptr_len_ptr ) from rowreq_ctx->data
 */
    /*
     * make sure there is enough space for portalUserUptime data
     */
    if ((NULL == (* portalUserUptime_val_ptr_ptr )) ||
        ((* portalUserUptime_val_ptr_len_ptr ) < (rowreq_ctx->data.portalUserUptime_len * sizeof((* portalUserUptime_val_ptr_ptr )[0])))) {
        /*
         * allocate space for portalUserUptime data
         */
        (* portalUserUptime_val_ptr_ptr ) = malloc(rowreq_ctx->data.portalUserUptime_len * sizeof((* portalUserUptime_val_ptr_ptr )[0]));
        if(NULL == (* portalUserUptime_val_ptr_ptr )) {
            snmp_log(LOG_ERR,"could not allocate memory\n");
            return MFD_ERROR;
        }
    }
    (* portalUserUptime_val_ptr_len_ptr ) = rowreq_ctx->data.portalUserUptime_len * sizeof((* portalUserUptime_val_ptr_ptr )[0]);
    memcpy( (* portalUserUptime_val_ptr_ptr ), rowreq_ctx->data.portalUserUptime, (* portalUserUptime_val_ptr_len_ptr ) );

    return MFD_SUCCESS;
} /* portalUserUptime_get */

/*---------------------------------------------------------------------
 * DOT11-WTP-MIB::dot11PortalUserInfoEntry.portalUseronline
 * portalUseronline is subid 4 of dot11PortalUserInfoEntry.
 * Its status is Current, and its access level is ReadOnly.
 * OID: .1.3.6.1.4.1.31656.6.1.1.17.2.1.4
 * Description:
The  portal user online time).
 *
 * Attributes:
 *   accessible 1     isscalar 0     enums  0      hasdefval 0
 *   readable   1     iscolumn 1     ranges 0      hashint   0
 *   settable   0
 *
 *
 * Its syntax is TICKS (based on perltype TICKS)
 * The net-snmp type is ASN_TIMETICKS. The C type decl is u_long (u_long)
 */
/**
 * map a value from its original native format to the MIB format.
 *
 * @retval MFD_SUCCESS         : success
 * @retval MFD_ERROR           : Any other error
 *
 * @note parameters follow the memset convention (dest, src).
 *
 * @note generation and use of this function can be turned off by re-running
 * mib2c after adding the following line to the file
 * default-node-portalUseronline.m2d :
 *   @eval $m2c_node_skip_mapping = 1@
 *
 * @remark
 *  If the values for your data type don't exactly match the
 *  possible values defined by the mib, you should map them here.
 *  Otherwise, just do a direct copy.
 */
int
portalUseronline_map(u_long *mib_portalUseronline_val_ptr, u_long raw_portalUseronline_val)
{
    netsnmp_assert(NULL != mib_portalUseronline_val_ptr);
    
    DEBUGMSGTL(("verbose:dot11PortalUserInfoTable:portalUseronline_map","called\n"));
    
    /*
     * TODO:241:o: |-> Implement portalUseronline mapping.
     * If the values for your data type don't exactly match the
     * possible values defined by the mib, you should map them here.
     */
    (*mib_portalUseronline_val_ptr) = raw_portalUseronline_val;

    return MFD_SUCCESS;
} /* portalUseronline_map */

/**
 * Extract the current value of the portalUseronline data.
 *
 * Set a value using the data context for the row.
 *
 * @param rowreq_ctx
 *        Pointer to the row request context.
 * @param portalUseronline_val_ptr
 *        Pointer to storage for a u_long variable
 *
 * @retval MFD_SUCCESS         : success
 * @retval MFD_SKIP            : skip this node (no value for now)
 * @retval MFD_ERROR           : Any other error
 */
int
portalUseronline_get( dot11PortalUserInfoTable_rowreq_ctx *rowreq_ctx, u_long * portalUseronline_val_ptr )
{
   /** we should have a non-NULL pointer */
   netsnmp_assert( NULL != portalUseronline_val_ptr );


    DEBUGMSGTL(("verbose:dot11PortalUserInfoTable:portalUseronline_get","called\n"));

    netsnmp_assert(NULL != rowreq_ctx);

/*
 * TODO:231:o: |-> Extract the current value of the portalUseronline data.
 * set (* portalUseronline_val_ptr ) from rowreq_ctx->data
 */
    (* portalUseronline_val_ptr ) = rowreq_ctx->data.portalUseronline;

    return MFD_SUCCESS;
} /* portalUseronline_get */

int
userAuthType_get( dot11PortalUserInfoTable_rowreq_ctx *rowreq_ctx, u_long * userAuthType_val_ptr )
{
   /** we should have a non-NULL pointer */
   netsnmp_assert( NULL != userAuthType_val_ptr );


    DEBUGMSGTL(("verbose:dot11PortalUserInfoTable:userAuthType_get","called\n"));

    netsnmp_assert(NULL != rowreq_ctx);

/*
 * TODO:231:o: |-> Extract the current value of the userAuthType data.
 * set (* userAuthType_val_ptr ) from rowreq_ctx->data
 */
    (* userAuthType_val_ptr ) = rowreq_ctx->data.userAuthType;

    return MFD_SUCCESS;
} 


/** @} */
