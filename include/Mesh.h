#ifndef MESH_H_INCLUDED
#define MESH_H_INCLUDED

#include <vector>
#include "types.h"
#include "Shader.h"

/// @brief holds vertex and texture data, and draw functionality
class Mesh
{
public:
	Mesh(
		std::vector<Vertex const> const &vertices,
		std::vector<unsigned int const> const &indices,
		std::vector<Texture const> const &textures) noexcept;
	void draw(Shader const &shader) const noexcept;

private:
	unsigned int VAO; // Vertex Array Object
	unsigned int VBO; // Vertex Buffer Object
	unsigned int EBO; // Element Buffer Object
	std::vector<Vertex const> const vertices;
	std::vector<unsigned int const> const indices;
	std::vector<Texture const> const textures;
};

#endif
