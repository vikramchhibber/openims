/**
 * $Id: mod_export.h 926 2010-03-08 10:55:30Z vingarzan $
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
 * \page CDP_AVP CDiameterPeer AVP helpers Interface 
 * 
 * Implements lots of helpers for AVP operations with the cdp module
 * 
 * 
 */
/**
 * \file
 * 
 * CDiameter AVP Operations modules - exports
 * 
 * 
 *  \author Dragos Vingarzan dragos dot vingarzan -at- fokus dot fraunhofer dot de
 * 
 */ 

#ifndef __CDP_AVP_EXPORT_H
#define __CDP_AVP_EXPORT_H

#ifdef CDP_FOR_SER
	#ifndef M_NAME
		#define M_NAME "cdp_avp"
	#endif
#endif

#include "../cdp/cdp_load.h"


#include "avp_new.h"
#include "avp_new_base_data_format.h"

#include "avp_add.h"

#include "avp_get.h"
#include "avp_get_base_data_format.h"

#include "base.h"
#include "nasapp.h"
#include "ccapp.h"
#include "imsapp.h"
#include "epcapp.h"



typedef struct {
	
	cdp_avp_new_f 					new;
	
	cdp_avp_add_new_to_list_f		add_new_to_list;
	cdp_avp_add_new_to_msg_f 		add_new_to_msg;
	cdp_avp_add_to_list_f			add_to_list;
	cdp_avp_add_to_msg_f			add_to_msg;
	
	cdp_avp_get_next_from_list_f	get_next_from_list;
	cdp_avp_get_next_from_msg_f		get_next_from_msg;
	cdp_avp_get_from_list_f			get_from_list;
	cdp_avp_get_from_msg_f			get_from_msg;

} cdp_avp_bind_basic_t ;

typedef struct {

	cdp_avp_new_OctetString_f 		new_OctetString;
	cdp_avp_new_Integer32_f			new_Integer32;
	cdp_avp_new_Integer64_f			new_Integer64;
	cdp_avp_new_Unsigned32_f		new_Unsigned32;
	cdp_avp_new_Unsigned64_f		new_Unsigned64;
	cdp_avp_new_Float32_f			new_Float32;
	cdp_avp_new_Float64_f			new_Float64;
	cdp_avp_new_Grouped_f			new_Grouped;
	
	cdp_avp_new_Address_f			new_Address;
	cdp_avp_new_Time_f				new_Time;
	cdp_avp_new_UTF8String_f		new_UTF8String;
	cdp_avp_new_DiameterIdentity_f	new_DiameterIdentity;
	cdp_avp_new_DiameterURI_f		new_DiameterURI;
	cdp_avp_new_Enumerated_f		new_Enumerated;
	cdp_avp_new_IPFilterRule_f		new_IPFilterRule;
	cdp_avp_new_QoSFilterRule_f		new_QoSFilterRule;
	
	
	cdp_avp_get_OctetString_f		get_OctetString;
	cdp_avp_get_Integer32_f			get_Integer32;
	cdp_avp_get_Integer64_f			get_Integer64;
	cdp_avp_get_Unsigned32_f		get_Unsigned32;
	cdp_avp_get_Unsigned64_f		get_Unsigned64;
	cdp_avp_get_Float32_f			get_Float32;
	cdp_avp_get_Float64_f			get_Float64;
	cdp_avp_get_Grouped_f			get_Grouped;
	cdp_avp_free_Grouped_f			free_Grouped;
	
	cdp_avp_get_Address_f			get_Address;
	cdp_avp_get_Time_f				get_Time;
	cdp_avp_get_UTF8String_f		get_UTF8String;
	cdp_avp_get_DiameterIdentity_f	get_DiameterIndentity;
	cdp_avp_get_DiameterURI_f		get_DiameterURI;
	cdp_avp_get_Enumerated_f		get_Enumerated;	
	cdp_avp_get_IPFilterRule_f		get_IPFilterRule;
	cdp_avp_get_QoSFilterRule_f		get_QoSFilterRule;
	
} cdp_avp_bind_base_data_format_t;


typedef struct {
	
	#define CDP_AVP_EXPORT			
		#include "base.h"		
	#undef	CDP_AVP_EXPORT
	
} cdp_avp_bind_base_avp_t;

typedef struct {
		
	#define CDP_AVP_EXPORT			
		#include "ccapp.h"		
	#undef	CDP_AVP_EXPORT

} cdp_avp_bind_ccapp_avp_t;


typedef struct {

	
	#define CDP_AVP_EXPORT			
		#include "nasapp.h"		
	#undef	CDP_AVP_EXPORT
	
} cdp_avp_bind_nasapp_avp_t;

typedef struct {
	
	#define CDP_AVP_EXPORT			
		#include "imsapp.h"		
	#undef	CDP_AVP_EXPORT

} cdp_avp_bind_imsapp_avp_t;

typedef struct {
		
	#define CDP_AVP_EXPORT			
		#include "epcapp.h"		
	#undef	CDP_AVP_EXPORT
	
} cdp_avp_bind_epcapp_avp_t;

typedef struct {
	
	struct cdp_binds				*cdp;
	
	cdp_avp_bind_basic_t 			basic;
	
	cdp_avp_bind_base_data_format_t data;
	
	cdp_avp_bind_base_avp_t			base;
	
	cdp_avp_bind_ccapp_avp_t		ccapp;
	
	cdp_avp_bind_nasapp_avp_t 		nasapp;
	
	cdp_avp_bind_imsapp_avp_t 		imsapp;

	cdp_avp_bind_epcapp_avp_t 		epcapp;

} cdp_avp_bind_t;



typedef cdp_avp_bind_t* (*cdp_avp_get_bind_f)(void);


#endif /* __CDP_AVP_EXPORT_H */
