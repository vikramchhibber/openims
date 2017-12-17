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
 *	store the information for a user wanting to contact a service depending on its location
 */
 

#include <ctype.h>
#include <sys/types.h>
#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <libxml/tree.h>
#include <libxml/parser.h>

#include "../../modules/tm/tm_load.h"
#include "../../modules/tm/t_lookup.h"
#include "../../parser/parser_f.h"
#include "../../parser/contact/parse_contact.h"
#include "mod.h"
#include "parse_content.h"
#include "sip.h"
#include "user_data.h"
#include "locsip_subscribe.h"

extern str esqk_prefix_str;
extern struct tm_binds tmb;    

int user_data_hash_size;
user_d_hash_slot * user_datas = 0;

static str service_hdr_name = {"Service",7};
static str esqk_hdr_s = {"Esqk: ",6};
static str esqk_hdr_e = {"\r\n",2};
static str content_type_hdr=  {"Content-Type: application/pidf+xml\r\n",36};
static str psap_uri_hdr_s=  {"PSAP-URI: ",10};

void del_loc_subscription(loc_subscription *s);

/**
 * Initialize the LRF user data hash table.
 * @param hash_size - size of the dialog hash table
 * @returns 1 if OK, 0 on error
 */
int init_lrf_user_data(int hash_size){

	int i;

	user_data_hash_size = hash_size;
	user_datas = shm_malloc(sizeof(user_d_hash_slot)*user_data_hash_size);

	if (!user_datas) return 0;

	memset(user_datas,0,sizeof(user_d_hash_slot)*user_data_hash_size);
	
	for(i=0;i<user_data_hash_size;i++){
		user_datas[i].lock = lock_alloc();
		if (!user_datas[i].lock){
			LOG(L_ERR,"ERR:"M_NAME":init_lrf_user_data(): Error creating lock\n");
			return 0;
		}
		user_datas[i].lock = lock_init(user_datas[i].lock);
	}

	return 1;
}

void free_user_data(user_d * data){

	if(!data)
		return;
	if(data->psap_uri.s){
		shm_free(data->psap_uri.s);
		data->psap_uri.s = NULL;
	}
	/*esqk, user_uri and service are allocated in the block of data, 
	 * at the same time*/
	shm_free(data);
	data = NULL;
}

/**
 * Locks the required slot of the user data hash table.
 * @param hash - index of the slot to lock
 */
void lrf_lock(unsigned int hash){

	lock_get(user_datas[(hash)].lock);
}

/**
 * Unlocks the required slot of the user data hash table.
 * @param hash - index of the slot to lock
 */
void lrf_unlock(unsigned int hash){

	lock_release(user_datas[(hash)].lock);
}

/**
 * Destroy the hash table
 */
void destroy_lrf_user_data(){

	int i;
	user_d *d,*nd;
	for(i=0;i<user_data_hash_size;i++){
		lrf_lock(i);
			d = user_datas[i].head;
			while(d){
				nd = d->next;
				free_user_data(d);
				d = nd;
			}
		lrf_unlock(i);
		lock_dealloc(user_datas[i].lock);
	}
	shm_free(user_datas);
	user_datas = NULL;
}

void print_user_data(user_d* d, int log_level){

	LOG(log_level, "INF:"M_NAME":print_user_data: user uri %.*s, esqk %.*s, service %.*s, psap_uri %.*s\n",
			d->user_uri.len, d->user_uri.s,
			d->esqk.len, d->esqk.s,
			d->service.len, d->service.s,
			d->psap_uri.len, d->psap_uri.s);

	LOG(log_level, "			OPTIONS trans: callid %.*s, index %u, label %u\n",
			d->options_tr.callid.len, d->options_tr.callid.s,
			d->options_tr.hash_index, d->options_tr.label);
}


/*
 * Print the user_data hash table
 */
