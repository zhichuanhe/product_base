/*
 * Generated by asn1c-0.9.23 (http://lionet.info/asn1c)
 * From ASN.1 module "RANAP-PDU-Contents"
 * 	found in "RANAP-PDU-Contents.asn"
 */

#ifndef	_RAB_SetupItem_EnhRelocInfoReq_H_
#define	_RAB_SetupItem_EnhRelocInfoReq_H_


#include <asn_application.h>

/* Including external dependencies */
#include "RAB-ID.h"
#include "RANAP-CN-DomainIndicator.h"
#include "RAB-Parameters.h"
#include "DataVolumeReportingIndication.h"
#include "UserPlaneInformation.h"
#include "Service-Handover.h"
#include <constr_SEQUENCE.h>

#ifdef __cplusplus
extern "C" {
#endif

/* Forward declarations */
struct PDP_TypeInformation;
struct TNLInformationEnhRelInfoReq;
struct Alt_RAB_Parameters;
/* auto changed by perl script */
/*struct ;*/

/* RAB-SetupItem-EnhRelocInfoReq */
typedef struct RAB_SetupItem_EnhRelocInfoReq {
	RAB_ID_t	 rAB_ID;
	RANAP_CN_DomainIndicator_t	 cN_DomainIndicator;
	RAB_Parameters_t	 rAB_Parameters;
/* auto changed by perl script */
	DataVolumeReportingIndication_t	*dataVolumeReportingIndication;	/* OPTIONAL */
/* auto changed by perl script */
	struct PDP_TypeInformation	*pDP_TypeInformation;	/* OPTIONAL */
	UserPlaneInformation_t	 userPlaneInformation;
/* auto changed by perl script */
	struct TNLInformationEnhRelInfoReq	*dataForwardingInformation;	/* OPTIONAL */
/* auto changed by perl script */
	struct TNLInformationEnhRelInfoReq	*sourceSideIuULTNLInfo;	/* OPTIONAL */
/* auto changed by perl script */
	Service_Handover_t	*service_Handover;	/* OPTIONAL */
/* auto changed by perl script */
	struct Alt_RAB_Parameters	*alt_RAB_Parameters;	/* OPTIONAL */
/* auto changed by perl script */
	struct 	 ProtocolExtensionContainer_209P0 *iE_Extensions ;	/* OPTIONAL */
	/*
	 * This type is extensible,
	 * possible extensions are below.
	 */
	
	/* Context for parsing across buffer boundaries */
	asn_struct_ctx_t _asn_ctx;
} RAB_SetupItem_EnhRelocInfoReq_t;

/* Implementation */
extern asn_TYPE_descriptor_t asn_DEF_RAB_SetupItem_EnhRelocInfoReq;

#ifdef __cplusplus
}
#endif

/* Referred external types */
#include "PDP-TypeInformation.h"
#include "TNLInformationEnhRelInfoReq.h"
#include "Alt-RAB-Parameters.h"
#include "ProtocolExtensionContainer.h"

#endif	/* _RAB_SetupItem_EnhRelocInfoReq_H_ */
#include <asn_internal.h>
