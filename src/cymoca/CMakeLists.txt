#-----------------------------------------------------------------------------
# Compiler
#-----------------------------------------------------------------------------

include_directories(SYSTEM
        ${PROJECT_BINARY_DIR}
        ${Antlr4Runtime_INCLUDE_DIRS}
        )

include_directories(
        ${PROJECT_SOURCE_DIR}/src)

add_compile_options(-pedantic -Werror -Weffc++ -Wall -Wextra -Wno-unused-parameter)

add_subdirectory(ast)

add_library(cymoca_compiler STATIC
        ${PROJECT_BINARY_DIR}/version.cc
        version.h
        util.h
        compiler.cc
        compiler.h
        )

target_link_libraries(cymoca_compiler
        ${Antlr4Runtime_LIBRARIES}
        modelica_antlr
        cymoca_ast
        ${CMAKE_THREAD_LIBS_INIT})
