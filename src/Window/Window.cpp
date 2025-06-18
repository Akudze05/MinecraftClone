/*
 sky color - (0.3f, 0.6f, 0.75f, 1.0f) last argument alpha
 */

#include <glad/glad.h>
#include <iostream>
#include <GLFW/glfw3.h>
#include <C:\Users\Asg\CLionProjects\MineEngine\dependencies\glm\glm.hpp> // SET YOUR PATH TO GLM
#include <C:\Users\Asg\CLionProjects\MineEngine\dependencies\glm/gtc/matrix_transform.hpp>
#include <C:\Users\Asg\CLionProjects\MineEngine\dependencies\glm/gtc/type_ptr.hpp>
#include "Camera.h"
#include "../Chunk/ChunkManager.h"
#include "Window.h"

unsigned int Window::textureAtlas = 0;

#define STB_IMAGE_IMPLEMENTATION
#include "../dependencies/stb/stb_image.h"

#define WINDOW_WIDTH 1920
#define WINDOW_HEIGHT 1080

Camera Window::camera;
float Window::lastX = 0.0f;
float Window::lastY = 0.0f;
bool Window::firstMouse = true;
unsigned int Window::projLoc = 0;  // <-- Добавьте это
unsigned int Window::viewLoc = 0;  // <-- И это
glm::mat4 Window::projection = glm::mat4(1.0f);

unsigned int Window::shaderProgram;  // Добавляем это
unsigned int Window::VAO = 0;        // Добавляем это
unsigned int Window::VBO = 0;

GLFWwindow* Window::window;

ChunkManager chunkManager;
glm::vec3 playerPosition;

float texCoords[] = {
    0.0f, 0.0f, // lower-left corner
    1.0f, 0.0f, // lower-right corner
    0.5f, 1.0f // top-center corner
};

unsigned int Window::texture = 0;

float deltaTime = 0.0f;
float lastFrame = 0.0f;

//              0,0,0 (top right) 1,0,0 (top left)
//              +-----------------+
//              | \               |
//              |   \             |
//              |     \           |
//              |       \         |
//              |         \       |
//              |           \     |
//              +-----------------+
//              0,0,1 (bottom right)  1,0,1 (bottom left)

float vertices[] = {
    // Позиции          // Цвета       // Текстурные координаты
    // Передняя грань (Z+)
    -0.5f, -0.5f,  0.5f, 1.0f,1.0f,1.0f, 0.0f, 0.0f, // 0
     0.5f, -0.5f,  0.5f, 1.0f,1.0f,1.0f, 1.0f, 0.0f, // 1
     0.5f,  0.5f,  0.5f, 1.0f,1.0f,1.0f, 1.0f, 1.0f, // 2
    -0.5f,  0.5f,  0.5f, 1.0f,1.0f,1.0f, 0.0f, 1.0f, // 3

    // Задняя грань (Z-)
    -0.5f, -0.5f, -0.5f, 1.0f,1.0f,1.0f, 1.0f, 0.0f, // 4
     0.5f, -0.5f, -0.5f, 1.0f,1.0f,1.0f, 0.0f, 0.0f, // 5
     0.5f,  0.5f, -0.5f, 1.0f,1.0f,1.0f, 0.0f, 1.0f, // 6
    -0.5f,  0.5f, -0.5f, 1.0f,1.0f,1.0f, 1.0f, 1.0f, // 7

    // Верхняя грань (Y+)
    -0.5f,  0.5f,  0.5f, 1.0f,1.0f,1.0f, 0.0f, 1.0f, // 8  (3)
     0.5f,  0.5f,  0.5f, 1.0f,1.0f,1.0f, 1.0f, 1.0f, // 9  (2)
     0.5f,  0.5f, -0.5f, 1.0f,1.0f,1.0f, 1.0f, 0.0f, // 10 (6)
    -0.5f,  0.5f, -0.5f, 1.0f,1.0f,1.0f, 0.0f, 0.0f, // 11 (7)

    // Нижняя грань (Y-)
    -0.5f, -0.5f,  0.5f, 1.0f,1.0f,1.0f, 0.0f, 0.0f, // 12 (0)
     0.5f, -0.5f,  0.5f, 1.0f,1.0f,1.0f, 1.0f, 0.0f, // 13 (1)
     0.5f, -0.5f, -0.5f, 1.0f,1.0f,1.0f, 1.0f, 1.0f, // 14 (5)
    -0.5f, -0.5f, -0.5f, 1.0f,1.0f,1.0f, 0.0f, 1.0f, // 15 (4)

    // Правая грань (X+)
     0.5f, -0.5f,  0.5f, 1.0f,1.0f,1.0f, 0.0f, 0.0f, // 16 (1)
     0.5f,  0.5f,  0.5f, 1.0f,1.0f,1.0f, 0.0f, 1.0f, // 17 (2)
     0.5f,  0.5f, -0.5f, 1.0f,1.0f,1.0f, 1.0f, 1.0f, // 18 (6)
     0.5f, -0.5f, -0.5f, 1.0f,1.0f,1.0f, 1.0f, 0.0f, // 19 (5)

    // Левая грань (X-)
    -0.5f, -0.5f,  0.5f, 1.0f,1.0f,1.0f, 1.0f, 0.0f, // 20 (0)
    -0.5f,  0.5f,  0.5f, 1.0f,1.0f,1.0f, 1.0f, 1.0f, // 21 (3)
    -0.5f,  0.5f, -0.5f, 1.0f,1.0f,1.0f, 0.0f, 1.0f, // 22 (7)
    -0.5f, -0.5f, -0.5f, 1.0f,1.0f,1.0f, 0.0f, 0.0f  // 23 (4)
};

