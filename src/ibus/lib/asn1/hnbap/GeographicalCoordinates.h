/*
 * Generated by asn1c-0.9.23 (http://lionet.info/asn1c)
 * From ASN.1 module "HNBAP-IEs"
 * 	found in "../asn/HNBAP-IEs.asn"
 */

#ifndef	_GeographicalCoordinates_H_
#define	_GeographicalCoordinates_H_


#include <asn_application.h>

/* Including external dependencies */
#include <ENUMERATED.h>
#include <NativeInteger.h>
#include <constr_SEQUENCE.h>

#ifdef __cplusplus
extern "C" {
#endif

/* Dependencies */
typedef enum latitudeSign {
	latitudeSign_north	= 0,
	latitudeSign_south	= 1
} e_latitudeSign;

/* Forward declarations */
/* auto changed by perl script */
/*struct ;*/

/* GeographicalCoordinates */
typedef struct GeographicalCoordinates {
	ENUMERATED_t	 latitudeSign;
	long	 latitude;
	long	 longitude;
/* auto changed by perl script */
	struct 	 ProtocolExtensionContainer_144P0 *iE_Extensions ;	/* OPTIONAL */
	/*
	 * This type is extensible,
	 * possible extensions are below.
	 */
	
	/* Context for parsing across buffer boundaries */
	asn_struct_ctx_t _asn_ctx;
} GeographicalCoordinates_t;

/* Implementation */
/* extern asn_TYPE_descriptor_t asn_DEF_latitudeSign_2;	// (Use -fall-defs-global to expose) */
extern asn_TYPE_descriptor_t asn_DEF_GeographicalCoordinates;

#ifdef __cplusplus
}
#endif

/* Referred external types */
#include "ProtocolExtensionContainer.h"

#endif	/* _GeographicalCoordinates_H_ */
#include <asn_internal.h>
