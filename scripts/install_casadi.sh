#!/bin/bash
set -e 

rm -rf /tmp/casadi
mkdir -p /tmp/casadi
pushd /tmp/casadi
echo downloading casadi
wget -q https://github.com/casadi/casadi/releases/download/3.4.4/libcasadi-v3.4.4.deb 
echo installing casadi
sudo dpkg -i libcasadi-v3.4.4.deb
popd
