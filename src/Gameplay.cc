#include "Gameplay.h"
#include "AttackComponent.h"
#include "HealthComponent.h"
#include "AOEComponent.h"
#include "Tile.h"

#include <iostream>
#include <sstream>

using namespace Ymir;

Gameplay::Gameplay(EngineManager& engineManager, CurrencyEngine& currencyEngine, ResourceManager<Texture>& textureManager, ResourceManager<Mesh>& meshManager,
    RenderEngine& renderEngine, glm::vec2 screenSize, AttackEngine& attackEngine)
        : scene(engineManager), currencyEngine(currencyEngine) ,textureManager(textureManager), meshManager(meshManager),
            renderEngine(renderEngine), screenSize(screenSize) ,playerNumber(1),currentSelectedUnit(nullptr),workerPrice(80),basicInfanteriePrice(150),orbitalDropBeaconPrice(250), infantryTimer(0), buildingTimer(0), unitIdentifyCounter(0), attackEngine(attackEngine)
{
    client = new Client();
    client->gp = this;

    client->setIPAdress(192, 168, 0, 2);
    //client->setIPAdress(127, 0, 0, 1);

    playerNumber = 1;
}

void Gameplay::createCamera()
{
    Entity& camera = scene.assign("camera");
    camera.assign<ListenerComponent>();
    camera.assign<CameraComponent>(screenSize.x / screenSize.y);

    auto& c_s = camera.assign<SpatialComponent>(glm::vec3{-7,5,10});

    if(playerNumber == 2)
        c_s.set_position(glm::vec3{7,5,-10});

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
    std::cout << 3-infantryTimer << std::endl;
    if (infantryTimer > INFTIMER) {
        if(currencyEngine.currency >= workerPrice) {
            position.y = 0.3;
            std::shared_ptr<Texture> worker_tex = textureManager.resource("workertex.png");
            std::shared_ptr<Mesh> worker_mesh = meshManager.resource("worker.obj");

            Entity& worker = scene.assign("worker");
            worker.assign<SpatialComponent>(position);
            worker.assign<ModelComponent>(worker_mesh, worker_tex);
            worker.assign<HealthComponent>(5);
            worker.assign<CurrencyComponent>(workerPrice);
            worker.assign<OwnerComponent>(playerNumber);

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
    worker.assign<HealthComponent>(5);
    worker.assign<CurrencyComponent>(workerPrice);
    worker.assign<OwnerComponent>(otherPlayerNumber);

    std::cout << "Builded a unit via network with ID: " << id << std::endl;
}

void Gameplay::createBasicInfantrie(glm::vec3 position)
{
    std::cout << 3-infantryTimer << std::endl;
        if (infantryTimer > INFTIMER) {
        if(currencyEngine.currency >= basicInfanteriePrice) {
            position.y = 0.0;
            std::shared_ptr<Texture> basicInfantrie_tex = textureManager.resource("truck_color_cleantest.jpg");
            std::shared_ptr<Mesh> basicInfantrie_mesh = meshManager.resource("car.obj");

            Entity& basicInfantrie = scene.assign("basicInfantrie");
            basicInfantrie.assign<SpatialComponent>(position);
            basicInfantrie.assign<ModelComponent>(basicInfantrie_mesh, basicInfantrie_tex);
            basicInfantrie.assign<AttackComponent>(2, 5, 2);
            basicInfantrie.assign<OwnerComponent>(playerNumber);
            basicInfantrie.assign<HealthComponent>(15);
            basicInfantrie.assign<CurrencyComponent>(basicInfanteriePrice);
            basicInfantrie.assign<AOEComponent>(1); //is square
            currencyEngine.currency -= basicInfanteriePrice;

            NetworkPacket np(basicInfantrie.id, BUILD);
            np.set(0, B_INFANTRY);
            np.set(1, position.x);
            np.set(2, position.y);
            np.set(3, position.z);

            client->write(np.char_array(), np.size());

            infantryTimer = 0;
            updateSelectedEntity(position);
            // TODO add other components
        } else {
            std::cout << "Not enough money for basic infantrie" << std::endl;
        }
    }
}

void Gameplay::createGhostBasicInfantrie(glm::vec3 position, int id)
{
    position.y = 0.0;
    std::shared_ptr<Texture> basicInfantrie_tex = textureManager.resource("truck_color_cleantest.jpg");
    std::shared_ptr<Mesh> basicInfantrie_mesh = meshManager.resource("car.obj");

    Entity& basicInfantrie = scene.assign("basicInfantrie", id);
    basicInfantrie.assign<SpatialComponent>(position);
    basicInfantrie.assign<ModelComponent>(basicInfantrie_mesh, basicInfantrie_tex);
    basicInfantrie.assign<OwnerComponent>(otherPlayerNumber);
    basicInfantrie.assign<HealthComponent>(15);
    basicInfantrie.assign<CurrencyComponent>(basicInfanteriePrice);

    std::cout << "Builded a unit via network with ID: " << id << std::endl;
}

void Gameplay::createAdvancedInfantrie(glm::vec3 position)
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

void Gameplay::createGhostAdvancedInfantrie(glm::vec3 position, int id)
{

}

void Gameplay::createEngineer(glm::vec3 position)
{
    std::cout << 3-infantryTimer << std::endl;
    if (infantryTimer > INFTIMER) {
        //TODO: implementation
        //...
        infantryTimer = 0;
    }
    // TODO
}

void Gameplay::createGhostEngineer(glm::vec3 position, int id)
{

}

void Gameplay::buildCentralIntelligenceCore()
{
    std::shared_ptr<Mesh> CentralIntelligenceCore_mesh = meshManager.resource("ciCore.obj");
    std::shared_ptr<Texture> CentralIntelligenceCore_tex = textureManager.resource("ci_core_tex.png");

    if(playerNumber == 1) {
        Entity& ciCore = scene.assign("ACiCore");
        glm::vec3 position = glm::vec3{-7,0,7};
        ciCore.assign<SpatialComponent>(position);
        ciCore.assign<ModelComponent>(CentralIntelligenceCore_mesh, CentralIntelligenceCore_tex);
        ciCore.assign<EnergyComponent>(150);
        ciCore.assign<HealthComponent>(100);
        ciCore.assign<OwnerComponent>(playerNumber);

        myCoreID = ciCore.id;

        NetworkPacket np(ciCore.id, BUILD);
        np.set(0, CICORE);
        np.set(1, position.x);
        np.set(2, position.y);
        np.set(3, position.z);
        client->write(np.char_array(), np.size());
    } else {
        Entity& ciCore = scene.assign("ACiCore");
        glm::vec3 position = glm::vec3{7,0,-7};
        ciCore.assign<SpatialComponent>(position);
        ciCore.assign<ModelComponent>(CentralIntelligenceCore_mesh, CentralIntelligenceCore_tex);
        ciCore.assign<EnergyComponent>(150);
        ciCore.assign<HealthComponent>(100);
        ciCore.assign<OwnerComponent>(playerNumber);

        myCoreID = ciCore.id;

        NetworkPacket np(ciCore.id, BUILD);
        np.set(0, CICORE);
        np.set(1, position.x);
        np.set(2, position.y);
        np.set(3, position.z);
        client->write(np.char_array(), np.size());
    }
}

void Gameplay::buildGhostCentralIntelligenceCore(glm::vec3 position, int id)
{
    std::shared_ptr<Mesh> CentralIntelligenceCore_mesh = meshManager.resource("ciCore.obj");
    std::shared_ptr<Texture> CentralIntelligenceCore_tex = textureManager.resource("ci_core_tex.png");

    if (playerNumber == 1) {
        Entity& cCore = scene.assign("BCiCore", id);
        cCore.assign<SpatialComponent>(position);
        cCore.assign<ModelComponent>(CentralIntelligenceCore_mesh, CentralIntelligenceCore_tex);
        cCore.assign<HealthComponent>(100);
        cCore.assign<OwnerComponent>(otherPlayerNumber);
    } else {
        Entity& cCore = scene.assign("BCiCore", id);
        cCore.assign<SpatialComponent>(position);
        cCore.assign<ModelComponent>(CentralIntelligenceCore_mesh, CentralIntelligenceCore_tex);
        cCore.assign<HealthComponent>(100);
        cCore.assign<OwnerComponent>(otherPlayerNumber);
    }
}

void Gameplay::buildOrbitalDropBeacon(glm::vec3 position)
{
    std::cout << 3-buildingTimer << std::endl;
    if ( buildingTimer > BUILDTIMER) {
         if (currencyEngine.currency >= orbitalDropBeaconPrice) {
            position.y = 0.0;
            std::shared_ptr<Texture> t = textureManager.resource("wooden-crate.jpg");
            std::shared_ptr<Mesh> house_mesh = meshManager.resource("house.obj");

            Entity& house = scene.assign("OrbitalDropBeacon");
            house.assign<SpatialComponent>(position);
            house.assign<ModelComponent>(house_mesh, t);
            house.assign<EnergyComponent>(-100);
            house.assign<OwnerComponent>(playerNumber);
            house.assign<HealthComponent>(40);
            house.assign<CurrencyComponent>(orbitalDropBeaconPrice);

            NetworkPacket np(house.id, BUILD);
            np.set(0, ORBITAL);
            np.set(1, position.x);
            np.set(2, position.y);
            np.set(3, position.z);

            client->write(np.char_array(), np.size());

            currencyEngine.currency -= orbitalDropBeaconPrice;
            buildingTimer = 0;

        } else {
            std::cout << "Not enough money for Orbital drop beacon" << std::endl;
        }
    }
}

void Gameplay::buildGhostOrbitalDropBeacon(glm::vec3 position, int id)
{
    position.y = 0.0;
    std::shared_ptr<Texture> t = textureManager.resource("wooden-crate.jpg");
    std::shared_ptr<Mesh> house_mesh = meshManager.resource("house.obj");

    Entity& house = scene.assign("OrbitalDropBeacon", id);
    house.assign<SpatialComponent>(position);
    house.assign<ModelComponent>(house_mesh, t);
    house.assign<EnergyComponent>(-100);
    house.assign<OwnerComponent>(otherPlayerNumber);
    house.assign<HealthComponent>(40);
    house.assign<CurrencyComponent>(orbitalDropBeaconPrice);

    std::cout << "Builded a unit via network with ID: " << id << std::endl;
}

void Gameplay::buildPowerCore(glm::vec3 position)
{

}

void Gameplay::buildGhostPowerCore(glm::vec3 position, int id)
{

}

void Gameplay::buildAcademyOfAdvancedTechnologies(glm::vec3 position)
    {
        std::cout << 3-buildingTimer << std::endl;
        if ( buildingTimer > 1) {
            //TODO: implementation
            //...
            buildingTimer = 0;
        }
    }

void Gameplay::buildGhostAcademyOfAdvancedTechnologies(glm::vec3 position, int id)
{
}

bool Gameplay::centralIntelligenceCoreDestoyed()
{
    if (scene.getEntity(myCoreID)) {
        return false;
    }

    return true;
}

void Gameplay::winGame()
{
    std::shared_ptr<Text> winningText = std::make_shared<Text>("YOU ARE VICTORIOUS", glm::vec2{10, 580}, 20);
    renderEngine.addText(winningText);
}

void Gameplay::loseGame()
{
    NetworkPacket np(0, WIN_LOSE);
    client->write(np.char_array(), np.size());

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

                    NetworkPacket np(aEntity->id, SELL);
                    client->write(np.char_array(), np.size());
                }
            }
        }
    }
}

