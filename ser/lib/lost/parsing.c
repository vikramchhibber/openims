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

#include "parsing.h"
#include "client.h"

xmlAttr * get_attr(xmlNode * node, char* attr_name);

void print_element_names(xmlNode * a_node){

    xmlNode *cur_node = NULL;
    xmlAttr * attr;
//    xmlChar* content;
    xmlNs * namesp;

    for (cur_node = a_node; cur_node; cur_node = cur_node->next) {
        if (cur_node->type == XML_ELEMENT_NODE) {
            DEBUG_LOG("node type: Element, name: %s\n", cur_node->name);
	    
//	    if((content = xmlNodeGetContent((xmlNodePtr)cur_node))){
//		    if(strlen((char*)content))
//			    DEBUG_LOG("node content %s\n", content);
//		    xmlFree(content);
//	   }
        }

	for(attr = cur_node->properties; attr != NULL; attr = attr->next){
		DEBUG_LOG("               attribute name [%s] : value [%s]\n", (char*)attr->name, 
				(attr->children == NULL)? "":(char*)attr->children->content);
	}

	for(namesp = cur_node->ns; namesp != NULL; namesp = namesp->next){
	
		DEBUG_LOG("               namespace prefix %s href %s\n",
				namesp->prefix, namesp->href);
	}

        print_element_names(cur_node->children);
    }
}

xmlNode * child_node(xmlNode * a_node){

	xmlNode *cur_node;
	
    	for (cur_node = a_node->children; cur_node; cur_node = cur_node->next) {
        
		if (cur_node->type == XML_ELEMENT_NODE){ 
			return cur_node;
		}
    	}
	return NULL;
}

xmlNode * child_named_node(xmlNode * a_node, char* name){

	return child_node_name_len(a_node, name, strlen(name));
}

xmlNode * child_node_name_len(xmlNode * a_node, char* name, int len){

	xmlNode *cur_node;
	char * cur_name;

    	for (cur_node = a_node->children; cur_node; cur_node = cur_node->next) {
        
		cur_name = (char*)cur_node->name;
		if ((cur_node->type == XML_ELEMENT_NODE) &&
				name_compar(cur_name, name, len)){ 
			return cur_node;
		}
    	}
	return NULL;
}

xmlNode * sibling_named_node(xmlNode * a_node, char* name){
	
	return sibling_node_name_len(a_node, name, strlen(name));
}

xmlNode * sibling_node_name_len(xmlNode * a_node, char* name, int len){

	xmlNode *cur_node;
	char * cur_name;

	if(!a_node)
		return NULL;

    	for (cur_node = a_node->next; cur_node; cur_node = cur_node->next) {
        
		cur_name = (char*)cur_node->name;
		if ((cur_node->type == XML_ELEMENT_NODE) &&
				name_compar(cur_name, name, len)){ 
			return cur_node;
		}
    	}
	return NULL;

}

void print_attr(xmlNode * node, char * attr_name){

	xmlAttr * attr;
	attr = get_attr(node, attr_name);
	if(attr)
		DEBUG_LOG("attribute [%s] : value [%s]\n", (char*)attr->name, 
			(attr->children == NULL)? "":(char*)attr->children->content);
}

xmlAttr * get_attr(xmlNode * node, char* attr_name){

	return get_attr_name_len(node, attr_name, strlen(attr_name));
}

xmlAttr * get_attr_name_len(xmlNode * node, char* attr_name, int attr_name_len){

	xmlAttr * attr;
	char* name;

	for(attr = node->properties; attr != NULL; attr = attr->next){

		name = (char*)attr->name;
		if(name && name_compar(name, attr_name, attr_name_len)){
			return attr;
		}
	}

	return NULL;
}

xmlNs* get_ns_prfx_len(xmlNode * node, char* prfx, int prfx_len){


	xmlNs * ns;
	char* name;

	for(ns=node->ns; ns ; ns= ns->next){
	
		name = (char*)ns->prefix;
		DEBUG("namespace %s compared to %.*s\n", name, prfx_len, prfx);
		if(name && name_compar(name, prfx, prfx_len))
			return ns;
	}

	return NULL;
}

xmlNs* get_ns_href_len(xmlNode* node, char* href, int href_len){

	xmlNs * ns;
	char* name;

	for(ns=node->ns; ns ; ns= ns->next){
	
		name = (char*)ns->href;
		//DEBUG_LOG("compare ns %s with %.*s", name, href_len, href);
		if(name && name_compar(name, href, href_len)){
			DEBUG_LOG("found namespace with href %.*s\n",
					href_len, href);
			return ns;
		}
	}

	DEBUG("could not found namespace with href %.*s\n",
			href_len, href);
	return NULL;
}

