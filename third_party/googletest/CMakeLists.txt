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

# vim: set et fenc=utf-8 ff=unix ft=cmake sts=0 sw=4 ts=4 :
