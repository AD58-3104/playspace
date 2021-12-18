cmake_minimum_required(VERSION 3.10)
project(aaa CXX )

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# file(GLOB_RECURSE protobuf_list ${CMAKE_CURRENT_SOURCE_DIR}/../new_infoshare/infoshare.pb*)

message("print!!")
foreach(f ${protobuf_list})
message(${f})
endforeach()

include_directories(
    # ${CMAKE_CURRENT_SOURCE_DIR}/../new_infoshare
    # ${CMAKE_CURRENT_SOURCE_DIR}/../new_infoshare/socket
#     ${CMAKE_CURRENT_SOURCE_DIR}/../HPL
#     ${CMAKE_CURRENT_SOURCE_DIR}/../HCServer
    )
add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/../new_infoshare ${CMAKE_CURRENT_BINARY_DIR}/./new_info)
add_library(main STATIC a.cpp)
target_link_directories(main PUBLIC ${CMAKE_CURRENT_BINARY_DIR}/new_info)
target_include_directories(
    main
    PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}
    ${BOOST_INCLUDE_DIRS}
    ${Boost_INCLUDE_DIR}
    ${CMAKE_CURRENT_SOURCE_DIR}/../HPL
    ${CMAKE_CURRENT_SOURCE_DIR}/../HCServer
    ${CMAKE_CURRENT_BINARY_DIR}/new_info
    )

    target_include_directories(
        main 
        PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}/../new_infoshare
        ${CMAKE_CURRENT_SOURCE_DIR}/../new_infoshare/socket
    )