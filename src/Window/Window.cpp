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

float deltaTime = 0.0f;
float lastFrame = 0.0f;

float vertices[] = {
    //1
    -0.3f, -0.5f, 0.0f, 1.0f, 0.4f, 0.2f,
    0.3f, -0.5f, 0.0f, 1.0f, 1.0f, 0.2f,
    -0.3, 0.5f, 0.0f, 1.0f, 1.0f, 0.2f,
    0.3f, 0.5f, 0.0f, 1.0f, 0.4f, 0.2f,
    0.3f, -0.5f, 0.0f, 1.0f, 1.0f, 0.2f,
    -0.3f, 0.5f, 0.0f, 1.0f, 1.0f, 0.2f,
};

bool firstMouse = true;

const char *vertexShaderSource = "#version 330 core\n"
"layout (location = 0) in vec3 aPos;\n"
"layout (location = 1) in vec3 aColor;\n"
"out vec3 ourColor;\n"  // Добавьте эту строку
"uniform mat4 view;\n"
"uniform mat4 projection;\n"
"void main()\n"
"{\n"
"   gl_Position = projection * view * vec4(aPos, 1.0);\n"
"   ourColor = aColor;\n"  // Добавьте точку с запятой
"}\0";

const char *fragmentShaderSource = "#version 330 core\n"
"in vec3 ourColor;\n"  // Добавьте эту строку
"out vec4 FragColor;\n"
"void main() {\n"
"   FragColor = vec4(ourColor, 1.0);\n"  // Исправьте 1f на 1.0
"}\0";

void framebuffer_size_callback(GLFWwindow* window, int width, int height) {
    glViewport(0, 0, width, height);
}

// Коллбэк для мыши:
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

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), static_cast<void *>(nullptr));
    glEnableVertexAttribArray(0);

    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3* sizeof(float)));
    glEnableVertexAttribArray(1);

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

        float currentFrame = glfwGetTime();
        deltaTime = currentFrame - lastFrame;
        lastFrame = currentFrame;

        camera.Speed = 60.0f * deltaTime;

        // Activate shader
        glUseProgram(shaderProgram);
        glBindVertexArray(VAO);

        glEnable(GL_DEPTH_TEST);

        // --- Добавляем тут ---
        glm::mat4 view = camera.GetViewMatrix();
        glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(view));
        glUniformMatrix4fv(projLoc, 1, GL_FALSE, glm::value_ptr(projection));
        // --------------------

        // Clear boofer color (background color)
        glClearColor(0.3f, 0.6f, 0.75f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        // rendering here ...
        glDrawArrays(GL_TRIANGLES, 0, 6);


        // Poll events and swap boofers
        swapBuffers();
        glfwPollEvents();
    }

    // Clear resources
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

