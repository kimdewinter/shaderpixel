#include "Gui.h"
#include "main.h"
#include <imgui.h>
#include <backends/imgui_impl_sdl2.h>
#include <backends/imgui_impl_opengl3.h>
#include <string>

void Gui::draw() const noexcept
{
	ImGui_ImplOpenGL3_NewFrame();
	ImGui_ImplSDL2_NewFrame();
	ImGui::NewFrame();

	ImGui::Begin(std::string(APP_NAME).c_str());
	static int counter = 0;
	if (ImGui::Button("Button"))
		counter++;
	ImGui::End();

	ImGui::Render();
	ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
}

void Gui::process_event(SDL_Event const *const event) const noexcept
{
	ImGui_ImplSDL2_ProcessEvent(event);
}

Gui::Gui(SDL_Window *const window_ptr, SDL_GLContext const context_ptr) noexcept
{
	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	ImGuiIO &io = ImGui::GetIO();
	(void)io;
	io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;
	// io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;
	ImGui::StyleColorsDark();
	// ImGui::StyleColorsLight();1
	ImGui_ImplSDL2_InitForOpenGL(window_ptr, context_ptr);
	ImGui_ImplOpenGL3_Init(GLSL_VERSION);
}

Gui::~Gui() noexcept
{
	ImGui_ImplOpenGL3_Shutdown();
	ImGui_ImplSDL2_Shutdown();
	ImGui::DestroyContext();
}
