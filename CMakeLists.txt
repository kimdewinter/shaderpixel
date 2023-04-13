cmake_minimum_required(VERSION 3.16 FATAL_ERROR)
project(shaderpixel CXX)

#===========================================================================================
# OPTIONAL LIBRARiES
#
# The following libraries can be pulled down and added to the project by turnning the
# desired options on. Open an issue for additional default support.
# See: https://github.com/Hoshiningen/OpenGL-Template/issues
# How to FetchContent: https://github.com/pr0g/cmake-examples/blob/main/examples/more/fetch-content/CMakeLists.txt
# It is highly recommended to have either GLFW or SDL2 enabled so you can make a context & windows

# set(ENABLE_GLFW   ON CACHE BOOL "Add GLFW OpenGL-utility library to the project" FORCE)
set(ENABLE_SDL2   ON CACHE BOOL "Add SDL2 hardware abstraction library to the project" FORCE)
set(ENABLE_ASSIMP   ON CACHE BOOL "Add Open Asset Import Library (assimp) to the project" FORCE)
set(ENABLE_GLM   ON CACHE BOOL "Add GLM OpenGL-mathematics library to the project" FORCE)
set(ENABLE_STB   ON CACHE BOOL "Add STB image-utility library to the project" FORCE)
set(ENABLE_IMGUI	ON CACHE BOOL "Add IMGUI gui library to the project" FORCE)

#===========================================================================================
# GLAD CONFIGURATION
#
# Configure GLAD loader generation variables (lines below pulled from the GLAD top-level
# CMakeLists.txt file, with slight modifications.
# Refer to https://github.com/Dav1dde/glad for more information)

set(GLAD_OUT_DIR        "${CMAKE_CURRENT_BINARY_DIR}" CACHE STRING "Output directory" FORCE)
set(GLAD_PROFILE        "core" CACHE STRING "OpenGL profile" FORCE)
set(GLAD_API            "gl=4.1" CACHE STRING "API type/version pairs, like \"gl=4.1,gles=\", no version means latest" FORCE)
set(GLAD_GENERATOR      "c" CACHE STRING "Language to generate the binding for" FORCE)
set(GLAD_EXTENSIONS     "" CACHE STRING "Path to extensions file or comma separated list of extensions, if missing all extensions are included" FORCE)
set(GLAD_SPEC           "gl" CACHE STRING "Name of the spec" FORCE)
set(GLAD_ALL_EXTENSIONS OFF CACHE BOOL "Include all extensions instead of those specified by GLAD_EXTENSIONS" FORCE)
set(GLAD_NO_LOADER      OFF CACHE BOOL "No loader" FORCE)
set(GLAD_REPRODUCIBLE   OFF CACHE BOOL "Reproducible build" FORCE)

#===========================================================================================

set_property(GLOBAL PROPERTY USE_FOLDERS ON)

# Set directory paths
set(SOURCE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/src)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR})
set(CMAKE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/cmake)

set(sources
${SOURCE_DIR}/main.cpp
${SOURCE_DIR}/Camera.cpp
${SOURCE_DIR}/Clock.cpp
${SOURCE_DIR}/ErrorHandler.cpp
${SOURCE_DIR}/EventHandler.cpp
${SOURCE_DIR}/Renderer.cpp
${SOURCE_DIR}/SdlHandler.cpp
${SOURCE_DIR}/Window.cpp
${SOURCE_DIR}/Mesh.cpp
${SOURCE_DIR}/Model.cpp
${SOURCE_DIR}/Shader.cpp
)

add_executable(${CMAKE_PROJECT_NAME} ${sources})

# Enable C++17
set(CMAKE_CXX_STANDARD 17)
set_target_properties(${CMAKE_PROJECT_NAME} PROPERTIES
    CXX_STANDARD 17
    CXX_STANDARD_REQUIRED YES
    CXX_EXTENSIONS NO)

target_include_directories(${CMAKE_PROJECT_NAME} PUBLIC
${CMAKE_CURRENT_SOURCE_DIR}/include
${CMAKE_CURRENT_SOURCE_DIR}/include/stb_image.h
)

# Perform dependency linkage
include(${CMAKE_DIR}/LinkGLAD.cmake)
LinkGLAD(${CMAKE_PROJECT_NAME} PRIVATE)

find_package(OpenGL REQUIRED)
if (OpenGL_FOUND)
    target_include_directories(${CMAKE_PROJECT_NAME} PRIVATE ${OPENGL_INCLUDE_DIRS})
    target_link_libraries(${CMAKE_PROJECT_NAME} PRIVATE ${OPENGL_LIBRARIES})
else()
    message(FATAL_ERROR "OpenGL was not found on the system")
endif()

# Handle the optional libraries
if (${ENABLE_GLFW})
	include(${CMAKE_DIR}/LinkGLFW.cmake)
	LinkGLFW(${CMAKE_PROJECT_NAME} PRIVATE)
endif()

if (${ENABLE_SDL2})
	include(${CMAKE_DIR}/LinkSDL2.cmake)
	LinkSDL2(${CMAKE_PROJECT_NAME} PRIVATE)
endif()

if (${ENABLE_ASSIMP})
    include(${CMAKE_DIR}/LinkASSIMP.cmake)
    LinkASSIMP(${CMAKE_PROJECT_NAME} PRIVATE)
endif()

if (${ENABLE_GLM})
	include(${CMAKE_DIR}/LinkGLM.cmake)
	LinkGLM(${CMAKE_PROJECT_NAME} PRIVATE)
endif()

if (${ENABLE_STB})
	include(${CMAKE_DIR}/LinkSTB.cmake)
	LinkSTB(${CMAKE_PROJECT_NAME} PRIVATE)
endif()

if (${ENABLE_IMGUI})
	include(${CMAKE_DIR}/LinkIMGUI.cmake)

	target_link_libraries(${CMAKE_PROJECT_NAME} PRIVATE imgui_sdl2)
endif()

# Set project folders
set_target_properties(${CMAKE_PROJECT_NAME} PROPERTIES FOLDER ${PROJECT_NAME})

# Set build type to Release or Debug
set(CMAKE_BUILD_TYPE Debug)

target_compile_options(shaderpixel
PRIVATE
-fno-exceptions)
