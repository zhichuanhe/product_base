/*******************************************************************************
Copyright (C) Autelan Technology


This software file is owned and distributed by Autelan Technology 
********************************************************************************


THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND 
ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED 
WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE 
DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR CONTRIBUTORS BE LIABLE FOR 
ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES 
(INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; 
LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON 
ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT 
(INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS 
SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
********************************************************************************
* eap.h
*
*
* CREATOR:
* autelan.software.WirelessControl. team
*
* DESCRIPTION:
* asd module
*
*
*******************************************************************************/
#ifndef EAP_H
#define EAP_H

#include "defs.h"
#include "ASDEAPMethod/eap_defs.h"
#include "ASDEAPMethod/eap_methods.h"
#include "wpabuf.h"

struct eap_sm;

#define EAP_MAX_METHODS 8

#define EAP_TTLS_AUTH_PAP 1
#define EAP_TTLS_AUTH_CHAP 2
#define EAP_TTLS_AUTH_MSCHAP 4
#define EAP_TTLS_AUTH_MSCHAPV2 8

struct eap_user {
	struct {
		int vendor;
		u32 method;
	} methods[EAP_MAX_METHODS];
	u8 *password;
	size_t password_len;
	int password_hash; /* whether password is hashed with
			    * nt_password_hash() */
	int phase2;
	int force_version;
	int ttls_auth; /* bitfield of
			* EAP_TTLS_AUTH_{PAP,CHAP,MSCHAP,MSCHAPV2} */
};

struct eap_eapol_interface {
	/* Lower layer to full authenticator variables */
	Boolean eapResp; /* shared with EAPOL Backend Authentication */
	struct wpabuf *eapRespData;
	Boolean portEnabled;
	int retransWhile;
	Boolean eapRestart; /* shared with EAPOL Authenticator PAE */
	int eapSRTT;
	int eapRTTVAR;

	/* Full authenticator to lower layer variables */
	Boolean eapReq; /* shared with EAPOL Backend Authentication */
	Boolean eapNoReq; /* shared with EAPOL Backend Authentication */
	Boolean eapSuccess;
	Boolean eapFail;
	Boolean eapTimeout;
	struct wpabuf *eapReqData;
	u8 *eapKeyData;
	struct wpabuf *aliveData;
	size_t eapKeyDataLen;
	Boolean eapKeyAvailable; /* called keyAvailable in IEEE 802.1X-2004 */

	/* AAA interface to full authenticator variables */
	Boolean aaaEapReq;
	Boolean aaaEapNoReq;
	Boolean aaaSuccess;
	Boolean aaaFail;
	struct wpabuf *aaaEapReqData;
	u8 *aaaEapKeyData;
	size_t aaaEapKeyDataLen;
	Boolean aaaEapKeyAvailable;
	int aaaMethodTimeout;

	/* Full authenticator to AAA interface variables */
	Boolean aaaEapResp;
	struct wpabuf *aaaEapRespData;
	/* aaaIdentity -> eap_get_identity() */
	Boolean aaaTimeout;
};

struct eapol_callbacks {
	int (*get_eap_user)(void *ctx, const u8 *identity, size_t identity_len,
			    int phase2, struct eap_user *user);
	const char * (*get_eap_req_id_text)(void *ctx, size_t *len);
};

struct eap_config {
	void *ssl_ctx;
	void *eap_sim_db_priv;
	Boolean backend_auth;
	int eap_server;
	u8 *pac_opaque_encr_key;
	char *eap_fast_a_id;
	int eap_sim_aka_result_ind;
};


struct eap_sm * eap_server_sm_init(void *eapol_ctx,
				   struct eapol_callbacks *eapol_cb,
				   struct eap_config *eap_conf);
void eap_server_sm_deinit(struct eap_sm *sm);
int eap_server_sm_step(struct eap_sm *sm);
void eap_sm_notify_cached(struct eap_sm *sm);
void eap_sm_pending_cb(struct eap_sm *sm);
int eap_sm_method_pending(struct eap_sm *sm);
const u8 * eap_get_identity(struct eap_sm *sm, size_t *len);
struct eap_eapol_interface * eap_get_interface(struct eap_sm *sm);

#endif /* EAP_H */
