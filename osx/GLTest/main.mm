#import <Foundation/Foundation.h>

#include <GLEW/glew.h>
#include <GLFW/GLFW.h>
#include <GLM/glm.hpp>

#include <iostream>
#include <vector>
#include <memory>

#include "Shader.h"
#include "Program.h"

const glm::vec2 SCREEN_SIZE(800, 600);
GLuint gVAO = 0;
GLuint gVBO = 0;

// returns the full path to the file `fileName` in the resources directory of the app bundle
static std::string ResourcePath(std::string fileName) {
    NSString* fname = [NSString stringWithCString:fileName.c_str() encoding:NSUTF8StringEncoding];
    NSString* path = [[[NSBundle mainBundle] resourcePath] stringByAppendingPathComponent:fname];
    return std::string([path cStringUsingEncoding:NSUTF8StringEncoding]);
}

std::shared_ptr<Program> loadShaders()
{
    std::vector<std::shared_ptr<Shader>> shaders;
    
    shaders.push_back(Shader::shaderFromFile(ResourcePath("test.vert"), GL_VERTEX_SHADER));
    shaders.push_back(Shader::shaderFromFile(ResourcePath("test.frag"), GL_FRAGMENT_SHADER));
    
    return std::make_shared<Program>(shaders);
}

void loadTriangle(std::shared_ptr<Program> gProgram)
{
    
    // make and bind the VAO
    glGenVertexArrays(1, &gVAO);
    glBindVertexArray(gVAO);
    
    // make and bind the VBO
    glGenBuffers(1, &gVBO);
    glBindBuffer(GL_ARRAY_BUFFER, gVBO);
    
    // Put the three triangle verticies into the VBO
    GLfloat vertexData[] = {
        //  X     Y     Z
        0.0f, 0.8f, 0.0f,
        -0.8f,-0.8f, 0.0f,
        0.8f,-0.8f, 0.0f,
    };
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertexData), vertexData, GL_STATIC_DRAW);
    
    // connect the xyz to the "vert" attribute of the vertex shader
    glEnableVertexAttribArray(gProgram->attrib("vert"));
    glVertexAttribPointer(gProgram->attrib("vert"), 3, GL_FLOAT, GL_FALSE, 0, NULL);
    
    // unbind the VBO and VAO
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
}

void render(std::shared_ptr<Program> p)
{
    glClearColor(0, 0, 0, 1); // black
    glClear(GL_COLOR_BUFFER_BIT);
    
    glUseProgram(p->object());
    
    glBindVertexArray(gVAO);
    glDrawArrays(GL_TRIANGLES, 0, 3);
    glBindVertexArray(0);
    
    glUseProgram(0);

    glfwSwapBuffers();
}

int main(int argc, char* argv[])
{
    if(!glfwInit())
        throw std::runtime_error("glfwInit failed");
    
    glfwOpenWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwOpenWindowHint(GLFW_OPENGL_VERSION_MAJOR, 3);
    glfwOpenWindowHint(GLFW_OPENGL_VERSION_MINOR, 2);
    glfwOpenWindowHint(GLFW_WINDOW_NO_RESIZE, GL_TRUE);
    if(!glfwOpenWindow(SCREEN_SIZE.x, SCREEN_SIZE.y, 8, 8, 8, 8, 0, 0, GLFW_WINDOW))
        throw std::runtime_error("glfwOpenWindow failed. Can your hardware handle OpenGL 3.2?");

    glewExperimental = GL_TRUE; //stops glew crashing on OSX :-/
    if(glewInit() != GLEW_OK)
        throw std::runtime_error("glewInit failed");
    
    std::cout << "OpenGL version: " << glGetString(GL_VERSION) << std::endl;
    std::cout << "GLSL version: " << glGetString(GL_SHADING_LANGUAGE_VERSION) << std::endl;
    std::cout << "Vendor: " << glGetString(GL_VENDOR) << std::endl;
    std::cout << "Renderer: " << glGetString(GL_RENDERER) << std::endl;
    
    if(!GLEW_VERSION_3_2)
        throw std::runtime_error("OpenGL 3.2 API is not available.");
    
    std::shared_ptr<Program> p = loadShaders();
    loadTriangle(p);
    
    while(glfwGetWindowParam(GLFW_OPENED))
    {
        render(p);
    }
    
    glfwTerminate();
    
    return 0;
}