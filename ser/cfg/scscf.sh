#!/bin/bash



LD_LIBRARY_PATH=/opt/OpenIMSCore/ser/lib /opt/OpenIMSCore/ser/ser -f /opt/OpenIMSCore/ser/scscf.cfg -D -D

ipcs -s
