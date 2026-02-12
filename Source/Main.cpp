#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include "Shader.h"
#include "Texture.h"
#include <iostream>
#include <cassert>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

const int WINDOW_WIDTH = 800;
const int WINDOW_HEIGHT = 600;

void framebufferSizeCallback(GLFWwindow* window, int width, int height)
{
    (void)window; // to supress warning
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
    
    Shader shader("Assets/Shaders/TriangleVertexShader.vs", "Assets/Shaders/TriangleFragmentShader.fs");

    float vertices[] = {
        // pos (3)          // tex (2)
        0.5f,  0.5f, 0.0f,  1.0f, 1.0f,   // top right
        -0.5f,  0.5f, 0.0f,  0.0f, 1.0f,   // top left 
        0.5f, -0.5f, 0.0f,  1.0f, 0.0f,   // bottom right
        -0.5f, -0.5f, 0.0f,  0.0f, 0.0f,   // bottom left
    };

    uint32_t indices[] = {
        0, 1, 2,
        1, 2, 3,
    };

    Texture texture1("Assets/Textures/container.jpg", GL_RGB);
    Texture texture2("Assets/Textures/smiley.png", GL_RGBA, true);

    unsigned int VAO;
    unsigned int VBO;
    unsigned int EBO;

    glGenBuffers(1, &EBO);
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);

    glBindVertexArray(VAO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);  

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW); 

    // posiiton attribute from vertices
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);  

    //texture attribute from vertrices
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);

    shader.use();
    glUniform1i(glGetUniformLocation(shader.ID, "texture1"), 0); 
    shader.setInt("texture2", 1);

    // Main loop
    while(!glfwWindowShouldClose(window))
    {
        processInput(window);
        // glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        texture1.bind(0);
        texture2.bind(1);
        shader.use();
        glBindVertexArray(VAO);

        // First container
        glm::mat4 transformations = glm::mat4(1.0f);
        transformations = glm::translate(transformations, glm::vec3(0.5f, -0.5f, 0.0f));
        transformations = glm::rotate(transformations, (float)glfwGetTime(), glm::vec3(0.0f, 0.0f, 1.0f));
        uint32_t transformLoc1 = glGetUniformLocation(shader.ID, "transform");
        glUniformMatrix4fv(transformLoc1, 1, GL_FALSE, glm::value_ptr(transformations));
        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

        // Second container
        transformations = glm::mat4(1.0f);
        transformations = glm::translate(transformations, glm::vec3(-0.5f, 0.5f, 0.0f));

        // Set scale so there is no negative value, and clamp it to prevent disappearing (range [0.5, 1.5]);
        float scale = (float)sin(glfwGetTime()) * 0.5f + 1.0f;
        transformations = glm::scale(transformations, glm::vec3(scale, scale, 0.0f));
        transformations = glm::rotate(transformations, -(float)glfwGetTime(), glm::vec3(0.0f, 0.0f, 1.0f));

        uint32_t transformLoc2 = glGetUniformLocation(shader.ID, "transform");
        glUniformMatrix4fv(transformLoc2, 1, GL_FALSE, glm::value_ptr(transformations));
        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
    
        glfwSwapBuffers(window);
        glfwPollEvents();
    }
    
    glDeleteBuffers(1, &VBO);
    glDeleteVertexArrays(1, &VAO);
    glfwTerminate();
    return 0;
}