void print_lrf_user_data(int log_level){

	int i;
	user_d *d;

	LOG(L_INFO, "INFO:"M_NAME":--------LRF user data list begin--------\n");
	for(i=0;i<user_data_hash_size;i++){
		lrf_lock(i);
			d = user_datas[i].head;
			while(d){
				print_user_data(d, log_level);
				d = d->next;
			}
		lrf_unlock(i);
	}
	LOG(L_INFO, "INFO:"M_NAME":--------LRF user data list end--------\n");
}

/**
 * The user timer prints the user datas information
 * @param ticks - the current time
 * @param param - pointer to the dialogs list
 */
void user_data_timer(unsigned int ticks, void* param){

		
	LOG(L_DBG,"DBG:"M_NAME":user_data_timer: Called at %d\n",ticks);
	if (!user_datas) user_datas = (user_d_hash_slot*)param;

	print_lrf_user_data(L_INFO);
}

/* get the postfix of the esqk: the number of seconds returned by time()
 * (timestamp)
 */
int get_esqk_post(int * stamp_len, unsigned int * tmstamp){

	time_t rawtime;

	if(!stamp_len || !tmstamp){
		LOG(L_ERR, "ERR:"M_NAME":get_esqk_post: invalid parameters\n");
		return 0;
	}

	time ( &rawtime );
	int2str((unsigned int) rawtime, stamp_len);
	if((*stamp_len <=0) || (*stamp_len > (INT2STR_MAX_LEN))){
		LOG(L_ERR, "ERR:"M_NAME":get_esqk_post: could not encode the timestamp %u\n", 
				(unsigned int)rawtime);
		return 0;
	}
	*tmstamp = (unsigned int) rawtime;
	return 1;
}

/**
 * Computes the hash for a string.
 * @param sip uri of the user - the string to compute for
 * @returns the hash % user_data_hash_size
 */
inline unsigned int get_user_d_hash(str user_uri)
{
	if (user_uri.len==0) return 0;
#define h_inc h+=v^(v>>3)
	char* p;
	register unsigned v;
	register unsigned h;
  	
	h=0;
	for (p=user_uri.s; p<=(user_uri.s+user_uri.len-4); p+=4){
		v=(*p<<24)+(p[1]<<16)+(p[2]<<8)+p[3];
		h_inc;
	}
	v=0;
	for (;p<(user_uri.s+user_uri.len); p++) {
		v<<=8;
		v+=*p;
	}
	h_inc;
	
	h=((h)+(h>>11))+((h>>13)+(h>>23));
	return (h)%user_data_hash_size;
#undef h_inc 
}

/* create new user data, based on the user SIP URI
 * @param user_uri the SIP uri of the user
 * @param sevice the service that the user is trying to access
 */
user_d * new_user_data(str user_uri, str service, struct trans_info * options_tr){

	user_d * user_data=NULL;
	str esqk = {0,0};
	int esqk_post_len;
	unsigned int tmstamp;

	if(!get_esqk_post(&esqk_post_len, &tmstamp)){
		LOG(L_ERR, "ERR:"M_NAME":new_user_data: could not allocate a new esqk\n");
		return user_data;
	}

	LOG(L_DBG, "DBG:"M_NAME":new_user_data: esqk_post is %u, len %i\n",
		tmstamp, esqk_post_len);

	esqk.len = esqk_prefix_str.len + esqk_post_len;

	str callid = options_tr->callid;

	char * p = (char*)shm_malloc(sizeof(user_d)+(esqk.len+user_uri.len+service.len+callid.len)*sizeof(char));
	if(!p){
		LOG(L_ERR, "ERR:"M_NAME":new_user_data: could not alloc %i bytes\n",
				sizeof(struct user_d_cell)+(esqk.len+user_uri.len+service.len+callid.len)*sizeof(char));
		return user_data;
	}

	user_data = (struct user_d_cell*) p;
	memset(user_data, 0, sizeof(struct user_d_cell));

	/*set the esqk identifier*/
	p += sizeof(struct user_d_cell);
	user_data->esqk.s = esqk.s =  p;
	user_data->esqk.len = esqk.len;
	sprintf(esqk.s, "%.*s", esqk_prefix_str.len, esqk_prefix_str.s);
	sprintf(esqk.s+esqk_prefix_str.len, "%u", tmstamp);
	
	LOG(L_DBG, "DBG:"M_NAME":new_alloc_user_data: the allocated esqk is %.*s\n",
			user_data->esqk.len, user_data->esqk.s);
	
	/*set the user uri*/
	p += esqk.len;
	user_data->user_uri.s = p;
	user_data->user_uri.len = user_uri.len;
	memcpy(user_data->user_uri.s, user_uri.s, user_uri.len*sizeof(char));
	
	LOG(L_DBG, "DBG:"M_NAME":new_alloc_user_data: the user uri is %.*s\n", 
			user_data->user_uri.len, user_data->user_uri.s);

	/*set the name of the service*/
	p += user_uri.len;
	user_data->service.s = p;
	user_data->service.len = service.len;
	memcpy(user_data->service.s, service.s, service.len*sizeof(char));
	LOG(L_DBG, "DBG:"M_NAME":new_alloc_user_data: the service uri is %.*s\n", 
			user_data->service.len, user_data->service.s);
	
	user_data->hash = get_user_d_hash(user_uri);
	p += service.len;
	user_data->options_tr.callid.s = p;
	user_data->options_tr.callid.len = callid.len;
	memcpy(user_data->options_tr.callid.s, callid.s, callid.len*sizeof(char));
	user_data->options_tr.hash_index = options_tr->hash_index;
	user_data->options_tr.label = options_tr->label;

	return user_data;
}

