/*
 * $Id: emerg.c 997 2010-09-21 14:22:22Z aon $
 *  
 * Copyright (C) 2004-2009 FhG Fokus
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
 */
 
/**
 * \file
 * 
 * Proxy-CSCF -Emergency Related Operations
 * 
 * 
 *  \author Ancuta Onofrei	andreea dot ancuta dot onofrei -at- fokus dot fraunhofer dot de
 */
 
#include <time.h>
 
#include "../../data_lump.h"
#include "../../data_lump_rpl.h"
#include "../../mem/mem.h"
#include "../../locking.h"
#include "../../parser/parse_from.h"
#include "../../modules/tm/tm_load.h"
#include "../../dset.h"
#include "mod.h"
#include "sip.h"
#include "emerg.h"
#include "registrar.h"
#include "registrar_subscribe.h"
#include "ims_pm_pcscf.h"
#include "e2.h"
#include "e2_avp.h"
#include "dlg_state.h"

extern struct tm_binds tmb;            				/**< Structure with pointers to tm funcs 			*/
extern char * ecscf_uri;
extern int emerg_support;
extern str ecscf_uri_str;
extern int anonym_em_call_support;
extern str pcscf_path_orig_em_uri_str;

/*global variables*/
str ecscf_uri_str;
xmlDocPtr reply_380_doc= NULL;
xmlNode * alt_serv_node = NULL;
xmlNode * reason_alt_serv_node = NULL;
xmlNode * action_alt_serv_node = NULL;

int init_em_alt_serv_body();


int init_emergency_cntxt(){
	
	/*init the XML library*/
	xmlInitParser();

	if(emerg_support){
		ecscf_uri_str.s = ecscf_uri;
		ecscf_uri_str.len = strlen(ecscf_uri);
		LOG(L_INFO, "INFO"M_NAME"mod_init:E-CSCF uri is %.*s\n",
				ecscf_uri_str.len, ecscf_uri_str.s);
		if(store_em_numbers())
			return -1;
	}
	/*init the xml doc*/
	return init_em_alt_serv_body();
}

void clean_emergency_cntxt(){

	if(emerg_support)
		clean_em_numbers();

}

/* Contructing the rough XML body for the 380 Alternative Service reply
 * when used the value of the node "reason" can be set, using xmlNodeSetContent 
 * and the "action_alt_serv" node can be temporarily removed if the request did not contain an emergency service URN,
 * using xmlUnlinkNode and in the end: AddChild	
 */
int init_em_alt_serv_body(){

   	xmlNodePtr em_alt_serv_node, type_node;

	/* creating the xml doc*/
   	reply_380_doc= xmlNewDoc(BAD_CAST "1.0");
   	if(reply_380_doc== NULL){

   		LOG(L_ERR, "ERR:"M_NAME":init_em_alt_serv_body: when creating new xml doc\n");
   		goto error;
   	}
   	
	em_alt_serv_node = xmlNewNode(NULL, BAD_CAST IMS_3GPP_XML_NODE);
   	if(em_alt_serv_node==0){

		LOG(L_ERR, "ERR:"M_NAME":init_em_alt_serv_body: when adding new node %s\n", IMS_3GPP_XML_NODE);
   		goto error;
   	}
   	xmlDocSetRootElement(reply_380_doc, em_alt_serv_node);

	alt_serv_node = xmlNewChild(em_alt_serv_node, NULL, BAD_CAST ALTERN_SERV_XML_NODE, NULL);
	if(!alt_serv_node){
		LOG(L_ERR, "ERR:"M_NAME":init_em_alt_serv_body: when adding new node %s\n", ALTERN_SERV_XML_NODE);
		goto error;
	}

	type_node = xmlNewChild(alt_serv_node, NULL, BAD_CAST TYPE_XML_NODE, BAD_CAST ALT_SERV_TYPE_VAL);
	if(!type_node){
		LOG(L_ERR, "ERR:"M_NAME":init_em_alt_serv_body: when adding new node %s\n", TYPE_XML_NODE);
		goto error;
	}

	reason_alt_serv_node = xmlNewChild(alt_serv_node, NULL, BAD_CAST REASON_XML_NODE, BAD_CAST "");
	if(!reason_alt_serv_node){
		LOG(L_ERR, "ERR:"M_NAME":init_em_alt_serv_body: when adding new node %s\n", REASON_XML_NODE);
		goto error;
	}

	action_alt_serv_node = xmlNewChild(alt_serv_node, NULL, BAD_CAST ACTION_XML_NODE, BAD_CAST ALT_SERV_ACTION_VAL);
	if(!action_alt_serv_node){
		LOG(L_ERR, "ERR:"M_NAME":init_em_alt_serv_body: when adding new node %s\n", ACTION_XML_NODE);
		goto error;
	}

	return 0;
error:
	return -1;
}


