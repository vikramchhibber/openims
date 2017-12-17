/*
 * $Id: Rf_data.c 1177 2012-01-24 15:43:04Z aon $
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


#include "Rf_data.h"
#include "config.h"
#include "charging.h"

extern client_rf_cfg cfg;
acct_record_info_list_t * acct_records;

int init_acct_records(){

	int i;

	mem_new(acct_records, cfg.hash_table_size*sizeof(acct_record_info_list_t), shm);

	for(i=0; i< cfg.hash_table_size; i++){
		acct_records[i].lock = lock_alloc();
		if(!acct_records[i].lock) goto out_of_memory;
		acct_records[i].lock=lock_init(acct_records[i].lock);

	}

	return 1;
out_of_memory:
	LOG(L_ERR, "out of shm memory");
	return 0;
}

void destroy_acct_records(){
	
	int i;
	for(i=0; i< cfg.hash_table_size; i++){

		lock_get(acct_records[i].lock);
		lock_destroy(acct_records[i].lock);
		lock_dealloc(acct_records[i].lock);

		WL_FREE_ALL(acct_records+i, acct_record_info_list_t,shm);
	}
}

/**
 * Computes the hash for a string.
 * @param id - input string
 * @returns - the hash
 */
inline unsigned int calc_hash(str id){
	if (id.len==0) return 0;
#define h_inc h+=v^(v>>3)
   char* p;
   register unsigned v;
   register unsigned h;

   h=0;
   for (p=id.s; p<=(id.s+id.len-4); p+=4){
       v=(*p<<24)+(p[1]<<16)+(p[2]<<8)+p[3];
       h_inc;
   }
   v=0;
   for (;p<(id.s+id.len); p++) {
       v<<=8;
       v+=*p;
   }
   h_inc;

   h=((h)+(h>>11))+((h>>13)+(h>>23));
   return (h)%cfg.hash_table_size;
#undef h_inc 
}

int add_new_acct_record_safe(int hash_index, str id, str uri, uint32_t acct_record_number, int dir, uint32_t expires){

	acct_record_info_list_slot_t * acct_rec = NULL;

	mem_new(acct_rec, sizeof(acct_record_info_list_slot_t), shm);
	acct_rec->acct_record_number = acct_record_number;
	acct_rec->expires = expires;
	acct_rec->dir = dir;
	str_dup(acct_rec->id, id, shm);
	str_dup(acct_rec->uri, uri, shm);
	WL_APPEND(acct_records+hash_index, acct_rec);

	return 1;

out_of_memory:
	LOG(L_ERR, "ERR: add_new_acct_record_safe: out of shm memory\n");
	return 0;
}

/**
 * search for the entry with the specific id, otherwise add it and initialize it with expires, if expires not 0 update
 */
int get_subseq_acct_record_nb(str id, str uri, int32_t acct_record_type, uint32_t * value, int * dir, uint32_t expires){
	
	int hash_index;
	acct_record_info_list_slot_t * acct_rec = NULL;

	if(acct_record_type == AAA_ACCT_EVENT){
		*value = 1;
		return 1;
	}

	hash_index = calc_hash(id);
	lock_get(acct_records[hash_index].lock);
		WL_FOREACH(&(acct_records[hash_index]), acct_rec){
			if(str_equal(acct_rec->id, id) && str_equal(acct_rec->uri, uri)){
				*value = acct_rec->acct_record_number +1;
				acct_rec->acct_record_number = *value;
				if(expires>0) acct_rec->expires = expires;
				*dir = acct_rec->dir;
				break;
			}
		}

		if(!acct_rec && acct_record_type != AAA_ACCT_STOP){
			*value = 1;
			if(!add_new_acct_record_safe(hash_index, id, uri, *value, *dir, expires))
				goto error;
		}

		if(!acct_rec && acct_record_type == AAA_ACCT_STOP){
			LOG(L_ERR, "could not find the record, returning error\n");
			goto error;
		}

		if(acct_rec && acct_record_type == AAA_ACCT_STOP){
			WL_DELETE(&(acct_records[hash_index]), acct_rec);
			acct_record_info_list_t_free(acct_rec,shm);
		}
	lock_release(acct_records[hash_index].lock);
	
	return 1;
error:
	lock_release(acct_records[hash_index].lock);
	return 0;
}


