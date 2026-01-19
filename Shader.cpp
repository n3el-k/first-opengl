#include "Shader.h"
#include <glad/glad.h>
#include <fstream>
#include <sstream>
#include <iostream>
#include <cassert>

Shader::Shader(const char *vertexPath, const char *fragmentPath)
{
    // Retrieve the shader source
    std::string vertexCode;
    std::string fragmentCode;
    std::ifstream vertexSourceFile;
    std::ifstream fragmentSourceFile;

    vertexSourceFile.exceptions (std::ifstream::failbit | std::ifstream::badbit);
    fragmentSourceFile.exceptions (std::ifstream::failbit | std::ifstream::badbit);

    try
    {
        vertexSourceFile.open(vertexPath);
        fragmentSourceFile.open(fragmentPath);
        std::stringstream vertexSS, fragmentSS;

        vertexSS << vertexSourceFile.rdbuf();
        fragmentSS << fragmentSourceFile.rdbuf();
        vertexSourceFile.close();
        fragmentSourceFile.close();

        vertexCode = vertexSS.str();
        fragmentCode = fragmentSS.str();
    }
    catch(std::ifstream::failure error)
    {
        std::cout << "SHADER FILE DID NOT SUCCESFULLY OPEN" << std::endl;
    }

    const char* c_strVertexCode = vertexCode.c_str();
    const char* c_strFragmentCode = fragmentCode.c_str();

    // Compile Shaders

    uint32_t vertexShader, fragmentShader;
    // Debug Info
    int success;
    char log[512];

    // Vertex:
    vertexShader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertexShader, 1, &c_strVertexCode, NULL);
    glCompileShader(vertexShader);

    // Success?
    glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);
    if (!success)
    {
        glGetShaderInfoLog(vertexShader, 512, NULL, log);
        std::cerr << "VERTEX SHADER DID NOT COMPILE: " << log << std::endl;
        assert(false);
    }

    // Fragment:
    fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragmentShader, 1, &c_strFragmentCode, NULL);
    glCompileShader(fragmentShader);
    
    glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);
    if (!success)
    {
        glGetShaderInfoLog(fragmentShader, 512, NULL, log);
        std::cerr << "FRAGEMENT SHADER DID NOT COMPILE: " << log << std::endl;
        assert(false);
    }

    ID = glCreateProgram();
    glAttachShader(ID, vertexShader);
    glAttachShader(ID, fragmentShader);
    glLinkProgram(ID);

    glGetProgramiv(ID, GL_LINK_STATUS, &success);
    if (!success)
    {
        glGetProgramInfoLog(ID, 512, NULL, log);
        std::cerr << "SHADER PROGRAM DID NOT LINK: " << log << std::endl;
        assert(false);
    }

    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);
}

void Shader::use()
{
    glUseProgram(ID);
}

void Shader::setBool(const std::string &name, bool value) const
{
    glUniform1i(glGetUniformLocation(ID, name.c_str()), (int)value);
}

void Shader::setInt(const std::string &name, int value) const
{
    glUniform1i(glGetUniformLocation(ID, name.c_str()), value);
}

void Shader::setFloat(const std::string &name, float value) const
{
    glUniform1f(glGetUniformLocation(ID, name.c_str()), value);
}
