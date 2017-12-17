/**
 * $Id: pcc_avp.c 1164 2011-10-11 07:46:06Z aon $
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
 * P-CSCF Policy and Charging Control interfaces AVPs
 *
 *\author Alberto Diez Albaladejo -at- fokus dot fraunhofer dot de
 */ 

#include <arpa/inet.h>
#include "pcc_avp.h"
#include "../../mem/shm_mem.h"
#include "../../parser/parse_uri.h"
#include "../../parser/contact/parse_contact.h"

extern str ipv4_for_signaling;
extern str ipv6_for_signaling;
extern unsigned short port_for_signaling;
extern int pcc_use_ports;	

static char* ip_s = "ip";
static char* udp_s = "udp";
static char* tcp_s = "tcp";
extern int pcc_use_protocol;                            /**< weather to include int the Flow Description AVP the protocol (udp, tcp) or use only ip>**/

/**< Structure with pointers to cdp funcs, global variable defined in mod.c  */
extern struct cdp_binds cdpb;
extern cdp_avp_bind_t *cdp_avp;
/**
 * Looks for the contact in the sip message and gets the ip address
 * @param r - sip message to look for contact (for dialogs)
 * @param parsed_aor - the parsed aor of a contact from a 200 reply to a REGISTER
 * @param ip - the ip address to return
 * @returns the version from the enum ip_type, 0 if error
 * TODO: aon: tests for ipv6 required
 */
uint16_t pcc_get_ip_port(struct sip_msg *r, struct sip_uri * parsed_aor, str *ip, unsigned short * port)
{
	uint16_t version= AF_INET;
	struct contact_body *cb;
	struct sip_uri uri;
	if (!ip) return 0;
	ip->len=0;
	ip->s=0;
	
	//if used for invite requests
	if (r&&r->contact&&r->contact->parsed){		
		cb = (struct contact_body *)r->contact->parsed;
		if (!cb || !cb->contacts)
			goto error;
		parse_uri(cb->contacts->uri.s,cb->contacts->uri.len,&uri);
		*ip = uri.host;
		*port = uri.port_no;

	//used for register replies	
	}else if(parsed_aor){
		*ip = parsed_aor->host;
		*port = parsed_aor->port_no;

	}else if(!parsed_aor)
		goto error;

	if(ip->len){
		if(ip->s[0] == '['){
			version = AF_INET6;
			ip->s +=1;
			ip->len -=1;
		}	
		if(ip->s[ip->len-1] == ']'){
			version  = AF_INET6;
			ip->len -= 1;
		}
	}

	LOG(L_DBG,"DBG:"M_NAME":pcc_get_ip_port: %.*s\n",ip->len,ip->s);
	return version;
error:
	LOG(L_ERR,"ERR:"M_NAME":pcc_get_ip_port: invalid or no aor found\n");
	return 0;
}

/*
 *******************************************************************************
 * Basic operations to modify an AVP, see also Cx_avp.c
 * 
 *******************************************************************************
 */

/**
 * Create and add an AVP to a Diameter message.
 * @param m - Diameter message to add to 
 * @param d - the payload data
 * @param len - length of the payload data
 * @param avp_code - the code of the AVP
 * @param flags - flags for the AVP
 * @param vendorid - the value of the vendor id or 0 if none
 * @param data_do - what to do with the data when done
 * @param func - the name of the calling function, for debugging purposes
 * @returns 1 on success or 0 on failure
 */
inline int PCC_add_avp(AAAMessage *m,char *d,int len,int avp_code,
	int flags,int vendorid,int data_do,const char *func)
{
	AAA_AVP *avp;
	if (vendorid!=0) flags |= AAA_AVP_FLAG_VENDOR_SPECIFIC;
	avp = cdpb.AAACreateAVP(avp_code,flags,vendorid,d,len,data_do);
	if (!avp) {
		LOG(L_ERR,"ERR:"M_NAME":%s: Failed creating avp\n",func);
		return 0;
	}
	if (cdpb.AAAAddAVPToMessage(m,avp,m->avpList.tail)!=AAA_ERR_SUCCESS) {
		LOG(L_ERR,"ERR:"M_NAME":%s: Failed adding avp to message\n",func);
		cdpb.AAAFreeAVP(&avp);
		return 0;
	}
	return 1;
}



/**
 * Create and add an AVP to a list of AVPs.
 * @param list - the AVP list to add to 
 * @param d - the payload data
 * @param len - length of the payload data
 * @param avp_code - the code of the AVP
 * @param flags - flags for the AVP
 * @param vendorid - the value of the vendor id or 0 if none
 * @param data_do - what to do with the data when done
 * @param func - the name of the calling function, for debugging purposes
 * @returns 1 on success or 0 on failure
 */
static inline int PCC_add_avp_list(AAA_AVP_LIST *list,char *d,int len,int avp_code,
	int flags,int vendorid,int data_do,const char *func)
{
	AAA_AVP *avp;
	if (vendorid!=0) flags |= AAA_AVP_FLAG_VENDOR_SPECIFIC;
	avp = cdpb.AAACreateAVP(avp_code,flags,vendorid,d,len,data_do);
	if (!avp) {
		LOG(L_ERR,"ERR:"M_NAME":%s: Failed creating avp\n",func);
		return 0;
	}
	if (list->tail) {
		avp->prev=list->tail;
		avp->next=0;	
		list->tail->next = avp;
		list->tail=avp;
	} else {
		list->head = avp;
		list->tail = avp;
		avp->next=0;
		avp->prev=0;
	}
	
	return 1;
}

/**
 * Returns the value of a certain AVP from a Diameter message.
 * @param m - Diameter message to look into
 * @param avp_code - the code to search for
 * @param vendorid - the value of the vendor id to look for or 0 if none
 * @param func - the name of the calling function, for debugging purposes
 * @returns the str with the payload on success or an empty string on failure
 */
static inline str PCC_get_avp(AAAMessage *msg,int avp_code,int vendor_id,
							const char *func)
{
	AAA_AVP *avp;
	str r={0,0};
	
	avp = cdpb.AAAFindMatchingAVP(msg,0,AVP_Result_Code,0,0);
	if (avp==0){
		//LOG(L_INFO,"INFO:"M_NAME":%s: Failed finding avp\n",func);
		return r;
	}
	else 
		return avp->data;
}

/*
 *******************************************************************************
 * Create and add AVPs to a Diameter message.
 * 
 *******************************************************************************
 */

/*creates an AVP for the framed-ip info: 
 * 	if ipv4: AVP_Framed_IP_Address, 
 * 	otherwise: AVP_Framed_IPv6_Prefix
 * 	using inet_pton to convert the IP addresses 
 * 	from human-readable strings to their bynary representation
 * 	see http://beej.us/guide/bgnet/output/html/multipage/inet_ntopman.html
 * 	http://beej.us/guide/bgnet/output/html/multipage/sockaddr_inman.html
 */ 	
int PCC_add_framed_ip_avp(AAA_AVP_LIST * list, str ip, uint16_t version)
{
	ip_address_prefix ip_adr;
	char* ip_pkg = 0;
	int ret = 0;

	if (ip.len<0) return 0;
	if(version == AF_INET){
		if(ip.len>INET_ADDRSTRLEN)
			goto error;
	}else{
		if(ip.len>INET6_ADDRSTRLEN)
			goto error;
	}
	ip_pkg = (char*)pkg_malloc((ip.len+1)*sizeof(char));
	if(!ip_pkg){
		LOG(L_ERR, "ERR:"M_NAME":PCC_create_framed_ip_avp: could not allocate %i from pkg\n", ip.len+1);
		goto error;
	}
	memcpy(ip_pkg, ip.s, ip.len);
	ip_pkg[ip.len] = '\0';

	ip_adr.addr.ai_family = version;
	
	if(version == AF_INET){
		
		if(inet_pton(AF_INET, ip_pkg, &(ip_adr.addr.ip.v4.s_addr))!= 1) goto error;
		ret= cdp_avp->nasapp.add_Framed_IP_Address(list, ip_adr.addr);
	}else{ 
		
		if(inet_pton(AF_INET6, ip_pkg, &(ip_adr.addr.ip.v6.s6_addr))!=1) goto error;
		ret= cdp_avp->nasapp.add_Framed_IPv6_Prefix(list, ip_adr);
	}

error:
	if(ip_pkg) pkg_free(ip_pkg);
	return ret;
}

