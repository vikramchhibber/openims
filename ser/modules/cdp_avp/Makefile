# $Id: Makefile 888 2010-02-12 13:03:30Z vingarzan $
#
# Makefile for the CDiameterPeer AVP Operations module
#
# \author Dragos Vingarzan dragos dot vingarzan -at- fokus dot fraunhofer dot de
#

include ../../Makefile.defs


NAME=cdp_avp.so


ifeq ($(CFG_NAME),sip-router)
	DEFS+=-DSER_MOD_INTERFACE
endif	

ifeq ($(CFG_NAME),Wharf)
	DEFS += -DWHARF
else
	DEFS += -DCDP_FOR_SER
	DEFS += -I/usr/include/libxml2 
endif

#libs+=
#includes+=


include ../../Makefile.modules

