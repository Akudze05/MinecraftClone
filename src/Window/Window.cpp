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

unsigned int VBO = 0;
unsigned int VAO = 0;
unsigned int EBO = 0;

float vertex[] = {
	// positions // colors // texture coords
	 0.5f,  0.5f,  0.0f,  1.0f,  0.0f,  0.0f,  1.0f,  1.0f, // top right
	 0.5f, -0.5f,  0.0f,  0.0f,  1.0f,  0.0f,  1.0f,  0.0f, // bottom right
	-0.5f, -0.5f,  0.0f,  0.0f,  0.0f,  1.0f,  0.0f,  0.0f, // bottom left
	-0.5f,  0.5f,  0.0f,  1.0f,  1.0f,  0.0f,  0.0f,  1.0f // top left
};

unsigned int indices[] = {
	0, 1, 3, // for first triangle
	1, 2 ,3 // for second triangles
};

float textCoords[] = {
	1.0f, 1.0f,
	1.0f, 0.0f,
	0.0f, 0.0f,
	0.0f, 1.0f
};

unsigned int texture = 0;

// shaders

unsigned int shaderProgram;

unsigned int vertexShader;
const char *vertex_shader_source = "#version 330 core\n"
"layout (location = 0) in vec3 aPos;\n"
"layout (location = 1) in vec2 aTexCoord;\n"
"out vec2 TexCoord;\n"
"void main() {\n"
"	gl_Position = vec4(aPos.x, aPos.y, aPos.z, 1.0);\n"
"	TexCoord = aTexCoord;\n"
"}\0";

unsigned int fragmentShader;
const char *fragment_shader_source = "#version 330 core\n"
"out vec4 FragColor;\n"
"in vec2 TexCoord;\n"
"uniform sampler2D ourTexture;\n"
"void main() {\n"
"	FragColor = texture(ourTexture, TexCoord);\n"
"}\0";

void framebuffer_size_callback(GLFWwindow* window, int width, int height) {
	glViewport(0, 0, width, height);
}

void Window::processInput() {
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
		glfwSetWindowShouldClose(window, true);

	// float cameraSpeed = 2.5f * deltaTime;  // Фиксированная скорость с учетом deltaTime

	// if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
	// 	camera.Position += cameraSpeed * camera.Front;
	// if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
	// 	camera.Position -= cameraSpeed * camera.Front;
	// if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
	// 	camera.Position -= glm::normalize(glm::cross(camera.Front, camera.Up)) * cameraSpeed;
	// if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
	// 	camera.Position += glm::normalize(glm::cross(camera.Front, camera.Up)) * cameraSpeed;
	// if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS)
	// 	camera.Position += cameraSpeed * camera.Up;
	// if (glfwGetKey(window, GLFW_KEY_LEFT_CONTROL) == GLFW_PRESS)
	// 	camera.Position -= cameraSpeed * camera.Up;
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


	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);

	// Текстурные координаты (атрибут 1 - соответствует шейдеру)
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));
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

	stbi_image_free(data);

	glViewport(0, 0, width, height);
	SetFramebufferSize();

	return 0;
}

void Window::mainloop() {
	Init(WINDOW_WIDTH, WINDOW_HEIGHT, "Minecraft 2");

	while (!glfwWindowShouldClose(window)) {
		processInput();

		glClearColor(0.23f, 0.3f, 0.25f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);

		glUseProgram(shaderProgram);
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, texture);

		glBindVertexArray(VAO);

		glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	glfwTerminate();
}

void Window::SetFramebufferSize() {
	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
}

