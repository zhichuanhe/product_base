/*
 * Generated by asn1c-0.9.23 (http://lionet.info/asn1c)
 * From ASN.1 module "HNBAP-PDU-Contents"
 * 	found in "../asn/HNBAP-PDU-Contents.asn"
 */

#ifndef	_PrivateMessage_H_
#define	_PrivateMessage_H_


#include <asn_application.h>

/* Including external dependencies */
#include "PrivateIE-Container.h"
#include <constr_SEQUENCE.h>

#ifdef __cplusplus
extern "C" {
#endif

/* PrivateMessage */
typedef struct PrivateMessage {
	PrivateIE_Container_165P0_t	 privateIEs;
	/*
	 * This type is extensible,
	 * possible extensions are below.
	 */
	
	/* Context for parsing across buffer boundaries */
	asn_struct_ctx_t _asn_ctx;
} PrivateMessage_t;

/* Implementation */
extern asn_TYPE_descriptor_t HNBAP_asn_DEF_PrivateMessage;

#ifdef __cplusplus
}
#endif

#endif	/* _PrivateMessage_H_ */
#include <asn_internal.h>
