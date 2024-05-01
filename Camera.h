#pragma once

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

class Camera
{
public:
	Camera(float windowWidth, float windowHeight);
	void mouseMoved(double xpos, double ypos);
	glm::mat4 getViewMatrix();
private:
	bool firstMouse = true;
	float mouseSensitivity = 0.1f;
	float lastMouseX;
	float lastMouseY
};