event_type_t * new_event_type(str * sip_method,
				str * event,
				uint32_t * expires)
{
	event_type_t * x = 0;
	
	mem_new(x, sizeof(event_type_t), pkg);
	if(sip_method && sip_method->s)
		str_dup_ptr(x->sip_method, *sip_method,pkg);
	
	if(event && event->s)
		str_dup_ptr(x->event,*event,pkg);
	
	if(expires && *expires!=0){
		mem_new(x->expires, sizeof(uint32_t), pkg);
		*(x->expires) = *expires;
	}
	
	return x;

out_of_memory:
	LOG(L_ERR, "out of pkg memory\n");
	event_type_free(x);
	return NULL;
}

time_stamps_t * new_time_stamps(time_t	*sip_request_timestamp,
		uint32_t *sip_request_timestamp_fraction,
		time_t 	*sip_response_timestamp,
		uint32_t *sip_response_timestamp_fraction){

	time_stamps_t * x= 0;

	mem_new(x, sizeof(time_stamps_t),pkg);

	if(sip_request_timestamp && *sip_request_timestamp>0){
		mem_new(x->sip_request_timestamp, sizeof(time_t), pkg);
		*(x->sip_request_timestamp) = *sip_request_timestamp;
	}

	if(sip_request_timestamp_fraction && *sip_request_timestamp_fraction>0){
		mem_new(x->sip_request_timestamp_fraction, sizeof(uint32_t), pkg);
		*(x->sip_request_timestamp_fraction) = *sip_request_timestamp_fraction;
	}

	if(sip_response_timestamp && *sip_response_timestamp>0){
		mem_new(x->sip_response_timestamp, sizeof(time_t), pkg);
		*(x->sip_response_timestamp) = *sip_response_timestamp;
	}

	if(sip_response_timestamp_fraction && *sip_response_timestamp_fraction>0){
		mem_new(x->sip_response_timestamp_fraction, sizeof(uint32_t), pkg);
		*(x->sip_response_timestamp_fraction) = *sip_response_timestamp_fraction;
	}


	return x;

out_of_memory:
	LOG(L_ERR, "out of pkg memory\n");
	time_stamps_free(x);
	return 0;
}

ims_information_t * new_ims_information(event_type_t * event_type,
					time_stamps_t * time_stamps,
					str * user_session_id, 
					str * outgoing_session_id,
					str * calling_party,
					str * called_party,
					str * icid,
					str * orig_ioi,
					str * term_ioi,
					int node_role)
{

	str_list_slot_t *sl =0;
	ims_information_t *x = 0;
	ioi_list_element_t * ioi_elem = 0;

	mem_new(x, sizeof(ims_information_t), pkg);

	x->event_type = event_type;
	x->time_stamps = time_stamps;

	mem_new(x->role_of_node,sizeof(int32_t),pkg);
	*(x->role_of_node) = node_role;

	x->node_functionality = cfg.node_func;

	if(outgoing_session_id && outgoing_session_id->s)
		str_dup_ptr(x->outgoing_session_id,*outgoing_session_id, pkg);

	if(user_session_id && user_session_id->s)
		str_dup_ptr(x->user_session_id, *user_session_id, pkg);
	
	if(calling_party && calling_party->s){
		mem_new(sl, sizeof(str_list_slot_t), pkg);
		str_dup(sl->data, *calling_party, pkg);
		WL_APPEND(&(x->calling_party_address), sl);
	}

	if(called_party && called_party->s)
		str_dup_ptr(x->called_party_address, *called_party, pkg);

	//WL_FREE_ALL(&(x->called_asserted_identity),str_list_t,pkg);
	//str_free_ptr(x->requested_party_address,pkg);
	
	if ((orig_ioi && orig_ioi->s && orig_ioi->len >0) || 
			(term_ioi && term_ioi->s && term_ioi->len>0)){
		mem_new(ioi_elem, sizeof(ioi_list_element_t),pkg) ;
		if(orig_ioi && orig_ioi->s && orig_ioi->len){
			str_dup_ptr_ptr(ioi_elem->info.originating_ioi, orig_ioi, pkg);
		}
		if(term_ioi && term_ioi->s && term_ioi->len)
			str_dup_ptr_ptr(ioi_elem->info.terminating_ioi, term_ioi, pkg);
		WL_APPEND(&(x->ioi), ioi_elem);
	}

	if (icid && icid->s)
		str_dup_ptr(x->icid,*icid,pkg);

	return x;

out_of_memory:
	LOG(L_ERR, "out of pkg memory\n");
	if(x && ioi_elem  && (!x->ioi.head))
		WL_FREE(ioi_elem, ioi_list_t, pkg);
	ims_information_free(x);
	return NULL;
}

