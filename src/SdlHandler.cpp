#include "SdlHandler.h"
#include <iostream>

namespace
{
#ifdef DEBUG
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

	void clear_set_of_windows(std::unordered_set<Window *> &set) noexcept
	{
		for (Window *window : set)
		{
			delete window;
			window = NULL;
		}
		set.clear();
	}
}

SdlHandler::SdlHandler(
	std::function<std::unordered_set<Window *>()> windows_creation_after_sdl_init,
	std::array<GLfloat, 4> const &clear_color) : clear_color(clear_color)
{
	if (SDL_Init(SDL_INIT_VIDEO) < 0)
		throw std::runtime_error(std::string("Error initializing SDL: ") + std::string(SDL_GetError()));

	// Create windows via the supplied function pointer
	this->windows = windows_creation_after_sdl_init();
	if (this->windows.empty())
	{
		SDL_Quit();
		throw std::logic_error("Error: window-set creation return empty set");
	}

	// Set vsync
	if (SDL_GL_SetSwapInterval(VSYNC) != 0)
	{
		clear_set_of_windows(this->windows);
		SDL_Quit();
		throw std::runtime_error("Error in SDL_GL_SetSwapInterval(): " + std::string(SDL_GetError()));
	}

	// Load OpenGL via GLAD
	if (!(gladLoadGLLoader((GLADloadproc)SDL_GL_GetProcAddress)))
	{
		clear_set_of_windows(this->windows);
		SDL_Quit();
		throw std::runtime_error("Error in gladLoadGLLoader()");
	}

	// Set required OpenGL settings
	glEnable(GL_DEPTH_TEST);
	glClearColor(clear_color[0], clear_color[1], clear_color[2], clear_color[3]);
#if WIREFRAME_MODE == 1
	glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
#endif

#ifdef DEBUG
	debug_info_opengl();
#endif

	// Check for any OpenGL errors
	GLenum error_check = glGetError();
	if (error_check != GL_NO_ERROR)
	{
		clear_set_of_windows(this->windows);
		SDL_Quit();
		throw std::runtime_error("Error in OpenGL.");
	}
}

SdlHandler::~SdlHandler() noexcept
{
	clear_set_of_windows(this->windows);
	SDL_Quit();
}

void SdlHandler::add_window(Window *window)
{
	if (this->windows.find(window) != this->windows.end())
		throw std::invalid_argument("Error adding window to SdlHandler: this Window already present");
	this->windows.insert(window);
}

std::optional<Window *> SdlHandler::find_window_by_name(std::string const &str) const noexcept
{
	for (Window *window : this->windows)
		if (window->get_name() == str)
			return window;
	return std::nullopt;
};

std::optional<Window *> SdlHandler::find_window_by_window_id(Uint32 const id) const noexcept
{
	for (Window *window : this->windows)
		if (window->get_id() == id)
			return window;
	return std::nullopt;
}

void SdlHandler::remove_window(Window *window)
{
	std::unordered_set<Window *>::iterator iter = this->windows.find(window);
	if (iter == this->windows.end())
		throw std::invalid_argument("Error removing window from SdlHandler: this Window already absent");
	delete *iter;			   // Delete actual Window instance
	this->windows.erase(iter); // Erase this's pointer to the Window
}

void SdlHandler::set_clear_color(std::array<GLfloat, 4> const &rgba) noexcept
{
	this->clear_color = rgba;
	glClearColor(
		this->clear_color[0],
		this->clear_color[1],
		this->clear_color[2],
		this->clear_color[3]);
}

void SdlHandler::clear() const noexcept
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}
