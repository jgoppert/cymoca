cmake_minimum_required(VERSION 3.9)

project(cymoca C CXX)

if (NOT CMAKE_BUILD_TYPE)
    message(STATUS "Setting build type to 'DEBUG' as none was specified.")
    set(CMAKE_BUILD_TYPE "Debug" CACHE
        STRING "Choose the type of build." FORCE)
    # Set the possible values of build type for cmake-gui
    set_property(CACHE CMAKE_BUILD_TYPE PROPERTY STRINGS
            "Debug" "Release" "MinSizeRel" "RelWithDebInfo")
endif ()

option(WITH_CASADI "enable casadi" OFF)
option(WITH_TEST "enable testing" OFF)
option(WITH_COVERAGE "enable coverage" OFF)
option(WITH_STATIC "build for static linking" OFF)

#-----------------------------------------------------------------------------
# Setup
#-----------------------------------------------------------------------------

set(CMAKE_MODULE_PATH ${PROJECT_SOURCE_DIR}/cmake ${CMAKE_MODULE_PATH})
set(CMAKE_CXX_STANDARD 17)

if (WITH_STATIC)
    # find only static libraries
    if (WIN32)
        SET(CMAKE_FIND_LIBRARY_SUFFIXES .lib .a ${CMAKE_FIND_LIBRARY_SUFFIXES})
    else (WIN32)
        SET(CMAKE_FIND_LIBRARY_SUFFIXES .a ${CMAKE_FIND_LIBRARY_SUFFIXES})
    endif (WIN32)
    set(Boost_USE_STATIC_LIBS ON)

    # set static linking
    if ("${CMAKE_CXX_COMPILER_ID}" MATCHES "GCC")
        set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -static-libgcc -static-libstdc++")
    endif()
endif()

# handle clang libc++
if ("${CMAKE_CXX_COMPILER_ID}" MATCHES "Clang")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -stdlib=libc++")
endif()

# copy compile commands to source directory for auto-completion with vim etc.
set(CMAKE_EXPORT_COMPILE_COMMANDS ON CACHE BOOL
        "export compile commands, useful for vim completion" FORCE)
add_custom_target(copy_compile_commands ALL
        COMMAND ${CMAKE_COMMAND} -E copy
        ${PROJECT_BINARY_DIR}/compile_commands.json ${PROJECT_SOURCE_DIR}
        DEPENDS cymoca)

# packages
find_package(Boost COMPONENTS
    filesystem
    program_options
    REQUIRED)
find_package(Antlr4Runtime REQUIRED)

#-----------------------------------------------------------------------------
# Version based on GIT tags
#-----------------------------------------------------------------------------
include(GetGitRevisionDescription)
git_describe(VERSION --tags --dirty=-dirty)
message(STATUS "Version: ${VERSION}")

#parse the version information into pieces.
set(version_regex "([0-9]*).([0-9]*).([0-9]*)-?(.*)$")
string(REGEX REPLACE ${version_regex} "\\1" VERSION_MAJOR ${VERSION})
string(REGEX REPLACE ${version_regex} "\\2" VERSION_MINOR ${VERSION})
string(REGEX REPLACE ${version_regex} "\\3" VERSION_PATCH ${VERSION})
string(REGEX REPLACE ${version_regex} "\\4" VERSION_SHA ${VERSION})
set(VERSION_SHORT "${VERSION_MAJOR}.${VERSION_MINOR}.${VERSION_PATCH}")
configure_file(${PROJECT_SOURCE_DIR}/cmake/version.cpp.in
        ${PROJECT_BINARY_DIR}/version.cpp)
set(version_file "${PROJECT_BINARY_DIR}/version.cpp")

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
if (WITH_TEST)
    enable_testing()
    add_subdirectory(test)
endif ()

#-----------------------------------------------------------------------------
# CPack
#-----------------------------------------------------------------------------
set(CPACK_PACKAGE_CONTACT james.goppert@gmail.com)
set(CPACK_PACKAGE_VERSION ${VERSION})
include(CPack)

# vim: set et fenc=utf-8 ff=unix ft=cmake sts=0 sw=4 ts=4 :
