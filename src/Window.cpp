#include "Window.h"
#include "main.h"
#include "ErrorHandler.h"

Window::Window(
	std::string const &window_name,
	int const window_width,
	int const window_height) : window_name(window_name),
							   window_width(window_width),
							   window_height(window_height)
{
	// check arguments
	if (window_width < 1 || window_height < 1)
		throw std::invalid_argument("Invalid window dimension, width and height must be > 0");
	if (window_name.empty())
		throw std::invalid_argument("Empty window name");

	// set attributes (should this be part of the std::function that calls Window constructor?)
	if (
#ifdef __APPLE__
		SDL_GL_SetAttribute(SDL_GL_CONTEXT_FLAGS, SDL_GL_CONTEXT_FORWARD_COMPATIBLE_FLAG) < 0 || // For extra performance, don't allow pre-3.0 backward compatability
#endif
		SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE) < 0 || // Only allow use of OpenGL's newer "Core" profile
		SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, OPENGL_VERSION_MAJOR) < 0 ||
		SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, OPENGL_VERSION_MINOR) < 0 ||
		SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, DOUBLE_BUFFER) < 0 ||
		SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, DEPTH_SIZE) < 0 ||
		SDL_GL_SetAttribute(SDL_GL_MULTISAMPLEBUFFERS, MSAA_BUFFERS) < 0 ||
		SDL_GL_SetAttribute(SDL_GL_MULTISAMPLESAMPLES, MSAA_SAMPLES) < 0)
	{
		throw std::runtime_error("Error calling SDL_GL_SetAttribute()" + std::string(SDL_GetError()));
	}

	// create window
	if (!(this->window_ptr = SDL_CreateWindow(
			  window_name.c_str(),
			  SDL_WINDOWPOS_CENTERED,
			  SDL_WINDOWPOS_CENTERED,
			  window_width,
			  window_height,
			  SDL_WINDOW_OPENGL | SDL_WINDOW_RESIZABLE)))
		throw std::runtime_error("Error in SDL_Createwindow(): " + std::string(SDL_GetError()));
	if (!(this->window_id = SDL_GetWindowID(this->window_ptr)))
	{
		SDL_DestroyWindow(this->window_ptr);
		this->window_ptr = NULL;
		throw std::runtime_error("Error retrieving SDL WindowID of window with name: " + this->window_name + '\n' + SDL_GetError());
	}

	// create context
	if (!(this->context_ptr = SDL_GL_CreateContext(this->window_ptr)))
	{
		SDL_DestroyWindow(this->window_ptr);
		this->window_ptr = NULL;
		this->window_id = 0;
		throw std::runtime_error("Error in SDL_GL_CreateContext(): " + std::string(SDL_GetError()));
	}
}

Window::~Window() noexcept
{
	SDL_DestroyWindow(this->window_ptr);
	this->window_ptr = NULL;
	this->window_id = 0;
	SDL_GL_DeleteContext(this->context_ptr);
	this->context_ptr = NULL;
}

void Window::clear() const noexcept
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

void Window::swap() const noexcept
{
	SDL_GL_SwapWindow(this->window_ptr);
}

void Window::make_current() const noexcept
{
	if (SDL_GL_MakeCurrent(this->window_ptr, this->context_ptr) < 0)
		Error::output_warning(
			"Failure to make_current for window: " +
			this->window_name +
			"\n" +
			"SDL error: " +
			SDL_GetError());
}
