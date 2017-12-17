/*
 * $Id: config.c 1167 2011-10-25 09:17:05Z aon $
 *
 * Copyright (C) 2009 Fhg Fokus
 *
 */
/**
 * \file
 * 
 * CDF module for Wharf - Module configuration
 * 
 * 
 *  \author Andreea Ancuta Onofrei Corici andreea dot ancuta dot corici -at- fokus dot fraunhofer dot de
 * 
 */  

#ifdef WHARF

#include "../../utils/utils.h"
 
#include "config.h"

extern client_rf_cfg cfg;

/**
 * Parse a MobileIP module configuration from a configuration XML
 * @param config
 * @return
 */
int client_rf_parse_config(str config)
{
	xmlDocPtr doc=0;
	xmlDtdPtr dtd=0;
	xmlNodePtr root=0,i=0;
	xmlChar *xc=0;
	long int l;
	str config_dtd;
	config_dtd.s=CLIENT_RF_CONFIG_DTD;
	config_dtd.len = strlen(config_dtd.s);
	
	doc = xml_get(config);
	if (!doc){
		LOG(L_ERR,"  The configuration does not contain a valid XML\n");
		goto error;
	}
	
	dtd = xml_get_dtd(config_dtd);
	if (!dtd){
		LOG(L_ERR,"  The DTD is not valid\n");
		goto error;
	}
	
	if (xml_validate_dtd(doc,dtd)!=1){
		LOG(L_ERR,"  Verification of configuration XML against DTD failed!\n");
		goto error;
	}
	
	root = xmlDocGetRootElement(doc);
	if (!root){
		LOG(L_ERR,"  Empty XML\n");
		goto error;
	}
	
	while(root && (root->type!=XML_ELEMENT_NODE || strcasecmp((char*)root->name,"Client_Rf")!=0)){
		root = root->next;
	}
	
	if (!root) {
		LOG(L_ERR," No <Client_Rf> found in configuration\n");
		goto error;
	}

	for(i=root->children;i;i=i->next)
		if (i->type==XML_ELEMENT_NODE) {

			/* Rf */
			if (strcasecmp((char*)i->name,"Rf")==0){
				
				if (!xml_get_prop_as_int(i,"node_func",&l)||l<0 || l>10){
					LOG(L_ERR,"node_role value between 0 and 10\n");
					goto error;
				}else
					cfg.node_func = (int)l;				
			

				xc = xmlGetProp(i,(xmlChar*)"origin_host");
				if (xc){	
					char_dup_str(cfg.origin_host,xc,shm);
					xmlFree(xc);
				}
				xc = xmlGetProp(i,(xmlChar*)"origin_realm");
				if (xc){	
					char_dup_str(cfg.origin_realm,xc,shm);
					xmlFree(xc);
				}			
				xc = xmlGetProp(i,(xmlChar*)"destination_host");
				if (xc){	
					char_dup_str(cfg.destination_host,xc,shm);
					xmlFree(xc);
				}	
				xc = xmlGetProp(i,(xmlChar*)"destination_realm");
				if (xc){	
					char_dup_str(cfg.destination_realm,xc,shm);
					xmlFree(xc);
				}
				xc = xmlGetProp(i,(xmlChar*)"service_context_id");
				if (xc){	
					char_dup_str(cfg.service_context_id,xc,shm);
					xmlFree(xc);
				}		
			} else if (strcasecmp((char*)i->name,"Hash")==0){
				if (!xml_get_prop_as_int(i,"hash_size",&l)||l<0){
					LOG(L_ERR,"invalid hash size, value >0 are accepted\n");
				goto error;
			}else
				cfg.hash_table_size = (int)l;			
			}
		}
	
	if (doc) xml_free(doc);	
	if (dtd) xml_free_dtd(dtd);
	return 1;
error:
out_of_memory:
	if (xc) xmlFree(xc);
	if (doc) xml_free(doc);
	if (dtd) xml_free_dtd(dtd);
	return 0;
	
}

void client_rf_free_config(){

	str_free(cfg.origin_host, shm);
	str_free(cfg.origin_realm, shm);
	str_free(cfg.destination_host, shm);
	str_free(cfg.destination_realm, shm);
}

#endif /* WHARF */

