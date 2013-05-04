/*
 * Generated by asn1c-0.9.23 (http://lionet.info/asn1c)
 * From ASN.1 module "RANAP-PDU-Contents"
 * 	found in "RANAP-PDU-Contents.asn"
 */

#include "MBMSSynchronisationInformation.h"

static asn_TYPE_member_t asn_MBR_MBMSSynchronisationInformation_1[] = {
	{ ATF_NOFLAGS, 0, offsetof(struct MBMSSynchronisationInformation, mBMSHCIndicator),
		(ASN_TAG_CLASS_CONTEXT | (0 << 2)),
		-1,	/* IMPLICIT tag at current level */
		&asn_DEF_MBMSHCIndicator,
		0,	/* Defer constraints checking to the member type */
		0,	/* No PER visible constraints */
		0,
		"mBMSHCIndicator"
		},
	{ ATF_NOFLAGS, 0, offsetof(struct MBMSSynchronisationInformation, iPMulticastAddress),
		(ASN_TAG_CLASS_CONTEXT | (1 << 2)),
		-1,	/* IMPLICIT tag at current level */
		&asn_DEF_IPMulticastAddress,
		0,	/* Defer constraints checking to the member type */
		0,	/* No PER visible constraints */
		0,
		"iPMulticastAddress"
		},
	{ ATF_NOFLAGS, 0, offsetof(struct MBMSSynchronisationInformation, gTPDLTEID),
		(ASN_TAG_CLASS_CONTEXT | (2 << 2)),
		-1,	/* IMPLICIT tag at current level */
		&asn_DEF_GTP_TEI,
		0,	/* Defer constraints checking to the member type */
		0,	/* No PER visible constraints */
		0,
		"gTPDLTEID"
		},
	{ ATF_POINTER, 1, offsetof(struct MBMSSynchronisationInformation, iE_Extensions),
		(ASN_TAG_CLASS_CONTEXT | (3 << 2)),
		-1,	/* IMPLICIT tag at current level */
		&asn_DEF_ProtocolExtensionContainer_209P0,
		0,	/* Defer constraints checking to the member type */
		0,	/* No PER visible constraints */
		0,
		"iE-Extensions"
		},
};
static int asn_MAP_MBMSSynchronisationInformation_oms_1[] = { 3 };
static ber_tlv_tag_t asn_DEF_MBMSSynchronisationInformation_tags_1[] = {
	(ASN_TAG_CLASS_UNIVERSAL | (16 << 2))
};
static asn_TYPE_tag2member_t asn_MAP_MBMSSynchronisationInformation_tag2el_1[] = {
    { (ASN_TAG_CLASS_CONTEXT | (0 << 2)), 0, 0, 0 }, /* mBMSHCIndicator at 3809 */
    { (ASN_TAG_CLASS_CONTEXT | (1 << 2)), 1, 0, 0 }, /* iPMulticastAddress at 3810 */
    { (ASN_TAG_CLASS_CONTEXT | (2 << 2)), 2, 0, 0 }, /* gTPDLTEID at 3811 */
    { (ASN_TAG_CLASS_CONTEXT | (3 << 2)), 3, 0, 0 } /* iE-Extensions at 3812 */
};
static asn_SEQUENCE_specifics_t asn_SPC_MBMSSynchronisationInformation_specs_1 = {
	sizeof(struct MBMSSynchronisationInformation),
	offsetof(struct MBMSSynchronisationInformation, _asn_ctx),
	asn_MAP_MBMSSynchronisationInformation_tag2el_1,
	4,	/* Count of tags in the map */
	asn_MAP_MBMSSynchronisationInformation_oms_1,	/* Optional members */
	1, 0,	/* Root/Additions */
	3,	/* Start extensions */
	5	/* Stop extensions */
};
asn_TYPE_descriptor_t asn_DEF_MBMSSynchronisationInformation = {
	"MBMSSynchronisationInformation",
	"MBMSSynchronisationInformation",
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
	asn_DEF_MBMSSynchronisationInformation_tags_1,
	sizeof(asn_DEF_MBMSSynchronisationInformation_tags_1)
		/sizeof(asn_DEF_MBMSSynchronisationInformation_tags_1[0]), /* 1 */
	asn_DEF_MBMSSynchronisationInformation_tags_1,	/* Same as above */
	sizeof(asn_DEF_MBMSSynchronisationInformation_tags_1)
		/sizeof(asn_DEF_MBMSSynchronisationInformation_tags_1[0]), /* 1 */
	0,	/* No PER visible constraints */
	asn_MBR_MBMSSynchronisationInformation_1,
	4,	/* Elements count */
	&asn_SPC_MBMSSynchronisationInformation_specs_1	/* Additional specs */
};

