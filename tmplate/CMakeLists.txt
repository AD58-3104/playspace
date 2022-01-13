include(myfunction.cmake)
list(
    APPEND
    aaa
    "imas"
    "786"
    "876"
    "765"
    "346"
    "283")
list_out("${aaa}")

add_executable(main a.cpp)
set_property(
    TARGET main
    APPEND_STRING
    PROPERTY LINK_FLAGS " -fuse-ld=lld")
# set_target_lld(main)
