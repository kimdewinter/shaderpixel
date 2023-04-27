#ifndef SHADER_H_INCLUDED
#define SHADER_H_INCLUDED

#include <glm/mat4x4.hpp>
#include <string>

class Shader
{
public:
	/// @param name identifier of this instance of the class
	/// @param projection_uniform_name name used to help OpenGL identify the projection-matrix uniform
	/// @param view_uniform_name name used to help OpenGL identify the view-matrix uniform
	/// @param model_uniform_name name used to help OpenGL identify the model-matrix uniform
	/// @param vertex_path where to find the vertex shader
	/// @param fragment_path where to find the fragment shader
	/// @param geometry_path where to find the geometry shader, if provided
	Shader(
		std::string const &name,
		std::string const &projection_uniform_name,
		std::string const &view_uniform_name,
		std::string const &model_uniform_name,
		std::string const &vertex_path,
		std::string const &fragment_path,
		std::string const &geometry_path = {}) noexcept;
	void use(bool const disable_textures) const noexcept;
	unsigned int get_opengl_id() const noexcept;
	void set_projection_matrix(glm::mat4 const &value) const noexcept;
	void set_view_matrix(glm::mat4 const &value) const noexcept;
	void set_model_matrix(glm::mat4 const &value) const noexcept;

	std::string const name;

private:
	void set_uniform_int(std::string const &name, int const value) const noexcept;
	void set_uniform_mat4(std::string const &name, glm::mat4 const &value) const noexcept;

	unsigned int opengl_id;
	std::string const projection_uniform_name;
	std::string const view_uniform_name;
	std::string const model_uniform_name;
};

#endif
