#pragma once
#include "Engine.h"
#include "World.h"
#include "PostProcess.h"
#include "Camera2D.h"

#include "Renderer.h"
#include "Object3D.h"

#include "Raycast.h"

#include "Util.h"

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
	
	// Post processing and framebuffer
	PostProcessEffect effect;
	PostProcess* postProcessing;
	Framebuffer* framebuffer;
	
	// Held Block Preview
	Framebuffer* blockBuffer;
	Object3D* selectedBlock;
	BaseCamera* camera2D;

	// Block Highlighting
	Object3D* blockHighlight;
	int currentBlock;

	// World
	World* world;

	glm::vec2 lastResolution;
	bool lockCursor = true;
};

