#include "main.h"
#include "ErrorHandler.h"
#include "EventHandler.h"
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

int main(int const argc, char const *const *const argv)
{
	// If segfaulting, this'll dump a stacktrace into cerr
	Error::setup_segfault_signalhandler();

	// Check validity of program's input
	if (argc != 1)
		Error::output_error(Error::Type::FATAL, "This program should not be given arguments.");

	// Start up SDL and OpenGL, create windows via the use of function pointers
	SdlHandler sdl_handler{&Configuration::Technical::windows_creation, Configuration::Technical::get_clear_colors()};

	// Select which window is to be the main window
	Window &main_window = Configuration::Technical::get_main_window(sdl_handler);

	EventHandler event_handler;
	while (!event_handler.get_should_quit())
	{
		// Poll and handle events (including input)
		event_handler.handle_all_events(sdl_handler);

		// Clear the buffer so we can start composing a new frame
		sdl_handler.clear();

		// Swap each window's buffer so that it gets rendered onto the screen
		for (Window *window : sdl_handler.windows)
			window->swap();
	}
	std::cerr << APP_NAME << " exited normally." << std::endl;
	return EXIT_SUCCESS;
}
