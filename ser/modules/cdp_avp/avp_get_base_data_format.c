/**
 * $Id: avp_get_base_data_format.c 1067 2011-03-31 07:17:46Z aon $
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
 * CDiameter AVP Operations modules - AVP Data Formats Decoding
 * 
 * 
 *  \author Dragos Vingarzan dragos dot vingarzan -at- fokus dot fraunhofer dot de
 * 
 */ 

#include "avp_get_base_data_format.h"

extern struct cdp_binds *cdp;

/* 
 * RFC 3588 Basic AVP Data Types
 * 
 * http://tools.ietf.org/html/rfc3588#section-4.2
 * 
 */

inline int cdp_avp_get_OctetString(AAA_AVP *avp,str *data)
{
	if (data) *data = avp->data;
	return 1;
}

inline int cdp_avp_get_Integer32(AAA_AVP *avp,int32_t *data)
{
	int32_t x=0;
	if (avp->data.len<4) {
		LOG(L_ERR,"Error decoding Integer32 from data len < 4 bytes!\n");
		return 0;
	}
	x = get_4bytes(avp->data.s);
	if (data) *data = x;
	return 1;
}

inline int cdp_avp_get_Integer64(AAA_AVP *avp,int64_t *data)
{
	int64_t x=0;
	int i;
	if (avp->data.len<8) {
		LOG(L_ERR,"Error decoding Integer64 from data len < 8 bytes!\n");
		return 0;
	}
	for(i=0;i<8;i++)
		x = (x*256) + avp->data.s[i];
	if (data) *data = x;
	return 1;	
}


inline int cdp_avp_get_Unsigned32(AAA_AVP *avp,uint32_t *data)
{
	if (avp->data.len<4) {
		LOG(L_ERR,"Error decoding Unsigned32 from data len < 4 bytes!\n");
		return 0;
	}
	if (data) *data = ntohl(*((uint32_t*)avp->data.s));
	return 1;
}


inline int cdp_avp_get_Unsigned64(AAA_AVP *avp,uint64_t *data)
{
	uint64_t x=0;
	int i;
	if (avp->data.len<8) {
		LOG(L_ERR,"Error decoding Unsigned64 from data len < 8 bytes!\n");
		return 0;
	}
	for(i=0;i<8;i++)
		x = (x*256) + avp->data.s[i];
	if (data) *data = x;
	return 1;
}


inline int cdp_avp_get_Float32(AAA_AVP *avp,float *data)
{
	float x;
	uint32_t y;
	if (avp->data.len<4) {
		LOG(L_ERR,"Error decoding Float32 from data len < 4 bytes!\n");
		return 0;
	}
	y = cdp_avp_get_Unsigned32(avp,(uint32_t*)data);//TODO - check if this is correct
	memcpy(&x,&y,sizeof(uint32_t));
	//*((uint32_t*)&x)=y;
	if (data) *data = y;
	return 1;
}


inline int cdp_avp_get_Float64(AAA_AVP *avp,double *data)
{
	double x;
	uint64_t y;
	if (avp->data.len<8) {
		LOG(L_ERR,"Error decoding Float64 from data len < 8 bytes!\n");
		return 0;
	}
	y = cdp_avp_get_Unsigned64(avp,(uint64_t*)data);//TODO - check if this is correct
	memcpy(&x,&y,sizeof(uint64_t));
	if (data) *data = y;
	return 1;
}

inline int cdp_avp_get_Grouped(AAA_AVP *avp,AAA_AVP_LIST *data)
{
	if (data) *data = cdp->AAAUngroupAVPS(avp->data);
	return 1;
}

inline void cdp_avp_free_Grouped(AAA_AVP_LIST *data)
{
	if (data) cdp->AAAFreeAVPList(data);
}

/*
 * RFC 3588 Derived AVP Data Formats
 * 
 * http://tools.ietf.org/html/rfc3588#section-4.3
 * 
 */

inline int cdp_avp_get_Address(AAA_AVP *avp,ip_address *data)
{
	ip_address x;
	bzero(&x,sizeof(ip_address));
	if (avp->data.len<6){
		LOG(L_ERR,"Error decoding Address from AVP data of length %d < 6",avp->data.len);
		goto error;
	}
	x.ai_family = (avp->data.s[0]<<8) | avp->data.s[1]; 
	switch (x.ai_family){
		case 1:
			x.ai_family = AF_INET;
			memcpy(&(x.ip.v4.s_addr), avp->data.s+2, 4);
			break;
		case 2:
			x.ai_family = AF_INET6;
			if (avp->data.len<18){
				LOG(L_ERR,"Error decoding Address (AF_INET6) from AVP data of length %d < 16",avp->data.len);
				goto error;
			}
			memcpy(x.ip.v6.s6_addr,avp->data.s+2,16);
	}
	if (data) *data = x;
	return 1;
error:
	bzero(&x,sizeof(ip_address));
	if (data) *data = x;
	return 0;
}

inline int cdp_avp_get_Time(AAA_AVP *avp,time_t *data)
{
	if (avp->data.len<4) {
		LOG(L_ERR,"Error decoding Time from data len < 4 bytes!\n");
		return 0;
	}
	if (data) *data = ntohl(*((uint32_t*)avp->data.s))-EPOCH_UNIX_TO_EPOCH_NTP;
	return 1;
}

inline int cdp_avp_get_UTF8String(AAA_AVP *avp,str *data)
{
	if (data) *data = avp->data;
	return 1;
}

inline int cdp_avp_get_DiameterIdentity(AAA_AVP *avp,str *data)
{
	if (data) *data = avp->data;
	return 1;
}


inline int cdp_avp_get_DiameterURI(AAA_AVP *avp,str *data)
{
	if (data) *data = avp->data;
	return 1;
}


inline int cdp_avp_get_Enumerated(AAA_AVP *avp,int32_t *data)
{
	int32_t x=0;
	if (!avp || avp->data.len<4) {
		LOG(L_ERR,"Error decoding Enumerated from data len < 4 bytes!\n");
		return 0;
	}
	x = get_4bytes(avp->data.s);
	if (data) *data = x;
	return 1;
}


inline int cdp_avp_get_IPFilterRule(AAA_AVP *avp,str *data)
{
	if (data) *data = avp->data;
	return 1;
}


inline int cdp_avp_get_QoSFilterRule(AAA_AVP *avp,str *data)
{
	if (data) *data = avp->data;
	return 1;
}

