/*
 * Generated by asn1c-0.9.23 (http://lionet.info/asn1c)
 * From ASN.1 module "RANAP-PDU-Contents"
 * 	found in "RANAP-PDU-Contents.asn"
 */

#include "RAB-DataVolumeReportItem.h"

static asn_TYPE_member_t asn_MBR_RAB_DataVolumeReportItem_1[] = {
	{ ATF_NOFLAGS, 0, offsetof(struct RAB_DataVolumeReportItem, rAB_ID),
		(ASN_TAG_CLASS_CONTEXT | (0 << 2)),
		-1,	/* IMPLICIT tag at current level */
		&asn_DEF_RAB_ID,
		0,	/* Defer constraints checking to the member type */
		0,	/* No PER visible constraints */
		0,
		"rAB-ID"
		},
	{ ATF_POINTER, 2, offsetof(struct RAB_DataVolumeReportItem, dl_UnsuccessfullyTransmittedDataVolume),
		(ASN_TAG_CLASS_CONTEXT | (1 << 2)),
		-1,	/* IMPLICIT tag at current level */
		&asn_DEF_DataVolumeList,
		0,	/* Defer constraints checking to the member type */
		0,	/* No PER visible constraints */
		0,
		"dl-UnsuccessfullyTransmittedDataVolume"
		},
	{ ATF_POINTER, 1, offsetof(struct RAB_DataVolumeReportItem, iE_Extensions),
		(ASN_TAG_CLASS_CONTEXT | (2 << 2)),
		-1,	/* IMPLICIT tag at current level */
		&asn_DEF_ProtocolExtensionContainer_209P0,
		0,	/* Defer constraints checking to the member type */
		0,	/* No PER visible constraints */
		0,
		"iE-Extensions"
		},
};
static int asn_MAP_RAB_DataVolumeReportItem_oms_1[] = { 1, 2 };
static ber_tlv_tag_t asn_DEF_RAB_DataVolumeReportItem_tags_1[] = {
	(ASN_TAG_CLASS_UNIVERSAL | (16 << 2))
};
static asn_TYPE_tag2member_t asn_MAP_RAB_DataVolumeReportItem_tag2el_1[] = {
    { (ASN_TAG_CLASS_CONTEXT | (0 << 2)), 0, 0, 0 }, /* rAB-ID at 470 */
    { (ASN_TAG_CLASS_CONTEXT | (1 << 2)), 1, 0, 0 }, /* dl-UnsuccessfullyTransmittedDataVolume at 471 */
    { (ASN_TAG_CLASS_CONTEXT | (2 << 2)), 2, 0, 0 } /* iE-Extensions at 473 */
};
static asn_SEQUENCE_specifics_t asn_SPC_RAB_DataVolumeReportItem_specs_1 = {
	sizeof(struct RAB_DataVolumeReportItem),
	offsetof(struct RAB_DataVolumeReportItem, _asn_ctx),
	asn_MAP_RAB_DataVolumeReportItem_tag2el_1,
	3,	/* Count of tags in the map */
	asn_MAP_RAB_DataVolumeReportItem_oms_1,	/* Optional members */
	2, 0,	/* Root/Additions */
	2,	/* Start extensions */
	4	/* Stop extensions */
};
asn_TYPE_descriptor_t asn_DEF_RAB_DataVolumeReportItem = {
	"RAB-DataVolumeReportItem",
	"RAB-DataVolumeReportItem",
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
	asn_DEF_RAB_DataVolumeReportItem_tags_1,
	sizeof(asn_DEF_RAB_DataVolumeReportItem_tags_1)
		/sizeof(asn_DEF_RAB_DataVolumeReportItem_tags_1[0]), /* 1 */
	asn_DEF_RAB_DataVolumeReportItem_tags_1,	/* Same as above */
	sizeof(asn_DEF_RAB_DataVolumeReportItem_tags_1)
		/sizeof(asn_DEF_RAB_DataVolumeReportItem_tags_1[0]), /* 1 */
	0,	/* No PER visible constraints */
	asn_MBR_RAB_DataVolumeReportItem_1,
	3,	/* Elements count */
	&asn_SPC_RAB_DataVolumeReportItem_specs_1	/* Additional specs */
};

