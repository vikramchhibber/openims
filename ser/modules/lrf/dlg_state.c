/**
 * $Id: dlg_state.c 686 2009-07-14 13:52:16Z aon $
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
 * LRF - Dialog State
 * 
 *  \author Dragos Vingarzan vingarzan -at- fokus dot fraunhofer dot de
 *  \author Alberto Diez - Support of release_call
 *  \author Andreea Ancuta Onofrei andreea.ancuta.onofrei -at- fokus.frunhofer.de - adapted to LRF purposes
 */
 
#include <time.h>

#include "dlg_state.h"

#include "../../mem/shm_mem.h"

#ifdef SER_MOD_INTERFACE
	#include "../../modules_s/sl/sl_funcs.h"
#else	
	#include "../sl/sl_funcs.h"
#endif

#include "sip.h"
//#include "release_call.h"

int lrf_dialogs_hash_size;					/**< size of the dialogs hash table 	*/
lrf_dialog_hash_slot * lrf_dialogs=0;			/**< the dialogs hash table				*/

extern int lrf_dialogs_expiration_time;	/**< expiration time for a dialog		*/
extern int lrf_dialogs_enable_release;	/**< if to enable dialog release		*/
time_t d_time_now;							/**< current time for dialog updates 	*/

extern str lrf_name_str;					/**< fixed SIP URI of this P-CSCF 		*/

extern struct tm_binds tmb;
//extern int lrf_min_se;


int (*sl_reply)(struct sip_msg* _msg, char* _str1, char* _str2); 
int supports_extension(struct sip_msg *m, str *extension);
int requires_extension(struct sip_msg *m, str *extension);

#define strtotime(src,dest) \
{\
	int i;\
	(dest)=0;\
	for(i=0;i<(src).len;i++)\
		if ((src).s[i]>='0' && (src).s[i]<='9')\
			(dest) = (dest)*10 + (src).s[i] -'0';\
}


#define FParam_INT(val) { \
	 .v = { \
		.i = val \
	 },\
	.type = FPARAM_INT, \
	.orig = "int_value", \
};

#define FParam_STRING(val) { \
	.v = { \
		.str = STR_STATIC_INIT(val) \
	},\
	.type = FPARAM_STR, \
	.orig = val, \
};


/**
 * Computes the hash for a string.
 * @param call_id - input string
 * @returns - the hash
 */
inline unsigned int get_lrf_dialog_hash(str call_id)
{
	if (call_id.len==0) return 0;
#define h_inc h+=v^(v>>3)
   char* p;
   register unsigned v;
   register unsigned h;

   h=0;
   for (p=call_id.s; p<=(call_id.s+call_id.len-4); p+=4){
       v=(*p<<24)+(p[1]<<16)+(p[2]<<8)+p[3];
       h_inc;
   }
   v=0;
   for (;p<(call_id.s+call_id.len); p++) {
       v<<=8;
       v+=*p;
   }
   h_inc;

   h=((h)+(h>>11))+((h>>13)+(h>>23));
   return (h)%lrf_dialogs_hash_size;
#undef h_inc 
}

/**
 * Initialize the registrar.
 * @param hash_size - the number of hash table cells
 * @returns 1 on success, 0 on error
 */
int lrf_dialogs_init(int hash_size)
{
	int i;
	
	lrf_dialogs_hash_size = hash_size;
	lrf_dialogs = shm_malloc(sizeof(lrf_dialog_hash_slot)*lrf_dialogs_hash_size);

	if (!lrf_dialogs) return 0;

	memset(lrf_dialogs,0,sizeof(lrf_dialog_hash_slot)*lrf_dialogs_hash_size);
	
	for(i=0;i<lrf_dialogs_hash_size;i++){
		lrf_dialogs[i].lock = lock_alloc();
		if (!lrf_dialogs[i].lock){
			LOG(L_ERR,"ERR:"M_NAME":d_hash_table_init(): Error creating lock\n");
			return 0;
		}
		lrf_dialogs[i].lock = lock_init(lrf_dialogs[i].lock);
	}
			
	return 1;
}

/**
 * Destroy the registrar.
 */
void lrf_dialogs_destroy()
{
	int i;
	lrf_dialog *d,*nd;
	for(i=0;i<lrf_dialogs_hash_size;i++){
		d_lock(i);
			d = lrf_dialogs[i].head;
			while(d){
				nd = d->next;
				free_lrf_dialog(d);
				d = nd;
			}
		d_unlock(i);
		lock_dealloc(lrf_dialogs[i].lock);
	}
	shm_free(lrf_dialogs);
}

/**
 * Locks the required part of the hash table.
 * @param hash - hash of the element to lock (hash slot number)
 */
inline void d_lock(unsigned int hash)
{
//	LOG(L_CRIT,"GET %d\n",hash);
	lock_get(lrf_dialogs[(hash)].lock);
//	LOG(L_CRIT,"GOT %d\n",hash);	
}

/**
 * UnLocks the required part of the hash table.
 * @param hash - hash of the element to lock (hash slot number)
 */
 inline void d_unlock(unsigned int hash)
{
	lock_release(lrf_dialogs[(hash)].lock);
//	LOG(L_CRIT,"RELEASED %d\n",hash);	
}

/**
 * Actualize the current time.
 * @returns the current time
 */
inline int d_act_time()
{
	d_time_now=time(0);
	return d_time_now;
}

extern int* lrf_dialog_count;
extern int lrf_max_dialog_count;
extern gen_lock_t* l_dialog_count_lock;

/**
 * Locks the dialog counter variable
 */
inline void lrf_dialog_count_lock()
{
	lock_get(l_dialog_count_lock);
}

/**
 * UnLocks the dialog counter variable
 */
inline void lrf_dialog_count_unlock()
{
        lock_release(l_dialog_count_lock);
}


/**
 * Try to increment the dialog count
 * @returns 1 on success or 0 if the total number of dialogs is already reached
 */
inline int lrf_dialog_count_increment ()
{
    if (lrf_max_dialog_count<0) return 1;
    lrf_dialog_count_lock();	
	if (*lrf_dialog_count<lrf_max_dialog_count){
    	(*lrf_dialog_count)++;
    	lrf_dialog_count_unlock();
    	return 1;
	} else {
    	lrf_dialog_count_unlock();
    	return 0;
	}
	LOG(L_DBG,"DBG:"M_NAME":lrf_dialog_count_increment():LRF Dialog counter value is %d\n", *lrf_dialog_count);
}

/**
 * Decrement the dialog count
 */
inline void lrf_dialog_count_decrement()
{
    if (lrf_max_dialog_count<0) return ;
    lrf_dialog_count_lock();
    (*lrf_dialog_count)--;
    lrf_dialog_count_unlock();
	LOG(L_DBG,"DBG:"M_NAME":lrf_dialog_count_decrement(): LRF Dialog counter value is %d\n", *lrf_dialog_count);    
}


