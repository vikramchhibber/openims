/**
 * $Id: diameter_epc_code_result.h 1137 2011-08-22 10:21:14Z vingarzan $
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


#ifndef DIAMETER_EPC_CODE_RESULT_H_
#define DIAMETER_EPC_CODE_RESULT_H_

/**	EPC Specific Result Codes				*/
enum{
/** 1001 to 1999	Informational			*/
/** 2001 to 2999	Success					*/
/** 4001 to 4999	Transient Failures		*/
	RC_EPC_DIAMETER_END_USER_SERVICE_DENIED				= 4010, //TS 32.299
	RC_EPC_DIAMETER_CREDIT_CONTROL_NOT_APPLICABLE		= 4011, //TS 32.299
	RC_EPC_DIAMETER_CREDIT_LIMIT_REACHED				= 4012, //TS 32.299
	RC_EPC_DIAMETER_AUTHENTICATION_DATA_UNAVAILABLE		= 4181,
/** 5001 to 5999	Permanent Failures		*/
	RC_EPC_DIAMETER_ERROR_USER_UNKNOWN					= 5001,
	RC_EPC_DIAMETER_ERROR_IDENTITY_NOT_REGISTERED		= 5003, //TS 29.273
	RC_EPC_DIAMETER_AUTHORIZATION_REJECTED				= 5003, //TS 32.299
	RC_EPC_DIAMETER_ERROR_ROAMING_NOT_ALLOWED			= 5004, //TS 29.273
	RC_EPC_DIAMETER_ERROR_IDENTITY_ALREADY_REGISTERED	= 5005, //TS 29.273	
	RC_EPC_DIAMETER_USER_UNKNOWN						= 5030, //TS 32.299
	RC_EPC_DIAMETER_RATING_FAILED						= 5031, //TS 32.299
	RC_EPC_DIAMETER_ERROR_UNKNOWN_EPS_SUBSCRIPTION		= 5420,
	RC_EPC_DIAMETER_ERROR_RAT_NOT_ALLOWED				= 5421,
	RC_EPC_DIAMETER_ERROR_EQUIPMENT_UNKNOWN				= 5422,
	RC_EPC_DIAMETER_ERROR_USER_NO_NON_3GPP_SUBSCRIPTION = 5450, //TS 29.273
	RC_EPC_DIAMETER_ERROR_USER_NO_APN_SUBSCRIPTION		= 5451, //TS 29.273
	RC_EPC_DIAMETER_ERROR_RAT_TYPE_NOT_ALLOWED			= 5452, //TS 29.273
};



#endif /*DIAMETER_EPC_CODE_RESULT_H_*/
