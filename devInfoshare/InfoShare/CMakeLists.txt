cmake_minimum_required(VERSION 3.10)
project(Infoshare)
find_package(Protobuf REQUIRED)
find_package(Boost 1.65.1 REQUIRED thread system date_time)
set(Boost_USE_MULTITHREAD ON)

option(NORMAL "nomal build" ON)
option(PROTO_DBG "proto debug build" OFF)

protobuf_generate_cpp(PROTO_SRCS PROTO_HDRS infoshare.proto)

set(InfoShareSrc test/main.cpp infoshare.cpp infoshare.h hajime_command.h try/sUDPSocket.hpp)

if(NORMAL)
    add_executable(main ${InfoShareSrc} ${PROTO_SRCS} ${PROTO_HDRS})
    add_executable(sender test/sender.cpp ${PROTO_SRCS} ${PROTO_HDRS})
    add_executable(receiver test/receiver.cpp ${PROTO_SRCS} ${PROTO_HDRS})
    add_executable(test_dict test/check_dict_class.cpp infoshare.cpp  infoshare.h ${PROTO_SRCS} ${PROTO_HDRS})
    target_compile_definitions(main PRIVATE "INFOSHARE_DEBUG=1")
    set(set_option_target main sender receiver test_dict)

    foreach(target ${set_option_target})
        target_compile_features(${target} PUBLIC cxx_std_17)
        target_compile_options(${target} PUBLIC -O2 -Wall -pedantic -Wall)
        target_include_directories(
            ${target} PRIVATE ${CMAKE_CURRENT_SOURCE_DIR} ${CMAKE_CURRENT_SOURCE_DIR}/try ${Protobuf_INCLUDE_DIRS}
                              ${Boost_INCLUDE_DIRS} ${CMAKE_CURRENT_BINARY_DIR})
        target_link_libraries(${target} ${Boost_LIBRARIES} -lrt protobuf::libprotobuf)
    endforeach()

elseif(PROTO_DBG)
    add_executable(protodebug test/proto.cpp ${PROTO_SRCS} ${PROTO_HDRS})
    target_compile_features(protodebug PUBLIC cxx_std_17)
    target_compile_options(protodebug PUBLIC -O2 -Wall -pedantic -Wall)
    target_include_directories(protodebug PRIVATE ${CMAKE_CURRENT_BINARY_DIR})
    # target_include_directories(protodebug PRIVATE ../${CMAKE_CURRENT_BINARY_DIR})
    target_include_directories(protodebug PRIVATE ${Protobuf_INCLUDE_DIRS})
    target_link_libraries(protodebug -lrt protobuf::libprotobuf)
endif()
