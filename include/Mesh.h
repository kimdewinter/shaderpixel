#ifndef MESH_H_INCLUDED
#define MESH_H_INCLUDED

#include <vector>
#include "types.h"

/// @brief holds vertex and texture data, and draw functionality
class Mesh
{
public:
	Mesh(
		std::vector<Vertex> const &vertices,
		std::vector<unsigned int> const &indices,
		std::vector<Texture> const &textures) noexcept;
	Mesh(
		std::vector<Vertex> &&vertices,
		std::vector<unsigned int> &&indices,
		std::vector<Texture> &&textures) noexcept;
	unsigned int get_VAO() const noexcept;
	size_t get_indices_size() const noexcept;
	std::vector<Texture> const &get_textures() const noexcept;

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
