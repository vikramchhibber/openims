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

#ifndef LRF_USER_DATA_H
#define LRF_USER_DATA_H

#include "../../str.h"
#include "../../locking.h"
#include "../../ut.h"
#ifdef SER_MOD_INTERFACE
	#include "../lib/lost/client.h"
	#include "../lib/lost/parsing.h"
	#include "../lib/lost/pidf_loc.h"
#else
	#include "../../lib/lost/client.h"
	#include "../../lib/lost/parsing.h"
	#include "../../lib/lost/pidf_loc.h"
#endif

struct trans_info {
	/* tells in which hash table entry the initial transaction lives */
	unsigned int  hash_index;
	/* sequence number within hash collision slot */
	unsigned int  label;
	/* callid of the initial trans*/
	str callid;
};

/** Structure for a LRF user data cell */
typedef struct user_d_cell{
	unsigned int hash;
	str esqk;
	str user_uri;
	str service; /*e.g urn:service:sos*/
	str psap_uri;
	str loc_str;
	xmlNode * loc;
	unsigned int locsip_loc;
	loc_fmt l_fmt;
	struct trans_info options_tr;
	void * loc_subscr;
	struct user_d_cell * next;
	struct user_d_cell * prev;
}user_d;

/** Structure for a LRF user data hash slot */
typedef struct {
	user_d *head;						/**< first dialog in this dialog hash slot 		*/
	user_d *tail;						/**< last dialog in this dialog hash slot 		*/
	gen_lock_t *lock;					/**< slot lock 									*/	
} user_d_hash_slot;

int init_lrf_user_data(int hash_size);
void destroy_lrf_user_data();

user_d * add_user_data(str user_uri, str service, struct trans_info * options_tr);
/*get_user_data acquires the lock of the d->hash also. Do not forget to unlock it*/
user_d * get_user_data(str user_uri, str service, str callid);
void lrf_unlock(unsigned int i);
void del_user_data(user_d *d);

void print_lrf_user_data();
void user_data_timer(unsigned int ticks, void* param);

int LRF_alloc_user_data(struct sip_msg* msg, char*str1, char*str2);
int LRF_del_user_data(struct sip_msg* msg, char* str1, char* str2);
int LRF_call_query_resp(struct sip_msg* msg, char*str1, char*str2);
int LRF_options_empty_repl(struct sip_msg* msg, char*str1, char*str2);

#endif
