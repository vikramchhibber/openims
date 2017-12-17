/*
 * $Id: mod_wharf.c 1172 2011-10-27 08:42:46Z aon $
 * 
 * Copyright (C) 2009 FhG Fokus
 * 
 * This file is part of the Wharf project.
 * 
 */

/**
 * \file
 * 
 * Diameter Rf interface towards the CDF - Wharf module interface
 * 
 * 
 *  \author Andreea Ancuta Corici andreea dot ancuta dot corici -at- fokus dot fraunhofer dot de
 * 
 */

#ifdef WHARF

#include "stdlib.h"

#include "../../base/mod.h"

#include "../../base/mod_ops.h"

#include "../../base/fork.h"

#include "../../utils/utils.h"

#include "mod_wharf.h"
#include "mod_export.h"
#include "../cdp/cdp_load.h"
#include "diameter_rf.h"
#include "config.h"
#include "charging.h"
#include "charging_id_generator.h"

cdp_avp_bind_t *cavpb=0;				/**< Structure with pointers to cdp_avp funcs*/
client_rf_cfg cfg;

/** This variable has to be called exactly mod_exports, as it is searched by the wharf base */
wharf_mod_export_t mod_exports={
		
		"Client_Rf",					/**< Module's unique name */
		
		WHARF_VERSION"-"WHARF_REVISION,		
		
		client_rf_init,				/**< Module init function */
		client_rf_child_init,			/**< Module child init function */
		client_rf_destroy,			/**< Module destroy function */
		
		client_rf_get_bind			/**< Module get binding function */
		
};	


/** 
 * Module init function.
 * 
 * - Initializes the Rf module using the provided configuration file.
 * - Registers with pt the required number of processes.
 */
int client_rf_init(str config_data)
{
	LOG(L_INFO," Client_Rf initializing\n");
	LOG(L_DBG, "initializing module Checking cdp\n");
	if (!mod_is_loaded("cdp"))	{
			LOG(L_ERR,"The C Diameter Peer module has not been loaded and is needed by Client_Rf\n");
			return 0;
	}
	LOG(L_DBG, "initializing module Checking cdp_avp\n");
	if (!mod_is_loaded("cdp_avp"))
	{
		LOG(L_ERR,"The C Diameter Peer module AVP helper has not been loaded and is needed by Client_Rx\n");
		return 0;
	} else {
		cavpb = (cdp_avp_bind_t *) mod_get_bind("cdp_avp");
	}

	if(!client_rf_parse_config(config_data)){
		return 0;
	}

	if(!init_gen_charg_id_info())
		return 0;

	return 1;
}

/**
 * Module initialization function - called once for every process.
 * \note All modules have by now executed the mod_init.
 * If this returns failure, wharf will exit
 * 
 * @param rank - rank of the process calling this
 * @return 1 on success or 0 on failure
 */

int client_rf_child_init(int rank)
{
	if (rank == WHARF_PROCESS_ATTENDANT) { 
		LOG(L_INFO,"Client_Rf starting ...\n");		
		cavpb->cdp->AAAAddResponseHandler(RfChargingResponseHandler, NULL);
		LOG(L_INFO," ... Client_Rf started\n");		
	}
	return 1;
}

/**
 * Module destroy function. 
 * Should clean-up and do nice shut-down.
 * \note Will be called multiple times, once from each process, although crashed processes might not.
 */
void client_rf_destroy(int rank)
{
	LOG(L_INFO," ... Client_Rf destroy\n");
	if (rank==WHARF_PROCESS_ATTENDANT){
		client_rf_free_config();
		destroy_gen_charg_id_info();
	}
}



struct client_rf_bind_t client_rf_binding={
	AAASendACR,
	Rf_add_an_chg_info,
	Rf_add_ims_chg_info_icid,
	Rf_get_ims_chg_info_icid,
	Rf_add_ims_chg_ps_info,
	create_Rf_data,
	free_Rf_data,
	gen_new_charg_id,
	release_charg_id
};


/**
 * Returns the module's binding. This will give the structure containing the 
 * functions and data to be used from other processes.
 * @return the pointer to the binding.
 */
void* client_rf_get_bind()
{
	return &client_rf_binding;
}



#endif /* WHARF */
