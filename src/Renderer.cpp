#include "Renderer.h"
#include "ErrorHandler.h"
#include <utility>

std::vector<std::pair<Shader &, Model &>> Renderer::assemble_pairs() noexcept
{
	std::vector<std::pair<Shader &, Model &>> render_pair_refs;
	for (std::pair<std::string, std::string> name_pair : this->render_pair_names)
	{
		auto shader_iter = this->shaders.find(name_pair.first);
		auto model_iter = this->models.find(name_pair.second);
		if (shader_iter == shaders.end() || model_iter == models.end())
		{
			ASSERT(false, "Renderer unable to find Shader or Model");
			continue;
		}
		render_pair_refs.push_back({shader_iter->second, model_iter->second});
	}
	return render_pair_refs;
}

void Renderer::draw_all(
	glm::mat4 const &projection_matrix,
	glm::mat4 const &view_matrix) noexcept
{
	// create queue of Shader-Model pairs to render
	std::vector<std::pair<Shader &, Model &>> render_pair_refs = this->assemble_pairs();

	for (std::pair<Shader &, Model &> ref_pair : render_pair_refs)
	{
		std::cout << "Currently using shader -" << ref_pair.first.name << "- to render model -" << ref_pair.second.name << "-" << std::endl;
		ref_pair.first.use();									 // might be more elegant to call "use" once on a Shader and execute all it's draws
		ref_pair.first.set_projection_matrix(projection_matrix); // unlikely it needs to be set again every frame
		ref_pair.first.set_view_matrix(view_matrix);
		ref_pair.first.set_model_matrix(ref_pair.second.get_model_matrix());
		ref_pair.second.draw(ref_pair.first);
	}
}

Renderer::Renderer(
	std::map<std::string, Shader> const &shaders,
	std::map<std::string, Model> const &models,
	std::multimap<std::string, std::string> const &render_pair_names) noexcept
	: shaders(shaders),
	  models(models),
	  render_pair_names(render_pair_names)
{
}

Renderer::Renderer(
	std::map<std::string, Shader> &&shaders,
	std::map<std::string, Model> &&models,
	std::multimap<std::string, std::string> &&render_pair_names) noexcept
	: shaders(std::move(shaders)),
	  models(std::move(models)),
	  render_pair_names(std::move(render_pair_names))
{
}
