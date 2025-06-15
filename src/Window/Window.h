
#ifndef WINDOW_H
#define WINDOW_H
#include <C:\Users\Asg\CLionProjects\MineEngine\dependencies\glm\glm.hpp> // SET YOUR PATH TO GLM
#include <C:\Users\Asg\CLionProjects\MineEngine\dependencies\glm/gtc/matrix_transform.hpp>
#include <C:\Users\Asg\CLionProjects\MineEngine\dependencies\glm/gtc/type_ptr.hpp>
#include <vector>
#include "GLFW/glfw3.h"


class Window {
public:
	static GLFWwindow* window;
	static unsigned int VBO, VAO, EBO;
	static unsigned int shaderProgram;
	static unsigned int texture;
	static glm::mat4 model, view, projection;
	static unsigned int transformLoc;
	static double lastTime;
	static int frameCount;
	static double fps;
	static glm::vec3 cameraPos;
	static glm::vec3 cameraFront;
	static glm::vec3 cameraUp;
	static glm::vec3 up;
	static glm::vec3 direction;
	// static float vertex[];

	static int Init(int width, int height, const char* title);
	static void processInput();
	static void SetFramebufferSize();
	static void mainloop();
};



#endif //WINDOW_H
