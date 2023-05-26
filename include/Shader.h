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
		std::string const &geometry_path) noexcept;
	void use() const noexcept;
	GLuint get_id() const noexcept;
	virtual void apply_uniforms() const noexcept = 0;

protected:
	GLuint id; // id that OpenGL knows the shader program by
};

class StandardShader : public ShaderInterface
{
public:
};

/// @brief a Shader class can own one or more instances of this to handle setting of uniforms
/// @tparam T the type of uniform (can be ints, floats, and vectors/matrices of these)
template <typename T>
class Uniform
{
public:
	std::string const name = {}; // what the uniform is called in the shader's source code

	/// @brief before you call this, first call glUseProgram()
	/// @param p the ShaderInterface that owns this Uniform
	/// @param value value to set to the uniform in OpenGL
	void set(ShaderInterface const &p, T value) const noexcept;

private:
	GLint get_uniform_location(ShaderInterface const &p) const noexcept;
};

#endif