xmlNode * xml_parse_string(str response){


	xmlParserCtxtPtr ctxt;
    	xmlDocPtr doc; /* the resulting document tree */
	xmlNode * root;
	int res;

	/*
     	 * Create a progressive parsing context, the 2 first arguments
     	 * are not used since we want to build a tree and not use a SAX
     	 * parsing interface. We also pass the first bytes of the string
     	 * to allow encoding detection when creating the parser but this
     	 * is optional.
     	 */
	
	ctxt = xmlCreatePushParserCtxt(NULL, NULL,
			response.s, 4, NULL);
	
	if (ctxt == NULL) {
        	ERROR_LOG("Failed to create parser context !\n");
		return NULL;
    	}

        xmlParseChunk(ctxt, response.s+4, response.len-4, 0);

    	/* there is no more input, indicate the parsing is finished.*/
    	xmlParseChunk(ctxt, response.s, 0, 1);

    	/* collect the document back and if it was wellformed
     	 * and destroy the parser context.
     	 */
    	doc = ctxt->myDoc;
    	res = ctxt->wellFormed;
    	xmlFreeParserCtxt(ctxt);

    	if (!res) {
        	ERROR_LOG("Failed to parse %.*s\n", 
				response.len, response.s);
		return NULL;
    	}
	
	root = xmlDocGetRootElement((xmlDoc*)doc);
	
	return root;
}

//according to ISO 8601:[-]CCYY-MM-DDThh:mm:ss[Z|(+|-)hh:mm]
// and LoST response RelaxNG Schema: expires {xsd:dateTime | "NO_CACHE" | "NO_EXPIRATION" }
// TODO: time zone handle
int get_time(char * expires_str, expire_type * exp_type, time_t * exp_timestamp){

	int year, month, day, hour, min, sec;
	//int hour_tz, min_tz; //for time zone
	//char * sign_tz;
  	struct tm * timeinfo;
	time_t rawtime;

	if(strcmp(expires_str, LOST_EXP_NO_EXPIRATION) == 0){
		
		*exp_type = EXP_NO_EXP;
	}else if(strcmp(expires_str, LOST_EXP_NO_CACHE) == 0){
		
		*exp_type = EXP_NO_CACHE;
	}else{
		if(expires_str[0] == '-'){
			ERROR_LOG("year begins with minus...too long ago\n");
			return -1;
		}
		
		if(sscanf(expires_str, "%4d-%2d-%2dT%2d:%2d:%2d", &year, &month, &day, &hour, &min, &sec) == EOF){
			ERROR_LOG("Error at sscanf: %s\n", strerror(errno));
			return -1;
		}

		if(year < 1900 ){
			ERROR_LOG("Invalid date: year [%i] must be >= 1900 \n", year);
			return -1;
		}
		if(month <1 || month > 12){
			ERROR_LOG("Invalid date: month [%i] must be >= 1 and <=12 \n", month);
			return -1;
		}
		if(day < 1 || day> 31){
			ERROR_LOG("Invalid date: day [%i] must be >= 1 and <=31 \n", day);
			return -1;	
		}
		if(hour <0 || hour> 23){
			ERROR_LOG("Invalid date: hour [%i] must be >= 0 and <=23 \n", hour);
			return -1;	
		}
		if(min<0 || min > 59 || sec < 0 || sec>60){
			ERROR_LOG("Invalid date: min [%i] must be >=0 and <=59 and sec [%i] >=0 and <=60\n", min, sec);
			return -1;
		}

  		/* get current timeinfo and modify it to the user's choice */
  		time ( &rawtime );
  		timeinfo = localtime ( &rawtime );
  		timeinfo->tm_year = year - 1900;
  		timeinfo->tm_mon  = month - 1;
  		timeinfo->tm_mday = day;
		timeinfo->tm_hour = hour;
		timeinfo->tm_min = min;
		timeinfo->tm_sec = sec;

  		/* call mktime: timeinfo->tm_wday will be set */
  		*exp_timestamp = mktime(timeinfo);
		if(*exp_timestamp < 0){
			ERROR_LOG("Invalid date: %s\n", strerror(errno));
			return -1;
		}

		DEBUG_LOG("Date time specified: year %i month %i day %i hour %i min %i sec %i\n",
				year, month, day, hour, min, sec);
		*exp_type = EXP_TIME;
	}
	return 0;
}

