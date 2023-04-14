#ifndef GUI_H_INCLUDED
#define GUI_H_INCLUDED

class Gui
{
public:
	Gui(SDL_Window *const window_ptr, SDL_GLContext const context_ptr) noexcept;
	void process_event(SDL_Event const *const event) const noexcept;
};

#endif
