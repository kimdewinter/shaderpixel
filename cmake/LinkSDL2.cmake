include(FetchContent)

macro(LinkSDL2 TARGET ACCESS)
	FetchContent_Declare(
		sdl2
		GIT_REPOSITORY https://github.com/libsdl-org/SDL
		GIT_TAG release-2.24.2
	)

	FetchContent_MakeAvailable(sdl2)

	target_include_directories(${TARGET} ${ACCESS} ${sdl2_SOURCE_DIR}/include)

	if (APPLE)
		target_link_libraries(${TARGET} ${ACCESS} sdl2)
	elseif(WIN32)
		target_link_libraries(${TARGET} ${ACCESS} SDL2::SDL2)
		target_link_libraries(${TARGET} ${ACCESS} SDL2::SDL2main)
	else()
		target_link_libraries(${TARGET} ${ACCESS} ${SDL2_LIBRARIES})
	endif()

endmacro()