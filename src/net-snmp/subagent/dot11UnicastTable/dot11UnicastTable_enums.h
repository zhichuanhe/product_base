/*
 * Note: this file originally auto-generated by mib2c using
 *  : generic-table-enums.m2c,v 1.3 2004/05/18 17:44:32 rstory Exp $
 *
 * $Id:$
 */
#ifndef DOT11UNICASTTABLE_ENUMS_H
#define DOT11UNICASTTABLE_ENUMS_H

#ifdef __cplusplus
extern "C" {
#endif

 /*
 * NOTES on enums
 * ==============
 *
 * Value Mapping
 * -------------
 * If the values for your data type don't exactly match the
 * possible values defined by the mib, you should map them
 * below. For example, a boolean flag (1/0) is usually represented
 * as a TruthValue in a MIB, which maps to the values (1/2).
 *
 */
/*************************************************************************
 *************************************************************************
 *
 * enum definitions for table dot11UnicastTable
 *
 *************************************************************************
 *************************************************************************/

/*************************************************************
 * constants for enums for the MIB node
 * NewUnicastCipherEnabled (TruthValue / ASN_INTEGER)
 *
 * since a Textual Convention may be referenced more than once in a
 * MIB, protect againt redefinitions of the enum values.
 */
#ifndef TRUTHVALUE_ENUMS
#define TRUTHVALUE_ENUMS

#define TRUTHVALUE_TRUE  1 
#define TRUTHVALUE_FALSE  2 


#endif /* TRUTHVALUE_ENUMS */

    /*
     * TODO:140:o: Define interal representation of NewUnicastCipherEnabled enums.
     * (used for value mapping; see notes at top of file)
     */
#define INTERNAL_NEWUNICASTCIPHERENABLED_TRUE  1 
#define INTERNAL_NEWUNICASTCIPHERENABLED_FALSE  2 



#ifdef __cplusplus
}
#endif

#endif /* DOT11UNICASTTABLE_ENUMS_H */
