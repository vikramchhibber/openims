/*
 * $Id: locsip_subscribe.c 161 2009-07-08 14:06:01Z aon $
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

#include "locsip_subscribe.h"

#include <libxml/xmlschemas.h>
#include <libxml/parser.h>
 

#include "mod.h" 
#include "../../mem/mem.h"
#include "../../mem/shm_mem.h"
#include "../../parser/parse_uri.h"
#include "../../locking.h"
#include "../../modules/tm/tm_load.h"

#ifdef SER_MOD_INTERFACE
        #include "../../modules_s/dialog/dlg_mod.h"
#else
        #include "../dialog/dlg_mod.h"
#endif

#include "sip.h"
#include "user_data.h"

extern int debug;
extern struct tm_binds tmb;   		/**< Structure with pointers to tm funcs 		*/
extern dlg_func_t dialogb;			/**< Structure with pointers to dialog funcs			*/

extern str lrf_name_str;			/**< fixed SIP URI of this P-CSCF 				*/

extern time_t time_now;				/**< current time 								*/

extern int lrf_subscribe_retries;	/**< times to retry subscribe to location on failure */

loc_subscription_hash_slot *subscriptions=0;/**< list of subscriptions					*/
extern int subscriptions_hash_size;	/**< the size of the hash table for subscriptions	*/
time_t time_now;				/**< current time 								*/

void set_dest_uri_dialog(dlg_t *d, str dest);

/**
 * Computes the hash for a contact.
 * @param aor - the string of the target
 * @returns the hash for the contact
 */
inline unsigned int get_subscription_hash(str uri)
{
#define h_inc h+=v^(v>>3)
   char* p;
   register unsigned v;
   register unsigned h;
   h=0;
   for (p=uri.s; p<=(uri.s+uri.len-4); p+=4){
       v=(*p<<24)+(p[1]<<16)+(p[2]<<8)+p[3];
       h_inc;
   }
   v=0;
   for (;p<(uri.s+uri.len); p++) {
       v<<=8;
       v+=*p;
   }
   h_inc;
   h=((h)+(h>>11))+((h>>13)+(h>>23));
   return (h)%subscriptions_hash_size;
#undef h_inc 
}

/**
 * Lock a subscription hash slot.
 * @param hash - index to lock
 */
inline void subs_lock(unsigned int hash)
{
//	LOG(L_CRIT,"GET %d\n",hash);
	lock_get(subscriptions[hash].lock);
//	LOG(L_CRIT,"GOT %d\n",hash);
}
/**
 * UnLock a subscriptions hash slot.
 * @param hash - index to unlock
 */
inline void subs_unlock(unsigned int hash)
{
//	LOG(L_CRIT,"REL %d\n",hash);	
	lock_release(subscriptions[hash].lock);
}
/**
 * Initialize the subscription list.
 * @returns 1 if ok, 0 on error
 */
int loc_subscription_init()
{
	int i;
	subscriptions = shm_malloc(sizeof(loc_subscription_hash_slot)*subscriptions_hash_size);
	if (!subscriptions) return 0;
	memset(subscriptions,0,sizeof(loc_subscription_hash_slot)*subscriptions_hash_size);
	for(i=0;i<subscriptions_hash_size;i++){
		subscriptions[i].lock = lock_alloc();
		if (!subscriptions[i].lock) return 0;
		subscriptions[i].lock = lock_init(subscriptions[i].lock);
	}
	return 1;
}

/**
 * Destroys the subscription list
 */
void loc_subscription_destroy()
{
	int i;
	loc_subscription *s,*ns;
	for(i=0;i<subscriptions_hash_size;i++){
		subs_lock(i);
		s = subscriptions[i].head;
		while(s){
			ns = s->next;
			//TODO send out unSUBSCRIBE
			free_loc_subscription(s);
			s = ns;
		}
		lock_destroy(subscriptions[i].lock);
		lock_dealloc(subscriptions[i].lock);
	}	
	shm_free(subscriptions);
}



/**
 * Creates a subcription and starts the timer resubscription for the given contact.
 * @param uri - the contact to subscribe to (actually to its default public id)
 * @param duration - SUBCRIBE expires
 * @returns 1 on success, 0 on failure
 */
loc_subscription* loc_subscribe(str uri,int duration, user_d * user_data)
{
	loc_subscription *s;
	/* first we try to update. if not found, add it */
	LOG(L_DBG,"DBG:"M_NAME":loc_subscription: begin\n");
	s = get_loc_subscription(uri, user_data);	
	if (s){
		s->duration = duration;
		s->attempts_left=lrf_subscribe_retries;
		subs_unlock(s->hash);
	}else{			
		s = new_loc_subscription(uri,duration, user_data);
		if (!s){
			LOG(L_ERR,"ERR:"M_NAME":loc_subscribe: Error creating new subscription\n");
			return 0;
		}
		add_loc_subscription(s);
		s->attempts_left=lrf_subscribe_retries;
	}
		
	return s;
}


