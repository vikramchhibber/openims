/*
 * Copyright (C) 2008-2009 FhG Fokus
 *
 * This file is part of Open IMS Core - an open source IMS CSCFs & HSS
 * implementation
 *
 * Open IMS Core is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * For a license to use the Open IMS Core software under conditions
 * other than those described here, or to purchase support for this
 * software, please contact Fraunhofer FOKUS by e-mail at the following
 * addresses:
 *     info@open-ims.org
 *
 * Open IMS Core is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 * 
 * It has to be noted that this Open Source IMS Core System is not 
 * intended to become or act as a product in a commercial context! Its 
 * sole purpose is to provide an IMS core reference implementation for 
 * IMS technology testing and IMS application prototyping for research 
 * purposes, typically performed in IMS test-beds.
 * 
 * Users of the Open Source IMS Core System have to be aware that IMS
 * technology may be subject of patents and licence terms, as being 
 * specified within the various IMS-related IETF, ITU-T, ETSI, and 3GPP
 * standards. Thus all Open IMS Core users have to take notice of this 
 * fact and have to agree to check out carefully before installing, 
 * using and extending the Open Source IMS Core System, if related 
 * patents and licences may become applicable to the intended usage 
 * context.  
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
 * 
 * author Ancuta Onofrei, 
 * 	email andreea dot ancuta dot onofrei -at- fokus dot fraunhofer dot de
 */

/* route.c deals with routing of the E-CSCF module
 *
 */

#include "../../parser/msg_parser.h"
#include "../../dset.h"
#include "dlg_state.h"
#include "sip.h"

static str route_s={"Route: <",8};
static str route_e={">\r\n",3};
extern str ecscf_record_route_mo;

static str esqk_hdr_s = {"Esqk: ",6};
static str esqk_hdr_e = {"\r\n",2};

static str to_hdr_s = {"To: <",5};
static str to_hdr_e={">\r\n",3};

int E_add_esqk(struct sip_msg * msg, str esqk){

	str  y = {0,0};
	if(!esqk.len || !esqk.s){
		goto ret_false;
	}

	y.s = pkg_malloc(esqk_hdr_s.len + esqk.len +esqk_hdr_e.len);
	if (!y.s){
		LOG(L_ERR, "ERR"M_NAME":E_fwd_to_psap: Error allocating %d bytes\n",
			esqk.len);
		goto ret_false;
	}

	y.len = 0;
	STR_APPEND(y, esqk_hdr_s);
	STR_APPEND(y, esqk);
	STR_APPEND(y, esqk_hdr_e);	

	if (!cscf_add_header_first(msg,&y,HDR_ESQK_T)){
		goto ret_false;
	}

	return CSCF_RETURN_TRUE;

ret_false:
	if(y.s){
		pkg_free(y.s);
		y.s = NULL;
	}
	return CSCF_RETURN_FALSE;
}

/* Deletes all the Route headers, Adds a Route towards the PSAP URI and sets the RURI to the PSAP URI, adds the ESQK header
 * @param d  - the dialog that the INVITE request created
 */
int E_fwd_to_psap(struct sip_msg * msg, str psap_uri){
	
	str x = {0,0},to = {0,0};

	if(!psap_uri.len || !psap_uri.s){
		LOG(L_ERR, "ERR:"M_NAME":E_fwd_to_psap: invalid psap_uri parameter\n");
		goto ret_false;
	}

	
	if(cscf_del_all_headers(msg, HDR_TO_T)==0){
		LOG(L_ERR, "ERR:"M_NAME":E_fwd_to_psap:could not delete the existing From headers\n");
		goto ret_false;
	}

	x.s = pkg_malloc(route_s.len + psap_uri.len +route_e.len);
	if (!x.s){
		LOG(L_ERR, "ERR"M_NAME":E_fwd_to_psap: Error allocating %d bytes\n",
			psap_uri.len);
		x.len=0;
		goto ret_false;

	}

	to.s = pkg_malloc(to_hdr_s.len + psap_uri.len +to_hdr_e.len);
	if (!to.s){
		LOG(L_ERR, "ERR"M_NAME":E_fwd_to_psap: Error allocating %d bytes\n",
			(to_hdr_s.len + psap_uri.len +to_hdr_e.len));
		goto ret_false;
	}

	LOG(L_DBG, "DBG:"M_NAME":E_fwd_to_psap:psap uri is %.*s\n",
			psap_uri.len, psap_uri.s);

	if(rewrite_uri(msg, &psap_uri) < 0) {
		LOG(L_ERR,"ERR:"M_NAME":E_fwd_to_psap: Error rewritting uri with <%.*s>\n",
			psap_uri.len, psap_uri.s);
		goto ret_false;	
	} 

	if(set_dst_uri(msg, &psap_uri)){
	
		LOG(L_ERR, "ERR:"M_NAME":E_fwd_to_psap: Could not set the destination uri %.*s\n",
				psap_uri.len, psap_uri.s);
		goto ret_false;
	}

	x.len = 0;
	STR_APPEND(x,route_s);
	STR_APPEND(x, psap_uri);
	STR_APPEND(x,route_e);	

	if(cscf_del_all_headers(msg, HDR_ROUTE_T)==0){
		LOG(L_ERR, "ERR:"M_NAME":E_fwd_to_psap:could not delete all the existing route headers\n");
		goto ret_false;
	}

	if (!cscf_add_header_first(msg,&x,HDR_ROUTE_T)){
		goto ret_false;
	}
	
	to.len = 0;
	STR_APPEND(to, to_hdr_s);
	STR_APPEND(to, psap_uri);
	STR_APPEND(to, to_hdr_e);	

	if (!cscf_add_header_first(msg,&to,HDR_TO_T)){
		goto ret_false;
	}

	return CSCF_RETURN_TRUE;

ret_false:
	LOG(L_ERR, "ERR:"M_NAME":E_fwd_to_psap: error while preparing to forward to the PSAP\n");
	if(x.s){
		pkg_free(x.s);
		x.s = NULL;
	}
	if(to.s){
		pkg_free(to.s);
		to.s = NULL;
	}
	if (msg->new_uri.s) {
                pkg_free(msg->new_uri.s);
		msg->new_uri.s = NULL;
        }
	if (msg->dst_uri.s) {
		pkg_free(msg->dst_uri.s);
		msg->dst_uri.s = NULL;
	}

	return CSCF_RETURN_FALSE;

}

/* Adds a Record-Route header with the URI of the mobile orig of the ECSCF, 
 * for example sip:mo@ecscf.open-ims.test
 * @param msg  - the INVITE request to be added to
 * @param str1 - not used
 * @param str2 - not used
 */
int E_add_record_route(struct sip_msg* msg, char* str1, char* str2){

	str rr={0,0};
	
	STR_PKG_DUP(rr,ecscf_record_route_mo,"pkg");
	
	if (cscf_add_header_first(msg,&rr,HDR_RECORDROUTE_T)) 
		return CSCF_RETURN_TRUE;
	else{
		if (rr.s) {pkg_free(rr.s);
			rr.s = NULL;
		}
		return CSCF_RETURN_BREAK;
	}

	return CSCF_RETURN_TRUE;

out_of_memory:
	LOG(L_ERR, "ERR:"M_NAME":E_add_record_route: out of memory\n");
	return CSCF_RETURN_FALSE;
}

