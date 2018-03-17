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
            <SOURCE_DIR>/runtime/Cpp
        )
add_library(antlr4-runtime STATIC IMPORTED)
set_property(TARGET antlr4-runtime PROPERTY IMPORTED_LOCATION
    ${EP_PREFIX}/lib/libantlr4-runtime${CMAKE_STATIC_LIBRARY_SUFFIX})
add_dependencies(antlr4-runtime antlr4cpp)

# vim: set et fenc=utf-8 ff=unix ft=cmake sts=0 sw=4 ts=4 :
