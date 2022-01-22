cmake_minimum_required(VERSION 3.10)
project(zmq)
find_package(cppzmq)

add_executable(main main.cpp)
add_executable(pub a.cpp)
add_executable(sub b.cpp)

list(APPEND set_option_target main pub sub)

foreach(target ${set_option_target})
target_compile_features(${target} PUBLIC cxx_std_17)
target_compile_options(${target} PUBLIC -O2 -Wall -pedantic -Wall)
target_include_directories(
    ${target}
    PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}
            ${CMAKE_CURRENT_BINARY_DIR}
)
target_link_libraries(${target} cppzmq pthread)
endforeach()