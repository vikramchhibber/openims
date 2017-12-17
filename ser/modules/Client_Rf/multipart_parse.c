 /* Copyright (C) 2001-2003 FhG Fokus
 *
 * This file is part of Kamailio, a free SIP server.
 *
 * Kamailio is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version
 *
 * Kamailio is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License 
 * along with this program; if not, write to the Free Software 
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
 *
 */

/* History:
 * Ancuta Onofrei - generalized the code to get the part of a multipart body for 
 * 		different types and subtypes
 * 		email andreea dot ancuta dot onofrei -at- fokus dot fraunhofer dot de
 *
 */

#ifdef OPEN_IMS_CORE

#include "mod.h"

#include "../../parser/parser_f.h"
#include "../mem/mem.h"
#include "sdp_helpr_funcs.h"
#include "parse_content.h"
#include "multipart_parse.h"
#include "sip.h"

static int str_trim_newline(str *s);
static int get_body_content(struct sip_msg * _m, str * body_content, unsigned int type, unsigned int subtype);
static int get_mixed_body_content(str* mixed_body, str delimiter, unsigned int type, unsigned int subtype, 
		str * body_content);

/* get the PIDF-LO body of a request
 * @param msg - the SIP request
 * @param pidf_body - the PIDF-LO body part
 * @returns -1 if not found  or error, 0 if found, 1 if no body content
 */
int get_pidf_lo_body(struct sip_msg* msg, str * pidf_body){

	unsigned int type, subtype;
	int ret;

	type = TYPE_APPLICATION;
	subtype = SUBTYPE_PIDFXML;

	ret = get_body_content(msg, pidf_body, type, subtype);
	if(ret == -1){
		LOG(L_ERR, "ERR: "M_NAME":get_pidf_lo_body:an error has occured while retrieving the pidf+xml information\n");
	}else if (ret == 1){
		LOG(L_DBG, "DBG: "M_NAME":no body found\n");
	}else{
	
		LOG(L_DBG, "DBG: "M_NAME":get_pidf_lo_body:content body for pidf+xml object is %.*s\n",
			pidf_body->len, pidf_body->s);
	}
	return ret;
}

/* get the body with a specific type and subtype from the multipart body
 * @param mixed_body - the string of the multipart body
 * @param delimiter - the string that is delimiting the different body parts
 * @param type and subtype - e.g. application and pdf+xml for a PIDF-LO body (application/pdf+xml)
 * @param body_content - the requested body part
 * @returns -1 if not found or error, 0 if found, 1 if no body is present
 */
int get_body_content(struct sip_msg * _m, str * body_content, unsigned int type, unsigned int subtype){

	str body, mp_delimiter;
	int mime;

	body.s = get_body(_m);
	if (body.s==0) {
		LOG(L_ERR, "ERR:"M_NAME":get_body_content:failed to get the message body\n");
		return -1;
	}

	body.len = _m->len -(int)(body.s - _m->buf);
	if (body.len==0) {
		LOG(L_DBG, "DBG:"M_NAME":get_body_content:message body has length zero\n");
		return 1;
	}

	mime = ecscf_parse_content_type_hdr(_m);
	if (mime <= 0) {
		LOG(L_ERR, "ERR:"M_NAME":get_body_content:could not parse the content type properly\n");
		return -1;
	}

	if((((unsigned int)mime)>>16) == type){
	       if((mime&0x00ff) == subtype){
			body_content->s = body.s;
			body_content->len = body.len;
			return 0;
		}else{
			LOG(L_DBG, "DBG:"M_NAME":get_body_content:TYPE_APPLICATION: unknown %d\n",mime&0x00ff);
			return -1;
		}
	} else if ((((unsigned int)mime)>>16) == TYPE_MULTIPART){
		switch (mime&0x00ff) {
		case SUBTYPE_MIXED:
			if(get_mixed_part_delimiter(&(_m->content_type->body),&mp_delimiter) > 0) {
				LOG(L_DBG, "DBG:"M_NAME":get_body_content: mp_delimiter is %.*s\n", 
						mp_delimiter.len, mp_delimiter.s);
				return get_mixed_body_content(&body, mp_delimiter, type, subtype, body_content);
			} else {
				LOG(L_ERR, "ERR:"M_NAME":get_body_content:could not get the delimiter of the multipart content\n");
				return -1;
			}
		default:
			LOG(L_DBG, "DBG:"M_NAME":get_body_content: TYPE_MULTIPART: unknown %d\n",mime&0x00ff);
			return -1;
		}
	} 

	LOG(L_DBG, "DBG:"M_NAME":get_body_content: type of mime unknown:%d\n",((unsigned int)mime)>>16);
	return -1;
}

