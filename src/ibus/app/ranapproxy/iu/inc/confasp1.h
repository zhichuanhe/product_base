/****************************************************************************
** Description:
*****************************************************************************
** Copyright(C) 2005 Shabd Communications Pvt. Ltd. http://www.shabdcom.org
*****************************************************************************
** Contact:
** vkgupta@shabdcom.org
*****************************************************************************
** License :
**
** This library is free software; you can redistribute it and/or
** modify it under the terms of the GNU Lesser General Public
** License as published by the Free Software Foundation; either
** version 2.1 of the License, or (at your option) any later version.
**
** This library is distributed in the hope that it will be useful,
** but WITHOUT ANY WARRANTY; without even the implied warranty of
** MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
** Lesser General Public License for more details.
**
** You should have received a copy of the GNU Lesser General Public
** License along with this library; if not, write to the Free Software
** Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
*****************************************************************************/

#ifndef __CONFASP1_H__
#define __CONFASP1_H__

#include <m3ua_defines.h>
#include <m3ua_types.h>
#include <m3ua_api.h>
#include <m3ua_errno.h>
#ifdef __UDP__
#include <udp.h>
#else
#include <sctpusr.h>
#endif
#include <timer.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>


int um3_get_asp1_addr(char *argv[], udp_addr_t *paddr);

int um3_get_asp2_addr(char *argv[], udp_addr_t *paddr);

int um3_get_sgp1_addr(char *argv[], udp_addr_t *paddr);

int um3_transport_init(char *argv[]);

m3_s32 um3_m3ua_init(void);

m3_s32 um3_m3ua_nwapp(m3_u8   oprn, m3_u32 proc);

m3_s32 um3_m3ua_asp(m3_u8    oprn, m3_u32 proc);

m3_s32 um3_m3ua_as(m3_u8    oprn, m3_u32 proc);

m3_s32 um3_m3ua_r_asp(m3_u8    oprn, m3_u32 proc);

m3_s32 um3_m3ua_r_as(m3_u8    oprn, m3_u32 proc);

m3_s32 um3_m3ua_r_sgp(m3_u8    oprn);

m3_s32 um3_m3ua_sg(m3_u8    oprn);

m3_s32 um3_m3ua_conn(m3_u8 oprn, m3_u32 lsp, m3_u32 rsp, m3_u32 assoc);

m3_s32 um3_m3ua_conn_state(m3_u8 oprn, m3_conn_state_t state, m3_u32 lsp, m3_u32 rsp);

m3_s32 um3_m3ua_asp_state(m3_u8 oprn, m3_asp_state_t state, m3_u32 proc);

m3_s32 um3_m3ua_route(m3_u8    oprn, m3_u32 proc);

m3_s32 um3_m3ua_r_asplock(m3_u8    oprn);

m3_s32 um3_m3ua_user(m3_u8    oprn, m3_u32 proc);

m3_s32 um3_m3ua_txr(m3_u16 user, m3_rt_lbl_t *prtlbl);

m3_s32 um3_m3ua_audit(m3_u8 user);

m3_s32 um3_m3ua_timer(m3_u8 oprn);

m3_s32 um3_m3ua_heartbeat(m3_u32 lsp, m3_u32 rsp, m3_u8 oprn);

void um3_process_mgmt_ntfy(m3ua_mgmt_ntfy_t *pntfy);

void um3_process_user_ntfy(m3ua_user_ntfy_t *pntfy);

void um3_m3ua_trace_map(void);

void asp_set_sctp_cn_mode(unsigned char conn_mode, unsigned char isps);

void as_set_routing_context(m3_u32 rtctx, m3_u8 asflag);

void as_set_traffic_mode(m3_u32 trfmode, m3_u8 asflag);

void as_set_network_apperance(m3_u32 nwapp, m3_u8 asflag);

m3_u32 as_get_traffic_mode(m3_u8 asflag);

m3_u32 as_get_routing_context(m3_u8 asflag);

void m3ua_set_gNi(int ni);

int um3_server_start(m3_u32 proc);

int um3_server_stop(m3_u32 proc);

int um3_transport_init_arg_v2(m3_u32 proc);

int um3_get_m3ua_conn_state(m3_u32 proc, m3_u8 *conn_state);

int um3_get_m3ua_asp_state(m3_u32 proc, m3_u8 *asp_state);

int um3_get_iu_interface_status(m3_u32 proc, m3_u8 *m3_asp_state, m3_u8 *m3_conn_state, int *sctp_state);

/* book add for sigtran,2011-11-09 */
m3_s32 um3_m3ua_route_sigtran(m3_u8    oprn);
m3_s32 um3_m3ua_asp_sigtran(m3_u8    oprn);
m3_s32 um3_m3ua_as_sigtran(m3_u8    oprn);
m3_s32 um3_m3ua_user_sigtran(m3_u8    oprn);

int read_file_info(char *FILENAME,char *buff);
int parse_int_ID(char* str,unsigned int* ID);
#endif

