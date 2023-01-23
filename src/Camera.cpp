#include "Camera.h"
#include <glm/glm.hpp>
#include <glm/ext/matrix_transform.hpp>
#include <cmath>

Camera::Camera(
	glm::vec3 position = glm::vec3(0.0f, 0.0f, 0.0f),
	glm::vec3 world_up = glm::vec3(0.0f, 1.0f, 0.0),
	float yaw = CAMERA_DEFAULT_YAW,
	float pitch = CAMERA_DEFAULT_PITCH) noexcept : position(position),
												   world_up(world_up),
												   yaw(yaw),
												   pitch(pitch),
												   front(glm::vec3(0.0f, 0.0f, -1.0f)),
												   movement_speed(CAMERA_DEFAULT_MOVEMENT_SPEED),
												   mouse_sensitivity(CAMERA_DEFAULT_MOUSE_SENSITIVITY),
												   zoom(CAMERA_DEFAULT_ZOOM)
{
	update_camera_vectors();
}

Camera::Camera(
	float position[3],
	float world_up[3],
	float yaw,
	float pitch) noexcept : position(glm::vec3(position[0], position[1], position[2])),
							world_up(glm::vec3(world_up[0], world_up[1], world_up[2])),
							yaw(yaw),
							pitch(pitch),
							front(glm::vec3(0.0f, 0.0f, -1.0f)),
							movement_speed(CAMERA_DEFAULT_MOVEMENT_SPEED),
							mouse_sensitivity(CAMERA_DEFAULT_MOUSE_SENSITIVITY),
							zoom(CAMERA_DEFAULT_ZOOM)
{
	update_camera_vectors();
}

glm::mat4 Camera::get_view_matrix() const noexcept
{
	return glm::lookAt(this->position, this->position + this->front, this->up);
}

void Camera::update_camera_vectors() noexcept
{
	// calculate new front vector
	glm::vec3 abnormal_front;
	abnormal_front.x = std::cos(glm::radians(this->yaw)) * std::cos(glm::radians(this->pitch));
	abnormal_front.y = sin(glm::radians(this->pitch));
	abnormal_front.z = sin(glm::radians(this->yaw)) * cos(glm::radians(this->pitch));
	this->front = glm::normalize(abnormal_front);

	// calculate new right vector
	this->right = glm::normalize(glm::cross(this->front, this->world_up));

	// calculate new up vector (camera-up, not world-up)
	this->up = glm::normalize(glm::cross(this->right, this->front));
}
