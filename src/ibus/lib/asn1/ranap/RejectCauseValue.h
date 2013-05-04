/*
 * Generated by asn1c-0.9.23 (http://lionet.info/asn1c)
 * From ASN.1 module "RANAP-IEs"
 * 	found in "RANAP-IEs.asn"
 */

#ifndef	_RejectCauseValue_H_
#define	_RejectCauseValue_H_


#include <asn_application.h>

/* Including external dependencies */
#include <ENUMERATED.h>

#ifdef __cplusplus
extern "C" {
#endif

/* Dependencies */
typedef enum RejectCauseValue {
	RejectCauseValue_pLMN_Not_Allowed	= 0,
	RejectCauseValue_location_Area_Not_Allowed	= 1,
	RejectCauseValue_roaming_Not_Allowed_In_This_Location_Area	= 2,
	RejectCauseValue_no_Suitable_Cell_In_Location_Area	= 3,
	RejectCauseValue_gPRS_Services_Not_Allowed_In_This_PLMN	= 4,
	RejectCauseValue_cS_PS_coordination_required	= 5
	/*
	 * Enumeration is extensible
	 */
} e_RejectCauseValue;

/* RejectCauseValue */
typedef ENUMERATED_t	 RejectCauseValue_t;

/* Implementation */
extern asn_TYPE_descriptor_t asn_DEF_RejectCauseValue;
asn_struct_free_f RejectCauseValue_free;
asn_struct_print_f RejectCauseValue_print;
asn_constr_check_f RejectCauseValue_constraint;
ber_type_decoder_f RejectCauseValue_decode_ber;
der_type_encoder_f RejectCauseValue_encode_der;
xer_type_decoder_f RejectCauseValue_decode_xer;
xer_type_encoder_f RejectCauseValue_encode_xer;
per_type_decoder_f RejectCauseValue_decode_uper;
per_type_encoder_f RejectCauseValue_encode_uper;

#ifdef __cplusplus
}
#endif

#endif	/* _RejectCauseValue_H_ */
#include <asn_internal.h>
