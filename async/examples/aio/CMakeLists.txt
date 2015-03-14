add_definitions(
	-g
)

add_executable(test_cerlaio
	test_cerlaio.cpp
)

add_executable(test_libaio
	test_libaio.cpp
)

target_link_libraries(test_libaio aio)

add_test(test_cerlaio ./test_cerlaio)
