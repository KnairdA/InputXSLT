cmake_minimum_required(VERSION 2.8)
project(InputXSLT)

set(
	CMAKE_CXX_FLAGS
	"-std=c++11 -W -Wall -Wextra -Winline -pedantic -g"
)

include_directories(
	src/
)

add_executable(
	test
	test.cc
	src/transformer_facade.cc
	src/function/read_file.cc
	src/function/read_xml_file.cc
)

target_link_libraries(
	test
	xalan-c
	xerces-c
)
