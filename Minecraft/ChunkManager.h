#pragma once
#include <unordered_map>
#include <thread>
#include <array>
#include <mutex>
#include "glm/glm.hpp"
#include "glm/gtx/hash.hpp"
#include "ShaderManager.h"
#include "WorldGeneration.h"
#include "Camera3D.h"
#include "Chunk.h"
#include "Object3D.h"

#include "TileMap.h"

#include "Engine.h"

class ChunkManager {
public:
	
	ChunkManager();
	~ChunkManager();

	void Update();
	void Render(BaseCamera& camera);

	// Returns a block in the world, will return AIR if bad position. X, Y & Z are in World Position.
	BlockIDs GetBlock(const float& x, const float& y, const float& z);
	BlockIDs GetBlock(const glm::vec3& worldPosition);

	// Locks the chunk and returns block ID in world position
	BlockIDs GetBlockLock(const float& x, const float& y, const float& z);

	int GetLightLevelAtBlock(const float& x, const float& y, const float& z);

	// Returns blocks inside area that are solid
	void GetSolidBlocksInArea(const glm::vec3& worldPosition, const glm::vec3& size, std::vector<glm::vec3>& output);

	// World Position
	void SetBlock(const float& x, const float& y, const float& z, const BlockIDs& block);
	void SetBlock(const glm::vec3& worldPosition, const BlockIDs& block);
	void SetBlocks(const std::vector<glm::vec3>& positions, std::vector<BlockIDs>& blocks);

	friend Chunk;

public: // Private

	WorldGeneration worldGen = WorldGeneration();

	// Multithreaded Funtions
	void ChunkLoader();

	Chunk* CreateChunk(const glm::ivec2& index);
	
	// Returns the chunk, otherwise returns nullptr;
	Chunk* FindChunk(const glm::ivec2& index);
	Chunk* FindChunk(const glm::vec3& worldPosition);
	Chunk* FindChunk(const float& x, const float& z);

private:
	unsigned int RENDERING_DISTANCE = 15;
	unsigned int DESTROY_DISTANCE = RENDERING_DISTANCE + 5;
	
	// Find a chunk that locks the mutex
	Chunk* FindChunkLock(const glm::ivec2& index);

	std::unordered_map<glm::ivec2, Chunk*> chunks = std::unordered_map <glm::ivec2, Chunk*>();
	Chunk* cacheChunk = nullptr;

	Shader* solidShader;
	Shader* waterShader;

	// Multithreading
	std::mutex mutex;
	std::thread* chunkGenerationThread;
	bool appEnding = false;

	std::array<BlockIDs, Chunk::CHUNK_MASS> data = std::array<BlockIDs, Chunk::CHUNK_MASS>();
	std::vector<BlockIDs> structureBlocks = std::vector<BlockIDs>();
	std::vector<glm::vec3> structurePos = std::vector<glm::vec3>();
};



