project (StarboundAPI)

if(WIN32)
	set(OS_FILES
		win32.cpp
	)
endif()

add_library(SBAPIDll SHARED
	${OS_FILES}
	main.cpp
	main.h
	Hooking.h
	Hooking.cpp
	UniverseServer.cpp
)