#pragma once

#include <GLFW/glfw3.h>

class Time
{
public:
	void tick();
	static float getDeltaTime() { return deltaTime; };
private:
	static float deltaTime;
	float lastFrame;
};