service_data_container_t * new_service_data_container(uint32_t rating_group){

	service_data_container_t * x = NULL;

	mem_new(x, sizeof(service_data_container_t),pkg);
	x->rating_group = rating_group;
	
	return x;
out_of_memory:
	LOG(L_ERR, "out of pkg memory\n");	
	service_data_container_free(x);
	return NULL;
}

ps_information_t * new_ps_information(uint32_t an_charg_id, service_data_container_t * serv_data_container)
{

	ps_information_t * x = NULL;

	mem_new(x, sizeof(ps_information_t), pkg);

	if(an_charg_id){
		mem_new(x->tgpp_charging_id, sizeof(uint32_t), pkg);
		*(x->tgpp_charging_id) = an_charg_id;
        }
        if (serv_data_container)
		WL_APPEND(&(x->service_data_container),serv_data_container);

	return x;

out_of_memory:
	LOG(L_ERR, "out of pkg memory\n");
	ps_information_free(x);
	return NULL;
}

service_information_t * new_service_information(ims_information_t * ims_info,
						ps_information_t * ps_info,
						subscription_id_t * subscription)
{
	subscription_id_list_element_t * sl =0;
	service_information_t * x = 0;

	/*allocate structure*/
	mem_new(x, sizeof(service_information_t), pkg);

	/*copy subscription id*/
	if(subscription){
			mem_new(sl, sizeof(subscription_id_list_element_t), pkg);
			subscription_id_list_t_copy(&(sl->s),subscription,pkg);
			WL_APPEND(&(x->subscription_id),sl);
	}
	
	/*set ims information*/
	x->ims_information = ims_info;
	x->ps_information = ps_info;

	return x;

out_of_memory:
	LOG(L_ERR, "new service information: out of pkg memory\n");
	service_information_free(x);
	return 0;
}


Rf_ACR_t * new_Rf_ACR(int32_t acct_record_type, uint32_t acct_record_number, 
			str * user_name, ims_information_t * ims_info,
			ps_information_t * ps_info,
			subscription_id_t * subscription){


	Rf_ACR_t *x=0;
	service_information_t * service_info=0;
	mem_new(x, sizeof(Rf_ACR_t), pkg);

	str_dup(x->origin_host, cfg.origin_host, pkg);
	str_dup(x->origin_realm, cfg.origin_realm, pkg);
	str_dup(x->destination_realm, cfg.destination_realm, pkg);
	x->acct_record_type = acct_record_type;
	x->acct_record_number = acct_record_number;

	if(user_name){
		str_dup_ptr_ptr(x->user_name, user_name, pkg);
	}
	
	if(cfg.service_context_id.len && cfg.service_context_id.s)
		str_dup_ptr(x->service_context_id, cfg.service_context_id, pkg);

	if(ims_info)	
		if(!(service_info = new_service_information(ims_info, ps_info, subscription)))
			goto error;
	
	x->service_information = service_info;
	service_info = 0;

	return x;

out_of_memory:
	LOG(L_ERR, "out of pkg memory\n");
error:
	Rf_free_ACR(x);
	service_information_free(service_info);

	return 0;
}

void event_type_free(event_type_t *x)
{
	if (!x) return;
	str_free_ptr(x->sip_method,pkg);
	str_free_ptr(x->event,pkg);
	mem_free(x->expires,pkg);
	mem_free(x,pkg);
}

void time_stamps_free(time_stamps_t *x)
{
	if (!x) return;
	mem_free(x->sip_request_timestamp,pkg);
	mem_free(x->sip_request_timestamp_fraction,pkg);
	mem_free(x->sip_response_timestamp,pkg);
	mem_free(x->sip_response_timestamp_fraction,pkg);
	mem_free(x,pkg);
}

