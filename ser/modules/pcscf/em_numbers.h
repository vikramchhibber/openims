/*
 * $Id: em_numbers.h 920 2010-03-05 19:07:50Z vingarzan $
 *  
 * Copyright (C) 2009 FhG Fokus
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
 * Proxy-CSCF -Emergency Numbers Related Operations
 * 
 * 
 *  \author Ancuta Onofrei	andreea dot ancuta dot onofrei -at- fokus dot fraunhofer dot de
 */

#ifndef P_CSCF_EM_NUMBERS_H
#define P_CSCF_EM_NUMBERS_H 

#include <libxml/parser.h>
#include "../../sr_module.h"
#include "mod.h"
#include <libxml/tree.h>
#include "../../mem/mem.h"

//emergency URNs from the RFC 5031 "A Uniform Resource Name (URN) for Emergency and Other Well-Known Services"
#define GEN_URN_STR		"urn:service:"
#define GEN_URN_STR_LEN		(sizeof(GEN_URN_STR)-1)

#define SOS_URN_STR   		"urn:service:sos"
#define SOS_URN_STR_LEN		(sizeof(SOS_URN_STR)-1)

#define SOS_URN_AMB		"ambulance"
#define SOS_URN_AMB_LEN		(sizeof(SOS_URN_AMB)-1)
#define SOS_URN_AMB_STR		"urn:service:sos.ambulance"
#define SOS_URN_AMB_STR_LEN	(sizeof(SOS_URN_AMB_STR)-1)

#define SOS_URN_AN_CTR		"animal-control"
#define SOS_URN_AN_CTR_LEN	(sizeof(SOS_URN_AN_CTR)-1)
#define SOS_URN_AN_CTR_STR	"urn:service:sos.animal-control"
#define SOS_URN_AN_CTR_STR_LEN	(sizeof(SOS_URN_AN_CTR_STR)-1)

#define SOS_URN_FIRE		"fire"
#define SOS_URN_FIRE_LEN	(sizeof(SOS_URN_FIRE)-1)
#define SOS_URN_FIRE_STR	"urn:service:sos.fire"
#define SOS_URN_FIRE_STR_LEN	(sizeof(SOS_URN_FIRE_STR)-1)

#define SOS_URN_GAS		"gas"
#define SOS_URN_GAS_LEN		(sizeof(SOS_URN_GAS)-1)
#define SOS_URN_GAS_STR		"urn:service:sos.gas"
#define SOS_URN_GAS_STR_LEN	(sizeof(SOS_URN_GAS_STR)-1)

#define SOS_URN_MAR		"marine"
#define SOS_URN_MAR_LEN		(sizeof(SOS_URN_MAR)-1)
#define SOS_URN_MAR_STR		"urn:service:sos.marine"
#define SOS_URN_MAR_STR_LEN	(sizeof(SOS_URN_MAR_STR)-1)

#define SOS_URN_MOUNT		"mountain"
#define SOS_URN_MOUNT_LEN	(sizeof(SOS_URN_MOUNT)-1)
#define SOS_URN_MOUNT_STR	"urn:service:sos.mountain"
#define SOS_URN_MOUNT_STR_LEN	(sizeof(SOS_URN_MOUNT_STR)-1)

#define SOS_URN_POL		"police"
#define SOS_URN_POL_LEN		(sizeof(SOS_URN_POL)-1)
#define SOS_URN_POL_STR		"urn:service:sos.police"
#define SOS_URN_POL_STR_LEN	(sizeof(SOS_URN_POL_STR)-1)

#define SOS_URN_PHYS		"physician"
#define SOS_URN_PHYS_LEN	(sizeof(SOS_URN_PHYS)-1)
#define SOS_URN_PHYS_STR	"urn:service:sos.physician"
#define SOS_URN_PHYS_STR_LEN	(sizeof(SOS_URN_PHYS_STR)-1)

#define SOS_URN_POIS		"poison"
#define SOS_URN_POIS_LEN	(sizeof(SOS_URN_POIS)-1)
#define SOS_URN_POIS_STR	"urn:service:sos.poison"
#define SOS_URN_POIS_STR_LEN	(sizeof(SOS_URN_POIS_STR)-1)


typedef enum urn_type{URN_GEN_SOS =0, URN_AMB, URN_AN_CTRL, 
	URN_FIRE, URN_GAS, URN_MAR, URN_MOUNT, URN_POL, URN_PHYS, URN_POIS, NOT_URN, NOT_EM_URN} urn_t;

typedef struct emerg_number{
	str number;
	str urn;
	urn_t type;
	struct emerg_number* next;
}em_nb, *em_nb_list;

int store_em_numbers();
void clean_em_numbers();

urn_t is_emerg_ruri(str uri, str * urn);
#endif
