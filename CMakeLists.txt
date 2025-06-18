
cmake_minimum_required(VERSION 3.31) # 3.31
project(untitled1)

set(CMAKE_CXX_STANDARD 20)

# Находим необходимые пакеты
find_package(OpenGL REQUIRED)

# Можно использовать встроенный fetch или найти установленный пакет
cmake_minimum_required(VERSION 3.31)
project(MineCraft2)

set(CMAKE_CXX_STANDARD 20)

find_package(OpenGL REQUIRED)

if(NOT GLFW_FOUND)


    include(FetchContent)
    FetchContent_Declare(
            glfw
            GIT_REPOSITORY https://github.com/glfw/glfw.git
            GIT_TAG 3.3.8
    )
    FetchContent_MakeAvailable(glfw)
endif()


FetchContent_Declare(
    glad
    GIT_REPOSITORY https://github.com/Dav1dde/glad.git
    GIT_TAG v0.1.36
)
FetchContent_MakeAvailable(glad)

# Подключаем зависимости

add_executable(${PROJECT_NAME} src/main.cpp
        src/Window/Window.cpp
        src/Window/Window.h
        src/Chunk/Chunk.cpp
        src/Chunk/Chunk.h
        src/Chunk/Block.cpp
        src/Chunk/Block.h
        src/Chunk/ChunkManager.cpp
        src/Chunk/ChunkManager.h
)


target_link_libraries(${PROJECT_NAME}
        OpenGL::GL
        glfw
        glad
)
