/**
 * $Id: sip_body.c 920 2010-03-05 19:07:50Z vingarzan $
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
 * P/I/S-CSCF/etc Module - SIP Message Body Operations 
 * 
 *  \author Dragos Vingarzan dragos dot vingarzan -at- fokus dot fraunhofer dot de
 * 
 */
 

#include "../../sr_module.h"
#include "../../mem/mem.h"

#include "../../parser/msg_parser.h"

#include "sip_body.h"
#include "mod.h"
#include "sip.h"

static str s_boundary={"boundary",8};
/**
 * Extract the boundary parameter of a multipart/mixed content type header body
 * TODO - make this quote-safe - don't parse what is inside
 * @param content_type - the body of the Content-type header
 * @return the boundary or an empty string if none found
 */
str cscf_get_multipart_boundary(str content_type)
{
	str x={0,0};
	int k;
	int quotes=0;
	
	LOG(L_DBG,"DBG:"M_NAME":cscf_get_multipart_boundary(): looking in <%.*s>\n",
			content_type.len,content_type.s);
	
	for(k=0;k<content_type.len;k++)
		if (content_type.s[k]==';'){
			k++;
			/* found a parameter */
			while(k<content_type.len && 
					(content_type.s[k]==' '||content_type.s[k]=='\t'))
				k++;		
			
			if (k>=content_type.len) 
				break;
			
			LOG(L_DBG,"DBG:"M_NAME":cscf_get_multipart_boundary(): found parameter <%.*s>\n",
					content_type.len-k,content_type.s+k);
			if (k+s_boundary.len<=content_type.len &&
				strncasecmp(content_type.s+k,s_boundary.s,s_boundary.len)==0){
				LOG(L_DBG,"DBG:"M_NAME":cscf_get_multipart_boundary(): found boundary parameter\n");
				/* found the boundary parameter */
				k+=s_boundary.len;
				while(k<content_type.len && 
						(content_type.s[k]==' '||content_type.s[k]=='\t'))
					k++;
				
				if (k>=content_type.len-1 ||content_type.s[k]!='=')
					continue;					
				k++;
				if (k>=content_type.len){
					LOG(L_ERR,"ERR:"M_NAME":cscf_get_multipart_boundary(): empty boundary parameter\n");
					return x;
				}
				if (content_type.s[k]=='"'){
					quotes = 1;
					x.s = content_type.s+k+1;
					k++;
					while(k<content_type.len && content_type.s[k]!='"')
						k++;
					x.len = content_type.s+k-x.s;
				}else{
					quotes = 0;
					x.s = content_type.s+k;
					k++;					
					while(k<content_type.len && content_type.s[k]!=' '&& content_type.s[k]!='\t'&& content_type.s[k]!=';')
						k++;
					x.len = content_type.s+k-x.s;
				}
				LOG(L_INFO,"INFO:"M_NAME":cscf_get_multipart_boundary(): boundary parameter was <%.*s>\n",x.len,x.s);
				return x;
			}
		}
	LOG(L_ERR,"ERR:"M_NAME":cscf_get_multipart_boundary(): no boundary parameter found\n");
	return x;
}


/**
 * Finds and returns the first part of a multipart body.
 * To get the next, call with the updated position.
 * @param body - the body to look into
 * @param boundary - what is the boundary separator
 * @param position - 0 to start from beginning or where was the search left at (last call's updated value)
 * @returns the next part. If *position>=body.len then none found
 */
str cscf_get_multipart_part(str body,str boundary,int *position)
{
	str x={0,0};
	int j,k=(position?*position:0);
	int found=0;
	
	/* look for --boundary */
	while(k<body.len-1){
		if  (body.s[k]=='-'&&body.s[k+1]=='-' &&
				k+2+boundary.len<=body.len &&
				memcmp(body.s+k+2,boundary.s,boundary.len)==0){
			found = 1;
			k+=2+boundary.len;
			break;
		}
		k++;
	}
	
	/* if not found or -- is next, then it's finished */
	if (!found||k>body.len-2||(body.s[k]=='-'&&body.s[k+1]=='-'))
		goto nomoreleft;
	
	/* skip until after CRLF */
	while(k<body.len-1&& 
			(body.s[k]!='\r'||body.s[k+1]!='\n'))
		k++;
	
	if (k>=body.len-1)
		goto nomoreleft;
	
	/* start found */
	k+=2;
	j = k;
	x.s = body.s+j;
	
	/* now the end is at the first CRLF--boundary */
	while(k<body.len-3-boundary.len&& 
			(body.s[k]!='\r'||body.s[k+1]!='\n'||
			 body.s[k+2]!='-'||body.s[k+3]!='-'||
			 memcmp(body.s+k+4,boundary.s,boundary.len)!=0)
		)
		k++;
	if (k>=body.len-3-boundary.len)
		goto nomoreleft;	
	x.len = k-j;
	if (position) *position = k;
	LOG(L_DBG,"DBG:"M_NAME":cscf_get_multipart_part(): Found part is:\n%.*s\n---\n",
			x.len,x.s);	
	return x;
nomoreleft:
	if (position) *position = body.len;
	x.s=0;
	x.len=0;
	return x;
}

