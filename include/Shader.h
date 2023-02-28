#ifndef SHADER_H_INCLUDED
#define SHADER_H_INCLUDED

#include <glm/mat4x4.hpp>
#include <string>

class Shader
{
public:
	Shader(
		std::string const &projection_uniform_name,
		std::string const &view_uniform_name,
		std::string const &model_uniform_name,
		std::string const &vertex_path,
		std::string const &fragment_path,
		std::string const &geometry_path = {}) noexcept;
	void use() const noexcept;
	unsigned int get_id() const noexcept;
	void set_projection_matrix(glm::mat4 const &value) const noexcept;
	void set_view_matrix(glm::mat4 const &value) const noexcept;
	void set_model_matrix(glm::mat4 const &value) const noexcept;

private:
	unsigned int id;
	std::string projection_uniform_name;
	std::string view_uniform_name;
	std::string model_uniform_name;
	void set_uniform_int(std::string const &name, int const value) const noexcept;
	void set_uniform_mat4(std::string const &name, glm::mat4 const &value) const noexcept;
};

#endif
