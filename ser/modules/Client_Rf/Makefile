# $Id: Makefile $date aon $
#
# Location Retrieval Function (LRF) including a Routing Determination Function based on a LoST (RFC 5222) client
#
# 
# WARNING: do not run this directly, it should be run by the master Makefile

include ../../Makefile.defs
auto_gen=
NAME=Client_Rf.so
LIBS=

ifeq ($(CFG_NAME),sip-router)
	DEFS += -DSER_MOD_INTERFACE
	SERLIBPATH=/../lib
	#SER_LIBS+=$(SERLIBPATH)/srdb1/srdb1
	DEFS += -I/usr/include/libxml2 -I../lib -DSER -I../..
	DEFS += -I../../lib 
	LIBS += -L/usr/local/lib -L../lib/cds -lcds
endif

ifeq ($(CFG_NAME),Wharf)
	DEFS += -DWHARF
else
	DEFS += -DCDP_FOR_SER
	DEFS += -I/usr/include/libxml2 -I../../lib -DOPEN_IMS_CORE -DSER -I../..
	LIBS += -L/usr/local/lib -L../../lib/cds
endif	

DEFS += -I/usr/include/libxml2
LIBS += -L$(LOCALBASE)/lib -lxml2 -lrt

include ../../Makefile.modules
