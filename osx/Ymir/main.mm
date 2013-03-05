#import <Foundation/Foundation.h>

#include <GLEW/glew.h>
#include <GLFW/GLFW.h>
#include <GLM/glm.hpp>

#include <iostream>
#include <vector>
#include <memory>
#include <sstream>

#include "Shader.h"
#include "Program.h"
#include "Bitmap.h"
#include "Texture.h"
#include "Camera.h"
#include "Mesh.h"
#include "Light.h"

#include "Scene.h"
#include "Entity.h"
#include "EngineManager.h"
#include "LightComponent.h"
#include "SpatialComponent.h"
#include "RenderEngine.h"
#include "BehaviorEngine.h"
#include "CollisionEngine.h"
#include "AudioEngine.h"
#include "CameraComponent.h"
#include "ModelComponent.h"
#include "ListenerComponent.h"
#include "SourceComponent.h"
#include "CarComponent.h"
#include "ObjGeometry.h"

#include "ResourceManager.h"
#include "DirectoryManifest.h"

#include "RotateBehavior.h"
#include "FPSCameraBehavior.h"
#include "WSADMoveBehavior.h"
#include "SimpleTestTriggerBehavior.h"

#include "ProgramResourceLoader.h"

#include "Font.h"

#include "Buffer.h"
#include "Source.h"
#include "Listener.h"

using namespace Ymir;

//TODO LIST
//
//SKYBOX
//HUD
//WIN-LOSE (OBJECTIVES)

const glm::vec2 SCREEN_SIZE(800, 600);

