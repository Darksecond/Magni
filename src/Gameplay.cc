#include "Gameplay.h"
#include "AttackComponent.h"
#include "HealthComponent.h"

#include <iostream>
#include <sstream>

using namespace Ymir;

Gameplay::Gameplay(EngineManager& engineManager, CurrencyEngine& currencyEngine, ResourceManager<Texture>& textureManager, ResourceManager<Mesh>& meshManager,
    RenderEngine& renderEngine, glm::vec2 screenSize, AttackEngine& attackEngine)
        : scene(engineManager), currencyEngine(currencyEngine) ,textureManager(textureManager), meshManager(meshManager),
            renderEngine(renderEngine), screenSize(screenSize) ,playernumber(1),currentSelectedUnit(nullptr),workerPrice(100),basicInfanteriePrice(1),orbitalDropBeaconPrice(250), infantryTimer(0), buildingTimer(0), unitIdentifyCounter(0), attackEngine(attackEngine)
{
    client = new Client();
    client->gp = this;
    client->setIPAdress(127, 0, 0, 1);

    playernumber = 1;
}

void Gameplay::createCamera()
{
    Entity& camera = scene.assign("camera");
    camera.assign<ListenerComponent>();
    camera.assign<CameraComponent>(screenSize.x / screenSize.y);
    auto& c_s = camera.assign<SpatialComponent>(glm::vec3{0.0, 5.0, 0});
    glm::vec3 euler{-60,0,0};
    c_s.setDirection(euler);
    camera.assignBehavior(std::unique_ptr<Behavior>{new RTSCameraBehavior(7, 6)});
}

void Gameplay::drawGrid(bool draw)
{
    renderEngine.setGrid(draw);
}

void Gameplay::createWorker(glm::vec3 position)
{
    std::cout << 5-infantryTimer << std::endl;
    if (infantryTimer > 5) {
        if(currencyEngine.currency >= workerPrice) {
            position.y = 0.3;
            std::shared_ptr<Texture> worker_tex = textureManager.resource("workertex.png");
            std::shared_ptr<Mesh> worker_mesh = meshManager.resource("worker.obj");

            Entity& worker = scene.assign("worker");
            worker.assign<SpatialComponent>(position);
            worker.assign<ModelComponent>(worker_mesh, worker_tex);
            worker.assign<HealthComponent>(1);
            worker.assign<CurrencyComponent>(workerPrice);
            worker.assign<OwnerComponent>(playernumber);

            NetworkPacket np(worker.id, BUILD);
            np.set(0, WORKER);
            np.set(1, position.x);
            np.set(2, position.y);
            np.set(3, position.z);

            client->write(np.char_array(), np.size());

            currencyEngine.currency -= workerPrice;
            infantryTimer = 0;
        } else {
            std::cout << "Not enough money for a worker unit " << std::endl;
        }
    }
}

void Gameplay::createGhostWorker(glm::vec3 position, int id)
{
    position.y = 0.3;
    std::shared_ptr<Texture> worker_tex = textureManager.resource("workertex.png");
    std::shared_ptr<Mesh> worker_mesh = meshManager.resource("worker.obj");

    Entity& worker = scene.assign("worker", id);
    worker.assign<SpatialComponent>(position);
    worker.assign<ModelComponent>(worker_mesh, worker_tex);
    worker.assign<HealthComponent>(100);
    worker.assign<CurrencyComponent>(workerPrice);
    worker.assign<OwnerComponent>(2);

    std::cout << "Builded a unit via network with ID: " << id << std::endl;
}

void Gameplay::createBasicInfantrie(glm::vec3 position)
{
    std::cout << 5-infantryTimer << std::endl;
        if (infantryTimer > 5) {
        if(currencyEngine.currency >= basicInfanteriePrice) {
            position.y = 0.0;
            std::shared_ptr<Texture> basicInfantrie_tex = textureManager.resource("truck_color_cleantest.jpg");
            std::shared_ptr<Mesh> basicInfantrie_mesh = meshManager.resource("car.obj");

            Entity& basicInfantrie = scene.assign("basicInfantrie");
            basicInfantrie.assign<SpatialComponent>(position);
            basicInfantrie.assign<ModelComponent>(basicInfantrie_mesh, basicInfantrie_tex);
            basicInfantrie.assign<AttackComponent>(1, 20);
            basicInfantrie.assign<OwnerComponent>(playernumber);
            basicInfantrie.assign<HealthComponent>(150);
            basicInfantrie.assign<CurrencyComponent>(basicInfanteriePrice);
            currencyEngine.currency -= basicInfanteriePrice;

            NetworkPacket np(basicInfantrie.id, BUILD);
            np.set(0, B_INFANTRY);
            np.set(1, position.x);
            np.set(2, position.y);
            np.set(3, position.z);

            infantryTimer = 0;
            // TODO add other components
        } else {
            std::cout << "Not enough money for basic infantrie" << std::endl;
        }
    }
}

