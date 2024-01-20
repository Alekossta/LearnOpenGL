#include "Camera.h"

Camera::Camera()
{
	position = glm::vec3(0.0f, 0.0f, 0.0f);
	up = glm::vec3(0.0f, 0.0f, 0.0f);
	yaw = -90;
	pitch = 0;
	front = glm::vec3(0.0f, 0.0f, -1.0f);
	movementSpeed = 2.5f;
	mouseSensitivity = 0.1f;
	zoom = 45.0f;

	updateCameraVectors();
}

glm::mat4 Camera::getViewMatrix()
{
	return glm::lookAt(position, position + front, up);
}

void Camera::rotate(float xoffset, float yoffset)
{
	yaw += xoffset * mouseSensitivity;
	pitch += yoffset * mouseSensitivity;

	if (pitch > 89.0f) pitch = 89.0f;
	if (pitch < -89.0f) pitch = -89.0f;

	updateCameraVectors();
}

void Camera::move(Direction direction, float deltaTime)
{
	if (direction == FORWARD) position += front * movementSpeed * deltaTime;
	if (direction == BACKWARD) position -= front * movementSpeed * deltaTime;
	if (direction == RIGHT) position += right * movementSpeed * deltaTime;
	if (direction == LEFT) position -= right * movementSpeed * deltaTime;
}

void Camera::zoomIn(float zoomAmount)
{
	zoom -= zoomAmount;

	// clamp zoom
	if (zoom <= 1.0f) zoom = 1.0f;
	if (zoom > 45.0f) zoom = 45.0f;
}

void Camera::updateCameraVectors()
{
	glm::vec3 newFront;
	front.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
	front.y = sin(glm::radians(pitch));
	front.z = sin(glm::radians(yaw)) * cos(glm::radians(pitch));
	front = glm::normalize(front);
	right = glm::normalize(glm::cross(front, worldUp));
	up = glm::normalize(glm::cross(right, front));
}