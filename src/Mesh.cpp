#include "Mesh.h"
#include <glad/glad.h>
#include <stdexcept>

Mesh::Mesh(
	std::vector<Vertex> vertices,
	std::vector<unsigned int> indices,
	std::vector<Texture> textures) : vertices(vertices),
									 indices(indices),
									 textures(textures)
{
	if (vertices.empty() || indices.empty() || textures.empty())
		throw std::invalid_argument("Warning: Mesh constructor encountered invalid argument");

	// Request OpenGL to generate a VAO, VBO and EBO
	glGenVertexArrays(1, &this->VAO);
	glGenBuffers(1, &this->VBO);
	glGenBuffers(1, &this->EBO);

	// Bind the objects we just made so we can modify them
	glBindVertexArray(this->VAO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);

	// Send vertex data to the GPU
	glBufferData(
		GL_ARRAY_BUFFER,
		this->vertices.size() * sizeof(Vertex),
		&this->vertices[0],
		GL_STATIC_DRAW);

	// Send EBO data to the GPU
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, this->EBO);
	glBufferData(
		GL_ELEMENT_ARRAY_BUFFER,
		this->indices.size() * sizeof(unsigned int),
		&this->indices[0],
		GL_STATIC_DRAW);

	// vertex positions
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void *)0);

	// vertex normals
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void *)offsetof(Vertex, normal));

	// vertex texture coords
	glEnableVertexAttribArray(2);
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void *)offsetof(Vertex, texture_coordinates));

	// release VAO binding
	glBindVertexArray(0);
}

void Mesh::draw(Shader &shader)
{
	unsigned int diffuse_textures = 1;
	unsigned int specular_textures = 1;
	for (unsigned int i = 0; i < textures.size(); i++)
	{
		glActiveTexture(GL_TEXTURE0 + i); // activate texture unit before callign glBindTexture()
		std::string number;
		std::string name = this->textures[i].type;
		if (name == "texture_diffuse")
			number = std::to_string(diffuse_textures++);
		else if (name == "texture_specular")
			number = std::to_string(specular_textures++);

		shader.set_uniform_int(("material." + name + number).c_str(), i);
		glBindTexture(GL_TEXTURE_2D, textures[i].id);
	}
	glActiveTexture(GL_TEXTURE0);

	// draw
	glBindVertexArray(VAO);
	glDrawElements(GL_TRIANGLES, this->indices.size(), GL_UNSIGNED_INT, 0);
	glBindVertexArray(0);
}
