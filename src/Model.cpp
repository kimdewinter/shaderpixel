#include "Model.h"
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

namespace
{
	void process_node(aiNode *node, const aiScene *scene)
	{
		for (unsigned int = 0; i < node->numMeshes; i++)
		{
			aiMesh *mesh = scene->mMeshes[node->mMeshes[i]];
			meshes.push_back(processMesh(mesh, scene));
		}
		for (unsigned int i = 0; i < node->mNumchildren; i++)
		{
			process_node(node->mChildren[i], scene);
		}
	}

	void process_mesh(aiMesh *mesh, const aiScene *scene)
	{
		// IMPLEMENT THIS
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