/* 
 * Add a user data in the user data hash table
 * @param user_uri the SIP uri of the user
 * @param sevice the service that the user is trying to access
 */
user_d * add_user_data(str user_uri, str service, struct trans_info * options_tr){
		
	user_d * d;
	
	d = get_user_data(user_uri, service, options_tr->callid);
	if(d){
		LOG(L_ERR, "ERR:"M_NAME":add_user_data: there is already a user data "
				"for the user %.*s for the service %.*s and callid %.*s\n",
				user_uri.len, user_uri.s, 
				service.len, service.s,
				options_tr->callid.len, options_tr->callid.s);
		lrf_unlock(d->hash);
		return NULL;
	}

	d = new_user_data(user_uri, service, options_tr);
	if(!d)
		return NULL;

	lrf_lock(d->hash);
		d->next = 0;
		d->prev = user_datas[d->hash].tail;
		if (d->prev) d->prev->next = d;
		user_datas[d->hash].tail = d;
		if (!user_datas[d->hash].head) user_datas[d->hash].head = d;
	lrf_unlock(d->hash);
	return d;

}

/*
 * Get a user data from the user data hash table
 * @param user_uri the user SIP URI
 * @param service the service urn
 */
user_d * get_user_data(str user_uri, str service, str callid){

	user_d* d=0;
	unsigned int hash = get_user_d_hash(user_uri);

	lrf_lock(hash);
		d = user_datas[hash].head;
		while(d){
			if (d->user_uri.len == user_uri.len &&
				d->service.len == service.len &&
				d->options_tr.callid.len == callid.len &&
				strncasecmp(d->options_tr.callid.s, callid.s, callid.len)== 0 &&
				strncasecmp(d->user_uri.s,user_uri.s,user_uri.len)==0 &&
				strncasecmp(d->service.s, service.s, service.len)==0) {
					return d;
				}
			d = d->next;
		}
	lrf_unlock(hash);
	return 0;
}

/**
 * Deletes a dialog from the hash table
 * \note Must be called with a lock on the dialogs slot
 * @param d - the dialog to delete
 */
void del_user_data(user_d *d)
{
	LOG(L_INFO,"DBG:"M_NAME":del_user_data(): Deleting user data for uri %.*s and service %.*s\n",
			d->user_uri.len,d->user_uri.s,
			d->service.len,d->service.s);
	if (d->prev) d->prev->next = d->next;
	else user_datas[d->hash].head = d->next;
	if (d->next) d->next->prev = d->prev;
	else user_datas[d->hash].tail = d->prev;
	free_user_data(d);
}


