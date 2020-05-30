#include "Chunk.h"
#include "ChunkManager.h"

Chunk::Chunk(glm::ivec2 index, ChunkManager& world) :localCoord(index), hasWorldData(false), highestBlock(0) {
	this->chunkManager = &world;
	worldCoord = glm::vec3(index.x * CHUNK_SIZE, 0.0f, index.y * CHUNK_SIZE);
	
	solidMesh.vao = 0;
	waterMesh.vao = 0;

	for(int i = 0; i < (int)NEIGHBOUR::_TOTAL; i++)
		neighbourChunks[i] = nullptr;

	solidMesh.meshData.reserve(4500);

	memset(lightMap, 0, CHUNK_MASS);

	// Create chunks AABB
	aabb = new AABB(glm::vec3(worldCoord.x + (CHUNK_SIZE / 2.0f), CHUNK_HEIGHT / 2.0f, worldCoord.z + (CHUNK_SIZE / 2.0f)), glm::vec3(CHUNK_SIZE, CHUNK_HEIGHT, CHUNK_SIZE));
}

Chunk::~Chunk() {
	delete aabb;

	GetNeighbours();
	for(int i = 0; i < (int)NEIGHBOUR::_TOTAL; i++) {
		if(neighbourChunks[i] != nullptr) {
			neighbourChunks[i]->RemoveNeighbour(this);
			neighbourChunks[i] = nullptr;
		}
	}

	// Destroy chunk
	if(solidMesh.vao != 0) {
		glDeleteVertexArrays(1, &solidMesh.vao);
		glDeleteBuffers(1, &solidMesh.vbo);
		glDeleteBuffers(1, &solidMesh.ebo);
	}

	if(waterMesh.vao != 0) {
 		glDeleteVertexArrays(1, &waterMesh.vao);
		glDeleteBuffers(1, &waterMesh.vbo);
		glDeleteBuffers(1, &waterMesh.ebo);
	}
}

void Chunk::Render(Shader& solidShader) {
	if(solidMesh.vao == 0)
		return;

	// Solids
	glBindVertexArray(solidMesh.vao);
	solidShader.SetVector3("position", worldCoord);
	glDrawElements(GL_TRIANGLES, solidMesh.indicesCount, GL_UNSIGNED_INT, 0);
}

void Chunk::RenderOpaque(Shader& waterShader) {
	// Liquids
	if(waterMesh.vao > 0) {
		glBindVertexArray(waterMesh.vao);
		waterShader.SetVector3("position", worldCoord);
		glDrawElements(GL_TRIANGLES, waterMesh.indicesCount, GL_UNSIGNED_INT, 0);
	}
}

//							[ START SET & GET BLOCKS ]

#pragma region SET & GET BLOCKS

//							[ SET BLOCKS ]

// X, Y & Z are World Position
void Chunk::SetBlock(const int& x, const int& y, const int& z, const BlockIDs& block) {
	if(y < 0 && y > CHUNK_HEIGHT)
		return;

	if(!IsPositionInChunk(x - worldCoord.x, y - worldCoord.y, z - worldCoord.z)) {
		printf("Should not be reaching here: Chunk/SetBlock() failed to find block inside chunk! Chunk Index: X%i, Z%i \n ", localCoord.x, localCoord.y);
		return;
		//return chunkManager->SetBlock(x, y, z, block);
	}

	SetBlock(ToBlockIndex(x - worldCoord.x, y - worldCoord.y, z - worldCoord.z), block);
}

void Chunk::SetBlock(const glm::vec3& worldPosition, const BlockIDs& block) {
	if(worldPosition.y < 0 && worldPosition.y > CHUNK_HEIGHT)
		return;

	if(!IsPositionInChunk(worldPosition - worldCoord)) {
		printf("Should not be reaching here: Chunk/SetBlock() failed to find block inside chunk! Chunk Index: X%i, Z%i \n ", localCoord.x, localCoord.y);
		return;
		//return chunkManager->SetBlock(worldPosition, block);
	}

	SetBlock(ToBlockIndex(worldPosition - worldCoord), block);
}