void Gameplay::createAdvancedInfantrie()
{
    std::cout << 5-infantryTimer << std::endl;
    if (infantryTimer > 5) {
        //TODO: implementation
        //...

        //NetworkPacket np(0, mode[BUILD]);
        //np.set(0, unit[A_INFANTRY]);
        //np.set(1, position.x);
        //np.set(2, position.y);
        //np.set(3, position.z);

        infantryTimer = 0;
    }
    // TODO
}

void Gameplay::createEngineer()
{
    std::cout << 5-infantryTimer << std::endl;
    if (infantryTimer > 5) {
        //TODO: implementation
        //...
        infantryTimer = 0;
    }
    // TODO
}

void Gameplay::buildCentralIntelligenceCore(glm::vec3 position)
{
    position.y = 0.0;
    std::shared_ptr<Mesh> CentralIntelligenceCore_mesh = meshManager.resource("ciCore.obj");
    std::shared_ptr<Texture> CentralIntelligenceCore_tex = textureManager.resource("house1.bmp");

    Entity& ciCore = scene.assign("ACiCore");
    ciCore.assign<SpatialComponent>(glm::vec3{-7,0,7});
    ciCore.assign<ModelComponent>(CentralIntelligenceCore_mesh, CentralIntelligenceCore_tex);
    ciCore.assign<EnergyComponent>(150);
    ciCore.assign<HealthComponent>(1);
    ciCore.assign<OwnerComponent>(1);

    Entity& cCore = scene.assign("BCiCore");
    cCore.assign<SpatialComponent>(glm::vec3{7,0,-7});
    cCore.assign<ModelComponent>(CentralIntelligenceCore_mesh, CentralIntelligenceCore_tex);
    cCore.assign<EnergyComponent>(150);
    cCore.assign<HealthComponent>(1);
    cCore.assign<OwnerComponent>(2);
}


void Gameplay::buildOrbitalDropBeacon(glm::vec3 position)
{
    std::cout << 5-buildingTimer << std::endl;
    if ( buildingTimer > 5) {
         if (currencyEngine.currency >= orbitalDropBeaconPrice) {
            position.y = 0.0;
            std::shared_ptr<Texture> t = textureManager.resource("wooden-crate.jpg");
            std::shared_ptr<Mesh> house_mesh = meshManager.resource("house.obj");

            Entity& house = scene.assign("OrbitalDropBeacon");
            house.assign<SpatialComponent>(position);
            house.assign<ModelComponent>(house_mesh, t);
            house.assign<EnergyComponent>(-100);
            house.assign<OwnerComponent>(playernumber);
            house.assign<HealthComponent>(250);
            house.assign<CurrencyComponent>(orbitalDropBeaconPrice);

            currencyEngine.currency -= orbitalDropBeaconPrice;
            buildingTimer = 0;

        } else {
            std::cout << "Not enough money for Orbital drop beacon" << std::endl;
        }
    }
}

void Gameplay::buildPowerCore()
{

}

void Gameplay::buildAcademyOfAdvancedTechnologies()
{
    std::cout << buildingTimer << std::endl;
    if ( buildingTimer > 5) {
        //TODO: implementation
        //...
        buildingTimer = 0;
    }
}

bool Gameplay::centralIntelligenceCoreDestoyed()
{
    if (playernumber == 1 && scene.containsEntity("ACiCore4")) {
        return false;
    }
    else if(playernumber == 2 && scene.containsEntity("BCiCore5")) {
        return false;
    }

    return true;
}

void Gameplay::winGame()
{
    NetworkPacket np(0, WIN_LOSE);
    np.set(0, 0);
    client->write(np.char_array(), np.size());

    std::shared_ptr<Text> winningText = std::make_shared<Text>("YOU ARE VICTORIOUS", glm::vec2{10, 580}, 20);
    renderEngine.addText(winningText);
}