/**
 * Creates a new lrf_dialog structure.
 * Does not add the structure to the list.
 * @param call_id - call-id of the dialog
 * @param target_uri - target of the dialog
 * @returns the new lrf_dialog* on success, or NULL on error;
 */
lrf_dialog* new_lrf_dialog(str call_id, str target_uri)
{
	lrf_dialog *d;
	
	if (!lrf_dialog_count_increment()) return 0;
	d = shm_malloc(sizeof(lrf_dialog));
	if (!d) {
		LOG(L_ERR,"ERR:"M_NAME":new_lrf_dialog(): Unable to alloc %d bytes\n",
			sizeof(lrf_dialog));
		goto error;
	}
	memset(d,0,sizeof(lrf_dialog));
	
	d->hash = get_lrf_dialog_hash(call_id);		
	STR_SHM_DUP(d->call_id,call_id,"shm");
	if(target_uri.len>0 && target_uri.s!=NULL)
		STR_SHM_DUP(d->target_uri, target_uri, "shm");
				
	return d;
error:
out_of_memory:
	if (d){
		if (d->call_id.s) shm_free(d->call_id.s);
		if (d->target_uri.s) shm_free(d->target_uri.s);
		shm_free(d);		
	}
	lrf_dialog_count_decrement();
	return 0;
}

/**
 * Creates and adds a lrf_dialog to the hash table.
 * \note Locks the hash slot if ok! Call d_unlock(lrf_dialog->hash) when you are finished)
 * \note make sure that is_lrf_dialog(call_id) returns 0 or there will be unreachable duplicates!
 * @param call_id - dialog's call_id
 * @param target_uri - dialog's target URI 
 * @returns the new lrf_dialog* on success, or NULL on error;
 */
lrf_dialog* add_lrf_dialog(str call_id,str target_URI)
{
	lrf_dialog *d;
	
	d = new_lrf_dialog(call_id, target_URI);
	if (!d) return 0;		
	
	d_lock(d->hash);
		d->next = 0;
		d->prev = lrf_dialogs[d->hash].tail;
		if (d->prev) d->prev->next = d;
		lrf_dialogs[d->hash].tail = d;
		if (!lrf_dialogs[d->hash].head) lrf_dialogs[d->hash].head = d;

		return d;
}

/**
 * Finds out if a dialog is in the hash table.
 * @param call_id - dialog's call_id
 * @param dir - the direction of the dialog. if NULL, it doesn't matter
 * @returns - 1 if the dialog exists, 0 if not
 * \note transport is ignored.
 */
int is_lrf_dialog(str call_id,enum lrf_dialog_direction *dir)
{
	lrf_dialog *d=0;
	unsigned int hash = get_lrf_dialog_hash(call_id);

	d_lock(hash);
		d = lrf_dialogs[hash].head;
		while(d){
			if ((!dir || d->direction == *dir) &&
				d->call_id.len == call_id.len &&
				strncasecmp(d->call_id.s,call_id.s,call_id.len)==0) {
					d_unlock(hash);
					return 1;
				}
			d = d->next;
		}
	d_unlock(hash);
	return 0;
}

/**
 * Finds out if a dialog is in the hash table.
 * @param call_id - dialog's call_id
 * @param dir - the direction of the dialog
 * @returns - 1 if the dialog exists, 0 if not
 * \note transport is ignored.
 */
int is_lrf_dialog_dir(str call_id,enum lrf_dialog_direction dir)
{
	lrf_dialog *d=0;
	unsigned int hash = get_lrf_dialog_hash(call_id);
		d_lock(hash);
		d = lrf_dialogs[hash].head;
		while(d){
			if (d->direction==dir && d->call_id.len == call_id.len &&
						strncasecmp(d->call_id.s,call_id.s,call_id.len)==0) {
				d_unlock(hash);
				return 1;
			}
			d=d->next;
		}
		d_unlock(hash);
		return 0;
}

/**
 * Finds and returns a dialog from the hash table.
 * \note Locks the hash slot if ok! Call d_unlock(lrf_dialog->hash) when you are finished)
 * @param call_id - dialog's call_id
 * \note transport is ignored.
 */
lrf_dialog* get_lrf_dialog(str call_id, enum lrf_dialog_direction *dir)
{
	lrf_dialog *d=0;
	unsigned int hash = get_lrf_dialog_hash(call_id);

	d_lock(hash);
		d = lrf_dialogs[hash].head;
		while(d){
			if ((!dir || d->direction == *dir) &&
				d->call_id.len == call_id.len &&
				strncasecmp(d->call_id.s,call_id.s,call_id.len)==0) {
					return d;
				}
			d = d->next;
		}
	d_unlock(hash);
	return 0;
}

/**
 * Finds and returns a dialog from the hash table.
 * @param call_id - call_id of the dialog
 * @param dir - the direction
 * @returns the lrf_dialog* or NULL if not found
 */
lrf_dialog* get_lrf_dialog_dir(str call_id,enum lrf_dialog_direction dir)
{
	lrf_dialog *d=0;
	unsigned int hash = get_lrf_dialog_hash(call_id);

	d_lock(hash);
		d = lrf_dialogs[hash].head;
		while(d){
			if (d->direction == dir &&
				d->call_id.len == call_id.len &&
				strncasecmp(d->call_id.s,call_id.s,call_id.len)==0) {
					return d;
				}
			d = d->next;
		}
	d_unlock(hash);
	return 0;
}


/**
 * Finds and returns a dialog from the hash table.
 * \note the table should be locked already for the call_id in the parameter
 * @param call_id - call_id of the dialog
 * @param dir - the direction
 * @returns the lrf_dialog* or NULL if not found
 */
lrf_dialog* get_lrf_dialog_dir_nolock(str call_id,enum lrf_dialog_direction dir)
{
	lrf_dialog *d=0;
	unsigned int hash = get_lrf_dialog_hash(call_id);

	d = lrf_dialogs[hash].head;
	while(d){
		if (d->direction == dir &&
			d->call_id.len == call_id.len &&
			strncasecmp(d->call_id.s,call_id.s,call_id.len)==0) {
				return d;
			}
		d = d->next;
	}
	return 0;
}

str reason_terminate_lrf_dialog_s={"Session terminated in the P-CSCF",32};
/** 
 * Terminates a dialog - called before del_lrf_dialog to send out terminatination messages.
 * @param d - the dialog to terminate
 * @returns - 1 if the requests were sent and the dialog will be deleted, 0 on error (you will have to delete the
 * dialog yourself!) 
 */
