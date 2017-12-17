/**
 * $Id: mod.c 926 2010-03-08 10:55:30Z vingarzan $
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
 * CDiameterPeer AVP Operations Module
 * 
 * 
 *  \author Dragos Vingarzan dragos dot vingarzan -at- fokus dot fraunhofer dot de
 * 
 */  

#include <time.h>

#ifdef WHARF
	#include "../../base/mod.h"
	#include "../../base/mod_ops.h"
	#include "../../utils/utils.h"
#endif

#ifdef CDP_FOR_SER
	#include "../../sr_module.h"
	MODULE_VERSION
#endif

#include "mod_export.h"

#include "../cdp/cdp_load.h"

	

#ifdef WHARF
	struct cdp_binds *cdp=0;

	int 	cdp_avp_init(str config);
	int 	cdp_avp_child_init(int rank);
	void 	cdp_avp_destroy();
	void* 	cdp_avp_get_bind();

	/**
	 * Exported Wharf modules interface 
	 */
	wharf_mod_export_t mod_exports={
			
		"cdp_avp",					/**< Module's unique name */
		
		WHARF_VERSION"-"WHARF_REVISION,		
		
		cdp_avp_init,					/**< Module init function */
		cdp_avp_child_init,				/**< Module child init function */
		cdp_avp_destroy,				/**< Module destroy function */
		
		cdp_avp_get_bind				/**< Module get binding function */
			
	};
	
#endif

#ifdef CDP_FOR_SER
	struct cdp_binds *cdp;
	
	int 	cdp_avp_init();
	int 	cdp_avp_child_init(int rank);
	void 	cdp_avp_destroy();
	cdp_avp_bind_t*	cdp_avp_get_bind();
	
	static cmd_export_t cdp_avp_cmds[] = {
		{"cdp_avp_get_bind",			(cmd_function)cdp_avp_get_bind, 	NO_SCRIPT, 0, 0},
		
		{ 0, 0, 0, 0, 0 }
	};
	
	/**
	 * Exported SER module interface
	 */
	struct module_exports exports = {
		"cdp_avp",
		cdp_avp_cmds,                   /**< Exported functions */
		0,
		0,                     			/**< Exported parameters */
		cdp_avp_init,                   /**< Module initialization function */
		(response_function) 0,
		(destroy_function) cdp_avp_destroy,
		0,
		(child_init_function) cdp_avp_child_init /**< per-child init function */
	};

#endif

/** Sample binding */
cdp_avp_bind_t cdp_avp_bind={
		0,	/* cdp 		*/
		
		{	/* basic 	*/
				cdp_avp_new,
				
				cdp_avp_add_new_to_list,
				cdp_avp_add_new_to_msg,
				cdp_avp_add_to_list,
				cdp_avp_add_to_msg,
				
				cdp_avp_get_next_from_list,
				cdp_avp_get_next_from_msg,
				cdp_avp_get_from_list,
				cdp_avp_get_from_msg,
		},
		
		{	/* base_data	*/
				cdp_avp_new_OctetString,
				cdp_avp_new_Integer32,
				cdp_avp_new_Integer64,
				cdp_avp_new_Unsigned32,
				cdp_avp_new_Unsigned64,
				cdp_avp_new_Float32,
				cdp_avp_new_Float64,
				cdp_avp_new_Grouped,
				
				cdp_avp_new_Address,
				cdp_avp_new_Time,
				cdp_avp_new_UTF8String,
				cdp_avp_new_DiameterIdentity,
				cdp_avp_new_DiameterURI,
				cdp_avp_new_Enumerated,
				cdp_avp_new_IPFilterRule,
				cdp_avp_new_QoSFilterRule,
				
				
				cdp_avp_get_OctetString,
				cdp_avp_get_Integer32,
				cdp_avp_get_Integer64,
				cdp_avp_get_Unsigned32,
				cdp_avp_get_Unsigned64,
				cdp_avp_get_Float32,
				cdp_avp_get_Float64,
				cdp_avp_get_Grouped,
				cdp_avp_free_Grouped,

				cdp_avp_get_Address,
				cdp_avp_get_Time,
				cdp_avp_get_UTF8String,
				cdp_avp_get_DiameterIdentity,
				cdp_avp_get_DiameterURI,
				cdp_avp_get_Enumerated,
				cdp_avp_get_IPFilterRule,
				cdp_avp_get_QoSFilterRule,
		},
		
		{	/*	base 	*/
				
				#define CDP_AVP_INIT					
					#include "base.h"				
				#undef	CDP_AVP_INIT												
		},
		
		{	/*	ccapp 	*/
				#define CDP_AVP_INIT			
					#include "ccapp.h"		
				#undef	CDP_AVP_INIT
		},

		{	/*  nasapp  */
				#define CDP_AVP_INIT					
					#include "nasapp.h"				
				#undef	CDP_AVP_INIT
		},
		
		{	/*  imsapp  */
				#define CDP_AVP_INIT					
					#include "imsapp.h"				
				#undef	CDP_AVP_INIT
		},	
		
		{	/*  epcapp  */				
				#define CDP_AVP_INIT					
					#include "epcapp.h"				
				#undef	CDP_AVP_INIT				
		}
};


