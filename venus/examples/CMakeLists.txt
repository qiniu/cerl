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
	../../async/include
	../include
)

subdirs(
	blocking_connect
	calc
	connection
	hashserver
	recursivecall
	sdl
	syncconn
)
