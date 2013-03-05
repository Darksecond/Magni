#include "RenderEngine.h"

#include <GLEW/glew.h>
#include <GLFW/GLFW.h>
#include <GLM/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <memory>
#include <iostream>
#include <sstream>

#include "Light.h"
#include "LightComponent.h"
#include "SpatialComponent.h"

using namespace Ymir;

const glm::vec2 SCREEN_SIZE(800, 600);

int frames = 0;
double t0 = 0.0;
void showFPS() {
    
    double t, fps;
    
    // Get current time
    t = glfwGetTime();  // Get number of seconds since glfwInit()
    // Calculate and display FPS (frames per second) in title bar.
    if( (t-t0) > 1.0 || frames == 0 )
    {
        fps = (double)frames / (t-t0);
        std::stringstream ss;
        ss << "Ymir | FPS: " << fps;
        glfwSetWindowTitle(ss.str().c_str());
        t0 = t;
        frames = 0;
    }
    frames ++;
}

RenderEngine::RenderEngine(ResourceManager<Program, ProgramResourceLoader>& programManager) : lights{}, models{}
{
    initGLFW();
    initGLEW();
    initOpenGL();
    
    texture_program = programManager.resource("texture");
    phong_program = programManager.resource("phong");
    overlay_program = programManager.resource("overlay");
}
    
void RenderEngine::initGLFW()
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
    //glfwSwapInterval(0); //uncomment for no v-sync
}

void RenderEngine::initGLEW()
{
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
}

void RenderEngine::initOpenGL()
{
    //OpenGL settings
    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LESS);
    glEnable(GL_CULL_FACE);
}

RenderEngine::~RenderEngine()
{
    glfwTerminate();
}

//TODO not really used anymore since Scene was introduced
void RenderEngine::registerEntity(Entity& entity)
{    
    lights.registerEntity(entity);
    models.registerEntity(entity);
    
    std::unique_ptr<Camera> cameraNode = Camera::fromEntity(entity);
    if(cameraNode)
    {
        _camera = std::move(cameraNode);
    }
}

void RenderEngine::unregisterEntity(Entity& entity)
{    
    lights.unregisterEntity(entity);
    models.unregisterEntity(entity);
    //camera
}

void RenderEngine::addComponent(Entity& entity, const BaseComponent::Type& component_type)
{
    lights.addComponent(entity, component_type);
    models.addComponent(entity, component_type);
    
    //camera
    if(component_type == CameraComponent::type() || component_type == SpatialComponent::type())
    {
        std::unique_ptr<Camera> cameraNode = Camera::fromEntity(entity);
        if(cameraNode && _camera == nullptr)
        {
            _camera = std::move(cameraNode);
        }
    }
}

void renderTexture(Program& p, const Texture& t, const Camera& c, const Mesh& m, const Model& model)
{
    //always
    //p is part of material!
    ProgramContext pc{p};
    
    //always
    glm::mat4 view = c.viewMatrix();
    glm::mat4 modelView = view * model.spatial.matrix();
    
    //always
    p.setUniform("projection", c.projectionMatrix());
    p.setUniform("modelView", modelView);
    
    //material
    // bind the texture and set the "tex" uniform in the fragment shader
    TextureContext tex{t, GL_TEXTURE0};
    p.setUniform("tex", 0); //set to 0 because the texture is bound to GL_TEXTURE0
    
    //always
    m.draw(p);
}

void renderLight(Program& p, const Camera& c, const Mesh& m, const Light& l, const Model& model)
{
    //always
    //p is part of material!
    ProgramContext pc{p};
    
    //always
    glm::mat4 view = c.viewMatrix();
    glm::mat4 modelView = view * model.spatial.matrix();
    
    glm::mat3 normal = glm::transpose(glm::inverse(glm::mat3(modelView))); //not part of light, but only needed for lights so far
    
    //material    
    model.attach(p);
    
    //light
    l.attach(p, c);
    
    p.setUniform("normal", normal); //not part of light, but only needed for lights so far
    
    //always
    p.setUniform("projection", c.projectionMatrix());
    p.setUniform("modelView", modelView);
    
    //always
    m.draw(p);
}

void render(Program& pt, Program& pl, const Camera& c,
            const NodeCache<Light>& lights, Model& model)
{
    //static settings, really
    glEnable(GL_BLEND);
    glDepthFunc(GL_LEQUAL);
    
    //transparency fix
    //http://www.opengl.org/discussion_boards/showthread.php/145630-Translucency-and-multi-pass-lighting
    bool first = true;
    for(auto& light : lights)
    {
        if(first)
        {
            glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
            first = false;
        }
        else
        {
            glBlendFunc(GL_SRC_ALPHA,GL_ONE);
        }
        renderLight(pl, c, *model.model.mesh, *light, model);
    }
    
    glBlendFunc(GL_ZERO,GL_SRC_COLOR);
    renderTexture(pt, *model.model.texture, c, *model.model.mesh, model);
    
    glDisable(GL_BLEND);
}

void renderOverlay(Program& p)
{
    //ProgramContext pc{p};
}

void RenderEngine::update(int pass, double delta)
{
    if(pass == 1)
    {
        glClearColor(0.0, 0.0, 0.0, 1); // black
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        
        for(auto& model : models)
        {
            render(*texture_program, *phong_program, *_camera, lights, *model);
        }
        
        renderOverlay(*overlay_program);
        
        showFPS();
        glfwSwapBuffers();
    }
}