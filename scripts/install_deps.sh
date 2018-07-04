#!/bin/bash

set -e 

# casadi
mkdir -p /tmp/casadi
pushd /tmp/casadi
wget https://github.com/jgoppert/casadi/archive/3.4.4-jmg.tar.gz
tar xzvf 3.4.4-jmg.tar.gz
pushd casadi-3.4.4-jmg
mkdir -p build
pushd build
cmake ..
make -j4
cpack -G DEB
sudo dpkg -i *.deb
popd 
popd
popd

# antlr4
mkdir -p /tmp/antlr4
pushd /tmp/antlr4
wget https://github.com/jgoppert/antlr4/archive/4.7.1-cpack.tar.gz
tar xzvf 4.7.1-cpack.tar.gz
pushd antlr4-4.7.1-cpack/runtime/Cpp
mkdir -p build
pushd build
cmake ..
make -j4
cpack -G DEB
sudo dpkg -i *.deb
popd 
popd
popd





