/**
 * $Id: avp_get_base_data_format.h 1076 2011-04-13 14:14:52Z aon $
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

#ifndef __AVP_GET_BASE_DATA_FORMAT_H
#define __AVP_GET_BASE_DATA_FORMAT_H

#ifndef CDP_AVP_REFERENCE
	#include "../cdp/cdp_load.h"

	
	#ifdef CDP_FOR_SER

		#include "avp_new_base_data_format.h"
		
	#endif

	#define EPOCH_UNIX_TO_EPOCH_NTP 2208988800u // according to http://www.cis.udel.edu/~mills/y2k.html
	
	/* 
	 * RFC 3588 Basic AVP Data Types
	 * 
	 * http://tools.ietf.org/html/rfc3588#section-4.2
	 * 
	 */
	
	int cdp_avp_get_OctetString(AAA_AVP *avp,str *data);
	typedef int (*cdp_avp_get_OctetString_f)(AAA_AVP *avp,str *data);
	
	
	int cdp_avp_get_Integer32(AAA_AVP *avp,int32_t *data);
	typedef int (*cdp_avp_get_Integer32_f)(AAA_AVP *avp,int32_t *data);
	
	
	int cdp_avp_get_Integer64(AAA_AVP *avp,int64_t *data);
	typedef int (*cdp_avp_get_Integer64_f)(AAA_AVP *avp,int64_t *data);
	
	
	int cdp_avp_get_Unsigned32(AAA_AVP *avp,uint32_t *data);
	typedef int (*cdp_avp_get_Unsigned32_f)(AAA_AVP *avp,uint32_t *data);
	
	
	int cdp_avp_get_Unsigned64(AAA_AVP *avp,uint64_t *data);
	typedef int (*cdp_avp_get_Unsigned64_f)(AAA_AVP *avp,uint64_t *data);
	
	
	int cdp_avp_get_Float32(AAA_AVP *avp,float *data);
	typedef int (*cdp_avp_get_Float32_f)(AAA_AVP *avp,float *data);
	
	
	int cdp_avp_get_Float64(AAA_AVP *avp,double *data);
	typedef int (*cdp_avp_get_Float64_f)(AAA_AVP *avp,double *data);
	
	
	int cdp_avp_get_Grouped(AAA_AVP *avp,AAA_AVP_LIST *data);
	typedef int (*cdp_avp_get_Grouped_f)(AAA_AVP *avp,AAA_AVP_LIST *data);
	
	
	void cdp_avp_free_Grouped(AAA_AVP_LIST *list);
	typedef void (*cdp_avp_free_Grouped_f)(AAA_AVP_LIST *list);
	
	/*
	 * RFC 3588 Derived AVP Data Formats
	 * 
	 * http://tools.ietf.org/html/rfc3588#section-4.3
	 * 
	 */
	
	int cdp_avp_get_Address(AAA_AVP *avp,ip_address *data);
	typedef int (*cdp_avp_get_Address_f)(AAA_AVP *avp,ip_address *data);
	
	int cdp_avp_get_Time(AAA_AVP *avp,time_t *data);
	typedef int (*cdp_avp_get_Time_f)(AAA_AVP *avp,time_t *data);
	
	int cdp_avp_get_UTF8String(AAA_AVP *avp,str *data);
	typedef int (*cdp_avp_get_UTF8String_f)(AAA_AVP *avp,str *data);
	
	int cdp_avp_get_DiameterIdentity(AAA_AVP *avp,str *data);
	typedef int (*cdp_avp_get_DiameterIdentity_f)(AAA_AVP *avp,str *data);
	
	int cdp_avp_get_DiameterURI(AAA_AVP *avp,str *data);
	typedef int (*cdp_avp_get_DiameterURI_f)(AAA_AVP *avp,str *data);
	
	int cdp_avp_get_Enumerated(AAA_AVP *avp,int32_t *data);
	typedef int (*cdp_avp_get_Enumerated_f)(AAA_AVP *avp,int32_t *data);
	
	int cdp_avp_get_IPFilterRule(AAA_AVP *avp,str *data);
	typedef int (*cdp_avp_get_IPFilterRule_f)(AAA_AVP *avp,str *data);
	
	int cdp_avp_get_QoSFilterRule(AAA_AVP *avp,str *data);
	typedef int (*cdp_avp_get_QoSFilterRule_f)(AAA_AVP *avp,str *data);
	

#else

	int basic.get_OctetString(AAA_AVP *avp,str *data);
	
	int basic.get_Integer32(AAA_AVP *avp,int32_t *data);
	
	int basic.get_Integer64(AAA_AVP *avp,int64_t *data);
	
	int basic.get_Unsigned32(AAA_AVP *avp,uint32_t *data);
	
	int basic.get_Unsigned64(AAA_AVP *avp,uint64_t *data);
	
	int basic.get_Float32(AAA_AVP *avp,float *data);
	
	int basic.get_Float64(AAA_AVP *avp,double *data);
	
	int basic.get_Grouped(AAA_AVP *avp,AAA_AVP_LIST *data);
	
	void basic.free_Grouped(AAA_AVP_LIST *list);
	
	int basic.get_Address(AAA_AVP *avp,ip_address *data);
	
	int basic.get_Time(AAA_AVP *avp,time_t *data);
	
	int basic.get_UTF8String(AAA_AVP *avp,str *data);
	
	int basic.get_DiameterIdentity(AAA_AVP *avp,str *data);
	
	int basic.get_DiameterURI(AAA_AVP *avp,str *data);
	
	int basic.get_Enumerated(AAA_AVP *avp,int32_t *data);
	
	int basic.get_IPFilterRule(AAA_AVP *avp,str *data);
	
	int basic.get_QoSFilterRule(AAA_AVP *avp,str *data);

#endif


#endif /* __AVP_GET_DATA_FORMAT_H */
