/*
 * $Id: str_int_list.c 1167 2011-10-25 09:17:05Z aon $
 *
 * Copyright (C) 2009 FhG Fokus
 *
 * This file is part of the Wharf project.
 *
 */


#ifndef WHARF

#include <string.h>
#include <stdlib.h>
#include "str_int_list.h"

/**
 * Convert a str to long int
 * @param x
 * @param base
 * @return
 */
inline long int str_to_long_int(str x,int base)
{
	char buf[65];
	if (x.len>64){
		LOG(L_ERR,"The give string is too long to convert - %d > 64\n",x.len);
		return 0;
	}
	memcpy(buf,x.s,x.len);
	buf[x.len]=0;
	return strtol(buf,0,base);
}

#endif
