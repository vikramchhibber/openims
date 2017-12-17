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

#include "client.h"
#include "pidf_loc.h"
#include "parsing.h"

//the root of the PIDF-LO is a node with the name "presence", see RFC 4119
static xmlNode* get_loc_info_kids(xmlNode* presence){

	xmlNode * res, *child;
	
	res = NULL;

	if(!presence){
	
		ERROR_LOG("null argument\n");
		return res;
	}
	//search for the first device element
	child = child_node_name_len(presence, PIDF_DEV, PIDF_DEV_LEN);
	//search for the first tuple element
	if(!child){
		child = child_node_name_len(presence, PIDF_TUPL, PIDF_TUPL_LEN);
		//search for the status element from a tuple
		if(child){
			child = child_node_name_len(child, PIDF_STATUS, PIDF_STATUS_LEN);
			if(!child){
				ERROR_LOG("malformed request: tuple node has no status child\n");
				return res;	
			}
		}
	}
	//search for the first person element
	if(!child){
		child = child_node_name_len(presence, PIDF_PERS, PIDF_PERS_LEN);
		if(!child){
			ERROR_LOG("malformed request: no device, tuple, or person element\n");
			return res;
		}

	}

	DEBUG("found a location node: type %s\n", (char*)child->name);

	child = child_node_name_len(child, PIDF_GEOPRIV, PIDF_GEOPRIV_LEN);
	if(!child){
		ERROR_LOG("malformed request: no geopriv child element\n");
		return res;
	}

	res = child_node_name_len(child, PIDF_INFO, PIDF_INFO_LEN);
	if(!res){
		ERROR_LOG("malformed request: geopriv element has no location-info child element\n");
		return res;
	}
	if(!res->children){
	
		ERROR_LOG("location-info node has no children\n");
	}

	return res;

}
//like in RFC 4119
xmlNode * is_geo_coord_fmt(xmlNode * locNode, loc_fmt * crt_loc_fmt){

	xmlNode * node;
	
	node = locNode;
	char * name = (char*)node->name;
	DEBUG("checking if %s node has geo_coord format, like in RFC 4119\n", name);

	if(!(name_compar(name,  PIDF_POINT_SHAPE, PIDF_POINT_SHAPE_LEN))){
		ERROR_LOG("the location node is not a RFC 4119 geodetic location info\n");
	        *crt_loc_fmt = ERR_LOC;
		return NULL;
	}
	DEBUG("found node Point, geo coord format present\n");
	*crt_loc_fmt = GEO_COORD_LOC;
	return node;
}


xmlNode * is_geo_shape_fmt(xmlNode* node, loc_fmt * crt_loc_fmt){

	char* name;

	name = (char*)node->name;
	DEBUG("checking if %s node has geo_shape format\n", name);

	if(name_compar(name, PIDF_POINT_SHAPE, PIDF_POINT_SHAPE_LEN) || 
			name_compar(name, PIDF_POLYGON_SHAPE, PIDF_POINT_SHAPE_LEN) ||
			name_compar(name, PIDF_CIRCLE_SHAPE, PIDF_CIRCLE_SHAPE_LEN) ||
			name_compar(name, PIDF_ELLIPSE_SHAPE, PIDF_ELLIPSE_SHAPE_LEN) ||
			name_compar(name, PIDF_ARC_BAND_SHAPE, PIDF_ARC_BAND_SHAPE_LEN)){
		
		*crt_loc_fmt = GEO_SHAPE_LOC;
	}else if(name_compar(name, PIDF_SPHERE_SHAPE, PIDF_SPHERE_SHAPE_LEN) ||
			name_compar(name, PIDF_ELLIPSOID_SHAPE, PIDF_ELLIPSOID_SHAPE_LEN) ||
			name_compar(name, PIDF_PRISM_SHAPE, PIDF_PRISM_SHAPE_LEN)){
		
		*crt_loc_fmt = GEO_NEW_SHAPE_LOC;
	}else{
		ERROR_LOG("no valid geo shape found\n");
		*crt_loc_fmt = ERR_LOC;
		node = NULL;
	}	
	
	return node;
}

xmlNode * is_RFC4119_civic_fmt(xmlNode * locNode, loc_fmt * crt_loc_fmt){

	xmlNode * node;
	char * cur_name;
	cur_name = (char*)locNode->name;
	
	DEBUG("checking if node %s has RFC 4119 civic format\n", cur_name);

	if (name_compar(cur_name, PIDF_CIV_LOC, PIDF_CIV_LOC_LEN)){
		DEBUG("found a %.*s child, has RFC 4119 civic format\n",
				PIDF_CIV_LOC_LEN, PIDF_CIV_LOC);
		*crt_loc_fmt = OLD_CIV_LOC;
		node = locNode;
	}else{
		ERROR_LOG("did not find a %.*s child, no civic fromat\n",
				PIDF_CIV_LOC_LEN, PIDF_CIV_LOC);
	       	*crt_loc_fmt = ERR_LOC;
	       	node = NULL;
	}

	return node;
}

