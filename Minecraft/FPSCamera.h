#pragma once
#include "Camera3D.h"
#include "Input.h"

class Entity;

class FPSCamera : public Camera3D {
public:

	FPSCamera(Entity& entity, const glm::vec3& position);
	~FPSCamera();
	
	void Update();

protected:

	Entity* entity;
};

