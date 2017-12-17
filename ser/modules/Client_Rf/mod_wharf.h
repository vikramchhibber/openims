/*
 * $Id: mod_wharf.h 1029 2011-02-16 12:44:31Z aon $
 * 
 * Copyright (C) 2009 FhG Fokus
 * 
 * This file is part of the Wharf project.
 * 
 */

/**
 * \file
 * 
 * Client_Rf Wharf module interface
 * 
 * 
 *  \author Andreea Ancuta Corici andreea dot ancuta dot corici -at- fokus dot fraunhofer dot de
 * 
 */

#ifdef WHARF

#ifndef _Client_Rf__H
#define _Client_Rf__H

#include "../../base/mod.h"


int client_rf_init(str config);
int client_rf_child_init(int rank);
void client_rf_destroy(int rank);
void* client_rf_get_bind();

#endif

#endif /* WHARF */
