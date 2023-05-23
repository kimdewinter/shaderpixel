#include <string>
#include <fstream>
#include <sstream>
#include <glad/glad.h>
#include "Shader.h"
#include "ErrorHandler.h"

namespace
{
	void check_compile_errors(GLuint const shader_id, std::string const type)
	{
		GLint success;
		GLchar info_log[1024];
		if (type != "PROGRAM")
		{
			glGetShaderiv(shader_id, GL_COMPILE_STATUS, &success);
			if (!success)
			{
				glGetShaderInfoLog(shader_id, 1024, NULL, info_log);
				ASSERT(false, "Shader compilation error of type: " + type + "\n" + std::string(info_log) + "\n");
			}
		}
		else
		{
			glGetProgramiv(shader_id, GL_LINK_STATUS, &success);
			if (!success)
			{
				glGetProgramInfoLog(shader_id, 1024, NULL, info_log);
				ASSERT(false, "Shader linking error of type: " + type + "\n" + std::string(info_log) + "\n");
			}
		}
	}

	unsigned int compile_shader(std::string const &path, GLenum const shader_type) noexcept
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
		std::string code = stream.str();

		// compile shader
		const char *code_cstr = code.c_str();
		unsigned int id = glCreateShader(shader_type);
		glShaderSource(id, 1, &code_cstr, NULL);
		glCompileShader(id);
		check_compile_errors(id, "SHADER");

		return id;
	}
}

Shader::Shader(
	std::string const &name,
	std::string const &modelview_uniform_name,
	std::string const &projection_uniform_name,
	std::string const &vertex_path,
	std::string const &fragment_path,
	std::string const &geometry_path) noexcept : name(name),
												 modelview_uniform_name(modelview_uniform_name),
												 projection_uniform_name(projection_uniform_name)
{
	// retrieve source code
	std::string vertex_code, fragment_code, geometry_code;
	std::ifstream vertex_file, fragment_file, geometry_file;

	// open files
	vertex_file.open(vertex_path);
	fragment_file.open(fragment_path);
	ASSERT(
		vertex_file.is_open() && fragment_file.is_open(),
		"failed to open vertex_file and/or fragment_file in Shader constructor");

	// read file buffer into stringstreams
	std::stringstream vertex_stream, fragment_stream;
	vertex_stream << vertex_file.rdbuf();
	fragment_stream << fragment_file.rdbuf();

	// close file handlers
	vertex_file.close();
	fragment_file.close();

	// convert stringstream to string
	vertex_code = vertex_stream.str();
	fragment_code = fragment_stream.str();

	// if geometry shader path provided: load geometry shader
	if (geometry_path.empty() == false)
	{
		geometry_file.open(geometry_path);
		ASSERT(geometry_file.is_open(), "failed to open geometry_file in Shader constructor");
		std::stringstream geometry_stream;
		geometry_stream << geometry_file.rdbuf();
		geometry_file.close();
		geometry_code = geometry_stream.str();
	}

	// compile vertex shader
	const char *vertex_code_cstr = vertex_code.c_str();
	unsigned int vertex_id;
	vertex_id = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(vertex_id, 1, &vertex_code_cstr, NULL);
	glCompileShader(vertex_id);
	check_compile_errors(vertex_id, "VERTEX");

	// compile fragment shader
	const char *fragment_code_cstr = fragment_code.c_str();
	unsigned int fragment_id = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fragment_id, 1, &fragment_code_cstr, NULL);
	glCompileShader(fragment_id);
	check_compile_errors(fragment_id, "FRAGMENT");

	// compile geometry shader (if necessary)
	unsigned int geometry_id = 0;
	if (geometry_path.empty() == false)
	{
		const char *geometry_code_cstr = geometry_code.c_str();
		unsigned int geometry_id = glCreateShader(GL_GEOMETRY_SHADER);
		glShaderSource(geometry_id, 1, &geometry_code_cstr, NULL);
		glCompileShader(geometry_id);
		check_compile_errors(geometry_id, "GEOMETRY");
	}

	// link shaders and create shader program
	this->opengl_id = glCreateProgram();
	glAttachShader(this->opengl_id, vertex_id);
	glAttachShader(this->opengl_id, fragment_id);
	if (geometry_path.empty() == false)
		glAttachShader(this->opengl_id, geometry_id);
	glLinkProgram(this->opengl_id);
	check_compile_errors(this->opengl_id, "PROGRAM");

	// shaders no logner necessary now that they've been linked and the shader program compiled
	glDeleteShader(vertex_id);
	glDeleteShader(fragment_id);
	if (geometry_path.empty() == false)
		glDeleteShader(geometry_id);
}

Shader::Shader(ShaderRecipe const &recipe) noexcept
{
}

void Shader::use() const noexcept
{
	glUseProgram(this->opengl_id);
}

unsigned int Shader::get_opengl_id() const noexcept
{
	return this->opengl_id;
}

void Shader::set_modelview_matrix(glm::mat4 const &value) const noexcept
{
	this->set_uniform_mat4(this->modelview_uniform_name, value);
}

void Shader::set_projection_matrix(glm::mat4 const &value) const noexcept
{
	this->set_uniform_mat4(this->projection_uniform_name, value);
}

void Shader::set_uniform_int(std::string const &name, int const value) const noexcept
{
	// ideally you wouldn't want to call glUseProgram() more than necessary
	this->use();
	glUniform1i(glGetUniformLocation(this->opengl_id, name.c_str()), value);
}

void Shader::set_uniform_mat4(std::string const &name, glm::mat4 const &value) const noexcept
{
	// ideally you wouldn't want to call glUseProgram() more than necessary
	this->use();
	glUniformMatrix4fv(glGetUniformLocation(this->opengl_id, name.c_str()), 1, GL_FALSE, &value[0][0]);
}
