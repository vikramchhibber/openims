/*
 * $Id: Rf_data.h 1177 2012-01-24 15:43:04Z aon $
 * 
 * Copyright (C) 2009 FhG Fokus
 * 
 * This file is part of the Wharf project.
 * 
 */

/**
 * \file
 * 
 * Client_Rf module - Rf Data structures
 * 
 * 
 *  \author Dragos Vingarzan dragos dot vingarzan -at- fokus dot fraunhofer dot de
 * 
 */ 

#ifndef __CLIENT_RF_Rf_data_H
#define __CLIENT_RF_Rf_data_H

#include "../cdp/diameter.h"
#include "../cdp/diameter_epc.h"
#include "../cdp_avp/mod_export.h"

#ifndef WHARF
#include "str_int_list.h"
#endif /* WHARF */

#ifndef STRUCT_QOS_INFO_T
#define STRUCT_QOS_INFO_T

enum preempt_cap_e_{
	PREEMPT_CAP_ENABLED=0,
	PREEMPT_CAP_DISABLED
} preempt_cap_e;

enum preempt_vul_e_{
	PREEMPT_VUL_ENABLED=0,
	PREEMPT_VUL_DISABLED
} preempt_vul_e;

typedef struct {
	uint32_t v;		/**< Retention Policy Value */
	uint8_t pre_cap;/**< this value is reversed as the AVP_EPC_PreEmption_Capability_Enabled/Disabled which are 0/1. 0 Means enabled. */
	uint8_t pre_vul;/**< this value is reversed as the AVP_EPC_PreEmption_Vulnerability_Enabled/Disabled which are 0/1. 0 Means enabled. */
} arp_t;


typedef struct {
	int32_t QCI; 									/**< QoS Class Identifier - 1 to 9 as in the AVP */
	str bearer_id;									/**< Bearer Identifier */
	uint32_t max_upload,max_download;				/**< Max Bandwidth */
	uint32_t guaranteed_upload,guaranteed_download;	/**< Guaranteed Bitrate */
	uint32_t apn_upload,apn_download; 				/**< APN Aggregate Max Bitrate - all non-GBR bearers for that APN */
	arp_t arp;
} qos_info_t;

#define qos_info_t_free(x,mem) \
do {\
	str_free((x)->bearer_id,mem);\
} while(0)

#define qos_info_t_copy(dst,src,mem) \
do {\
	(dst)->QCI = (src)->QCI;\
	str_dup((dst)->bearer_id,(src)->bearer_id,mem);\
	(dst)->max_upload = (src)->max_upload;\
	(dst)->max_download = (src)->max_download;\
	(dst)->guaranteed_upload = (src)->guaranteed_upload;\
	(dst)->guaranteed_download = (src)->guaranteed_download;\
	(dst)->apn_upload = (src)->apn_upload;\
	(dst)->apn_download = (src)->apn_download;\
	(dst)->arp = (src)->arp;\
} while(0)

/**
 * List of QoS-Info is needed in QoS-Info per QCI
 */
typedef struct _qos_info_list_t_slot {
	int db_id;
	qos_info_t q;
	struct _qos_info_list_t_slot *next,*prev;
} qos_info_list_slot_t;

/**
 * a List of QoS-Info slots
 */
typedef struct {
	qos_info_list_slot_t *head,*tail;
} qos_info_list_t;


#define qos_info_list_t_free(x,mem) \
do{\
	if (x){\
		qos_info_t_free(&((x)->q),mem);\
		mem##_free(x);\
		(x) = 0;\
	}\
}while(0)

#define qos_info_list_t_copy(dst,src,mem) \
do{\
	qos_info_t_copy(&((dst)->q),&((src)->q),mem);\
}while(0)

#endif

#ifdef WHARF
#include "../PCC_common/subscription_id.h"
#include "../PCC_common/reporting_level.h"
#else

typedef enum {
	Subscription_Type_MSISDN	= AVP_EPC_Subscription_Id_Type_End_User_E164,
	Subscription_Type_IMSI		= AVP_EPC_Subscription_Id_Type_End_User_IMSI,
	Subscription_Type_IMPU		= AVP_EPC_Subscription_Id_Type_End_User_SIP_URI,
	Subscription_Type_NAI		= AVP_EPC_Subscription_Id_Type_End_User_NAI,
	Subscription_Type_IMPI 		= AVP_EPC_Subscription_Id_Type_End_User_Private,
} subscription_id_type_e;

