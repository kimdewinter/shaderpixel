#include "Renderer.h"
#include "ErrorHandler.h"
#include <utility>

std::vector<std::pair<std::unique_ptr<ShaderInterface> const &, Model &>> Renderer::assemble_pairs() noexcept
{
	std::vector<std::pair<std::unique_ptr<ShaderInterface> const &, Model &>> render_pair_refs;
	for (auto &name_pair : this->render_pair_names)
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

std::optional<Model *> Renderer::find_model(std::string const &name) noexcept
{
	std::map<std::string, Model>::iterator model_iter = this->models.find(name);
	if (model_iter == this->models.end())
		return std::nullopt;
	return &model_iter->second;
}

void draw_one(
	glm::mat4 const &projection_matrix,
	glm::mat4 const &view_matrix,
	std::unique_ptr<ShaderInterface> const &shader,
	Model &model) noexcept
{
	ASSERT(false, "draw_one() called on unimplemented ShaderInterface-derivation");
}

void Renderer::draw_all(
	glm::mat4 const &projection_matrix = glm::mat4(1.0f),
	glm::mat4 const &view_matrix = glm::mat4(1.0f)) noexcept
{
	std::vector<std::pair<std::unique_ptr<ShaderInterface> const &, Model &>> pair_vec = this->assemble_pairs();
	for (auto pair_iter = pair_vec.begin(); pair_iter < pair_vec.end(); pair_iter++)
		pair_iter->first->draw(pair_iter->second, view_matrix, projection_matrix);
}

Renderer::Renderer(
	std::map<std::string, std::unique_ptr<ShaderInterface>> const &shaders,
	std::map<std::string, Model> const &models,
	std::multimap<std::string, std::string> const &render_pair_names) noexcept : shaders(shaders),
																				 models(models),
																				 render_pair_names(render_pair_names)
{
}

Renderer::Renderer(
	std::map<std::string, std::unique_ptr<ShaderInterface>> &&shaders,
	std::map<std::string, Model> &&models,
	std::multimap<std::string, std::string> &&render_pair_names) noexcept : shaders(std::move(shaders)),
																			models(std::move(models)),
																			render_pair_names(std::move(render_pair_names))
{
}
