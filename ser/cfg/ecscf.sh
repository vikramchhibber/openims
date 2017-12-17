#!/bin/bash



./ser_ims/cfg/killser ecscf
LD_LIBRARY_PATH=/usr/local/lib/ser /opt/OpenIMSCore/ser_ims/ser -f /opt/OpenIMSCore/ecscf.cfg -D -D

ipcs -s
