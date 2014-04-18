cmake_minimum_required(VERSION 2.8)
project(InputXSLT)

set(
	CMAKE_CXX_FLAGS
	"-std=c++11 -W -Wall -Wextra -Winline -pedantic"
)

include_directories(
	src/
)

add_executable(
	test
	test.cc
)

target_link_libraries(
	test
	xalan-c
	xerces-c
)
