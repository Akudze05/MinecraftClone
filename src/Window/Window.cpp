#include <glad/glad.h>
#include <iostream>
#include <GLFW/glfw3.h>
#include "Window.h"

#define WINDOW_WIDTH 800
#define WINDOW_HEIGHT 600

const char* TITLE = "Minecraft";

GLFWwindow* Window::window = nullptr;

unsigned int VBO = 0;
unsigned int VAO = 0;
unsigned int EBO = 0;

float vertex[] = {
	0.5f, 0.5f, 0.0f,
	0.5f, -0.5f, 0.0f,
	-0.5f, -0.5f, 0.0f,
	-0.5f, 0.5f, 0.0f
};

unsigned int indices[] = {
	0, 1, 3, // for first triangle
	1, 2 ,3 // for second triangles
};

// shaders

unsigned int shaderProgram;

unsigned int vertexShader;
const char *vertex_shader_source = "#version 330 core\n"
"layout (location = 0) in vec3 aPos;\n"
"void main() {\n"
"	gl_Position = vec4(aPos.x, aPos.y, aPos.z, 1.0);\n"
"}\0";

unsigned int fragmentShader;
const char *fragment_shader_source = "#version 330 core\n"
"out vec4 FragColor;\n"
"\n"
"void main() {\n"
"	FragColor = vec4(1.0, 0.5, 0.2, 1.0);\n"
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

	glGenBuffers(1, &VBO);
	glGenVertexArrays(1, &VAO);

	glBindBuffer(GL_ARRAY_BUFFER, VBO); // copy vertex array to buffer for openGL use to
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertex), vertex, GL_STATIC_DRAW);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(float) * 3, (void*)0);

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

	glGetShaderiv(shaderProgram, GL_LINK_STATUS, &success);
	if (!success) {
		glGetShaderInfoLog(shaderProgram, 512, nullptr, info_log);
		std::cout << "[:ERROR:LINK_SHADER:] compilation failed!\n";
	}

	glUseProgram(shaderProgram);
	glBindVertexArray(VAO);

	glDeleteShader(vertexShader);
	glDeleteShader(fragmentShader);

	glGenBuffers(1, &EBO);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, nullptr);
	glEnableVertexAttribArray(0);

	glViewport(0, 0, width, height);
	SetFramebufferSize();

	return 0;
}

void Window::mainloop() {
	Init(WINDOW_WIDTH, WINDOW_HEIGHT, "Minecraft 2");

	while (!glfwWindowShouldClose(window)) {
		processInput();

		// clear screen
		glClearColor(0.23f, 0.3f, 0.25f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);

		glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

		// swap buffers
		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	glfwTerminate();
}

void Window::SetFramebufferSize() {
	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
}

