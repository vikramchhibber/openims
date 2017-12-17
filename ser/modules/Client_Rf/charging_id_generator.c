/*
 * $Id: charging_id_generator.c 1167 2011-10-25 09:17:05Z aon $
 *
 * Copyright (C) 2009 FhG Fokus
 *
 * This file is part of the Wharf project.
 *
 */

#ifdef WHARF

#include <math.h>
#include "config.h"
#include "charging_id_generator.h"


extern client_rf_cfg cfg;
gen_charg_info_list_t * gen_charg_info;

int init_gen_charg_id_info(){

	int i;
	
	mem_new(gen_charg_info, cfg.hash_table_size*sizeof(gen_charg_info_list_t), shm);

	for(i=0; i< cfg.hash_table_size; i++){
		gen_charg_info[i].lock = lock_alloc();
		if(!gen_charg_info[i].lock) goto out_of_memory;
		gen_charg_info[i].lock=lock_init(gen_charg_info[i].lock);

	}

	return 1;
out_of_memory:
	LOG(L_ERR, "out of shm memory");
	return 0;
}

void destroy_gen_charg_id_info(){

	int i;
	for(i=0; i< cfg.hash_table_size; i++){

		lock_get(gen_charg_info[i].lock);
		lock_destroy(gen_charg_info[i].lock);
		lock_dealloc(gen_charg_info[i].lock);

		WL_FREE_ALL(gen_charg_info+i, gen_charg_info_list_t,shm);
	}
}


inline unsigned int gen_charg_info_calc_hash(str input_id){
	if (input_id.len==0) return 0;
#define h_inc h+=v^(v>>3)
   char* p;
   register unsigned v;
   register unsigned h;

   h=0;
   for (p=input_id.s; p<=(input_id.s+input_id.len-4); p+=4){
       v=(*p<<24)+(p[1]<<16)+(p[2]<<8)+p[3];
       h_inc;
   }
   v=0;
   for (;p<(input_id.s+input_id.len); p++) {
       v<<=8;
       v+=*p;
   }
   h_inc;

   h=((h)+(h>>11))+((h>>13)+(h>>23));
   return (h)%cfg.hash_table_size;
#undef h_inc
}

gen_charg_info_list_slot_t * create_new_gen_charg_id_info(int hash_index, str user_id,
															str instance_id, uint32_t gen_charg_id){

	gen_charg_info_list_slot_t * info = NULL;

	mem_new(info, sizeof(gen_charg_info_list_slot_t), shm);
	str_dup(info->user_id, user_id, shm);
	str_dup(info->instance_id, instance_id, shm);
	info->charging_id = gen_charg_id;

	return info;

out_of_memory:
	LOG(L_ERR, "ERR: add_new_gen_charg_id_info_safe: out of shm memory\n");
	return 0;
}

uint32_t compute_charg_id(str input_id, time_t timestamp){

    uint32_t id =0;
    if(!input_id.len) return id;
    char * start = (char*)&timestamp;
    char * end = (char*)&timestamp+sizeof(time_t)-1;
	
#define h_inc h+=v^(v>>3)
   char* p;
   register unsigned v;
   register unsigned h;

   h=0;
   for (p=input_id.s; p<=(input_id.s+input_id.len-4); p+=4){
       v=(*p<<24)+(p[1]<<16)+(p[2]<<8)+p[3];
       h_inc;
   }
   v=0;
   for (;p<(input_id.s+input_id.len); p++) {
       v<<=8;
       v+=*p;
   }
   h_inc;
   
   for (p=start; p<=(end-4); p+=4){
       v=(*p<<24)+(p[1]<<16)+(p[2]<<8)+p[3];
       h_inc;
   }
   v=0;
   for (;p<end; p++) {
       v<<=8;
       v+=*p;
   }
   h_inc;

   h=((h)+(h>>11))+((h>>13)+(h>>23));
   return (h);
#undef h_inc
}


int exists_charging_id_safe(uint32_t id, int hash_index){

	gen_charg_info_list_slot_t * info = NULL;
	WL_FOREACH(&gen_charg_info[hash_index], info){
		LOG(L_DBG, "info chg id is %x\n", info->charging_id);
		if(info->charging_id == id)
			return 1;
	}

	return 0;
}

#define MAX_UINT_32_T	pow(2,32)-1
#define MAX_GEN_TRIALS	10
uint32_t gen_new_charg_id_info(str user_id, str instance_id, int hash_index){

	uint32_t id = 0;
	int trials =0;
	gen_charg_info_list_slot_t * info = NULL;

	id = compute_charg_id(user_id, time(0));
	if(!id) id =1;
	//LOG(L_DBG, "id is %x\n", id);

	lock_get(gen_charg_info[hash_index].lock);
search:
		if(exists_charging_id_safe(id, hash_index)){
			if(trials<MAX_GEN_TRIALS && id< MAX_UINT_32_T ){
				id++;
				trials++;
				//LOG(L_DBG, "increased id is %x\n", id);
				goto search;
			}else {
				LOG(L_ERR, "no more trials, trials is %i, id is %x\n",
					trials, id);
				goto error;
			}
		}
		if(id!=0){
			info = create_new_gen_charg_id_info(hash_index, user_id, instance_id, id);
			if(!info)
				goto error;
			WL_APPEND(&gen_charg_info[hash_index], info);
			/*LOG(L_DBG, "hash_index is %i, head of the list is %p, info is %p\n", 
				hash_index, gen_charg_info[hash_index].head, info);*/
		}
	lock_release(gen_charg_info[hash_index].lock);
	return id;
error:
	lock_release(gen_charg_info[hash_index].lock);
	return 0;
}

uint32_t gen_new_charg_id(str user_id, str instance_id){

	int hash_index;
	uint32_t charging_id =0;

	hash_index = gen_charg_info_calc_hash(user_id);
	/*LOG(L_DBG, "hash_index is %i\n", hash_index);*/
	charging_id = gen_new_charg_id_info(user_id, instance_id, hash_index);
	if(!charging_id)
		goto error;

	return charging_id;
error:
	LOG(L_ERR, "could not generate the new charging id\n");
	return 0;
}

void release_charg_id(str user_id, str instance_id){

	int hash_index;
	gen_charg_info_list_slot_t * info = NULL;

	hash_index = gen_charg_info_calc_hash(user_id);
	/*LOG(L_DBG, "hash_index is %i\n", hash_index);*/

	lock_get(gen_charg_info[hash_index].lock);
		WL_FOREACH(&gen_charg_info[hash_index], info){
			if(str_equal(info->user_id, user_id) && str_equal(info->instance_id, instance_id)){
				break;
			}
		}
		
		if(info){
			WL_DELETE(&gen_charg_info[hash_index], info);
			WL_FREE(info, gen_charg_info_list_t, shm);
		}
		
	lock_release(gen_charg_info[hash_index].lock);
}

#endif