/*
 * alloc a new ESQK and a user data element for the current user
 * @param msg: current OPTIONS request
 * @param str1: not used
 * @param str2: not used
 */
int LRF_alloc_user_data(struct sip_msg* msg, char*str1, char*str2){

	str user_uri;
	user_d * user_data;
	str service;
	struct trans_info options_tr;

	if(tmb.t_get_trans_ident(msg, &options_tr.hash_index, &options_tr.label) != 1){
		LOG(L_ERR, "ERR:"M_NAME":LRF_alloc_user_data:could not retrive hash_index and label of the current message's transaction\n");
		return CSCF_RETURN_FALSE;
	}

	service = cscf_get_headers_content(msg , service_hdr_name);
	if(!service.len || !service.s){
		LOG(L_ERR, "ERR:"M_NAME":LRF_alloc_user_data: could not find the service header in the OPTIONS, or could not be parsed\n");
		return CSCF_RETURN_FALSE;
	}

	str callid = cscf_get_call_id(msg, NULL);
	if(!callid.s || !callid.len){
		LOG(L_ERR, "ERR:"M_NAME":LRF_alloc_user_data: could not find the callid header in the OPTIONS request\n");
		return CSCF_RETURN_FALSE;
	}
	options_tr.callid.s = callid.s;
	options_tr.callid.len = callid.len;
	
	user_uri = msg->first_line.u.request.uri;
	user_data = add_user_data(user_uri, service, &options_tr);
	if(!user_data)
		return CSCF_RETURN_FALSE;
	print_lrf_user_data(L_INFO);
	return CSCF_RETURN_TRUE;
}

/*
 * delete the user data of the current user if exists(the URI is the R-URI of the current OPTIONS msg)
 * @param msg: current OPTIONS request
 * @param str1: not used
 * @param str2: not used
 */
int LRF_del_user_data(struct sip_msg* msg, char*str1, char*str2){

	str user_uri;
	user_d * d;
	unsigned int hash;
	str service;

	if (msg->first_line.u.request.method.len!=7||
		memcmp(msg->first_line.u.request.method.s,"OPTIONS",7)!=0){
		LOG(L_WARN,"WARN:"M_NAME":LRF_del_user_data: The method is not an OPTIONS, trying to replace the message\n");

		msg = cscf_get_request_from_reply(NULL);
		if(! msg || msg->first_line.type!=SIP_REQUEST || msg->first_line.u.request.method.len!=7||
			memcmp(msg->first_line.u.request.method.s,"OPTIONS",7)!=0){
					
			LOG(L_ERR,"BUG:"M_NAME":LRF_del_user_data: The new message is not an OPTIONS request either\n");
			return CSCF_RETURN_ERROR;
		}
	}

	service = cscf_get_headers_content(msg , service_hdr_name);
	if(!service.len || !service.s){
		LOG(L_ERR, "ERR:"M_NAME":LRF_del_user_data: could not find the service header in the OPTIONS, or could not be parsed\n");
		return CSCF_RETURN_FALSE;
	}

	str callid = cscf_get_call_id(msg, NULL);
	if(!callid.s || !callid.len){
		LOG(L_ERR, "ERR:"M_NAME":LRF_get_psap: could not find the callid header in the OPTIONS request\n");
		return CSCF_RETURN_FALSE;
	}

	user_uri = msg->first_line.u.request.uri;
	d = get_user_data(user_uri, service, callid);
	if(!d) return CSCF_RETURN_FALSE;

	if(d->loc_subscr)
		del_loc_subscription((loc_subscription*)d->loc_subscr);
	hash = d->hash;
	del_user_data(d);
	lrf_unlock(hash);

	return CSCF_RETURN_TRUE;
}

