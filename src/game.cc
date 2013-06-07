#include "game.h"

#include "game_object.h"
#include "model.h"
#include "camera.h"
#include "spatial_updater_visitor.h"

#ifdef __APPLE__
    #include <GLFW/GLFW.h>
    #include <GLEW/glew.h>
#endif
#ifdef _WIN32
    #include <GL/glfw.h>
    #include <GL/glew.h>
#endif // _WIN32

#include <iostream>

const glm::vec2 SCREEN_SIZE(800, 600);

game::game() : _running(true), _world()
{
}

void game::boot(const std::string& resource_dir)
{
    manifest = std::make_shared<Ymir::DirectoryManifest>(resource_dir);
    
    textureManager.addManifest(manifest);
    programManager.addManifest(manifest);
    meshManager.addManifest(manifest);
    
    //BEGIN MOVE TO RENDERER
    //GLFW
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
    glfwSwapInterval(0); //uncomment for no v-sync
    
    //GLEW
    glewExperimental = GL_TRUE; //stops glew crashing on OSX
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
    //END MOVE TO RENDERER
}

void game::shutdown()
{
    glfwTerminate();
}

void game::build()
{
    _world = std::make_shared<game_object>("world");
    
    _world->add(std::make_shared<camera>("camera", SCREEN_SIZE.x/SCREEN_SIZE.y));
    
    Ymir::Mesh m = std::move(Ymir::Mesh::cube());
    std::shared_ptr<Ymir::Mesh> cube = std::make_shared<Ymir::Mesh>(std::move(m));
    _world->add(std::make_shared<model>("cube", cube));
}

void game::run()
{
    while(_running)
    {
        //TODO moar!
        
        spatial_updater_visitor spatial_updater;
        _world->accept(spatial_updater);
        
        stop(); //TODO TEMP REMOVE
    }
}

void game::stop()
{
    _running = false;
}