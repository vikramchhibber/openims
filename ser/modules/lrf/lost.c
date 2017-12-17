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
/**
 * Location Routing Function - SER module interface
 * 
 * Scope:
 *	interface with the lost_client library 
 * 
 */
 

#include "mod.h"

#include "../../locking.h"
#include "../../modules/tm/tm_load.h"
#include <lost/client.h>
#include <lost/parsing.h>
#include <lost/pidf_loc.h>

#include "lost.h"
#include "multipart_parse.h"
#include "sip.h"
#include "user_data.h"

extern lost_server_info lost_server;	
extern int using_lost_srv;

static str service_hdr_name = {"Service",7};

int snd_rcv_LoST(str lost_req, str * result){

	CURL* conn;

	//initiate the CURL handle for the HTTP connenction
	//"http://lost.open-ims.test/lost/LoSTServlet";
	LOG(L_DBG, "DBG:"M_NAME":snd_rcv_LoST: lost server: %.*s port %i\n",
			lost_server.host.len, lost_server.host.s,
			lost_server.port);
	conn = lost_http_conn(lost_server.host.s, lost_server.port, result);
	if(!conn){
	
		LOG(L_ERR, "ERR:"M_NAME": snd_rcv_LoST: could not connect to the LoST server [%s]:%i\n",
				lost_server.host.s, lost_server.port);
		goto error;
	}
	
	//send the POST request including the xml body content for LoST
	LOG(L_DBG, "DBG:"M_NAME":snd_rcv_LoST: trying to send %.*s to the lost server\n", 
			lost_req.len, lost_req.s);

	if(send_POST_data(conn, lost_req)){
		LOG(L_ERR, "ERR:"M_NAME": snd_rcv_LoST: could not send data %.*s the LoST server [%s]:%i\n",
		   lost_req.len, lost_req.s, lost_server.host.s, lost_server.port);
		goto error;
	}

	LOG(L_DBG, "DBG:"M_NAME": snd_rcv_LoST: the response from the LoST server is %.*s\n",
			result->len, result->s);
	//disconnect the server
	lost_http_disconn(conn);

	return 0;
error:
	return 1;	
}

#define NO_LOC_FOUND		-3
#define ERR_PARSE_LOC		-4
#define	NO_SUPP_FMT_LOC		-5
#define OK_LOC			1

int LRF_get_location(struct sip_msg* msg, loc_fmt *crt_loc_fmt, xmlNode** loc);

/* Get the PSAP URI by interrogating the LoST server
 * @param d - the user data for which the PSAP URI is searched
 * @returns a null string if error, otherwise the PSAP URI
 */
str get_psap_by_LoST(user_d * d){

	str reason, psap_uri = {NULL, 0}, result = {NULL, 0}, lost_req = {NULL, 0};
	lost_resp_type resp_type;
	xmlNode* location = NULL, *root= NULL;
	loc_fmt d_loc_fmt;
	struct sip_uri puri;
	expire_type exp_type;
       	time_t exp_timestamp;

	location = d->loc;
	d_loc_fmt = d->l_fmt;
	
	char * service_val = pkg_malloc((d->service.len+1)*sizeof(char));
	if (!service_val){
		LOG(L_ERR,"ERR:"M_NAME":get_psap_by_LoST: Error allocating %d bytes\n",d->service.len+1);
		goto end;
	}
	
	memcpy(service_val,d->service.s,d->service.len*sizeof(char));
	service_val[d->service.len] = '\0';
	
	if(create_lost_req(location, service_val, d_loc_fmt, &lost_req)){
	
		LOG(L_ERR, "ERR:"M_NAME":get_psap_by_LoST:could not create the LoST request\n");
		goto end;
	}
	
	if(snd_rcv_LoST(lost_req, &result)){
		LOG(L_ERR, "ERR:"M_NAME":get_psap_by_LoST:could not send the LoST request, setting the default PSAP URI\n");
		goto end;
	}
	
	//verify what kind of message we have received
	root = get_LoST_resp_type(result, &resp_type, &reason);
	if(resp_type != LOST_OK){
		
		LOG(L_ERR, "ERR:"M_NAME":get_psap_by_LoST: LoST response type is not OK\n");
		if(reason.s != NULL)
			LOG(L_DBG, "DBG:"M_NAME": get_psap_by_LoST:reason: %s\n", reason.s);
		
		goto end;
	}

	//get the PSAP URI
	psap_uri = get_mapped_psap(root, &exp_type, &exp_timestamp, &puri);
	if(!psap_uri.s || !psap_uri.len){
		LOG(L_ERR, "ERR:"M_NAME": get_psap_by_LoST:LoST response had no valid SIP uri\n");
		goto end;
	}

	LOG(L_DBG, "DBG:"M_NAME":get_psap_by_LoST:found psap uri is %.*s\n", psap_uri.len, psap_uri.s);
	
end:
	if(result.s)
		pkg_free(result.s);
	if(lost_req.s)
		pkg_free(lost_req.s);
	if(root)
		xmlFreeDoc(root->doc);
	return psap_uri;
}

