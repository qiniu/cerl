cmake_minimum_required(VERSION 2.6)

set(BUILD .)

if (MSVC)

else (MSVC)
	add_definitions(
		-g
		-O2
		-W
	)

	link_libraries(pthread rt)
endif (MSVC)

include_directories(
	../include
	../../async/include
)

subdirs(
	venus
)
