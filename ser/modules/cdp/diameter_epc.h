/**
 * $Id: diameter_epc.h 1018 2011-02-10 14:00:57Z vingarzan $
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
 * CDiameterPeer Diameter EPC IANA defined numbers
 * 
 * This is a compilation of different 3GPP TSs for EPC:
 * 
 *  \author Alberto Diez   alberto dot diez -at- fokus dot fraunhofer dot de
 *  \author Dragos Vingarzan dragos dot vingarzan -at- fokus dot fraunhofer dot de
 *  
 */


#ifndef DIAMETER_EPC_H_
#define DIAMETER_EPC_H_


#include "diameter_epc_code_app.h"
#include "diameter_epc_code_avp.h"
#include "diameter_epc_code_cmd.h"
#include "diameter_epc_code_result.h"

// Diameter Application Identifier used in the EPC

#define EPC_vendor_id_3GPP 		10415		/**< Vendor Id for 3GPP */
#define EPC_vendor_id_3GPP_char "10415" 	/**< char value for 3GPP's Vendor Id */
#define EPC_vendor_id_3GPP_len	5			/**< len of char value for 3GPP's Vendor Id */




#endif /*DIAMETER_EPC_H_*/
