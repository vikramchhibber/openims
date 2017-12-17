# $Id: Makefile 671 2009-06-12 07:51:06Z vingarzan $
#
# Makefile for the MGCF module
# \author Dragos Vingarzan dragos dot vingarzan -at- fokus dot fraunhofer dot de
#
# WARNING: do not run this directly, it should be run by the master Makefile

include ../../Makefile.defs
auto_gen=
NAME=mgcf.so

include ../../Makefile.modules

DEFS += -I../../lib -DSER -I../..
LIBS += -L../../lib/cds -l_ser_cds

#DEFS += -DWITH_IMS_PM