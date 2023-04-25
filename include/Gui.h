#ifndef GUI_H_INCLUDED
#define GUI_H_INCLUDED

#include <SDL.h>
#include "Renderer.h"

class Gui
{
public:
	Gui(
		SDL_Window *const window_ptr,
		SDL_GLContext const context_ptr,
		Renderer &renderer,
		std::string const &selected_model_name = {}) noexcept;
	~Gui() noexcept;
	void process_event(SDL_Event const *const event) const noexcept;
	void draw() noexcept;
	void select_model(std::string const &model_name) noexcept;

private:
	Renderer &renderer;
	char selection_name_buffer[256];
};

#endif
