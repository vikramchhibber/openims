/*
 *  $Id: locsip.c 655 2009-08-7 15:31:52Z aon $
 *
 * Copyright (C) 2009 FhG Fokus
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
 * Location Retrieving Function module
 * 
 * Scope:
 * - LOCSIP interface, see http://member.openmobilealliance.org/ftp/Public_documents/LOC/Permanent_documents/
 * 
 */


#include "mod.h"
#include "sip.h"
#include "locsip.h"
#include "locsip_subscribe.h"
#include "user_data.h"
#include "lost.h"
#include "multipart_parse.h"
#include "../../sr_module.h"

extern int locsip_srv_port;
extern str locsip_srv_ip_s;
extern int use_locsip;
extern str locsip_srv_uri;
static str service_hdr_name = {"Service",7};
extern struct tm_binds tmb; 

int sw_tr_2_options_tr(struct sip_msg *, user_d * user_data);
xmlNode * retrieve_pidf_lo(struct sip_msg * msg, str * pidf_body, loc_fmt * crt_loc_fmt);

#define FParam_INT(val) { \
	 .v = { \
		.i = val \
	 },\
	.type = FPARAM_INT, \
	.orig = "int_value", \
};

#define FParam_STRING(val) { \
	.v = { \
		.str = STR_STATIC_INIT(val) \
	},\
	.type = FPARAM_STR, \
	.orig = val, \
};

int (*sl_reply)(struct sip_msg* _msg, char* _str1, char* _str2); 

static int str_trim(str *s)
{
	int i;
	for (i = 0;i < s->len; i++)
	{
		if (s->s[i] != '\r' && s->s[i] != '\t' && s->s[i] != ' ')
		{
			break;
		}
	}
	s->s = s->s + i;	
	s->len -= i;

	for (i = s->len;i >=0; i--)
	{
		if (s->s[i] == '\r' && s->s[i] == '\t' && s->s[i] == ' ')
		{
			s->len--;
		}
		else
		{
			break;
		}
	}
	return 1;
}

/*
 * check if the LRF was configured to use the LOCSIP interface
 * @param msg - not used
 * @param str1 - not used
 * @param str2 - not used
 * @returns #CSCF_RETURN_TRUE if true and #CSCF_RETURN_FALSE otherwise
 */
int LRF_uses_LOCSIP(struct sip_msg* msg, char * str1, char* str2){

	if(use_locsip == 0){

		return CSCF_RETURN_FALSE;
	}
	return CSCF_RETURN_TRUE;
}

/**
 * Subscribe to the location event to the LOCSIP server
 * @param req - OPTIONS req from the E-CSCF
 * @param str1 - not used
 * @param str2 - not used
 * @returns #CSCF_RETURN_TRUE if subscribed, #CSCF_RETURN_FALSE if not or #CSCF_RETURN_ERROR on error
 */
int LRF_subscribe_LOCSIP(struct sip_msg *req, char* str1, char* str2)
{
	int max_expires;
	str public_id={0,0};
	loc_subscription * subscr;
	str user_uri;
	user_d * d=NULL;
	str service;

	LOG(L_INFO, "INFO:"M_NAME":LRF_subscribe_LOCSIP\n");
	service = cscf_get_headers_content(req , service_hdr_name);
	if(!service.len || !service.s){
		LOG(L_ERR, "ERR:"M_NAME":LRF_subscribe_LOCSIP: could not find the service header in the OPTIONS, or could not be parsed\n");
		return CSCF_RETURN_FALSE;
	}
	
	str callid = cscf_get_call_id(req, NULL);
	if(!callid.s || !callid.len){
		LOG(L_ERR, "ERR:"M_NAME":LRF_subscribe_LOCSIP: could not find the callid header in the OPTIONS request\n");
		return CSCF_RETURN_FALSE;
	}

	user_uri = req->first_line.u.request.uri;
	d = get_user_data(user_uri, service, callid);
	if(!d) {
		LOG(L_ERR, "ERR:"M_NAME":LRF_subscribe_LOCSIP: could not found user data for uri %.*s and service %.*s\n",
				user_uri.len, user_uri.s, service.len, service.s);
		return CSCF_RETURN_FALSE;
	}

	public_id.s = req->first_line.u.request.uri.s;
	public_id.len = req->first_line.u.request.uri.len;
	
	LOG(L_DBG, "DBG:"M_NAME":LRF_subscribe_LOCSIP: trying to send a subscribe for public id %.*s\n",
			public_id.len, public_id.s);

	max_expires = 0;
	
	if (public_id.s){
		subscr = loc_subscribe(public_id,max_expires, d);
		if(!subscr)
			goto error;
		d->loc_subscr = (void*) subscr;
		LOG(L_DBG, "DBG:"M_NAME":LRF_subscribe_LOCSIP: created a subscription for %.*s\n",
				subscr->req_uri.len, subscr->req_uri.s);
		if(!loc_send_subscribe(subscr, locsip_srv_uri, max_expires)){
			LOG(L_ERR, "ERR:"M_NAME":LRF_subscribe_LOCSIP: could not send the subscription\n");
			goto error;
		}
		lrf_unlock(d->hash);
		return CSCF_RETURN_TRUE;
	}

error:
	lrf_unlock(d->hash);
	return CSCF_RETURN_FALSE;
}


