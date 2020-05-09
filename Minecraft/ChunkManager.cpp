#include "ChunkManager.h"
#include "World.h"

ChunkManager::ChunkManager() {
	solidShader = ShaderManager::AddShader("voxel", ShaderManager::ShaderType::GRAPHIC);
	waterShader = ShaderManager::AddShader("water", ShaderManager::ShaderType::GRAPHIC);

	solidShader->SetInt("texture1", 0);
	waterShader->SetInt("texture1", 0);

	terrainTexture = new TileMap("./data/textures/tileset.png", 8, 8);
	waterTexture = new TileMap("./data/textures/water.png", 16, 1);

	thread = std::thread(&ChunkManager::ChunkLoader, this);

	// TODO: Implement a loading wait
	std::this_thread::sleep_for(std::chrono::milliseconds(1));
}

ChunkManager::~ChunkManager() {
	delete solidShader;
	delete terrainTexture;

	ending = true;
	thread.join();
}

// MULTITHREAD TEST
void ChunkManager::ChunkLoader() {
	glm::ivec2 lastChunk = glm::ivec2(0);

	/*
	Plan A:
		1. If a chunk doesnt exist in this location, create one and lock it
		2. If chunk does NOT have worldData, create it.
			2-A. If a tree leaves are inside another chunk and said chunk doesnt exist, create it blankly.
		3. If chunk didnt exist, add it to the chunk list.
	
		Rendering: If chunk is dirty, lock it, recreate mesh and apply it. Continue
		Placing: Add block and make chunk dirty

	Plan B:
		1. If a chunk doesnt exist in this location, create one and add it to the list
		2. If chunk does NOT have worldData, Create world data and store it all into a buffer.
			2-A. If a tree leaves are inside another chunk, create that chunk and add it to the chunk list.
		3. After world data is created, lock it and add it all to the chunk.

		Rendering: If chunk is dirty, lock it, recreate mesh and apply it. Continue
		Placing: Add block and make chunk dirty
	*/


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

	std::array<BlockIDs, Chunk::CHUNK_MASS> data = std::array<BlockIDs, Chunk::CHUNK_MASS>();
	std::vector<BlockIDs> structureBlocks = std::vector<BlockIDs>();
	std::vector<glm::vec3> structurePos = std::vector<glm::vec3>();

	while(ending == false) {
		for(int i = 0; i <= RENDERING_DISTANCE; i++) {
			int minX = lastChunk.x + -i;
			int maxX = lastChunk.x + i;
			int minZ = lastChunk.y + -i;
			int maxZ = lastChunk.y + i;

			for(int z = minZ; z <= maxZ; z++) {
				for(int x = minX; x <= maxX; x++) {
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

						// Lock chunk
						std::unique_lock<std::mutex> lock(mutex);

						// Set world data
						chunk->SetWorldData(data, height);
						// Generate Mesh
						chunk->GenerateMeshData();
			
						std::this_thread::sleep_for(std::chrono::microseconds(50));
					}
				}
			}
			
			glm::ivec2 currChunk = glm::ivec2(World::GetPlayer().GetPosition().x / 16, World::GetPlayer().GetPosition().z / 16);
			if(currChunk != lastChunk) {
				i = -1;
				lastChunk = currChunk;
			}

			std::this_thread::sleep_for(std::chrono::microseconds(50));
			// Old
			//for(int z = minZ; z <= maxZ; z++) {
			//	for(int x = minX; x <= maxX; x++) {
			//		Chunk* chunk = CreateChunk(glm::ivec2(x, z));
			//
			//		if(!chunk->hasWorldData) {
			//
			//			// Create neighbour data
			//			//UpdateNeighbours(x, z);
			//
			//			std::unique_lock<std::mutex> lock(mutex);
			//			worldGen.CreateChunkWorldData(*chunk);
			//			chunk->GenerateMeshData();
			//		} else if (chunk->isDirty){
			//
			//			//UpdateNeighbours(x, z);
			//
			//			//std::unique_lock<std::mutex> lock(mutex);
			//			//chunk->GenerateMeshData();
			//		} else {
			//			continue;
			//		}
			//
			//		std::this_thread::sleep_for(std::chrono::microseconds(50));
			//	}
			//}
			//
			//std::this_thread::sleep_for(std::chrono::microseconds(50));
			//
			//glm::ivec2 currChunk = glm::ivec2(World::GetPlayer().GetPosition().x / 16, World::GetPlayer().GetPosition().z / 16);
			//if(currChunk != lastChunk) {
			//	i = -1;
			//	lastChunk = currChunk;
			//}
		}

		std::this_thread::sleep_for(std::chrono::milliseconds(1));
	}

}