/**
 * Module initialization function - called once at startup.
 * \note Other modules might not be loaded at this moment.
 * If this returns failure, wharf will exit
 * 
 * @param config - abstract configuration string
 * @return 1 on success or 0 on failure
 */
#ifdef WHARF
int cdp_avp_init(str config)
{
	LOG(L_DBG," Initializing module cdp_avp with configuration string of length [%d]\n",
			config.len);

	/* CDiameterPeer prerequisites */
	if (!mod_is_loaded("cdp")){
		LOG(L_ERR,"The cdp_avp module requires the CDiameterPeer (cdp) module.\n");
		return 0;
	}
	
	/* Load CDP module bindings*/
	cdp = mod_get_bind("cdp");
	cdp_avp_bind.cdp = cdp;
	
	return 1;
}
#endif

#ifdef CDP_FOR_SER
int cdp_avp_init()
{
	LOG(L_DBG," Initializing module cdp_avp\n");
	load_cdp_f load_cdp;
	/* bind to the cdp module */
	if (!(load_cdp = (load_cdp_f)find_export("load_cdp",NO_SCRIPT,0))) {
		LOG(L_ERR, "ERR"M_NAME":mod_init: Can not import load_cdp. This module requires cdp module\n");
		goto error;
	}
	cdp = pkg_malloc(sizeof(struct cdp_binds));
	if (!cdp) return 0;
	/* Load CDP module bindings*/
	if (load_cdp(cdp) == -1)
		goto error;
	
	cdp_avp_bind.cdp = cdp;
	
	return 0;
error:
	return -1;
}
#endif

/**
 * Module initialization function - called once for every process.
 * \note All modules have by now executed the mod_init.
 * If this returns failure, wharf will exit
 * 
 * @param rank - rank of the process calling this
 * @return 1 on success or 0 on failure
 */
#ifdef WHARF
int cdp_avp_child_init(int rank)
{
	LOG(L_DBG,"Initializing child in module cdp_avp for rank [%d]\n",
			rank);
	return 1;
}
#endif

#ifdef CDP_FOR_SER
int cdp_avp_child_init(int rank)
{
	LOG(L_DBG,"Initializing child in module cdp_avp for rank [%d]\n",
			rank);
	return 1;
}
#endif



/**
 * Module destroy function. 
 * Spould clean-up and do nice shut-down.
 * \note Will be called multiple times, once from each process, although crashed processes might not.
 */
#ifdef WHARF
void cdp_avp_destroy(int rank)
{
	LOG(L_DBG,"Destroying module cdp_avp\n");
}
#endif

#ifdef CDP_FOR_SER
void cdp_avp_destroy(void)
{
	LOG(L_DBG,"Destroying module cdp_avp\n");
	pkg_free(cdp);
}
#endif



/**
 * Returns the module's binding. This will give the structure containing the 
 * functions and data to be used from other processes.
 * @return the pointer to the binding.
 */
#ifdef WHARF
	void* cdp_avp_get_bind()
	{
		return &cdp_avp_bind;
	}
#endif

#ifdef CDP_FOR_SER
	cdp_avp_bind_t* cdp_avp_get_bind()
	{
		return &cdp_avp_bind;
	}
#endif