/* Checks if the Request Uri is used for an Emergency Service
 * @param msg - the SIP message
 * @param str1 - not used
 * @param str2 - not used
 * @returns #CSCF_RETURN_TRUE if the case, #CSCF_RETURN_FALSE if not
 */
int P_emergency_ruri(struct sip_msg *msg, char* str1, char* str2){

	int sos;


	str ruri = {msg->first_line.u.request.uri.s,
				msg->first_line.u.request.uri.len};

	LOG(L_DBG, "DBG:"M_NAME":P_emergency_ruri:checking if the ruri %.*s is an emergency ruri\n",
			ruri.len, ruri.s);	

	sos = is_emerg_ruri(ruri, NULL);
	switch(sos){
	
		case NOT_URN:	
		case NOT_EM_URN: 
			return CSCF_RETURN_ERROR;
		default: 
				return CSCF_RETURN_TRUE;
	}
}

/**
 * check if the P-CSCF should accept anonymous Emergency calls
 * @param msg - the SIP message
 * @param str1 - not used
 * @param str2 - not used
 * @returns #CSCF_RETURN_TRUE if anonymous user, #CSCF_RETURN_FALSE if not 
 */
int P_accept_anonym_em_call(struct sip_msg *msg,char *str1,char *str2)
{
	LOG(L_INFO,"DBG:"M_NAME":P_accept_anonym_em_call: Check if the P-CSCF is configured to accept an anonymous emergency call or not\n");

	if(anonym_em_call_support)	
		return CSCF_RETURN_TRUE;
	else	
		return CSCF_RETURN_FALSE;
}

static str anonym_display = {"Anonymous", 9};

/**
 * Finds if the message comes from an anonymous sip uri at this P-CSCF
 * @param msg - the SIP message
 * @param str1 - not used
 * @param str2 - not used
 * @returns #CSCF_RETURN_TRUE if anonymous user, #CSCF_RETURN_FALSE if not, #CSCF_RETURN_BREAK if error parsing 
 */
int P_is_anonymous_user(struct sip_msg *msg,char *str1,char *str2)
{
	struct to_body * from_body;

	LOG(L_INFO,"DBG:"M_NAME":P_is_anonymous_user: Check if anonymous identity used\n");

	if((!msg->from || !msg->from->parsed) && (parse_from_header(msg)<0))
		return CSCF_RETURN_BREAK;
	

	from_body = (struct to_body*)msg->from->parsed;
	if((from_body->display.len == anonym_display.len) &&
		(strncmp(from_body->display.s, anonym_display.s, anonym_display.len)==0)){
		LOG(L_INFO,"DBG:"M_NAME":P_is_anonymous_user: using anonymous identity\n");
		return CSCF_RETURN_TRUE;
	}

	return CSCF_RETURN_FALSE;
}

/**
 * Finds if the message comes from a user that made an Emergency Registration
 * @param msg - the SIP message
 * @param str1 - not used
 * @param str2 - not used
 * @returns #CSCF_RETURN_TRUE if sos uri parameter in Contact header, #CSCF_RETURN_FALSE if not 
 */
