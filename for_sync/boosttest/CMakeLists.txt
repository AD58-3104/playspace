cmake_minimum_required(VERSION 3.20)
project(boostTest)
find_package(Boost 1.65 REQUIRED system unit_test_framework)
# message(${Boost_LIBRARIES})
include_directories(tester ${Boost_INCLUDE_DIRS})
add_executable(tester tester.cpp func.hpp)
target_link_libraries(tester ${Boost_LIBRARIES} -lrt)