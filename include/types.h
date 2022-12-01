#ifndef TYPES_H_INCLUDED
#define TYPES_H_INCLUDED

#include <glm/glm.hpp>
#include <string>

/// @brief this struct is laid out in a manner that it forms contiguous data, which can be sent to OpenGL as one (absolute) unit
struct Vertex
{
	glm::vec3 position;
	glm::vec3 normal;
	glm::vec2 texture_coordinates;
};
/*
The following:
vertex.position  = glm::vec3(0.2f, 0.4f, 0.6f);
vertex.normal    = glm::vec3(0.0f, 1.0f, 0.0f);
vertex.texture_coordinates = glm::vec2(1.0f, 0.0f);
Would in memory translate into:
[0.2f, 0.4f, 0.6f, 0.0f, 1.0f, 0.0f, 1.0f, 0.0f];
*/

/// @param type should be
struct Texture
{
	unsigned int id;
	std::string type;
};

#endif