#include "SdlHandler.h"
#include <iostream>

SdlHandler::SdlHandler(
	std::string const &window_name,
	int const window_width,
	int const window_height) : window_name(window_name),
							   window_width(window_width),
							   window_height(window_height)
{
	if (SDL_Init(SDL_INIT_VIDEO) < 0)
	{
		std::cerr << "Error initializing SDL" << std::endl;
	}
}
