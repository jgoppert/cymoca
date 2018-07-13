#!/bin/bash
set -e 

rm -rf /tmp/casadi
mkdir -p /tmp/casadi
pushd /tmp/casadi
echo downloading casadi
wget -q https://github.com/jgoppert/casadi/archive/3.4.4-jmg.tar.gz
tar xzf 3.4.4-jmg.tar.gz
pushd casadi-3.4.4-jmg
mkdir -p build
pushd build
echo building casadi
cmake .. -DCMAKE_BUILD_TYPE=RelWithDebInfo
make -j4
echo packiaging casadi
cpack -G DEB
sudo dpkg -i *.deb
popd
popd
popd
