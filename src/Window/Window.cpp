#include <glad/glad.h>
#include <iostream>
#include <GLFW/glfw3.h>
#define STB_IMAGE_IMPLEMENTATION
#include "../dependencies/stb/stb_image.h"
#include "Window.h"

#define WINDOW_WIDTH 800
#define WINDOW_HEIGHT 600

const char* TITLE = "Minecraft";

GLFWwindow* Window::window = nullptr;

unsigned int Window::transformLoc = 0;

float vertex[] = {

	// Передняя грань
	-0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
	 0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
	 0.5f,  0.5f,  0.5f,  1.0f, 1.0f,
	-0.5f,  0.5f,  0.5f,  0.0f, 1.0f,

	// Задняя грань
	-0.5f, -0.5f, -0.5f,  1.0f, 0.0f,
	 0.5f, -0.5f, -0.5f,  0.0f, 0.0f,
	 0.5f,  0.5f, -0.5f,  0.0f, 1.0f,
	-0.5f,  0.5f, -0.5f,  1.0f, 1.0f,

	// Левая грань
	-0.5f, -0.5f, -0.5f,  0.0f, 0.0f,
	-0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
	-0.5f,  0.5f,  0.5f,  1.0f, 1.0f,
	-0.5f,  0.5f, -0.5f,  0.0f, 1.0f,

	// Правая грань
	 0.5f, -0.5f, -0.5f,  1.0f, 0.0f,
	 0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
	 0.5f,  0.5f,  0.5f,  0.0f, 1.0f,
	 0.5f,  0.5f, -0.5f,  1.0f, 1.0f,

	// Верхняя грань
	-0.5f,  0.5f,  0.5f,  0.0f, 1.0f,
	 0.5f,  0.5f,  0.5f,  1.0f, 1.0f,
	 0.5f,  0.5f, -0.5f,  1.0f, 0.0f,
	-0.5f,  0.5f, -0.5f,  0.0f, 0.0f,

	// Нижняя грань
	-0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
	 0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
	 0.5f, -0.5f, -0.5f,  1.0f, 1.0f,
	-0.5f, -0.5f, -0.5f,  0.0f, 1.0f

};

unsigned int indices[] = {

	// Передняя грань
	0, 1, 2,
	2, 3, 0,

	// Задняя грань
	4, 5, 6,
	6, 7, 4,

	// Левая грань
	8, 9, 10,
	10, 11, 8,

	// Правая грань
	12, 13, 14,
	14, 15, 12,

	// Верхняя грань
	16, 17, 18,
	18, 19, 16,

	// Нижняя грань
	20, 21, 22,
	22, 23, 20
};

glm::vec3 cubePositions[] = {
	glm::vec3( 0.0f, 0.0f, 0.0f),
	glm::vec3( 2.0f, 5.0f, -15.0f),
	glm::vec3(-1.5f, -2.2f, -2.5f),
	glm::vec3(-3.8f, -2.0f, -12.3f),
	glm::vec3( 2.4f, -0.4f, -3.5f),
	glm::vec3(-1.7f, 3.0f, -7.5f),
	glm::vec3( 1.3f, -2.0f, -2.5f),
	glm::vec3( 1.5f, 2.0f, -2.5f),
	glm::vec3( 1.5f, 0.2f, -1.5f),
	glm::vec3(-1.3f, 1.0f, -1.5f)
};

unsigned int transformLoc;
unsigned int modelLoc;
unsigned int viewLoc;
unsigned int projectionLoc;

glm::mat4 Window::model;
glm::mat4 Window::view;
glm::mat4 Window::projection;

unsigned int Window::VBO;
unsigned int Window::VAO;
unsigned int Window::EBO;
unsigned int Window::texture;

double Window::lastTime;
int Window::frameCount;
double Window::fps;



glm::vec3 Window::cameraPos;
glm::vec3 Window::cameraFront = glm::vec3(0.0f, 0.0f, -1.0f);
glm::vec3 Window::cameraUp = glm::vec3(0.0f, 1.0f, 0.0f);
glm::vec3 Window::up;

glm::vec3 Window::direction;

float deltaTime = 0.0f; // Time between current frame and last frame
float lastsFrame = 0.0f; // Time of last frame

// shaders

unsigned int Window::shaderProgram;

unsigned int vertexShader;
const char *vertex_shader_source = "#version 330 core\n"
"layout (location = 0) in vec3 aPos;\n"
"layout (location = 1) in vec2 aTexCoord;\n"
"out vec2 TexCoord;\n"
"uniform mat4 transform;\n"
"uniform mat4 model;\n"
"uniform mat4 view;\n"
"uniform mat4 projection;\n"
"void main() {\n"
"   gl_Position = projection * view * model * transform * vec4(aPos, 1.0f);\n"
"   TexCoord = aTexCoord;\n"
"}\0";

