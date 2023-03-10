#include "Model.h"
#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>
#include <SDL.h>
#include <glad/glad.h>
#include "ErrorHandler.h"

namespace
{
	unsigned int TextureFromFile(std::string const &path, std::string const &directory)
	{
		std::string file_name = directory + '/' + path;

		// ask OpenGL to declare a new texture object
		unsigned int id;
		glGenTextures(1, &id);

		// use stb_image to import an image
		stbi_set_flip_vertically_on_load(true); // tell stb_image.h to flip loaded textures on the y-axis before loading model
		int width, height, n_components;
		unsigned char *data = stbi_load(file_name.c_str(), &width, &height, &n_components, 0);
		ASSERT(data, "texture failed to load at path: " + path);

		// set the colour components
		GLenum format;
		if (n_components == 1)
			format = GL_RED;
		else if (n_components == 4)
			format = GL_RGBA;
		else
			format = GL_RGB;

		// send texture to GPU
		glBindTexture(GL_TEXTURE_2D, id);
		glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data);
		glGenerateMipmap(GL_TEXTURE_2D);

		// set miscellaneous image characteristics
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

		// now that the GPU holds the texture, we no longer need to keep it in RAM
		stbi_image_free(data);

		return id;
	}
}

void Model::draw(Shader const &shader) const noexcept
{
	for (Mesh mesh : this->meshes)
		mesh.draw(shader);
}

std::optional<Texture> Model::find_loaded_texture(char const *const path) const noexcept
{
	for (Texture texture : this->textures_loaded)
	{
		if (std::strcmp(texture.path.data(), path) == 0)
			return texture;
	}
	return std::nullopt;
}

std::vector<Texture> Model::load_material_textures(
	aiMaterial const *const mat,
	aiTextureType const type,
	std::string const &type_name) noexcept
{
	std::vector<Texture> textures;
	for (unsigned int i = 0; i < mat->GetTextureCount(type); i++)
	{
		aiString str;
		mat->GetTexture(type, i, &str);
		if (std::optional<Texture> found_texture = find_loaded_texture(str.C_Str()))
		{
			// texture has already been loaded, so copy it's info so it may be used
			textures.push_back(*found_texture);
		}
		else
		{
			// texture hasn't been loaded yet, so load it
			Texture new_texture;
			new_texture.id = TextureFromFile(std::string(str.C_Str()), this->directory);
			new_texture.type = type_name;
			new_texture.path = str.C_Str();
			textures.push_back(new_texture);
			this->textures_loaded.push_back(new_texture);
		}
	}
	return textures;
}

Mesh Model::process_mesh(aiMesh const *const mesh, aiScene const *const scene) noexcept
{
	std::vector<Vertex> vertices;
	for (unsigned int i = 0; i < mesh->mNumVertices; i++)
	{
		Vertex vertex;
		// process vertex positions
		vertex.position = {mesh->mVertices[i].x, mesh->mVertices[i].y, mesh->mVertices[i].z};

		// process vertex normals
		if (mesh->HasNormals())
			vertex.normal = {mesh->mNormals[i].x, mesh->mNormals[i].y, mesh->mNormals[i].z};

		// process texture coordinates
		if (mesh->mTextureCoords[0])
		{
			// a vertex can contain up to 8 different texture coordinates
			// we make the assumption that we won't use models where a vertex can have
			// multiple texture coordinates so we always take the first set (0)
			vertex.texture_coordinates = {mesh->mTextureCoords[0][i].x, mesh->mTextureCoords[0][i].y};
			vertex.tangent = {mesh->mTangents[i].x, mesh->mTangents[i].y, mesh->mTangents[i].z};
			vertex.bitangent = {mesh->mBitangents[i].x, mesh->mBitangents[i].y, mesh->mBitangents[i].z};
		}

		vertices.push_back(vertex);
	}

	// process indices
	std::vector<unsigned int> indices;
	for (unsigned int i = 0; i < mesh->mNumFaces; i++)
		for (unsigned int j = 0; j < mesh->mFaces[i].mNumIndices; j++)
			indices.push_back(mesh->mFaces[i].mIndices[j]);

	// from here on we're processing textures
	std::vector<Texture> textures;
	// process materials
	// we assume a convention for sampler names in the shaders
	// each diffuse texture should be named as 'texture_diffuseN' where N is a sequential number
	// same applies to other textures such as:
	// diffuse: texture_diffuseN
	// specular: texture_specularN
	// normal: texture_normalN
	aiMaterial *material = scene->mMaterials[mesh->mMaterialIndex];
	// diffuse maps
	std::vector<Texture> const diffuse_maps = load_material_textures(material, aiTextureType_DIFFUSE, "texture_diffuse");
	textures.insert(textures.end(), diffuse_maps.begin(), diffuse_maps.end());
	// specular maps
	std::vector<Texture> const specular_maps = load_material_textures(material, aiTextureType_SPECULAR, "texture_specular");
	textures.insert(textures.end(), specular_maps.begin(), specular_maps.end());
	// normal maps
	std::vector<Texture> const normal_maps = load_material_textures(material, aiTextureType_NORMALS, "texture_normal");
	textures.insert(textures.end(), normal_maps.begin(), normal_maps.end());
	// height maps
	std::vector<Texture> const height_maps = load_material_textures(material, aiTextureType_HEIGHT, "texture_height");
	textures.insert(textures.end(), height_maps.begin(), height_maps.end());

	return Mesh(vertices, indices, textures);
}

void Model::process_node(aiNode const *const node, aiScene const *const scene) noexcept
{
	// process all of the node's meshes
	for (unsigned int i = 0; i < node->mNumMeshes; i++)
	{
		aiMesh const *const assimp_type_mesh = scene->mMeshes[node->mMeshes[i]];
		Mesh own_type_mesh = process_mesh(assimp_type_mesh, scene);
		this->meshes.push_back(own_type_mesh);
	}
	// then do the same for each of it's children
	for (unsigned int i = 0; i < node->mNumChildren; i++)
		process_node(node->mChildren[i], scene);
}

Model::Model(
	std::string const &name,
	std::string const &path,
	glm::vec3 const &position,
	glm::quat const &orientation,
	glm::vec3 const &scaling) noexcept : name(name),
										 directory(path.substr(0, path.find_last_of('/'))),
										 position(position),
										 orientation(orientation),
										 scaling(scaling)
{
	// read file with Assimp
	Assimp::Importer importer;
	aiScene const *const scene = importer.ReadFile(path, ASSIMP_POSTPROCESSING);

	// check if read was succesful, if data is complete, and if the root node is not null
	ASSERT(scene && !(scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE) && scene->mRootNode,
		   "error with Assimp file read error: " + std::string(importer.GetErrorString()));

	// process Assimp's root node recursively
	process_node(scene->mRootNode, scene);
}

glm::mat4 const Model::get_model_matrix() const noexcept
{
	glm::mat4 matrix = glm::mat4(1.0f);				 // start with a non-transforming identity matrix
	matrix = glm::translate(matrix, this->position); // add translation
	matrix = matrix * glm::mat4(this->orientation);	 // add rotation
	matrix = glm::scale(matrix, this->scaling);		 // add scaling
	return matrix;
}
