cmake_minimum_required(VERSION 3.10)
project(proto)

find_package(Protobuf REQUIRED)
find_package(Boost REQUIRED system date_time)
include_directories(${Protobuf_INCLUDE_DIRS} ${Boost_INCLUDE_DIRS})
include_directories(${CMAKE_CURRENT_BINARY_DIR})
protobuf_generate_cpp(PROTO_SRCS PROTO_HDRS data.proto)
add_executable(main main.cpp protobuf_msg_worker.hpp ${PROTO_SRCS} ${PROTO_HDRS} )

add_executable(a a.cpp)
target_compile_features(main PRIVATE cxx_std_17)
target_link_libraries(main ${Protobuf_LIBRARIES} ${Boost_LIBRARIES})

target_compile_features(a PRIVATE cxx_std_17)
target_link_libraries(a ${Protobuf_LIBRARIES} ${Boost_LIBRARIES})