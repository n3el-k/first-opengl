#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <iostream>
#include <cassert>

const int WINDOW_WIDTH = 800;
const int WINDOW_HEIGHT = 600;
const char* vertexShaderSource = "#version 330 core\n"
    "layout (location = 0) in vec3 aPos;\n"
    "layout (location = 1) in vec3 aColor;\n"
    "out vec3 outColor;\n"
    "void main()\n"
    "{\n"
    "gl_Position = vec4(aPos.x, aPos.y, aPos.z, 1.0);\n"
    "outColor = aColor;\n"
    "}\0";
const char* fragmentShaderSource = "#version 330 core\n"
    "out vec4 fragColor;\n"
    "in vec3 outColor;\n"
    "void main()\n"
    "{\n"
    "fragColor = vec4(outColor, 1.0);\n"
    "}\n\0";

void framebufferSizeCallback(GLFWwindow* window, int width, int height)
{
    (void)window;
    glViewport(0, 0, width, height);
}

void processInput(GLFWwindow* window)
{
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
    {
        glfwSetWindowShouldClose(window, true);
    }
}

int main()
{
    // GLFW Window Hints
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);

    // Window Object
    GLFWwindow* window = glfwCreateWindow(WINDOW_WIDTH, WINDOW_HEIGHT, "First OpenGL", NULL, NULL);
    assert(window != NULL);
    glfwMakeContextCurrent(window);

    assert(gladLoadGLLoader((GLADloadproc)glfwGetProcAddress));
    int framebufferWidth, framebufferHeight;
    glfwGetFramebufferSize(window, &framebufferWidth, &framebufferHeight);
    glViewport(0, 0, framebufferWidth, framebufferHeight);
    glfwSetFramebufferSizeCallback(window, framebufferSizeCallback);

    // SETUP SHADERS
    unsigned int vertexShader;
    vertexShader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertexShader, 1, &vertexShaderSource, NULL);
    glCompileShader(vertexShader);

    int success;
    char log[512];
    glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);
    if (!success)
    {
        glGetShaderInfoLog(vertexShader, 512, NULL, log);
        std::cout << "VERTEX SHADER DID NOT COMPILE: " << log << std::endl;
        assert(success);
    }

    unsigned int fragShader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragShader, 1, &fragmentShaderSource, NULL);
    glCompileShader(fragShader);

    glGetShaderiv(fragShader, GL_COMPILE_STATUS, &success);

    if (!success)
    {
        glGetShaderInfoLog(fragShader, 512, NULL, log);
        std::cout << "FRAGMENT SHADER DID NOT COMPILE: " << log << std::endl;
        assert(success);
    }

    unsigned int shaderProgram;
    shaderProgram = glCreateProgram();
    glAttachShader(shaderProgram, vertexShader);
    glAttachShader(shaderProgram, fragShader);
    glLinkProgram(shaderProgram);
    glUseProgram(shaderProgram);
    glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);
    if (!success) {
        glGetProgramInfoLog(shaderProgram, 512, NULL, log);
        std::cout << "SHADER PROGRAM DID NOT LINK: " << log << std::endl;
        assert(success);
    }
    glDeleteShader(vertexShader);
    glDeleteShader(fragShader);

    // vertex data
    // float vertices[] = {
    //     -0.5f, -0.5f, 0.0f, // bottom left
    //     0.5f, -0.5f, 0.0f, // bottom right
    //     -0.5f, 0.5f, 0.0f, // top left
    //     0.5f, 0.5f, 0.0f, // top right
    // };

    float vertices[] = {
        // position         // color
        0.5f, -0.5f, 0.0f,  1.0f, 0.0f, 0.0f,   // bottom right
        -0.5f, -0.5f, 0.0f, 0.0f, 1.0f, 0.0f,   // bottom left
        0.0f,  0.5f, 0.0f,  0.0f, 0.0f, 1.0f    // top 
    };

    // unsigned int indices[] = {
    //     0, 1, 2,
    //     1, 2, 3
    // };

    unsigned int VAO;
    unsigned int VBO;
    // unsigned int EBO;

    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    // glGenBuffers(1, &EBO);

    glBindVertexArray(VAO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);  

    // glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    // glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

    // posiiton attribute from vertices
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);  

    // color attribute from vertices
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);

    // glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

    // Main loop
    while(!glfwWindowShouldClose(window))
    {
        processInput(window);
        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        glUseProgram(shaderProgram);
        // float time = glfwGetTime();
        // float greenVal = sin(time) / 2.0f + 0.5f;
        // int colorLoc = glGetUniformLocation(shaderProgram, "setColor");
        // // assert(colorLoc!=-1);
        // glUniform4f(colorLoc, 0.0f, greenVal, 0.0f, 1.0f);

        glBindVertexArray(VAO);
        glDrawArrays(GL_TRIANGLES, 0, 3);
        // glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

        glfwSwapBuffers(window);
        glfwPollEvents();
    }
    
    glDeleteBuffers(1, &VBO);
    glDeleteVertexArrays(1, &VAO);
    glfwTerminate();
    return 0;
}