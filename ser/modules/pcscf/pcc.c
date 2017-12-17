/**
 * $Id: pcc.c 1184 2013-10-23 10:05:17Z aon $
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

#include "pcc.h"

#include "pcc_avp.h"
#include "dlg_state.h"
#include "release_call.h" // for the ASR-ASA
#include "../tm/tm_load.h"
#include "pcc_gqprima.h"
#include "registrar_storage.h"
#include "registrar.h"
#include "sip_body.h"
#include "sip.h"
#include "../Client_Rf/client_rf_load.h"

/**< Structure with pointers to tm funcs */
extern struct tm_binds tmb;

/**< Structure with pointers to cdp and cdp_avp funcs */
extern struct cdp_binds cdpb;
extern cdp_avp_bind_t *cdp_avp;
extern int pcscf_use_client_rf;
extern struct client_rf_binds client_rfb;

/**< FQDN of PDF, defined in mod.c */
extern str forced_qos_peer; /*its the PCRF in this case*/

/* the destination realm*/
extern str pcc_dest_realm;

extern char * gg_af_ip;
extern uint32_t gg_af_port;
int gg_af_socket;
ip_address gg_af_addr;

extern int pcscf_qos_release7;
extern int pcscf_qos_side;
extern str pcscf_record_route_mo_uri;
extern str pcscf_record_route_mt_uri;
extern int pcc_use_icid;				/**< weather to send the IMS charging id on the Rx interface >**/
extern int pcscf_use_client_rf;
extern str pcc_serv_id_register;			/**< the Service Identifier to be used when sending an AAR to PCRF for registrations>**/
extern str pcc_serv_id_call;				/**< the Service Identifier to be used when sending an AAR to PCRF for calls >**/
str reason_terminate_dialog_s={"Session terminated ordered by the PCRF",38};



/**
 * Frees memory taken by a pcc_authdata_t structure
 * @param x - the pcc_authdata_t to be deallocated
 */
void free_pcc_authdata(pcc_authdata_t *x)
{
	if (!x) return;
	if (x->callid.s) shm_free(x->callid.s);
	if (x->sip_uri.s)	 shm_free(x->sip_uri.s);
	if (x->host.s) shm_free(x->host.s);
	if (x->icid.s)		shm_free(x->icid.s);
	if (x->icsi.s)		shm_free(x->icsi.s);
	shm_free(x);
}

/**
 * Creates a new pcc_authdata_t structure and initializes it.
 * @returns - the pcc_authdata_t created, NULL on error
 */
pcc_authdata_t* new_pcc_authdata()
{
	pcc_authdata_t *x;
	
	x = shm_malloc(sizeof(pcc_authdata_t));
	if (!x){
		LOG(L_ERR,"ERR:"M_NAME":pcc_authdata_t(): Unable to alloc %u bytes\n",
			sizeof(pcc_authdata_t));
		goto error;
	}	
	bzero(x,sizeof(pcc_authdata_t));
		
	return x;
error:
	free_pcc_authdata(x);
	return 0;	
}

/*Return the destination realm*/
inline str pcc_get_destination_realm(){
	return pcc_dest_realm;
}

/*
 * Auxiliary function that gives if the first route header is mt or mo
 * if reply is -1 you should use what the config file gave you
 * @param msg - SIP message to check, if reply will get response from it
 * @returns -1 on error, 0 on mo , 1 mt
*/
int cscf_get_mobile_side(struct sip_msg *msg, int is_shm)
{

	str first_route={0,0};

	if (!msg) return -1;

	if ( msg->first_line.type==SIP_REPLY ) {
		msg= cscf_get_request_from_reply(msg);
		first_route=cscf_get_first_route(msg,0,1);
	}else {
		if(is_shm){ 
			first_route=cscf_get_first_route(msg,0,1);
		} else {
			first_route=cscf_get_first_route(msg,0,0);
		}
	}

	// first_route should return 0 if none route header found
	if (first_route.len == 0 || first_route.s == 0 ) {
		LOG(L_DBG,"cscf_get_mobile_side : empty first route, returning unknown\n");	
		return DLG_MOBILE_UNKNOWN;	
	}
	LOG(L_DBG,"cscf_get_mobile_side : first route is %.*s comparing with %.*s and %.*s \n",
			first_route.len,first_route.s,
			pcscf_record_route_mt_uri.len,pcscf_record_route_mt_uri.s,
			pcscf_record_route_mo_uri.len,pcscf_record_route_mo_uri.s);

	if (strncmp(first_route.s,pcscf_record_route_mt_uri.s,pcscf_record_route_mt_uri.len)==0)
		return DLG_MOBILE_TERMINATING;
	else  if (strncmp(first_route.s,pcscf_record_route_mo_uri.s,pcscf_record_route_mo_uri.len)==0)
		return DLG_MOBILE_ORIGINATING;
	else {
		//it could be the first INVITE so here i can rely in what the config file gave me
		LOG(L_DBG,"cscf_get_mobile_side : returning unknown\n");	
		return DLG_MOBILE_UNKNOWN;
	}

}

int create_gg_socket(){

	str gg_ip_s;

	gg_ip_s.s = gg_af_ip;
	gg_ip_s.len = strlen(gg_af_ip);

	str_to_ip_address(gg_ip_s, &gg_af_addr);

	if ( (gg_af_socket = socket(gg_af_addr.ai_family, SOCK_DGRAM, IPPROTO_UDP)) < 0){ 	
		LOG(L_ERR, "Error opening udp socket. socket() said %s\n",
				strerror(errno));
		return 0;
	}

	return gg_af_socket;

error:
	LOG(L_ERR, "could not cnvert well the ip %s\n", gg_af_ip);
	return 0;
}

void close_gg_socket(){

	if(gg_af_socket)
		close(gg_af_socket);
}

/*
static str s_orig={";orig",5};
static str s_term={";term",5};
*/
/**
 * Modifies the call id by adding at the end orig or term
 * \note results is in shm, needs to be freed!
 * @param call_id - the original call_id
 * @param tag - 0 for orig, 1 for term
 * @returns the new shm string
 */
/*str pcc_modify_call_id(str call_id, int tag)
{
	str t;
	t.len = call_id.len + (tag?s_orig.len:s_term.len);
	t.s = shm_malloc(t.len);
	if (!t.s) {
		LOG(L_ERR,"ERR:"M_NAME":rx_modify_call_id(): error allocating %d bytes\n",t.len);
		t.len = 0;
		return t;
	}
	t.len=0;
	STR_APPEND(t,call_id);
	if (tag == 0) {
		STR_APPEND(t,s_orig);
	}
	else{ 
		STR_APPEND(t,s_term);
	}
	return t;
}*/



