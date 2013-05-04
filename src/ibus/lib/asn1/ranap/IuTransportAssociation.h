/*
 * Generated by asn1c-0.9.23 (http://lionet.info/asn1c)
 * From ASN.1 module "RANAP-IEs"
 * 	found in "RANAP-IEs.asn"
 */

#ifndef	_IuTransportAssociation_H_
#define	_IuTransportAssociation_H_


#include <asn_application.h>

/* Including external dependencies */
#include "GTP-TEI.h"
#include "BindingID.h"
#include <constr_CHOICE.h>

#ifdef __cplusplus
extern "C" {
#endif

/* Dependencies */
typedef enum IuTransportAssociation_PR {
	IuTransportAssociation_PR_NOTHING,	/* No components present */
	IuTransportAssociation_PR_gTP_TEI,
	IuTransportAssociation_PR_bindingID,
	/* Extensions may appear below */
	
} IuTransportAssociation_PR;

/* IuTransportAssociation */
typedef struct IuTransportAssociation {
	IuTransportAssociation_PR present;
	union IuTransportAssociation_u {
		GTP_TEI_t	 gTP_TEI;
		BindingID_t	 bindingID;
		/*
		 * This type is extensible,
		 * possible extensions are below.
		 */
	} choice;
	
	/* Context for parsing across buffer boundaries */
	asn_struct_ctx_t _asn_ctx;
} IuTransportAssociation_t;

/* Implementation */
extern asn_TYPE_descriptor_t asn_DEF_IuTransportAssociation;

#ifdef __cplusplus
}
#endif

#endif	/* _IuTransportAssociation_H_ */
#include <asn_internal.h>
