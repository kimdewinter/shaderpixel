#include "Renderer.h"
#include "ErrorHandler.h"
#include <utility>

std::vector<std::pair<Shader const &, Model &>> Renderer::assemble_pairs() noexcept
{
	std::vector<std::pair<Shader const &, Model &>> shader_model_refs;
	for (std::pair<std::string const, std::string const> name_pair : this->render_pairs)
	{
		auto shader_ref = this->shaders.find(name_pair.first);
		auto model_ref = this->models.find(name_pair.second);
		if (shader_ref == shaders.end() || model_ref == models.end())
		{
			ASSERT(false, "Renderer unable to find Shader or Model");
			continue;
		}
		shader_model_refs.push_back({shader_ref->second, model_ref->second});
	}
	return shader_model_refs;
}

void Renderer::render() const noexcept
{
	std::vector<std::pair<Shader const &, Model &>> shader_model_refs = this->assemble_pairs();

	// PLACEHOLDER: set up shader matrices and draw

	// draw each Model with the help of it's assigned Shader
	for (std::pair<Shader const &, Model const &> pair : shader_model_refs)
		pair.second.draw(pair.first);
}

Renderer::Renderer(std::vector<std::pair<std::string const, std::string const>> const &render_pairs) noexcept
	: render_pairs(render_pairs)
{
}