int P_emergency_flag(struct sip_msg *msg,char *str1,char *str2)
{
	contact_t *c;
	int sos_reg;
	contact_body_t * contact_bd=NULL;

	sos_reg = 0;

	LOG(L_INFO,"DBG:"M_NAME":P_emergency_flag: Check if the user made an Emergency Registration\n");

	//contact parsed
	if(!(contact_bd = cscf_parse_contacts(msg)) ){
		LOG(L_ERR, "ERR:"M_NAME":P_emergency_flag: parsing Contact header failed\n");
		return CSCF_RETURN_ERROR;
	}
	
	for(c=contact_bd->contacts;c;c=c->next){
		LOG(L_DBG,"DBG:"M_NAME":P_emergency_flag: contact <%.*s>\n",c->uri.len,c->uri.s);
			
		sos_reg += cscf_get_sos_uri_param(c->uri);
		if(sos_reg < 0)
			return CSCF_RETURN_FALSE;
	}
	
	if(sos_reg)
		return CSCF_RETURN_TRUE;

	return CSCF_RETURN_FALSE;
}

/**
 * Finds if the message comes from an emergency registered UE at this P-CSCF
 * @param msg - the SIP message
 * @param str1 - the realm to look into
 * @param str2 - not used
 * @returns #CSCF_RETURN_TRUE if registered, #CSCF_RETURN_FALSE if not 
 */
int P_is_em_registered(struct sip_msg *msg,char *str1,char *str2)
{
	int ret=CSCF_RETURN_FALSE;
	struct via_body *vb;

	LOG(L_INFO,"DBG:"M_NAME":P_is_em_registered: Looking if it has emergency registered\n");
//	print_r(L_INFO);

	vb = cscf_get_ue_via(msg);

	
	if (vb->port==0) vb->port=5060;
	LOG(L_INFO,"DBG:"M_NAME":P_is_em_registered: Looking for <%d://%.*s:%d>\n",
		vb->proto,vb->host.len,vb->host.s,vb->port);
	
	if (r_is_registered(vb->host,vb->port,vb->proto, EMERG_REG)) 
		ret = CSCF_RETURN_TRUE;
	else 
		ret = CSCF_RETURN_FALSE;	
	
	return ret;
}

int select_ECSCF(str * ecscf_used){

	ecscf_used->s = ecscf_uri_str.s;
	ecscf_used->len = ecscf_uri_str.len;

	return 0;
}

/**
 * selects the ecscf uri to be enforced
 * @param msg - the SIP message to add to
 * @param str1 - not used
 * @param str2 - not used
 * @returns #CSCF_RETURN_TRUE if ok or #CSCF_RETURN_ERROR on error
 */
int P_select_ecscf(struct sip_msg *msg,char *str1,char*str2)
{
	p_dialog *d = NULL;
	str sel_ecscf_uri, call_id, host;
	int port,transport;
	enum p_dialog_direction dir;

	dir = DLG_MOBILE_ORIGINATING;
	
	if (!find_dialog_contact(msg,dir,&host,&port,&transport)){
		LOG(L_ERR,"ERR:"M_NAME":P_select_ecscf(): Error retrieving orig contact\n");
		return CSCF_RETURN_BREAK;
	}		
		
	call_id = cscf_get_call_id(msg,0);
	if (!call_id.len)
		return CSCF_RETURN_FALSE;

	LOG(L_DBG,"DBG:"M_NAME":P_select_ecscf(): Call-ID <%.*s>\n",call_id.len,call_id.s);

	d = get_p_dialog(call_id,host,port,transport,&dir);
	if(!d){
		LOG(L_ERR, "ERR:"M_NAME":P_select_ecscf: could not find the emergency dialog\n");
		return CSCF_RETURN_BREAK;
	}

	if(!d->em_info.em_dialog){
		LOG(L_ERR, "ERR:"M_NAME":P_select_ecscf: script error: trying to use Emergency Services to route a non-emergency call\n");
		goto error;
	}

	if(select_ECSCF(&sel_ecscf_uri))
		goto error;

	STR_SHM_DUP(d->em_info.ecscf_uri, sel_ecscf_uri, "P_select_ecscf");
	d_unlock(d->hash);
	return CSCF_RETURN_TRUE;

error:
out_of_memory:
	LOG(L_ERR, "ERR:"M_NAME":P_select_ecscf: could not select an ECSCF\n");
	if(d) d_unlock(d->hash);
	return CSCF_RETURN_ERROR;
	
}


static str route_s={"Route: <",8};
static str route_e={">\r\n",3};

