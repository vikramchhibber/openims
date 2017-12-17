/**
 * $Id: sip_body.h 1147 2011-09-06 07:02:28Z aon $
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
 * 
 *  \author Dragos Vingarzan dragos dot vingarzan -at- fokus dot fraunhofer dot de
 * 
 */
#ifndef SIP_BODY_H
#define SIP_BODY_H


str cscf_get_multipart_boundary(str content_type);

str cscf_get_multipart_part(str body,str boundary,int *position);

str cscf_get_multipart_body(str part);

str cscf_get_multipart_content_type(str part);


str cscf_get_body_with_type_from_body(str body,str body_content_type,str search_content_type,str *ret_part);

str cscf_get_body_with_type(struct sip_msg *msg,str search_content_type,str *ret_part);

/* this function calls all the other functions, 
 * this is the one that should be used when searching for a specific content-type*/
str cscf_get_body_with_content_type(struct sip_msg *msg,str search_content_type);

int extract_sdp_body(struct sip_msg *msg, str *body );

#endif /* SIP_BODY_H */
