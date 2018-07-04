#-----------------------------------------------------------------------------
# Compiler
#-----------------------------------------------------------------------------

include_directories(SYSTEM
        ${PROJECT_BINARY_DIR}
        ${Antlr4Runtime_INCLUDE_DIRS}
        ${EP_PREFIX}/include)

include_directories(
        ${PROJECT_SOURCE_DIR}/src)

add_compile_options(-pedantic -Werror -Weffc++ -Wall -Wextra -Wno-unused-parameter)

add_subdirectory(ast)

add_library(cymoca_compiler STATIC
        ${PROJECT_BINARY_DIR}/version.cpp
        version.h
        util.h
        Compiler.cpp
        Compiler.h
        listener/LispPrinter.h
        listener/WhenExpander.h
        listener/Flattener.h
        )

target_link_libraries(cymoca_compiler
        ${Antlr4Runtime_LIBRARIES}
        modelica_antlr
        cymoca_ast
        ${Boost_LIBRARIES}
        ${CMAKE_THREAD_LIBS_INIT})


# vim: set et fenc=utf-8 ff=unix ft=cmake sts=0 sw=4 ts=4 :
