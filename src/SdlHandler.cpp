#include "SdlHandler.h"
#include <iostream>

SdlHandler::SdlHandler(
	std::function<std::unordered_set<Window *>()> windows_creation_after_sdl_init)
{
	if (SDL_Init(SDL_INIT_VIDEO) < 0)
		throw std::runtime_error(std::string("Error initializing SDL: ") + std::string(SDL_GetError()));

	// Create windows via the supplied function pointer
	for (Window *window : windows_creation_after_sdl_init())
		this->windows.insert(window);

	// Load OpenGL via GLAD
	if (!(gladLoadGLLoader((GLADloadproc)SDL_GL_GetProcAddress)))
		throw std::runtime_error("Error in gladLoadGLLoader()");

	// Set OpenGL enables
	glEnable(GL_DEPTH_TEST);

	// Check for any OpenGL errors
	if (glGetError() != GL_NO_ERROR)
		throw std::runtime_error("Error in OpenGL");
}

SdlHandler::~SdlHandler() noexcept
{
	for (Window *window : this->windows)
	{
		if (window)
			delete window;
	}
	SDL_Quit();
}

void SdlHandler::add_window(Window *window)
{
	if (this->windows.find(window) != this->windows.end())
		throw std::logic_error("Error adding window to SdlHandler: this Window already present");
	this->windows.insert(window);
}

void SdlHandler::remove_window(Window *window)
{
	std::unordered_set<Window *>::iterator iter = this->windows.find(window);
	if (iter == this->windows.end())
		throw std::logic_error("Error removing window from SdlHandler: this Window already absent");
	delete *iter;			   // Delete actual Window instance
	this->windows.erase(iter); // Erase this's pointer to the Window
}
