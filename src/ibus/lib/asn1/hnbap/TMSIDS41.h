/*
 * Generated by asn1c-0.9.23 (http://lionet.info/asn1c)
 * From ASN.1 module "HNBAP-IEs"
 * 	found in "../asn/HNBAP-IEs.asn"
 */

#ifndef	_TMSIDS41_H_
#define	_TMSIDS41_H_


#include <asn_application.h>

/* Including external dependencies */
#include <OCTET_STRING.h>

#ifdef __cplusplus
extern "C" {
#endif

/* TMSIDS41 */
typedef OCTET_STRING_t	 TMSIDS41_t;

/* Implementation */
extern asn_TYPE_descriptor_t asn_DEF_TMSIDS41;
asn_struct_free_f TMSIDS41_free;
asn_struct_print_f TMSIDS41_print;
asn_constr_check_f TMSIDS41_constraint;
ber_type_decoder_f TMSIDS41_decode_ber;
der_type_encoder_f TMSIDS41_encode_der;
xer_type_decoder_f TMSIDS41_decode_xer;
xer_type_encoder_f TMSIDS41_encode_xer;
per_type_decoder_f TMSIDS41_decode_uper;
per_type_encoder_f TMSIDS41_encode_uper;

#ifdef __cplusplus
}
#endif

#endif	/* _TMSIDS41_H_ */
#include <asn_internal.h>
