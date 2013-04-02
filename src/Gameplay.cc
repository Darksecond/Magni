#include "Gameplay.h"
#include <iostream>

using namespace Ymir;

Gameplay::Gameplay(EngineManager& engines, ResourceManager<Texture>& textureManager, ResourceManager<Mesh>& meshManager, RenderEngine& renderEngine, glm::vec2 screenSize) : scene(engines), textureManager(textureManager), meshManager(meshManager), renderEngine(renderEngine), screenSize(screenSize)
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
    // camera.assignBehavior(std::unique_ptr<Behavior>{new WSADMoveBehavior});
    camera.assignBehavior(std::unique_ptr<Behavior>{new RTSCameraBehavior});
}

void Gameplay::createWorker(glm::vec3 position)
{
    position.y = 0.3;
    std::shared_ptr<Texture> worker_tex = textureManager.resource("workertex.png");
    std::shared_ptr<Mesh> worker_mesh = meshManager.resource("worker.obj");

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
    std::shared_ptr<Mesh> house_mesh = meshManager.resource("orbitaldropbeacon.obj");
    std::shared_ptr<Texture> house_tex = textureManager.resource("house1.bmp");

    Entity& house = scene.assign("houseMesh");
    house.assign<SpatialComponent>(position);
    house.assign<ModelComponent>(house_mesh, house_tex);
    house.assign<EnergyComponent>(150);
}

void Gameplay::buildOrbitalDropBeacon(glm::vec3 position)
{
    position.y = 0.0;
    std::shared_ptr<Texture> t = textureManager.resource("wooden-crate.jpg");
    std::shared_ptr<Mesh> house_mesh = meshManager.resource("house.obj");

    Entity& house = scene.assign("OrbitalDropBeacon");
    house.assign<SpatialComponent>(position);
    house.assign<ModelComponent>(house_mesh, t);
    house.assign<EnergyComponent>(-100);
}

void Gameplay::buildPowerCore()
{

}

void Gameplay::buildAcademyOfAdvancedTechnologies()
{

}

void Gameplay::winGame()
{
    std::shared_ptr<Text> winningText = std::make_shared<Text>("YOU ARE VICTORIOUS", glm::vec2{10, 10}, 20);
    renderEngine.addText(winningText);

}

void Gameplay::loseGame()
{
    std::shared_ptr<Text> losingText = std::make_shared<Text>("YOU ARE DEFEATED", glm::vec2{10, 30}, 20);
    renderEngine.addText(losingText);
}

void Gameplay::sellEntity(Entity* aEntity)
{
    if(aEntity != nullptr) {
        scene.deleteEntity(aEntity);
        currentSelectedUnit = nullptr;
        std::cout << " You now got +4 money :) " << std::endl;
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
    currentSelectedUnit = getEntityAtPosition(position);
}

Entity* Gameplay::getEntityAtPosition(glm::vec3 position)
{
    double distance = 2.5f;
    Entity* theEntity = nullptr;

    for(std::unique_ptr<Entity>& entity : scene.entities)
    {
        auto test = entity->component<SpatialComponent>();
        double distanceBetween = glm::distance(test->position, position);

        if(distanceBetween < 2.5f && distanceBetween < distance) {
            distance = distanceBetween;
            theEntity = entity.get();
        }
    }

    return theEntity;
}

Entity* Gameplay::getCurrentSelectedEntity()
{
    return currentSelectedUnit;
}

Scene& Gameplay::getScene()
{
    return scene;
}