int terminate_lrf_dialog(lrf_dialog *d)
{
	//if (!pcscf_dialogs_enable_release) return 0;
	/*switch (d->method){
		case DLG_METHOD_INVITE:
			if (release_call_p(d,503,reason_terminate_lrf_dialog_s)==-1){				
				del_lrf_dialog(d);
			}
			return 1;
			break;
		case DLG_METHOD_SUBSCRIBE:
			LOG(L_ERR,"ERR:"M_NAME":terminate_lrf_dialog(): Not needed for SUBSCRIBE dialogs - silent drop on expiration.\n");
			return 0;
			break;
		default:
			LOG(L_ERR,"ERR:"M_NAME":terminate_lrf_dialog(): Not implemented yet for method[%d]!\n",d->method);
			return 0;
	}*/
	return 0;
}

/**
 * Deletes and destroys the dialog from the hash table.
 * \note Must be called with a lock on the dialogs slot
 * @param d - the dialog to delete
 */
void del_lrf_dialog(lrf_dialog *d)
{
	if (d->prev) d->prev->next = d->next;
	else lrf_dialogs[d->hash].head = d->next;
	if (d->next) d->next->prev = d->prev;
	else lrf_dialogs[d->hash].tail = d->prev;
	free_lrf_dialog(d);
}

/**
 * Destroys the dialog.
 * @param d - the dialog to delete
 */
void free_lrf_dialog(lrf_dialog *d)
{
	int i;
	if (!d) return;
	if (d->call_id.s) shm_free(d->call_id.s);
	if (d->method_str.s) shm_free(d->method_str.s);	
	if (d->routes){
		for(i=0;i<d->routes_cnt;i++)
			shm_free(d->routes[i].s);
		shm_free(d->routes);
	}
	if (d->dialog_s) tmb.free_dlg(d->dialog_s);
	if (d->dialog_c) tmb.free_dlg(d->dialog_c);
	if (d->refresher.s) shm_free(d->refresher.s);
	shm_free(d);
	lrf_dialog_count_decrement(); 
}

/**
 * Prints the dialog hash table.
 * @param log_level - the log_level to print with
 */
void print_lrf_dialogs(int log_level)
{
	lrf_dialog *d;
	int i,j;
	//if (debug<log_level) return; /* to avoid useless calls when nothing will be printed */
	d_act_time();
	LOG(log_level,"INF:"M_NAME":----------  LRF Dialog List begin --------------\n");
	for(i=0;i<lrf_dialogs_hash_size;i++){
		d_lock(i);
			d = lrf_dialogs[i].head;
			while(d){
				LOG(log_level,"INF:"M_NAME":[%4d] Dir:["ANSI_MAGENTA"%d"ANSI_GREEN
					"]\tCall-ID:<"ANSI_BLUE"%.*s"ANSI_GREEN
					">\t\n"
					,i,				
					d->direction,
					d->call_id.len,d->call_id.s);
				LOG(log_level,"INF:"M_NAME":\t\tMethod:["ANSI_MAGENTA"%d"ANSI_GREEN
					"] State:["ANSI_MAGENTA"%d"ANSI_GREEN
					"] Exp:["ANSI_MAGENTA"%4d"ANSI_GREEN"]\n",
					d->method,d->state,
					(int)(d->expires - d_time_now));	
				for(j=0;j<d->routes_cnt;j++)
					LOG(log_level,"INF:"M_NAME":\t\t RR: <"ANSI_YELLOW"%.*s"ANSI_GREEN">\n",			
						d->routes[j].len,d->routes[j].s);					
				d = d->next;
			} 		
		d_unlock(i);
	}
	LOG(log_level,"INF:"M_NAME":----------  LRF Dialog List end   --------------\n");	
}

/**
 * Returns the lrf_dialog_direction from the direction string.
 * @param direction - "orig" or "term"
 * @returns the lrf_dialog_direction if ok or #DLG_MOBILE_UNKNOWN if not found
 */
static inline enum lrf_dialog_direction get_dialog_direction(char *direction)
{
	if (!direction) {
		LOG(L_CRIT,"ERR:"M_NAME":get_dialog_direction(): Unknown direction NULL");
		return DLG_MOBILE_UNKNOWN;
	}
	switch(direction[0]){
		case 'o':
		case 'O':
		case '0':
			return DLG_MOBILE_ORIGINATING;
		case 't':
		case 'T':
		case '1':
			return DLG_MOBILE_TERMINATING;
		default:
			LOG(L_CRIT,"ERR:"M_NAME":get_dialog_direction(): Unknown direction %s",direction);
			return DLG_MOBILE_UNKNOWN;
	}
}

/**
 * Finds the contact target_uri for a dialog.
 * @param msg - the SIP message to look into
 * @param direction - look for originating or terminating contact ("orig"/"term")
 * @returns 1 if found, 0 if not
 */
inline int find_dialog_contact(struct sip_msg *msg,enum lrf_dialog_direction dir,str *target_uri)
{
	switch(dir){
		case DLG_MOBILE_ORIGINATING:
			/*if (!cscf_get_originating_contact(msg,host,port,transport))
				return 0;
			return 1;*/
		case DLG_MOBILE_TERMINATING:
			/*if (!cscf_get_terminating_contact(msg))
				return 0;*/
			return 1;
		default:
			LOG(L_CRIT,"ERR:"M_NAME":find_dialog_contact(): Unknown direction %d",dir);
			return 0;
	}
	return 1;
}



/**
 * Find out if a message is within a saved dialog.
 * @param msg - the SIP message
 * @param str1 - the direction of the dialog ("orig"/"term")
 * @param str2 - not used
 * @returns #CSCF_RETURN_TRUE if in, #CSCF_RETURN_FALSE else or #CSCF_RETURN_BREAK on error
 */
int LRF_is_in_dialog(struct sip_msg* msg, char* str1, char* str2)
{
	str call_id;
	enum lrf_dialog_direction dir;
	dir = get_dialog_direction(str1);
	str target_uri={0,0};
	
	if (!find_dialog_contact(msg,dir,&target_uri)){
		LOG(L_ERR,"ERR:"M_NAME":LRF_is_in_dialog(%s): Error retrieving %s contact\n",str1,str1);
		return CSCF_RETURN_BREAK;
	}		

	//print_lrf_dialog(L_ERR);
	call_id = cscf_get_call_id(msg,0);
	if (!call_id.len)
		return CSCF_RETURN_FALSE;
			
	if (is_lrf_dialog(call_id, 0)) {
		return CSCF_RETURN_TRUE;
	}
	else 
		return CSCF_RETURN_FALSE;
}

