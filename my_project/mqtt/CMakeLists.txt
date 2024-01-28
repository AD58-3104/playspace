
find_package(Threads REQUIRED)
find_package(PahoMqttCpp REQUIRED)

# # include directories
# include_directories(${CMAKE_CURRENT_SOURCE_DIR})

# The sample applications
set(EXECUTABLES
    send
)


# # Build the samples
foreach(EXECUTABLE ${EXECUTABLES})
    add_executable(${EXECUTABLE} ${EXECUTABLE}.cpp)
    target_link_libraries(${EXECUTABLE} PahoMqttCpp::paho-mqttpp3)
endforeach()