unsigned int fragmentShader;
const char *fragment_shader_source = "#version 330 core\n"
"out vec4 FragColor;\n"
"in vec2 TexCoord;\n"
"uniform sampler2D ourTexture;\n"
"void main() {\n"
"   FragColor = texture(ourTexture, TexCoord);\n"
"}\0";

void framebuffer_size_callback(GLFWwindow* window, int width, int height) {
	glViewport(0, 0, width, height);
}

float lastX = 400, lastY = 300;
float yaw = -90.0f;
float pitch = 0.0f;
bool firstMouse;

void mouse_callback(GLFWwindow* window, double xpos, double ypos)
{
	if (firstMouse)
	{
		lastX = xpos;
		lastY = ypos;
		firstMouse = false;
	}
	float xoffset = xpos - lastX;
	float yoffset = lastY - ypos;
	lastX = xpos;
	lastY = ypos;
	float sensitivity = 0.1f;
	xoffset *= sensitivity;
	yoffset *= sensitivity;
	yaw += xoffset;
	pitch += yoffset;
	if(pitch > 89.0f)
		pitch = 89.0f;
	if(pitch < -89.0f)
		pitch = -89.0f;
	glm::vec3 direction;
	direction.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
	direction.y = sin(glm::radians(pitch));
	direction.z = sin(glm::radians(yaw)) * cos(glm::radians(pitch));
	Window::cameraFront = glm::normalize(direction);
}

void Window::processInput() {
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
		glfwSetWindowShouldClose(window, true);

	const float cameraSpeed = 2.5f * deltaTime; // adjust accordingly

	if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
		cameraPos += cameraSpeed * cameraFront;
	if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
		cameraPos -= cameraSpeed * cameraFront;
	if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
		cameraPos -= glm::normalize(glm::cross(cameraFront, cameraUp)) * cameraSpeed;
	if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
		cameraPos += glm::normalize(glm::cross(cameraFront, cameraUp)) * cameraSpeed;
}
int Window::Init(int width, int height, const char *title) {
	if (!glfwInit()) {
		std::cerr << "Failed to initialize GLFW." << std::endl;
		glfwTerminate();
		return -1;
	}

	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	window = glfwCreateWindow(WINDOW_WIDTH, WINDOW_HEIGHT, TITLE, nullptr, nullptr);

	if (window == nullptr) {
		std::cerr << "Failed to create GLFW window." << std::endl;
		glfwTerminate();
		return -1;
	}
	glfwMakeContextCurrent(window);

	// Init Glad
	if (!gladLoadGLLoader(reinterpret_cast<GLADloadproc>(glfwGetProcAddress))) {
		std::cerr << "Failed to initialize GLAD." << std::endl;
		return -1;
	}

	// 1. Set VAO
	glGenVertexArrays(1, &VAO);
	glBindVertexArray(VAO);

	// 2. Set VBO
	glGenBuffers(1, &VBO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertex), vertex, GL_STATIC_DRAW);


	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);

	// Текстурные координаты (атрибут 1 - соответствует шейдеру)
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float))); // 5 or 8
	glEnableVertexAttribArray(1);;

	// Set EBO
	glGenBuffers(1, &EBO);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

	// set shaders and check success compile shaders

	vertexShader = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(vertexShader, 1, &vertex_shader_source, nullptr);
	glCompileShader(vertexShader);

	fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fragmentShader, 1, &fragment_shader_source, nullptr);
	glCompileShader(fragmentShader);

	int success;
	char info_log[512];
	glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);

	if (!success) {
		glGetShaderInfoLog(vertexShader, 512, nullptr, info_log);
		std::cout << "[:ERROR:VERTEX_SHADER:] compilation failed!\n";
	}

	glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);

	if (!success) {
		glGetShaderInfoLog(fragmentShader, 512, nullptr, info_log);
		std::cout << "[:ERROR:FRAGMENT_SHADER:] compilation failed!\n";
	}

	shaderProgram = glCreateProgram();
	glAttachShader(shaderProgram, vertexShader);
	glAttachShader(shaderProgram, fragmentShader);
	glLinkProgram(shaderProgram);

	// 6. cheack shaders
	glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);
	if (!success) {
		glGetProgramInfoLog(shaderProgram, 512, nullptr, info_log);
		std::cout << "[:ERROR:SHADER_PROGRAM:] Linking failed!\n" << info_log << std::endl;
	}

	glUseProgram(shaderProgram);

	glDeleteShader(vertexShader);
	glDeleteShader(fragmentShader);

	// Получаем location uniform-переменных
	transformLoc = glGetUniformLocation(shaderProgram, "transform");
	modelLoc = glGetUniformLocation(shaderProgram, "model");
	viewLoc = glGetUniformLocation(shaderProgram, "view");
	projectionLoc = glGetUniformLocation(shaderProgram, "projection");

	// Настройка матриц
	model = glm::mat4(1.0f);
	model = glm::rotate(model, glm::radians(-55.0f), glm::vec3(1.0f, 0.0f, 0.0f));

	view = glm::mat4(1.0f);
	view = glm::translate(view, glm::vec3(0.0f, 0.0f, -3.0f));

	projection = glm::perspective(glm::radians(45.0f), (float)WINDOW_WIDTH / (float)WINDOW_HEIGHT, 0.1f, 100.0f);

	// Camera

	cameraPos = glm::vec3(0.0f, 0.0f, 3.0f);

	glm::vec3 cameraTarget = glm::vec3(0.0f, 0.0f, 0.0f);
	glm::vec3 cameraDirection = glm::normalize(cameraPos - cameraTarget);

	up = glm::vec3(0.0f, 1.0f, 0.0f);
	glm::vec3 cameraRight = glm::normalize(glm::cross(up, cameraDirection));

	glm::vec3 cameraUp = glm::cross(cameraDirection, cameraRight);

    // texture
	glGenTextures(1, &texture);
	glBindTexture(GL_TEXTURE_2D, texture);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	float borderColor[] = { 1.0f, 1.0f, 0.0f, 1.0f };
	glTexParameterfv(GL_TEXTURE_2D, GL_TEXTURE_BORDER_COLOR, borderColor);

	stbi_set_flip_vertically_on_load(true);

	int texture_width, texture_height, nrChannels;
	unsigned char *data = stbi_load("C:/Users/Asg/CLionProjects/MineCraft2/src/Window/stone_texture.png", &texture_width, &texture_height, &nrChannels, 0);

	if (data) {
		// generating texture
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, texture_width, texture_height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
		glGenerateMipmap(GL_TEXTURE_2D);
	} else {
		std::cout << "[:ERROR:TEXTURE:] Failed to load texture" << std::endl;
	}

	glUniform1i(glGetUniformLocation(shaderProgram, "ourTexture"), 0);

	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

	glfwSetCursorPosCallback(window, mouse_callback);

	stbi_image_free(data);

	glViewport(0, 0, width, height);
	SetFramebufferSize();

	glEnable(GL_DEPTH_TEST);

	return 0;
}

