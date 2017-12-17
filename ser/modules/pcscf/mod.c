/*
 * $Id: mod.c 1131 2011-08-08 10:11:55Z aon $
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
 * Proxy-CSCF - SER module interface
 * 
 * Scope:
 * - Exports parameters and functions
 * - Initialization functions
 * 
 *  \author Dragos Vingarzan vingarzan -at- fokus dot fraunhofer dot de
 *  \author Ancuta Onofrei	andreea dot ancuta dot onofrei -at- fokus dot fraunhofer dot de 
 *  				adapt P-CSCF to 3GPP TS 23.167, but also considering 24.229 and 23.003
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
#else 
	#include "../../modules/dialog/dlg_mod.h"
#endif
#include "../cdp/cdp_load.h"
#include "../cdp_avp/mod_export.h"
#include "../Client_Rf/client_rf_load.h"

#include "registration.h"
#include "registrar_storage.h"
#include "registrar_subscribe.h"
#include "registrar.h"
#include "nat_helper.h"
#include "security.h"
#include "dlg_state.h"
#include "sdp_util.h"
#include "p_persistency.h"
#include "release_call.h"
#include "ims_pm_pcscf.h"
#include "emerg.h"


#include "policy_control.h"
#include "pcc.h"

MODULE_VERSION

static int mod_init(void);
static int mod_child_init(int rank);
static void mod_destroy(void);


/* parameters storage */
char* pcscf_name="sip:pcscf.open-ims.test:4060";	/**< SIP URI of this P-CSCF */


/* P-Charging-Vector parameters */
extern char* cscf_icid_value_prefix;			/**< hexadecimal prefix for the icid-value - must be unique on each node */
extern unsigned int* cscf_icid_value_count;		/**< to keep the number of generated icid-values 	*/
extern gen_lock_t* cscf_icid_value_count_lock;	/**< to lock acces on the above counter				*/
extern char* cscf_icid_gen_addr;				/**< address of the generator of the icid-value 	*/
extern char* cscf_orig_ioi;						/**< name of the Originating network 				*/
extern char* cscf_term_ioi;						/**< name of the Terminating network 				*/


int   pcscf_use_ipsec=0;					/**< whether to use or not ipsec 					*/
char* pcscf_ipsec_host="127.0.0.1";			/**< IP for protected server 						*/
int   pcscf_ipsec_port_c=4060;				/**< PORT for protected client 						*/
int   pcscf_ipsec_port_s=4060;				/**< PORT for protected server 						*/

int   pcscf_use_tls=0;						/**< whether to use or not tls 						*/
int   pcscf_tls_port=4061;					/**< PORT for TLS server 							*/

char* pcscf_ipsec_P_Inc_Req	="/opt/OpenIMSCore/ser_ims/modules/pcscf/ipsec_P_Inc_Req.sh";		/**< Req E->P */
char* pcscf_ipsec_P_Out_Rpl	="/opt/OpenIMSCore/ser_ims/modules/pcscf/ipsec_P_Out_Rpl.sh";		/**< Rpl E<-P */
char* pcscf_ipsec_P_Out_Req	="/opt/OpenIMSCore/ser_ims/modules/pcscf/ipsec_P_Out_Req.sh";		/**< Req E<-P */
char* pcscf_ipsec_P_Inc_Rpl	="/opt/OpenIMSCore/ser_ims/modules/pcscf/ipsec_P_Inc_Rpl.sh";		/**< Rpl E->P */
char* pcscf_ipsec_P_Drop	="/opt/OpenIMSCore/ser_ims/modules/pcscf/ipsec_P_Drop.sh";		/**< Drop */

int registrar_hash_size=1024;				/**< the size of the hash table for registrar		*/

char *pcscf_reginfo_dtd="/opt/OpenIMSCore/ser_ims/modules/pcscf/reginfo.dtd";/**< DTD to check the reginfo/xml in the NOTIFY to reg */
int pcscf_subscribe_retries = 1;			/**< times to retry subscribe to reg on failure 	*/

int pcscf_assert_fallback = 0;				/**< whether to fallback and use the From header on 
												 identity assertion when P-Preferred-Identity is
												 missing 										*/ 


int subscriptions_hash_size=1024;			/**< the size of the hash table for subscriptions	*/

int pcscf_dialogs_hash_size=1024;			/**< the size of the hash table for dialogs			*/
int pcscf_dialogs_expiration_time=3600;		/**< expiration time for a dialog					*/
int pcscf_dialogs_enable_release=1;			/**< if to enable dialog release					*/
int pcscf_min_se=90;						/**< Minimum session-expires accepted value			*/
int* pcscf_dialog_count = 0;				/**< Counter for saved dialogs						*/
int pcscf_max_dialog_count=20000;			/**< Maximum number of dialogs						*/ 
gen_lock_t* pcscf_dialog_count_lock=0; 		/**< Lock for the dialog counter					*/


int pcscf_nat_enable = 1; 					/**< whether to enable NAT							*/
int pcscf_nat_ping = 1; 					/**< whether to ping anything 						*/
int pcscf_nat_pingall = 0; 					/**< whether to ping also the UA that don't look like being behind a NAT */
int pcscf_nat_detection_type = 0; 			/**< the NAT detection tests 						*/

struct socket_info* force_socket = 0;		/**< 												*/

struct rtpp_head rtpp_list;					/**< RTPProxy list 									*/
int rtpp_node_count = 0;					/**< RTPProxy list count 							*/
char *force_socket_str = 0 ;				/**<  												*/
char *rtpproxy_sock = "unix:/var/run/rtpproxy.sock"; /* list of RTPProxy sockets				*/
int rtpproxy_enable = 0; 					/**< if the RTPProxy is enabled 					*/
int rtpproxy_disable_tout = 60 ;			/**< disabling timeout for the RTPProxy 			*/
int rtpproxy_retr = 5;						/**< Retry count 									*/
int rtpproxy_tout = 1;						/**< Timeout 										*/

/* e2 interface with CLF */
char* forced_clf_peer="";					/**< FQDN of the forced CLF Diameter Peer (CLF) */
int pcscf_use_e2=0;							/**< if to enable usage of e2 or not			*/ 


/* fixed parameter storage */
str pcscf_name_str;							/**< fixed SIP URI of this P-CSCF 					*/
str pcscf_path_hdr_str;						/**< fixed Path header 								*/
str pcscf_path_str;							/**< fixed Path URI  								*/

str forced_clf_peer_str;					/**< fxied FQDN of the forced CLF DiameterPeer (CLF)*/

