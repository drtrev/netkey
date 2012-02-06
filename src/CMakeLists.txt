cmake_minimum_required(VERSION 2.6)

project( netkey )

# to debug, change the CMAKE_BUILD_TYPE in ccmake to Debug
SET (CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -O2")
MESSAGE( STATUS "CMAKE_CXX_FLAGS: " ${CMAKE_CXX_FLAGS} )

if(WIN32)
  add_definitions ( -D_MSC_VER ) # note the projectname_EXPORTS is defined by default
endif(WIN32)

include_directories( ../include )
link_directories( ../lib )

set( netkey_SRCS main input )

if(WIN32)
  set( netkey_SRCS ${netkey_SRCS} inputWin )
else(WIN32)
  set( netkey_SRCS ${netkey_SRCS} inputLinux )
endif(WIN32)

# TODO change libs for win/lin
set( netkey_LIBS udpraw getopt_basic User32 )

add_executable( netkey ${netkey_SRCS} )
target_link_libraries( netkey ${netkey_LIBS} )

