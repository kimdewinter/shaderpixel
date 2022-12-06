#ifndef MODEL_H_INCLUDED
#define MODEL_H_INCLUDED

// what operations assimp should carry out on the data after importing it, available are:
// aiProcess_Triangulate
// aiProcess_FlipUVs
// aiProcess_GenNormals
// aiProcess_SplitLargeMeshes
// aiProcess_OptimizeMeshes
#define ASSIMP_POSTPROCESSING aiProcess_Triangulate | aiProcess_FlipUVs

#include <iostream>
#include <vector>
#include "Mesh.h"

class Model
{
public:
	Model(std::string const &path);

	std::vector<Mesh> meshes;
	std::string directory;

private:
	Mesh process_mesh(aiMesh const *const mesh, aiScene const *const scene) noexcept;
	std::vector<Texture> Model::load_material_textures(
		aiMaterial const *const mat,
		aiTextureType const type,
		std::string const &type_name) noexcept;
	void process_node(aiNode *node, const aiScene *scene) noexcept;
};

#endif