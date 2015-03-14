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
	../../venus/include
	../../stdext/include
)

subdirs(
	async/server
	async/client
	async/mtcli
	callback
	epoll/server
	epoll/client
	fiber
	libcoro
	libcoro10k
	libcoro10k_rand
	libevent/server
	socket/server
	socket/client
	syscall/client
	syscall/epoll
	syscall/server
	syscall/sockread
	#winFiber
)

find_package( Boost )

if (Boost_FOUND)
subdirs(
	asio/server
	asio/client
)
endif (Boost_FOUND)
