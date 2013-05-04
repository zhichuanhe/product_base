/*
 * Generated by asn1c-0.9.23 (http://lionet.info/asn1c)
 * From ASN.1 module "RANAP-IEs"
 * 	found in "RANAP-IEs.asn"
 */

#ifndef	_HorizontalWithVerticalVelocity_H_
#define	_HorizontalWithVerticalVelocity_H_


#include <asn_application.h>

/* Including external dependencies */
#include "HorizontalSpeedAndBearing.h"
#include "VerticalVelocity.h"
#include <constr_SEQUENCE.h>

#ifdef __cplusplus
extern "C" {
#endif

/* Forward declarations */
/* auto changed by perl script */
/*struct ;*/

/* HorizontalWithVerticalVelocity */
typedef struct HorizontalWithVerticalVelocity {
	HorizontalSpeedAndBearing_t	 horizontalSpeedAndBearing;
	VerticalVelocity_t	 veritcalVelocity;
/* auto changed by perl script */
	struct 	 ProtocolExtensionContainer_209P0 *iE_Extensions ;	/* OPTIONAL */
	/*
	 * This type is extensible,
	 * possible extensions are below.
	 */
	
	/* Context for parsing across buffer boundaries */
	asn_struct_ctx_t _asn_ctx;
} HorizontalWithVerticalVelocity_t;

/* Implementation */
extern asn_TYPE_descriptor_t asn_DEF_HorizontalWithVerticalVelocity;

#ifdef __cplusplus
}
#endif

/* Referred external types */
#include "ProtocolExtensionContainer.h"

#endif	/* _HorizontalWithVerticalVelocity_H_ */
#include <asn_internal.h>
