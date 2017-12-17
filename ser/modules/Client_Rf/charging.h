/*
 * $Id: charging.h 1172 2011-10-27 08:42:46Z aon $
 * 
 * Copyright (C) 2009 FhG Fokus
 * 
 * This file is part of the Wharf project.
 * 
 */

/**
 * \file
 * 
 * Wharf CDF_Rf module -  
 * 
 * 
 *  \author Andreea Onofrei Corici andreea dot ancuta dot corici -at- fokus dot fraunhofer dot de
 * 
 */
 
#ifndef _Client_Rf_CHARGING_H
#define _Client_Rf_CHARGING_H

#ifdef WHARF
#include "../../utils/utils.h"
#endif /*WHARF*/

#include "../cdp/diameter.h"
#include "Rf_data.h"

typedef struct _an_charg_info_list_t_slot{
	str sip_uri;
	uint32_t an_charg_id;
	//time_t expires;
	struct _an_charg_info_list_t_slot * next, * prev;
} an_charg_info_list_slot_t;

typedef struct _an_charg_info_list_t{
	gen_lock_t * lock;
	struct _an_charg_info_list_t_slot * head, * tail;
} an_charg_info_list_t;

#define an_charg_info_list_t_free(x,mem) \
do{\
	if (x) {\
		str_free((x)->sip_uri,mem);\
		mem##_free(x);\
		(x) = 0;\
	}\
}while(0)

typedef struct _ims_charg_info_list_t_slot{
	str call_id;
	int dir;
	str ims_charg_id;
	//str af_app_id;
	uint32_t rating_group;
	//time_t expires;
	struct _ims_charg_info_list_t_slot * next, * prev;
} ims_charg_info_list_slot_t;

typedef struct _ims_charg_info_list_t{
	gen_lock_t * lock;
	struct _ims_charg_info_list_t_slot * head, * tail;
} ims_charg_info_list_t;

#define ims_charg_info_list_t_free(x,mem) \
do{\
	if (x) {\
		str_free((x)->call_id,mem);\
		str_free((x)->ims_charg_id,mem);\
		mem##_free(x);\
		(x) = 0;\
	}\
}while(0)

int init_an_charg_info();
void destroy_an_charg_info();
int Rf_add_an_chg_info(str sip_uri, uint32_t an_charg_id);
uint32_t get_an_charg_info(str sip_uri);


int init_ims_charg_info();
void destroy_ims_charg_info();
int Rf_add_ims_chg_info_icid(str call_id, int dir, str ims_charg_id);
int Rf_get_ims_chg_info_icid(str call_id, int dir, str  * ims_charg_id);
int Rf_add_ims_chg_ps_info(str call_id, int dir, uint32_t rating_group);
int get_ims_charg_info(str call_id, int dir, str * icid, uint32_t *rating_group);

#endif
