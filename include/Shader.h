#ifndef SHADER_H_INCLUDED
#define SHADER_H_INCLUDED

class Shader
{
public:
	Shader(
		const char *vertex_path,
		const char *fragment_path,
		const char *geometry_path = NULL);

	unsigned int id;

private:
};

#endif