/**
 * Creates and adds a Vendor Specific Application ID Group AVP.
 * @param msg - the Diameter message to add to.
 * @param vendor_id - the value for the vendor id AVP
 * @param auth_app_id - the value of the authentication application AVP
 * @returns 1 on success or 0 on error
 */
int PCC_add_vendor_specific_application_id_group(AAAMessage * msg, uint32_t vendor_id, uint32_t auth_app_id)
{
	return cdp_avp->base.add_Vendor_Specific_Application_Id_Group(&(msg->avpList),vendor_id,auth_app_id,0);
}

/**
 * Creates and adds a Destination-Realm AVP.
 * @param msg - the Diameter message to add to.
 * @param data - the value for the AVP payload
 * @returns 1 on success or 0 on error
 */
inline int PCC_add_destination_realm(AAAMessage *msg, str data)
{
	return 
	PCC_add_avp(msg,data.s,data.len,
		AVP_Destination_Realm,
		AAA_AVP_FLAG_MANDATORY,
		0,
		AVP_DUPLICATE_DATA,
		__FUNCTION__);
}

/**
 * Creates and adds an Acct-Application-Id AVP.
 * @param msg - the Diameter message to add to.
 * @param data - the value for the AVP payload
 * @return 1 on success or 0 on error
 */
inline int PCC_add_auth_application_id(AAAMessage *msg, unsigned int data)
{
	char x[4];
	set_4bytes(x, data);	

	return
	PCC_add_avp(msg, x, 4,
			AVP_Auth_Application_Id,
			AAA_AVP_FLAG_MANDATORY,
			0,
			AVP_DUPLICATE_DATA,
			__FUNCTION__);
}
/*
 * Creates and adds a Subscription_Id AVP
 * @param msg - the Diameter message to add to.
 * @param r - the sip_message to extract the data from.
 * @param tag - originating (0) terminating (1)
 * @return 1 on success or 0 on error
 * 
 */
/*
 * For now i add the IMPU because i dont know where to get the IMSI from
 */
inline int PCC_add_subscription_ID(AAAMessage *msg,struct sip_msg *r,int tag, 
		int * subscr_type, str * subscr_value)
{
	AAA_AVP_LIST list;
	AAA_AVP *type,*data;
	str identification;
	char x[4];
	list.head=0;
	list.tail=0;
	
	LOG(L_INFO,"ADDING SUBSCRIPTION ID\n");
	*subscr_type=extract_id(r,tag,subscr_value);
	LOG(L_DBG, "extracted subscr is %.*s\n", subscr_value->len, subscr_value->s);
	// if this returns -1 then better not to add anything
	if (*subscr_type==-1)
		return 0;
		
 	set_4bytes(x,*subscr_type);
	
	/*identification is just a pointer to something reserved somewhere else and a number...*/
	type=cdpb.AAACreateAVP(AVP_Subscription_Id_Type,
 											AAA_AVP_FLAG_MANDATORY,
 											0,x,4,
 											AVP_DUPLICATE_DATA);
	
	data=cdpb.AAACreateAVP(AVP_Subscription_Id_Data,
 											AAA_AVP_FLAG_MANDATORY,
 											0,subscr_value->s,subscr_value->len,
 											AVP_DUPLICATE_DATA);
 											
 	cdpb.AAAAddAVPToList(&list,type);
 	cdpb.AAAAddAVPToList(&list,data);
 	
	identification=cdpb.AAAGroupAVPS(list);
  	 		
  	PCC_add_avp(msg,identification.s, identification.len,AVP_Subscription_Id,
 				AAA_AVP_FLAG_MANDATORY,0,
 				AVP_DUPLICATE_DATA,
 				__FUNCTION__);
 				
 	cdpb.AAAFreeAVPList(&list);
 	return 1;
}

/*add all the AVPs the AAR requires that can be build from the contact URI
 * @param msg - the AAR message
 * @param parsed_uri - the parsed contact URI
 * returns 0 if error, 1 if ok
 */
int PCC_AAR_add_avps_for_register(AAAMessage *msg, struct sip_uri* parsed_uri){

	str ip;
	unsigned short port_no;
	uint16_t ip_version;

	ip_version = pcc_get_ip_port(NULL, parsed_uri, &ip, &port_no);
	
	if(!PCC_add_media_component_description_for_register(msg, ip, port_no, ip_version)) goto error;

	if(!PCC_add_framed_ip_avp(&msg->avpList, ip, ip_version)) goto error;

	if(!cdp_avp->epcapp.add_Specific_Action(&msg->avpList, 
				AVP_EPC_Specific_Action_Indication_of_Release_of_Bearer)) goto error;

	if(!cdp_avp->epcapp.add_Specific_Action(&msg->avpList, 
				AVP_EPC_Specific_Action_IPCAN_Change)) goto error;

	if(!cdp_avp->epcapp.add_Specific_Action(&msg->avpList, 
				AVP_EPC_Specific_Action_Indication_of_Generic_Gateway_Change)) goto error;

	return 1;
error:
	return 0;
}

/**
 * This function creates a Media Component Description with a wildcarded flow
 * that will be set to AF Signaling for the messages between this P-CSCF and the
 * registered UE both in uplink and downlink
 * @param msg - the Diameter message to add the avps to
 * @param req - the reuqest
 * @param res - the response
 */
int PCC_add_media_component_description_for_register(AAAMessage *msg, str ip_from, unsigned short from_port_no, uint16_t version)
{
	AAA_AVP *avp=0;
	AAA_AVP_LIST list={0,0};
	str data={0,0};
	str ip_to;

	if(version == AF_INET){
		ip_to  = ipv4_for_signaling;
	}else{
		ip_to  = ipv6_for_signaling;
	}
	
	avp=PCC_create_media_subcomponent(0, ip_s, 
					ip_from, from_port_no, 
					ip_to, port_for_signaling,  "", 4);
	if(!avp) goto error;
	cdpb.AAAAddAVPToList(&list,avp);
	if(!cdp_avp->epcapp.add_Media_Component_Number(&list, 0))
		goto error;
	
	data=cdpb.AAAGroupAVPS(list);
	if(!data.s) goto error;
  	cdpb.AAAFreeAVPList(&list);
  	PCC_add_avp(msg,data.s,data.len,AVP_IMS_Media_Component_Description,
 				AAA_AVP_FLAG_MANDATORY|AAA_AVP_FLAG_VENDOR_SPECIFIC,
 				IMS_vendor_id_3GPP,
 				AVP_FREE_DATA,
 				__FUNCTION__);

	return 1;
error:
	cdpb.AAAFreeAVPList(&list);
	return 0;
}
 
/**
 * Creates and adds a Media Component Description AVP
 * @param msg - the Diameter message to add to.
 * @param sdpinvite - the SDP body of the INVITE message
 * @param sdp200 - the SDP body of the 200 OK message
 * @param mline - pointer to m= line in sdpinvite
 * @number - the media component number relating to this SDP description
 * @tag - 0 is originating side and 1 is terminating side
 * @return 1 on success or 0 on error
 */
 
