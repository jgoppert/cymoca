cmake_minimum_required(VERSION 3.5)

project(cymoca C CXX)

option(WITH_CASADI "enable casadi" ON)


#-----------------------------------------------------------------------------
# Setup
#-----------------------------------------------------------------------------
set(CMAKE_MODULE_PATH ${CMAKE_CURRENT_SOURCE_DIR}/cmake ${CMAKE_MODULE_PATH})
set(CMAKE_CXX_STANDARD 14)
set(EP_PREFIX ${CMAKE_BINARY_DIR}/deps)
set_property(DIRECTORY PROPERTY EP_PREFIX ${EP_PREFIX})

# find only static libraries
if (WIN32)
    SET(CMAKE_FIND_LIBRARY_SUFFIXES .lib .a ${CMAKE_FIND_LIBRARY_SUFFIXES})
else (WIN32)
    SET(CMAKE_FIND_LIBRARY_SUFFIXES .a ${CMAKE_FIND_LIBRARY_SUFFIXES})
endif (WIN32)

# packages
set(Boost_USE_STATIC_LIBS ON)
find_package(Boost COMPONENTS
        filesystem
        program_options
        REQUIRED)

set(CMAKE_EXE_LINKER_FLAGS "-static-libgcc -static-libstdc++")

# includes
include(ExternalProject)

# googletest
find_package(Threads REQUIRED)

include_directories(
        ${PROJECT_SOURCE_DIR}/src
        ${PROJECT_BINARY_DIR}
        ${EP_PREFIX}/include/antlr4-runtime
        ${EP_PREFIX}/include)
link_directories(${EP_PREFIX}/lib)

# disable this to quiet warnings in XSDCXX, travis version doesn't support
# c++ 11
add_compile_options(-Wno-deprecated-declarations)

#-----------------------------------------------------------------------------
# Version based on GIT tags
#-----------------------------------------------------------------------------
include(GetGitRevisionDescription)
git_describe(VERSION --tags --dirty=-dirty)

#parse the version information into pieces.
set(version_regex "([0-9]*).([0-9]*).([0-9]*)-?(.*)$")
string(REGEX REPLACE ${version_regex} "\\1" VERSION_MAJOR ${VERSION})
string(REGEX REPLACE ${version_regex} "\\2" VERSION_MINOR ${VERSION})
string(REGEX REPLACE ${version_regex} "\\3" VERSION_PATCH ${VERSION})
string(REGEX REPLACE ${version_regex} "\\4" VERSION_SHA ${VERSION})
set(VERSION_SHORT "${VERSION_MAJOR}.${VERSION_MINOR}.${VERSION_PATCH}")
configure_file(${CMAKE_CURRENT_SOURCE_DIR}/cmake/version.cpp.in
        ${CMAKE_CURRENT_BINARY_DIR}/version.cpp)
set(version_file "${CMAKE_CURRENT_BINARY_DIR}/version.cpp")

#-----------------------------------------------------------------------------
# Google Test
#-----------------------------------------------------------------------------
ExternalProject_Add(googletest
        URL https://github.com/google/googletest/archive/release-1.8.0.zip
        URL_MD5 adfafc8512ab65fd3cf7955ef0100ff5
        TIMEOUT 10
        LOG_CONFIGURE ON
        LOG_BUILD ON
        LOG_INSTALL ON
        LOG_DOWNLOAD ON
        LOG_UPDATE ON
        PREFIX ${EP_PREFIX}
        BUILD_COMMAND make -j4
        CMAKE_ARGS -DCMAKE_INSTALL_PREFIX=${EP_PREFIX}
        )
add_library(gtest STATIC IMPORTED)
add_dependencies(gtest googletest)
set_property(TARGET gtest PROPERTY IMPORTED_LOCATION
        ${EP_PREFIX}/lib/libgtest${CMAKE_STATIC_LIBRARY_SUFFIX})
add_library(gtest_main STATIC IMPORTED)
add_dependencies(gtest_main googletest)
set_property(TARGET gtest_main PROPERTY IMPORTED_LOCATION
        ${EP_PREFIX}/lib/libgtest_main${CMAKE_STATIC_LIBRARY_SUFFIX})

#-----------------------------------------------------------------------------
# Antlr C++ Runtime
#-----------------------------------------------------------------------------
ExternalProject_Add(antlr4cpp
        URL https://github.com/antlr/antlr4/archive/4.7.1.zip
        URL_MD5 2fc44eefafb2af020c918a635673f458
        TIMEOUT 10
        PREFIX ${EP_PREFIX}
        LOG_CONFIGURE ON
        LOG_BUILD ON
        LOG_INSTALL ON
        LOG_DOWNLOAD ON
        LOG_UPDATE ON
        BUILD_COMMAND make -j4
        CONFIGURE_COMMAND ${CMAKE_COMMAND} -DCMAKE_BUILD_TYPE=Debug
        -DWITH_DEMO=OFF
        -DBUILD_SHARED_LIBS=OFF -BUILD_TESTS=OFF
        -DCMAKE_INSTALL_PREFIX:PATH=<INSTALL_DIR>
        -DCMAKE_SOURCE_DIR=<SOURCE_DIR>/runtime/Cpp
        <SOURCE_DIR>/runtime/Cpp
        )
add_library(antlr4-runtime STATIC IMPORTED)
set_property(TARGET antlr4-runtime PROPERTY IMPORTED_LOCATION
        ${EP_PREFIX}/lib/libantlr4-runtime${CMAKE_STATIC_LIBRARY_SUFFIX})
add_dependencies(antlr4-runtime antlr4cpp)

#-----------------------------------------------------------------------------
# Libraries
#-----------------------------------------------------------------------------
add_subdirectory(src/modelica_antlr)
add_subdirectory(src/cymoca_compiler)

#-----------------------------------------------------------------------------
# Applications
#-----------------------------------------------------------------------------
add_subdirectory(apps/cymoca)

#-----------------------------------------------------------------------------
# Tests
#-----------------------------------------------------------------------------
enable_testing()
add_subdirectory(test)

#-----------------------------------------------------------------------------
# CPack
#-----------------------------------------------------------------------------
set(CPACK_PACKAGE_CONTACT james.goppert@gmail.com)
set(CPACK_PACKAGE_VERSION ${VERSION})
set(CPACK_PACKAGE_VERSION_MAJOR ${VERSION_MAJOR})
set(CPACK_PACKAGE_VERSION_MINOR ${VERSION_MINOR})
set(CPACK_PACKAGE_VERSION_PATCH ${VERSION_PATCH})
include(CPack)

# vim: set et fenc=utf-8 ff=unix ft=cmake sts=0 sw=4 ts=4 :
