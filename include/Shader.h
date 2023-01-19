#ifndef SHADER_H_INCLUDED
#define SHADER_H_INCLUDED

#include <glm/glm.hpp>

class Shader
{
public:
	Shader(
		char const *const vertex_path,
		char const *const fragment_path,
		char const *const geometry_path = NULL);
	void use() const noexcept;
	unsigned int get_id() const noexcept;
	void set_uniform_int(std::string const &name, int const value) const noexcept;
	void set_uniform_mat4(std::string const &name, glm::mat4 const &value) const noexcept;

private:
	unsigned int id;
};

#endif
