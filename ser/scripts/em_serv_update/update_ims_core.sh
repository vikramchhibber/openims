#!/bin/bash

cd /opt/OpenIMSCore/ser_ims
echo "Updating ser_ims"
svn update 
echo "Cleaning ser_ims"
make clean
echo "Compiling ser_ims"
make all
echo "Installing libs from ser_ims"
make install-libs all 
echo "Copying the running scripts"
cd cfg/
cp pcscf.cfg ../../orig_pcscf.cfg
cp icscf.cfg ../../orig_icscf.cfg
cp scscf.cfg ../../orig_scscf.cfg
cp ecscf.cfg ../../orig_ecscf.cfg
cp lrf.cfg ../../orig_lrf.cfg


cp pcscf.xml ../../orig_pcscf.xml
cp icscf.xml ../../orig_icscf.xml
cp scscf.xml ../../orig_scscf.xml

cd ../../
cd FHoSS
echo "Updating FHoSS"
svn update
rm -fr deploy/
echo "Compiling FHoSS"
ant compile
ant deploy
cd deploy/
echo "Copying running configuration"
cp hss.properties orig_hss.properties
cp DiameterPeerHSS.xml orig_DiameterPeerHSS.xml

