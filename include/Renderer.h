#ifndef RENDERER_H_INCLUDED
#define RENDERER_H_INCLUDED

#include "Model.h"
#include <vector>
#include <map>

/// @brief This class holds pairs of Models and Shaders. This way each Model is associated with the Shader it needs to be drawable.
class Renderer
{
public:
	Renderer(std::vector<std::pair<std::string const, std::string const>> const &shader_model_pairs) noexcept;
	/// @brief renders the models specified in member variable "shader_model_pairs"
	/// @param shaders string must match the name in member var "shader_model_pairs"
	/// @param models string must match the name in member var "shader_model_pairs"
	void Renderer::render(
		std::map<std::string const, Shader const> const &shaders,
		std::map<std::string const, Model> const &models) const noexcept;

private:
	/// @brief helper function that pairs Shader and Model instance refs based on member variable shader_model_pairs, so they can be drawn
	/// @param shaders string must match the name in member var "shader_model_pairs"
	/// @param models string must match the name in member var "shader_model_pairs"
	/// @return vector of paired Shader and Model instance refs, ready for rendering
	std::vector<std::pair<Shader const &, Model const &>> assemble_pairs(
		std::map<std::string const, Shader const> const &shaders,
		std::map<std::string const, Model> const &models) const noexcept;

	std::vector<std::pair<std::string const, std::string const>> shader_model_pairs; // first is Shader name, second is Model name
};

#endif