static str pidflo_info = {"application/pidf+xml", 20};

static fparam_t fp_481 = FParam_INT(481);
static fparam_t fp_subscr_not_found = FParam_STRING("Subscription doea not exist");

static fparam_t fp_400 = FParam_INT(400);
static fparam_t fp_bad_req = FParam_STRING("Bad request");

static fparam_t fp_200 = FParam_INT(200);
static fparam_t fp_ok = FParam_STRING("OK - Notification processed");

static str s_subscription_state={"Subscription-State",18};
//static str s_active={"active",6};
//static str s_pending={"pending",7};
static str s_terminated={"terminated",10};

/* process the notification for the location event, received from the LOCSIP server
 *	sends the appropriate reply to the LOCSIP server
 *	releases the NOTIFY transaction and switches to the OPTIONS transaction
 * @param msg - the request
 * @param str1 - not used
 * @param str2 - not used
 * @returns #CSCF_RETURN_ERROR if errors have occured and #CSCF_RETURN_TRUE if everything ok
 */
int LRF_process_notification(struct sip_msg* msg, char * str1, char* str2){

	int ret;
	str user_uri;
	loc_subscription * subscr = NULL;
	user_d * user_data = NULL;
	xmlNode *loc = NULL;
	loc_fmt crt_loc_fmt;
	str pidf_body;
	str subscr_state_val = {0,0};
	int terminated = 0;


	LOG(L_INFO,"INFO:"M_NAME":LRF_process_notification: Checking NOTIFY\n");

	if(!cscf_get_from_uri(msg, &user_uri) || !user_uri.len || !user_uri.s){
		LOG(L_ERR,"ERR:"M_NAME":LRF_process_notification: could not retrieve the From uri\n");
		goto parse_error;
	}

	str callid = cscf_get_call_id(msg, NULL);
	if(!callid.len || !callid.s){
		LOG(L_ERR,"ERR:"M_NAME":LRF_process_notification: could not retrieve the callid header\n");
		goto parse_error;
	}
	
	subscr = get_loc_subscription_callid(user_uri, callid);
	if(!subscr){

		LOG(L_ERR,"ERR:"M_NAME":LRF_process_notification: could not retrieve the location "
				"subscription for uri %.*s\n", user_uri.len, user_uri.s);
		goto subscr_error;		
	}
	
	/* update the subscription state */
	subscr_state_val = cscf_get_headers_content(msg, s_subscription_state);
	if(!subscr_state_val.len || !subscr_state_val.s){
		LOG(L_ERR, "ERR:"M_NAME"LRF_process_notification: could not retrieve the Subscription-State header\n");
		goto parse_error;
	}
	str_trim(&subscr_state_val);
	LOG(L_DBG,"DBG:"M_NAME":LRF_process_notification: subscription state field %.*s\n", 
		subscr_state_val.len, subscr_state_val.s);
	
	if(strncasecmp(subscr_state_val.s, s_terminated.s, s_terminated.len) == 0){
		LOG(L_DBG,"DBG:"M_NAME":LRF_process_notification: subscription in state terminated\n"); 
		terminated = 1;
		goto reply;
	}
	
	/* treat event */	
	loc = retrieve_pidf_lo(msg, &pidf_body, &crt_loc_fmt);
	if(!loc){
		LOG(L_ERR, "ERR:"M_NAME"LRF_process_notification: could not retrieve the PIDF-LO\n");
		goto parse_error;
	}
reply:
	if((ret = sl_reply(msg, (char *)&fp_200, (char *)&fp_ok)) == -1){
		LOG(L_ERR,"ERR:"M_NAME":LRF_process_notification: could not send the 200 ok reply\n");
		goto internal_error;
	}
	
	//release the notify transaction
	if(tmb.t_release(msg) == -1){
		LOG(L_ERR,"ERR:"M_NAME":LRF_process_notification: could not release the notify transaction\n");
		goto internal_error;
	}
	
	if(terminated >0)
		goto terminated_exit;

	user_data = subscr->user_data;
	if(!user_data){
		LOG(L_ERR,"BUG:"M_NAME":LRF_process_notification: null user_data in the subscription info "
			"for uri %.*s\n", user_uri.len, user_uri.s);
		goto internal_error;		
	}

	user_data->loc = loc;
	user_data->locsip_loc = 1;
	user_data->l_fmt = crt_loc_fmt;
		
	lrf_unlock(user_data->hash);
	subs_unlock(subscr->hash);
	return CSCF_RETURN_TRUE;

parse_error:
	if(subscr) subs_unlock(subscr->hash);
	sl_reply(msg, (char *)&fp_400, (char *)&fp_bad_req);
	return CSCF_RETURN_ERROR;

subscr_error:
	sl_reply(msg, (char *)&fp_481, (char *)&fp_subscr_not_found);
	return CSCF_RETURN_ERROR;

internal_error:
terminated_exit:
	if(subscr) subs_unlock(subscr->hash);
	//TODO: locking the user data
	if(user_data) lrf_unlock(user_data->hash);
	return CSCF_RETURN_ERROR;
}


