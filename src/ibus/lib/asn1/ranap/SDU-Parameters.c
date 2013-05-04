/*
 * Generated by asn1c-0.9.23 (http://lionet.info/asn1c)
 * From ASN.1 module "RANAP-IEs"
 * 	found in "RANAP-IEs.asn"
 */

#include "SDU-Parameters.h"

static asn_per_constraints_t asn_PER_type_SDU_Parameters_constr_1 = {
	{ APC_UNCONSTRAINED,	-1, -1,  0,  0 },
	{ APC_CONSTRAINED,	 3,  3,  1,  7 }	/* (SIZE(1..7)) */,
	0, 0	/* No PER value map */
};
static asn_TYPE_member_t asn_MBR_Member_2[] = {
	{ ATF_POINTER, 1, offsetof(struct Member_SDU_Parameters, sDU_ErrorRatio),
		(ASN_TAG_CLASS_CONTEXT | (0 << 2)),
		-1,	/* IMPLICIT tag at current level */
		&asn_DEF_SDU_ErrorRatio,
		0,	/* Defer constraints checking to the member type */
		0,	/* No PER visible constraints */
		0,
		"sDU-ErrorRatio"
		},
	{ ATF_NOFLAGS, 0, offsetof(struct Member_SDU_Parameters, residualBitErrorRatio),
		(ASN_TAG_CLASS_CONTEXT | (1 << 2)),
		-1,	/* IMPLICIT tag at current level */
		&asn_DEF_ResidualBitErrorRatio,
		0,	/* Defer constraints checking to the member type */
		0,	/* No PER visible constraints */
		0,
		"residualBitErrorRatio"
		},
	{ ATF_NOFLAGS, 0, offsetof(struct Member_SDU_Parameters, deliveryOfErroneousSDU),
		(ASN_TAG_CLASS_CONTEXT | (2 << 2)),
		-1,	/* IMPLICIT tag at current level */
		&asn_DEF_DeliveryOfErroneousSDU,
		0,	/* Defer constraints checking to the member type */
		0,	/* No PER visible constraints */
		0,
		"deliveryOfErroneousSDU"
		},
	{ ATF_POINTER, 2, offsetof(struct Member_SDU_Parameters, sDU_FormatInformationParameters),
		(ASN_TAG_CLASS_CONTEXT | (3 << 2)),
		-1,	/* IMPLICIT tag at current level */
		&asn_DEF_SDU_FormatInformationParameters,
		0,	/* Defer constraints checking to the member type */
		0,	/* No PER visible constraints */
		0,
		"sDU-FormatInformationParameters"
		},
	{ ATF_POINTER, 1, offsetof(struct Member_SDU_Parameters, iE_Extensions),
		(ASN_TAG_CLASS_CONTEXT | (4 << 2)),
		-1,	/* IMPLICIT tag at current level */
		&asn_DEF_ProtocolExtensionContainer_209P0,
		0,	/* Defer constraints checking to the member type */
		0,	/* No PER visible constraints */
		0,
		"iE-Extensions"
		},
};
static int asn_MAP_Member_oms_2[] = { 0, 3, 4 };
static ber_tlv_tag_t asn_DEF_Member_tags_2[] = {
	(ASN_TAG_CLASS_UNIVERSAL | (16 << 2))
};
static asn_TYPE_tag2member_t asn_MAP_Member_tag2el_2[] = {
    { (ASN_TAG_CLASS_CONTEXT | (0 << 2)), 0, 0, 0 }, /* sDU-ErrorRatio at 1719 */
    { (ASN_TAG_CLASS_CONTEXT | (1 << 2)), 1, 0, 0 }, /* residualBitErrorRatio at 1721 */
    { (ASN_TAG_CLASS_CONTEXT | (2 << 2)), 2, 0, 0 }, /* deliveryOfErroneousSDU at 1722 */
    { (ASN_TAG_CLASS_CONTEXT | (3 << 2)), 3, 0, 0 }, /* sDU-FormatInformationParameters at 1723 */
    { (ASN_TAG_CLASS_CONTEXT | (4 << 2)), 4, 0, 0 } /* iE-Extensions at 1724 */
};
static asn_SEQUENCE_specifics_t asn_SPC_Member_specs_2 = {
	sizeof(struct Member_SDU_Parameters),
	offsetof(struct Member_SDU_Parameters, _asn_ctx),
	asn_MAP_Member_tag2el_2,
	5,	/* Count of tags in the map */
	asn_MAP_Member_oms_2,	/* Optional members */
	3, 0,	/* Root/Additions */
	4,	/* Start extensions */
	6	/* Stop extensions */
};
static /* Use -fall-defs-global to expose */
asn_TYPE_descriptor_t asn_DEF_Member_2 = {
	"SEQUENCE",
	"SEQUENCE",
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
	asn_DEF_Member_tags_2,
	sizeof(asn_DEF_Member_tags_2)
		/sizeof(asn_DEF_Member_tags_2[0]), /* 1 */
	asn_DEF_Member_tags_2,	/* Same as above */
	sizeof(asn_DEF_Member_tags_2)
		/sizeof(asn_DEF_Member_tags_2[0]), /* 1 */
	0,	/* No PER visible constraints */
	asn_MBR_Member_2,
	5,	/* Elements count */
	&asn_SPC_Member_specs_2	/* Additional specs */
};

static asn_TYPE_member_t asn_MBR_SDU_Parameters_1[] = {
	{ ATF_POINTER, 0, 0,
		(ASN_TAG_CLASS_UNIVERSAL | (16 << 2)),
		0,
		&asn_DEF_Member_2,
		0,	/* Defer constraints checking to the member type */
		0,	/* No PER visible constraints */
		0,
		""
		},
};
static ber_tlv_tag_t asn_DEF_SDU_Parameters_tags_1[] = {
	(ASN_TAG_CLASS_UNIVERSAL | (16 << 2))
};
static asn_SET_OF_specifics_t asn_SPC_SDU_Parameters_specs_1 = {
	sizeof(struct SDU_Parameters),
	offsetof(struct SDU_Parameters, _asn_ctx),
	0,	/* XER encoding is XMLDelimitedItemList */
};
asn_TYPE_descriptor_t asn_DEF_SDU_Parameters = {
	"SDU-Parameters",
	"SDU-Parameters",
	SEQUENCE_OF_free,
	SEQUENCE_OF_print,
	SEQUENCE_OF_constraint,
	SEQUENCE_OF_decode_ber,
	SEQUENCE_OF_encode_der,
	SEQUENCE_OF_decode_xer,
	SEQUENCE_OF_encode_xer,
	SEQUENCE_OF_decode_uper,
	SEQUENCE_OF_encode_uper,
	0,	/* Use generic outmost tag fetcher */
	asn_DEF_SDU_Parameters_tags_1,
	sizeof(asn_DEF_SDU_Parameters_tags_1)
		/sizeof(asn_DEF_SDU_Parameters_tags_1[0]), /* 1 */
	asn_DEF_SDU_Parameters_tags_1,	/* Same as above */
	sizeof(asn_DEF_SDU_Parameters_tags_1)
		/sizeof(asn_DEF_SDU_Parameters_tags_1[0]), /* 1 */
	&asn_PER_type_SDU_Parameters_constr_1,
	asn_MBR_SDU_Parameters_1,
	1,	/* Single element */
	&asn_SPC_SDU_Parameters_specs_1	/* Additional specs */
};

