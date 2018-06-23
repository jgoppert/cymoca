#-----------------------------------------------------------------------------
# Test
#-----------------------------------------------------------------------------
add_compile_options(-Wall)
add_executable(test_one
        test_one.cpp)
target_link_libraries(test_one
        cymoca_compiler
        gtest_main
        gtest
        )
add_test(test_one test_one)

if (WITH_CASADI)
    add_executable(test_casadi
            test_casadi.cpp)
    target_link_libraries(test_casadi
            casadi
            cymoca_compiler
            gtest_main
            gtest
            )
    add_test(test_casadi test_casadi)
endif()


add_test(cli_help ${CMAKE_BINARY_DIR}/apps/cymoca/cymoca -h)
add_test(cli_compile ${CMAKE_BINARY_DIR}/apps/cymoca/cymoca ${CMAKE_CURRENT_SOURCE_DIR}/models/Simple.mo)

# vim: set et fenc=utf-8 ff=unix ft=cmake sts=0 sw=4 ts=4 :
