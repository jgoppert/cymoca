#-----------------------------------------------------------------------------
# Test
#-----------------------------------------------------------------------------

include_directories(SYSTEM
        ${PROJECT_BINARY_DIR}
        ${EP_PREFIX}/include/antlr4-runtime
        ${EP_PREFIX}/include)

include_directories(
        ${PROJECT_SOURCE_DIR}/src)

add_compile_options(-Wno-unused-parameter -pedantic -Werror -Weffc++ -Wall -Wextra)

# test parse
add_executable(test_parse test_parse.cpp)
target_link_libraries(test_parse cymoca_compiler gtest_main gtest)
add_test(test_parse test_parse)

# test ast
add_executable(test_ast test_ast.cpp)
target_link_libraries(test_ast cymoca_compiler gtest_main gtest)
add_test(test_ast test_ast)

# test casadi
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

# cli
add_test(cli_help ${CMAKE_BINARY_DIR}/apps/cymoca/cymoca -h)
add_test(cli_compile ${CMAKE_BINARY_DIR}/apps/cymoca/cymoca ${CMAKE_CURRENT_SOURCE_DIR}/models/Simple.mo)

# vim: set et fenc=utf-8 ff=unix ft=cmake sts=0 sw=4 ts=4 :
