#ifndef WINDOW_H_INCLUDED
#define WINDOW_H_INCLUDED

#include "main.h"
#include <SDL.h>
#include <string>
#include <array>
#include <glad/glad.h>

class Window
{
public:
	Window(std::string const &window_name, int const window_width, int const window_height) noexcept;
	~Window() noexcept;
	/// @brief swaps the buffer to the screen, displaying what has been drawn on the buffer
	void clear() const noexcept;
	void swap() const noexcept;
	/// @brief makes this window and it's context the "current" window and context
	void make_current() const noexcept;
	SDL_GLContext const get_context_ptr() const noexcept;
	SDL_Window *const get_window_ptr() const noexcept;

private:
	std::string window_name = "";
	SDL_Window *window_ptr = NULL;
	Uint32 window_id = 0;			  // this is an SDL_GL WindowID
	SDL_GLContext context_ptr = NULL; // SDL_GLContext is an alias for a void * to a SDL_GLContext object
	int window_width = WINDOW_WIDTH;
	int window_height = WINDOW_HEIGHT;
};

#endif
