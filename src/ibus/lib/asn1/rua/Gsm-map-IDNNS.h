/*
 * Generated by asn1c-0.9.23 (http://lionet.info/asn1c)
 * From ASN.1 module "RUA-IEs"
 * 	found in "RUA-IEs.asn"
 */

#ifndef	_Gsm_map_IDNNS_H_
#define	_Gsm_map_IDNNS_H_


#include <asn_application.h>

/* Including external dependencies */
#include <BOOLEAN.h>
#include "RoutingParameter.h"
#include <constr_SEQUENCE.h>
#include <constr_CHOICE.h>

#ifdef __cplusplus
extern "C" {
#endif

/* Dependencies */
typedef enum routingbasis_PR {
	routingbasis_PR_NOTHING,	/* No components present */
	routingbasis_PR_localPTMSI,
	routingbasis_PR_tMSIofsamePLMN,
	routingbasis_PR_tMSIofdifferentPLMN,
	routingbasis_PR_iMSIresponsetopaging,
	routingbasis_PR_iMSIcauseUEinitiatedEvent,
	routingbasis_PR_iMEI,
	routingbasis_PR_spare2,
	routingbasis_PR_spare1
} routingbasis_PR;

/* Gsm-map-IDNNS */
typedef struct Gsm_map_IDNNS {
	struct routingbasis {
		routingbasis_PR present;
		union Gsm_map_IDNNS__routingbasis_u {
			struct localPTMSI {
				RoutingParameter_t	 routingparameter;
				
				/* Context for parsing across buffer boundaries */
				asn_struct_ctx_t _asn_ctx;
			} localPTMSI;
			struct tMSIofsamePLMN {
				RoutingParameter_t	 routingparameter;
				
				/* Context for parsing across buffer boundaries */
				asn_struct_ctx_t _asn_ctx;
			} tMSIofsamePLMN;
			struct tMSIofdifferentPLMN {
				RoutingParameter_t	 routingparameter;
				
				/* Context for parsing across buffer boundaries */
				asn_struct_ctx_t _asn_ctx;
			} tMSIofdifferentPLMN;
			struct iMSIresponsetopaging {
				RoutingParameter_t	 routingparameter;
				
				/* Context for parsing across buffer boundaries */
				asn_struct_ctx_t _asn_ctx;
			} iMSIresponsetopaging;
			struct iMSIcauseUEinitiatedEvent {
				RoutingParameter_t	 routingparameter;
				
				/* Context for parsing across buffer boundaries */
				asn_struct_ctx_t _asn_ctx;
			} iMSIcauseUEinitiatedEvent;
			struct iMEI {
				RoutingParameter_t	 routingparameter;
				
				/* Context for parsing across buffer boundaries */
				asn_struct_ctx_t _asn_ctx;
			} iMEI;
			struct spare2 {
				RoutingParameter_t	 routingparameter;
				
				/* Context for parsing across buffer boundaries */
				asn_struct_ctx_t _asn_ctx;
			} spare2;
			struct spare1 {
				RoutingParameter_t	 routingparameter;
				
				/* Context for parsing across buffer boundaries */
				asn_struct_ctx_t _asn_ctx;
			} spare1;
		} choice;
		
		/* Context for parsing across buffer boundaries */
		asn_struct_ctx_t _asn_ctx;
	} routingbasis;
	BOOLEAN_t	 dummy;
	
	/* Context for parsing across buffer boundaries */
	asn_struct_ctx_t _asn_ctx;
} Gsm_map_IDNNS_t;

/* Implementation */
extern asn_TYPE_descriptor_t asn_DEF_Gsm_map_IDNNS;

#ifdef __cplusplus
}
#endif

#endif	/* _Gsm_map_IDNNS_H_ */
#include <asn_internal.h>
