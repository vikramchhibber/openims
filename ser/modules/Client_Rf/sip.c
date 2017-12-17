/**
 * $Id: sip.c 1114 2011-06-09 14:17:50Z aon $
 *  
 * Copyright (C) 2004-2006 FhG Fokus
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
 * Main SIP Operations 
 * 
 * \author Dragos Vingarzan vingarzan -at- fokus dot fraunhofer dot de
 * \author Alberto Diez - get_from_tag,get_to_tag,get_from_uri added
 * \author Ancuta Onofrei	andreea dot ancuta dot onofrei -at- fokus dot fraunhofer dot de
 * 
 * Copyright (C) 2005 FhG Fokus
 * 		
 */

/*
 * Please be careful about parsing the content of headers from shared-memory
 * all headers are parsed and stored in tm shared-memory
 * but only via/from/to/cseq/auth contents are parsed and stored to the shm
 * so if other headers' contents need to be parsed, be sure to free the memory  
 * see: 
 * http://lists.sip-router.org/pipermail/sr-dev/2009-December/005314.html
 * http://lists.berlios.de/pipermail/openimscore-cscf/2009-December/002349.html
 * Feb, 1, 2010,  Min Wang ( wang@basis-audionet.com )
 */

#ifndef WHARF

#include "sip.h"

#include "../../mem/mem.h"
#include "../../data_lump.h"
#include "../../data_lump_rpl.h"
#include "../../parser/parse_to.h"
#include "../../parser/parse_from.h"
#include "../../parser/parse_expires.h"
#include "../../parser/parse_via.h"
#include "../../parser/parse_content.h"
#include "../../parser/parse_nameaddr.h"
#include "../../parser/parse_param.h"
#include "../../parser/contact/contact.h"
#include "../../parser/contact/parse_contact.h"

#include "../../modules/tm/tm_load.h"

#include "mod.h"

/**
 * Duplicate a str, safely.
 * \Note This checks if:
 *  - src was an empty string
 *  - malloc failed
 * \Note On any error, the dst values are reset for safety
 * \Note A label "out_of_memory" must be defined in the calling function to handle
 * allocation errors. 
 * @param dst - destination str
 * @param src - source src
 * @param mem - type of mem to duplicate into (shm/pkg)
 */
