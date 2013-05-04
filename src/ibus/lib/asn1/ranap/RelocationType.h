/*
 * Generated by asn1c-0.9.23 (http://lionet.info/asn1c)
 * From ASN.1 module "RANAP-IEs"
 * 	found in "RANAP-IEs.asn"
 */

#ifndef	_RelocationType_H_
#define	_RelocationType_H_


#include <asn_application.h>

/* Including external dependencies */
#include <ENUMERATED.h>

#ifdef __cplusplus
extern "C" {
#endif

/* Dependencies */
typedef enum RelocationType {
	RelocationType_ue_not_involved	= 0,
	RelocationType_ue_involved	= 1
	/*
	 * Enumeration is extensible
	 */
} e_RelocationType;

/* RelocationType */
typedef ENUMERATED_t	 RelocationType_t;

/* Implementation */
extern asn_TYPE_descriptor_t asn_DEF_RelocationType;
asn_struct_free_f RelocationType_free;
asn_struct_print_f RelocationType_print;
asn_constr_check_f RelocationType_constraint;
ber_type_decoder_f RelocationType_decode_ber;
der_type_encoder_f RelocationType_encode_der;
xer_type_decoder_f RelocationType_decode_xer;
xer_type_encoder_f RelocationType_encode_xer;
per_type_decoder_f RelocationType_decode_uper;
per_type_encoder_f RelocationType_encode_uper;

#ifdef __cplusplus
}
#endif

#endif	/* _RelocationType_H_ */
#include <asn_internal.h>