void ims_information_free(ims_information_t *x)
{
	if (!x) return;

	event_type_free(x->event_type);
	
	mem_free(x->role_of_node,pkg);
	str_free_ptr(x->user_session_id,pkg);
	str_free_ptr(x->outgoing_session_id,pkg);
	
	WL_FREE_ALL(&(x->calling_party_address),str_list_t,pkg);
	str_free_ptr(x->called_party_address,pkg);
	WL_FREE_ALL(&(x->called_asserted_identity),str_list_t,pkg);
	str_free_ptr(x->requested_party_address,pkg);
	
	time_stamps_free(x->time_stamps);
	
	WL_FREE_ALL(&(x->as_info),as_info_list_t,pkg);
	
	WL_FREE_ALL(&(x->ioi),ioi_list_t,pkg);
	str_free_ptr(x->icid,pkg);
	WL_FREE_ALL(&(x->message_body), message_body_list_t, pkg);
	WL_FREE_ALL(&(x->service_specific_info), service_specific_info_list_t, pkg);
	WL_FREE_ALL(&(x->sdp_media_component),sdp_media_component_list_t,pkg);
	WL_FREE_ALL(&(x->sdp_session_description),str_list_t,pkg);
	WL_FREE_ALL(&(x->early_media_description), early_media_description_list_t, pkg);
	
	str_free_ptr(x->service_id,pkg);
	
	mem_free(x->cause_code,pkg);
	
	mem_free(x,pkg);
}

void service_data_container_free(service_data_container_t *x){
	service_data_container_list_t_free(x,pkg);
}

void ps_information_free(ps_information_t * x){

	if(!x)
		return;
	WL_FREE_ALL(&(x->service_data_container), service_data_container_list_t, pkg);
	if(x->tgpp_charging_id)	mem_free(x->tgpp_charging_id, pkg);
	WL_FREE_ALL(&x->service_data_container, service_data_container_list_t, pkg);
	mem_free(x, pkg);
}

void service_information_free(service_information_t *x)
{
	if (!x) return;

	WL_FREE_ALL(&(x->subscription_id),subscription_id_list_t,pkg);
	ims_information_free(x->ims_information);
	ps_information_free(x->ps_information);
	
	mem_free(x,pkg);
}


void Rf_free_ACR(Rf_ACR_t *x)
{
	if (!x) return;

	str_free(x->origin_host,pkg);
	str_free(x->origin_realm,pkg);
	str_free(x->destination_realm,pkg);
	
	str_free(x->session_id, pkg);

	str_free_ptr(x->user_name,pkg);
	mem_free(x->acct_interim_interval,pkg);
	mem_free(x->origin_state_id,pkg);
	mem_free(x->event_timestamp,pkg);
	
	str_free_ptr(x->service_context_id,pkg);
	
	service_information_free(x->service_information);
	
	mem_free(x,pkg);
}

/*
void Rf_free_ACA(Rf_ACA_t *x)
{
	if (!x) return;
	
	str_free_ptr(x->user_name,pkg);
	mem_free(x->acct_interim_interval,pkg);
	mem_free(x->origin_state_id,pkg);
	mem_free(x->event_timestamp,pkg);
	
	mem_free(x,pkg);
}
*/

service_data_container_t * create_service_data_container(str id,
		ps_report_charging_data_t * charging_data,qos_info_t * qos){

	service_data_container_t * service_data_container = 0;

	WL_NEW(service_data_container, service_data_container_list_t, pkg);

	service_data_container->rating_group = charging_data->rating_group;
	str_dup(service_data_container->af_correlation_info, charging_data->af_correlation_info, pkg);

	if(charging_data->angw_addr.ai_family == AF_INET || charging_data->angw_addr.ai_family == AF_INET6)
		memcpy(&(service_data_container->sgsn_addr), &(charging_data->angw_addr), sizeof(ip_address));

	str_dup(service_data_container->charging_rule_base_name, id, pkg);

	service_data_container->first_usage=charging_data->first_usage;
	service_data_container->last_usage=charging_data->last_usage;

	service_data_container->upload_octets=charging_data->upload_octets;
	service_data_container->download_octets=charging_data->download_octets;
	service_data_container->upload_packets=charging_data->upload_packets;
	service_data_container->download_packets=charging_data->download_packets;

	service_data_container->service_identifier=charging_data->service_identifier;

	if(qos)
		qos_info_t_copy(&(service_data_container->qos),qos,pkg);

	return service_data_container;
out_of_memory:
	LOG(L_ERR, "could not build a new service data container\n");
	WL_FREE(service_data_container, service_data_container_list_t, pkg);
	return NULL;
}

