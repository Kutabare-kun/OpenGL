#include <iostream>
#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <fstream>
#include <sstream>
#include <streambuf>
#include <string>

void FramebufferSizeCallback(GLFWwindow* Window, int Width, int Height);
void ProcessInput(GLFWwindow* Window);

std::string LoadShaderSrc(const char* Filename);

unsigned CompileShader(const char* Filename, const unsigned GL_SHADER);

int main()
{
    glfwInit();

    // OpenGL version 3.3
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

#ifdef __APPLE__
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif

    GLFWwindow* Window = glfwCreateWindow(800, 600, "OpenGL", nullptr, nullptr);
    if (!Window)
    {
        // Window not created
        std::cout << "Failed to create GLFW window." << std::endl;
        glfwTerminate();
        return EXIT_FAILURE;
    }

    glfwMakeContextCurrent(Window);

    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        std::cout << "Failed to initialize GLAD" << std::endl;
        glfwTerminate();
        return EXIT_FAILURE;
    }

    glViewport(0, 0, 800, 600);

    glfwSetFramebufferSizeCallback(Window, FramebufferSizeCallback);

    /*
     * Shaders
     */

    // Compile vertex shader
    unsigned int VertexShader = CompileShader("Assets/Vertex_Core.glsl", GL_VERTEX_SHADER);

    // Compile fragment shader
    unsigned int FragmentShaders[2];
    FragmentShaders[0] = CompileShader("Assets/Fragment_Core.glsl", GL_FRAGMENT_SHADER);

    // Compile fragment shader
    FragmentShaders[1] = CompileShader("Assets/Fragment_Core2.glsl", GL_FRAGMENT_SHADER);

    // Create Program
    unsigned int ShaderPrograms[2];

    ShaderPrograms[0] = glCreateProgram();
    ShaderPrograms[1] = glCreateProgram();

    auto AttachShader = [](unsigned ShaderProgram, unsigned VertexShader, unsigned FragmentShader)
    {
        glAttachShader(ShaderProgram, VertexShader);
        glAttachShader(ShaderProgram, FragmentShader);
        glLinkProgram(ShaderProgram);
    };

    AttachShader(ShaderPrograms[0], VertexShader, FragmentShaders[0]);
    AttachShader(ShaderPrograms[1], VertexShader, FragmentShaders[1]);

    // Catch Errors
    auto SuccessShaderPrograms = [](unsigned ShaderProgram)
    {
        int Success{};
    
        glGetProgramiv(ShaderProgram, GL_LINK_STATUS, &Success);
        if (!Success)
        {
            char InfoLog[512];
        
            glGetProgramInfoLog(ShaderProgram, 512, nullptr, InfoLog);
            std::cout << "Linking error: " << InfoLog << std::endl;
        }
    };

    SuccessShaderPrograms(ShaderPrograms[0]);
    SuccessShaderPrograms(ShaderPrograms[1]);

    glDeleteShader(VertexShader);
    glDeleteShader(FragmentShaders[0]);
    glDeleteShader(FragmentShaders[1]);

    // Vertex array
    // float Vertices[] = {
    //      0.5,   0.5f, 0.0f,         // Top Right
    //     -0.5f,  0.5f, 0.0f,         // Top Left
    //     -0.5f, -0.5f, 0.0f,         // Bottom Left
    //      0.5f, -0.5f, 0.0f,         // Bottom Right
    // };
    //
    // unsigned int Indices[] = {
    //     0, 1, 2,                    // First Triangle
    //     2, 3, 0                     // Second Triangle
    // };

    float Vertices[] = {
        // First Triangle
        -0.5f, -0.5f, 0.0f,
        -0.25f, 0.5f, 0.0f,
        -0.1f, -0.5f, 0.0f,

        // Second Triangle
        0.5f, -0.5f, 0.0f,
        0.25f, 0.5f, 0.0f,
        0.1f, -0.5f, 0.0f,
    };

    unsigned int Indices[] = {
        0, 1, 2,
        3, 4, 5
    };

    // VAO, VBO
    unsigned int VAO, VBO, EBO;
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glGenBuffers(1, &EBO);

    // Bind VAO
    glBindVertexArray(VAO);

    // Bind VBO
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(Vertices), Vertices, GL_STATIC_DRAW);

    // Set attribute pointer
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    // Set up EBO
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(Indices), Indices, GL_STATIC_DRAW);
    
    while (!glfwWindowShouldClose(Window))
    {
        // Process input
        ProcessInput(Window);

        // Render
        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        // Draw shapes
        glBindVertexArray(VAO);

        // First Triangle
        glUseProgram(ShaderPrograms[0]);
        glDrawElements(GL_TRIANGLES, 3, GL_UNSIGNED_INT, 0);

        // Second Triangle
        glUseProgram(ShaderPrograms[1]);
        glDrawElements(GL_TRIANGLES, 3, GL_UNSIGNED_INT, (void*)(3 * sizeof(unsigned)));
        
        // Send new frame to Window
        glfwSwapBuffers(Window);
        glfwPollEvents();
    }

    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VAO);
    glDeleteBuffers(1, &EBO);

    glfwTerminate();
    return EXIT_SUCCESS;
}

void FramebufferSizeCallback(GLFWwindow* Window, int Width, int Height)
{
    glViewport(0, 0, Width, Height);
}

void ProcessInput(GLFWwindow* Window)
{
    if (glfwGetKey(Window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(Window, true);
}

std::string LoadShaderSrc(const char* Filename)
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

unsigned CompileShader(const char* Filename, const unsigned GL_SHADER)
{
    int Success;
    char InfoLog[512];
    
    unsigned int MyShader = glCreateShader(GL_SHADER);
    std::string MyShaderSrc = LoadShaderSrc(Filename);
    const GLchar* ShaderLog = MyShaderSrc.c_str();
    glShaderSource(MyShader, 1, &ShaderLog, nullptr);
    glCompileShader(MyShader);

    // Catch Error
    glGetShaderiv(MyShader, GL_COMPILE_STATUS, &Success);
    if (!Success)
    {
        glGetShaderInfoLog(MyShader, 512, nullptr, InfoLog);
        std::cout << "Error with vertex shader compiler: " << InfoLog << std::endl;
    }

    return MyShader;
}
