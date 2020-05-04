#pragma once
#include "BaseCamera.h"
#include "Input.h"

class Entity;

class FPSCamera : public BaseCamera {
public:

	FPSCamera(Entity& entity, const glm::vec3& position);
	virtual ~FPSCamera() override;
	
	void Update();

private:

	Entity* entity;
};

