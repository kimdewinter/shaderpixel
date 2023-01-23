#ifndef CAMERA__H__
#define CAMERA__H__

#include "main.h"
#include <chrono>
#include <glm/vec3.hpp>
#include <glm/mat4x4.hpp>
#include <glm/trigonometric.hpp>
#include <glad/glad.h>

class Camera
{
public:
	enum MoveDirection
	{
		FORWARD,
		BACKWARD,
		LEFT,
		RIGHT
	};

	/// @brief constructs with vectors
	Camera(
		glm::vec3 position = glm::vec3(0.0f, 0.0f, 0.0f),
		glm::vec3 world_up = glm::vec3(0.0f, 1.0f, 0.0),
		float yaw = CAMERA_DEFAULT_YAW,
		float pitch = CAMERA_DEFAULT_PITCH) noexcept;
	/// @brief constructs with scalars
	Camera(
		float position[3],
		float world_up[3],
		float yaw,
		float pitch) noexcept;
	/// @brief uses the updated Euler angles to calculate new front, right, and (cam-)up vector
	void update_camera_vectors() noexcept;
	glm::mat4 get_view_matrix() const noexcept;
	/// @brief moves camera's position in the world
	/// @param direction direction of movement
	/// @param delta_time how much time has passed since last frame
	void move_camera(Camera::MoveDirection const direction, std::chrono::duration<long long, std::nano> const delta_time) noexcept;
	/// @brief Pans camera around, usually depending on how much the mouse has moved
	/// @param x_offset
	/// @param y_offset
	/// @param constrain_pitch
	void pan_camera(float const x_offset, float const y_offset, GLboolean constrain_pitch = true) noexcept;
	/// @brief Zooms camera in and out, obviously a mousewheels main axis is vertical
	void zoom_camera(float const y_offset) noexcept;

private:
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