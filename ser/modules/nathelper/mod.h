/**
 * $Id: mod.h 920 2010-03-05 19:07:50Z vingarzan $
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
 

#ifndef NATH_MOD_H
#define NATH_MOD_H

#include "../../sr_module.h"

#define M_NAME "NATHELPER"


/** Return and break the execution of routing script */
#define CSCF_RETURN_BREAK	0 
/** Return true in the routing script */
#define CSCF_RETURN_TRUE	1
/** Return false in the routing script */
#define CSCF_RETURN_FALSE -1
/** Return error in the routing script */
#define CSCF_RETURN_ERROR -2

#define STR_SHM_DUP(dest,src,txt)\
{\
	if ((src).len==0) {\
		(dest).s=0;\
		(dest).len=0;\
	}else {\
		(dest).s = shm_malloc((src).len);\
		if (!(dest).s){\
			LOG(L_ERR,"ERR:"M_NAME":"txt": Error allocating %d bytes\n",(src).len);\
			(dest).len = 0;\
			goto out_of_memory;\
		}else{\
			(dest).len = (src).len;\
			memcpy((dest).s,(src).s,(src).len);\
		}\
	}\
}

#define STR_PKG_DUP(dest,src,txt)\
{\
	if ((src).len==0) {\
		(dest).s=0;\
		(dest).len=0;\
	}else {\
		(dest).s = pkg_malloc((src).len);\
		if (!(dest).s){\
			LOG(L_ERR,"ERRL:"M_NAME":"txt": Error allocating %d bytes\n",(src).len);\
			(dest).len = 0;\
			goto out_of_memory;\
		}else{\
			(dest).len = (src).len;\
			memcpy((dest).s,(src).s,(src).len);\
		}\
	}\
}

#define STR_APPEND(dst,src)\
	{memcpy((dst).s+(dst).len,(src).s,(src).len);\
	(dst).len = (dst).len + (src).len;}

/* ANSI Terminal colors */
#define ANSI_GRAY		"\033[01;30m"
#define ANSI_BLINK_RED 	"\033[00;31m"
#define ANSI_RED 		"\033[01;31m"
#define ANSI_GREEN		"\033[01;32m"
#define ANSI_YELLOW 	"\033[01;33m"
#define ANSI_BLUE 		"\033[01;34m"
#define ANSI_MAGENTA	"\033[01;35m"
#define ANSI_CYAN		"\033[01;36m"
#define ANSI_WHITE		"\033[01;37m"

#endif /* NATH_MOD_H */
