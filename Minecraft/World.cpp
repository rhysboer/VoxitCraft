#include "World.h"

ChunkManager* World::chunkManager = nullptr;
EntityPlayer* World::player = nullptr;
const glm::vec3 World::worldGravity = glm::vec3(0, -0.5, 0);


World::World() {
	BlockManager::Init();
	player = new EntityPlayer(glm::vec3(0, 50, 0));

	skybox = new SkyBox("./data/textures/skybox/");
	
	// Create managers
	chunkManager = new ChunkManager();
}

World::~World() {
	delete skybox;
	delete chunkManager;

	delete player;
	BlockManager::Destroy();
}

void World::Update() {
	chunkManager->Update();
	EntityManager::Update();
}

void World::Render() {
	skybox->Render(*player->Camera());

	EntityManager::Render(*player->Camera());

	chunkManager->Render(*player->Camera());
}

ChunkManager& World::GetChunkManager() {
	return *chunkManager;
}

EntityPlayer& World::GetPlayer() {
	return *player;
}
