/*
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
 * author Ancuta Onofrei, 
 * 	email andreea dot ancuta dot onofrei -at- fokus dot fraunhofer dot de
 */

/**
 * \file
 * 
 * emergency-CSCF - SER module interface
 * 
 * Scope:
 * - Exports parameters and functions
 * - Initialization functions
 * 
 *  \author Ancuta Onofrei ancuta_onofrei -at- yahoo dot com
 * 
 */
 

#include <ctype.h>
#include <sys/types.h>
#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

#include "mod.h"

#include "../../sr_module.h"
#include "../../socket_info.h"
#include "../../timer.h"
#include "../../locking.h"
#include "../../modules/tm/tm_load.h"

#ifdef SER_MOD_INTERFACE
	#include "../../modules_s/dialog/dlg_mod.h"
	#include "../lib/lost/client.h"
#else
	#include "../../modules/dialog/dlg_mod.h"
	#include "../../lib/lost/client.h"
#endif

#include "dlg_state.h"
#include "lrf.h"
#include "route.h"

MODULE_VERSION

static int mod_init(void);
static int mod_child_init(int rank);
static void mod_destroy(void);

/* parameters storage */
char* ecscf_name="sip:ecscf.open-ims.test:7060";	/**< SIP URI of this E-CSCF */
int ecscf_dialogs_hash_size=1024;			/**< the size of the hash table for dialogs			*/
int ecscf_dialogs_expiration_time=3600;		/**< expiration time for a dialog					*/
int ecscf_dialogs_enable_release=1;			/**< if to enable dialog release					*/
int ecscf_min_se=90;						/**< Minimum session-expires accepted value			*/
int* ecscf_dialog_count = 0;				/**< Counter for saved dialogs						*/
int ecscf_max_dialog_count=20000;			/**< Maximum number of dialogs						*/ 
gen_lock_t* ecscf_dialog_count_lock=0; 		/**< Lock for the dialog counter					*/
int * shutdown_singleton;				/**< Shutdown singleton 	*/
char* lrf_sip_uri = "sip:lrf.open-ims.test:8060";	/**< sip uri or the LRF>*/
str lrf_sip_uri_str={NULL, 0};
int use_default_psap = 1;				/* policy to enable/disable using a default PSAP */
char * default_psap_uri = "sip:default_psap@open-ims.test";	/* the URI for the default PSAP */
str default_psap_uri_str = {0,0};
enum user_id_type user_id = SIP_URI_ID;

str ecscf_name_str;					/**< SIP URI of the node>*/
str ecscf_record_route_mo;					/**< Record-route for originating case 				*/
str ecscf_record_route_mo_uri;				/**< URI for Record-route originating				*/ 
str ecscf_record_route_mt;					/**< Record-route for terminating case 				*/
str ecscf_record_route_mt_uri;				/**< URI for Record-route terminating				*/


int E_trans_in_processing(struct sip_msg* msg, char* str1, char* str2);

/** 
 * Exported functions.
 * int E_trans_in_processing(struct sip_msg* msg, char* str1, char* str2)
 * int E_is_in_dialog(struct sip_msg* msg, char* str1, char* str2)
 * E_save_dialog
 * E_update_dialog
 * E_drop_dialog
 * E_get_psap - checks if there is a location information 
 * and if so sends a LoST request and parses the LoST response, sets the psap_uri to the dialog information
 * E_fwd_to_psap - adds Route header containing the psap_uri, modifies the R-URI
 * E_add_record_route - add a Record-Route header containing the SIP URI of the ecscf for mobile orig
 */
