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
	~Window();
	void set_clear_color(std::array<GLfloat, 4> const &rgba) noexcept;
	void clear() noexcept;
	void swap() noexcept;

private:
	std::string window_name;
	int window_width;
	int window_height;
	std::array<GLfloat, 4> clear_color = {CLEAR_COLOR_R, CLEAR_COLOR_G, CLEAR_COLOR_B, CLEAR_COLOR_A}; // The background color when glClear() is called
	SDL_Window *window_ptr = NULL;
	SDL_GLContext context_ptr = NULL; // SDL_GLContext is an alias for void *
};

#endif
