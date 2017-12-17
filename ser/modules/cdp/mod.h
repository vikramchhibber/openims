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
  
 
/**
 * \dir modules/cdp
 *
 * This is the CDiameterPeer module. For general documentation, look at \ref CDP
 * 
 */
 
 /**
  *  \file modules/cdp/Makefile 
  * CDiameterPeer SER module Makefile
  * \author Dragos Vingarzan vingarzan -at- fokus dot fraunhofer dot de
  */
 
/** \page CDP The CDiameterPeer Module (cdp)
 *  \b Module \b Documentation
 * 
 * [\subpage cdp_overview]
 * [\subpage cdp_realm_routing]
 * [\subpage cdp_code]
 * [\subpage cdp_config]
 * [\subpage cdp_example]
 *
 * \author Dragos Vingarzan vingarzan -at- fokus dot fraunhofer dot de
 * 
 * Authorization Session support is now integrated. For Accounting Sessions there is for now no state machine
 * defined, although the generic session support could be easily extended in the future if this would be required.
 * 
 * 
 * 
 * \section cdp_overview Overview
 * This module is supposed to allow efficient Diameter communication to and from
 * SER. Some parts of the diameter codec are inherited from DISC 
 * http://developer.berlios.de/projects/disc/ . 
 * 
 * 
 * A series of processes are forked:
 * - 1 x Timer - timer.c
 * - 1 x Acceptor - acceptor.c
 * - k x Worker - worker.c (k configurable)   
 * - p x Receiver - receiver.c - one for each initially configured peers
 * - 1 x Receiver - receiver.c - one receiver for all other unknown peers
 * 
 * Due to sip-router restrictions, all processes must be forked at the start-up, without dynamic ones.
 * The initial cdp architecture, which assigned one process for each peer, has been modified:
 * - there are dedicated receiver processes for each initially configured peers. Each peer should be then initially
 * configured as to obtain the best performance, or none if the minimum number of processes are to be obtained
 * - there is one special receiver process for handling all the unknown peers
 * - on incoming connections, the accept process passed the TCP socket descriptor to the special receiver (as the 
 * peer identity is unknown before the CER)
 * - after CER, if the peer is not dynamic, the special receiver passes once again the TCP socket descriptor to the
 * dedicated receiver if one exists
 * - on outgoing connections, the timer makes the connection and then passes it to the respective receiver process 
 * assigned to that process or to the special receiver if the peer does not have a dedicated receiver.
 * 
 * \section cdp_realm_routing Realm Routing
 * Realm routing is integrated - see the example configuration file for a simple usage example.
 * 
 * For proper routing of Diameter requests, if a specific destination peer is to be targeted, the application
 * should add a Destination-Host AVP. Then the message will be routed directly there, if that peer is connected.
 * 
 * Otherwise, if the destination peer is not previously discovered, a Destination Realm must be filled, which will
 * trigger the use of specific Realm routes. Then the application should include an Auth/Acct-Application-Id AVP 
 * or a Vendor-Specific-Application-Id AVP. The stack will search then in the list of connected peers for one that
 * supports that. The support list is composed of what the peers are declaring at connection, during the 
 * capabilities exchange operations. Keep in mind that not only the Application-Id, but also the Vendor-Id (or 0 in
 * case none is defined) are taken into account.
 * 
 * The most often issue of messages not being routed is the lack of the Application-Id indicating AVP as presented
 * above, or a misconfiguration of supported applications on the other peer.   
 * 
 * 
 * \section cdp_code Code Structure
 * The SER module exported API can be seen in mod.c. The full Diameter API is in diameter.h
 * and diameter_api.h. IMS specific constants and assigned numbers are defined in 
 * diameter_ims.h. 
 * 
 * The protocol codec is implemented into diameter_avp.c and diameter_msg.c. 
 * The Diameter base protocol state machine is implemented in peerstatemachine.c. 
 * peermanager.c takes care of peer administration. Basic transactions are defined in 
 * transaction.c.   
 * 
 * \section cdp_config Configuration and Usage
 * For exported functions look at #cdp_cmds.\n
 * For configuration parameters look at #cdp_params.\n 
 * 
 * For input configuration file, take a look at configdtd.h for the structure. Here we have
 * an example (not really usable, but you should be able to derive your own):
 * \code
<?xml version="1.0" encoding="UTF-8"?>
<DiameterPeer 
	FQDN="xcscf.open-ims.test"
	Realm="open-ims.test"
	Vendor_Id="10415"
	Product_Name="CDiameterPeer"
	AcceptUnknownPeers="0"
	DropUnknownOnDisconnect="1"
	Tc="30"
	Workers="4"
	QueueLength="32"
>
	<Peer FQDN="hss1.open-ims.test" Realm="open-ims.test" port="3868"/>
	<Peer FQDN="hss2.open-ims.test" Realm="open-ims.test" port="3868"/>

	<Acceptor port="3868"  />
	<Acceptor port="3869" bind="127.0.0.1" />
	<Acceptor port="3870" bind="192.168.1.1" />
	
    <Auth id="16777216" vendor="10415"/><!-- 3GPP Cx -->
    <Auth id="16777216" vendor="4491"/><!-- CableLabs Cx -->
    <Auth id="16777216" vendor="13019"/><!-- ETSI/TISPAN Cx -->

	<Realm name="my.open-ims.test">
		<Route FQDN="blackjack" metric="2"/>
		<Route FQDN="test1" metric="3"/>
		<Route FQDN="test2" metric="5"/>
	</Realm>
	<Realm name="test1.open-ims.test">
		<Route FQDN="test3" metric="7"/>
		<Route FQDN="test4" metric="11"/>
	</Realm>
	<Realm name="test2.open-ims.test">
		<Route FQDN="test5" metric="13"/>
	</Realm>
	<DefaultRoute FQDN="hss1.open-ims.test" metric="15"/>
	<DefaultRoute FQDN="hss2.open-ims.test" metric="13"/>

</DiameterPeer>
 * \endcode
 * 
 * It is used by :
 * - \ref ICSCF ;
 * - \ref SCSCF ;
 *
 * \section cdp_example Examples
 * 
 * And here are some real usage examples:
 * - Interrogating-CSCF CDiameterPeer configuration file \include icscf.xml
 * - Serving-CSCF CDiameterPeer configuration file \include scscf.xml
 * 
 */

/**
 * \file
 * 
 * CDiameterPeer SER module interface and declarations.
 * 
 *  \author Dragos Vingarzan vingarzan -at- fokus dot fraunhofer dot de
 */
 
#ifdef CDP_FOR_SER

#ifndef _CDP__H
#define _CDP__H

#include "../../sr_module.h"

#define M_NAME "cdp"


static int cdp_init( void );
static int cdp_child_init( int rank );
static int cdp_exit( void );


#endif

#endif /* CDP_FOR_SER */
