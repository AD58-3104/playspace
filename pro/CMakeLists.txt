cmake_minimum_required(VERSION 3.10)
project(prototry)
SET(CMAKE_INSTALL_PREFIX ${CMAKE_CURRENT_SOURCE_DIR})

add_subdirectory(proto)
add_subdirectory(rece)
add_subdirectory(send)