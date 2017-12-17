/** 
 * $Id: sem.h 951 2010-06-01 11:03:08Z vingarzan $
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
 * \file POSIX Semaphores
 * 
 */

#ifndef __WHARF_SEM_H
#define __WHARF_SEM_H

#ifdef SHM_MEM

//	#include "../../mem/mem.h"
//	#include "../../mem/shm_mem.h"
	
	#include "utils.h"
	
	#include <semaphore.h>

	typedef sem_t gen_sem_t;

	/**
	 * Create a new unnamed semaphore and initialize it
	 * @param value - 0 if it should be pre-locked, 1 if not, or how many locks until block
	 * @return
	 */
    #define sem_new(sem_ptr,value)\
	do {\
		sem_ptr=shm_malloc(sizeof(gen_sem_t));\
		if (!sem_ptr){\
			LOG(L_ERR,"Error allocating %d bytes of shm!\n",sizeof(gen_sem_t));\
			goto out_of_memory;\
		}	\
		if (sem_init(sem_ptr, 1, value)<0) {\
			LOG(L_ERR,"Error > %s\n",strerror(errno));\
			goto out_of_memory;\
		}\
	} while(0)
	
    #define sem_free(sem)\
	do {\
		if (sem) {\
			sem_destroy(sem);\
			shm_free(sem);\
			sem=0;\
		}\
	} while(0)
	
	
	#define sem_get(sem) sem_wait(sem)
	#define sem_tryget(sem) sem_trywait(sem)
	#define sem_timedget(sem,abs_timeout) sem_trywait(sem,abs_timeout)
	
	#define sem_release(sem) sem_post(sem)

#else
	//#error "locking requires shared memory support"
#endif

#endif
