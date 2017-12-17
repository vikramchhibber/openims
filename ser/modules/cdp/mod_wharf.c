/*
 * $Id: mod_wharf.c 920 2010-03-05 19:07:50Z vingarzan $
 * 
 * Copyright (C) 2009 FhG Fokus
 * 
 * This file is part of the Wharf project.
 * 
 */

/**
 * \file
 * 
 * CDiameterPeer Wharf module interface
 * 
 * 
 *  \author Dragos Vingarzan dragos dot vingarzan -at- fokus dot fraunhofer dot de
 * 
 */

#ifdef WHARF

#include "stdlib.h"

#include "../../base/mod.h"

#include "../../base/mod_ops.h"

#include "../../base/fork.h"

#include "../../utils/utils.h"

#include "mod_wharf.h"

#include "cdp_load.h"

#include "diameter_peer.h"

/** This variable has to be called exactly mod_exports, as it is searched by the wharf base */
wharf_mod_export_t mod_exports={
		
		"cdp",					/**< Module's unique name */
		
		WHARF_VERSION"-"WHARF_REVISION,		
		
		cdp_init,				/**< Module init function */
		cdp_child_init,			/**< Module child init function */
		cdp_destroy,			/**< Module destroy function */
		
		cdp_get_bind			/**< Module get binding function */
		
};	


/** 
 * Module init function.
 * 
 * - Initializes the diameter peer using the provided configuration file.
 * - Registers with pt the required number of processes.
 */
int cdp_init(str config_data)
{
	LOG(L_INFO," CDiameterPeer initializing\n");
	if (!diameter_peer_init_str(config_data)){
		LOG(L_CRIT," error initializing the diameter peer\n");
		return 1;
	}	
	return 1;
}

/**
 * Module initialization function - called once for every process.
 * \note All modules have by now executed the mod_init.
 * If this returns failure, wharf will exit
 * 
 * @param rank - rank of the process calling this
 * @return 1 on success or 0 on failure
 */

int cdp_child_init(int rank)
{
	if (rank == WHARF_PROCESS_ATTENDANT) { 
		LOG(L_INFO,"CDiameterPeer starting ...\n");		
		diameter_peer_start(0);
		LOG(L_INFO," ... CDiameterPeer started\n");		
	}
	return 1;
}

/**
 * Module destroy function. 
 * Should clean-up and do nice shut-down.
 * \note Will be called multiple times, once from each process, although crashed processes might not.
 */
void cdp_destroy(int rank)
{
	switch (rank){
//		case WHARF_PROCESS_CDP_WORKER:
//		case WHARF_PROCESS_CDP_RECEIVER:
//		case WHARF_PROCESS_CDP_ACCEPTOR:
//		case WHARF_PROCESS_CDP_TIMER:
		case WHARF_PROCESS_TIMER:
			LOG(L_INFO,"CDiameterPeer stoping ...\n");		
			diameter_peer_destroy();
			LOG(L_INFO,"... CDiameterPeer stoped\n");
			break;
	}
}



struct cdp_binds cdp_binding={
	AAACreateRequest,
	AAACreateResponse,
	AAAFreeMessage,


	AAACreateAVP,
	AAAAddAVPToMessage,
	AAAAddAVPToList,
	AAAFindMatchingAVP,
	AAAFindMatchingAVPList,
	AAAGetNextAVP,
	AAAFreeAVP,
	AAAFreeAVPList,
	AAAGroupAVPS,
	AAAUngroupAVPS,

	AAASendMessage,
	AAASendMessageToPeer,
	AAASendRecvMessage,
	AAASendRecvMessageToPeer,


	AAAAddRequestHandler,
	AAAAddResponseHandler,


	AAACreateTransaction,
	AAADropTransaction,


	AAACreateSession,
	AAAMakeSession,
	AAAGetSession,
	AAADropSession,
	AAASessionsLock,
	AAASessionsUnlock,

	AAACreateClientAuthSession,
	AAACreateServerAuthSession,
	AAAGetAuthSession,
	AAADropAuthSession,
	AAATerminateAuthSession,

};


/**
 * Returns the module's binding. This will give the structure containing the 
 * functions and data to be used from other processes.
 * @return the pointer to the binding.
 */
void* cdp_get_bind()
{
	return &cdp_binding;
}



#endif /* WHARF */
