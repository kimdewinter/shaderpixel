#include "EventHandler.h"
#include "ErrorHandler.h"

namespace
{
	void resize_window(Window const &current_window, Window const &event_window, SDL_Event const &event)
	{
		// Make the event-related window and context current so it can be operated on
		event_window.make_current();

		// Do the required resizing
		// Might have to also update the camera with new window width and height
		glViewport(0, 0, event.window.data1, event.window.data2);

		// Restore previously current window
		current_window.make_current();
	}
}

void EventHandler::handle_keyboard_event(SDL_Event const &event, SdlHandler &sdl_handler)
{
	switch (event.key.keysym.sym)
	{
	case SDLK_ESCAPE:
	{
		this->should_quit = true;
		break;
	}
	}
}

void EventHandler::handle_window_event(SDL_Event const &event, SdlHandler &sdl_handler)
{
	switch (event.window.event)
	{
	case SDL_WINDOWEVENT_RESIZED:
	{
		std::optional<Window *> current_window = sdl_handler.find_window_by_window_id(SDL_GetWindowID(SDL_GL_GetCurrentWindow()));
		std::optional<Window *> event_window = sdl_handler.find_window_by_window_id(event.window.windowID);
		if (event_window == std::nullopt || current_window == std::nullopt)
		{
			Error::output_error(Error::Type::WARNING, "Unable to retrieve window_id");
			break;
		}
		resize_window(**current_window, **event_window, event);
		break;
	}
	}
}

void EventHandler::handle_all_events(SdlHandler &sdl_handler)
{
	SDL_Event event;
	while (SDL_PollEvent(&event))
	{
		switch (event.type)
		{
		case SDL_KEYDOWN:
		{
			this->handle_keyboard_event(event, sdl_handler);
			break;
		}
		case SDL_QUIT:
		{
			this->should_quit = true;
			break;
		}
		case SDL_WINDOWEVENT:
		{
			this->handle_window_event(event, sdl_handler);
			break;
		}
		}
	}
}

bool const EventHandler::get_should_quit() const noexcept
{
	return this->should_quit;
}
