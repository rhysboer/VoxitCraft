#pragma once
#include "glad/glad.h"
#include "GLFW/glfw3.h"

class Time {
public:
	static void Update();
	static float DeltaTime();
	static float TotalTime();
private:
	static float deltaTime;
	static float sinceLastUpdate;
	static float totalTime;
};