void ChunkManager::Update() {
	ImGui::Begin("TOTAL TIME");
	ImGui::Text("Average Mesh Data Gen Time: %f", DELETTHIS::GetAverage2());
	ImGui::Text("Average Mesh To GPU Time: %f", DELETTHIS::GetAverage());
	ImGui::End();
}

void ChunkManager::Render(BaseCamera& camera) {
	std::unique_lock<std::mutex> lock(mutex);

	// Solid Rendering
	terrainTexture->BindTexture(0);
	solidShader->SetMatrix4("projectionView", camera.ProjectionView());

	std::unordered_map<glm::ivec2, Chunk*>::iterator iter = chunks.begin();
	for(; iter != chunks.end(); iter++) {
		// If Chunk has no world data
		if(!iter->second->hasWorldData)
			continue;

		// Regenerate Mesh
		if(iter->second->isDirty)
			iter->second->GenerateMeshData();
		
		// Upload Mesh to GPU
		if(iter->second->uploadMeshToGPU == true)
			iter->second->CreateMesh();

		// Render Mesh
		iter->second->Render(*solidShader);
	}

	// Opaque Rendering
	waterTexture->BindTexture(0);
	waterShader->SetMatrix4("projectionView", camera.ProjectionView());
	waterShader->SetFloat("time", Time::TotalTime());

	iter = chunks.begin();
	glDisable(GL_CULL_FACE);
	for(; iter != chunks.end(); iter++) {
		// If Chunk has no world data
		if(!iter->second->hasWorldData)
			continue;

		// Regenerate Mesh
		//if(iter->second->isDirty)
		//	iter->second->GenerateMeshData();

		// Upload Mesh to GPU
		//if(iter->second->uploadMeshToGPU == true)
		//	iter->second->CreateMesh();

		iter->second->RenderOpaque(*waterShader);
	}
	glEnable(GL_CULL_FACE);
}

Chunk* ChunkManager::CreateChunk(const glm::ivec2& index) {
	Chunk* chunk = FindChunk(index);
	if(chunk != nullptr) return chunk;

	chunk = new Chunk(index, *this);
	chunks.emplace(index, chunk);

	return chunk;
}

BlockIDs ChunkManager::GetBlock(const float& x, const float& y, const float& z) const {
	if(y >= Chunk::CHUNK_HEIGHT || y < 0.0f)
		return BlockIDs::AIR;

	Chunk* chunk = FindChunk(x, z);
	if(!chunk)
		return BlockIDs::AIR;

	return chunk->GetBlock(x, y, z);
}

BlockIDs ChunkManager::GetBlock(const glm::vec3& worldPosition) const {
	return GetBlock(worldPosition.x, worldPosition.y, worldPosition.z);
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
				if(BlockManager::GetBlockData(GetBlock(x, y, z)).isSolid == true) {
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
	//chunk->GenerateMeshData();

	cacheChunk = chunk;
}

Chunk* ChunkManager::FindChunk(const glm::ivec2& index) const {
	glm::ivec2 key = index;

	if(cacheChunk != nullptr && cacheChunk->localCoord == key)
		return cacheChunk;

	auto iter = chunks.find(key);
	return (iter != chunks.end()) ? iter->second : nullptr;
}

Chunk* ChunkManager::FindChunk(const glm::vec3& worldPosition) const {
	glm::ivec2 key = glm::ivec2(std::floor(worldPosition.x / Chunk::CHUNK_SIZE), std::floor(worldPosition.z / Chunk::CHUNK_SIZE));
	
	if(cacheChunk != nullptr && cacheChunk->localCoord == key)
		return cacheChunk;

	auto iter = chunks.find(key);
	return (iter != chunks.end()) ? iter->second : nullptr;
}

// Returns nullptr if chunk doesn't exists, X & Z are worldPosition
Chunk* ChunkManager::FindChunk(const float& x, const float& z) const {
	glm::ivec2 key = glm::ivec2(std::floor(x / Chunk::CHUNK_SIZE), std::floor(z / Chunk::CHUNK_SIZE));

	if(cacheChunk != nullptr && cacheChunk->localCoord == key)
		return cacheChunk;

	auto iter = chunks.find(key);
	return (iter != chunks.end()) ? iter->second : nullptr;
}
