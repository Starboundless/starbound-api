cmake_minimum_required(VERSION 2.8)
project (StarboundAPI)

if(WIN32)
	set(OS_FILES
		win32.cpp
	)
endif()

if(MSVC)
MESSAGE("TURN OFF INCREMENTAL LINKING OR BAD SHIT WILL HAPPEN")
endif()

add_library(SBAPIDll SHARED
	${OS_FILES}
	main.cpp
	main.h
	Hooking.h
	Hooking.cpp
	Title.cpp
)