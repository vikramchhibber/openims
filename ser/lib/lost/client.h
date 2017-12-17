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
 * author Ancuta Onofrei, 
 * 	email andreea dot ancuta dot onofrei -at- fokus dot fraunhofer dot de
 */

#ifndef LIB_LOST_CLIENT_H
#define LIB_LOST_CLIENT_H

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>

#define SER	1
#include <curl/curl.h>
//#include <curl/types.h>
#include <curl/easy.h>

//#include <libxml/tree.h>
//#include <libxml/parser.h>
//#include <libxml/HTMLparser.h>

//#include <cds/memory.h>
#include <cds/logger.h>
#include <cds/list.h>

#include "../../version.h"
#include "../../str.h"
#include "../../mem/mem.h"
#include "../../mem/shm_mem.h"
#include "../../parser/msg_parser.h"
#include "../../parser/parse_uri.h"
#include "pidf_loc.h"


#define LOST_CONTENT_TYPE		"Content-Type: application/lost+xml"
#define LOST_CACHE_CONTROL		"Cache-Control: no-cache"
//#define LOST_DEBUG			0
#define LOST_ERR_NODE_NAME		"errors"
#define LOST_REDIR_NODE_NAME		"redirect"
#define LOST_WRNG_NODE_NAME		"warnings"
#define LOST_MAPPING_NODE_NAME		"mapping"
#define LOST_URI_NODE_NAME		"uri"
#define LOST_FINDSRESP_NODE_NAME	"findServiceResponse"

#define LOST_MSG_ATTR_NAME		"message"
#define LOST_TGT_ATTR_NAME		"target"
#define LOST_EXPIRES_ATTR_NAME		"expires"

#define LOST_EXP_NO_CACHE		"NO-CACHE"
#define LOST_EXP_NO_EXPIRATION		"NO-EXPIRATION"

#define LOST_NS_HREF			"urn:ietf:params:xml:ns:lost1"
#define LOST_XML_ENC			"UTF-8"
#define LOST_FIND_SERVICE_CMD		"findService"
#define LOST_LOCATION_NODE		"location"
#define LOST_SERVICE_NODE		"service"

#define LOST_ID_PROP			"id"
#define LOST_PROFILE_PROP		"profile"

int init_lost_lib();
void end_lost_lib();

CURL* lost_http_conn(char *url, int port, str* chunk);
int create_lost_req(xmlNode* location, char * service, loc_fmt d_loc_fmt, str* lost_req);
int send_POST_data(CURL* connhandle, str data);
void lost_http_disconn(CURL* connhandle);

#endif
