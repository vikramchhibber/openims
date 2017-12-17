/**
 * $Id: macros.h 926 2010-03-08 10:55:30Z vingarzan $
 *   
 * Copyright (C) 2009-2010 FhG Fokus
 *
 * This file is part of Open EPC - an implementation of EPS core components
 * 
 * The Open EPC project  is a prototype implementation of the 3GPP 
 * Release 8 and later Evolved Packet Core (EPC) that will allow academic and 
 * industrial researchers and engineers around the world to obtain a 
 * practical look and feel of the capabilities of the Evolved Packet Core.
 * For more information on the Open EPC project, please visit www.openepc.net
 * or please contact Fraunhofer FOKUS by e-mail at the following addresses:
 *     info@openepc.net
 *
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
 * These are the macros that do the magic in defining avps like templates. 
 * 
 * 
 * 
 *  \author Dragos Vingarzan dragos dot vingarzan -at- fokus dot fraunhofer dot de
 * 
 */

#ifndef CDP_AVP_MODULE
	#define CDP_AVP_MODULE cdp_avp
#endif

#undef CDP_AVP_NAME
#define CDP_AVP_NAME(avp_name) AVP_##avp_name


#if defined(CDP_AVP_UNDEF_MACROS)

	#undef cdp_avp_add
	#undef cdp_avp_add_ptr
	#undef cdp_avp_add2
	#undef cdp_avp_add3

	#undef cdp_avp_get
	#undef cdp_avp_get2
	#undef cdp_avp_get3

	#undef cdp_avp
	#undef cdp_avp_ptr
	#undef cdp_avp2
	#undef cdp_avp3

