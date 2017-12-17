/**
 * $Id: mgcf.c 1107 2011-05-19 08:31:46Z aon $
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
 * MGCF - MGCF helping functions
 * 
 *  \author Dragos Vingarzan dragos dot vingarzan -at- fokus dot fraunhofer dot de
 * 
 */
 

#include "../../sr_module.h"
#include "../../mem/mem.h"
#include "../../data_lump.h"

#include "mod.h"
#include "mgcf.h"

#include "sip.h"
#include "dlg_state.h"
#include "sip_body.h"


/* fixed parameter storage */
extern str mgcf_name_str;				/**< fixed name of the MGCF */
extern str mgcf_path_str;				/**< fixed path name of the MGCF (for indicating PSTN terminating) */
extern str mgcf_record_route_mo;		/**< record route header for PSTN origination dialogs */
extern str mgcf_record_route_mt;		/**< record route header for PSTN termination dialogs */
extern str mgcf_record_route_mo_uri;	/**< record route header for PSTN origination dialogs */
extern str mgcf_record_route_mt_uri;	/**< record route header for PSTN termination dialogs */
extern str mgcf_default_realm_str;		/**< fixed default realm */




/**
 * Determines if this is the PSTN Terminating case.
 * uses the indication in the first Route header
 * @param msg - the SIP message
 * @param str1 - not used
 * @param str2 - not used
 * @returns #CSCF_RETURN_TRUE if terminating case, #CSCF_RETURN_FALSE if not
 */
int M_PSTN_terminating(struct sip_msg *msg,char *str1,char *str2)
{
	int ret=CSCF_RETURN_FALSE;
	str route={0,0};
	int i;
	
	route = cscf_get_first_route(msg,0,0);
	if (!route.len){
		LOG(L_DBG,"DBG:"M_NAME":M_PSTN_terminating: No Route header.\n");
		goto done;
	}
	i=0;
	while(i<route.len && (route.s[i]==' ' ||route.s[i]=='\t'||route.s[i]=='<'))
		i++;
	route.s += i;
	route.len -= i;
	i=0;
	while(route.s[i]!=';' && route.s[i]!='>' && i<route.len)
		i++;
	route.len = i;
	
	if (route.len == mgcf_path_str.len &&
		strncasecmp(route.s,mgcf_path_str.s,mgcf_path_str.len)==0)
	{
		LOG(L_DBG,"DBG:"M_NAME":M_PSTN_terminating: Term indication found.\n");
		ret = CSCF_RETURN_TRUE;
		goto done;
	}else{
		LOG(L_DBG,"DBG:"M_NAME":M_PSTN_terminating: Term indication not found in <%.*s> as <%.*s>.\n",
			route.len,route.s,mgcf_path_str.len,mgcf_path_str.s);
	}	
	
done:
	return ret;
}

/**
 * Inserts the P-Charging-Vector header
 * P-Charging-Vector:
 * @param msg - the SIP message to add to
 * @param str1 - not used
 * @param str2 - not used
 * @returns #CSCF_RETURN_TRUE if ok or #CSCF_RETURN_FALSE on error
 */
int M_add_p_charging_vector(struct sip_msg *msg,char *str1,char*str2)
{
	return cscf_add_p_charging_vector(msg, NULL);
}


static str s_record_route2_s={"Record-Route: ",14};
static str s_record_route2_e={"\r\n",2};
/**
 * Enforce a response coming from the PSTN Gw to contain the same and proper Record Route headers sent in the
 * corresponding request.
 * @param msg - the SIP reply
 * @param str1 - not used
 * @param str2 - not used
 * @returns #CSCF_RETURN_TRUE on success, #CSCF_RETURN_ERROR on error
 */
