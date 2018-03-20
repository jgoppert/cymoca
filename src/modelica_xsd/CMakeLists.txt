#-----------------------------------------------------------------------------
# Generated XML AST/binding
#-----------------------------------------------------------------------------
find_package(XercesC 3 REQUIRED)
find_package(ICU COMPONENTS io le test i18n uc data tu REQUIRED)
find_program(XSDCXX xsdcxx)
if (NOT XSDCXX)
    message(FATAL_ERROR "xsdcxx not found")
endif ()

set(xsd_files
        ${CMAKE_SOURCE_DIR}/third_party/ModelicaXML/schemas/Modelica.xsd
        ${CMAKE_SOURCE_DIR}/third_party/ModelicaXML/schemas/Expression.xsd
        ${CMAKE_SOURCE_DIR}/third_party/ModelicaXML/schemas/Equation_Statement_separate.xsd
        ${CMAKE_SOURCE_DIR}/third_party/ModelicaXML/schemas/Types.xsd
        )
set(xsd_hdrs)
set(xsd_srcs)
set(xsd_dir ${CMAKE_BINARY_DIR}/modelica_xsd)
file(MAKE_DIRECTORY ${xsd_dir})
foreach (xsd_file ${xsd_files})
    get_filename_component(file_name ${xsd_file} NAME_WE)
    set(_hdr ${xsd_dir}/${file_name}.hxx)
    set(_src ${xsd_dir}/${file_name}.cxx)
    list(APPEND xsd_hdrs ${_hdr})
    list(APPEND xsd_srcs ${_src})
    add_custom_command(OUTPUT ${_hdr} ${_src}
            COMMAND ${XSDCXX} cxx-tree
            --generate-serialization
            --type-naming ucc
            --function-naming lcc
            --namespace-map =modelica_xsd
            # std c++11 not supported on version of xsdcxx on travis
            # --std c++11
            ${xsd_file}
            DEPENDS ${xsd_file}
            WORKING_DIRECTORY ${xsd_dir})
endforeach ()
include_directories(${CMAKE_BINARY_DIR})
add_library(modelica_xsd STATIC
        ${xsd_hdrs}
        ${xsd_srcs})
target_link_libraries(modelica_xsd ${XercesC_LIBRARIES} ${ICU_LIBRARIES} dl)
set_source_files_properties(${xsd_hdrs} PROPERTIES GENERATED True)
set_source_files_properties(${xsd_srcs} PROPERTIES GENERATED True)

# vim: set et fenc=utf-8 ff=unix ft=cmake sts=0 sw=4 ts=4 :
