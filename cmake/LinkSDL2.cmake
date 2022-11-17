# include(FetchContent)

# macro(LinkSDL2 TARGET ACCESS)
# 	FetchContent_Declare(
# 		sdl2
# 		GIT_REPOSITORY https://github.com/libsdl-org/SDL
# 		GIT_TAG release-2.24.2
# 	)

# 	FetchContent_GetProperties(sdl2)

# 	if (NOT sdl2_POPULATED)
# 		FetchContent_Populate(sdl2)
# 	endif()

# 	target_include_directories(${TARGET} ${ACCESS} ${sdl2_SOURCE_DIR}/include)
# 	target_link_libraries(${TARGET} ${ACCESS} sdl2)
# endmacro()

include(FetchContent)

macro(LinkSDL2 TARGET ACCESS)
	FetchContent_Declare(
		sdl2
		GIT_REPOSITORY https://github.com/libsdl-org/SDL
		GIT_TAG release-2.24.2
		GIT_PROGRESS TRUE
	)
	FetchContent_Declare(
		sdl2image
		GIT_REPOSITORY https://github.com/libsdl-org/SDL_image
		GIT_TAG release-2.6.2
		GIT_PROGRESS TRUE
	)
	FetchContent_GetProperties(sdl2 sdl2image)
	FetchContent_MakeAvailable(sdl2)

	set(SDL_LIBRARIES ${SDL_LIBRARIES} SDL2 SDL2main SDL2_image)

	target_include_directories(${TARGET} ${ACCESS} ${sdl2_SOURCE_DIR}/include)
	target_link_libraries(${TARGET} ${ACCESS} ${SDL_LIBRARIES})
endmacro()
