/**
 * $Id: avp_new_base_data_format.c 1145 2011-08-30 13:42:36Z vingarzan $
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
 * CDiameter AVP Operations modules - AVP Data Formats
 * 
 * 
 *  \author Dragos Vingarzan dragos dot vingarzan -at- fokus dot fraunhofer dot de
 * 
 */ 

#include "avp_new_base_data_format.h"

#include "avp_new.h"

extern struct cdp_binds *cdp;


/* 
 * RFC 3588 Basic AVP Data Types
 * 
 * http://tools.ietf.org/html/rfc3588#section-4.2
 * 
 */

inline AAA_AVP *cdp_avp_new_OctetString(int avp_code,int avp_flags,int avp_vendorid,str data,AVPDataStatus data_do)
{
	return cdp_avp_new(avp_code,avp_flags,avp_vendorid,data,data_do);
}

inline AAA_AVP *cdp_avp_new_Integer32(int avp_code,int avp_flags,int avp_vendorid,int32_t data)
{
	char x[4];
	str s={x,4};
	set_4bytes(x,data);
	return cdp_avp_new(avp_code,avp_flags,avp_vendorid,s,AVP_DUPLICATE_DATA);
}


inline AAA_AVP *cdp_avp_new_Integer64(int avp_code,int avp_flags,int avp_vendorid,int64_t data)
{
	char x[8];
	str s={x,8};
	int i;
	for(i=7;i>=0;i--){
		x[i] = data%256;
		data /= 256;
	}					//TODO - check if this is correct
	return cdp_avp_new(avp_code,avp_flags,avp_vendorid,s,AVP_DUPLICATE_DATA);	
}

inline AAA_AVP *cdp_avp_new_Unsigned32(int avp_code,int avp_flags,int avp_vendorid,uint32_t data)
{
	char x[4];
	str s={x,4};
	uint32_t ndata=htonl(data);
	memcpy(x,&ndata,sizeof(uint32_t));
	//*((uint32_t*)x) = htonl(data);
	return cdp_avp_new(avp_code,avp_flags,avp_vendorid,s,AVP_DUPLICATE_DATA);
}

inline AAA_AVP *cdp_avp_new_Unsigned64(int avp_code,int avp_flags,int avp_vendorid,uint64_t data)
{
	char x[8];
	str s={x,8};
	int i;
	for(i=7;i>=0;i--){
		x[i] = data%256;
		data /= 256;
	}
	return cdp_avp_new(avp_code,avp_flags,avp_vendorid,s,AVP_DUPLICATE_DATA);
}

inline AAA_AVP *cdp_avp_new_Float32(int avp_code,int avp_flags,int avp_vendorid,float data)
{
	uint32_t udata;
	memcpy(&udata,&data,sizeof(uint32_t));
	return cdp_avp_new_Unsigned32(avp_code,avp_flags,avp_vendorid,udata);//TODO - check if this is correct
}

inline AAA_AVP *cdp_avp_new_Float64(int avp_code,int avp_flags,int avp_vendorid,double data)
{
	uint64_t udata;
	memcpy(&udata,&data,sizeof(uint32_t));	
	return cdp_avp_new_Unsigned64(avp_code,avp_flags,avp_vendorid,udata);//TODO - check if this is correct
}

/**
 * Creates a grouped AVP from a list
 * @param avp_code
 * @param avp_flags
 * @param avp_vendorid
 * @param list
 * @param list_do - if this is AVP_FREE_DATA then the list will aso be freed
 * @return
 */
inline AAA_AVP *cdp_avp_new_Grouped(int avp_code,int avp_flags,int avp_vendorid,AAA_AVP_LIST *list,AVPDataStatus list_do)
{
	str grp;
	if (!list){
		LOG(L_ERR,"The AAA_AVP_LIST was NULL!\n");
		return 0;
	}
	grp = cdp->AAAGroupAVPS(*list);
	if (!grp.len){
		LOG(L_ERR,"The AAA_AVP_LIST provided was empty! (AVP Code %d VendorId %d)\n",avp_code,avp_vendorid);
		return 0;
	}
	if (list_do==AVP_FREE_DATA)
		cdp->AAAFreeAVPList(list);
	return cdp_avp_new(avp_code,avp_flags,avp_vendorid,grp,AVP_FREE_DATA);
}

/*
 * RFC 3588 Derived AVP Data Formats
 * 
 * http://tools.ietf.org/html/rfc3588#section-4.3
 * 
 */

inline AAA_AVP* cdp_avp_new_Address(int avp_code,int avp_flags,int avp_vendorid,ip_address data)
{
	char x[18];
	str s;
	s.s = x;
	s.len = 0;

	switch (data.ai_family){
		case AF_INET:
			x[0]=0;
			x[1]=1;
			memcpy(x+2, (char*)(&data.ip.v4.s_addr), 4);
			s.len=6;
			break;
		case AF_INET6:
			x[0]=0;
			x[1]=2;
			s.len=18;
			memcpy(x+2,data.ip.v6.s6_addr,16);
			break;
		default:
			LOG(L_ERR,"Unimplemented for ai_family %d! (AVP Code %d Vendor-Id %d)\n",data.ai_family,avp_code,avp_vendorid);
			return 0;
	}
	return cdp_avp_new(avp_code,avp_flags,avp_vendorid,s,AVP_DUPLICATE_DATA);
}

inline AAA_AVP* cdp_avp_new_Time(int avp_code,int avp_flags,int avp_vendorid,time_t data)
{
	char x[4];
	str s={x,4};
	uint32_t ntime = htonl(data+EPOCH_UNIX_TO_EPOCH_NTP);
	memcpy(x,&ntime,sizeof(uint32_t));	
	return cdp_avp_new(avp_code,avp_flags,avp_vendorid,s,AVP_DUPLICATE_DATA);
}

inline AAA_AVP *cdp_avp_new_UTF8String(int avp_code,int avp_flags,int avp_vendorid,str data,AVPDataStatus data_do)
{
	return cdp_avp_new(avp_code,avp_flags,avp_vendorid,data,data_do);
}

inline AAA_AVP* cdp_avp_new_DiameterIdentity(int avp_code,int avp_flags,int avp_vendorid,str data,AVPDataStatus data_do)
{
	return cdp_avp_new(avp_code,avp_flags,avp_vendorid,data,data_do);
}

inline AAA_AVP* cdp_avp_new_DiameterURI(int avp_code,int avp_flags,int avp_vendorid,str data,AVPDataStatus data_do)
{
	return cdp_avp_new(avp_code,avp_flags,avp_vendorid,data,data_do);
}

inline AAA_AVP* cdp_avp_new_Enumerated(int avp_code,int avp_flags,int avp_vendorid,int32_t data)
{
	char x[4];
	str s={x,4};
	set_4bytes(x,data);
	return cdp_avp_new(avp_code,avp_flags,avp_vendorid,s,AVP_DUPLICATE_DATA);
}

inline AAA_AVP* cdp_avp_new_IPFilterRule(int avp_code,int avp_flags,int avp_vendorid,str data,AVPDataStatus data_do)
{
	return cdp_avp_new(avp_code,avp_flags,avp_vendorid,data,data_do);
}

inline AAA_AVP* cdp_avp_new_QoSFilterRule(int avp_code,int avp_flags,int avp_vendorid,str data,AVPDataStatus data_do)
{
	return cdp_avp_new(avp_code,avp_flags,avp_vendorid,data,data_do);
}