/*Emergency Services variables/parameters*/
char* ecscf_uri ="sip:ecscf.open-ims.test:7060";				/** the e-cscf uri*/
str ecscf_uri_str;
int emerg_support = 1;
int anonym_em_call_support = 1;
char* emerg_numbers_file="/opt/OpenIMSCore/ser_ims/modules/pcscf/emerg_info.xml";
str pcscf_path_orig_em_uri_str={0,0};
char * pcscf_path_orig_em_uri = "Path: sip:orig.em@pcscf.open-ims.test\r\n";

str pcscf_record_route_mo;					/**< Record-route for originating case 				*/
str pcscf_record_route_mo_uri;				/**< URI for Record-route originating				*/ 
str pcscf_record_route_mt;					/**< Record-route for terminating case 				*/
str pcscf_record_route_mt_uri;				/**< URI for Record-route terminating				*/

extern str cscf_icid_value_prefix_str;				/**< fixed hexadecimal prefix for the icid-value - must be unique on each node */
extern str cscf_icid_gen_addr_str;					/**< fixed address of the generator of the icid-value */
extern str cscf_orig_ioi_str;						/**< fixed name of the Originating network 			*/
extern str cscf_term_ioi_str;						/**< fixed name of the Terminating network 			*/


persistency_mode_t pcscf_persistency_mode=NO_PERSISTENCY;			/**< the type of persistency				*/
char* pcscf_persistency_location="/opt/OpenIMSCore/persistency";	/**< where to dump the persistency data 	*/
int pcscf_persistency_timer_dialogs=60;								/**< interval to snapshot dialogs data		*/ 
int pcscf_persistency_timer_registrar=60;							/**< interval to snapshot registrar data	*/ 
int pcscf_persistency_timer_subscriptions=60;						/**< interval to snapshot subscriptions data*/ 
char* pcscf_db_url="postgres://mario:mario@localhost/pcscfdb";
int* subs_snapshot_version;	/**< the version of the next subscriptions snapshot on the db*/
int* subs_step_version;	/**< the step version within the current subscriptions snapshot version*/
int* dialogs_snapshot_version; /**< the version of the next dialogs snapshot on the db*/
int* dialogs_step_version; /**< the step version within the current dialogs snapshot version*/
int* registrar_snapshot_version; /**< the version of the next registrar snapshot on the db*/
int* registrar_step_version; /**< the step version within the current registrar snapshot version*/

gen_lock_t* db_lock; /**< lock for db access*/ 

int * shutdown_singleton;				/**< Shutdown singleton 								*/
int * callback_singleton;				/**< Callback singleton 								*/

#ifdef WITH_IMS_PM
	/** IMS PM parameters storage */
	char* ims_pm_node_type="P-CSCF";
	char* ims_pm_logfile="/opt/OpenIMSCore/default_ims_pm.log";
#endif /* WITH_IMS_PM */


int pcscf_use_pcc = 0;								/**< whether to enable or disable pcc */
char* pcscf_forced_qos_peer = "";					/**< FQDN of Policy Decision Function (PDF) for policy control */
str forced_qos_peer; 
int pcscf_qos_release7 = 0; 						/**< weather to use Gq or Rx >**/
int pcc_use_ports = 1; 						/**< weather to use ports in the IPFilterRule >**/
int pcc_use_icid = 1;						/**< weather to send the IMS charging id on the Rx interface >**/
int pcc_use_protocol = 1;					/**< weather to include the protocol (tcp, udp) int the Flow Description, or use only ip as generic protocol >**/

char* ipv4_for_signaling_char="127.0.0.1";
str ipv4_for_signaling;

char* ipv6_for_signaling_char="::1";
str ipv6_for_signaling;

unsigned short port_for_signaling = 4060;

char* pcc_serv_id_register_s = "IMS Registration";
str pcc_serv_id_register;

char* pcc_serv_id_call_s = "IMS Services";
str pcc_serv_id_call;

char* pcc_dest_realm_s = "open-ims.test";
str pcc_dest_realm;

char * gg_af_ip = NULL;
uint32_t gg_af_port = 0;
int pcscf_use_client_rf = 0;								/**< whether to enable or disable pcc */


