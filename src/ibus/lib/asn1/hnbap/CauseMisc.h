/*
 * Generated by asn1c-0.9.23 (http://lionet.info/asn1c)
 * From ASN.1 module "HNBAP-IEs"
 * 	found in "../asn/HNBAP-IEs.asn"
 */

#ifndef	_CauseMisc_H_
#define	_CauseMisc_H_


#include <asn_application.h>

/* Including external dependencies */
#include <ENUMERATED.h>

#ifdef __cplusplus
extern "C" {
#endif

/* Dependencies */
typedef enum CauseMisc {
	CauseMisc_processing_overload	= 0,
	CauseMisc_hardware_failure	= 1,
	CauseMisc_o_and_m_intervention	= 2,
	CauseMisc_unspecified	= 3
	/*
	 * Enumeration is extensible
	 */
} e_CauseMisc;

/* CauseMisc */
typedef ENUMERATED_t	 CauseMisc_t;

/* Implementation */
extern asn_TYPE_descriptor_t asn_DEF_CauseMisc;
asn_struct_free_f CauseMisc_free;
asn_struct_print_f CauseMisc_print;
asn_constr_check_f CauseMisc_constraint;
ber_type_decoder_f CauseMisc_decode_ber;
der_type_encoder_f CauseMisc_encode_der;
xer_type_decoder_f CauseMisc_decode_xer;
xer_type_encoder_f CauseMisc_encode_xer;
per_type_decoder_f CauseMisc_decode_uper;
per_type_encoder_f CauseMisc_encode_uper;

#ifdef __cplusplus
}
#endif

#endif	/* _CauseMisc_H_ */
#include <asn_internal.h>
