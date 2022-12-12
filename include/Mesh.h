#ifndef MESH_H_INCLUDED
#define MESH_H_INCLUDED

#include <vector>
#include "types.h"
#include "Shader.h"

/// @brief holds vertex and texture data, and draw functionality
class Mesh
{
public:
	Mesh(std::vector<Vertex> vertices, std::vector<unsigned int> indices, std::vector<Texture> textures);
	void draw(Shader const &shader) const noexcept;
	unsigned int get_vao() const noexcept;
	std::vector<Vertex> const vertices;
	std::vector<unsigned int> const indices;
	std::vector<Texture> const textures;

private:
	unsigned int VAO; // Vertex Array Object
	unsigned int VBO; // Vertex Buffer Object
	unsigned int EBO; // Element Buffer Object
};

#endif
