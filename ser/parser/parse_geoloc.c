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
 * \file geoloc.c
 * 
 * Emergency-CSCF -Geolocation header parsing
 * (conforming to the draft-ietf-sip-location-conveyance)
 * 
 * author Ancuta Onofrei, 
 * 	email andreea dot ancuta dot onofrei -at- fokus dot fraunhofer dot de
 */
 
#include "parse_geoloc.h"

#include "parse_uri.h"

//static str Loc_byVal_start 	= {"cid:", 4};
static str inserted_by_par 	= {"inserted-by=\"", 13};
static str used_routing_par 	= {"used-for-routing", 16};
static str routing_allowed_par 	= {"routing-allowed=", 16};
//static str Geolocation_header_name = {"Geolocation", (sizeof("Geolocation")-1)};

char * strnchr(char* string, char car, int len){

	char * p;

	for(p = string; p<string+len; p++)
		if(*p == car)
			return p;
	return NULL;
}

#define set_yes_no_value(pch, end_string, val)\
	do{\
		if((pch+4) <=end_string && strncmp(pch, "\"no\"", 4)==0)\
			val = 0;\
		else if((pch+5)<=end_string && strncmp(pch, "\"yes\"", 5)==0)\
			val = 1;\
		else goto malformed;\
	}while(0);

int parse_token(char * pch, int len, struct geoloc_body* geo_body){

	char * start, *end, * end_string;
	struct loc_value * value = NULL;
	
	end_string = pch+len;
	for(start = pch; start < end_string && (*start)!='<' && !isalpha(*start); start++);
	if(!start) goto malformed;
	
	if(*start == '<'){ 
		for(end = start+1; end < end_string && (*end)!='>'; end++);
		if(!end) goto malformed;
		
		value = (struct loc_value *)pkg_malloc(sizeof(struct loc_value));
		if(!value) goto no_mem;
		memset(value, 0, sizeof(struct loc_value));

		if(parse_uri(start+1, end-start-1, &value->locURI))
			goto malformed;
		
		for(pch = strchr(end+1, ';'); pch!=NULL && pch<end_string; pch = strchr(pch+1, ';')){
			pch++;
			if(*pch == 'i' &&  ((pch+inserted_by_par.len) < end_string) 
					&& strncmp(pch, inserted_by_par.s, inserted_by_par.len)==0){
				//DBG("inserted-by par present\n");
				pch += inserted_by_par.len;
				if((!(end = strchr(pch+2, '"')) || end>end_string))
					goto malformed;	
				value->inserted_by.s = pch;
				value->inserted_by.len = end - pch;
			}else if(*pch == 'u' && ((pch+used_routing_par.len)<=end_string) 
					&& strncmp(pch, used_routing_par.s, used_routing_par.len)==0){
				//DBG("used for routing par present\n");
				pch += used_routing_par.len;
				value->used_for_routing = 1;
			}else
				goto malformed;
		}

		value->next = geo_body->loc_list;
		geo_body->loc_list = value;
		
	}else if(*start == 'r'&& (start+routing_allowed_par.len)<end_string && 
			strncmp(start, routing_allowed_par.s, routing_allowed_par.len)==0){
		//DBG("global routing allowed par present\n");
		pch = start + routing_allowed_par.len;
		set_yes_no_value(pch, end_string, geo_body->retrans_par);
	}else 
		goto malformed;

	return 0;
no_mem:
	LOG(L_ERR, "ERROR:parse_geoloc_body: not enough memory\n");
	return -1;

malformed:

	LOG(L_ERR,"ERROR:parse_geoloc_body: malformed header\n");
	return -1;
}
//parsing of the Geolocation header
int parse_geoloc_body(str body, struct geoloc_body * geo_body){
	
	int len, first;
	char *string, * end, * pch, car;
	struct loc_value* value;

	string = body.s;
	end = string+body.len;
	car = *end;
	//put string terminal in order that the strtok will search only in the current header field
	*end = '\0';
	first =1;

	pch = strtok (string,",");
  	while (pch != NULL){
	    //DBG(" token is %s\n",pch);
	    len = strlen(pch);
	    if(!first)
		    *(pch-1) = ',';
	    else 
		    first = 0;

	    if(parse_token(pch, len, geo_body))
		    return -1;
	    pch = strtok (NULL, ",");
	}
	*end = car;

	value = geo_body->loc_list;
	if(!value){
	
		LOG(L_ERR, "ERROR:parse_geoloc_body: no location value supplied in the Geolocation header\n");
		return -1;
	}
	
	return 0;

}

/* parses the Geolocation header, see http://tools.ietf.org/html/draft-ietf-sipcore-location-conveyance
 * @param msg - the INVITE request that might contain a Geolocation header
 * @returns 1 if not found, 0 if correctly parsed and -1 in case of error parsing
 */
int parse_geoloc(struct sip_msg * msg){

	struct geoloc_body * geo_body;
	
	/* look for Geolocation header */
	if (msg->geolocation==0) {
		if (parse_headers(msg, HDR_GEOLOCATION_F, 0)==-1)
			goto error;
		if (msg->geolocation==0) {
			DBG("DEBUG:parse_geoloc: hdr not found\n");
			return 1;
		}
	}

	/* now, we have the header -> look if it isn't already parsed */
	if (msg->geolocation->parsed!=0) {
		/* already parsed, nothing more to be done */
		return 0;
	}

	/* parse the body */
	geo_body = (struct geoloc_body*)pkg_malloc(sizeof(struct geoloc_body));
	if (geo_body==0) {
		LOG(L_ERR,"ERROR:parse_content_geolocation: no more pkg memory\n");
		goto error;
	}
	memset(geo_body,0,sizeof(struct geoloc_body));

	if (parse_geoloc_body(msg->geolocation->body, geo_body)==-1) {
		/* error when parsing the body */
		free_geoloc( &geo_body );
		goto error;
	}

	/* attach the parsed form to the header */
	msg->geolocation->parsed = (void*)geo_body;

	return 0;
error:
	return -1;

}

void free_geoloc(struct geoloc_body ** geo_body){

	struct loc_value *crt, * next;

	/* free the location values */
	for(crt = (*geo_body)->loc_list; crt!=NULL;){
		next = crt->next;
		pkg_free(crt);
		crt = next;
	}

	pkg_free( *geo_body );
	*geo_body = 0;

}

void print_geoloc(struct geoloc_body *  geo_body){

	struct loc_value * value;
	if(!geo_body){
		LOG(L_ERR, "ERROR:print_geoloc: null parameter\n");
		return;
	}

	LOG(L_DBG, "DBG:print_geoloc: retrans parameter: %s\n", (geo_body->retrans_par>0)?"yes":"no");
	for(value = geo_body->loc_list; value!= NULL; value = value->next){
	
		LOG(L_DBG, "DBG: print_geoloc: location value has the uri: user %.*s, host %.*s\n",
				value->locURI.user.len, value->locURI.user.s,
				value->locURI.host.len, value->locURI.host.s);
		LOG(L_DBG, "DBG: inserted by %.*s and used for routing %s\n",
				value->inserted_by.len, value->inserted_by.s,
				(value->used_for_routing>0)?"yes":"no");
	}
}
