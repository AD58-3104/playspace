cmake_minimum_required(VERSION 3.10)
# これは一番最初に書く必要があるっぽい。
if(DEFINED ENV{VCPKG_ROOT} AND WIN32)
    message("vcpkg is found !!!")
    message("use toolchains in $ENV{VCPKG_ROOT}/scripts/buildsystems/vcpkg.cmake ")
    set(VCPKG_PATH $ENV{VCPKG_ROOT})
    string(REPLACE "\\" "/" VCPKG_PATH "${VCPKG_PATH}")
    message("${VCPKG_PATH}/scripts/buildsystems/vcpkg.cmake")
    set(VCPKG_TARGET_TRIPLET "x64-windows" CACHE STRING "VCPKG Target Triplet to use")
    set(CMAKE_TOOLCHAIN_FILE "${VCPKG_PATH}/scripts/buildsystems/vcpkg.cmake" CACHE STRING "Vcpkg toolchain file")
    set(CMAKE_CXX_MP_FLAG ON)
    set(CMAKE_CXX_MP_NUM_PROCESSORS 16)
endif()

project(zmq)



find_package(cppzmq)
find_package(Boost)

if(NOT WIN32)
    add_executable(lcl thread_local.cpp)
    add_executable(repreq rep_req.cpp)
    list(APPEND set_option_target lcl repreq)
endif()
add_executable(main main.cpp)
add_executable(pub a.cpp)
add_executable(sub b.cpp)
add_executable(pub2 pub2.cpp)
add_executable(simple_pub simple_pub.cpp)
add_executable(simple_sub simple_sub.cpp)

list(APPEND set_option_target main pub sub pub2 simple_pub simple_sub)

foreach(target ${set_option_target})
    target_compile_features(${target} PUBLIC cxx_std_17)
    target_include_directories(${target} PRIVATE ${CMAKE_CURRENT_SOURCE_DIR} ${CMAKE_CURRENT_BINARY_DIR} ${BOOST_INCLUDE_DIRS})
    if(NOT WIN32)
        target_compile_options(${target} PUBLIC -O2 -Wall -pedantic -Wall)
        target_link_libraries(${target} cppzmq pthread ${BOOST_LIBRARIES})
    endif(NOT WIN32)
    target_link_libraries(${target} cppzmq ${BOOST_LIBRARIES})
endforeach()
