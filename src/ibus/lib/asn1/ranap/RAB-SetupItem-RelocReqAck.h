/*
 * Generated by asn1c-0.9.23 (http://lionet.info/asn1c)
 * From ASN.1 module "RANAP-PDU-Contents"
 * 	found in "RANAP-PDU-Contents.asn"
 */

#ifndef	_RAB_SetupItem_RelocReqAck_H_
#define	_RAB_SetupItem_RelocReqAck_H_


#include <asn_application.h>

/* Including external dependencies */
#include "RAB-ID.h"
#include "TransportLayerAddress.h"
#include <constr_SEQUENCE.h>

#ifdef __cplusplus
extern "C" {
#endif

/* Forward declarations */
struct IuTransportAssociation;
/* auto changed by perl script */
/*struct ;*/

/* RAB-SetupItem-RelocReqAck */
typedef struct RAB_SetupItem_RelocReqAck {
	RAB_ID_t	 rAB_ID;
/* auto changed by perl script */
	TransportLayerAddress_t	*transportLayerAddress;	/* OPTIONAL */
/* auto changed by perl script */
	struct IuTransportAssociation	*iuTransportAssociation;	/* OPTIONAL */
/* auto changed by perl script */
	struct 	 ProtocolExtensionContainer_209P0 *iE_Extensions ;	/* OPTIONAL */
	/*
	 * This type is extensible,
	 * possible extensions are below.
	 */
	
	/* Context for parsing across buffer boundaries */
	asn_struct_ctx_t _asn_ctx;
} RAB_SetupItem_RelocReqAck_t;

/* Implementation */
extern asn_TYPE_descriptor_t asn_DEF_RAB_SetupItem_RelocReqAck;

#ifdef __cplusplus
}
#endif

/* Referred external types */
#include "IuTransportAssociation.h"
#include "ProtocolExtensionContainer.h"

#endif	/* _RAB_SetupItem_RelocReqAck_H_ */
#include <asn_internal.h>
