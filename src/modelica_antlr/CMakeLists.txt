#-----------------------------------------------------------------------------
# Generated Antlr Parser
#-----------------------------------------------------------------------------

# antlr
find_program(ANTLR4_JAR NAMES antlr-4.7.1-complete.jar HINTS ${PROJECT_SOURCE_DIR}/java)
if (NOT ANTLR4_JAR)
    message(FATAL_ERROR "antlr4_jar not found: ${ANTLR4_JAR}")
endif()
find_package(Java COMPONENTS Runtime REQUIRED)

set(antlr_dir ${PROJECT_BINARY_DIR}/modelica_antlr)
set(antlr_parser ${PROJECT_SOURCE_DIR}/src/cymoca/Modelica.g4)
set(antlr_lexer ${PROJECT_SOURCE_DIR}/src/cymoca/Modelica.g4)
file(MAKE_DIRECTORY ${antlr_dir})
set(antlr_srcs
    ${antlr_dir}/ModelicaBaseListener.cpp
    ${antlr_dir}/ModelicaLexer.cpp
    ${antlr_dir}/ModelicaListener.cpp
    ${antlr_dir}/ModelicaParser.cpp
    )
set_source_files_properties(${antlr_srcs} PROPERTIES GENERATED True)
set(antlr_hdrs
    ${antlr_dir}/ModelicaBaseListener.h
    ${antlr_dir}/ModelicaLexer.h
    ${antlr_dir}/ModelicaListener.h
    ${antlr_dir}/ModelicaParser.h
    )
set_source_files_properties(${antlr_hdrs} PROPERTIES GENERATED True)
add_custom_command(OUTPUT ${antlr_srcs} ${antlr_hdrs}
        COMMAND ${CMAKE_COMMAND} -E make_directory ${antlr_dir}
        COMMAND "${Java_JAVA_EXECUTABLE}" -jar "${ANTLR4_JAR}" -Werror
            -Dlanguage=Cpp -listener -o "${antlr_dir}"
            -package modelica_antlr "${antlr_lexer}" "${antlr_parser}"
            WORKING_DIRECTORY "${PROJECT_BINARY_DIR}"
        DEPENDS ${antlr_parser} ${antlr_lexer})
include_directories(${EP_PREFIX}/include/antlr4-runtime)
add_library(modelica_antlr STATIC
    ${antlr_srcs}
    ${antlr_hdrs}
    )
add_dependencies(modelica_antlr antlr4cpp)
target_link_libraries(modelica_antlr antlr4-runtime)

# vim: set et fenc=utf-8 ff=unix ft=cmake sts=0 sw=4 ts=4 :