#elif defined(CDP_AVP_DEFINITION)

	/*
	 * This is the definition of the function, so this needs to generate the .c file
	 */
	
	#define cdp_avp_add(avp_name,vendor_id,flags,avp_type,data_type)\
	int cdp_avp_add_##avp_name(AAA_AVP_LIST *list,data_type data) \
	{															\
		return cdp_avp_add_to_list(list,						\
					cdp_avp_new_##avp_type(						\
							CDP_AVP_NAME(avp_name),				\
							flags,								\
							vendor_id,							\
							data));								\
	}

	#define cdp_avp_add_ptr(avp_name,vendor_id,flags,avp_type,data_type)				\
	int cdp_avp_add_##avp_name(AAA_AVP_LIST *list,data_type data,AVPDataStatus data_do) \
	{															\
		return cdp_avp_add_to_list(list,						\
					cdp_avp_new_##avp_type(						\
							CDP_AVP_NAME(avp_name),				\
							flags,								\
							vendor_id,							\
							data,								\
							data_do));							\
	}

	#define cdp_avp_add2(avp_name,vendor_id,flags,avp_name_1,data_type_1,avp_name_2,data_type_2)	\
	int cdp_avp_add_##avp_name##_Group(AAA_AVP_LIST *list,data_type_1 avp_name_1,data_type_2 avp_name_2) \
	{																					\
		AAA_AVP_LIST list_grp={0,0};													\
		if (!cdp_avp_add_##avp_name_1(&list_grp,avp_name_1)||							\
			!cdp_avp_add_##avp_name_2(&list_grp,avp_name_2)) {							\
				cdp->AAAFreeAVPList(&list_grp);											\
				return 0;																\
		}																				\
		return cdp_avp_add_to_list(list,												\
				cdp_avp_new_Grouped(													\
						CDP_AVP_NAME(avp_name),											\
						flags,															\
						vendor_id,														\
						&list_grp,														\
						AVP_FREE_DATA));												\
	}

	#define cdp_avp_add3(avp_name,vendor_id,flags,avp_name_1,data_type_1,avp_name_2,data_type_2,avp_name_3,data_type_3)	\
	int cdp_avp_add_##avp_name##_Group(AAA_AVP_LIST *list,data_type_1 avp_name_1,data_type_2 avp_name_2,data_type_3 avp_name_3) \
	{																					\
		AAA_AVP_LIST list_grp={0,0};													\
		if (!cdp_avp_add_##avp_name_1(&list_grp,avp_name_1)||							\
			!cdp_avp_add_##avp_name_2(&list_grp,avp_name_2)||							\
			!cdp_avp_add_##avp_name_3(&list_grp,avp_name_3)) {							\
				cdp->AAAFreeAVPList(&list_grp);											\
				return 0;																\
		}																				\
		return cdp_avp_add_to_list(list,												\
				cdp_avp_new_Grouped(													\
						CDP_AVP_NAME(avp_name),											\
						flags,															\
						vendor_id,														\
						&list_grp,														\
						AVP_FREE_DATA));												\
	}






	#define cdp_avp_get(avp_name,vendor_id,flags,avp_type,data_type)		\
	int cdp_avp_get_##avp_name(AAA_AVP_LIST list,data_type *data,AAA_AVP **avp_ptr) \
	{															\
		AAA_AVP *avp = cdp_avp_get_next_from_list(list,			\
				CDP_AVP_NAME(avp_name),							\
				vendor_id,										\
				avp_ptr?*avp_ptr:0);							\
		if (avp_ptr) *avp_ptr = avp;							\
		if (!avp) {												\
			if (data) bzero(data,sizeof(data_type));			\
			return 0;											\
		}														\
		return cdp_avp_get_##avp_type(avp,data);				\
	}

	
	#define cdp_avp_get2(avp_name,vendor_id,flags,avp_name_1,data_type_1,avp_name_2,data_type_2)	\
	int cdp_avp_get_##avp_name##_Group(AAA_AVP_LIST list,data_type_1 *avp_name_1,data_type_2 *avp_name_2,AAA_AVP **avp_ptr) \
	{															\
		int cnt=0;												\
		AAA_AVP_LIST list_grp={0,0};							\
		AAA_AVP *avp = cdp_avp_get_next_from_list(list,			\
				CDP_AVP_NAME(avp_name),							\
				vendor_id,										\
				avp_ptr?*avp_ptr:0);							\
		if (avp_ptr) *avp_ptr = avp;							\
		if (!avp||!cdp_avp_get_Grouped(avp,&list_grp)) {		\
			if (avp_name_1) bzero(avp_name_1,sizeof(data_type_1));		\
			if (avp_name_2) bzero(avp_name_2,sizeof(data_type_2));		\
			return 0;											\
		}														\
		cnt += cdp_avp_get_##avp_name_1(list_grp,avp_name_1,0);		\
		cnt += cdp_avp_get_##avp_name_2(list_grp,avp_name_2,0);		\
		cdp->AAAFreeAVPList(&list_grp);							\
		return cnt;												\
	}

	
	#define cdp_avp_get3(avp_name,vendor_id,flags,avp_name_1,data_type_1,avp_name_2,data_type_2,avp_name_3,data_type_3)	\
	int cdp_avp_get_##avp_name##_Group(AAA_AVP_LIST list,data_type_1 *avp_name_1,data_type_2 *avp_name_2,data_type_3 *avp_name_3,AAA_AVP **avp_ptr) \
	{															\
		int cnt=0;												\
		AAA_AVP_LIST list_grp={0,0};							\
		AAA_AVP *avp = cdp_avp_get_next_from_list(list,			\
				CDP_AVP_NAME(avp_name),							\
				vendor_id,										\
				avp_ptr?*avp_ptr:0);							\
		if (avp_ptr) *avp_ptr = avp;							\
		if (!avp||!cdp_avp_get_Grouped(avp,&list_grp)) {		\
			if (avp_name_1) bzero(avp_name_1,sizeof(data_type_1));		\
			if (avp_name_2) bzero(avp_name_2,sizeof(data_type_2));		\
			if (avp_name_3) bzero(avp_name_3,sizeof(data_type_3));		\
			return 0;											\
		}														\
		cnt += cdp_avp_get_##avp_name_1(list_grp,avp_name_1,0);		\
		cnt += cdp_avp_get_##avp_name_2(list_grp,avp_name_2,0);		\
		cnt += cdp_avp_get_##avp_name_3(list_grp,avp_name_3,0);		\
		cdp->AAAFreeAVPList(&list_grp);							\
		return cnt;												\
	}


	#define cdp_avp(avp_name,vendor_id,flags,avp_type,data_type)	\
		cdp_avp_add(avp_name,vendor_id,flags,avp_type,data_type)	\
		cdp_avp_get(avp_name,vendor_id,flags,avp_type,data_type)

	#define cdp_avp_ptr(avp_name,vendor_id,flags,avp_type,data_type)	\
		cdp_avp_add_ptr(avp_name,vendor_id,flags,avp_type,data_type)	\
		cdp_avp_get(avp_name,vendor_id,flags,avp_type,data_type)

	#define cdp_avp2(avp_name,vendor_id,flags,avp_name_1,data_type_1,avp_name_2,data_type_2) \
		cdp_avp_add2(avp_name,vendor_id,flags,avp_name_1,data_type_1,avp_name_2,data_type_2) \
		cdp_avp_get2(avp_name,vendor_id,flags,avp_name_1,data_type_1,avp_name_2,data_type_2)
		
	#define cdp_avp3(avp_name,vendor_id,flags,avp_name_1,data_type_1,avp_name_2,data_type_2,avp_name_3,data_type_3) \
		cdp_avp_add3(avp_name,vendor_id,flags,avp_name_1,data_type_1,avp_name_2,data_type_2,avp_name_3,data_type_3) \
		cdp_avp_get3(avp_name,vendor_id,flags,avp_name_1,data_type_1,avp_name_2,data_type_2,avp_name_3,data_type_3)

