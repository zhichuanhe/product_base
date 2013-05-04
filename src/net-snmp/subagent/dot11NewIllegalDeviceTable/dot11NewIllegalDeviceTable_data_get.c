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
#include "dot11NewIllegalDeviceTable.h"


/** @defgroup data_get data_get: Routines to get data
 *
 * TODO:230:M: Implement dot11NewIllegalDeviceTable get routines.
 * TODO:240:M: Implement dot11NewIllegalDeviceTable mapping routines (if any).
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
 *** Table dot11NewIllegalDeviceTable
 ***
 **********************************************************************
 **********************************************************************/
/*
 * dot11NewIllegalDeviceTable is subid 3 of dot11WhiteBlackList.
 * Its status is Current.
 * OID: .1.3.6.1.4.1.31656.6.1.2.8.3, length: 12
*/

/* ---------------------------------------------------------------------
 * TODO:200:r: Implement dot11NewIllegalDeviceTable data context functions.
 */

/*---------------------------------------------------------------------
 * DOT11-AC-MIB::dot11NewIllegalDeviceEntry.NewblackListDeviceMacID
 * NewblackListDeviceMacID is subid 1 of dot11NewIllegalDeviceEntry.
 * Its status is Current, and its access level is ReadOnly.
 * OID: .1.3.6.1.4.1.31656.6.1.2.8.3.1.1
 * Description:
Black list MAC ID.
 *
 * Attributes:
 *   accessible 1     isscalar 0     enums  0      hasdefval 0
 *   readable   1     iscolumn 1     ranges 0      hashint   0
 *   settable   0
 *
 *
 * Its syntax is INTEGER (based on perltype INTEGER)
 * The net-snmp type is ASN_INTEGER. The C type decl is long (long)
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
 * default-node-NewblackListDeviceMacID.m2d :
 *   @eval $m2c_node_skip_mapping = 1@
 *
 * @remark
 *  If the values for your data type don't exactly match the
 *  possible values defined by the mib, you should map them here.
 *  Otherwise, just do a direct copy.
 */
int
NewblackListDeviceMacID_map(long *mib_NewblackListDeviceMacID_val_ptr, long raw_NewblackListDeviceMacID_val)
{
    netsnmp_assert(NULL != mib_NewblackListDeviceMacID_val_ptr);
    
    DEBUGMSGTL(("verbose:dot11NewIllegalDeviceTable:NewblackListDeviceMacID_map","called\n"));
    
    /*
     * TODO:241:o: |-> Implement NewblackListDeviceMacID mapping.
     * If the values for your data type don't exactly match the
     * possible values defined by the mib, you should map them here.
     */
    (*mib_NewblackListDeviceMacID_val_ptr) = raw_NewblackListDeviceMacID_val;

    return MFD_SUCCESS;
} /* NewblackListDeviceMacID_map */


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
dot11NewIllegalDeviceTable_indexes_set_tbl_idx(dot11NewIllegalDeviceTable_mib_index *tbl_idx, long NewblackListDeviceMacID_val)
{
    DEBUGMSGTL(("verbose:dot11NewIllegalDeviceTable:dot11NewIllegalDeviceTable_indexes_set_tbl_idx","called\n"));

    /* NewblackListDeviceMacID(1)/INTEGER/ASN_INTEGER/long(long)//l/A/w/e/r/d/h */
    tbl_idx->NewblackListDeviceMacID = NewblackListDeviceMacID_val;
    

    return MFD_SUCCESS;
} /* dot11NewIllegalDeviceTable_indexes_set_tbl_idx */

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
dot11NewIllegalDeviceTable_indexes_set(dot11NewIllegalDeviceTable_rowreq_ctx *rowreq_ctx, long NewblackListDeviceMacID_val)
{
    DEBUGMSGTL(("verbose:dot11NewIllegalDeviceTable:dot11NewIllegalDeviceTable_indexes_set","called\n"));

    if(MFD_SUCCESS != dot11NewIllegalDeviceTable_indexes_set_tbl_idx(&rowreq_ctx->tbl_idx
                                   , NewblackListDeviceMacID_val
           ))
        return MFD_ERROR;

    /*
     * convert mib index to oid index
     */
    rowreq_ctx->oid_idx.len = sizeof(rowreq_ctx->oid_tmp) / sizeof(oid);
    if(0 != dot11NewIllegalDeviceTable_index_to_oid(&rowreq_ctx->oid_idx,
                                    &rowreq_ctx->tbl_idx)) {
        return MFD_ERROR;
    }

    return MFD_SUCCESS;
} /* dot11NewIllegalDeviceTable_indexes_set */


