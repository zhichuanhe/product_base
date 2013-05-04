/*
 * Generated by asn1c-0.9.23 (http://lionet.info/asn1c)
 * From ASN.1 module "RANAP-IEs"
 * 	found in "RANAP-IEs.asn"
 */

#include "InformationRequested.h"

static asn_per_constraints_t asn_PER_type_InformationRequested_constr_1 = {
	{ APC_CONSTRAINED | APC_EXTENSIBLE,  1,  1,  0,  1 }	/* (0..1,...) */,
	{ APC_UNCONSTRAINED,	-1, -1,  0,  0 },
	0, 0	/* No PER value map */
};
static asn_TYPE_member_t asn_MBR_InformationRequested_1[] = {
	{ ATF_NOFLAGS, 0, offsetof(struct InformationRequested, choice.requestedMBMSIPMulticastAddressandAPNRequest),
		(ASN_TAG_CLASS_CONTEXT | (0 << 2)),
		-1,	/* IMPLICIT tag at current level */
		&asn_DEF_RequestedMBMSIPMulticastAddressandAPNRequest,
		0,	/* Defer constraints checking to the member type */
		0,	/* No PER visible constraints */
		0,
		"requestedMBMSIPMulticastAddressandAPNRequest"
		},
	{ ATF_NOFLAGS, 0, offsetof(struct InformationRequested, choice.requestedMulticastServiceList),
		(ASN_TAG_CLASS_CONTEXT | (1 << 2)),
		-1,	/* IMPLICIT tag at current level */
		&asn_DEF_RequestedMulticastServiceList,
		0,	/* Defer constraints checking to the member type */
		0,	/* No PER visible constraints */
		0,
		"requestedMulticastServiceList"
		},
};
static asn_TYPE_tag2member_t asn_MAP_InformationRequested_tag2el_1[] = {
    { (ASN_TAG_CLASS_CONTEXT | (0 << 2)), 0, 0, 0 }, /* requestedMBMSIPMulticastAddressandAPNRequest at 958 */
    { (ASN_TAG_CLASS_CONTEXT | (1 << 2)), 1, 0, 0 } /* requestedMulticastServiceList at 959 */
};
static asn_CHOICE_specifics_t asn_SPC_InformationRequested_specs_1 = {
	sizeof(struct InformationRequested),
	offsetof(struct InformationRequested, _asn_ctx),
	offsetof(struct InformationRequested, present),
	sizeof(((struct InformationRequested *)0)->present),
	asn_MAP_InformationRequested_tag2el_1,
	2,	/* Count of tags in the map */
	0,
	2	/* Extensions start */
};
asn_TYPE_descriptor_t asn_DEF_InformationRequested = {
	"InformationRequested",
	"InformationRequested",
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
	&asn_PER_type_InformationRequested_constr_1,
	asn_MBR_InformationRequested_1,
	2,	/* Elements count */
	&asn_SPC_InformationRequested_specs_1	/* Additional specs */
};

