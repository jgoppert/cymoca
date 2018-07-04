#!/bin/bash
set -e 

rm -rf /tmp/googletest
mkdir -p /tmp/googletest
pushd /tmp/googletest
echo downloading googletest
wget -q https://github.com/google/googletest/archive/release-1.8.0.tar.gz
tar xzf release-1.8.0.tar.gz
pushd googletest-release-1.8.0
echo """
set(CPACK_SET_DESTDIR True)
set(CPACK_PACKAGE_VERSION "1.8.0")
set(CPACK_PACKAGE_CONTACT "googletest@googlegroups.com")
include(CPack)
""" >> CMakeLists.txt
mkdir -p build
pushd build
echo building googletest
cmake -DCMAKE_INSTALL_PREFIX=/usr/local ..
make -j4
echo packaging googletest
cpack -G DEB
sudo dpkg -i *.deb
popd
popd
popd
