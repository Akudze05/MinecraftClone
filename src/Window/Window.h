//
// Created by Asg on 09.06.2025.
//

#ifndef WINDOW_H
#define WINDOW_H
#include "GLFW/glfw3.h"


class Window {
public:
	static GLFWwindow* window;
	// static float vertex[];

	static int Init(int width, int height, const char* title);
	static void processInput();
	static void SetFramebufferSize();
	static void mainloop();
};



#endif //WINDOW_H
