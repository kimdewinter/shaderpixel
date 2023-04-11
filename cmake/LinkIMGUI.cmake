include(FetchContent)

FetchContent_Populate(imgui
  URL https://github.com/ocornut/imgui/archive/docking.zip
  SOURCE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/imgui
)

add_library(imgui_sdl2 STATIC
  imgui/imgui.cpp
  imgui/imgui_draw.cpp
  imgui/imgui_demo.cpp
  imgui/imgui_tables.cpp
  imgui/imgui_widgets.cpp

  imgui/backends/imgui_impl_sdl2.cpp
  imgui/backends/imgui_impl_opengl3.cpp
)

target_link_libraries(imgui_sdl2 PUBLIC sdl2 ${OPENGL_LIBRARIES})

target_include_directories(imgui_sdl2
PUBLIC
  ${CMAKE_CURRENT_SOURCE_DIR}/imgui
  ${CMAKE_CURRENT_SOURCE_DIR}/imgui/backends
)