/** 
 * Exported functions.
 * - P_add_path() - Add the Path header to the message
 * - P_add_require() - Add the Require header to the message
 * - P_add_p_charging_vector() - Add the P-Charging-Vector header to the message
 * - P_add_integrity_protected("yes"/"no") - Add the integrity-protected parameter to the Authorization header
 * - P_add_p_visited_network_id("network_identifier") - Add the P-Visited-Network-Id header to the message
 * <p>
 * - P_remove_ck_ik() - remove the Cypher Key and the Integrity Key parameteres from the 
 * WWW-Authorize header
 * - P_remove_security_client() - remove the Security-Client header
 * - P_remove_security_verify() - remove the Security-Verify header
 * - P_remove_security_headers() - remove the Security- headers header
 * - P_remove_header_tag(hdr_name,tag) - Removes tags from headers (for example sec-agree from Require).
 * <p>
 * - P_verify_security() - Verify Register Security-Verify
 * - P_security_401() - create IPSec Security Associations for the 401 Unauthorized response to REGISTER
 * - P_security_200() - create/drop IPSec Security Associations for the 200 OK response to REGISTER
 * - P_is_integrity_protected() - checks if the message was received over a secure channel
 * - P_security_relay() - forward a message through TLS/IPSec
 * <p>
 * - P_route_to_IBCF() - add a Route header to IBCF and set dst_uri to IBCF uri
 * <p>
 * - P_save_location() - save the contacts for the 200 OK response to REGISTER in the local registrar
 * - P_subscribe() - subscribe to the reg event to the S-CSCF for the 200 OK response to REGISTER
 * - P_is_registered() - check if the originator contact of this message is registered at a S-CSCF
 * - P_assert_identity() - assert the identity by removing the P-Preffered-Identity/P-Asserted-Identity header and 
 *  then add a trusted P-Asserted-Identity header
 * - P_is_deregistration() - returns true if there's an Expires header set to 0 or the maximum expires parameter between all contacts is 0
 * <p>
 * - P_process_notification() - process a NOTIFY for the reg event
 * <p>
 * - P_mobile_terminating() - checks if the request contains the Routes indicating a terminating case,
 * as specified in the Path header at registration
 * - P_remove_route() - remove a route header - deprecated, loose_route() should do the job
 * <p>
 * - P_NAT_relay() - forward a message through a NAT
 * - P_SDP_manipulate() - manipulate a SDP to pipe the media through the RTP Proxy (for NAT)
 * <p>
 * - P_follows_service_routes() - checks if the request follows the Service-Route headers
 * indicated at the registration
 * - P_enforce_service_routes() - enforces the Service-Route headers indicated at registration
 * <p>
 * - P_is_in_dialog("orig"/"term") - checks if a requests is in a dialog
 * - P_save_dialog("orig"/"term") - saves a dialog in the local list
 * - P_update_dialog("orig"/"term") - updates a dialog in the local list
 * - P_drop_dialog("orig"/"term") - drops a dialog from the local list
 * - P_follows_dialog_routes("orig"/"term") - checks if a subsequent request follows the saved dialog routes
 * - P_enforce_dialog_routes("orig"/"term") - enforces the dialog routes
 * - P_record_route("orig"/"term") - records route
 * - P_check_session_expires() - Checks if Session-Expires value is over Min_SE local policy
 * - P_422_session_expires() - Return a 422 response with Min_SE set to local policy 
 * <p>
 * - P_assert_called_identity() - asserts the called identity by adding the P-Asserted-Identity header
 * <p>
 * - P_trans_in_processing() - checks if the transaction is already in processing
 * <p>
 * - P_check_via_sent_by() - checks if the sent-by parameter in the first Via header equals the source IP address of the message
 * - P_add_via_received() - adds a received parameter to the first via header with the srouce IP address 
 * <p>
 * - P_follows_via_list() - checks if a response coming from a UE contains the same Via headers sent in the corresponding request
 * - P_enforce_via_list() - enforce a response coming from a UE to contain the same Via headers sent in the corresponding request
 * <p>
 * - P_follows_record_routes() - checks if a response coming from a UE on the MT side contains the same Record-Routes as the request
 * - P_enforce_record_route() - enforce a response coming from a UE on the MT side to contain the same Record-Routes as the request
 * <p>
 * - P_release_call_on_reply() - destroy a call on a reply
 * <p>
 * - P_access_network_info() - modify the P_Access_Network_Info header with e2 information from CLF
 * <p>
 * - P_is_anonymous_user()- checks if the request came from an anonymous user, rfc 3261
 * - P_emergency_flag() - checks if the user made an Emergency Registration
 * - P_enforce_sos_routes() - deletes all Route headers and adds one with the URI of the selected E-CSCF
 * - P_380_em_alternative_serv() - Create the body of a 380 Alternative Service reply for Emergency reasons (e.g. emergency Registration needed) 
 * - P_emergency_ruri() - checks if the Request Uri is used for an Emergency Service (e.g. "urn:service:sos")
 * - P_emergency_serv_enabled() - checks if the module has Emergency Services enabled 
 * - P_enforce_sos_routes() - Inserts the Route header containing the ecscf selected to be enforced
 * <p>
 */
static cmd_export_t pcscf_cmds[]={
	{"P_add_path",					P_add_path, 				0, 0, REQUEST_ROUTE},
	{"P_add_require",				P_add_require, 				0, 0, REQUEST_ROUTE},
	{"P_add_p_charging_vector",		P_add_p_charging_vector, 	0, 0, REQUEST_ROUTE},
	{"P_add_integrity_protected",	P_add_integrity_protected, 	1, 0, REQUEST_ROUTE},
	{"P_add_p_visited_network_id",	P_add_p_visited_network_id, 1, 0, REQUEST_ROUTE},
	
	{"P_remove_ck_ik",				P_remove_ck_ik, 			0, 0, ONREPLY_ROUTE},
	{"P_remove_security_client",	P_remove_security_client, 	0, 0, REQUEST_ROUTE},	
	{"P_remove_security_verify",	P_remove_security_verify, 	0, 0, REQUEST_ROUTE},	
	{"P_remove_security_headers",	P_remove_security_headers, 	0, 0, REQUEST_ROUTE},	
	{"P_remove_header_tag",			P_remove_header_tag, 		2, 0, REQUEST_ROUTE},

	{"P_verify_security",			P_verify_security, 			0, 0, REQUEST_ROUTE},
	{"P_security_401",				P_security_401, 			0, 0, ONREPLY_ROUTE},
	{"P_security_200",				P_security_200,				0, 0, ONREPLY_ROUTE},	
	{"P_is_integrity_protected",	P_is_integrity_protected, 	0, 0, REQUEST_ROUTE},	
	{"P_security_relay", 			P_security_relay, 			0, 0, REQUEST_ROUTE|ONREPLY_ROUTE},
	
	{"P_route_to_IBCF",				P_route_to_IBCF,			1, 0, REQUEST_ROUTE},
	
	{"P_save_location",				P_save_location, 			0, 0, ONREPLY_ROUTE},	
	{"P_subscribe",					P_subscribe, 				0, 0, ONREPLY_ROUTE},	
	{"P_is_registered",				P_is_registered, 			0, 0, REQUEST_ROUTE},
	{"P_assert_identity",			P_assert_identity, 			1, fixup_assert_id, REQUEST_ROUTE},
	{"P_is_deregistration",			P_is_deregistration, 		0, 0, REQUEST_ROUTE|ONREPLY_ROUTE},

	{"P_process_notification",		P_process_notification, 	0, 0, REQUEST_ROUTE},

	{"P_mobile_terminating",		P_mobile_terminating, 		0, 0, REQUEST_ROUTE},
	{"P_remove_route",				P_remove_route, 			1, 0, REQUEST_ROUTE},
	
	{"P_NAT_relay", 				P_NAT_relay, 				0, 0, REQUEST_ROUTE|ONREPLY_ROUTE},
	{"P_SDP_manipulate", 			P_SDP_manipulate, 			1, 0, REQUEST_ROUTE | ONREPLY_ROUTE },
	
	{"P_follows_service_routes",	P_follows_service_routes, 	0, 0, REQUEST_ROUTE},
	{"P_enforce_service_routes",	P_enforce_service_routes, 	0, 0, REQUEST_ROUTE},

	{"P_is_in_dialog",				P_is_in_dialog, 			1, 0, REQUEST_ROUTE},
	{"P_save_dialog",				P_save_dialog, 				2, fixup_save_dialog, REQUEST_ROUTE},
	{"P_update_dialog",				P_update_dialog, 			1, 0, REQUEST_ROUTE|ONREPLY_ROUTE},
	{"P_drop_dialog",				P_drop_dialog, 				1, 0, REQUEST_ROUTE|ONREPLY_ROUTE|FAILURE_ROUTE},
	{"P_follows_dialog_routes",		P_follows_dialog_routes, 	1, 0, REQUEST_ROUTE},
	{"P_enforce_dialog_routes",		P_enforce_dialog_routes, 	1, 0, REQUEST_ROUTE},
	{"P_record_route",				P_record_route,				1, 0, REQUEST_ROUTE},		
	{"P_check_session_expires",		P_check_session_expires, 	0, 0, REQUEST_ROUTE},
	{"P_422_session_expires",		P_422_session_expires,	 	0, 0, REQUEST_ROUTE},

	{"P_assert_called_identity",	P_assert_called_identity, 	0, 0, ONREPLY_ROUTE},
	
	{"P_trans_in_processing",		P_trans_in_processing, 		0, 0, REQUEST_ROUTE},

	{"P_check_via_sent_by",			P_check_via_sent_by, 		0, 0, REQUEST_ROUTE},
	{"P_add_via_received",			P_add_via_received, 		0, 0, REQUEST_ROUTE},
	
	{"P_follows_via_list",			P_follows_via_list, 		0, 0, ONREPLY_ROUTE|FAILURE_ROUTE},
	{"P_enforce_via_list",			P_enforce_via_list, 		0, 0, ONREPLY_ROUTE|FAILURE_ROUTE},

	{"P_follows_record_routes",		P_follows_record_routes, 	0, 0, ONREPLY_ROUTE},
	{"P_enforce_record_routes",		P_enforce_record_routes, 	0, 0, ONREPLY_ROUTE},

	{"P_release_call_onreply",		P_release_call_onreply,		1, 0, ONREPLY_ROUTE},
	
	{"P_access_network_info",		P_access_network_info, 		1, 0, REQUEST_ROUTE},
	
	
	/*emergency services exported functions*/
	{"P_accept_anonym_em_call",		P_accept_anonym_em_call, 	0, 0, REQUEST_ROUTE},
	{"P_is_anonymous_user",			P_is_anonymous_user, 		0, 0, REQUEST_ROUTE},
	{"P_emergency_flag",			P_emergency_flag,		0, 0, REQUEST_ROUTE|ONREPLY_ROUTE},
	{"P_380_em_alternative_serv",		P_380_em_alternative_serv,	1, fixup_380_alt_serv, REQUEST_ROUTE},
	{"P_emergency_ruri",			P_emergency_ruri,		0, 0, REQUEST_ROUTE},
	{"P_emergency_serv_enabled",		P_emergency_serv_enabled,	0, 0, REQUEST_ROUTE},
	{"P_select_ecscf",			P_select_ecscf,			0, 0, REQUEST_ROUTE},
	{"P_enforce_sos_routes",		P_enforce_sos_routes, 		0, 0, REQUEST_ROUTE},
	{"P_is_em_registered",			P_is_em_registered, 		0, 0, REQUEST_ROUTE},
	{"P_add_em_path", 			P_add_em_path,			0, 0, REQUEST_ROUTE},
	{"P_check_em_path", 			P_check_em_path,		0, 0, REQUEST_ROUTE|ONREPLY_ROUTE|FAILURE_ROUTE},

	/* For Gq or Rx*/
	{"P_release_call_onreply",		P_release_call_onreply,		1, 0, ONREPLY_ROUTE}, 
	{"P_AAR",						P_AAR,						1, 0, ONREPLY_ROUTE},
	{"P_STR",						P_STR,						1, 0, REQUEST_ROUTE|ONREPLY_ROUTE},
	{"P_generates_aar",				P_generates_aar,			1, 0, ONREPLY_ROUTE},

	{0, 0, 0, 0, 0}
}; 