/*---------------------------------------------------------------------
 * DOT11-AC-MIB::dot11NewIllegalDeviceEntry.NewblackListDeviceMAC
 * NewblackListDeviceMAC is subid 2 of dot11NewIllegalDeviceEntry.
 * Its status is Current, and its access level is ReadOnly.
 * OID: .1.3.6.1.4.1.31656.6.1.2.8.3.1.2
 * Description:
Black list MAC address.
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
 * default-node-NewblackListDeviceMAC.m2d :
 *   @eval $m2c_node_skip_mapping = 1@
 *
 * @remark
 *  If the values for your data type don't exactly match the
 *  possible values defined by the mib, you should map them here.
 *  Otherwise, just do a direct copy.
 */
int
NewblackListDeviceMAC_map(char **mib_NewblackListDeviceMAC_val_ptr_ptr, size_t *mib_NewblackListDeviceMAC_val_ptr_len_ptr, char *raw_NewblackListDeviceMAC_val_ptr, size_t raw_NewblackListDeviceMAC_val_ptr_len, int allow_realloc)
{
    int converted_len;

    netsnmp_assert(NULL != raw_NewblackListDeviceMAC_val_ptr);
    netsnmp_assert((NULL != mib_NewblackListDeviceMAC_val_ptr_ptr) && (NULL != mib_NewblackListDeviceMAC_val_ptr_len_ptr));
    
    DEBUGMSGTL(("verbose:dot11NewIllegalDeviceTable:NewblackListDeviceMAC_map","called\n"));
    
    /*
     * TODO:241:r: |-> Implement NewblackListDeviceMAC non-integer mapping
     * it is hard to autogenerate code for mapping types that are not simple
     * integers, so here is an idea of what you might need to do. It will
     * probably need some tweaking to get right.
     */
    /*
     * if the length of the raw data doesn't directly correspond with
     * the length of the mib data, set converted_len to the
     * space required.
     */
    converted_len = raw_NewblackListDeviceMAC_val_ptr_len; /* assume equal */
    if((NULL == *mib_NewblackListDeviceMAC_val_ptr_ptr) || (*mib_NewblackListDeviceMAC_val_ptr_len_ptr < converted_len)) {
        if(! allow_realloc) {
            snmp_log(LOG_ERR,"not enough space for value mapping\n");
            return SNMP_ERR_GENERR;
        }
        *mib_NewblackListDeviceMAC_val_ptr_ptr = realloc( *mib_NewblackListDeviceMAC_val_ptr_ptr, converted_len * sizeof(**mib_NewblackListDeviceMAC_val_ptr_ptr));
        if(NULL == *mib_NewblackListDeviceMAC_val_ptr_ptr) {
            snmp_log(LOG_ERR,"could not allocate memory\n");
            return SNMP_ERR_GENERR;
        }
    }
    *mib_NewblackListDeviceMAC_val_ptr_len_ptr = converted_len;
    memcpy( *mib_NewblackListDeviceMAC_val_ptr_ptr, raw_NewblackListDeviceMAC_val_ptr, converted_len );

    return MFD_SUCCESS;
} /* NewblackListDeviceMAC_map */