int M_enforce_record_routes(struct sip_msg *msg,char *str1, char *str2)
{
	str hdr = {0,0};
	str rr_req = {0,0};
	struct sip_msg *req = cscf_get_request_from_reply(msg);

	LOG(L_INFO,"INF:"M_NAME":M_enforce_record_routes(): Enforcing RR in %d reply with the request ones\n",
			msg->first_line.u.reply.statuscode);

	if (!req){
		LOG(L_ERR,"ERR:"M_NAME":M_enforce_record_routes(): No transactional request found.\n");
		return CSCF_RETURN_ERROR;
	}
	
	if(!cscf_del_all_headers(msg, HDR_RECORDROUTE_T)){
		LOG(L_ERR,"ERR:"M_NAME":M_enforce_record_routes(): error while deleting headers\n");
		return CSCF_RETURN_ERROR;
	}
	
	rr_req = cscf_get_record_routes(req);
	if(rr_req.len){
		hdr.len = mgcf_record_route_mt.len + s_record_route2_s.len + rr_req.len+s_record_route2_e.len;
		if(!(hdr.s = pkg_malloc(hdr.len))){
		    LOG(L_ERR,"ERR:"M_NAME":M_enforce_record_routes(): Unable to allocate memory for hdr\n");
		    goto out_of_memory;
		}
		hdr.len = 0;
		STR_APPEND(hdr,mgcf_record_route_mt);
		STR_APPEND(hdr,s_record_route2_s);
		STR_APPEND(hdr,rr_req);
		STR_APPEND(hdr,s_record_route2_e);		
	}else{		
		LOG(L_INFO,"INFO:"M_NAME":M_enforce_record_routes(): Unable to get record routes - the RR should not be empty...\n");
		// still, let it continue, maybe it was empty on purpose
		//return CSCF_RETURN_ERROR;
		STR_PKG_DUP(hdr,mgcf_record_route_mt,"pkg");
	}
	
	if(!cscf_add_header_first(msg,&hdr,HDR_RECORDROUTE_T)){
		LOG(L_ERR,"ERR:"M_NAME":M_enforce_record_routes(): Unable to add header\n");
		if (hdr.s) pkg_free(hdr.s);
		return CSCF_RETURN_FALSE;
	}

	return CSCF_RETURN_TRUE;
	
out_of_memory:
	return CSCF_RETURN_ERROR;
}


/**
 * Returns the p_dialog_direction from the direction string.
 * @param direction - "orig" or "term"
 * @returns the p_dialog_direction if ok or #DLG_MOBILE_UNKNOWN if not found
 */
static inline enum m_dialog_direction get_dialog_direction(char *direction)
{
	if (!direction) {
		LOG(L_CRIT,"ERR:"M_NAME":get_dialog_direction(): Unknown direction NULL");
		return DLG_MOBILE_UNKNOWN;
	}
	switch(direction[0]){
		case 'o':
		case 'O':
		case '0':
			return DLG_MOBILE_ORIGINATING;
		case 't':
		case 'T':
		case '1':
			return DLG_MOBILE_TERMINATING;
		default:
			LOG(L_CRIT,"ERR:"M_NAME":get_dialog_direction(): Unknown direction %s",direction);
			return DLG_MOBILE_UNKNOWN;
	}
}

static str s_record_route_s={"Record-Route: <",15};
static str s_record_route_lr={";lr",3};
static str s_record_route_e={">\r\n",3};
/**
 * Record routes, with given user as parameter.
 * @param msg - the SIP message to add to
 * @param str1 - direction - "orig" or "term"
 * @param str2 - not used
 * @returns #CSCF_RETURN_TRUE if ok, #CSCF_RETURN_FALSE if not or #CSCF_RETURN_BREAK on error
 */ 
int M_record_route_simple(struct sip_msg *msg,char *str1,char *str2)
{
	str rr;
	str u = {0,0},scheme={0,0},mgcf={0,0};
	
	enum m_dialog_direction dir = get_dialog_direction(str1);
	
	switch (dir){
		case DLG_MOBILE_ORIGINATING:
			STR_PKG_DUP(rr,mgcf_record_route_mo,"pkg");
			break;
		case DLG_MOBILE_TERMINATING:
			STR_PKG_DUP(rr,mgcf_record_route_mt,"pkg");
			break;
		default:
			u.s = str1;
			u.len = strlen(str1);
			if (mgcf_name_str.len>4 &&
				strncasecmp(mgcf_name_str.s,"sip:",4)==0){
				scheme.s = mgcf_name_str.s;
				scheme.len = 4;
			}else if (mgcf_name_str.len>5 &&
				strncasecmp(mgcf_name_str.s,"sips:",5)==0){
				scheme.s = mgcf_name_str.s;
				scheme.len = 4;
			}
			mgcf.s = scheme.s+scheme.len;
			mgcf.len = mgcf_name_str.len - scheme.len;
			
			rr.len = s_record_route_s.len+scheme.len+u.len+1+mgcf.len+s_record_route_lr.len+s_record_route_e.len;
			rr.s = pkg_malloc(rr.len);
			if (!rr.s){
				LOG(L_ERR,"ERR:"M_NAME":P_record_route: error allocating %d bytes!\n",rr.len);	
				return CSCF_RETURN_BREAK;
			}
			rr.len = 0;
			STR_APPEND(rr,s_record_route_s);
			STR_APPEND(rr,scheme);
			STR_APPEND(rr,u);
			rr.s[rr.len++]='@';
			STR_APPEND(rr,mgcf);
			STR_APPEND(rr,s_record_route_lr);
			STR_APPEND(rr,s_record_route_e);					
	}
	
	if (cscf_add_header_first(msg,&rr,HDR_RECORDROUTE_T)) return CSCF_RETURN_TRUE;
	else{
		if (rr.s) pkg_free(rr.s);
		return CSCF_RETURN_BREAK;
	}
out_of_memory:
	return CSCF_RETURN_ERROR;	
}

