#include <string>
#include <fstream>
#include <sstream>
#include "Shader.h"
#include "ErrorHandler.h"

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
	// FINISH THIS
}
