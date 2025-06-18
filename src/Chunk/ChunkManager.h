#pragma once
#include <unordered_map>
#include <string>
#include <C:\Users\Asg\CLionProjects\MineEngine\dependencies\glm\glm.hpp> // SET YOUR PATH TO GLM
#include "Chunk.h"

class ChunkManager {
public:
	ChunkManager();
	~ChunkManager();

	void Update(const glm::vec3& playerPosition);
	void Render() const;

	unsigned char GetBlock(int x, int y, int z) const;
	void SetBlock(int x, int y, int z, unsigned char type);

	void SetRenderDistance(int distance) { renderDistance = distance; }

private:
	std::unordered_map<std::string, Chunk*> chunks;
	int renderDistance = 8;

	void LoadChunk(int x, int z);
	void UnloadChunk(int x, int z);
	void UnloadAllChunks();

	std::string GetChunkKey(int x, int z) const;
	void GetChunkCoordinates(int worldX, int worldZ, int& chunkX, int& chunkZ) const;
};