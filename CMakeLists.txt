cmake_minimum_required(VERSION 3.5)

project(cymoca C CXX)

#-----------------------------------------------------------------------------
# Setup
#-----------------------------------------------------------------------------
set(CMAKE_MODULE_PATH ${CMAKE_CURRENT_SOURCE_DIR}/cmake ${CMAKE_MODULE_PATH})
set(CMAKE_CXX_STANDARD 11)
set(EP_PREFIX ${CMAKE_BINARY_DIR}/deps)
set_property(DIRECTORY PROPERTY EP_PREFIX ${EP_PREFIX})

# packages
find_package(Boost COMPONENTS
        filesystem
        program_options
        REQUIRED)

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
string(REGEX REPLACE ${version_regex} "\\1" VERSION_MAJOR ${VERSION} )
string(REGEX REPLACE ${version_regex} "\\2" VERSION_MINOR ${VERSION} )
string(REGEX REPLACE ${version_regex} "\\3" VERSION_PATCH ${VERSION} )
string(REGEX REPLACE ${version_regex} "\\4" VERSION_SHA ${VERSION} )
set(VERSION_SHORT "${VERSION_MAJOR}.${VERSION_MINOR}.${VERSION_PATCH}")
configure_file(${CMAKE_CURRENT_SOURCE_DIR}/cmake/version.cpp.in
                ${CMAKE_CURRENT_BINARY_DIR}/version.cpp)
set(version_file "${CMAKE_CURRENT_BINARY_DIR}/version.cpp")

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
add_subdirectory(src/cymoca_compiler)

#-----------------------------------------------------------------------------
# Applications
#-----------------------------------------------------------------------------
add_subdirectory(apps/cymoca)

#----------------------------------------------------------------------------- # Tests
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
