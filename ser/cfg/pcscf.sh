#!/bin/bash

setkey -F
setkey -FP

LD_LIBRARY_PATH=/opt/OpenIMSCore/ser/lib /opt/OpenIMSCore/ser/ser -f /opt/OpenIMSCore/ser/pcscf.cfg -D -D

