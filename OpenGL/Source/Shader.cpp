#include "Shader.h"

Shader::Shader(const char* VertexShaderPath, const char* FragmentShaderPtr)
{
    int Success;
    char InfoLog[512];

    GLuint VertexShader = CompileShader(VertexShaderPath, GL_VERTEX_SHADER);
    GLuint FragmentShader = CompileShader(FragmentShaderPtr, GL_FRAGMENT_SHADER);

    ID = glCreateProgram();
    glAttachShader(ID, VertexShader);
    glAttachShader(ID, FragmentShader);
    glLinkProgram(ID);

    glGetProgramiv(ID, GL_LINK_STATUS, &Success);
    if (!Success)
    {
        glGetProgramInfoLog(ID, 512, nullptr, InfoLog);
        std::cout << "[Linking Error] " << InfoLog << std::endl;
    }

    glDeleteShader(VertexShader);
    glDeleteShader(FragmentShader);
}

void Shader::Activate()
{
    glUseProgram(ID);
}

std::string Shader::LoadShaderSrc(const char* Filename)
{
    std::ifstream MyFile;
    std::stringstream Buffer;

    std::string Ret{};
    
    MyFile.open(Filename);

    if (MyFile.is_open())
    {
        Buffer << MyFile.rdbuf();
        Ret = Buffer.str();
    }
    else
    {
        std::cout << "Could not open " << Filename << std::endl; 
    }

    MyFile.close();

    return Ret;
}

GLuint Shader::CompileShader(const char* Filepath, GLenum Type)
{
    int Success;
    char InfoLog[512];

    GLuint Ret = glCreateShader(Type);
    
    std::string MyShaderSrc = LoadShaderSrc(Filepath);
    const GLchar* ShaderLog = MyShaderSrc.c_str();
    glShaderSource(Ret, 1, &ShaderLog, nullptr);
    glCompileShader(Ret);

    // Catch Error
    glGetShaderiv(Ret, GL_COMPILE_STATUS, &Success);
    if (!Success)
    {
        glGetShaderInfoLog(Ret, 512, nullptr, InfoLog);
        std::cout << "Error with vertex shader compiler: " << InfoLog << std::endl;
    }

    return Ret;
}

void Shader::SetMat4(const std::string& Name, glm::mat4 Value)
{
    glUniformMatrix4fv(glGetUniformLocation(ID, Name.c_str()), 1, GL_FALSE, glm::value_ptr(Value));
}
