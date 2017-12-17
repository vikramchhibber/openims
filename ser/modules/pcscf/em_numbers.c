/*
 * $Id: em_numbers.c 920 2010-03-05 19:07:50Z vingarzan $
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
 */
 
/**
 * \file
 * 
 * Proxy-CSCF -Emergency Numbers Related Operations
 * 
 * 
 *  \author Ancuta Onofrei	andreea dot ancuta dot onofrei -at- fokus dot fraunhofer dot de
 */

#include "em_numbers.h"
#include "../../parser/parse_uri.h"

extern char* emerg_numbers_file;
em_nb_list emerg_nb_list = NULL;


#define get_urn(_urn, str)\
  if(_urn){\
      _urn->s = str##_STR;\
      _urn->len = str##_STR_LEN;\
  }
  

//return 0 if matched
int check_emerg_URN(char *p, char* service, int service_len, str * urn){
	
	return strncmp(p, service, service_len);
}

urn_t is_emerg_urn(str ruri, str * urn){
		
	char* uri = ruri.s;
	int len = ruri.len;

	urn_t type;

	if((len < GEN_URN_STR_LEN ) || (strncmp(uri, GEN_URN_STR, GEN_URN_STR_LEN)!=0)){
		LOG(L_DBG, "DBG:"M_NAME":emergency_urn: %.*s is not an URN\n",
			len, uri);
		return NOT_URN;
	}

	if((len < SOS_URN_STR_LEN ) || (strncmp(uri,SOS_URN_STR, SOS_URN_STR_LEN)!=0)){
		goto not_emerg;
	}

	if(len == SOS_URN_STR_LEN){
		type = URN_GEN_SOS;
		goto is_emerg;
	}

	char* p = uri+SOS_URN_STR_LEN;
	if(*p !='.'){	
		goto error;
	}
	p++;

	switch(*p){
		case 'a':
			if(!check_emerg_URN(p, SOS_URN_AMB, SOS_URN_AMB_LEN, urn)){ 
				type = URN_AMB;
				get_urn(urn, SOS_URN_AMB);
				goto is_emerg;
			}
			if(!check_emerg_URN(p, SOS_URN_AN_CTR, SOS_URN_AN_CTR_LEN, urn)){
				type = URN_AN_CTRL;
				get_urn(urn, SOS_URN_AN_CTR);
				goto is_emerg;
			}break;			
		case 'f':
			if(!check_emerg_URN(p, SOS_URN_FIRE, SOS_URN_FIRE_LEN, urn)){
				type = URN_FIRE;
				get_urn(urn, SOS_URN_FIRE);
				goto is_emerg;
			}break;
		case 'g':
			if(!check_emerg_URN(p, SOS_URN_GAS, SOS_URN_GAS_LEN, urn)){
				type = URN_GAS;
				get_urn(urn, SOS_URN_GAS);
				goto is_emerg;
			}break;
		case 'm':
			if(!check_emerg_URN(p, SOS_URN_MAR, SOS_URN_MAR_LEN, urn)){
				type = URN_MAR;
				get_urn(urn, SOS_URN_MAR);
				goto is_emerg;
			}

			if(!check_emerg_URN(p, SOS_URN_MOUNT, SOS_URN_MOUNT_LEN, urn)){
				type = URN_MOUNT;
				get_urn(urn, SOS_URN_MOUNT);
				goto is_emerg;
			}break;
		case 'p':
			if(!check_emerg_URN(p, SOS_URN_POL, SOS_URN_POL_LEN, urn)){
				type = URN_POL;
				get_urn(urn, SOS_URN_POL);
				goto is_emerg;
			}
			if(!check_emerg_URN(p, SOS_URN_POIS, SOS_URN_POIS_LEN, urn)){
				type = URN_POIS;
				get_urn(urn, SOS_URN_POIS);
				goto is_emerg;
			}
			if(!check_emerg_URN(p, SOS_URN_PHYS, SOS_URN_PHYS_LEN, urn)){
				type = URN_PHYS;
				get_urn(urn, SOS_URN_PHYS);
				goto is_emerg;
			}break;
		default:
			break;
	}

error:
	LOG(L_DBG, "DBG:"M_NAME":emergency_urn: invalid emergency URN %.*s\n",
			len, uri);
	return NOT_EM_URN;

not_emerg:
	LOG(L_DBG, "DBG:"M_NAME":emergency_urn: no emergency URN %.*s\n",
			len, uri);
	return NOT_EM_URN;

is_emerg:
	LOG(L_DBG, "DBG:"M_NAME":emergency_urn: we have a valid emergency URN for the URI %.*s\n",
			len, uri);
	if(urn){
		urn->s = uri;
		urn->len = len;
	}
	return type;

}

urn_t check_emerg_number(str user, str* urn){
  
  em_nb * emerg_number;
  LOG(L_DBG, "DBG:"M_NAME":check_emerg_number: checking %.*s\n", user.len, user.s);
  
  for(emerg_number = emerg_nb_list; emerg_number!=NULL; 
	emerg_number = emerg_number->next){
    LOG(L_DBG, "DBG:"M_NAME":check_emerg_number: comparing to %.*s\n", 
	emerg_number->number.len, emerg_number->number.s);
    if(user.len == emerg_number->number.len &&
      strncmp(user.s, emerg_number->number.s, user.len) == 0){
      if(urn){
	urn->s = emerg_number->urn.s;
	urn->len = emerg_number->urn.len;
      }
      return emerg_number->type;
      }
  }
  
  return NOT_URN;

}

