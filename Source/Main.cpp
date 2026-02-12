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
    glEnable(GL_DEPTH_TEST);
    Shader shader("Assets/Shaders/TriangleVertexShader.vs", "Assets/Shaders/TriangleFragmentShader.fs");

    float vertices[] = {

        // (x, y, z, u, v) :)

        // Front face
        -0.5f, -0.5f, 0.5f, 0.0f, 0.0f,
        -0.5f, 0.5f, 0.5f, 0.0f, 1.0f,
        0.5f, 0.5f, 0.5f, 1.0f, 1.0f,
        0.5f, -0.5f, 0.5f, 1.0f, 0.0f,

        // Back face
        -0.5f, -0.5f, -0.5f, 0.0f, 0.0f,
        -0.5f, 0.5f, -0.5f, 0.0f, 1.0f,
        0.5f, 0.5f, -0.5f, 1.0f, 1.0f,
        0.5f, -0.5f, -0.5f, 1.0f, 0.0f,

        // Top face
        -0.5f, 0.5f, 0.5f, 0.0f, 0.0f,
        -0.5f, 0.5f, -0.5f, 0.0f, 1.0f,
        0.5f, 0.5f, -0.5f, 1.0f, 1.0f,
        0.5f, 0.5f, 0.5f, 1.0f, 0.0f,

        // Bottom face
        -0.5f, -0.5f, 0.5f, 0.0f, 0.0f,
        -0.5f, -0.5f, -0.5f, 0.0f, 1.0f,
        0.5f, -0.5f, -0.5f, 1.0f, 1.0f,
        0.5f, -0.5f, 0.5f, 1.0f, 0.0f,

        // Left face
        -0.5f, 0.5f, 0.5f, 1.0f, 1.0f,
        -0.5f, -0.5f, 0.5f, 1.0f, 0.0f,
        -0.5f, -0.5f, -0.5f, 0.0f, 0.0f,
        -0.5f, 0.5f, -0.5f, 0.0f, 1.0f,

        // Right face
        0.5f, 0.5f, 0.5f, 1.0f, 1.0f,
        0.5f, -0.5f, 0.5f, 1.0f, 0.0f,
        0.5f, -0.5f, -0.5f, 0.0f, 0.0f,
        0.5f, 0.5f, -0.5f, 0.0f, 1.0f,
    };

    uint32_t indices[] = {
        // Front
        0, 1, 3,
        1, 2, 3,

        // Back
        4, 5, 7,
        5, 6, 7,

        // Top
        8, 9, 11,
        9, 10, 11,

        // Bottom
        12, 13, 15,
        13, 14, 15,

        // Left
        16, 17, 18,
        16, 18, 19,

        // Right
        20, 21, 22,
        20, 22, 23,
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

    /*CAMERA MATRICES*/
    glm::mat4 model = glm::mat4(1.0f);
    model = glm::rotate(model, glm::radians(-55.0f), glm::vec3(1.0f, 0.0f, 0.0f));

    glm::mat4 view = glm::mat4(1.0f);
    view = glm::translate(view, glm::vec3(0.0f, 0.0f, -3.0f));

    glm::mat4 projection = glm::perspective(glm::radians(45.0f), (float)WINDOW_WIDTH/WINDOW_HEIGHT, 0.1f, 100.0f);

    glm::vec3 cubePos[] = {
        glm::vec3(-2.0f,  0.0f, -20.f),
        glm::vec3( 3.5f,  1.5f, -7.0f),
        glm::vec3(-3.5f, -1.5f, -8.0f),
        glm::vec3( 2.5f, -1.0f, -6.0f),
        glm::vec3(-2.5f,  1.0f, -9.0f),
        glm::vec3( 1.0f,  2.0f, -5.5f),
        glm::vec3( 4.0f,  0.0f, -10.0f),
        glm::vec3(-4.0f,  0.5f, -6.5f),
        glm::vec3( 1.5f, -2.0f, -7.5f),
        glm::vec3(-1.5f, -0.5f, -45.0f),
    };

    shader.use();
    glUniform1i(glGetUniformLocation(shader.ID, "texture1"), 0); 
    shader.setInt("texture2", 1);

    // Main loop
    while(!glfwWindowShouldClose(window))
    {
        processInput(window);
        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        texture1.bind(0);
        texture2.bind(1);
        shader.use();
        glBindVertexArray(VAO);

        uint32_t viewLoc = glGetUniformLocation(shader.ID, "view");
        uint32_t projectionLoc = glGetUniformLocation(shader.ID, "projection");

        glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(view));
        glUniformMatrix4fv(projectionLoc, 1, GL_FALSE, glm::value_ptr(projection));

        for(uint32_t i = 0; i < 10; i++)
        {
            model = glm::mat4(1.0f);
            model = glm::translate(model, cubePos[i]);

            float angle = glfwGetTime() * 50.0f + (20.0f * i);
            model = glm::rotate(model, glm::radians(angle), glm::vec3(0.5f, 1.0f, 0.0f));
            shader.setMat4("model", model);
            glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);
        }

        glfwSwapBuffers(window);
        glfwPollEvents();
    }
    
    glDeleteBuffers(1, &VBO);
    glDeleteVertexArrays(1, &VAO);
    glfwTerminate();
    return 0;
}