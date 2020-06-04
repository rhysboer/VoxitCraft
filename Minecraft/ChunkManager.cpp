#include "ChunkManager.h"
#include "World.h"

ChunkManager::ChunkManager() {
	solidShader = ShaderManager::AddShader("voxel", ShaderManager::ShaderType::GRAPHIC);
	waterShader = ShaderManager::AddShader("water", ShaderManager::ShaderType::GRAPHIC);

	solidShader->SetTextureUnit("terrainTexture", 3);
	solidShader->SetTextureUnit("bloomTexture", 4);
	waterShader->SetTextureUnit("texture1", 0);

	WorldGeneration::Init();

	chunkGenerationThread = new std::thread(&ChunkManager::ChunkLoader, this);

	// TODO: Implement a loading wait
	std::this_thread::sleep_for(std::chrono::milliseconds(1));
}

ChunkManager::~ChunkManager() {
	if(chunkGenerationThread != nullptr) {
		appEnding = true;
		chunkGenerationThread->join();

		delete chunkGenerationThread;
	}

	std::unordered_map<glm::ivec2, Chunk*>::iterator iter = chunks.begin();
	for(; iter != chunks.end();) {
		if(iter->second != nullptr) {
			delete iter->second;
			iter->second = nullptr;

			if(iter == chunks.end()) {
				chunks.erase(iter);
				break;
			} else {
				iter = chunks.erase(iter);
			}
		}
	}
}

void ChunkManager::ChunkLoader() {
	glm::ivec2 lastChunk = glm::ivec2(0);
	
	//const glm::ivec2 offset[8] = { glm::ivec2(-1, 0), glm::ivec2(0, 1), glm::ivec2(1, 0), glm::ivec2(0, -1), glm::ivec2(-1, -1), glm::ivec2(1, -1), glm::ivec2(-1, 1), glm::ivec2(1, 1) };
	
	std::unique_lock<std::mutex> lock(mutex);
	lock.unlock();
	
	while(appEnding == false) {
		for(int i = 0; i <= RENDERING_DISTANCE; i++) {
			int minX = lastChunk.x + -i;
			int maxX = lastChunk.x + i;
			int minZ = lastChunk.y + -i;
			int maxZ = lastChunk.y + i;
	
			for(int z = minZ; z <= maxZ; z++) {
				for(int x = minX; x <= maxX; x++) {

					// Thread Safety
					Chunk* chunk = CreateChunk(glm::ivec2(x, z));
			
					if(!chunk->hasWorldData) {
						data.fill(BlockIDs::AIR);
						structurePos.clear();
						structureBlocks.clear();
	
						// Generate World Data
						int height = worldGen.CreateChunkWorldData(*chunk, data, structurePos, structureBlocks);
						if(height == 0)
							continue;
	
						
						// Add neighbouring structure overhangs
						SetBlocks(structurePos, structureBlocks);


						lock.lock();
						{
							// Set Chunk world data
							chunk->SetWorldData(data, height);
							// Generate Mesh
							chunk->GenerateMeshData();
						}
						lock.unlock();

	
						std::this_thread::sleep_for(std::chrono::microseconds(50));
					}
				}
			}
			
			glm::ivec2 currChunk = glm::ivec2(glm::floor(World::GetPlayer().GetPosition().x / Chunk::CHUNK_SIZE), glm::floor(World::GetPlayer().GetPosition().z / Chunk::CHUNK_SIZE));
			if(currChunk != lastChunk) {
 				i = -1;
				lastChunk = currChunk;
			}
	
			if(appEnding == true)
				break;
	
			std::this_thread::sleep_for(std::chrono::microseconds(50));
		}
	
		std::this_thread::sleep_for(std::chrono::microseconds(50));
	}
	
}

void ChunkManager::Update() {
	solidShader->SetFloat("timeOfDay", Time::TotalTime());
	waterShader->SetFloat("timeOfDay", Time::TotalTime());
}

