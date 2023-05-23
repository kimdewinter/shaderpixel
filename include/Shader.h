#ifndef SHADER_H_INCLUDED
#define SHADER_H_INCLUDED

#include <glm/mat4x4.hpp>
#include <string>

class AstractShader
{
public:
	std::string const name = {};

	/// @brief this function should call "glUseProgram() and then "apply_uniforms()"
	virtual void use_and_apply_uniforms() const noexcept = 0;

	// UNIFORM STAGING FUNCTIONS
	// function should not send value to shader program on GPU yet; for that, call "use_and_apply_uniforms()" afterwards
	// function should have a parameter of "pair<name, value>",
	// where "name" is uniform name in shader source, "value" is value to apply to uniform
	void prepare_mvp_matrix(std::pair<std::string, glm::mat4> mvp_matrix) const noexcept;

protected:
	unsigned int id; // OpenGL's identifier for the shader

	virtual void apply_uniforms() const noexcept = 0;

	// UNIFORM STAGING DATA
	// first is uniform name in shader source, second is value to apply to uniform
	std::pair<std::string, glm::mat4> mvp_matrix = {{}, glm::mat4()};
};

class Shader
{
public:
	/// @param name identifier of this instance of the class
	/// @param modelview_uniform_name name used to help OpenGL identify the model-view matrix uniform
	/// @param projection_uniform_name name used to help OpenGL identify the projection-matrix uniform
	/// @param vertex_path where to find the vertex shader
	/// @param fragment_path where to find the fragment shader
	/// @param geometry_path where to find the geometry shader, if provided
	Shader(
		std::string const &name,
		std::string const &modelview_uniform_name,
		std::string const &projection_uniform_name,
		std::string const &vertex_path,
		std::string const &fragment_path,
		std::string const &geometry_path = {}) noexcept;
	void use() const noexcept;
	unsigned int get_opengl_id() const noexcept;
	void set_modelview_matrix(glm::mat4 const &value) const noexcept;
	void set_projection_matrix(glm::mat4 const &value) const noexcept;
	void set_view_matrix(glm::mat4 const &value) const noexcept;
	void set_model_matrix(glm::mat4 const &value) const noexcept;

	std::string const name;

private:
	void set_uniform_int(std::string const &name, int const value) const noexcept;
	void set_uniform_mat4(std::string const &name, glm::mat4 const &value) const noexcept;

	unsigned int opengl_id;
	std::string const modelview_uniform_name = "u_modelview";
	std::string const projection_uniform_name = "u_projection";
};

#endif