#elif defined(CDP_AVP_EXPORT)

	/*
	 * This is the export of the functions in the module, so this needs to generate the mod_export.h lists
	 */
	
	
	#define cdp_avp_add(avp_name,vendor_id,flags,avp_type,data_type)		\
		cdp_avp_add_##avp_name##_f		add_##avp_name;
	
	#define cdp_avp_add_ptr(avp_name,vendor_id,flags,avp_type,data_type)	\
		cdp_avp_add_##avp_name##_f		add_##avp_name;

	#define cdp_avp_add2(avp_name,vendor_id,flags,avp_name_1,data_type_1,avp_name_2,data_type_2)	\
		cdp_avp_add_##avp_name##_Group_f	add_##avp_name##_Group;  	
	
	#define cdp_avp_add3(avp_name,vendor_id,flags,avp_name_1,data_type_1,avp_name_2,data_type_2,avp_name_3,data_type_3)	\
		cdp_avp_add_##avp_name##_Group_f	add_##avp_name##_Group;  	



	#define cdp_avp_get(avp_name,vendor_id,flags,avp_type,data_type)		\
		cdp_avp_get_##avp_name##_f		get_##avp_name;
		
	#define cdp_avp_get2(avp_name,vendor_id,flags,avp_name_1,data_type_1,avp_name_2,data_type_2)	\
		cdp_avp_get_##avp_name##_Group_f	get_##avp_name##_Group;  	

	#define cdp_avp_get3(avp_name,vendor_id,flags,avp_name_1,data_type_1,avp_name_2,data_type_2,avp_name_3,data_type_3)	\
		cdp_avp_get_##avp_name##_Group_f	get_##avp_name##_Group;  	




	#define cdp_avp(avp_name,vendor_id,flags,avp_type,data_type)	\
		cdp_avp_add(avp_name,vendor_id,flags,avp_type,data_type)	\
		cdp_avp_get(avp_name,vendor_id,flags,avp_type,data_type)
	
	#define cdp_avp_ptr(avp_name,vendor_id,flags,avp_type,data_type)	\
		cdp_avp_add_ptr(avp_name,vendor_id,flags,avp_type,data_type)	\
		cdp_avp_get(avp_name,vendor_id,flags,avp_type,data_type)
	
	#define cdp_avp2(avp_name,vendor_id,flags,avp_name_1,data_type_1,avp_name_2,data_type_2) \
		cdp_avp_add2(avp_name,vendor_id,flags,avp_name_1,data_type_1,avp_name_2,data_type_2) \
		cdp_avp_get2(avp_name,vendor_id,flags,avp_name_1,data_type_1,avp_name_2,data_type_2)
		
	#define cdp_avp3(avp_name,vendor_id,flags,avp_name_1,data_type_1,avp_name_2,data_type_2,avp_name_3,data_type_3) \
		cdp_avp_add3(avp_name,vendor_id,flags,avp_name_1,data_type_1,avp_name_2,data_type_2,avp_name_3,data_type_3) \
		cdp_avp_get3(avp_name,vendor_id,flags,avp_name_1,data_type_1,avp_name_2,data_type_2,avp_name_3,data_type_3)

