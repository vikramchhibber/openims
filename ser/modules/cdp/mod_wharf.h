/*
 * $Id: mod_wharf.h 710 2009-08-07 21:41:31Z vingarzan $
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

#ifndef _CDP__H
#define _CDP__H

#include "../../base/mod.h"


int cdp_init(str config);
int cdp_child_init(int rank);
void cdp_destroy();
void* cdp_get_bind();



#endif

#endif /* WHARF */
