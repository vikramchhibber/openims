/**
 * $Id: avp_new_base_data_format.h 1075 2011-04-13 07:34:15Z vingarzan $
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
 * CDiameter AVP Operations modules - AVP Data Formats Encoding
 * 
 * 
 *  \author Dragos Vingarzan dragos dot vingarzan -at- fokus dot fraunhofer dot de
 * 
 */ 

#ifndef __AVP_NEW_BASE_DATA_FORMAT_H
#define __AVP_NEW_BASE_DATA_FORMAT_H

#ifndef CDP_AVP_REFERENCE

	#include "../cdp/cdp_load.h"

	#ifdef CDP_FOR_SER

		#include <inttypes.h>
		#include <netinet/ip6.h>

		typedef struct {
			uint16_t ai_family;
			union{
				struct in_addr v4;
				struct in6_addr v6;
			} ip;
		} ip_address;
	
		typedef struct {
			uint8_t prefix;
			ip_address addr;	
		} ip_address_prefix;
		
	#endif


	#define EPOCH_UNIX_TO_EPOCH_NTP 2208988800u // according to http://www.cis.udel.edu/~mills/y2k.html
	
	
	/* 
	 * RFC 3588 Basic AVP Data Types
	 * 
	 * http://tools.ietf.org/html/rfc3588#section-4.2
	 * 
	 */
	
	AAA_AVP* cdp_avp_new_OctetString(int avp_code,int avp_flags,int avp_vendorid,str data,AVPDataStatus data_do);
	typedef AAA_AVP* (*cdp_avp_new_OctetString_f)(int avp_code,int avp_flags,int avp_vendorid,str data,AVPDataStatus data_do);
	
	AAA_AVP* cdp_avp_new_Integer32(int avp_code,int avp_flags,int avp_vendorid,int32_t data);
	typedef AAA_AVP* (*cdp_avp_new_Integer32_f)(int avp_code,int avp_flags,int avp_vendorid,int32_t data);
	
	
	AAA_AVP* cdp_avp_new_Integer64(int avp_code,int avp_flags,int avp_vendorid,int64_t data);
	typedef AAA_AVP* (*cdp_avp_new_Integer64_f)(int avp_code,int avp_flags,int avp_vendorid,int64_t data);
	
	
	AAA_AVP* cdp_avp_new_Unsigned32(int avp_code,int avp_flags,int avp_vendorid,uint32_t data);
	typedef AAA_AVP* (*cdp_avp_new_Unsigned32_f)(int avp_code,int avp_flags,int avp_vendorid,uint32_t data);
	
	
	AAA_AVP* cdp_avp_new_Unsigned64(int avp_code,int avp_flags,int avp_vendorid,uint64_t data);
	typedef AAA_AVP* (*cdp_avp_new_Unsigned64_f)(int avp_code,int avp_flags,int avp_vendorid,uint64_t data);
	
	
	AAA_AVP* cdp_avp_new_Float32(int avp_code,int avp_flags,int avp_vendorid,float data);
	typedef AAA_AVP* (*cdp_avp_new_Float32_f)(int avp_code,int avp_flags,int avp_vendorid,float data);
	
	
	AAA_AVP* cdp_avp_new_Float64(int avp_code,int avp_flags,int avp_vendorid,double data);
	typedef AAA_AVP* (*cdp_avp_new_Float64_f)(int avp_code,int avp_flags,int avp_vendorid,double data);
	
	
	AAA_AVP* cdp_avp_new_Grouped(int avp_code,int avp_flags,int avp_vendorid,AAA_AVP_LIST *list,AVPDataStatus data_do);
	typedef AAA_AVP* (*cdp_avp_new_Grouped_f)(int avp_code,int avp_flags,int avp_vendorid,AAA_AVP_LIST *list,AVPDataStatus data_do);
	
	
	/*
	 * RFC 3588 Derived AVP Data Formats
	 * 
	 * http://tools.ietf.org/html/rfc3588#section-4.3
	 * 
	 */
	
	AAA_AVP* cdp_avp_new_Address(int avp_code,int avp_flags,int avp_vendorid,ip_address data);
	typedef AAA_AVP* (*cdp_avp_new_Address_f)(int avp_code,int avp_flags,int avp_vendorid,ip_address data);
	
	AAA_AVP* cdp_avp_new_Time(int avp_code,int avp_flags,int avp_vendorid,time_t data);
	typedef AAA_AVP* (*cdp_avp_new_Time_f)(int avp_code,int avp_flags,int avp_vendorid,time_t data);
	
	AAA_AVP* cdp_avp_new_UTF8String(int avp_code,int avp_flags,int avp_vendorid,str data,AVPDataStatus data_do);
	typedef AAA_AVP* (*cdp_avp_new_UTF8String_f)(int avp_code,int avp_flags,int avp_vendorid,str data,AVPDataStatus data_do);
	
	AAA_AVP* cdp_avp_new_DiameterIdentity(int avp_code,int avp_flags,int avp_vendorid,str data,AVPDataStatus data_do);
	typedef AAA_AVP* (*cdp_avp_new_DiameterIdentity_f)(int avp_code,int avp_flags,int avp_vendorid,str data,AVPDataStatus data_do);
	
	AAA_AVP* cdp_avp_new_DiameterURI(int avp_code,int avp_flags,int avp_vendorid,str data,AVPDataStatus data_do);
	typedef AAA_AVP* (*cdp_avp_new_DiameterURI_f)(int avp_code,int avp_flags,int avp_vendorid,str data,AVPDataStatus data_do);
	
	AAA_AVP* cdp_avp_new_Enumerated(int avp_code,int avp_flags,int avp_vendorid,int32_t data);
	typedef AAA_AVP* (*cdp_avp_new_Enumerated_f)(int avp_code,int avp_flags,int avp_vendorid,int32_t data);
	
	AAA_AVP* cdp_avp_new_IPFilterRule(int avp_code,int avp_flags,int avp_vendorid,str data,AVPDataStatus data_do);
	typedef AAA_AVP* (*cdp_avp_new_IPFilterRule_f)(int avp_code,int avp_flags,int avp_vendorid,str data,AVPDataStatus data_do);
	
	AAA_AVP* cdp_avp_new_QoSFilterRule(int avp_code,int avp_flags,int avp_vendorid,str data,AVPDataStatus data_do);
	typedef AAA_AVP* (*cdp_avp_new_QoSFilterRule_f)(int avp_code,int avp_flags,int avp_vendorid,str data,AVPDataStatus data_do);
	