/**
 * Inserts the Route header containing the ecscf selected to be enforced
 * @param msg - the SIP message to add to
 * @param str1 - not used
 * @param str2 - not used
 * @returns #CSCF_RETURN_TRUE if ok or #CSCF_RETURN_ERROR on error
 */
int P_enforce_sos_routes(struct sip_msg *msg,char *str1,char*str2)
{
	int sos;
	str newuri={0,0};
	str x = {0,0}, urn = {0,0};
	p_dialog *d = NULL;
	str call_id, host;
	int port,transport;
	enum p_dialog_direction dir;
	str ruri = {msg->first_line.u.request.uri.s,
			msg->first_line.u.request.uri.len};

	sos = is_emerg_ruri(ruri, &urn);
	if(sos == NOT_URN || sos == NOT_EM_URN){
		LOG(L_ERR, "ERR:"M_NAME":P_enforce_sos_routes: invalid use: no emergency request URI\n");
		return CSCF_RETURN_ERROR;
	}

	LOG(L_DBG, "DBG:"M_NAME":P_enforce_sos_routes: rewritting uri with <%.*s>\n",
				urn.len, urn.s);

	
	dir = DLG_MOBILE_ORIGINATING;
	
	if (!find_dialog_contact(msg,dir,&host,&port,&transport)){
		LOG(L_ERR,"ERR:"M_NAME":P_enforce_sos_routes(): Error retrieving orig contact\n");
		return CSCF_RETURN_BREAK;
	}		
		
	call_id = cscf_get_call_id(msg,0);
	if (!call_id.len)
		return CSCF_RETURN_FALSE;

	LOG(L_DBG,"DBG:"M_NAME":P_enforce_sos_routes(): Call-ID <%.*s>\n",call_id.len,call_id.s);

	d = get_p_dialog(call_id,host,port,transport,&dir);
	if(!d){
		LOG(L_ERR, "ERR:"M_NAME":P_enforce_sos_routes: could not find the emergency dialog\n");
		return CSCF_RETURN_BREAK;
	}

	if(!d->em_info.em_dialog){
		LOG(L_ERR, "ERR:"M_NAME":P_enforce_sos_routes: script error: trying to use Emergency Services to route a non-emergency call\n");
		goto error;
	}

	if(!d->em_info.ecscf_uri.len || !d->em_info.ecscf_uri.s){
		LOG(L_ERR, "ERR:"M_NAME":P_enforce_sos_routes: script_error: no selected ecscf uri in the dialog info\n");
		goto error;
	}

	x.len = route_s.len + route_e.len + d->em_info.ecscf_uri.len;
			
	x.s = pkg_malloc(x.len);
	if (!x.s){
		LOG(L_ERR, "ERR:"M_NAME":P_enforce_sos_routes: Error allocating %d bytes\n",
			x.len);
		x.len=0;
		goto error;
	}
	x.len=0;
	STR_APPEND(x,route_s);
	STR_APPEND(x,d->em_info.ecscf_uri);
	STR_APPEND(x,route_e);
	
	if(set_dst_uri(msg, &d->em_info.ecscf_uri)){
	
		LOG(L_ERR, "ERR:"M_NAME":P_enforce_sos_routes: Could not set the destination uri %.*s\n",
				d->em_info.ecscf_uri.len, d->em_info.ecscf_uri.s);
		goto error;
	}

	if(urn.len && urn.s){
		LOG(L_DBG,"DBG:"M_NAME":P_enforce_sos_routes: rewritting uri with <%.*s>\n",
				urn.len, urn.s);

		if(rewrite_uri(msg, &urn) < 0) {
			LOG(L_ERR,"ERR:"M_NAME":P_enforce_sos_routes: Error rewritting uri with <%.*s>\n",
				urn.len, urn.s);
			goto error;	
		}
	} 


	if (cscf_add_header_first(msg,&x,HDR_ROUTE_T)) {
		if (cscf_del_all_headers(msg,HDR_ROUTE_T))
			goto end;
		else {
			LOG(L_ERR,"ERR:"M_NAME":P_enforce_sos_routes: new Route header added, but failed to drop old ones.\n");
		}
	}

error:
	LOG(L_ERR, "ERR:"M_NAME":P_enforce_sos_routes: could not enforce the E-CSCF URI\n");
	if(d) d_unlock(d->hash);
	if (x.s) pkg_free(x.s);
	if(newuri.s) pkg_free(newuri.s);
	return CSCF_RETURN_ERROR;
end:
	LOG(L_DBG, "DBG:"M_NAME":P_enforce_sos_routes: modified the info in order to be fwd to the E-CSCF %.*s\n",
			d->em_info.ecscf_uri.len, d->em_info.ecscf_uri.s);
	d_unlock(d->hash);
	return CSCF_RETURN_TRUE;
}

