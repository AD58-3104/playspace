project(InfoShare)

IF(WIN32)
    set(Boost_USE_STATIC_LIBS        ON)
ELSE(WIN32)
    set(Boost_USE_STATIC_LIBS       OFF)
ENDIF(WIN32)
set(Boost_USE_MULTITHREADED      ON)
set(Boost_USE_STATIC_RUNTIME    OFF)

FIND_PACKAGE(Boost COMPONENTS system thread date_time)

include_directories(
	${BOOST_INCLUDE_DIRS}
	${Boost_INCLUDE_DIR}
	../HPL
	../HCServer
)

link_directories(
	${Boost_LIBRARY_DIRS}
	${Boost_LIBRARY_DIR}
	${ICE_DIR}/lib
)

set (InfoShare_SRCS
	InfoShare.h
	InfoShare.cpp
)

add_library(InfoShareLib
	${InfoShare_SRCS}
)

IF(0) #InfoShareTest
add_executable(InfoShareTest
	InfoShareTest.cpp
)

IF(UNIX)
target_link_libraries(InfoShareTest
	InfoShareLib
	HCLib
)
ELSE(UNIX)
target_link_libraries(InfoShareTest
	InfoShareLib
	HCLib
)
ENDIF(UNIX)
ENDIF(0) #InfoShareTest
