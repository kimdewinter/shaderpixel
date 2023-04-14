#include "EventHandler.h"
#include "ErrorHandler.h"
#include "Gui.h"

namespace
{
	/*
	void resize_window(Window const &current_window, Window const &event_window, SDL_Event const &event)
	{
		// make the event-related window and context current so it can be operated on
		event_window.make_current();

		// do the required resizing
		// might have to also update the camera with new window width and height
		glViewport(0, 0, event.window.data1, event.window.data2);

		// restore previously current window
		current_window.make_current();
	}
	*/
}

void EventHandler::toggle_camera_lock() noexcept
{
	if (this->camera_locked)
	{
		this->camera_locked = false;
		SDL_SetRelativeMouseMode(SDL_TRUE);
	}
	else
	{
		this->camera_locked = true;
		SDL_SetRelativeMouseMode(SDL_FALSE);
	}
}

void EventHandler::handle_keyboard_event(
	SDL_Event const &event,
	SdlHandler &sdl_handler,
	Camera &camera,
	Clock const &clock) noexcept
{
	switch (event.key.keysym.sym)
	{
	case SDLK_ESCAPE:
		this->should_quit = true;
		break;
	case SDLK_w:
		if (!this->camera_locked)
			camera.move_camera(Camera::Direction::FORWARD, clock.get_time_delta());
		break;
	case SDLK_s:
		if (!this->camera_locked)
			camera.move_camera(Camera::Direction::BACKWARD, clock.get_time_delta());
		break;
	case SDLK_a:
		if (!this->camera_locked)
			camera.move_camera(Camera::Direction::LEFT, clock.get_time_delta());
		break;
	case SDLK_d:
		if (!this->camera_locked)
			camera.move_camera(Camera::Direction::RIGHT, clock.get_time_delta());
		break;
	case SDLK_r:
		if (!this->camera_locked)
			camera.move_camera(Camera::Direction::UP, clock.get_time_delta());
		break;
	case SDLK_f:
		if (!this->camera_locked)
			camera.move_camera(Camera::Direction::DOWN, clock.get_time_delta());
		break;
	case SDLK_q:
		if (!this->camera_locked)
			camera.roll_camera(Camera::Direction::LEFT, clock.get_time_delta());
		break;
	case SDLK_e:
		if (!this->camera_locked)
			camera.roll_camera(Camera::Direction::RIGHT, clock.get_time_delta());
		break;
	case SDLK_c:
		this->toggle_camera_lock();
		break;
	}
}

void EventHandler::handle_mouse_motion(SDL_Event const &event, Camera &camera) noexcept
{
	if (this->first_mouse_event)
	{
		this->first_mouse_event = false;
		this->mouse_rel_x = 0.0f;
		this->mouse_rel_y = 0.0f;
	}
	else
	{
		this->mouse_rel_x = static_cast<float>(event.motion.xrel);
		this->mouse_rel_y = static_cast<float>(event.motion.yrel);
	}
	if (!this->camera_locked)
	{
		camera.pan_camera(this->mouse_rel_x, this->mouse_rel_y, true);
	}
}

void EventHandler::handle_window_event(SDL_Event const &event, SdlHandler &sdl_handler) noexcept
{
	switch (event.window.event)
	{
		/*
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
		*/
	}
}

void EventHandler::handle_all_events(
	SdlHandler &sdl_handler,
	Camera &camera,
	Clock const &clock,
	Gui &gui) noexcept
{
	SDL_Event event;
	while (SDL_PollEvent(&event))
	{
		gui.process_event(&event);
		switch (event.type)
		{
		case SDL_KEYDOWN:
		{
			this->handle_keyboard_event(event, sdl_handler, camera, clock);
			break;
		}
		case SDL_MOUSEMOTION:
		{
			this->handle_mouse_motion(event, camera);
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

bool EventHandler::get_should_quit() const noexcept
{
	return this->should_quit;
}