inline int PCC_add_media_component_description(AAAMessage *msg,str sdpinvite,str sdp200,char *mline,int number,int tag)
 {
 	str data;
 	AAA_AVP_LIST list;
 	AAA_AVP *media_component_number,*media_type;
 	AAA_AVP *codec_data1,*codec_data2;
 	AAA_AVP *media_sub_component[PCC_Media_Sub_Components];
 	AAA_AVP *Max_DL,*Max_UL;
 	AAA_AVP *RR,*RS;
 	AAA_AVP *flow_status;
 	
 	bandwidth bwUL,bwDL;
 	
 	
 	char *ptr;
 	str port;
 	int type,i,n,a;
 	char x[4];
	
 	
 	/*needed to use AAA_AVP-LIST!!*/
 	list.head=0;	 	
 	list.tail=0;
 	
 	LOG(L_DBG,"PCC_add_media_component_description() : starting\n");
 	
 	/*media-component-number*/
 	 	
 	set_4bytes(x,number);	
 	media_component_number=cdpb.AAACreateAVP(AVP_IMS_Media_Component_Number,
 											AAA_AVP_FLAG_MANDATORY|AAA_AVP_FLAG_VENDOR_SPECIFIC,
 											IMS_vendor_id_3GPP,x,4,
 											AVP_DUPLICATE_DATA);

 	if (media_component_number!=NULL) 
 	{
 		cdpb.AAAAddAVPToList(&list,media_component_number);
 	} else {
 		LOG(L_ERR,"ERR:"M_NAME"Unable to create media_component_number AVP");
 		return 0;
 	}
 	
 	/*media-sub-component*/
 	
	n=PCC_create_add_media_subcomp_dialog(&list,sdpinvite,sdp200,number,media_sub_component,tag);
 	if(n==-1)
 	{
 		LOG(L_ERR,"ERR:"M_NAME"Unable to create media_sub_components list AVP");
 		cdpb.AAAFreeAVP(&media_component_number);
 		list.head=0;
 		list.tail=0;	
 		return 0;
 	}
 	if (n==0) {
 		/*we don't create a Media Description for this media line*/
 		/*because answerer rejected the offer!*/
 		cdpb.AAAFreeAVP(&media_component_number);
 		list.head=0;
 		list.tail=0;
 		return 1;
 	}
 	
 	
 	
 	/*if n=-1 then its error*/
 	/*if n=0 is because answerer rejected this media offer*/
 	/*or offerer wanted it to be disabled*/
 	 	
 	/*media-type*/
 	 ptr=mline;
 	 ptr+=2; /*skip m=*/
 	 	 
 	 
 	 
 	 if (strncmp(ptr,"audio",5)==0)
 	 {
 	 	type=AVP_IMS_Media_Type_Audio;
 	 } else if(strncmp(ptr,"video",5)==0)
 	 {
 	 	type=AVP_IMS_Media_Type_Video;
 	 }else if(strncmp(ptr,"data",4)==0)
 	 {
 	 	type=AVP_IMS_Media_Type_Data;
 	 }else if(strncmp(ptr,"application",11)==0)
 	 {
 	 	type=AVP_IMS_Media_Type_Application;
 	 }else if(strncmp(ptr,"control",7)==0)
 	 {
 	 	type=AVP_IMS_Media_Type_Control;
 	 }else if(strncmp(ptr,"text",4)==0)
 	 {
 	 	type=AVP_IMS_Media_Type_Text;
 	 }else if(strncmp(ptr,"message",7)==0)
 	 {
 	 	type=AVP_IMS_Media_Type_Message;
 	 } else 
 	 {
 	 	type=AVP_IMS_Media_Type_Other;
 	 }
 	 
 	  	 
 	 set_4bytes(x,type);
 	 media_type=cdpb.AAACreateAVP(AVP_IMS_Media_Type,
 	 								AAA_AVP_FLAG_MANDATORY|AAA_AVP_FLAG_VENDOR_SPECIFIC,
 	 								IMS_vendor_id_3GPP,x,4,
 	 								AVP_DUPLICATE_DATA);
 	 cdpb.AAAAddAVPToList(&list,media_type);								

 		 								
 	
 	/*Max-Requested-Bandwidth-UL*/
 	/*Max-Requested-Bandwidth-DL*/
 	/*SDP bodies have been checked by pcc_create_add_media_subcomponents*/
 	
 	i=1;
 	ptr=find_sdp_line(sdp200.s,(sdp200.s+sdp200.len),'m');
 	
 	while(i!=number) 
 	{
 		ptr=find_next_sdp_line(ptr,(sdp200.s+sdp200.len),'m',NULL);
 		i++;	
 	}
 	
 	
 	
 	if(tag==1)
 	{
 		/*in the invite its defined how much bandwidth
 		 * you want in the downlink!*/
 		extract_bandwidth(&bwDL,sdpinvite,mline);
 		extract_bandwidth(&bwUL,sdp200,ptr);
 	} else {
 		extract_bandwidth(&bwDL,sdp200,ptr);
 		extract_bandwidth(&bwUL,sdpinvite,mline);	
 		 		
 	}
 	/*
 	 * IN a SDP b=AS:x line  the x is the amount of bandwidth
 	 * the sender of the SDP body is willing to RECEIVE 
 	 * therefor the next code is right 
 	 * 
 	 * */
	if (bwDL.bAS!=0)
	 {
	 	    // @luminate - Increasing the requested BW by one as polaris divides 
            // this value by 1024 instead of 1000 and the validation fails for MBR and GBR
 			// set_4bytes(x,bwDL.bAS*1000);
 			set_4bytes(x,(bwDL.bAS + 1)*1000);
 			Max_UL=cdpb.AAACreateAVP(AVP_IMS_Max_Requested_Bandwidth_UL,
											AAA_AVP_FLAG_MANDATORY|AAA_AVP_FLAG_VENDOR_SPECIFIC,
											IMS_vendor_id_3GPP,x,4,
											AVP_DUPLICATE_DATA);
			cdpb.AAAAddAVPToList(&list,Max_UL);
		
	 }
	if (bwUL.bAS!=0)
	{
	 	    // @luminate - Increasing the requested BW by one as polaris divides 
            // this value by 1024 instead of 1000 and the validation fails for MBR and GBR
	        // set_4bytes(x,bwUL.bAS*1000);
 			set_4bytes(x,(bwUL.bAS + 1) *1000);
 			Max_DL=cdpb.AAACreateAVP(AVP_IMS_Max_Requested_Bandwidth_DL,
											AAA_AVP_FLAG_MANDATORY|AAA_AVP_FLAG_VENDOR_SPECIFIC,
											IMS_vendor_id_3GPP,x,4,
											AVP_DUPLICATE_DATA);
			cdpb.AAAAddAVPToList(&list,Max_DL);
			
	}
	
	
	
 	/*Flow-Status*/
 	/*lets follow the specs*/
	if (tag==0)
	{
		extract_token(mline,&port,PCC_MAX_Char,2);
	} else {
		extract_token(ptr,&port,PCC_MAX_Char,2);
	}

	if(strncmp(port.s,"0",1)==0)
	{
		set_4bytes(x,AVP_IMS_Flow_Status_Removed);
	} else {
  			
		if (tag==1)
		{
			a=check_atributes(sdp200,ptr);
		} else {
			a=check_atributes(sdpinvite,mline);
		}
  			
		if (a==1)
		{
			set_4bytes(x,AVP_IMS_Flow_Status_Enabled_Uplink);
  		} else if(a==2)
  		{
  			set_4bytes(x,AVP_IMS_Flow_Status_Enabled_Downlink);
  		} else {
  			set_4bytes(x,AVP_IMS_Flow_Status_Enabled);
  		} 
  			
  			
 	}
 	if(port.s){ pkg_free(port.s); port.s = 0; }
 	
 		
 	flow_status=cdpb.AAACreateAVP(AVP_IMS_Flow_Status,
 											AAA_AVP_FLAG_MANDATORY|AAA_AVP_FLAG_VENDOR_SPECIFIC,
 											IMS_vendor_id_3GPP,x,4,
 											AVP_DUPLICATE_DATA);
		cdpb.AAAAddAVPToList(&list,flow_status);
 	
 	
 	/*RR and RS*/
 	
 	
 	x[0]=0; x[1]=0; x[2]=0; x[3]=0;
 	
		
			if(bwUL.bRR!=0) {
				set_4bytes(x,bwUL.bRR);
			} /*else if (bwDL.bRS!=0) {
				set_4bytes(x,bwDL.bRS);
			}*/
		
	RR=0;
	if (x[0]!=0 || x[1]!=0 || x[2]!=0 || x[3]!=0) {
			RR=cdpb.AAACreateAVP(AVP_IMS_RR_Bandwidth,
											AAA_AVP_FLAG_MANDATORY|AAA_AVP_FLAG_VENDOR_SPECIFIC,
											IMS_vendor_id_3GPP,x,4,
											AVP_DUPLICATE_DATA);
			cdpb.AAAAddAVPToList(&list,RR);
			 	 		
	}	
	

 	x[0]=0; x[1]=0; x[2]=0; x[3]=0;
 	
			if(bwUL.bRS!=0) 
			{
				set_4bytes(x,bwUL.bRS);
			} /*else if(bwDL.bRR!=0){
				set_4bytes(x,bwDL.bRR);
			}*/
	
			
 	RS=0;
	if (x[0]!=0 || x[1]!=0 || x[2]!=0 || x[3]!=0)
	 {
		RS=cdpb.AAACreateAVP(AVP_IMS_RS_Bandwidth,
								AAA_AVP_FLAG_MANDATORY|AAA_AVP_FLAG_VENDOR_SPECIFIC,
								IMS_vendor_id_3GPP,x,4,
								AVP_DUPLICATE_DATA);
		cdpb.AAAAddAVPToList(&list,RS);
		
	}		
 	
 	
 	
 	/*codec-data*/
 	 	
 	
 	if (tag==0)
 	{
 		/*0 means uplink offer*/
 		codec_data1=PCC_create_codec_data(sdpinvite,number,0);
 		cdpb.AAAAddAVPToList(&list,codec_data1);
 		/*3 means downlink answer*/
 		codec_data2=PCC_create_codec_data(sdp200,number,3);
 		cdpb.AAAAddAVPToList(&list,codec_data2);
 	} else { 
 		/*2 means downlink offer*/
 		codec_data1=PCC_create_codec_data(sdpinvite,number,2);
 		cdpb.AAAAddAVPToList(&list,codec_data1);
 		/*1 means uplink answer*/
 		codec_data2=PCC_create_codec_data(sdp200,number,1);
 		cdpb.AAAAddAVPToList(&list,codec_data2);
 	
 	}
 	/*now group them in one big AVP and free them*/
 	
 	data=cdpb.AAAGroupAVPS(list);
  	 cdpb.AAAFreeAVPList(&list);
  	PCC_add_avp(msg,data.s,data.len,AVP_IMS_Media_Component_Description,
 				AAA_AVP_FLAG_MANDATORY|AAA_AVP_FLAG_VENDOR_SPECIFIC,
 				IMS_vendor_id_3GPP,
 				AVP_FREE_DATA,
 				__FUNCTION__);
 	
 	LOG(L_DBG,"PCC_add_media_component_description() : about to end\n");		
 	
 	return 1;
 }

