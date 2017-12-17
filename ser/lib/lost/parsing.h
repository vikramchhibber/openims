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

#ifndef LIB_LOST_PARSING_H
#define LIB_LOST_PARSING_H

#include <libxml/tree.h>
#include <libxml/parser.h>
#include <libxml/HTMLparser.h>

#include "../../str.h"
#include "../../parser/msg_parser.h"

typedef enum {LOST_OK=0, LOST_WRNG, LOST_ERR, LOST_REDIR} lost_resp_type;
typedef enum {EXP_NO_CACHE, EXP_NO_EXP, EXP_TIME} expire_type;

#define name_compar(str1, str2, len)\
	(strncmp(str1, str2, len)==0)

//parse a string using the libxml2 library
xmlNode * xml_parse_string(str response);
//parse a LoST response and set the type (OK, ERR, WARN) and the reason, if the case
xmlNode* get_LoST_resp_type(str response, lost_resp_type* resp_type, str* reason);
//get the URI from a LoST response, the expiration information and the parsed URI
str get_mapped_psap(xmlNode* root, expire_type *, time_t*, struct sip_uri *);

//get the first node child node of a node
xmlNode* child_node( xmlNode *);
//get the child node with a name
xmlNode * child_named_node(xmlNode * a_node, char* name);
//get the child node with a name and name lenght
xmlNode * child_node_name_len(xmlNode * a_node, char* name, int len);

//get the sibling node with a name
xmlNode * sibling_named_node(xmlNode * a_node, char* name);
//get the sibling node with a name and a name length
xmlNode * sibling_node_name_len(xmlNode * a_node, char* name, int len);

//get a attribute with a name [of a node]
xmlAttr * get_attr(xmlNode * node, char* attr_name);
//get a attribute with a name and name length[of a node]
xmlAttr * get_attr_name_len(xmlNode * node, char* attr_name, int attr_name_len);

//get a namespace with a certain prefix [of a node]
xmlNs* get_ns_prfx_len(xmlNode * node, char* prfx, int prfx_len);
//get a namespace with a certain href [of a node]
xmlNs* get_ns_href_len(xmlNode* node, char* href, int href_len);

//printing functions
void print_attr(xmlNode * node, char * attr_name);
void print_element_names(xmlNode * a_node);

#endif
