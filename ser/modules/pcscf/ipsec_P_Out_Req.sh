#!/bin/bash
#
# Proxy-CSCF SA for Outgoing Replies ( US <- PC )
#
# \author Dragos Vingarzan vingarzan -at- fokus dot fraunhofer dot de
# \author Laurent Etiemble laurent.etiemble -at- inexbee -dot- com
# \author Mamadou Diop mamadou.diop -at- inexbee -dot- com
#

# Strip unwanted characters that surrounds IPv6 addresses
ue=`echo $1 | tr -d "'[]"`
port_us=$2
# Strip unwanted characters that surrounds IPv6 addresses
pcscf=`echo $3 | tr -d "'[]"`
port_pc=$4

spi_us=$5

ealg=$6
ck=$7
alg=$8
ik=$9

prot=${10}
mod=${11}
unique_numb=${12}
if [ "$12" = "null" ]
then
unique_numb="3"
fi

if [ "$6" = "null" ] 
then
	ck=""
fi

if [ "$mod" = "tun" ]
then
	mod="tunnel"
	tunnel=$pcscf-$ue
else
	mod="transport"
	tunnel=""
fi

setkey -c << EOF
spdadd $pcscf[$port_pc] $ue[$port_us] tcp -P out ipsec $prot/$mod/$tunnel/unique:$unique_numb ;
spdadd $pcscf[$port_pc] $ue[$port_us] udp -P out ipsec $prot/$mod/$tunnel/unique:$unique_numb ;
add $pcscf $ue $prot $spi_us -m $mod -u $unique_numb -E $ealg $ck -A  $alg $ik ;
EOF
