# $Id: Makefile 1178 2012-01-30 15:03:37Z vingarzan $
#
# Makefile for the CDP module
# \author Dragos Vingarzan vingarzan -at- fokus dot fraunhofer dot de
#
# WARNING: do not run this directly, it should be run by the master Makefile

include ../../Makefile.defs
auto_gen=
NAME=cdp.so

ifeq ($(CFG_NAME),sip-router)
	DEFS+=-DSER_MOD_INTERFACE
endif	

ifeq ($(CFG_NAME),Wharf)
	DEFS += -DWHARF
else
	DEFS += -DCDP_FOR_SER
endif	

DEFS += -I/usr/include/libxml2 
LIBS += -L$(LOCALBASE)/lib -lxml2 -lrt -pthread

include ../../Makefile.modules

