#-----------------------------------------------------------------------------
# Command Line Interface
#-----------------------------------------------------------------------------
add_executable(cymoca
        cymoca.cpp)
target_link_libraries(cymoca
        cymoca_compiler
        modelica_antlr
        antlr4-runtime
        ${XercesC_LIBRARIES}
        ${ICU_LIBRARIES})

target_compile_options(cymoca PRIVATE -pedantic -Werror -Weffc++ -Wall -Wextra)
target_include_directories(cymoca SYSTEM PRIVATE
        ${PROJECT_SOURCE_DIR}/src
        ${PROJECT_BINARY_DIR}
        ${EP_PREFIX}/include/antlr4-runtime
        ${EP_PREFIX}/include)

install(TARGETS cymoca
        RUNTIME DESTINATION bin
        LIBRARY DESTINATION lib
        ARCHIVE DESTINATION lib/static)

# vim: set et fenc=utf-8 ff=unix ft=cmake sts=0 sw=4 ts=4 :