typedef struct _subscription_id_t {
	int32_t type;
	str id;
} subscription_id_t;

typedef struct _subscription_id_list_t_slot {
	subscription_id_t s;
	struct _subscription_id_list_t_slot *next,*prev;
} subscription_id_list_element_t;

typedef struct {
	subscription_id_list_element_t *head,*tail;
} subscription_id_list_t;


#define subscription_id_list_t_free(x,mem) \
do{\
	if (x) {\
		str_free((x)->s.id,mem);\
		mem##_free(x);\
		(x) = 0;\
	}\
}while(0)

#define subscription_id_list_t_copy(dst,src,mem) \
do {\
	(dst)->type = (src)->type;\
	str_dup((dst)->id,(src)->id,mem);\
} while(0)

typedef enum {
	Rep_Level_Service_Identifier		=0,
	Rep_Level_Rating_Group			=1,
	Rep_Level_Sponsored_Conectivity		=2,

} reporting_level_e;

#endif

typedef struct {
	str *sip_method;
	str *event;
	uint32_t *expires;
} event_type_t;

typedef struct {
	time_t 		*sip_request_timestamp;
	uint32_t 	*sip_request_timestamp_fraction;
	time_t 		*sip_response_timestamp;
	uint32_t 	*sip_response_timestamp_fraction;
} time_stamps_t;

typedef struct {
	str *application_server;
	str_list_t application_provided_called_party_address;
} as_info_t;

typedef struct _as_info_list_t_slot {
	as_info_t info;
	struct _as_info_list_t_slot *next,*prev;
} as_info_list_element_t;

typedef struct {
	as_info_list_element_t *head,*tail;
} as_info_list_t;


#define as_info_list_t_free(x,mem) \
do{\
	str_list_slot_t *y=0,*z;\
	if (x) {\
		str_free_ptr((x)->info.application_server,mem);\
		for(y=(x)->info.application_provided_called_party_address.head;y;y=z){\
			z=y->next;\
			str_free(y->data,mem);\
			mem_free(y,mem);\
		}\
		mem##_free(x);\
		(x) = 0;\
	}\
}while(0)

#define as_info_list_t_copy(dst,src,mem) \
do {\
	str_dup_ptr_ptr((dst)->info.application_server,(src)->info.application_server,mem);\
	WL_DUP_ALL(&((dst)->info.application_provided_called_party_address),&((src)->info.application_provided_called_party_address),str_list_t,mem);\
} while(0)

typedef struct {
	str *originating_ioi;
	str *terminating_ioi;	
} ioi_t;

typedef struct _ioi_list_t_slot {
	ioi_t info;
	struct _ioi_list_t_slot *next,*prev;
} ioi_list_element_t;

typedef struct {
	ioi_list_element_t *head,*tail;
} ioi_list_t;


#define ioi_list_t_free(x,mem) \
do{\
	if (x) {\
		str_free_ptr((x)->info.originating_ioi,mem);\
		str_free_ptr((x)->info.terminating_ioi,mem);\
		mem##_free(x);\
		(x) = 0;\
	}\
}while(0)

#define ioi_list_t_copy(dst,src,mem) \
do {\
	str_dup_ptr_ptr((dst)->info.originating_ioi,(src)->info.originating_ioi,mem);\
	str_dup_ptr_ptr((dst)->info.terminating_ioi,(src)->info.terminating_ioi,mem);\
} while(0)

	
typedef struct {
	str *data;
	uint32_t *type;	
} service_specific_info_t;

typedef struct _service_specific_info_list_t_slot {
	service_specific_info_t info;
	struct _service_specific_info_list_t_slot *next,*prev;
} service_specific_info_list_element_t;

typedef struct {
	service_specific_info_list_element_t *head,*tail;
} service_specific_info_list_t;


#define service_specific_info_list_t_free(x,mem) \
do{\
	if (x) {\
		str_free_ptr((x)->info.data,mem);\
		mem_free((x)->info.type,mem);\
		mem##_free(x);\
		(x) = 0;\
	}\
}while(0)

#define service_specific_info_list_t_copy(dst,src,mem) \
do {\
	str_dup_ptr_ptr((dst)->info.data,(src)->info.data,mem);\
	mem_dup((dst)->info.type,(src)->info.type,sizeof(uint32_t),mem);\
} while(0)

typedef struct _flow_list_t_slot {
	uint32_t media_component_number;
	int_list_t flow_number;	
	int32_t *final_unit_action;

	struct _flow_list_t_slot *next,*prev;
} flow_t;

