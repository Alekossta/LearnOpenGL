#include "Time.h"

float Time::deltaTime = 0.0f;

void Time::tick() {
    float currentFrame = glfwGetTime();
    deltaTime = currentFrame - lastFrame;
    lastFrame = currentFrame;
}