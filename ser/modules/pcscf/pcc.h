/**
 * $Id: pcc.h 1135 2011-08-10 10:27:42Z aon $
 *   
 * Copyright (C) 2004-2007 FhG Fokus
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
 * P-CSCF Policy and Charging Control interface ops
 *  
 * \author Alberto Diez Albaladejo -at- fokus dot fraunhofer dot de
 * \author Dragos dot Vingarzan -at- fokus dot fraunhofer dot de
 */
 
#ifndef __PCC_H_
#define __PCC_H_

#include "mod.h"
#include "../cdp/cdp_load.h"

#include "dlg_state.h"
#include "sip.h"




typedef struct pcc_authdata {
	str callid;
	str sip_uri;
	str host;
	str icid;
	str icsi;
	int port,transport;
	enum p_dialog_direction direction; // 0 ORIGINATING  1 TERMINATING

	//for registration session
	int subscribed_to_signaling_path_status;
	//for Gqprima only
	int latch;
} pcc_authdata_t;


int create_gg_socket();
void close_gg_socket();
int cscf_get_mobile_side(struct sip_msg *msg, int is_shm);
void terminate_pcc_session(str session_id, int is_register);
void pcc_auth_clean_dlg_safe(p_dialog *dlg);


AAAMessage* PCC_AAR(struct sip_msg *req, struct sip_msg *res, char *str1, contact_t *aor, str * pcc_session_id, int is_shm);
AAAMessage* PCC_STR(struct sip_msg *msg, char *str1, contact_t * aor);
AAAMessage* PCC_ASA(AAAMessage *request);
int PCC_AAA(AAAMessage *msg, unsigned int * rc, str pcc_session_id);
int PCC_STA(AAAMessage *aaa, unsigned int *rc);


AAAMessage* PCCRequestHandler(AAAMessage *request,void *param);



#endif /*__PCC_H_*/