/**
 * Find out if a message is within a saved dialog.
 * @param msg - the SIP message
 * @param str1 - the direction of the dialog ("orig"/"term")
 * @param str2 - not used
 * @returns #CSCF_RETURN_TRUE if in, #CSCF_RETURN_FALSE else or #CSCF_RETURN_BREAK on error
 */
int M_is_in_dialog_simple(struct sip_msg* msg, char* str1, char* str2)
{
	str mark,route;
	enum m_dialog_direction dir;
	int i;

	dir = get_dialog_direction(str1);

	switch (dir){
		case DLG_MOBILE_ORIGINATING:
			mark = mgcf_record_route_mo_uri;
			break;
		case DLG_MOBILE_TERMINATING:
			mark = mgcf_record_route_mt_uri;
			break;
		default:
			LOG(L_ERR,"ERR:"M_NAME":M_is_in_dialog: not implemented for %s\n",str1);
			return CSCF_RETURN_FALSE;
	}
	route = cscf_get_first_route(msg,0,0);
	if (!route.len){
		LOG(L_DBG,"DBG:"M_NAME":M_is_in_dialog: No Route header.\n");
		return CSCF_RETURN_FALSE;
	}
	i=0;
	while(i<route.len && (route.s[i]==' ' ||route.s[i]=='\t'||route.s[i]=='<'))
		i++;
	route.s += i;
	route.len -= i;
	i=0;
	while(route.s[i]!=';' && route.s[i]!='>' && i<route.len)
		i++;
	route.len = i;
	
	if (route.len == mark.len &&
		strncasecmp(route.s,mark.s,mark.len)==0)
	{
		LOG(L_INFO,"DBG:"M_NAME":M_is_in_dialog: Dialog %s indication found.\n",str1);
		return CSCF_RETURN_TRUE;
	}else{
		LOG(L_INFO,"DBG:"M_NAME":M_is_in_dialog: Dialog indication not found in <%.*s> as <%.*s>.\n",
			route.len,route.s,mark.len,mark.s);
		return CSCF_RETURN_FALSE;
	}		
}

static str p_asserted_identity_s={"P-Asserted-Identity: ",21};
static str p_asserted_identity_m={"<",1};
static str p_asserted_identity_e={">\r\n",3};
/**
 * Asserts the P-Preferred-Identity if registered and inserts the P-Asserted-Identity.
 * @param msg - the SIP message
 * @param str1 - the realm to look into
 * @param str2 - not used
 * @returns #CSCF_RETURN_TRUE if asseted, #CSCF_RETURN_FALSE if not, #CSCF_RETURN_ERROR on error 
 */
int M_assert_identity(struct sip_msg *msg,char *str1,char *str2)
{
	int ret=CSCF_RETURN_FALSE;
	struct hdr_field *h=0;
	name_addr_t preferred,asserted;
	str x={0,0};

	LOG(L_INFO,"INF:"M_NAME":M_assert_identity: Asserting Identity\n");
//	print_r(L_INFO);
	
	
	preferred = cscf_get_preferred_identity(msg,&h);
	if (preferred.uri.len==0) {
		/* Fallback to From header */
		preferred = cscf_get_preferred_identity_from_from(msg,&h);
		/* but don't remove it */
		h=0;
	}
	
	LOG(L_INFO,"DBG:"M_NAME":M_assert_identity: Asserting: %.*s\n",
		preferred.uri.len,preferred.uri.s);

	asserted = preferred;
	if (!asserted.uri.len){
		ret = CSCF_RETURN_FALSE;	
	}else{
		if (h) cscf_del_header(msg,h);
		x.len = p_asserted_identity_s.len+asserted.name.len+p_asserted_identity_m.len + 
			asserted.uri.len+p_asserted_identity_e.len;
		x.s = pkg_malloc(x.len);
		if (!x.s){
			LOG(L_ERR, "ERR"M_NAME":M_assert_identity: Error allocating %d bytes\n",
				x.len);
			x.len=0;
			goto error;		
		}
		x.len=0;
		STR_APPEND(x,p_asserted_identity_s);
		STR_APPEND(x,asserted.name);
		STR_APPEND(x,p_asserted_identity_m);
		STR_APPEND(x,asserted.uri);
		STR_APPEND(x,p_asserted_identity_e);

		LOG(L_INFO,"DBG:"M_NAME":M_assert_identity: Adding header: <%.*s>\n",
			x.len,x.s);
		
		if (cscf_add_header_first(msg,&x,HDR_OTHER_T))
			ret = CSCF_RETURN_TRUE;
		else
			ret = CSCF_RETURN_FALSE;	
	}
	
	
	return ret;
error:
	ret=CSCF_RETURN_ERROR;
	return ret;	
}

