#include "Renderer.h"
#include "ErrorHandler.h"
#include <utility>

std::vector<std::pair<Shader &, ModelOwner &>> Renderer::assemble_pairs() const noexcept
{
	std::vector<std::pair<Shader &, ModelOwner &>> render_pair_refs;
	for (std::pair<std::string, std::string> name_pair : this->render_pair_names)
	{
		auto shader_iter = this->shaders.find(name_pair.first);
		auto model_iter = this->models.find(name_pair.second);
		if (shader_iter == shaders.end() || model_iter == models.end())
		{
			ASSERT(false, "Renderer unable to find Shader or ModelOwner");
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
	// create queue of Shader-ModelOwner pairs to render
	std::vector<std::pair<Shader &, ModelOwner &>> render_pair_refs = this->assemble_pairs();

	for (std::pair<Shader &, ModelOwner &> ref_pair : render_pair_refs)
	{
		ref_pair.first.use();									 // might be more elegant to call "use" once on a Shader and execute all it's draws
		ref_pair.first.set_projection_matrix(projection_matrix); // unlikely it needs to be set again every frame
		ref_pair.first.set_view_matrix(view_matrix);
		ref_pair.first.set_model_matrix(ref_pair.second.get_model_matrix());
		ref_pair.second.draw(ref_pair.first);
	}
}

Renderer::Renderer(
	std::map<std::string, Shader> const &shaders,
	std::map<std::string, ModelOwner> const &models,
	std::multimap<std::string, std::string> const &render_pair_names) noexcept
	: shaders(shaders),
	  models(models),
	  render_pair_names(render_pair_names)
{
}

Renderer::Renderer(
	std::map<std::string, Shader> &&shaders,
	std::map<std::string, ModelOwner> &&models,
	std::multimap<std::string, std::string> &&render_pair_names) noexcept
	: shaders(std::move(shaders)),
	  models(std::move(models)),
	  render_pair_names(std::move(render_pair_names))
{
}

std::optional<ModelOwner *> Renderer::find_model(std::string const &name) noexcept
{
	std::map<std::string, ModelOwner>::iterator model = this->models.find(name);
	if (model == this->models.end())
		return std::nullopt;
	return &model->second;
}

void Renderer::signal_model_removal(std::string const &model_name) noexcept
{
	// erase entry from private member map "render_pair_names"
	for (std::multimap<std::string, std::string>::iterator i = this->render_pair_names.begin(); i != this->render_pair_names.end();)
	{
		// check if the second string in this pair refers to this particular model
		if (i->second == model_name)
			i = this->render_pair_names.erase(i); // after erasing iterator to next element is returned
		else
			i++;
	}
}
