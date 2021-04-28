cmake_minimum_required(VERSION 3.10)
project(socket)

set(Boost_USE_STATIC_LIBS OFF)
set(Boost_USE_MULTITHREADED ON)
SET(CMAKE_INSTALL_PREFIX ${CMAKE_CURRENT_SOURCE_DIR})

find_package(Boost REQUIRED COMPONENTS thread system date_time )
find_package(Protobuf REQUIRED)

add_subdirectory(proto)

include_directories(${Boost_INCLUDE_DIRS}/boost)
include_directories(proto /usr/local/include/google/protobuf)

#message(${Boost_INCLUDE_DIRS})
link_directories(${Boost_LIBRARY_DIR})
#message(${Boost_LIBRARY_DIRS})

add_executable(receiver receiver.cpp)
add_executable(sender sender.cpp)

target_link_libraries(receiver ${Boost_LIBRARIES} -lrt protobuf::libprotobuf libpictureproto)
target_link_libraries(sender ${Boost_LIBRARIES} -lrt protobuf::libprotobuf libpictureproto)