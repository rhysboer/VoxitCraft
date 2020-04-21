#pragma once
#include "Engine.h"
#include "World.h"

class Application : public Engine{
public:

	Camera* camera;
	World* world;

private:
	// Inherited via Engine
	virtual void OnStart() override;
	virtual void OnEnd() override;
	virtual void OnUpdate() override;
	virtual void OnRender() override;

private:

	bool isWireFrame = false;


	// DEBUG
	glm::vec3 pos = glm::vec3();

};

