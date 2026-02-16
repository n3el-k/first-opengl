#include "App.h"
#include <GLFW/glfw3.h>
#include <string>

App::App(int width, int height, const char* title)
{
    window = nullptr;
    windowWidth = width;
    windowHeight = height;
    windowTitle = title;
    lastX = width/2;
    lastY = height/2;
    firstMoueInput = true;
    deltaTime = 0.0f;
    lastFrame = 0.0f;
    frameCount = 0;
    fpsTimer = 0.0f;
}

App::~App()
{
    renderer.reset();
    camera.reset();

    if (window)
    {
        glfwDestroyWindow(window);
        glfwTerminate();
    }
}

bool App::init()
{
    // GLFW Window Hints
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);

    // Window Object
    window = glfwCreateWindow(windowWidth, windowHeight, windowTitle, NULL, NULL);
    assert(window != NULL);
    glfwMakeContextCurrent(window);
    glfwSetWindowUserPointer(window, this);
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED); 

    assert(gladLoadGLLoader((GLADloadproc)glfwGetProcAddress));
    int framebufferWidth, framebufferHeight;
    glfwGetFramebufferSize(window, &framebufferWidth, &framebufferHeight);
    glViewport(0, 0, framebufferWidth, framebufferHeight);
    glfwSetFramebufferSizeCallback(window, framebufferSizeCallback);
    glEnable(GL_DEPTH_TEST);

    // callbacks

    camera = std::make_unique<Camera>(glm::vec3(0.0f, 0.0f, 3.0f));
    renderer = std::make_unique<Renderer>();
    renderer->init();

    glfwSetCursorPosCallback(window, mouseCallback); 

    return true;
}

void App::calculateFps()
{
    deltaTime = glfwGetTime() - lastFrame;
    lastFrame = glfwGetTime();
    frameCount++;
    fpsTimer += deltaTime;
    
    if (fpsTimer >= 1.0f)
    {
        int fps = frameCount / fpsTimer;
        std::string title = std::string(windowTitle) + " " + std::to_string(fps) + " FPS";
        glfwSetWindowTitle(window, title.c_str());

        // Reset
        frameCount = 0;
        fpsTimer = 0;
    }
}

void App::run()
{
    while (!glfwWindowShouldClose(window))
    {
        calculateFps();
        processInput();
        render();
        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    render();
}

void App::processInput()
{
    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
    {
        camera->processKeyboard(FORWARD, deltaTime);
    }
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
    {
        camera->processKeyboard(BACKWARD, deltaTime);
    }
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
    {
        camera->processKeyboard(LEFT, deltaTime);
    }
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
    {
        camera->processKeyboard(RIGHT, deltaTime);
    }
    if (glfwGetKey(window, GLFW_KEY_E) == GLFW_PRESS)
    {
        camera->processKeyboard(UP, deltaTime);
    }
    if (glfwGetKey(window, GLFW_KEY_Q) == GLFW_PRESS)
    {
        camera->processKeyboard(DOWN, deltaTime);
    }
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
    {
        glfwSetWindowShouldClose(window, true);
    }
}

void App::render()
{
    renderer->render(*camera, windowWidth, windowHeight);
}

void App::framebufferSizeCallback(GLFWwindow *window, int width, int height)
{
    (void)window; // to supress warning
    glViewport(0, 0, width, height);
    App* app = static_cast<App*>(glfwGetWindowUserPointer(window));
    app->windowWidth = width;
    app->windowHeight = height;
}

void App::mouseCallback(GLFWwindow *window, double xpos, double ypos)
{
    (void)window;
    App* app = static_cast<App*>(glfwGetWindowUserPointer(window));

    if (app->firstMoueInput)
    {
        app->lastX = xpos;
        app->lastY = ypos;
        app->firstMoueInput = false;
    }

    float xOff = xpos - app->lastX;
    float yOff = -(ypos - app->lastY);
    app->lastX = xpos;
    app->lastY = ypos;
    app->camera->processMouse(xOff, yOff);
}
