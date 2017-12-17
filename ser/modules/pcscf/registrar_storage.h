/*
 * $Id: registrar_storage.h 1055 2011-03-29 11:00:59Z neits $
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
 * Proxy-CSCF - Registrar Storage
 * 
 * 
 *  \author Dragos Vingarzan vingarzan -at- fokus dot fraunhofer dot de
 *  \author Ancuta Onofrei	andreea dot ancuta dot onofrei -at- fokus dot fraunhofer dot de
 */
#ifndef P_CSCF_REGISTRAR_STORAGE_H_
#define P_CSCF_REGISTRAR_STORAGE_H_

#include "../../sr_module.h"
#include "../../locking.h"
#include "../cdp/cdp_load.h"
#include "../../parser/contact/contact.h"

/** NAT address */
typedef struct _r_nat_dest{
	short proto;
	struct ip_addr nat_addr;	/**< address of the pin hole in the NAT	*/
	unsigned short nat_port;	/**< port of the pin hole in the NAT 	*/
	int proto_reserved1;		/**< id of the TCP connection 			*/
	struct socket_info* bind_address; /**< local address to send from	*/
}r_nat_dest;

/** Registrar Public Identity Structure */
typedef struct _r_public {
	str aor;					/**< the public identity 				*/
	char is_default;			/**< if this is the default id			*/			
	
	struct _r_public *next;		/**< next public identity for this contact */
	struct _r_public *prev; 	/**< previous public identity for this contact */
} r_public;

/** User Registration States */ 
enum Reg_States {
	NOT_REGISTERED=0,			/**< User not-registered */
	REGISTERED=1,				/**< User registered	 */
	REG_PENDING=-1,				/**< Registration pending*/
	DEREGISTERED=-2				/**< DeRegistered-IPSec on - used to relay last messages to freshly de-registered user*/
} ;

/** IPSec SA Information */
typedef struct _r_ipsec {
	unsigned int spi_uc;					/**< SPI Client to use					*/
	unsigned int spi_us;					/**< SPI Server to use					*/
	unsigned int spi_pc;					/**< SPI Client to use					*/
	unsigned int spi_ps;					/**< SPI Server to use					*/
	unsigned short port_uc;		/**< Port UE Client						*/
	unsigned short port_us;		/**< Port UE Server						*/
	
	str ealg;					/**< Cypher Algorithm - ESP				*/
	str r_ealg;					/**< received Cypher Algorithm - ESP				*/
	str ck;						/**< Cypher Key							*/
	str alg;					/**< Integrity Algorithm - AH			*/
	str r_alg;				/**<received Integrity Algorithm - AH			*/
	str ik;						/**< Integrity Key						*/
	str prot;						/**< Protocol (ah/esp) */
	str mod;						/**< Mode (transport/tunnel) */
} r_ipsec;

/** TLS SA Information */
typedef struct _r_tls {
	unsigned short port_tls;	/**< Port UE TLS						*/
	unsigned long session_hash;
} r_tls;

typedef enum _r_sec_type {
	SEC_NONE	=0,
	SEC_IPSEC	=1,
	SEC_TLS		=2,
} r_security_type;

typedef struct _r_security {
	str sec_header;				/**< Security Header value 				*/
	r_security_type type;		/**< Type of security in use			*/ 
	union {
		r_ipsec *ipsec;			/**< IPSec SA information, if any		*/
		r_tls *tls;				/**< TLS SA information, if any 		*/
	} data;
	float q;
} r_security;

/* Registration type:
 * 1 for normal registration only
 * 2 if emergency registration only
 * 3 one of the above types
 */
typedef enum reg_type_{NORMAL_REG = 1, EMERG_REG = 2, ANY_REG = 3} r_reg_type;

