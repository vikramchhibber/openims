/**
 * $Id: diameter_epc_code_app.h 1137 2011-08-22 10:21:14Z vingarzan $
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


#ifndef DIAMETER_EPC_CODE_APP_H_
#define DIAMETER_EPC_CODE_APP_H_

// Diameter Application Identifier used in the EPC

//this applications are specific to the PCC
#define EPC_Rx 	16777236
#define EPC_Gx 	16777238
#define EPC_STa 16777250
#define EPC_S6a 16777251
#define EPC_S6d 16777251
#define EPC_S13 16777252
#define EPC_SWm 16777264
#define EPC_SWx 16777265
#define EPC_Gxx 16777266
#define EPC_S9 	16777267
#define EPC_S6b	16777272
#define EPC_Sp	16777280 //not in current standards
#define EPC_PCRF_MC	16777281	//not in current standards


#endif /*DIAMETER_EPC_CODE_APP_H_*/
