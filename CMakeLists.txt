cmake_minimum_required(VERSION 2.6)

add_definitions(
	-W
	-O2
	-g
)

OPTION(DEBUG
        "Build project using debug mode"
            ON)

if (DEBUG)
        SET(CMAKE_BUILD_TYPE Debug)
else (DEBUG)
        SET(CMAKE_BUILD_TYPE Release)
endif (DEBUG)

message("Build type is " ${CMAKE_BUILD_TYPE})

set(CMAKE_CXX_FLAGS         "-g -W")
set(CMAKE_CXX_FLAGS_DEBUG   "-D_DEBUG -DHTTP_DEBUG")
set(CMAKE_CXX_FLAGS_RELEASE "-O2 -DNDEBUG")


subdirs(
	async/examples
	async/performance
	venus/examples
	venus/performance
)

if (MSVC)

else (MSVC)
	add_definitions(
		-g
		-O2
		-W
	)

	link_libraries(pthread rt)
endif (MSVC)

enable_testing()

install(
	DIRECTORY
		async/include/async
		venus/include/venus
		stdext/include/stdext
		tpl/include/tpl
	DESTINATION cerl/include
	FILES_MATCHING
		PATTERN "*.h"
		PATTERN "*.hpp"
		PATTERN "*.inl"
		PATTERN "*.c"
)