/** 
 * Exported parameters.
 * - name - name of the P-CSCF
 * <p>  
 * - registrar_hash_size - size of the registrar hash table
 * - reginfo_dtd - DTD file for checking the reginfo/xml in the NOTIFY to reg event
 * - subscriptions_hash_size - size of the subscriptions hash table
 * <p>
 * - dialogs_hash_size - size of the dialog hash table
 * - dialogs_expiration_time - time-out for dialog expiration
 * - dialogs_enable_release - whether to enable P-CSCF initiated dialog release
 * - max_dialog_count - the maximum number of dialogs to keep, -1 if not limited
 * - min_se - default value for Min_SE header
 * <p>
 * - use_ipsec - if to enable the use of IPSec
 * - ipsec_host - IP of the IPSec host
 * - ipsec_port_c - client port for IPSec
 * - ipsec_port_s - server port for IPSec
 * - ipsec_P_Inc_Req - path to IPSec setting script/executable for Incoming Requests
 * - ipsec_P_Out_Rpl - path to IPSec setting script/executable for Outgoing Replies
 * - ipsec_P_Out_Req - path to IPSec setting script/executable for Outgoing Requests
 * - ipsec_P_Inc_Rpl - path to IPSec setting script/executable for Incoming Replies
 * - ipsec_P_Drop - path to IPSec setting script/executable for dropping all SAs
 * <p>
 * - use_tls - if to enable the use of TLS
 * - tls_port - server port for IPSec
 * <p>
 * - NAT_enable - if to enable NAT detection for signalling
 * - ping - if to ping endpoints to keep pinholes alive
 * - ping_all - if to ping all endpoints, irespective of their IP networks being public
 * - nat_detection_type - which NATs to detect
 * <p>
 * - rtpproxy_enable - if the enable usage of the RTPProxy
 * - rtpproxy_socket - socket to communicate with the RTPProxy through
 * - rtpproxy_disable_tout - timeout to disable the RTPProxy
 * - rtpproxy_retr - retries for RTPProxy
 * - rtpproxy_tout - timeout for RTPProxy
 * <p>
 * - subscribe_retries - how many times to attempt SUBSCRIBE to reg on failure
 * <p>
 * - assert_fallback - whether to fallback and use the From header on identity assertion when P-Preferred-Identity is missing 										
 * <p>
 * - icid_value_prefix - prefix for the ICID in the P-Charging-Vector header
 * - icid_gen_addr - ICID Gen Addr. in the P-Charging-Vector header
 * - orig_ioi - Originating IOI in the P-Charging-Vector header
 * - term_ioi - Terminating IOI in the P-Charging-Vector header
 * <p>
 * - persistency_mode - how to do persistency - 0 none; 1 with files; 2 with db	
 * - persistency_location - where to dump/load the persistency data to/from
 * - persistency_timer_dialogs - interval to make dialogs data snapshots at
 * - persistency_timer_registrar - interval to make registrar snapshots at
 * - persistency_timer_subscriptions - interval to make subscriptions snapshots at
 * <p>
 * - forced_clf_peer - if you enable the use of the e2 interface, you can force the use of a certain CLF
 * - use_e2 - if to enable the usage of e2 interface with a CLF
 *  <p>
 *  - ecscf_uri - the E-CSCF URI to forward the Emergency calls
 *  - emerg_support - if the P-CSCF has support for Emrgency Services or not
 *  - anonym_em_call_support - set if the pcscf should support of not anonymous calls
 *  - emerg_numbers_file -  the file where the emergency numbers and their associated emergency URNs are configured
 *  <p>
 *  - use_pcc - if to use the Policy and Charging Control part
 *  - qos_release7 - whether to use Rx or Gq
 *  - forced_qos_peer - the address of the forced qos peer
 *  - ip_address_for_signaling - 
 *  - pcc_dest_realm - the destination realm, used in PCC
 */	