void callback_for_pccsession(int event,void *session)
{
	pcc_authdata_t *pcc_data;
	AAASession *x=session;
	p_dialog *dlg=0;
	r_contact *contact=0;
	
	LOG(L_DBG,"callback_for_pccsession(): called\n");
	
	switch (event)
	{
		case AUTH_EV_SESSION_TIMEOUT:
		case AUTH_EV_SESSION_GRACE_TIMEOUT:
		case AUTH_EV_SERVICE_TERMINATED:
			
			pcc_data=(pcc_authdata_t *)x->u.auth.generic_data;
			if (pcc_data) {
				if (pcc_data->callid.s && pcc_data->host.s)
				{	
					//it's a call
					//dlg=get_p_dialog_dir(g->callid,g->direction);
					dlg=get_p_dialog(pcc_data->callid,pcc_data->host,pcc_data->port,pcc_data->transport,&(pcc_data->direction));
					
					if (dlg) { 
						if (dlg->pcc_session_id.s)
						// if its not set, someone has handled this before us so...
						{
							// the session is going to be deleted
							pcc_auth_clean_dlg_safe(dlg);
							terminate_p_dialog(dlg);
						}
						d_unlock(dlg->hash);
					}
				}
				else if (pcc_data->subscribed_to_signaling_path_status && pcc_data->host.s && pcc_data->port){
					//its a registration that is being released
					LOG(L_WARN,"WARN: received end of session for %.*s:%d\n",pcc_data->host.len,pcc_data->host.s,pcc_data->port);

					contact=get_r_contact(pcc_data->host,pcc_data->port,pcc_data->transport, ANY_REG);
					if (!contact)
					{
						LOG(L_ERR,"ERR:callback_for_pccsession:no contact associated\n");

					} else {
						if (contact->pcc_session_id.s)
						// if its not set, someone has handled this before us so...
						{
							// the session is going to be deleted
							shm_free(contact->pcc_session_id.s);
							contact->pcc_session_id.s=0; 
							contact->pcc_session_id.len=0; 
							//TODO //give this contact a grace time for expiration
						}
						//set expires to 0, so that the registrar timer will clean the r_contact
						contact->expires=time(0)-1; 
						r_unlock(contact->hash);
					}
				}				
				free_pcc_authdata(pcc_data);
				x->u.auth.generic_data=0;
			}
			break;
			
			
		case AUTH_EV_SESSION_DROP:
			// The session is being dropped, so just drop the generic data too and the links
			pcc_data=(pcc_authdata_t *)x->u.auth.generic_data;
			if (pcc_data) {
				if (pcc_data->callid.s && pcc_data->host.s)
				{						
					//dlg=get_p_dialog_dir(g->callid,g->direction);
					dlg=get_p_dialog(pcc_data->callid,pcc_data->host,pcc_data->port,pcc_data->transport,&(pcc_data->direction));					
					if (dlg) { 
						if (dlg->pcc_session_id.s) {
							// silently unlink
							shm_free(dlg->pcc_session_id.s);
							dlg->pcc_session_id.s=0; 
							dlg->pcc_session_id.len=0; 							
						}
						d_unlock(dlg->hash);
					}
				}
				else if (pcc_data->subscribed_to_signaling_path_status && pcc_data->host.s && pcc_data->port)
				{
					//its a registration that is being released
					LOG(L_WARN,"WARN: received end of session for %.*s:%d\n",pcc_data->host.len,pcc_data->host.s,pcc_data->port);

					contact=get_r_contact(pcc_data->host,pcc_data->port,pcc_data->transport, ANY_REG);
					if (!contact) {
						LOG(L_ERR,"ERR:callback_for_pccsession:no contact associated\n");
					} else {
						if (contact->pcc_session_id.s) {
							// silently unlink
							shm_free(contact->pcc_session_id.s);
							contact->pcc_session_id.s=0; 
							contact->pcc_session_id.len=0; 							
						}
						contact->expires=time(0)-1; 
						r_unlock(contact->hash);
					}
				}				
				free_pcc_authdata(pcc_data);
				x->u.auth.generic_data=0;
			}
			break;
		default:
			break;
	}
}

void pcc_auth_clean_reg_safe(r_contact* contact){

	if (contact->pcc_session_id.s) {
		shm_free(contact->pcc_session_id.s);
		contact->pcc_session_id.s = 0;
		contact->pcc_session_id.len = 0;
	}
}

r_contact* pcc_auth_clean_reg(str host, unsigned int port, unsigned int proto){
	
	r_contact * contact = get_r_contact(host, port, proto, ANY_REG);

	if (!contact){
		LOG(LOG_CRIT,"BUG:"M_NAME":pcc_auth_clean_reg: no contact "\
				"in the registrar found for %.*s:%u\n", host.len, host.s, port);
		return NULL;
	}
	pcc_auth_clean_reg_safe(contact);
	return contact;
}

/*
 * clean the pcc_session_id string from a contact/contact list
 * @param contacts - the contacts to be processed
 * @param aor  - the aor of which contact_t should be cleaned
 * @param from_registrar - 0 if the contacts are from the msg (usually Contact header), considered an AOR
 * 			 - 1 from registrar
 */
AAASession * pcc_auth_clean_register(r_contact * cnt, contact_t* aor, int from_registrar){

	struct sip_uri parsed_cnt;
	AAASession* auth = NULL;

	if(from_registrar){
		if (cnt->pcc_session_id.s) {
			auth = cdpb.AAAGetAuthSession(cnt->pcc_session_id);
			shm_free(cnt->pcc_session_id.s);
			cnt->pcc_session_id.s = 0;
			cnt->pcc_session_id.len = 0;
		}
		r_unlock(cnt->hash);
		return auth;
	}else{
		str crt_uri = aor->uri;
		if(parse_uri(crt_uri.s, crt_uri.len, &parsed_cnt) != E_OK){
			LOG(L_ERR,"ERR:"M_NAME":pcc_auth_clean_register: error parsing uri of the contact\n");
			return NULL;		
		}
	
		r_contact * contact = get_r_contact(parsed_cnt.host, parsed_cnt.port_no, 
				parsed_cnt.proto, ANY_REG);

		if (!contact){
			LOG(LOG_CRIT,"BUG:"M_NAME":pcc_auth_clean_register: contact not found in the registrar\n");
			return NULL;
		}
		return pcc_auth_clean_register(contact, NULL, 1);
	}
}


/* 
 * initiates a subscription to the signalling path status of an aor, in a case of an registration
 * @param aor: the aor to be handled
 * @return: 0 - ok, 1 - goto end, -1 - error, -2 - out of memory
 */
int pcc_auth_init_register(contact_t * aor, str callid,
		unsigned int * expireReg, AAASession ** authp, struct sip_uri * parsed_aor){

	// REGISTRATION
	
	uint32_t duration=-1; //forever - this should be overwritten anyway
	pcc_authdata_t *pcc_authdata=0;
	AAASession * auth = *authp;
	str icid = {0,0};
	
	r_contact *contact = NULL;
	int ret = 1;
	str uri = aor->uri;

	if(parse_uri(uri.s, uri.len, parsed_aor) != E_OK){
		LOG(L_ERR,"ERR:"M_NAME":pcc_auth_init_register: error parsing uri of the contact\n");
		goto end;
	}
	
	contact = get_r_contact(parsed_aor->host, parsed_aor->port_no, 
			parsed_aor->proto, ANY_REG);
	if(!contact){
		LOG(L_ERR,"ERR:"M_NAME":pcc_auth_init_register: not sending subscription to path status, for unknown contact\n");
		goto end;
	}

	if (contact->pcc_session_id.len) {
		LOG(L_DBG,"DBG:"M_NAME":pcc_auth_init_register: re-registration, retrieving the AAAsession \n");
		auth=cdpb.AAAGetAuthSession(contact->pcc_session_id);
		if(!auth){
			LOG(L_ERR,"ERR:"M_NAME":pcc_auth_init_register: no AAAsession found\n");
			ret = -1;
			goto end;
		}
		pcc_authdata = (pcc_authdata_t*)auth->u.auth.generic_data;
		if(pcc_authdata &&
				(pcc_authdata->callid.len != callid.len ||strncmp(pcc_authdata->callid.s, callid.s, callid.len))){

			shm_free(pcc_authdata->callid.s);
			STR_SHM_DUP(pcc_authdata->callid, callid,"pcc_auth_init_register");
		}
		*expireReg = contact->expires-time(0);
	} else {
		LOG(L_DBG,"DBG:"M_NAME":pcc_auth_init_register: new registration sending AAR to PCRF for AF signaling path status subscription\n");
		//its a registration so create a new cdp session
		pcc_authdata = new_pcc_authdata();
		if(!pcc_authdata) {
			LOG(L_ERR,"ERR:"M_NAME":pcc_auth_init_register: no memory left for generic\n");
			goto out_of_memory;
		}				
		pcc_authdata->subscribed_to_signaling_path_status=1;
		STR_SHM_DUP(pcc_authdata->host, contact->host,"pcc_auth_init_register");
		STR_SHM_DUP(pcc_authdata->callid, callid,"pcc_auth_init_register");
		if(pcscf_use_client_rf && pcc_use_icid){
			if(client_rfb.Rf_get_ims_chg_info_icid(callid, DLG_MOBILE_ORIGINATING, &icid) && icid.s){

				STR_SHM_DUP(pcc_authdata->icid, icid, "pcc_auth_init_register");
				pkg_free(icid.s);
			}
		}
		pcc_authdata->port=contact->port;
		pcc_authdata->transport=contact->transport;

		LOG(L_INFO,"INFO:"M_NAME":pcc_auth_init_register: creating PCC Session for registration\n");
		auth = cdpb.AAACreateClientAuthSession(1,callback_for_pccsession,(void *)pcc_authdata);
		if (!auth) {
			LOG(L_ERR,"INFO:"M_NAME":pcc_auth_init_register: unable to create the PCC Session\n");
			free_pcc_authdata(pcc_authdata);
			pcc_authdata = 0;
			ret = -1;
			goto end;
		}
		
		STR_SHM_DUP(contact->pcc_session_id,auth->id,"pcc_auth_init_register") ;

		if (contact->reg_state==REGISTERED) duration = contact->expires;
		else duration = time(0);
		auth->u.auth.lifetime = duration;
		auth->u.auth.grace_period = REGISTRATION_GRACE_PERIOD;
		if (auth->u.auth.timeout<auth->u.auth.lifetime) auth->u.auth.timeout = duration;
		*expireReg = 0;
				
	}
	*authp = auth;
	ret = 0;
end:
	if(contact) r_unlock(contact->hash);
	return ret;
out_of_memory:
	if(contact) r_unlock(contact->hash);
	return -2;
}

