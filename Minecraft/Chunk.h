#pragma once
#include <array>
#include "glad/glad.h"
#include "GLFW/glfw3.h"
#include "BlockManager.h"
#include "Camera.h"
#include "Shader.h"
#include "Util.h"

#include <chrono>


enum class STATE {
	DIRTY,
	UPDATE_MESH,
	IDLE
};

struct MeshData {
	std::vector<float> meshData = std::vector<float>();
	std::vector<unsigned int> indices = std::vector<unsigned int>();

	unsigned int vao;
	unsigned int vbo;
	unsigned int ebo;

	unsigned int indicesCount;
};

class ChunkManager;

class Chunk {
public:
	// Chunk statics 
	static const int CHUNK_SIZE = 16;
	static const int CHUNK_SLICE = CHUNK_SIZE * CHUNK_SIZE;
	static const int CHUNK_HEIGHT = 128;
	static const int CHUNK_MASS = CHUNK_SIZE * CHUNK_SIZE * CHUNK_HEIGHT;

	Chunk(glm::ivec2 index, ChunkManager& world);
	~Chunk();

	// Get a Block Inside the Chunk
	BlockIDs GetBlock(const float& x, const float& y, const float& z) const; // World Position
	BlockIDs GetBlock(const glm::vec3& worldPosition) const;
	// Returns air if the block is outside of the local position inside the chunk
	BlockIDs GetBlockLocal(const float& x, const float& y, const float& z) const;

	// Set Block Inside Chunk
	void SetBlock(const int& x, const int& y, const int& z, const BlockIDs& block); // World Position
	void SetBlock(const glm::vec3& worldPosition, const BlockIDs& block);

	friend class ChunkManager;
	friend class WorldGeneration;

private:

	// TEST
	void SetWorldData(const std::array<BlockIDs, CHUNK_MASS>& data, int height);

	void GenerateMeshData(); // Create mesh data for the chunk to make into a mesh
	void CreateMesh(); // Turn mesh data into a mesh
	
	void OnTick();
	void Render(Shader& solidShader, Shader& waterShader);
	void RenderOpaque(Shader& waterShader);

	// Set a block inside the chunk with the block index
	void SetBlock(const unsigned int& index, const BlockIDs& id);

	// Returns true if the world position is inside the chunks borders
	bool IsPositionInChunk(const glm::vec3& localPosition) const;
	bool IsPositionInChunk(const float& x, const float& y, const float& z) const; // Local Position

	// Returns the index of a block in the array from local position.
	unsigned int ToBlockIndex(const float& x, const float& y, const float& z) const;
	unsigned int ToBlockIndex(const glm::vec3& localPosition) const;

	// Returns the block at location inside the chunk or its neighbour, ONLY USED FOR CHUNK MESH GENERATION
	BlockIDs GetChunkOrNeighbourBlock(const float& x, const float& y, const float& z);

	// 
	bool NeighbourSlices(const unsigned int& y);

public: // Private

	// Chunk Data
	struct ChunkSlice {
		int solidBlocks[Chunk::CHUNK_SLICE];
		int transparentBlocks[Chunk::CHUNK_SLICE];
	} chunkSlice = ChunkSlice(); // TEST

	//int slices[CHUNK_HEIGHT]; // How many blocks are on each layer of the chunk
	BlockIDs blocks[CHUNK_MASS]; // All blocks inside of the chunk
	glm::ivec2 localCoord; // Local Coordinates of the chunk
	glm::vec3 worldCoord; // World Coordinates of the chunk
	ChunkManager* chunkManager; // Pointer to world
	int highestBlock; // Highest block inside the chunk
	bool isDirty; // Is chunk dirty 
	bool meshUpdate; // Does the chunk need to update its mesh
	bool hasWorldData; // Has generated world data

	enum class NEIGHBOUR {
		LEFT, RIGHT, FRONT, BACK
	};
	const glm::ivec2 neighbourOffsets[4] = { glm::ivec2(-1, 0), glm::ivec2(1, 0), glm::ivec2(0, -1), glm::ivec2(0, 1) };
	Chunk* neighbourChunks[4] = { nullptr, nullptr, nullptr, nullptr };

	// Mesh Data
	MeshData solidMesh = MeshData();
	MeshData waterMesh = MeshData();
};