void Gameplay::removeEntity(int id) {
    scene.deleteEntity(scene.getEntity(id));
}

void Gameplay::setAOE(bool reset) {
    Entity* aEntity = getCurrentSelectedEntity();
    if(aEntity != nullptr) {
        auto aoe = aEntity->component<AOEComponent>();
        auto spatial = aEntity->component<SpatialComponent>();
        if(aoe != nullptr && spatial != nullptr){
            int radius   = aoe->radius;
            float xStart = spatial->position.x - radius;
            float xEnd   = spatial->position.x + radius;
            float zStart = spatial->position.z - radius;
            float zEnd   = spatial->position.z + radius;
            int xTileLocationStart  = (int) (xStart +10); //Offset to tilemap is 10.
            int xTileLocationEnd    = (int) (xEnd   +10); //Dit is de helft van het aantal rows/collommen
            int zTileLocationStart  = (int) (zStart +10); //
            int zTileLocationEnd    = (int) (zEnd   +10); //

            for(int i = xTileLocationStart; i <= xTileLocationEnd; i++) {
                for(int y = zTileLocationStart; y <= zTileLocationEnd; y++) {
                    if(i >= 0 && i <= 20 && y >= 0 && y <= 20)
                    if(reset)
                        tileMap->setType(i,y,Tile::Type::NONE);
                     else
                        tileMap->setType(i,y,Tile::Type::AOE);
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
            if(owner->playerNumber == playerNumber) {
                setAOE(true);
                auto spatial = aEntity->component<SpatialComponent>();
                glm::vec3 newPos = renderEngine.GetTilePosition();
                newPos.y = 0;
                spatial->set_position(newPos);
                setAOE();
                
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
            client->write(np.char_array(), np.size());
        }
    }
}

void Gameplay::attackEntityLocal(int id_attacking_unit, int id_to_be_attacked)
{
    Entity* attacking_unit = scene.getEntity(id_attacking_unit);
    Entity* to_be_attacked = scene.getEntity(id_to_be_attacked);

    if(attacking_unit && to_be_attacked && attacking_unit != to_be_attacked)
    {
        if(attacking_unit->component<AttackComponent>() && to_be_attacked->component<HealthComponent>())
        {
            std::cout << "Unit: " << attacking_unit->name << " is attacking: " << to_be_attacked->name << std::endl;
            attackEngine.attack(*to_be_attacked, *attacking_unit);

            NetworkPacket np(attacking_unit->id, ATTACK);
            np.set(0, to_be_attacked->id);
            client->write(np.char_array(), np.size());
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

void Gameplay::automaticAttackCheck() {
    for(auto& firstEntityEntry : scene.entities) {
        std::unique_ptr<Entity>& firstEntity = firstEntityEntry.second;
        auto owner = firstEntity->component<OwnerComponent>();
        if(owner != nullptr) {
            if(owner->playerNumber == playerNumber) {
                auto attackcomponent = firstEntity->component<AttackComponent>();

                if ( attackcomponent != nullptr ) {
                    for (auto& secondEntityEntry : scene.entities) {
                        std::unique_ptr<Entity>& secondEntity = secondEntityEntry.second;

                        auto secondowner = secondEntity->component<OwnerComponent>();

                        if(secondowner != nullptr) {
                            if(secondowner->playerNumber == otherPlayerNumber) {
                                auto firstEntitySpatial     = firstEntity->component<SpatialComponent>();
                                auto secondEntitySpatial    = secondEntity->component<SpatialComponent>();
                                auto firstEntityRange       = firstEntity->component<AttackComponent>();

                                if (glm::distance(firstEntitySpatial->get_position(), secondEntitySpatial->get_position()) < firstEntityRange->range) {
                                    attackEntity(firstEntity->id, secondEntity->id);
                                    break;
                                }
                            }
                        }
                    }
                }
            }
        }
    }
}

void Gameplay::updateSelectedEntity(glm::vec3 position)
{
    currentSelectedUnit = getEntityAtPosition(position);
    setAOE();
}

Entity* Gameplay::getEntityAtPosition(glm::vec3 position)
{
    double distance = 2.5f;
    Entity* theEntity = nullptr;

    for (auto& entitiesEntry : scene.entities)
    {
        std::unique_ptr<Entity>& entity = entitiesEntry.second;
        auto test = entity->component<SpatialComponent>();
        if(test == nullptr) break;
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
