#!/bin/ash

sudo apt-get install -y bridge-utils

#Note the eth2 should be replaced by your physial network card and 
#192.168.3.11 should be replaced by your NIC IP

#sudo brctl addbr br0
#sudo brctl addif br0 eth2
#sudo ifconfig br0 192.168.3.11 netmask 255.255.255.0 up
#sudo ifconfig eth2 0.0.0.0 up

