#ifndef MODEL_H_INCLUDED
#define MODEL_H_INCLUDED

// what operations assimp should carry out on the data after importing it, available are:
// aiProcess_Triangulate
// aiProcess_FlipUVs
// aiProcess_GenNormals
// aiProcess_GenSmoothNormals
// aiProcess_SplitLargeMeshes
// aiProcess_OptimizeMeshes
// aiProcess_CalcTangentSpace
#define ASSIMP_POSTPROCESSING aiProcess_Triangulate | aiProcess_FlipUVs | aiProcess_GenSmoothNormals | aiProcess_CalcTangentSpace

#include <iostream>
#include <vector>
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>
#include "Mesh.h"

class Model
{
public:
	Model(std::string const &path);

	std::vector<Texture> textures_loaded; // all textures loaded so far; optimization to make sure textures are not loaded twice unnecessarily
	std::vector<Mesh> meshes;
	std::string directory;

private:
	Mesh process_mesh(aiMesh const *const mesh, aiScene const *const scene) noexcept;
	std::vector<Texture> load_material_textures(
		aiMaterial const *const mat,
		aiTextureType const type,
		std::string const &type_name);
	void process_node(aiNode const *const node, aiScene const *const scene);
};

#endif