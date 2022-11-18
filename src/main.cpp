#include "main.h"
#include "ErrorHandler.h"
#include "SdlHandler.h"
#include "Window.h"
#include <functional>
#include <cstdlib>

namespace
{
	/// @brief Here you can decide what window the main() function treats as being your main window
	Window &get_main_window(SdlHandler const &sdlhandler)
	{
		for (Window *window : sdlhandler.windows)
			if (window->get_name() == APP_NAME) // Change APP_NAME if you want a different main_window in main()
				return *window;
		throw std::logic_error("Error in getting main_window");
	}

	/// @brief Manually implement function that can construct the windows you want
	std::unordered_set<Window *> windows_creation() noexcept
	{
		std::unordered_set<Window *> set;
		set.insert(new Window(APP_NAME, WINDOW_WIDTH, WINDOW_HEIGHT));
		return set;
	}

	void resize_window(int width, int height)
	{
		// Might have to also update the camera with new window width and height
		glViewport(0, 0, width, height);
	}
}

int main(int const argc, char const *const *const argv)
{
	Error::setup_segfault_signalhandler();

	if (argc != 1)
		std::invalid_argument("This program should not be given arguments.");

	SdlHandler sdlhandler{
		std::function<std::unordered_set<Window *>()>(windows_creation),
		{CLEAR_COLOR_R, CLEAR_COLOR_G, CLEAR_COLOR_B, CLEAR_COLOR_A}};

	Window &main_window = get_main_window(sdlhandler);

	SDL_Event e;
	bool quit = false;
	while (!quit)
	{
		while (SDL_PollEvent(&e))
		{
			if (e.type == SDL_KEYDOWN && e.key.keysym.sym == SDLK_ESCAPE)
			{
				exit(EXIT_SUCCESS);
			}
			else if (e.type == SDL_WINDOWEVENT)
			{
				resize_window(e.window.data1, e.window.data2);
			}
			else if (e.type == SDL_QUIT)
			{
				quit = true;
			}
		}
		sdlhandler.clear();
		for (Window *window : sdlhandler.windows)
			window->swap();
	}
	return EXIT_SUCCESS;
}
