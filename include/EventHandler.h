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
	void handle_all_events(SdlHandler &sdl_handler, Camera &camera, Clock const &clock) noexcept;
	void handle_keyboard_event(
		SDL_Event const &event,
		SdlHandler &sdl_handler,
		Camera &camera,
		Clock const &clock) noexcept;
	void handle_mouse_motion(
		SDL_Event const &event,
		Camera &camera) noexcept;
	void handle_window_event(SDL_Event const &event, SdlHandler &sdl_handler) noexcept;

private:
	bool should_quit = false;	   // if game should quit after current render loop
	bool first_mouse_event = true; // should become false after a mouse event has occurred
	float mouse_rel_x = 0;
	float mouse_rel_y = 0;
};

#endif
