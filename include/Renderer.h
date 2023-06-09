#ifndef RENDERER_H_INCLUDED
#define RENDERER_H_INCLUDED

#include "Model.h"
#include "Shader.h"
#include <vector>
#include <map>

/// @brief This class holds Shaders and Models, and also remembers which Shaders should be used to draw which Models
class Renderer
{
public:
	/// @param render_pair_names map->first should be the name of the Shader, map->second the name of the Model
	Renderer(
		std::map<std::string, ShaderInterface *> const &shaders,
		std::map<std::string, Model> const &models,
		std::multimap<std::string, std::string> const &render_pair_names) noexcept;
	Renderer(
		std::map<std::string, ShaderInterface *> &&shaders,
		std::map<std::string, Model> &&models,
		std::multimap<std::string, std::string> &&render_pair_names) noexcept;
	~Renderer() noexcept;
	Renderer(Renderer const &other) noexcept = delete;
	Renderer(Renderer &&other) noexcept = default;
	Renderer &operator=(Renderer const &other) noexcept = delete;
	Renderer &operator=(Renderer &&other) noexcept = default;

	/// @brief draw the models specified in member variable "render_pair_names"
	void draw_all(
		glm::mat4 const &projection_matrix,
		glm::mat4 const &view_matrix) noexcept;

	/// @return returns std::nullopt if model is not found
	std::optional<Model *> find_model(std::string const &name) noexcept;

private:
	/// @brief helper function that pairs Shaders and Models so they can be rendered, according to member variable "render_pair_names"
	/// @return vector of paired Shader and Model instance refs
	std::vector<std::pair<ShaderInterface const &, Model &>> assemble_pairs() noexcept;

	/// @brief all loaded shaders; first is the name, second is the Shader instance
	std::map<std::string, ShaderInterface *> shaders;
	/// @brief all loaded models; first is the name, second is the Model instance
	std::map<std::string, Model> models;
	/// @brief what to render each frame; first is the Shader name, second is the Model name
	std::multimap<std::string, std::string> render_pair_names;
};

#endif
