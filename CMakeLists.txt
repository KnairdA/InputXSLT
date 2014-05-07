cmake_minimum_required(VERSION 2.8)
project(InputXSLT)

set(
	CMAKE_CXX_FLAGS
	"-std=c++11 -W -Wall -Wextra -Winline -pedantic -g"
)

include_directories(
	src/
)

add_library(
	InputXSLT
	STATIC
	src/plattform_guard.cc
	src/transformation_facade.cc
	src/function/read_file.cc
	src/function/read_xml_file.cc
	src/function/read_directory.cc
	src/support/filesystem_context.cc
	src/support/dom/document_cache.cc
	src/support/dom/document_cache_item.cc
)

add_executable(
	example
	example.cc
)

add_executable(
	test
	test.cc
	tests/function_read_file.cc
)

set(
	Libraries
	InputXSLT
	xalan-c
	xerces-c
	boost_system
	boost_filesystem
)

target_link_libraries(
	example
	${Libraries}
)

target_link_libraries(
	test
	gtest
	${Libraries}
)
