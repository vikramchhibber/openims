/**
 * $Id: receiver.h 702 2009-08-04 17:58:43Z vingarzan $
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
 * CDiameterPeer Receiver process procedures 
 * 
 *  \author Dragos Vingarzan vingarzan -at- fokus dot fraunhofer dot de
 * 
 */



#ifndef __RECEIVER_H
#define __RECEIVER_H

#include "peer.h"
#include "diameter.h"

/** Maximum incoming message length */
#define DP_MAX_MSG_LENGTH 65536

#define DIAMETER_HEADER_LEN 20


typedef enum {
	Receiver_Waiting=0,
	Receiver_Header=1,
	Receiver_Rest_of_Message=2
} receiver_state_t;

/** list of receiver attached peers */
typedef struct _serviced_peer_t {
	peer *p;									/**< the attached peer */
	
	int tcp_socket;								/**< socket used for the Diameter communication */
	
	str send_pipe_name;							/**< name of the pipe to signal messages to be sent out */ 
	int send_pipe_fd;							/**< reader from the pipe to signal messages to be sent out */
	int send_pipe_fd_out;						/**< keep-alive writer for the pipe to signal messages to be sent out */

	receiver_state_t state;						/**< current receiving state */
	char buf[DIAMETER_HEADER_LEN];				/**< buffer to receive header into */
	int buf_len;								/**< received bytes in the header */
	int length;									/**< length of the message as written in the header */
	char *msg;									/**< dynamic buffer for receiving one message */
	int msg_len;								/**< received bytes in the dynamic buffer */
		
	
	struct _serviced_peer_t *next;	/**< first peer in the list */	
	struct _serviced_peer_t *prev; /**< last peer in the list */
} serviced_peer_t;

int receiver_init(peer *p);
void receiver_process(peer *p);

int receiver_send_socket(int sock,peer *p);

int peer_connect(peer *p);
int peer_send(peer *p,int sock,AAAMessage *msg,int locked);
int peer_send_msg(peer *p,AAAMessage *msg);

#endif

