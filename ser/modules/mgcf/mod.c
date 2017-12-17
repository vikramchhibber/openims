/**
 * $Id: mod.c 676 2009-06-17 06:07:04Z vingarzan $
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
 * MGCF helper functions - SER module interface
 * 
 * Scope:
 * - Exports parameters and functions
 * - Initialization functions
 * 
 *  \author Dragos Vingarzan dragos dot vingarzan -at- fokus dot fraunhofer dot de
 * 
 */
 

#include "mod.h"

#include "../../sr_module.h"
#include "../../timer.h"
#include "../../locking.h"
#include "../tm/tm_load.h"
#include "../dialog/dlg_mod.h"
#include "../../pt.h"


#include "sip.h"
#include "mgcf.h"
#include "dlg_state.h"
#include "release_call.h"

MODULE_VERSION

static int mgcf_mod_init(void);
static int mgcf_mod_child_init(int rank);
static void mgcf_mod_destroy(void);


/** parameters storage */
char* mgcf_name="mgcf.open-ims.test";					/**< name of the MGCF */

char* mgcf_path="sip:term@mgcf.open-ims.test";			/**< name of the MGCF including PSTN termination indicator */

char* mgcf_default_realm="open-ims.test";				/**< default realm for filling if none available	*/

/* P-Charging-Vector parameters */
extern char* cscf_icid_value_prefix;			/**< hexadecimal prefix for the icid-value - must be unique on each node */
extern unsigned int* cscf_icid_value_count;		/**< to keep the number of generated icid-values 	*/
extern gen_lock_t* cscf_icid_value_count_lock;	/**< to lock acces on the above counter				*/
extern char* cscf_icid_gen_addr;				/**< address of the generator of the icid-value 	*/
extern char* cscf_orig_ioi;						/**< name of the Originating network 				*/
extern char* cscf_term_ioi;						/**< name of the Terminating network 				*/

int mgcf_dialogs_hash_size=1024;				/**< the size of the hash table for dialogs			*/
int mgcf_dialogs_expiration_time=3600;			/**< expiration time for a dialog					*/
int mgcf_dialogs_enable_release=1;				/**< if to enable dialog release					*/
int mgcf_min_se=90;								/**< Minimum session-expires accepted value			*/
int* mgcf_dialog_count = 0;						/**< Counter for saved dialogs						*/
int mgcf_max_dialog_count=20000;				/**< Maximum number of dialogs						*/ 
gen_lock_t* mgcf_dialog_count_lock=0; 			/**< Lock for the dialog counter					*/


/* fixed parameter storage */
str mgcf_name_str;				/**< fixed name of the MGCF */
str mgcf_path_str;				/**< fixed path name of the MGCF (for indicating PSTN terminating) */
str mgcf_record_route_mo;		/**< record route header for PSTN origination dialogs */
str mgcf_record_route_mt;		/**< record route header for PSTN termination dialogs */
str mgcf_record_route_mo_uri;	/**< record route header for PSTN origination dialogs */
str mgcf_record_route_mt_uri;	/**< record route header for PSTN termination dialogs */
str mgcf_record_route_mo_body;	/**< record route header for PSTN origination dialogs */
str mgcf_record_route_mt_body;	/**< record route header for PSTN termination dialogs */
str mgcf_default_realm_str;		/**< fixed default realm */

extern str cscf_icid_value_prefix_str;				/**< fixed hexadecimal prefix for the icid-value - must be unique on each node */
extern str cscf_icid_gen_addr_str;					/**< fixed address of the generator of the icid-value */
extern str cscf_orig_ioi_str;						/**< fixed name of the Originating network 			*/
extern str cscf_term_ioi_str;						/**< fixed name of the Terminating network 			*/