void Chunk::SetBlock(const unsigned int& index, const BlockIDs& id) {
	if(index >= CHUNK_MASS)
		return;

	BlockData const * newblock = BlockManager::GetBlockData(id);
	BlockData const * oldBlock = BlockManager::GetBlockData(blocks[index]);
	
	int y = index / (CHUNK_SIZE * CHUNK_SIZE);

	// if new block is NOT transparent & old is block
	if(!newblock->isTransparent && oldBlock->isTransparent) {
		chunkSlice.solidBlocks[y] += 1;
		if(oldBlock->id != BlockIDs::AIR)
			chunkSlice.transparentBlocks[y] -= 1;
	} else if(newblock->isTransparent && !oldBlock->isTransparent) {
		chunkSlice.solidBlocks[y] -= 1;
		if(newblock->id != BlockIDs::AIR)
			chunkSlice.transparentBlocks[y] += 1;
	} else if(newblock->isTransparent && oldBlock->isTransparent) {
		if(newblock->id != BlockIDs::AIR && oldBlock->id != BlockIDs::AIR)
			chunkSlice.transparentBlocks[y] += 1;
	}

	highestBlock = glm::max(highestBlock, (int)glm::floor(index / CHUNK_SLICE));


	if(newblock->lightLevel > 0) {
		int x = index % CHUNK_SIZE;
		int y = index / CHUNK_SLICE;
		int z = (index % (CHUNK_SIZE * CHUNK_SIZE)) / CHUNK_SIZE;
	
		SetLight(x, y, z, newblock->lightLevel);
		lightQueue.emplace(index, this);
		this->SetDirty();
		CalculateLight();
	}


	// IF block is on the side of the chunk, dirty neighbour
	glm::vec3 pos = IndexToLocalPos(index);
	if(pos.x == 0)
		SetNeighbourDirty(NEIGHBOUR::LEFT);
	if(pos.x == CHUNK_SIZE - 1)
		SetNeighbourDirty(NEIGHBOUR::RIGHT);
	if(pos.z == 0)
		SetNeighbourDirty(NEIGHBOUR::BACK);
	if(pos.z == CHUNK_SIZE - 1)
		SetNeighbourDirty(NEIGHBOUR::FRONT);
	if(pos.x == 0 && pos.z == 0)
		SetNeighbourDirty(NEIGHBOUR::BACK_LEFT);
	if(pos.x == CHUNK_SIZE - 1 && pos.z == 0)
		SetNeighbourDirty(NEIGHBOUR::BACK_RIGHT);
	if(pos.x == 0 && pos.z == CHUNK_SIZE - 1)
		SetNeighbourDirty(NEIGHBOUR::FRONT_LEFT);
	if(pos.x == CHUNK_SIZE - 1 && pos.z == CHUNK_SIZE - 1)
		SetNeighbourDirty(NEIGHBOUR::FRONT_RIGHT);

	// Set Block
	blocks[index] = id;
	this->isDirty = true;
}

//							[ GET BLOCKS ]

// Returns the block at position, X, Y & Z are in World Position.
BlockIDs Chunk::GetBlock(const float& x, const float& y, const float& z) const {
	return GetBlock(glm::vec3(x, y, z));
}

BlockIDs Chunk::GetBlock(const glm::vec3& worldPosition) const {
	glm::vec3 blockPos = (glm::floor(worldPosition)) - worldCoord;

	if(IsPositionInChunk(blockPos))
		return blocks[ToBlockIndex(blockPos)];

	return chunkManager->GetBlock(worldPosition);
}

BlockIDs Chunk::GetBlockLocal(const float& x, const float& y, const float& z) const {
	if(IsPositionInChunk(x, y, z))
		return blocks[ToBlockIndex(x, y, z)];
	return BlockIDs::AIR;
}

glm::vec2 Chunk::GetLocalCoords() const {
	return localCoord;
}

