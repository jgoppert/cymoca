#-----------------------------------------------------------------------------
# Compiler
#-----------------------------------------------------------------------------

include_directories(SYSTEM
        ${PROJECT_BINARY_DIR}
        ${EP_PREFIX}/include/antlr4-runtime
        ${EP_PREFIX}/include)

include_directories(
        ${PROJECT_SOURCE_DIR}/src)

add_compile_options(-Wno-unused-parameter -pedantic -Werror -Weffc++ -Wall -Wextra)

add_subdirectory(ast)

add_library(cymoca_compiler STATIC
        ${PROJECT_BINARY_DIR}/version.cpp
        version.h
        Compiler.cpp
        Compiler.h
        listener/LispPrinter.h
        )

target_link_libraries(cymoca_compiler
        antlr4-runtime
        modelica_antlr
        cymoca_ast
        ${Boost_LIBRARIES}
        ${CMAKE_THREAD_LIBS_INIT})


# vim: set et fenc=utf-8 ff=unix ft=cmake sts=0 sw=4 ts=4 :