void pcc_auth_clean_dlg_safe(p_dialog * dlg){

	if(!dlg)
		return;

	if (dlg->pcc_session_id.s) {
		shm_free(dlg->pcc_session_id.s);
		dlg->pcc_session_id.s = 0;
		dlg->pcc_session_id.len = 0;
	}
}

/* 
 * initiates/gets the dialog pcc session for a session establishment
 * @param aor: the aor to be handled
 * @return: 0 - ok, -1 - error, -2 - out of memory
 */
int pcc_auth_init_dlg(p_dialog **dlgp, AAASession ** authp, int * relatch, int pcc_side, 
		unsigned int * auth_lifetime, str * service_urn){

	pcc_authdata_t *pcc_authdata=0;
	p_dialog * dlg = *dlgp;
	AAASession * auth = NULL;
	
	if (!dlg->pcc_session_id.len) {
		/*For the first time*/	 
		pcc_authdata = new_pcc_authdata();
		if(!pcc_authdata) {
			LOG(L_ERR,"ERR:"M_NAME":pcc_auth_init_dlg: no memory left for generic\n");
			goto out_of_memory;
		}	
		pcc_authdata->callid.s=0; pcc_authdata->callid.len=0;
		STR_SHM_DUP(pcc_authdata->callid, dlg->call_id,"pcc_auth_init_dlg");
		pcc_authdata->direction=pcc_side;
		pcc_authdata->host.s=0; pcc_authdata->host.len=0;
		STR_SHM_DUP(pcc_authdata->host,dlg->host,"pcc_auth_init_dlg");
		pcc_authdata->port=dlg->port;
		pcc_authdata->transport=dlg->transport;

		if (pcscf_qos_release7==-1){
			 //LATCH default set to 1 , could be a config issue
			 pcc_authdata->latch = 1;
		}
		LOG(L_INFO,"INFO:"M_NAME":pcc_auth_init_dlg: creating PCC Session\n");
		auth = cdpb.AAACreateClientAuthSession(1,callback_for_pccsession,(void *)pcc_authdata);
		if (!auth) {
			LOG(L_ERR,"INFO:"M_NAME":pcc_auth_init_dlg: unable to create the PCC Session\n");
			free_pcc_authdata(pcc_authdata);
			pcc_authdata = 0;
			goto error;
		}		 
		STR_SHM_DUP(dlg->pcc_session_id,auth->id,"pcc_auth_init_dlg") ;
		auth->u.auth.lifetime = dlg->expires;
		STR_SHM_DUP(pcc_authdata->icid,dlg->icid,"pcc_auth_init_dlg");
		
	} else {
		auth = cdpb.AAAGetAuthSession(dlg->pcc_session_id);
		if (!auth){
			LOG(L_INFO,"INFO:"M_NAME":pcc_auth_init_dlg: pcc session in dialog %.*s %i with id %.*s was not found in cdp\n",
					dlg->call_id.len, dlg->call_id.s,pcc_side,
					dlg->pcc_session_id.len,dlg->pcc_session_id.s);
		}else{
			LOG(L_INFO,"INFO:"M_NAME":pcc_auth_init_dlg:found a pcc session in dialog %.*s %i\n", dlg->call_id.len,dlg->call_id.s,pcc_side);
			if (pcscf_qos_release7==-1)
			{
				pcc_authdata = (pcc_authdata_t *)auth->u.auth.generic_data;
				*relatch = pcc_authdata->latch;
			}
		}
		*auth_lifetime = dlg->expires-time(0);
	}
	
	if(dlg->em_info.em_dialog){
		//the stored urn includes "urn:", 
		//the PCRF expects the service urn without it
		LOG(L_INFO,"INFO:"M_NAME":pcc_auth_init_dlg: emergency dialog towards %.*s\n",
					dlg->em_info.service_urn.len,
					dlg->em_info.service_urn.s);
		service_urn->s = dlg->em_info.service_urn.s+4;
		service_urn->len = dlg->em_info.service_urn.len-4;
	}

	d_unlock(dlg->hash);
	*dlgp=0;
	*authp = auth;
	return 0;

out_of_memory:
	d_unlock(dlg->hash);
	*dlgp=NULL;
	return -2;
error:
	d_unlock(dlg->hash);
	*dlgp=NULL;
	return -1;
}

//AF Service is IMS Services although it should be the IMS Communication Services
static str IMS_Em_Serv_AVP_val = {"Emergency IMS Call", 18};

/* Session-Id, Origin-Host, Origin-Realm AVP are added by the stack. */
int pcc_rx_req_add_mandat_avps(AAAMessage* msg,
		unsigned int dia_req_code, struct sip_msg * res,
		int registr, int sos,
		str * icid, str * ims_comm_service_id){

	char x[4];
	AAA_AVP * avp;
	str af_id = {0,0};

	/* Add Auth-Application-Id AVP */
	if (pcscf_qos_release7==1){
		if (!PCC_add_auth_application_id(msg, IMS_Rx)) goto error;
		if (!PCC_add_vendor_specific_application_id_group(msg, IMS_vendor_id_3GPP, 
				IMS_Rx)) goto error;
	}else{
		if (!PCC_add_auth_application_id(msg, IMS_Gq)) goto error;
		if (!PCC_add_vendor_specific_application_id_group(msg, IMS_vendor_id_ETSI, 
				IMS_Gq)) goto error;
	} 

	/* Add Destination-Realm AVP, if not already there */
	avp = cdpb.AAAFindMatchingAVP(msg, msg->avpList.head, AVP_Destination_Realm,
					0, AAA_FORWARD_SEARCH);
	if(!avp){
		str realm = pcc_get_destination_realm();
		if (realm.len && !PCC_add_destination_realm(msg, realm)) goto error;
	}
	
	/* Add AF-Application-Identifier AVP */
	if(ims_comm_service_id && ims_comm_service_id->len && ims_comm_service_id->s){
		af_id = *(ims_comm_service_id);

	}else {
		if(!registr){

			if(sos)
				af_id = IMS_Em_Serv_AVP_val;
			else af_id = pcc_serv_id_call;

		}else	af_id = pcc_serv_id_register;
		if(ims_comm_service_id)	*(ims_comm_service_id) = af_id;
	}

	if(!PCC_add_avp(msg, af_id.s,af_id.len,
			AVP_IMS_AF_Application_Identifier, 
			AAA_AVP_FLAG_MANDATORY, IMS_vendor_id_3GPP,
			AVP_DUPLICATE_DATA, __FUNCTION__)) goto error;

	/* Add Service-Info-Status AVP, if prelimiary
	 * by default(when absent): final status is considered*/
	if (dia_req_code == IMS_AAR && !res){
		set_4bytes(x,AVP_EPC_Service_Info_Status_Preliminary_Service_Information);
		if(!PCC_add_avp(msg, x, 4, AVP_IMS_Service_Info_Status, 
					AAA_AVP_FLAG_MANDATORY, IMS_vendor_id_3GPP,
					AVP_DUPLICATE_DATA,__FUNCTION__)) goto error;
	}
	/*AF-Charging-Identifier*/
	if(icid && icid->s && icid->len)
		if(!cdp_avp->epcapp.add_AF_Charging_Identifier(&msg->avpList,*icid,0)) goto error;

	return 1;
error:
	return 0;
}

