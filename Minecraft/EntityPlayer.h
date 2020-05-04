#pragma once
#include "Entity.h"
#include "Input.h"
#include "Time.h"
#include "imgui.h"
#include "Sprite.h"

class FPSCamera;

class EntityPlayer : public Entity {
public:

	EntityPlayer(const glm::vec3& position);
	~EntityPlayer();

	virtual void OnUpdate() override;

	FPSCamera* Camera();
private:

	Sprite* crosshair;
	FPSCamera* camera;
};

