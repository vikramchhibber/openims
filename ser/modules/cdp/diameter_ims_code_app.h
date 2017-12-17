/**
 * $Id: diameter_ims_code_app.h 1179 2012-02-17 14:29:01Z vingarzan $
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
 * CDiameterPeer Diameter IMS IANA defined Application Identifiers
 * 
 *  \author Dragos Vingarzan dragos dot vingarzan -at- fokus dot fraunhofer dot de
 *  
 */
 
#ifndef __DIAMETER_IMS_CODE_APP_H
#define __DIAMETER_IMS_CODE_APP_H

/* Application Identifiers	*/
#define IMS_Gq	16777222	/**< Gq interface between P-CSCF and PDF 		*/
#define IMS_Cx	16777216	/**< Cx interface between I/S-CSCF and HSS 		*/
#define IMS_Dx	16777216	/**< Cx interface between I/S-CSCF and SLF 		*/
#define IMS_Sh	16777217	/**< Sh interface between AS and HSS	 		*/
#define IMS_e2	16777231	/**< e2 interface between CLF and AF	 		*/
#define IMS_Ph	16777217	/**< Sh interface between PresenceServer and HSS*/
#define IMS_Rx  16777236	/**< Rx interface between P-CSCF and PCRF 		*/
#define IMS_Gx	16777224	/**< Gx interface between PCRF and PCEF 		*/
#define IMS_Rf  3			/**< Rf interface between P/I/S-CSCF and CDF, 
								according to TS32.299 R7 it should be 16777223.
								But R10/11 indicates 3, from RFC 3588 Diameter Base Accounting    */ 

#endif /* __DIAMETER_IMS_CODE_APP_H */