typedef struct {
	flow_t *head,*tail;
} flow_list_t;


#define flow_list_t_free(x,mem) \
do{\
	if (x) {\
		int_list_slot_t *i,*in;\
		for(i=(x)->flow_number.head;i;i=in){\
			in = i->next;\
			mem_free(i,mem);\
		}\
		mem_free((x)->final_unit_action,mem);\
		mem##_free(x);\
		(x) = 0;\
	}\
}while(0)

/*
#define flow_list_t_copy(dst,src,mem) \
do {\
	str_dup_ptr_ptr((dst)->info.data,(src)->info.data,mem);\
	mem_dup((dst)->info.type,(src)->info.type,sizeof(uint32_t),mem);\
} while(0)
*/


typedef struct {
	str value;
	flow_list_t flow;
} an_charging_id_t;

#define an_charging_id_t_free(x,mem) \
do{\
	if (x) {\
		str_free((x)->value,mem);\
		flow_t *f,*fn;\
		for(f=(x)->flow.head;f;f=fn){\
			fn = f->next;\
			flow_list_t_free(f,mem);\
		}\
		mem##_free(x);\
		(x) = 0;\
	}\
}while(0)


typedef struct _sdp_media_component_list_t_slot {
	str *sdp_media_name;
	str_list_t sdp_media_descriptions;
	int32_t *media_initiator_flag;
	str	*media_initiator_party;
	str *authorized_qos;
	str *tgpp_charging_id;
	an_charging_id_t *an_charging_id;
	int32_t *sdp_type;
	
	struct _sdp_media_component_list_t_slot *next,*prev;
} sdp_media_component_t;

typedef struct {
	sdp_media_component_t *head,*tail;
} sdp_media_component_list_t;

#define sdp_media_component_list_t_free(x,mem) \
do{\
	if (x) {\
		str_free_ptr((x)->sdp_media_name,mem);\
		mem_free((x)->media_initiator_flag,mem);\
		str_free_ptr((x)->media_initiator_party,mem);\
		str_free_ptr((x)->authorized_qos,mem);\
		str_free_ptr((x)->tgpp_charging_id,mem);\
		an_charging_id_t_free((x)->an_charging_id,mem);\
		mem_free((x)->sdp_type,mem);\
		str_list_slot_t *ls,*lsn;\
		for(ls = (x)->sdp_media_descriptions.head;ls;ls=lsn){\
		     lsn = ls->next;\
		     str_list_t_free(ls,mem);\
		}\
		mem##_free(x);\
		(x) = 0;\
	}\
}while(0)

/*
#define sdp_media_component_list_t_copy(dst,src,mem) \
do {\
	str_dup_ptr_ptr((dst)->info.data,(src)->info.data,mem);\
	mem_dup((dst)->info.type,(src)->info.type,sizeof(uint32_t),mem);\
} while(0)
*/
	

typedef struct _message_body_list_t_slot {
	str content_type;
	uint32_t content_length;	
	str	*content_disposition;
	int32_t *originator;
	
	struct _message_body_list_t_slot *next,*prev;
} message_body_t;

typedef struct {
	message_body_t *head,*tail;
} message_body_list_t;


#define message_body_list_t_free(x,mem) \
do{\
	if (x) {\
		str_free((x)->content_type,mem);\
		str_free_ptr((x)->content_disposition,mem);\
		mem_free((x)->originator,mem);\
		mem##_free(x);\
		(x) = 0;\
	}\
}while(0)

/*
#define message_body_list_t_copy(dst,src,mem) \
do {\
	str_dup_ptr_ptr((dst)->info.data,(src)->info.data,mem);\
	mem_dup((dst)->info.type,(src)->info.type,sizeof(uint32_t),mem);\
} while(0)
*/

typedef struct {
	time_t *sdp_offer_timestamp;
	time_t *sdp_answer_timestamp;
} sdp_timestamps_t;

#define sdp_timestamps_t_free(x,mem) \
do{\
	if (x) {\
		mem_free((x)->sdp_offer_timestamp,mem);\
		mem_free((x)->sdp_answer_timestamp,mem);\
		mem##_free(x);\
		(x) = 0;\
	}\
}while(0)


typedef struct _early_media_description_list_t_slot {
	sdp_timestamps_t *sdp_timestamps;
	sdp_media_component_list_t sdp_media_component;
	str_list_t sdp_session_description;
	
	struct _early_media_description_list_t_slot *next,*prev;
} early_media_description_t;