void ChunkManager::Render(BaseCamera& camera) {
	std::unique_lock<std::mutex> lock(mutex);

	// Solid Rendering
	BlockManager::GetTileMap(SpriteSheet::BLOCK)->BindTexture(3);
	BlockManager::GetTileMap(SpriteSheet::BLOOM)->BindTexture(4);
	solidShader->SetMatrix4("projectionView", camera.ProjectionView());

	int renderCount = 0;

	// Current chunk the player is in
	glm::ivec3 playerChunk = glm::floor(World::GetPlayer().GetPosition() / (float)Chunk::CHUNK_SIZE);

	std::unordered_map<glm::ivec2, Chunk*>::iterator iter = chunks.begin();
	for(; iter != chunks.end();) {
		glm::ivec2 distance = glm::abs(glm::ivec2(iter->second->GetLocalCoords().x - playerChunk.x, iter->second->GetLocalCoords().y - playerChunk.z));

		// Delete Chunk out of destroy distance
		if(distance.x > DESTROY_DISTANCE || distance.y > DESTROY_DISTANCE) {
			delete iter->second;
			iter->second = nullptr;
		
			if(iter == chunks.end()) {
				chunks.erase(iter);
				break;
			} else {
				iter = chunks.erase(iter);
			}
		
			continue;
		}

		// Dont render chunk out of render distance
		if(distance.x >= RENDERING_DISTANCE || distance.y >= RENDERING_DISTANCE) {
			++iter;
			continue;
		}

		// If Chunk has no world data
		if(!iter->second->hasWorldData) {
			++iter;
			continue;
		}

		// Regenerate Mesh
		if(iter->second->isDirty)
			iter->second->GenerateMeshData();
		
		// Upload Mesh to GPU
		if(iter->second->uploadMeshToGPU == true)
			iter->second->CreateMesh();

		// Check if the chunk is in camera frustum
		if(camera.IsAABBInFrustum(*iter->second->GetAABB()) == false) {
			++iter;
			continue;
		}

		// Render Mesh
		iter->second->Render(*solidShader);
		++iter;

		renderCount++;
	}

	// Opaque Rendering
	BlockManager::GetTileMap(SpriteSheet::WATER)->BindTexture(0);
	waterShader->SetMatrix4("projectionView", camera.ProjectionView());
	waterShader->SetFloat("time", Time::TotalTime());

	iter = chunks.begin();
	glDisable(GL_CULL_FACE);
	for(; iter != chunks.end(); iter++) {
		// If Chunk has no world data
		if(!iter->second->hasWorldData)
			continue;

		glm::ivec2 distance = glm::abs(glm::ivec2(iter->second->GetLocalCoords().x - playerChunk.x, iter->second->GetLocalCoords().y - playerChunk.z));
		if(distance.x >= RENDERING_DISTANCE || distance.y >= RENDERING_DISTANCE)
			continue;

		if(camera.IsAABBInFrustum(*iter->second->GetAABB()) == false)
			continue;

		iter->second->RenderOpaque(*waterShader);
	}
	glEnable(GL_CULL_FACE);

	ImGui::Begin("Render Info");
	ImGui::Text("Rendered Chunks: %i / %i", renderCount, chunks.size());
	ImGui::End();

}

Chunk* ChunkManager::CreateChunk(const glm::ivec2& index) {
	Chunk* chunk = FindChunkLock(index);
	if(chunk != nullptr) return chunk;


	chunk = new Chunk(index, *this);
	
	std::unique_lock<std::mutex> lock(mutex);
	chunks.emplace(index, chunk);

	return chunk;
}

BlockIDs ChunkManager::GetBlock(const float& x, const float& y, const float& z) {
	if(y >= Chunk::CHUNK_HEIGHT || y < 0.0f)
		return BlockIDs::AIR;

	Chunk* chunk = FindChunk(x, z);
	if(!chunk)
		return BlockIDs::AIR;

	return chunk->GetBlock(x, y, z);
}

