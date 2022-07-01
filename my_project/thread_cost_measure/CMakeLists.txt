cmake_minimum_required(VERSION 3.10)
project(asio_training)
find_package(Boost REQUIRED)


add_executable(main thread_time.cpp )
add_executable(stdout_time stdout_time.cpp )
list(APPEND set_option_target main  stdout_time)

foreach(target ${set_option_target})
    target_compile_features(${target} PUBLIC cxx_std_17)
    target_compile_options(${target} PUBLIC -O2 -Wall -pedantic -Wall)
    target_include_directories(${target} PRIVATE ${CMAKE_CURRENT_SOURCE_DIR} ${CMAKE_CURRENT_BINARY_DIR}
                                                 ${BOOST_INCLUDE_DIRS} )
    target_link_libraries(${target} rt pthread pthread ${BOOST_LIBRARIES} )
endforeach()