int get_options_resp_body(str * body, user_d * d){
	
	body->s = NULL;
	body->len = 0;
	xmlChar * req_body = NULL;
	int req_body_len = 0;

	xmlNode * loc = d->loc;

	xmlDocDumpFormatMemoryEnc(loc->doc, &req_body, &req_body_len, "UTF-8", 1);
	body->s = (char*)req_body;
	body->len = req_body_len;

	if(!body->s || !body->len){
		LOG(L_ERR, "ERR:"M_NAME":get_options_resp_body: could not output the xml location document\n");
		return -1;
	}

	LOG(L_DBG, "DBG:"M_NAME":get_options_resp_body: body of the OPTIONS response is %.*s\n",
			body->len, body->s);
	return 0;
}

/* build the extra headers for the OPTIONS reply : containing Content-type and ESQK
 * @param headers -  the resulting string will be stored there
 * @param d - the user data cell
 * @return 0 if ok, -1 if error
 */
int get_options_resp_headers(str * headers, user_d * d){

	if(!headers || !d){
		LOG(L_ERR, "ERR:"M_NAME":get_options_resp_headers: invalid parameters\n");
		return -1;
	}
	
	if(!d->esqk.s || !d->esqk.len){
		LOG(L_ERR, "BUG:"M_NAME":get_options_resp_headers: esqk not well set");
		return -1;
	}
	
	headers->len = esqk_hdr_s.len + d->esqk.len + esqk_hdr_e.len + content_type_hdr.len +
			psap_uri_hdr_s.len+d->psap_uri.len+esqk_hdr_e.len+1;
	headers->s = pkg_malloc(headers->len* sizeof(char));
	if(!headers->s){
		LOG(L_ERR, "ERR:"M_NAME":get_options_resp_headers: out of pkg memory\n");
		return -1;
	}

	headers->len = 0;
	STR_APPEND(*headers, esqk_hdr_s);
	STR_APPEND(*headers, d->esqk);
	STR_APPEND(*headers, esqk_hdr_e);
	STR_APPEND(*headers, psap_uri_hdr_s);
	STR_APPEND(*headers, d->psap_uri);
	STR_APPEND(*headers, esqk_hdr_e);
	if(d->loc && d->locsip_loc)
		STR_APPEND(*headers, content_type_hdr);
	headers->s[headers->len] = '\0';

	LOG(L_DBG, "DBG:"M_NAME":get_options_resp_headers: headers are: %s\n", headers->s);

	return 0;
}

/* Send the OPTIONS response to the E-CSCF
 * could be used after a LOCSIP NOTIFY is received
 * @param msg  - the OPTIONS request from the ECSCF
 * @param str1 - not used
 * @param str2 - not used
 * @return CSCF_RETURN_TRUE if ok, or CSCF_RETURN_FALSE if error
 */
