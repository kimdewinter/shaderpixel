#include "Renderer.h"
#include "ErrorHandler.h"
#include <utility>

std::vector<std::pair<Shader const &, Model &>> Renderer::assemble_pairs() noexcept
{
	std::vector<std::pair<Shader const &, Model &>> render_pair_refs;
	for (std::pair<std::string const, std::string const> name_pair : this->render_pair_names)
	{
		auto shader_ref = this->shaders.find(name_pair.first);
		auto model_ref = this->models.find(name_pair.second);
		if (shader_ref == shaders.end() || model_ref == models.end())
		{
			ASSERT(false, "Renderer unable to find Shader or Model");
			continue;
		}
		render_pair_refs.push_back({shader_ref->second, model_ref->second});
	}
	return render_pair_refs;
}

void Renderer::draw_all(
	glm::mat4 const &projection_matrix,
	glm::mat4 const &view_matrix) noexcept
{
	// create queue of Shader-Model pairs to render
	std::vector<std::pair<Shader const &, Model &>> render_pair_refs = this->assemble_pairs();

	for (std::pair<Shader const &, Model &> ref_pair : render_pair_refs)
	{
		ref_pair.first.use();									 // might be more elegant to call "use" once on a Shader and execute all it's draws
		ref_pair.first.set_projection_matrix(projection_matrix); // unlikely it needs to be set again every frame
		ref_pair.first.set_view_matrix(view_matrix);
		ref_pair.first.set_model_matrix(ref_pair.second.get_model_matrix());
		ref_pair.second.draw(ref_pair.first);
	}
}

Renderer::Renderer(
	std::map<std::string const, Shader const> const &shaders,
	std::map<std::string const, Model> const &models,
	std::map<std::string const, std::string const> const &render_pair_names) noexcept
	: shaders(shaders),
	  models(models),
	  render_pair_names(render_pair_names)
{
}