/* Switch to the OPTIONS transaction from the NOTIFY trans
 * @returns #CSCF_RETURN_ERROR in case of error or #CSCF_RETURN_TRUE if ok
 */
int LRF_sw2options(struct sip_msg* msg, char* str1, char* str2){

	str user_uri;
	loc_subscription * subscr = NULL;
	user_d * user_data = NULL;
	int ret = CSCF_RETURN_ERROR;

	LOG(L_INFO,"INFO:"M_NAME":LRF_sw2options: trying to switch to the OPTIONS transaction\n");

	if(!cscf_get_from_uri(msg, &user_uri) || !user_uri.len || !user_uri.s){
		LOG(L_ERR,"ERR:"M_NAME":LRF_sw2options: could not retrieve the From uri\n");
		goto end;
	}

	str callid = cscf_get_call_id(msg, NULL);
	if(!callid.len || !callid.s){
		LOG(L_ERR,"ERR:"M_NAME":LRF_sw2options: could not retrieve the callid header\n");
		goto end;
	}

	subscr = get_loc_subscription_callid(user_uri, callid);
	if(!subscr){

		LOG(L_ERR,"ERR:"M_NAME":LRF_sw2options: could not retrieve the location "
				"subscription for uri %.*s\n", user_uri.len, user_uri.s);
		goto end;		
	}
	
	user_data = subscr->user_data;
	if(!user_data){
		LOG(L_ERR,"BUG:"M_NAME":LRF_sw2options: null user_data in the subscription info "
			"for uri %.*s\n", user_uri.len, user_uri.s);
		goto end;		
	}

	//switch to the options transaction
	if(sw_tr_2_options_tr(msg, user_data)<0){
		
		LOG(L_ERR,"BUG:"M_NAME":LRF_sw2options: could not switch to the options trans\n");
		goto end;
	}

	ret = CSCF_RETURN_TRUE;
end:
	if(subscr) subs_unlock(subscr->hash);
	return ret;

}

int sw_tr_2_options_tr(struct sip_msg * msg, user_d * user_data){

	struct cell* options_trans, * crt_trans;
	enum route_mode crt_rmode;
	unsigned int hash_index, label;

	hash_index = user_data->options_tr.hash_index;
	label = user_data->options_tr.label;

	LOG(L_DBG,"DBG:"M_NAME":sw_tr_2_options_tr: index: %u label: %u callid: %.*s\n", 
			hash_index, label, 
			user_data->options_tr.callid.len, user_data->options_tr.callid.s);

	//set the OPTIONS transaction as the current one
	if(tmb.t_enter_ctx(hash_index, label,
		&crt_rmode, MODE_REQUEST, &crt_trans, &options_trans)!=0){
		LOG(L_ERR, "ERR:"M_NAME":sw_tr_2_options_tr: could not switch to the OPTIONS transaction\n");
		return -1;
	}

	return 0;
}

xmlNode * retrieve_pidf_lo(struct sip_msg * msg, str * pidf_body, loc_fmt * crt_loc_fmt){

	str content_type;
	xmlNode *loc = NULL;

	content_type = cscf_get_content_type(msg);
	if (content_type.len!=pidflo_info.len ||
		strncasecmp(content_type.s, pidflo_info.s, pidflo_info.len)!=0){

		LOG(L_ERR,"ERR:"M_NAME":LRF_process_notification: The content should be %.*s but it is %.*s\n",
			pidflo_info.len, pidflo_info.s, content_type.len, content_type.s);
		goto parse_error;
	}

	if(get_pidf_lo_body(msg, pidf_body)<0){
		LOG(L_ERR, "ERR:"M_NAME":LRF_process_notification:could not get the pidf+xml body\n");
		goto parse_error;
	}
		
	loc = verify_pidf_xml_body(*pidf_body, crt_loc_fmt);
parse_error:	
	return loc;
}


