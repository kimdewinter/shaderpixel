#ifndef TYPES_H_INCLUDED
#define TYPES_H_INCLUDED

#include <glm/vec2.hpp>
#include <glm/vec3.hpp>
#include <string>
#include <array>
#include "main.h"

/// @brief this struct is laid out in a manner that it forms contiguous data, which can be sent to OpenGL as one (absolute) unit
struct Vertex
{
	glm::vec3 position;
	glm::vec3 normal = glm::vec3{0.0f, 0.0f, 0.0f};
	glm::vec2 texture_coordinates = glm::vec2{0.0f, 0.0f};
	glm::vec3 tangent = glm::vec3{0.0f, 0.0f, 0.0f};
	glm::vec3 bitangent = glm::vec3{0.0f, 0.0f, 0.0f};
	std::array<int, MAX_BONE_INFLUENCE> bone_ids;		// bone indexes that influence this vertex
	std::array<float, MAX_BONE_INFLUENCE> bone_weights; // weights of each bone
};
/*
the following:
vertex.position  = glm::vec3(0.2f, 0.4f, 0.6f);
vertex.normal    = glm::vec3(0.0f, 1.0f, 0.0f);
vertex.texture_coordinates = glm::vec2(1.0f, 0.0f);
would in memory translate into:
[0.2f, 0.4f, 0.6f, 0.0f, 1.0f, 0.0f, 1.0f, 0.0f];
*/

struct Texture
{
	unsigned int id;
	std::string type; // e.g. texture_diffuse, texture_specular, texture_normal, texture_height
	std::string path;
};

#endif