BlockIDs ChunkManager::GetBlock(const glm::vec3& worldPosition) {
	return GetBlock(worldPosition.x, worldPosition.y, worldPosition.z);
}

BlockIDs ChunkManager::GetBlockLock(const float& x, const float& y, const float& z) {
	std::unique_lock<std::mutex> lock(mutex);
	return GetBlock(x, y, z);
}

void ChunkManager::SetBlock(const glm::vec3& worldPosition, const BlockIDs& block) {
	return SetBlock(worldPosition.x, worldPosition.y, worldPosition.z, block);
}

void ChunkManager::SetBlocks(const std::vector<glm::vec3>& pos, std::vector<BlockIDs>& blocks) {

	int blockSize = blocks.size();

	for(int i = 0; i < pos.size(); i++) {
		Chunk* chunk = FindChunk(pos[i].x, pos[i].z);

		if(chunk == nullptr)
			chunk = CreateChunk(glm::ivec2(glm::floor(pos[i].x / Chunk::CHUNK_SIZE), glm::floor(pos[i].z / Chunk::CHUNK_SIZE)));

		std::unique_lock<std::mutex> lock(mutex);
		chunk->SetBlock(pos[i], (i < blockSize) ? blocks[i] : BlockIDs::AIR);
	}
}

void ChunkManager::GetSolidBlocksInArea(const glm::vec3& worldPosition, const glm::vec3& size, std::vector<glm::vec3>& output) {
	float xMin = glm::floor(worldPosition.x - size.x / 2.0f);
	float xMax = glm::floor(worldPosition.x + size.x / 2.0f);
	float yMin = glm::floor(worldPosition.y - size.y / 2.0f);
	float yMax = glm::floor(worldPosition.y + size.y / 2.0f);
	float zMin = glm::floor(worldPosition.z - size.z / 2.0f);
	float zMax = glm::floor(worldPosition.z + size.z / 2.0f);

	for(int x = xMin; x <= xMax; x++) {
		for(int y = yMin; y <= yMax; y++) {
			for(int z = zMin; z <= zMax; z++) {
				if(BlockManager::GetBlockData(GetBlock(x, y, z))->walkThrough == true) {
					output.emplace_back(x, y, z);
				}
			}
		}
	}
}

// X, Y & Z are World Position
void ChunkManager::SetBlock(const float& x, const float& y, const float& z, const BlockIDs& block) {
	if(y >= Chunk::CHUNK_HEIGHT || y < 0)
		return;

	Chunk* chunk = FindChunk(x, z);
	if(chunk == nullptr)
		chunk = CreateChunk(glm::ivec2(std::floor(x / Chunk::CHUNK_SIZE), std::floor(z / Chunk::CHUNK_SIZE)));

	chunk->SetBlock(x, y, z, block);
}

Chunk* ChunkManager::FindChunk(const glm::ivec2& index) {
	auto iter = chunks.find(index);
	return (iter != chunks.end()) ? iter->second : nullptr;
}

Chunk* ChunkManager::FindChunk(const glm::vec3& worldPosition) {
	return FindChunk(glm::ivec2(std::floor(worldPosition.x / Chunk::CHUNK_SIZE), std::floor(worldPosition.z / Chunk::CHUNK_SIZE)));
}

// Returns nullptr if chunk doesn't exists, X & Z are worldPosition
Chunk* ChunkManager::FindChunk(const float& x, const float& z) {
	return FindChunk(glm::ivec2(std::floor(x / Chunk::CHUNK_SIZE), std::floor(z / Chunk::CHUNK_SIZE)));
}

Chunk* ChunkManager::FindChunkLock(const glm::ivec2& index) {
	std::unique_lock<std::mutex> lock(mutex);
	return FindChunk(index);
}