/** 
 * Exported functions.
 * - M_trans_in_processing() - if this transaction is already in processing
 * <p>
 * - M_PSTN_terminating() - checks if this is a PSTN terminating call, by looking for the term@mgcf indicator in first Route
 * <p>
 * - M_assert_identity() - Assert the identity of the caller by adding a P-Asserted-Identity header
 * - M_assert_called_identity() - Assert the identity of the called user by adding a P-Called-Party-ID header
 * <p> 
 * - M_add_p_charging_vector() - add a new P-Charging-Vector
 * <p>
 * - M_enforce_record_routes() - enforces the use of the Record-Route headers received in the request to be used in the response 
 * <p>
 * - M_is_in_dialog("orig"/"term") - checks if a requests is in a dialog
 * - M_save_dialog("orig"/"term") - saves a dialog in the local list
 * - M_update_dialog("orig"/"term") - updates a dialog in the local list
 * - M_drop_dialog("orig"/"term") - drops a dialog from the local list
 * - M_follows_dialog_routes("orig"/"term") - checks if a subsequent request follows the saved dialog routes
 * - M_enforce_dialog_routes("orig"/"term") - enforces the dialog routes
 * - M_record_route("orig"/"term") - records route
 * - M_check_session_expires() - Checks if Session-Expires value is over Min_SE local policy
 * - M_422_session_expires() - Return a 422 response with Min_SE set to local policy
 * <p>
 * - M_release_call("orig"/"term",reason) - release a call
 * <p>
 * - M_enforce_dialog_requri("orig"/"term") - enforces the use of the proper Request-URI on subsequent requests, as saved from the contact header
 * <p>
 * - M_record_route_simple("orig"/"term") - simply adds a Record-Route header
 * - M_is_in_dialog_simple("orig"/"term") - simply check if the msg contains in the first Route a mo/mt@mgcf route mark
 * <p>
 * - M_filter_content_type("type/subtype") - filter only the given content-type and remove all others
 * - M_drop_body() - delete the body and the Content-type/length headers.
 */
static cmd_export_t mgcf_cmds[]={
//	{"M_trans_in_processing", 		M_trans_in_processing, 		0, 0, REQUEST_ROUTE}, 
	
	{"M_PSTN_terminating",			M_PSTN_terminating,			0, 0, REQUEST_ROUTE},
	
	{"M_assert_identity",			M_assert_identity,			0, 0, REQUEST_ROUTE},
	{"M_assert_called_identity",	M_assert_called_identity,	0, 0, ONREPLY_ROUTE},
	
	{"M_add_p_charging_vector",		M_add_p_charging_vector, 	0, 0, REQUEST_ROUTE},

	{"M_enforce_record_routes",		M_enforce_record_routes, 	0, 0, ONREPLY_ROUTE},

	{"M_is_in_dialog",				M_is_in_dialog, 			1, 0, REQUEST_ROUTE},
	{"M_save_dialog",				M_save_dialog, 				1, 0, REQUEST_ROUTE},
	{"M_update_dialog",				M_update_dialog, 			1, 0, REQUEST_ROUTE|ONREPLY_ROUTE},
	{"M_drop_dialog",				M_drop_dialog, 				1, 0, REQUEST_ROUTE|ONREPLY_ROUTE|FAILURE_ROUTE},
	{"M_follows_dialog_routes",		M_follows_dialog_routes, 	1, 0, REQUEST_ROUTE},
	{"M_enforce_dialog_routes",		M_enforce_dialog_routes, 	1, 0, REQUEST_ROUTE},
	{"M_record_route",				M_record_route,				1, 0, REQUEST_ROUTE},		
	{"M_check_session_expires",		M_check_session_expires, 	0, 0, REQUEST_ROUTE},
	{"M_422_session_expires",		M_422_session_expires,	 	0, 0, REQUEST_ROUTE},
	
	{"M_release_call",				M_release_call,			 	2, 0, REQUEST_ROUTE|ONREPLY_ROUTE|FAILURE_ROUTE},

	
	{"M_enforce_dialog_requri",		M_enforce_dialog_requri,	1, 0, REQUEST_ROUTE},
	
	{"M_record_route_simple",		M_record_route_simple,		1, 0, REQUEST_ROUTE},		
	{"M_is_in_dialog_simple",		M_is_in_dialog_simple, 		1, 0, REQUEST_ROUTE},
		
	{"M_filter_content_type",		M_filter_content_type,		1, 0, REQUEST_ROUTE|ONREPLY_ROUTE},
	{"M_drop_body",					M_drop_body,				0, 0, REQUEST_ROUTE|ONREPLY_ROUTE},
	
	{0, 0, 0, 0, 0}
};

/**
 * Exported parameters.
 * - name - name of the MGCF
 * <p>
 * - dialogs_hash_size - size of the dialog hash table
 * - dialogs_expiration_time - time-out for dialog expiration
 * - dialogs_enable_release - whether to enable P-CSCF initiated dialog release
 * - max_dialog_count - the maximum number of dialogs to keep, -1 if not limited
 * - min_se - default value for Min_SE header
 * <p>
 * - icid_value_prefix - prefix for the ICID in the P-Charging-Vector header
 * - icid_gen_addr - ICID Gen Addr. in the P-Charging-Vector header
 * - orig_ioi - Originating IOI in the P-Charging-Vector header
 * - term_ioi - Terminating IOI in the P-Charging-Vector header
 */	
