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
				throw std::runtime_error("Shader compilation error of type: " + type + "\n" + std::string(info_log) + "\n");
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
	const char *vertex_path,
	const char *fragment_path,
	const char *geometry_path)
{
	// Retrieve source code
	std::string vertex_code, fragment_code, geometry_code;
	std::ifstream vertex_file, fragment_file, geometry_file;

	// Allow filestreams to throw exceptions
	vertex_file.exceptions(std::ifstream::failbit | std::ifstream::badbit);
	fragment_file.exceptions(std::ifstream::failbit | std::ifstream::badbit);
	geometry_file.exceptions(std::ifstream::failbit | std::ifstream::badbit);
	try
	{
		// Open files
		vertex_file.open(vertex_path);
		fragment_file.open(fragment_path);
		std::stringstream vertex_stream, fragment_stream;

		// Read file buffer into stringstreams
		vertex_stream << vertex_file.rdbuf();
		fragment_stream << fragment_file.rdbuf();

		// Close file handlers
		vertex_file.close();
		fragment_file.close();

		// Convert stringstream to string
		vertex_code = vertex_stream.str();
		fragment_code = fragment_stream.str();

		// If geometry shader path provided: load geometry shader
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
		Error::output_error(Error::Type::WARNING, exception.what());
	}

	// Compile vertex shader
	const char *vertex_code_cstr = vertex_code.c_str();
	unsigned int vertex_id, fragment_id;
	vertex_id = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(vertex_id, 1, &vertex_code_cstr, NULL);
	glCompileShader(vertex_id);
	check_compile_errors(vertex_id, "VERTEX");

	// Compile fragment shader
	const char *fragment_code_cstr = fragment_code.c_str();
	fragment_id = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fragment_id, 1, &fragment_code_cstr, NULL);
	glCompileShader(fragment_id);
	check_compile_errors(fragment_id, "FRAGMENT");

	// Compile geometry shader (if necessary)
	if (geometry_path != NULL)
	{
		const char *geometry_code_cstr = geometry_code.c_str();
		unsigned int geometry_id = glCreateShader(GL_GEOMETRY_SHADER);
		glShaderSource(geometry_id, 1, &geometry_code_cstr, NULL);
		check_compile_errors(geometry_id, "GEOMETRY");
	}

	// Link shaders and create shader program
	this->id = glCreateProgram();
}

void Shader::set_uniform_int(const std::string &name, int const value) const noexcept
{
	glUniform1i(glGetUniformLocation(this->id, name.c_str()), value);
}
