/**
 * $Id: policy_control.c 1131 2011-08-08 10:11:55Z aon $
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
 * P-CSCF Policy Control Ops
 *
 *
 */

#include "policy_control.h"

#include "../tm/tm_load.h"
#include "mod.h"

#include "pcc.h"
#include "sip.h"
#include "dlg_state.h"

extern struct tm_binds tmb;            /**< Structure with pointers to tm funcs 		*/
extern struct cdp_binds cdpb;          /**< Structure with pointers to cdp funcs 		*/

extern int pcscf_use_pcc;
extern int pcscf_qos_release7;


int P_local_policy(struct sip_msg* msg, char* str1, char* str2) 
{
	if (!pcscf_use_pcc) return CSCF_RETURN_FALSE;
	LOG(L_INFO, ANSI_WHITE"INF:"M_NAME":P_local_policy: SDP local policy control\n");
	
	return CSCF_RETURN_TRUE;	
}


/**
 * Checks if this reply is one that is suitable of generating an AAR message...
 * @param msg - SIP response to check
 * @param str1 - Orig or Term , 0 or 1
 * @param str2 - not used
 * 
 * @returns 0 if this reply is not AAR suitable, or 1 if yes
*/
int P_generates_aar(struct sip_msg *msg,char *str1,char *str2)
{
	struct cell *t;
	if (!pcscf_use_pcc) return CSCF_RETURN_FALSE;
	
	LOG(L_INFO,"P_generates_aar(): checking if necessary for qos on side %s\n",str1);
	t=tmb.t_gett();
	if (!t) {
		LOG(L_ERR,"P_generates_aar(): unable to get transaction\n");
		return 0;
	}
	
	if ((t->method.len==5 && memcmp(t->method.s,"PRACK",5)==0)||(t->method.len==6 && (memcmp(t->method.s,"INVITE",6)==0||memcmp(t->method.s,"UPDATE",6)==0)))
	{
		if (cscf_get_content_len(msg)!=0 && cscf_get_content_len(t->uas.request)!=0)
		{
			// Need to check SDP validity
			return CSCF_RETURN_TRUE;
		} 
	} 
	return CSCF_RETURN_FALSE;
}

/*
 * sends an AAR for every registered contact
 * returns CSCF_RETURN_TRUE on ok and CSCF_RETURN_FALSE on error
 * TODO:on error: if any AAR was sent, STR will be sent by the authstatemachine
 */
int P_AAR_register(struct sip_msg *req, struct sip_msg * rpl){

	AAAMessage* resp;
	unsigned int result = AAA_SUCCESS;
	contact_body_t * aor_list;
	contact_t * crt_aor = NULL;
	str pcc_session_id = {0,0};
		
	if (parse_headers(rpl, HDR_EOH_F, 0) <0) {
		LOG(L_ERR,"ERR:"M_NAME":P_Rx_register: error parsing headers\n");
		return CSCF_RETURN_FALSE;
	}	
		
	aor_list = cscf_parse_contacts(req);
	if (!aor_list || (!aor_list->contacts && !aor_list->star)) {
		LOG(L_ERR,"ERR:"M_NAME":P_Rx_register: no contacts found in the Contact header\n");
		return CSCF_RETURN_FALSE;
	}

	for(crt_aor = aor_list->contacts; crt_aor!=NULL; crt_aor= crt_aor->next){
		resp = PCC_AAR(req, rpl, "reg", crt_aor, &pcc_session_id, 0);
		if(!resp)
			goto error;
		if(PCC_AAA(resp, &result, pcc_session_id) > 0){
			LOG(L_INFO,"INFO:"M_NAME":P_Rx_register:recieved an AAA with result code %u\n",result);
			if(result<2000 || result >= 3000){
				cdpb.AAAFreeMessage(&resp); 
				goto error;
			}

		}
		cdpb.AAAFreeMessage(&resp); 
	}
	
	return CSCF_RETURN_TRUE;

error:
	LOG(L_INFO,"INFO:"M_NAME":P_Rx_register:recieved an AAA with error code or null AAA\n");
	return CSCF_RETURN_FALSE;
}


/**
 * P_AAR() will be called, if a SIP 183 Session Progress comes back from the callee.  
 * It retrieves relevant SIP headers from SDP offer and answer from INVITE and 183
 * respectively. It converts these SIP headers to corresponding AVPs and creates
 * a AAR and sends it to the PDF and waits for the answer returned by the PDF.
 * 
 * P_AAR() is also called upon registration in that case parameter str1 should
 * start with R or r so that the functions do appropriate
 * not to be used for deregistrations
 * @param msg - The SIP response  
 * @param str1 - orig/term/register
 * @param str2 - not used 
 * @returns 1 on Diameter success or 0 on failure   
 */
