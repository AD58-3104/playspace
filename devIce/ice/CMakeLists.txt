cmake_minimum_required(VERSION 3.10)
find_package(Ice)
add_executable(a.cpp)
set(sliced_list )
add_custom_command(
    OUTPUT ${sliced_list} PRE_BUILD 
    COMMAND slice2cpp
    ARGS --output-dir .
    TARGET print.ice 
    )

