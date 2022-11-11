#ifndef SDLHANDLER_H_INCLUDED
#define SDLHANDLER_H_INCLUDED

#include <SDL.h>
#include <string>

class SdlHandler
{
public:
	SdlHandler(std::string const &window_name, int const window_width, int const window_height);

private:
	std::string window_name;
	int window_width;
	int window_height;
};

#endif