/**
 * Sends the Authorization Authentication Request.
 * @param req - SIP request  
 * @param res - SIP response
 * @param str1 - 0/o/orig for originating side, 1/t/term for terminating side, r/REGISTER for registration
 * @param pcc_session_id - the returned AAAsession id
 * @param is_shm - req is from shared memory 
 * 
 * @returns AAA message or NULL on error  
 */
AAAMessage *PCC_AAR(struct sip_msg *req, struct sip_msg *res, char *str1, contact_t* aor, 
		str* pcc_session_id, int is_shm)
{
	AAAMessage* aar = NULL;
	AAAMessage* aaa = 0;
	AAASession* auth = 0;
	AAA_AVP* avp=0;

	contact_body_t* b=0;	
	p_dialog *dlg=0;
	str host={0,0};
	int port=0,transport=0;
	str sdpbodyinvite={0,0},sdpbody200={0,0};
	char *mline=0;
	char x[4];
	int i, relatch=0;
	str call_id={0,0};
	int pcc_side =cscf_get_mobile_side(req, is_shm);
	enum p_dialog_direction dir = 0;
	unsigned int auth_lifetime = 0;
	struct sip_uri parsed_aor;
	str service_urn = {0,0};
	int subscr_type;
	str subscr_value={0,0};
	str icid = {0,0};
	str ims_comm_service_id = {0,0};
	pcc_authdata_t* pcc_auth = 0;

	int is_register=(str1 && (str1[0]=='r' || str1[0]=='R'));
	
	call_id = cscf_get_call_id(req, 0);
	LOG(L_INFO,"INF:"M_NAME":PCC_AAR: getting dialog with %.*s %.*s %i %i\n",call_id.len,call_id.s,host.len,host.s,port,transport);

	if (is_register){
		//REGISTRATION
		int ret = pcc_auth_init_register(aor, call_id, &auth_lifetime, &auth, &parsed_aor);
		switch(ret){
			case 0: break;
			case 1: goto end;
			case -1: goto error;
			case -2: goto out_of_memory;	 
		}
		
	} else {	
		// CALL
		dir = get_dialog_direction(str1);
		if (pcc_side== DLG_MOBILE_UNKNOWN) pcc_side=(int) dir;
		if (pcc_side == DLG_MOBILE_TERMINATING) 
			LOG(L_DBG, "INF:"M_NAME":PCC_AAR: terminating side\n");
		else if(pcc_side == DLG_MOBILE_ORIGINATING) 
			LOG(L_DBG, "INF:"M_NAME":PCC_AAR: originating side\n");
		else{
			LOG(L_ERR, "INF:"M_NAME":PCC_AAR: unknown side, end of story\n");
			goto error;
		}
	
		/* Check for the existence of an auth session for this dialog */
		/* if not, create an authorization session */
		/* first check on the response */
		find_dialog_contact(res,dir,&host,&port,&transport);
		dlg = get_p_dialog(call_id,host,port,transport,0);	
	
		if (!dlg) {
			/* then fallback on the request */
			find_dialog_contact(req,dir,&host,&port,&transport);
			dlg = get_p_dialog(call_id,host,port,transport,0);
			LOG(L_INFO,"INF:"M_NAME":PCC_AAR: getting dialog with %.*s %.*s %i %i\n",call_id.len,call_id.s,host.len,host.s,port,transport);
		}
		if (!dlg) goto error;
		ims_comm_service_id = cscf_get_icsi(req, res);
		int ret = pcc_auth_init_dlg(&dlg, &auth, &relatch, pcc_side, &auth_lifetime, &service_urn);
		switch(ret){
			case 0: break;
			case -1: goto error;
			case -2: goto out_of_memory;	
		}
	}
	pcc_auth = (pcc_authdata_t *)auth->u.auth.generic_data;
	
	/* Create an AAR prototype */
	if (pcscf_qos_release7==1)
		aar = cdpb.AAACreateRequest(IMS_Rx, IMS_AAR, Flag_Proxyable, auth);
	else
		aar = cdpb.AAACreateRequest(IMS_Gq, IMS_AAR, Flag_Proxyable, auth);
	
	if (!aar) goto error;
	if(pcc_session_id){
		pcc_session_id->s = auth->id.s;
		pcc_session_id->len = auth->id.len;
	}
	
	/*---------- 1. Add mandatory AVPs ----------*/
	icid = ((pcc_authdata_t*)auth->u.auth.generic_data)->icid;
	if(!pcc_rx_req_add_mandat_avps(aar, IMS_AAR, res, is_register, service_urn.len>0,
			&icid, &ims_comm_service_id))
		goto error;
	if(ims_comm_service_id.len && ims_comm_service_id.s){
		STR_SHM_DUP(pcc_auth->icsi, ims_comm_service_id, "PCC_AAR");
	}

	LOG(L_INFO,"INF:"M_NAME":PCC_AAR: auth_lifetime %u\n", auth_lifetime);
	//auth_lifetime: add an Authorization_Lifetime AVP to update  the lifetime of the cdp session as well
	if(auth_lifetime){
		set_4bytes(x,auth_lifetime);
		avp = cdpb.AAACreateAVP(AVP_Authorization_Lifetime,AAA_AVP_FLAG_MANDATORY,0,x,4,AVP_DUPLICATE_DATA);
		if (avp) cdpb.AAAAddAVPToMessage(aar,avp,aar->avpList.tail);
	}	
	
	/*---------- 2. Create and add Media-Component-Description AVP ----------*/
	
	/*	
	 *  See 3GPP TS29214 V7.1.0:
	 * 
	 *  <Media-Component-Description> = {Media-Component-Number}
	 * 								 	[Media-Sub-Component]
	 * 								 	[AF-Application-Identifier]
	 * 								 	[Media-Type]
	 * 								 	[Max-Requested-Bandwidth-UL]
	 * 									[Max-Requested-Bandwidth-DL]
	 * 									[Flow-Status]
	 * 									[Reservation-Priority] (Not used yet)
	 * 								 	[RS-Bandwidth]
	 * 									[RR-Bandwidth]
	 * 									*[Codec-Data]
	 */

	if (is_register){
		// Registration
		PCC_AAR_add_avps_for_register(aar, &parsed_aor);

	} else {
		// Call
		if(extract_sdp_body(req,&sdpbodyinvite)==-1) {
			LOG(L_ERR,"ERROR:"M_NAME":PCC_AAR: No SDP Body to extract from INVITE\n");
			goto error;
		}
		if(res && extract_sdp_body(res,&sdpbody200)==-1) {
			LOG(L_ERR,"ERROR:"M_NAME":PCC_AAR: No SDP Body to extract from 200 reply\n");
			goto error;
		}
		/*Create and add 1 media-component-description AVP for each
		 * m= line in the SDP body 
		 */
		mline=find_sdp_line(sdpbodyinvite.s,(sdpbodyinvite.s+sdpbodyinvite.len),'m');
		for(i=1;mline!=NULL;i++){
			
			if (!PCC_add_media_component_description(aar,sdpbodyinvite,sdpbody200,mline,i,pcc_side))
			{
				LOG(L_ERR,"ERROR:"M_NAME":PCC_AAR: unable to add media component description AVP for line %i\n",i);
				goto error; /* Think about this*/
			}
			
			mline=find_next_sdp_line(mline,(sdpbodyinvite.s+sdpbodyinvite.len),'m',NULL);
		}

		LOG(L_DBG,"DEBUG:"M_NAME":PCC_AAR: host ip is  %.*s\n",pcc_auth->host.len, pcc_auth->host.s);

		if(!PCC_add_framed_ip_avp(&aar->avpList, pcc_auth->host, AF_INET) &&
			!PCC_add_framed_ip_avp(&aar->avpList, pcc_auth->host, AF_INET6) ) goto error;
		
		//add Service URN for emergency sessions
		if(service_urn.s && service_urn.len){
				avp = cdpb.AAACreateAVP(AVP_IMS_Service_URN, 
						AAA_AVP_FLAG_VENDOR_SPECIFIC,
						IMS_vendor_id_3GPP,
						service_urn.s,
						service_urn.len,
						AVP_DUPLICATE_DATA);
				cdpb.AAAAddAVPToMessage(aar,avp,aar->avpList.tail);
		}

	}
	
	
	
	if (pcscf_qos_release7!=-1)
	{
		if(PCC_add_subscription_ID(aar,req,pcc_side, &subscr_type, &subscr_value)){
			STR_SHM_DUP(pcc_auth->sip_uri, subscr_value, "PCC_AAR");
		}
//		set_4bytes(x,AVP_IMS_Specific_Action_Indication_Of_Release_Of_Bearer);
//		cdpb.AAAAddAVPToMessage(aar,
//				cdpb.AAACreateAVP(
//						AVP_IMS_Specific_Action,
//						AAA_AVP_FLAG_VENDOR_SPECIFIC,
//						IMS_vendor_id_3GPP,
//						x,4,
//						AVP_DUPLICATE_DATA),
//						aar->avpList.tail);

		set_4bytes(x,0);
		avp = cdpb.AAACreateAVP(
				AVP_ETSI_Reservation_Priority,
				AAA_AVP_FLAG_VENDOR_SPECIFIC,
				IMS_vendor_id_ETSI,
				x,4,
				AVP_DUPLICATE_DATA);
		cdpb.AAAAddAVPToMessage(aar,avp,aar->avpList.tail);

	} else {
		if(!gqprima_AAR(aar,req,res,str1,&parsed_aor, relatch))
			goto error;
	}

	if (auth) cdpb.AAASessionsUnlock(auth->hash);
	
	LOG(L_INFO,"INFO:"M_NAME":PCC_AAR: sending AAR to PCRF\n");
	/*---------- 3. Send AAR to PCRF ----------*/
	if (forced_qos_peer.len)
		aaa = cdpb.AAASendRecvMessageToPeer(aar,&forced_qos_peer);
	else 
		aaa = cdpb.AAASendRecvMessage(aar);	
	
	LOG(L_INFO,"INFO:"M_NAME":PCC_AAR: received AAA from PCRF\n");
	return aaa;

out_of_memory:
	LOG(L_CRIT,"CRIT:"M_NAME":PCC_AAR:out of memory\n");
error:
	LOG(L_ERR,"ERR:"M_NAME":PCC_AAR: unexpected ERROR\n");
	if (aar) cdpb.AAAFreeMessage(&aar);
	if (auth) {
		if(dlg){
			d_lock(dlg->hash);
			pcc_auth_clean_dlg_safe(dlg);
			d_unlock(dlg->hash);
		}
		if(is_register && b)
			pcc_auth_clean_register(NULL, aor, 0);
		cdpb.AAASessionsUnlock(auth->hash);
		cdpb.AAADropAuthSession(auth);
		auth=0;
	}
end:
	return NULL;
}