/*
 * Parses the sdp payload and extract information like ip addresses and ports
 * @param sdpA - the SDP body of the INVITE
 * @param sdpB - the SDP body of the 200 OK answer for the INVITE
 * @param number - the number of the media component to use (which m= line?)
 * @param tag - 0 originating side 1 terminating side 
 * @param mediaLine - the "a=" line found in the sdp payload
 * @param atributes - type of media returned from the parsing of the "a=" line
 * @param port - the number od ports, to be used by the calling function
 * @param ipA, ipB, intportA, intportB - the ip addresses and ports of the caller and called party
 * returns 0 on "do nothing", 1 on success, -1 on error
 */

inline int pcc_get_ip_port_dialog(str sdpA,str sdpB, int number, int tag, 
					char ** mediaLine, int * atributes, int *ports,
					str * ipA, unsigned int * intportA,
					str * ipB, unsigned int * intportB){
	
	char *mlineA, *mlineB,*clineA,*clineB;
	char *rtp;
	str portA = {0,0};
	str portB = {0,0};
	int ret= -1;

 	if (!extract_mclines(sdpA,sdpB,&mlineA,&clineA,&mlineB,&clineB,number)){

 		LOG(L_ERR,"ERR"M_NAME":PCC_create_add_media_subcomp_dialog: error extracting lines of sdp content\n");
 		return -1;
 	}
 		   
	/*a= lines are also needed*/
	/*atributes: 	1 means sendonly*/
 	/*		2 means recvonly*/
 	/*		0 is no a= line or sendrecv*/
 	if (tag==0) {
 	 	*atributes=check_atributes(sdpA,mlineA);

 	} else if (sdpB.len) {
 	 	*atributes=check_atributes(sdpB,mlineB);
 	}
 		 
 	  		 
	if (!extract_token(mlineA,&portA,PCC_MAX_Char,2)){

 		goto error; /*problem extracting port*/
 	}
 	if (sdpB.len && !extract_token(mlineB,&portB,PCC_MAX_Char,2)){

 		goto error; /* either no ' ' or PCC_MAX_Char too small*/
 	}
 		
 	/*check if this are ports or multiports!*/
 		
 	if(!is_a_port(portA) || (sdpB.len && !is_a_port(portB))){
		goto error; /* there was a word there but it isn't a port*/
	}
 		 		 		
 	if (strncmp(portA.s,"0",1)!=0 && (sdpB.len && strncmp(portB.s,"0",1)==0)){
 			/*this means answerer rejected the offer*/
 			
		ret = 0; goto end;/*this is more logical*/
	}
 		
 	/*the next lines have nothing to do with rtp
 	 * i just reused the pointer*/	
 	rtp=strchr(portA.s,'/');
 	if (rtp!=NULL){
 		sscanf(portA.s,"%*i/%i%*s",ports);
 		*rtp='\0'; 
 	}	
 		

 	if(!extract_token(clineA,ipA,PCC_MAX_Char,3)){
		goto end;
	}
	if(ipA->s[0]=='['){
		ipA->s = ipA->s+1;
		ipA->len = ipA->len-1;
	}
	if(ipA->s[ipA->len-1] == ']'){
		ipA->len = ipA->len-1;
	}

	if(sdpB.len && !extract_token(clineB,ipB,PCC_MAX_Char,3)){
		goto end;
	}
	if(ipB->s[0]=='['){
		ipB->s = ipB->s+1;
		ipB->len = ipB->len-1;
	}
	if(ipB->s[ipB->len-1] == ']'){
		ipB->len = ipB->len-1;
	}
 		
  	/* i is the flow number */
 	/*flows is the number of data flows .. for each port 1 data flow*/
	sscanf(portA.s,"%u",intportA);
	sscanf(portB.s,"%u",intportB);
	*mediaLine = mlineA;
	ret = 1;
error:
end:
	if(portA.s) pkg_free(portA.s);	portA.s = 0;
	if(portB.s) pkg_free(portB.s);	portB.s = 0;
	return ret;

}

/* Creates and adds Media-Sub-Components to AVP_LIST
 * @param list - pointer to the AVP_LIST
 * @param sdpA - the SDP body of the INVITE
 * @param sdpB - the SDP body of the 200 OK answer for the INVITE
 * @param number - the number of the media component to use (which m= line?)
 * @param media_sub_component -  array of PCC_Media_Sub_Components elements of type *AAA_AVP that were added
 * @param tag - 0 originating side 1 terminating side 
 * returns the number of media_sub_components added on success -1 on error
 * the media_sub_component is given in order to free the AVPS after grouping them!
 * the transport protocol will be considered : 
 * UDP if RTP/AVP, TCP if TCP (RFC 4145) or TCP/MSRP (RFC 4975), or IP if neither
 */
