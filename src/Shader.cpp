#include <fstream>
#include <sstream>
#include <optional>
#include "Shader.h"
#include "ErrorHandler.h"

namespace
{
	void check_compile_errors(GLuint const id, std::string const &type)
	{
		GLint success;
		GLchar info_log[1024];
		if (type != "PROGRAM")
		{
			glGetShaderiv(id, GL_COMPILE_STATUS, &success);
			if (!success)
			{
				glGetShaderInfoLog(id, 1024, NULL, info_log);
				ASSERT(false, "Shader compilation error of type: " + type + "\n" + std::string(info_log) + "\n");
			}
		}
		else
		{
			glGetProgramiv(id, GL_LINK_STATUS, &success);
			if (!success)
			{
				glGetProgramInfoLog(id, 1024, NULL, info_log);
				ASSERT(false, "Shader linking error of type: " + type + "\n" + std::string(info_log) + "\n");
			}
		}
	}

	GLuint compile_shader(std::string const &path, GLenum const shader_type) noexcept
	{
		// open file
		std::ifstream file;
		file.open(path);
		ASSERT(file.is_open(), "compile_shader failed to open shader source file: " + path);

		// read file buffer into stringstream
		std::stringstream stream;
		stream << file.rdbuf();

		// close file handler
		file.close();

		// convert stringstream to string
		std::string const code = stream.str();

		// compile shader
		char const *code_cstr = code.c_str();
		GLuint const id = glCreateShader(shader_type);
		glShaderSource(id, 1, &code_cstr, NULL);
		glCompileShader(id);
		check_compile_errors(id, "SHADER");

		return id;
	}

	GLuint link_shaders(GLuint const vertex, GLuint const fragment, std::optional<GLuint> const &geometry)
	{
		GLuint const program = glCreateProgram();
		glAttachShader(program, vertex);
		glAttachShader(program, fragment);
		if (geometry.has_value())
			glAttachShader(program, *geometry);
		glLinkProgram(program);
		check_compile_errors(program, "PROGRAM");
		return program;
	}
}

ShaderInterface::ShaderInterface(
	std::string const &vertex_path,
	std::string const &fragment_path,
	std::string const &geometry_path) noexcept
{
	// prepare vertex shader
	GLuint const vertex = compile_shader(vertex_path, GL_VERTEX_SHADER);
	// prepare fragment shader
	GLuint const fragment = compile_shader(fragment_path, GL_FRAGMENT_SHADER);
	// prepare geometry shader if source code is provided, otherwise set to null
	std::optional<GLuint> geometry = std::nullopt;
	if (geometry_path.empty() == false)
		geometry = compile_shader(geometry_path, GL_GEOMETRY_SHADER);

	// link shaders and create shader program
	this->id = link_shaders(vertex, fragment, geometry);

	// shaders no longer necessary now that they've been linked and the shader program compiled
	glDeleteShader(vertex);
	glDeleteShader(fragment);
	if (geometry.has_value())
		glDeleteShader(*geometry);
}

GLuint ShaderInterface::get_id() const noexcept
{
	return this->id;
}

void ShaderInterface::use() const noexcept
{
	glUseProgram(this->id);
}

template <typename T>
void Uniform<T>::set(ShaderInterface const &p, T const &value) const noexcept
{
	ASSERT(false, "unrecognized type given to Uniform::set");
}

template <>
void Uniform<int>::set(ShaderInterface const &p, int const &value) const noexcept
{
	glUniform1i(this->get_uniform_location(p), static_cast<GLint>(value));
}

template <>
void Uniform<unsigned int>::set(ShaderInterface const &p, unsigned int const &value) const noexcept
{
	glUniform1ui(this->get_uniform_location(p), static_cast<GLuint>(value));
}

template <>
void Uniform<float>::set(ShaderInterface const &p, float const &value) const noexcept
{
	glUniform1f(this->get_uniform_location(p), static_cast<GLfloat>(value));
}

template <>
void Uniform<glm::vec3>::set(ShaderInterface const &p, glm::vec3 const &value) const noexcept
{
	glUniform3f(
		this->get_uniform_location(p),
		static_cast<GLfloat>(value.x),
		static_cast<GLfloat>(value.y),
		static_cast<GLfloat>(value.z));
}

template <>
void Uniform<glm::vec4>::set(ShaderInterface const &p, glm::vec4 const &value) const noexcept
{
	glUniform4f(
		this->get_uniform_location(p),
		static_cast<GLfloat>(value.x),
		static_cast<GLfloat>(value.y),
		static_cast<GLfloat>(value.z),
		static_cast<GLfloat>(value.w));
}

template <>
void Uniform<glm::mat4>::set(ShaderInterface const &p, glm::mat4 const &value) const noexcept
{
	glUniformMatrix4fv(
		this->get_uniform_location(p),
		1,
		GL_FALSE,
		static_cast<GLfloat const *>(&value[0][0]));
}

template <typename T>
GLint Uniform<T>::get_uniform_location(ShaderInterface const &p) const noexcept
{
	GLint const n = glGetUniformLocation(p.get_id(), this->name.c_str());
	ASSERT(static_cast<int>(n) >= 0, "glGetUniformLocation returned -1");
	return n;
}
