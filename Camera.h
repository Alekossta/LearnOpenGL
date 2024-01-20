#pragma once

#include <glad/glad.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

enum Direction
{
	FORWARD,
	BACKWARD,
	LEFT,
	RIGHT
};

class Camera
{
public:

	// variables
	glm::vec3 position;
	glm::vec3 front;
	glm::vec3 up;
	glm::vec3 right;
	glm::vec3 worldUp;

	float yaw;
	float pitch;

	float movementSpeed;
	float mouseSensitivity;
	float zoom;

	// constructor
	Camera();

	// functions
	glm::mat4 getViewMatrix();
	void updateCameraVectors();
	void zoomIn(float zoomAmount);
	void rotate(float xoffset, float yoffset);
	void move(Direction direction, float deltaTime);
};