static str method={"SUBSCRIBE",9};
static str event_hdr={"Event: presence\r\n",17};
static str accept_hdr={"Accept: application/pidf+xml\r\n",30};
static str content_len_hdr={"Content-Length: 0\r\n",19};
static str max_fwds_hdr={"Max-Forwards: 10\r\n",18};
static str expires_s={"Expires: ",9};
static str expires_e={"\r\n",2};
static str contact_s={"Contact: <",10};
static str contact_e={">;g.oma.locsip=\"TRUE\"\r\n",23};
/**
 * Send a subscription to a LOCSIP server, according to 
 * http://member.openmobilealliance.org/ftp/public_documents/loc/Permanent_documents/OMA-TS-LOCSIP-V1_0-20100708-D.zip
 * @param s - the subsription to send for
 * @param duration - expires time
 * @returns true if OK, false if not, error on failure
 * \todo store the dialog and reSubscribe on the same dialog
 */
int loc_send_subscribe(loc_subscription *s, str dest, int duration)
{
	str h={0,0};

	LOG(L_DBG,"DBG:"M_NAME":loc_send_subscribe: SUBSCRIBE to <%.*s>\n",
		s->req_uri.len,s->req_uri.s);
	
	h.len = event_hdr.len+accept_hdr.len+content_len_hdr.len+max_fwds_hdr.len;
	h.len += expires_s.len + 12 + expires_e.len;

	h.len += contact_s.len + lrf_name_str.len + contact_e.len;

	h.s = pkg_malloc(h.len);
	if (!h.s){
		LOG(L_ERR,"ERR:"M_NAME":loc_send_subscribe: Error allocating %d bytes\n",h.len);
		h.len = 0;
		return 0;
	}

	h.len = 0;
	STR_APPEND(h,event_hdr);
	STR_APPEND(h,accept_hdr);
	STR_APPEND(h,content_len_hdr);
	STR_APPEND(h,max_fwds_hdr);

	STR_APPEND(h,expires_s);
	sprintf(h.s+h.len,"%d",duration);
	h.len += strlen(h.s+h.len);
	STR_APPEND(h,expires_e);

	STR_APPEND(h,contact_s);
	STR_APPEND(h,lrf_name_str);
	STR_APPEND(h,contact_e);
	
	if (!s->dialog){
		/* this is the first request in the dialog */
		if (tmb.new_dlg_uac(0,0,1,&lrf_name_str,&s->req_uri,&s->dialog)<0){
			LOG(L_ERR,"ERR:"M_NAME":loc_send_subscribe: Error creating a dialog for SUBSCRIBE\n");
			goto error;
		}
		set_dest_uri_dialog(s->dialog, dest);
		if (dialogb.request_outside(&method, &h, 0, s->dialog, loc_subscribe_response,  (void *)s->user_data) < 0){
			LOG(L_ERR,"ERR:"M_NAME":loc_send_subscribe: Error sending initial request in a SUBSCRIBE dialog\n");
			goto error;
		}		
	}else{
		/* this is a subsequent subscribe */
		set_dest_uri_dialog(s->dialog, dest);
		if (dialogb.request_inside(&method, &h, 0, s->dialog, loc_subscribe_response,  (void *)s->user_data) < 0){
			LOG(L_ERR,"ERR:"M_NAME":loc_send_subscribe: Error sending subsequent request in a SUBSCRIBE dialog\n");
			goto error;
		}				
	}

	if (h.s) pkg_free(h.s);
	return 1;

error:
	if (h.s) pkg_free(h.s);
	return 0;
}

void lrf_lock(unsigned int hash);

