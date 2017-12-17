/**
 * $Id: cdp_load.h 920 2010-03-05 19:07:50Z vingarzan $
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
 * CDiameterPeer - functional bindings for usage in other SER modules
 * 
 *  \author Dragos Vingarzan vingarzan -at- fokus dot fraunhofer dot de
 */

#ifndef _CDP_BIND_H
#define _CDP_BIND_H


#include "utils.h"
#include "diameter.h"
#include "diameter_api.h"
#include "diameter_ims.h"
#include "diameter_epc.h"
#include "session.h"
#include "peer.h"


struct cdp_binds {
	AAACreateRequest_f			AAACreateRequest;
	AAACreateResponse_f			AAACreateResponse;	
	AAAFreeMessage_f			AAAFreeMessage;
	
	
	AAACreateAVP_f				AAACreateAVP;
	AAAAddAVPToMessage_f		AAAAddAVPToMessage;
	AAAAddAVPToList_f			AAAAddAVPToList;
	AAAFindMatchingAVP_f		AAAFindMatchingAVP;
	AAAFindMatchingAVPList_f	AAAFindMatchingAVPList;
	AAAGetNextAVP_f				AAAGetNextAVP;
	AAAFreeAVP_f				AAAFreeAVP;
	AAAFreeAVPList_f			AAAFreeAVPList;
	AAAGroupAVPS_f				AAAGroupAVPS;
	AAAUngroupAVPS_f			AAAUngroupAVPS;

	AAASendMessage_f			AAASendMessage;
	AAASendMessageToPeer_f		AAASendMessageToPeer;
	AAASendRecvMessage_f		AAASendRecvMessage;
	AAASendRecvMessageToPeer_f	AAASendRecvMessageToPeer;
	
	
	AAAAddRequestHandler_f		AAAAddRequestHandler;
	AAAAddResponseHandler_f		AAAAddResponseHandler;


	AAACreateTransaction_f		AAACreateTransaction;
	AAADropTransaction_f		AAADropTransaction;
	
	
	AAACreateSession_f			AAACreateSession;
	AAAMakeSession_f			AAAMakeSession;
	AAAGetSession_f				AAAGetSession;
	AAADropSession_f			AAADropSession;
	AAASessionsLock_f 			AAASessionsLock;
	AAASessionsUnlock_f			AAASessionsUnlock;

	AAACreateClientAuthSession_f AAACreateClientAuthSession;
	AAACreateServerAuthSession_f AAACreateServerAuthSession;
	AAAGetAuthSession_f			AAAGetAuthSession;
	AAADropAuthSession_f		AAADropAuthSession;
	AAATerminateAuthSession_f	AAATerminateAuthSession;

};


#ifdef CDP_FOR_SER

#define NO_SCRIPT	-1


typedef int(*load_cdp_f)( struct cdp_binds *cdpb );
int load_cdp( struct cdp_binds *cdpb);

#endif /* CDP_FOR_SER */

#endif

