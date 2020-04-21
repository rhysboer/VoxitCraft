#include "World.h"

World::World() {
	BlockManager::LoadBlockDatabase();
	skybox = new SkyBox("data/textures/skybox/");

	chunkManager = new ChunkManager();
}

World::~World() {
	delete chunkManager;
}

void World::Update() {
	chunkManager->Update();
}

void World::Render(Camera& camera) {
	skybox->Render(camera);
	chunkManager->Render(camera);
}

ChunkManager& World::GetChunkManager() {
	return *chunkManager;
}