static param_export_t pcscf_params[]={ 
	{"name", STR_PARAM, &pcscf_name},

	{"registrar_hash_size",				INT_PARAM, 		&registrar_hash_size},
	{"reginfo_dtd", 					STR_PARAM, 		&pcscf_reginfo_dtd},
	{"subscriptions_hash_size",			INT_PARAM,		&subscriptions_hash_size},

	{"dialogs_hash_size",				INT_PARAM,		&pcscf_dialogs_hash_size},
	{"dialogs_expiration_time",			INT_PARAM,		&pcscf_dialogs_expiration_time},
	{"dialogs_enable_release",			INT_PARAM,		&pcscf_dialogs_enable_release},
	{"max_dialog_count",				INT_PARAM,		&pcscf_max_dialog_count},
	{"min_se",		 					INT_PARAM, 		&pcscf_min_se},
	
	
	{"use_ipsec", 						INT_PARAM,		&pcscf_use_ipsec},
	{"ipsec_host", 						STR_PARAM,		&pcscf_ipsec_host},	
	{"ipsec_port_c",					INT_PARAM,		&pcscf_ipsec_port_c},
	{"ipsec_port_s", 					INT_PARAM,		&pcscf_ipsec_port_s},
	
	{"use_tls", 						INT_PARAM,		&pcscf_use_tls},
	{"tls_port", 						INT_PARAM,		&pcscf_tls_port},
	
	{"ipsec_P_Inc_Req", 				STR_PARAM,		&pcscf_ipsec_P_Inc_Req},
	{"ipsec_P_Out_Rpl", 				STR_PARAM,		&pcscf_ipsec_P_Out_Rpl},
	{"ipsec_P_Out_Req", 				STR_PARAM,		&pcscf_ipsec_P_Out_Req},
	{"ipsec_P_Inc_Rpl", 				STR_PARAM,		&pcscf_ipsec_P_Inc_Rpl},
	{"ipsec_P_Drop", 					STR_PARAM,		&pcscf_ipsec_P_Drop},
	
	{"NAT_enable",						INT_PARAM,		&pcscf_nat_enable},
	{"ping",							INT_PARAM,		&pcscf_nat_ping},
	{"ping_all",						INT_PARAM,		&pcscf_nat_pingall},
	{"nat_detection_type",				INT_PARAM,		&pcscf_nat_detection_type},
	
	{"rtpproxy_enable",     			PARAM_INT,		&rtpproxy_enable      },
	{"rtpproxy_socket",					PARAM_STRING,	&rtpproxy_sock},
	{"rtpproxy_disable_tout", 			PARAM_INT,		&rtpproxy_disable_tout },
	{"rtpproxy_retr",        			PARAM_INT,		&rtpproxy_retr         },
	{"rtpproxy_tout",         			PARAM_INT,		&rtpproxy_tout         },
	
	{"subscribe_retries",				INT_PARAM,		&pcscf_subscribe_retries},

	{"assert_fallback",					INT_PARAM,		&pcscf_assert_fallback},
	
	{"icid_value_prefix",				STR_PARAM,		&cscf_icid_value_prefix},
	{"icid_gen_addr",					STR_PARAM,		&cscf_icid_gen_addr},
	{"orig_ioi",						STR_PARAM,		&cscf_orig_ioi},
	{"term_ioi",						STR_PARAM,		&cscf_term_ioi},

	{"persistency_mode",	 			INT_PARAM, 		&pcscf_persistency_mode},	
	{"persistency_location", 			STR_PARAM,		&pcscf_persistency_location},
	{"persistency_timer_dialogs",		INT_PARAM, 		&pcscf_persistency_timer_dialogs},
	{"persistency_timer_registrar",		INT_PARAM, 		&pcscf_persistency_timer_registrar},
	{"persistency_timer_subscriptions",	INT_PARAM, 		&pcscf_persistency_timer_subscriptions},
	{"pcscf_db_url",					STR_PARAM, 		&pcscf_db_url},
	
#ifdef WITH_IMS_PM
	{"ims_pm_node_type",				STR_PARAM, 		&ims_pm_node_type},
	{"ims_pm_logfile",					STR_PARAM, 		&ims_pm_logfile},
#endif /* WITH_IMS_PM */	
	
	{"forced_clf_peer",					STR_PARAM, 		&forced_clf_peer},
	{"use_e2",							INT_PARAM, 		&pcscf_use_e2},


    {"forced_qos_peer",					STR_PARAM,		&pcscf_forced_qos_peer},
	{"qos_release7",					INT_PARAM,		&pcscf_qos_release7},
	{"use_pcc",							INT_PARAM,		&pcscf_use_pcc},
	{"pcc_ipv4_for_signaling",				STR_PARAM,		&ipv4_for_signaling_char},
	{"pcc_ipv6_for_signaling",				STR_PARAM,		&ipv6_for_signaling_char},
	{"pcc_port_for_signaling",				INT_PARAM,		&port_for_signaling},
	
	{"pcc_dest_realm",					STR_PARAM,		&pcc_dest_realm_s},
	{"pcc_use_ports",					INT_PARAM,		&pcc_use_ports},
	{"pcc_use_protocol",					INT_PARAM,		&pcc_use_protocol},
	{"pcc_serv_id_register",				STR_PARAM,		&pcc_serv_id_register_s},
	{"pcc_serv_id_call",				STR_PARAM,		&pcc_serv_id_call_s},
	{"gg_ip",					STR_PARAM,		&gg_af_ip},
	{"gg_port",					INT_PARAM,		&gg_af_port},

	{"ecscf_uri",						STR_PARAM, 		&ecscf_uri},
	{"emerg_support",					INT_PARAM, 		&emerg_support},
	{"anonym_em_call_support",			INT_PARAM, 		&anonym_em_call_support},
	{"emerg_numbers_file",				STR_PARAM,		&emerg_numbers_file},
	{"pcscf_path_orig_em_uri",				STR_PARAM, &pcscf_path_orig_em_uri},
	{0,0,0} 
};