inline int PCC_create_add_media_subcomp_dialog(AAA_AVP_LIST *list,str sdpA,str sdpB,int number,AAA_AVP **media_sub_component,int tag)
 {

 	char * newline=0, *rtp=0, *mlineA=0, *tcp = NULL;
 	unsigned int intportA=0,intportB=0;
	str ipA = {0,0}, ipB = {0,0};
 	int i=0,flows=0;
 	int atributes=0; /* a= lines present?*/
 	int ports=1; /*how many ports does this m line define?*/
	int ret;
	char* proto;	

 	LOG(L_DBG,"DBG"M_NAME":PCC_create_add_media_subcomp_dialog : starting\n");
	
	ret = pcc_get_ip_port_dialog(sdpA, sdpB, number, tag, &mlineA, &atributes, &ports,
					&ipA, &intportA, &ipB, &intportB);
	if(ret!=1) return ret;
	
	proto=ip_s;
	if(pcc_use_protocol){
		rtp=strstr(mlineA,"RTP");
		newline=index(mlineA,'\n');
		if (newline==NULL) newline=index(mlineA,'\0');
		if(rtp!=NULL && rtp<newline) proto = udp_s;
		else if((tcp = strstr(mlineA, "TCP"))!= NULL && tcp<newline)
			proto = tcp_s;
	}
	
	while(flows<ports && i+2<PCC_Media_Sub_Components){
 		i++;
		if (tag!=1){
			media_sub_component[i-1]=PCC_create_media_subcomponent(i,proto, ipA, intportA, 
											ipB, intportB, "",atributes);
			cdpb.AAAAddAVPToList(list,media_sub_component[i-1]);		
		} else {
 		
 			media_sub_component[i-1]=PCC_create_media_subcomponent(i,proto,ipB, intportB,
											ipA, intportA,"",atributes);
 			cdpb.AAAAddAVPToList(list,media_sub_component[i-1]);		
 		}
 		flows++;

 		if (1){

			if (rtp!=NULL && rtp < newline){
				i++;
		 		intportA++; 
		 		intportB++;
		 		if (tag!=1){
		 			media_sub_component[i-1]=PCC_create_media_subcomponent(i,proto,ipA,intportA,
												ipB,intportB,"",3);
	 				cdpb.AAAAddAVPToList(list,media_sub_component[i-1]);	
	 			} else {
	 				media_sub_component[i-1]=PCC_create_media_subcomponent(i,proto,ipB,intportB,
												ipA,intportA,"",3);
		 			cdpb.AAAAddAVPToList(list,media_sub_component[i-1]);	
		 		}		 		
		 	}
	 			
			intportA++;  
	 		intportB++;

	 		/*if its not an RTP flow and it has multiports then the odd ports
	 		 * are used for the next component .. if it is RTP and multiports than 
	 		 * the even ports are for the next component because the odd are used for 
	 		 * RTCP flows*/
 		}						
 											
 	}
 	LOG(L_DBG,"DBG"M_NAME":PCC_create_add_media_subcomp_dialog : ending\n");	
 	  	
 	return (i);
 }
/**
 * Creates a media-sub-component AVP
 * 
 * TODO - fix this ... or just delete it and do it again! It adds 2x Flow-Description for example, as a bug!
 * I don't think that more than 1 can be in one Media Subcomponent.
 * adapted to support also RFC 4145 (http://tools.ietf.org/html/rfc4145) special port number 9 -- the peer will initiate the connection
 * 
 * @param number - the flow number
 * @param proto - the protocol of the IPFilterRule
 * @param ipA - ip of the INVITE  (if creating rule for UE that sent INVITE)
 * @param portA - port of the INVITE (if creating rule for UE that sent INVITE)
 * @param ipB - ip of 200 OK (if creating rule for UE that sent INVITE)
 * @param portB - port of 200 OK (if creating rule for UE that sent INVITE)
 * @param options - any options to append to the IPFilterRules
 * @param atributes - indication of atributes 
 * 						0 no atributes , 1 sendonly , 2 recvonly , 3 RTCP flows, 4 AF signaling flows
 * @param bwUL - bandwidth uplink
 * @param bwDL - bandiwdth downlink
 */ 