// X, Y & Z are in local coords
BlockIDs Chunk::GetChunkOrNeighbourBlock(const float& x, const float& y, const float& z) {
	// If position is above or under the chunk
	if(y < 0 || y >= CHUNK_HEIGHT)
		return BlockIDs::AIR;
	
	// If inside chunk
	if(IsPositionInChunk(x, y, z))
		return blocks[ToBlockIndex(x, y, z)];
	
	glm::ivec2 index = glm::ivec2(glm::floor(x / CHUNK_SIZE), glm::floor(z / CHUNK_SIZE));

	NEIGHBOUR neighbour;
	if(index.x == 1 && index.y == 0) neighbour = NEIGHBOUR::RIGHT;
	else if(index.x == -1 && index.y == 0) neighbour = NEIGHBOUR::LEFT;
	else if(index.x == 0 && index.y == 1) neighbour = NEIGHBOUR::FRONT;
	else if(index.x == 0 && index.y == -1) neighbour = NEIGHBOUR::BACK;
	else if(index.x == -1 && index.y == -1) neighbour = NEIGHBOUR::BACK_LEFT;
	else if(index.x == 1 && index.y == -1) neighbour = NEIGHBOUR::BACK_RIGHT;
	else if(index.x == -1 && index.y == 1) neighbour = NEIGHBOUR::FRONT_LEFT;
	else neighbour = NEIGHBOUR::FRONT_RIGHT;

	if(neighbourChunks[(int)neighbour] == nullptr)
		return BlockIDs::AIR;
	
	return neighbourChunks[(int)neighbour]->GetBlockLocal(Math::Modulo(x, CHUNK_SIZE), y, Math::Modulo(z, CHUNK_SIZE));
}

AABB const* Chunk::GetAABB() const {
	return aabb;
}

void Chunk::GetNeighbours() {
	for(int i = 0; i < (int)NEIGHBOUR::_TOTAL; i++) {
		if(neighbourChunks[i] == nullptr) {
			neighbourChunks[i] = chunkManager->FindChunk(glm::vec2(localCoord.x + neighbourOffsets[i].x, localCoord.y + neighbourOffsets[i].y));
		}
	}
}

void Chunk::RemoveNeighbour(Chunk* neighbour) {
	for(int i = 0; i < (int)NEIGHBOUR::_TOTAL; i++) {
		if(neighbourChunks[i] == neighbour) {
			neighbourChunks[i] = nullptr;
			return;
		}
	}
}

