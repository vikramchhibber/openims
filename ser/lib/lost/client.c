/*
 * Implementation of LoST client (RFC 5222 )
 *
 * prerequisites: libxml, libcurl
 *
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

#include "client.h"
#include "parsing.h"
#include "../cds/memory.h"

static struct curl_slist *headers=NULL;
static char errorBuffer[CURL_ERROR_SIZE];
char* map_profile[] = {"geodetic-2d", "geodetic-2d", "not standard format", "civic", "civic", "not defined"};

static xmlFreeFunc defaultXmlFreeFunc;
static xmlMallocFunc defaultXmlMallocFunc;
static xmlReallocFunc defaultXmlReallocFunc;
static xmlStrdupFunc defaultXmlStrdupFunc;

int get_time(char * expires_str, expire_type * exp_type, time_t * exp_timestamp);

//void	xmlFreeFunc			(void * mem);

//elif defined(SHM_MEM) && defined(USE_SHM_MEM)
//	include "shm_mem.h"
//define pkg_malloc(s) shm_malloc((s))
//so...do not define SHM_MEM and USE_SHM_MEM simultaneous, otherwise pkg is shm

void* my_pkg_malloc(size_t size){
	
	return pkg_malloc(size);
}

void* my_pkg_realloc(void * p, size_t size){

	return pkg_realloc(p, size);
}

void my_pkg_free(void * p){

	pkg_free(p);
}

char* my_pkg_strdup(const char * p){

	char * r;

	r = (char*)my_pkg_malloc((strlen(p)+1)*sizeof(char));
	if(!r)
		return r;
	memcpy(r, p, (strlen(p)+1)*sizeof(char));

	return r;
}

int init_lost_lib(){

	if(xmlInitMemory()){
	
		ERROR_LOG("Failed to initialize teh memory layer from libxml\n");
		return -1;
	}

	if(xmlMemGet(&defaultXmlFreeFunc, &defaultXmlMallocFunc, 
				&defaultXmlReallocFunc, &defaultXmlStrdupFunc)){
		ERROR_LOG("Failed to get the default memory functions from libxml\n");
		return -1;
	}

	if(xmlMemSetup(my_pkg_free, my_pkg_malloc, my_pkg_realloc, my_pkg_strdup)){
	
		ERROR_LOG("Failed to set the pkg memory functions to libxml\n");
		return -1;
	}

	headers = curl_slist_append(headers, LOST_CONTENT_TYPE);
	if(!headers){
		ERROR_LOG("Failed to initialize header content type\n");
		return -1;
	}

	headers = curl_slist_append(headers, LOST_CACHE_CONTROL);
	if(!headers){
		ERROR_LOG("Failed to initialize header cache control\n");
		return -1;
	}

	curl_global_init(CURL_GLOBAL_DEFAULT);
	return 0;
}

void end_lost_lib(){

	/* free the header list */ 
	curl_slist_free_all(headers); 

	/* Cleanup function for the XML library.*/
	xmlCleanupParser();

	if(xmlMemSetup(defaultXmlFreeFunc, defaultXmlMallocFunc, defaultXmlReallocFunc, defaultXmlStrdupFunc)){
	
		ERROR_LOG("Failed to set the default memory functions to libxml\n");
	}

}

size_t WriteMemoryCallback(void *ptr, size_t size, size_t nmemb, void *data){
	
	size_t realsize = size * nmemb;
	str *mem = (str *)data;
	mem->s = (char*)my_pkg_realloc(mem->s, mem->len + realsize);
	
	if (mem->s) {
		memcpy(&(mem->s[mem->len]), ptr, realsize);
		mem->len += realsize;
		//mem->s[mem->len] = 0;
	}
	return realsize;
}
  
/* create a connection handle using CURL
 * @param url is the LoST server URL, e.g. http://ng911serv.irt.cs.columbia.edu:8080
 * @param port is the port number of the server
 * @param chunk is the str string where the body response of POST request will be stored
 */
