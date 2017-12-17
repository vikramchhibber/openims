/*
 * $Id: mod.c 1172 2011-10-27 08:42:46Z aon $
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
 
#ifdef OPEN_IMS_CORE

#include <ctype.h>
#include <sys/types.h>
#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

#include "mod.h"

#include "../../parser/parse_uri.h"
#include "../../sr_module.h"
#include "../../socket_info.h"
#include "../../timer.h"
#include "../../locking.h"
#include "../../modules/tm/tm_load.h"

#include "../cdp/cdp_load.h"
#include "../cdp_avp/mod_export.h"

#include "diameter_rf.h"
#include "ims_rf.h"
#include "config.h"
#include "Rf_data.h"
#include "charging.h"
#include "client_rf_load.h"

MODULE_VERSION

static int mod_init(void);
static int mod_child_init(int rank);
static void mod_destroy(void);

/* parameters storage */
int * shutdown_singleton;
char * rf_origin_host_s = "scscf.open-ims.test";
char * rf_origin_realm_s = "open-ims.test";
char * rf_destination_realm_s = "open-ims.test";
char * rf_destination_host_s = "cdf.open-ims.test";
char * rf_service_context_id_root_s = "32260@3gpp.org";
char * rf_service_context_id_ext_s = "ext";
char * rf_service_context_id_mnc_s = "01";
char * rf_service_context_id_mcc_s = "001";
char * rf_service_context_id_release_s = "8";
client_rf_cfg cfg;


