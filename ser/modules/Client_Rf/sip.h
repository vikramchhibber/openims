/*
 * $Id: sip.h 1069 2011-03-31 12:10:33Z aon $
 *  
 * Copyright (C) 2004-2006 FhG Fokus
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
 * Main SIP Operations 
 * 
 *  \author Dragos Vingarzan vingarzan -at- fokus dot fraunhofer dot de
 * 
 * Copyright (C) 2005 FhG Fokus
 * 		
 */  

#ifndef WHARF

#ifndef CLIENT_RF_SIP_H
#define CLIENT_RF_SIP_H

#include "../../sr_module.h"
#include "../../parser/contact/parse_contact.h"
#include "../../parser/parse_rr.h"


str cscf_get_call_id(struct sip_msg *msg,struct hdr_field **hr);
int cscf_get_from_uri(struct sip_msg* msg,str *local_uri);
int cscf_get_to_uri(struct sip_msg* msg,str *local_uri);
int cscf_get_expires_hdr(struct sip_msg *msg, int is_shm);
str cscf_get_event(struct sip_msg *msg);
str cscf_get_asserted_identity(struct sip_msg *msg);
int cscf_get_p_charging_vector(struct sip_msg *msg, str * icid, str * orig_ioi, str * term_ioi);
str cscf_get_content_type(struct sip_msg *msg);
str cscf_get_body(struct sip_msg * msg);

#endif /* CLIENT_RF_SIP_H */

#endif /* WHARF */
