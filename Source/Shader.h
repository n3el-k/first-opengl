#pragma once
#include <cstdint>
#include <string>
#include <glm/glm.hpp>

class Shader
{
public:
    uint32_t ID;

    Shader(const char* vertexPath, const char* fragmentPath);
    ~Shader();

    void use();

    void setBool(const std::string& name, bool value) const;
    void setInt(const std::string& name, int value) const;
    void setFloat(const std::string& name, float value) const;
    void setMat4(const std::string &name, const glm::mat4 &mat) const;

private:
    void compileVertexShader(const char* vertexPath);
    void compileFragmentShader(const char* fragmentPath);
    void linkProgram();
    uint32_t vertexShader;
    uint32_t fragmentShader;

    // Debugging Info
    void checkCompilationErrors(uint32_t item, const std::string& type);
};