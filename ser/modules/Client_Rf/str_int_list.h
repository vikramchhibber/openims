/*
 * $Id: str_int_list.h 1167 2011-10-25 09:17:05Z aon $
 *
 * Copyright (C) 2009 FhG Fokus
 *
 * This file is part of the Wharf project.
 *
 */

#ifndef STR_INT_LIST_H_
#define STR_INT_LIST_H_

#include "../../ut.h"
/**
 * Allocate and blank a memory area
 */
#define mem_new(dst,len,mem) \
do {\
	if (!(len)) (dst)=0;\
	else {\
		(dst) = mem##_malloc(len);\
		if (!(dst)) {\
			LOG(L_ERR,"Error allocating %ld bytes in %s!\n",(long int)len,#mem);\
			goto out_of_memory;\
		}\
		bzero((dst),(len));\
	}\
} while(0)

#define struct_new(dst,type,mem) \
		men_new(dst,sizeof(type),mem)


/**
 * Duplicate an arbitrary memory area
 */
#define mem_dup(dst,src,len,mem) \
do {\
	if (!(src)||!(len)) (dst)=0;\
	else {\
		(dst) = mem##_malloc(len);\
		if (!(dst)) {\
			LOG(L_ERR,"Error allocating %ld bytes in %s!\n",(long int)len,#mem);\
			goto out_of_memory;\
		}\
		memcpy((dst),(src),len);\
	}\
} while(0)


/**
 * Free an arbitrary memory area
 */
#define mem_free(x,mem) \
do {\
	if (x) {\
		mem##_free(x);\
		x=0;\
	}\
} while(0)

/**
 * Duplicate a str, safely.
 * \Note This checks if:
 *  - src was an empty string
 *  - malloc failed
 * \Note On any error, the dst values are reset for safety
 * \Note A label "out_of_memory" must be defined in the calling function to handle
 * allocation errors.
 * @param dst - destination str
 * @param src - source src
 * @param mem - type of mem to duplicate into (shm/pkg)
 */
#define str_dup(dst,src,mem) \
do {\
	if ((src).len) {\
		(dst).s = mem##_malloc((src).len);\
		if (!(dst).s){\
			LOG(L_ERR,"Error allocating %d bytes in %s!\n",(src).len,#mem);\
			(dst).len = 0;\
			goto out_of_memory;\
		}\
		memcpy((dst).s,(src).s,(src).len);\
		(dst).len = (src).len;\
	}else{\
		(dst).s=0;(dst).len=0;\
	}\
} while (0)


#define str_dup_ptr(dst,src,mem) \
do {\
	dst = mem##_malloc(sizeof(str));\
	if ((src).len) {\
		(dst)->s = mem##_malloc((src).len);\
		if (!(dst)->s){\
			LOG(L_ERR,"Error allocating %d bytes in %s!\n",(src).len,#mem);\
			mem##_free(dst);\
			goto out_of_memory;\
		}\
		memcpy((dst)->s,(src).s,(src).len);\
		(dst)->len = (src).len;\
	}else{\
		(dst)->s=0;(dst)->len=0;\
	}\
} while (0)

#define str_dup_ptr_ptr(dst,src,mem) \
do {\
	dst = mem##_malloc(sizeof(str));\
	if ((src) && (src)->len) {\
		(dst)->s = mem##_malloc((src)->len);\
		if (!(dst)->s){\
			LOG(L_ERR,"Error allocating %d bytes in %s!\n",(src)->len,#mem);\
			mem##_free(dst);\
			goto out_of_memory;\
		}\
		memcpy((dst)->s,(src)->s,(src)->len);\
		(dst)->len = (src)->len;\
	}else{\
		(dst)->s=0;(dst)->len=0;\
	}\
} while (0)

/**
 * Duplicate a char* safely into a str
 * \Note This actually copies also the \0 at the end, unlike str_dup!
 * @param dst - destination str
 * @param src - source char*
 * @param mem - type of memory to use for malloc (pkg/shm)
 */
#define char_dup_str(dst,src,mem) \
do {\
        int char_dup_len = strlen((char*)src);\
        if (!char_dup_len) {(dst).s=0;(dst).len=0;}\
        else {\
                (dst).s = mem##_malloc(char_dup_len+1);\
                if (!(dst).s){\
                        LOG(L_ERR,"Error allocating %d bytes in %s!\n",(char_dup_len)+1,#mem);\
                        (dst).len=0;\
                        goto out_of_memory;\
                }\
                memcpy((dst).s,(src),char_dup_len);\
                (dst).s[char_dup_len]=0;\
                (dst).len=char_dup_len;\
        }\
} while (0)

/**
 * Frees a str content.
 * @param x - the str to free
 * @param mem - type of memory that the content is using (shm/pkg)
 */
#define str_free(x,mem) \
do {\
	if ((x).s) mem##_free((x).s);\
	(x).s=0;(x).len=0;\
} while(0)

#define str_free_ptr(x,mem) \
do {\
	if (x) {\
		if ((x)->s) mem##_free((x)->s);\
		mem##_free((x));\
	}\
} while(0)

/**
 * list operations
 */

#define WL_NEW(el,list_type,mem)                                                                                                \
do {                                                                                                                                                    \
        (el) = mem##_malloc(sizeof(struct _##list_type##_slot));                                        \
        if (!(el)) {                                                                                                                            \
                LOG(L_ERR,"Unable to allocate %ld bytes in %s!",(long int)sizeof(struct _##list_type##_slot),#mem);\
                goto out_of_memory;                                                                                                             \
        }                                                                                                                                                       \
        bzero((el),sizeof(struct _##list_type##_slot));                                                         \
} while(0)

#define WL_APPEND(list,add)                                                      \
do {                                                                             \
  (add)->next = NULL;								\
  (add)->prev = (list)->tail;													 \
  if ((list)->tail) ((list)->tail)->next=(add);									 \
  else (list)->head = (add);                                                     \
  (list)->tail=(add);                                                        	 \
} while (0)

#define WL_DELETE(list,del)                                                    	\
do {                                                                            \
	if ((del)->prev) ((del)->prev)->next=(del)->next;							\
	else ((list)->head)=(del)->next;											\
	if ((del)->next) ((del)->next)->prev=(del)->prev;							\
	else ((list)->tail)=(del)->prev;											\
	(del)->next=0;(del)->prev=0;												\
} while (0)

#define WL_FREE(el,list_type,mem)												\
	list_type##_free(el,mem)

#define WL_FREE_ALL(list,list_type,mem)\
do {\
	struct _##list_type##_slot *el,*nel;\
	for(el=(list)->head;el;el=nel){\
		nel = el->next;\
		WL_FREE(el,list_type,mem);\
	}\
	(list)->head=0;\
	(list)->tail=0;\
} while(0)

#define WL_FOREACH(list,el)                                                      \
    for(el=(list)->head;el;el=(el)->next)

#define WL_DUP(dst,src,list_type,mem) \
do {\
	WL_NEW(dst,list_type,mem);\
	list_type##_copy(dst,src,mem);\
}while(0)

#define WL_DUP_ALL(dst,src,list_type,mem)\
do {\
	struct _##list_type##_slot *el1,*el2;\
	for(el1=(src)->head;el1;el1=(el1)->next){\
		WL_DUP(el2,el1,list_type,mem);\
		if (el2) WL_APPEND(dst,el2);\
	}\
} while(0)


/* List of str */
#define str_equal(a,b) ((a).len==(b).len && memcmp((a).s,(b).s,(a).len)==0)

typedef struct _str_list_t_slot {
	str data;
	struct _str_list_t_slot *prev,*next;
} str_list_slot_t;

typedef struct {
	str_list_slot_t *head,*tail;
} str_list_t;

#define str_list_t_free(x,mem) \
do{\
	if (x) {\
		str_free((x)->data,mem);\
		mem##_free(x);\
		(x)=0;\
	}\
}while(0)


#define str_list_t_copy(dst,src,mem) \
	str_dup((dst)->data,(src)->data,mem)

/* List of int */

typedef struct _int_list_t_slot {
	int data;
	struct _int_list_t_slot *prev,*next;
} int_list_slot_t;

typedef struct {
	int_list_slot_t *head,*tail;
} int_list_t;

#define int_list_t_free(x,mem) \
do {\
	if (x){\
		mem##_free(x);\
		(x) = 0;\
	}\
}while(0)

#define int_list_t_copy(dst,src,mem) \
	dst->data = src->data

long int str_to_long_int(str x,int base);

#endif /* STR_INT_LIST_H_ */