bool firstMouse = true;

int texture_width, texture_height, texture_nrChannels;
unsigned char *data = stbi_load("C:/Users/Asg/CLionProjects/untitled1/src/assets/Sprite-0001.png", &texture_width, &texture_height,
&texture_nrChannels, 0);

const char *vertexShaderSource =
"#version 330 core\n"
"layout (location = 0) in vec3 aPos;\n"
"layout (location = 1) in vec3 aNormal;\n"
"layout (location = 2) in vec2 aTexCoord;\n"
"out vec3 FragPos;\n"
"out vec2 TexCoord;\n"
"out vec3 Normal;\n"
"uniform mat4 view;\n"
"uniform mat4 projection;\n"
"uniform mat4 model;\n"
"\n"
"void main() {\n"
"   FragPos = vec3(model * vec4(aPos, 1.0));\n"
"   Normal = mat3(transpose(inverse(model))) * aNormal;\n"
"   TexCoord = aTexCoord;\n"
"   gl_Position = projection * view * model * vec4(aPos, 1.0);\n"
"}\0";

const char *fragmentShaderSource =
"#version 330 core\n"
"in vec3 FragPos;\n"
"in vec3 Normal;\n"
"in vec2 TexCoord;\n"
"\n"
"out vec4 FragColor;\n"
"\n"
"uniform sampler2D textureAtlas;\n"
"uniform vec3 lightPos = vec3(100.0, 100.0, 100.0);\n"
"\n"
"void main() {\n"
"    // Простое освещение\n"
"    vec3 norm = normalize(Normal);\n"
"    vec3 lightDir = normalize(lightPos - FragPos);\n"
"    float diff = max(dot(norm, lightDir), 0.0);\n"
"    vec3 diffuse = diff * vec3(1.0, 1.0, 1.0);\n"
"    \n"
"    // Текстура + освещение\n"
"    vec3 texColor = texture(textureAtlas, TexCoord).rgb;\n"
"    FragColor = vec4(texColor * (diffuse + 0.3), 1.0); // 0.3 - ambient\n"
"}\0";

void framebuffer_size_callback(GLFWwindow* window, int width, int height) {
    glViewport(0, 0, width, height);
}

unsigned int LoadTexture(const char* path) {
    unsigned int textureID;
    glGenTextures(1, &textureID);

    int width, height, nrComponents;
    stbi_set_flip_vertically_on_load(true); // Добавьте эту строку
    unsigned char *data = stbi_load(path, &width, &height, &nrComponents, 0);

    if (data) {
        GLenum format;
        if (nrComponents == 1) format = GL_RED;
        else if (nrComponents == 3) format = GL_RGB;
        else if (nrComponents == 4) format = GL_RGBA;

        glBindTexture(GL_TEXTURE_2D, textureID);
        glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(GL_TEXTURE_2D);

        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

        stbi_image_free(data);
    } else {
        std::cerr << "Failed to load texture: " << path << std::endl;
        stbi_image_free(data);
        return 0;
    }
    return textureID;
}

// Callback for mouse:
void mouse_callback(GLFWwindow* window, double xpos, double ypos) {
    if (Window::firstMouse) {
        Window::lastX = xpos;
        Window::lastY = ypos;
        Window::firstMouse = false;
    }

    float xoffset = xpos - Window::lastX;
    float yoffset = Window::lastY - ypos; // Обратно, так как Y-координаты идут снизу вверх
    Window::lastX = xpos;
    Window::lastY = ypos;

    xoffset *= Window::camera.Sensitivity;
    yoffset *= Window::camera.Sensitivity;

    Window::camera.Yaw += xoffset;
    Window::camera.Pitch += yoffset;

    // Ограничиваем угол наклона
    if (Window::camera.Pitch > 89.0f)
        Window::camera.Pitch = 89.0f;
    if (Window::camera.Pitch < -89.0f)
        Window::camera.Pitch = -89.0f;

    // Пересчитываем направление взгляда
    glm::vec3 front;
    front.x = cos(glm::radians(Window::camera.Yaw)) * cos(glm::radians(Window::camera.Pitch));
    front.y = sin(glm::radians(Window::camera.Pitch));
    front.z = sin(glm::radians(Window::camera.Yaw)) * cos(glm::radians(Window::camera.Pitch));
    Window::camera.Front = glm::normalize(front);
}

