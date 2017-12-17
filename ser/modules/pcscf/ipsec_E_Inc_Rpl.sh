#!/bin/bash
#
# UserEndpoint SA for Incoming Replies ( UC <- PS )
#
# \author Dragos Vingarzan vingarzan -at- fokus dot fraunhofer dot de
# \author Laurent Etiemble laurent.etiemble -at- inexbee -dot- com
# \author Mamadou Diop mamadou.diop -at- inexbee -dot- com
#

# Strip unwanted characters that surrounds IPv6 addresses
ue=`echo $1 | tr -d "'[]"`
port_uc=$2
# Strip unwanted characters that surrounds IPv6 addresses
pcscf=`echo $3 | tr -d "'[]"`
port_ps=$4

spi_uc=$5

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
	tunnel=$pcscf-$ue
else
	mod="transport"
	tunnel=""
fi

setkey -c << EOF
spdadd $pcscf[$port_ps] $ue[$port_uc] tcp -P in ipsec $prot/$mod/$tunnel/require ;
spdadd $pcscf[$port_ps] $ue[$port_uc] udp -P in ipsec $prot/$mod/$tunnel/require ;
add $pcscf $ue $prot $spi_uc -m $mod -E $ealg $ck -A $alg $ik ;
EOF
