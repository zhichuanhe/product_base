/*
 * Generated by asn1c-0.9.23 (http://lionet.info/asn1c)
 * From ASN.1 module "RANAP-IEs"
 * 	found in "RANAP-IEs.asn"
 */

#ifndef	_MBMSIPMulticastAddressandAPNRequest_H_
#define	_MBMSIPMulticastAddressandAPNRequest_H_


#include <asn_application.h>

/* Including external dependencies */
#include <asn_SEQUENCE_OF.h>
#include <constr_SEQUENCE_OF.h>

#ifdef __cplusplus
extern "C" {
#endif

/* Forward declarations */
struct TMGI;

/* MBMSIPMulticastAddressandAPNRequest */
typedef struct MBMSIPMulticastAddressandAPNRequest {
	A_SEQUENCE_OF(struct TMGI) list;
	
	/* Context for parsing across buffer boundaries */
	asn_struct_ctx_t _asn_ctx;
} MBMSIPMulticastAddressandAPNRequest_t;

/* Implementation */
extern asn_TYPE_descriptor_t asn_DEF_MBMSIPMulticastAddressandAPNRequest;

#ifdef __cplusplus
}
#endif

/* Referred external types */
#include "TMGI.h"

#endif	/* _MBMSIPMulticastAddressandAPNRequest_H_ */
#include <asn_internal.h>
