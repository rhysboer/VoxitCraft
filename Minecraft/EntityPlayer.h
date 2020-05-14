#pragma once
#include "Entity.h"
#include "Input.h"
#include "Time.h"
#include "imgui.h"
#include "Sprite.h"
#include "Raycast.h"
#include "AABB.h"

#include "Util.h"

class FPSCamera;

class EntityPlayer : public Entity {
public:

	EntityPlayer(const glm::vec3& position);
	~EntityPlayer();

	virtual void OnUpdate() override;

	FPSCamera* Camera();
	
	BlockIDs GetHeldBlock();
	
private:

	int currentBlock;

	Sprite* crosshair;
	FPSCamera* camera;
};

