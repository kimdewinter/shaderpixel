#ifndef SDLHANDLER_H_INCLUDED
#define SDLHANDLER_H_INCLUDED

#include "Window.h"
#include <unordered_set>
#include <functional>
#include <optional>

class SdlHandler
{
public:
	/// @brief initializes SDL and OpenGL, and creates a window and context
	/// @param windows_creation_after_sdl_init necessary because Window can only be created after SDL initialization
	/// @param clear_color background color when glClear() is called
	SdlHandler(
		std::function<Window *()> window_creation_after_sdl_init,
		std::array<GLfloat, 4> const &clear_color = {0.0, 0.0, 0.0, 1.0});
	~SdlHandler() noexcept;
	/// @brief Clears the buffer so it can be drawn on
	void clear() const noexcept;

	Window *window = NULL;

private:
	std::array<GLfloat, 4> clear_color;
};

#endif
