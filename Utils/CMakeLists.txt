cmake_minimum_required(VERSION 3.5)

project(Utils LANGUAGES C)

include_directories(include)
include_directories(include/encoding)

set(HEADERS
	include/ClientUtility.h
	include/GetAddrInfo.h
	include/Practical.h
	include/ServerUtility.h
	include/encoding/BruteForceCoding.h
	)

set(SOURCES
	ClientUtility.c
	GetAddrInfo.c
	ServerUtility.c
	Practical.c
	encoding/BruteForceCoding.c
	)

add_library(utils ${HEADERS} ${SOURCES})
