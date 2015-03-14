include_directories(
	../../../../venus/include
)

link_libraries( boost_system boost_thread pthread )

add_executable(asio_client
   main.cpp
)

