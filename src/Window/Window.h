
#ifndef WINDOW_H
#define WINDOW_H

class GLFWwindow;

class Window {
    static GLFWwindow* window;
    static float vertex[];
public:

    static int initialize(int width, int height, const char* title);
    static void terminate();
    static void swapBuffers();
    static void SetFramebufferSize();
    static void processInput();
    static bool isShouldClose();
    static void mainloop();
};



#endif //WINDOW_H