void Window::mainloop() {
	if (Init(WINDOW_WIDTH, WINDOW_HEIGHT, "Minecraft 2") != 0) {
		std::cout << "[:ERROR:] Failed initialization!" << std::endl;
		return; // Инициализация не удалась
	}

	lastTime = glfwGetTime();
	frameCount = 0;
	fps = 0.0;

	while (!glfwWindowShouldClose(window)) {
		// Подсчёт FPS
		double currentTime = glfwGetTime();
		frameCount++;

		// Если прошла секунда - обновляем FPS
		if (currentTime - lastTime >= 1.0) {
			fps = frameCount;
			frameCount = 0;
			lastTime = currentTime;

			// Выводим FPS в заголовок окна
			std::string title = std::string(TITLE) + " | FPS: " + std::to_string((int)fps);
			glfwSetWindowTitle(window, title.c_str());
		}

		float currentFrame = glfwGetTime();
		deltaTime = currentFrame - lastsFrame;
		lastsFrame = currentFrame;

		processInput();

		view = glm::lookAt(cameraPos, cameraPos + cameraFront, cameraUp);

		glClearColor(0.23f, 0.3f, 0.25f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		glUseProgram(shaderProgram);
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, texture);

		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(view));
		glUniformMatrix4fv(projectionLoc, 1, GL_FALSE, glm::value_ptr(projection));

		// Создаем матрицу преобразования для вращения куба
		auto trans = glm::mat4(1.0f);
		trans = glm::translate(trans, glm::vec3(0.0f, 0.0f, 0.0f));
		trans = glm::rotate(trans, (float)glfwGetTime(), glm::vec3(0.5f, 1.0f, 5.0f));

		glUniformMatrix4fv(transformLoc, 1, GL_FALSE, glm::value_ptr(trans));

		glBindVertexArray(VAO);

		for(unsigned int i = 0; i < 10; i++) {
			// Создаем модельную матрицу для каждого куба
			glm::mat4 model = glm::mat4(1.0f);
			model = glm::translate(model, cubePositions[i]);
			model = glm::rotate(model, (float)glfwGetTime(), glm::vec3(0.5f, 1.0f, 0.0f));

			// Передаем модельную матрицу в шейдер
			glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));

			glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);
		}

		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	glDeleteVertexArrays(1, &VAO);
	glDeleteBuffers(1, &VBO);
	glDeleteBuffers(1, &EBO);
	glDeleteTextures(1, &texture);
	glDeleteProgram(shaderProgram);

	glfwTerminate();
}

void Window::SetFramebufferSize() {
	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
}

