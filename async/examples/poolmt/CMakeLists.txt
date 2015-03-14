add_definitions(
	-g
)

add_executable(test_poolmt
	test_poolmt.cpp
)

target_link_libraries(test_poolmt pthread)

add_test(test_poolmt ./test_poolmt)
