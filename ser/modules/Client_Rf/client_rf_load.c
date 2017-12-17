/**
 * $Id: client_rf_load.c 1171 2011-10-27 08:24:39Z aon $
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
 * Client_Rf - functional bindings for usage in SER/OSIM modules
 * 
 *  \author Ancuta Corici, email andreea dot ancuta dot corici -at- fokus dot fraunhofer dot de
 */
 
#include "client_rf_load.h"

#ifdef CDP_FOR_SER
#include "../../sr_module.h"


#define LOAD_ERROR "ERROR: client_rf_bind: S-CSCF module function "

#define FIND_EXP(NAME) \
	if (!( client_rfb->NAME=(NAME##_f) \
		find_export(#NAME, NO_SCRIPT, 0)) ) {\
		LOG(L_ERR, LOAD_ERROR "'"#NAME"' not found\n");\
		return -1;\
	}


/**
 * Load the Client_Rf bindings
 * @param *client_rfb - target structure to load the bindings into
 * @returns 1 on success, -1 on failure
 */  
int load_client_rf( struct client_rf_binds *client_rfb)
{
	FIND_EXP(Rf_add_an_chg_info);
	FIND_EXP(Rf_add_ims_chg_info_icid);
	FIND_EXP(Rf_get_ims_chg_info_icid);
	FIND_EXP(Rf_add_ims_chg_ps_info);
	FIND_EXP(Rf_send_stop_record);
	return 1;
}

#endif /* CDP_FOR_SER */

