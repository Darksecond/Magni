
#include "Gameplay.h"

#include <iostream>

using namespace Ymir;

Gameplay::Gameplay(EngineManager& engines, ResourceManager<Texture>& textureManager, ResourceManager<Mesh>& meshManager, glm::vec2 screenSize) : scene(engines), textureManager(textureManager), meshManager(meshManager), screenSize(screenSize) {

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
    // camera.assignBehavior(std::unique_ptr<Behavior>{new WSADMoveBehavior});
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

void Gameplay::buildCentralIntelligenceCore(glm::vec3 position)
{
    position.y = 0.0;
    std::shared_ptr<Mesh> house_mesh = meshManager.resource("house.obj");
    std::shared_ptr<Texture> house_tex = textureManager.resource("house1.bmp");

    Entity& house = scene.assign("houseMesh");
    house.assign<SpatialComponent>(position);
    house.assign<ModelComponent>(house_mesh, house_tex);
}

void Gameplay::buildOrbitalDropBeacon(glm::vec3 position)
{
    position.y = 0.0;
    std::shared_ptr<Texture> t = textureManager.resource("wooden-crate.jpg");
    std::shared_ptr<Mesh> house_mesh = meshManager.resource("house.obj");

    Entity& house = scene.assign("houseMesh");
    house.assign<SpatialComponent>(position);
    house.assign<ModelComponent>(house_mesh, t);
}

void Gameplay::buildPowerCore()
{

}

void Gameplay::buildAcademyOfAdvancedTechnologies()
{

}

void Gameplay::sellEntity(Entity* aEntity) {
    if(aEntity != nullptr) {
        scene.deleteEntity(aEntity);
        currentSelectedUnit = nullptr;
    }
    /* TODO
    * Remove entity from scene graph DONE
    * Add money to resources. based on health left en intial building costs?
    * int money = 0.5*(aEntity.Health*aEntityCosts.)
    * if( money < 1 ) {
    *   money = 0
    */
}

void Gameplay::updateSelectedEntity(glm::vec3 position)
{
    double distance = 10000;
    Entity* theEntity;

    for(std::unique_ptr<Entity>& entity : scene.entities)
    {
        auto test = entity->component<SpatialComponent>();
        double distanceBetween = glm::distance(test->position, position);

        if(distanceBetween < 2.5f && distanceBetween < distance)
            theEntity = entity.get();
    }
    currentSelectedUnit = theEntity;
}

Entity* Gameplay::getCurrentSelectedEntity() {
    return currentSelectedUnit;
}

Scene& Gameplay::getScene()
{
    return scene;
}
