#include "Window.h"

Window::Window(
	std::string const &window_name,
	int const window_width,
	int const window_height) : window_name(window_name),
							   window_width(window_width),
							   window_height(window_height)
{
	// Check arguments
	if (window_width < 1 || window_height < 1)
		throw std::invalid_argument("Invalid window dimension, width and height must be > 0");
	if (window_name.empty())
		throw std::invalid_argument("Empty window name");

	// Create window
	if (!(this->window_ptr = SDL_CreateWindow(
			  window_name.c_str(),
			  SDL_WINDOWPOS_CENTERED,
			  SDL_WINDOWPOS_CENTERED,
			  window_width,
			  window_height,
			  SDL_WINDOW_SHOWN | SDL_WINDOW_OPENGL | SDL_WINDOW_RESIZABLE)))
		throw std::runtime_error("Error in SDL_Createwindow(): " + std::string(SDL_GetError()));

	// Create context
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_FLAGS, SDL_GL_CONTEXT_FORWARD_COMPATIBLE_FLAG); // For extra performance, don't allow pre-3.0 backward compatability
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);	   // Only allow use of OpenGL's newer "Core" profile
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, OPENGL_VERSION_MAJOR);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, OPENGL_VERSION_MINOR);
	SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, DOUBLE_BUFFER);
	SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, DEPTH_SIZE);

	// Create context
	if (!(this->context_ptr = SDL_GL_CreateContext(this->window_ptr)))
		throw std::runtime_error("Error in SDL_GL_CreateContext(): " + std::string(SDL_GetError()));

	// Set vsync
	// if (SDL_GL_SetSwapInterval(VSYNC) != 0)
	// throw std::runtime_error("Error in SDL_GL_SetSwapInterval(): " + std::string(SDL_GetError()));
}

Window::~Window()
{
	if (this->window_ptr)
	{
		SDL_DestroyWindow(this->window_ptr);
		this->window_ptr = NULL;
	}
	if (this->context_ptr)
	{
		SDL_GL_DeleteContext(this->context_ptr);
		this->context_ptr = NULL;
	}
}

void Window::set_clear_color(std::array<GLfloat, 4> const &rgba) noexcept
{
	this->clear_color = rgba;
	glClearColor(rgba[0], rgba[1], rgba[2], rgba[3]);
}

void Window::clear() noexcept
{
	glClearColor(
		this->clear_color[0],
		this->clear_color[1],
		this->clear_color[2],
		this->clear_color[3]);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

void Window::swap() noexcept
{
	SDL_GL_SwapWindow(this->window_ptr);
}
