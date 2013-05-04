/*
 * Generated by asn1c-0.9.23 (http://lionet.info/asn1c)
 * From ASN.1 module "RANAP-IEs"
 * 	found in "RANAP-IEs.asn"
 */

#include "TemporaryUE-ID.h"

static asn_per_constraints_t asn_PER_type_TemporaryUE_ID_constr_1 = {
	{ APC_CONSTRAINED | APC_EXTENSIBLE,  1,  1,  0,  1 }	/* (0..1,...) */,
	{ APC_UNCONSTRAINED,	-1, -1,  0,  0 },
	0, 0	/* No PER value map */
};
static asn_TYPE_member_t asn_MBR_TemporaryUE_ID_1[] = {
	{ ATF_NOFLAGS, 0, offsetof(struct TemporaryUE_ID, choice.tMSI),
		(ASN_TAG_CLASS_CONTEXT | (0 << 2)),
		-1,	/* IMPLICIT tag at current level */
		&asn_DEF_TMSI,
		0,	/* Defer constraints checking to the member type */
		0,	/* No PER visible constraints */
		0,
		"tMSI"
		},
	{ ATF_NOFLAGS, 0, offsetof(struct TemporaryUE_ID, choice.p_TMSI),
		(ASN_TAG_CLASS_CONTEXT | (1 << 2)),
		-1,	/* IMPLICIT tag at current level */
		&asn_DEF_P_TMSI,
		0,	/* Defer constraints checking to the member type */
		0,	/* No PER visible constraints */
		0,
		"p-TMSI"
		},
};
static asn_TYPE_tag2member_t asn_MAP_TemporaryUE_ID_tag2el_1[] = {
    { (ASN_TAG_CLASS_CONTEXT | (0 << 2)), 0, 0, 0 }, /* tMSI at 1999 */
    { (ASN_TAG_CLASS_CONTEXT | (1 << 2)), 1, 0, 0 } /* p-TMSI at 2000 */
};
static asn_CHOICE_specifics_t asn_SPC_TemporaryUE_ID_specs_1 = {
	sizeof(struct TemporaryUE_ID),
	offsetof(struct TemporaryUE_ID, _asn_ctx),
	offsetof(struct TemporaryUE_ID, present),
	sizeof(((struct TemporaryUE_ID *)0)->present),
	asn_MAP_TemporaryUE_ID_tag2el_1,
	2,	/* Count of tags in the map */
	0,
	2	/* Extensions start */
};
asn_TYPE_descriptor_t asn_DEF_TemporaryUE_ID = {
	"TemporaryUE-ID",
	"TemporaryUE-ID",
	CHOICE_free,
	CHOICE_print,
	CHOICE_constraint,
	CHOICE_decode_ber,
	CHOICE_encode_der,
	CHOICE_decode_xer,
	CHOICE_encode_xer,
	CHOICE_decode_uper,
	CHOICE_encode_uper,
	CHOICE_outmost_tag,
	0,	/* No effective tags (pointer) */
	0,	/* No effective tags (count) */
	0,	/* No tags (pointer) */
	0,	/* No tags (count) */
	&asn_PER_type_TemporaryUE_ID_constr_1,
	asn_MBR_TemporaryUE_ID_1,
	2,	/* Elements count */
	&asn_SPC_TemporaryUE_ID_specs_1	/* Additional specs */
};

