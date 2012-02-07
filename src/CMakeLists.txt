cmake_minimum_required(VERSION 2.6)

project( netkey )

# to debug, change the CMAKE_BUILD_TYPE in ccmake to Debug
SET (CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -O2")
MESSAGE( STATUS "CMAKE_CXX_FLAGS: " ${CMAKE_CXX_FLAGS} )


set( netkey_SRCS main input )
set( netkey_LIBS udpraw getopt_basic )
set( netkey_INCLUDEDIRS ../include ../../motlab/getopt_basic/trunk/include ../../motlab/udpraw/trunk/include )

if(WIN32)
  add_definitions ( -D_MSC_VER ) # note the projectname_EXPORTS is defined by default
  link_directories( ../libwin )
  set( netkey_SRCS ${netkey_SRCS} inputWin )
  set( netkey_LIBS ${netkey_LIBS} User32 )
  set( netkey_INCLUDEDIRS ${netkey_INCLUDEDIRS} ../hackinclude )
else(WIN32)
  link_directories( ../lib )
  set( netkey_SRCS ${netkey_SRCS} inputLinux )
endif(WIN32)

include_directories ( ${netkey_INCLUDEDIRS} )

add_executable( netkey ${netkey_SRCS} )
target_link_libraries( netkey ${netkey_LIBS} )

