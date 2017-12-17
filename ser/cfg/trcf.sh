#!/bin/bash

./ser_ims/cfg/killser trcf
setkey -F
setkey -FP

/opt/OpenIMSCore/ser_ims/ser -f /opt/OpenIMSCore/trcf.cfg -D -D

