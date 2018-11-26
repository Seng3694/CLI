cmake_minimum_required(VERSION 3.5)

project (CLI)

add_subdirectory(cli)

if(CLI_BUILD_TESTS)
	add_subdirectory(tests)
endif()

if(CLI_BUILD_EXAMPLES)
	add_subdirectory(examples)
endif()