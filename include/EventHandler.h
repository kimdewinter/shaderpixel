#ifndef EVENT_HANDLER_H_INCLUDED
#define EVENT_HANDLER_H_INCLUDED

#include "Camera.h"
#include "Clock.h"
#include "main.h"
#include "SdlHandler.h"

class EventHandler
{
public:
	bool const get_should_quit() const noexcept;
	void handle_all_events(SdlHandler &sdl_handler, Camera &camera, Clock const &clock);
	void handle_keyboard_event(
		SDL_Event const &event,
		SdlHandler &sdl_handler,
		Camera &camera,
		Clock const &clock);
	void handle_mouse_motion(
		SDL_Event const &event,
		Camera &camera);
	void handle_window_event(SDL_Event const &event, SdlHandler &sdl_handler);
	bool get_mouse_moved() const noexcept;

private:
	bool should_quit = false; // if game should quit after current render loop
	bool first_mouse_event = true;
	bool mouse_moved = false;
	float mouse_rel_x = 0;
	float mouse_rel_y = 0;
};

#endif
