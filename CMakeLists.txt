cmake_minimum_required(VERSION 3.5)

project(cymoca C CXX)

#-----------------------------------------------------------------------------
# Setup
#-----------------------------------------------------------------------------
set(CMAKE_CXX_STANDARD 11)
set(EP_PREFIX ${CMAKE_BINARY_DIR}/deps)
set_property(DIRECTORY PROPERTY EP_PREFIX ${EP_PREFIX})

# packages
find_package(Boost COMPONENTS filesystem REQUIRED)

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
# External Projects
#-----------------------------------------------------------------------------
add_subdirectory(third_party/googletest)
add_subdirectory(third_party/antlr4cpp)

#-----------------------------------------------------------------------------
# Libraries
#-----------------------------------------------------------------------------
add_subdirectory(src/modelica_antlr)
add_subdirectory(src/modelica_xsd)
add_subdirectory(src/cymoca)

#-----------------------------------------------------------------------------
# Applications
#-----------------------------------------------------------------------------
add_subdirectory(apps/cli)

#-----------------------------------------------------------------------------
# Tests
#-----------------------------------------------------------------------------
enable_testing()
add_subdirectory(test)

#-----------------------------------------------------------------------------
# CPack
#-----------------------------------------------------------------------------
set(CPACK_PACKAGE_CONTACT james.goppert@gmail.com)
include(CPack)

# vim: set et fenc=utf-8 ff=unix ft=cmake sts=0 sw=4 ts=4 :