#elif defined(CDP_AVP_INIT)

	/*
	 * This is the initialization of the exported binding in the module, so this needs to generate the list of functions
	 */

	#define cdp_avp_add(avp_name,vendor_id,flags,avp_type,data_type)		\
		cdp_avp_add_##avp_name,

	#define cdp_avp_add_ptr(avp_name,vendor_id,flags,avp_type,data_type)		\
		cdp_avp_add_##avp_name,

	#define cdp_avp_add2(avp_name,vendor_id,flags,avp_name_1,data_type_1,avp_name_2,data_type_2)	\
		cdp_avp_add_##avp_name##_Group,  	
	
	#define cdp_avp_add3(avp_name,vendor_id,flags,avp_name_1,data_type_1,avp_name_2,data_type_2,avp_name_3,data_type_3)	\
		cdp_avp_add_##avp_name##_Group,  	



	#define cdp_avp_get(avp_name,vendor_id,flags,avp_type,data_type)		\
			cdp_avp_get_##avp_name,

	#define cdp_avp_get2(avp_name,vendor_id,flags,avp_name_1,data_type_1,avp_name_2,data_type_2)	\
		cdp_avp_get_##avp_name##_Group,  	
	
	#define cdp_avp_get3(avp_name,vendor_id,flags,avp_name_1,data_type_1,avp_name_2,data_type_2,avp_name_3,data_type_3)	\
		cdp_avp_get_##avp_name##_Group,  	




	#define cdp_avp(avp_name,vendor_id,flags,avp_type,data_type)	\
		cdp_avp_add(avp_name,vendor_id,flags,avp_type,data_type)	\
		cdp_avp_get(avp_name,vendor_id,flags,avp_type,data_type)
	
	#define cdp_avp_ptr(avp_name,vendor_id,flags,avp_type,data_type)	\
		cdp_avp_add_ptr(avp_name,vendor_id,flags,avp_type,data_type)	\
		cdp_avp_get(avp_name,vendor_id,flags,avp_type,data_type)
	
	#define cdp_avp2(avp_name,vendor_id,flags,avp_name_1,data_type_1,avp_name_2,data_type_2) \
		cdp_avp_add2(avp_name,vendor_id,flags,avp_name_1,data_type_1,avp_name_2,data_type_2) \
		cdp_avp_get2(avp_name,vendor_id,flags,avp_name_1,data_type_1,avp_name_2,data_type_2) 
		
	#define cdp_avp3(avp_name,vendor_id,flags,avp_name_1,data_type_1,avp_name_2,data_type_2,avp_name_3,data_type_3) \
		cdp_avp_add3(avp_name,vendor_id,flags,avp_name_1,data_type_1,avp_name_2,data_type_2,avp_name_3,data_type_3) \
		cdp_avp_get3(avp_name,vendor_id,flags,avp_name_1,data_type_1,avp_name_2,data_type_2,avp_name_3,data_type_3) 


