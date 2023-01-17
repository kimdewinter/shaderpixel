#include "main.h"
#include "ErrorHandler.h"
#include "EventHandler.h"
#include "SdlHandler.h"
#include "Shader.h"
#include "Window.h"
#include "Model.h"
#include <functional>
#include <cstdlib>
#include <map>

/// @brief the configuration namespace is where you manually change what you want the gameworld to be like, note that there are also defines that can be set in main.h
namespace Configuration
{
	/// @brief here you set up the gameworld itself
	namespace WorldCreation
	{
		/// @brief here you define what shaders you want to load, and from what files, names must be unique
		/// @return a map<string, Shader>, where the string is a name to identify that Shader
		std::map<std::string, Shader> const load_shaders()
		{
			std::map<std::string, Shader> shaders;
			shaders.insert({"standard_shader", Shader("resources/standard_shader.vert", "resources/standard_shader.frag")});
			return shaders;
		}

		/// @brief here you define what models you want to load, names must be unique, path must not end in a slash
		std::map<std::string, Model> load_models()
		{
			std::map<std::string, Model> models;
			models.insert({"backpack", Model("resources/backpack/backpack.obj")});
			return models;
		}
	}

	/// @brief here are some more technical settings
	namespace Technical
	{
		/// @brief manually implement function that can construct the window you want
		/// @return Window pointer or throw in case of failure
		Window *window_creation()
		{
			return new Window(APP_NAME, WINDOW_WIDTH, WINDOW_HEIGHT);
		}

		/// @brief here you can decide what the default clear color is (this is the background color after calling glClear())
		std::array<GLfloat, 4> const get_clear_colors() noexcept
		{
			return {CLEAR_COLOR_R, CLEAR_COLOR_G, CLEAR_COLOR_B, CLEAR_COLOR_A};
		}
	}
}

int main(int const argc, char const *const *const argv)
{
	// if segfaulting, this'll dump a stacktrace into cerr
	Error::setup_segfault_signalhandler();

	// check validity of program's input
	if (argc != 1)
		Error::output_error(Error::Type::FATAL, "This program should not be given arguments.");

	// start up SDL and OpenGL, create window via the use of function pointer, and make window context current
	SdlHandler sdl_handler{&Configuration::Technical::window_creation, Configuration::Technical::get_clear_colors()};

	EventHandler event_handler;

	// load shaders and models
	std::map<std::string, Shader> const shaders = Configuration::WorldCreation::load_shaders();
	std::map<std::string, Model> models = Configuration::WorldCreation::load_models();
	while (!event_handler.get_should_quit())
	{
		// poll and handle events (including input)
		event_handler.handle_all_events(sdl_handler);

		// ensure window context is made current so it can be acted upon
		sdl_handler.window->make_current();

		// clear the buffer so we can start composing a new frame
		sdl_handler.window->clear();

		// use shaders and set uniforms
		shader.use();
		// FINISH THIS

		// draw models
		// FINISH THIS

		// swap window's buffer so that it gets rendered onto the screen
		sdl_handler.window->swap();
	}
	std::cerr << APP_NAME << " exited normally." << std::endl;
	return EXIT_SUCCESS;
}
