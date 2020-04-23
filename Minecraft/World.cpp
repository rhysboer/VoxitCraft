#include "World.h"

ChunkManager* World::chunkManager = nullptr;
const glm::vec3 World::worldGravity = glm::vec3(0, -0.5, 0);


World::World() {
	BlockManager::LoadBlockDatabase();
	skybox = new SkyBox("./data/textures/skybox/");

	// Create managers
	chunkManager = new ChunkManager();

	Entity* entity = new Entity("Entity", glm::vec3(0, 50.5f, 0), new ECPlayer());
}

World::~World() {
	delete chunkManager;
	delete skybox;
}

void World::Update() {
	chunkManager->Update();
	EntityManager::Update();
}

void World::Render(Camera& camera) {
	skybox->Render(camera);

	EntityManager::Render(camera);
	chunkManager->Render(camera);
}

ChunkManager& World::GetChunkManager() {
	return *chunkManager;
}
