#ifndef RENDERER_H_INCLUDED
#define RENDERER_H_INCLUDED

#include "Model.h"
#include <vector>
#include <map>

/// @brief This class holds pairs of Models and Shaders. This way each Model is associated with the Shader it needs to be drawable.
class Renderer
{
public:
	Renderer(std::vector<std::pair<std::string const, std::string const>> shader_model_pairs) noexcept;
	void Renderer::render(
		std::map<std::string const, Shader const> const &shaders,
		std::map<std::string const, Model> const &models) const noexcept;

private:
	std::vector<std::pair<Shader const &, Model const &>> assembly_pairs(
		std::map<std::string const, Shader const> const &shaders,
		std::map<std::string const, Model> const &models) const noexcept;

	std::vector<std::pair<std::string const, std::string const>> shader_model_pairs; // first is Shader name, second is Model name
};

#endif