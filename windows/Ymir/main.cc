#include <GL/glew.h>
#include <GL/GLFW.h>
#include <GLM/glm.hpp>

#include <iostream>
#include <vector>
#include <memory>
#include <sstream>

#include "../../src/Shader.h"
#include "../../src/Program.h"
#include "../../src/Bitmap.h"
#include "../../src/Texture.h"
#include "../../src/Renderer/Camera.h"
#include "../../src/Mesh.h"
#include "../../src/Renderer/Light.h"

#include "../../src/Scene.h"
#include "../../src/Entity.h"
#include "../../src/EngineManager.h"
#include "../../src/LightComponent.h"
#include "../../src/SpatialComponent.h"
#include "../../src/Renderer/RenderEngine.h"
#include "../../src/BehaviorEngine.h"
#include "../../src/CollisionEngine.h"
#include "../../src/AudioEngine.h"
#include "../../src/CameraComponent.h"
#include "../../src/ModelComponent.h"
#include "../../src/ListenerComponent.h"
#include "../../src/SourceComponent.h"
#include "../../src/CarComponent.h"
#include "../../src/ObjGeometry.h"

#include "../../src/ResourceManager.h"
#include "../../src/DirectoryManifest.h"

#include "../../src/RotateBehavior.h"
#include "../../src/FPSCameraBehavior.h"
#include "../../src/WSADMoveBehavior.h"
#include "../../src/RTSCameraBehavior.h"
#include "../../src/SimpleTestTriggerBehavior.h"
#include "../../src/CarCollisionBehavior.h"

#include "../../src/ProgramResourceLoader.h"
#include "../../src/CubemapResourceLoader.h"

#include "../../src/Audio/Buffer.h"
#include "../../src/Audio/Source.h"
#include "../../src/Audio/Listener.h"

#include "../../src/TileMap.h"

#include "../../src/Gameplay.h"

using namespace Ymir;

//TODO LIST
//
//SKYBOX
//WIN-LOSE (OBJECTIVES)
//DOCUMENTATION
//WINDOWS SUPPORT

const glm::vec2 SCREEN_SIZE(800, 600);
bool isDone, isDone1, isDone2, isDone3;

static std::string ResourceDirectory()
{
    return "./";
}

int main(int argc, char* argv[])
{
    // Limits for building units and buildings
    isDone = true;
    isDone1 = true;
    isDone2 = true;
    isDone3 = true;

    //resource management
    std::shared_ptr<DirectoryManifest> manifest = std::make_shared<DirectoryManifest>(ResourceDirectory());

    ResourceManager<Texture> textureManager;
    textureManager.addManifest(manifest);

    ResourceManager<Program, ProgramResourceLoader> programManager;
    programManager.addManifest(manifest);

    ResourceManager<Mesh> meshManager;
    meshManager.addManifest(manifest);

    ResourceManager<Audio::Buffer> audioBufferManager;
    audioBufferManager.addManifest(manifest);

    ResourceManager<Cubemap, CubemapResourceLoader> cubemapManager;
    cubemapManager.addManifest(manifest);

    //engine creation
    EngineManager engines;
    engines.assign<BehaviorEngine>();
    engines.assign<CollisionEngine>();
    engines.assign<AudioEngine>();
    RenderEngine& renderEngine = engines.assign<RenderEngine>(programManager, textureManager, cubemapManager);

    std::shared_ptr<Texture> track_tex = textureManager.resource("grass.png");
    std::shared_ptr<Texture> house_tex = textureManager.resource("house1.bmp");
    std::shared_ptr<Texture> t = textureManager.resource("wooden-crate.jpg");

    std::shared_ptr<Mesh> house_mesh = meshManager.resource("house.obj");
    std::shared_ptr<Mesh> tire = meshManager.resource("carTire.obj");
    std::shared_ptr<Mesh> car = meshManager.resource("car.obj");

    std::shared_ptr<Mesh> unit_mesh = meshManager.resource("unit.obj");
    std::shared_ptr<Mesh> track_mesh = meshManager.resource("track.obj");
    std::shared_ptr<Mesh> monkey_mesh = meshManager.resource("monkey.obj");

    std::shared_ptr<Audio::Buffer> hello_world_buffer = audioBufferManager.resource("helloworld.wav");
    std::shared_ptr<Audio::Buffer> crash_sound = audioBufferManager.resource("crash.wav");

    Gameplay gameplay{engines, textureManager, meshManager, SCREEN_SIZE};
    gameplay.createCamera();

    Scene& scene = gameplay.getScene();

    //light one (spot)
    Entity& light_one = scene.assign("light one");
    light_one.assign<LightComponent>(glm::vec3{1.0, 1.0, 1.0}, glm::vec3{0.0, 0.25, 0.05});
    light_one.assign<SpatialComponent>(glm::vec3{7.0, 0.0, 0.0});

    //light two (directional)
    Entity& light_two = scene.assign("light two");
    light_two.assign<SpatialComponent>(glm::vec3{0.0, 1.0, 0.0});
    auto light_two_lightc = light_two.assign<LightComponent>(glm::vec3{0.8, 0.8, 0.8});
    light_two_lightc.lightType = LightComponent::LightType::DIRECTIONAL;

//    track
    Entity& track = scene.assign("track");
    track.assign<SpatialComponent>(glm::vec3{0, 0.045, 0});
    track.assign<ModelComponent>(track_mesh, track_tex);

    Entity& unit = scene.assign("unit");
    unit.assign<SpatialComponent>(glm::vec3{0, 0.045, 0});
    unit.assign<ModelComponent>(unit_mesh, t);

    TileMap* tiles = new TileMap(100, 2, 2);

    double lastTime = glfwGetTime();
    while(glfwGetWindowParam(GLFW_OPENED))
    {
        double thisTime = glfwGetTime();
        double delta = thisTime - lastTime;
        lastTime = thisTime;

        engines.update(-1, delta);
        engines.update(0, delta);
        engines.update(1, delta);

        glfwDisable(GLFW_KEY_REPEAT);

        if(glfwGetKey( GLFW_KEY_LEFT ) == GLFW_PRESS ) {
            auto test = unit.component<SpatialComponent>();
            glm::vec3 newPos = renderEngine.get3DPositionFromMousePosition();
            newPos.y=0;
            test->position = newPos;
        }

        if(glfwGetKey( GLFW_KEY_UP ) == GLFW_PRESS && isDone) {
            isDone = false;
            gameplay.buildCentralIntelligenceCore(renderEngine.get3DPositionFromMousePosition());
        }

        if(glfwGetKey( GLFW_KEY_DOWN ) == GLFW_PRESS && isDone1) {
            isDone1 = false;
            gameplay.buildOrbitalDropBeacon(renderEngine.get3DPositionFromMousePosition());
        }
        if(glfwGetMouseButton( GLFW_MOUSE_BUTTON_LEFT ) == GLFW_PRESS && isDone2) {
            isDone2 = false;
            gameplay.createWorker(renderEngine.get3DPositionFromMousePosition());
        }
        if(glfwGetMouseButton( GLFW_MOUSE_BUTTON_RIGHT ) == GLFW_PRESS && isDone3) {
            isDone3 = false;
            gameplay.createBasicInfantrie(renderEngine.get3DPositionFromMousePosition());
        }

        GLenum error = glGetError();
        if(error != GL_NO_ERROR)
            std::cerr << "OpenGL Error " << error << ": " << (const char*)gluErrorString(error) << std::endl;

        //exit program if escape key is pressed
        if(glfwGetKey(GLFW_KEY_ESC))
            glfwCloseWindow();
    }
    return 0;
}
