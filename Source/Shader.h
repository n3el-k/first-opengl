#pragma once
#include <cstdint>
#include <string>

class Shader
{
public:
    uint32_t ID;

    Shader(const char* vertexPath, const char* fragmentPath);

    void use();

    void setBool(const std::string& name, bool value) const;
    void setInt(const std::string& name, int value) const;
    void setFloat(const std::string& name, float value) const;

private:
    void compileVertexShader(const char* vertexPath);
    void compileFragmentShader(const char* fragmentPath);
    void linkProgram();
    uint32_t vertexShader;
    uint32_t fragmentShader;

    // Debugging Info
    void checkCompilationErrors(uint32_t item, const std::string& type);
};