/**
 * Extract the current value of the NewblackListDeviceMAC data.
 *
 * Set a value using the data context for the row.
 *
 * @param rowreq_ctx
 *        Pointer to the row request context.
 * @param NewblackListDeviceMAC_val_ptr_ptr
 *        Pointer to storage for a char variable
 * @param NewblackListDeviceMAC_val_ptr_len_ptr
 *        Pointer to a size_t. On entry, it will contain the size (in bytes)
 *        pointed to by NewblackListDeviceMAC.
 *        On exit, this value should contain the data size (in bytes).
 *
 * @retval MFD_SUCCESS         : success
 * @retval MFD_SKIP            : skip this node (no value for now)
 * @retval MFD_ERROR           : Any other error
*
 * @note If you need more than (*NewblackListDeviceMAC_val_ptr_len_ptr) bytes of memory,
 *       allocate it using malloc() and update NewblackListDeviceMAC_val_ptr_ptr.
 *       <b>DO NOT</b> free the previous pointer.
 *       The MFD helper will release the memory you allocate.
 *
 * @remark If you call this function yourself, you are responsible
 *         for checking if the pointer changed, and freeing any
 *         previously allocated memory. (Not necessary if you pass
 *         in a pointer to static memory, obviously.)
 */
int
NewblackListDeviceMAC_get( dot11NewIllegalDeviceTable_rowreq_ctx *rowreq_ctx, char **NewblackListDeviceMAC_val_ptr_ptr, size_t *NewblackListDeviceMAC_val_ptr_len_ptr )
{
   /** we should have a non-NULL pointer and enough storage */
   netsnmp_assert( (NULL != NewblackListDeviceMAC_val_ptr_ptr) && (NULL != *NewblackListDeviceMAC_val_ptr_ptr));
   netsnmp_assert( NULL != NewblackListDeviceMAC_val_ptr_len_ptr );


    DEBUGMSGTL(("verbose:dot11NewIllegalDeviceTable:NewblackListDeviceMAC_get","called\n"));

    netsnmp_assert(NULL != rowreq_ctx);

/*
 * TODO:231:o: |-> Extract the current value of the NewblackListDeviceMAC data.
 * set (* NewblackListDeviceMAC_val_ptr_ptr ) and (* NewblackListDeviceMAC_val_ptr_len_ptr ) from rowreq_ctx->data
 */
    /*
     * make sure there is enough space for NewblackListDeviceMAC data
     */
    if ((NULL == (* NewblackListDeviceMAC_val_ptr_ptr )) ||
        ((* NewblackListDeviceMAC_val_ptr_len_ptr ) < (rowreq_ctx->data.NewblackListDeviceMAC_len * sizeof((* NewblackListDeviceMAC_val_ptr_ptr )[0])))) {
        /*
         * allocate space for NewblackListDeviceMAC data
         */
        (* NewblackListDeviceMAC_val_ptr_ptr ) = malloc(rowreq_ctx->data.NewblackListDeviceMAC_len * sizeof((* NewblackListDeviceMAC_val_ptr_ptr )[0]));
        if(NULL == (* NewblackListDeviceMAC_val_ptr_ptr )) {
            snmp_log(LOG_ERR,"could not allocate memory\n");
            return MFD_ERROR;
        }
    }
    (* NewblackListDeviceMAC_val_ptr_len_ptr ) = rowreq_ctx->data.NewblackListDeviceMAC_len * sizeof((* NewblackListDeviceMAC_val_ptr_ptr )[0]);
    memcpy( (* NewblackListDeviceMAC_val_ptr_ptr ), rowreq_ctx->data.NewblackListDeviceMAC, (* NewblackListDeviceMAC_val_ptr_len_ptr ) );

    return MFD_SUCCESS;
} /* NewblackListDeviceMAC_get */

/*---------------------------------------------------------------------
 * DOT11-AC-MIB::dot11NewIllegalDeviceEntry.NewblackListAddReason
 * NewblackListAddReason is subid 3 of dot11NewIllegalDeviceEntry.
 * Its status is Current, and its access level is ReadOnly.
 * OID: .1.3.6.1.4.1.31656.6.1.2.8.3.1.3
 * Description:
 The reason why add to black list.
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
 * default-node-NewblackListAddReason.m2d :
 *   @eval $m2c_node_skip_mapping = 1@
 *
 * @remark
 *  If the values for your data type don't exactly match the
 *  possible values defined by the mib, you should map them here.
 *  Otherwise, just do a direct copy.
 */
