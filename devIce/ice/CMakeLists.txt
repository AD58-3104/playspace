cmake_minimum_required(VERSION 3.10)
find_package(Ice)
add_executable(a a.cpp)
set(sliced_list ${CMAKE_CURRENT_BINARY_DIR}/"print.h" ${CMAKE_CURRENT_BINARY_DIR}/"print.cpp")
add_custom_command(
    OUTPUT ${sliced_list}  
    COMMAND Ice::slice2cpp
    ARGS ${CMAKE_CURRENT_SOURCE_DIR}/print.ice --output-dir .
    USES_TERMINAL
    # DEPENDS ${CMAKE_CURRENT_SOURCE_DIR}/print.ice
    )

    # add_custom_command(
    #   OUTPUT ${_generated_srcs}
    #   COMMAND  protobuf::protoc
    #   ARGS --${protobuf_generate_LANGUAGE}_out ${_dll_export_decl}${protobuf_generate_PROTOC_OUT_DIR} ${_dll_desc_out} ${_protobuf_include_path} ${_abs_file}
    #   DEPENDS ${_abs_file} protobuf::protoc
    #   COMMENT "Running ${protobuf_generate_LANGUAGE} protocol buffer compiler on ${_proto}"
    #   VERBATIM )