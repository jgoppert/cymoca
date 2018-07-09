#!/bin/bash
set -e 

rm -rf /tmp/antlr4
mkdir -p /tmp/antlr4
pushd /tmp/antlr4
echo downloading antlr4
wget -q https://github.com/jgoppert/antlr4/archive/4.7.1-cpack.tar.gz
tar xzf 4.7.1-cpack.tar.gz
pushd antlr4-4.7.1-cpack/runtime/Cpp
mkdir -p build
pushd build
echo building antlr4
cmake .. -DCMAKE_BUILD_TYPE=RelWithDebInfo
make -j4
echo packaging antlr4
cpack -G DEB
sudo dpkg -i *.deb
popd
popd
popd
