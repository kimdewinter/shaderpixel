include(FetchContent)

#FetchContent_Populate(imgui
#  URL https://github.com/ocornut/imgui/archive/docking.zip
#)

FetchContent_Declare(
	imgui
	GIT_REPOSITORY https://github.com/ocornut/imgui.git
	GIT_TAG v1.89.5
)

FetchContent_MakeAvailable(imgui)

add_library(imgui STATIC
  ${CMAKE_CURRENT_BINARY_DIR}/_deps/imgui-src/imgui.cpp
  ${CMAKE_CURRENT_BINARY_DIR}/_deps/imgui-src/imgui_draw.cpp
  ${CMAKE_CURRENT_BINARY_DIR}/_deps/imgui-src/imgui_demo.cpp
  ${CMAKE_CURRENT_BINARY_DIR}/_deps/imgui-src/imgui_tables.cpp
  ${CMAKE_CURRENT_BINARY_DIR}/_deps/imgui-src/imgui_widgets.cpp
  ${CMAKE_CURRENT_BINARY_DIR}/_deps/imgui-src/backends/imgui_impl_sdl2.cpp
  ${CMAKE_CURRENT_BINARY_DIR}/_deps/imgui-src/backends/imgui_impl_opengl3.cpp
)

target_link_libraries(imgui PUBLIC)

target_include_directories(imgui
PUBLIC
  ${sdl2_SOURCE_DIR}/include
  ${CMAKE_CURRENT_BINARY_DIR}/_deps/imgui-src
)