/** module exports */
struct module_exports exports = {
	"pcscf", 
	pcscf_cmds,
	0,
	pcscf_params,
	
	mod_init,		/* module initialization function */
	0,				/* response function*/
	mod_destroy,	/* destroy function */
	0,				/* onbreak function */
	mod_child_init	/* per-child init function */
};


/* Global variables and imported functions */
int (*sl_reply)(struct sip_msg* _msg, char* _str1, char* _str2); 
										/**< link to the stateless reply function in sl module */

struct tm_binds tmb;            		/**< Structure with pointers to tm funcs 		*/
dlg_func_t dialogb;						/**< Structure with pointers to dialog funcs			*/
struct cdp_binds cdpb;					/**< Structure with pointers to cdp funcs				*/
cdp_avp_bind_t *cdp_avp=0;
struct client_rf_binds client_rfb; 		/**< Structure with pointers to client_rf funcs				*/

extern r_hash_slot *registrar;			/**< the contacts */

extern p_dialog_hash_slot *p_dialogs;	/**< the dialogs hash table				*/

extern unsigned int *current_spi;			/**< current SPI value */
extern gen_lock_t *lock_spi;


static str path_str_s={"Path: <",7};
static str path_str_1={"sip:term@",9};
static str path_str_e={";lr>\r\n",6};

static str s_record_route_s={"Record-Route: <",15};
static str s_mo = {"sip:mo@",7};
static str s_mt = {"sip:mt@",7};
static str s_record_route_lr={";lr",3};
static str s_record_route_e={">\r\n",3};

/**
 * Fix the configuration parameters.
 */
int fix_parameters()
{
	str x;	
		
	pcscf_name_str.s = pcscf_name;
	pcscf_name_str.len = strlen(pcscf_name);	
	
	x = pcscf_name_str;
	if (pcscf_name_str.len>=4 &&
		strncasecmp(pcscf_name_str.s,"sip:",4)==0) 
	{
		x.s += 4;
		x.len -= 4;	
	}
			
	cscf_icid_value_prefix_str.s = cscf_icid_value_prefix;
	cscf_icid_value_prefix_str.len = strlen(cscf_icid_value_prefix);

	cscf_icid_gen_addr_str.s = cscf_icid_gen_addr;
	cscf_icid_gen_addr_str.len = strlen(cscf_icid_gen_addr);
	
	cscf_orig_ioi_str.s = cscf_orig_ioi;
	cscf_orig_ioi_str.len = strlen(cscf_orig_ioi);
	
	cscf_term_ioi_str.s = cscf_term_ioi;
	cscf_term_ioi_str.len = strlen(cscf_term_ioi);

	/*Path header*/
	pcscf_path_str.len = path_str_1.len+x.len;
	pcscf_path_str.s = pkg_malloc(pcscf_path_str.len);
	if (!pcscf_path_str.s){
		LOG(L_ERR, "ERR"M_NAME":mod_init: Error allocating %d bytes\n",
			pcscf_path_str.len);
		pcscf_path_str.len=0;
		return 0;
	}
	pcscf_path_str.len=0;
	STR_APPEND(pcscf_path_str,path_str_1);
	STR_APPEND(pcscf_path_str,x);

	pcscf_path_hdr_str.len = path_str_s.len + pcscf_path_str.len + path_str_e.len;
	pcscf_path_hdr_str.s = pkg_malloc(pcscf_path_hdr_str.len);
	if (!pcscf_path_hdr_str.s){
		LOG(L_ERR, "ERR"M_NAME":mod_init: Error allocating %d bytes\n",
			pcscf_path_hdr_str.len);
		pcscf_path_hdr_str.len=0;
		return 0;
	}
	pcscf_path_hdr_str.len=0;
	STR_APPEND(pcscf_path_hdr_str,path_str_s);	
	STR_APPEND(pcscf_path_hdr_str,pcscf_path_str);
	STR_APPEND(pcscf_path_hdr_str,path_str_e);

	/* Record-routes */
	pcscf_record_route_mo.s = pkg_malloc(s_record_route_s.len+s_mo.len+pcscf_name_str.len+s_record_route_lr.len+s_record_route_e.len);
	if (!pcscf_record_route_mo.s){
		LOG(L_ERR, "ERR"M_NAME":mod_init: Error allocating %d bytes\n",
			s_record_route_s.len+s_mo.len+pcscf_name_str.len+s_record_route_lr.len+s_record_route_e.len);
		return 0;
	}
	pcscf_record_route_mt.s = pkg_malloc(s_record_route_s.len+s_mt.len+pcscf_name_str.len+s_record_route_lr.len+s_record_route_e.len);
	if (!pcscf_record_route_mt.s){
		LOG(L_ERR, "ERR"M_NAME":mod_init: Error allocating %d bytes\n",
			s_record_route_s.len+s_mt.len+pcscf_name_str.len+s_record_route_lr.len+s_record_route_e.len);
		return 0;
	}
	
	pcscf_record_route_mo.len=0;
	STR_APPEND(pcscf_record_route_mo,s_record_route_s);
	if (pcscf_name_str.len>4 && strncasecmp(pcscf_name_str.s,"sip:",4)==0){
		STR_APPEND(pcscf_record_route_mo,s_mo);
		memcpy(pcscf_record_route_mo.s+pcscf_record_route_mo.len,pcscf_name_str.s+4,
			pcscf_name_str.len-4);
		pcscf_record_route_mo.len += pcscf_name_str.len-4;
	} else {
		STR_APPEND(pcscf_record_route_mo,s_mo);
		STR_APPEND(pcscf_record_route_mo,pcscf_name_str);
	}
	STR_APPEND(pcscf_record_route_mo,s_record_route_lr);
	STR_APPEND(pcscf_record_route_mo,s_record_route_e);
	pcscf_record_route_mo_uri.s = pcscf_record_route_mo.s + s_record_route_s.len;
	pcscf_record_route_mo_uri.len = pcscf_record_route_mo.len - s_record_route_s.len - s_record_route_e.len;

	pcscf_record_route_mt.len=0;
	STR_APPEND(pcscf_record_route_mt,s_record_route_s);
	if (pcscf_name_str.len>4 && strncasecmp(pcscf_name_str.s,"sip:",4)==0){
		STR_APPEND(pcscf_record_route_mt,s_mt);
		memcpy(pcscf_record_route_mt.s+pcscf_record_route_mt.len,pcscf_name_str.s+4,
			pcscf_name_str.len-4);
		pcscf_record_route_mt.len += pcscf_name_str.len-4;
	} else {
		STR_APPEND(pcscf_record_route_mt,s_mt);
		STR_APPEND(pcscf_record_route_mt,pcscf_name_str);
	}
	STR_APPEND(pcscf_record_route_mt,s_record_route_lr);
	STR_APPEND(pcscf_record_route_mt,s_record_route_e);
	pcscf_record_route_mt_uri.s = pcscf_record_route_mt.s + s_record_route_s.len;
	pcscf_record_route_mt_uri.len = pcscf_record_route_mt.len - s_record_route_s.len - s_record_route_e.len;

	if(emerg_support){
		ecscf_uri_str.s = ecscf_uri;
		ecscf_uri_str.len = strlen(ecscf_uri);
		LOG(L_INFO, "INFO"M_NAME":mod_init: E-CSCF uri is %.*s\n", ecscf_uri_str.len, ecscf_uri_str.s);
		pcscf_path_orig_em_uri_str.s = pcscf_path_orig_em_uri;
		pcscf_path_orig_em_uri_str.len = strlen(pcscf_path_orig_em_uri);

	}


	/* fix the parameters */
	forced_clf_peer_str.s = forced_clf_peer;
	forced_clf_peer_str.len = strlen(forced_clf_peer);

	/* Address initialization of PDF for policy control */
	forced_qos_peer.s = pcscf_forced_qos_peer;
	forced_qos_peer.len = strlen(pcscf_forced_qos_peer);
	
	//pcc params for signaling parameters of the PCSCF
	ipv4_for_signaling.s = ipv4_for_signaling_char;
	ipv4_for_signaling.len = strlen(ipv4_for_signaling_char);
	
	ipv6_for_signaling.s = ipv6_for_signaling_char;
	ipv6_for_signaling.len = strlen(ipv6_for_signaling_char);
	
	pcc_dest_realm.s = pcc_dest_realm_s;
	pcc_dest_realm.len = strlen(pcc_dest_realm_s);
	
	pcc_serv_id_register.s = pcc_serv_id_register_s;
	pcc_serv_id_register.len = strlen(pcc_serv_id_register_s);

	pcc_serv_id_call.s = pcc_serv_id_call_s;
	pcc_serv_id_call.len = strlen(pcc_serv_id_call_s);

	if(gg_af_ip && pcscf_use_pcc){

		if(gg_af_port<=0 || gg_af_port > 65535) {
			LOG(L_ERR, "invalid port number %i\n", gg_af_port);
			return 0;
		}
		
		if(!create_gg_socket())
			return 0;
		LOG(L_DBG, "using the AF ip %s and port %i\n",
				gg_af_ip, gg_af_port);
	}

	return 1;
}