int Window::initialize(int width, int height, const char *title) {

    playerPosition = camera.Position; // Обновляем позицию игрока
    camera.Position = glm::vec3(0, 70, 50); // Жёстко задайте позицию для теста
    camera.Front = glm::vec3(0, -0.5f, -1.0f); // Смотрит вниз и вперёд

    // Init GLFW
    if (!glfwInit()) {
        std::cerr << "Failed to initialize GLFW!" << std::endl;
        return -1;
    }
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    // Create window
    window = glfwCreateWindow(width, height, title, nullptr, nullptr);

    if (window == nullptr) {
        std::cerr << "Failed to create GLFW window." << std::endl;
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window); // Set current window

    // Init Glad
    if (!gladLoadGLLoader(reinterpret_cast<GLADloadproc>(glfwGetProcAddress))) {
        std::cerr << "Failed to initialize GLAD." << std::endl;
        return -1;
    }

    glfwSetCursorPosCallback(window, mouse_callback);
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED); // Захватываем курсор

    unsigned int VAO;
    glGenVertexArrays(1, &VAO);
    glBindVertexArray(VAO);

    unsigned int VBO;
    glGenBuffers(1, &VBO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    Window::VAO = VAO;
    Window::VBO = VBO;

    unsigned int vertexShader;
    vertexShader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertexShader, 1, &vertexShaderSource, nullptr);
    glCompileShader(vertexShader);

    unsigned int fragmentShader;
    fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragmentShader, 1, &fragmentShaderSource, nullptr);
    glCompileShader(fragmentShader);

    Window::shaderProgram = glCreateProgram();

    glAttachShader(shaderProgram, vertexShader);
    glAttachShader(shaderProgram, fragmentShader);
    glLinkProgram(shaderProgram);

    glUseProgram(shaderProgram);

    // Исправьте:
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));
    glEnableVertexAttribArray(0);
    glEnableVertexAttribArray(1);
    glEnableVertexAttribArray(2);

    textureAtlas = LoadTexture("C:/Users/Asg/CLionProjects/MineCraft2/src/Assets/Sprite-0001-export.png");
    if (textureAtlas == 0) {
        std::cerr << "Failed to load texture atlas!" << std::endl;
        return -1;
    }

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

    glUseProgram(shaderProgram);
    glBindVertexArray(Window::VAO);



    projection = glm::perspective(
    glm::radians(45.0f),
    (float)WINDOW_WIDTH / (float)WINDOW_HEIGHT,
    0.1f, 100.0f
);

    // Получаем location uniform-переменных (один раз!)
    Window::viewLoc = glGetUniformLocation(Window::shaderProgram, "view");
    Window::projLoc = glGetUniformLocation(Window::shaderProgram, "projection");

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_MIRRORED_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_MIRRORED_REPEAT);

    // Set viewport
    glViewport(0, 0, width, height);
    SetFramebufferSize();

    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);

    return 0;
}

void Window::terminate() {
    glfwTerminate();
}

void Window::processInput() {
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);

    float cameraSpeed = 2.5f * deltaTime;  // Фиксированная скорость с учетом deltaTime

    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
        camera.Position += cameraSpeed * camera.Front;
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
        camera.Position -= cameraSpeed * camera.Front;
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
        camera.Position -= glm::normalize(glm::cross(camera.Front, camera.Up)) * cameraSpeed;
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
        camera.Position += glm::normalize(glm::cross(camera.Front, camera.Up)) * cameraSpeed;
    if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS)
        camera.Position += cameraSpeed * camera.Up;
    if (glfwGetKey(window, GLFW_KEY_LEFT_CONTROL) == GLFW_PRESS)
        camera.Position -= cameraSpeed * camera.Up;
}

void Window::mainloop() {
    initialize(WINDOW_WIDTH, WINDOW_HEIGHT, "MineCraft");

    // Main loop rendering
    while (!isShouldClose()) {
        // Input processing
        processInput();

        // Обновляем чанки
        chunkManager.Update(playerPosition);

        float currentFrame = glfwGetTime();
        deltaTime = currentFrame - lastFrame;
        lastFrame = currentFrame;

        // Clear buffers
        glClearColor(0.3f, 0.6f, 0.75f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        // Set up view/projection matrices
        glm::mat4 view = camera.GetViewMatrix();
        // В Window::mainloop() замените:
        glm::mat4 projection = glm::perspective(glm::radians(45.0f),
            (float)WINDOW_WIDTH/(float)WINDOW_HEIGHT, 0.1f, 1000.0f); // Было 100.0f
        glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(view));
        glUniformMatrix4fv(projLoc, 1, GL_FALSE, glm::value_ptr(projection));

        // Bind texture

        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, textureAtlas);
        glUniform1i(glGetUniformLocation(shaderProgram, "textureAtlas"), 0);

        // Activate shader
        glUseProgram(shaderProgram);

        glEnable(GL_DEPTH_TEST);

        // Draw
        // glBindVertexArray(VAO);
        // glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);

        chunkManager.Render();

        // Swap buffers and poll events
        swapBuffers();
        glfwPollEvents();
    }

    terminate();
}

void Window::SetFramebufferSize() {
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
}

void Window::swapBuffers() {
    glfwSwapBuffers(window);
}

bool Window::isShouldClose() {
    return glfwWindowShouldClose(window);
}
