#pragma once

#include <glad/glad.h>
#include <glm/glm.hpp>
#include "Shader.h"
#include "Texture.h"
#include "Camera.h"

class Renderer
{
public:
    Renderer();
    ~Renderer();

    bool init();
    void render(const Camera& camera, int width, int height);

private:

    void clear(const glm::vec3& color = glm::vec3(0.1f, 0.1f, 0.1f));
    void setupGeometryAndBuffers();

    std::unique_ptr<Shader> shader;
    std::unique_ptr<Texture> texture1;
    std::unique_ptr<Texture> texture2;
    uint32_t VAO, VBO, EBO;

    glm::vec3 cubePos[10];
};