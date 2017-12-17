/*
 * $Id: acr.h 1040 2011-02-17 16:41:54Z aon $
 * 
 * Copyright (C) 2009 FhG Fokus
 * 
 * This file is part of the Wharf project.
 * 
 */

/**
 * \file
 * 
 * Wharf CDF_Rf module -  
 * 
 * 
 *  \author Andreea Onofrei Corici andreea dot ancuta dot corici -at- fokus dot fraunhofer dot de
 * 
 */
 
#ifndef _Client_Rf_ACR_H
#define _Client_Rf_ACR_H

#ifdef WHARF
#include "../../utils/utils.h"
#endif /*WHARF*/

#include "../cdp/diameter.h"
#include "Rf_data.h"

AAAMessage *Rf_new_acr(AAASession * session, Rf_ACR_t *);

#endif
