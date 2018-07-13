#-----------------------------------------------------------------------------
# Generated Antlr Parser
#-----------------------------------------------------------------------------

# antlr
find_program(ANTLR4_JAR NAMES antlr-4.7.1-complete.jar HINTS ${PROJECT_SOURCE_DIR}/java)
if (NOT ANTLR4_JAR)
    message(FATAL_ERROR "antlr4_jar not found: ${ANTLR4_JAR}")
endif ()
find_package(Java COMPONENTS Runtime REQUIRED)

set(antlr_grammar ${CMAKE_CURRENT_SOURCE_DIR}/Modelica.g4)
set(antlr_srcs
        ${CMAKE_CURRENT_BINARY_DIR}/ModelicaBaseListener.cpp
        ${CMAKE_CURRENT_BINARY_DIR}/ModelicaLexer.cpp
        ${CMAKE_CURRENT_BINARY_DIR}/ModelicaListener.cpp
        ${CMAKE_CURRENT_BINARY_DIR}/ModelicaParser.cpp
        )
set(antlr_hdrs
        ${CMAKE_CURRENT_BINARY_DIR}/ModelicaBaseListener.h
        ${CMAKE_CURRENT_BINARY_DIR}/ModelicaLexer.h
        ${CMAKE_CURRENT_BINARY_DIR}/ModelicaListener.h
        ${CMAKE_CURRENT_BINARY_DIR}/ModelicaParser.h
        )
add_custom_command(OUTPUT ${antlr_srcs} ${antlr_hdrs}
        COMMAND "${Java_JAVA_EXECUTABLE}" -jar "${ANTLR4_JAR}" -Werror
        -Dlanguage=Cpp -listener -o "."
        -package modelica_antlr "${antlr_grammar}"
        DEPENDS ${antlr_grammar})
add_library(modelica_antlr STATIC
        ${antlr_hdrs}
        ${antlr_srcs}
        )
target_include_directories(modelica_antlr SYSTEM PUBLIC ${ANTLR4RUNTIME_INCLUDE_DIRS})
target_compile_options(modelica_antlr PRIVATE -Wno-attributes)
target_link_libraries(modelica_antlr PUBLIC ${ANTLR4RUNTIME_LIBRARIES})