str s_OTHER={"<OTHER>",7};
str s_INVITE={"INVITE",6};
str s_SUBSCRIBE={"SUBSCRIBE",9};
/**
 * Return lrf_dialog_method for a method string.
 * @param method - the string containing the method
 * @returns the lrf_dialog_method corresponding if known or #DLG_METHOD_OTHER if not
 */
static enum lrf_dialog_method get_dialog_method(str method)
{
	if (method.len == s_INVITE.len &&
		strncasecmp(method.s,s_INVITE.s,s_INVITE.len)==0) return DLG_METHOD_INVITE;
	if (method.len == s_SUBSCRIBE.len &&
		strncasecmp(method.s,s_SUBSCRIBE.s,s_SUBSCRIBE.len)==0) return DLG_METHOD_SUBSCRIBE;
	return DLG_METHOD_OTHER;
}

#ifdef WITH_IMS_PM
/** 
 * Returns the Method string give the method enum id
 * @param method - the enum id
 * @returns the string method
 */	
static str get_dialog_method_str(enum lrf_dialog_method method)
{
	switch(method){
		case DLG_METHOD_INVITE:
			return s_INVITE;
		case DLG_METHOD_SUBSCRIBE:
			return s_SUBSCRIBE;
		default:
			return s_OTHER;
	}	
}
#endif

static fparam_t fp_422 = FParam_INT(422);
static fparam_t fp_se_small = FParam_STRING("Session Interval Too Small");

/**
 * Send a 422 Session Interval Too Small.
 * @param msg - the msg to respond to
 * @param str1 - not used
 * @param str2 - not used
 * @returns
 */
int LRF_422_session_expires(struct sip_msg* msg, char* str1, char* str2)
{
	str hdr = {pkg_malloc(32), 0};
	
	if (!hdr.s) {
		LOG(L_ERR, "ERR:"M_NAME":LRF_422_session_expires(): no memory for hdr\n");
		goto error;
	}

	//hdr.len = snprintf(hdr.s, 31, "Min-SE: %d\r\n", pcscf_min_se);

	if (!cscf_add_header_rpl(msg, &hdr)) {
		LOG(L_ERR, "ERR:"M_NAME":LRF_422_session_expires(): Can't add header\n");
		goto error;
 	}
	
	return sl_reply(msg, (char *)&fp_422, (char *)&fp_se_small);

error:
	if (hdr.s) pkg_free(hdr.s);
	return CSCF_RETURN_FALSE;
}


static str s_refresher = {"refresher=", 10};
static str str_ext_timer = {"timer", 5};
static str str_min_se = {"Min-SE:",7};
static str str_se = {"Session-Expires:",16}; 
static str str_require = {"Require:",8}; 

/**
 * Checks if Session-Expires value is over Min_SE local policy
 * @param msg - the initial request
 * @param str1 - not used
 * @param str2 - not used
 * @returns #CSCF_RETURN_TRUE if ok, #CSCF_RETURN_FALSE if not
*/
int LRF_check_session_expires(struct sip_msg* msg, char* str1, char* str2)
{
	return CSCF_RETURN_FALSE;
}		

int fixup_save_dialog(void** param, int param_no){

	char* str;
	int len;
	fparam_t * p;

	if(param_no!=2){
		return 0;
	}

	str = (char*) *param;
	if(!str || str[0] == '\0'){
	
		LOG(L_ERR, "ERR:"M_NAME":fixup_save_dialog: NULL param 2\n");
		return -1;
	}

	len = strlen(str);

	if(len == 5 && strncmp(str, "emerg", 5) == 0){
		str[0] = '1';
	}else if (len ==9 && strncmp(str, "non-emerg", 9)==0){
		str[0] = '0';
	}else {
		LOG(L_ERR, "ERR:"M_NAME":fixup_save_dialog: invalid param 2, "
				"possible values are \"emerg\" or \"non-emerg\"\n");
		return -1;
	}	

	p = (fparam_t*)pkg_malloc(sizeof(fparam_t));
	if (!p) {
		ERR("No memory left\n");
		return E_OUT_OF_MEM;
    	}
	memset(p, 0, sizeof(fparam_t));
	p->orig = *param;

	switch(str[0]){
	
		case '0':
			p->v.i = 0;
			break;
		case '1':
			p->v.i = 1;
			break;
	}

	*param = (void*)p;

	return 0;
}


/**
 * Saves a dialog.
 * @param msg - the initial request
 * @param str1 - direction - "orig" or "term"
 * @returns #CSCF_RETURN_TRUE if ok, #CSCF_RETURN_FALSE if not or #CSCF_RETURN_BREAK on error 
 */
int LRF_save_dialog(struct sip_msg* msg, char* str1, char* str2)
{
	str call_id;
	lrf_dialog *d =NULL;
	time_t t_time;
	str ses_exp = {0,0};
	str refresher = {0,0};
	char buf1[256],buf2[256];
	str tag,ruri,uri,x;
	struct hdr_field *h;
	unsigned int hash;
	enum lrf_dialog_direction dir;
	str target_uri={0,0};
	
	dir = get_dialog_direction(str1);
	
	if (!find_dialog_contact(msg,dir, &target_uri)){
		LOG(L_ERR,"ERR:"M_NAME":LRF_is_in_dialog(): Error retrieving %s contact\n",str1);
		return CSCF_RETURN_BREAK;
	}		
		
	call_id = cscf_get_call_id(msg,0);
	if (!call_id.len)
		return CSCF_RETURN_FALSE;

	LOG(L_DBG,"DBG:"M_NAME":LRF_save_dialog(%s): Call-ID <%.*s>\n",str1,call_id.len,call_id.s);
	
	if (is_lrf_dialog(call_id, &dir)){
		LOG(L_ERR,"ERR:"M_NAME":LRF_save_dialog: dialog already exists!\n");	
		return CSCF_RETURN_TRUE;
	}
	
	d = add_lrf_dialog(call_id, target_uri);
	if (!d) return CSCF_RETURN_FALSE;

	d->method = get_dialog_method(msg->first_line.u.request.method);
	STR_SHM_DUP(d->method_str,msg->first_line.u.request.method,"shm");
	d->first_cseq = cscf_get_cseq(msg,0);
	d->last_cseq = d->first_cseq;
	d->state = DLG_STATE_INITIAL;

	d->uac_supp_timer = supports_extension(msg, &str_ext_timer);

	ses_exp = cscf_get_session_expires_body(msg, &h);
	t_time = cscf_get_session_expires(ses_exp, &refresher);
	if (!t_time){
		d->expires = d_act_time() + 60;
		d->lr_session_expires = 0;
	}else {
		d->expires = d_act_time() + t_time;
		d->lr_session_expires = t_time;
		if (refresher.len)
			STR_SHM_DUP(d->refresher, refresher, "DIALOG_REFRESHER");
	}
	
	d->direction=dir;
				
	cscf_get_from_tag(msg,&tag);
	cscf_get_from_uri(msg,&x);
	uri.len = snprintf(buf1,256,"<%.*s>",x.len,x.s);
	uri.s = buf1;	
	cscf_get_to_uri(msg,&x);
	ruri.len = snprintf(buf2,256,"<%.*s>",x.len,x.s);
	ruri.s = buf2;
		
	tmb.new_dlg_uac(&call_id,
					&tag,
					d->first_cseq,
					&uri,
					&ruri,
					&d->dialog_c);
					
	tmb.new_dlg_uas(msg,99,&d->dialog_s);
		
	d_unlock(d->hash);
	print_lrf_dialogs(L_INFO);
	
	return CSCF_RETURN_TRUE;
out_of_memory:
	if (d){
		hash = d->hash;
		del_lrf_dialog(d);
		d_unlock(hash);
	}
	return CSCF_RETURN_FALSE;
}

