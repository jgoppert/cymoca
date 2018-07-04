#!/bin/bash

set -e 

# casadi
mkdir -p /tmp/casadi
pushd /tmp/casadi
echo downloading casadi
wget https://github.com/jgoppert/casadi/archive/3.4.4-jmg.tar.gz > download.log
tar xzvf 3.4.4-jmg.tar.gz > extract.log
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
wget https://github.com/jgoppert/antlr4/archive/4.7.1-cpack.tar.gz > download.log
tar xzvf 4.7.1-cpack.tar.gz > extract.log
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





