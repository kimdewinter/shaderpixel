#ifndef CAMERA__H__
#define CAMERA__H__

#include "main.h"
#include <glm/vec3.hpp>
#include <glm/trigonometric.hpp>

class Camera
{
public:
	enum Direction
	{
		FORWARD,
		BACKWARD,
		LEFT,
		RIGHT
	};

	/// @brief construct with vectors
	Camera(
		glm::vec3 position = glm::vec3(0.0f, 0.0f, 0.0f),
		glm::vec3 world_up = glm::vec3(0.0f, 1.0f, 0.0),
		float yaw = CAMERA_DEFAULT_YAW,
		float pitch = CAMERA_DEFAULT_PITCH);
	/// @brief construct with scalars
	Camera(
		float position[3],
		float world_up[3],
		float yaw,
		float pitch) noexcept; 
	glm::mat4 get_view_matrix() const noexcept;

private:
	/// @brief uses the updated Euler angles to calculate new front, right, and (cam-)up vector
	void update_camera_vectors() noexcept;

	// camera attributes
	glm::vec3 position; // camera's position in world space (so relative to world center)
	glm::vec3 front;	// direction vector of what the camera points it's front towards
	glm::vec3 up;		// vector that points upwards of the camera (important when the camera is rolled)
	glm::vec3 right;	// vector that points to the right of the camera
	glm::vec3 world_up; // vector that points upwards in world-space
	// euler angles
	float yaw;
	float pitch;
	// camera options
	float movement_speed;
	float mouse_sensitivity;
	float zoom;
};

/*
Camera direction:
vec3 camera_direction = normalize_vector(camera_position - camera_target_position);

Right axis:
vec3 camera_right = normalize_vector(cross_product(world_up, camera_direction));

Up axis:
vec3 camera_up = cross_product(camera_direction, camera_right);

View matrix where
R = camera_right
U = camera_up
D = camera_direction
P = camera_position
			[	R.x		R.y		R.z		0	]					[	1	0	0	-P.y	]
			[	U.x		U.y		U.z		0	]		*			[	0	1	0	-P.y	]
LookAt =	[	D.x		D.y		D.z		0	]	(dot product)	[	0	0	1	-P.z	]
			[	0		0		0		1	]					[	0	0	0	1		]


*/

#endif