/**
 * Save the Record-routes for a dialog.
 * @param msg - the SIP message to extract RRs from
 * @param str1 - the direction to know if to reverse the RR list or not
 * @param d - dialog to save to
 */
void save_dialog_routes(struct sip_msg* msg, char* str1,lrf_dialog *d)
{
	int i;
	rr_t *rr,*ri;	
	struct hdr_field *hdr;
	if (d->routes){
		for(i=0;i<d->routes_cnt;i++)
			shm_free(d->routes[i].s);
		shm_free(d->routes);
		d->routes = 0;
	}
	d->routes_cnt = 0;
	for(hdr=cscf_get_next_record_route(msg,0);hdr;hdr=cscf_get_next_record_route(msg,hdr)){
		rr = (rr_t*)hdr->parsed;
		for(ri=rr;ri;ri=ri->next)
			d->routes_cnt++;
	}
	d->routes = shm_malloc(sizeof(str)*d->routes_cnt);
	if (!d->routes){
		LOG(L_ERR,"ERR:"M_NAME":save_dialog_routes(): Unable to alloc %d bytes\n",
			sizeof(str)*d->routes_cnt);
		d->routes_cnt = 0;
		return;		
	}
	if (!str1) return;
	if (str1[0]=='o'||str1[0]=='0'||str1[0]=='O'){
		/* originating - reverse order */
		i = d->routes_cnt-1;
		for(hdr=cscf_get_next_record_route(msg,0);hdr;hdr=cscf_get_next_record_route(msg,hdr)){
			rr = (rr_t*)hdr->parsed;
			for(ri=rr;ri;ri=ri->next){
				STR_SHM_DUP(d->routes[i],ri->nameaddr.uri,"shm");
				i--;
			}
		}
	}else{
		/* terminating - normal order */
		i = 0;
		for(hdr=cscf_get_next_record_route(msg,0);hdr;hdr=cscf_get_next_record_route(msg,hdr)){
			rr = (rr_t*)hdr->parsed;
			for(ri=rr;ri;ri=ri->next){
				STR_SHM_DUP(d->routes[i],ri->nameaddr.uri,"shm");
				i++;
			}
		}
	}		
out_of_memory:
	return;	
}
/**
 * Updates dialog on reply message
 * @param msg - the SIP message 
 * @param d - dialog to modify
 */
int update_dialog_on_reply(struct sip_msg *msg, lrf_dialog *d)
{
	struct hdr_field *h_req;
	struct hdr_field *h=0;
	int res=0;
	time_t t_time=0;
	str ses_exp = {0,0};
	str refresher = {0,0};
	str new_ses_exp = {0,0};
	str new_ext = {0,0};
	int expires = 0;

	ses_exp = cscf_get_session_expires_body(msg, &h);
	t_time = cscf_get_session_expires(ses_exp, &refresher);
	if (!t_time) //i.e not session-expires header in response
	{
		if (!d->uac_supp_timer || !d->lr_session_expires)
		{
			expires = cscf_get_expires_hdr(msg,0);
			if (expires >= 0)
			{
				d->expires = d_act_time()+expires;	
			}
			else
			{
				d->expires = d_act_time()+lrf_dialogs_expiration_time;	
			}
		}
		else// uac supports timer, but no session-expires header found in response
		{
			d->expires = d_act_time()+d->lr_session_expires;
			
			new_ses_exp.len = 11/*int value*/ + str_se.len+s_refresher.len+8;
			new_ses_exp.s = pkg_malloc(new_ses_exp.len+1);
			if (!new_ses_exp.s) {
				LOG(L_ERR,"ERR:"M_NAME":update_dialog_on_reply: Error allocating %d bytes\n",new_ses_exp.len);
				goto error;
			}
			new_ses_exp.len = snprintf(new_ses_exp.s, new_ses_exp.len, "%.*s %d; %.*suac\r\n",str_se.len, str_se.s, (int)d->lr_session_expires ,s_refresher.len, s_refresher.s);
			cscf_add_header(msg, &new_ses_exp, HDR_OTHER_T);
			if (!requires_extension(msg, &str_ext_timer)) //must have require timer extenstion
			{
				/* walk through all Require headers to find first require header*/
				res = parse_headers(msg, HDR_EOH_F, 0);
				if (res == -1) {
					ERR("Error while parsing headers (%d)\n", res);
					return 0; /* what to return here ? */
				}
				
				h_req = msg->require;
				while (h_req) {
					if (h_req->type == HDR_REQUIRE_T) {
						if (h_req->body.s[new_ext.len-1]=='\n')
						{
							new_ext.len = str_require.len + 1/* */+h_req->body.len + 7;/*, timer*/
							new_ext.s = pkg_malloc(new_ext.len);
							if (!new_ext.s) {
								LOG(L_ERR,"ERR:"M_NAME":update_dialog_on_reply: Error allocating %d bytes\n",new_ext.len);
								goto error;
							}			
							new_ext.len = snprintf(new_ext.s, str_require.len, "%.*s %.*s, timer\r\n", str_require.len, str_require.s, h_req->body.len-2, h_req->body.s);
						}
						else
						{
							new_ext.len = str_require.len + 1/*space*/ + h_req->body.len + 9;/*, timer\r\n*/
							new_ext.s = pkg_malloc(new_ext.len);
							if (!new_ext.s) {
								LOG(L_ERR,"ERR:"M_NAME":update_dialog_on_reply: Error allocating %d bytes\n",new_ext.len);
								goto error;
							}			
							new_ext.len = snprintf(new_ext.s, str_require.len, "%.*s %.*s, timer\r\n", str_require.len, str_require.s, h_req->body.len, h_req->body.s);
						}
						cscf_del_header(msg, h_req);
						cscf_add_header(msg, &new_ext, HDR_REQUIRE_T);
						break;
					}
					h_req = h_req->next;
				}
			}
		}
	}
	else{
		d->expires = d_act_time() + t_time;
		d->lr_session_expires = t_time;
	}	
	return 1;
error:
	if (new_ses_exp.s) pkg_free(new_ses_exp.s);
	if (new_ext.s) pkg_free(new_ext.s);
	return 0;	
}

