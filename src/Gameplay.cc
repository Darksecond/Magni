
#include "Gameplay.h"

using namespace Ymir;

Gameplay::Gameplay(EngineManager& engines, ResourceManager<Texture>& textureManager, ResourceManager<Mesh>& meshManager, glm::vec2 screenSize) : scene(engines), textureManager(textureManager), meshManager(meshManager), screenSize(screenSize)
{
}

void Gameplay::createCamera()
{
    Entity& camera = scene.assign("camera");
    camera.assign<ListenerComponent>();
    camera.assign<CameraComponent>(screenSize.x / screenSize.y);
    auto& c_s = camera.assign<SpatialComponent>(glm::vec3{0.0, 5.0, 0});
    glm::vec3 euler{-60,0,0};
    c_s.setDirection(euler);
    //camera.assignBehavior(std::unique_ptr<Behavior>{new FPSCameraBehavior});
    //camera.assignBehavior(std::unique_ptr<Behavior>{new WSADMoveBehavior});
    camera.assignBehavior(std::unique_ptr<Behavior>{new RTSCameraBehavior});
}

void Gameplay::createWorker(glm::vec3 position)
{
    position.y = 0.0;
    std::shared_ptr<Texture> worker_tex = textureManager.resource("wooden-crate.jpg");
    std::shared_ptr<Mesh> worker_mesh = meshManager.resource("monkey.obj");

    Entity& worker = scene.assign("worker");
    worker.assign<SpatialComponent>(position);
    worker.assign<ModelComponent>(worker_mesh, worker_tex);
    // TODO add other components
}

void Gameplay::createBasicInfantrie(glm::vec3 position)
{
    position.y = 0.0;
    std::shared_ptr<Texture> basicInfantrie_tex = textureManager.resource("truck_color_cleantest.jpg");
    std::shared_ptr<Mesh> basicInfantrie_mesh = meshManager.resource("car.obj");

    Entity& basicInfantrie = scene.assign("basicInfantrie");
    basicInfantrie.assign<SpatialComponent>(position);
    basicInfantrie.assign<ModelComponent>(basicInfantrie_mesh, basicInfantrie_tex);
    // TODO add other components
}

void Gameplay::createAdvancedInfantrie()
{
    // TODO
}

void Gameplay::createEngineer()
{
    // TODO
}

Scene& Gameplay::getScene()
{
    return scene;
}