/**
 * Extracts the body out of a multipart part by skipping past the possible headers
 * @param part
 * @return
 */
str cscf_get_multipart_body(str part)
{
	str x={0,0};
	int k=0;
	
	/* look for first CRLFCRLF and return from there till end */
	while(k<part.len-3&& 
			(part.s[k]!='\r'||part.s[k+1]!='\n'||
			 part.s[k+2]!='\r'||part.s[k+3]!='\n'))
		k++;
	k+=4;
	x.s = part.s+k;
	x.len = part.len-k;
	if (x.len<0) x.len=0;	
	LOG(L_DBG,"DBG:"M_NAME":cscf_get_multipart_body(): Found body is:\n%.*s\n---\n",
			x.len,x.s);	
	return x;
}

/**
 * Gets the next hdr parsed out of a multipart part.
 * Don't forget to clean-up with clean_hdr_field(hdr) or you'll have memory leaks from hdr->parsed.
 * @param part - part to look into
 * @param position - where to start or 
 * @param hdr - the hdr field to parse into
 * @return 1 on success, -1 on error or 0 on no more headers left
 */
int cscf_get_multipart_hdr_field(str part, int *position, struct hdr_field* hdr)
{
	char *start=part.s+(position?*position:0);
	char *p = get_hdr_field(start,part.s+part.len,hdr);
	*position = p-part.s;
	if (hdr->type==HDR_EOH_T)
		return 0;
	else if (hdr->type==HDR_ERROR_T)
		return -1;
	else {
		LOG(L_DBG,"DBG:"M_NAME":cscf_get_multipart_hdr_field(): Found header <%.*s: %.*s>\n",
				hdr->name.len,hdr->name.s,
				hdr->body.len,hdr->body.s);			
		return 1;
	}
}

/**
 * Extracts the Content-Type header from a part
 * @param part
 * @return
 */
str cscf_get_multipart_content_type(str part)
{
	str x={0,0};
	struct hdr_field hdr;
	int position=0;

	while(cscf_get_multipart_hdr_field(part,&position,&hdr)){
		clean_hdr_field(&hdr);
		if (hdr.type==HDR_CONTENTTYPE_T)
			return hdr.body;
	}
	return x;
}


static str s_textplain={"text/plain",10};
static str s_multipart={"multipart/",10};
/**
 * Finds a part with a given content type inside a multipart body.
 * TODO - make it extract more than just the first match
 * @param body - the multipart/... body to look inside
 * @param body_content_type - the content type of the body, including boundary definitions
 * @param search_content_type - the content_type body to look for
 * @param part - pointer to return the entire part string, including the possible headers
 * @returns the body if found or a str with s=0 if not found
 */