int
NewblackListAddReason_map(char **mib_NewblackListAddReason_val_ptr_ptr, size_t *mib_NewblackListAddReason_val_ptr_len_ptr, char *raw_NewblackListAddReason_val_ptr, size_t raw_NewblackListAddReason_val_ptr_len, int allow_realloc)
{
    int converted_len;

    netsnmp_assert(NULL != raw_NewblackListAddReason_val_ptr);
    netsnmp_assert((NULL != mib_NewblackListAddReason_val_ptr_ptr) && (NULL != mib_NewblackListAddReason_val_ptr_len_ptr));
    
    DEBUGMSGTL(("verbose:dot11NewIllegalDeviceTable:NewblackListAddReason_map","called\n"));
    
    /*
     * TODO:241:r: |-> Implement NewblackListAddReason non-integer mapping
     * it is hard to autogenerate code for mapping types that are not simple
     * integers, so here is an idea of what you might need to do. It will
     * probably need some tweaking to get right.
     */
    /*
     * if the length of the raw data doesn't directly correspond with
     * the length of the mib data, set converted_len to the
     * space required.
     */
    converted_len = raw_NewblackListAddReason_val_ptr_len; /* assume equal */
    if((NULL == *mib_NewblackListAddReason_val_ptr_ptr) || (*mib_NewblackListAddReason_val_ptr_len_ptr < converted_len)) {
        if(! allow_realloc) {
            snmp_log(LOG_ERR,"not enough space for value mapping\n");
            return SNMP_ERR_GENERR;
        }
        *mib_NewblackListAddReason_val_ptr_ptr = realloc( *mib_NewblackListAddReason_val_ptr_ptr, converted_len * sizeof(**mib_NewblackListAddReason_val_ptr_ptr));
        if(NULL == *mib_NewblackListAddReason_val_ptr_ptr) {
            snmp_log(LOG_ERR,"could not allocate memory\n");
            return SNMP_ERR_GENERR;
        }
    }
    *mib_NewblackListAddReason_val_ptr_len_ptr = converted_len;
    memcpy( *mib_NewblackListAddReason_val_ptr_ptr, raw_NewblackListAddReason_val_ptr, converted_len );

    return MFD_SUCCESS;
} /* NewblackListAddReason_map */

/**
 * Extract the current value of the NewblackListAddReason data.
 *
 * Set a value using the data context for the row.
 *
 * @param rowreq_ctx
 *        Pointer to the row request context.
 * @param NewblackListAddReason_val_ptr_ptr
 *        Pointer to storage for a char variable
 * @param NewblackListAddReason_val_ptr_len_ptr
 *        Pointer to a size_t. On entry, it will contain the size (in bytes)
 *        pointed to by NewblackListAddReason.
 *        On exit, this value should contain the data size (in bytes).
 *
 * @retval MFD_SUCCESS         : success
 * @retval MFD_SKIP            : skip this node (no value for now)
 * @retval MFD_ERROR           : Any other error
*
 * @note If you need more than (*NewblackListAddReason_val_ptr_len_ptr) bytes of memory,
 *       allocate it using malloc() and update NewblackListAddReason_val_ptr_ptr.
 *       <b>DO NOT</b> free the previous pointer.
 *       The MFD helper will release the memory you allocate.
 *
 * @remark If you call this function yourself, you are responsible
 *         for checking if the pointer changed, and freeing any
 *         previously allocated memory. (Not necessary if you pass
 *         in a pointer to static memory, obviously.)
 */
