#!/bin/bash

set -e 

# casadi
mkdir -p /tmp/casadi > /dev/null
pushd /tmp/casadi > /dev/null
wget https://github.com/jgoppert/casadi/archive/3.4.4-jmg.tar.gz &> download.log
tar xzvf 3.4.4-jmg.tar.gz &> extract.log
pushd casadi-3.4.4-jmg > /dev/null
mkdir -p build > /dev/null
pushd build > /dev/null
cmake .. &>> build.log
make -j4 &>> build.log
cpack -G DEB &>> build.log
sudo dpkg -i *.deb
popd > /dev/null
popd > /dev/null
popd > /dev/null

# antlr4
mkdir -p /tmp/antlr4 > /dev/null
pushd /tmp/antlr4 > /dev/null
wget https://github.com/jgoppert/antlr4/archive/4.7.1-cpack.tar.gz &> download.log
tar xzvf 4.7.1-cpack.tar.gz &> extract.log
pushd antlr4-4.7.1-cpack/runtime/Cpp > /dev/null
mkdir -p build > /dev/null
pushd build &>> /dev/null
cmake .. &>> build.log
make -j4 &>> build.log
cpack -G DEB &>> build.log
sudo dpkg -i *.deb
popd > /dev/null
popd > /dev/null
popd > /dev/null