static param_export_t mgcf_params[]={ 
	{"name", 					STR_PARAM, &mgcf_name},
	{"path", 					STR_PARAM, &mgcf_path},
	{"default_realm", 			STR_PARAM, &mgcf_default_realm},	

	{"icid_value_prefix",		STR_PARAM, &cscf_icid_value_prefix},
	{"icid_gen_addr",			STR_PARAM, &cscf_icid_gen_addr},
	{"orig_ioi",				STR_PARAM, &cscf_orig_ioi},
	{"term_ioi",				STR_PARAM, &cscf_term_ioi},
	
	{"dialogs_hash_size",		INT_PARAM,	&mgcf_dialogs_hash_size},
	{"dialogs_expiration_time",	INT_PARAM,	&mgcf_dialogs_expiration_time},
	{"dialogs_enable_release",	INT_PARAM,	&mgcf_dialogs_enable_release},
	{"max_dialog_count",		INT_PARAM,	&mgcf_max_dialog_count},
	{"min_se",		 			INT_PARAM, 	&mgcf_min_se},

	{0,0,0} 
};

/** module exports */
struct module_exports exports = {
	"mgcf", 
	mgcf_cmds,
	0,
	mgcf_params,
	
	mgcf_mod_init,		/* module initialization function */
	0,					/* response function*/
	mgcf_mod_destroy,	/* destroy function */
	0,					/* onbreak function */
	mgcf_mod_child_init	/* per-child init function */
};


/** Global variables and imported functions */
int (*sl_reply)(struct sip_msg* _msg, char* _str1, char* _str2); 
										/**< link to the stateless reply function in sl module 	*/

struct tm_binds tmb;          			/**< Structure with pointers to tm funcs 				*/
dlg_func_t dialogb;						/**< Structure with pointers to dialog funcs			*/

static str s_record_route_s={"Record-Route: <",15};
static str s_mo = {"sip:mo@",7};
static str s_mt = {"sip:mt@",7};
static str s_record_route_lr={";lr",3};
static str s_record_route_e={">\r\n",3};

static int fix_parameters()
{
	mgcf_name_str.s = mgcf_name;
	mgcf_name_str.len = strlen(mgcf_name);

	mgcf_path_str.s = mgcf_path;
	mgcf_path_str.len = strlen(mgcf_path);
	
	/* Record-routes */
	mgcf_record_route_mo.s = pkg_malloc(s_record_route_s.len+s_mo.len+mgcf_name_str.len+s_record_route_lr.len+s_record_route_e.len);
	if (!mgcf_record_route_mo.s){
		LOG(L_ERR, "ERR"M_NAME":mod_init: Error allocating %d bytes\n",
			s_record_route_s.len+s_mo.len+mgcf_name_str.len+s_record_route_lr.len+s_record_route_e.len);
		return 0;
	}
	mgcf_record_route_mt.s = pkg_malloc(s_record_route_s.len+s_mt.len+mgcf_name_str.len+s_record_route_lr.len+s_record_route_e.len);
	if (!mgcf_record_route_mt.s){
		LOG(L_ERR, "ERR"M_NAME":mod_init: Error allocating %d bytes\n",
			s_record_route_s.len+s_mt.len+mgcf_name_str.len+s_record_route_lr.len+s_record_route_e.len);
		return 0;
	}
	
	mgcf_record_route_mo.len=0;
	STR_APPEND(mgcf_record_route_mo,s_record_route_s);
	if (mgcf_name_str.len>4 && strncasecmp(mgcf_name_str.s,"sip:",4)==0){
		STR_APPEND(mgcf_record_route_mo,s_mo);
		memcpy(mgcf_record_route_mo.s+mgcf_record_route_mo.len,mgcf_name_str.s+4,
			mgcf_name_str.len-4);
		mgcf_record_route_mo.len += mgcf_name_str.len-4;
	} else {
		STR_APPEND(mgcf_record_route_mo,s_mo);
		STR_APPEND(mgcf_record_route_mo,mgcf_name_str);
	}
	STR_APPEND(mgcf_record_route_mo,s_record_route_lr);
	STR_APPEND(mgcf_record_route_mo,s_record_route_e);
	mgcf_record_route_mo_uri.s = mgcf_record_route_mo.s + s_record_route_s.len;
	mgcf_record_route_mo_uri.len = mgcf_record_route_mo.len - s_record_route_s.len - s_record_route_lr.len - s_record_route_e.len;
	mgcf_record_route_mo_body.s = mgcf_record_route_mo_uri.s-1;
	mgcf_record_route_mo_body.len = mgcf_record_route_mo_uri.len+1 + s_record_route_lr.len + s_record_route_e.len;

	mgcf_record_route_mt.len=0;
	STR_APPEND(mgcf_record_route_mt,s_record_route_s);
	if (mgcf_name_str.len>4 && strncasecmp(mgcf_name_str.s,"sip:",4)==0){
		STR_APPEND(mgcf_record_route_mt,s_mt);
		memcpy(mgcf_record_route_mt.s+mgcf_record_route_mt.len,mgcf_name_str.s+4,
			mgcf_name_str.len-4);
		mgcf_record_route_mt.len += mgcf_name_str.len-4;
	} else {
		STR_APPEND(mgcf_record_route_mt,s_mt);
		STR_APPEND(mgcf_record_route_mt,mgcf_name_str);
	}
	STR_APPEND(mgcf_record_route_mt,s_record_route_lr);
	STR_APPEND(mgcf_record_route_mt,s_record_route_e);
	mgcf_record_route_mt_uri.s = mgcf_record_route_mt.s + s_record_route_s.len;
	mgcf_record_route_mt_uri.len = mgcf_record_route_mt.len - s_record_route_s.len - s_record_route_lr.len - s_record_route_e.len;
	mgcf_record_route_mt_body.s = mgcf_record_route_mt_uri.s-1;
	mgcf_record_route_mt_body.len = mgcf_record_route_mt_uri.len+1+ s_record_route_lr.len + s_record_route_e.len;

	
	mgcf_default_realm_str.s = mgcf_default_realm; 
	mgcf_default_realm_str.len = strlen(mgcf_default_realm);

	cscf_icid_value_prefix_str.s = cscf_icid_value_prefix;
	cscf_icid_value_prefix_str.len = strlen(cscf_icid_value_prefix);

	cscf_icid_gen_addr_str.s = cscf_icid_gen_addr;
	cscf_icid_gen_addr_str.len = strlen(cscf_icid_gen_addr);
	
	cscf_orig_ioi_str.s = cscf_orig_ioi;
	cscf_orig_ioi_str.len = strlen(cscf_orig_ioi);
	
	cscf_term_ioi_str.s = cscf_term_ioi;
	cscf_term_ioi_str.len = strlen(cscf_term_ioi);
	
	return 1;
}

