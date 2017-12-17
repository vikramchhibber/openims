/**
 * $Id: dlg_state.h 708 2009-04-23 15:31:52Z aon $
 *  
 * Copyright (C) 2009 FhG Fokus
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
 * LRF - Dialog State
 * 
 *  \author Andreea Ancuta Onofrei andreea.ancuta.onofrei-at-fokus.fraunhofer.de
 *   
 */
 

#ifndef LRF_DLG_STATE_H
#define LRF_DLG_STATE_H

#include "../../sr_module.h"
#include "mod.h"
#include "../../locking.h"
#include "../../modules/tm/dlg.h"
#include "../../modules/tm/tm_load.h"

enum lrf_dialog_method {
	DLG_METHOD_OTHER=0,
	DLG_METHOD_INVITE=1,
	DLG_METHOD_SUBSCRIBE=2	
};

/** The last dialog type */
#define DLG_METHOD_MAX DLG_METHOD_SUBSCRIBE

enum lrf_dialog_state {
	DLG_STATE_UNKNOWN=0,
	DLG_STATE_INITIAL=1,
	DLG_STATE_EARLY=2,
	DLG_STATE_CONFIRMED=3,
	DLG_STATE_TERMINATED_ONE_SIDE=4,
	DLG_STATE_TERMINATED=5	
};

enum lrf_dialog_direction {
	DLG_MOBILE_ORIGINATING=0,
	DLG_MOBILE_TERMINATING=1,
	DLG_MOBILE_UNKNOWN=2
};

typedef struct _lrf_dialog {
	unsigned int hash;
	str call_id;
	str target_uri;
	enum lrf_dialog_direction direction; 
	
	enum lrf_dialog_method method;
	str method_str;
	int first_cseq;
	int last_cseq;
	enum lrf_dialog_state state;
	time_t expires;
	time_t lr_session_expires;  		/**< last remember request - session-expires header			*/
	str refresher;						/**< session refresher				*/
	unsigned char uac_supp_timer; 		/** < requester uac supports timer */
	
	unsigned char is_releasing;			/**< weather this dialog is already being 
	  										released or not, or its peer, with count on 
											tries 										*/	
	str *routes;
	unsigned short routes_cnt;

	dlg_t *dialog_s;  /* dialog as UAS*/
	dlg_t *dialog_c;  /* dialog as UAC*/
			
	struct _lrf_dialog *next,*prev;	
} lrf_dialog;

typedef struct {
	lrf_dialog *head,*tail;
	gen_lock_t *lock;				/**< slot lock 					*/	
} lrf_dialog_hash_slot;


inline unsigned int get_lrf_dialog_hash(str call_id);

int lrf_dialogs_init(int hash_size);

void lrf_dialogs_destroy();

inline void d_lock(unsigned int hash);
inline void d_unlock(unsigned int hash);

lrf_dialog* new_lrf_dialog(str call_id, str target_URI);
lrf_dialog* add_lrf_dialog(str call_id, str target_URI);
//int is_lrf_dialog(str call_id, str target_URI, enum p_dialog_direction *dir);
int is_lrf_dialog_dir(str call_id,enum lrf_dialog_direction dir);

lrf_dialog* get_lrf_dialog(str call_id, enum lrf_dialog_direction *dir);
lrf_dialog* get_lrf_dialog_dir(str call_id,enum lrf_dialog_direction dir);
lrf_dialog* get_lrf_dialog_dir_nolock(str call_id,enum lrf_dialog_direction dir);
//int terminate_lrf_dialog(lrf_dialog *d);
void del_lrf_dialog(lrf_dialog *d);
void free_lrf_dialog(lrf_dialog *d);
void print_lrf_dialogs(int log_level);
		


int LRF_is_in_dialog(struct sip_msg* msg, char* str1, char* str2);

int LRF_save_dialog(struct sip_msg* msg, char* str1, char* str2);

int LRF_update_dialog(struct sip_msg* msg, char* str1, char* str2);

int LRF_drop_dialog(struct sip_msg* msg, char* str1, char* str2);

void dialog_timer(unsigned int ticks, void* param);
		
#endif
