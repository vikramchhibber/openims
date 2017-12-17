/*
 * $Id: mod_export.h 1172 2011-10-27 08:42:46Z aon $
 * 
 * Copyright (C) 2009 FhG Fokus
 * 
 * This file is part of the Wharf project.
 * 
 */

/**
 * \file
 * 
 * Client_Rf Wharf module interface
 * 
 * 
 *  \author Andreea Ancuta Corici andreea dot ancuta dot corici -at- fokus dot fraunhofer dot de
 * 
 */


#ifdef WHARF
#ifndef _Client_Rf_EXPORT__H
#define _Client_Rf_EXPORT__H

#include "../cdp/session.h"
#include "Rf_data.h"

typedef int (*AAASendAccRequest_f)(str *session_id, Rf_ACR_t * rf_data);

typedef int (*Rf_add_an_chg_info_f)(str sip_uri, uint32_t an_charg_id);
typedef int (*Rf_add_ims_chg_info_icid_f)(str callid, int dir, str ims_charg_id);
typedef int (*Rf_get_ims_chg_info_icid_f)(str callid, int dir, str  * ims_charg_id);
typedef int (*Rf_add_ims_chg_ps_info_f) (str call_id, int dir, uint32_t rating_group);

typedef Rf_ACR_t* (*Rf_create_Rf_data_f) (str sessionid, int32_t acct_record_type,
											ps_report_charging_data_t * charging_data, qos_info_t *qos);
typedef void (*Rf_free_Rf_data_f) (Rf_ACR_t *);

typedef uint32_t (*gen_new_charg_id_f) (str user_id, str instance_id);
typedef void (*release_charg_id_f) (str user_id, str instance_id);


struct client_rf_bind_t{
	AAASendAccRequest_f	AAASendACR;
	Rf_add_an_chg_info_f	Rf_add_an_chg_info;
	Rf_add_ims_chg_info_icid_f	Rf_add_ims_chg_info_icid;
	Rf_get_ims_chg_info_icid_f	Rf_get_ims_chg_info_icid;
	Rf_add_ims_chg_ps_info_f	Rf_add_ims_chg_ps_info;
	Rf_create_Rf_data_f		create_Rf_data;
	Rf_free_Rf_data_f		free_Rf_data;
	gen_new_charg_id_f		gen_new_charging_id;
	release_charg_id_f		release_charging_id;
};

#endif
#endif /* WHARF */