typedef struct {
	early_media_description_t *head,*tail;
} early_media_description_list_t;


#define early_media_description_list_t_free(x,mem) \
do{\
	if (x) {\
		sdp_timestamps_t_free((x)->sdp_timestamps,mem);\
		sdp_media_component_t *s,*sn;\
		for(s = (x)->sdp_media_component.head;s;s=sn){\
			sn = s->next;\
			sdp_media_component_list_t_free(s,mem);\
		}\
		str_list_slot_t *ls,*lsn;\
		for(ls = (x)->sdp_session_description.head;ls;ls=lsn){\
			lsn = ls->next;\
			str_list_t_free(ls,mem);\
		}\
		mem##_free(x);\
		(x) = 0;\
	}\
}while(0)

/*
#define early_media_description_list_t_copy(dst,src,mem) \
do {\
	str_dup_ptr_ptr((dst)->info.data,(src)->info.data,mem);\
	mem_dup((dst)->info.type,(src)->info.type,sizeof(uint32_t),mem);\
} while(0)
*/

	
typedef struct {
	event_type_t *event_type;
	
	int32_t *role_of_node;
	int32_t node_functionality;
	
	str *user_session_id;
	str *outgoing_session_id;
	
	str_list_t calling_party_address;
	str *called_party_address;
	str_list_t called_asserted_identity;	
	str * requested_party_address;
	
	time_stamps_t *time_stamps;
	
	as_info_list_t as_info;
	
	ioi_list_t ioi;	
	str *icid;

	str_list_t sdp_session_description;
	sdp_media_component_list_t sdp_media_component;
	
	str *service_id;
	
	message_body_list_t message_body;
	early_media_description_list_t early_media_description;

	service_specific_info_list_t service_specific_info;
	
	int32_t *cause_code;
	
} ims_information_t;

/*TS 32.299 for offline charging*/

#ifndef PCC_STRUCTS_RAT_T
#define PCC_STRUCTS_RAT_T
typedef int32_t rat_t;
#endif

typedef struct _service_data_container_list_t_slot{

	ip_address sgsn_addr;
	str af_correlation_info;
	str charging_rule_base_name;
	qos_info_t qos;

	time_t first_usage;
	time_t last_usage;

	uint64_t upload_octets;		/// Amount of traffic matching this rule
	uint64_t download_octets;
	uint64_t upload_packets;		/// Amount of nb of packets matching this rule
	uint64_t download_packets;

	uint32_t service_identifier;

	uint32_t rating_group;
	uint32_t local_sequence_number;
	struct _service_data_container_list_t_slot * next, *prev;
} service_data_container_t;

typedef struct{
	service_data_container_t * head, *tail;
} service_data_container_list_t;


#define service_data_container_list_t_free(x,mem) \
	do{\
		if(x){\
			str_free((x)->af_correlation_info, mem);\
			str_free((x)->charging_rule_base_name, mem);\
			qos_info_t_free(&((x)->qos),mem);\
		}\
	}while(0);

typedef struct{
	uint32_t * tgpp_charging_id;
	rat_t rat;
	ip_address sgsn_addr;
	ip_address ggsn_addr;
	int32_t pdp_type;
	str called_station_id;
	int32_t * node_type;
	service_data_container_list_t service_data_container;
} ps_information_t;

typedef struct {
	subscription_id_list_t subscription_id;
	ps_information_t * ps_information;
	ims_information_t *ims_information;
	
} service_information_t;

typedef struct {
	str origin_host;
	str origin_realm;
	str destination_realm;
	
	str session_id;

	int32_t acct_record_type;
	uint32_t acct_record_number;
	
	str *user_name;
	uint32_t *acct_interim_interval;
	uint32_t *origin_state_id;
	time_t *event_timestamp;
	
	str *service_context_id;
	
	service_information_t *service_information;
} Rf_ACR_t;

typedef struct {
	uint32_t result_code;
		
	int32_t acct_record_type;
	uint32_t acct_record_number;
	
	str *user_name;
	uint32_t *acct_interim_interval;
	uint32_t *origin_state_id;
	time_t *event_timestamp;	
} Rf_ACA_t;


event_type_t * new_event_type(str * sip_method,
				str * event,
				uint32_t * expires);

