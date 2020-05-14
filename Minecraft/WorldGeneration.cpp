#include "WorldGeneration.h"
#include "ChunkManager.h"

WorldGeneration::WorldGeneration(unsigned int heightMapSeed, unsigned int moistureSeed) {
	// TODO: SEED RAND

	//srand((chunk.worldCoord.x * chunk.worldCoord.z));	
}

WorldGeneration::~WorldGeneration() {
}

void WorldGeneration::CreateChunkWorldData(Chunk& chunk) {
	if(chunk.hasWorldData)
		return;
	
	std::vector<glm::vec3> treePositions = std::vector<glm::vec3>();
	treePositions.reserve(10);

	CreateBiomeMap(chunk);
	GetHeightMap(chunk, heightMap);

	// OPTIMIZE THIS
	glm::vec2 coords;
	for(int z = 0; z < 16; z++) {
		for(int x = 0; x < 16; x++) {
			coords = glm::vec2((x + chunk.worldCoord.x) / 512.0f, (z + chunk.worldCoord.z) / 512.0f );
			heightMap[(z * 16) + x] += (glm::simplex(coords)) * 20;
		}
	}


	int maxHeight = glm::max((*std::max_element(heightMap.begin(), heightMap.end())) + 1, waterLevel);
	for(int y = 0; y < maxHeight; y++) {
		for(int z = 0; z < Chunk::CHUNK_SIZE; z++) {
			for(int x = 0; x < Chunk::CHUNK_SIZE; x++) {
				int blockIndex = (y * Chunk::CHUNK_SIZE * Chunk::CHUNK_SIZE) + (z * Chunk::CHUNK_SIZE) + x;
				
				int mapHeight = (int)heightMap[(z * Chunk::CHUNK_SIZE) + x];
				float height = y - mapHeight;

				Biome* biome = GetBiome(x, z);
				BlockIDs block = biome->GetBlock(height);

				if(y == 0)
					chunk.SetBlock(blockIndex, BlockIDs::GRASS);

				//if(x == 0 || x == Chunk::CHUNK_SIZE || z == 0 || z == Chunk::CHUNK_SIZE)
				//	chunk.SetBlock(blockIndex, BlockIDs::DIRT);
				//
				//if(x == 0 || x == Chunk::CHUNK_SIZE || z == 0 || z == Chunk::CHUNK_SIZE)
				//	continue;
				
				// Water
				//if(y < 16 && height == 0) {
				//	chunk.SetBlock(blockIndex, BlockIDs::SAND);
				//} else if(y > mapHeight && y < 15) {
				//	chunk.SetBlock(blockIndex, BlockIDs::WATER);
				//} else if(chunk.blocks[blockIndex] == BlockIDs::AIR) {
				//	chunk.SetBlock(blockIndex, block);
				//}
				//
				//if(height == 1 && y > waterLevel) {
				//	if(biome->GetGenerationParameters().structureDensity <= 0) 
				//		continue;
				//	
				//	if(rand() % biome->GetGenerationParameters().structureDensity == 1) {
				//		treePositions.emplace_back(x, y, z);
				//	}
				//}
			}
		}
	}

	for(int i = 0; i < treePositions.size(); i++) {
		Biome* biome = GetBiome(treePositions[i].x, treePositions[i].z);
	
		if(biome != nullptr) {
			biome->AddStructure(chunk, treePositions[i] + chunk.worldCoord);
		}
	}

	chunk.hasWorldData = true;
	chunk.isDirty = true;
}

