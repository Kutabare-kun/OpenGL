#include <iostream>
#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <fstream>
#include <sstream>
#include <streambuf>
#include <string>

#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include "Shader.h"

void FramebufferSizeCallback(GLFWwindow* Window, int Width, int Height);
void ProcessInput(GLFWwindow* Window);

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

    Shader MyShaders[] = {
        Shader("Assets/Vertex_Core.glsl", "Assets/Fragment_Core.glsl"),
        Shader("Assets/Vertex_Core.glsl", "Assets/Fragment_Core2.glsl"),
    };

    float Vertices[] = {
        // Positions            Colors
        -0.25f, -0.5f, 0.0f,    1.0f, 1.0f, 0.5f,       // Bottom Left
         0.15f,  0.0f, 0.0f,    0.5f, 1.0f, 0.75f,      // Top Left
         0.0f,   0.5f, 0.0f,    0.6f, 1.0f, 0.2f,       // Bottom Right
         0.5f,  -0.4f, 0.0f,    1.0f, 0.2f, 1.0f        // Top Right
    };

    unsigned int Indices[] = {
        0, 1, 2,
        3, 1, 2
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

    // Put index array in EBO
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(Indices), Indices, GL_STATIC_DRAW);

    // Set attribute pointer

    // Positions
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    // Colors
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);

    glm::mat4 trans = glm::mat4{1.0f};
    trans = glm::rotate(trans, glm::radians(45.0f), glm::vec3(0.0f, 0.0f, 1.0f));
    MyShaders[0].Activate();
    MyShaders[0].SetMat4("transform", trans);

    glm::mat4 trans2 = glm::mat4{1.0f};
    trans2 = glm::scale(trans2, glm::vec3(1.5f));
    trans2 = glm::rotate(trans2, glm::radians(15.0f), glm::vec3(0.0f, 0.0f, 1.0f));
    MyShaders[1].Activate();
    MyShaders[1].SetMat4("transform", trans2);
    
    while (!glfwWindowShouldClose(Window))
    {
        // Process input
        ProcessInput(Window);

        // Render
        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        trans = glm::rotate(trans, glm::radians((float)glfwGetTime() / 100.0f), glm::vec3(0.0f, 0.0f, 1.0f));
        MyShaders[0].Activate();
        MyShaders[0].SetMat4("transform", trans);

        // Draw shapes
        glBindVertexArray(VAO);

        // First Triangle
        MyShaders[0].Activate();
        glDrawElements(GL_TRIANGLES, 3, GL_UNSIGNED_INT, 0);

        trans2 = glm::rotate(trans2, glm::radians((float)glfwGetTime() / -100.0f), glm::vec3(0.0f, 0.0f, 1.0f));
        MyShaders[1].Activate();
        MyShaders[1].SetMat4("transform", trans2);

        MyShaders[1].Activate();
        glDrawElements(GL_TRIANGLES, 3, GL_UNSIGNED_INT, (void*)(3 * sizeof(GLuint)));

        // Second Triangle
        // glUseProgram(ShaderPrograms[1]);
        // glDrawElements(GL_TRIANGLES, 3, GL_UNSIGNED_INT, (void*)(3 * sizeof(float)));
        glBindVertexArray(0);
        
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
