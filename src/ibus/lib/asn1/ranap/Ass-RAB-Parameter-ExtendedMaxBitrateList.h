/*
 * Generated by asn1c-0.9.23 (http://lionet.info/asn1c)
 * From ASN.1 module "RANAP-IEs"
 * 	found in "RANAP-IEs.asn"
 */

#ifndef	_Ass_RAB_Parameter_ExtendedMaxBitrateList_H_
#define	_Ass_RAB_Parameter_ExtendedMaxBitrateList_H_


#include <asn_application.h>

/* Including external dependencies */
#include "ExtendedMaxBitrate.h"
#include <asn_SEQUENCE_OF.h>
#include <constr_SEQUENCE_OF.h>

#ifdef __cplusplus
extern "C" {
#endif

/* Ass-RAB-Parameter-ExtendedMaxBitrateList */
typedef struct Ass_RAB_Parameter_ExtendedMaxBitrateList {
	A_SEQUENCE_OF(ExtendedMaxBitrate_t) list;
	
	/* Context for parsing across buffer boundaries */
	asn_struct_ctx_t _asn_ctx;
} Ass_RAB_Parameter_ExtendedMaxBitrateList_t;

/* Implementation */
extern asn_TYPE_descriptor_t asn_DEF_Ass_RAB_Parameter_ExtendedMaxBitrateList;

#ifdef __cplusplus
}
#endif

#endif	/* _Ass_RAB_Parameter_ExtendedMaxBitrateList_H_ */
#include <asn_internal.h>
