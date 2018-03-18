#-----------------------------------------------------------------------------
# Command Line Interface
#-----------------------------------------------------------------------------
add_executable(cymoca
    cymoca.cpp)
target_link_libraries(cymoca
    cymoca_compiler
    antlr4-runtime
    modelica_antlr
    modelica_xsd
    ${XercesC_LIBRARIES}
    ${Boost_LIBRARIES})

install(TARGETS cymoca
    RUNTIME DESTINATION bin
    LIBRARY DESTINATION lib
    ARCHIVE DESTINATION lib/static)

# vim: set et fenc=utf-8 ff=unix ft=cmake sts=0 sw=4 ts=4 :
