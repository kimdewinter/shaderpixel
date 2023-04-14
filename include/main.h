#ifndef MAIN_H_INCLUDED
#define MAIN_H_INCLUDED

#define APP_NAME "shaderpixel"
#define OPENGL_VERSION_MAJOR 4		// OpenGL version to use before the decimal
#define OPENGL_VERSION_MINOR 1		// OpenGL version to use after the decimal
#define GLSL_VERSION "#version 410" // needed by ImGui
#define DOUBLE_BUFFER 1				// whether to use a double buffer; set to 0 or 1, defaults to 1
#define DEPTH_SIZE 24				// min number of bits in depth buffer; defaults to 16
#define VSYNC 1						// 0 for immediate updates, 1 for updates synched with vertical retrace, -1 for adaptive vsync
#define WINDOW_WIDTH 1024
#define WINDOW_HEIGHT 768
#define CLEAR_COLOR_R 0.7f	 // red
#define CLEAR_COLOR_G 0.7f	 // green
#define CLEAR_COLOR_B 0.7f	 // blue
#define CLEAR_COLOR_A 1.0f	 // alpha
#define MSAA_BUFFERS 1		 // MSAA buffers (anti-aliasing)
#define MSAA_SAMPLES 8		 // MSAA samples (anti-aliasing)
#define WIREFRAME_MODE 0	 // set to 1 if you want to only the lines between vertices
#define MAX_BONE_INFLUENCE 4 // maximum amount of bones that may influence a vertex
#define CAMERA_DEFAULT_YAW -90.0f
#define CAMERA_DEFAULT_PITCH 0.0f
#define CAMERA_DEFAULT_MOVEMENT_SPEED 10.0f
#define CAMERA_DEFAULT_MOUSE_SENSITIVITY 0.1f
#define CAMERA_DEFAULT_ZOOM 45.0f
#define CAMERA_LOCKED_BY_DEFAULT 1

// things that might later be needed, but should preferrably not be included in main.h:
// #define FOV 45 // In degrees
// #define PROJECT_NEAR 0.1
// #define PROJECT_FAR 100.0
// #include <glad/glad.h>
// #include <SDL.h>
// #include <assimp/Importer.hpp>
// #include <glm/vec3.hpp>
// #include <stb_image.h>
// SDL_SetRelativeMouseMode(SDL_TRUE);
// glEnable(GL_BLEND);
// glEnable(GL_CULL_FACE);
// glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

enum ReturnCode
{
	SUCCESS,
	FAILURE
};

#endif