int LRF_call_query_resp(struct sip_msg* msg, char*str1, char*str2){

	str user_uri;
	user_d * d= NULL;
	str service;
	struct cell * trans = NULL;
	str resp_body = {0,0};
	str headers = {0,0};
	unsigned int hash_index, label;

	LOG(L_INFO, "INFO:"M_NAME":LRF_call_query_resp\n");

	if (msg->first_line.u.request.method.len!=7||
		memcmp(msg->first_line.u.request.method.s,"OPTIONS",7)!=0){
		LOG(L_WARN,"WARN:"M_NAME":LRF_call_query_resp: The method is not an OPTIONS, trying to replace the message\n");

		msg = cscf_get_request_from_reply(NULL);
		if(! msg || msg->first_line.type!=SIP_REQUEST || msg->first_line.u.request.method.len!=7||
			memcmp(msg->first_line.u.request.method.s,"OPTIONS",7)!=0){
					
			LOG(L_ERR,"BUG:"M_NAME":LRF_call_query_resp: The new message is not an OPTIONS request either\n");
			return CSCF_RETURN_ERROR;
		}
	}

	service = cscf_get_headers_content(msg, service_hdr_name);
	if(!service.len || !service.s){
		LOG(L_ERR, "ERR:"M_NAME":LRF_call_query_resp: could not find the service header in the OPTIONS, or could not be parsed\n");
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
		LOG(L_ERR, "ERR:"M_NAME":LRF_call_query_resp: could not found user data for uri %.*s and service %.*s\n",
				user_uri.len, user_uri.s, service.len, service.s);
		goto error;
	}

	if(!d->psap_uri.len || !d->psap_uri.s){
		LOG(L_ERR, "ERR: "M_NAME":LRF_call_query_resp: null psap uri\n");
		goto error;
	}

	if(d->loc && d->locsip_loc){
		LOG(L_DBG, "DBG: "M_NAME":LRF_call_query_resp: LRF has location information from the LOCSIP server\n");
		if(get_options_resp_body(&resp_body, d)){
			LOG(L_ERR, "ERR:"M_NAME":LRF_call_query_resp:could not get the OPTIONS response body\n");
			goto error;
		}
	}
	
	if(get_options_resp_headers(&headers, d)){
		LOG(L_ERR, "ERR:"M_NAME":LRF_call_query_resp:could not get the OPTIONS response headers\n");
		goto error;
	}
	
	//get crt trans	
	if(cscf_get_transaction(msg, &hash_index, &label)<0){
		LOG(L_ERR, "ERR:"M_NAME":LRF_call_query_resp: could not get the trans from the message\n");
		goto error;
	}

	if(tmb.t_lookup_ident(&trans, hash_index, label)!=1){
		LOG(L_ERR, "ERR:"M_NAME":LRF_call_query_resp: could not get the trans from the hash and index\n");
		goto error;
	}
	
	if(tmb.t_reply_with_body(trans, 200, "OK - PSAP found", resp_body.s, headers.s, "lrf" )!= 1){
		LOG(L_ERR, "ERR:"M_NAME":LRF_call_query_resp: could not send the response\n");
		goto error2;
	}

#ifndef TM_DEL_UNREF
	LOG(L_ERR, "ERR:"M_NAME":LRF_call_query_resp: options ref count %i\n", trans->ref_count);
#endif
	

	lrf_unlock(d->hash);
	if(resp_body.s)
		pkg_free(resp_body.s);
	if(headers.s)
		pkg_free(headers.s);

	return CSCF_RETURN_TRUE;

error2:
	//tmb.t_unref_ident(trans->hash_index, trans->label);

error:
	LOG(L_DBG, "DBG: "M_NAME":LRF_call_query_resp: error label\n");
	if(d)
		lrf_unlock(d->hash);
	LOG(L_DBG, "DBG: "M_NAME":LRF_call_query_resp: error label\n");
	//if(resp_body.s) pkg_free(resp_body.s);
	LOG(L_DBG, "DBG: "M_NAME":LRF_call_query_resp: error label\n");
	//if(headers.s)	pkg_free(headers.s);
	LOG(L_DBG, "DBG: "M_NAME":LRF_call_query_resp: error label\n");

	return CSCF_RETURN_FALSE;
}

/* Send a transaction reply
 * could be used after to send replies to the OPTIONS trans
 * @param msg  - (possible)the OPTIONS request from the ECSCF
 * @param str1 - reply code 
 * @param str2 - reply reason
 * @return CSCF_RETURN_TRUE if ok, or CSCF_RETURN_FALSE if error
 */
int LRF_options_empty_repl(struct sip_msg* msg, char*str1, char*str2){

	LOG(L_INFO, "INFO:"M_NAME":LRF_options_empty_repl\n");

	if (msg->first_line.u.request.method.len!=7||
		memcmp(msg->first_line.u.request.method.s,"OPTIONS",7)!=0){
		LOG(L_WARN,"WARN:"M_NAME":LRF_options_empty_repl: The method is not an OPTIONS, trying to replace the message\n");

		msg = cscf_get_request_from_reply(NULL);
		if(! msg || msg->first_line.type!=SIP_REQUEST || msg->first_line.u.request.method.len!=7||
			memcmp(msg->first_line.u.request.method.s,"OPTIONS",7)!=0){
					
			LOG(L_ERR,"BUG:"M_NAME":LRF_options_empty_repl: The new message is not an OPTIONS request either\n");
			return CSCF_RETURN_ERROR;
		}
	}

	cscf_reply_transactional(msg, 400, str2);
	return CSCF_RETURN_TRUE;
}


