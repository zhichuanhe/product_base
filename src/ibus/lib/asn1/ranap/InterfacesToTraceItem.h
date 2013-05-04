/*
 * Generated by asn1c-0.9.23 (http://lionet.info/asn1c)
 * From ASN.1 module "RANAP-IEs"
 * 	found in "RANAP-IEs.asn"
 */

#ifndef	_InterfacesToTraceItem_H_
#define	_InterfacesToTraceItem_H_


#include <asn_application.h>

/* Including external dependencies */
#include <ENUMERATED.h>
#include <constr_SEQUENCE.h>

#ifdef __cplusplus
extern "C" {
#endif

/* Dependencies */
typedef enum interface {
	interface_iu_cs	= 0,
	interface_iu_ps	= 1,
	interface_iur	= 2,
	interface_iub	= 3,
	interface_uu	= 4
	/*
	 * Enumeration is extensible
	 */
} e_interface;

/* Forward declarations */
/* auto changed by perl script */
/*struct ;*/

/* InterfacesToTraceItem */
typedef struct InterfacesToTraceItem {
	ENUMERATED_t	 interface;
/* auto changed by perl script */
	struct 	 ProtocolExtensionContainer_209P0 *iE_Extensions ;	/* OPTIONAL */
	/*
	 * This type is extensible,
	 * possible extensions are below.
	 */
	
	/* Context for parsing across buffer boundaries */
	asn_struct_ctx_t _asn_ctx;
} InterfacesToTraceItem_t;

/* Implementation */
/* extern asn_TYPE_descriptor_t asn_DEF_interface_2;	// (Use -fall-defs-global to expose) */
extern asn_TYPE_descriptor_t asn_DEF_InterfacesToTraceItem;

#ifdef __cplusplus
}
#endif

/* Referred external types */
#include "ProtocolExtensionContainer.h"

#endif	/* _InterfacesToTraceItem_H_ */
#include <asn_internal.h>
