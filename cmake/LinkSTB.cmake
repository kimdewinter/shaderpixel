include(FetchContent)

macro(LinkSTB TARGET ACCESS)
    FetchContent_Declare(
        stb
        GIT_REPOSITORY https://github.com/nothings/stb
        GIT_TAG 5736b15f7ea0ffb08dd38af21067c314d6a3aae9
    )

    FetchContent_GetProperties(stb)

    if (NOT stb_POPULATED)
        FetchContent_Populate(stb)
    endif()

    target_include_directories(${TARGET} ${ACCESS} ${stb_SOURCE_DIR})
endmacro()