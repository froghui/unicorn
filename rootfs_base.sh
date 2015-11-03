#!/bin/bash

sudo apt-get -y install debootstrap
sudo mkdir -p ~/rootfs_base
cd ~/rootfs_base 
if [ -d ubuntu-precise ];then
    echo "has done debootstrap. exit"
else
    sudo debootstrap --arch amd64 precise ubuntu-precise http://mirrors.163.com/ubuntu/
fi

sudo mkdir -p /tmp/rootfs_base
sudo rm -rf /tmp/rootfs_base
echo "copying rootfs to /tmp/rootfs_base..."
sudo cp -r ~/rootfs_base/ubuntu-precise /tmp/rootfs_base
sudo rm -rf /tmp/rootfs_base/dev/ptmx