void Chunk::GetFaceAmbient(const glm::ivec3& faceDirection, const glm::vec3& origin_local, std::array<int, 4>& blocks) {	
	glm::vec3 offsets[9];

	if(BlockManager::GetBlockData(GetBlockLocal(origin_local.x, origin_local.y, origin_local.z))->useAmbient == false) {
		for(int i = 0; i < 4; i++)
			blocks[i] = 3;

		return;
	}

	if(faceDirection.y != 0) {
		offsets[0] = glm::vec3( 0 * faceDirection.y, faceDirection.y, 1);
		offsets[1] = glm::vec3( 1 * faceDirection.y, faceDirection.y, 1);
		offsets[2] = glm::vec3( 1 * faceDirection.y, faceDirection.y, 0);
		offsets[3] = glm::vec3( 1 * faceDirection.y, faceDirection.y,-1);
		offsets[4] = glm::vec3( 0 * faceDirection.y, faceDirection.y,-1);
		offsets[5] = glm::vec3(-1 * faceDirection.y, faceDirection.y,-1);
		offsets[6] = glm::vec3(-1 * faceDirection.y, faceDirection.y, 0);
		offsets[7] = glm::vec3(-1 * faceDirection.y, faceDirection.y, 1);
		offsets[8] = glm::vec3( 0 * faceDirection.y, faceDirection.y, 1);
	} else if(faceDirection.x != 0) {
		offsets[0] = glm::vec3(faceDirection.x, 0, 1 * faceDirection.x);
		offsets[1] = glm::vec3(faceDirection.x,-1, 1 * faceDirection.x);
		offsets[2] = glm::vec3(faceDirection.x,-1, 0 * faceDirection.x);
		offsets[3] = glm::vec3(faceDirection.x,-1,-1 * faceDirection.x);
		offsets[4] = glm::vec3(faceDirection.x, 0,-1 * faceDirection.x);
		offsets[5] = glm::vec3(faceDirection.x, 1,-1 * faceDirection.x);
		offsets[6] = glm::vec3(faceDirection.x, 1, 0 * faceDirection.x);
		offsets[7] = glm::vec3(faceDirection.x, 1, 1 * faceDirection.x);
		offsets[8] = glm::vec3(faceDirection.x, 0, 1 * faceDirection.x);
	} else if(faceDirection.z != 0) {
		offsets[0] = glm::vec3(-1 * faceDirection.z, 0 , faceDirection.z);
		offsets[1] = glm::vec3(-1 * faceDirection.z,-1,  faceDirection.z);
		offsets[2] = glm::vec3( 0 * faceDirection.z,-1,  faceDirection.z);
		offsets[3] = glm::vec3( 1 * faceDirection.z,-1,  faceDirection.z);
		offsets[4] = glm::vec3( 1 * faceDirection.z, 0 , faceDirection.z);
		offsets[5] = glm::vec3( 1 * faceDirection.z, 1 , faceDirection.z);
		offsets[6] = glm::vec3( 0 * faceDirection.z, 1 , faceDirection.z);
		offsets[7] = glm::vec3(-1 * faceDirection.z, 1 , faceDirection.z);
		offsets[8] = glm::vec3(-1 * faceDirection.z, 0 , faceDirection.z);
	}

	BlockData const* block;
	glm::vec3 pos;
	for(int i = 0; i < 4; i++) {
		int index = i * 2;
		pos = origin_local + offsets[i];

		blocks[i] = 0; 
		block = BlockManager::GetBlockData(GetChunkOrNeighbourBlock(origin_local.x + offsets[index + 0].x, origin_local.y + offsets[index + 0].y, origin_local.z + offsets[index + 0].z));
		blocks[i] += (!block->useAmbient) ? 1.0f : 0.0f;

		block = BlockManager::GetBlockData(GetChunkOrNeighbourBlock(origin_local.x + offsets[index + 1].x, origin_local.y + offsets[index + 1].y, origin_local.z + offsets[index + 1].z));
		blocks[i] += (!block->useAmbient) ? 1.0f : 0.0f;
		
		block = BlockManager::GetBlockData(GetChunkOrNeighbourBlock(origin_local.x + offsets[index + 2].x, origin_local.y + offsets[index + 2].y, origin_local.z + offsets[index + 2].z));
		blocks[i] += (!block->useAmbient) ? 1.0f : 0.0f;
	}
}

void Chunk::SetNeighbourDirty(NEIGHBOUR neighbour) {
	int index = (int)neighbour;

	if(neighbourChunks[index] == nullptr) { 
		neighbourChunks[index] = chunkManager->FindChunk(localCoord + neighbourOffsets[index]);
	}

	if(neighbourChunks[index] != nullptr)
		neighbourChunks[index]->SetDirty();
}
					
#pragma endregion

//							[ END SET & GET BLOCKS ]

bool Chunk::IsPositionInChunk(const float& x, const float& y, const float& z) const {
	if(x < 0.0f || x >= CHUNK_SIZE)
		return false;
	if(y < 0.0f || y >= CHUNK_HEIGHT)
		return false;
	if(z < 0.0f || z >= CHUNK_SIZE)
		return false;

	return true;
}

bool Chunk::IsPositionInChunk(const glm::vec3& localPosition) const {
	return IsPositionInChunk(localPosition.x, localPosition.y, localPosition.z);
}

unsigned int Chunk::ToBlockIndex(const float& x, const float& y, const float& z) const {
	return (y * (CHUNK_SIZE * CHUNK_SIZE)) + (z * CHUNK_SIZE) + x;
}

unsigned int Chunk::ToBlockIndex(const glm::vec3& localPosition) const {
	return ToBlockIndex(localPosition.x, localPosition.y, localPosition.z);
}

glm::vec3 Chunk::IndexToLocalPos(const unsigned int& index) const {
	float x = index % CHUNK_SIZE;
	float y = glm::floor(index / CHUNK_SLICE);
	float z = (int)glm::floor(index / CHUNK_SIZE) % CHUNK_SIZE;
	return glm::vec3(x, y, z);
}

