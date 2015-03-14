set(BUILD .)

include_directories(
	../include
)



subdirs(
	aio
	app
	buffered
	dataque
	echo
	epoll
	event
	fiber
	fileio
	future
	ioservice
	memory
	multisleep
	mutex
	sleep
	sockio
	syncio
	syncsockio
	task
	timedmutex
    poolmt
	timer
)

