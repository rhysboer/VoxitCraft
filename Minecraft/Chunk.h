#pragma once
#include <array>
#include "glad/glad.h"
#include "GLFW/glfw3.h"
#include "BlockManager.h"
#include "Shader.h"
#include "Util.h"
#include "AABB.h"

#include <chrono>

class ChunkManager;

struct MeshData {
	std::vector<float> meshData = std::vector<float>();
	std::vector<unsigned int> indices = std::vector<unsigned int>();

	unsigned int vao;
	unsigned int vbo;
	unsigned int ebo;

	unsigned int indicesCount;

	void Clear() {
		meshData.clear();
		indices.clear();
	}
};

class Chunk {
private:
	enum class NEIGHBOUR {
		LEFT = 0, // X-
		RIGHT = 1, // X+
		FRONT = 2, // Z+
		BACK = 3, // Z-
		FRONT_LEFT = 4, // X-, Z+
		FRONT_RIGHT = 5, // X+, Z+
		BACK_LEFT = 6, // X-, Z-
		BACK_RIGHT = 7, // X+, Z-

		_TOTAL = 8
	};

public:
	// Chunk statics 
	static const int CHUNK_SIZE = 16;
	static const int CHUNK_SLICE = CHUNK_SIZE * CHUNK_SIZE;
	static const int CHUNK_HEIGHT = 128;
	static const int CHUNK_MASS = CHUNK_SIZE * CHUNK_SIZE * CHUNK_HEIGHT;

	Chunk(glm::ivec2 index, ChunkManager& world);
	~Chunk();

	// --------- GET CHUNKS BLOCK

	// Returns the block at position, X, Y & Z are in World Position.
	BlockIDs GetBlock(const float& x, const float& y, const float& z) const;
	BlockIDs GetBlock(const glm::vec3& worldPosition) const;

	// Returns air if the block is outside of the local position inside the chunk
	BlockIDs GetBlockLocal(const float& x, const float& y, const float& z) const;

	glm::vec2 GetLocalCoords() const;

	// Set Block Inside Chunk. X, Y & Z are in World Position.
	void SetBlock(const int& x, const int& y, const int& z, const BlockIDs& block);
	void SetBlock(const glm::vec3& worldPosition, const BlockIDs& block);

	AABB const* GetAABB() const;

	friend class ChunkManager;
	friend class WorldGeneration;

public: // private

	// TEST
	void SetWorldData(const std::array<BlockIDs, CHUNK_MASS>& data, int height);

	void GenerateMeshData(); // Create mesh data for the chunk to make into a mesh
	inline void CreateFacce(); 
	void CreateMesh(); // Turn mesh data into a mesh
	
	void Render(Shader& solidShader);
	void RenderOpaque(Shader& waterShader);

	// Set a block inside the chunk with the block index
	void SetBlock(const unsigned int& index, const BlockIDs& id);

	// Returns true if the world position is inside the chunks borders
	bool IsPositionInChunk(const glm::vec3& localPosition) const;
	bool IsPositionInChunk(const float& x, const float& y, const float& z) const; // Local Position

	// Returns the index of a block in the array from local position.
	unsigned int ToBlockIndex(const float& x, const float& y, const float& z) const;
	unsigned int ToBlockIndex(const glm::vec3& localPosition) const;
	glm::vec3 IndexToLocalPos(const unsigned int& index) const;

	// Returns the block at location inside the chunk or its neighbour, ONLY USED FOR CHUNK MESH GENERATION
	BlockIDs GetChunkOrNeighbourBlock(const float& x, const float& y, const float& z);
	void GetFaceAmbient(const glm::ivec3& faceDirection, const glm::vec3& origin_local, std::array<int, 4>& blocks);
	// Set neighbouring chunk dirty
	void SetNeighbourDirty(NEIGHBOUR neighbour);
	// Updates the neighbour pointers
	void GetNeighbours();
	void RemoveNeighbour(Chunk* neighbour);



	void SetDirty() { isDirty = true; }

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
	bool isDirty; // Is chunk dirty, needs to regenerate mesh data
	bool uploadMeshToGPU; // Does chunk need to upload its data to the gpu
	bool hasWorldData; // Has generated world data
	AABB* aabb; // Used for frustum culling chunk

	/*
		LEFT, // X-
		RIGHT, // X+
		FRONT, // Z+
		BACK, // Z-
		FRONT_LEFT, // X-, Z+
		FRONT_RIGHT, // X+, Z+
		BACK_LEFT, // X-, Z-
		BACK_RIGHT // X+, Z-
	*/
	const glm::ivec2 neighbourOffsets[8] = { 
		{-1, 0}, {1, 0}, {0, 1}, {0, -1},
		{-1, 1}, {1, 1}, {-1, -1}, {1, -1}
	};
	Chunk* neighbourChunks[8];

	// Mesh Data
	MeshData solidMesh = MeshData();
	MeshData waterMesh = MeshData();
};