bool Chunk::NeighbourSlices(const unsigned int& y) {
	/*
		Solid blocks only need to check if there is solid slices around their slice, and render regardless if there is transparency slices

		Transpanrency slices only render if there is less than 256 transparent blocks around them
	*/

	// Check Above
	if(y < CHUNK_HEIGHT)
		if(chunkSlice.solidBlocks[y + 1] < CHUNK_SLICE || chunkSlice.transparentBlocks[y] > 0 && chunkSlice.transparentBlocks[y + 1] < 256)
			return false;

	// Check Below
	if(y > 0)
		if(chunkSlice.solidBlocks[y - 1] < CHUNK_SLICE || chunkSlice.transparentBlocks[y] > 0 && chunkSlice.transparentBlocks[y + 1] < 256)
			return false;

	// Check Neighbours
	for(int i = 0; i < 4; i++) {
		

		// CHANGE THIS SHIT
		//if(neighbourChunks[i] == nullptr) {
		//	neighbourChunks[i] = chunkManager->FindChunk(localCoord + neighbourOffsets[i]);
		//}
	
		Chunk* chunk = neighbourChunks[i];
		if(chunk != nullptr) {
			if(!chunk->hasWorldData)
				continue;
			//if(chunkSlice.transparentBlocks[y] > 0 && chunkSlice.transparentBlocks[y + 1] < 256)
			//	return false;
			if(chunk->chunkSlice.solidBlocks[y] < CHUNK_SLICE) //chunk->chunkSlice.solidBlocks[y] < CHUNK_SLICE)
				return false;

		}
	}

	return true;
}

