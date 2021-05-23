cmake_minimum_required(VERSION 3.5)

project(Utils LANGUAGES C)

include_directories(include)

set(HEADERS
	include/ClientUtility.h
	include/GetAddrInfo.h
	include/Practical.h
	include/ServerUtility.h
	)

set(SOURCES
	ClientUtility.c
	GetAddrInfo.c
	ServerUtility.c
	Practical.c
	)

add_library(utils ${HEADERS} ${SOURCES})
