project(fast_io)
cmake_minimum_required(VERSION 3.20)

add_executable(main main.cpp)
# target_link_libraries(main PRIVATE fast_io)
target_compile_features(main PRIVATE cxx_std_20)