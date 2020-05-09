#pragma once
#include "Engine.h"
#include "World.h"
#include "PostProcess.h"
#include "Camera2D.h"

#include "Renderer.h"
#include "Object3D.h"

enum class PostProcessEffect {
	NORMAL,
	UNDER_WATER
};

class Application : public Engine {
public:

	static Application& Get();

	static void SetPostProcessEffect(const PostProcessEffect& effect);

private:

	// Inherited via Engine
	virtual void OnStart() override;
	virtual void OnEnd() override;
	virtual void OnUpdate() override;
	virtual void OnRender() override;

private:
	
	static Application* app;
	
	PostProcessEffect effect;
	PostProcess* postProcessing;
	Framebuffer* framebuffer;
	
	// Test
	Framebuffer* blockBuffer;
	BaseCamera* camera;
	Object3D* cube;

	World* world;

	glm::vec2 lastResolution;
	bool lockCursor = true;
	bool isWireFrame = false;
};

