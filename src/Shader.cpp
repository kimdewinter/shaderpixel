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
				throw std::runtime_error("Shader linking error of type: " + type + "\n" + std::string(info_log) + "\n");
			}
		}
		else
		{
			glGetProgramiv(shader_id, GL_LINK_STATUS, &success);
			if (!success)
			{
				glGetProgramInfoLog(shader_id, 1024, NULL, info_log);
				throw std::runtime_error("Shader linking error of type: " + type + "\n" + std::string(info_log) + "\n");
			}
		}
	}
}

Shader::Shader(
	char const *const vertex_path,
	char const *const fragment_path,
	char const *const geometry_path)
{
	// retrieve source code
	std::string vertex_code, fragment_code, geometry_code;
	std::ifstream vertex_file, fragment_file, geometry_file;

	// allow filestreams to throw exceptions
	vertex_file.exceptions(std::ifstream::failbit | std::ifstream::badbit);
	fragment_file.exceptions(std::ifstream::failbit | std::ifstream::badbit);
	geometry_file.exceptions(std::ifstream::failbit | std::ifstream::badbit);
	try
	{
		// open files
		vertex_file.open(vertex_path);
		fragment_file.open(fragment_path);
		std::stringstream vertex_stream, fragment_stream;

		// read file buffer into stringstreams
		vertex_stream << vertex_file.rdbuf();
		fragment_stream << fragment_file.rdbuf();

		// close file handlers
		vertex_file.close();
		fragment_file.close();

		// convert stringstream to string
		vertex_code = vertex_stream.str();
		fragment_code = fragment_stream.str();

		// if geometry shader path provided: load geometry shader
		if (geometry_path != NULL)
		{
			geometry_file.open(geometry_path);
			std::stringstream geometry_stream;
			geometry_stream << geometry_file.rdbuf();
			geometry_file.close();
			geometry_code = geometry_stream.str();
		}
	}
	catch (std::ifstream::failure &exception)
	{
		ASSERT(false, exception.what());
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

	unsigned int geometry_id = 0;
	// compile geometry shader (if necessary)
	if (geometry_path != NULL)
	{
		const char *geometry_code_cstr = geometry_code.c_str();
		unsigned int geometry_id = glCreateShader(GL_GEOMETRY_SHADER);
		glShaderSource(geometry_id, 1, &geometry_code_cstr, NULL);
		glCompileShader(geometry_id);
		check_compile_errors(geometry_id, "GEOMETRY");
	}

	// link shaders and create shader program
	this->id = glCreateProgram();
	glAttachShader(this->id, vertex_id);
	glAttachShader(this->id, fragment_id);
	if (geometry_path != NULL)
		glAttachShader(this->id, geometry_id);
	glLinkProgram(this->id);
	check_compile_errors(this->id, "PROGRAM");

	// shaders no logner necessary now that they've been linked and the shader program compiled
	glDeleteShader(vertex_id);
	glDeleteShader(fragment_id);
	if (geometry_path != NULL)
		glDeleteShader(geometry_id);
}

void Shader::use() const noexcept
{
	glUseProgram(this->id);
}

unsigned int Shader::get_id() const noexcept
{
	return this->id;
}

void Shader::set_uniform_int(std::string const &name, int const value) const noexcept
{
	this->use();
	glUniform1i(glGetUniformLocation(this->id, name.c_str()), value);
}

void Shader::set_uniform_mat4(std::string const &name, glm::mat4 const &value) const noexcept
{
	this->use();
	glUniformMatrix4fv(glGetUniformLocation(this->id, name.c_str()), 1, GL_FALSE, &value[0][0]);
}
