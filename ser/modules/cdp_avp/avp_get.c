/**
 * $Id: avp_get.c 926 2010-03-08 10:55:30Z vingarzan $
 *   
 * Copyright (C) 2009-2010 FhG Fokus
 *
 * This file is part of Open EPC - an implementation of EPS core components
 * 
 * The Open EPC project  is a prototype implementation of the 3GPP 
 * Release 8 and later Evolved Packet Core (EPC) that will allow academic and 
 * industrial researchers and engineers around the world to obtain a 
 * practical look and feel of the capabilities of the Evolved Packet Core.
 * For more information on the Open EPC project, please visit www.openepc.net
 * or please contact Fraunhofer FOKUS by e-mail at the following addresses:
 *     info@openepc.net
 *
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
 * CDiameter AVP Operations modules - AVP Retrieval
 * 
 * 
 *  \author Dragos Vingarzan dragos dot vingarzan -at- fokus dot fraunhofer dot de
 * 
 */ 

#include "avp_get.h"

extern struct cdp_binds *cdp;

/**
 * Iteratively find an AVP matching in a list
 *  Should be called subsequently with the old result, unless it was null (which would cause a restart of the search).
 * @param msg
 * @param avp_code
 * @param avp_vendor_id
 * @param start_avp - avp where to resume. A null value will trigger a restart of the search.
 * @return the AAA_AVP* or NULL if not found (anymore)
 */
inline AAA_AVP* cdp_avp_get_next_from_list(AAA_AVP_LIST list,int avp_code,int avp_vendor_id,AAA_AVP *start_avp)
{
	AAA_AVP *avp;
	if (!start_avp)	start_avp = list.head;
	else start_avp = start_avp->next;
	LOG(L_DBG,"Looking for AVP with code %d vendor id %d startin at avp %p\n",
			avp_code,avp_vendor_id,start_avp);
	
	if (!start_avp){
		LOG(L_DBG,"Failed finding AVP with Code %d and VendorId %d - Empty list or at end of list\n",avp_code,avp_vendor_id);
		return 0;
	}
	avp = cdp->AAAFindMatchingAVPList(list,start_avp,avp_code,avp_vendor_id,AAA_FORWARD_SEARCH);
	if (avp==0){
		LOG(L_DBG,"Failed finding AVP with Code %d and VendorId %d - at end of list\n",avp_code,avp_vendor_id);
		return 0;
	}

	return avp;
}

/**
 * Iteratively find an AVP matching in a message
 * @param msg
 * @param avp_code
 * @param avp_vendor_id
 * @param start_avp
 * @return the AAA_AVP* or NULL if not found (anymore)
 */
inline AAA_AVP* cdp_avp_get_next_from_msg(AAAMessage *msg,int avp_code,int avp_vendor_id,AAA_AVP *start_avp)
{
	return cdp_avp_get_next_from_list(msg->avpList,avp_code,avp_vendor_id,start_avp);
}

/**
 * Find the first matching AVP in a list and return it 
 * @param list
 * @param avp_code
 * @param avp_vendor_id
 * @return the AAA_AVP* or null if not found
 */
inline AAA_AVP* cdp_avp_get_from_list(AAA_AVP_LIST list,int avp_code,int avp_vendor_id)
{
	return cdp_avp_get_next_from_list(list,avp_code,avp_vendor_id,0);
}

/**
 * Find the first AVP matching in the message and return it
 * @param msg
 * @param avp_code
 * @param avp_vendor_id
 * @return the AAA_AVP* or null if not found
 */
inline AAA_AVP* cdp_avp_get_from_msg(AAAMessage *msg,int avp_code,int avp_vendor_id)
{
	if (!msg){
		LOG(L_ERR,"Failed finding AVP with Code %d and VendorId %d in NULL message!\n",avp_code,avp_vendor_id);
		return 0;
	}
	return cdp_avp_get_from_list(msg->avpList,avp_code,avp_vendor_id);
}
