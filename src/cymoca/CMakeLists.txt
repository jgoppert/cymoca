#-----------------------------------------------------------------------------
# Compiler
#-----------------------------------------------------------------------------

add_subdirectory(ast)

add_library(cymoca_compiler STATIC
        ${CMAKE_CURRENT_BINARY_DIR}/version.cc
        compiler.cc
        version.h
        util.h
        compiler.h
        )

target_link_libraries(cymoca_compiler PUBLIC
        modelica_antlr
        )

target_include_directories(cymoca_compiler SYSTEM PUBLIC
        ${CYMOCA_BINARY_DIR}/src
        )

target_include_directories(cymoca_compiler PRIVATE
        ${CYMOCA_SOURCE_DIR}/src
        )

target_compile_options(cymoca_compiler PRIVATE
        -pedantic -Werror -Weffc++ -Wall -Wextra -Wno-unused-parameter)
