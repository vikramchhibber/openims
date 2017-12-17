#!/bin/bash
#
# UserEndpoint SA for Outgoing Replies ( US -> PC )
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

spi_pc=$5

ealg=$6
ck=$7
alg=$8
ik=$9

prot=${10}
mod=${11}

if [ "$6" = "null" ] 
then
	ck=""
fi

if [ "$prot" = "" ] 
then
	prot="esp"
fi

if [ "$mod" = "tun" ]
then
	mod="tunnel"
	tunnel=$ue-$pcscf
else
	mod="transport"
	tunnel=""
fi

setkey -c << EOF
spdadd $ue[$port_us] $pcscf[$port_pc] tcp -P out ipsec $prot/$mod/$tunnel/unique:2 ;
spdadd $ue[$port_us] $pcscf[$port_pc] udp -P out ipsec $prot/$mod/$tunnel/unique:2 ;
add $ue $pcscf $prot $spi_pc -m $mod -u 2 -E $ealg $ck -A $alg $ik ;
EOF
