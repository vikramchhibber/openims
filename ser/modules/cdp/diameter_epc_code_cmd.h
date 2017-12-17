/**
 * $Id: diameter_epc_code_cmd.h 1137 2011-08-22 10:21:14Z vingarzan $
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
 * CDiameterPeer Diameter IMS IANA defined numbers
 * 
 * This is a compilation of different 3GPP TSs for EPC:
 * 
 *  \author Alberto Diez   alberto dot diez -at- fokus dot fraunhofer dot de
 *  \author Dragos Vingarzan dragos dot vingarzan -at- fokus dot fraunhofer dot de
 *  
 */


#ifndef DIAMETER_EPC_CODE_CMD_H_
#define DIAMETER_EPC_CODE_CMD_H_
/*	Command Codes used in the EPC 	*/

/*		The Rx Interface 			*/
#define Diameter_AAR		265		/**< Bearer-Authorization		Request	*/
#define Diameter_AAA		265		/**< Bearer-Authorization		Answer	*/
#define Diameter_RAR		258		/**< Re-Auth					Request */
#define Diameter_RAA		258		/**< Re-Auth					Answer	*/
#define Diameter_STR		275		/**< Session Termination 		Request */
#define Diameter_STA		275		/**< Session Termination 		Answer	*/
#define Diameter_ASR		274		/**< Abort-Session-Request		Request */
#define Diameter_ASA		274		/**< Abort-Session-Request		Answer	*/
/*		The Rf Interface			*/
#define Diameter_ACR		271		/**< Accounting Request */
#define Diameter_ACA		271		/**< Accounting Answer  */
/* The Gx and Gxx Interface */
#define Diameter_CCR		272
#define Diameter_CCA		272

/* The Sh/Sp interface */
#define Diameter_UDR 		306
#define Diameter_UDA 		306
#define Diameter_PUR		307
#define Diameter_PUA		307
#define Diameter_SNR		308
#define Diameter_SNA		308
#define Diameter_PNR		309
#define Diameter_PNA		309


/* The S6a/S6d Interfaces */
#define Diameter_ULR		316
#define Diameter_ULA		316
#define Diameter_CLR		317
#define Diameter_CLA		317
#define Diameter_AIR		318
#define Diameter_AIA		318
#define Diameter_IDR		319
#define Diameter_IDA		319
#define Diameter_DSR		320
#define Diameter_DSA		320
#define Diameter_PurgeUER	321
#define Diameter_PurgeUEA	321
#define Diameter_RSR		322
#define Diameter_RSA		322
#define Diameter_NOR		323
#define Diameter_NOA		323

/* The 3GPP EPS AAA Interfaces */
/* SWa - non-3GPP untrusted AN <-> AAA Server/Proxy */
/* SWm - non-3GPP untrusted ePDG <-> AAA Server/Proxy */
/* STa - non-3GPP trusted AN <-> AAA Server/Proxy */
/* S6b/H2 - PGW/HA <-> AAA Server/Proxy */
/* SWd - AAA Server <-> AAA Proxy */
#define Diameter_DER		268
#define Diameter_DEA		268
/* SWx - HSS <-> AAA Server/Proxy */
#define Diameter_SAR		301
#define Diameter_SAA		301
#define Diameter_MAR		303
#define Diameter_MAA		303
#define Diameter_RTR		304
#define Diameter_RTA		304
#define Diameter_PPR		305
#define Diameter_PPA		305


/* The S13 Interface */
#define Diameter_ECR		324
#define Diameter_ECA		324

/*not standard interfaces*/
#define Diameter_MC_AF_ROUTE_UP		400
#define Diameter_MC_AF_ROUTE_DEL	401

#endif /*DIAMETER_EPC_CODE_CMD_H_*/
