#ifndef SDLHANDLER_H_INCLUDED
#define SDLHANDLER_H_INCLUDED

#include "Window.h"
#include <SDL.h>
#include <unordered_set>
#include <functional>

class SdlHandler
{
public:
	SdlHandler(std::function<std::unordered_set<Window *>()> windows_creation_after_sdl_init);
	~SdlHandler() noexcept;
	void add_window(Window *window);
	void remove_window(Window *window);

	std::unordered_set<Window *> windows = {};
};

#endif