/* handle an AAA response to an AAR for resource reservation for a successfull registration or initiated/updated dialog 
 * @param aaa - the diameter reply
 * @return -  1 if result code found and processing ok, -1 if error
 */
int PCC_AAA(AAAMessage *aaa, unsigned int * rc, str pcc_session_id)
{
	AAASession * auth = NULL;
	AAA_AVP *avp=0;
	AAA_AVP_LIST avp_list = {0,0};
	str an_charg_id_str = {0,0};
	uint32_t an_charg_id = 0;
	str sip_uri = {0,0}, callid={0,0};
	uint32_t rating_group=0;
	enum p_dialog_direction direction;
	int ret = 1;

	if (pcscf_qos_release7==-1)
		ret = gqprima_AAA(aaa, rc);
	else
	{
		ret = PCC_get_result_code(aaa, rc);
	}
	if (ret == 0){
		LOG(L_DBG,"DBG:"M_NAME":PCC_AAA: AAA message without result code\n");
		return ret;
	}

	LOG(L_DBG,"DBG:"M_NAME":PCC_AAA: AAR request result code %u\n",*rc);
	if((*rc)>2999){
		LOG(L_DBG,"DBG:"M_NAME":PCC_AAA: AAR request rejected with error code %u\n",*rc);
		avp=cdpb.AAAFindMatchingAVP(aaa,aaa->avpList.head,AVP_IMS_Acceptable_Service_Info,IMS_vendor_id_3GPP,AAA_FORWARD_SEARCH);
		if (avp)
		{
			LOG(L_WARN,"WARN:"M_NAME":PCC_AAA: PCRF provided Acceptable-Service-Info, not handled\n");
		}
	}
	
	auth = cdpb.AAAGetAuthSession(pcc_session_id);
	if(!auth){
		LOG(L_ERR,"BUG:"M_NAME":PCC_AAA: did not find the AAASession associated with the id %.*s\n", 
				pcc_session_id.len, pcc_session_id.s);
		goto error;
	}

	avp = cdpb.AAAFindMatchingAVP(aaa,aaa->avpList.head,AVP_Origin_Host,0,AAA_FORWARD_SEARCH);
	if(!avp || !avp->data.s || !avp->data.len) {
		LOG(L_ERR,"ERR:"M_NAME":PCC_AAA: missing or empty Origin Host AVP\n");
		goto error;
	}

	LOG(L_DBG,"DBG:"M_NAME":PCC_AAA: AAA contains Origin Host %.*s\n",avp->data.len, avp->data.s);	
	if(!auth->dest_host.s){
		STR_SHM_DUP(auth->dest_host, avp->data, "PCC_AAA");
	}

	avp = cdpb.AAAFindMatchingAVP(aaa,aaa->avpList.head,AVP_Origin_Realm,0,AAA_FORWARD_SEARCH);
	if(!avp || !avp->data.s || !avp->data.len) {
		LOG(L_ERR,"ERR:"M_NAME":PCC_AAA: missing or empty Origin Realm AVP\n");
		goto error;
	}

	LOG(L_DBG,"DBG:"M_NAME":PCC_AAA: AAA contains Origin Realm %.*s\n",avp->data.len, avp->data.s);	
	if(!auth->dest_realm.s){
		STR_SHM_DUP(auth->dest_realm, avp->data, "PCC_AAA");
	}

	/*access network charging identifier*/
	avp_list.head = avp_list.tail = NULL;
	if(cdp_avp->epcapp.get_Access_Network_Charging_Identifier(aaa->avpList, &avp_list, 0)){
		cdp_avp->epcapp.get_Access_Network_Charging_Identifier_Value(avp_list, &an_charg_id_str, 0);
		if(pcscf_use_client_rf){
			if(an_charg_id_str.len!=4){
				LOG(L_ERR, "ERR:"M_NAME":PCC_AAA: AN charging ID invalid length %i\n", an_charg_id_str.len*8);
			}else{
				an_charg_id = ntohl(*(uint32_t*)an_charg_id_str.s);
				sip_uri = ((pcc_authdata_t*)auth->u.auth.generic_data)->sip_uri;
				//LOG(L_DBG, "sip uri is %.*s\n", sip_uri.len, sip_uri.s);
				client_rfb.Rf_add_an_chg_info(sip_uri, an_charg_id);
			}
		}
	}

	/*rating group of the service, not in the Rx specification (yet), enhancement needed in some cases*/
	if(cdp_avp->ccapp.get_Rating_Group(aaa->avpList, &rating_group, 0)){
		if(pcscf_use_client_rf){
			callid = ((pcc_authdata_t*)auth->u.auth.generic_data)->callid;
			direction = ((pcc_authdata_t*)auth->u.auth.generic_data)->direction;
			//LOG(L_DBG, "callid is %.*s and direction %s with rating group %u\n",
			//		callid.len, callid.s, (direction==DLG_MOBILE_ORIGINATING)?"originating":"terminating",
			//		rating_group);
			client_rfb.Rf_add_ims_chg_ps_info(callid, direction, rating_group);
		}
	}

	//cdpb.AAAFreeMessage(&aaa);
	cdpb.AAASessionsUnlock(auth->hash);
	return ret;
error:
out_of_memory:
	if(auth){
		if(auth->dest_host.s){
			shm_free(auth->dest_host.s);
			auth->dest_host.s = 0; auth->dest_host.len = 0;
		}
		cdpb.AAASessionsUnlock(auth->hash);
	}
	return -1;
	
}
/*
 * After a succesfull STA is recieved the auth session should be dropped
 * and the dialog tooo.. 
 * but when? 
 * 
 * case A) STR after 6xx Decline
 * 				-> dialog is dropped in config file
 * case B) STR after BYE recieved
 * 				-> dialog is dropped by the SIP part of P-CSCF
 * case C) STR after ASR-ASA 
 * 				-> for now its done upon reciept of ASR
 * 				-> this is an automaticly generated  STR by the State Machine
 * 					so when i recieve an ashyncronous STA for this session, it should be
 * 					this case, i can then drop the dialog
 * 
 */
