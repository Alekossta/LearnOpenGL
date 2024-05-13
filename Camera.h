#pragma once

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <GLFW/glfw3.h>
#include "Time.h"

class Camera
{
public:
	Camera(float windowWidth, float windowHeight);
	void rotate(double xpos, double ypos);
	void move(GLFWwindow* window);
	glm::mat4 getViewMatrix();
	glm::vec3 getPos() { return cameraPos; };
	glm::vec3 getFront() { return cameraFront; };
private:
	bool firstMouse = true;
	float mouseSensitivity = 0.1f;
	float lastMouseX;
	float lastMouseY;
	glm::vec3 cameraPos = glm::vec3(0.0f, 0.0f, 3.0f);
	glm::vec3 cameraFront = glm::vec3(0.0f, 0.0f, -1.0f);
	glm::vec3 cameraUp = glm::vec3(0.0f, 1.0f, 0.0f);

	float cameraYaw = -90.f;
	float cameraPitch = 0.f;
};

