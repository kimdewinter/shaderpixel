#include "Model.h"
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>
#include <stb_image.h>

unsigned int TextureFromFile(const char *path, const string &directory, bool gamma = false);

void Model::process_node(aiNode *node, const aiScene *scene) noexcept
{
	// process all of the node's meshes
	for (unsigned int i = 0; i < node->mNumMeshes; i++)
	{
		aiMesh *mesh = scene->mMeshes[node->mMeshes[i]];
		this->meshes.push_back(process_mesh(mesh, scene));
	}
	// then do the same for each of it's children
	for (unsigned int i = 0; i < node->mNumChildren; i++)
	{
		process_node(node->mChildren[i], scene);
	}
}

Mesh Model::process_mesh(aiMesh const *const mesh, aiScene const *const scene) noexcept
{
	std::vector<Vertex> vertices;
	for (unsigned int i = 0; i < mesh->mNumVertices; i++)
	{
		Vertex vertex;
		// process vertex positions
		glm::vec3 position_vector;
		position_vector.x = mesh->mVertices[i].x;
		position_vector.y = mesh->mVertices[i].y;
		position_vector.z = mesh->mVertices[i].z;
		vertex.position = position_vector;

		// process vertex normals
		glm::vec3 normal_vector;
		normal_vector.x = mesh->mNormals[i].x;
		normal_vector.y = mesh->mNormals[i].y;
		normal_vector.z = mesh->mNormals[i].z;
		vertex.normal = normal_vector;

		// process texture coordinates
		if (mesh->mTextureCoords[0])
		{
			glm::vec2 texture_vector;
			texture_vector.x = mesh->mTextureCoords[0][i].x;
			texture_vector.y = mesh->mTextureCoords[0][i].y;
			vertex.texture_coordinates = texture_vector;
		}
		else
			vertex.texture_coordinates = glm::vec2(0.0f, 0.0f);
	}

	std::vector<unsigned int> indices;
	// process indices
	for (unsigned int i = 0; i < mesh->mNumFaces; i++)
	{
		aiFace face = mesh->mFaces[i];
		for (unsigned int j = 0; i < face.mNumIndices; j++)
			indices.push_back(face.mIndices[j]);
	}

	std::vector<Texture> textures;
	// process material, if needed
	if (mesh->mMaterialIndex >= 0)
	{
		aiMaterial const *const material = scene->mMaterials[mesh->mMaterialIndex];
		std::vector<Texture> diffuse_maps = Model::load_material_textures(
			material,
			aiTextureType_DIFFUSE,
			"texture_diffuse");
		textures.insert(textures.end(), diffuse_maps.begin(), diffuse_maps.end());
	}
	return Mesh(vertices, indices, textures);
}

std::vector<Texture> Model::load_material_textures(
	aiMaterial const *const mat,
	aiTextureType const type,
	std::string const &type_name) noexcept
{
	std::vector<Texture> texture_vector;
	for (unsigned int i = 0; i < mat->GetTextureCount(type); i++)
	{
		aiString str;
		mat->GetTexture(type, i, &str);
		Texture texture;
		texture.id = TextureFromFile(str.C_Str(), this->directory);
		texture.type = type_name;
		texture.path = str;
		texture_vector.push_back(texture);
	}
	return textures;
}

Model::Model(std::string const &path)
{
	// read file with Assimp
	Assimp::Importer importer;
	aiScene const *const scene = importer.ReadFile(path, ASSIMP_POSTPROCESSING);

	// check if read was succesful, if data is complete, and if the root node is not null
	if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode)
		std::runtime_error("Error with Assimp file read error: " + std::string(importer.GetErrorString()));
	// retrieve directory path
	this->directory = path.substr(0, path.find_last_of('/'));

	// process Assimp's root node recursively
	process_node(scene->mRootNode, scene);
}
