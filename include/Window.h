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
	Window(std::string const &window_name, int const window_width, int const window_height);
	~Window() noexcept;
	/// @brief Swaps the buffer to the screen, displaying what has been drawn on the buffer
	void swap() const noexcept;
	/// @brief Makes this window and it's context the "current" window and context
	void make_current() const noexcept;
	std::string const &get_name() const noexcept;
	/// @return ID associated with this Window's SDL_Window *
	Uint32 const get_id() const noexcept;

private:
	std::string window_name = "";
	SDL_Window *window_ptr = NULL;
	Uint32 window_id = 0;			  // this is an SDL_GL WindowID
	SDL_GLContext context_ptr = NULL; // SDL_GLContext is an alias for a void * to a SDL_GLContext object
	int window_width = WINDOW_WIDTH;
	int window_height = WINDOW_HEIGHT;
};

#endif