/**
 * Asserts the P-Called-Identity
 * @param rpl - the SIP response message
 * @param str1 - not used
 * @param str2 - not used
 * @returns #CSCF_RETURN_TRUE if asseted, #CSCF_RETURN_FALSE if not, #CSCF_RETURN_ERROR on error 
 */
int M_assert_called_identity(struct sip_msg *rpl,char *str1,char *str2)
{
	int ret=CSCF_RETURN_FALSE;
	struct hdr_field *h=0;
	str called={0,0},x={0,0};
	struct sip_msg *req = cscf_get_request_from_reply(rpl);

	LOG(L_INFO,"INF:"M_NAME":P_assert_called_identity: Asserting Identity\n");

	cscf_get_preferred_identity(rpl,&h);
	cscf_del_header(rpl,h);

	if (!req){
		LOG(L_INFO,"INF:"M_NAME":P_assert_called_identity: Error finding correspondent request.\n");
		goto error;		
	}

	called = cscf_get_called_party_id(req,&h);
	if (!called.len){
		cscf_get_to_uri(rpl,&called);
	}
	if (!called.len){
		ret = CSCF_RETURN_FALSE;	
	}else{
		x.len = p_asserted_identity_s.len+p_asserted_identity_m.len+called.len+p_asserted_identity_e.len;
		x.s = pkg_malloc(x.len);
		if (!x.s){
			LOG(L_ERR, "ERR"M_NAME":P_assert_called_identity: Error allocating %d bytes\n",
				x.len);
			x.len=0;
			goto error;		
		}
		x.len=0;
		STR_APPEND(x,p_asserted_identity_s);
		STR_APPEND(x,p_asserted_identity_m);
		STR_APPEND(x,called);
		STR_APPEND(x,p_asserted_identity_e);
		
		if (cscf_add_header(rpl,&x,HDR_OTHER_T))
			ret = CSCF_RETURN_TRUE;
		else
			ret = CSCF_RETURN_FALSE;	
	}
	
	return ret;
error:
	ret=CSCF_RETURN_FALSE;
	return ret;	
}


static str s_multipart={"multipart/",10};
static str content_length_s={"Content-Length: ",16};
static str content_length_e={"\r\n",2};
/**
 * Looks for a part with the given content_type and replaces the original body with that part.
 * The Content-Type and Content-Length header is also replaced. If the original body is not a multipart one, it is left untouched.
 * (this is usually useful to filter for example application/sdp out of a multipart/mixed body, like the one
 * sent by many trunking interfaces to relay also the ISUP information)
 * @param msg
 * @param content_type
 * @param str2
 * @return CSCF_RETURN_TRUE if the message had before or after the required content_type, CSCF_RETURN_FALSE if not
 * or CSCF_RETURN_ERROR
 */
