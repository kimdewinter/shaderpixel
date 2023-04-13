include(FetchContent)

FetchContent_Populate(imgui
  URL https://github.com/ocornut/imgui/archive/docking.zip
)

add_library(imgui_sdl2 STATIC
  ${CMAKE_CURRENT_SOURCE_DIR}/build/_deps/imgui-src/imgui.cpp
  ${CMAKE_CURRENT_SOURCE_DIR}/build/_deps/imgui-src/imgui_draw.cpp
  ${CMAKE_CURRENT_SOURCE_DIR}/build/_deps/imgui-src/imgui_demo.cpp
  ${CMAKE_CURRENT_SOURCE_DIR}/build/_deps/imgui-src/imgui_tables.cpp
  ${CMAKE_CURRENT_SOURCE_DIR}/build/_deps/imgui-src/imgui_widgets.cpp

  ${CMAKE_CURRENT_SOURCE_DIR}/build/_deps/imgui-src/backends/imgui_impl_sdl2.cpp
  ${CMAKE_CURRENT_SOURCE_DIR}/build/_deps/imgui-src/backends/imgui_impl_opengl3.cpp
)

target_link_libraries(imgui_sdl2 PUBLIC sdl2 ${OPENGL_LIBRARIES})

target_include_directories(imgui_sdl2
PUBLIC
  ${sdl2_SOURCE_DIR}/include
  ${CMAKE_CURRENT_SOURCE_DIR}/build/_deps/imgui-src
)