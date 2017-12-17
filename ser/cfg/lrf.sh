#!/bin/bash



./ser_ims/cfg/killser lrf
LD_LIBRARY_PATH=/usr/local/lib/ser /opt/OpenIMSCore/ser_ims/ser -f /opt/OpenIMSCore/lrf.cfg -D -D

ipcs -s
