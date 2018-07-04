#-----------------------------------------------------------------------------
# Command Line Interface
#-----------------------------------------------------------------------------
add_executable(cymoca
        cymoca.cpp)
target_link_libraries(cymoca
        cymoca_compiler
        modelica_antlr
        ${Antlr4Runtime_LIBRARIES}
        )

target_compile_options(cymoca PRIVATE -pedantic -Werror -Weffc++ -Wall -Wextra)
target_include_directories(cymoca SYSTEM PRIVATE
        ${PROJECT_SOURCE_DIR}/src
        ${PROJECT_BINARY_DIR}
        ${EP_PREFIX}/include
        ${Antlr4Runtime_INCLUDE_DIRS}
        )

install(TARGETS cymoca
        RUNTIME DESTINATION bin
        LIBRARY DESTINATION lib
        ARCHIVE DESTINATION lib/static)

# vim: set et fenc=utf-8 ff=unix ft=cmake sts=0 sw=4 ts=4 :
