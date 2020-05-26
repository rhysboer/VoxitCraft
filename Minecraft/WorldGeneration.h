#pragma once
#include <cmath>
#include <array>
#include <vector>
#include "glm/gtc/noise.hpp"
#include "Chunk.h"


#include "BiomeField.h"
#include "BiomeDesert.h"
#include "BiomeSnow.h"

class ChunkManager;

enum class BiomeIndex {
	FIELD,
	DESERT,
	SEA
};

class WorldGeneration {
public:
	WorldGeneration(unsigned int heightMapSeed = 0, unsigned int moistureSeed = 0);
	~WorldGeneration();

	unsigned int CreateChunkWorldData(const Chunk& chunk, std::array<BlockIDs, Chunk::CHUNK_MASS>& data, std::vector<glm::vec3>& structurePos, std::vector<BlockIDs>& structureBlocks);

private:
	const float waterLevel = 16.0f;

	const float biomeScale = 512.0f;

	Biome* field = new BiomeField();
	Biome* desert = new BiomeDesert();
	Biome* snow = new BiomeSnow();

	std::array<float, Chunk::CHUNK_SLICE> heightMap = std::array<float, Chunk::CHUNK_SLICE>();
	std::array<float, Chunk::CHUNK_SLICE> biomeMap = std::array<float, Chunk::CHUNK_SLICE>();

	// Creates the biome map
	void CreateBiomeMap(const Chunk& chunk);

	// Gets the height map of a chunk, blending the biomes if merging
	void GetHeightMap(const Chunk& chunk, std::array<float, Chunk::CHUNK_SLICE>& heightMap) const;

	// Turns a biome index into a biome pointer
	BiomeIndex GetBiomeIndexAtPos(const int& local_x, const int& local_z) const;


	Biome* GetBiome(const int& local_x, const int& local_z) const;
	Biome* GetBiome(BiomeIndex index) const;
	Biome* GetBiomeAt(const int& world_x, const int& world_z) const;
};


// OPTIMISATIONS
// Get max height from heightmap and use that for the Y value