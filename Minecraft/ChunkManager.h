#pragma once
#include <unordered_map>
#include <thread>
#include <array>
#include <mutex>
#include "glm/glm.hpp"
#include "glm/gtx/hash.hpp"
#include "ShaderManager.h"
#include "WorldGeneration.h"
#include "BaseCamera.h"
#include "Chunk.h"

#include "TileMap.h"

#include "Engine.h"

#include "DELETTHIS.h"

class ChunkManager {
public:
	
	ChunkManager();
	~ChunkManager();

	void Update();
	void Render(BaseCamera& camera);

	// Returns a block in the world, will return AIR if bad position. X, Y & Z are in World Position.
	BlockIDs GetBlock(const float& x, const float& y, const float& z) const;
	BlockIDs GetBlock(const glm::vec3& worldPosition) const;

	// Returns blocks inside area that are solid
	void GetSolidBlocksInArea(const glm::vec3& worldPosition, const glm::vec3& size, std::vector<glm::vec3>& output);

	// World Position
	void SetBlock(const float& x, const float& y, const float& z, const BlockIDs& block);
	void SetBlock(const glm::vec3& worldPosition, const BlockIDs& block);
	void SetBlocks(const std::vector<glm::vec3>& positions, std::vector<BlockIDs>& blocks);

	TileMap& GetTileMapTerrain() const;
	TileMap& GetTileMapWater() const;

	friend Chunk;

public: // Private

	WorldGeneration worldGen = WorldGeneration();

	// Multithreaded Funtions
	void ChunkLoader();

	Chunk* CreateChunk(const glm::ivec2& index);
	
	// Returns the chunk, otherwise returns nullptr;
	Chunk* FindChunk(const glm::ivec2& index) const;
	Chunk* FindChunk(const glm::vec3& worldPosition) const;
	Chunk* FindChunk(const float& x, const float& z) const;

	glm::ivec2 PositionToChunk(const glm::vec3& worldPosition) const;

private:
	const unsigned int RENDERING_DISTANCE = 12;
	const unsigned int DESTROY_DISTANCE = RENDERING_DISTANCE + 5;
	
	std::unordered_map<glm::ivec2, Chunk*> chunks = std::unordered_map <glm::ivec2, Chunk*>();
	Chunk* cacheChunk = nullptr;

	TileMap* terrainTexture;
	TileMap* waterTexture;

	Shader* solidShader;
	Shader* waterShader;

	glm::vec3 position = glm::vec3(0);

	// TESTING
	std::mutex mutex;
	std::thread thread;
	bool ending = false;

	bool START = false;
};



