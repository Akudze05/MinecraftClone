
#ifndef WINDOW_H
#define WINDOW_H

#include "Camera.h"

class GLFWwindow;

class Window {
    static GLFWwindow* window;
    static float vertex[];
public:
    static Camera camera;
    static unsigned int shaderProgram;
    static unsigned int VAO, VBO;
    static glm::mat4 projection;
    static unsigned int projLoc;
    static unsigned int viewLoc;
    static float lastX, lastY;
    static bool firstMouse;
    static unsigned int texture;

    static int initialize(int width, int height, const char* title);
    static void terminate();
    static void swapBuffers();
    static void SetFramebufferSize();
    static void processInput();
    static bool isShouldClose();
    static void mainloop();
};



#endif //WINDOW_H
