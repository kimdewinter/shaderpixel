#ifndef SHADER_H_INCLUDED
#define SHADER_H_INCLUDED

#include <string>
#include <glad/glad.h>
#include <glm/mat4x4.hpp>
#include <vector>
#include "types.h"
#include "Mesh.h"
#include "Model.h"
#include "main.h"

#define SINGLECOLORSHADER_COLOR_R 1.0f
#define SINGLECOLORSHADER_COLOR_G 1.0f
#define SINGLECOLORSHADER_COLOR_B 1.0f
#define SINGLECOLORSHADER_COLOR_A 1.0f

/*
ShaderInterface is there as a base for individual shaders you wish to implement

Uniform is there to set the uniforms for each shader program prior to drawing

When you implement a new shader:
- derive from ShaderInterface
- add Uniforms as necessary
- add a Uniform<std::vector<Texture>> to handle texture binding
- implement a .draw() function on your new shader

Drawing is done by:
- calling YourNewShader.draw(Model const &, etc),
- your .draw() function should call .use(),
- then it should call .set() on the Uniforms,
- then for each Mesh:
	call .set() on the texture-Uniform
	bind the VAO (glBindVertexArray())
	draw elements (glDrawElements())
*/

/// @brief base class to create shaders from, which will need additional Uniforms to be added.
/// Usage: call .use(), then .set() on each Uniform/TextureUniform, then .draw()
class ShaderInterface
{
public:
	ShaderInterface(
		std::string const &vertex_path,
		std::string const &fragment_path,
		std::string const &geometry_path = {}) noexcept;
	virtual ~ShaderInterface() noexcept;
	ShaderInterface(ShaderInterface const &other) noexcept = delete;
	ShaderInterface(ShaderInterface &&other) noexcept = default;
	ShaderInterface &operator=(ShaderInterface const &other) noexcept = delete;
	ShaderInterface &operator=(ShaderInterface &&other) noexcept = default;

	GLuint get_id() const noexcept;

	void use() const noexcept;

	virtual void draw(
		Model const &model,
		glm::mat4 const &view,
		glm::mat4 const &projection) const noexcept = 0;

protected:
	GLuint id; // id that OpenGL knows the shader program by
};

/// @brief can be owned by a ShaderInterface-derived class, to handle setting of uniforms,
/// note that it does not store any kind of actual value, it just sends it to the GPU and forgets it
/// @tparam T the type of uniform (can be ints, floats, and vectors/matrices of these, as well as a std::vector<Texture>)
template <typename T>
class Uniform
{
public:
	/// @param p const reference to caller (which should own this Uniform)
	/// @param name what the uniform is called in the shader's source code; empty string if T=std::vector<Texture>
	Uniform(ShaderInterface const &p, std::string const &name) noexcept;
	~Uniform() noexcept = default;
	Uniform(Uniform const &other) = default;
	Uniform(Uniform &&other) = default;
	Uniform &operator=(Uniform const &other) noexcept = delete;
	Uniform &operator=(Uniform &&other) noexcept = delete;

	void set(T const &value) const noexcept;

private:
	GLint get_uniform_location(std::string const &name) const noexcept;

	GLuint const shader_id;		 // id of owner (a Shader of some kind)
	std::string const name = {}; // what uniform is called in shader's source code
};

class StandardShader : public ShaderInterface
{
public:
	StandardShader(
		std::string const &vertex_path,
		std::string const &fragment_path,
		std::string const &geometry_path = {}) noexcept;
	~StandardShader() noexcept = default;
	StandardShader(StandardShader const &other) noexcept = delete;
	StandardShader(StandardShader &&other) = default;
	StandardShader &operator=(StandardShader const &other) noexcept = delete;
	StandardShader &operator=(StandardShader &&other) noexcept = delete;

	void draw(
		Model const &model,
		glm::mat4 const &view,
		glm::mat4 const &projection) const noexcept;

private:
	Uniform<glm::mat4> const modelview_matrix;
	Uniform<glm::mat4> const projection_matrix;
	Uniform<std::vector<Texture>> const textures;
};

// class SingleColorShader : public ShaderInterface
// {
// public:
// 	SingleColorShader(
// 		std::string const &vertex_path,
// 		std::string const &fragment_path,
// 		std::string const &geometry_path = {}) noexcept;
// 	~SingleColorShader() noexcept = default;
// 	SingleColorShader(SingleColorShader const &other) noexcept = delete;
// 	SingleColorShader(SingleColorShader &&other) noexcept = default;
// 	SingleColorShader &operator=(SingleColorShader const &other) noexcept = delete;
// 	SingleColorShader &operator=(SingleColorShader &&other) noexcept = default;

// 	void draw(
// 		Model const &model,
// 		glm::mat4 const &view,
// 		glm::mat4 const &projection) const noexcept;

// private:
// 	Uniform<glm::mat4> const modelview_matrix = Uniform<glm::mat4>(*this, "u_modelview");
// 	Uniform<glm::mat4> const projection_matrix = Uniform<glm::mat4>(*this, "u_projection");
// 	Uniform<glm::vec4> const color = Uniform<glm::vec4>(*this, "u_color");
// };

#endif