//get the OPTIONS transaction
//send an error response
void subscription_err_resp(struct cell * t, struct tmcb_params *ps){

	user_d * user_data;
	struct cell* options_trans, * crt_trans;
	enum route_mode crt_rmode;
	unsigned int hash_d;

	user_data = *((user_d **)ps->param);
	hash_d = user_data->hash;
	lrf_lock(hash_d);

	LOG(L_DBG, "DBG:"M_NAME":subscription_err_resp: user_data: %p, user_uri %.*s, OPTIONS trans index: %u label: %u callid: %.*s\n", 
			user_data, user_data->user_uri.len, user_data->user_uri.s,
			user_data->options_tr.hash_index, user_data->options_tr.label, 
			user_data->options_tr.callid.len, user_data->options_tr.callid.s);

	if(tmb.t_enter_ctx(user_data->options_tr.hash_index, user_data->options_tr.label,
		&crt_rmode, MODE_REQUEST, &crt_trans, &options_trans)!=0){
		LOG(L_ERR, "ERR:"M_NAME":subscription_err_resp: could not switch to the OPTIONS transaction\n");
		goto error;
	}

	LOG(L_DBG, "DBG:"M_NAME":subscription_err_resp: OPTIONS trans %p callid: %.*s\n", 
			options_trans, options_trans->callid.len, options_trans->callid.s);

	if(tmb.t_reply(options_trans->uas.request, 405, NO_LOCATION)<0){
		LOG(L_ERR, "ERR:"M_NAME":subscription_err_resp:Could not reply to the OPTIONS request\n");
		goto error;
	}
	LOG(L_ERR, "ERR:"M_NAME":subscription_err_resp: sent a 405 no available location error response\n");

	tmb.t_release(options_trans->uas.request);
error:
	//clean user data
	if(user_data->loc_subscr)
		del_loc_subscription((loc_subscription*)user_data->loc_subscr);
	del_user_data(user_data);
	lrf_unlock(hash_d);
	//set the SUBSCRIBE trans as the current one
	ps->param = NULL;
	return;

}

/*
 * Response callback for subscribe
 * TODO: when no reply send an error response to the E-CSCF?
 */
void loc_subscribe_response(struct cell *t,int type,struct tmcb_params *ps)
{
	str req_uri;
	int expires;
	loc_subscription *s=0;

	LOG(L_DBG,"DBG:"M_NAME":loc_subscribe_response: code %d\n",ps->code);
	if (!ps->rpl || ps->rpl==(void*) -1) {
		LOG(L_ERR,"INF:"M_NAME":loc_subscribe_response: No reply\n");
		if(ps->code == 408){
			LOG(L_DBG,"INF:"M_NAME":loc_subscribe_response: timeout\n");
			subscription_err_resp(t, ps);
		}	
		return;
	}

	if(!cscf_get_to_uri(ps->rpl, &req_uri)){
		LOG(L_ERR,"ERR:"M_NAME":loc_subscribe_response: could not retrieve the to uri\n");
		return;
	}

	str callid = cscf_get_call_id(ps->rpl, NULL);
	if(!callid.len || !callid.s){
		LOG(L_ERR,"ERR:"M_NAME":loc_subscribe_response: could not retrieve the callid header\n");
		//TODO: error handling
		return;
	}

	s = get_loc_subscription_callid(req_uri, callid);
	if (!s){
		LOG(L_ERR,"INF:"M_NAME":loc_subscribe_response: received a SUBSCRIBE response but no subscription for <%.*s>\n",
			req_uri.len,req_uri.s);
		return;
	}
	if (ps->code>=200 && ps->code<300){
		expires = cscf_get_expires_hdr(ps->rpl,0);
		tmb.dlg_response_uac(s->dialog, ps->rpl, IS_TARGET_REFRESH);
		update_loc_subscription(s,expires);
	}else if(ps->code >= 300){
		LOG(L_INFO,"INF:"M_NAME":loc_subscribe_response: SUBSCRIRE error response code %d\n",ps->code);				
		update_loc_subscription(s,0);
		subscription_err_resp(t, ps);
	}else
		goto end;

	if(tmb.t_release(ps->rpl) == -1){
		LOG(L_ERR,"ERR:"M_NAME":loc_subscribe_response: could not release the subscribe transaction\n");
	}else
		LOG(L_ERR,"ERR:"M_NAME":loc_subscribe_response: released the subscribe transaction\n");

end:	
	if (s) subs_unlock(s->hash);		
}


#ifdef WITH_IMS_PM
	static str zero={0,0};
#endif

/**
 * set the destination uri for the subscription dialog 
 * required because the location subscription is not necesarily 
 * sent to the same domain as the user URI
 * @param d - the dialog to br modified
 * @param dest - the new destination uri
 */
void set_dest_uri_dialog(dlg_t *d, str dest)
{
	rr_t *r,*r_new;	
		
	for(r=d->route_set;r!=NULL;r=r->next) {
		r_new=r->next;
		r->next=NULL;
		shm_free_rr(&r);
		d->route_set = r_new;
  	}	

	if(d->dst_uri.s)
		shm_free(d->dst_uri.s);
	d->dst_uri.s = 0;
	d->dst_uri.len = 0;
	
	tmb.calculate_hooks(d);

	STR_SHM_DUP(d->dst_uri, dest, "set_dest_uri_dialog");
	
out_of_memory:
	return;

}

