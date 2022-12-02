#ifndef SDLHANDLER_H_INCLUDED
#define SDLHANDLER_H_INCLUDED

#include "Window.h"
#include <SDL.h>
#include <unordered_set>
#include <functional>
#include <optional>

class SdlHandler
{
public:
	/// @param windows_creation_after_sdl_init Necessary because Window can only be created after SDL initialization
	/// @param clear_color Background color when glClear() is called
	SdlHandler(
		std::function<std::unordered_set<Window *>()> windows_creation_after_sdl_init,
		std::array<GLfloat, 4> const &clear_color = {0.0, 0.0, 0.0, 1.0});
	~SdlHandler() noexcept;
	void add_window(Window *window);
	std::optional<Window *> find_window_by_name(std::string const &str) const noexcept;
	std::optional<Window *> find_window_by_window_id(Uint32 const id) const noexcept;
	void remove_window(Window *window);
	void set_clear_color(std::array<GLfloat, 4> const &rgba) noexcept;
	/// @brief Clears the buffer so it can be drawn on
	void clear() const noexcept;

	std::unordered_set<Window *> windows = {};

private:
	std::array<GLfloat, 4> clear_color;
};

#endif
