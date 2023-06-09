#include "Mesh.h"
#include <glad/glad.h>
#include <stdexcept>
#include "ErrorHandler.h"

void Mesh::constructor_helper() noexcept
{
	ASSERT(!this->vertices.empty() && !this->indices.empty(),
		   "warning: mesh constructor encountered invalid argument");

	// request OpenGL to generate VAO, VBO, EBO objects
	glGenVertexArrays(1, &this->VAO);
	glGenBuffers(1, &this->VBO);
	glGenBuffers(1, &this->EBO);

	// bind the objects we just made so we can modify them
	glBindVertexArray(this->VAO);
	glBindBuffer(GL_ARRAY_BUFFER, this->VBO);

	// send vertex data to the GPU, all vertices memory is sequential because we use a struct
	glBufferData(
		GL_ARRAY_BUFFER,
		this->vertices.size() * sizeof(Vertex),
		&this->vertices[0],
		GL_STATIC_DRAW);

	// send EBO data to the GPU
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, this->EBO);
	glBufferData(
		GL_ELEMENT_ARRAY_BUFFER,
		this->indices.size() * sizeof(unsigned int),
		&this->indices[0],
		GL_STATIC_DRAW);

	// now we set vertex attribute pointers, which explains where in the Vertex struct each attribute begins
	// vertex positions
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void *)0);

	// vertex normals
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void *)offsetof(Vertex, normal));

	// vertex texture coords
	glEnableVertexAttribArray(2);
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void *)offsetof(Vertex, texture_coordinates));

	// vertex tangent
	glEnableVertexAttribArray(3);
	glVertexAttribPointer(3, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void *)offsetof(Vertex, tangent));

	// vertex bitangent
	glEnableVertexAttribArray(4);
	glVertexAttribPointer(4, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void *)offsetof(Vertex, bitangent));

	// ids
	glEnableVertexAttribArray(5);
	glVertexAttribPointer(5, 4, GL_INT, GL_FALSE, sizeof(Vertex), (void *)offsetof(Vertex, bone_ids));

	// weights
	glEnableVertexAttribArray(6);
	glVertexAttribPointer(6, 4, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void *)offsetof(Vertex, bone_weights));

	// release VAO binding
	glBindVertexArray(0);
}

Mesh::Mesh(
	std::vector<Vertex> &&vertices,
	std::vector<unsigned int> &&indices,
	std::vector<Texture> &&textures) noexcept : vertices(vertices),
												indices(indices),
												textures(textures)
{
	constructor_helper();
}

Mesh::Mesh(
	std::vector<Vertex> const &vertices,
	std::vector<unsigned int> const &indices,
	std::vector<Texture> const &textures) noexcept : vertices(vertices),
													 indices(indices),
													 textures(textures)
{
	constructor_helper();
}

unsigned int Mesh::get_VAO() const noexcept
{
	return this->VAO;
}

size_t Mesh::get_indices_size() const noexcept
{
	return this->indices.size();
}

std::vector<Texture> const &Mesh::get_textures() const noexcept
{
	return this->textures;
}