int M_filter_content_type(struct sip_msg *msg,char *content_type,char *str2)
{
	str search_content_type ={content_type,strlen(content_type)};
	str body_content_type = cscf_get_content_type(msg);
	str new_body,new_part,new_content_len,x={0,0};
	struct lump* anchor;	
	
	LOG(L_DBG,"DBG:"M_NAME":M_filter_content_type: filtering content type %s\n",content_type);
	
	if (body_content_type.len>=s_multipart.len &&
			strncasecmp(body_content_type.s,s_multipart.s,s_multipart.len)==0){
		new_body = cscf_get_body_with_type_from_body(cscf_get_body(msg),body_content_type,search_content_type,&new_part);
		if (new_body.s){
			LOG(L_INFO,"INFO:"M_NAME":M_filter_content_type: found body with type %s, and len %d\n",
					content_type,new_body.len);
			
			/* add new Content-Length header */
			new_content_len.len = content_length_s.len + 12 + content_length_e.len;
			new_content_len.s = pkg_malloc(new_content_len.len);
			if (!new_content_len.s){
				LOG(L_ERR, "ERR:"M_NAME":M_filter_content_type: Error allocating %d bytes\n",
					x.len);
				x.len=0;
				return CSCF_RETURN_ERROR;		
			}
			new_content_len.len = sprintf(new_content_len.s,"%.*s%d%.*s",
					content_length_s.len,content_length_s.s,
					new_body.len,
					content_length_e.len,content_length_e.s);
			if (!cscf_add_header(msg,&new_content_len,HDR_CONTENTLENGTH_T)) 
				return CSCF_RETURN_ERROR;

			/* remove old headers */
			cscf_del_all_headers(msg,HDR_CONTENTTYPE_T);
			cscf_del_all_headers(msg,HDR_CONTENTLENGTH_T);
			
			/* remove old body */
			if ( parse_headers(msg, HDR_EOH_F, 0)==-1 )
				return CSCF_RETURN_ERROR;
			
			if (!del_lump(msg,msg->unparsed-msg->buf,msg->len-(msg->unparsed-msg->buf),0)){
				LOG(L_ERR,"ERR:"M_NAME":M_filter_content_type: Error adding del lump\n");
				return CSCF_RETURN_ERROR;		
			}		

			/* add new part (headers+body) */
			anchor = anchor_lump(msg,msg->unparsed - msg->buf, 0 , 0);
			if (anchor == NULL) {
				LOG(L_ERR, "ERR:"M_NAME":M_filter_content_type: anchor_lump failed\n");
				return CSCF_RETURN_ERROR;
			}
			STR_PKG_DUP(x,new_part,"pkg");
			if (!insert_new_lump_after(anchor, x.s,x.len,0)){
				LOG(L_ERR, "ERR:"M_NAME":M_filter_content_type: error creating lump for header\n" );
				return CSCF_RETURN_ERROR;
			}	
			LOG(L_DBG,"DBG:"M_NAME":M_filter_content_type: content-type %s found in multipart and replaced.\n",content_type);											
			return CSCF_RETURN_TRUE;
		} else {
			LOG(L_DBG,"DBG:"M_NAME":M_filter_content_type: content-type %s not found in multipart body.\n",content_type);
			return CSCF_RETURN_FALSE;
		}
	} else {
		if (body_content_type.len>=search_content_type.len &&
				strncasecmp(body_content_type.s,search_content_type.s,search_content_type.len)==0){
			LOG(L_DBG,"DBG:"M_NAME":M_filter_content_type: content-type %s found as main body - nothing to do.\n",content_type);											
			return CSCF_RETURN_TRUE;
		} else {
			LOG(L_DBG,"DBG:"M_NAME":M_filter_content_type: content-type %s not found in body.\n",content_type);			
			return CSCF_RETURN_FALSE;			
		}
	}
out_of_memory:
	return CSCF_RETURN_ERROR;
}

/**
 * Drops the body of the message
 * @param msg
 * @return CSCF_RETURN_TRUE on success or CSCF_RETURN_ERROR
 */
int M_drop_body(struct sip_msg *msg,char *str1,char *str2)
{
	str new_content_len,x={0,0};
	char *body_start = get_body(msg);
	
	if (cscf_get_content_len(msg)<=0){
		LOG(L_DBG,"DBG:"M_NAME":M_drop_body: old body was empty - nothing to do\n");
		return CSCF_RETURN_TRUE;
	}
	
	/* add new Content-Length header */
	new_content_len.len = content_length_s.len + 1 + content_length_e.len;
	new_content_len.s = pkg_malloc(new_content_len.len);
	if (!new_content_len.s){
		LOG(L_ERR, "ERR:"M_NAME":M_drop_body: Error allocating %d bytes\n",
			x.len);
		x.len=0;
		goto error;		
	}
	new_content_len.len = sprintf(new_content_len.s,"%.*s%d%.*s",
			content_length_s.len,content_length_s.s,
			0,
			content_length_e.len,content_length_e.s);
	if (!cscf_add_header(msg,&new_content_len,HDR_CONTENTLENGTH_T)) 
		goto error;

	/* remove old headers */
	cscf_del_all_headers(msg,HDR_CONTENTTYPE_T);
	cscf_del_all_headers(msg,HDR_CONTENTLENGTH_T);
			
	/* remove old body */
	if ( parse_headers(msg, HDR_EOH_F, 0)==-1 )
		goto error;
	get_body(msg);
	if (!del_lump(msg,body_start-msg->buf,msg->len-(body_start-msg->buf),0)){
		LOG(L_ERR,"ERR:"M_NAME":M_drop_body: Error adding del lump\n");
		goto error;		
	}		

	LOG(L_DBG,"DBG:"M_NAME":M_drop_body: done\n");
	return CSCF_RETURN_TRUE;
	
error:	
	LOG(L_ERR,"ERR:"M_NAME":M_drop_body: Error on deletion\n");
	return CSCF_RETURN_ERROR;
}