#define EXP_FUNC(NAME) \
		{#NAME, (cmd_function)NAME, NO_SCRIPT, 0, 0},

/** 
 * Exported functions.
 *
 */
static cmd_export_t client_rf_cmds[]={
	{"Rf_Send_ACR",			Rf_Send_ACR, 			2, 0, REQUEST_ROUTE|ONREPLY_ROUTE|FAILURE_ROUTE},
	EXP_FUNC(load_client_rf)
	EXP_FUNC(Rf_add_an_chg_info)
	EXP_FUNC(Rf_add_ims_chg_info_icid)
	EXP_FUNC(Rf_get_ims_chg_info_icid)
	EXP_FUNC(Rf_add_ims_chg_ps_info)
	EXP_FUNC(Rf_send_stop_record)
	{0, 0, 0, 0, 0}
}; 

/** 
 * Exported parameters.
 */	
static param_export_t client_rf_params[]={ 
	{"node_functionality", INT_PARAM, &(cfg.node_func)},
	{"origin_host", STR_PARAM, &rf_origin_host_s},
	{"origin_realm", STR_PARAM, &rf_origin_realm_s},
	{"destination_realm", STR_PARAM, &rf_destination_realm_s},
	{"destination_host", STR_PARAM, &rf_destination_host_s},
	{"service_context_id_root", STR_PARAM, &rf_service_context_id_root_s},
	{"service_context_id_ext", STR_PARAM, &rf_service_context_id_ext_s},
	{"service_context_id_mnc", STR_PARAM, &rf_service_context_id_mnc_s},
	{"service_context_id_mcc", STR_PARAM, &rf_service_context_id_mcc_s},
	{"service_context_id_release", STR_PARAM, &rf_service_context_id_release_s},
	{0,0,0} 
};

/** module exports */
struct module_exports exports = {
	"Client_Rf", 
	client_rf_cmds,
	0,
	client_rf_params,
	mod_init,			/* module initialization function */
	0,				/* response function*/
	mod_destroy,			/* destroy function */
	0,				/* onbreak function */
	mod_child_init			/* per-child init function */
};


/* Global variables and imported functions */
cdp_avp_bind_t *cavpb=0;

/**< link to the stateless reply function in sl module */
int (*sl_reply)(struct sip_msg* _msg, char* _str1, char* _str2); 

/**< Structure with pointers to tm funcs 		*/
struct tm_binds tmb;            						

/**
 * Fix the configuration parameters.
 */
int fix_parameters()
{
	cfg.origin_host.s = rf_origin_host_s;
	cfg.origin_host.len = strlen(rf_origin_host_s);

	cfg.origin_realm.s = rf_origin_realm_s;
	cfg.origin_realm.len = strlen(rf_origin_realm_s);

	cfg.destination_realm.s = rf_destination_realm_s;
	cfg.destination_realm.len = strlen(rf_destination_realm_s);

	cfg.destination_host.s = rf_destination_host_s;
	cfg.destination_host.len = strlen(rf_destination_host_s);

	cfg.service_context_id.len = strlen(rf_service_context_id_ext_s) + strlen(rf_service_context_id_mnc_s) +
			strlen(rf_service_context_id_mcc_s) + strlen(rf_service_context_id_release_s) +
			strlen(rf_service_context_id_root_s)+5;
	cfg.service_context_id.s = pkg_malloc(cfg.service_context_id.len*sizeof(char));
	if(!cfg.service_context_id.s){
		cfg.service_context_id.len = 0;
		LOG(L_ERR, "ERR:"M_NAME":fix_parameters: not enough memory!\n");
		return 0;
	}
	cfg.service_context_id.len = sprintf(cfg.service_context_id.s, "%s.%s.%s.%s.%s",
			rf_service_context_id_ext_s, rf_service_context_id_mnc_s, 
			rf_service_context_id_mcc_s, rf_service_context_id_release_s,
			rf_service_context_id_root_s);
	if(cfg.service_context_id.len <0){
		LOG(L_ERR, "ERR:"M_NAME":fix_parameters: error while creating service_context_id\n");
		return 0;
	}

	cfg.hash_table_size = 32;


	return 1;
}


/**
 * Initializes the module.
 */
static int mod_init(void)
{
	load_tm_f load_tm;
	load_cdp_f load_cdp;
	cdp_avp_get_bind_f load_cdp_avp;
			
	LOG(L_INFO,"INFO:"M_NAME":mod_init: Initialization of module\n");
	shutdown_singleton=shm_malloc(sizeof(int));
	*shutdown_singleton=0;
	
	
	/* fix the parameters */
	if (!fix_parameters()) goto error;

	/* load the send_reply function from sl module */
    	sl_reply = find_export("sl_send_reply", 2, 0);
	if (!sl_reply) {
		LOG(L_ERR, "ERR"M_NAME":mod_init: This module requires sl module\n");
		goto error;
	}
	
	/* bind to the tm module */
	if (!(load_tm = (load_tm_f)find_export("load_tm",NO_SCRIPT,0))) {
		LOG(L_ERR, "ERR:"M_NAME":mod_init: Can not import load_tm. This module requires tm module\n");
		goto error;
	}
	if (load_tm(&tmb) == -1)
		goto error;

	if (!(load_cdp = (load_cdp_f)find_export("load_cdp",NO_SCRIPT,0))) {
		LOG(L_ERR, "DBG:"M_NAME":mod_init: Can not import load_cdp. This module requires cdp module.\n");
		goto error;
	}

	if (!(load_cdp_avp = (cdp_avp_get_bind_f)find_export("cdp_avp_get_bind",NO_SCRIPT,0))) {
		LOG(L_ERR, "DBG:"M_NAME":mod_init: loading cdp_avp module unsuccessful. exiting\n");			
		goto error;
	}	
	cavpb = load_cdp_avp();
	if (!cavpb)
		goto error;

	if (!init_acct_records()){
		LOG(L_ERR, "DBG:"M_NAME":mod_init: failed to initiate local accounting records\n");			
		goto error;
	}

	if(!init_an_charg_info()){
		LOG(L_ERR, "DBG:"M_NAME":mod_init: failed to initiate local user charging info\n");			
		goto error;
	}

	if(!init_ims_charg_info()){
		LOG(L_ERR, "DBG:"M_NAME":mod_init: failed to initiate local user charging info\n");			
		goto error;
	}
	
	return 0;
error:
	return -1;
}

extern gen_lock_t* process_lock;		/* lock on the process table */

/**
 * Initializes the module in child.
 */
static int mod_child_init(int rank)
{
	LOG(L_INFO,"INFO:"M_NAME":mod_init: Initialization of module in child [%d] \n",
		rank);
	/* don't do anything for main process and TCP manager process */
	if ( rank == PROC_MAIN || rank == PROC_TCP_MAIN )
		return 0;

	lock_get(process_lock);
		cavpb->cdp->AAAAddResponseHandler(RfChargingResponseHandler, NULL);
	lock_release(process_lock);
			
	return 0;
}

extern gen_lock_t* process_lock;		/* lock on the process table */

/**
 * Destroys the module.
 */
static void mod_destroy(void)
{
	int do_destroy=0;
	LOG(L_INFO,"INFO:"M_NAME":mod_destroy: child exit\n");
	
	lock_get(process_lock);
	if((*shutdown_singleton)==0){
		*shutdown_singleton=1;
		do_destroy=1;
	}
	lock_release(process_lock);

	if (do_destroy){
		/* Then nuke it all */	
		destroy_acct_records();
		destroy_an_charg_info();
		destroy_ims_charg_info();
	}
	
}


/**
 * Checks if the transaction is in processing.
 * @param msg - the SIP message to check
 * @param str1 - not used
 * @param str2 - not used
 * @returns #CSCF_RETURN_TRUE if the transaction is already in processing, #CSCF_RETURN_FALSE if not
 */
int trans_in_processing(struct sip_msg* msg, char* str1, char* str2)
{
	unsigned int hash, label;
	if (tmb.t_get_trans_ident(msg,&hash,&label)<0)
		return CSCF_RETURN_FALSE;
	return CSCF_RETURN_TRUE;	
}

#endif /*OPEN_IMS_CORE*/