static str permit_out = {"permit out ", 11};
static str permit_in = {"permit in ", 10};
static str from_s = {" from ", 6};
static str to_s = {" to ", 4};
static char * permit_out_with_ports =    "permit out %s from %.*s %u to %.*s %u %s";
static char * permit_out_without_ports = "permit out %s from %.*s to %.*s %s";
static char * permit_in_with_ports = "permit in %s from %.*s %u to %.*s %u %s";
static char * permit_in_without_ports = "permit in %s from %.*s to %.*s %s";

 AAA_AVP *PCC_create_media_subcomponent(int number, char* proto, 
		 			str ipA, unsigned int intportA, 
					str ipB, unsigned int intportB ,
					char *options,int atributes)
 {
 
 		str data;
 		int len,len2;
 		str flow_data = {0,0};
 		str flow_data2 = {0,0};
 		AAA_AVP *flow_description1=0,*flow_description2=0,*flow_number=0;
 		AAA_AVP *flow_usage=0;
 		
 		AAA_AVP_LIST list;
 		list.tail=0;
 		list.head=0;
 		char x[4];
		int portAlen=0, portBlen=0;
		
		if(pcc_use_ports){
			if(intportA == 9)	portAlen = 0;
			else	int2str(intportA, &portAlen);
			if(intportB == 9)	portBlen = 0;
			else	int2str(intportB, &portBlen);
			len = (permit_out.len + from_s.len + to_s.len+ipB.len+ipA.len+4+
				strlen(proto)+portAlen + portBlen+ strlen(options))*sizeof(char);
		}else{
			len = (permit_out.len + from_s.len + to_s.len+ipB.len+ipA.len+2+
				strlen(proto)+portAlen + portBlen+ strlen(options))*sizeof(char);
		}

		flow_data.s = (char*)pkg_malloc(len); 
		if(!flow_data.s){
			LOG(L_ERR, "ERR:"M_NAME":PCC_create_media_component: out of memory \
					when allocating %i bytes in pkg\n", len);			
			return NULL;
		}

		len2 = len - (permit_out.len -permit_in.len)*sizeof(char);
		flow_data2.s = (char*)pkg_malloc(len2);
	        if(!flow_data2.s){
			LOG(L_ERR, "ERR:"M_NAME":PCC_create_media_component: out of memory \
					when allocating %i bytes in pkg\n", len);			
			pkg_free(flow_data.s); flow_data.s = 0;
			return NULL;
		}		

 		set_4bytes(x, number);
 				
 		flow_number=cdpb.AAACreateAVP(AVP_IMS_Flow_Number,
 											AAA_AVP_FLAG_MANDATORY|AAA_AVP_FLAG_VENDOR_SPECIFIC,
 											IMS_vendor_id_3GPP,x,4,
 											AVP_DUPLICATE_DATA);
		cdpb.AAAAddAVPToList(&list,flow_number);
		/*first flow is the recieve flow*/
		//snprintf: The trailing nul character is counted towards the size limit, so you must allocate at least size characters for str.	

		if (atributes==0 || atributes==1 || atributes==3 || atributes==4)
                {
                        /*second flow is the send flow*/
                        if(pcc_use_ports){
                                if(intportB ==9){
                                        flow_data.len=snprintf(flow_data.s, len, "permit in %s from %.*s %u to %.*s %s",
                                                                                                proto,
                                                                                                ipA.len, ipA.s, intportA,
                                                                                                ipB.len, ipB.s, options);
                                }else if(intportA ==9 ){
                                        flow_data.len=snprintf(flow_data.s, len, "permit in %s from %.*s to %.*s %u %s",
                                                                                                proto,
                                                                                                ipA.len, ipA.s,
                                                                                                ipB.len, ipB.s, intportB, options);
                                }else{

                                        flow_data2.len=snprintf(flow_data2.s, len2, permit_in_with_ports,proto,
                                                        ipA.len, ipA.s, intportA,
                                                        ipB.len, ipB.s, intportB, options);
                                }
                        }else{
                                flow_data2.len=snprintf(flow_data2.s, len2, permit_in_without_ports,proto,
                                        ipA.len, ipA.s, ipB.len, ipB.s, options);
                        }
                        flow_data2.len = strlen(flow_data2.s);
                        LOG(L_DBG, "DBG:"M_NAME":PCC_create_media_component: second flow is %.*s\n", flow_data2.len, flow_data2.s);
                        flow_description2=cdpb.AAACreateAVP(AVP_IMS_Flow_Description,
                                                                                        AAA_AVP_FLAG_MANDATORY|AAA_AVP_FLAG_VENDOR_SPECIFIC,
                                                                                        IMS_vendor_id_3GPP, flow_data2.s, flow_data2.len,
                                                                                        AVP_DUPLICATE_DATA);
                        cdpb.AAAAddAVPToList(&list,flow_description2);
                }
		if (atributes==0 || atributes==2 || atributes==3 || atributes==4)
		{
			if(pcc_use_ports){
				if(intportA ==9){
					flow_data.len=snprintf(flow_data.s, len, "permit out %s from %.*s %u to %.*s %s",
										proto,
										ipB.len, ipB.s, intportB,
										ipA.len, ipA.s, options);
				}else if(intportB ==9 ){
					flow_data.len=snprintf(flow_data.s, len, "permit out %s from %.*s to %.*s %u %s",
														proto,
														ipB.len, ipB.s,
														ipA.len, ipA.s, intportA, options);
				}else{
					flow_data.len=snprintf(flow_data.s, len, permit_out_with_ports,proto,
							ipB.len, ipB.s, intportB,
							ipA.len, ipA.s, intportA, options);
				}
			}else{
				flow_data.len=snprintf(flow_data.s, len, permit_out_without_ports,proto,
					ipB.len, ipB.s, ipA.len, ipA.s, options); 
			}
			flow_data.len = strlen(flow_data.s);
			LOG(L_DBG, "DBG:"M_NAME":PCC_create_media_component: first flow is %.*s\n", flow_data.len, flow_data.s);
 			flow_description1=cdpb.AAACreateAVP(AVP_IMS_Flow_Description,
 											AAA_AVP_FLAG_MANDATORY|AAA_AVP_FLAG_VENDOR_SPECIFIC,
 											IMS_vendor_id_3GPP, flow_data.s, flow_data.len,
 											AVP_DUPLICATE_DATA);
 			cdpb.AAAAddAVPToList(&list,flow_description1);
		} 
		
 		if (atributes==3)
		{
			set_4bytes(x,AVP_EPC_Flow_Usage_Rtcp);
			flow_usage=cdpb.AAACreateAVP(AVP_EPC_Flow_Usage,
											AAA_AVP_FLAG_MANDATORY|AAA_AVP_FLAG_VENDOR_SPECIFIC,
											IMS_vendor_id_3GPP,x,4,
											AVP_DUPLICATE_DATA);
			cdpb.AAAAddAVPToList(&list,flow_usage);
		} 											
 		if (atributes==4)
 				{
 					set_4bytes(x,AVP_EPC_Flow_Usage_AF_Signaling);
 					flow_usage=cdpb.AAACreateAVP(AVP_IMS_Flow_Usage,
 													AAA_AVP_FLAG_MANDATORY|AAA_AVP_FLAG_VENDOR_SPECIFIC,
 													IMS_vendor_id_3GPP,x,4,
 													AVP_DUPLICATE_DATA);
 					cdpb.AAAAddAVPToList(&list,flow_usage);
 				}
 		
 		
 		
 
 		 	
 		 /*group all AVPS into one big.. and then free the small ones*/	
 		 		
 		data=cdpb.AAAGroupAVPS(list);
 		
 		
 		cdpb.AAAFreeAVPList(&list);
		pkg_free(flow_data.s); flow_data.s =0;
		pkg_free(flow_data2.s); flow_data2.s = 0;

 		return (cdpb.AAACreateAVP(AVP_IMS_Media_Sub_Component,
 											AAA_AVP_FLAG_MANDATORY|AAA_AVP_FLAG_VENDOR_SPECIFIC,
 											IMS_vendor_id_3GPP,data.s,data.len,
											AVP_FREE_DATA));
 }

/*
 * Creates a Codec-Data AVP as defined in TS29214 (Rx interface)
 * @param sdp - sdp body of message
 * @param number - the number of the m= line being used
 * @param direction - 0 means uplink offer 1 means uplink answer ,
 * 	2 means downlink offer , 3 downlink answer
 * returns NULL on failure or the pointer to the AAA_AVP on success
 * (this AVP should be freed!)
*/

AAA_AVP* PCC_create_codec_data(str sdp,int number,int direction)
{
		char data[PCC_MAX_Char4];
		char *p,*q,*r;
		int i=1,l;
		
		switch(direction) {
			
			case 0: sprintf(data,"uplink\noffer\n");	
					break;
			case 1: sprintf(data,"uplink\nanswer\n");
					break;
			case 2: sprintf(data,"downlink\noffer\n");
					break;
			case 3: sprintf(data,"downlink\nanswer\n");
					break;
			default: 
					break;
						
		}
		l=strlen(data);
		
		p=find_sdp_line(sdp.s,(sdp.s+sdp.len),'m');
		
		while (p!= NULL && i<number)
		{
			p=find_next_sdp_line(p,(sdp.s+sdp.len),'m',NULL);
			i++;
		} 
		if (p==NULL)
		{
			return NULL;
		}
		q=find_next_sdp_line(p,(sdp.s+sdp.len),'m',(sdp.s+sdp.len));
		
		
		r=index(p,'\n');
		if (!r) r=index(p,'\0');
		memcpy(data+l,p,r-p+1);
		l+=r-p+1;
		/*the m= line is always copied*/
		
		
		
		
		p=r+1; /* p is always the start of the line*/
		r=index(p,'\n'); /*r always the \n char of that line*/
		if (!r) r=index(p,'\0');
		while(p+2<q && (l+r-p+1)<PCC_MAX_Char4)
			{
				/*what about spaces? think it closely*/
				if ((strstr(p,"a=sendonly")!=NULL && strstr(p,"a=sendonly")<r) || 
						(strstr(p,"a=recvonly")!=NULL && strstr(p,"a=recvonly")<r) ||
						(strstr(p,"a=sendrecv")!=NULL && strstr(p,"a=sendrecv")<r) ||
						strncmp(p,"b=RS",4)==0 || strncmp(p,"b=RR",4)==0 || strncmp(p,"b=AS",4)==0)
				{
							p=r+1; 		/*skip this line*/
							r=index(p,'\n');
							if (!r) r=index(p,'\0');
						
				} else {
						memcpy(data+l,p,r-p+1);
						l+=r-p+1;
						p=r+1;
						r=index(p,'\n');
						if (!r) r=index(p,'\0');
				}
			}
		data[l-2]='\0';
			
			
		l=strlen(data);
		return (cdpb.AAACreateAVP(AVP_IMS_Codec_Data,
 											AAA_AVP_FLAG_MANDATORY|AAA_AVP_FLAG_VENDOR_SPECIFIC,
 											IMS_vendor_id_3GPP,data,l,
 											AVP_DUPLICATE_DATA));
 											
}




