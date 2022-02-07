find_package(yaml-cpp)
add_executable(a main.cpp)
target_include_directories(a PUBLIC ${YAML_CPP_INCLUDE_DIRS})
target_link_libraries(a ${YAML_CPP_LIBRARIES})

#message(${YAML_CPP_LIBRARIES})
#message(${YAML_CPP_LIBRARIES})
