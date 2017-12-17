# $Id: Makefile 692 2009-07-24 12:35:05Z vingarzan $
#
# Makefile for the ISC module
# \author Dragos Vingarzan dragos dot vingarzan -at- fokus dot fraunhofer dot de
#
#
# WARNING: do not run this directly, it should be run by the master Makefile

include ../../Makefile.defs
auto_gen=
NAME=isc.so

ifeq ($(CFG_NAME),sip-router)
	DEFS+=-DSER_MOD_INTERFACE
endif	

DEFS += -I/usr/include/libxml2
LIBS += -L$(LOCALBASE)/lib -lxml2

#DEFS += -DWITH_IMS_PM

include ../../Makefile.modules