//if firt child is location, then geodetic-2d type
//if the node is civicAddress, then type civic type 
//if there is a namespace with href PIDF_OLD_CIV_NS_HREF=>old type of civic address
//if there is a namespace with href PIDF_NEW_CIV_NS_HREF=>new type of civic address
xmlNode* is_new_civic_fmt(xmlNode * locNode, loc_fmt * crt_loc_fmt){

	xmlNode * node;

	char * cur_name;
	cur_name = (char*)locNode->name;
	
	DEBUG("checking if node %s has new civic format\n", cur_name);
	
	if (name_compar(cur_name, PIDF_CIV_LOC, PIDF_CIV_LOC_LEN)){
		DEBUG("found a %.*s node, has new civic format\n",
				PIDF_CIV_LOC_LEN, PIDF_CIV_LOC);
		*crt_loc_fmt = NEW_CIV_LOC;
		node = locNode;
	}else{
		ERROR_LOG("did not find a %.*s node, no civic fromat\n",
				PIDF_CIV_LOC_LEN, PIDF_CIV_LOC);
	       	*crt_loc_fmt = ERR_LOC;
	       	node = NULL;
	}

	return node;
}

/* Checks if the xml tree corresponds to a PIDF-LO
 * @param presence is the root node, for the presence element node
 * @param loc - the address of the xmlNode pointing to location information if ok, otherwise NULL
 * @param crt_loc_fmt - the type of location format: geodetic-2d, civic new or old, or other
 * @returns 0 if ok, 1 if malformed and 2 if not a location object is present, 3 if BUG
 */
xmlNode* has_loc_info(int * code, xmlNode* presence, loc_fmt * crt_loc_fmt){

	xmlNs * ns;
	char * content;
	xmlNode * locNode,* res , *cur_node;

	res = NULL;

	if(!presence){
		ERROR_LOG("has_loc_info: NULL presence object!!!\n");
		*code = 3;
		goto err;
	}
	
	locNode = get_loc_info_kids(presence);
	if(!locNode){
		*code = 1;
		goto err;
	}

	ns = get_ns_prfx_len(presence, PIDF_GEO_NS_PRFX, PIDF_GEO_NS_PRFX_LEN);
	if(!ns){
	
		DEBUG("has_loc_info:the application/pidf+xml has no xmlns:%.*s namespace in the presence node\n",
				PIDF_GEO_NS_PRFX_LEN, PIDF_GEO_NS_PRFX);
	
		ns = get_ns_prfx_len(locNode, PIDF_GEO_NS_PRFX, PIDF_GEO_NS_PRFX_LEN);
		if(!ns){
	
			DEBUG("has_loc_info:the application/pidf+xml location-info node has no xmlns:%.*s namespace, ignoring it\n",
				PIDF_GEO_NS_PRFX_LEN, PIDF_GEO_NS_PRFX);
			*code = 2;
			goto err;
		}
	}
	
	if(ns->href == NULL){

		ERROR_LOG("Malformed PIDF_LO : location-info node with no value for the namespace xmlns:%.*s\n",
				PIDF_GEO_NS_PRFX_LEN, PIDF_GEO_NS_PRFX);
		*code = 1;
		goto err;
	}
	
	content = (char*)ns->href;
	if(!(name_compar(content, PIDF_GEO_NS_HREF, PIDF_GEO_NS_HREF_LEN))){
		ERROR_LOG("namespace xmlns:%.*s of the location-info node has not the expected value %.*s\n",
				PIDF_GEO_NS_PRFX_LEN, PIDF_GEO_NS_PRFX,
				PIDF_GEO_NS_PRFX_LEN, PIDF_GEO_NS_HREF);
		*code = 2;
		goto err;
	}
	
	//for all the children of the location-info node
	for (cur_node = locNode->children; cur_node; cur_node = cur_node->next) {
        
		if (cur_node->type != XML_ELEMENT_NODE) 	
			continue;

		//check if the prefix ca is present in a xmlnamespace
		ns = get_ns_href_len(cur_node, PIDF_NEW_CIV_NS_HREF, PIDF_NEW_CIV_NS_HREF_LEN);
		if(ns){
			res = is_new_civic_fmt(cur_node, crt_loc_fmt);
			if(res)
				return res;
		}

		//check if the prefix gs is present in a xmlnamespace
		ns = get_ns_href_len(cur_node, PIDF_GEO_SHAPE_NS_HREF, PIDF_GEO_SHAPE_NS_HREF_LEN);
		if(ns){
			res = is_geo_shape_fmt(cur_node, crt_loc_fmt);
			if(res)
				return res;
		}

		//check if the prefix gml is present in a xmlnamespace
		ns = get_ns_href_len(cur_node, PIDF_GEO_COORD_NS_HREF, PIDF_GEO_COORD_NS_HREF_LEN);
		if(ns){
			res = is_geo_coord_fmt(cur_node, crt_loc_fmt);
			if(res)
				return res;
		}

	
		//check if the prefix cl is present in a xmlnamespace
		ns = get_ns_href_len(cur_node, PIDF_OLD_CIV_NS_HREF, PIDF_OLD_CIV_NS_HREF_LEN);
		if(ns){
			res = is_RFC4119_civic_fmt(cur_node, crt_loc_fmt);
			if(res)
				return res;
		}

		ERROR_LOG("no valid xml namespace with the valid href values  %.*s, %.*s, %.*s or %.*s, or the right format\n",
				PIDF_NEW_CIV_NS_HREF_LEN, PIDF_NEW_CIV_NS_HREF,
				PIDF_GEO_SHAPE_NS_HREF_LEN, PIDF_GEO_SHAPE_NS_HREF,
				PIDF_GEO_COORD_NS_HREF_LEN, PIDF_GEO_COORD_NS_HREF,
				PIDF_OLD_CIV_NS_HREF_LEN, PIDF_OLD_CIV_NS_HREF);
	}
err:
	return res;
}
