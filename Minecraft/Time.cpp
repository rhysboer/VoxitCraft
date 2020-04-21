#include "Time.h"

float Time::deltaTime = 0.0f;
float Time::sinceLastUpdate = 0.0f;
float Time::totalTime = 0.0f;

void Time::Update() {
	deltaTime = glfwGetTime() - sinceLastUpdate;
	sinceLastUpdate = glfwGetTime();

	totalTime += deltaTime;
}

float Time::DeltaTime() {
	return deltaTime;
}

float Time::TotalTime() {
	return totalTime;
}
