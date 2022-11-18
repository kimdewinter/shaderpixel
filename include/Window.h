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
	void swap() const noexcept;
	std::string const &get_name() const noexcept;

private:
	std::string window_name = "";
	int window_width = WINDOW_WIDTH;
	int window_height = WINDOW_HEIGHT;
	SDL_Window *window_ptr = NULL;
	SDL_GLContext context_ptr = NULL; // SDL_GLContext is an alias for a void * to a SDL_GLContext object
};

#endif