time_stamps_t * new_time_stamps(time_t	*sip_request_timestamp,
		uint32_t *sip_request_timestamp_fraction,
		time_t 	*sip_response_timestamp,
		uint32_t *sip_response_timestamp_fraction);

ims_information_t * new_ims_information(event_type_t * event_type,
					time_stamps_t * time_stamps,
					str * user_session_id, 
					str * outgoing_session_id,
					str * calling_party,
					str * called_party,
					str * icid,
					str * orig_ioi,
					str * term_ioi,
					int node_role);

service_data_container_t * new_service_data_container(uint32_t rating_group);
ps_information_t * new_ps_information(uint32_t an_charg_id, service_data_container_t * serv_data_container);

void event_type_free(event_type_t *x);
void time_stamps_free(time_stamps_t *x);
void ims_information_free(ims_information_t *x);
void service_data_container_free(service_data_container_t *x);
void ps_information_free(ps_information_t *x);
void service_information_free(service_information_t *x);


typedef struct _acct_record_info_list_t_slot{
	str id;
	str uri;
	uint32_t acct_record_number;
	int dir;
	time_t expires;
	struct _acct_record_info_list_t_slot * next, * prev;
} acct_record_info_list_slot_t;

typedef struct _acct_record_info_list_t{
	gen_lock_t * lock;
	struct _acct_record_info_list_t_slot * head, * tail;
} acct_record_info_list_t;

#define acct_record_info_list_t_free(x,mem) \
do{\
	if (x) {\
		str_free((x)->id,mem);\
		str_free((x)->uri,mem);\
		mem##_free(x);\
		(x) = 0;\
	}\
}while(0)

typedef struct _ps_report_charging_data_t{
	subscription_id_t subs;
	str apn;
	str session_id;
	uint32_t acct_record_number;

	reporting_level_e reporting_level;
	int32_t rating_group;
	int32_t service_identifier;
	rat_t rat;
	ip_address lma_addr;
	ip_address angw_addr;
	uint32_t an_chg_id;
	str af_correlation_info;

	time_t first_usage;
	time_t last_usage;
	time_t last_report;

	/*the next information should be reset after sending the charging information*/
	int32_t upload_octets;		/// Amount of traffic matching this rule that passed through the routing module
	int32_t download_octets;
	int32_t upload_packets;		/// Amount of nb of packets matching this rule that passed through the routing module
	int32_t download_packets;
} ps_report_charging_data_t;


#define ps_report_charging_data_copy(dst,src,mem) \
do {\
	(dst)->subs.type = (src)->subs.type;\
	str_dup((dst)->subs.id, (src)->subs.id, mem);\
	str_dup((dst)->apn, (src)->apn, mem);\
	str_dup((dst)->session_id, (src)->session_id, mem);\
	(dst)->rat = (src)->rat;\
	memcpy(&((dst)->lma_addr), &((src)->lma_addr), sizeof(ip_address));\
	memcpy(&((dst)->angw_addr), &((src)->angw_addr), sizeof(ip_address));\
	(dst)->an_chg_id = (src)->an_chg_id;\
	(dst)->acct_record_number = (src)->acct_record_number;\
	(dst)->reporting_level = (src)->reporting_level;\
	(dst)->rating_group = (src)->rating_group;\
	(dst)->service_identifier = (src)->service_identifier;\
	str_dup((dst)->af_correlation_info, (src)->af_correlation_info, mem);\
	(dst)->first_usage = (src)->first_usage;\
	(dst)->last_usage = (src)->last_usage;\
	(dst)->upload_octets = (src)->upload_octets;\
	(dst)->download_octets = (src)->download_octets;\
	(dst)->upload_packets = (src)->upload_packets;\
	(dst)->download_packets = (src)->download_packets;\
} while(0)



int init_acct_records();
void destroy_acct_records();
int get_subseq_acct_record_nb(str id, str uri, int32_t acct_record_type, uint32_t * value, int * dir, uint32_t expires);


Rf_ACR_t * new_Rf_ACR(int32_t acct_record_type, uint32_t acct_record_number,
		str * user_name, ims_information_t * ims_info,
		ps_information_t * ps_info, subscription_id_t * subscription);
void Rf_free_ACR(Rf_ACR_t *x);

Rf_ACR_t* create_Rf_data (str sessionid, int32_t acct_record_type,
		ps_report_charging_data_t * charging_data, qos_info_t *qos);
void free_Rf_data(Rf_ACR_t * x);

#endif /* __CLIENT_RF_Rf_data_H */
