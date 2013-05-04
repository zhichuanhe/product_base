/*
 * Generated by asn1c-0.9.23 (http://lionet.info/asn1c)
 * From ASN.1 module "RANAP-PDU-Contents"
 * 	found in "RANAP-PDU-Contents.asn"
 */

#include "LocationReportingControl.h"

static asn_TYPE_member_t asn_MBR_LocationReportingControl_1[] = {
	{ ATF_NOFLAGS, 0, offsetof(struct LocationReportingControl, protocolIEs),
		(ASN_TAG_CLASS_CONTEXT | (0 << 2)),
		-1,	/* IMPLICIT tag at current level */
		&asn_DEF_ProtocolIE_Container_139P0,
		0,	/* Defer constraints checking to the member type */
		0,	/* No PER visible constraints */
		0,
		"protocolIEs"
		},
	{ ATF_POINTER, 1, offsetof(struct LocationReportingControl, protocolExtensions),
		(ASN_TAG_CLASS_CONTEXT | (1 << 2)),
		-1,	/* IMPLICIT tag at current level */
		&asn_DEF_ProtocolExtensionContainer_209P0,
		0,	/* Defer constraints checking to the member type */
		0,	/* No PER visible constraints */
		0,
		"protocolExtensions"
		},
};
static int asn_MAP_LocationReportingControl_oms_1[] = { 1 };
static ber_tlv_tag_t asn_DEF_LocationReportingControl_tags_1[] = {
	(ASN_TAG_CLASS_UNIVERSAL | (16 << 2))
};
static asn_TYPE_tag2member_t asn_MAP_LocationReportingControl_tag2el_1[] = {
    { (ASN_TAG_CLASS_CONTEXT | (0 << 2)), 0, 0, 0 }, /* protocolIEs at 2226 */
    { (ASN_TAG_CLASS_CONTEXT | (1 << 2)), 1, 0, 0 } /* protocolExtensions at 2227 */
};
static asn_SEQUENCE_specifics_t asn_SPC_LocationReportingControl_specs_1 = {
	sizeof(struct LocationReportingControl),
	offsetof(struct LocationReportingControl, _asn_ctx),
	asn_MAP_LocationReportingControl_tag2el_1,
	2,	/* Count of tags in the map */
	asn_MAP_LocationReportingControl_oms_1,	/* Optional members */
	1, 0,	/* Root/Additions */
	1,	/* Start extensions */
	3	/* Stop extensions */
};
asn_TYPE_descriptor_t asn_DEF_LocationReportingControl = {
	"LocationReportingControl",
	"LocationReportingControl",
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
	asn_DEF_LocationReportingControl_tags_1,
	sizeof(asn_DEF_LocationReportingControl_tags_1)
		/sizeof(asn_DEF_LocationReportingControl_tags_1[0]), /* 1 */
	asn_DEF_LocationReportingControl_tags_1,	/* Same as above */
	sizeof(asn_DEF_LocationReportingControl_tags_1)
		/sizeof(asn_DEF_LocationReportingControl_tags_1[0]), /* 1 */
	0,	/* No PER visible constraints */
	asn_MBR_LocationReportingControl_1,
	2,	/* Elements count */
	&asn_SPC_LocationReportingControl_specs_1	/* Additional specs */
};