static cmd_export_t ecscf_cmds[]={
	{"E_is_in_dialog",				E_is_in_dialog, 			1, 0, REQUEST_ROUTE},
	{"E_is_anonymous_user",				E_is_anonymous_user, 			0, 0, REQUEST_ROUTE},
	{"E_trans_in_processing",			E_trans_in_processing, 			0, 0, REQUEST_ROUTE},
	{"E_save_dialog",				E_save_dialog,				2, 0, REQUEST_ROUTE},
	{"E_update_dialog",				E_update_dialog, 			1, 0, REQUEST_ROUTE|ONREPLY_ROUTE},
	{"E_drop_dialog",				E_drop_dialog, 				1, 0, REQUEST_ROUTE|ONREPLY_ROUTE|FAILURE_ROUTE},
	{"E_get_location",				E_get_location,				1, 0, REQUEST_ROUTE},
	{"E_query_LRF",					E_query_LRF,				1, 0, REQUEST_ROUTE},
	{"E_add_rr",					E_add_record_route,			0, 0, REQUEST_ROUTE},
	{"E_del_ESQK_info",				E_del_ESQK_info,			0, 0, REQUEST_ROUTE},
	{"E_replace_to_header",				E_replace_to_header,			1, 0, REQUEST_ROUTE |ONREPLY_ROUTE |FAILURE_ROUTE},
	{"E_replace_from_header",			E_replace_from_header,			1, 0, REQUEST_ROUTE |ONREPLY_ROUTE |FAILURE_ROUTE},
	{"E_fwded_dialog",				E_fwded_dialog,				1, 0, REQUEST_ROUTE},
	{0, 0, 0, 0, 0}
}; 

/** 
 * Exported parameters.
 * - name - name of the E-CSCF
 */	
static param_export_t ecscf_params[]={ 
	{"name", 			STR_PARAM, 		&ecscf_name},
	{"dialogs_hash_size",		INT_PARAM,		&ecscf_dialogs_hash_size},
	{"dialogs_expiration_time",	INT_PARAM,		&ecscf_dialogs_expiration_time},
	{"dialogs_enable_release",	INT_PARAM,		&ecscf_dialogs_enable_release},
	{"max_dialog_count",		INT_PARAM,		&ecscf_max_dialog_count},
	{"min_se", 			INT_PARAM, 		&ecscf_min_se},
	{"lrf_sip_uri",			STR_PARAM, 		&lrf_sip_uri},
	{"use_default_psap",		INT_PARAM, 		&use_default_psap},
	{"user_id_type",		INT_PARAM, 		&user_id},
	{"default_psap_uri",		STR_PARAM, 		&default_psap_uri},
	{0,0,0} 
};

/** module exports */
struct module_exports exports = {
	"ecscf", 
	ecscf_cmds,
	0,
	ecscf_params,
	
	mod_init,			/* module initialization function */
	0,				/* response function*/
	mod_destroy,			/* destroy function */
	0,				/* onbreak function */
	mod_child_init			/* per-child init function */
};


/* Global variables and imported functions */
int (*sl_reply)(struct sip_msg* _msg, char* _str1, char* _str2); 
										/**< link to the stateless reply function in sl module */

struct tm_binds tmb;            						/**< Structure with pointers to tm funcs 		*/
dlg_func_t dialogb;								/**< Structure with pointers to dialog funcs			*/
extern e_dialog_hash_slot *e_dialogs;						/**< the dialogs hash table				*/


static str path_str_s={"Path: <",7};
static str path_str_1={"sip:term@",9};
static str path_str_e={";lr>\r\n",6};

static str s_record_route_s={"Record-Route: <",15};
static str s_mo = {"sip:mo@",7};
static str s_mt = {"sip:mt@",7};
static str s_record_route_e={";lr>\r\n",6};

/**
 * Fix the configuration parameters.
 */
