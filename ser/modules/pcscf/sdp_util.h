/*
 * $Id: sdp_util.h 920 2010-03-05 19:07:50Z vingarzan $
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
 * P-CSCF - SDP Util functions
 * 
 * 
 */

#include "../../dset.h"
#include "../../dprint.h"
#include "../../str.h"
#include "../../parser/msg_parser.h"
#include "../../ut.h"


#ifndef AF_LOCAL
#define AF_LOCAL AF_UNIX
#endif

/* #define RPORT 8000  */
/* Supported version of the RTP proxy command protocol */
#define	SUP_CPROTOVER	20040107
/* Required additional version of the RTP proxy command protocol */
#define	REQ_CPROTOVER	"20050322"
#define	CPORT		"22222"
#define STANDARD_SIPPORT 5060

#define ADIRECTION  "A=direction:active\r\n"
#define ADIRECTION_LEN ( sizeof(ADIRECTION) -1 )
#define ANORTPPROXY  "a=nortpproxy:yes\r\n"
#define ADD_ADIRECTION 0x01
#define ADD_ANORTPPROXY 0x04
#define FIX_MEDIP 0x02
#define ANORTPPROXY_LEN (sizeof(ANORTPPROXY) -1 ) 

#define AOLDMEDIP "a=oldmediaip:"
#define AOLDMEDIP_LEN (sizeof(AOLDMEDIP) - 1 ) 

#define AOLDMEDIP6 "a=oldmediaip6:"
#define AOLDMEDIP6_LEN (sizeof(AOLDMEDIP6) - 1 ) 

#define	AOLDMEDPRT	"a=oldmediaport:"
#define	AOLDMEDPRT_LEN	(sizeof(AOLDMEDPRT) - 1)

#define READ(val) \
	(*(val + 0) + (*(val + 1) << 8) + (*(val + 2) << 16) + (*(val + 3) << 24))

#define advance(_ptr,_n,_str,_error) \
	do{\
		if ((_ptr)+(_n)>(_str).s+(_str).len)\
			goto _error;\
		(_ptr) = (_ptr) + (_n);\
	}while(0);
#define one_of_16( _x , _t ) \
	(_x==_t[0]||_x==_t[15]||_x==_t[8]||_x==_t[2]||_x==_t[3]||_x==_t[4]\
	||_x==_t[5]||_x==_t[6]||_x==_t[7]||_x==_t[1]||_x==_t[9]||_x==_t[10]\
	||_x==_t[11]||_x==_t[12]||_x==_t[13]||_x==_t[14])
#define one_of_8( _x , _t ) \
	(_x==_t[0]||_x==_t[7]||_x==_t[1]||_x==_t[2]||_x==_t[3]||_x==_t[4]\
	||_x==_t[5]||_x==_t[6])


struct rtpp_node {
	char			*rn_url;	/* unparsed, deletable */
	int			rn_umode;
	char			*rn_address;	/* substring of rn_url */
	int			rn_fd;		/* control fd */
	int			rn_disabled;	/* found unaccessible? */
	unsigned		rn_weight;	/* for load balancing */
	int			rn_recheck_ticks;
	struct rtpp_node	*rn_next;
};

struct rtpp_head {
	struct rtpp_node	*rn_first;
	struct rtpp_node	*rn_last;
};


#define	STR2IOVEC(sx, ix)	do {(ix).iov_base = (sx).s; (ix).iov_len = (sx).len;} while(0)
#define SZ2IOVEC(sx, ix)	do {char *_t_p = (ix).iov_base = (sx); (ix).iov_len = strlen(_t_p);} while(0)

int rtpp_test(struct rtpp_node *node, int isdisabled, int force);

int rtpproxy_init();
int rtpproxy_child_init(int rank);

int P_SDP_manipulate(struct sip_msg *msg,char *str1,char *str2);
char * find_next_sdp_line(char* p, char* plimit, char linechar, char* defptr);
char * find_sdp_line(char* p, char* plimit, char linechar);
