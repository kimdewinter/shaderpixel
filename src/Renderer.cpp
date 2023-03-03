#include "Renderer.h"
#include "ErrorHandler.h"

namespace
{
	/// @brief uses two string-names to pair together a Shader& and Model&
	/// @param shader_name name of the shader you want to select
	/// @param model_name name of the model you want to select
	/// @param shaders map of available shaders
	/// @param models map of available models
	/// @return an optional with a pair of Shader& and Model&, so they can be rendered
	std::optional<std::pair<Shader const &, Model const &> const> assemble_pair(
		std::string const &shader_name,
		std::string const &model_name,
		std::map<std::string const, Shader const> const &shaders,
		std::map<std::string const, Model> const &models)
	{
		std::map<std::string const, Shader const>::const_iterator shader_iter = shaders.find(shader_name);
		std::map<std::string const, Model>::const_iterator model_iter = models.find(model_name);
		if (shader_iter == shaders.end() || model_iter == models.end())
			return std::nullopt;

		return std::pair<Shader const &, Model const &>(shader_iter->second, model_iter->second);
	}
}

std::vector<std::pair<Shader const &, Model const &>> Renderer::assembly_pairs(
	std::map<std::string const, Shader const> const &shaders,
	std::map<std::string const, Model> const &models) const noexcept
{
	std::vector<std::pair<Shader const &, Model const &>> shader_model_refs;
	for (std::pair<std::string const, std::string const> pair_names : this->shader_model_pairs)
	{
		auto shader_ref = shaders.find(pair_names.first);
		auto model_ref = models.find(pair_names.second);
		if (shader_ref == shaders.end() || model_ref == models.end())
		{
			ASSERT(false, "Renderer unable to find Shader or Model");
			continue;
		}
		shader_model_refs.push_back({shader_ref->second, model_ref->second});
	}
}

void Renderer::render(std::map<std::string const, Shader const> const &shaders,
					  std::map<std::string const, Model> const &models) const noexcept
{
	std::vector<std::pair<Shader const &, Model const &>> shader_model_refs;
	for (std::pair<std::string const, std::string const> pair_names : this->shader_model_pairs)
	{
		auto assembled_pair = assemble_pair(pair_names.first, pair_names.second, shaders, models);
		if (!assembled_pair)
		{
			ASSERT(false, "Renderer unable to find Shader or Model");
			continue;
		}
		shader_model_refs.push_back(*assembled_pair);
	}
}