unsigned int WorldGeneration::CreateChunkWorldData(const Chunk& chunk, std::array<BlockIDs, Chunk::CHUNK_MASS>& data, std::vector<glm::vec3>& structurePos, std::vector<BlockIDs>& structureBlocks) {
	if(chunk.hasWorldData)
		return 0;

	std::vector<glm::vec3> treePositions = std::vector<glm::vec3>();
	treePositions.reserve(10);

	CreateBiomeMap(chunk);
	GetHeightMap(chunk, heightMap);

	// OPTIMIZE THIS
	glm::vec2 coords;
	for(int z = 0; z < 16; z++) {
		for(int x = 0; x < 16; x++) {
			coords = glm::vec2((x + chunk.worldCoord.x) / 512.0f, (z + chunk.worldCoord.z) / 512.0f);
			heightMap[(z * 16) + x] += (glm::simplex(coords)) * 20;
		}
	}


	int maxHeight = glm::max((*std::max_element(heightMap.begin(), heightMap.end())) + 1, waterLevel);
	for(int y = 0; y < maxHeight; y++) {
		for(int z = 0; z < Chunk::CHUNK_SIZE; z++) {
			for(int x = 0; x < Chunk::CHUNK_SIZE; x++) {
				int blockIndex = (y * Chunk::CHUNK_SIZE * Chunk::CHUNK_SIZE) + (z * Chunk::CHUNK_SIZE) + x;

				int mapHeight = (int)heightMap[(z * Chunk::CHUNK_SIZE) + x];
				float height = y - mapHeight;

				Biome* biome = GetBiome(x, z);
				BlockIDs block = biome->GetBlock(height);

				if(y == 0)
					data[blockIndex] = BlockIDs::GRASS;	//chunk.SetBlock(blockIndex, BlockIDs::GRASS);

				data[blockIndex] = block;

				// Water
				if(y < 16 && height == 0) {
					data[blockIndex] = BlockIDs::SAND; //chunk.SetBlock(blockIndex, BlockIDs::SAND);
				} else if(y > mapHeight&& y < 15) {
					data[blockIndex] = BlockIDs::WATER; //chunk.SetBlock(blockIndex, BlockIDs::WATER);
				} else if(data[blockIndex] == BlockIDs::AIR) {
					data[blockIndex] = block; //chunk.SetBlock(blockIndex, block);
				}
				
				if(height == 1 && y > waterLevel) {
					if(biome->GetGenerationParameters().structureDensity <= 0)
						continue;
				
					if(rand() % biome->GetGenerationParameters().structureDensity == 1) {
						treePositions.emplace_back(x, y, z);
					}
				}
			}
		}
	}

	for(int i = 0; i < treePositions.size(); i++) {
		Biome* biome = GetBiome(treePositions[i].x, treePositions[i].z);
	
		if(biome != nullptr) {
			//biome->AddStructure(chunk, treePositions[i] + chunk.worldCoord);
			biome->AddStructure(treePositions[i] + chunk.worldCoord, data, structurePos, structureBlocks);
			//biome->AddStructure()
		}
	}

	//chunk.hasWorldData = true;
	//chunk.isDirty = true;
	
	return maxHeight + 10;
}

// TEST
int WorldGeneration::CreateChunkWorldData(const Chunk& chunk, std::array<BlockIDs, Chunk::CHUNK_MASS>& data) {
	if(chunk.hasWorldData)
		return 0.0f;

	std::vector<glm::vec3> treePositions = std::vector<glm::vec3>();
	treePositions.reserve(10);

	CreateBiomeMap(chunk);
	GetHeightMap(chunk, heightMap);

	int maxHeight = (*std::max_element(heightMap.begin(), heightMap.end())) + 1;

	for(int y = 0; y < maxHeight; y++) {
		for(int z = 0; z < Chunk::CHUNK_SIZE; z++) {
			for(int x = 0; x < Chunk::CHUNK_SIZE; x++) {
				int blockIndex = (y * Chunk::CHUNK_SIZE * Chunk::CHUNK_SIZE) + (z * Chunk::CHUNK_SIZE) + x;

				float height = y - (int)heightMap[(z * Chunk::CHUNK_SIZE) + x];


				Biome* biome = GetBiome(x, z);
				BlockIDs block = biome->GetBlock(height);


				if(chunk.blocks[blockIndex] == BlockIDs::AIR)
					data[blockIndex] = block; //chunk.SetBlock(blockIndex, block);

				if(height == 1) {
					if(biome->GetGenerationParameters().structureDensity <= 0)
						continue;

					if(rand() % biome->GetGenerationParameters().structureDensity == 1) {
						treePositions.emplace_back(x, y, z);
					}
				}
			}
		}
	}


	//for(int i = 0; i < treePositions.size(); i++) {
	//	Biome* biome = GetBiome(treePositions[i].x, treePositions[i].z);
	//
	//	if(biome != nullptr) {
	//		biome->AddStructure(chunk, treePositions[i] + chunk.worldCoord);
	//	}
	//}

	return maxHeight;
}


