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
	glBindBuffer(GL_ARRAY_BUFFER, VBO);

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
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)0);

	// vertex normals
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, normal));

	// vertex texture coords
	glEnableVertexAttribArray(2);
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, texture_coordinates));

	// vertex tangent
	glEnableVertexAttribArray(3);
	glVertexAttribPointer(3, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, tangent));

	// vertex bitangent
	glEnableVertexAttribArray(4);
	glVertexAttribPointer(4, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, bitangent));

	// ids
	glEnableVertexAttribArray(5);
	glVertexAttribPointer(5, 4, GL_INT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, bone_ids));

	// weights
	glEnableVertexAttribArray(6);
	glVertexAttribPointer(6, 4, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, bone_weights));

	// release VAO binding
	glBindVertexArray(0);
}

Mesh::Mesh(
	std::vector<Vertex>&& vertices,
	std::vector<unsigned int>&& indices,
	std::vector<Texture>&& textures) noexcept
{
	this->vertices = vertices;
	this->indices = indices;
	this->textures = textures;
	constructor_helper();
}

Mesh::Mesh(
	std::vector<Vertex> const& vertices,
	std::vector<unsigned int> const& indices,
	std::vector<Texture> const& textures) noexcept
{
	this->vertices = vertices;
	this->indices = indices;
	this->textures = textures;
	constructor_helper();
}

void Mesh::draw(Shader const& shader) const noexcept
{
	if (!this->textures.size())
		glUniform1i(glGetUniformLocation(shader.get_opengl_id(), "disable_textures"), 1);
	else
	{
		glUniform1i(glGetUniformLocation(shader.get_opengl_id(), "disable_textures"), 0);

		// bind appropriate textures
		unsigned int texture_diffusen = 1;
		unsigned int texture_specularn = 1;
		unsigned int texture_normaln = 1;
		unsigned int texture_heightn = 1;
		for (unsigned int i = 0; i < textures.size(); i++)
		{
			glActiveTexture(GL_TEXTURE0 + i); // activate texture unit before calling glBindTexture()
			// retrieve texture number (the n in texture_diffusen)
			std::string number;
			std::string name = this->textures[i].type;
			if (name == "texture_diffuse")
				number = std::to_string(texture_diffusen++);
			else if (name == "texture_specular")
				number = std::to_string(texture_specularn++);
			else if (name == "texture_normal")
				number = std::to_string(texture_normaln++);
			else if (name == "texture_height")
				number = std::to_string(texture_heightn++);

			glUniform1i(glGetUniformLocation(shader.get_opengl_id(), (name + number).c_str()), i);
			glBindTexture(GL_TEXTURE_2D, textures[i].id);
		}
		glActiveTexture(GL_TEXTURE0);
	}

	// draw
	glBindVertexArray(VAO);
	glDrawElements(GL_TRIANGLES, static_cast<unsigned int>(this->indices.size()), GL_UNSIGNED_INT, 0);

	// reset everything for safety
	glBindVertexArray(0);
	glActiveTexture(GL_TEXTURE0);
}