#elif defined(CDP_AVP_REFERENCE)

	/*
	 * This is for generating a function reference for documentation
	 */
	#define cdp_avp_add(avp_name,vendor_id,flags,avp_type,data_type)		\
	int CDP_AVP_MODULE.add_##avp_name(AAA_AVP_LIST *list,data_type data); 
	
	#define cdp_avp_add_ptr(avp_name,vendor_id,flags,avp_type,data_type)		\
	int CDP_AVP_MODULE.add_##avp_name(AAA_AVP_LIST *list,data_type data,AVPDataStatus data_do); 
	
	#define cdp_avp_add2(avp_name,vendor_id,flags,avp_name_1,data_type_1,avp_name_2,data_type_2)	\
	int CDP_AVP_MODULE.add_##avp_name##_Group(AAA_AVP_LIST *list,data_type_1 avp_name_1,data_type_2 avp_name_2); 
	
	#define cdp_avp_add3(avp_name,vendor_id,flags,avp_name_1,data_type_1,avp_name_2,data_type_2,avp_name_3,data_type_3)	\
	int CDP_AVP_MODULE.add_##avp_name##_Group(AAA_AVP_LIST *list,data_type_1 avp_name_1,data_type_2 avp_name_2,data_type_3 avp_name_3);
		
		
		
	#define cdp_avp_get(avp_name,vendor_id,flags,avp_type,data_type)		\
	int CDP_AVP_MODULE.get_##avp_name(AAA_AVP_LIST list,data_type *data,AAA_AVP **avp_ptr); 

	#define cdp_avp_get2(avp_name,vendor_id,flags,avp_name_1,data_type_1,avp_name_2,data_type_2)	\
	int CDP_AVP_MODULE.get_##avp_name##_Group(AAA_AVP_LIST list,data_type_1 *avp_name_1,data_type_2 *avp_name_2,AAA_AVP **avp_ptr); 

	#define cdp_avp_get3(avp_name,vendor_id,flags,avp_name_1,data_type_1,avp_name_2,data_type_2,avp_name_3,data_type_3)	\
	int CDP_AVP_MODULE.get_##avp_name##_Group(AAA_AVP_LIST list,data_type_1 *avp_name_1,data_type_2 *avp_name_2,data_type_3 *avp_name_3,AAA_AVP **avp_ptr);





	#define cdp_avp(avp_name,vendor_id,flags,avp_type,data_type)	\
		cdp_avp_add(avp_name,vendor_id,flags,avp_type,data_type)	\
		|cdp_avp_get(avp_name,vendor_id,flags,avp_type,data_type)	
		
	
	#define cdp_avp_ptr(avp_name,vendor_id,flags,avp_type,data_type) \
		cdp_avp_add_ptr(avp_name,vendor_id,flags,avp_type,data_type) \
		|cdp_avp_get(avp_name,vendor_id,flags,avp_type,data_type)		
	
	#define cdp_avp2(avp_name,vendor_id,flags,avp_name_1,data_type_1,avp_name_2,data_type_2) \
		cdp_avp_add2(avp_name,vendor_id,flags,avp_name_1,data_type_1,avp_name_2,data_type_2) \
		|cdp_avp_get2(avp_name,vendor_id,flags,avp_name_1,data_type_1,avp_name_2,data_type_2)
		
	#define cdp_avp3(avp_name,vendor_id,flags,avp_name_1,data_type_1,avp_name_2,data_type_2,avp_name_3,data_type_3) \
		cdp_avp_add3(avp_name,vendor_id,flags,avp_name_1,data_type_1,avp_name_2,data_type_2,avp_name_3,data_type_3) \
		|cdp_avp_get3(avp_name,vendor_id,flags,avp_name_1,data_type_1,avp_name_2,data_type_2,avp_name_3,data_type_3)
		

#elif defined(CDP_AVP_EMPTY_MACROS)

	/*
	 * This is for ensuring that the templates won't actually output anything 
	 */

	#define cdp_avp_add(avp_name,vendor_id,flags,avp_type,data_type)	
	#define cdp_avp_add_ptr(avp_name,vendor_id,flags,avp_type,data_type)	
	#define cdp_avp_add2(avp_name,vendor_id,flags,avp_name_1,data_type_1,avp_name_2,data_type_2)
	#define cdp_avp_add3(avp_name,vendor_id,flags,avp_name_1,data_type_1,avp_name_2,data_type_2,avp_name_3,data_type_3)
	


	#define cdp_avp_get(avp_name,vendor_id,flags,avp_type,data_type)
	#define cdp_avp_get2(avp_name,vendor_id,flags,avp_name_1,data_type_1,avp_name_2,data_type_2)
	#define cdp_avp_get3(avp_name,vendor_id,flags,avp_name_1,data_type_1,avp_name_2,data_type_2,avp_name_3,data_type_3)

	#define cdp_avp(avp_name,vendor_id,flags,avp_type,data_type)	
	#define cdp_avp_ptr(avp_name,vendor_id,flags,avp_type,data_type)	
	#define cdp_avp2(avp_name,vendor_id,flags,avp_name_1,data_type_1,avp_name_2,data_type_2)		
	#define cdp_avp3(avp_name,vendor_id,flags,avp_name_1,data_type_1,avp_name_2,data_type_2,avp_name_3,data_type_3)
		
