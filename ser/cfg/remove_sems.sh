#!/bin/bash

ipcs -s|grep "root"|cut -f 2 -d ' '|awk '{x=sprintf("ipcrm -s %d",$1);system(x);}'
