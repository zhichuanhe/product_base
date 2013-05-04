/*
 * Note: this file originally auto-generated by mib2c using
 *       version : 1.18.2.1 $ of : mfd-data-get.m2c,v $
 *
 * $Id:$
 *
 * @file dot11NasPortTable_data_get.h
 *
 * @addtogroup get
 *
 * Prototypes for get functions
 *
 * @{
 */
#ifndef DOT11NASPORTTABLE_DATA_GET_H
#define DOT11NASPORTTABLE_DATA_GET_H

#ifdef __cplusplus
extern "C" {
#endif

/* *********************************************************************
 * GET function declarations
 */

/* *********************************************************************
 * GET Table declarations
 */
/**********************************************************************
 **********************************************************************
 ***
 *** Table dot11NasPortTable
 ***
 **********************************************************************
 **********************************************************************/
/*
 * dot11NasPortTable is subid 2 of dot11ConfigNasGroup.
 * Its status is Current.
 * OID: .1.3.6.1.4.1.31656.6.1.2.21.2, length: 12
*/
    /*
     * indexes
     */
    int wtpMacAddr_map(char **mib_wtpMacAddr_val_ptr_ptr, size_t *mib_wtpMacAddr_val_ptr_len_ptr, char *raw_wtpMacAddr_val_ptr, size_t raw_wtpMacAddr_val_ptr_len, int allow_realloc);
    int wlanID_map(long *mib_wlanID_val_ptr, long raw_wlanID_val);

    int NasPortID_map(char **mib_NasPortID_val_ptr_ptr, size_t *mib_NasPortID_val_ptr_len_ptr, char *raw_NasPortID_val_ptr, size_t raw_NasPortID_val_ptr_len, int allow_realloc);
    int NasPortID_get( dot11NasPortTable_rowreq_ctx *rowreq_ctx, char **NasPortID_val_ptr_ptr, size_t *NasPortID_val_ptr_len_ptr );


int dot11NasPortTable_indexes_set_tbl_idx(dot11NasPortTable_mib_index *tbl_idx, char *wtpMacAddr_val_ptr,  size_t wtpMacAddr_val_ptr_len, long wlanID_val);
int dot11NasPortTable_indexes_set(dot11NasPortTable_rowreq_ctx *rowreq_ctx, char *wtpMacAddr_val_ptr,  size_t wtpMacAddr_val_ptr_len, long wlanID_val);




#ifdef __cplusplus
}
#endif

#endif /* DOT11NASPORTTABLE_DATA_GET_H */
/** @} */