/**
 * Initializes the module.
 */
static int mod_init(void)
{
	load_tm_f load_tm;
	load_cdp_f load_cdp;
	cdp_avp_get_bind_f load_cdp_avp;
	load_client_rf_f load_client_rf;
	bind_dlg_mod_f load_dlg;
			
	LOG(L_INFO,"INFO:"M_NAME":mod_init: Initialization of module\n");
	shutdown_singleton=shm_malloc(sizeof(int));
	*shutdown_singleton=0;
	callback_singleton=shm_malloc(sizeof(int));
	*callback_singleton=0;
	
	lock_spi = lock_alloc();
	lock_spi = lock_init(lock_spi);
	current_spi = shm_malloc(sizeof(unsigned int));
	*current_spi = 5000;
	
	/* fix the parameters */
	if (!fix_parameters()) goto error;

	#ifdef WITH_IMS_PM
		ims_pm_init(pcscf_name_str,ims_pm_node_type, ims_pm_logfile);
		ims_pm_init_pcscf();
	#endif /* WITH_IMS_PM */
	
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
	
	if(pcscf_persistency_mode==WITH_DATABASE_BULK || pcscf_persistency_mode==WITH_DATABASE_CACHE){
		/* bind to the db module */
		if (!pcscf_db_url) {
			LOG(L_ERR, "ERR:"M_NAME":mod_init: no db_url specified but DB has to be used "
				"(pcscf_persistency_mode=%d\n", pcscf_persistency_mode);
			return -1;
		}
		
		if (!pcscf_db_init(pcscf_db_url)<0){
			LOG(L_ERR, "ERR:"M_NAME": mod_init: Error while connecting database\n");
			return -1;
		}
		
		/* db lock */
		db_lock = (gen_lock_t*)lock_alloc();
		if(!db_lock){
	    	LOG(L_ERR, "ERR:"M_NAME": mod_init: No memory left\n");
			return -1;
		}
		lock_init(db_lock);
	
		/* snapshot and step versions */
	
		subs_snapshot_version=(int*)shm_malloc(sizeof(int));
		if(!subs_snapshot_version){
			LOG(L_ERR, "ERR:"M_NAME":mod_init: subs_snapshot_version, no memory left\n");
			return -1;
		}
		*subs_snapshot_version=0;
	
		subs_step_version=(int*)shm_malloc(sizeof(int));
		if(!subs_step_version){
			LOG(L_ERR, "ERR:"M_NAME":mod_init: subs_step_version, no memory left\n");
			return -1;
		}
		*subs_step_version=0;
	
		dialogs_snapshot_version=(int*)shm_malloc(sizeof(int));
		if(!dialogs_snapshot_version){
			LOG(L_ERR, "ERR:"M_NAME":mod_init: dialogs_snapshot_version, no memory left\n");
			return -1;
		}
		*dialogs_snapshot_version=0;
	
		dialogs_step_version=(int*)shm_malloc(sizeof(int));
		if(!dialogs_step_version){
			LOG(L_ERR, "ERR:"M_NAME":mod_init: dialogs_step_version, no memory left\n");
			return -1;
		}
		*dialogs_step_version=0;
	
		registrar_snapshot_version=(int*)shm_malloc(sizeof(int));
		if(!registrar_snapshot_version){
			LOG(L_ERR, "ERR:"M_NAME":mod_init: registrar_snapshot_version, no memory left\n");
			return -1;
		}
		*registrar_snapshot_version=0;
	
		registrar_step_version=(int*)shm_malloc(sizeof(int));
		if(!registrar_step_version){
			LOG(L_ERR, "ERR:"M_NAME":mod_init: registrar_step_version, no memory left\n");
			return -1;
		}
		*registrar_step_version=0;	
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

	/* bind to the cdp module */
	if (pcscf_use_e2||pcscf_use_pcc){
		if (!(load_cdp = (load_cdp_f)find_export("load_cdp",NO_SCRIPT,0))) {
			LOG(L_ERR, "DBG:"M_NAME":mod_init: Can not import load_cdp. This module requires cdp module.\n");
			
			LOG(L_ERR, "DBG:"M_NAME":mod_init: Usage of the e2 interface as well as the PCC ones have been disabled.\n");			
			pcscf_use_e2 = 0;
			pcscf_use_pcc = 0;
		}else{
			if (load_cdp(&cdpb) == -1)
				goto error;
		}
		
		if (!(load_cdp_avp = (cdp_avp_get_bind_f)find_export("cdp_avp_get_bind",NO_SCRIPT,0))) {
			LOG(L_ERR, "DBG:"M_NAME":mod_init: Can not import cdp_avp_get_bind. This module requires cdp_avp module.\n");			
			LOG(L_ERR, "DBG:"M_NAME":mod_init: Usage of the e2 interface as well as the PCC ones have been disabled.\n");			
			pcscf_use_e2 = 0;
			pcscf_use_pcc = 0;
		}else{
			cdp_avp = load_cdp_avp();
			if (!cdp_avp)
				goto error;
		}
	}

	if (!(load_client_rf = (load_client_rf_f)find_export("load_client_rf",NO_SCRIPT,0))) {
		LOG(L_ERR, "DBG:"M_NAME":mod_init: Can not import load_client_rf. This module might require client_rf module.\n");
			
		LOG(L_ERR, "DBG:"M_NAME":mod_init: Usage of the charging info in the pcscf module will be disabled, no charging correlation possible.\n");			
		pcscf_use_client_rf = 0;
	}else{
		pcscf_use_client_rf = 1;
		if (load_client_rf(&client_rfb) == -1)
			goto error;
	}	
	
	/* init the registrar storage */
	if (!r_storage_init(registrar_hash_size)) goto error;
	if (pcscf_persistency_mode!=NO_PERSISTENCY){
		load_snapshot_registrar();
		if (register_timer(persistency_timer_registrar,0,pcscf_persistency_timer_registrar)<0) goto error;
	}

	/* register the registrar timer */
	if (register_timer(registrar_timer,registrar,10)<0) goto error;
	
	/* init the registrar subscriptions */
	if (!r_subscription_init()) goto error;
	if (pcscf_persistency_mode!=NO_PERSISTENCY){
		load_snapshot_subscriptions();
		if (register_timer(persistency_timer_subscriptions,0,pcscf_persistency_timer_subscriptions)<0) goto error;
	}

	/* register the subscription timer */
	if (register_timer(subscription_timer,registrar,10)<0) goto error;
	
	/* init the dialog storage */
	if (!p_dialogs_init(pcscf_dialogs_hash_size)){
		LOG(L_ERR, "ERR"M_NAME":mod_init: Error initializing the Hash Table for stored dialogs\n");
		goto error;
	}		
	pcscf_dialog_count = shm_malloc(sizeof(int));
	*pcscf_dialog_count = 0;
	pcscf_dialog_count_lock = lock_alloc();
	pcscf_dialog_count_lock = lock_init(pcscf_dialog_count_lock);

	if (pcscf_persistency_mode!=NO_PERSISTENCY){
		load_snapshot_dialogs();
		if (register_timer(persistency_timer_dialogs,0,pcscf_persistency_timer_dialogs)<0) goto error;
	}

	/* register the dialog timer */
	if (register_timer(dialog_timer,p_dialogs,60)<0) goto error;
	
	if (pcscf_nat_enable)
		if(!nat_prepare_1918addr()) goto error;

	/* rtp proxy initilazition */
	if (pcscf_nat_enable && rtpproxy_enable) 
		if (!rtpproxy_init()) goto error;

	if (pcscf_use_tls)
	{
		get_tls_session_hash = (void *)find_export("get_tls_session_hash", 0, 0);
		if (! get_tls_session_hash) {
			LOG(L_ERR,"ERR:"M_NAME":mod_init: get_tls_session_hash not found !\n");
			goto error;
		}
	}

	/* initializing the variables needed for the Emergency Services support*/
	if (init_emergency_cntxt()<0){
		LOG(L_ERR,"ERR:"M_NAME":mod_init: error on init_emergency_cntxt()\n");
		goto error;	
	}
	
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
			
	/* Init the user data parser */
	if (!parser_init(pcscf_reginfo_dtd)) return -1;
	
	if (pcscf_use_pcc){
		lock_get(process_lock);
			if((*callback_singleton)==0){
				*callback_singleton=1;
				cdpb.AAAAddRequestHandler(PCCRequestHandler,NULL);
			}
		lock_release(process_lock);
	}
	/* rtpproxy child init */
	if (pcscf_nat_enable && rtpproxy_enable) 
		if (!rtpproxy_child_init(rank)) return -1;

	return 0;
}

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
		if (pcscf_persistency_mode!=NO_PERSISTENCY){		
			/* First let's snapshot everything */
			make_snapshot_dialogs();
			make_snapshot_registrar();
			make_snapshot_subscriptions();
		}
		/* Then nuke it all */		
		clean_emergency_cntxt();
		parser_destroy();
		r_subscription_destroy();
		r_storage_destroy();
		p_dialogs_destroy();
        lock_get(pcscf_dialog_count_lock);
        shm_free(pcscf_dialog_count);
        lock_destroy(pcscf_dialog_count_lock);
        
        lock_destroy(lock_spi);
        lock_dealloc(lock_spi);
        shm_free(current_spi);
	}
	
	if(gg_af_ip && pcscf_use_pcc){
		close_gg_socket();
	}
	
	if (pcscf_persistency_mode==WITH_DATABASE_BULK || pcscf_persistency_mode==WITH_DATABASE_CACHE) {
		DBG("INFO:"M_NAME": ... closing db connection\n");
		pcscf_db_close();		
	}

	#ifdef WITH_IMS_PM
		ims_pm_destroy();	
	#endif /* WITH_IMS_PM */		
}


/**
 * Checks if the transaction is in processing.
 * @param msg - the SIP message to check
 * @param str1 - not used
 * @param str2 - not used
 * @returns #CSCF_RETURN_TRUE if the transaction is already in processing, #CSCF_RETURN_FALSE if not
 */
int P_trans_in_processing(struct sip_msg* msg, char* str1, char* str2)
{
	unsigned int hash, label;
	if (tmb.t_get_trans_ident(msg,&hash,&label)<0)
		return CSCF_RETURN_FALSE;
	return CSCF_RETURN_TRUE;	
}

