#pragma once
#include "Camera.h"
#include "ChunkManager.h"
#include "BlockManager.h"
#include "EntityManager.h"
#include "Entity.h"
#include "SkyBox.h"

#include "ECPlayer.h"

class World {
public:
	const static glm::vec3 worldGravity;

	World();
	~World();

	void Update();
	void Render(Camera& camera);

	static ChunkManager& GetChunkManager();

private:

	static ChunkManager* chunkManager;

	SkyBox* skybox;

};

