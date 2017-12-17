#!/bin/bash
#
# Proxy-CSCF drop all 4 SA
#
# \author Dragos Vingarzan vingarzan -at- fokus dot fraunhofer dot de
# \author Laurent Etiemble laurent.etiemble -at- inexbee -dot- com
# \author Mamadou Diop mamadou.diop -at- inexbee -dot- com
#

# Strip unwanted characters that surrounds IPv6 addresses
ue=`echo $1 | tr -d "'[]"`
port_uc=$2
port_us=$3

# Strip unwanted characters that surrounds IPv6 addresses
pcscf=`echo $4 | tr -d "'[]"`
port_pc=$5
port_ps=$6

spi_uc=$7
spi_us=$8

spi_pc=$9
spi_ps=${10}

prot=${11}


setkey -c << EOF
spddelete $ue[$port_uc] $pcscf[$port_ps] tcp -P in ;
spddelete $ue[$port_uc] $pcscf[$port_ps] udp -P in ;
delete $ue $pcscf $prot $spi_ps ;

spddelete $pcscf[$port_ps] $ue[$port_uc] tcp -P out ;
spddelete $pcscf[$port_ps] $ue[$port_uc] udp -P out ;
delete $pcscf $ue $prot $spi_uc ;

spddelete $pcscf[$port_pc] $ue[$port_us] tcp -P out ;
spddelete $pcscf[$port_pc] $ue[$port_us] udp -P out ;
delete $pcscf $ue $prot $spi_us ;

spddelete $ue[$port_us] $pcscf[$port_pc] tcp -P in ;
spddelete $ue[$port_us] $pcscf[$port_pc] udp -P in ;
delete $ue $pcscf $prot $spi_pc ;

EOF