int
NewblackListAddReason_get( dot11NewIllegalDeviceTable_rowreq_ctx *rowreq_ctx, char **NewblackListAddReason_val_ptr_ptr, size_t *NewblackListAddReason_val_ptr_len_ptr )
{
   /** we should have a non-NULL pointer and enough storage */
   netsnmp_assert( (NULL != NewblackListAddReason_val_ptr_ptr) && (NULL != *NewblackListAddReason_val_ptr_ptr));
   netsnmp_assert( NULL != NewblackListAddReason_val_ptr_len_ptr );


    DEBUGMSGTL(("verbose:dot11NewIllegalDeviceTable:NewblackListAddReason_get","called\n"));

    netsnmp_assert(NULL != rowreq_ctx);

/*
 * TODO:231:o: |-> Extract the current value of the NewblackListAddReason data.
 * set (* NewblackListAddReason_val_ptr_ptr ) and (* NewblackListAddReason_val_ptr_len_ptr ) from rowreq_ctx->data
 */
    /*
     * make sure there is enough space for NewblackListAddReason data
     */
    if ((NULL == (* NewblackListAddReason_val_ptr_ptr )) ||
        ((* NewblackListAddReason_val_ptr_len_ptr ) < (rowreq_ctx->data.NewblackListAddReason_len * sizeof((* NewblackListAddReason_val_ptr_ptr )[0])))) {
        /*
         * allocate space for NewblackListAddReason data
         */
        (* NewblackListAddReason_val_ptr_ptr ) = malloc(rowreq_ctx->data.NewblackListAddReason_len * sizeof((* NewblackListAddReason_val_ptr_ptr )[0]));
        if(NULL == (* NewblackListAddReason_val_ptr_ptr )) {
            snmp_log(LOG_ERR,"could not allocate memory\n");
            return MFD_ERROR;
        }
    }
    (* NewblackListAddReason_val_ptr_len_ptr ) = rowreq_ctx->data.NewblackListAddReason_len * sizeof((* NewblackListAddReason_val_ptr_ptr )[0]);
    memcpy( (* NewblackListAddReason_val_ptr_ptr ), rowreq_ctx->data.NewblackListAddReason, (* NewblackListAddReason_val_ptr_len_ptr ) );

    return MFD_SUCCESS;
} /* NewblackListAddReason_get */

/*---------------------------------------------------------------------
 * DOT11-AC-MIB::dot11NewIllegalDeviceEntry.NewblackListDuration
 * NewblackListDuration is subid 4 of dot11NewIllegalDeviceEntry.
 * Its status is Current, and its access level is ReadOnly.
 * OID: .1.3.6.1.4.1.31656.6.1.2.8.3.1.4
 * Description:
 How long it keeps in black list.
 *
 * Attributes:
 *   accessible 1     isscalar 0     enums  0      hasdefval 0
 *   readable   1     iscolumn 1     ranges 0      hashint   0
 *   settable   0
 *
 *
 * Its syntax is UNSIGNED32 (based on perltype UNSIGNED32)
 * The net-snmp type is ASN_UNSIGNED. The C type decl is u_long (u_long)
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
 * default-node-NewblackListDuration.m2d :
 *   @eval $m2c_node_skip_mapping = 1@
 *
 * @remark
 *  If the values for your data type don't exactly match the
 *  possible values defined by the mib, you should map them here.
 *  Otherwise, just do a direct copy.
 */
int
NewblackListDuration_map(u_long *mib_NewblackListDuration_val_ptr, u_long raw_NewblackListDuration_val)
{
    netsnmp_assert(NULL != mib_NewblackListDuration_val_ptr);
    
    DEBUGMSGTL(("verbose:dot11NewIllegalDeviceTable:NewblackListDuration_map","called\n"));
    
    /*
     * TODO:241:o: |-> Implement NewblackListDuration mapping.
     * If the values for your data type don't exactly match the
     * possible values defined by the mib, you should map them here.
     */
    (*mib_NewblackListDuration_val_ptr) = raw_NewblackListDuration_val;

    return MFD_SUCCESS;
} /* NewblackListDuration_map */

/**
 * Extract the current value of the NewblackListDuration data.
 *
 * Set a value using the data context for the row.
 *
 * @param rowreq_ctx
 *        Pointer to the row request context.
 * @param NewblackListDuration_val_ptr
 *        Pointer to storage for a u_long variable
 *
 * @retval MFD_SUCCESS         : success
 * @retval MFD_SKIP            : skip this node (no value for now)
 * @retval MFD_ERROR           : Any other error
 */
