cmake_minimum_required(VERSION 3.10)
project(visibility_graph)
find_package(Boost REQUIRED)
find_package(OpenCV REQUIRED)
add_executable(main main.cpp cv.hpp graph.hpp)
add_executable(test test.cpp)

target_include_directories(main PUBLIC ${OpenCV_INCLUDE_DIRS})
target_link_libraries(main ${OpenCV_LIBRARIES})

target_include_directories(test PUBLIC ${OpenCV_INCLUDE_DIRS})
target_link_libraries(test ${OpenCV_LIBRARIES})