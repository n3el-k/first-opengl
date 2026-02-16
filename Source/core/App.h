#pragma once
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include "rendering/Renderer.h"
#include "rendering/Camera.h"

class App
{
public:
    App(int width = 1280, int height = 720, const char* title = "OpenGL Application");
    ~App();

    bool init();
    void run();

private:

    void processInput();
    void render();
    void calculateFps();

    // callbacks
    static void framebufferSizeCallback(GLFWwindow* window, int width, int height);
    static void mouseCallback(GLFWwindow* window, double xpos, double ypos);

    // Window
    GLFWwindow* window;
    int windowWidth, windowHeight;
    const char* windowTitle;
    float deltaTime;
    float lastFrame;
    int frameCount;
    float fpsTimer;

    // Mouse
    float lastX, lastY;
    bool firstMoueInput;

    std::unique_ptr<Camera> camera;
    std::unique_ptr<Renderer> renderer;
};