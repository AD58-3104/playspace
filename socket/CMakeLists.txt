cmake_minimum_required(VERSION 3.10)
project(socket)

set(Boost_USE_STATIC_LIBS OFF)
set(Boost_USE_MULTITHREADED ON)
SET(CMAKE_INSTALL_PREFIX ${CMAKE_CURRENT_SOURCE_DIR})

find_package(Boost REQUIRED COMPONENTS thread system date_time Asio)
# find_package(Protobuf REQUIRED)

add_subdirectory(proto)

include_directories(${Boost_INCLUDE_DIRS}/boost)

link_directories(${Boost_LIBRARY_DIR})

add_executable(client boostsocketclient.cpp)
add_executable(server boostsocketserver.cpp)