#else
		
	AAA_AVP* basic.new_OctetString(int avp_code,int avp_flags,int avp_vendorid,str data,AVPDataStatus data_do);
	
	AAA_AVP* basic.new_Integer32(int avp_code,int avp_flags,int avp_vendorid,int32_t data);
	
	AAA_AVP* basic.new_Integer64(int avp_code,int avp_flags,int avp_vendorid,int64_t data);
	
	AAA_AVP* basic.new_Unsigned32(int avp_code,int avp_flags,int avp_vendorid,uint32_t data);
	
	AAA_AVP* basic.new_Unsigned64(int avp_code,int avp_flags,int avp_vendorid,uint64_t data);
	
	AAA_AVP* basic.new_Float32(int avp_code,int avp_flags,int avp_vendorid,float data);
	
	AAA_AVP* basic.new_Float64(int avp_code,int avp_flags,int avp_vendorid,double data);
	
	AAA_AVP* basic.new_Grouped(int avp_code,int avp_flags,int avp_vendorid,AAA_AVP_LIST *list,AVPDataStatus data_do);
		
	AAA_AVP* basic.new_Address(int avp_code,int avp_flags,int avp_vendorid,ip_address data);
	
	AAA_AVP* basic.new_Time(int avp_code,int avp_flags,int avp_vendorid,time_t data);
	
	AAA_AVP* basic.new_UTF8String(int avp_code,int avp_flags,int avp_vendorid,str data,AVPDataStatus data_do);
	
	AAA_AVP* basic.new_DiameterIdentity(int avp_code,int avp_flags,int avp_vendorid,str data,AVPDataStatus data_do);
	
	AAA_AVP* basic.new_DiameterURI(int avp_code,int avp_flags,int avp_vendorid,str data,AVPDataStatus data_do);
	
	AAA_AVP* basic.new_Enumerated(int avp_code,int avp_flags,int avp_vendorid,int32_t data);
	
	AAA_AVP* basic.new_IPFilterRule(int avp_code,int avp_flags,int avp_vendorid,str data,AVPDataStatus data_do);
	
	AAA_AVP* basic.new_QoSFilterRule(int avp_code,int avp_flags,int avp_vendorid,str data,AVPDataStatus data_do);
#endif


#endif /* __AVP_NEW_DATA_FORMAT_H */