/* Find the appropriate psap uri that the request should be forwarded to
 * could be called after a LOCSIP NOTIFY is received
 * @param msg - the sip request from the ECSCF node
 * @param str1 
 * @param str2 - not used
 * @returns #CSCF_RETURN_TRUE if ok, #CSCF_RETURN_FALSE if not or #CSCF_RETURN_BREAK on error 
 */
int LRF_get_psap(struct sip_msg* msg, char* str1, char* str2){

	str psap_uri = {NULL, 0};
	str user_uri;
	user_d * d=NULL;
	str service;
	
	/* check if we received what we should */
	if (msg->first_line.type!=SIP_REQUEST) {
		LOG(L_ERR,"ERR:"M_NAME":LRF_get_psap: The message is not a request\n");
		return CSCF_RETURN_ERROR;
	}

	if (msg->first_line.u.request.method.len!=7||
		memcmp(msg->first_line.u.request.method.s,"OPTIONS",7)!=0){
		LOG(L_WARN,"WARN:"M_NAME":LRF_get_psap: The method is not an OPTIONS, trying to replace the message\n");

		msg = cscf_get_request_from_reply(NULL);
		if(! msg || msg->first_line.type!=SIP_REQUEST || msg->first_line.u.request.method.len!=7||
			memcmp(msg->first_line.u.request.method.s,"OPTIONS",7)!=0){
					
			LOG(L_ERR,"BUG:"M_NAME":LRF_get_psap: The new message is not an OPTIONS request either\n");
			return CSCF_RETURN_ERROR;
		}
	}


	LOG(L_INFO, "INFO:"M_NAME":LRF_get_psap \n");
	service = cscf_get_headers_content(msg , service_hdr_name);
	if(!service.len || !service.s){
		LOG(L_ERR, "ERR:"M_NAME":LRF_get_psap: could not find the service header in the OPTIONS, or could not be parsed\n");
		return CSCF_RETURN_FALSE;
	}
	
	str callid = cscf_get_call_id(msg, NULL);
	if(!callid.s || !callid.len){
		LOG(L_ERR, "ERR:"M_NAME":LRF_get_psap: could not find the callid header in the OPTIONS request\n");
		return CSCF_RETURN_FALSE;
	}

	user_uri = msg->first_line.u.request.uri;

	d = get_user_data(user_uri, service, callid);
	if(!d) {
		LOG(L_ERR, "ERR:"M_NAME":LRF_get_psap: could not found user data for uri %.*s and service %.*s\n",
				user_uri.len, user_uri.s, service.len, service.s);
		return CSCF_RETURN_FALSE;
	}

	//if not using a LoST server
	if(using_lost_srv){
		LOG(L_DBG, "DBG:"M_NAME":LRF_get_psap: LoST server enabled");
		psap_uri = get_psap_by_LoST(d);
	}else{
		LOG(L_DBG, "DBG:"M_NAME":LRF_get_psap: LoST server disabled");
	}

	if(!psap_uri.s || !psap_uri.len){
		LOG(L_DBG, "DBG:"M_NAME":LRF_get_psap: no PSAP available");
		goto error;
	}
	LOG(L_DBG, "DBG:"M_NAME":LRF_get_psap:psap uri is %.*s\n", psap_uri.len, psap_uri.s);

	STR_SHM_DUP(d->psap_uri, psap_uri, "LRF_get_psap");
	
	lrf_unlock(d->hash);
	return CSCF_RETURN_TRUE;

error:	
out_of_memory:
	if(d)
		lrf_unlock(d->hash);
	return CSCF_RETURN_FALSE;
}

/* Parse the location information by value from the OPTIONS request
 * store the location information in the user data
 * @param msg - the SIP request
 * @param str1 -not used
 * @param str2 -not used
 * @returns #CSCF_RETURN_TRUE if ok, #CSCF_RETURN_FALSE if not or #CSCF_RETURN_BREAK on error 
 */
