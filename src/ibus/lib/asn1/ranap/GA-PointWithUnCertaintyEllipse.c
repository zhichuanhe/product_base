/*
 * Generated by asn1c-0.9.23 (http://lionet.info/asn1c)
 * From ASN.1 module "RANAP-IEs"
 * 	found in "RANAP-IEs.asn"
 */

#include "GA-PointWithUnCertaintyEllipse.h"

static int
memb_confidence_constraint_1(asn_TYPE_descriptor_t *td, const void *sptr,
			asn_app_constraint_failed_f *ctfailcb, void *app_key) {
	long value;
	
	if(!sptr) {
		_ASN_CTFAIL(app_key, td, sptr,
			"%s: value not given (%s:%d)",
			td->name, __FILE__, __LINE__);
		return -1;
	}
	
	value = *(const long *)sptr;
	
	if((value >= 0 && value <= 127)) {
		/* Constraint check succeeded */
		return 0;
	} else {
		_ASN_CTFAIL(app_key, td, sptr,
			"%s: constraint failed (%s:%d)",
			td->name, __FILE__, __LINE__);
		return -1;
	}
}

static asn_per_constraints_t asn_PER_memb_confidence_constr_4 = {
	{ APC_CONSTRAINED,	 7,  7,  0,  127 }	/* (0..127) */,
	{ APC_UNCONSTRAINED,	-1, -1,  0,  0 },
	0, 0	/* No PER value map */
};
static asn_TYPE_member_t asn_MBR_GA_PointWithUnCertaintyEllipse_1[] = {
	{ ATF_NOFLAGS, 0, offsetof(struct GA_PointWithUnCertaintyEllipse, geographicalCoordinates),
		(ASN_TAG_CLASS_CONTEXT | (0 << 2)),
		-1,	/* IMPLICIT tag at current level */
		&asn_DEF_GeographicalCoordinates,
		0,	/* Defer constraints checking to the member type */
		0,	/* No PER visible constraints */
		0,
		"geographicalCoordinates"
		},
	{ ATF_NOFLAGS, 0, offsetof(struct GA_PointWithUnCertaintyEllipse, uncertaintyEllipse),
		(ASN_TAG_CLASS_CONTEXT | (1 << 2)),
		-1,	/* IMPLICIT tag at current level */
		&asn_DEF_GA_UncertaintyEllipse,
		0,	/* Defer constraints checking to the member type */
		0,	/* No PER visible constraints */
		0,
		"uncertaintyEllipse"
		},
	{ ATF_NOFLAGS, 0, offsetof(struct GA_PointWithUnCertaintyEllipse, confidence),
		(ASN_TAG_CLASS_CONTEXT | (2 << 2)),
		-1,	/* IMPLICIT tag at current level */
		&asn_DEF_NativeInteger,
		memb_confidence_constraint_1,
		&asn_PER_memb_confidence_constr_4,
		0,
		"confidence"
		},
	{ ATF_POINTER, 1, offsetof(struct GA_PointWithUnCertaintyEllipse, iE_Extensions),
		(ASN_TAG_CLASS_CONTEXT | (3 << 2)),
		-1,	/* IMPLICIT tag at current level */
		&asn_DEF_ProtocolExtensionContainer_209P0,
		0,	/* Defer constraints checking to the member type */
		0,	/* No PER visible constraints */
		0,
		"iE-Extensions"
		},
};
static int asn_MAP_GA_PointWithUnCertaintyEllipse_oms_1[] = { 3 };
static ber_tlv_tag_t asn_DEF_GA_PointWithUnCertaintyEllipse_tags_1[] = {
	(ASN_TAG_CLASS_UNIVERSAL | (16 << 2))
};
static asn_TYPE_tag2member_t asn_MAP_GA_PointWithUnCertaintyEllipse_tag2el_1[] = {
    { (ASN_TAG_CLASS_CONTEXT | (0 << 2)), 0, 0, 0 }, /* geographicalCoordinates at 842 */
    { (ASN_TAG_CLASS_CONTEXT | (1 << 2)), 1, 0, 0 }, /* uncertaintyEllipse at 843 */
    { (ASN_TAG_CLASS_CONTEXT | (2 << 2)), 2, 0, 0 }, /* confidence at 844 */
    { (ASN_TAG_CLASS_CONTEXT | (3 << 2)), 3, 0, 0 } /* iE-Extensions at 845 */
};
static asn_SEQUENCE_specifics_t asn_SPC_GA_PointWithUnCertaintyEllipse_specs_1 = {
	sizeof(struct GA_PointWithUnCertaintyEllipse),
	offsetof(struct GA_PointWithUnCertaintyEllipse, _asn_ctx),
	asn_MAP_GA_PointWithUnCertaintyEllipse_tag2el_1,
	4,	/* Count of tags in the map */
	asn_MAP_GA_PointWithUnCertaintyEllipse_oms_1,	/* Optional members */
	1, 0,	/* Root/Additions */
	3,	/* Start extensions */
	5	/* Stop extensions */
};
asn_TYPE_descriptor_t asn_DEF_GA_PointWithUnCertaintyEllipse = {
	"GA-PointWithUnCertaintyEllipse",
	"GA-PointWithUnCertaintyEllipse",
	SEQUENCE_free_for_RANAP,
	SEQUENCE_print,
	SEQUENCE_constraint,
	SEQUENCE_decode_ber,
	SEQUENCE_encode_der,
	SEQUENCE_decode_xer,
	SEQUENCE_encode_xer,
	SEQUENCE_decode_uper_for_RANAP,
	SEQUENCE_encode_uper_for_RANAP,
	0,	/* Use generic outmost tag fetcher */
	asn_DEF_GA_PointWithUnCertaintyEllipse_tags_1,
	sizeof(asn_DEF_GA_PointWithUnCertaintyEllipse_tags_1)
		/sizeof(asn_DEF_GA_PointWithUnCertaintyEllipse_tags_1[0]), /* 1 */
	asn_DEF_GA_PointWithUnCertaintyEllipse_tags_1,	/* Same as above */
	sizeof(asn_DEF_GA_PointWithUnCertaintyEllipse_tags_1)
		/sizeof(asn_DEF_GA_PointWithUnCertaintyEllipse_tags_1[0]), /* 1 */
	0,	/* No PER visible constraints */
	asn_MBR_GA_PointWithUnCertaintyEllipse_1,
	4,	/* Elements count */
	&asn_SPC_GA_PointWithUnCertaintyEllipse_specs_1	/* Additional specs */
};

