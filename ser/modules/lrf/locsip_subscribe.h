/*
 * $Id: locsip_subscribe.h 161 2009-07-08 14:06:01Z aon $
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
 * LRF : subscription to location information interface
 *
 * \author Andreea Ancuta Onofrei andreea.ancuta.onofrei -at- fokus.fraunhofer.de
 */

 
#ifndef LRF_LOCSIP_SUBSCRIBE_H_
#define LRF_LOCSIP_SUBSCRIBE_H_

#include "../../sr_module.h"
#include "../../locking.h"
#include "../../modules/tm/tm_load.h"

#include "user_data.h"

#define NO_LOCATION "no location was retrieved, could not get a PSAP"


/** LOCINFO constants */
enum {
	IMS_LOCNFO_FULL,
	IMS_LOCINFO_PARTIAL,
	IMS_LOCINFO_ACTIVE,
	IMS_LOCINFO_TERMINATED
}IMS_LOCinfo_states;

/** Event types for "location" to generated notifications after */
enum {
	IMS_LOCINFO_NONE,					/**< no event - donothing */	
	IMS_LOCINFO_SUBSCRIBE,				/**< Initial SUBSCRIBE - just send all data - this should not be treated though */
	IMS_LOCINFO_SUBSCRIBE_EXPIRED,		/**< The subscribe has expired */
	IMS_LOCINFO_EM_CALL_ENDED		/*the emergency call has been ended*/
}IMS_Subscriber_events;

/** location Subscription Structure */
typedef struct _loc_subscription {
	unsigned int hash;
	str req_uri;			/**< public id of the user, same thing for To: 	*/
	int duration;			/**< duration of subscription					*/
	time_t expires;			/**< time of expiration							*/
	char attempts_left;		/**< number of unsuccesful attempts to subscribe*/
	user_d * user_data;
	dlg_t *dialog; 
	struct _loc_subscription *next, *prev;
} loc_subscription;

/** Subscription list */
typedef struct {	
	gen_lock_t *lock;		/**< lock fo subscription list operations 	*/
	loc_subscription *head;	/**< first subscription in the list			*/
	loc_subscription *tail;	/**< last subscription in the list			*/ 
} loc_subscription_hash_slot;

void subs_lock(unsigned int hash);
void subs_unlock(unsigned int hash);
unsigned int get_subscription_hash(str uri);
int loc_subscription_init();
void loc_subscription_destroy();


loc_subscription* loc_subscribe(str uri,int duration, user_d *);
int loc_send_subscribe(loc_subscription *s,str route, int duration);

void loc_subscribe_response(struct cell *t,int type,struct tmcb_params *ps);

void subscription_timer(unsigned int ticks, void* param);

loc_subscription* new_loc_subscription(str req_uri,int duration, user_d * user_data);
void add_loc_subscription(loc_subscription *s);
int update_loc_subscription(loc_subscription *s,int expires);
loc_subscription* get_loc_subscription(str aor, user_d * user_data);
loc_subscription* get_loc_subscription_callid(str aor, str callid);
int is_loc_subscription(str aor);
void del_loc_subscription(loc_subscription *s);
void del_loc_subscription_nolock(loc_subscription *s);
void free_loc_subscription(loc_subscription *s);
void print_subs(int log_level);

int parser_init(char *dtd_filename);
void parser_destroy();


#endif //LRF_LOCSIP_SUBSCRIBE_H
