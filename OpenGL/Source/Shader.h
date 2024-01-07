#pragma once

#include <glad/glad.h>

#include <string>
#include <fstream>
#include <sstream>
#include <iostream>

#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>

class Shader
{
public:
    unsigned int ID;

    Shader(const char* VertexShaderPath, const char* FragmentShaderPtr);

    void Activate();

    // Utility Functions
    std::string LoadShaderSrc(const char* Filename);
    GLuint CompileShader(const char* Filepath, GLenum Type);

    // Uniform Functions
    void SetMat4(const std::string& Name, glm::mat4 Value);
};
