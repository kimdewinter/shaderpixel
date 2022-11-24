#include "main.h"
#include "ErrorHandler.h"
#include "SdlHandler.h"
#include "Window.h"
#include <functional>
#include <cstdlib>

/// @brief The configuration namespace is where you manually change what you want the gameworld to be like, note that there are also defines that can be set in main.h
namespace Configuration
{
	/// @brief Here you set up the gameworld itself
	namespace WorldCreation
	{

	}
	/// @brief Here are some more technical settings
	namespace Technical
	{
		/// @brief Here you can decide what window the main() function treats as being your main window
		Window &get_main_window(SdlHandler const &sdlhandler) noexcept
		{
			for (Window *window : sdlhandler.windows)
				if (window->get_name() == APP_NAME) // Change APP_NAME if you want a different main_window in main()
					return *window;
			Error::output_error(Error::Type::FATAL, "Error in getting main_window");
			exit(EXIT_FAILURE);
		}

		/// @brief Manually implement function that can construct the windows you want
		std::unordered_set<Window *> windows_creation() noexcept
		{
			std::unordered_set<Window *> set;
			set.insert(new Window(APP_NAME, WINDOW_WIDTH, WINDOW_HEIGHT));
			if (set.empty())
				Error::output_error(Error::Type::FATAL, "unable to create windows");
			return set;
		}

		/// @brief Here you can decide what the default clear color is (this is the background color after calling glClear())
		std::array<GLfloat, 4> const get_clear_colors() noexcept
		{
			return {CLEAR_COLOR_R, CLEAR_COLOR_G, CLEAR_COLOR_B, CLEAR_COLOR_A};
		}
	}
}

namespace
{
	void resize_window(Window const &current_window, Window const &event_window, SDL_Event const &event)
	{
		// Make the event-related window and context current so it can be operated on
		event_window.make_current();

		// Do the required resizing
		// Might have to also update the camera with new window width and height
		glViewport(0, 0, event.window.data1, event.window.data2);

		// Restore previously current window
		current_window.make_current();
	}
}

int main(int const argc, char const *const *const argv)
{
	Error::setup_segfault_signalhandler();

	if (argc != 1)
		Error::output_error(Error::Type::FATAL, "This program should not be given arguments.");

	SdlHandler sdlhandler{&Configuration::Technical::windows_creation, Configuration::Technical::get_clear_colors()};

	Window &main_window = Configuration::Technical::get_main_window(sdlhandler);

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
				std::optional<Window *> current_window = sdlhandler.find_window_by_window_id(SDL_GetWindowID(SDL_GL_GetCurrentWindow()));
				std::optional<Window *> event_window = sdlhandler.find_window_by_window_id(e.window.windowID);
				if (event_window == std::nullopt || current_window == std::nullopt)
					continue;
				resize_window(**current_window, **event_window, e);
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
