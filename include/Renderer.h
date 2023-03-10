#ifndef RENDERER_H_INCLUDED
#define RENDERER_H_INCLUDED

#include "Model.h"
#include <vector>
#include <map>

/// @brief This class holds Shaders and Models, and also remembers which Shaders should be used to draw which Models
class Renderer
{
public:
	/// @param render_pair_names map->first should be the name of the Shader, map->second the name of the Model
	Renderer(
		std::map<std::string const, Shader const> const &shaders,
		std::map<std::string const, Model> const &models,
		std::map<std::string const, std::string const> const &render_pair_names) noexcept;
	/// @brief draw the models specified in member variable "render_pair_names"
	void draw_all(
		glm::mat4 const &projection_matrix,
		glm::mat4 const &view_matrix) noexcept;

private:
	/// @brief helper function that pairs Shaders and Models so they can be rendered, according to member variable "render_pair_names"
	/// @return vector of paired Shader and Model instance refs
	std::vector<std::pair<Shader const &, Model &>> assemble_pairs() noexcept;

	/// @brief all loaded shaders; first is the name, second is the Shader instance
	std::map<std::string const, Shader const> shaders;
	/// @brief all loaded models; first is the name, second is the Model instance
	std::map<std::string const, Model> models;
	/// @brief what to render each frame; first is the Shader name, second is the Model name
	std::map<std::string const, std::string const> render_pair_names;
};

#endif