int LRF_has_loc(struct sip_msg* msg, char * str1, char* str2){

	str pidf_body={NULL, 0};

	str user_uri;
	user_d * d= NULL;
	str service;

	service = cscf_get_headers_content(msg , service_hdr_name);
	if(!service.len || !service.s){
		LOG(L_ERR, "ERR:"M_NAME":LRF_has_loc: could not find the service header in the OPTIONS, or could not be parsed\n");
		return CSCF_RETURN_FALSE;
	}

	str callid = cscf_get_call_id(msg, NULL);
	if(!callid.s || !callid.len){
		LOG(L_ERR, "ERR:"M_NAME":LRF_has_loc: could not find the callid header in the OPTIONS request\n");
		return CSCF_RETURN_FALSE;
	}

	user_uri = msg->first_line.u.request.uri;
	d = get_user_data(user_uri, service, callid);
	if(!d) {
		LOG(L_ERR, "ERR:"M_NAME":LRF_has_loc: could not found user data for uri %.*s and service %.*s\n",
				user_uri.len, user_uri.s, service.len, service.s);
		return CSCF_RETURN_FALSE;
	}

	int ret = get_pidf_lo_body(msg, &pidf_body);
	if(ret != 0){
		if(ret==-1)
			LOG(L_ERR, "ERR:"M_NAME":LRF_has_loc:could not get the pidf+xml body\n");
		else
			LOG(L_DBG, "DBG:"M_NAME":LRF_has_loc:could not get the pidf+xml body\n");
	
		lrf_unlock(d->hash);
		return NO_LOC_FOUND;
	}

	d->loc_str.s = pidf_body.s;
	d->loc_str.len = pidf_body.len;
	lrf_unlock(d->hash);
	
	return CSCF_RETURN_TRUE;
}

xmlNode * verify_pidf_xml_body(str pidf_body, loc_fmt * crt_loc_fmt){
	
	xmlNode * presence, * loc;
	int ret;

	if(!(presence = xml_parse_string(pidf_body))){
		LOG(L_ERR, "ERR:"M_NAME": verify_pidf_xml_body:invalid xml content\n");
		goto error;
	}

	//print_element_names(presence);

	if(!(loc = has_loc_info(&ret, presence, crt_loc_fmt))){
	
		LOG(L_ERR, "ERR:"M_NAME":verify_pidf_xml_body:could not find a valid location element\n");
		goto error;
	}

	if((*crt_loc_fmt == GEO_SHAPE_LOC) || (*crt_loc_fmt == NEW_CIV_LOC) ||
			(*crt_loc_fmt == OLD_CIV_LOC) || (*crt_loc_fmt == GEO_COORD_LOC)){
	
		LOG(L_DBG, "DBG:"M_NAME":verify_pidf_xml_body:LoST supported format, setting the location\n");

	}else if(*crt_loc_fmt == ERR_LOC){
		LOG(L_ERR, "ERR:"M_NAME":verify_pidf_xml_body:error while parsing the location information\n");
		goto error;
	}else{
		LOG(L_DBG, "DBG:"M_NAME":verify_pidf_xml_body:no LoST supported format\n");
		goto error;
	}

	return loc;

error:
	if(presence)
		xmlFreeDoc(presence->doc);
	return NULL;
}

/* check that the location information from the user data is a valid PIDF-LO
 * @param msg - OPTIONS request from the E-CSCF
 * @param str1 - not used
 * @param str2 - not used
 */
int LRF_parse_user_loc(struct sip_msg * msg, char* str1, char* str2){

	xmlNode *loc = NULL;
	loc_fmt crt_loc_fmt;
	str pidf_body={NULL, 0};

	str user_uri;
	user_d * d=NULL;
	str service;

	service = cscf_get_headers_content(msg , service_hdr_name);
	if(!service.len || !service.s){
		LOG(L_ERR, "ERR:"M_NAME":LRF_save_user_loc: could not find the service header in the OPTIONS, or could not be parsed\n");
		return CSCF_RETURN_FALSE;
	}

	str callid = cscf_get_call_id(msg, NULL);
	if(!callid.s || !callid.len){
		LOG(L_ERR, "ERR:"M_NAME":LRF_save_user_loc: could not find the callid header in the OPTIONS request\n");
		return CSCF_RETURN_FALSE;
	}

	user_uri = msg->first_line.u.request.uri;
	d = get_user_data(user_uri, service, callid);
	if(!d) {
		LOG(L_ERR, "ERR:"M_NAME":LRF_save_user_loc: could not found user data for uri %.*s and service %.*s\n",
				user_uri.len, user_uri.s, service.len, service.s);
		return CSCF_RETURN_FALSE;
	}

	pidf_body.s = d->loc_str.s;
	pidf_body.len = d->loc_str.len;
	if (!(loc = verify_pidf_xml_body(pidf_body, &crt_loc_fmt))){
		lrf_unlock(d->hash);
		return CSCF_RETURN_FALSE;
	}	
	//	LOG(L_DBG, "DBG:"M_NAME":LRF_save_user_loc:printing the location useful tree\n");
//	print_element_names(loc);
	d->loc = loc;
	d->l_fmt = crt_loc_fmt;
	lrf_unlock(d->hash);
	return OK_LOC;
}



