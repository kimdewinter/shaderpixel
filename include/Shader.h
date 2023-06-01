#ifndef SHADER_H_INCLUDED
#define SHADER_H_INCLUDED

#include <string>
#include <glad/glad.h>
#include <glm/mat4x4.hpp>
#include <vector>
#include "types.h"
#include "Mesh.h"

/// @brief base class to create shaders from, which will need additional Uniforms to be added.
/// Usage: call .use(), then .set() on each Uniform/TextureUniform, then .draw()
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
	void draw(unsigned int const VAO_id, int const element_count) const noexcept;

protected:
	GLuint id; // id that OpenGL knows the shader program by
};

/// @brief can be owned by a ShaderInterface-derived class, to handle setting of uniforms
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

	/// @brief sets value to this Uniform; does not send to OpenGL yet
	void set(T const &value) const noexcept;

private:
	GLint const location;
};

/// @brief handles multiple textures, must be named like "u_texture_diffuse" (or specular/normal/height)
class TextureUniform
{
public:
	TextureUniform(unsigned int const shader_id) noexcept;
	void set(std::vector<Texture> const &textures) const noexcept;

private:
	GLint const shader_id; // id of owner
};

class StandardShader : public ShaderInterface
{
public:
	StandardShader(
		std::string const &vertex_path,
		std::string const &fragment_path,
		std::string const &geometry_path = {}) noexcept;

	// the variables hereunder must be set before .draw() is called
	Uniform<glm::mat4> modelview_matrix = Uniform<glm::mat4>(*this, "u_modelview");
	Uniform<glm::mat4> projection_matrix = Uniform<glm::mat4>(*this, "u_projection");
	TextureUniform texture_binder = TextureUniform(this->id);
};

#endif
