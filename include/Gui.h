#ifndef GUI_H_INCLUDED
#define GUI_H_INCLUDED

#include <SDL.h>

class Gui
{
public:
	Gui(SDL_Window *const window_ptr, SDL_GLContext const context_ptr) noexcept;
	~Gui() noexcept;
	void process_event(SDL_Event const *const event) const noexcept;
	void draw() const noexcept;
};

#endif
