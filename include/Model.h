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
#include <optional>

class Model
{
public:
	Model(std::string const &path) noexcept;
	void draw(Shader const &shader) const noexcept;

	std::vector<Texture> textures_loaded; // all textures loaded so far; optimization to make sure textures are not loaded twice unnecessarily
	std::vector<Mesh> meshes;			  // meshes that make up the parts of a model
	std::string const directory;		  // directory where model file is stored

private:
	/// @brief processes rootnode, and then it's child nodes in recursive fashion
	void process_node(aiNode const *const node, aiScene const *const scene) noexcept;
	/// @brief convert assimp aiMesh into our own Mesh type
	Mesh process_mesh(aiMesh const *const mesh, aiScene const *const scene) noexcept;
	/// @brief checks material textures of given type, and loads them if they're not already loaded
	std::vector<Texture> load_material_textures(
		aiMaterial const *const mat,
		aiTextureType const type,
		std::string const &type_name) noexcept;
	/// @brief checks whether the texture is already loaded
	/// @return Texture, or std::nullopt if not already loaded
	std::optional<Texture> find_loaded_texture(char const *const path) const noexcept;
};

#endif