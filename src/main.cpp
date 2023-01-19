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
#include <glm/gtc/quaternion.hpp>

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
		// ensure window context is made current so it can be acted upon
		sdl_handler.window->make_current();

		// clear the buffer so we can start composing a new frame
		sdl_handler.window->clear();

		// set view and projection transformations for shaders
		glm::mat4 projection = glm::perspective(glm::radians(45.0f), (float)WINDOW_WIDTH / (float)WINDOW_HEIGHT, 0.1f, 100.0f);
		glm::mat4 view = glm::lookAt(glm::vec3(0.0f, 0.0f, 10.0f), glm::vec3(0.0f, 0.0f, 0.0f) + glm::vec3(0.0f, 0.0f, -1.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		shaders.at("standard_shader").set_uniform_mat4("projection", projection);
		shaders.at("standard_shader").set_uniform_mat4("view", view);

		// draw models
		glm::mat4 model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(0.0f, 0.0f, 0.0f));
		glm::quat rotation = glm::quat(glm::vec3(glm::radians(20.0f), glm::radians(30.0f), glm::radians(15.0f)));
		model = model * glm::mat4(rotation);
		model = glm::scale(model, glm::vec3(1.0f, 1.0f, 1.0f));
		shaders.at("standard_shader").set_uniform_mat4("model", model);
		models.at("backpack").draw(shaders.at("standard_shader"));

		// swap window's buffer so that it gets rendered onto the screen
		sdl_handler.window->swap();

		// poll and handle events (including input)
		event_handler.handle_all_events(sdl_handler);
	}
	std::cerr << APP_NAME << " exited normally." << std::endl;
	return EXIT_SUCCESS;
}