int
NewblackListDuration_get( dot11NewIllegalDeviceTable_rowreq_ctx *rowreq_ctx, u_long * NewblackListDuration_val_ptr )
{
   /** we should have a non-NULL pointer */
   netsnmp_assert( NULL != NewblackListDuration_val_ptr );


    DEBUGMSGTL(("verbose:dot11NewIllegalDeviceTable:NewblackListDuration_get","called\n"));

    netsnmp_assert(NULL != rowreq_ctx);

/*
 * TODO:231:o: |-> Extract the current value of the NewblackListDuration data.
 * set (* NewblackListDuration_val_ptr ) from rowreq_ctx->data
 */
    (* NewblackListDuration_val_ptr ) = rowreq_ctx->data.NewblackListDuration;

    return MFD_SUCCESS;
} /* NewblackListDuration_get */

/*---------------------------------------------------------------------
 * DOT11-AC-MIB::dot11NewIllegalDeviceEntry.NewblackListAddType
 * NewblackListAddType is subid 5 of dot11NewIllegalDeviceEntry.
 * Its status is Current, and its access level is ReadOnly.
 * OID: .1.3.6.1.4.1.31656.6.1.2.8.3.1.5
 * Description:
 Black list add type.
 *
 * Attributes:
 *   accessible 1     isscalar 0     enums  1      hasdefval 0
 *   readable   1     iscolumn 1     ranges 0      hashint   0
 *   settable   0
 *
 * Enum range: 2/8. Values:  static(1), dynamic(2)
 *
 * Its syntax is INTEGER (based on perltype INTEGER)
 * The net-snmp type is ASN_INTEGER. The C type decl is long (u_long)
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
 * default-node-NewblackListAddType.m2d :
 *   @eval $m2c_node_skip_mapping = 1@
 *
 * @remark
 *  If the values for your data type don't exactly match the
 *  possible values defined by the mib, you should map them here.
 *  Otherwise, just do a direct copy.
 */
int
NewblackListAddType_map(u_long *mib_NewblackListAddType_val_ptr, u_long raw_NewblackListAddType_val)
{
    netsnmp_assert(NULL != mib_NewblackListAddType_val_ptr);
    
    DEBUGMSGTL(("verbose:dot11NewIllegalDeviceTable:NewblackListAddType_map","called\n"));
    
    /*
     * TODO:241:o: |-> Implement NewblackListAddType enum mapping.
     * uses INTERNAL_* macros defined in the header files
     */
    switch(raw_NewblackListAddType_val) {
        case INTERNAL_NEWBLACKLISTADDTYPE_STATIC:
             *mib_NewblackListAddType_val_ptr = NEWBLACKLISTADDTYPE_STATIC;
             break;

        case INTERNAL_NEWBLACKLISTADDTYPE_DYNAMIC:
             *mib_NewblackListAddType_val_ptr = NEWBLACKLISTADDTYPE_DYNAMIC;
             break;

             default:
                 snmp_log(LOG_ERR, "couldn't map value %ld for NewblackListAddType\n", raw_NewblackListAddType_val );
                 return MFD_ERROR;
    }

    return MFD_SUCCESS;
} /* NewblackListAddType_map */

/**
 * Extract the current value of the NewblackListAddType data.
 *
 * Set a value using the data context for the row.
 *
 * @param rowreq_ctx
 *        Pointer to the row request context.
 * @param NewblackListAddType_val_ptr
 *        Pointer to storage for a long variable
 *
 * @retval MFD_SUCCESS         : success
 * @retval MFD_SKIP            : skip this node (no value for now)
 * @retval MFD_ERROR           : Any other error
 */
int
NewblackListAddType_get( dot11NewIllegalDeviceTable_rowreq_ctx *rowreq_ctx, u_long * NewblackListAddType_val_ptr )
{
   /** we should have a non-NULL pointer */
   netsnmp_assert( NULL != NewblackListAddType_val_ptr );


    DEBUGMSGTL(("verbose:dot11NewIllegalDeviceTable:NewblackListAddType_get","called\n"));

    netsnmp_assert(NULL != rowreq_ctx);

/*
 * TODO:231:o: |-> Extract the current value of the NewblackListAddType data.
 * set (* NewblackListAddType_val_ptr ) from rowreq_ctx->data
 */
    (* NewblackListAddType_val_ptr ) = rowreq_ctx->data.NewblackListAddType;

    return MFD_SUCCESS;
} /* NewblackListAddType_get */



/** @} */
