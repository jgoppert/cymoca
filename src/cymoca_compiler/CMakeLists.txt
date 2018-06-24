#-----------------------------------------------------------------------------
# Compiler
#-----------------------------------------------------------------------------

include_directories(
        ${PROJECT_SOURCE_DIR}/src
        ${PROJECT_BINARY_DIR}
        ${EP_PREFIX}/include/antlr4-runtime
        ${EP_PREFIX}/include)

add_library(cymoca_compiler STATIC
        ${PROJECT_BINARY_DIR}/version.cpp
        version.h
        Compiler.cpp
        Compiler.h
        Ast.cpp
        Ast.h
        listener/Lisp.h
        listener/WhenExpander.h
        )
target_link_libraries(cymoca_compiler
        antlr4-runtime
        modelica_antlr
        ${Boost_LIBRARIES}
        ${CMAKE_THREAD_LIBS_INIT})

# vim: set et fenc=utf-8 ff=unix ft=cmake sts=0 sw=4 ts=4 :
