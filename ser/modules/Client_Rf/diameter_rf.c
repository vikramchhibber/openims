/*
 * $Id: diameter_rf.c 1167 2011-10-25 09:17:05Z aon $  
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
 * author Ancuta Corici, 
 * 	email andreea dot ancuta dot corici -at- fokus dot fraunhofer dot de
 */
/**
 * Client_Rf - implementation of the Rf interface from the CTF side, according to TS32.299 R7
 * 
 * Scope:
 * - Exports parameters and functions
 * - Initialization functions
 * 
 */

#include "../cdp/diameter_epc_code_cmd.h"
#include "Rf_data.h"
#include "config.h"
#include "diameter_rf.h"
#include "acr.h"

#ifdef WHARF
#define M_NAME "Client_Rf"
#endif

extern cdp_avp_bind_t *cavpb;
extern client_rf_cfg cfg;

int AAASendACR(str * session_id, Rf_ACR_t * rf_data){

	AAAMessage * acr = 0;
	AAASession * auth = NULL;

	if(!session_id){
		auth = cavpb->cdp->AAACreateSession(NULL);
		if(!auth){
			LOG(L_ERR,"ERR:"M_NAME":could not create a new Diameter session\n");
			return 0;
		}
	}else{
	
		if(session_id->s && session_id->len){
			auth = cavpb->cdp->AAAGetAuthSession(*session_id);
			if (!auth) {
				LOG(L_ERR,"ERR:"M_NAME":no Diameter session found\n");
				return 0;
			}
		}else {
			LOG(L_ERR, "empty session id\n");
			return 0;
		}
	}
	
	if(!(acr = Rf_new_acr(auth, rf_data)))
                goto error;

	cavpb->cdp->AAASessionsUnlock(auth->hash);
        cavpb->cdp->AAASendMessageToPeer(acr, &cfg.destination_host, 0,0);

	if(!session_id)
		cavpb->cdp->AAADropSession(auth);

	return 1;
error:
	return 0;
}

/**
 * Handler for incoming Diameter requests.
 * @param request - the received request
 * @param param - generic pointer
 * @returns the answer to this request
 */
void RfChargingResponseHandler(AAAMessage *response,void *param)
{
	switch(response->applicationId){
        	case IMS_Rf:
			switch(response->commandCode){	
				case Diameter_ACA:
					break;			
				default :
					LOG(L_ERR,"ERR:"M_NAME":RfChargingResponseHandler: - "
							"Received unknown response for Rf command %d, flags %#1x endtoend %u hopbyhop %u\n",
							response->commandCode,response->flags, 
							response->endtoendId, response->hopbyhopId);
					return;
							
			}
			break;
		default:
			/*LOG(L_ERR,"DBG:"M_NAME":RfChargingResponseHandler(): - Received unknown response for app %d command %d\n",
				response->applicationId,
				response->commandCode);*/
				return;
							
	}
}

