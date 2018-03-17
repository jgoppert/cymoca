#-----------------------------------------------------------------------------
# Compiler
#-----------------------------------------------------------------------------
add_library(cymoca STATIC
    Compiler.cpp
    Compiler.h)
target_link_libraries(cymoca
    antlr4-runtime
    modelica_antlr
    modelica_xsd
    gtest
    gtest_main
    ${XercesC_LIBRARIES}
    ${Boost_LIBRARIES}
    ${CMAKE_THREAD_LIBS_INIT})

# vim: set et fenc=utf-8 ff=unix ft=cmake sts=0 sw=4 ts=4 :
