/*
 * Generated by asn1c-0.9.23 (http://lionet.info/asn1c)
 * From ASN.1 module "HNBAP-PDU-Contents"
 * 	found in "../asn/HNBAP-PDU-Contents.asn"
 */

#ifndef	_HNBDe_Register_H_
#define	_HNBDe_Register_H_


#include <asn_application.h>

/* Including external dependencies */
#include "HNBAP-ProtocolIE-Container.h"
#include <constr_SEQUENCE.h>

#ifdef __cplusplus
extern "C" {
#endif

/* Forward declarations */
/* auto changed by perl script */
/*struct ;*/

/* HNBDe-Register */
typedef struct HNBDe_Register {
	ProtocolIE_Container_110P0_t	 protocolIEs;
/* auto changed by perl script */
	struct 	 ProtocolExtensionContainer_144P0 *protocolExtensions ;	/* OPTIONAL */
	/*
	 * This type is extensible,
	 * possible extensions are below.
	 */
	
	/* Context for parsing across buffer boundaries */
	asn_struct_ctx_t _asn_ctx;
} HNBDe_Register_t;

/* Implementation */
extern asn_TYPE_descriptor_t asn_DEF_HNBDe_Register;

#ifdef __cplusplus
}
#endif

/* Referred external types */
#include "ProtocolExtensionContainer.h"

#endif	/* _HNBDe_Register_H_ */
#include <asn_internal.h>
