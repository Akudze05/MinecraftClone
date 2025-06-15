//
// Created by Asg on 12.06.2025.
//
#pragma once
#include <vector>
#include <glad/glad.h>
#include <C:\Users\Asg\CLionProjects\MineEngine\dependencies\glm\glm.hpp> // SET YOUR PATH TO GLM

class Chunk {
public:
	const int CHUNK_SIZE = 16;
	const int CHUNK_HEIGHT = 256;

	static void generateChunk();

private:

};