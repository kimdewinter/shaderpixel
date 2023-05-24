#ifndef MESH_H_INCLUDED
#define MESH_H_INCLUDED

#include <vector>
#include "types.h"
#include "oldShader.h"

/// @brief holds vertex and texture data, and draw functionality
class Mesh
{
public:
	Mesh(
		std::vector<Vertex> const& vertices,
		std::vector<unsigned int> const& indices,
		std::vector<Texture> const& textures) noexcept;
	Mesh(
		std::vector<Vertex>&& vertices,
		std::vector<unsigned int>&& indices,
		std::vector<Texture>&& textures) noexcept;
	void draw(Shader const& shader) const noexcept;

private:
	void constructor_helper() noexcept;

	unsigned int VAO; // Vertex Array Object
	unsigned int VBO; // Vertex Buffer Object
	unsigned int EBO; // Element Buffer Object
	std::vector<Vertex> vertices;
	std::vector<unsigned int> indices;
	std::vector<Texture> textures;
};

#endif
