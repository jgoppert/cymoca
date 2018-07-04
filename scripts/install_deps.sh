#!/bin/bash

set -e 

# casadi
mkdir -p /tmp/casadi
pushd /tmp/casadi
echo downloading casadi
wget -q https://github.com/jgoppert/casadi/archive/3.4.4-jmg.tar.gz
tar xzf 3.4.4-jmg.tar.gz
pushd casadi-3.4.4-jmg
mkdir -p build
pushd build
echo building casadi
cmake ..
make -j4
echo packiaging casadi
cpack -G DEB
sudo dpkg -i *.deb
popd
popd
popd

# antlr4
mkdir -p /tmp/antlr4
pushd /tmp/antlr4
echo downloading antlr4
wget -q https://github.com/jgoppert/antlr4/archive/4.7.1-cpack.tar.gz
tar xzf 4.7.1-cpack.tar.gz
pushd antlr4-4.7.1-cpack/runtime/Cpp
mkdir -p build
pushd build
echo building antlr4
cmake ..
make -j4
echo packaging antlr4
cpack -G DEB
sudo dpkg -i *.deb
popd
popd
popd