int fix_parameters()
{
	ecscf_name_str.s = ecscf_name;
	ecscf_name_str.len = strlen(ecscf_name);

	lrf_sip_uri_str.s = lrf_sip_uri;
	lrf_sip_uri_str.len = strlen(lrf_sip_uri);
	
	LOG(L_DBG, "DBG:"M_NAME":fix_parameters: lrf uri: %.*s\n", lrf_sip_uri_str.len, lrf_sip_uri_str.s);

	/* Record-routes */
	ecscf_record_route_mo.s = pkg_malloc(s_record_route_s.len+s_mo.len+ecscf_name_str.len+s_record_route_e.len);
	if (!ecscf_record_route_mo.s){
		LOG(L_ERR, "ERR"M_NAME":fix_parameters: Error allocating %d bytes\n",
			s_record_route_s.len+s_mo.len+ecscf_name_str.len+s_record_route_e.len);
		return 0;
	}
	ecscf_record_route_mt.s = pkg_malloc(s_record_route_s.len+s_mt.len+ecscf_name_str.len+s_record_route_e.len);
	if (!ecscf_record_route_mt.s){
		LOG(L_ERR, "ERR"M_NAME":fix_parameters: Error allocating %d bytes\n",
			s_record_route_s.len+s_mt.len+ecscf_name_str.len+s_record_route_e.len);
		return 0;
	}
	
	ecscf_record_route_mo.len=0;
	STR_APPEND(ecscf_record_route_mo,s_record_route_s);
	if (ecscf_name_str.len>4 && strncasecmp(ecscf_name_str.s,"sip:",4)==0){
		STR_APPEND(ecscf_record_route_mo,s_mo);
		memcpy(ecscf_record_route_mo.s+ecscf_record_route_mo.len,ecscf_name_str.s+4,
			ecscf_name_str.len-4);
		ecscf_record_route_mo.len += ecscf_name_str.len-4;
	} else {
		STR_APPEND(ecscf_record_route_mo,s_mo);
		STR_APPEND(ecscf_record_route_mo,ecscf_name_str);
	}
	STR_APPEND(ecscf_record_route_mo,s_record_route_e);
	ecscf_record_route_mo_uri.s = ecscf_record_route_mo.s + s_record_route_s.len;
	ecscf_record_route_mo_uri.len = ecscf_record_route_mo.len - s_record_route_s.len - s_record_route_e.len;

	ecscf_record_route_mt.len=0;
	STR_APPEND(ecscf_record_route_mt,s_record_route_s);
	if (ecscf_name_str.len>4 && strncasecmp(ecscf_name_str.s,"sip:",4)==0){
		STR_APPEND(ecscf_record_route_mt,s_mt);
		memcpy(ecscf_record_route_mt.s+ecscf_record_route_mt.len,ecscf_name_str.s+4,
			ecscf_name_str.len-4);
		ecscf_record_route_mt.len += ecscf_name_str.len-4;
	} else {
		STR_APPEND(ecscf_record_route_mt,s_mt);
		STR_APPEND(ecscf_record_route_mt,ecscf_name_str);
	}
	STR_APPEND(ecscf_record_route_mt,s_record_route_e);
	ecscf_record_route_mt_uri.s = ecscf_record_route_mt.s + s_record_route_s.len;
	ecscf_record_route_mt_uri.len = ecscf_record_route_mt.len - s_record_route_s.len - s_record_route_e.len;

	if(use_default_psap){
		default_psap_uri_str.s = default_psap_uri;
		default_psap_uri_str.len = strlen(default_psap_uri);
	}

	return 1;
}


/**
 * Initializes the module.
 */
static int mod_init(void)
{
	load_tm_f load_tm;
	bind_dlg_mod_f load_dlg;
			
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

	/* bind to the dialog module */
	load_dlg = (bind_dlg_mod_f)find_export("bind_dlg_mod", -1, 0);
	if (!load_dlg) {
		LOG(L_ERR, "ERR:"M_NAME":mod_init:  Can not import bind_dlg_mod. This module requires dialog module\n");
		return -1;
	}
	if (load_dlg(&dialogb) != 0) {
		return -1;
	}

	/* init the dialog storage */
	if (!e_dialogs_init(ecscf_dialogs_hash_size)){
		LOG(L_ERR, "ERR"M_NAME":mod_init: Error initializing the Hash Table for stored dialogs\n");
		goto error;
	}		
	ecscf_dialog_count = shm_malloc(sizeof(int));
	*ecscf_dialog_count = 0;
	ecscf_dialog_count_lock = lock_alloc();
	ecscf_dialog_count_lock = lock_init(ecscf_dialog_count_lock);

	/* register the dialog timer */
	if (register_timer(dialog_timer,e_dialogs,60)<0) goto error;

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
		e_dialogs_destroy();
        	lock_get(ecscf_dialog_count_lock);
	        shm_free(ecscf_dialog_count);
        	lock_destroy(ecscf_dialog_count_lock);
	}
	
}


/**
 * Checks if the transaction is in processing.
 * @param msg - the SIP message to check
 * @param str1 - not used
 * @param str2 - not used
 * @returns #CSCF_RETURN_TRUE if the transaction is already in processing, #CSCF_RETURN_FALSE if not
 */
int E_trans_in_processing(struct sip_msg* msg, char* str1, char* str2)
{
	unsigned int hash, label;
	if (tmb.t_get_trans_ident(msg,&hash,&label)<0)
		return CSCF_RETURN_FALSE;
	return CSCF_RETURN_TRUE;	
}

