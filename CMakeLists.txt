cmake_minimum_required(VERSION 3.9)
project(cymoca C CXX)

if (NOT CMAKE_BUILD_TYPE)
    message(STATUS "Setting build type to 'Release' as none was specified.")
    set(CMAKE_BUILD_TYPE "Release" CACHE
        STRING "Choose the type of build." FORCE)
    # Set the possible values of build type for cmake-gui
    set_property(CACHE CMAKE_BUILD_TYPE PROPERTY STRINGS
            "Debug" "Release" "MinSizeRel" "RelWithDebInfo")
endif ()

option(WITH_CASADI "enable casadi" OFF)
option(WITH_TEST "enable testing" OFF)
option(WITH_COVERAGE "enable coverage" OFF)
option(WITH_STATIC "build for static linking" ON)
set(options WITH_CASADI WITH_TEST WITH_COVERAGE WITH_STATIC)

message(STATUS "options:")
foreach(option ${options})
message(STATUS "\t${option}: ${${option}}")
endforeach()

#-----------------------------------------------------------------------------
# Setup
#-----------------------------------------------------------------------------

# instrument code for coverage
if (WITH_COVERAGE)
    add_compile_options(--coverage)
    link_libraries(--coverage)
endif()

# support multi-build like catkin by avoiding CMAKE_PROJECT
set(CYMOCA_SOURCE_DIR ${CMAKE_CURRENT_SOURCE_DIR})
set(CYMOCA_BINARY_DIR ${CMAKE_CURRENT_BINARY_DIR})

set(CMAKE_POSITION_INDEPENDENT_CODE ON)
set(CMAKE_MODULE_PATH ${CYMOCA_SOURCE_DIR}/cmake ${CMAKE_MODULE_PATH})
set(CMAKE_CXX_STANDARD 17)

if (WITH_STATIC)
    # prefer static libraries
    if (WIN32)
        SET(CMAKE_FIND_LIBRARY_SUFFIXES .lib .a ${CMAKE_FIND_LIBRARY_SUFFIXES})
    else (WIN32)
        SET(CMAKE_FIND_LIBRARY_SUFFIXES .a ${CMAKE_FIND_LIBRARY_SUFFIXES})
    endif (WIN32)

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
        ${CYMOCA_BINARY_DIR}/compile_commands.json ${CYMOCA_SOURCE_DIR}
        DEPENDS cymoca)

# format
add_custom_target(format
        COMMAND ./scripts/format.sh 1
        WORKING_DIRECTORY ${CYMOCA_SOURCE_DIR}
        )

# packages
find_package(Antlr4Runtime REQUIRED)
if (WITH_CASADI)
  find_package(Casadi REQUIRED)
endif()

# common flags
add_compile_options(-Wfatal-errors)

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
configure_file(${CYMOCA_SOURCE_DIR}/cmake/version.cc.in
    ${CYMOCA_BINARY_DIR}/src/cymoca/version.cc)
set(version_file "${CYMOCA_BINARY_DIR}/src/cymoca/version.cc")

#-----------------------------------------------------------------------------
# Libraries
#-----------------------------------------------------------------------------
add_subdirectory(src/modelica_antlr)
add_subdirectory(src/cymoca)

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
