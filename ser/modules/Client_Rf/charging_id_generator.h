/* $Id: charging_id_generator.h 1167 2011-10-25 09:17:05Z aon $
 *
 * Copyright (C) 2009 FhG Fokus
 *
 * This file is part of the Wharf project.
 *
 */

#ifndef CHARGING_ID_GENERATOR_H_
#define CHARGING_ID_GENERATOR_H_

typedef struct _gen_charg_info_list_t_slot{
	int hash_index;
	str user_id;
	str instance_id;
	uint32_t charging_id;
	struct _gen_charg_info_list_t_slot * next, * prev;
} gen_charg_info_list_slot_t;

typedef struct _gen_charg_info_list_t{
	gen_lock_t * lock;
	struct _gen_charg_info_list_t_slot * head, * tail;
} gen_charg_info_list_t;

#define gen_charg_info_list_t_free(x,mem) \
do{\
	if (x) {\
		str_free((x)->user_id,mem);\
		str_free((x)->instance_id,mem);\
		mem##_free(x);\
		(x) = 0;\
	}\
}while(0)

int init_gen_charg_id_info();
void destroy_gen_charg_id_info();
uint32_t gen_new_charg_id(str user_id, str instance_id);
void release_charg_id(str user_id, str instance_id);

#endif /* CHARGING_ID_GENERATOR_H_ */