void Chunk::CalculateLight() {
	while(lightQueue.empty() == false) {
		LightNode& node = lightQueue.front();
		Chunk* chunk = node.chunk;
		
		lightQueue.pop();

		int x = node.index % CHUNK_SIZE;
		int y = node.index / CHUNK_SLICE;
		int z = (node.index % (CHUNK_SIZE * CHUNK_SIZE)) / CHUNK_SIZE;

		int light = chunk->GetLight(x, y, z);

		// Left
		Chunk* neighbour = (x - 1 < 0) ? chunk->neighbourChunks[(int)NEIGHBOUR::LEFT] : chunk;
		if(neighbour == nullptr)
			continue;
		
		glm::vec3 offset = glm::vec3(Math::Modulo(x - 1, CHUNK_SIZE), y, z);
		if(BlockManager::GetBlockData(neighbour->GetBlockLocal(offset.x, offset.y, offset.z))->isTransparent && neighbour->GetLight(offset.x, offset.y, offset.z) + 2 <= light) {
			short index = (y * CHUNK_SLICE) + (z * CHUNK_SIZE) + (Math::Modulo(x - 1, CHUNK_SIZE));
		
			neighbour->SetLight(offset.x, offset.y, offset.z, light - 1);
			neighbour->SetDirty();
			lightQueue.emplace(index, neighbour);
		}
		
		// Right
		neighbour = (x + 1 > CHUNK_SIZE - 1) ? chunk->neighbourChunks[(int)NEIGHBOUR::RIGHT] : chunk;
		if(neighbour == nullptr)
			continue;
		
		offset = glm::vec3(Math::Modulo(x + 1, CHUNK_SIZE), y, z);
		if(BlockManager::GetBlockData(neighbour->GetBlockLocal(offset.x, offset.y, offset.z))->isTransparent && neighbour->GetLight(offset.x, offset.y, offset.z) + 2 <= light) {
			short index = (y * CHUNK_SLICE) + (z * CHUNK_SIZE) + Math::Modulo(x + 1, CHUNK_SIZE);
		
			neighbour->SetLight(offset.x, offset.y, offset.z, light - 1);
			neighbour->SetDirty();
			lightQueue.emplace(index, neighbour);
		}
		
		// Back
		neighbour = (z - 1 < 0) ? chunk->neighbourChunks[(int)NEIGHBOUR::BACK] : chunk;
		if(neighbour == nullptr)
			continue;
		
		offset = glm::vec3(x, y, Math::Modulo(z - 1, CHUNK_SIZE));
		
		if(BlockManager::GetBlockData(neighbour->GetBlockLocal(offset.x, offset.y, offset.z))->isTransparent && neighbour->GetLight(offset.x, offset.y, offset.z) + 2 <= light) {
			short index = (y * CHUNK_SLICE) + (offset.z * CHUNK_SIZE) + x;
		
			neighbour->SetLight(offset.x, offset.y, offset.z, light - 1);
			neighbour->SetDirty();
			lightQueue.emplace(index, neighbour);
		}
		
		// Forward
		neighbour = (z + 1 > CHUNK_SIZE - 1) ? chunk->neighbourChunks[(int)NEIGHBOUR::FRONT] : chunk;
		if(neighbour == nullptr)
			continue;
		
		offset = glm::vec3(x, y, Math::Modulo(z + 1, CHUNK_SIZE));
		
		if(BlockManager::GetBlockData(neighbour->GetBlockLocal(offset.x, offset.y, offset.z))->isTransparent && neighbour->GetLight(offset.x, offset.y, offset.z) + 2 <= light) {
			short index = (y * CHUNK_SLICE) + (offset.z * CHUNK_SIZE) + x;
		
			neighbour->SetLight(offset.x, offset.y, offset.z, light - 1);
			neighbour->SetDirty();
			lightQueue.emplace(index, neighbour);
		}
		
		
		// UP
		offset = glm::vec3(x, y + 1, z);
		if(offset.y >= 0 && offset.y < CHUNK_HEIGHT - 1) {
			if(BlockManager::GetBlockData(neighbour->GetBlockLocal(offset.x, offset.y, offset.z))->isTransparent && chunk->GetLight(offset.x, offset.y, offset.z) + 2 <= light) {
				short index = (offset.y * CHUNK_SLICE) + (z * CHUNK_SIZE) + x;
		
				chunk->SetLight(offset.x, offset.y, offset.z, light - 1);
				chunk->SetDirty();
				lightQueue.emplace(index, chunk);
			}
		}
		//
		//// DOWN
		offset = glm::vec3(x, y - 1, z);
		if(offset.y >= 0 && offset.y < CHUNK_HEIGHT - 1) {
			if(BlockManager::GetBlockData(neighbour->GetBlockLocal(offset.x, offset.y, offset.z))->isTransparent && chunk->GetLight(offset.x, offset.y, offset.z) + 2 <= light) {
				short index = (offset.y * CHUNK_SLICE) + (z * CHUNK_SIZE) + x;
		
				chunk->SetLight(offset.x, offset.y, offset.z, light - 1);
				chunk->SetDirty();
				lightQueue.emplace(index, chunk);
			}
		}

	}
}

#pragma region Generation

void Chunk::SetWorldData(const std::array<BlockIDs, CHUNK_MASS>& data, int height) {
	GetNeighbours();

	// Transfer all data to blocks inside chunk
	int max = height * CHUNK_SLICE;
	for(int i = 0; i < max; i++) {
		if(data[i] == BlockIDs::AIR)
			continue;

		BlockData const* newblock = BlockManager::GetBlockData(data[i]);
		BlockData const* oldBlock = BlockManager::GetBlockData(blocks[i]);

		int y = i / (CHUNK_SIZE * CHUNK_SIZE);

		if(!newblock->isTransparent && oldBlock->isTransparent) {
			chunkSlice.solidBlocks[y] += 1;
			if(oldBlock->id != BlockIDs::AIR)
				chunkSlice.transparentBlocks[y] -= 1;
		} else if(newblock->isTransparent && !oldBlock->isTransparent) {
			chunkSlice.solidBlocks[y] -= 1;
			if(newblock->id != BlockIDs::AIR)
				chunkSlice.transparentBlocks[y] += 1;
		} else if(newblock->isTransparent && oldBlock->isTransparent) {
			if(newblock->id != BlockIDs::AIR && oldBlock->id != BlockIDs::AIR)
				chunkSlice.transparentBlocks[y] += 1;
		}

		//SetBlock(i, data[i]);
		blocks[i] = data[i];
	}

	for(int i = 0; i < 4; i++)
		SetNeighbourDirty((NEIGHBOUR)i);

	highestBlock = height;

	hasWorldData = true;
	isDirty = true;
}