extern m_dialog_hash_slot *m_dialogs;			/**< the dialogs hash table				*/

static int mgcf_mod_init(void)
{
	load_tm_f load_tm;
	bind_dlg_mod_f load_dlg;
	
	
	LOG(L_INFO,"INFO:"M_NAME":mod_init: Initialization of module\n");
	/* fix the parameters */
	if (!fix_parameters()) goto error;

	cscf_icid_value_count = shm_malloc(sizeof(unsigned int));
	*cscf_icid_value_count = 0;
	cscf_icid_value_count_lock = lock_alloc();
	cscf_icid_value_count_lock = lock_init(cscf_icid_value_count_lock);
		
	/* load the send_reply function from sl module */
	sl_reply = find_export("sl_send_reply", 2, 0);
	if (!sl_reply) {
		LOG(L_ERR, "ERR"M_NAME":mod_init: This module requires sl module\n");
		goto error;
	}
	
	/* bind to the tm module */
	if (!(load_tm = (load_tm_f)find_export("load_tm",NO_SCRIPT,0))) {
		LOG(L_ERR, "ERR"M_NAME":mod_init: Can not import load_tm. This module requires tm module\n");
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
	if (!m_dialogs_init(mgcf_dialogs_hash_size)){
		LOG(L_ERR, "ERR"M_NAME":mod_init: Error initializing the Hash Table for stored dialogs\n");
		goto error;
	}		
	mgcf_dialog_count = shm_malloc(sizeof(int));
	*mgcf_dialog_count = 0;
	mgcf_dialog_count_lock = lock_alloc();
	mgcf_dialog_count_lock = lock_init(mgcf_dialog_count_lock);

	/* register the dialog timer */
	if (register_timer(dialog_timer,m_dialogs,60)<0) goto error;

	return 0;
error:
	return -1;
}

static int mgcf_mod_child_init(int rank)
{
	LOG(L_INFO,"INFO:"M_NAME":mod_init: Initialization of module in child [%d] %s \n",
		rank,pt[process_no].desc);
	/* don't do anything for main process and TCP manager process */
	if ( rank == PROC_MAIN || rank == PROC_TCP_MAIN )
		return 0;
	
	return 0;
}

static void mgcf_mod_destroy(void)
{
	LOG(L_INFO,"INFO:"M_NAME":mod_destroy: child exit\n");
	m_dialogs_destroy();
	lock_get(mgcf_dialog_count_lock);
	shm_free(mgcf_dialog_count);
	lock_destroy(mgcf_dialog_count_lock);
}
