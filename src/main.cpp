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
#include "Renderer.h"
#include <imgui.h>
#include <backends/imgui_impl_sdl2.h>
#include <backends/imgui_impl_opengl3.h>

/// @brief the configuration namespace is where you manually change what you want the gameworld to be like, note that there are also defines that can be set in main.h
namespace Configuration
{
	/// @brief here you set up the gameworld itself
	namespace WorldCreation
	{
		/// @brief here you define what shaders you want to load, and from what files, names must be unique
		/// @return a map<string, Shader>, where the string is a name to identify that Shader
		std::map<std::string, Shader> load_shaders() noexcept
		{
			std::map<std::string, Shader> shaders;
			shaders.insert({"standard_shader", Shader("standard_shader", "projection", "view", "model", "resources/standard_shader.vert", "resources/standard_shader.frag")});
			// shaders.insert({"standard_shader", Shader("standard_shader2", "projection", "view", "model", "resources/standard_shader.vert", "resources/standard_shader.frag")});
			return shaders;
		}

		/// @brief here you define what models you want to load, names must be unique, path must not end in a slash
		std::map<std::string, Model> load_models() noexcept
		{
			std::map<std::string, Model> models;
			models.insert(std::pair<std::string, Model>{"backpack", Model("backpack", "resources/backpack/backpack.obj", {0.0f, 0.0f, -10.0f})});
			models.insert(std::pair<std::string, Model>{"pillar", Model("pillar", "resources/Pillar/LP_Pillar_Textured.obj", {0.0f, 0.0f, -20.0f})});
			models.insert(std::pair<std::string, Model>{"pedestal", Model("pedestal", "resources/pedestal/10421_square_pedastal_iterations-2.obj", {0.0f, 10.0f, -30.0f}, glm::quat({0.0f, 0.0f, 0.0f}), {0.1f, 0.1f, 0.1f})});
			models.insert(std::pair<std::string, Model>{"terrain", Model("terrain", "resources/terrain/terrain.obj", {0.0f, -10.0f, 0.0f})});

			return models;
		}

		/// @brief assign which Shaders should render which Models, creating a map of pairings that should be rendered each frame
		/// @return first string is Shader name, second string is Model name
		std::multimap<std::string, std::string> pair_shader_and_model_names() noexcept
		{
			std::multimap<std::string, std::string> name_pairings;
			name_pairings.insert(std::pair<std::string, std::string>{"standard_shader", "backpack"});
			name_pairings.insert(std::pair<std::string, std::string>{"standard_shader", "pillar"});
			name_pairings.insert(std::pair<std::string, std::string>{"standard_shader", "pedestal"});
			name_pairings.insert(std::pair<std::string, std::string>{"standard_shader", "terrain"});
			return name_pairings;
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
	// if segfaulting, this'll dump a stacktrace into cerr
	Error::setup_segfault_signalhandler();

	// check validity of program's input
	ASSERT(argc == 1, "This program should not be given arguments.");

	// start up SDL and OpenGL, create window via the use of function pointer, and make window context current
	SdlHandler sdl_handler{&Configuration::Technical::window_creation, Configuration::Technical::get_clear_colors()};

	EventHandler event_handler;
	Camera camera({0.0f, 0.0f, 0.0f});
	Clock clock;

	// load shaders and models to render
	Renderer renderer(
		Configuration::WorldCreation::load_shaders(),
		Configuration::WorldCreation::load_models(),
		Configuration::WorldCreation::pair_shader_and_model_names());

	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	ImGuiIO &io = ImGui::GetIO();
	(void)io;
	io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;
	// io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;
	ImGui::StyleColorsDark();
	// ImGui::StyleColorsLight();1
	ImGui_ImplSDL2_InitForOpenGL(sdl_handler.window->get_window_ptr(), sdl_handler.window->get_context());
	ImGui_ImplOpenGL3_Init(GLSL_VERSION);

	while (!event_handler.get_should_quit())
	{
		// UPDATE CLOCK
		clock.update_clock(); // gets delta time since last update

		// INPUT HANDLING
		{
			// poll and handle events (including input)
			event_handler.handle_all_events(sdl_handler, camera, clock);
		}

		// RENDERING
		{
			sdl_handler.window->make_current(); // ensure window context is made current so it can be acted upon
			sdl_handler.window->clear();		// clear the buffer so we can start composing a new frame

			ImGui_ImplOpenGL3_NewFrame();
			ImGui_ImplSDL2_NewFrame();
			ImGui::NewFrame();

			ImGui::Begin("Hello, world!");
			static int counter = 0;
			if (ImGui::Button("Button"))
				counter++;
			ImGui::End();
			ImGui::Render();
			ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

			// tell renderer to draw all actively loaded Models, using their assigned Shaders
			renderer.draw_all(camera.get_projection_matrix(), camera.get_view_matrix());

			// swap window's buffer so that it gets rendered onto the screen
			sdl_handler.window->swap();
		}
	}
	ImGui_ImplOpenGL3_Shutdown();
	ImGui_ImplSDL2_Shutdown();
	ImGui::DestroyContext();
	std::cerr << APP_NAME << " exited normally." << std::endl;
	return EXIT_SUCCESS;
}
