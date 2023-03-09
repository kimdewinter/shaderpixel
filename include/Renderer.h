#ifndef RENDERER_H_INCLUDED
#define RENDERER_H_INCLUDED

#include "Model.h"
#include <vector>
#include <map>

/// @brief This class holds pairs of Models and Shaders. This way each Model is associated with the Shader it needs to be drawable.
class Renderer
{
public:
	/// @param render_pairs map->first should be the name of the Shader, map->second the name of the Model
	Renderer::Renderer(std::map<std::string const, std::string const> const &render_pairs) noexcept;
	/// @brief renders the models specified in member variable "render_pairs"
	void Renderer::render() const noexcept;

private:
	/// @brief helper function that pairs Shader and Model instance refs based on member variable "render_pairs", so they can be drawn
	/// @return vector of paired Shader and Model instance refs, ready for rendering
	std::vector<std::pair<Shader const &, Model &>> assemble_pairs() noexcept;

	std::map<std::string const, Shader const> shaders;
	std::map<std::string const, Model> models;
	std::map<std::string const, std::string const> render_pairs; // first is Shader name, second is Model name
																 // std::vector<std::pair<std::string const, std::string const>> shader_model_pairs; // first is Shader name, second is Model name
};

#endif