/* Check if the module has Emergency Services enabled
 * @param msg - not used
 * @param str1 - not used
 * @param str2 - not used
 */
int P_emergency_serv_enabled(struct sip_msg *msg,char *str1,char*str2){

	return (emerg_support>0)?CSCF_RETURN_TRUE:CSCF_RETURN_FALSE;
}

int fixup_380_alt_serv(void** param, int param_no){

	char* str1;

	if(param_no!=1){
		LOG(L_ERR, "ERR:"M_NAME":fixup_380_alt_serv: invalid param number");
		return -1;
	}

	str1 = (char*) *param;
	if(!str1 || str1[0] == '\0'){
	
		LOG(L_ERR, "ERR:"M_NAME":fixup_380_alt_serv: NULL reason");
		return -1;
	}

	return 0;
}


//Content-type as specified in TS 24.229, subsection 7.6
#define IMS_3GPP_XML_CNTENT_TYPE "Content-type: application/3gpp-ims+xml;schemaversion=\"1\"\n"
str Cont_type_3gpp_app_xml= {IMS_3GPP_XML_CNTENT_TYPE, (sizeof(IMS_3GPP_XML_CNTENT_TYPE)-1)};
static str invite_method={"INVITE",6}; 
/* Create the body of a 380 Alternative Service reply for Emergency reasons
 * @param msg - the SIP Request
 * @param str1 - the reason of the 380 reply
 * @param str2 - not used
 */
int P_380_em_alternative_serv(struct sip_msg * msg, char* str1, char* str2){

	str body_str = {0, 0};
	xmlChar * body = NULL;
       	const xmlChar *reason;
	int len = 0, ret;

	str uri = {msg->first_line.u.request.uri.s, 
				msg->first_line.u.request.uri.len};
	ret = CSCF_RETURN_FALSE;

	if(!reply_380_doc){
		LOG(L_ERR, "ERR:"M_NAME":P_380_em_alternative_service: the xml body of the reply was not intialized\n");
		return CSCF_RETURN_FALSE;
	}
	
	if(msg->first_line.u.request.method.len == invite_method.len &&
			strncmp(msg->first_line.u.request.method.s, invite_method.s, invite_method.len) == 0){
		
		
		ret = is_emerg_ruri(uri, NULL);
		if(ret == NOT_EM_URN)
			return CSCF_RETURN_ERROR;
	}

	reason = (xmlChar*) str1;
	xmlNodeSetContent(reason_alt_serv_node, BAD_CAST reason);

	if(ret == CSCF_RETURN_FALSE){
		xmlUnlinkNode(action_alt_serv_node);	
	}

	xmlDocDumpFormatMemoryEnc(reply_380_doc, &body, &len, IMS_3GPP_XML_ENC, 1);
	xmlAddChild(alt_serv_node, action_alt_serv_node);

	body_str.s = (char*) body;
	body_str.len = len;
	if(!body_str.s || !body_str.len){
		LOG(L_ERR, "ERR:"M_NAME":P_380_em_alternative_service: could not output the xml document\n");
		return CSCF_RETURN_FALSE;
	}

	LOG(L_DBG, "DBG:"M_NAME":P_380_em_alternative_service: the body for the 380 reply is:\n %.*s\n",
			body_str.len, body_str.s);

	cscf_add_header_rpl(msg, &Cont_type_3gpp_app_xml);
	
	if (add_lump_rpl( msg, body_str.s, body_str.len, LUMP_RPL_BODY)==0) {
		LOG(L_ERR, "ERR:"M_NAME":cscf_add_header_rpl: Can't add header <%.*s>\n",
			body_str.len,body_str.s);
 		return 0;
 	}

	return CSCF_RETURN_TRUE;
}

