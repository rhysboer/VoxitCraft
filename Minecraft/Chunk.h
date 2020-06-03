#pragma once
#include <array>
#include <queue>
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
	inline unsigned int ToBlockIndex(const float& x, const float& y, const float& z) const {
		return (y * CHUNK_SLICE) + (z * CHUNK_SIZE) + x;
	}
	inline unsigned int ToBlockIndex(const glm::vec3& localPosition) const {
		return ToBlockIndex(localPosition.x, localPosition.y, localPosition.z);
	}
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

	bool NeighbourSlices(const unsigned int& y);


	// LIGHTING
	struct LightNode {
		LightNode(short _index, Chunk* _chunk) : index(_index), chunk(_chunk)  { }
		short index;
		Chunk* chunk;
	};
	struct LightRemoveNode {
		LightRemoveNode(short _index, short _value, Chunk* _chunk) : index(_index), value(_value), chunk(_chunk) {}
		short index;
		short value;
		Chunk* chunk;
	};


	void CalculateLight();
	void CalculateRemovalLight();
	

	void CreateSunlight();
	void CalculateSunlight();
	void CalculateSunlightRemoval();
	void RemoveSunColumn(const int& x, const int& y, const int& z);


	// Credit: https://www.seedofandromeda.com/blogs/29-fast-flood-fill-lighting-in-a-blocky-voxel-game-pt-1 & part 2
	inline int GetSunlight(const int& x, const int& y, const int& z) {
		return (lightMap[y][z][x] >> 4) & 0xF;
	}
	inline int GetSunlightNeighbourhood(const int& x, const int& y, const int& z) {
		Chunk* chunk = this;

		if(x >= CHUNK_SIZE)	chunk = neighbourChunks[(int)NEIGHBOUR::RIGHT];
		else if(x < 0)		chunk = neighbourChunks[(int)NEIGHBOUR::LEFT];
		else if(z >= CHUNK_SIZE)	chunk = neighbourChunks[(int)NEIGHBOUR::FRONT];
		else if(z < 0)				chunk = neighbourChunks[(int)NEIGHBOUR::BACK];
		else if(x < 0 && z < 0)				chunk = neighbourChunks[(int)NEIGHBOUR::BACK_LEFT];
		else if(x >= CHUNK_SIZE && z < 0)	chunk = neighbourChunks[(int)NEIGHBOUR::BACK_RIGHT];
		else if(x < 0 && z >= CHUNK_SIZE)			chunk = neighbourChunks[(int)NEIGHBOUR::FRONT_LEFT];
		else if(x >= CHUNK_SIZE && z >= CHUNK_SIZE)	chunk = neighbourChunks[(int)NEIGHBOUR::FRONT_RIGHT];

		if(chunk == nullptr)
			return 0;

		return chunk->GetSunlight(Math::Modulo(x, CHUNK_SIZE), glm::clamp(y, 0, CHUNK_HEIGHT - 1), Math::Modulo(z, CHUNK_SIZE));
	}
	inline void SetSunlight(const int& x, const int& y, const int& z, const int& level) {
		lightMap[y][z][x] = (lightMap[y][z][x] & 0xF) | (level << 4);
	}
	inline int GetLight(const int& x, const int& y, const int& z) {
		return lightMap[y][z][x] & 0xF;
	}
	// Get Light in sourrounding neighbours, will return 0 if neighbour is null
	inline int GetLightNeighbourhood(const int& x, const int& y, const int& z) {
		Chunk* chunk = this;

		// Reverse sides
		if(x >= CHUNK_SIZE)	chunk = neighbourChunks[(int)NEIGHBOUR::RIGHT];
		else if(x < 0)		chunk = neighbourChunks[(int)NEIGHBOUR::LEFT];
		else if(z >= CHUNK_SIZE)	chunk = neighbourChunks[(int)NEIGHBOUR::FRONT];
		else if(z < 0)				chunk = neighbourChunks[(int)NEIGHBOUR::BACK];
		else if(x < 0 && z < 0)				chunk = neighbourChunks[(int)NEIGHBOUR::BACK_LEFT];
		else if(x >= CHUNK_SIZE && z < 0)	chunk = neighbourChunks[(int)NEIGHBOUR::BACK_RIGHT];
		else if(x < 0 && z >= CHUNK_SIZE)			chunk = neighbourChunks[(int)NEIGHBOUR::FRONT_LEFT];
		else if(x >= CHUNK_SIZE && z >= CHUNK_SIZE)	chunk = neighbourChunks[(int)NEIGHBOUR::FRONT_RIGHT];

		if(chunk == nullptr)
			return 0;

		return chunk->GetLight(Math::Modulo(x, CHUNK_SIZE), glm::clamp(y, 0, CHUNK_HEIGHT - 1), Math::Modulo(z, CHUNK_SIZE));
	}
	inline void SetLight(const int& x, const int& y, const int& z, const int& level) {
		if(x < 0 || x >= CHUNK_SIZE || y < 0 || y >= CHUNK_HEIGHT || z < 0 || z >= CHUNK_HEIGHT)
			return;

		lightMap[y][z][x] = (lightMap[y][z][x] & 0xF0) | level;
	}


public: // Private

	// Chunk Data
	struct ChunkSlice {
		int solidBlocks[Chunk::CHUNK_SLICE];
		int transparentBlocks[Chunk::CHUNK_SLICE];
	} chunkSlice = ChunkSlice(); // TEST

	BlockIDs blocks[CHUNK_MASS]; // All blocks inside of the chunk
	glm::ivec2 localCoord; // Local Coordinates of the chunk
	glm::vec3 worldCoord; // World Coordinates of the chunk
	ChunkManager* chunkManager; // Pointer to world
	int highestBlock; // Highest block inside the chunk
	bool isDirty; // Is chunk dirty, needs to regenerate mesh data
	bool uploadMeshToGPU; // Does chunk need to upload its data to the gpu
	bool hasWorldData; // Has generated world data
	bool hasSunlight;
	AABB* aabb; // Used for frustum culling chunk


	// LIGHTING
	unsigned char lightMap[CHUNK_HEIGHT][CHUNK_SIZE][CHUNK_SIZE];

	std::queue<LightNode> lightQueue = std::queue<LightNode>();
	std::queue<LightRemoveNode> lightRemoveQueue = std::queue<LightRemoveNode>();
	std::queue<LightNode> sunlightQueue = std::queue<LightNode>();
	std::queue<LightRemoveNode> sunlightRemoveQueue = std::queue<LightRemoveNode>();

	// Neighbours
	const glm::ivec2 neighbourOffsets[8] = { 
		{-1, 0}, {1, 0}, {0, 1}, {0, -1},
		{-1, 1}, {1, 1}, {-1, -1}, {1, -1}
	};
	Chunk* neighbourChunks[8];

	// Mesh Data
	MeshData solidMesh = MeshData();
	MeshData waterMesh = MeshData();
};

