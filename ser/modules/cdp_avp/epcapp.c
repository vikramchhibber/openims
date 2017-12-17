/**
 * $Id: epcapp.c 984 2010-09-12 12:46:43Z aon $
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
 * CDiameter AVP Operations modules
 * 
 * 
 *  \author Dragos Vingarzan dragos dot vingarzan -at- fokus dot fraunhofer dot de
 * 
 */ 

#include "avp_new.h"
#include "avp_new_base_data_format.h"
#include "avp_add.h"
#include "avp_get.h"
#include "avp_get_base_data_format.h"

extern struct cdp_binds *cdp;


#include "../cdp/cdp_load.h"


#include "epcapp.h"
#include "ccapp.h"


#define CDP_AVP_DEFINITION

	#include "epcapp.h"
	int cdp_avp_add_GG_Enforce_Group(AAA_AVP_LIST * avpList, 
		int32_t type, str id, 
		ip_address ue_ip, ip_address gg_ip, 
		uint32_t interval,
		AVPDataStatus status){

		AAA_AVP_LIST        avp_list = {0,0}, avp_list2 = {0,0};

		if(!cdp_avp_add_UE_Locator(&avp_list, ue_ip))
			goto error;

		if(id.len && id.s){
			if(!cdp_avp_add_Subscription_Id_Group(&avp_list,
					type,
					id,
					AVP_DUPLICATE_DATA))
				goto error;
		}

		if(!cdp_avp_add_UE_Locator_Id_Group(&avp_list2, 
				&avp_list, AVP_FREE_DATA))
			goto error;

		if(!cdp_avp_add_GG_IP(&avp_list2, gg_ip))
			goto error;
	
		if(!cdp_avp_add_GG_Enforce(avpList, &avp_list2,AVP_FREE_DATA)){
			LOG(L_ERR, "could not find the GG_Enforce AVP\n");
			goto error;
		}
		return 1;
	error:
		LOG(L_ERR, "error while adding the GG change AVPs\n");
		return 0;
	}

#undef CDP_AVP_DEFINITION