str cscf_get_body_with_type_from_body(str body,str body_content_type,str search_content_type,str *ret_part)
{
	str x={0,0},boundary,part,part_content_type,part_body;
	int position=0;
	
	LOG(L_DBG,"DBG:"M_NAME":cscf_get_body_with_type_from_body(): looking for %.*s inside body of type %.*s.\n",
			search_content_type.len,search_content_type.s,
			body_content_type.len,body_content_type.s);
	
	if (ret_part) *ret_part = x;
	
	if (body_content_type.len>=search_content_type.len &&
			strncasecmp(body_content_type.s,search_content_type.s,search_content_type.len)==0){
		LOG(L_DBG,"DBG:"M_NAME":cscf_get_body_with_type_from_body(): found!\n");		
		return body;
	}
	if (body_content_type.len>=s_multipart.len &&
		strncasecmp(body_content_type.s,s_multipart.s,s_multipart.len)==0){
		if (!body.len) goto notfound;
		LOG(L_DBG,"DBG:"M_NAME":cscf_get_body_with_type_from_body(): found a multipart - looking inside.\n");
		
		boundary = cscf_get_multipart_boundary(body_content_type);
		if (!boundary.len){
			LOG(L_DBG,"DBG:"M_NAME":cscf_get_body_with_type_from_body(): the boundary is empty... can't split multipart.\n");
			goto notfound;
		}
		for(part = cscf_get_multipart_part(body,boundary,&position);
			position<body.len;
			part = cscf_get_multipart_part(body,boundary,&position)){
			if (!part.len) continue;
			
			part_content_type = cscf_get_multipart_content_type(part);
			LOG(L_DBG,"DBG:"M_NAME":cscf_get_body_with_type_from_body(): found a non-empty part with type <%.*s>.\n",
					part_content_type.len,part_content_type.s);
			/*
			 *  TODO - fix the next line to fill the default content-type based on which multipart/? there is
			 *  text/plain is the default for many, but not all
			 */ 
			if (!part_content_type.len) part_content_type = s_textplain;
			part_body = cscf_get_multipart_body(part);
			
			x = cscf_get_body_with_type_from_body(part_body,part_content_type,search_content_type,ret_part);
			if (x.s){
				if (ret_part&&ret_part->s==0) *ret_part = part;
				return x;
			}
		}		
	}
	
notfound:	
	x.s=0;x.len=0;
	LOG(L_DBG,"DBG:"M_NAME":cscf_get_body_with_type(): body of type %.*s not found.\n",
		search_content_type.len,search_content_type.s);
	if (ret_part) *ret_part = x;
	return x;	
}


/**
 * Find and extract a body with a given type, by looking through normal body and multipart ones
 * @param content_type - the type (e.g. application/sdp)
 * @returns the body if found or a str with s=0 if not found
 */
str cscf_get_body_with_type(struct sip_msg *msg,str search_content_type,str *ret_part)
{
	str body_content_type = cscf_get_content_type(msg);
	
	if (body_content_type.len==0) body_content_type = s_textplain;
	
	if (body_content_type.len>=search_content_type.len &&
			strncasecmp(body_content_type.s,search_content_type.s,search_content_type.len)==0){
		LOG(L_DBG,"DBG:"M_NAME":cscf_get_body_with_type(): found body of type %.*s as main body.\n",
				body_content_type.len,body_content_type.s);
		if (ret_part) {
			ret_part->s=0;
			ret_part->len=0;
		}
		return cscf_get_body(msg);
	}
	return cscf_get_body_with_type_from_body(cscf_get_body(msg),body_content_type,search_content_type,ret_part);
}

/* wrapper of cscf_get_body_with_type function
 * searches for a body part with a specific content-type
 * @param msg - the message
 * @param search_content_type - the content-type to be search for
 */
str cscf_get_body_with_content_type(struct sip_msg *msg,str search_content_type){

	str ret_part = {NULL, 0};

	return cscf_get_body_with_type(msg, search_content_type, &ret_part);
}

static str sdp_content_type = {"application/sdp", 15};
int extract_sdp_body(struct sip_msg *msg, str *body )
{
	
	str sdp_body = {NULL, 0};

	body->s = get_body(msg);
	if (body->s==0) {
		LOG(L_ERR, "ERROR: extract_sdp_body: failed to get the message body\n");
		goto error;
	}
	body->len = msg->len -(int)(body->s-msg->buf);
	if (body->len==0) {
		LOG(L_ERR, "ERROR: extract_sdp_body: message body has length zero\n");
		goto error;
	}
	
	sdp_body = cscf_get_body_with_content_type(msg, sdp_content_type);
	if(!sdp_body.s || !sdp_body.len){
		LOG(L_ERR, "ERROR: extract_sdp_body: could not find the sdp body\n");
		goto error;
	}

	body->s = sdp_body.s;
	body->len = sdp_body.len;

	LOG(L_DBG, "DBG:"M_NAME":extract_sdp_body: found sdp body: \n%.*s\n",
			body->len, body->s);
	
	return 1;
error:
	return -1;
}