// I send STR and i dont wait for STA because the diameter state machine will do
// This prevents a memory leak !!!
// The SM sometimes sends STR by itself and then later has to free STA
// but if i do it there i cant access sta here.. 

//LOG(L_INFO,"PCC_STR successful STR-STA exchange\n");
		
//cdpb.AAADropAuthSession(auth);

AAAMessage * PCC_create_STR_auth_session_safe(AAASession * auth, int is_register){

	AAAMessage* dia_str = NULL;
	char x[4];
	str ims_comm_service_id = {0,0};

	LOG(L_INFO, "INFO:"M_NAME":PCC_STR_auth_session_safe : called\n");
	if (!auth){
		LOG(L_ERR,"ERR:"M_NAME":PCC_STR_auth_session: null session parameter\n");
		goto end;
	}

	if (auth->u.auth.state==AUTH_ST_DISCON){
		// If we are in DISCON is because an STR was already sent
		// so just wait for STA or for Grace Timout to happen
		goto end;
	} 

	if (pcscf_qos_release7)
		dia_str = cdpb.AAACreateRequest(IMS_Rx, IMS_STR, Flag_Proxyable, auth);
	else
		dia_str = cdpb.AAACreateRequest(IMS_Gq, IMS_STR, Flag_Proxyable, auth);
	
	if (!dia_str) goto error;
	ims_comm_service_id = ((pcc_authdata_t*)auth->u.auth.generic_data)->icsi;
	
	if(!pcc_rx_req_add_mandat_avps(dia_str, IMS_STR, NULL, is_register, 0, 0, &ims_comm_service_id))	goto error;

	//Termination-Cause
	set_4bytes(x,1);
	cdpb.AAAAddAVPToMessage(dia_str,cdpb.AAACreateAVP(AVP_Termination_Cause,AAA_AVP_FLAG_MANDATORY,0,x,4,AVP_DUPLICATE_DATA),dia_str->avpList.tail);
	
	LOG(L_INFO, "INFO:"M_NAME":PCC_STR_auth_session_safe : succsessfully created STR\n");
	return dia_str;
end:
error:
	LOG(L_INFO, "INFO:"M_NAME":PCC_STR_auth_session_safe : error while creating STR\n");
	if(dia_str) cdpb.AAAFreeMessage(&dia_str);
	return NULL;
}

int send_STR(AAASession * auth, int is_register){

	AAAMessage *dia_str = NULL;
	dia_str = PCC_create_STR_auth_session_safe(auth, is_register);
	if(dia_str){
		cdpb.AAASessionsUnlock(auth->hash);
		if (forced_qos_peer.len)
			cdpb.AAASendMessageToPeer(dia_str,&forced_qos_peer,NULL,NULL);
		else 
			cdpb.AAASendMessage(dia_str,NULL,NULL);
	
		LOG(L_INFO, "INFO:"M_NAME":send_STR : STR sent\n");
		return 1;
	}
	return 0;
}

/**
 * Sends and Session Termination Request
 * @param msg - SIP request  
 * @param tag - 0 for originating side, 1 for terminating side
 * 
 * @returns STA message or NULL on error
 */
AAAMessage* PCC_STR(struct sip_msg* msg, char *str1, contact_t * aor)
{
	AAASession *auth=0;
	p_dialog *dlg=0;
	str host;
	int port,transport;
	struct sip_msg *req;
	int is_register=(str1 && (str1[0]=='r' || str1[0]=='R'));
	enum p_dialog_direction dir;
	str call_id;	
	
	LOG(L_DBG,"PCC_STR()\n");
/** get Diameter session based on sip call_id */
	if (is_register){
		//Registration
		auth = pcc_auth_clean_register(NULL, aor, 0);

	}else {
		//Call
		dir = get_dialog_direction(str1);
		find_dialog_contact(msg,dir,&host,&port,&transport);
		call_id =cscf_get_call_id(msg, 0);
		dlg=get_p_dialog(call_id,host,port,transport,0);
		if (!dlg && msg->first_line.type==SIP_REPLY) {
			req = cscf_get_request_from_reply(msg);
			find_dialog_contact(req,dir,&host,&port,&transport);
			dlg=get_p_dialog(call_id,host,port,transport,0);
		}
			
		if (!dlg) {
			LOG(L_ERR,"PCC_STR(): ending a dialog already dropped? callid %.*s and tag %i\n",call_id.len,call_id.s,dir);
			goto end;
		}
		if (!dlg->pcc_session_id.len) {
			LOG(L_ERR,"PCC_STR(): this dialog has no pcc session associated [%.*s tag %i]\n",call_id.len,call_id.s,dir);
			d_unlock(dlg->hash);
			goto end;
		} else {	
			auth=cdpb.AAAGetAuthSession(dlg->pcc_session_id);
			// done here , so that the callback doesnt call release_call
			pcc_auth_clean_dlg_safe(dlg);
			d_unlock(dlg->hash);	
		}
	}

	if(auth && !send_STR(auth,is_register))
		cdpb.AAASessionsUnlock(auth->hash);

end:
	return NULL;
}

/* Called upon receipt of an response to a STR
 * for the moment, just a dummy handler
 */
int PCC_STA(AAAMessage *aaa, unsigned int *rc){
	
	if (pcscf_qos_release7==-1)
		return gqprima_AAA(aaa, rc);
	else
	{
		return PCC_get_result_code(aaa, rc);
	}

}

/* can be used to send an answer to a request containing a result code or an experimental result code
 *
 *
 */
AAAMessage * PCC_create_answer(AAAMessage *request, unsigned int * result_code, unsigned int * exper_result_code){

	AAAMessage * answer = NULL;
	char x[4];
	
	answer=cdpb.AAACreateResponse(request);
	if (!answer) goto error;

	if (result_code){
		set_4bytes(x,*result_code);
		cdpb.AAAAddAVPToMessage(answer,cdpb.AAACreateAVP(AVP_Result_Code,AAA_AVP_FLAG_MANDATORY,0,
					x,4,AVP_DUPLICATE_DATA),answer->avpList.tail);
	} else if (exper_result_code){
		//TODO:
		LOG(L_ERR,"ERR:PCC_RAA: experimental result code to be added here\n");
	}
error:
	return answer;

}

/* starting from the auth cdp session:
 * for registrations: clean the data from the registrar entry 
 * for dialogs: clean the data related to pcc and drop the dialog
 * TODO: release for registrations
 */
void pcc_data_release_safe(AAASession* session, int * is_register){

	pcc_authdata_t *adata;

	*is_register =0;

	LOG(L_INFO,"INFO:"M_NAME":pcc_release_all_data_safe: releasing all data of the session %p\n", session);
	if(!session)
		return;

	adata = (pcc_authdata_t *) session->u.auth.generic_data; //casting
	if(!adata)
		return;

	if(adata->subscribed_to_signaling_path_status){

		r_contact * contact = pcc_auth_clean_reg(adata->host, adata->port, adata->transport);
		if(contact) r_unlock(contact->hash);
		*is_register =1;
	}else{

		p_dialog * dlg=get_p_dialog(adata->callid,adata->host,adata->port,adata->transport,&adata->direction);
		pcc_auth_clean_dlg_safe(dlg);
		terminate_p_dialog(dlg);
		d_unlock(dlg->hash);
	}
}