/**Extracts the m= and c= lines for the two SDP bodys of the SDP exchange
 * @param sdpA - body of the INVITE
 * @param sdpB - body of the 200 OK
 * @param mlineA - pointer to beginning of m= in sdpA 
 * @param clineA - pointer to beginning of matching c= in sdpA
 * @param mlineB - pointer to beginning of m= in sdpB
 * @param clineB - pointer to beginning of matching c= in sdpB
 * @param number - the ordinal number of the m= to use in case of more than one
 * returns 1 on success 0 on error
 */
 
 /*
  * If the clients are not following the offer-answer thing we die after this
  * because i keep searching for m lines that arent there
 */
 
int extract_mclines(str sdpA,str sdpB,char **mlineA,char **clineA,char **mlineB,char **clineB,int number)
{
 	
 	char *nclineA=NULL,*nclineB=NULL; /*next*/
 	char *sclineA=NULL,*sclineB=NULL; /*session*/
 	char *nmlineA=NULL,*nmlineB= NULL; /*next*/
 	int i;
 	
 	 		
 	
 	
 	*clineA=find_sdp_line(sdpA.s,(sdpA.s+sdpA.len),'c');
 	*clineB=find_sdp_line(sdpB.s,(sdpB.s+sdpB.len),'c');
 	*mlineA=find_sdp_line(sdpA.s,(sdpA.s+sdpA.len),'m');
 	*mlineB=find_sdp_line(sdpB.s,(sdpB.s+sdpB.len),'m');
	
	
 	
	if (*clineA==NULL || *mlineA==NULL )
 	{
 		/*missing at least one cline and mline in each SDPbody*/
 		LOG(L_ERR,"ERR:"M_NAME" Malformed SDP body\n");
 		return 0;
 	} 	else {

 		nclineA=find_next_sdp_line(*clineA,(sdpA.s+sdpA.len),'c',NULL);
 		nmlineA=find_next_sdp_line(*mlineA,(sdpA.s+sdpA.len),'m',NULL);
 		if (*clineA < *mlineA)
 		 	{
 		 		sclineA=*clineA;
 		 	}
 	}

	if (sdpB.len)
	{

		nclineB=find_next_sdp_line(*clineB,(sdpB.s+sdpB.len),'c',NULL);
		nmlineB=find_next_sdp_line(*mlineB,(sdpB.s+sdpB.len),'m',NULL);

		if (*clineB < *mlineB)
		{
			sclineB=*clineB;
		}
 	}
 	
 	
 	
 	if (number > 1)
 	{
 		for (i=1;i<number;i++)
 		{
 			*mlineA=nmlineA;
 			nmlineA=find_next_sdp_line(*mlineA,(sdpA.s+sdpA.len),'m',NULL);
 			

 			if(nclineA >*mlineA && (nclineA < nmlineA || nmlineA== NULL))
 			{
 				// if there is a c line between two m lines or after the last m line
 				// then this c line belongs to the first one 
 			 		*clineA=nclineA;
 			 		nclineA=find_next_sdp_line(*clineA,(sdpA.s+sdpA.len),'c',NULL);
 			} else {
 				// if not then the session description one is the one 
 				*clineA=sclineA;
 			}

 			if (sdpB.len)
 			{
 				*mlineB=nmlineB;
 				nmlineB=find_next_sdp_line(*mlineB,(sdpB.s+sdpB.len),'m',NULL);

				if(nclineB >*mlineB && (nclineB < nmlineB || nmlineB== NULL))
				{
					// if there is a c line between two m lines or after the last m line
					// then this c line belongs to the first one
						*clineB=nclineB;
						nclineB=find_next_sdp_line(*clineB,(sdpB.s+sdpB.len),'c',NULL);
				} else {
					// if not then the session description one is the one
					*clineB=sclineB;
				}
 			}
 		
 		
			if (*mlineA == NULL  || *clineA == NULL)
			{
				LOG(L_ERR,"ERR:"M_NAME":%s: Failed getting m= and c= lines in SDP\n","extract_mclines");
				return 0;
			}
 					
 		}
 		// after this we should have mlineA , mlineB , clineA, clineB
 		// with the right values	
 		
 	 }
 	 
 	
 	 return 1;
}

/*
 * Extract the token
 * @param line - pointer to line in SDP body
 * @param token - pointer to buffer to copy the token to
 * @param max - size of the buffer
 * @number - which token .. separated by ' ' or ' ' and then '\n'
 * returns 1 on success, 0 on error
 */
 
  
int extract_token(char *line, str *token,int max,int number)
{
	char *p,*q,*r;
	int i;
	
	p=line;
	for(i=1; i<number;i++)
	{
		p=index(p,' ');
		if (p==NULL)
			return 0;
		while (*p==' ')
		{
			p++;
		}
		
	}
	q=index(p,' ');
	r=index(p,'\n');
	if (!r) r=index(p,'\0');
	if (!q||r<q) q = r;
	if (!q) return 0;
	while isspace(*(q-1)) q--;
	if (q-p<max) 
	{
		token->s = (char*)pkg_malloc(q-p+1);
		if(!token->s){
			LOG(L_ERR, "ERR:"M_NAME":extract_token: out_of_memory \
					when allocating %i bytes\n", q-p);
			return 0;
		}
		memcpy(token->s,p,q-p);
		token->len = q-p;
		token->s[token->len] = '\0';
		return 1;
	} else {
		return 0;
	}
	
}

/*
 * Extract the data from b=  lines and add it to the structure
 * @param bw - the pointer to the bandwidth structure to fill
 * @param sdp - the sdp body where to look for b= lines 
 * @param start - the pointer to the starting point to look at
 
 * returns 1 on success 0 if no b= line was found
 */
int extract_bandwidth(bandwidth *bw,str sdp,char *start)
{
	char *b;
	char *m; /*next mline*/
	
	bw->bAS=0; bw->bRS=0; bw->bRR=0;	
	
	b=find_next_sdp_line(start,(sdp.s+sdp.len),'b',NULL);
	if (!b) return 0;
	m=find_next_sdp_line(b,(sdp.s+sdp.len),'m',NULL); // we only do this once!!!
	
	while (b!=NULL) {
				
		
		if (m!=NULL && b>m)
		{
		/*this bandwidth belongs to some other media!*/
			return 0;
		}
		b+=2; /*skip b and =*/
		while (*b==' ')	
		{
			b++;		
		}
		if (*b=='A' && *(b+1)=='S')
		{
			sscanf(b,"AS:%i%*s",&bw->bAS);
					
		} else if( *b=='R') {
			if (*(b+1)=='S') 
			{
				sscanf(b,"RS:%i%*s",&bw->bRS);
				
			} else if (*(b+1)=='R')	{
				sscanf(b,"RR:%i%*s",&bw->bRR);
				
			}
		} 
		/*find next b line*/
		b=find_next_sdp_line(b,(sdp.s+sdp.len),'b',NULL);
			
	}
		
		return 1;
}

/* Gets the identification and the type 
 * @param r - the sip request or reply
 * @param tag - the tag
 * @param identification - pointer to str to fill (no memory allocation needed)
 * returns the type of identificator according to the definition of RFC 4006 or -1 on error
*/

/*
 * TODO - fix this horrible hack...
 * could return something like 
 * 
 * <tel:+1-234-1234-2134>  and 0
 * <sip:elbueno@estatriste.com> and 3
 */

