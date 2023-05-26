#ifndef SHADER_H_INCLUDED
#define SHADER_H_INCLUDED

#include <string>
#include <glad/glad.h>
#include <glm/mat4x4.hpp>

/// @brief abstract base class to create shaders from, which might need additional Uniforms to be added
class ShaderInterface
{
public:
	ShaderInterface(
		std::string const &vertex_path,
		std::string const &fragment_path,
		std::string const &geometry_path = {}) noexcept;
	~ShaderInterface() noexcept;
	GLuint get_id() const noexcept;
	void use() const noexcept;
	virtual void apply_uniforms() const noexcept = 0;

protected:
	GLuint id; // id that OpenGL knows the shader program by
};

/// @brief a ShaderInterface-derived class can own one or more instances of this to handle setting of uniforms
/// @tparam T the type of uniform (can be ints, floats, and vectors/matrices of these)
template <typename T>
class Uniform
{
public:
	/// @param p const reference to caller (which should own this Uniform)
	/// @param name what the uniform is called in the shader's source code
	Uniform(ShaderInterface const &p, std::string const &name) noexcept;
	// ~Uniform() noexcept = default;
	// Uniform(Uniform const &orig) noexcept = default;

	/// @brief before you call this, first call glUseProgram()
	/// @param p the ShaderInterface that owns this Uniform
	/// @param value value to set to the uniform in OpenGL
	void set(T const &value) const noexcept;

private:
	GLint const location;
};

class StandardShader : public ShaderInterface
{
public:
	StandardShader(
		std::string const &vertex_path,
		std::string const &fragment_path,
		std::string const &geometry_path = {}) noexcept;
	void apply_uniforms() const noexcept;

	Uniform<glm::mat4> modelview_matrix;
	// Uniform<glm::mat4> const projection_matrix;
};

#endif
