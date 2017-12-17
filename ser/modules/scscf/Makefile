# $Id: Makefile 719 2009-08-12 15:51:50Z vingarzan $ Makefile  $Date: 2009-08-12 08:51:50 -0700 (Wed, 12 Aug 2009) $ $Author: vingarzan $ dvi
#
# example module makefile
#
#
# WARNING: do not run this directly, it should be run by the master Makefile



include ../../Makefile.defs
auto_gen=
NAME=scscf.so

ifeq ($(CFG_NAME),sip-router)
	DEFS+=-DSER_MOD_INTERFACE
	SERLIBPATH=../../lib
	SER_LIBS+=$(SERLIBPATH)/srdb1/srdb1
	LIBS+=-L../../lib/cds -lser_cds
else
	LIBS+=-L../../lib/cds -l_ser_cds
endif

DEFS += -DCDP_FOR_SER
DEFS += -I/usr/include/libxml2 -I../../lib -DSER -I../..
LIBS += -L$(LOCALBASE)/lib -lxml2

#DEFS += -DWITH_IMS_PM

include ../../Makefile.modules



