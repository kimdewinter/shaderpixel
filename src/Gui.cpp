#include "Gui.h"
#include "main.h"
#include "ErrorHandler.h"
#include <imgui.h>
#include <backends/imgui_impl_sdl2.h>
#include <backends/imgui_impl_opengl3.h>
#include <string>

void Gui::draw() noexcept
{
	ImGui_ImplOpenGL3_NewFrame();
	ImGui_ImplSDL2_NewFrame();
	ImGui::NewFrame();
	ImGui::Begin(std::string(APP_NAME).c_str());
	ImGui::InputText("string", this->selection_name_buffer, IM_ARRAYSIZE(this->selection_name_buffer));
	std::optional<Model *> model_opt = this->renderer.find_model(std::string(this->selection_name_buffer));
	if (model_opt == std::nullopt)
	{
		ImGui::Text("Selected model: none\n(no model found with that name)");
	}
	else
	{
		Model &model = **model_opt;
		ImGui::Text("Selected model: %s", model.name.data());
	}

	ImGui::End();

	ImGui::Render();
	ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
}

void Gui::select_model(std::string const &name) noexcept
{
	ASSERT(sizeof(char) * name.length() < sizeof(this->selection_name_buffer), "Entered model name too long for buffer");
	memcpy(this->selection_name_buffer, name.data(), sizeof(char) * name.length());
}

void Gui::process_event(SDL_Event const *const event) const noexcept
{
	ImGui_ImplSDL2_ProcessEvent(event);
}

Gui::Gui(
	SDL_Window *const window_ptr,
	SDL_GLContext const context_ptr,
	Renderer &renderer,
	std::string const &selected_model_name) noexcept
	: renderer(renderer)
{
	memset(this->selection_name_buffer, '\0', sizeof(this->selection_name_buffer));
	memcpy(this->selection_name_buffer, selected_model_name.data(), sizeof(char) * selected_model_name.length());
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
