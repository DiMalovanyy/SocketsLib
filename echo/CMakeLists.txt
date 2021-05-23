cmake_minimum_required(VERSION 3.5)


foreach(binary echo_server echo_client)
	add_executable(${binary} ${binary}.c echo_utils.h)
	target_link_libraries(${binary} ${CMAKE_BINARY_DIR}/Utils/libutils.a)
	target_include_directories(${binary} PUBLIC  ${CMAKE_SOURCE_DIR}/Utils/include)
endforeach()
