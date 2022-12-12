#ifndef EVENT_HANDLER_H_INCLUDED
#define EVENT_HANDLER_H_INCLUDED

#include "main.h"
#include "SdlHandler.h"

class EventHandler
{
public:
	bool const get_should_quit() const noexcept;
	void handle_all_events(SdlHandler &sdl_handler);
	void handle_keyboard_event(SDL_Event const &event, SdlHandler &sdl_handler);
	void handle_window_event(SDL_Event const &event, SdlHandler &sdl_handler);

private:
	bool should_quit = false; // if game should quit after current render loop
};

#endif
