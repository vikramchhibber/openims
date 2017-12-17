/**
 * $Id: client_rf_load.h 1171 2011-10-27 08:24:39Z aon $
 *  
 * Copyright (C) 2004-2006 FhG Fokus
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
 * Client_Rf - functional bindings for usage in other SER modules
 * 
 *  \author Ancuta Corici andreea dot ancuta dot corici -at- fokus dot fraunhofer dot de
 */

#ifndef CLIENT_RF_BIND_H
#define CLIENT_RF_BIND_H

#ifdef CDP_FOR_SER
#include <stdint.h>
#include "../../str.h"

typedef int (*Rf_add_an_chg_info_f)(str sip_uri, uint32_t an_charg_id);
typedef int (*Rf_add_ims_chg_info_icid_f)(str call_id, int dir, str ims_charg_id);
typedef int (*Rf_get_ims_chg_info_icid_f)(str call_id, int dir, str  * ims_charg_id);
typedef int (*Rf_add_ims_chg_ps_info_f) (str call_id, int dir, uint32_t rating_group);
typedef int (*Rf_send_stop_record_f) (str call_id, int dir, str from, str to);

struct client_rf_binds {
	Rf_add_an_chg_info_f			Rf_add_an_chg_info;
	Rf_add_ims_chg_info_icid_f			Rf_add_ims_chg_info_icid;
	Rf_get_ims_chg_info_icid_f			Rf_get_ims_chg_info_icid;
	Rf_add_ims_chg_ps_info_f			Rf_add_ims_chg_ps_info;
	Rf_send_stop_record_f			Rf_send_stop_record;
};



#define NO_SCRIPT	-1


typedef int(*load_client_rf_f)( struct client_rf_binds *cdpb );

int load_client_rf( struct client_rf_binds *cdpb);

#endif /* CDP_FOR_SER */

#endif