CURL* lost_http_conn(char *url, int port, str * chunk){

	CURLcode code;
	CURL* conn;

	conn = curl_easy_init();
       
	if (conn == NULL){
		ERROR_LOG("Failed to create CURL connection\n");
 		return NULL;
 	}
 
	code = curl_easy_setopt(conn, CURLOPT_ERRORBUFFER, errorBuffer);
	if (code != CURLE_OK){
		ERROR_LOG("Failed to set error buffer [%d]\n", code);
	 	return NULL;
	}
 
	code = curl_easy_setopt(conn, CURLOPT_URL, url);
	if (code != CURLE_OK){
		ERROR_LOG("Failed to set URL [%s]\n", errorBuffer);
		return NULL;
	}

	code = curl_easy_setopt(conn, CURLOPT_PORT, port);
	if(code != CURLE_OK){
		ERROR_LOG("Failed to set port [%i]\n", port);
		return NULL;
	}
	/*re-send the same request on the new location and follow 
	 * new Location: headers all the way until no more such headers are returned*/
	code = curl_easy_setopt(conn, CURLOPT_FOLLOWLOCATION, 1L);
	if (code != CURLE_OK){
		ERROR_LOG("Failed to set redirect option [%s]\n", errorBuffer);
		return NULL;
	}

	/* send all data to this function  */
	curl_easy_setopt(conn, CURLOPT_WRITEFUNCTION, WriteMemoryCallback);
	
	/* we pass our 'chunk' struct to the callback function */
	curl_easy_setopt(conn, CURLOPT_WRITEDATA, (void *)chunk);
	
	return conn;
}

void lost_http_disconn(CURL* connhandle){

	curl_easy_cleanup(connhandle);
}

int create_lost_req(xmlNode* location, char* service, loc_fmt d_loc_fmt, str* lost_req){

	xmlDocPtr doc= NULL;
   	xmlNodePtr root_node;
   	xmlNodePtr loc_node = NULL, loc_copy=NULL;
	char * id = "1234";
	char * profile;
	xmlChar * req_body = NULL;
	int req_len = 0;

	if(d_loc_fmt == ERR_LOC){
		ERROR_LOG("cannot use location with errornous format\n");
		goto error;
	}

	profile = map_profile[d_loc_fmt];

	/* creating the xml doc for the LoST message*/
   	doc= xmlNewDoc(BAD_CAST "1.0");
   	if(doc== NULL){

   		ERROR_LOG("when creating new xml doc\n");
   		goto error;
   	}
   	root_node = xmlNewNode(NULL, BAD_CAST LOST_FIND_SERVICE_CMD);
   	if(root_node==0){

   		ERROR_LOG("when adding new node %s\n", LOST_FIND_SERVICE_CMD);
   		goto error;
   	}
   	xmlDocSetRootElement(doc, root_node);

	if(!xmlNewNs(root_node, BAD_CAST LOST_NS_HREF, NULL)){
		ERROR_LOG("could not add the namespace %s to the root node\n",
				LOST_NS_HREF);
		goto error;
	}

	loc_node = xmlNewNode(NULL, BAD_CAST LOST_LOCATION_NODE);
	if(!loc_node){
		ERROR_LOG("when creating new node %s\n", LOST_LOCATION_NODE);
		goto error;
	}

	if(!xmlNewProp(loc_node, BAD_CAST LOST_ID_PROP , BAD_CAST id)){
	
		ERROR_LOG("could not add the property %s\n", LOST_ID_PROP);
		goto error;
	}

	if(!xmlNewProp(loc_node, BAD_CAST LOST_PROFILE_PROP , BAD_CAST profile)){
	
		ERROR_LOG("could not add the property %s\n", LOST_ID_PROP);
		goto error;
	}

	//do a recursive copy of the location information
	loc_copy = xmlCopyNode(location, 1);
	if(!loc_copy){
		ERROR_LOG("could not duplicate the location information node\n");
		goto error;
	}

	if(!xmlAddChild(loc_node, loc_copy)){
		ERROR_LOG("could not add the location information to the location node\n");
		goto error;
	}
	loc_copy = NULL;

	if(!xmlAddChild(root_node, loc_node)){
		ERROR_LOG("could not add the %s node to root\n", LOST_LOCATION_NODE);
		goto error;
	}

	loc_node = NULL;
	if(!xmlNewChild(root_node, NULL, BAD_CAST LOST_SERVICE_NODE, BAD_CAST service)){
	
		ERROR_LOG("could not add the %s node to root\n", LOST_SERVICE_NODE);
		goto error;
	}
	//print_element_names(root_node);

	xmlDocDumpFormatMemoryEnc(doc, &req_body, &req_len, LOST_XML_ENC, 1);
	lost_req->s = (char*) req_body;
	lost_req->len = req_len;
	if(!lost_req->s || !lost_req->len){
		ERROR_LOG("could not output the xml document\n");
		goto error;
	}

	//DEBUG_LOG("lost request: %.*s\n", lost_req->len, lost_req->s);

	xmlFreeDoc(doc);

   	return 0;
error:
	if(loc_node)
		xmlFreeNode(loc_node);
	if(loc_copy)
		xmlFreeNode(loc_copy);
	if(doc)
		xmlFreeDoc(doc);
	return 1;
}

