/**
 * $Id: diameter_ims_code_result.h 1018 2011-02-10 14:00:57Z vingarzan $
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
 * CDiameterPeer Diameter IMS IANA defined Result Codes
 * 
 * This is a compilation of different 3GPP TSs:
 * - TS 29.209 for IMS_Gq
 * - TS 29.229 for IMS_Cx IMS_Dx
 * - TS 29.329 for IMS_Sh IMS_Ph
 * 
 *  \author Dragos Vingarzan dragos dot vingarzan -at- fokus dot fraunhofer dot de
 *  
 */
 
#ifndef __DIAMETER_IMS_CODE_RESULT_H
#define __DIAMETER_IMS_CODE_RESULT_H



/**	IMS Specific Result Codes			*/
enum{
/** 1001 to 1999	Informational			*/
/** 2001 to 2999	Success					*/
/**	2001 to 2020 Reserved for TS29.229	*/
	RC_IMS_DIAMETER_FIRST_REGISTRATION 					= 2001,
	RC_IMS_DIAMETER_SUBSEQUENT_REGISTRATION				= 2002,
	RC_IMS_DIAMETER_UNREGISTERED_SERVICE				= 2003,
	RC_IMS_DIAMETER_SUCCESS_SERVER_NAME_NOT_STORED		= 2004,
	RC_IMS_DIAMETER_SERVER_SELECTION					= 2005,
/**	2401 to 2420 Reserved for TS29.109	*/
/** 4001 to 4999	Transient Failures	*/
/**	4100 to 4120 Reserved for TS29.329	*/
	RC_IMS_DIAMETER_USER_DATA_NOT_AVAILABLE 			= 4100,
	RC_IMS_DIAMETER_PRIOR_UPDATE_IN_PROGRESS			= 4101,
/**	41xx to 41yy Reserved for TS32.299	*/
/** 5001 to 5999	Permanent Failures		*/
/**	5001 to 5020 Reserved for TS29.229	*/
	RC_IMS_DIAMETER_ERROR_USER_UNKNOWN					= 5001,
	RC_IMS_DIAMETER_ERROR_IDENTITIES_DONT_MATCH			= 5002,
	RC_IMS_DIAMETER_ERROR_IDENTITY_NOT_REGISTERED		= 5003,
	RC_IMS_DIAMETER_ERROR_ROAMING_NOT_ALLOWED			= 5004,
	RC_IMS_DIAMETER_ERROR_IDENTITY_ALREADY_REGISTERED	= 5005,
	RC_IMS_DIAMETER_ERROR_AUTH_SCHEME_NOT_SUPPORTED		= 5006,
	RC_IMS_DIAMETER_ERROR_IN_ASSIGNMENT_TYPE			= 5007,
	RC_IMS_DIAMETER_ERROR_TOO_MUCH_DATA					= 5008,
	RC_IMS_DIAMETER_ERROR_NOT_SUPPORTED_USER_DATA		= 5009,
	RC_IMS_DIAMETER_MISSING_USER_ID						= 5010,
	RC_IMS_DIAMETER_ERROR_FEATURE_UNSUPPORTED			= 5011,
/**	5021 to 5040 Reserved for TS32.299	*/
/**	5041 to 5060 Reserved for TS29.234	*/
/**	5061 to 5080 Reserved for TS29.209	*/
	RC_IMS_DIAMETER_ERROR_INVALID_SERVICE_INFORMATION	= 5061,
	RC_IMS_DIAMETER_ERROR_FILTER_RESTRICTIONS			= 5062,
/**	5061 to 5066 Reserved for TS29.214	*/
	RC_IMS_DIAMETER_ERROR_REQUESTED_SERVICE_NOT_AUTHORIZED		= 5063,
	RC_IMS_DIAMETER_ERROR_DUPLICATED_AF_SESSION			= 5064,
	RC_IMS_DIAMETER_ERROR_IPCAN_SESSION_NOT_AVAILABLE		= 5065,
	RC_IMS_DIAMETER_ERROR_UNAUTHORIZED_NON_EMERGENCY_SESSION	= 5066,
/**	5100 to 5119 Reserved for TS29.329	*/
	RC_IMS_DIAMETER_ERROR_USER_DATA_NOT_RECOGNIZED		= 5100,
	RC_IMS_DIAMETER_ERROR_OPERATION_NOT_ALLOWED			= 5101,
	RC_IMS_DIAMETER_ERROR_USER_DATA_CANNOT_BE_READ		= 5102,
	RC_IMS_DIAMETER_ERROR_USER_DATA_CANNOT_BE_MODIFIED	= 5103,
	RC_IMS_DIAMETER_ERROR_USER_DATA_CANNOT_BE_NOTIFIED	= 5104,
	RC_IMS_DIAMETER_ERROR_TRANSPARENT_DATA_OUT_OF_SYNC	= 5105,
	RC_IMS_DIAMETER_ERROR_SUBS_DATA_ABSENT				= 5106,
	RC_IMS_DIAMETER_ERROR_NO_SUBSCRIPTION_TO_DATA		= 5107,
	RC_IMS_DIAMETER_ERROR_DSAI_NOT_AVAILABLE 			= 5108
/** 5400 to 5419 Reserved for TS29.109	*/
};

#endif /* __DIAMETER_IMS_CODE_RESULT_H */