/*
 * Called upon receipt of an ASR terminates the user session and returns the ASA
 * Terminates the corresponding dialog
 * @param request - the received request
 * @returns 0 always because ASA will be generated by the State Machine
 * 
*/
AAAMessage* PCC_ASA(AAAMessage *request)
{
	AAASession* session;
	AAA_AVP *avp=0;
	unsigned int code =0;
	int is_register;

	if (!request || !request->sessionId) return 0;
	session=cdpb.AAAGetAuthSession(request->sessionId->data);
	if (!session) {
		LOG(L_DBG,"DBG:"M_NAME":PCC_ASA:recovered an ASR but the session is already deleted\n");
		return 0;
	}
	
	avp=cdpb.AAAFindMatchingAVP(request,request->avpList.head,AVP_IMS_Abort_Cause,IMS_vendor_id_3GPP,AAA_FORWARD_SEARCH);
	if (avp){
		code=get_4bytes(avp->data.s);
		LOG(L_DBG,"DBG:"M_NAME":PCC_ASA: abort-cause code is %u\n", code);
	}

	LOG(L_DBG,"DBG:"M_NAME":PCC_ASA: PCRF requested an ASR.. ok, releasing all data\n");
	
	pcc_data_release_safe(session, &is_register);
	
	cdpb.AAASessionsUnlock(session->hash);
	return 0;
}

static str gg_update_cmd = {"gg.update_route", 15};
static str gg_delete_cmd = {"gg.delete_route", 15};
static str space         = {" ", 1};
#define MAX_IP_ADDRESS_LEN	64

str create_update_cmd(int32_t subs_type, str subs_name, 
		ip_address ue_ip, ip_address gg_ip, 
		time_t timestamp){

	str msg = {0,0};
	int timestamp_len;
	int subs_type_len;
	char * timestamp_s, * subs_type_s;
	str ue_ip_s ={0,0}, gg_ip_s={0,0};
	char buf[64];

	ip_address_to_str(&gg_ip, &gg_ip_s, buf, pkg);
	LOG(L_DBG, "the GG IP address is %.*s\n", gg_ip_s.len, gg_ip_s.s);

	ip_address_to_str(&ue_ip, &ue_ip_s, buf, pkg);
	LOG(L_DBG, "the UE Locator IP address is %.*s\n", 
			ue_ip_s.len, ue_ip_s.s);

	int2str((unsigned int) timestamp, &timestamp_len);
	int2str((unsigned int) subs_type, &subs_type_len);

	msg.len = gg_update_cmd.len + subs_type_len + subs_name.len +
		gg_ip_s.len + ue_ip_s.len + timestamp_len + 5*space.len + 1;
	if(!(msg.s = pkg_malloc(msg.len)))
		goto end;
	
	msg.len = sprintf(msg.s, "%.*s", gg_update_cmd.len, gg_update_cmd.s);
	
	subs_type_s = int2str((unsigned int) subs_type, &subs_type_len);
	msg.len += sprintf(msg.s+msg.len, " %.*s", subs_type_len, subs_type_s);

	msg.len += sprintf(msg.s+msg.len, " %.*s", subs_name.len, subs_name.s);

	msg.len += sprintf(msg.s+msg.len, " %.*s", ue_ip_s.len, ue_ip_s.s);

	msg.len += sprintf(msg.s+msg.len, " %.*s", gg_ip_s.len, gg_ip_s.s);

	timestamp -= time(NULL);
	timestamp_s = int2str((unsigned int) timestamp, &timestamp_len);
	msg.len += sprintf(msg.s+msg.len, " %.*s", timestamp_len, timestamp_s);
	LOG(L_DBG, "cmd is %.*s", msg.len, msg.s);
end:
	pkg_free(ue_ip_s.s); ue_ip_s.s = NULL;
	pkg_free(gg_ip_s.s); gg_ip_s.s = NULL;
	
	return msg;
out_of_memory:
	LOG(L_ERR, "out of pkg memory\n");
error:
	pkg_free(ue_ip_s.s); ue_ip_s.s = NULL;
	pkg_free(gg_ip_s.s); gg_ip_s.s = NULL;
	
	return msg;
}

str create_delete_cmd(ip_address ue_ip){

	str msg = {0,0};
	str ue_ip_s = {0,0};
	char buf[64];

	ip_address_to_str(&ue_ip, &ue_ip_s, buf, pkg);
	LOG(L_DBG, "the UE Locator IP address is %.*s\n", 
			ue_ip_s.len, ue_ip_s.s);

	msg.len = gg_delete_cmd.len + ue_ip_s.len + 1;
	if(!(msg.s = pkg_malloc(msg.len)))
		goto end;
	
	msg.len = sprintf(msg.s, "%.*s", gg_delete_cmd.len, gg_delete_cmd.s);
	
	msg.len += sprintf(msg.s+msg.len, " %.*s", ue_ip_s.len, ue_ip_s.s);

	LOG(L_DBG, "cmd is %.*s", msg.len, msg.s);
end:
	pkg_free(ue_ip_s.s); ue_ip_s.s = NULL;
	return msg;

out_of_memory:
	LOG(L_ERR, "out of pkg memory\n");
error:
	pkg_free(ue_ip_s.s); ue_ip_s.s = NULL;
	
	return msg;
}

int gg_change_event_handler(AAAMessage * rar, str * msg){

	AAA_AVP_LIST        gg_enforce, avp_list;
	ip_address gg_ip, ue_ip;

	time_t timestamp = time(NULL);
	int32_t subs_type;
	str subs_name;
	int is_update = 0;
	
	msg->s = 0; msg->len = 0;

	if(cdp_avp->epcapp.get_GG_Enforce(rar->avpList,&gg_enforce,0)){
		
			is_update= 1;

		if(!cdp_avp->epcapp.get_GG_IP(gg_enforce, &gg_ip, 0)){
			LOG(L_ERR, "could not find the GG_IP AVP\n");
			return -1;	
		}

		if(!(cdp_avp->epcapp.get_UE_Locator_Id_Group(gg_enforce, &avp_list, 0))){
			LOG(L_ERR, "could not find the UE Locator Id AVP\n");
			return -1;	
		}
	}else
		avp_list = rar->avpList;

	if(!(cdp_avp->epcapp.get_UE_Locator(avp_list, &ue_ip, 0))){
		LOG(L_ERR, "could not find the UE Locator AVP\n");
		return -1;	
	}

	if(!cdp_avp->ccapp.get_Subscription_Id_Group(avp_list,
						&subs_type,
						&subs_name,
						NULL)){
		LOG(L_ERR, "could not find the Subscription Id AVP\n");
		return -1;
	}

	if (subs_type!=AVP_EPC_Subscription_Id_Type_End_User_IMSI){
		
		LOG(L_ERR, "unexpected subscription type in Locator_Id_Group\n");
		return -1;
	}

	//LOG(L_DBG, "IMSI is %.*s\n", subs_name.len, subs_name.s);

	if(is_update)
		*msg  = create_update_cmd(subs_type, subs_name, ue_ip, gg_ip, timestamp);
	else	
		*msg  = create_delete_cmd(ue_ip);
	if(!msg->s || !msg->len)
		goto error;

		
	return 1;
error:
	if(msg->s) pkg_free(msg->s);
	return -1;
}

int udp_socket_send(str msg,int socket,ip_address to_addr,uint16_t to_port)
{
	struct sockaddr_in saddr4;	
	struct sockaddr_in6 saddr6;
	struct sockaddr *saddr=0;
	socklen_t to_addr_len = 0;
	int len=0;
	bzero(&saddr4,to_addr_len);
	
	switch (to_addr.ai_family) {
		case AF_INET:
			to_addr_len = sizeof(struct sockaddr_in);
			saddr4.sin_family = to_addr.ai_family;
			saddr4.sin_addr = to_addr.ip.v4;
			saddr4.sin_port = htons(to_port);	
			saddr = (struct sockaddr*)&saddr4;		
			break;
		case AF_INET6:
			to_addr_len = sizeof(struct sockaddr_in6);
			saddr6.sin6_family = to_addr.ai_family;
			memcpy(&(saddr6.sin6_addr),&(to_addr.ip.v6),sizeof(to_addr.ip.v6));
			saddr6.sin6_port = htons(to_port);	
			saddr = (struct sockaddr*)&saddr6;		
			break;
		default:
			LOG(L_ERR,"Not handled ai_family %d!",to_addr.ai_family);
			return 0;
	}
	
	
	len = sendto(socket,
			msg.s,msg.len,
			0,
			saddr,
			to_addr_len);
	if (len==msg.len) LOG(L_DBG,"Successfully sent %d bytes.\n",len);
	else LOG(L_ERR,"Error sending %d bytes!\n",len);
	
	return len;
}

