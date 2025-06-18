//
// Created by Asg on 12.06.2025.
//
#pragma once
#include <vector>
#include <glad/glad.h>
#include <C:\Users\Asg\CLionProjects\MineEngine\dependencies\glm\glm.hpp> // SET YOUR PATH TO GLM

class Chunk {
public:
	static constexpr int CHUNK_SIZE = 16;
	static constexpr int CHUNK_HEIGHT = 256;

	Chunk(int x, int z) : x(x), z(z) {
		blocks = new BlockType[CHUNK_SIZE * CHUNK_HEIGHT * CHUNK_SIZE];
		// Инициализация блоков (можно заполнить воздухом)
	}

	static void generateChunk();

private:

};