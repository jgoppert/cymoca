#-----------------------------------------------------------------------------
# Test
#-----------------------------------------------------------------------------
add_compile_options(-Wall)
add_executable(test_one
    test_one.cpp)
target_link_libraries(test_one
    cymoca_compiler
    gtest_main
    gtest)
add_test(test_one test_one)
add_dependencies(test_one googletest)
set_tests_properties(test_one PROPERTIES TIMEOUT 5)

add_test(cli ${CMAKE_BINARY_DIR}/apps/cymoca/cymoca -h)
set_tests_properties(cli PROPERTIES TIMEOUT 5)

# vim: set et fenc=utf-8 ff=unix ft=cmake sts=0 sw=4 ts=4 :
