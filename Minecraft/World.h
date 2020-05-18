#pragma once
#include "Camera3D.h"
#include "ChunkManager.h"
#include "BlockManager.h"
#include "EntityManager.h"
#include "Entity.h"
#include "SkyBox.h"

#include "EntityPlayer.h"
#include "FPSCamera.h"

class World {
public:
	const static glm::vec3 worldGravity;

	World();
	~World();

	void Update();
	void Render();

	static ChunkManager& GetChunkManager();
	static EntityPlayer& GetPlayer();

private:

	static ChunkManager* chunkManager;
	static EntityPlayer* player;

	SkyBox* skybox;
};

