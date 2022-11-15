#include "main.h"
#include "SdlHandler.h"
#include "Window.h"
#include <functional>
// #include <glad/glad.h>
// #include <SDL.h>
// #include <assimp/Importer.hpp>
// #include <glm/vec3.hpp>
// #include <stb_image.h>
#include <cstdlib>

namespace
{
	std::unordered_set<Window *> windows_creation()
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
	SdlHandler sdlhandler{std::function<std::unordered_set<Window *>()>(windows_creation)};
	Window *main_window = *sdlhandler.windows.begin();
	while (1)
	{
		main_window->clear();
		main_window->swap();
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