/* part of an own security solution for securing the interface between the P-CSCF and the E-CSCF
 * using the Path header
 */
int P_add_em_path(struct sip_msg * msg, char* str1, char* str2){

	urn_t sos;
	str x={0,0};
	str urn;

	str ruri = {msg->first_line.u.request.uri.s,
			msg->first_line.u.request.uri.len};

	sos = is_emerg_ruri(ruri, &urn);
	if(sos == NOT_URN || sos == NOT_EM_URN){
		LOG(L_ERR, "ERR:"M_NAME":P_add_em_path: invalid use: no emergency request URI\n");
		return CSCF_RETURN_ERROR;
	}


	STR_PKG_DUP(x, pcscf_path_orig_em_uri_str, "pkg");
	if (!x.s) return CSCF_RETURN_ERROR;
	if (cscf_add_header(msg,&x,HDR_OTHER_T)) return CSCF_RETURN_TRUE;
	else {
		pkg_free(x.s);
		return CSCF_RETURN_ERROR;
	}
out_of_memory:
	return CSCF_RETURN_ERROR;	
}

static str path_header_name = {"Path", 4};
/* part of an own security solution for securing the interface between the P-CSCF and the E-CSCF
 * using the Path header*/
int P_check_em_path(struct sip_msg * msg, char * str1, char * str2){

	struct hdr_field* hdr;
	str path_body;
	str call_id;
	enum p_dialog_direction dir;
	struct sip_msg * req;
	str host;
	int port,transport;
	p_dialog *d;

	if(msg->first_line.type == SIP_REQUEST){
		if(msg->first_line.u.request.method.len == 3 && 
			strncasecmp(msg->first_line.u.request.method.s,"ACK",3)==0)
			return CSCF_RETURN_TRUE;
		req = msg;
		dir = DLG_MOBILE_TERMINATING;	
	}else{
		req = cscf_get_request_from_reply(msg);
		dir = DLG_MOBILE_ORIGINATING;	
	}
	
	if (!find_dialog_contact(req,dir,&host,&port,&transport)){
		LOG(L_ERR,"ERR:"M_NAME":P_check_em_path(): Error retrieving orig contact\n");
		return CSCF_RETURN_BREAK;
	}		
		
	call_id = cscf_get_call_id(msg,0);
	if (!call_id.len)
		return CSCF_RETURN_FALSE;

	LOG(L_DBG,"DBG:"M_NAME":P_check_em_path(): Call-ID <%.*s>\n",call_id.len,call_id.s);

	d = get_p_dialog(call_id,host,port,transport,&dir);
	if (!d)
		d = get_p_dialog(call_id,host,port,transport,0);
	if (!d){
		LOG(L_CRIT,"ERR:"M_NAME":P_update_dialog: dialog does not exists!\n");
		return CSCF_RETURN_FALSE;
	}
	if(!d->em_info.em_dialog){
		d_unlock(d->hash);
		return CSCF_RETURN_TRUE;
	}

	d_unlock(d->hash);

	hdr = cscf_get_header(msg, path_header_name);
	if(!hdr){
		LOG(L_ERR, "ERR:"M_NAME":P_check_em_path: invalid use: no Path header\n");
		return CSCF_RETURN_FALSE;
	}
	
	path_body = hdr->body;
	if(!path_body.s || !path_body.len){
		LOG(L_ERR, "ERR:"M_NAME":P_check_em_path: invalid use: null Path header body\n");
		return CSCF_RETURN_FALSE;
	}
	
	if(path_body.len != pcscf_path_orig_em_uri_str.len ||
			strncmp(path_body.s, pcscf_path_orig_em_uri_str.s, path_body.len)!=0){
		LOG(L_ERR, "ERR:"M_NAME":P_check_em_path: invalid use: invalid Path header body\n");
		return CSCF_RETURN_FALSE;
	}
	
	return CSCF_RETURN_TRUE;
}
