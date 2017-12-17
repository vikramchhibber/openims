/*
 * $Id: emerg.c 998 2010-09-21 14:24:14Z aon $
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
 * Service-CSCF -Emergency Related Operations
 * 
 * 
 *  \author Ancuta Onofrei	andreea dot ancuta dot onofrei -at- fokus dot fraunhofer dot de
 */

#include <time.h>

#include "../../parser/contact/contact.h"
#include "../../parser/contact/parse_contact.h"
#include "../../ut.h"
#include "../../modules/tm/tm_load.h"
#include "../cdp/cdp_load.h"
#include "../../dset.h"

#include "emerg.h"
#include "registrar.h"
#include "registration.h"
#include "mod.h"
#include "registrar_parser.h"
#include "registrar_storage.h"
#include "sip.h"
#include "cx.h"
#include "cx_avp.h"
#include "sip_messages.h"
#include "dlg_state.h"


/**
 * Finds if the message comes from a user that made an Emergency Registration
 * @param msg - the SIP message
 * @param str1 - not used
 * @param str2 - not used
 * @returns #CSCF_RETURN_TRUE if sos uri parameter in Contact header, #CSCF_RETURN_FALSE if not, or #CSCF_RETURN_FALSE
 */
int S_emergency_flag(struct sip_msg *msg,char *str1,char *str2)
{
	contact_t *c;
	struct hdr_field *h;
	int sos_reg;
	int x;

	sos_reg = 0;

	LOG(L_INFO,"DBG:"M_NAME":S_emergency_flag: Check if the user made an Emergency Registration\n");
	
	for(h=msg->contact;h;h=h->next)
		if (h->type==HDR_CONTACT_T && h->parsed)
		 for(c=((contact_body_t*)h->parsed)->contacts;c;c=c->next){
			LOG(L_DBG,"DBG:"M_NAME":S_emergency_flag: contact <%.*s>\n",c->uri.len,c->uri.s);
			
			x = cscf_get_sos_uri_param(c->uri);
			if(x < 0)
				return CSCF_RETURN_ERROR;
			sos_reg += x;
		}
	
	if(sos_reg)
		return CSCF_RETURN_TRUE;

	return CSCF_RETURN_FALSE;
}

