#-----------------------------------------------------------------------------
# Command Line Interface
#-----------------------------------------------------------------------------
add_executable(cymoca
        cymoca.cc)
target_link_libraries(cymoca
        cymoca_compiler
        modelica_antlr
        ${Antlr4Runtime_LIBRARIES}
        stdc++fs
        )

target_compile_options(cymoca PRIVATE -pedantic -Werror -Weffc++ -Wall -Wextra)
target_include_directories(cymoca SYSTEM PRIVATE
        ${CYMOCA_SOURCE_DIR}/src
        ${CYMOCA_BINARY_DIR}/src
        ${Antlr4Runtime_INCLUDE_DIRS}
        )

install(TARGETS cymoca
        RUNTIME DESTINATION bin
        LIBRARY DESTINATION lib
        ARCHIVE DESTINATION lib/static)