void WorldGeneration::CreateBiomeMap(const Chunk& chunk) {
	for(int z = 0; z < Chunk::CHUNK_SIZE; z++) {
		for(int x = 0; x < Chunk::CHUNK_SIZE; x++) {
			biomeMap[(z * Chunk::CHUNK_SIZE) + x] = (glm::simplex(glm::vec2((x + chunk.worldCoord.x) / biomeScale, (z + chunk.worldCoord.z) / biomeScale)) + 1.0f) / 2.0f;
		}
	}
}

void WorldGeneration::GetHeightMap(const Chunk& chunk, std::array<float, Chunk::CHUNK_SLICE>& heightMap) const {
	BiomeIndex topLeft = GetBiomeIndexAtPos(0, 0);
	BiomeIndex topRight = GetBiomeIndexAtPos(Chunk::CHUNK_SIZE - 1, 0);
	BiomeIndex botLeft = GetBiomeIndexAtPos(0, Chunk::CHUNK_SIZE - 1);
	BiomeIndex botRight = GetBiomeIndexAtPos(Chunk::CHUNK_SIZE - 1, Chunk::CHUNK_SIZE - 1);
	
	// Interperlate heightmap
	if(botLeft != botRight || botLeft != topLeft || botLeft != topRight) {
		float tLeft =		GetBiome(topLeft)->GetHeightAt(chunk.worldCoord.x, chunk.worldCoord.z);
		float tRight =		GetBiome(topRight)->GetHeightAt(chunk.worldCoord.x + Chunk::CHUNK_SIZE - 1, chunk.worldCoord.z);
		float bLeft =		GetBiome(botLeft)->GetHeightAt(chunk.worldCoord.x, chunk.worldCoord.z + Chunk::CHUNK_SIZE - 1);
		float bRight =		GetBiome(botRight)->GetHeightAt(chunk.worldCoord.x + Chunk::CHUNK_SIZE - 1, chunk.worldCoord.z + Chunk::CHUNK_SIZE - 1);
		
		for(int z = 0; z < Chunk::CHUNK_SIZE; z++) {
			for(int x = 0; x < Chunk::CHUNK_SIZE; x++) {
				heightMap[(z * Chunk::CHUNK_SIZE) + x] =
					glm::mix(
						glm::mix(tLeft, tRight, x / (Chunk::CHUNK_SIZE - 1.0f)),
						glm::mix(bLeft, bRight, x / (Chunk::CHUNK_SIZE - 1.0f)),
						z / (Chunk::CHUNK_SIZE - 1.0f)
					);
			}
		}
	
		return;
	}
	
	// Return heightmap in any corner since they're all the same biome
	GetBiome(botLeft)->GetHeightMap(heightMap, chunk);
}

BiomeIndex WorldGeneration::GetBiomeIndexAtPos(const int& local_x, const int& local_z) const {
	float sample = biomeMap[(local_z * Chunk::CHUNK_SIZE) + local_x];

	//if(sample > rate)
		return BiomeIndex::FIELD;
	//return BiomeIndex::DESERT;
}

Biome* WorldGeneration::GetBiome(const int& local_x, const int& local_z) const {
	float sample = biomeMap[(local_z * Chunk::CHUNK_SIZE) + local_x];

	//if(sample > rate)
		return field;
	//return desert;
}

Biome* WorldGeneration::GetBiome(BiomeIndex index) const {
	//switch(index) {
	//	case BiomeIndex::FIELD:
	//		return field;
	//	case BiomeIndex::DESERT:
	//		return desert;
	//}
	return field;
}

Biome* WorldGeneration::GetBiomeAt(const int& world_x, const int& world_z) const {
	float sample = glm::simplex(glm::vec2(world_x / biomeScale, world_z / biomeScale));

	//if(sample > rate)
	//	return field;
	return desert;
}
