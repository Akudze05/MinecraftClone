#include "ChunkManager.h"
#include <algorithm>

ChunkManager::ChunkManager() {}

ChunkManager::~ChunkManager() {
    UnloadAllChunks();
}

void ChunkManager::Update(const glm::vec3& playerPosition) {
    int playerChunkX, playerChunkZ;
    GetChunkCoordinates((int)playerPosition.x, (int)playerPosition.z, playerChunkX, playerChunkZ);
    
    // Выгрузка дальних чанков
    std::vector<std::string> toUnload;
    for (auto& pair : chunks) {
        Chunk* chunk = pair.second;
        int dx = abs(chunk->GetX() - playerChunkX);
        int dz = abs(chunk->GetZ() - playerChunkZ);
        
        if (dx > renderDistance || dz > renderDistance) {
            toUnload.push_back(pair.first);
        }
    }
    
    for (auto& key : toUnload) {
        delete chunks[key];
        chunks.erase(key);
    }
    
    // Загрузка новых чанков
    for (int x = -renderDistance; x <= renderDistance; x++) {
        for (int z = -renderDistance; z <= renderDistance; z++) {
            int chunkX = playerChunkX + x;
            int chunkZ = playerChunkZ + z;
            std::string key = GetChunkKey(chunkX, chunkZ);
            
            if (chunks.find(key) == chunks.end()) {
                LoadChunk(chunkX, chunkZ);
            }
        }
    }
}

void ChunkManager::Render() const {
    for (auto& pair : chunks) {
        pair.second->Render();
    }
}

unsigned char ChunkManager::GetBlock(int x, int y, int z) const {
    int chunkX, chunkZ;
    GetChunkCoordinates(x, z, chunkX, chunkZ);
    
    std::string key = GetChunkKey(chunkX, chunkZ);
    auto it = chunks.find(key);
    if (it != chunks.end()) {
        int localX = x - chunkX * Chunk::SIZE;
        int localZ = z - chunkZ * Chunk::SIZE;
        return it->second->GetBlock(localX, y, localZ);
    }
    return 0; // Воздух для не загруженных чанков
}

void ChunkManager::SetBlock(int x, int y, int z, unsigned char type) {
    int chunkX, chunkZ;
    GetChunkCoordinates(x, z, chunkX, chunkZ);
    
    std::string key = GetChunkKey(chunkX, chunkZ);
    auto it = chunks.find(key);
    if (it != chunks.end()) {
        int localX = x - chunkX * Chunk::SIZE;
        int localZ = z - chunkZ * Chunk::SIZE;
        it->second->SetBlock(localX, y, localZ, 1);
        it->second->RebuildMesh(); // Перестраиваем меш после изменения блоков
    }
}

void ChunkManager::LoadChunk(int x, int z) {
    std::string key = GetChunkKey(x, z);
    if (chunks.find(key) != chunks.end()) return;
    
    Chunk* chunk = new Chunk(x, z);
    chunk->GenerateTerrain();
    chunk->RebuildMesh();
    chunks[key] = chunk;
}

void ChunkManager::UnloadChunk(int x, int z) {
    std::string key = GetChunkKey(x, z);
    auto it = chunks.find(key);
    if (it != chunks.end()) {
        delete it->second;
        chunks.erase(it);
    }
}

void ChunkManager::UnloadAllChunks() {
    for (auto& pair : chunks) {
        delete pair.second;
    }
    chunks.clear();
}

std::string ChunkManager::GetChunkKey(int x, int z) const {
    return std::to_string(x) + ":" + std::to_string(z);
}

void ChunkManager::GetChunkCoordinates(int worldX, int worldZ, int& chunkX, int& chunkZ) const {
    chunkX = worldX >= 0 ? worldX / Chunk::SIZE : (worldX + 1) / Chunk::SIZE - 1;
    chunkZ = worldZ >= 0 ? worldZ / Chunk::SIZE : (worldZ + 1) / Chunk::SIZE - 1;
}