/**
 * Updates a dialog.
 * If the initial request was:
 * - INVITE - refreshes the expiration or looks for the BYE and destroys the dialog 
 * if found
 * - SUBSCRIBE - looks for the Subscription-state in NOTIFY, refreshes the expiration 
 * and if terminated destroys the dialog
 * - When adding more dialogs, add the refreshal methods here or they will expire and will
 * be destroyed. Also add the termination to reduce the memory consumption and improve the
 * performance.
 * @param msg - the request/response
 * @param str1 - direction - "orig" or "term"
 * @param str2 - not used
 * @returns #CSCF_RETURN_TRUE if ok, #CSCF_RETURN_FALSE if not or #CSCF_RETURN_BREAK on error 
 */
int LRF_update_dialog(struct sip_msg* msg, char* str1, char* str2)
{
	str call_id;
	lrf_dialog *d;
	int response;
	int cseq;
	struct hdr_field *h=0;
	struct sip_msg *req=0;
	int expires;
	str totag;
	time_t t_time=0;
	str ses_exp = {0,0};
	str refresher = {0,0};
	enum lrf_dialog_direction dir;
	str target_uri={0,0};
	dir = get_dialog_direction(str1);
	
	if (msg->first_line.type==SIP_REPLY) req = cscf_get_request_from_reply(msg);
	else req = msg;
	if (!find_dialog_contact(req,dir,&target_uri)){
		LOG(L_ERR,"ERR:"M_NAME":LRF_update_dialog(%s): Error retrieving %s contact\n",str1,str1);
		return CSCF_RETURN_BREAK;
	}		
		
	call_id = cscf_get_call_id(msg,0);
	if (!call_id.len)
		return CSCF_RETURN_FALSE;

	LOG(L_DBG,"DBG:"M_NAME":LRF_update_dialog(%s): Call-ID <%.*s>\n",str1,call_id.len,call_id.s);

	d = get_lrf_dialog(call_id, &dir);
	if (!d)
		d = get_lrf_dialog(call_id, NULL);
	if (!d){
		if (msg->first_line.type==SIP_REQUEST &&
			msg->first_line.u.request.method.len == 3 && 
			strncasecmp(msg->first_line.u.request.method.s,"ACK",3)){
			/* to skip the ACK after a 4xx when the dialog was dropped already*/
			return CSCF_RETURN_TRUE;
		}else{
			LOG(L_CRIT,"ERR:"M_NAME":LRF_update_dialog: dialog does not exists!\n");
			return CSCF_RETURN_FALSE;
		}	
	}


	if (msg->first_line.type==SIP_REQUEST){
		/* Request */
		LOG(L_DBG,"DBG:"M_NAME":LRF_update_dialog(%s): Method <%.*s> \n",str1,
			msg->first_line.u.request.method.len,msg->first_line.u.request.method.s);
		cseq = cscf_get_cseq(msg,&h);
		if (cseq>d->last_cseq) d->last_cseq = cseq;
		if (get_dialog_method(msg->first_line.u.request.method) == DLG_METHOD_INVITE)
		{
			d->uac_supp_timer = supports_extension(msg, &str_ext_timer);
	
			ses_exp = cscf_get_session_expires_body(msg, &h);
			t_time = cscf_get_session_expires(ses_exp, &refresher);
			if (!t_time){
				d->expires = d_act_time()+lrf_dialogs_expiration_time;
				d->lr_session_expires = 0;
			} else {
				d->expires = d_act_time() + t_time;
				d->lr_session_expires = t_time;
				if (refresher.len)
					STR_SHM_DUP(d->refresher, refresher, "DIALOG_REFRESHER");
			}
		}
		else if (d->method == DLG_METHOD_SUBSCRIBE && 
			msg->first_line.u.request.method.len == 6 && 
			strncasecmp(msg->first_line.u.request.method.s,"NOTIFY",6)==0)
		{
			// Subscription-State header is mandatory for NOTIFY. See RFC 3265, Section 7.2
			expires = cscf_get_subscription_state(msg);
			if (expires >= 0)
			{
				d->expires = d_act_time()+expires;
			}
			else
			{
				d->expires = d_act_time()+lrf_dialogs_expiration_time;
			}
		}
		else
		{
			expires = cscf_get_expires_hdr(msg,0);
			if (expires >= 0) 
			{
				LOG(L_INFO,"DBG:"M_NAME":LRF_update_dialog(%.*s): Update expiration time to %d via Expire header 2\n",call_id.len,call_id.s,expires);
				d->expires = d_act_time()+expires;
			}
			else
			{
				LOG(L_INFO,"INF:"M_NAME": update_dialog(%.*s): d->expires+=lrf_dialogs_expiration_time 4\n",call_id.len,call_id.s);
				d->expires = d_act_time()+lrf_dialogs_expiration_time;
			}
			d->lr_session_expires = 0;		
		}
	}else{
		/* Reply */
		response = msg->first_line.u.reply.statuscode;
		LOG(L_DBG,"DBG:"M_NAME":LRF_update_dialog(%s): <%d> \n",str1,response);
		cseq = cscf_get_cseq(msg,&h);
		if (cseq==0 || h==0) return CSCF_RETURN_FALSE;
		if (d->first_cseq==cseq && d->method_str.len == ((struct cseq_body *)h->parsed)->method.len &&
			strncasecmp(d->method_str.s,((struct cseq_body *)h->parsed)->method.s,d->method_str.len)==0 &&
			d->state < DLG_STATE_CONFIRMED){
			/* reply to initial request */
			if (response<200 && response>100){
				save_dialog_routes(msg,str1,d);
				d->state = DLG_STATE_EARLY;
				d->expires = d_act_time()+300;
				cscf_get_to_tag(msg,&totag);
				tmb.update_dlg_uas(d->dialog_s,response,&totag);
				tmb.dlg_response_uac(d->dialog_c,msg,IS_NOT_TARGET_REFRESH);
			}else
			if (response>=200 && response<300){
				save_dialog_routes(msg,str1,d);
				d->state = DLG_STATE_CONFIRMED;
				
				update_dialog_on_reply(msg, d);
				
				cscf_get_to_tag(msg,&totag);
				tmb.update_dlg_uas(d->dialog_s,response,&totag);
				tmb.dlg_response_uac(d->dialog_c,msg,IS_NOT_TARGET_REFRESH);
				
			}else
				if (response>300){
					d->state = DLG_STATE_TERMINATED;
					d_unlock(d->hash);				
					return LRF_drop_dialog(msg,str1,str2);
				}				
		}else{
			/* reply to subsequent request */			
			if (!req) req = cscf_get_request_from_reply(msg);
			
			/* destroy dialogs on specific methods */
			switch (d->method){
				case DLG_METHOD_OTHER:							
					expires = cscf_get_expires_hdr(msg,0);
					if (expires >= 0)
					{
						d->expires = d_act_time()+expires;
					}
					else
					{
						d->expires = d_act_time()+lrf_dialogs_expiration_time;
					}
					d->lr_session_expires = 0;
					break;
				case DLG_METHOD_INVITE:
					if (req && req->first_line.u.request.method.len==3 &&
						strncasecmp(req->first_line.u.request.method.s,"BYE",3)==0){
						d->state = DLG_STATE_TERMINATED;
						d_unlock(d->hash);				
						return LRF_drop_dialog(msg,str1,str2);
					}
					update_dialog_on_reply(msg, d);
					break;
				case DLG_METHOD_SUBSCRIBE:
//					if (req && req->first_line.u.request.method.len==9 &&
//						strncasecmp(req->first_line.u.request.method.s,"SUBSCRIBE",9)==0 &&
//						cscf_get_expires_hdr(msg)==0){						
//						d->state = DLG_STATE_TERMINATED;
//						d_unlock(d->hash);				
//						return LRF_drop_dialog(msg,str1,str2);
//					}
					if (req && req->first_line.u.request.method.len==6 &&
						strncasecmp(req->first_line.u.request.method.s,"NOTIFY",6)==0){
						expires = cscf_get_subscription_state(req);
						if (expires==0){						
							d->state = DLG_STATE_TERMINATED;
							d_unlock(d->hash);				
							return LRF_drop_dialog(msg,str1,str2);
						}else if (expires>0){
							d->expires = d_act_time() + expires;
						}
					}
					break;
			}
			if (cseq>d->last_cseq) d->last_cseq = cseq;
		}
	}
	
	d_unlock(d->hash);
	
	print_lrf_dialogs(L_INFO);
	
	return CSCF_RETURN_TRUE;	
out_of_memory:
	d_unlock(d->hash);
	return CSCF_RETURN_ERROR;	
}


