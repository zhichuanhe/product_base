/*
 * Generated by asn1c-0.9.23 (http://lionet.info/asn1c)
 * From ASN.1 module "RANAP-IEs"
 * 	found in "RANAP-IEs.asn"
 */

#ifndef	_RNCTraceInformation_H_
#define	_RNCTraceInformation_H_


#include <asn_application.h>

/* Including external dependencies */
#include "TraceReference.h"
#include <ENUMERATED.h>
#include <constr_SEQUENCE.h>

#ifdef __cplusplus
extern "C" {
#endif

/* Dependencies */
typedef enum traceActivationIndicator {
	traceActivationIndicator_activated	= 0,
	traceActivationIndicator_deactivated	= 1
} e_traceActivationIndicator;

/* Forward declarations */
struct EquipmentsToBeTraced;
/* auto changed by perl script */
/*struct ;*/

/* RNCTraceInformation */
typedef struct RNCTraceInformation {
	TraceReference_t	 traceReference;
	ENUMERATED_t	 traceActivationIndicator;
/* auto changed by perl script */
	struct EquipmentsToBeTraced	*equipmentsToBeTraced;	/* OPTIONAL */
/* auto changed by perl script */
	struct 	 ProtocolExtensionContainer_209P0 *iE_Extensions ;	/* OPTIONAL */
	
	/* Context for parsing across buffer boundaries */
	asn_struct_ctx_t _asn_ctx;
} RNCTraceInformation_t;

/* Implementation */
/* extern asn_TYPE_descriptor_t asn_DEF_traceActivationIndicator_3;	// (Use -fall-defs-global to expose) */
extern asn_TYPE_descriptor_t asn_DEF_RNCTraceInformation;

#ifdef __cplusplus
}
#endif

/* Referred external types */
#include "EquipmentsToBeTraced.h"
#include "ProtocolExtensionContainer.h"

#endif	/* _RNCTraceInformation_H_ */
#include <asn_internal.h>
