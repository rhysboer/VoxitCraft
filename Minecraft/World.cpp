#include "World.h"

ChunkManager* World::chunkManager = nullptr;
EntityPlayer* World::player = nullptr;
ParticleSystem* World::particleSystem = nullptr;
const glm::vec3 World::worldGravity = glm::vec3(0, -0.5, 0);


World::World() {
	BlockManager::Init();
	player = new EntityPlayer(glm::vec3(0, 50, 0));


	skybox = new SkyBox("./data/textures/skybox/");
	
	// Create managers
	chunkManager = new ChunkManager();

	particleSystem = new ParticleSystem();

}

World::~World() {
	delete skybox;
	delete chunkManager;
	delete particleSystem;

	delete player;
	BlockManager::Destroy();
}

void World::Update() {
	skybox->GetShader()->SetFloat("timeOfDay", Time::TotalTime());

	chunkManager->Update();
	EntityManager::Update();

	particleSystem->Update();
}

void World::Render() {
	skybox->Render(*player->Camera());

	EntityManager::Render(*player->Camera());

	chunkManager->Render(*player->Camera());

	particleSystem->RenderEmitters(*player->Camera());
}

ChunkManager& World::GetChunkManager() {
	return *chunkManager;
}

EntityPlayer& World::GetPlayer() {
	return *player;
}

ParticleSystem& World::GetParticleManager() {
	return *particleSystem;
}

void World::CreateBreakParticleEffect(const BlockIDs& blockType, const glm::vec3& position) {
	World::GetParticleManager().AddEmitter(new ParticleEmitter(glm::floor(position) + glm::vec3(0.5), 24, blockType));
}
