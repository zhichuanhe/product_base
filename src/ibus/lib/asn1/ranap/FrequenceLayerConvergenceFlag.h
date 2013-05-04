/*
 * Generated by asn1c-0.9.23 (http://lionet.info/asn1c)
 * From ASN.1 module "RANAP-IEs"
 * 	found in "RANAP-IEs.asn"
 */

#ifndef	_FrequenceLayerConvergenceFlag_H_
#define	_FrequenceLayerConvergenceFlag_H_


#include <asn_application.h>

/* Including external dependencies */
#include <ENUMERATED.h>

#ifdef __cplusplus
extern "C" {
#endif

/* Dependencies */
typedef enum FrequenceLayerConvergenceFlag {
	FrequenceLayerConvergenceFlag_no_FLC_flag	= 0
	/*
	 * Enumeration is extensible
	 */
} e_FrequenceLayerConvergenceFlag;

/* FrequenceLayerConvergenceFlag */
typedef ENUMERATED_t	 FrequenceLayerConvergenceFlag_t;

/* Implementation */
extern asn_TYPE_descriptor_t asn_DEF_FrequenceLayerConvergenceFlag;
asn_struct_free_f FrequenceLayerConvergenceFlag_free;
asn_struct_print_f FrequenceLayerConvergenceFlag_print;
asn_constr_check_f FrequenceLayerConvergenceFlag_constraint;
ber_type_decoder_f FrequenceLayerConvergenceFlag_decode_ber;
der_type_encoder_f FrequenceLayerConvergenceFlag_encode_der;
xer_type_decoder_f FrequenceLayerConvergenceFlag_decode_xer;
xer_type_encoder_f FrequenceLayerConvergenceFlag_encode_xer;
per_type_decoder_f FrequenceLayerConvergenceFlag_decode_uper;
per_type_encoder_f FrequenceLayerConvergenceFlag_encode_uper;

#ifdef __cplusplus
}
#endif

#endif	/* _FrequenceLayerConvergenceFlag_H_ */
#include <asn_internal.h>