extern str locsip_srv_uri;
/**
 * The Subscription timer looks for almost expired subscriptions and subscribes again.
 * @param ticks - the current time
 * @param param - the generic parameter
 */
void subscription_timer(unsigned int ticks, void* param)
{
	#ifdef WITH_IMS_PM
		int subs_cnt=0;
	#endif
	LOG(L_DBG, "DBG:"M_NAME":subscription_timer\n");
	print_subs(L_INFO);
	#ifdef WITH_IMS_PM
		IMS_PM_LOG01(RD_NbrSubs,subs_cnt);
	#endif	
}

/**
 * Creates a subscription based on the given parameters.
 * @param req_uri - the AOR of the user to subcribe to
 * @param duration - expires time in seconds
 * @returns the loc_notification or NULL on error
 */
loc_subscription* new_loc_subscription(str req_uri,int duration, user_d * user_data)
{
	loc_subscription *s=0;
	
	LOG(L_ERR,"ERR:"M_NAME":new_loc_subscription: begin\n");
	s = shm_malloc(sizeof(loc_subscription));
	if (!s){
		LOG(L_ERR,"ERR:"M_NAME":new_loc_subscription: Error allocating %d bytes\n",
			sizeof(loc_subscription));
		goto error;
	}
	memset(s,0,sizeof(loc_subscription));
	
	STR_SHM_DUP(s->req_uri,req_uri,"new_loc_subscription");
	LOG(L_DBG,"DBG:"M_NAME":new_loc_subscription: initial req uri %.*s, final %.*s\n",
			req_uri.len, req_uri.s, s->req_uri.len, s->req_uri.s);

	if (!s->req_uri.s) goto error;
		
	s->duration = duration;
	s->expires = 0;
	s->user_data = user_data;
	
	return s;
error:
out_of_memory:
	if (s->req_uri.s) shm_free(s->req_uri.s);
	if (s) shm_free(s);	
	return 0;
}

/**
 * Adds a subscription to the list of subscriptions at the end (FIFO).
 * @param s - the subscription to be added
 */
void add_loc_subscription(loc_subscription *s)
{
	if (!s) return;
	s->hash = get_subscription_hash(s->req_uri);
	subs_lock(s->hash);
		s->next = 0;
		s->prev = subscriptions[s->hash].tail;
		if (subscriptions[s->hash].tail) subscriptions[s->hash].tail->next = s;			
		subscriptions[s->hash].tail = s;
		if (!subscriptions[s->hash].head) subscriptions[s->hash].head = s;		
	subs_unlock(s->hash);
}

/**
 * Updates the expiration time of a subscription.
 * \todo Maybe we should use a hash here to index it as this is called for every notification
 * @param aor - aor to look for
 * @param expires - new expiration time
 * @returns 1 if found, 0 if not
 */
int update_loc_subscription(loc_subscription *s,int expires)
{
	LOG(L_DBG,"DBG:"M_NAME":update_loc_subscription: refreshing subscription for <%.*s> [%d]\n",
		s->req_uri.len,s->req_uri.s,expires);
	s->attempts_left = -1;
	//if (expires == 0) del_loc_subscription_nolock(s);
	//else s->expires = expires+time_now;;
	subs_unlock(s->hash);	
	return 1;
}

/**
 * Returns a subscription if it exists
 * \note - this returns with a lock on the subscriptions[s->hash] if found. Don't forget to unlock when done!!!
 * @param aor - AOR to look for
 * @returns 1 if found, 0 if not
 */
loc_subscription* get_loc_subscription(str aor, user_d * user_data)
{
	loc_subscription *s;
	unsigned int hash = get_subscription_hash(aor);
	subs_lock(hash);
		s = subscriptions[hash].head;
		while(s){
			if (s->user_data == user_data && 
				s->req_uri.len == aor.len &&
				strncasecmp(s->req_uri.s,aor.s,aor.len)==0)
			{
				return s;
			}
			s = s->next;
		}
	subs_unlock(hash);	
	return 0;
}

/**
 * Returns a subscription if it exists
 * \note - this returns with a lock on the subscriptions[s->hash] if found. Don't forget to unlock when done!!!
 * @param aor - AOR to look for
 * @returns 1 if found, 0 if not
 */
