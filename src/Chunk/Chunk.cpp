#include "Chunk.h"
#include <cmath>
#include <iostream>
#include "../Window/Window.h"

Chunk::Chunk(int x, int z) : x(x), z(z) {
    blocks.resize(SIZE * HEIGHT * SIZE, 0);
}

Chunk::~Chunk() {
    if (meshBuilt) {
        glDeleteVertexArrays(1, &VAO);
        glDeleteBuffers(1, &VBO);
    }
}

void Chunk::GenerateTerrain() {
    for (int x = 0; x < SIZE; x++) {
        for (int z = 0; z < SIZE; z++) {
            float wx = x + this->x * SIZE;
            float wz = z + this->z * SIZE;
            // Исправленная формула высоты:
            int height = 64 + (int)(sin(wx * 0.1f) * 10.0f + cos(wz * 0.1f) * 10.0f);

            for (int y = 0; y < HEIGHT; y++) {
                if (y < height - 4) {
                    SetBlock(x, y, z, 1); // Камень
                } else if (y < height) {
                    SetBlock(x, y, z, 2); // Земля
                } else if (y == height) {
                    SetBlock(x, y, z, 3); // Трава
                }
            }
        }
    }
}

void Chunk::RebuildMesh() {
    meshData.clear(); // Очищаем данные меша

    for (int x = 0; x < SIZE; x++) {
        for (int y = 0; y < HEIGHT; y++) {
            for (int z = 0; z < SIZE; z++) {
                unsigned char block = GetBlock(x, y, z);
                if (block == 0) continue;

                glm::vec3 pos(x, y, z);

                // Проверка соседних блоков
                if (x == 0 || IsBlockTransparent(GetBlock(x-1, y, z)))
                    AddFace(pos, glm::vec3(-1, 0, 0), block, 0);
                if (x == SIZE-1 || IsBlockTransparent(GetBlock(x+1, y, z)))
                    AddFace(pos, glm::vec3(1, 0, 0), block, 1);
                if (y == 0 || IsBlockTransparent(GetBlock(x, y-1, z)))
                    AddFace(pos, glm::vec3(0, -1, 0), block, 2);
                if (y == HEIGHT-1 || IsBlockTransparent(GetBlock(x, y+1, z)))
                    AddFace(pos, glm::vec3(0, 1, 0), block, 3);
                if (z == 0 || IsBlockTransparent(GetBlock(x, y, z-1)))
                    AddFace(pos, glm::vec3(0, 0, -1), block, 4);
                if (z == SIZE-1 || IsBlockTransparent(GetBlock(x, y, z+1)))
                    AddFace(pos, glm::vec3(0, 0, 1), block, 5);
            }
        }
    }

    if (!meshBuilt) {
        glGenVertexArrays(1, &VAO);
        glGenBuffers(1, &VBO);
        meshBuilt = true;
    }

    glBindVertexArray(VAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, meshData.size() * sizeof(float), meshData.data(), GL_STATIC_DRAW);

    // Настройка атрибутов вершин
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(2);
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));

    glBindVertexArray(0);
    vertexCount = meshData.size() / 8; // 8 значений на вершину

    std::cout << "Rebuilt mesh for chunk (" << x << "," << z << ") with "
              << vertexCount << " vertices" << std::endl;
}

void Chunk::Render() const {
    if (!meshBuilt || vertexCount == 0) {
        std::cout << "Chunk (" << x << "," << z << ") not ready to render" << std::endl;
        return;
    }

    glm::mat4 model = glm::mat4(1.0f);
    model = glm::translate(model, glm::vec3(x * SIZE, 0, z * SIZE));

    GLuint modelLoc = glGetUniformLocation(Window::shaderProgram, "model");
    if (modelLoc == -1) {
        std::cerr << "Failed to get model uniform location" << std::endl;
        return;
    }
    glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));

    glBindVertexArray(VAO);
    glDrawArrays(GL_TRIANGLES, 0, vertexCount);
    glBindVertexArray(0);
}

unsigned char Chunk::GetBlock(int x, int y, int z) const {
    if (x < 0 || x >= SIZE || y < 0 || y >= HEIGHT || z < 0 || z >= SIZE)
        return 0;
    return blocks[y * (SIZE * SIZE) + z * SIZE + x];
}

