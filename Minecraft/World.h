#pragma once
#include "Camera3D.h"
#include "ChunkManager.h"
#include "BlockManager.h"
#include "EntityManager.h"
#include "Entity.h"
#include "SkyBox.h"
#include "ParticleSystem.h"

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
	static ParticleSystem& GetParticleManager();

	static void CreateBreakParticleEffect(const BlockIDs& blockType, const glm::vec3& position);

private:

	static ParticleSystem* particleSystem;
	static ChunkManager* chunkManager;
	static EntityPlayer* player;

	SkyBox* skybox;
};

