/*
 * Generated by asn1c-0.9.23 (http://lionet.info/asn1c)
 * From ASN.1 module "RANAP-Containers"
 * 	found in "RANAP-Containers.asn"
 */

#ifndef	_RANAP_ProtocolExtensionContainer_H_
#define	_RANAP_ProtocolExtensionContainer_H_


#include <asn_application.h>
#include "PDU_ARRAY.h"

/* Including external dependencies */
#include <asn_SEQUENCE_OF.h>
#include "ProtocolExtensionID.h"
#include "Criticality.h"
#include <ANY.h>
#include <constr_SEQUENCE.h>
#include <constr_SEQUENCE_OF.h>

#ifdef __cplusplus
extern "C" {
#endif

struct  Ranap_Member_ProtocolExtensionContainer {
	ProtocolExtensionID_t	 id;
	Criticality_t	 criticality;
	ANY_t	 extensionValue;
	
	/* Context for parsing across buffer boundaries */
	asn_struct_ctx_t _asn_ctx;
};


/* ProtocolExtensionContainer */
typedef struct ProtocolExtensionContainer_209P0 {
	A_SEQUENCE_OF(struct Ranap_Member_ProtocolExtensionContainer) list;
	
	/* Context for parsing across buffer boundaries */
	asn_struct_ctx_t _asn_ctx;
} ProtocolExtensionContainer_209P0_t;

/* Implementation */
extern asn_TYPE_descriptor_t asn_DEF_ProtocolExtensionContainer_209P0;

#ifdef __cplusplus
}
#endif

#endif	/* _ProtocolExtensionContainer_H_ */
#include <asn_internal.h>
