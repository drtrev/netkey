cmake_minimum_required(VERSION 2.6)

project( netkey )

# to debug, change the CMAKE_BUILD_TYPE in ccmake to Debug
SET (CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -O2")
MESSAGE( STATUS "CMAKE_CXX_FLAGS: " ${CMAKE_CXX_FLAGS} )

include_directories( ../include )
link_directories( ../lib )

set( netkey_SRCS main )
set( netkey_LIBS udpraw getopt_basic )

add_executable( netkey ${netkey_SRCS} )
target_link_libraries( netkey ${netkey_LIBS} )