/** Registrar Contact Structure */
typedef struct _r_contact {
	unsigned int hash;			/**< the hash value 					*/
	
	str host;					/**< host of the UE						*/
	unsigned short port;		/**< port of the UE						*/
	char transport;				/**< transport for the UE				*/
			
	r_security *security_temp;  /**< Security-Client Information		*/
	r_security *security;	    /**< Security-Client Information		*/
	
	str uri;					/**< uri of contact						*/		
	
	enum Reg_States reg_state;	/**< registration state					*/
	time_t expires;				/**< time of expiration					*/
	
	unsigned short service_route_cnt;/**< size of the above vector		*/
	str *service_route;			/**< service route entries				*/

	r_nat_dest * pinhole;		/**< address of the receive				*/ 
	r_reg_type sos_flag;				/** address used to do an IMS Emergency Registration*/
	
	str pcc_session_id;			/** Session used for Subscription to Signaling Path status  when PCC applies*/

	r_public *head;				/**< first (and default) public identity*/
	r_public *tail;				/**< last public identity				*/
    struct socket_info * si_pc;
    struct socket_info * si_ps;

	struct _r_contact *next;	/**< next contact in this hash slot 	*/
	struct _r_contact *prev;	/**< previous contact in this hash slot	*/
} r_contact;

/** Registrar Slot Structure */
typedef struct {
	r_contact *head;			/**< first contact in the slot			*/
	r_contact *tail;			/**< last contact in the slot			*/
	gen_lock_t *lock;			/**< slot lock 							*/
} r_hash_slot;


void r_act_time();
inline int r_valid_contact(r_contact *c);
inline int r_reg_contact(r_contact *c);


int r_storage_init(int hash_size);
void r_storage_destroy();

inline void r_lock(unsigned int hash);
inline void r_unlock(unsigned int hash);

unsigned int get_contact_hash(str aor,int port,int transport,int hash_size);

r_public* new_r_public(str aor, int is_default);
r_public* get_r_public(r_contact *c, str aor);
r_public* add_r_public(r_contact *c,str aor,int is_default);
r_public* update_r_public(r_contact *c,str aor,int *is_default);
void del_r_public(r_contact *c,r_public *p);
void free_r_public(r_public *p);

r_ipsec* new_r_ipsec(unsigned int spi_uc,unsigned int spi_us,unsigned int spi_pc,unsigned int spi_ps,unsigned short int port_uc,unsigned short int port_us,
	str ealg_setkey,str r_ealg, str ck_esp,str alg_setkey,str r_alg, str ik_esp, str prot, str mod);
void free_r_ipsec(r_ipsec *ipsec);

r_tls* new_r_tls(int port_tls, unsigned long session_hash);
void free_r_tls(r_tls *tls);

r_security *new_r_security(str sec_header,r_security_type type,float q);
void free_r_security(r_security *s);

r_contact* new_r_contact(str host,int port,int transport,str uri,enum Reg_States reg_state,int expires,
	str *service_route,int service_route_cnt, r_reg_type sos_flag);	
r_contact* get_r_contact(str host,int port,int transport, r_reg_type sos_mask);
r_contact* add_r_contact(str host,int port,int transport,str uri,
	enum Reg_States reg_state,int expires,str *service_route,int service_route_cnt, r_nat_dest * pinhole, r_reg_type sos_flag);
r_contact* update_r_contact(str host,int port,int transport,
				str *uri,enum Reg_States  *reg_state,int *expires,str **service_route,
				int *service_route_cnt, r_nat_dest ** pinhole, int *sos_flag);
r_contact* update_r_contact_sec(str host,int port,int transport,
	str *uri,enum Reg_States *reg_state,int *expires,
	r_security *s, int * sos_flag);
void del_r_contact(r_contact *c);
void free_r_contact(r_contact *c);

r_nat_dest * get_r_nat_pinhole(str host, int port, int transport);

r_contact * get_next_em_r_contact(str pub_id, contact_t * contact);


void print_r(int log_level);



#endif //P_CSCF_REGISTRAR_STORAGE_H_
