#pragma once
#include "Camera.h"
#include "ChunkManager.h"
#include "BlockManager.h"
#include "SkyBox.h"

class World {
public:

	World();
	~World();

	void Update();
	void Render(Camera& camera);

	ChunkManager& GetChunkManager();

private:

	SkyBox* skybox;

	ChunkManager* chunkManager;
};

