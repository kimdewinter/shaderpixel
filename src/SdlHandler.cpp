#include "SdlHandler.h"
#include <iostream>

namespace
{
#ifndef NDEBUG
	void print_opengl_version() noexcept
	{
		int major, minor;
		glGetIntegerv(GL_MAJOR_VERSION, &major);
		glGetIntegerv(GL_MINOR_VERSION, &minor);
		std::cout << "OpenGL version " << major << '.' << minor << std::endl;
	}

	void print_opengl_extensions() noexcept
	{
		std::cout << "OpenGL extensions:" << std::endl;
		int len;
		glGetIntegerv(GL_NUM_EXTENSIONS, &len);
		for (int i = 0; i < len; i++)
			std::cout << glGetStringi(GL_EXTENSIONS, i) << std::endl;
	}

	void debug_info_opengl() noexcept
	{
		std::cout << "Implementation by " << glGetString(GL_VENDOR) << std::endl;
		std::cout << "Using renderer " << glGetString(GL_RENDERER) << std::endl;
		std::cout << "Primary GLSL supported is " << glGetString(GL_SHADING_LANGUAGE_VERSION) << std::endl;
		int n;
		glGetIntegerv(GL_MAX_VERTEX_ATTRIBS, &n);
		std::cout << "MAximum nr of vertex attributes supported: " << n << std::endl;
		print_opengl_extensions();
	}
#endif
}

SdlHandler::SdlHandler(
	std::function<Window *()> window_creation_after_sdl_init,
	std::array<GLfloat, 4> const &clear_color) : clear_color(clear_color)
{
	if (SDL_Init(SDL_INIT_VIDEO) < 0)
		throw std::runtime_error(std::string("Error initializing SDL: ") + std::string(SDL_GetError()));

	// set required SDL settings
	SDL_SetRelativeMouseMode(SDL_TRUE);

	// create window via the supplied function pointer
	this->window = window_creation_after_sdl_init();
	if (!this->window)
	{
		SDL_Quit();
		throw std::logic_error("Error: window creation failed");
	}

	this->window->make_current();

	// set vsync
	if (SDL_GL_SetSwapInterval(VSYNC) != 0)
	{
		delete this->window;
		this->window = NULL;
		SDL_Quit();
		throw std::runtime_error("Error in SDL_GL_SetSwapInterval(): " + std::string(SDL_GetError()));
	}

	// load OpenGL via GLAD
	if (!(gladLoadGLLoader((GLADloadproc)SDL_GL_GetProcAddress)))
	{
		delete this->window;
		this->window = NULL;
		SDL_Quit();
		throw std::runtime_error("Error in gladLoadGLLoader()");
	}

	// set required OpenGL settings
	glEnable(GL_DEPTH_TEST);
	glClearColor(clear_color[0], clear_color[1], clear_color[2], clear_color[3]);
#if WIREFRAME_MODE == 1
	glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
#endif

#ifndef NDEBUG
	debug_info_opengl();
#endif

	// check for any OpenGL errors
	GLenum error_check = glGetError();
	if (error_check != GL_NO_ERROR)
	{
		delete this->window;
		this->window = NULL;
		SDL_Quit();
		throw std::runtime_error("Error in OpenGL.");
	}
}

SdlHandler::~SdlHandler() noexcept
{
	if (this->window)
	{
		delete this->window;
		this->window = NULL;
	}
	SDL_Quit();
}
