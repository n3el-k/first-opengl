#include "Renderer.h"
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/glm.hpp>
#include <GLFW/glfw3.h>

Renderer::Renderer()
{

}

Renderer::~Renderer()
{
    if (VAO) glDeleteVertexArrays(1, &VAO);
    if (VBO) glDeleteBuffers(1, &VBO);
    if (EBO) glDeleteBuffers(1, &EBO);
}

bool Renderer::init()
{
    shader = std::make_unique<Shader>("Assets/Shaders/TriangleVertexShader.vs", "Assets/Shaders/TriangleFragmentShader.fs");
    texture1 = std::make_unique<Texture>("Assets/Textures/brick.jpg", GL_RGB);
    texture2 = std::make_unique<Texture>("Assets/Textures/smiley.png", GL_RGBA, true);
    glUniform1i(glGetUniformLocation(shader->ID, "texture1"), 0); 
    shader->use();
    shader->setInt("texture2", 1);
    setupGeometryAndBuffers();
    return true;
}

void Renderer::render(const Camera &camera, int width, int height)
{
    clear(glm::vec3(0.15f, 0.15f, 0.2f));
    shader->use();
    texture1->bind(0);
    texture2->bind(1);
    glBindVertexArray(VAO);

    glm::mat4 viewMatrix = camera.getViewMatrix();
    glm::mat4 projection = glm::perspective(glm::radians(45.0f), (float)width/height, 0.1f, 100.0f);

    glUniform1i(glGetUniformLocation(shader->ID, "texture1"), 0); 
    shader->setInt("texture2", 1);

    uint32_t viewLoc = glGetUniformLocation(shader->ID, "view");
    uint32_t projectionLoc = glGetUniformLocation(shader->ID, "projection");

    glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(viewMatrix));
    glUniformMatrix4fv(projectionLoc, 1, GL_FALSE, glm::value_ptr(projection));

    glm::mat4 model = glm::mat4(1.0f);

    for(uint32_t i = 0; i < 10; i++)
    {
        model = glm::mat4(1.0f);
        model = glm::translate(model, cubePos[i]);

        float angle = glfwGetTime() * 50.0f + (20.0f * i);
        model = glm::rotate(model, glm::radians(angle), glm::vec3(0.5f, 1.0f, 0.0f));
        shader->setMat4("model", model);
        glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);
    }
}

void Renderer::clear(const glm::vec3 &color)
{
    glClearColor(color.r, color.g, color.b, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

void Renderer::setupGeometryAndBuffers()
{
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

    cubePos[0] = glm::vec3(-2.0f, 0.0f, -20.0f);
    cubePos[1] = glm::vec3(3.5f, 1.5f, -7.0f);
    cubePos[2] = glm::vec3(-3.5f, -1.5f, -8.0f);
    cubePos[3] = glm::vec3(2.5f, -1.0f, -6.0f);
    cubePos[4] = glm::vec3(-2.5f, 1.0f, -9.0f);
    cubePos[5] = glm::vec3(1.0f, 2.0f, -5.5f);
    cubePos[6] = glm::vec3(4.0f, 0.0f, -10.0f);
    cubePos[7] = glm::vec3(-4.0f, 0.5f, -6.5f);
    cubePos[8] = glm::vec3(1.5f, -2.0f, -7.5f);
    cubePos[9] = glm::vec3(-1.5f, -0.5f, -45.0f);
}
