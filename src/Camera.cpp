#include "Camera.h"
#include "ErrorHandler.h"
#include <glm/glm.hpp>
#include <glm/ext/matrix_transform.hpp>
#include <glm/gtc/quaternion.hpp>
#include <cmath>

namespace
{
	/// @return a float that signifies how many seconds variable "nanoseconds" is
	float nanoseconds_to_seconds(std::chrono::duration<long long, std::nano> const nanoseconds)
	{
		return std::chrono::duration<float>(nanoseconds).count();
	}
}

Camera::Camera(
	glm::vec3 const& position,
	glm::vec3 const& world_up,
	float yaw,
	float pitch) noexcept : position(position),
	world_up(world_up),
	yaw(yaw),
	pitch(pitch),
	front(glm::vec3(0.0f, 0.0f, 0.0f)),
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

glm::mat4 Camera::get_view_matrix() noexcept
{
	this->update_camera_vectors();
	return glm::lookAt(this->position, this->position + this->front, this->up);
}

glm::mat4 Camera::get_projection_matrix() const noexcept
{
	return this->projection_matrix;
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

void Camera::move_camera(Camera::Direction const direction, std::chrono::duration<long long, std::nano> const& delta_time) noexcept
{
	float const movement_magnitude = this->movement_speed * nanoseconds_to_seconds(delta_time);
	switch (direction)
	{
	case FORWARD:
		this->position += this->front * movement_magnitude;
		break;
	case BACKWARD:
		this->position -= this->front * movement_magnitude;
		break;
	case LEFT:
		this->position -= this->right * movement_magnitude;
		break;
	case RIGHT:
		this->position += this->right * movement_magnitude;
		break;
	case UP:
		this->position += this->up * movement_magnitude;
		break;
	case DOWN:
		this->position -= this->up * movement_magnitude;
		break;
	}
}

void Camera::pan_camera(float const x_offset, float const y_offset, GLboolean constrain_pitch) noexcept
{
	this->yaw += (x_offset * this->mouse_sensitivity);
	this->pitch -= (y_offset * this->mouse_sensitivity);

	if (constrain_pitch)
	{
		if (this->pitch > 89.9f)
			this->pitch = 89.9f;
		else if (this->pitch < -89.9f)
			this->pitch = -89.9f;
	}

	this->update_camera_vectors();
}

void Camera::roll_camera(Camera::Direction const direction, std::chrono::duration<long long, std::nano> const delta_time) noexcept
{
	float const magnitude = this->movement_speed * nanoseconds_to_seconds(delta_time);
	switch (direction)
	{
	case LEFT:
		this->world_up -= this->right * magnitude;
		break;
	case RIGHT:
		this->world_up += this->right * magnitude;
		break;
	default:
		ASSERT(false, "roll_camera() received direction other than LEFT or RIGHT");
	}
	this->update_camera_vectors();
}

void Camera::zoom_camera(float const y_offset) noexcept
{
	this->zoom -= y_offset;
	if (this->zoom < 1.0f)
		this->zoom = 1.0f;
	else if (this->zoom > 45.0f)
		this->zoom = 45.0f;
}
