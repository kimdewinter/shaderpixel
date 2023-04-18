#ifndef RENDERER_H_INCLUDED
#define RENDERER_H_INCLUDED

#include "Model.h"
#include "Owner.h"
#include <vector>
#include <map>

/// @brief This class holds Shaders and Models, and also remembers which Shaders should be used to draw which Models
class Renderer
{
public:
	/// @param render_pair_names map->first should be the name of the Shader, map->second the name of the ModelOwner
	Renderer(
		std::map<std::string, Shader> const &shaders,
		std::multimap<std::string, std::string> const &render_pair_names) noexcept;
	Renderer(
		std::map<std::string, Shader> &&shaders,
		std::multimap<std::string, std::string> &&render_pair_names) noexcept;

	/// @brief draw the models specified in member variable "render_pair_names"
	void draw_all(
		glm::mat4 const &projection_matrix,
		glm::mat4 const &view_matrix) noexcept;

	std::optional<ModelOwner *> find_model(std::string const &name) noexcept;
	void signal_model_removal(std::string const &model_name) noexcept;

private:
	/// @brief helper function that pairs Shaders and Models so they can be rendered, according to member variable "render_pair_names"
	/// @return vector of paired Shader and ModelOwner instance refs
	std::vector<std::pair<Shader &, ModelOwner &>> assemble_pairs() const noexcept;

	/// @brief all loaded shaders; first is the name, second is the Shader instance
	std::map<std::string, Shader> shaders;
	/// @brief what to render each frame; first is the Shader name, second is the ModelOwner name
	// std::multimap<std::string, std::string> render_pair_names;
	std::multimap<std::string, std::string> render_pair_names;
};

#endif
