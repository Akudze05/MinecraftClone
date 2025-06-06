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
#include "Window.h"

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

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

const char *vertexShaderSource = "#version 330 core\n"
"layout (location = 0) in vec3 aPos;\n"
"layout (location = 1) in vec3 aColor;\n"
"layout (location = 2) in vec2 aTexCoord;\n"
"out vec3 ourColor;\n"
"out vec2 TexCoord;\n"
"uniform mat4 view;\n"
"uniform mat4 projection;\n"
"\n"
"void main() {\n"
"   gl_Position = projection * view * vec4(aPos, 1.0);\n"
"   ourColor = aColor;\n"
"   TexCoord = aTexCoord;\n"
"}\0";

const char *fragmentShaderSource = "#version 330 core\n"
"out vec4 FragColor;\n"
"in vec3 ourColor;\n"
"in vec2 TexCoord;\n"
"uniform sampler2D ourTexture;\n"
"void main()\n"
"{\n"
"    FragColor = texture(ourTexture, TexCoord) * vec4(ourColor, 1.0);\n"
"}\0";

void framebuffer_size_callback(GLFWwindow* window, int width, int height) {
    glViewport(0, 0, width, height);
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

    glUseProgram(shaderProgram);
    glBindVertexArray(Window::VAO);

    // ******************************  Texture  ******************************

    unsigned int texture;
    glGenTextures(1, &texture);
    glBindTexture(GL_TEXTURE_2D, texture);
    Window::texture = texture;

    // set the texture wrapping/filtering options (on currently bound texture)
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    // load and generate the texture
    int texture_width, texture_height, texture_nrChannels;
    unsigned char *data = stbi_load("C:/Users/Asg/CLionProjects/untitled1/src/Window/Sprite-0001-export.png", &texture_width, &texture_height,
    &texture_nrChannels, 0);

    if (data)
    {
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, texture_width, texture_height, 0,
             GL_RGBA, GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(GL_TEXTURE_2D);
    } else {
        std::cout << "[:ERROR:] Failed to load texture" << std::endl;
    }

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

    stbi_image_free(data);

    // ********************************    End    ***********************************

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

    // Set up EBO
    unsigned int indices[] = {
        // Передняя грань
        0, 1, 2,
        2, 3, 0,

        // Задняя грань
        4, 5, 6,
        6, 7, 4,

        // Верхняя грань
        8, 9, 10,
        10, 11, 8,

        // Нижняя грань
        12, 13, 14,
        14, 15, 12,

        // Правая грань
        16, 17, 18,
        18, 19, 16,

        // Левая грань
        20, 21, 22,
        22, 23, 20
    };

    unsigned int EBO;
    glGenBuffers(1, &EBO);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

    // Main loop rendering
    while (!isShouldClose()) {
        // Input processing
        processInput();

        float currentFrame = glfwGetTime();
        deltaTime = currentFrame - lastFrame;
        lastFrame = currentFrame;

        // Clear buffers
        glClearColor(0.3f, 0.6f, 0.75f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        
        // Activate shader
        glUseProgram(shaderProgram);

        glEnable(GL_DEPTH_TEST);

        // Set up view/projection matrices
        glm::mat4 view = camera.GetViewMatrix();
        glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(view));
        glUniformMatrix4fv(projLoc, 1, GL_FALSE, glm::value_ptr(projection));

        // Bind texture
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, texture);
        glUniform1i(glGetUniformLocation(shaderProgram, "ourTexture"), 0);

        // Draw
        glBindVertexArray(VAO);
        glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);

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