#define str_dup(dst,src,mem) \
do {\
	if ((src).len) {\
		(dst).s = mem##_malloc((src).len);\
		if (!(dst).s){\
			LOG(L_ERR,"Error allocating %d bytes in %s!\n",(src).len,#mem);\
			(dst).len = 0;\
			goto out_of_memory;\
		}\
		memcpy((dst).s,(src).s,(src).len);\
		(dst).len = (src).len;\
	}else{\
		(dst).s=0;(dst).len=0;\
	}\
} while (0)

/**
 * Frees a str content.
 * @param x - the str to free
 * @param mem - type of memory that the content is using (shm/pkg)
 */
#define str_free(x,mem) \
do {\
	if ((x).s) mem##_free((x).s);\
	(x).s=0;(x).len=0;\
} while(0)


/**
 * Looks for the Call-ID header
 * @param msg - the sip message
 * @param hr - ptr to return the found hdr_field 
 * @returns the callid value
 */
str cscf_get_call_id(struct sip_msg *msg,struct hdr_field **hr)
{
	struct hdr_field *h;
	str call_id={0,0};
	if (hr) *hr = 0;	
	if (!msg) return call_id;
	if (parse_headers(msg, HDR_CALLID_F, 0)<0){
		LOG(L_ERR,"ERR:"M_NAME":cscf_get_call_id: error parsing headers\n");
		return call_id;
	}
	h = msg->callid;
	if (!h){
		LOG(L_ERR,"ERR:"M_NAME":cscf_get_call_id: Header Call-ID not found\n");
		return call_id;
	}
	if (hr) *hr = h;
	call_id = h->body;	
	return call_id;
}

/**
 * Get the local uri from the From header.
 * @param msg - the message to look into
 * @param local_uri - ptr to fill with the value
 * @returns 1 on success or 0 on error
 */  
int cscf_get_from_uri(struct sip_msg* msg,str *local_uri)
{	
	struct to_body* from;

	if (!msg || parse_from_header(msg)<0 || !msg->from || !msg->from->parsed){
		LOG(L_ERR,"ERR:"M_NAME":cscf_get_from_uri: error parsing From header\n");
		if (local_uri) {local_uri->s = 0;local_uri->len = 0;}
		return 0;
	}
	from = msg->from->parsed;		
	if (local_uri) *local_uri = from->uri;
	return 1;
	
}

/**
 * Get the local uri from the To header.
 * @param msg - the message to look into
 * @param local_uri - ptr to fill with the value
 * @returns 1 on success or 0 on error
 */  
int cscf_get_to_uri(struct sip_msg* msg,str *local_uri)
{	
	struct to_body* to=	NULL;

	if (!msg || !msg->to || !msg->to->parsed || parse_headers(msg,HDR_TO_F,0)==-1 ){
		LOG(L_ERR,"ERR:"M_NAME":cscf_get_to_uri: error parsing TO header\n");
		if (local_uri) {local_uri->s = 0;local_uri->len = 0;}
		return 0;
	}
	to = msg->to->parsed;		
	if (local_uri) *local_uri = to->uri;
	return 1;
	
}

/**
 * Returns the expires value from the Expires header in the message.
 * It searches into the Expires header and if not found returns -1
 * @param msg - the SIP message, if available
 * @is_shm - msg from from shared memory 
 * @returns the value of the expire or -1 if not found
 */
int cscf_get_expires_hdr(struct sip_msg *msg, int is_shm)
{
	exp_body_t *exp;
	int expires;
	if (!msg) return -1;
	/*first search in Expires header */
	if (parse_headers(msg,HDR_EXPIRES_F,0)!=0) {
		LOG(L_ERR,"ERR:"M_NAME":cscf_get_expires_hdr: Error parsing until header EXPIRES: \n");
		return -1;
	}
	if (msg->expires){		
		if (!msg->expires->parsed) {
			parse_expires(msg->expires);
		}
		if (msg->expires->parsed) {
			exp = (exp_body_t*) msg->expires->parsed;
			if (exp->valid) {
				expires = exp->val;
				LOG(L_DBG,"DBG:"M_NAME":cscf_get_expires_hdr: <%d> \n",expires);
				if(is_shm) {
					free_expires((exp_body_t**)&exp);
					msg->expires->parsed = 0;	
				}
				return expires;
			}
		}
	}
	
	return -1;
}

/**
 * Looks for the Event header and extracts its content.
 * @param msg - the sip message
 * @returns the string event value or an empty string if none found
 */
str cscf_get_event(struct sip_msg *msg)
{
	str e={0,0};
	if (!msg) return e;
	if (parse_headers(msg, HDR_EVENT_F, 0) != -1 && msg->event &&
	    msg->event->body.len > 0) 
	{
		e.len = msg->event->body.len;
		e.s = msg->event->body.s;
	}
	return e;
}

str s_asserted_identity={"P-Asserted-Identity",19};
/**
 * Looks for the P-Asserted-Identity header and extracts its content
 * @param msg - the sip message
 * @returns the asserted identity
 */
str cscf_get_asserted_identity(struct sip_msg *msg)
{
	name_addr_t id;
	struct hdr_field *h;
	rr_t *r;
	memset(&id,0,sizeof(name_addr_t));
	if (!msg) return id.uri;
	if (parse_headers(msg, HDR_EOH_F, 0)<0) {
		return id.uri;
	}
	h = msg->headers;
	while(h)
	{
		if (h->name.len == s_asserted_identity.len  &&
			strncasecmp(h->name.s,s_asserted_identity.s,s_asserted_identity.len)==0)
		{
			if (parse_rr(h)<0){
				//This might be an old client
				LOG(L_CRIT,"WARN:"M_NAME":cscf_get_asserted_identity: P-Asserted-Identity header must contain a Nameaddr!!! Fix the client!\n");
				id.name.s = h->body.s;
				id.name.len = 0;
				id.len = h->body.len;
				id.uri = h->body;
				while(id.uri.len && (id.uri.s[0]==' ' || id.uri.s[0]=='\t' || id.uri.s[0]=='<')){
					id.uri.s = id.uri.s+1;
					id.uri.len --;
				}
				while(id.uri.len && (id.uri.s[id.uri.len-1]==' ' || id.uri.s[id.uri.len-1]=='\t' || id.uri.s[id.uri.len-1]=='>')){
					id.uri.len--;
				}
				return id.uri;	
			}
			r = (rr_t*) h->parsed;
			id = r->nameaddr; 
			free_rr(&r);
			h->parsed=r;
			//LOG(L_CRIT,"%.*s",id.uri.len,id.uri.s);
			return id.uri;
		}
		h = h->next;
	}
	return id.uri;
}

/**
 * Returns the first header structure for a given header name. 
 * @param msg - the SIP message to look into
 * @param header_name - the name of the header to search for
 * @returns the hdr_field on success or NULL if not found  
 */
struct hdr_field* cscf_get_header(struct sip_msg * msg , str header_name)
{		
	struct hdr_field *h = 0;
	/*if (parse_headers(msg, HDR_EOH_F, 0)<0){
		LOG(L_ERR,"ERR:"M_NAME":cscf_get_path: error parsing headers\n");
		return NULL;
	}*/
	h = msg->headers;
	while(h){
		if (h->name.len==header_name.len &&
			strncasecmp(h->name.s,header_name.s,header_name.len)==0)
				break;
		h = h->next;
	}
	return h;
}



static str p_charging_vector={"P-Charging-Vector",17};
/*static str p_charging_vector_1={"\";icid-generated-at=",20};
static str p_charging_vector_2={";orig-ioi=\"",11};
static str p_charging_vector_e={"\"\r\n",3};
static char hex_chars[17]="0123456789abcdef";*/

int get_param_value(str header_body, str param_name, int * start_value_index, int  * value_length){
	
	char * p;
	char * end = header_body.s + header_body.len;
	
	*start_value_index = 0;
	*value_length = 0;	

	p = header_body.s;
loop:
	while(p<end && ((*p==' ')||(*p==';')||(*p==':')||(*p=='=')||(*p=='"')||(*p=='\r')||(*p=='\t')||(*p=='\n'))){
		*start_value_index = *start_value_index +1;
		p++;
	}
	if(p==end)
		return 0;
	if(strncmp(p, param_name.s, param_name.len) == 0){
		p+=param_name.len;
		*start_value_index = *start_value_index+param_name.len;
		while(p<end && ((*p==' ')||(*p==';')||(*p==':')||(*p=='=')||(*p=='"')||(*p=='\r')||(*p=='\t')||(*p=='\n'))){
			*start_value_index = *start_value_index +1;
			p++;
		}
		if(p==end) return 0;
		while(p<end && !((*p==' ')||(*p==';')||(*p==':')||(*p=='=')||(*p=='"')||(*p=='\r')||(*p=='\t')||(*p=='\n'))){
			*value_length = *value_length +1;
			p++;
		}

		return 1;
	
	}else {
		while(p<end && (*p!=';')){
			*start_value_index = *start_value_index +1;
			p++;
		}
		if(p==end)
			return 0;

		goto loop;
	}
}

/**
 * Retrieves the P-Charging-Vector header information
 * P-Charging-Vector:
 * @param msg - the SIP message to retrieve from
 * @returns #CSCF_RETURN_TRUE if ok or #CSCF_RETURN_FALSE on error
 */
int cscf_get_p_charging_vector(struct sip_msg *msg, str * icid, str * orig_ioi, str * term_ioi)
{
	struct hdr_field* header = 0;
	str header_body = {0,0};
	int index, len;
	str param_name;

	LOG(L_DBG, "get_p_charging_vector\n");	
	header = cscf_get_header(msg, p_charging_vector);
	if(!header){
		LOG(L_DBG, "no header %.*s was found\n", p_charging_vector.len, p_charging_vector.s);
		return 0;
	}
	if(!header->body.s || !header->body.len)
		return 0;
	
	str_dup(header_body, header->body, pkg);

	//LOG(L_DBG, "p_charging_vector body is %.*s\n", header_body.len, header_body.s);
	if(icid){
		param_name.s = "icid-value";
		param_name.len = 10;

		if(get_param_value(header_body, param_name, &index, &len)){
		
			icid->s = pkg_malloc(len * sizeof(char));
			if(!icid->s)	goto out_of_memory;
			memcpy(icid->s, header->body.s + index, len*sizeof(char));
			icid->len = len;

			LOG(L_DBG, "param %.*s is %.*s\n",
					param_name.len, param_name.s, icid->len, icid->s);
		}
	}	

	if(orig_ioi){
		param_name.s = "orig-ioi";
		param_name.len = 8;

		if(get_param_value(header_body, param_name, &index, &len)){
			orig_ioi->len = len;
			orig_ioi->s = header->body.s + index;
			LOG(L_DBG, "param %.*s is %.*s\n",
					param_name.len, param_name.s, orig_ioi->len, orig_ioi->s);
		}
	}

	if(term_ioi){
		param_name.s = "term-ioi";
		param_name.len = 8;

		if(get_param_value(header_body, param_name, &index, &len)){
			term_ioi->len = len;
			term_ioi->s = header->body.s + index;
			LOG(L_DBG, "param %.*s is %.*s\n",
					param_name.len, param_name.s, term_ioi->len, term_ioi->s);
		}
	}

	str_free(header_body, pkg);
	return 1;
out_of_memory:
	LOG(L_ERR, "ERR:"M_NAME":cscf_get_p_charging_vector:out of pkg memory\n");
	return 0;
}

/**
 * Looks for the Content-Type header and extracts its content.
 * @param msg - the sip message
 * @returns the content-type string, or an empty string if not found
 */
str cscf_get_content_type(struct sip_msg *msg)
{
	str ct={0,0};
	if (!msg) return ct;
	if (parse_headers(msg, HDR_CONTENTTYPE_F, 0) != -1 && msg->content_type){
		ct = msg->content_type->body;		
		while(ct.s[0]==' '||ct.s[0]=='\t'){
			ct.s++;
			ct.len--;
		}
		while(ct.s[ct.len-1]==' '||ct.s[ct.len-1]=='\t')
			ct.len--;
	}
	return ct;
}

/**
 * Returns the first header structure for a given header name. 
 * @param msg - the SIP message to look into
 * @param header_name - the name of the header to search for
 * @returns the hdr_field on success or NULL if not found  
 */
str cscf_get_body(struct sip_msg * msg)
{		
	str x={0,0};
	x.s = get_body(msg);	
	if (x.s==0) return x;
	if (parse_headers(msg,HDR_CONTENTLENGTH_F,0)!=0) {
		LOG(L_DBG,"DBG:"M_NAME":cscf_get_body: Error parsing until header Content-Length: \n");
		return x;
	}
	if  (msg->content_length->parsed==NULL) {
		LOG(L_ERR," body <%.*s>\n",msg->content_length->body.len,msg->content_length->body.s);
		parse_content_length(msg->content_length->body.s,
			msg->content_length->body.s+msg->content_length->body.len,&(x.len));
		msg->content_length->parsed=(void*)(long)(x.len);
	}else 
		x.len = (long)msg->content_length->parsed;
	return x;
}

#endif /* WHARF*/
