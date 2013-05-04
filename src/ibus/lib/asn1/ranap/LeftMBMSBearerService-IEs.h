/*
 * Generated by asn1c-0.9.23 (http://lionet.info/asn1c)
 * From ASN.1 module "RANAP-PDU-Contents"
 * 	found in "RANAP-PDU-Contents.asn"
 */

#ifndef	_LeftMBMSBearerService_IEs_H_
#define	_LeftMBMSBearerService_IEs_H_


#include <asn_application.h>

/* Including external dependencies */
#include <asn_SEQUENCE_OF.h>
#include "TMGI.h"
#include <constr_SEQUENCE.h>
#include <constr_SEQUENCE_OF.h>

#ifdef __cplusplus
extern "C" {
#endif

/* Forward declarations */
/* auto changed by perl script */
/*struct ;*/

/* LeftMBMSBearerService-IEs */
typedef struct LeftMBMSBearerService_IEs {
	A_SEQUENCE_OF(struct  Member_LeftMBMSBearerService_IEs {
		TMGI_t	 tMGI;
/* auto changed by perl script */
		struct 	 ProtocolExtensionContainer_209P0 *iE_Extensions ;	/* OPTIONAL */
		/*
		 * This type is extensible,
		 * possible extensions are below.
		 */
		
		/* Context for parsing across buffer boundaries */
		asn_struct_ctx_t _asn_ctx;
	} ) list;
	
	/* Context for parsing across buffer boundaries */
	asn_struct_ctx_t _asn_ctx;
} LeftMBMSBearerService_IEs_t;

/* Implementation */
extern asn_TYPE_descriptor_t asn_DEF_LeftMBMSBearerService_IEs;

#ifdef __cplusplus
}
#endif

/* Referred external types */
#include "ProtocolExtensionContainer.h"

#endif	/* _LeftMBMSBearerService_IEs_H_ */
#include <asn_internal.h>
