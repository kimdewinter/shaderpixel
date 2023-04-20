#include "Owner.h"

ModelOwner::ModelOwner(
	std::string const name,
	Model model,
	Renderer &renderer,
	Gui &gui) noexcept
	: name(std::move(name)),
	  model(std::move(model)),
	  renderer(renderer),
	  gui(gui)
{
}

ModelOwner::~ModelOwner() noexcept
{
	// signal to model users that to delete their references/pointers to this ModelOwner
	this->renderer.signal_model_removal(this);
	this->gui.signal_model_removal(this);

	// owned objects should be either freed by stack-collapse or be manually deleted hereunder
	for (this->referrers.signal_deletion();)
}

ShaderOwner::ShaderOwner(std::string const name, Shader shader, Renderer &renderer) noexcept
	: name(std::move(name)),
	  shader(std::move(shader)),
	  renderer(renderer)
{
}

ShaderOwner::~ShaderOwner() noexcept
{
	// signal to shader users to delete their references/pointers to this ModelOwner
	this->renderer.signal_shader_removal(this);

	// owned objects should be either freed by stack-collapse or be manually deleted hereunder
}