urn_t is_emerg_ruri(str ruri, str* urn) {
	
	struct sip_uri puri;
	
	if(urn){
	  urn->s = NULL; urn->len = 0;
	}
		
	if(parse_uri(ruri.s, ruri.len, &puri)<0){
		LOG(L_ERR,"ERR:"M_NAME":is_emerg_ruri: failed to parse %.*s\n",	
		  ruri.len, ruri.s);
		return NOT_URN;
	}
	switch(puri.type){
		case	SIP_URI_T:
		case	SIPS_URI_T:
			  LOG(L_DBG, "DBG:"M_NAME":is_emerg_ruri: sip URI\n");
			  return check_emerg_number(puri.user, urn);
		case TEL_URI_T:
		case TELS_URI_T:
			LOG(L_DBG, "DBG:"M_NAME":is_emerg_ruri: tel URI\n");
			return check_emerg_number(puri.user, urn);
		case ERROR_URI_T:
		case CID_T:
			return NOT_URN;
		case URN_T:
			return is_emerg_urn(ruri, urn);

	}	

	return NOT_URN;
}


int add_em_number(str number, str urn){
	
	urn_t type;

	LOG(L_DBG, "DBG:"M_NAME":add_em_number: adding number %.*s and urn %.*s\n",
			number.len, number.s, urn.len, urn.s);

	type = is_emerg_urn(urn, NULL);
	if((type == NOT_URN) || (type == NOT_EM_URN)){
		LOG(L_ERR, "ERR:"M_NAME":add_em_number:invalid URN %.*s\n", urn.len, urn.s);
		return -1;
	}

	em_nb * emerg_number = (em_nb*)pkg_malloc(sizeof(em_nb)+number.len+urn.len);
	if(!emerg_number){
		LOG(L_ERR, "ERR:"M_NAME":add_em_number:could not allocate %i bytes of pkg memory\n", 
		  sizeof(em_nb)+number.len+urn.len);
		return -1;
	}

	emerg_number->type = type;
	char * p = (char*)emerg_number;
       	p += sizeof(em_nb);
	memcpy(p, number.s, number.len*sizeof(char));
	emerg_number->number.s = p;
	emerg_number->number.len = number.len;
	p += number.len;
	memcpy(p, urn.s, urn.len*sizeof(char));
	emerg_number->urn.s = p;
	emerg_number->urn.len = urn.len;
	emerg_number->next = emerg_nb_list;
	emerg_nb_list = emerg_number;
	return 0;
}

#define NUMBER_NODE_NAME	"number"
#define URN_NODE_NAME		"urn"

xmlNode * child_node_name_len(xmlNode * a_node, char* name, int len){

	xmlNode *cur_node;
	char * cur_name;

    	for (cur_node = a_node->children; cur_node; cur_node = cur_node->next) {
        
		cur_name = (char*)cur_node->name;
		if ((cur_node->type == XML_ELEMENT_NODE) &&
				strncmp(cur_name, name, len) == 0){ 
			return cur_node;
		}
    	}
	return NULL;
}

xmlNode * child_named_node(xmlNode * a_node, char* name){

	return child_node_name_len(a_node, name, strlen(name));
}

int read_em_nb(xmlNode * node, str* number, str* urn){
	
	xmlChar* content, *content2;
	xmlNode * nb =NULL, * urn_node = NULL;

	nb = child_named_node(node, NUMBER_NODE_NAME);
	if(!nb || !(content = xmlNodeGetContent((xmlNodePtr)nb)) || !(strlen((char*)content))){
		LOG(L_ERR, "ERR:"M_NAME":no or invalid %s tag, invalid config file\n", NUMBER_NODE_NAME);
		return -1;
	}

	number->s = (char*)content;
	number->len = strlen(number->s);

	urn_node = child_named_node(node, URN_NODE_NAME);
	if(!urn_node ||  !(content2 = xmlNodeGetContent((xmlNodePtr)urn_node))
	 	|| !(strlen((char*)content2)) ){
		LOG(L_ERR, "ERR:"M_NAME":no or invalid %s tag, invalid config file\n", URN_NODE_NAME);
		xmlFree(content);
		return -1;
	}
	
	urn->s = (char*)content2;
	urn->len = strlen(urn->s);

/*	LOG(L_DBG, "DBG:"M_NAME":read_em_nb: read em number %.*s and urn %.*s\n",
			number->len, number->s, urn->len, urn->s);
*/
	return 0;
}

int store_em_numbers(){

	xmlDocPtr doc = NULL; /* the resulting document tree */
	xmlNode * root = NULL, * cur_node = NULL;

	str number, rpl_urn;

	if(!emerg_numbers_file || emerg_numbers_file[0]=='\0')
		return 0;
	LOG(L_DBG, "DBG:"M_NAME":store_em_numbers from the file %s\n", emerg_numbers_file);

	doc = xmlReadFile(emerg_numbers_file, NULL, 0);
	if (doc == NULL) {
        	LOG(L_ERR, "ERR:"M_NAME":Failed to parse %s\n", emerg_numbers_file);
		return -1;
    	}
	root = xmlDocGetRootElement((xmlDoc*)doc);
	for (cur_node = root->children; cur_node; cur_node = cur_node->next) {
        	if (cur_node->type == XML_ELEMENT_NODE) {
			//LOG(L_DBG, "DBG:"M_NAME":node type: Element, name: %s\n", cur_node->name);
	        	if(read_em_nb(cur_node, &number, &rpl_urn))
				goto error;
			if(add_em_number(number, rpl_urn))
				goto error;
		}
    	}
	xmlFreeDoc(doc);
	return 0;
error:
	xmlFreeDoc(doc);
	return -1;

}

void clean_em_numbers(){

	em_nb * emerg_number;

	while(emerg_nb_list){
		
		emerg_number = emerg_nb_list;
		emerg_nb_list = emerg_number->next;
		pkg_free(emerg_number);	
		emerg_number = NULL;
	}
}

