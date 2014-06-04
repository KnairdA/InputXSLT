cmake_minimum_required(VERSION 2.8)
project(InputXSLT)

set(
	CMAKE_CXX_FLAGS
	"${CMAKE_CXX_FLAGS} -std=c++11 -W -Wall -Wextra -Winline -pedantic"
)

include_directories(
	src/
)

set(
	Libraries
	InputXSLT
	xalan-c
	xerces-c
	boost_system
	boost_filesystem
	boost_program_options
)

set(
	Sources
	src/plattform_guard.cc
	src/transformation_facade.cc
	src/function/read_file.cc
	src/function/read_xml_file.cc
	src/function/read_directory.cc
	src/function/transform.cc
	src/function/external_text_formatter.cc
	src/support/filesystem_context.cc
	src/support/stylesheet_parameter_guard.cc
	src/support/xalan_string.cc
	src/support/include_entity_resolver.cc
	src/support/error_capacitor.cc
	src/support/tuple/xobject_value.cc
	src/support/dom/document_cache.cc
	src/support/dom/document_cache_item.cc
	src/support/dom/result_node_facade.cc
)

add_executable(
	ixslt
	ixslt.cc
)

add_library(
	InputXSLT
	STATIC
	${Sources}
)

target_link_libraries(
	ixslt
	${Libraries}
)

add_custom_target(
	testcase
	ALL
	WORKING_DIRECTORY
	${CMAKE_CURRENT_SOURCE_DIR}/test/
	COMMAND
	sh check.sh
	DEPENDS
	ixslt
)
