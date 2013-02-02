#import <Foundation/Foundation.h>

#include <GLEW/glew.h>
#include <GLFW/GLFW.h>
#include <GLM/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include <iostream>
#include <vector>
#include <memory>

#include "Shader.h"
#include "Program.h"
#include "Bitmap.h"
#include "Texture.h"
#include "Camera.h"
#include "Mesh.h"

const glm::vec2 SCREEN_SIZE(800, 600);
GLuint gVAO = 0;
GLuint gVBO = 0;
GLfloat gDegreesRotated = 0.0f;

// returns the full path to the file `fileName` in the resources directory of the app bundle
static std::string ResourcePath(std::string fileName) {
    NSString* fname = [NSString stringWithCString:fileName.c_str() encoding:NSUTF8StringEncoding];
    NSString* path = [[[NSBundle mainBundle] resourcePath] stringByAppendingPathComponent:fname];
    return std::string([path cStringUsingEncoding:NSUTF8StringEncoding]);
}

Program loadShaders()
{
    std::vector<Shader> shaders;
    
    shaders.push_back(Shader::shaderFromFile(ResourcePath("test.vert"), GL_VERTEX_SHADER));
    shaders.push_back(Shader::shaderFromFile(ResourcePath("test.frag"), GL_FRAGMENT_SHADER));
    
    Program p = Program(shaders);
    
    ProgramContext pc(p);
    
    //set the "projection" uniform in the vertex shader, because it's not going to change
    
    //set the "camera" uniform in the vertex shader, because it's also not going to change
    
    return p;
}

// loads the file "hazard.png" into gTexture
static Texture LoadTexture() {
    Bitmap bmp = Bitmap::bitmapFromFile(ResourcePath("wooden-crate.jpg"));
    bmp.flipVertically();
    return Texture{bmp};
}

void render(Program& p, const Texture& t, const Camera& c, const Mesh& m)
{
    glClearColor(0, 0, 0, 1); // black
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    
    ProgramContext pc{p};
    
    p.setUniform("model", glm::rotate(glm::mat4(), gDegreesRotated, glm::vec3(0,1,0)));
    p.setUniform("camera", c.matrix());
    
    // bind the texture and set the "tex" uniform in the fragment shader
    TextureContext tex{t, GL_TEXTURE0};
    p.setUniform("tex", 0); //set to 0 because the texture is bound to GL_TEXTURE0
    
    m.draw(p);
        
    glfwSwapBuffers();
}

// update the scene based on the time elapsed since last update
void Update(float secondsElapsed, Camera& c) {
    const GLfloat degreesPerSecond = 45.0f;
    gDegreesRotated += secondsElapsed * degreesPerSecond;
    while(gDegreesRotated > 360.0f) gDegreesRotated -= 360.0f;
    
    //CAMERA MOVEMENT
    const float moveSpeed = 2.0; //units per second
    if(glfwGetKey('S')){
        c.offsetPosition(secondsElapsed * moveSpeed * -c.forward());
    } else if(glfwGetKey('W')){
        c.offsetPosition(secondsElapsed * moveSpeed * c.forward());
    }
    if(glfwGetKey('A')){
        c.offsetPosition(secondsElapsed * moveSpeed * -c.right());
    } else if(glfwGetKey('D')){
        c.offsetPosition(secondsElapsed * moveSpeed * c.right());
    }
    if(glfwGetKey('Z')){
        c.offsetPosition(secondsElapsed * moveSpeed * -glm::vec3(0,1,0));
    } else if(glfwGetKey('X')){
        c.offsetPosition(secondsElapsed * moveSpeed * glm::vec3(0,1,0));
    }
    
    //rotate camera based on mouse movement
    const float mouseSensitivity = 0.1;
    int mouseX, mouseY;
    glfwGetMousePos(&mouseX, &mouseY);
    c.offsetOrientation(mouseSensitivity * mouseY, mouseSensitivity * mouseX);
    glfwSetMousePos(0, 0); //reset the mouse, so it doesn't go out of the window
}

int main(int argc, char* argv[])
{
    if(!glfwInit())
        throw std::runtime_error("glfwInit failed");
    
    glfwOpenWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwOpenWindowHint(GLFW_OPENGL_VERSION_MAJOR, 3);
    glfwOpenWindowHint(GLFW_OPENGL_VERSION_MINOR, 2);
    glfwOpenWindowHint(GLFW_WINDOW_NO_RESIZE, GL_TRUE);
    if(!glfwOpenWindow(SCREEN_SIZE.x, SCREEN_SIZE.y, 8, 8, 8, 8, 16, 0, GLFW_WINDOW))
        throw std::runtime_error("glfwOpenWindow failed. Can your hardware handle OpenGL 3.2?");
    
    // GLFW settings
    glfwDisable(GLFW_MOUSE_CURSOR);
    glfwSetMousePos(0, 0);
    glfwSetMouseWheel(0);

    glewExperimental = GL_TRUE; //stops glew crashing on OSX :-/
    if(glewInit() != GLEW_OK)
        throw std::runtime_error("glewInit failed");
    glGetError(); //ignore any errors coming from glewInit, because they can be safely ignored.
    
    std::cout << "OpenGL version: " << glGetString(GL_VERSION) << std::endl;
    std::cout << "GLSL version: " << glGetString(GL_SHADING_LANGUAGE_VERSION) << std::endl;
    std::cout << "Vendor: " << glGetString(GL_VENDOR) << std::endl;
    std::cout << "Renderer: " << glGetString(GL_RENDERER) << std::endl;
    
    if(!GLEW_VERSION_3_2)
        throw std::runtime_error("OpenGL 3.2 API is not available.");
    
    
    //OpenGL settings
    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LESS);
    glEnable(GL_CULL_FACE);
    
    Program p = loadShaders();
    Texture t = LoadTexture();
    
    Camera c = Camera{};
    c.setAspectRatio(SCREEN_SIZE.x / SCREEN_SIZE.y);
    c.setPosition(glm::vec3{0,0,5});
    
    Mesh m = Mesh::cube();
    
    double lastTime = glfwGetTime();
    while(glfwGetWindowParam(GLFW_OPENED))
    {
        double thisTime = glfwGetTime();
        Update(thisTime - lastTime, c);
        lastTime = thisTime;
        
        render(p, t, c, m);
        
        GLenum error = glGetError();
        if(error != GL_NO_ERROR)
            std::cerr << "OpenGL Error " << error << ": " << (const char*)gluErrorString(error) << std::endl;
        
        //exit program if escape key is pressed
        if(glfwGetKey(GLFW_KEY_ESC))
            glfwCloseWindow();
    }
    
    glfwTerminate();
    
    return 0;
}