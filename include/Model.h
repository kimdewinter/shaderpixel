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
	Model(char const *const path);

	std::vector<Mesh> meshes;
	std::string directory;
};

#endif