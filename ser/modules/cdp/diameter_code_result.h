/**
 * $Id: diameter_code_result.h 1019 2011-02-10 17:59:20Z vingarzan $
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
 * CDiameterPeer Diameter Result Codes
 *
 *  
 *  \author Dragos Vingarzan dragos dot vingarzan -at- fokus dot fraunhofer dot de
 * 
 */ 
#ifndef DIAMETER_CODE_RESULT_H_
#define DIAMETER_CODE_RESULT_H_


/** The following are the result codes returned from remote servers as
 * part of messages */
typedef enum {
	AAA_MULTI_ROUND_AUTH          = 1001,
	AAA_SUCCESS                   = 2001,
	AAA_COMMAND_UNSUPPORTED       = 3001,
	AAA_UNABLE_TO_DELIVER         = 3002,
	AAA_REALM_NOT_SERVED          = 3003,
	AAA_TOO_BUSY                  = 3004,
	AAA_LOOP_DETECTED             = 3005,
	AAA_REDIRECT_INDICATION       = 3006,
	AAA_APPLICATION_UNSUPPORTED   = 3007,
	AAA_INVALID_HDR_BITS          = 3008,
	AAA_INVALID_AVP_BITS          = 3009,
	AAA_UNKNOWN_PEER              = 3010,
	AAA_AUTHENTICATION_REJECTED   = 4001,
	AAA_OUT_OF_SPACE              = 4002,
	AAA_ELECTION_LOST             = 4003,
	AAA_AVP_UNSUPPORTED           = 5001,
	AAA_UNKNOWN_SESSION_ID        = 5002,
	AAA_AUTHORIZATION_REJECTED    = 5003,
	AAA_INVALID_AVP_VALUE         = 5004,
	AAA_MISSING_AVP               = 5005,
	AAA_RESOURCES_EXCEEDED        = 5006,
	AAA_CONTRADICTING_AVPS        = 5007,
	AAA_AVP_NOT_ALLOWED           = 5008,
	AAA_AVP_OCCURS_TOO_MANY_TIMES = 5009,
	AAA_NO_COMMON_APPLICATION     = 5010,
	AAA_UNSUPPORTED_VERSION       = 5011,
	AAA_UNABLE_TO_COMPLY          = 5012,
	AAA_INVALID_BIT_IN_HEADER     = 5013,
	AAA_INVALIS_AVP_LENGTH        = 5014,
	AAA_INVALID_MESSGE_LENGTH     = 5015,
	AAA_INVALID_AVP_BIT_COMBO     = 5016,
	AAA_NO_COMMON_SECURITY        = 5017,
} AAAResultCode;



/**	Diameter Result Codes				*/
enum {
	/* RFC 3588 */
	DIAMETER_MULTI_ROUND_AUTH							= 1001,	
	
	DIAMETER_SUCCESS									= 2001,//7D1
	DIAMETER_LIMITED_SUCCESS							= 2002,
	
	DIAMETER_COMMAND_UNSUPPORTED						= 3001,
	DIAMETER_UNABLE_TO_DELIVER							= 3002,
	DIAMETER_REALM_NOT_SERVED							= 3003,//0xBBB
	DIAMETER_TOO_BUSY									= 3004,
	DIAMETER_LOOP_DETECTED								= 3005,
	DIAMETER_REDIRECT_INDICATION						= 3006,
	DIAMETER_APPLICATION_UNSUPPORTED					= 3007,
	DIAMETER_INVALID_HDR_BITS							= 3008,
	DIAMETER_INVALID_AVP_BITS							= 3009,
	DIAMETER_UNKNOWN_PEER								= 3010,
	
	DIAMETER_AUTHENTICATION_REJECTED					= 4001,//FA1
	DIAMETER_OUT_OF_SPACE								= 4002,
	DIAMETER_ELECTION_LOST								= 4003,
	
	DIAMETER_AVP_UNSUPPORTED							= 5001,
	DIAMETER_UNKNOWN_SESSION_ID							= 5002,
	DIAMETER_AUTHORIZATION_REJECTED						= 5003,//138B
	DIAMETER_INVALID_AVP_VALUE							= 5004,
	DIAMETER_MISSING_AVP								= 5005,//0x138D
	DIAMETER_RESOURCES_EXCEEDED							= 5006,
	DIAMETER_CONTRADICTING_AVPS							= 5007,
	DIAMETER_AVP_NOT_ALLOWED							= 5008,//0x140
	DIAMETER_AVP_OCCURS_TOO_MANY_TIMES 					= 5009,
	DIAMETER_NO_COMMON_APPLICATION						= 5010,//0x1392
	DIAMETER_UNSUPPORTED_VERSION						= 5011,
	DIAMETER_UNABLE_TO_COMPLY							= 5012,//0x1394
	DIAMETER_INVALID_BIT_IN_HEADER						= 5013,
	DIAMETER_INVALID_AVP_LENGTH							= 5014,
	DIAMETER_INVALID_MESSAGE_LENGTH						= 5015,
	DIAMETER_INVALID_AVP_BIT_COMBO						= 5016,
	DIAMETER_NO_COMMON_SECURITY							= 5017,//0x1399
	
	/* RFC 4006 */
	DIAMETER_END_USER_SERVICE_DENIED					= 4010,
	DIAMETER_CREDIT_CONTROL_NOT_APPLICABLE				= 4011,
	DIAMETER_CREDIT_LIMIT_REACHED						= 4012,
	
	DIAMETER_USER_UNKNOWN								= 5030,
	DIAMETER_RATING_FAILED								= 5031
};



#endif /*DIAMETER_CODE_RESULT_H_*/
