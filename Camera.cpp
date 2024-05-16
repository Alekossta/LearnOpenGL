#include "Camera.h"


Camera::Camera(float windowWidth, float windowHeight) : lastMouseX(windowWidth/2), lastMouseY(windowHeight/2)
{
}

void Camera::rotate(double xpos, double ypos)
{
    if (firstMouse)
    {
        lastMouseX = xpos;
        lastMouseY = ypos;
        firstMouse = false;
        return;
    }

    float xoffset = xpos - lastMouseX;
    float yoffset = lastMouseY - ypos;
    lastMouseX = xpos;
    lastMouseY = ypos;

    cameraYaw += xoffset * mouseSensitivity;
    cameraPitch += yoffset * mouseSensitivity;

    if (cameraPitch > 89.0f)
        cameraPitch = 89.0f;
    if (cameraPitch < -89.f)
        cameraPitch = -89.f;

    glm::vec3 direction;
    direction.x = cos(glm::radians(cameraYaw)) * cos(glm::radians(cameraPitch));
    direction.y = sin(glm::radians(cameraPitch));
    direction.z = sin(glm::radians(cameraYaw)) * cos(glm::radians(cameraPitch));
    cameraFront = glm::normalize(direction);
}

void Camera::move(GLFWwindow* window)
{
    const float cameraSpeed = 5.f * Time::getDeltaTime();
    // CAMERA MOVEMENT
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
        cameraPos += glm::normalize(glm::cross(cameraFront, cameraUp)) * cameraSpeed;
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
        cameraPos -= glm::normalize(glm::cross(cameraFront, cameraUp)) * cameraSpeed;


    if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS)
    {
        cameraPos += glm::vec3(0, 1, 0) * cameraSpeed;
        return;
    }

    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
        cameraPos += cameraSpeed * cameraFront;
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
        cameraPos -= cameraSpeed * cameraFront;

}

glm::mat4 Camera::getViewMatrix()
{
    return glm::lookAt(cameraPos, cameraPos + cameraFront, cameraUp);
}
