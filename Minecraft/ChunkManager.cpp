#include "ChunkManager.h"

ChunkManager::ChunkManager() {
	solidShader = ShaderManager::AddShader("voxel", ShaderManager::ShaderType::GRAPHIC);
	waterShader = ShaderManager::AddShader("water", ShaderManager::ShaderType::GRAPHIC);

	solidShader->SetInt("texture1", 0);
	waterShader->SetInt("texture1", 0);

	terrainTexture = new TileMap("./data/textures/tileset.png", 8, 8);
	waterTexture = new TileMap("./data/textures/water.png", 16, 1);

	thread = std::thread(&ChunkManager::ChunkLoader, this);
}

ChunkManager::~ChunkManager() {
	delete solidShader;
	delete terrainTexture;

	ending = true;
	thread.join();
}

// MULTITHREAD TEST
void ChunkManager::ChunkLoader() {
	const unsigned int size = 12; 
	glm::ivec2 lastChunk = glm::ivec2(0);


	// Lamdba - Update all neighbours around chunk
	auto UpdateNeighbours = [this](const int& x, const int& z) {
		std::unique_lock<std::mutex> lock(mutex);
		lock.unlock();

		for(int innerZ = -1; innerZ <= 1; innerZ++) {
			for(int innerX = -1; innerX <= 1; innerX++) {
				if(innerZ == 0 && innerX == 0)
					continue;

				Chunk* neighbour = CreateChunk(glm::ivec2(x + innerX, z + innerZ));
				if(!neighbour->hasWorldData) {
					lock.lock();
					worldGen.CreateChunkWorldData(*neighbour);
					lock.unlock();
					std::this_thread::sleep_for(std::chrono::microseconds(5));
				}
			}
		}
	};

	while(ending == false) {
		for(int i = 0; i <= size; i++) {
			int minX = lastChunk.x + -i;
			int maxX = lastChunk.x + i;
			int minZ = lastChunk.y + -i;
			int maxZ = lastChunk.y + i;

			for(int z = minZ; z <= maxZ; z++) {
				for(int x = minX; x <= maxX; x++) {
					Chunk* chunk = CreateChunk(glm::ivec2(x, z));

					if(!chunk->hasWorldData) {

						// Create neighbour data
						UpdateNeighbours(x, z);
	
						std::unique_lock<std::mutex> lock(mutex);
						worldGen.CreateChunkWorldData(*chunk);
						chunk->GenerateMeshData();
					} else if (chunk->isDirty){

						UpdateNeighbours(x, z);

						//std::unique_lock<std::mutex> lock(mutex);
						chunk->GenerateMeshData();
					} else {
						continue;
					}

					std::this_thread::sleep_for(std::chrono::microseconds(50));
				}
			}

			std::this_thread::sleep_for(std::chrono::microseconds(50));
			
			glm::ivec2 currChunk = glm::ivec2(position.x / 16, position.z / 16);
			if(currChunk != lastChunk) {
				i = -1;
				lastChunk = currChunk;
			}
		}

		std::this_thread::sleep_for(std::chrono::milliseconds(1));
	}

}

void ChunkManager::Update() {
}

void ChunkManager::Render(Camera& camera) {
	std::unique_lock<std::mutex> lock(mutex);


	// Solid Rendering
	terrainTexture->BindTexture(0);
	solidShader->SetMatrix4("projectionView", camera.ProjectionView());

	std::unordered_map<glm::ivec2, Chunk*>::iterator iter = chunks.begin();
	for(; iter != chunks.end(); iter++) {
		if(!iter->second->hasWorldData)
			continue;

		if(iter->second->meshUpdate == true) {
			iter->second->CreateMesh();
		}

		iter->second->Render(*solidShader, *waterShader);
	}

	// Opaque Rendering
	waterTexture->BindTexture(0);
	waterShader->SetMatrix4("projectionView", camera.ProjectionView());
	waterShader->SetFloat("time", Time::TotalTime());

	iter = chunks.begin();
	for(; iter != chunks.end(); iter++) {
		if(!iter->second->hasWorldData)
			continue;

		if(iter->second->meshUpdate == true) {
			iter->second->CreateMesh();
		}

		iter->second->RenderOpaque(*waterShader);
	}


	// TEST
	position = camera.Position();
}

Chunk* ChunkManager::CreateChunk(const glm::ivec2& index) {
	Chunk* chunk = FindChunk(index);
	if(chunk != nullptr) return chunk;

	chunk = new Chunk(index, *this);
	chunks.emplace(index, chunk);

	return chunk;
}

BlockIDs ChunkManager::GetBlock(const glm::vec3& worldPosition) const {
	if(worldPosition.y >= Chunk::CHUNK_HEIGHT || worldPosition.y < 0.0f)
		return BlockIDs::AIR;

	Chunk* chunk = FindChunk(worldPosition);
	if(chunk)
		return chunk->GetBlock(worldPosition);

	return BlockIDs::AIR;
}

void ChunkManager::SetBlock(const glm::vec3& worldPosition, const BlockIDs& block) {
	return SetBlock(worldPosition.x, worldPosition.y, worldPosition.z, block);
}

void ChunkManager::SetBlock(const float& x, const float& y, const float& z, const BlockIDs& block) {
	if(y >= Chunk::CHUNK_HEIGHT || y < 0)
		return;

	Chunk* chunk = FindChunk(x, z);
	if(chunk == nullptr)
		chunk = CreateChunk(glm::ivec2(std::floor(x / Chunk::CHUNK_SIZE), std::floor(z / Chunk::CHUNK_SIZE)));

	chunk->SetBlock(x, y, z, block);
	cacheChunk = chunk;
}

Chunk* ChunkManager::FindChunk(const glm::ivec2& index) const {
	if(cacheChunk != nullptr && cacheChunk->localCoord == index)
		return cacheChunk;

	auto iter = chunks.find(index);
	return (iter != chunks.end()) ? iter->second : nullptr;
}

Chunk* ChunkManager::FindChunk(const glm::vec3& worldPosition) const {
	glm::ivec2 key = glm::ivec2(std::floor(worldPosition.x / Chunk::CHUNK_SIZE), std::floor(worldPosition.z / Chunk::CHUNK_SIZE));
	
	if(cacheChunk != nullptr && cacheChunk->localCoord == key)
		return cacheChunk;

	auto iter = chunks.find(key);
	return (iter != chunks.end()) ? iter->second : nullptr;
}

Chunk* ChunkManager::FindChunk(const float& x, const float& z) const {
	glm::ivec2 key = glm::ivec2(std::floor(x / Chunk::CHUNK_SIZE), std::floor(z / Chunk::CHUNK_SIZE));

	if(cacheChunk != nullptr && cacheChunk->localCoord == key)
		return cacheChunk;

	auto iter = chunks.find(key);
	return (iter != chunks.end()) ? iter->second : nullptr;
}