void gg_send_notify(str notify){

	if(!notify.s || !notify.len)
		return;
	
	if(!gg_af_socket) {
		LOG(L_ERR, "no corresponding GG was configured\n");
		return;
	}
	
	LOG(L_DBG, "sending the notify %.*s\n", notify.len, notify.s);
	udp_socket_send(notify, gg_af_socket, gg_af_addr, gg_af_port);
}

/*
 * TODO:
 * In the RAR
 *
 * Specific-Action AVP
 * Fows AVP
 * -Media-Component-Number AVP (mandatory)
 * -FlowNumber*
 * Abort-Cause AVP
 *
 * Actions can be
 * INDICATION_OF_RELEASE_OF_BEARER,INDICATION_OF_SUBSCRIBER_DETACHMENT,INDICATION_OF_RESERVATION_EXPIRATION,
 * INDICATION_OF_LOSS_OF_BEARER
 */

AAAMessage* PCC_RAA(AAAMessage *request)
{
	AAASession* session;
	AAA_AVP *avp=0;
	AAAMessage *raa=0, *dia_str = 0;
	unsigned int code=0, rc=0;
	int is_register;
	str notify_msg = {0,0};
	
	if (!request && !request->sessionId) return 0;
	session = cdpb.AAAGetAuthSession(request->sessionId->data);
	if (!session){
		LOG(L_DBG,"DBG:"M_NAME":PCC_RAA: received a RAR for non existing session\n");
		return 0;
	}

	rc = AAA_SUCCESS;
	if(!(raa= PCC_create_answer(request, &rc, NULL)))
		goto error;
	
	avp=cdpb.AAAFindMatchingAVP(request,request->avpList.head,AVP_IMS_Abort_Cause,IMS_vendor_id_3GPP,AAA_FORWARD_SEARCH);
	if (avp)
	{
		code=get_4bytes(avp->data.s);
		switch (code)
		{
			case AVP_IMS_Abort_Cause_Bearer_Released:
				LOG(L_DBG,"DBG:"M_NAME":PCC_RAA:session %.*s aborted because bearer released\n",session->id.len,session->id.s);
				break;
			case AVP_IMS_Abort_Cause_Insufficient_Bearer_Resources:
				LOG(L_DBG,"DBG:"M_NAME":PCC_RAA:session %.*s aborted because insufficient bearer resources\n",session->id.len,session->id.s);
				break;
			case AVP_IMS_Abort_Cause_Insufficient_Server_Resources:
				LOG(L_DBG,"DBG:"M_NAME":PCC_RAA:session %.*s aborted because insufficient server resources\n",session->id.len,session->id.s);
				break;
			default:
				LOG(L_DBG,"DBG:"M_NAME":PCC_RAA:session %.*s aborted because unknown reason %d\n",session->id.len,session->id.s,code);
				break;
		}
		goto terminate;
	}

	avp=cdpb.AAAFindMatchingAVP(request,request->avpList.head,AVP_IMS_Specific_Action,IMS_vendor_id_3GPP,AAA_FORWARD_SEARCH);
	if (avp)
	{
		//what to do on specific actions
		code=get_4bytes(avp->data.s);
		switch (code)
		{
			case AVP_IMS_Specific_Action_Charging_Correlation_Exchange:
				LOG(L_DBG,"DBG:"M_NAME":PCC_RAA: specific action charging correlation exchange not supported\n");
				break;
			case AVP_IMS_Specific_Action_Indication_Of_Establishment_Of_Bearer:
				LOG(L_DBG,"DBG:"M_NAME":PCC_RAA: specific action establishment of bearer :\n\t pull mode until AF not supported\n");
				break;
			case AVP_IMS_Specific_Action_Indication_Of_Loss_Of_Bearer:
				LOG(L_DBG,"DBG:"M_NAME":PCC_RAA: specific action loss of bearer\n");
				break;
			case AVP_IMS_Specific_Action_Indication_Of_Recovery_Of_Bearer:
				LOG(L_DBG,"DBG:"M_NAME":PCC_RAA: specific action recovery of bearer\n");
				break;
			case AVP_IMS_Specific_Action_Indication_Of_Release_Of_Bearer:
				LOG(L_DBG,"DBG:"M_NAME":PCC_RAA: specific-action release of bearer - termination\n");
				goto terminate;
				break;
			case AVP_IMS_Specific_Action_Service_Information_Request:
				LOG(L_DBG,"DBG:"M_NAME":PCC_RAA: specific action service information request :\n\t pull mode until AF not supported\n");
				break;
			case AVP_EPC_Specific_Action_Indication_of_Generic_Gateway_Change:
				LOG(L_DBG,"DBG:"M_NAME":PCC_RAA: specific action generic gateway change\n");
				if(gg_change_event_handler(request, 
							&notify_msg)<0)
					goto error;
				gg_send_notify(notify_msg);
				if(notify_msg.s) pkg_free(notify_msg.s);
				break;
			default:
				LOG(L_DBG,"DBG:"M_NAME":PCC_RAA: specific action received unknown %d\n",code);
				break;
		}
	}
	//look for
	//Flows, Subscription-Id,
	//IP-CAN Type, RAT Type (this might have changed)

	cdpb.AAASessionsUnlock(session->hash);
	session = 0;
	
	return raa;

	
terminate:
	pcc_data_release_safe(session, &is_register);
	//first reply to the RAR
	if (forced_qos_peer.len){
		cdpb.AAASendMessageToPeer(raa,&forced_qos_peer,NULL,NULL);
	}else {
		cdpb.AAASendMessage(raa,NULL,NULL);
	}

	//then generate a STR-STA
	dia_str = PCC_create_STR_auth_session_safe(session, is_register);
	if(dia_str){
		cdpb.AAASessionsUnlock(session->hash);
		session = 0;
		if (forced_qos_peer.len)
			cdpb.AAASendMessageToPeer(dia_str,&forced_qos_peer,NULL,NULL);
		else 
			cdpb.AAASendMessage(dia_str,NULL,NULL);
	
		LOG(L_INFO, "INFO:"M_NAME":PCC_RAA : STR sent\n");
	}
	
error:
	if(session) cdpb.AAASessionsUnlock(session->hash);
	session=0;
	return 0;

}

/**
 * Handler for incoming Diameter requests.
 * @param request - the received request
 * @param param - generic pointer
 * @returns the answer to this request
 */
AAAMessage* PCCRequestHandler(AAAMessage *request,void *param)
{
	if (is_req(request)){		
		LOG(L_INFO,"INFO:"M_NAME":PCCRequestHandler(): We have received a request\n");
		#ifdef WITH_IMS_PM
			ims_pm_diameter_request(request);
		#endif		
		switch(request->applicationId){
        	case IMS_Rx:
        	case IMS_Gq: // Its almost the same!
				switch(request->commandCode){				
					case IMS_RAR:
						LOG(L_INFO,"INFO:"M_NAME":PCCRequestHandler():- Received an IMS_RAR \n");						
						return PCC_RAA(request);
						break;
					case IMS_ASR:
						LOG(L_INFO,"INFO:"M_NAME":PCCRequestHandler(): - Received an IMS_ASR \n");
						return PCC_ASA(request);
						break;
					default :
						LOG(L_ERR,"ERR:"M_NAME":PCCRequestHandler(): - Received unknown request for Rx  or Gq command %d, flags %#1x endtoend %u hopbyhop %u\n",request->commandCode,request->flags, request->endtoendId, request->hopbyhopId);
						return 0;
						break;
							
				}
				break;
			default:
				LOG(L_ERR,"ERR:"M_NAME":PCCRequestHandler(): - Received unknown request for app %d command %d\n",
					request->applicationId,
					request->commandCode);
					return 0;
				break;				
		}					
	}
	return 0;		
}

void terminate_pcc_session(str session_id, int is_register)
{
	AAASession *s=0;
	s = cdpb.AAAGetAuthSession(session_id);
	if (s)
	{
		if(!send_STR(s, is_register))
			cdpb.AAASessionsUnlock(s->hash);
			
		//LOG(L_DBG,"terminate_pcc_session calling AAATerminateAuthSession\n");
		//cdpb.AAATerminateAuthSession(s);
	}
}