int extract_id(struct sip_msg *r,int tag,str *identification)
{

	char *f;
	int len,i=0;
	
	identification->s=0;
	identification->len=0;
	if (!tag) {
		f=r->from->body.s;
		len=r->from->body.len;
	} else {
		f=r->to->body.s;
		len=r->to->body.len;
	}
	while (i<len) {
		switch (f[i]) {
			case '<' :
				identification->s=f+i+1;
				i++;
				break;
			case '>' :
				if (identification->s) {
				 	identification->len=(f+i)-identification->s;
				 	i=len; // just to get out of here
					break;
				 } else {
				 	LOG(L_ERR,"extract_id return -1\n");
				 	return -1;
				 }
			default :
				i++;
		}
	}

	if (!identification->s || !identification->len)
	{
		LOG(L_ERR,"extract id returns -1\n");
		return -1;
	}
	
	if (*(identification->s+1)=='t' && *(identification->s+2)=='e' && *(identification->s+3)=='l')
	{
		return AVP_Subscription_Id_Type_E164;
	} else {
		return AVP_Subscription_Id_Type_SIP_URI;
	}
	
}






/* Check for sendonly or recvonly modifiers in a= lines
 * @param sdpbody - SDP body
 * @param mline - pointer to beginning of m= line
 * returns  0 if no modifier was found,
 *  -1 on error (malformed SDP body)
 * 1 if sendonly , 2 if recvonly
 * */
int check_atributes(str sdpbody,char *mline) 
{
	char *p,*q;
	int s=0;
	
	p=find_sdp_line(sdpbody.s,sdpbody.s+sdpbody.len,'a');
	q=find_sdp_line(sdpbody.s,sdpbody.s+sdpbody.len,'m');
	
	
	if (p==NULL) 
	{
		return 0;
	}
	
	/*see if there is a sessionwide  a= line*/
	if (p<q)
	{
		p+=2; /*skip a=*/
		while(*p==' ')
		{
			p++;
		}
		if(strncmp(p,"sendonly",8)==0)
		{
			s=1;
		} else if(strncmp(p,"recvonly",8)==0)
		{
			s=2;
		}
		
		
	}
	/*see if there is a mediawide a= line*/
	
	p=find_sdp_line(mline,(sdpbody.s+sdpbody.len),'a');
	if (p==NULL)
	{
		return s;
	}
	mline++;
	q=find_next_sdp_line(mline,(sdpbody.s+sdpbody.len),'m',NULL);
	
	if (q!=NULL && p>q) 
	{		
			return s; /*the a= line found belongs to some other m= line*/	
	}
	
	p+=2; /*skip a=*/
	while(*p==' ')
	{
		p++;
	}
	if(strncmp(p,"sendonly",8)==0)
	{
		return 1;
	} else if(strncmp(p,"recvonly",8)==0)
	{
		return 2;
	} else {
		return s;
	}
			
}

/*
 * Check if this token extracted is a port or multiport 
 * @param port - the string to check
 * returns 0 if this is not a port  1 if it is a port or 2 if its a multiport
*/
int is_a_port(str port) {
	int i,multiport=0;
	for (i=0;i< port.len; i++)
	{
		switch(port.s[i]) {
			case '1':
			case '2':
			case '3':
			case '4':
			case '5':
			case '6':
			case '7':
			case '8':
			case '9':
			case '0':
				continue;
			
			case '/':
				if (multiport==0 && i>0)
				{
					multiport=1;
					continue;
			
				} else {
					return 0;
			
				}
			default :
				return 0;
			
		}	
	}
	if (!multiport) return 1;
	else return 2;
}
/*
 * Check if its an address
 * @param ad  - the pointer to the array of characters representing the address
 * returns 1 if it is an address 0 if not
 * for now any amount of numbers with . or : between them is ok 
*/
/*
int is_an_address(char *ad)
{
	int i,last=0,ip4=0;
	for (i=0;i<strlen(ad);i++)
	{
		switch(ad[i]) {
			case '1':
			case '2':
			case '3':
			case '4':
			case '5':
			case '6':
			case '7':
			case '8':
			case '9':
			case '0':
				last=0;
				continue;
			case '.':
				if (!last)
					{
						ip4=1;
						last=1;
						continue;
					} else {
						return 0;
					}
			case ':':
					if (ip4)
					{
						return 0;
					}
					continue;
			default :
					return 0;
		}
	}
	return 1;
}*/		


#define macro_name(_rc)	#_rc
/**
 * Returns the Result-Code AVP from a Diameter message.
 * or the Experimental-Result-Code if there is no Result-Code , because .. who cares
 * @param msg - the Diameter message
 * @returns 1 if result code found or 0 if error
 */
inline int PCC_get_result_code(AAAMessage *msg, unsigned int *data)
{

	AAA_AVP *avp;
	AAA_AVP_LIST list;
	list.head=0;
	list.tail=0;
	*data=0;
	int ret = 0;
	
	for (avp=msg->avpList.tail;avp;avp=avp->prev)
	{
		//LOG(L_INFO,"pcc_get_result_code: looping with avp code %i\n",avp->code);
		if (avp->code==AVP_Result_Code)
		{
			*data = get_4bytes(avp->data.s);
			ret = 1;	
				
		} else if (avp->code==AVP_Experimental_Result)
		{
			list=cdpb.AAAUngroupAVPS(avp->data);
			for(avp=list.head;avp;avp=avp->next)
			{
				//LOG(L_CRIT,"in the loop with avp code %i\n",avp->code);
				if (avp->code==AVP_IMS_Experimental_Result_Code)
				{
					*data = get_4bytes(avp->data.s);
					cdpb.AAAFreeAVPList(&list);
					ret = 1;				
					break;
				}
			}
			cdpb.AAAFreeAVPList(&list);
			break; // this has to be here because i have changed the avp!!!
					
		}
	
	}
	switch((*data)){
		case DIAMETER_SUCCESS:
			LOG(L_DBG,"DBG:"M_NAME":PCC_get_result_code: AAA result %s\n", 
				macro_name(DIAMETER_SUCCESS)); break;

		case RC_IMS_DIAMETER_ERROR_INVALID_SERVICE_INFORMATION:
			LOG(L_DBG,"DBG:"M_NAME":PCC_get_result_code: AAA result %s\n", 
				macro_name(RC_IMS_DIAMETER_ERROR_INVALID_SERVICE_INFORMATION));break;

		case RC_IMS_DIAMETER_ERROR_FILTER_RESTRICTIONS:
			LOG(L_DBG,"DBG:"M_NAME":PCC_get_result_code: AAA result %s\n", 
				macro_name(RC_IMS_DIAMETER_ERROR_FILTER_RESTRICTIONS));break;

		case RC_IMS_DIAMETER_ERROR_REQUESTED_SERVICE_NOT_AUTHORIZED:
			LOG(L_DBG,"DBG:"M_NAME":PCC_get_result_code: AAA result %s\n", 
				macro_name(RC_IMS_DIAMETER_ERROR_REQUESTED_SERVICE_NOT_AUTHORIZED)); break;

		case RC_IMS_DIAMETER_ERROR_DUPLICATED_AF_SESSION:
			LOG(L_DBG,"DBG:"M_NAME":PCC_get_result_code: AAA result %s\n", 
				macro_name(RC_IMS_DIAMETER_ERROR_DUPLICATED_AF_SESSION)); break;

		case RC_IMS_DIAMETER_ERROR_IPCAN_SESSION_NOT_AVAILABLE:
			LOG(L_DBG,"DBG:"M_NAME":PCC_get_result_code: AAA result %s\n", 
				macro_name(RC_IMS_DIAMETER_ERROR_IPCAN_SESSION_NOT_AVAILABLE)); break;

		case RC_IMS_DIAMETER_ERROR_UNAUTHORIZED_NON_EMERGENCY_SESSION:
			LOG(L_DBG,"DBG:"M_NAME":PCC_get_result_code: AAA result %s\n", 
				macro_name(RC_IMS_DIAMETER_ERROR_UNAUTHORIZED_NON_EMERGENCY_SESSION));break;
	}

	return ret;
}