int send_POST_data(CURL* connhandle, str data){

	//set headers
	if(curl_easy_setopt(connhandle, CURLOPT_HTTPHEADER, headers) != CURLE_OK){
		ERROR_LOG("Failed to set headers: %s\n", errorBuffer);
		return -1;
	}

	//CURLOPT_POSTFIELDS options to specify what data to post
	if(curl_easy_setopt(connhandle, CURLOPT_POSTFIELDS, data.s) != CURLE_OK){
		ERROR_LOG("Failed to set data: %s\n", errorBuffer);
		return -1;
	}

	//CURLOPT_POSTFIELDSIZE set the data size
	if(curl_easy_setopt(connhandle, CURLOPT_POSTFIELDSIZE, data.len) != CURLE_OK){
		ERROR_LOG("Failed to set content length: %s\n", errorBuffer);
		return -1;
	}
	
	/* post away! */
	if(curl_easy_perform(connhandle) != CURLE_OK){
		ERROR_LOG("Failed to send the request: %s\n", errorBuffer);
		return -1;
	}

	return 0;
}


/* Check if the response is an error, redirect response
 * or it includes a warning
 * @param response - the LoST response body
 * @param resp_type - the response type: ERROR, REDIRECT, WARNING or OK
 * @returns the root node of the parsed xml body
 */ 
xmlNode* get_LoST_resp_type(str response, lost_resp_type * resp_type, str * reason){
			
	xmlNode * root, * node, * child;

	reason->s = NULL;
	reason->len = 0;

	root = xml_parse_string(response);
	if(!root)
		return root;

#ifdef LOST_DEBUG	
	print_element_names(root);
#endif

	if(strcmp((char*)root->name, LOST_ERR_NODE_NAME) == 0){
		DEBUG_LOG("LoST response has an error response\n");
		
		node = child_node(root);
		if(node){
			DEBUG_LOG("reason %s\n", node->name);
			reason->s = (char*)node->name;
			reason->len = strlen(reason->s);			
#ifdef LOST_DEBUG
			print_attr(node, LOST_MSG_ATTR_NAME);
#endif
		}else {
			DEBUG_LOG("the message has no reason\n");
		}

		*resp_type = LOST_ERR;

	} else if(strcmp((char*)root->name, LOST_REDIR_NODE_NAME) == 0){

		DEBUG_LOG("LoST response has a redirect response\n");
#ifdef LOST_DEBUG		
		print_attr(root, LOST_TGT_ATTR_NAME);
#endif
		*resp_type = LOST_REDIR;

	} else 	if(strcmp((char*)root->name, LOST_FINDSRESP_NODE_NAME) != 0){

		ERROR_LOG("invalid LoST response\n");
		*resp_type = LOST_ERR;

	} else {
		
		node = child_named_node(root, LOST_WRNG_NODE_NAME);
		if(node){
			DEBUG_LOG("LoST response has a response with warning\n");
			if((child = child_node(node))){
				DEBUG_LOG("reason %s\n", child->name);
				reason->s = (char*)child->name;
				reason->len = strlen(reason->s);			
#ifdef LOST_DEBUG
				print_attr(node, LOST_MSG_ATTR_NAME);
#endif
			}else {
				DEBUG_LOG("warning without reason\n");
			}

			*resp_type = LOST_WRNG;
		}else
			*resp_type = LOST_OK;
	}

	return root;
}