/**
 * Drops and deletes a dialog.
 * @param msg - the request/response
 * @param str1 - direction - "orig" or "term"
 * @param str2 - not used
 * @returns #CSCF_RETURN_TRUE if ok, #CSCF_RETURN_FALSE if not or #CSCF_RETURN_BREAK on error 
 */
int LRF_drop_dialog(struct sip_msg* msg, char* str1, char* str2)
{
	str call_id;
	lrf_dialog *d;
	int hash;
	struct sip_msg *req;
	enum lrf_dialog_direction dir;
	str target_uri = {0,0};
	dir = get_dialog_direction(str1);
	
	if (msg->first_line.type==SIP_REPLY) req = cscf_get_request_from_reply(msg);
	else req = msg;
	if (!find_dialog_contact(req,dir,&target_uri)){
		LOG(L_ERR,"ERR:"M_NAME":LRF_is_in_dialog(): Error retrieving %s contact\n",str1);
		return CSCF_RETURN_BREAK;
	}		
		
	call_id = cscf_get_call_id(msg,0);
	if (!call_id.len)
		return CSCF_RETURN_FALSE;

	LOG(L_DBG,"DBG:"M_NAME":LRF_drop_dialog(%s): Call-ID <%.*s>: target <%.*s>\n",
		str1,call_id.len,call_id.s,
		target_uri.len, target_uri.s);
	d = get_lrf_dialog(call_id, &dir);
	if (!d)
		d = get_lrf_dialog(call_id, NULL);
	if (!d){
		LOG(L_INFO,"INFO:"M_NAME":LRF_drop_dialog: dialog does not exists!\n");	
		return CSCF_RETURN_FALSE;
	}

	hash = d->hash;
	
	del_lrf_dialog(d);
		
	d_unlock(hash);
	
	print_lrf_dialogs(L_INFO);
	
	return CSCF_RETURN_TRUE;	
}


/**
 * Checks if the message follows the saved dialog routes.
 * @param msg - the SIP request
 * @param str1 - direction - "orig" or "term"
 * @param str2 - not used
 * @returns #CSCF_RETURN_TRUE if ok, #CSCF_RETURN_FALSE if not or #CSCF_RETURN_BREAK on error 
 */
int LRF_follows_dialog_routes(struct sip_msg *msg,char *str1,char *str2)
{
	int i;
	struct hdr_field *hdr=0;
	rr_t *r;
	lrf_dialog *d;
	str call_id;
	enum lrf_dialog_direction dir;
	str target_uri={0,0};
	
	dir = get_dialog_direction(str1);
	
	if (!find_dialog_contact(msg,dir,&target_uri)){
		LOG(L_ERR,"ERR:"M_NAME":LRF_follows_dialog_routes(): Error retrieving %s contact\n",str1);
		return CSCF_RETURN_BREAK;
	}		
		
	call_id = cscf_get_call_id(msg,0);
	if (!call_id.len)
		return CSCF_RETURN_FALSE;

	LOG(L_DBG,"DBG:"M_NAME":LRF_follows_dialog_routes(%s): Call-ID <%.*s> TARGET <%.*s>\n",
		str1,call_id.len,call_id.s,
		target_uri.len, target_uri.s);
	d = get_lrf_dialog(call_id, &dir);
	if (!d)
		d = get_lrf_dialog(call_id, NULL);
	if (!d){
		LOG(L_ERR,"ERR:"M_NAME":LRF_follows_dialog_routes: dialog does not exists!\n");	
		return CSCF_RETURN_FALSE;
	}
	/* todo - fix this to match exactly the first request */
	if (d->first_cseq == cscf_get_cseq(msg,0) &&
		d->method == get_dialog_method(msg->first_line.u.request.method)){
		LOG(L_INFO,"INF:"M_NAME":LRF_follows_dialog_routes: this looks like the initial request (retransmission?)!\n");
		goto ok;		
	}

	hdr = cscf_get_next_route(msg,0);
	r = 0;
	if (!hdr){
		if (d->routes_cnt==0) goto ok;
		else goto nok;
	}
	r = (rr_t*) hdr->parsed;	
	for(i=0;i<d->routes_cnt;i++){
		LOG(L_DBG,"DBG:"M_NAME":LRF_follows_dialog_routes:  must <%.*s>\n",
			d->routes[i].len,d->routes[i].s);		
		if (!r) {
			hdr = cscf_get_next_route(msg,hdr);
			if (!hdr) goto nok;
			r = (rr_t*) hdr->parsed;	
		}
		LOG(L_DBG,"DBG:"M_NAME":LRF_follows_dialog_routes: src %.*s\n",
			r->nameaddr.uri.len,r->nameaddr.uri.s);		
		if (r->nameaddr.uri.len==d->routes[i].len &&
				strncasecmp(r->nameaddr.uri.s,
					d->routes[i].s,d->routes[i].len)==0)
		{
			LOG(L_DBG,"DBG:"M_NAME":LRF_follows_dialog_routes: src match\n");		
		} else {
			LOG(L_DBG,"DBG:"M_NAME":LRF_follows_dialog_routes: found <%.*s>\n",
				r->nameaddr.uri.len,r->nameaddr.uri.s);					
			goto nok;
		}
		r = r->next;
	}
	if (r) {
		LOG(L_DBG,"DBG:"M_NAME":LRF_follows_dialog_routes: still has some extra Routes\n");		
		goto nok;
	}
	else 
		if (cscf_get_next_route(msg,hdr)) goto nok;
	
ok:
	if (d) d_unlock(d->hash);
	return CSCF_RETURN_TRUE;	
nok:
	if (d) d_unlock(d->hash);
	return CSCF_RETURN_FALSE;		
}