int get_mixed_body_content(str* mixed_body, str delimiter, unsigned int type, unsigned int subtype, 
		str * body_content){

	int no_eoh_found, found;
	char *bodylimit, *rest;
	char *d1p, *d2p;
	char *ret, *end;
	unsigned int mime;
	str cnt_disp;
	struct hdr_field hf;

	bodylimit = mixed_body->s + mixed_body->len;
	d1p = find_sdp_line_delimiter(mixed_body->s, bodylimit, delimiter);
	if (d1p == NULL) {
		LOG(L_ERR, "ERR:"M_NAME":get_mixed_body_content: empty multipart content\n");
		return -1;
	}
	found = 0;
	d2p = d1p;

	for(;!found && d1p != NULL && d1p < bodylimit; d1p = d2p) {
		
		d2p = find_next_sdp_line_delimiter(d1p, bodylimit, delimiter, bodylimit);
		/* d2p is text limit for application parsing */
		memset(&hf,0, sizeof(struct hdr_field));
		rest = eat_line(d1p + delimiter.len + 2, d2p - d1p - delimiter.len - 2);
		if ( rest > d2p ) {
			LOG(L_ERR, "ERR:"M_NAME":get_mixed_body_content:Unparsable <%.*s>\n", (int)(d2p-d1p), d1p);
			return -1;
		}
		no_eoh_found = 1;
		found = 0;
		/*LM_DBG("we need to parse this: <%.*s>\n", d2p-rest, rest); */
		while( rest<d2p && no_eoh_found ) {
			rest = get_sdp_hdr_field(rest, d2p, &hf);
			switch (hf.type){
			case HDR_EOH_T:
				no_eoh_found = 0;
				break;
			case HDR_CONTENTTYPE_T:
				end = hf.body.s + hf.body.len;
				ret = ecscf_decode_mime_type(hf.body.s, end , &mime);
				if (ret==0)
					return -1;
				if (ret!=end) {
					LOG(L_ERR, "ERR:"M_NAME":get_mixed_body_content:the header CONTENT_TYPE contains "
						"more then one mime type :-(!\n");
					return -1;
				}
				if ((mime&0x00ff)==SUBTYPE_ALL || (mime>>16)==TYPE_ALL) {
					LOG(L_ERR, "ERR:"M_NAME":get_mixed_body_content:invalid mime with wildcard '*' in Content-Type hdr!\n");
					return -1;
				}
				
				if (((((unsigned int)mime)>>16) == type) && ((mime&0x00ff) == subtype)) {
					found = 1;
				}
				break;
			case HDR_CONTENTDISPOSITION_T:
				cnt_disp.s = hf.body.s;
				cnt_disp.len = hf.body.len;
				break;
			case HDR_ERROR_T:
				return -1;
				break;
			default:
				LOG(L_DBG, "DBG:"M_NAME":get_mixed_body_content:unknown header: <%.*s:%.*s>\n",hf.name.len,hf.name.s,hf.body.len,hf.body.s);
			}
		} /* end of while */
	}
	if(!found)
		return -1;

	body_content->s = rest;
	body_content->len = d2p-rest;

	str_trim_newline(body_content);

	return 0;
}

/**
 * trims the str , including the newlines also
 * @param s - str param to trim
 */
static int str_trim_newline(str *s)
{
	int i;
	for (i = 0;i < s->len; i++)
	{
		if (s->s[i] != '\r' && s->s[i] != '\t' && s->s[i] != ' ' && s->s[i] !='\n')
		{
			break;
		}
	}
	s->s = s->s + i;	
	s->len -= i;

	for (i = s->len;i >=0; i--)
	{
		if (s->s[i] == '\r' && s->s[i] == '\t' && s->s[i] == ' ' && s->s[i] == '\n')
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


#endif /*OPEN_IMS_CORE*/
