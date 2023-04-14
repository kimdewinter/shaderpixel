#include "SdlHandler.h"
#include <iostream>
#include "ErrorHandler.h"

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
	std::array<GLfloat, 4> const &clear_color) noexcept : clear_color(clear_color)
{
	ASSERT(SDL_Init(SDL_INIT_VIDEO) == 0, "Error initializing SDL: " + std::string(SDL_GetError()));

// set required SDL settings
#if CAMERA_LOCKED_BY_DEFAULT 1
	SDL_SetRelativeMouseMode(SDL_FALSE);
#else
	SDL_SetRelativeMouseMode(SDL_TRUE);
#endif

	// create window via the supplied function pointer
	this->window = window_creation_after_sdl_init();
	ASSERT(this->window, "window creation failed");

	this->window->make_current();

	// set vsync
	ASSERT(SDL_GL_SetSwapInterval(VSYNC) == 0, "error in SDL_GL_SetSwapInterval(): " + std::string(SDL_GetError()));

	// load OpenGL via GLAD
	ASSERT(gladLoadGLLoader((GLADloadproc)SDL_GL_GetProcAddress), "error in gloadLoadGLLoader()");

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
	ASSERT(glGetError() == GL_NO_ERROR, "error in OpenGL");
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

void SdlHandler::set_relative_mouse_mode(SDL_bool const value) const noexcept
{
	SDL_SetRelativeMouseMode(value);
}