int P_AAR(struct sip_msg* msg, char* str1, char* str2)
{	
	struct cell *t;
	int preliminary=0;
	AAAMessage* resp;
	unsigned int result = AAA_SUCCESS;
	if (!pcscf_use_pcc) return CSCF_RETURN_TRUE;
	str pcc_session_id = {0,0};
	
	LOG(L_INFO, ANSI_WHITE"INF:"M_NAME":P_Rx: CALLED\n");
	if (msg->first_line.type == SIP_REQUEST) {
		LOG(L_ERR, ANSI_WHITE"ERR:"M_NAME": P_Rx: called upon request, preliminary information \n");
		preliminary=1;
	}

	/* Get the SIP request from this transaction */
	/* Create an AAR based on request and reply and send it to PDF */
	if (!preliminary) {
		t=tmb.t_gett();
		if (!t) {
			LOG(L_ERR, ANSI_WHITE"ERR:"M_NAME": P_Rx: cannot get the transaction\n");
			return CSCF_RETURN_FALSE;
		}
		LOG(L_DBG,"DBG:"M_NAME":P_Rx: t->method is %.*s\n",t->method.len,t->method.s);
		if ((strncmp(t->method.s,"INVITE",6)==0)||(strncmp(t->method.s,"UPDATE",6)==0)||(strncmp(t->method.s,"PRACK",5)==0))
		{
			//we dont apply QoS if its not a reply to an INVITE! or UPDATE or PRACK!
			resp = PCC_AAR(t->uas.request, msg, str1, NULL, &pcc_session_id, 1);

		}else if ((strncmp(t->method.s,"REGISTER",8)==0))
		{
			return P_AAR_register(t->uas.request, msg);
		} else {
			LOG(L_DBG,"DBG:"M_NAME":P_Rx: Policy and Charging Control non-applicable\n");
			return CSCF_RETURN_TRUE;
		}
	} else {
		//preliminary
		if ((strncmp(msg->first_line.u.request.method.s,"INVITE",6)==0))
			resp = PCC_AAR(msg,0,str1, NULL,&pcc_session_id, 0);
	}


	//cdpb.AAAPrintMessage(resp);
	if (!resp) goto error;
	if(PCC_AAA(resp, &result, pcc_session_id)>0){
		LOG(L_INFO,"INFO:"M_NAME":P_Rx:recieved an AAA with result code %u\n",result);
	}

	cdpb.AAAFreeMessage(&resp); // if frequency
	//LOG(L_INFO, ANSI_WHITE"INF: rc %d\n", result);
	if (result >= 2000 && result < 3000) {
		return CSCF_RETURN_TRUE;
	} else {
		 return CSCF_RETURN_FALSE; // if its not a success then that means i want to reject this call!
	} 
	/*
	 * TODO:
	 * if its a reINVITE  then the rules already exist, the PCRF may
	 * not install the news but remain with the old ones and in that case it will send a DIAMETER_UNABLE_TO_COMPLY
	 * so we need to catch that possibility here maybe looking at the Error-Message
	 */
	
	
error:
	return CSCF_RETURN_TRUE; // default policy is if PDF/PCRF not working or errors , then leave everything flow
}



/**
 * P_STR() will be called, if PCSCF receives BYE, CANCEL, 3xx, 4xx, 5xx or 6xx.
 * It terminates the created auth session with the PDF.
 *  
 */
int P_STR(struct sip_msg* msg, char* str1, char* str2)
{
	// get session id from sip msg   
	// Gq_STR(session_id) terminate the session
	AAAMessage* sta=0;
	contact_t * crt_cnt = NULL;
	contact_body_t * cnt_list;

	if (!pcscf_use_pcc) return CSCF_RETURN_TRUE;
	
	LOG(L_INFO, ANSI_WHITE"INF:"M_NAME":P_STR:\n");

	int is_register=(str1 && (str1[0]=='r' || str1[0]=='R'));
	
	if(is_register){
		if (parse_headers(msg, HDR_EOH_F, 0) <0) {
			LOG(L_ERR,"ERR:"M_NAME":P_STR: error parsing headers\n");
			return CSCF_RETURN_FALSE;
		}	
		
		cnt_list = cscf_parse_contacts(msg);
		if (!cnt_list || (!cnt_list->contacts && !cnt_list->star)) {
			LOG(L_ERR,"ERR:"M_NAME":P_STR: no contacts found in the Contact header\n");
			return CSCF_RETURN_FALSE;
		}

		for(crt_cnt = cnt_list->contacts; crt_cnt!=NULL; crt_cnt= crt_cnt->next){
			sta = PCC_STR(msg,str1, crt_cnt);
			// if you really want the STA just declare a ResponseHandler for it because its never going
			// to arrive here.. or never again
			if (sta) cdpb.AAAFreeMessage(&sta);
		}
	}else{
		sta = PCC_STR(msg,str1, NULL);
	}

	return CSCF_RETURN_TRUE;
}
