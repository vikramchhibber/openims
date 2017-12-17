/*
 * $Id: config.h 1167 2011-10-25 09:17:05Z aon $
 *
 * Copyright (C) 2008-2009 FhG Fokus
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
 * author Ancuta Corici, 
 * 	email andreea dot ancuta dot corici -at- fokus dot fraunhofer dot de
 */
/**
 * Client_Rf - implementation of the Rf interface from the CTF side, according to TS32.299 R7
 * 
 * Scope:
 * - Exports parameters and functions
 * - Initialization functions
 * 
 */


#ifndef __CLIENT_RF_CONFIG_H
#define __CLIENT_RF_CONFIG_H

#ifndef WHARF
#include "str_int_list.h"
#else
#include "../../utils/utils.h"
#endif

typedef struct {
	int32_t hash_table_size;
	str origin_host;
	str origin_realm;
	str destination_realm;
	str destination_host;
	str service_context_id;
	/* Node functionality:
	 *  S-CSCF: 0, P-CSCF: 1, I-CSCF: 2, MRFC: 3, MGCF: 4, BGCF: 5, 
	 *  AS: 6, IBCF: 7, S-GW: 8, P-GW: 9, HSGW: 10 */
	int32_t node_func;
} client_rf_cfg;

#ifdef WHARF

#define CLIENT_RF_CONFIG_DTD "\
<?xml version=\"1.0\" encoding=\"UTF-8\"?>\
<!ELEMENT Client_Rf ( Hash, Rf )>\
\
<!ELEMENT Hash (#PCDATA)>\
<!ATTLIST Hash\
	hash_size			CDATA	#REQUIRED\
>\
<!ELEMENT Rf (#PCDATA)>\
<!ATTLIST Rf\
	node_func			CDATA	#REQUIRED\
	origin_host			CDATA	#REQUIRED\
	origin_realm			CDATA	#REQUIRED\
	destination_host		CDATA	#REQUIRED\
	destination_realm		CDATA	#REQUIRED\
	service_context_id		CDATA	#REQUIRED\
>\
\
";

int client_rf_parse_config(str config);
void client_rf_free_config();

#endif /* WHARF*/

#endif /* __CLIENT_RF_CONFIG_H*/
