#include "main.h"
#include "Camera.h"
#include "Clock.h"
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
#include "Renderer.h"

/// @brief the configuration namespace is where you manually change what you want the gameworld to be like, note that there are also defines that can be set in main.h
namespace Configuration
{
	/// @brief here you set up the gameworld itself
	namespace WorldCreation
	{
		/// @brief here you define what shaders you want to load, and from what files, names must be unique
		/// @return a map<string, Shader>, where the string is a name to identify that Shader
		std::map<std::string const, Shader const> const load_shaders() noexcept
		{
			std::map<std::string const, Shader const> shaders;
			shaders.insert({"standard_shader", Shader("standard_shader", "projection", "view", "model", "resources/standard_shader.vert", "resources/standard_shader.frag")});
			return shaders;
		}

		/// @brief here you define what models you want to load, names must be unique, path must not end in a slash
		std::map<std::string const, Model> load_models() noexcept
		{
			std::map<std::string const, Model> models;
			models.insert({"backpack", Model("resources/backpack/backpack.obj", {0.0f, -1.0f, 0.0f})});
			return models;
		}
	}

	/// @brief here are some more technical settings
	namespace Technical
	{
		/// @brief manually implement function that can construct the window you want
		/// @return Window pointer or NULL in case of failure
		Window *window_creation() noexcept
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
	Renderer renderer{{{"standard_shader", "backpack"},
					   {"other_shader", "other_model"}}};

	// if segfaulting, this'll dump a stacktrace into cerr
	Error::setup_segfault_signalhandler();

	// check validity of program's input
	ASSERT(argc == 1, "This program should not be given arguments.");

	// start up SDL and OpenGL, create window via the use of function pointer, and make window context current
	SdlHandler sdl_handler{&Configuration::Technical::window_creation, Configuration::Technical::get_clear_colors()};

	EventHandler event_handler;
	Camera camera({0.0f, 0.0f, 10.0f});
	Clock clock;

	// load shaders and models
	std::map<std::string const, Shader const> const shaders = Configuration::WorldCreation::load_shaders();
	std::map<std::string const, Model> models = Configuration::WorldCreation::load_models();

	while (!event_handler.get_should_quit())
	{
		// UPDATE CLOCK
		clock.update_clock(); // gets delta time since last update

		// INPUT HANDLING
		{
			// poll and handle events (including input)
			event_handler.handle_all_events(sdl_handler, camera, clock);
		}

		// Set projection matrix (unlikely to be needing to be recalculated each cycle)
		glm::mat4 projection_matrix = glm::perspective(
			glm::radians(45.0f),
			(float)WINDOW_WIDTH / (float)WINDOW_HEIGHT,
			0.1f,
			100.0f);
		for (auto shader : shaders)
			shader.second.set_projection_matrix(projection_matrix);

		// RENDERING
		{
			sdl_handler.window->make_current(); // ensure window context is made current so it can be acted upon
			sdl_handler.window->clear();		// clear the buffer so we can start composing a new frame

			// set view matrix for shaders
			glm::mat4 view_matrix = camera.get_view_matrix();
			for (auto shader : shaders)
				shader.second.set_view_matrix(view_matrix);

			// draw models
			for (auto model : models)
				model.second.draw()

					glm::mat4 model_matrix = glm::mat4(1.0f);
			model_matrix = glm::translate(model_matrix, glm::vec3({0.0f, 0.0f, 0.0f}));
			glm::quat rotation = glm::quat(glm::vec3(glm::radians(20.0f), glm::radians(30.0f), glm::radians(15.0f)));
			model_matrix = model_matrix * glm::mat4(rotation);
			model_matrix = glm::scale(model_matrix, glm::vec3(1.0f, 1.0f, 1.0f));
			shaders.at("standard_shader").set_model_matrix(model_matrix);
			models.at("backpack").draw(shaders.at("standard_shader"));

			// swap window's buffer so that it gets rendered onto the screen
			sdl_handler.window->swap();
		}
	}
	std::cerr << APP_NAME << " exited normally." << std::endl;
	return EXIT_SUCCESS;
}

// TASKS:
// 2. abstract shader uniform setting away into Configuration::Technical
// 3. abstract away model drawing in the same way
// 4. make it so multiple models can be loaded and displayed in specific starting positions