void Chunk::GenerateMeshData() {
	// Update Neighbour Pointers
	GetNeighbours();

	for(int i = 0; i < (int)NEIGHBOUR::_TOTAL; i++) {
		if(neighbourChunks[i] == nullptr)
			return;
	}

	if(!lightQueue.empty())
		CalculateLight();

	// Vertex Offset
	const glm::vec3 offsets[] = {
		glm::vec3(0, 0, -1), glm::vec3(1, 0, 0), glm::vec3(0, 0, 1), glm::vec3(-1, 0, 0), glm::vec3(0, 1, 0), glm::vec3(0, -1, 0)
	};

	// Current indices Index
	unsigned int indicesSolid = 0;
	unsigned int indicesOpaque = 0;
	unsigned int* indicesIndex = nullptr;

	// Current Block
	BlockData const * block = nullptr;
	MeshData* mesh = &solidMesh;

	// Texture Coordinates of a block face
	std::array<glm::vec2, 4> texCoords = std::array<glm::vec2, 4>();
	std::array<int, 4> ambient = std::array<int, 4>();

	auto BuildFace = [](const BlockData const * block, const BlockData const * neighbour) {
		if(!block->isTransparent && neighbour->isTransparent)
			return true;
		else if (block->isTransparent && neighbour->id == BlockIDs::AIR || (block->isTransparent && block->id != neighbour->id && neighbour->isTransparent))
			return true;
		return false;
	};

	for(int y = 0; y < highestBlock + 1; y++) {
		if(NeighbourSlices(y))
			continue;

		for(int z = 0; z < CHUNK_SIZE; z++) {
			for(int x = 0; x < CHUNK_SIZE; x++) {
				block = BlockManager::GetBlockData(GetChunkOrNeighbourBlock(x, y, z));

				if(block->id == BlockIDs::AIR)
					continue;

				if(block->id == BlockIDs::WATER) {
					mesh = &waterMesh;
					indicesIndex = &indicesOpaque;
				} else {
					mesh = &solidMesh;
					indicesIndex = &indicesSolid;
				}

				int faceCount = BlockManager::GetMeshFaceCount(block->meshType);
				glm::vec3 pos = glm::vec3(x, y, z);

				for(int i = 0; i < faceCount; i++) {

					if(block->meshType == MeshType::BLOCK || block->meshType == MeshType::LIQUID)
						if(!BuildFace(block, BlockManager::GetBlockData(GetChunkOrNeighbourBlock(x + offsets[i].x, y + offsets[i].y, z + offsets[i].z))))
							continue;

					// Get Texture Coords
					BlockManager::GetTextureCoords(block->id, i, texCoords);

					// Get Ambient
					GetFaceAmbient(offsets[i], pos, ambient);

					for(int verts = 0; verts < 4; verts++) {
						BlockManager::GetMeshFace(block->meshType, (TextureIndex::Face)i, verts, pos, mesh->meshData);

						mesh->meshData.emplace_back(texCoords[verts].x);
						mesh->meshData.emplace_back(texCoords[verts].y);

						mesh->meshData.emplace_back(ambient[verts]);

						if(z + offsets[i].z < 0 && this->localCoord.x == 0 && this->localCoord.y == -1) {
							int asd = 0;
						}

						mesh->meshData.emplace_back(GetLightNeighbourHood(x + offsets[i].x, y + offsets[i].y, z + offsets[i].z));
					}

					mesh->indices.emplace_back(*indicesIndex + 0);
					mesh->indices.emplace_back(*indicesIndex + 1);
					mesh->indices.emplace_back(*indicesIndex + 2);

					mesh->indices.emplace_back(*indicesIndex + 0);
					mesh->indices.emplace_back(*indicesIndex + 2);
					mesh->indices.emplace_back(*indicesIndex + 3);

					*indicesIndex += 4;
				}
			}
		}
	}

	solidMesh.indicesCount = solidMesh.indices.size();
	waterMesh.indicesCount = waterMesh.indices.size();

	uploadMeshToGPU = true;
	isDirty = false;
}

