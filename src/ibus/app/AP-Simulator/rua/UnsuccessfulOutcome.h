/*
 * Generated by asn1c-0.9.23 (http://lionet.info/asn1c)
 * From ASN.1 module "RUA-PDU-Descriptions"
 * 	found in "RUA-PDU-Descriptions.asn"
 */

#ifndef	_UnsuccessfulOutcome_H_
#define	_UnsuccessfulOutcome_H_


#include <asn_application.h>

/* Including external dependencies */
#include "ProcedureCode.h"
#include "Criticality.h"
#include <ANY.h>
#include <constr_SEQUENCE.h>

#ifdef __cplusplus
extern "C" {
#endif

/* UnsuccessfulOutcome */
typedef struct UnsuccessfulOutcome {
	ProcedureCode_t	 procedureCode;
	Criticality_t	 criticality;
	ANY_t	 value;
	
	/* Context for parsing across buffer boundaries */
	asn_struct_ctx_t _asn_ctx;
} UnsuccessfulOutcome_t;

/* Implementation */
extern asn_TYPE_descriptor_t asn_DEF_UnsuccessfulOutcome;

#ifdef __cplusplus
}
#endif

#endif	/* _UnsuccessfulOutcome_H_ */
#include <asn_internal.h>
