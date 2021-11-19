cmake_minimum_required(VERSION 3.10)
find_package(Boost REQUIRED COMPONENT date_time asio thread)
add_executable(listen socket.hpp listen.cpp)
add_executable(send socket.hpp send.cpp)