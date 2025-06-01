cmake_minimum_required(VERSION 3.31)
project(untitled1)

set(CMAKE_CXX_STANDARD 20)

# Находим необходимые пакеты
find_package(OpenGL REQUIRED)

# Для GLFW (один из вариантов оконной системы)
if(NOT GLFW_FOUND)
    # Можно использовать встроенный fetch или найти установленный пакет
    include(FetchContent)
    FetchContent_Declare(
            glfw
            GIT_REPOSITORY https://github.com/glfw/glfw.git
            GIT_TAG 3.3.8
    )
    FetchContent_MakeAvailable(glfw)
endif()

# Для Glad (загрузчик OpenGL)
# Можно сгенерировать glad на https://glad.dav1d.de и добавить в проект
# Или использовать fetch
FetchContent_Declare(
        glad
        GIT_REPOSITORY https://github.com/Dav1dde/glad.git
        GIT_TAG v0.1.36
)
FetchContent_MakeAvailable(glad)

add_executable(untitled1 src/main.cpp
)

# Подключаем зависимости
target_link_libraries(${PROJECT_NAME}
        OpenGL::GL
        glfw
        glad
)
