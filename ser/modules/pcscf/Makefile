# $Id: Makefile 1055 2011-03-29 11:00:59Z neits $ Makefile  $Date: 2011-03-29 04:00:59 -0700 (Tue, 29 Mar 2011) $ $Author: neits $ dvi
#
# Makefile for the Proxy-CSCF module
# \author Dragos Vingarzan vingarzan -at- fokus dot fraunhofer dot de
#
# WARNING: do not run this directly, it should be run by the master Makefile

include ../../Makefile.defs
auto_gen=
NAME=pcscf.so


ifeq ($(CFG_NAME),sip-router)
	DEFS+=-DSER_MOD_INTERFACE
	SERLIBPATH=../../lib
	SER_LIBS+=$(SERLIBPATH)/srdb1/srdb1
	LIBS+=-L../../lib/cds -lser_cds
else
	LIBS+=-L../../lib/cds -l_ser_cds
endif

DEFS += -I/usr/include/libxml2 -I../../lib -DSER -I../..
LIBS += -lxml2

DEFS += -DCDP_FOR_SER
DEFS += -D_REENTERANT
DEFS += -I/usr/include/libxml2 
LIBS += -L$(LOCALBASE)/lib -lxml2
LIBS += -lpthread

#DEFS += -DWITH_IMS_PM

include ../../Makefile.modules