loc_subscription* get_loc_subscription_callid(str aor, str callid)
{
	loc_subscription *s;

	LOG(L_DBG, "DBG:"M_NAME": get_loc_subscription: aor: %.*s callid %.*s\n",
		aor.len, aor.s, callid.len, callid.s);

	unsigned int hash = get_subscription_hash(aor);
	subs_lock(hash);
		s = subscriptions[hash].head;
		while(s){
			LOG(L_DBG, "DBG:"M_NAME":get_loc_subscription_callid: crt callid is %.*s, compared to %.*s\n",
					s->dialog->id.call_id.len, s->dialog->id.call_id.s,
					callid.len, callid.s);
			if (s->dialog->id.call_id.len == callid.len && 
				s->req_uri.len == aor.len &&
				strncasecmp(s->req_uri.s,aor.s,aor.len)==0 &&
				strncasecmp(s->dialog->id.call_id.s, callid.s, callid.len) == 0)
			{
				return s;
			}
			s = s->next;
		}
	subs_unlock(hash);	
	return 0;
}


/**
 * Finds out if a subscription exists
 * @param aor - AOR to look for
 * @returns 1 if found, 0 if not
 */
int is_loc_subscription(str aor)
{
	loc_subscription *s;
	unsigned int hash = get_subscription_hash(aor);
	subs_lock(hash);
		s = subscriptions[hash].head;
		while(s){
			if (s->req_uri.len == aor.len &&
				strncasecmp(s->req_uri.s,aor.s,aor.len)==0)
			{
				subs_unlock(hash);	
				return 1;
			}
			s = s->next;
		}
	subs_unlock(hash);	
	return 0;
}

/**
 * Deletes a subscription from the list of subscriptions 
 * @param s - the subscription to be deleted
 */
void del_loc_subscription(loc_subscription *s)
{
	if (!s) return;
	subs_lock(s->hash);
		if (subscriptions[s->hash].head == s) subscriptions[s->hash].head = s->next;
		else s->prev->next = s->next;
		if (subscriptions[s->hash].tail == s) subscriptions[s->hash].tail = s->prev;
		else s->next->prev = s->prev;
	subs_unlock(s->hash);
	free_loc_subscription(s);
}

/**
 * Deletes a subscription from the list of subscriptions.
 * \note Must have the lock to do this
 * @param s - the subscription to be deleted
 */
void del_loc_subscription_nolock(loc_subscription *s)
{
	if (!s) return;
	if (subscriptions[s->hash].head == s) subscriptions[s->hash].head = s->next;
	else s->prev->next = s->next;
	if (subscriptions[s->hash].tail == s) subscriptions[s->hash].tail = s->prev;
	else s->next->prev = s->prev;
	free_loc_subscription(s);
}

/**
 * Frees up space taken by a subscription
 * @param s - the subscription to free
 */
void free_loc_subscription(loc_subscription *s)
{
	if (s){
		if (s->req_uri.s) shm_free(s->req_uri.s);
		//if (s->dialog) tmb.free_dlg(s->dialog);
		shm_free(s);
	}
}

void print_subs(int log_level)
{
	loc_subscription *s;
	int i;
	if (debug<log_level) return; /* to avoid useless calls when nothing will be printed */	
	LOG(log_level,ANSI_GREEN"INF:"M_NAME":----------  Subscription list begin ---------\n");
	for(i=0;i<subscriptions_hash_size;i++){
		subs_lock(i);
		s = subscriptions[i].head;
//		r_act_time();
		while(s){
			LOG(log_level,ANSI_GREEN"INF:"M_NAME":[%4u]\tP: <"ANSI_BLUE"%.*s"ANSI_GREEN"> D:["ANSI_CYAN"%5d"ANSI_GREEN"] E:["ANSI_MAGENTA"%5d"ANSI_GREEN"] Att:[%2d]\n",
				s->hash,s->req_uri.len,s->req_uri.s,s->duration,(int)(s->expires-time_now),s->attempts_left);
			s = s->next;			
		}
		subs_unlock(i);
	}
	LOG(log_level,ANSI_GREEN"INF:"M_NAME":----------  Subscription list end -----------\n");	
}

/**
 * Trims spaces and duplicate content into pkg.
 * @param dest - destination
 * @param src - source
 */
static inline void space_trim_dup(str *dest, char *src)
{
	int i;
	dest->s=0;
	dest->len=0;
	if (!src) return;
	dest->len = strlen(src);
	i = dest->len-1;
	while((src[i]==' '||src[i]=='\t') && i>0) 
		i--;
	i=0;
	while((src[i]==' '||src[i]=='\t') && i<dest->len)
		i++;
	dest->len -= i;
	dest->s = pkg_malloc(dest->len);
	if (!dest->s) {
		LOG(L_ERR,"ERR:"M_NAME":space_trim_dup: Out of memory allocating %d bytes\n",dest->len);
		dest->len=0;
		return;
	}
	memcpy(dest->s,src+i,dest->len);
}