Rf_ACR_t * create_Rf_data(str id, int32_t acct_record_type,
							ps_report_charging_data_t * charging_data,
							qos_info_t *qos){

	Rf_ACR_t * res = 0;
	subscription_id_list_element_t * subscr_el=0;
	ps_information_t * ps_info=0;
	service_data_container_t * service_data_container = 0;
	uint32_t local_seq_nb = 1;

	if(!charging_data){
		LOG(L_ERR, "null charging data parameter\n");
		return NULL;
	}

	mem_new(res, sizeof(Rf_ACR_t), pkg);
	str_dup(res->destination_realm, cfg.destination_realm, pkg);
	str_dup(res->origin_host, cfg.origin_host, pkg);
	str_dup(res->origin_realm, cfg.origin_realm, pkg);

	if(cfg.service_context_id.len && cfg.service_context_id.s){
		mem_new(res->service_context_id, sizeof(str), pkg);
		str_dup(*(res->service_context_id), cfg.service_context_id, pkg);
	}

	str_dup(res->session_id, id, pkg);

	res->acct_record_type = acct_record_type;
	res->acct_record_number = charging_data->acct_record_number;
	mem_new(res->user_name, sizeof(str), pkg);
	str_dup(*(res->user_name), charging_data->subs.id, pkg);

	mem_new(res->service_information, sizeof(service_information_t), pkg);

	WL_NEW(subscr_el, subscription_id_list_t, pkg);
	subscription_id_list_t_copy(&(subscr_el->s),&(charging_data->subs),pkg);
	WL_APPEND(&(res->service_information->subscription_id), subscr_el);
	subscr_el=0;

	mem_new(res->service_information->ims_information, sizeof(ims_information_t), pkg);
	res->service_information->ims_information->node_functionality = cfg.node_func;
	str_dup_ptr(res->service_information->ims_information->user_session_id, charging_data->session_id, pkg);

	mem_new(res->service_information->ps_information, sizeof(ps_information_t), pkg);
	ps_info = res->service_information->ps_information;
	if(charging_data->angw_addr.ai_family == AF_INET || charging_data->angw_addr.ai_family == AF_INET6)
		memcpy(&(ps_info->sgsn_addr), &(charging_data->angw_addr), sizeof(ip_address));
	if(charging_data->lma_addr.ai_family == AF_INET || charging_data->lma_addr.ai_family == AF_INET6)
		memcpy(&(ps_info->ggsn_addr), &(charging_data->lma_addr), sizeof(ip_address));
	if(charging_data->an_chg_id){
		mem_new(ps_info->tgpp_charging_id, sizeof(uint32_t), pkg);
		*ps_info->tgpp_charging_id = charging_data->an_chg_id;
	}
	ps_info->rat = charging_data->rat;

	mem_new(ps_info->node_type, sizeof(int32_t), pkg);
	*ps_info->node_type = cfg.node_func;
	str_dup(ps_info->called_station_id, charging_data->apn, pkg);

	service_data_container = create_service_data_container(id, charging_data, qos);
	if(!service_data_container)
		goto error;
	WL_APPEND(&(ps_info->service_data_container), service_data_container);
	service_data_container->local_sequence_number = local_seq_nb;

	return res;
out_of_memory:
	LOG(L_ERR, "out of memory, could not build the Rf_ACR_t\n");
error:
	if(subscr_el) WL_FREE(subscr_el, subscription_id_list_t, pkg);
	Rf_free_ACR(res);
	return NULL;
}

void free_Rf_data(Rf_ACR_t * x){

	Rf_free_ACR(x);
}


