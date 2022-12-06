include(FetchContent)

macro(LinkSDL2 TARGET ACCESS)
	FetchContent_Declare(
		sdl2
		GIT_REPOSITORY https://github.com/libsdl-org/SDL
		GIT_TAG release-2.24.2
	)

	FetchContent_MakeAvailable(sdl2)

	find_package(SDL2 REQUIRED)

	target_include_directories(${TARGET} ${ACCESS} ${sdl2_SOURCE_DIR}/include)
	target_link_libraries(${TARGET} ${ACCESS} ${SDL2_LIBRARIES})
endmacro()