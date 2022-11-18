#include "main.h"
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
}

int main(int const argc, char const *const *const argv)
{
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
			if (e.type == SDL_QUIT)
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

/*
#include <iostream>

#include <glad/glad.h>
#include <GLFW/glfw3.h>

void ProcessInput(GLFWwindow *window)
{
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
		glfwSetWindowShouldClose(window, true);
}

int main()
{
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	// Construct the window
	GLFWwindow *window = glfwCreateWindow(800, 600, "OpenGL Template", nullptr, nullptr);
	if (!window)
	{
		std::cout << "Failed to create the GLFW window\n";
		glfwTerminate();
	}

	glfwMakeContextCurrent(window);

	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		std::cout << "Failed to initialize GLAD\n";
		return -1;
	}

	// Handle view port dimensions
	glViewport(0, 0, 800, 600);
	glfwSetFramebufferSizeCallback(window, [](GLFWwindow *window, int width, int height)
								   { glViewport(0, 0, width, height); });

	// This is the render loop
	while (!glfwWindowShouldClose(window))
	{
		ProcessInput(window);

		// Druids are the best
		glClearColor(1.00f, 0.49f, 0.04f, 1.00f);
		glClear(GL_COLOR_BUFFER_BIT);

		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	glfwTerminate();
	return 0;
}
*/