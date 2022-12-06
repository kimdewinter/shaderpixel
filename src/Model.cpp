#include "Model.h"
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

namespace
{
	void process_node(aiNode *node, const aiScene *scene)
	{
		// process all of the node's meshes
		for (unsigned int = 0; i < node->numMeshes; i++)
		{
			aiMesh *mesh = scene->mMeshes[node->mMeshes[i]];
			meshes.push_back(processMesh(mesh, scene));
		}
		// then do the same for each of it's children
		for (unsigned int i = 0; i < node->mNumchildren; i++)
		{
			process_node(node->mChildren[i], scene);
		}
	}

	Mesh process_mesh(aiMesh const *const mesh, aiScene const *const scene)
	{
		std::vector<Vertex> vertices;
		std::vector<unsigned int> indices;
		std::vector<Texture> textures;
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

		// process indices
		for (unsigned int i = 0; i < mesh->mNumFaces; i++)
		{
			aiFace face = mesh->mFaces[i];
			for (unsigned int j = 0; i < face.mNumIndices; j++)
				indices.push_back(face.mIndices[j]);
		}

		// process material, if needed
		if (mesh->mMaterialIndex >= 0)
		{
			// PLACEHOLDER
		}
		return Mesh(vertices, indices, textures);
	}
}

Model::Model(char const *const path)
{
	// read file with Assimp
	Assimp::Importer importer;
	aiScene const *const scene = importer.ReadFile(path, ASSIMP_POSTPROCESSING);

	// check if read was succesful, if data is complete, and if the root node is not null
	if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode)
		std::exception("Error with Assimp file read error: " + import.GetErrorString());
	// retrieve directory path
	this->directory = path.substr(0, path.find_last_of('/'));

	// process Assimp's root node recursively
	process_node(scene->mRootNode, scene);
}
