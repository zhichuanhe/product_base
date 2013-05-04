/*
 * Note: this file originally auto-generated by mib2c using
 *  : generic-table-oids.m2c 12855 2005-09-27 15:56:08Z rstory $
 *
 * $Id:$
 */
#ifndef DOT11CONFIGPORTALSERVERGROUPTABLE_OIDS_H
#define DOT11CONFIGPORTALSERVERGROUPTABLE_OIDS_H

#ifdef __cplusplus
extern "C" {
#endif


/* column number definitions for table dot11ConfigPortalServerGroupTable */
//#define DOT11CONFIGPORTALSERVERGROUPTABLE_OID              1,3,6,1,4,1,31656,6,1,2,23,1

#define COLUMN_PORTALKEY         1
#define COLUMN_PORTALKEY_FLAG    (0x1 << 0)
    
#define COLUMN_PORTALURL         2
#define COLUMN_PORTALURL_FLAG    (0x1 << 1)
    
#define COLUMN_PORTALPORT         3
#define COLUMN_PORTALPORT_FLAG    (0x1 << 2)
    
#define COLUMN_PORTALINDEX         4
#define COLUMN_PORTALINDEX_FLAG    (0x1 << 3)
    
#define COLUMN_PORTALDOMAIN         5
#define COLUMN_PORTALDOMAIN_FLAG    (0x1 << 4)
    
#define COLUMN_PORTALROWSTATUS         6
#define COLUMN_PORTALROWSTATUS_FLAG    (0x1 << 5)
    
#define COLUMN_PORTALKEYTYPE         7
#define COLUMN_PORTALKEYTYPE_FLAG    (0x1 << 6)
    

#define DOT11CONFIGPORTALSERVERGROUPTABLE_MIN_COL   COLUMN_PORTALKEY
#define DOT11CONFIGPORTALSERVERGROUPTABLE_MAX_COL   COLUMN_PORTALKEYTYPE
    

    /*
     * TODO:405:r: Review DOT11CONFIGPORTALSERVERGROUPTABLE_SETTABLE_COLS macro.
     * OR together all the writable cols.
     */
#define DOT11CONFIGPORTALSERVERGROUPTABLE_SETTABLE_COLS (COLUMN_PORTALINDEX_FLAG | COLUMN_PORTALKEYTYPE_FLAG | COLUMN_PORTALKEY_FLAG | COLUMN_PORTALURL_FLAG | COLUMN_PORTALPORT_FLAG | COLUMN_PORTALDOMAIN_FLAG | COLUMN_PORTALROWSTATUS_FLAG)
    /*
     * TODO:405:r: Review DOT11CONFIGPORTALSERVERGROUPTABLE_REQUIRED_COLS macro.
     * OR together all the required rows for row creation.
     * default is writable cols w/out defaults.
     */
#define DOT11CONFIGPORTALSERVERGROUPTABLE_REQUIRED_COLS (COLUMN_PORTALINDEX_FLAG | COLUMN_PORTALKEYTYPE_FLAG | COLUMN_PORTALKEY_FLAG | COLUMN_PORTALURL_FLAG | COLUMN_PORTALPORT_FLAG | COLUMN_PORTALDOMAIN_FLAG | COLUMN_PORTALROWSTATUS_FLAG)
    

#ifdef __cplusplus
}
#endif

#endif /* DOT11CONFIGPORTALSERVERGROUPTABLE_OIDS_H */
