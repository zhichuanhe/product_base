/*
 * Generated by asn1c-0.9.23 (http://lionet.info/asn1c)
 * From ASN.1 module "RANAP-IEs"
 * 	found in "RANAP-IEs.asn"
 */

#ifndef	_PLMNs_in_shared_network_H_
#define	_PLMNs_in_shared_network_H_


#include <asn_application.h>

/* Including external dependencies */
#include <asn_SEQUENCE_OF.h>
#include "PLMNidentity.h"
#include "LA-LIST.h"
#include <constr_SEQUENCE.h>
#include <constr_SEQUENCE_OF.h>

#ifdef __cplusplus
extern "C" {
#endif

/* Forward declarations */
/* auto changed by perl script */
/*struct ;*/

/* PLMNs-in-shared-network */
typedef struct PLMNs_in_shared_network {
	A_SEQUENCE_OF(struct  Member_PLMNs_in_shared_network {
		PLMNidentity_t	 pLMNidentity;
		LA_LIST_t	 lA_LIST;
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
} PLMNs_in_shared_network_t;

/* Implementation */
extern asn_TYPE_descriptor_t asn_DEF_PLMNs_in_shared_network;

#ifdef __cplusplus
}
#endif

/* Referred external types */
#include "ProtocolExtensionContainer.h"

#endif	/* _PLMNs_in_shared_network_H_ */
#include <asn_internal.h>
