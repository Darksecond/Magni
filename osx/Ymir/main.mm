#import <Foundation/Foundation.h>

#include <GLEW/glew.h>
#include <GLFW/GLFW.h>
#include <GLM/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/rotate_vector.hpp>

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

#include "Entity.h"
#include "LightComponent.h"
#include "SpatialComponent.h"
#include "RenderEngine.h"
#include "CameraComponent.h"
#include "ModelComponent.h"

#include "ResourceManager.h"
#include "DirectoryManifest.h"

using namespace Ymir;

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

static std::string ResourceDirectory()
{
    NSString* path = [[NSBundle mainBundle] resourcePath];
    return std::string([path cStringUsingEncoding:NSUTF8StringEncoding]);
}

Program loadTextureShaders()
{
    std::vector<Shader> shaders;
    
    shaders.push_back(Shader::shaderFromFile(ResourcePath("test.vert"), GL_VERTEX_SHADER));
    shaders.push_back(Shader::shaderFromFile(ResourcePath("test.frag"), GL_FRAGMENT_SHADER));
    
    Program p = Program(shaders);
    
    ProgramContext pc(p);
            
    return p;
}

Program loadLightingShaders()
{
    std::vector<Shader> shaders;
    
    shaders.push_back(Shader::shaderFromFile(ResourcePath("test2.vert"), GL_VERTEX_SHADER));
    shaders.push_back(Shader::shaderFromFile(ResourcePath("test2.frag"), GL_FRAGMENT_SHADER));
    
    Program p = Program(shaders);
    
    ProgramContext pc(p);
    
    return p;
}

// loads the file "hazard.png" into gTexture
static Texture LoadTexture() {
    Bitmap bmp = Bitmap::bitmapFromFile(ResourcePath("wooden-crate.jpg"));
    bmp.flipVertically();
    return Texture{bmp};
}


// update the scene based on the time elapsed since last update
void Update(float secondsElapsed, Camera& c, Model& m) {
    const GLfloat degreesPerSecond = 45.0f;
    gDegreesRotated = secondsElapsed * degreesPerSecond;
    m.spatial.direction = glm::rotate(m.spatial.direction, gDegreesRotated, glm::vec3(1,0,0));
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
        ss << "FPS: " << fps;
        glfwSetWindowTitle(ss.str().c_str());
        t0 = t;
        frames = 0;
    }
    frames ++;
}

//TODO TEST
class ProgramResourceLoader
{
public:
    std::shared_ptr<Program> load(const ManifestContainer& manifests, const std::string& identifier)
    {
        auto vertStream = manifests.read(identifier + std::string{".vert"});
        auto fragStream = manifests.read(identifier + std::string{".frag"});
        if(vertStream && fragStream)
        {
            Shader vert{vertStream->string(), GL_VERTEX_SHADER};
            Shader frag{fragStream->string(), GL_FRAGMENT_SHADER};
            std::vector<Shader> shaders{};
            shaders.push_back(std::move(vert));
            shaders.push_back(std::move(frag));
            return std::make_shared<Program>(shaders);
        }
        return nullptr;
    }
};


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
    
    
    std::shared_ptr<Program> test = programManager.resource("test");
    std::shared_ptr<Program> test2 = programManager.resource("test2");
    std::shared_ptr<Texture> t = textureManager.resource("wooden-crate.jpg");
    
    std::shared_ptr<Mesh> m = std::make_shared<Mesh>(Mesh::cube());
    RenderEngine engine{*test, *test2};
    
    Entity camera;
    camera.assign<CameraComponent>(SCREEN_SIZE.x / SCREEN_SIZE.y);
    camera.assign<SpatialComponent>(glm::vec3{0.0, 0.0, 5.0});
    engine.registerEntity(camera);
    
    
    //light one (spot)
    Entity light_one{};
    light_one.assign<LightComponent>(glm::vec3{1.0, 1.0, 1.0}, glm::vec3{0.0, 0.15, 0.0});
    light_one.assign<SpatialComponent>(glm::vec3{7.0, 0.0, 0.0});
    engine.registerEntity(light_one);
    
    //light two (directional)
    Entity light_two{};    
    auto light_two_lightc = light_two.assign<LightComponent>(glm::vec3{0.5, 0.5, 0.5});
    light_two_lightc->lightType = LightComponent::LightType::DIRECTIONAL;
    auto light_two_spatialc = light_two.assign<SpatialComponent>(glm::vec3{0.0, 3.0, 0.0});
    engine.registerEntity(light_two);
    

    std::cout << light_two_lightc->type() << std::endl;
    std::cout << light_two_spatialc->type() << std::endl;

    
    //model one (box)
    Entity model_one{};
    model_one.assign<SpatialComponent>(glm::vec3{0.0, 0.0, 0.0});
    model_one.assign<ModelComponent>(m, t);
    engine.registerEntity(model_one);
    
    Entity model_two{};
    model_two.assign<SpatialComponent>(glm::vec3{5.0, 0.0, 0.0});
    engine.registerEntity(model_two);
    
    model_two.assign<ModelComponent>(m, t);
    engine.addComponent(model_two, ModelComponent::type());
    
    //engine.unregisterEntity(light_two);
    
    double lastTime = glfwGetTime();
    while(glfwGetWindowParam(GLFW_OPENED))
    {
        double thisTime = glfwGetTime();
        Update(thisTime - lastTime, *camera.node<Camera>(), *model_one.node<Model>());
        lastTime = thisTime;
        
        showFPS(); //in titlebar
        
        engine.execute();
        
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