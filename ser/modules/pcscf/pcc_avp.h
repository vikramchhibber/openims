/**
 * $Id: pcc_avp.h 1089 2011-05-06 16:43:08Z aon $
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
 * \author Alberto Diez Albaladejo -at- fokus dot fraunhofer dot de
 */ 
 
#ifndef __PCC_AVP_H
#define __PCC_AVP_H

#include <arpa/inet.h>
#include "../../sr_module.h"
#include "mod.h"
#include "../cdp/cdp_load.h"
#include "../cdp_avp/mod_export.h"
#include "sdp_util.h"



#define PCC_MAX_Char 64
#define PCC_MAX_Char4 256
/* Maximum Number of characters to represent some AVP datas*/
/*and ipv6 addresses in character*/
#define PCC_Media_Sub_Components 10

#include <string.h>
#include <stdio.h>

/**
 * Convert an ip_address to the str text representation
 * @param src - the source ip address
 * @param dst - where to write to
 * @param in_shm - whether to allocate memory for the result in shm or pkg
 * @return the number of bytes in the text, or 0 on error
 */
#define ip_address_to_str(src, dst, buf, mem)\
do{\
	(dst)->s = 0;	(dst)->len = 0;   \
	switch ((src)->ai_family){        \
	case AF_INET:			\
		if (!inet_ntop((src)->ai_family,&((src)->ip.v4),buf,64)) goto error; \
		break;								\
	case AF_INET6:								\
		if (!inet_ntop((src)->ai_family,&((src)->ip.v6),buf,64)) goto error; \
		break;									\
	default:									\
		goto error;								\
	}										\
	(dst)->s = mem##_malloc(strlen(buf)+1);						\
	if(!(dst)->s)	goto out_of_memory;						\
	memcpy((dst)->s, buf, strlen(buf)*sizeof(char));				\
	(dst)->len = strlen(buf);							\
}while(0);

/**
 * Convert a str text representation of an IPv4/6 address to the ip_address structure
 * @param src - string of the address 
 * @param dst - where to write the result
 * @return - AF_INET/AF_INET6 or 0 on error
 */
#define str_to_ip_address(src, dst)\
do{					\
	char buf[64];			\
	bzero((dst),sizeof(ip_address));	\
	if (!(src).len) goto error;		\
						\
	sprintf(buf,"%.*s",(src).len,(src).s);	\
						\
	if (inet_pton(AF_INET6,buf,(void*)&((dst)->ip.v6))>0)	\
		(dst)->ai_family = AF_INET6;			\
	else if (inet_pton(AF_INET,buf,&((dst)->ip.v4))>0) 	\
		(dst)->ai_family = AF_INET;			\
}while(0);



/** NO DATA WILL BE DUPLICATED OR FREED - DO THAT AFTER SENDING THE MESSAGE!!! */

typedef struct _bandwidth {
		int bAS;
		int bRS;
		int bRR;		
} bandwidth;

/*helper*/
uint16_t pcc_get_ip_port(struct sip_msg *r, struct sip_uri * parsed_aor, str *ip, unsigned short * port);
int PCC_add_framed_ip_avp(AAA_AVP_LIST * list, str ip, uint16_t version);
/*just headers*/

int PCC_add_avp(AAAMessage *m,char *d,int len,int avp_code,
	int flags,int vendorid,int data_do,const char *func);

int PCC_add_vendor_specific_application_id_group(AAAMessage *msg, unsigned int vendorid, unsigned int auth_app_id);
int PCC_add_destination_realm(AAAMessage *msg, str data);
inline int PCC_add_subscription_ID(AAAMessage *msg,struct sip_msg *r,int tag, int * subscr_type, str * subscr_value);
AAA_AVP *PCC_create_media_subcomponent(int number, char *proto, 
					str ipA, unsigned int intportA, 
					str ipB, unsigned int intportB ,
					char *options,int atributes);
inline int PCC_create_add_media_subcomp_dialog(AAA_AVP_LIST *list,str sdpA,
											str sdpB,int number,AAA_AVP **media_sub_component,int tag);
int PCC_AAR_add_avps_for_register(AAAMessage *msg, struct sip_uri * parsed_uri);
int PCC_add_media_component_description_for_register(AAAMessage *msg, str ip, unsigned short port, uint16_t version);
inline int PCC_add_media_component_description(AAAMessage *msg,str sdpinvite,str sdp200,char *mline,int number,int tag);
AAA_AVP* PCC_create_codec_data(str sdp,int number,int direction);

int extract_mclines(str sdpA,str sdpB,char **mlineA,char **clineA,char **mlineB,char **clineB,int number);
int extract_token(char *line, str *token,int max,int number);
int extract_bandwidth(bandwidth *bw,str sdp,char *start);
int extract_id(struct sip_msg *r,int tag,str *identification);
int check_atributes(str sdpbody,char *mline);
int is_a_port(str port);
/*int is_an_address(char *ad);*/
inline int PCC_add_auth_application_id(AAAMessage *msg, unsigned int data);
inline int PCC_get_result_code(AAAMessage *msg, unsigned int *data);
#endif /*__PCC_AVP_H*/