void Chunk::CreateMesh() {

	std::chrono::high_resolution_clock::time_point t1 = std::chrono::high_resolution_clock::now();

	// Generate Solid Buffers
	if(solidMesh.meshData.size() > 0) {
		if(solidMesh.vao == 0) {
			glGenVertexArrays(1, &solidMesh.vao);
			glGenBuffers(1, &solidMesh.vbo);
			glGenBuffers(1, &solidMesh.ebo);
		}

		glBindVertexArray(solidMesh.vao);

		glBindBuffer(GL_ARRAY_BUFFER, solidMesh.vbo);
		glBufferData(GL_ARRAY_BUFFER, sizeof(float) * solidMesh.meshData.size(), solidMesh.meshData.data(), GL_DYNAMIC_DRAW);

		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, solidMesh.ebo);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(unsigned int) * solidMesh.indices.size(), solidMesh.indices.data(), GL_DYNAMIC_DRAW);


		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 10 * sizeof(float), (void*)(0 * sizeof(float))); // Vertices
		glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 10 * sizeof(float), (void*)(3 * sizeof(float))); // Normals
		glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 10 * sizeof(float), (void*)(6 * sizeof(float))); // Tex Coords
		glVertexAttribPointer(3, 1, GL_FLOAT, GL_FALSE, 10 * sizeof(float), (void*)(8 * sizeof(float))); // PVAO
		glVertexAttribPointer(4, 1, GL_FLOAT, GL_FALSE, 10 * sizeof(float), (void*)(9 * sizeof(float)));

		glEnableVertexAttribArray(0);
		glEnableVertexAttribArray(1);
		glEnableVertexAttribArray(2);
		glEnableVertexAttribArray(3);
		glEnableVertexAttribArray(4);
	}

	// Generate Water Buffer
	if(waterMesh.meshData.size() > 0) {
		if(waterMesh.vao == 0) {
			glGenVertexArrays(1, &waterMesh.vao);
			glGenBuffers(1, &waterMesh.vbo);
			glGenBuffers(1, &waterMesh.ebo);
		}

		glBindVertexArray(waterMesh.vao);

		glBindBuffer(GL_ARRAY_BUFFER, waterMesh.vbo);
		glBufferData(GL_ARRAY_BUFFER, sizeof(float) * waterMesh.meshData.size(), waterMesh.meshData.data(), GL_DYNAMIC_DRAW);

		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, waterMesh.ebo);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(unsigned int) * waterMesh.indices.size(), waterMesh.indices.data(), GL_DYNAMIC_DRAW);

		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 10 * sizeof(float), (void*)(0 * sizeof(float))); // Vertices
		glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 10 * sizeof(float), (void*)(3 * sizeof(float))); // Normals
		glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 10 * sizeof(float), (void*)(6 * sizeof(float))); // Tex Coords
		glVertexAttribPointer(3, 1, GL_FLOAT, GL_FALSE, 10 * sizeof(float), (void*)(8 * sizeof(float))); // PVAO
		glVertexAttribPointer(4, 1, GL_FLOAT, GL_FALSE, 10 * sizeof(float), (void*)(9 * sizeof(float)));

		glEnableVertexAttribArray(0);
		glEnableVertexAttribArray(1);
		glEnableVertexAttribArray(2);
		glEnableVertexAttribArray(3);
		glEnableVertexAttribArray(4);
	}

	glBindVertexArray(0);

	solidMesh.Clear();
	waterMesh.Clear();
	solidMesh.meshData.shrink_to_fit();
	waterMesh.meshData.shrink_to_fit();
	uploadMeshToGPU = false;

	std::chrono::high_resolution_clock::time_point t2 = std::chrono::high_resolution_clock::now();
	std::chrono::duration<double> time_span = std::chrono::duration_cast<std::chrono::duration<double>>(t2 - t1);
	//printf("Generation Duration: %f seconds\n", time_span.count());

	DELETTHIS::AddTime(time_span.count());
}

#pragma endregion
