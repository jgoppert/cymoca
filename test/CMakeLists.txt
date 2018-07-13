#-----------------------------------------------------------------------------
# Test
#-----------------------------------------------------------------------------
set(GTEST_ROOT "/usr")
find_package(Threads REQUIRED)
find_package(GTest REQUIRED)
set(TEST_PATH ${CMAKE_CURRENT_SOURCE_DIR})

set(test_libs
    cymoca_compiler
    ${GTEST_BOTH_LIBRARIES}
    ${CMAKE_THREAD_LIBS_INIT}
    stdc++fs
    )

configure_file(test_config.h.in test_config.h)

include_directories(
    ${CMAKE_CURRENT_BINARY_DIR}
    ${CYMOCA_SOURCE_DIR}/src
    )

add_compile_options(-pedantic -Werror -Weffc++ -Wall -Wextra -Wno-unused-parameter)

# test parse
add_executable(test_parse
    test_parse.cc
    )
target_link_libraries(test_parse
    ${test_libs}
    )
add_test(test_parse test_parse)

# test ast
add_executable(test_ast
    test_ast.cc
    )
target_link_libraries(test_ast ${test_libs})
add_test(test_ast test_ast)

# keep track of test targets so ctest builds what is required before running
set(test_deps cymoca cymoca_compiler test_parse test_ast)

# test casadi
if (WITH_CASADI)
    add_executable(test_casadi
        test_casadi.cc)
    message(STATUS "casadi include: ${CASADI_INCLUDE_DIRS}")
    target_include_directories(test_casadi PUBLIC ${CASADI_INCLUDE_DIRS})
    target_link_libraries(test_casadi ${test_libs} ${CASADI_LIBRARIES})
    add_test(test_casadi test_casadi)
    list(APPEND test_deps test_casadi)
endif()

# cli
add_test(cli_help ${CMAKE_BINARY_DIR}/apps/cymoca/cymoca)
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
        COMMAND lcov --remove coverage.info '/usr/*' '*/build/*' '*/test/*' -o coverage_filtered.info
        COMMAND lcov --summary coverage_filtered.info
        COMMAND genhtml coverage_filtered.info --output-directory out
        COMMAND x-www-browser out/index.html
        WORKING_DIRECTORY ${CMAKE_BINARY_DIR}
        DEPENDS run_tests
        )
endif()
# check for CI
add_custom_target(check
    DEPENDS ${check_deps})
