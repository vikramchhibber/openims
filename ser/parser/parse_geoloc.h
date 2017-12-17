/*
 * Copyright (C) 2008-2009 FhG Fokus
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
 *  * \file geoloc.h
 * 
 * used by the Emergency-CSCF -Geolocation header parsing
 * (conforming to the draft-ietf-sip-location-conveyance)
 *
 * author Ancuta Onofrei, 
 * 	email andreea dot ancuta dot onofrei -at- fokus dot fraunhofer dot de
 */


#ifndef PARSE_GEOLOC_H
#define PARSE_GEOLOC_H

#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <unistd.h>
#include "../mem/mem.h"
#include "../dprint.h"
#include "../ut.h"
#include "../str.h"
#include "hf.h"
#include "msg_parser.h"

struct loc_value{
	struct sip_uri locURI;
	//used to send error responses
	str inserted_by; //mandatory
	//used by the receiving PSAP to know according to which location, the call was routed
	int used_for_routing; //0 for no and 1 for yes, 
	struct loc_value * next;
};

struct geoloc_body{
	//implemented as a stack, the first is the last in the body content
	struct loc_value * loc_list;
	int retrans_par; //0 for no and 1 for yes, default no/0
};

int parse_geoloc(struct sip_msg * msg);
void free_geoloc(struct geoloc_body **);
void print_geoloc(struct geoloc_body *);

#endif