void Chunk::SetBlock(int x, int y, int z, unsigned char type) {
    if (x >= 0 && x < SIZE && y >= 0 && y < HEIGHT && z >= 0 && z < SIZE) {
        blocks[y * (SIZE * SIZE) + z * SIZE + x] = type;
    }
}

void Chunk::AddFace(const glm::vec3& position, const glm::vec3& normal, 
                   unsigned char blockType, int face) {

    // Текстурные координаты (0-1 для всего блока)
    const glm::vec2 texCoords[4] = {
        {0.0f, 0.0f}, {1.0f, 0.0f}, {1.0f, 1.0f}, {0.0f, 1.0f}
    };

    // Текстурные координаты для грани (нижний левый, нижний правый, верхний правый, верхний левый)
    const glm::vec2 baseTexCoords[4] = {
        glm::vec2(0.0f, 0.0f),
        glm::vec2(1.0f, 0.0f),
        glm::vec2(1.0f, 1.0f),
        glm::vec2(0.0f, 1.0f)
    };

    // Определяем вершины грани в зависимости от направления
    glm::vec3 vertices[4];
    switch(face) {
        case 0: // Левая грань (-X)
            vertices[0] = position + glm::vec3(0, 0, 0);
            vertices[1] = position + glm::vec3(0, 1, 0);
            vertices[2] = position + glm::vec3(0, 1, 1);
            vertices[3] = position + glm::vec3(0, 0, 1);
            break;
        case 1: // Правая грань (+X)
            vertices[0] = position + glm::vec3(1, 0, 0);
            vertices[1] = position + glm::vec3(1, 0, 1);
            vertices[2] = position + glm::vec3(1, 1, 1);
            vertices[3] = position + glm::vec3(1, 1, 0);
            break;
        case 2: // Нижняя грань (-Y)
            vertices[0] = position + glm::vec3(0, 0, 0);
            vertices[1] = position + glm::vec3(1, 0, 0);
            vertices[2] = position + glm::vec3(1, 0, 1);
            vertices[3] = position + glm::vec3(0, 0, 1);
            break;
        case 3: // Верхняя грань (+Y)
            vertices[0] = position + glm::vec3(0, 1, 0);
            vertices[1] = position + glm::vec3(0, 1, 1);
            vertices[2] = position + glm::vec3(1, 1, 1);
            vertices[3] = position + glm::vec3(1, 1, 0);
            break;
        case 4: // Задняя грань (-Z)
            vertices[0] = position + glm::vec3(0, 0, 0);
            vertices[1] = position + glm::vec3(0, 1, 0);
            vertices[2] = position + glm::vec3(1, 1, 0);
            vertices[3] = position + glm::vec3(1, 0, 0);
            break;
        case 5: // Передняя грань (+Z)
            vertices[0] = position + glm::vec3(0, 0, 1);
            vertices[1] = position + glm::vec3(1, 0, 1);
            vertices[2] = position + glm::vec3(1, 1, 1);
            vertices[3] = position + glm::vec3(0, 1, 1);
            break;
    }

    // Определяем текстурные координаты в атласе текстур
    float tileSize = 1.0f / 16.0f; // Предполагаем атлас 16x16 текстур
    float texOffsetX = (blockType % 16) * tileSize;
    float texOffsetY = (blockType / 16) * tileSize;

    // Индексы для создания двух треугольников (0,1,2 и 0,2,3)
    const int indices[6] = {0, 1, 2, 0, 2, 3};

    // Добавляем 6 вершин (2 треугольника) в meshData
    for (int i = 0; i < 6; i++) {
        int vertexIndex = indices[i];

        // Позиция вершины
        meshData.push_back(vertices[vertexIndex].x);
        meshData.push_back(vertices[vertexIndex].y);
        meshData.push_back(vertices[vertexIndex].z);

        // Нормаль
        meshData.push_back(normal.x);
        meshData.push_back(normal.y);
        meshData.push_back(normal.z);

        // Текстурные координаты (с учётом атласа)
        meshData.push_back(baseTexCoords[vertexIndex].x * tileSize + texOffsetX);
        meshData.push_back(baseTexCoords[vertexIndex].y * tileSize + texOffsetY);
    }
}

bool Chunk::IsBlockTransparent(unsigned char type) const {
    return type == 0; // Только воздух прозрачный
}