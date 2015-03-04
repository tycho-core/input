cmake_minimum_required (VERSION 2.8)

set(link_libs "")

if(ty_platform MATCHES "pc")
	include_directories("${CMAKE_SOURCE_DIR}/3rdparty/pc/dx9")
	link_directories("${CMAKE_SOURCE_DIR}/3rdparty/pc/dx9/lib")
	set(link_libs "xinput")
endif()

tycho_add_library(input "tycore;${link_libs}" "libs")



