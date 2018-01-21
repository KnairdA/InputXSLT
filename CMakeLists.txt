cmake_minimum_required(VERSION 2.8)
project(InputXSLT)

set(
	CMAKE_CXX_FLAGS
	"${CMAKE_CXX_FLAGS} -std=c++14 -W -Wall -Wextra -Winline -pedantic"
)

include_directories(
	src/
)

add_executable(
	ixslt
		ixslt.cc
)

add_library(
	InputXSLT
	STATIC
		src/platform_guard.cc
		src/transformer_facade.cc
		src/function/read_file.cc
		src/function/write_file.cc
		src/function/read_directory.cc
		src/function/generate.cc
		src/function/external_command.cc
		src/support/filesystem_context.cc
		src/support/xalan_string.cc
		src/support/include_entity_resolver.cc
		src/support/error/error_multiplexer.cc
		src/support/error/error_capacitor.cc
		src/support/error/warning_capacitor.cc
		src/support/type/xobject_value.cc
		src/support/dom/document_cache.cc
		src/support/dom/document_cache_item.cc
		src/support/dom/result_node_facade.cc
)

target_link_libraries(
	ixslt
		InputXSLT
		xalan-c
		xerces-c
		pthread
		boost_system
		boost_filesystem
		boost_program_options
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

install(
	TARGETS
		ixslt
	DESTINATION
		bin
)