static str route_s={"Route: <",8};
static str route_1={">, <",4};
static str route_e={">\r\n",3};
/**
 * Inserts the Route header containing the dialog routes to be enforced
 * @param msg - the SIP message to add to
 * @param str1 - the value to insert - !!! quoted if needed
 * @param str2 - not used
 * @returns #CSCF_RETURN_TRUE if ok, #CSCF_RETURN_FALSE if not or #CSCF_RETURN_BREAK on error 
 */
int LRF_enforce_dialog_routes(struct sip_msg *msg,char *str1,char*str2)
{
	int i;
	str newuri={0,0};
	lrf_dialog *d;
	str call_id;
	str x;
	enum lrf_dialog_direction dir;
	str target_uri={0,0};
	
	dir = get_dialog_direction(str1);
		
	if (!find_dialog_contact(msg,dir,&target_uri)){
		LOG(L_ERR,"ERR:"M_NAME":LRF_enforce_dialog_routes(): Error retrieving %s contact\n",str1);
		return CSCF_RETURN_BREAK;
	}		
		
	call_id = cscf_get_call_id(msg,0);
	if (!call_id.len)
		return CSCF_RETURN_FALSE;

	LOG(L_INFO,"DBG:"M_NAME":LRF_enforce_dialog_routes(%s): Call-ID <%.*s> TARGET <%.*s>\n",
		str1,call_id.len,call_id.s,
		target_uri.len, target_uri.s);
	d = get_lrf_dialog(call_id, &dir);
	if (!d)
		d = get_lrf_dialog(call_id, 0);
	if (!d){
		LOG(L_ERR,"ERR:"M_NAME":LRF_enforce_dialog_routes: dialog does not exists!\n");	
		return CSCF_RETURN_FALSE;
	}

	if (!d->routes_cnt){
		d_unlock(d->hash);
		return CSCF_RETURN_TRUE;
	}

	x.len = route_s.len + route_e.len + (d->routes_cnt-1)*route_1.len;
	for(i=0;i<d->routes_cnt;i++)
		x.len+=d->routes[i].len;
			
	x.s = pkg_malloc(x.len);
	if (!x.s){
		LOG(L_ERR, "ERR:"M_NAME":LRF_enforce_dialog_routes: Error allocating %d bytes\n",
			x.len);
		x.len=0;
		d_unlock(d->hash);
		return CSCF_RETURN_ERROR;
	}
	x.len=0;
	STR_APPEND(x,route_s);
	for(i=0;i<d->routes_cnt;i++){
		if (i) STR_APPEND(x,route_1);
		STR_APPEND(x,d->routes[i]);
	}	
	STR_APPEND(x,route_e);
	
	newuri.s = pkg_malloc(d->routes[0].len);
	if (!newuri.s){
		LOG(L_ERR, "ERR:"M_NAME":LRF_enforce_dialog_routes: Error allocating %d bytes\n",
			d->routes[0].len);
		d_unlock(d->hash);
		return CSCF_RETURN_ERROR;
	}
	newuri.len = d->routes[0].len;
	memcpy(newuri.s,d->routes[0].s,newuri.len);
	if (msg->dst_uri.s) pkg_free(msg->dst_uri.s);
	msg->dst_uri = newuri;
	
	//LOG(L_ERR,"%.*s",x.len,x.s);
	d_unlock(d->hash);
	if (cscf_add_header_first(msg,&x,HDR_ROUTE_T)) {
		if (cscf_del_all_headers(msg,HDR_ROUTE_T))
			return CSCF_RETURN_TRUE;
		else {
			LOG(L_ERR,"ERR:"M_NAME":LRF_enforce_dialog_routes: new Route headers added, but failed to drop old ones.\n");
			return CSCF_RETURN_ERROR;
		}
	}
	else {
		if (x.s) pkg_free(x.s);
		return CSCF_RETURN_ERROR;
	}
}





static str s_record_route_s={"Record-Route: <",15};
static str s_record_route_lr={";lr",3};
static str s_record_route_e={">\r\n",3};
/**
 * The dialog timer looks for expired dialogs and removes them.
 * @param ticks - the current time
 * @param param - pointer to the dialogs list
 */
void dialog_timer(unsigned int ticks, void* param)
{
	lrf_dialog *d,*dn;
	int i;
	#ifdef WITH_IMS_PM
		int dialog_cnt[DLG_METHOD_MAX+1];
		for(i=0;i<=DLG_METHOD_MAX;i++)
			dialog_cnt[i]=0;
	#endif
	
	LOG(L_DBG,"DBG:"M_NAME":dialog_timer: Called at %d\n",ticks);
	if (!lrf_dialogs) lrf_dialogs = (lrf_dialog_hash_slot*)param;

	d_act_time();
	
	for(i=0;i<lrf_dialogs_hash_size;i++){
		d_lock(i);
			d = lrf_dialogs[i].head;
			while(d){
				dn = d->next;
				if (d->expires<=d_time_now) {						
						if (!terminate_lrf_dialog(d)) 
							del_lrf_dialog(d);					
				}
				#ifdef WITH_IMS_PM
					else dialog_cnt[d->method]++;
				#endif										
				d = dn;
			}
		d_unlock(i);
	}
	print_lrf_dialogs(L_INFO);
	#ifdef WITH_IMS_PM
		for(i=0;i<=DLG_METHOD_MAX;i++)
			IMS_PM_LOG11(RD_NbrDialogs,get_dialog_method_str(i),dialog_cnt[i]);		
	#endif	
}