#else

	/*
	 * This is the declaration of the functions, so this needs to generate the normal .h header file 
	 */

	#define cdp_avp_add(avp_name,vendor_id,flags,avp_type,data_type)		\
	int cdp_avp_add_##avp_name(AAA_AVP_LIST *list,data_type data);			\
	typedef int (*cdp_avp_add_##avp_name##_f)(AAA_AVP_LIST *list,data_type data); 
	
	#define cdp_avp_add_ptr(avp_name,vendor_id,flags,avp_type,data_type)		\
	int cdp_avp_add_##avp_name(AAA_AVP_LIST *list,data_type data,AVPDataStatus data_do); \
	typedef int (*cdp_avp_add_##avp_name##_f)(AAA_AVP_LIST *list,data_type data,AVPDataStatus data_do); 
	
	#define cdp_avp_add2(avp_name,vendor_id,flags,avp_name_1,data_type_1,avp_name_2,data_type_2)	\
	int cdp_avp_add_##avp_name##_Group(AAA_AVP_LIST *list,data_type_1 avp_name_1,data_type_2 avp_name_2);  \
	typedef int (*cdp_avp_add_##avp_name##_Group_f)(AAA_AVP_LIST *list,data_type_1 avp_name_1,data_type_2 avp_name_2);
	
	#define cdp_avp_add3(avp_name,vendor_id,flags,avp_name_1,data_type_1,avp_name_2,data_type_2,avp_name_3,data_type_3)	\
	int cdp_avp_add_##avp_name##_Group(AAA_AVP_LIST *list,data_type_1 avp_name_1,data_type_2 avp_name_2,data_type_3 avp_name_3); \
	typedef int (*cdp_avp_add_##avp_name##_Group_f)(AAA_AVP_LIST *list,data_type_1 avp_name_1,data_type_2 avp_name_2,data_type_3 avp_name_3);



	#define cdp_avp_get(avp_name,vendor_id,flags,avp_type,data_type)		\
		int cdp_avp_get_##avp_name(AAA_AVP_LIST list,data_type *data,AAA_AVP **avp_ptr); \
		typedef	int (*cdp_avp_get_##avp_name##_f)(AAA_AVP_LIST list,data_type *data,AAA_AVP **avp_ptr); 

	#define cdp_avp_get2(avp_name,vendor_id,flags,avp_name_1,data_type_1,avp_name_2,data_type_2)	\
		int cdp_avp_get_##avp_name##_Group(AAA_AVP_LIST list,data_type_1 *avp_name_1,data_type_2 *avp_name_2,AAA_AVP **avp_ptr); \
		typedef int (*cdp_avp_get_##avp_name##_Group_f)(AAA_AVP_LIST list,data_type_1 *avp_name_1,data_type_2 *avp_name_2,AAA_AVP **avp_ptr);

	#define cdp_avp_get3(avp_name,vendor_id,flags,avp_name_1,data_type_1,avp_name_2,data_type_2,avp_name_3,data_type_3)	\
		int cdp_avp_get_##avp_name##_Group(AAA_AVP_LIST list,data_type_1 *avp_name_1,data_type_2 *avp_name_2,data_type_3 *avp_name_3,AAA_AVP **avp_ptr); \
		typedef int (*cdp_avp_get_##avp_name##_Group_f)(AAA_AVP_LIST list,data_type_1 *avp_name_1,data_type_2 *avp_name_2,data_type_3 *avp_name_3,AAA_AVP **avp_ptr);




	#define cdp_avp(avp_name,vendor_id,flags,avp_type,data_type)	\
		cdp_avp_add(avp_name,vendor_id,flags,avp_type,data_type)	\
		cdp_avp_get(avp_name,vendor_id,flags,avp_type,data_type)
	
	#define cdp_avp_ptr(avp_name,vendor_id,flags,avp_type,data_type)	\
		cdp_avp_add_ptr(avp_name,vendor_id,flags,avp_type,data_type)	\
		cdp_avp_get(avp_name,vendor_id,flags,avp_type,data_type)
	
	#define cdp_avp2(avp_name,vendor_id,flags,avp_name_1,data_type_1,avp_name_2,data_type_2) \
		cdp_avp_add2(avp_name,vendor_id,flags,avp_name_1,data_type_1,avp_name_2,data_type_2) \
		cdp_avp_get2(avp_name,vendor_id,flags,avp_name_1,data_type_1,avp_name_2,data_type_2)
		
	#define cdp_avp3(avp_name,vendor_id,flags,avp_name_1,data_type_1,avp_name_2,data_type_2,avp_name_3,data_type_3) \
		cdp_avp_add3(avp_name,vendor_id,flags,avp_name_1,data_type_1,avp_name_2,data_type_2,avp_name_3,data_type_3) \
		cdp_avp_get3(avp_name,vendor_id,flags,avp_name_1,data_type_1,avp_name_2,data_type_2,avp_name_3,data_type_3)
#endif