void Gameplay::loseGame()
{
    std::shared_ptr<Text> losingText = std::make_shared<Text>("YOU ARE DEFEATED", glm::vec2{10, 580}, 20);
    renderEngine.addText(losingText);
}

void Gameplay::sellEntity(Entity* aEntity)
{
    if(aEntity != nullptr) {
        auto light = aEntity->component<LightComponent>();
        if ( light == nullptr ) {
            auto healthComponent = aEntity->component<HealthComponent>();
            if ( healthComponent  != nullptr) {
                auto currencyComponent = aEntity->component<CurrencyComponent>();
                if ( currencyComponent   != nullptr) {
                    double healthPercentage =  healthComponent->health;
                    healthPercentage /= healthComponent->startHealth;
                    currencyEngine.currency += currencyComponent->price * healthPercentage;
                    scene.deleteEntity(aEntity);
                    currentSelectedUnit = nullptr;
                }
            }
        }
    }
}

void Gameplay::moveEntity() {
    Entity* aEntity = getCurrentSelectedEntity();
    if(aEntity != nullptr ) {
        auto owner = aEntity->component<OwnerComponent>();
        if(owner != nullptr) {
            if(owner->playerNumber == playernumber) {
                auto spatial = aEntity->component<SpatialComponent>();
                glm::vec3 newPos = renderEngine.get3DPositionFromMousePosition();
                newPos.y = 0;
                spatial->set_position(newPos);

                NetworkPacket np(aEntity->id, MOVE);
                np.set(0, newPos.x);
                np.set(1, newPos.y);
                np.set(2, newPos.z);

                client->write(np.char_array(), np.size());
            }
        }
    }
}

void Gameplay::moveEntity(glm::vec3 position, int id) {
    Entity* aEntity = scene.getEntity(id);

    if(aEntity != nullptr ) {
        auto spatial = aEntity->component<SpatialComponent>();
        glm::vec3 newPos = position;
        newPos.y = 0;
        spatial->set_position(newPos);
    }
}

void Gameplay::attackEntity()
{
    Entity* attacking_unit = getCurrentSelectedEntity();
    Entity* to_be_attacked = getEntityAtPosition(renderEngine.GetTilePosition());
    if(attacking_unit && to_be_attacked && attacking_unit != to_be_attacked)
    {
        if(attacking_unit->component<AttackComponent>() && to_be_attacked->component<HealthComponent>())
        {
            std::cout << "Unit: " << attacking_unit->name << " is attacking: " << to_be_attacked->name << std::endl;
            attackEngine.attack(*to_be_attacked, *attacking_unit);

            NetworkPacket np(attacking_unit->id, ATTACK);
            np.set(0, to_be_attacked->id);
        }
    }
}

void Gameplay::attackEntity(int id_attacking_unit, int id_to_be_attacked)
{
    Entity* attacking_unit = scene.getEntity(id_attacking_unit);
    Entity* to_be_attacked = scene.getEntity(id_to_be_attacked);

    if (attacking_unit && to_be_attacked && attacking_unit != to_be_attacked)
    {
        if (attacking_unit->component<AttackComponent>() && to_be_attacked->component<HealthComponent>())
        {
            attackEngine.attack(*to_be_attacked, *attacking_unit);
        }
    }
}

void Gameplay::updateSelectedEntity(glm::vec3 position)
{
    currentSelectedUnit = getEntityAtPosition(position);
}

Entity* Gameplay::getEntityAtPosition(glm::vec3 position)
{
    double distance = 2.5f;
    Entity* theEntity = nullptr;

    for (auto& entitiesEntry : scene.entities)
    {
        std::unique_ptr<Entity>& entity = entitiesEntry.second;
        auto test = entity->component<SpatialComponent>();
        double distanceBetween = glm::distance(test->get_position(), position);

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

void Gameplay::switchOwner(int owner) {
    playernumber = owner;
}

void Gameplay::updateTimer(float delta) {
    infantryTimer += delta;
    buildingTimer += delta;
}

void Gameplay::setTileMap(TileMap* tilemap)
{
    if (tilemap) {
        tileMap = tilemap;
        renderEngine.setTileMap(tileMap);
    }
}
