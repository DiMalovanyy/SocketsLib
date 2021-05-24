cmake_minimum_required(VERSION 3.5)

set(HEADERS 
	utils/voting_utils.h
	utils/VoteEncoding.h
	utils/VoteProtocol.h
	utils/Framer.h
	)
set(SOURCES
	DelimFramer.c
	LengthFramer.c
	VoteEncodingText.c
	)
	

foreach(binary voting_client voting_server)
	add_executable(${binary} ${binary}.c ${SOURCES} ${HEADERS})
	target_link_libraries(${binary} ${CMAKE_BINARY_DIR}/Utils/libutils.a)
	target_include_directories(${binary} PUBLIC ${CMAKE_SOURCE_DIR}/Utils/include)
endforeach()
