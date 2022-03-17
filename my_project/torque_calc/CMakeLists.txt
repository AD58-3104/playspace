cmake_minimum_required(VERSION 3.10)
project(inverse_kine)

find_package(Eigen3 REQUIRED)

add_executable(main main.cpp)
list(APPEND target_list main)

foreach(target ${target_list})
    target_compile_options(${target} PRIVATE -Wall -Wpedantic -std=c++17)
    target_link_libraries(${target} Eigen3::Eigen m)
    target_compile_options(${target} PUBLIC -Wpedantic)
    target_include_directories(${target} PUBLIC ${EIGEN3_INCLUDE_DIRS} ${WEBOTS_INCLUDE_DIRS})
    target_include_directories(${target} PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
endforeach()
