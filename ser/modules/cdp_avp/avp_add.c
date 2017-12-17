/**
 * $Id: avp_add.c 926 2010-03-08 10:55:30Z vingarzan $
 *   
 * Copyright (C) 2009-2010 FhG Fokus
 *
 * This file is part of Open EPC - an implementation of the EPS core components
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
 * CDiameter AVP Operations modules - AVP adding to lists, messages
 * 
 * 
 *  \author Dragos Vingarzan dragos dot vingarzan -at- fokus dot fraunhofer dot de
 * 
 */ 

#include "avp_add.h"
#include "avp_new.h"

extern struct cdp_binds *cdp;


/**
 * Adds a new AVP to a list
 * @param list
 * @param d
 * @param len
 * @param avp_code
 * @param avp_flags
 * @param avp_vendorid
 * @param data_do
 * @return
 */
inline int cdp_avp_add_new_to_list(AAA_AVP_LIST *list,int avp_code,int avp_flags,int avp_vendorid,str data,AVPDataStatus data_do)
{
	AAA_AVP *avp = cdp_avp_new(avp_code,avp_flags,avp_vendorid,data,data_do);
	if (!list){
		LOG(L_ERR,"Failed adding to NULL list AVP with Code [%d] Flags [%d] VendorID [%d] from data of length [%d]!\n",
			avp_code,avp_flags,avp_vendorid,data.len);
		if (avp) cdp->AAAFreeAVP(&avp);
		return 0;
	}
	if (!avp){
		LOG(L_ERR,"Error creating AVP with Code [%d] Flags [%d] VendorID [%d] from data of length [%d]!\n",
			avp_code,avp_flags,avp_vendorid,data.len);
		return 0;
	}
	cdp->AAAAddAVPToList(list,avp);
	return 1;			
}

/**
 * Adds a new AVP to a message
 * @param msg
 * @param d
 * @param len
 * @param avp_code
 * @param avp_flags
 * @param avp_vendorid
 * @param data_do
 * @return 1 on success or 0 on failure
 */
inline int cdp_avp_add_new_to_msg(AAAMessage *msg,int avp_code,int avp_flags,int avp_vendorid,str data,AVPDataStatus data_do)
{
	if (!msg){
		LOG(L_ERR,"Failed adding to NULL message AVP with Code [%d] Flags [%d] VendorID [%d] from data of length [%d]!\n",
			avp_code,avp_flags,avp_vendorid,data.len);
		return 0;
	}
	return cdp_avp_add_new_to_list(&(msg->avpList),avp_code,avp_flags,avp_vendorid,data,data_do);
}

/**
 * Adds an AVP to a list
 * \Note The avp should no longer be used after this operation. If multiple additions are required, duplicate before
 * @param list
 * @param avp
 * @return 1 on success or 0 on failure
 */
inline int cdp_avp_add_to_list(AAA_AVP_LIST *list,AAA_AVP *avp)
{
	if (!avp){
		LOG(L_ERR,"Can not add NULL AVP to list!\n");
		return 0;
	}	
	if (!list){
		LOG(L_ERR,"Failed adding to NULL list AVP with Code [%d] Flags [%d] VendorID [%d] from data of length [%d]!\n",
			avp->code,avp->flags,avp->vendorId,avp->data.len);
		return 0;
	}
	cdp->AAAAddAVPToList(list,avp);
	return 1;			
}

/**
 * Adds an AVP to a message
 * \Note The avp should no longer be used after this operation. If multiple additions are required, duplicate before
 * @param msg
 * @param avp
 * @return 1 on success or 0 on failure
 */
inline int cdp_avp_add_to_msg(AAAMessage *msg,AAA_AVP *avp)
{
	if (!msg){
		LOG(L_ERR,"Failed adding to NULL message AVP with Code [%d] Flags [%d] VendorID [%d] from data of length [%d]!\n",
			avp->code,avp->flags,avp->vendorId,avp->data.len);
		return 0;
	}
	return cdp_avp_add_to_list(&(msg->avpList),avp);
}
