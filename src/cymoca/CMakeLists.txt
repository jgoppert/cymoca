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

target_link_libraries(cymoca_compiler PRIVATE
        modelica_antlr
        ${Antlr4Runtime_LIBRARIES}
        )

target_include_directories(cymoca_compiler SYSTEM PUBLIC
        ${CYMOCA_BINARY_DIR}/src
        ${Antlr4Runtime_INCLUDE_DIRS}
        )

target_include_directories(cymoca_compiler PUBLIC
        ${CYMOCA_SOURCE_DIR}/src
        )

target_compile_options(cymoca_compiler PUBLIC
        -pedantic -Werror -Weffc++ -Wall -Wextra -Wno-unused-parameter)
