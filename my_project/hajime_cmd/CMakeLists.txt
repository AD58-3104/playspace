cmake_minimum_required(VERSION 3.10)
project(zmq)
find_package(cppzmq)
find_package(Boost)
find_package(Protobuf REQUIRED)

protobuf_generate_cpp(PROTO_SRCS PROTO_HDRS hajime.proto)

add_executable(main main.cpp classes.hpp classes.cpp wrapperclass.hpp ${PROTO_SRCS} ${PROTO_HDRS})
list(APPEND set_option_target main)

foreach(target ${set_option_target})
    target_compile_features(${target} PUBLIC cxx_std_17)
    target_compile_options(${target} PUBLIC -O2 -Wall -pedantic -Wall)
    target_include_directories(${target} PRIVATE ${CMAKE_CURRENT_SOURCE_DIR} ${CMAKE_CURRENT_BINARY_DIR}
                                                 ${BOOST_INCLUDE_DIRS} ${Protobuf_INCLUDE_DIR})
    target_link_libraries(${target} cppzmq pthread ${BOOST_LIBRARIES} ${Protobuf_LIBRARIES})
endforeach()
