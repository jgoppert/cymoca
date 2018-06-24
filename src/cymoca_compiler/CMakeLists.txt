#-----------------------------------------------------------------------------
# Compiler
#-----------------------------------------------------------------------------

add_library(cymoca_compiler STATIC
        ${PROJECT_BINARY_DIR}/version.cpp
        version.h
        Compiler.cpp
        Compiler.h
        Ast.cpp
        Ast.h
        listener/Lisp.h
        listener/Lisp.cpp
        listener/WhenExpander.h
        listener/WhenExpander.cpp
        listener/ClassFlattener.h
        listener/ClassFlattener.cpp
        listener/AlgorithmEvaluator.h
        listener/AlgorithmEvaluator.cpp
        listener/ConnectionExpander.h
        listener/ConnectionExpander.cpp
        listener/PreNamer.h
        listener/PreNamer.cpp
        )

target_include_directories(cymoca_compiler SYSTEM PRIVATE
        ${PROJECT_BINARY_DIR}
        ${EP_PREFIX}/include/antlr4-runtime
        ${EP_PREFIX}/include)

target_include_directories(cymoca_compiler PRIVATE
        ${PROJECT_SOURCE_DIR}/src)

target_link_libraries(cymoca_compiler
        antlr4-runtime
        modelica_antlr
        ${Boost_LIBRARIES}
        ${CMAKE_THREAD_LIBS_INIT})

target_compile_options(cymoca_compiler PRIVATE -Wno-unused-parameter -pedantic -Werror -Weffc++ -Wall -Wextra)

# vim: set et fenc=utf-8 ff=unix ft=cmake sts=0 sw=4 ts=4 :
