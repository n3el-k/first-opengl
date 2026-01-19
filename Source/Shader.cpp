#include "Shader.h"
#include <glad/glad.h>
#include <fstream>
#include <sstream>
#include <iostream>
#include <cassert>

Shader::Shader(const char *vertexPath, const char *fragmentPath)
{
    compileVertexShader(vertexPath);
    compileFragmentShader(fragmentPath);
    linkProgram();
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

// PRIVATE FUNCTIONS

void Shader::compileVertexShader(const char* vertexPath)
{
    std::string vertexCode;
    std::ifstream vertexSourceFile;
    vertexSourceFile.exceptions (std::ifstream::failbit | std::ifstream::badbit);

    try
    {
        vertexSourceFile.open(vertexPath);
        std::stringstream vertexSS;
        vertexSS << vertexSourceFile.rdbuf();
        vertexSourceFile.close();
        vertexCode = vertexSS.str();
        
    }
    catch(std::ifstream::failure error)
    {
        std::cout << "SHADER FILE DID NOT SUCCESFULLY OPEN" << std::endl;
    }
    const char* c_strVertexCode = vertexCode.c_str();

    vertexShader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertexShader, 1, &c_strVertexCode, NULL);
    glCompileShader(vertexShader);

    checkCompilationErrors(vertexShader, "SHADER");
}

void Shader::compileFragmentShader(const char* fragmentPath)
{
    std::string fragmentCode;
    std::ifstream fragmentSourceFile;

    fragmentSourceFile.exceptions (std::ifstream::failbit | std::ifstream::badbit);

    try
    {
        fragmentSourceFile.open(fragmentPath);
        std::stringstream vertexSS, fragmentSS;

        fragmentSS << fragmentSourceFile.rdbuf();
        fragmentSourceFile.close();

        fragmentCode = fragmentSS.str();
    }
    catch(std::ifstream::failure error)
    {
        std::cout << "SHADER FILE DID NOT SUCCESFULLY OPEN" << std::endl;
    }

    const char* c_strFragmentCode = fragmentCode.c_str();

    // Fragment:
    fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragmentShader, 1, &c_strFragmentCode, NULL);
    glCompileShader(fragmentShader);

    checkCompilationErrors(fragmentShader, "SHADER");
}

void Shader::linkProgram()
{
    ID = glCreateProgram();
    glAttachShader(ID, vertexShader);
    glAttachShader(ID, fragmentShader);
    glLinkProgram(ID);

    checkCompilationErrors(ID, "PROGRAM");

    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);
}

void Shader::checkCompilationErrors(uint32_t item, const std::string& type)
{
    int success;
    char log[512];
    if (type != "PROGRAM")
    {
        // Shader
        glGetShaderiv(item, GL_COMPILE_STATUS, &success);
        if (!success)
        {
            glGetShaderInfoLog(item, 512, NULL, log);
            std::cerr << "SHADER PROGRAM DID NOT LINK: " << log << std::endl;
            assert(false);
        }
    }

    // Else it is a program check:
    else
    {
        glGetProgramiv(item, GL_LINK_STATUS, &success);
        if (!success)
        {
            glGetProgramInfoLog(item, 512, NULL, log);
            std::cerr << "SHADER PROGRAM DID NOT LINK: " << log << std::endl;
            assert(false);
        }
    }
}
