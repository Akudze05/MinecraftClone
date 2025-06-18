#pragma once
#include <vector>
#include <C:\Users\Asg\CLionProjects\MineEngine\dependencies\glm\glm.hpp> // SET YOUR PATH TO GLM
#include <C:\Users\Asg\CLionProjects\MineEngine\dependencies\glm/gtc/type_ptr.hpp>
#include <glad/glad.h>

class Chunk {
public:
	static const int SIZE = 16;
	static const int HEIGHT = 256;

	Chunk(int x, int z);
	~Chunk();

	void GenerateTerrain();
	void RebuildMesh();
	void Render() const;

	unsigned char GetBlock(int x, int y, int z) const;
	void SetBlock(int x, int y, int z, unsigned char type);

	bool IsMeshBuilt() const { return meshBuilt; }
	int GetX() const { return x; }
	int GetZ() const { return z; }

private:
	int x, z;
	std::vector<unsigned char> blocks;
	std::vector<float> meshData; // Добавьте это
	bool meshBuilt = false;

	unsigned int VAO, VBO;
	size_t vertexCount = 0;

	void AddFace(const glm::vec3& position, const glm::vec3& normal,
				unsigned char blockType, int face);
	bool IsBlockTransparent(unsigned char type) const;
};