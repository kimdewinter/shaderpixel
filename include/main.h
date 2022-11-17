#ifndef MAIN_H_INCLUDED
#define MAIN_H_INCLUDED

#define APP_NAME "humangl"
#define OPENGL_VERSION_MAJOR 4 // OpenGL version to use before the decimal
#define OPENGL_VERSION_MINOR 1 // OpenGL version to use after the decimal
#define DOUBLE_BUFFER 1		   // Whether to use a double buffer; set to 0 or 1, defaults to 1
#define DEPTH_SIZE 24		   // Min number of bits in depth buffer; defaults to 16
#define VSYNC 1				   // 0 for immediate updates, 1 for updates synched with vertical retrace, -1 for adaptive vsync
#define WINDOW_WIDTH 1280
#define WINDOW_HEIGHT 900
#define CLEAR_COLOR_R 0.0f // Red
#define CLEAR_COLOR_G 0.0f // Green
#define CLEAR_COLOR_B 0.0f // Blue
#define CLEAR_COLOR_A 1.0f // Alpha

#include <glad/glad.h>
#include <SDL.h>
// #include <SDL_image.h>

enum ReturnCode
{
	SUCCESS,
	FAILURE
};

#endif
