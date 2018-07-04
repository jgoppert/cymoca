#-----------------------------------------------------------------------------
# Test
#-----------------------------------------------------------------------------

# instrument code for coverage
if (WITH_COVERAGE)
    add_compile_options(--coverage)
    link_libraries(--coverage)
endif()

include_directories(SYSTEM
        ${PROJECT_BINARY_DIR}
        ${Antlr4Runtime_INCLUDE_DIRS}
        ${EP_PREFIX}/include)

include_directories(
        ${PROJECT_SOURCE_DIR}/src)

add_compile_options(-pedantic -Werror -Weffc++ -Wall -Wextra -Wno-unused-parameter)

# test parse
add_executable(test_parse test_parse.cpp)
target_link_libraries(test_parse cymoca_compiler gtest_main gtest)
add_test(test_parse test_parse)

# test ast
add_executable(test_ast test_ast.cpp)
target_link_libraries(test_ast cymoca_compiler gtest_main gtest)
add_test(test_ast test_ast)

# keep track of test targets so ctest builds what is required before running
set(test_deps cymoca cymoca_compiler test_parse test_ast)

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
    list(APPEND test_deps test_casadi)
endif()

# cli
add_test(cli_help ${CMAKE_BINARY_DIR}/apps/cymoca/cymoca -h)
add_test(cli_compile ${CMAKE_BINARY_DIR}/apps/cymoca/cymoca ${CMAKE_CURRENT_SOURCE_DIR}/models/Simple.mo)

# ctest
add_custom_target(run_tests
    COMMAND ${CMAKE_CTEST_COMMAND}
    DEPENDS ${test_deps})
set(check_deps run_tests)

# coverage with lcov
if (WITH_COVERAGE)
    add_custom_target(coverage_html
        COMMAND lcov --capture --directory . --output-file coverage.info
        COMMAND lcov --remove coverage.info '/usr/*' 'build/*' 'test/*' -o coverage_filtered.info
        COMMAND lcov --summary coverage_filtered.info
        COMMAND genhtml coverage_filtered.info --output-directory out
        COMMAND x-www-browser out/index.html
        WORKING_DIRECTORY ${CMAKE_BUILD_DIR}
        DEPENDS run_tests
        )
endif()

# check for CI
add_custom_target(check
    DEPENDS ${check_deps})

# vim: set et fenc=utf-8 ff=unix ft=cmake sts=0 sw=4 ts=4 :