/* Get the value of the returned psap URI from a LoST response
 * @param root - the root of an parsed xml LoST response other than error or redirect
 * @param exp_type - used to get the type of mapping: no-cache, no-expiration or with an expiration date
 * @param exp_timestamp- used to get the timestamp of when the mapping will expire
 * @param parsed_uri- used to get the parsed uri
 * @returns the name of the psap uri in shm memory
 */
str get_mapped_psap(xmlNode* root, expire_type * exp_type, time_t* exp_timestamp, struct sip_uri* parsed_uri){

	xmlNode * mapping, *uri;
	xmlAttr * expires_attr;
	xmlChar* content;
	str uri_str, shm_uri={NULL, 0};
	char * expires_str;

	if(!(mapping = child_named_node(root, LOST_MAPPING_NODE_NAME))){
		ERROR_LOG("Could not find a mapping element in the LoST response\n");
		return shm_uri;
	}

	if(!(uri = child_named_node(mapping, LOST_URI_NODE_NAME))){
		ERROR_LOG("Could not find any uri child on the mapping element in the LoST response\n");
		return shm_uri;
	}

get_uri:
	content = xmlNodeGetContent((xmlNodePtr)uri);
	uri_str.s = (char*)content;
	if(!uri_str.s || (uri_str.s[0] == '\0')){
		ERROR_LOG("Could not get the content of the uri element\n");
		return shm_uri;
	}
	uri_str.len = strlen(uri_str.s);

	DEBUG_LOG("Found a uri: %.*s\n", uri_str.len, uri_str.s);

	//check if the uri is a well formed sip uri
	if((parse_uri(uri_str.s, uri_str.len, parsed_uri)<0) ||  
			((parsed_uri->type != SIP_URI_T) &&  (parsed_uri->type != SIPS_URI_T))){
	
		ERROR_LOG("the URI %.*s is no SIP/SIPS URI, for the moment only SIP/SIPS URIs are supported\n",
			uri_str.len, uri_str.s);
	
		if(!(uri = sibling_named_node(uri, LOST_URI_NODE_NAME))){
			ERROR_LOG("Could not find any other uri child on the mapping element in the LoST response\n");
			return shm_uri;
		}

		goto get_uri;
	}

	if(!(expires_attr = get_attr(mapping, LOST_EXPIRES_ATTR_NAME))){
		ERROR_LOG("Could not find an expires attr of mapping element in the LoST response\n");
		return shm_uri;
	}	

	expires_str = (char*)expires_attr->children->content;
	if(!expires_str || !strlen(expires_str)){
		ERROR_LOG("Expires attribute with null content\n");
		return shm_uri;
	}

	DEBUG_LOG("expires is %s\n", expires_str);

	//get expiration time ISO 8601
	if(get_time(expires_str, exp_type, exp_timestamp)){
	
		ERROR_LOG("Invalid value for the attribute expires %s\n", expires_str);
		return shm_uri;
	}

	//copy in shm memory
	shm_uri.s = (char*)cds_malloc(uri_str.len*sizeof(char));
	if(!shm_uri.s){
		ERROR_LOG("Out of shm memory\n");

	}else{
		memcpy(shm_uri.s, uri_str.s, uri_str.len*sizeof(char));
		shm_uri.len = uri_str.len;
	}
	xmlFree(content);
	return shm_uri;
}


