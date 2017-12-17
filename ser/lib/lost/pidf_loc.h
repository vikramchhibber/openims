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

#ifndef LIB_LOST_PIDF_LOCAT_H
#define LIB_LOST_PIDF_LOCAT_H

#include "parsing.h"

typedef enum {GEO_COORD_LOC=0, GEO_SHAPE_LOC, GEO_NEW_SHAPE_LOC, OLD_CIV_LOC, NEW_CIV_LOC, OTHER_LOC, ERR_LOC} loc_fmt;

#define PIDF_DEV			"device"
#define PIDF_DEV_LEN			((sizeof(PIDF_DEV)-1)/sizeof(char))

#define PIDF_TUPL			"tuple"
#define PIDF_TUPL_LEN			((sizeof(PIDF_TUPL)-1)/sizeof(char))

#define PIDF_PERS			"person"
#define PIDF_PERS_LEN			((sizeof(PIDF_PERS)-1)/sizeof(char))

#define PIDF_STATUS			"status"
#define PIDF_STATUS_LEN			((sizeof(PIDF_STATUS)-1)/sizeof(char))

#define PIDF_GEOPRIV			"geopriv"
#define PIDF_GEOPRIV_LEN		((sizeof(PIDF_GEOPRIV)-1)/sizeof(char))

#define PIDF_INFO			"location-info"
#define PIDF_INFO_LEN			((sizeof(PIDF_INFO)-1)/sizeof(char))

#define PIDF_GML_LOC			"location"
#define PIDF_GML_LOC_LEN		((sizeof(PIDF_GML_LOC)-1)/sizeof(char))

#define PIDF_CIV_LOC			"civicAddress"
#define PIDF_CIV_LOC_LEN		((sizeof(PIDF_CIV_LOC)-1)/sizeof(char))

//geopriv location information, defined in RFC 4119
#define PIDF_GEO_NS_PRFX		"gp"
#define PIDF_GEO_NS_PRFX_LEN		((sizeof(PIDF_GEO_NS_PRFX)-1)/sizeof(char))

#define PIDF_GEO_NS_HREF		"urn:ietf:params:xml:ns:pidf:geopriv10"
#define PIDF_GEO_NS_HREF_LEN 		((sizeof(PIDF_GEO_NS_HREF)-1)/sizeof(char))

//geopriv coordinated namespace, defined in RFC 4119
#define PIDF_GEO_COORD_NS_PRFX		"gml"
#define PIDF_GEO_COORD_NS_PRFX_LEN	((sizeof(PIDF_GEO_COORD_NS_PRFX)-1)/sizeof(char))

#define PIDF_GEO_COORD_NS_HREF		"urn:opengis:specification:gml:schema-xsd:feature:v3.0"
#define PIDF_GEO_COORD_NS_HREF_LEN 	((sizeof(PIDF_GEO_COORD_NS_HREF)-1)/sizeof(char))

//geopriv shapes namespace, defined in draft-ietf-geopriv-pdif-lo-profile
#define PIDF_GEO_SHAPE_NS_PRFX		"gs"
#define PIDF_GEO_SHAPE_NS_PRFX_LEN	((sizeof(PIDF_GEO_SHAPE_NS_PRFX)-1)/sizeof(char))

#define PIDF_GEO_SHAPE_NS_HREF		"http://www.opengis.net/gml"
#define PIDF_GEO_SHAPE_NS_HREF_LEN 	((sizeof(PIDF_GEO_SHAPE_NS_HREF)-1)/sizeof(char))


//xmlns:gml="http://www.opengis.net/gml or 
//xmlns:gml="urn:opengis:specification:gml:schema-xsd:feature:v3.0" for 
//geodetic location information

//old format for civic location information, defined in RFC 4119
#define PIDF_OLD_CIV_NS_PRFX		"cl"
#define PIDF_OLD_CIV_NS_PRFX_LEN	((sizeof(PIDF_OLD_CIV_NS_PRFX)-1)/sizeof(char))

#define PIDF_OLD_CIV_NS_HREF		"urn:ietf:params:xml:ns:pidf:geopriv10:civicLoc"
#define PIDF_OLD_CIV_NS_HREF_LEN	((sizeof(PIDF_OLD_CIV_NS_HREF)-1)/sizeof(char))

//new format for civic location information, defined in RFC 5139 and used by the LoST protocol
#define PIDF_NEW_CIV_NS_PRFX		"ca"
#define PIDF_NEW_CIV_NS_PRFX_LEN	((sizeof(PIDF_NEW_CIV_NS_PRFX)-1)/sizeof(char))

#define PIDF_NEW_CIV_NS_HREF		"urn:ietf:params:xml:ns:pidf:geopriv10:civicAddr"
#define PIDF_NEW_CIV_NS_HREF_LEN	((sizeof(PIDF_NEW_CIV_NS_HREF)-1)/sizeof(char))	


//types of geodetic shapes: supported by LoST
#define PIDF_POINT_SHAPE		"Point"
#define PIDF_POINT_SHAPE_LEN		((sizeof(PIDF_POINT_SHAPE)-1)/sizeof(char))

#define PIDF_POLYGON_SHAPE		"Polygon"
#define PIDF_POLYGON_SHAPE_LEN		((sizeof(PIDF_POLYGON_SHAPE)-1)/sizeof(char))


#define PIDF_CIRCLE_SHAPE		"Circle"
#define PIDF_CIRCLE_SHAPE_LEN		((sizeof(PIDF_CIRCLE_SHAPE)-1)/sizeof(char))

#define PIDF_ELLIPSE_SHAPE		"Ellipse"
#define PIDF_ELLIPSE_SHAPE_LEN		((sizeof(PIDF_ELLIPSE_SHAPE)-1)/sizeof(char))

#define PIDF_ARC_BAND_SHAPE		"ArcBand"
#define PIDF_ARC_BAND_SHAPE_LEN		((sizeof(PIDF_ARC_BAND_SHAPE)-1)/sizeof(char))

//types of shapes defined in draft-ietf-geopriv-pdif-lo-profile and not supported by LoST
#define PIDF_SPHERE_SHAPE		"Sphere"
#define PIDF_SPHERE_SHAPE_LEN		((sizeof(PIDF_SPHERE_SHAPE)-1)/sizeof(char))

#define PIDF_ELLIPSOID_SHAPE		"Ellipsoid"
#define PIDF_ELLIPSOID_SHAPE_LEN	((sizeof(PIDF_ELLIPSOID_SHAPE)-1)/sizeof(char))

#define PIDF_PRISM_SHAPE		"Prism"
#define PIDF_PRISM_SHAPE_LEN		((sizeof(PIDF_PRISM_SHAPE)-1)/sizeof(char))

xmlNode* has_loc_info(int *, xmlNode* presence, loc_fmt * crt_loc_fmt);

#endif