static std::string ResourceDirectory()
{
    NSString* path = [[NSBundle mainBundle] resourcePath];
    return std::string([path cStringUsingEncoding:NSUTF8StringEncoding]);
}

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
    //glfwSwapInterval(0); //uncomment for no v-sync

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
    
    //resource management
    std::shared_ptr<DirectoryManifest> manifest = std::make_shared<DirectoryManifest>(ResourceDirectory());

    ResourceManager<Texture> textureManager;
    textureManager.addManifest(manifest);
    
    ResourceManager<Program, ProgramResourceLoader> programManager;
    programManager.addManifest(manifest);
    
    ResourceManager<Mesh> meshManager;
    meshManager.addManifest(manifest);
    
    //engine creation
    EngineManager engines;
    engines.assign<BehaviorEngine>();
    engines.assign<CollisionEngine>();
    engines.assign<AudioEngine>();
    
    std::shared_ptr<Program> texture_program = programManager.resource("texture");
    std::shared_ptr<Program> phong_program = programManager.resource("phong");
    std::shared_ptr<Program> overlay_program = programManager.resource("overlay");
    std::shared_ptr<Texture> t = textureManager.resource("wooden-crate.jpg");
    std::shared_ptr<Texture> car_tex = textureManager.resource("truck_color_cleantest.jpg");
    std::shared_ptr<Texture> track_tex = textureManager.resource("track.jpg");
    std::shared_ptr<Mesh> tire = meshManager.resource("carTire.obj");
    std::shared_ptr<Mesh> car = meshManager.resource("car.obj");
    std::shared_ptr<Mesh> track_mesh = meshManager.resource("track.obj");
    std::shared_ptr<Mesh> monkey_mesh = meshManager.resource("monkey.obj");
    std::shared_ptr<Audio::Buffer> hello_world_buffer = std::make_shared<Audio::Buffer>(Audio::Buffer::fromFile(ResourceDirectory() + "/helloworld.wav"));
    
    engines.assign<RenderEngine>(*texture_program, *phong_program, *overlay_program);
    
    Scene scene{engines};
    
    Entity& monkey = scene.assign("monkey");
    monkey.assign<SpatialComponent>(glm::vec3{5.0, 0.0, 0.0}).scale = glm::vec3{0.5};
    monkey.assign<ModelComponent>(monkey_mesh, t);
    monkey.assign<SphereColliderComponent>(0.5).trigger = true;
    //monkey.assign<SourceComponent>(hello_world_buffer).playing = true;
    monkey.assignBehavior(std::unique_ptr<Behavior>{new SimpleTestTriggerBehavior});

    //car body
    Entity& car_body = scene.assign("car body");
    car_body.assign<SpatialComponent>(glm::vec3{0.0, 0.0, 0.0});
    car_body.assign<ModelComponent>(car, car_tex);
    car_body.assign<CarComponent>();
    car_body.assign<SphereColliderComponent>(0.5);
    car_body.assign<SourceComponent>(hello_world_buffer).playing = true;
    car_body.assignBehavior(std::unique_ptr<Behavior>{new WSADMoveBehavior});
    
    Entity& camera = scene.assign("camera", &car_body);
    camera.assign<ListenerComponent>();
    camera.assign<CameraComponent>(SCREEN_SIZE.x / SCREEN_SIZE.y);
    auto& c_s = camera.assign<SpatialComponent>(glm::vec3{0.0, -1.0, 3.0});
    glm::vec3 euler{-10,180,0};
    c_s.setDirection(euler);
    //camera.assignBehavior(std::unique_ptr<Behavior>{new FPSCameraBehavior});
    //camera.assignBehavior(std::unique_ptr<Behavior>{new WSADMoveBehavior});
    
    //light one (spot)
    Entity& light_one = scene.assign("light one");
    light_one.assign<LightComponent>(glm::vec3{1.0, 1.0, 1.0}, glm::vec3{0.0, 0.25, 0.05});
    light_one.assign<SpatialComponent>(glm::vec3{7.0, 0.0, 0.0});
    
    //light two (directional)
    Entity& light_two = scene.assign("light two");
    light_two.assign<SpatialComponent>(glm::vec3{0.0, 1.0, 0.0});
    auto light_two_lightc = light_two.assign<LightComponent>(glm::vec3{0.8, 0.8, 0.8});
    light_two_lightc.lightType = LightComponent::LightType::DIRECTIONAL;
    
    //back right wheel
    Entity& car_f_r_wheel = scene.assign("wheel", &car_body);
    auto& s = car_f_r_wheel.assign<SpatialComponent>(glm::vec3{0.15, -0.06, 0.27});
    s.scale = glm::vec3{3};
    car_f_r_wheel.assignBehavior(std::unique_ptr<Behavior>{new RotateBehavior{false}});
    car_f_r_wheel.assign<ModelComponent>(tire, car_tex);
    
    //front right wheel
    Entity& car_b_r_wheel = scene.assign("wheel", &car_body);
    auto& s2 = car_b_r_wheel.assign<SpatialComponent>(glm::vec3{0.15, -0.06, -0.33});
    s2.scale = glm::vec3{3};
    car_b_r_wheel.assignBehavior(std::unique_ptr<Behavior>{new RotateBehavior{true}});
    car_b_r_wheel.assign<ModelComponent>(tire, car_tex);
    
    //back left wheel
    Entity& car_f_l_wheel = scene.assign("wheel", &car_body);
    auto& s3 = car_f_l_wheel.assign<SpatialComponent>(glm::vec3{-0.15, -0.06, 0.27});
    s3.scale = glm::vec3{3};
    car_f_l_wheel.assignBehavior(std::unique_ptr<Behavior>{new RotateBehavior{false}});
    car_f_l_wheel.assign<ModelComponent>(tire, car_tex);
    
    //front left wheel
    Entity& car_b_l_wheel = scene.assign("wheel", &car_body);
    auto& s4 = car_b_l_wheel.assign<SpatialComponent>(glm::vec3{-0.15, -0.06, -0.33});
    s4.scale = glm::vec3{3};
    car_b_l_wheel.assignBehavior(std::unique_ptr<Behavior>{new RotateBehavior{true}});
    car_b_l_wheel.assign<ModelComponent>(tire, car_tex);
    
    //track
    Entity& track = scene.assign("track");
    track.assign<SpatialComponent>(glm::vec3{0, 0.045, 0});
    track.assign<ModelComponent>(track_mesh, track_tex);
    //track.assign<SphereColliderComponent>(10);
    
    //font
    //std::unique_ptr<Font> fnt = Font::fontFromFile("DroidSerif-Regular.ttf", 23, *overlay_program);
    
    
    double lastTime = glfwGetTime();
    while(glfwGetWindowParam(GLFW_OPENED))
    {
        double thisTime = glfwGetTime();
        double delta = thisTime - lastTime;
        lastTime = thisTime;
        
        showFPS(); //in titlebar
        
        engines.update(-1, delta);
        engines.update(0, delta);
        engines.update(1, delta);
        
        GLenum error = glGetError();
        if(error != GL_NO_ERROR)
            std::cerr << "OpenGL Error " << error << ": " << (const char*)gluErrorString(error) << std::endl;
        
        //exit program if escape key is pressed
        if(glfwGetKey(GLFW_KEY_ESC))
            glfwCloseWindow();
        
        //std::cout << "" << std::endl;
    }
    
    glfwTerminate();
    
    return 0;
}