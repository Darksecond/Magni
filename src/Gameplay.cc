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
            renderEngine(renderEngine), screenSize(screenSize) ,playerNumber(1),currentSelectedUnit(nullptr),workerPrice(50),basicInfanteriePrice(100),orbitalDropBeaconPrice(100), infantryTimer(0), buildingTimer(0), unitIdentifyCounter(0), attackEngine(attackEngine)
{
    client = new Client();
    client->gp = this;

    ghe = nullptr;
    currentlyBuildingEntity = nullptr;

    //client->setIPAdress(192, 168, 0, 2);
    barracksBuild = true;
    workerBuild = true;
    client->setIPAdress(127, 0, 0, 1);

    playerNumber = 1;
}

void Gameplay::createCamera()
{
    Entity& camera = scene.assign("camera");
    camera.assign<ListenerComponent>();
    camera.assign<CameraComponent>(screenSize.x / screenSize.y);

    auto& c_s = camera.assign<SpatialComponent>(glm::vec3{0,5,0});
    myCamera = &camera;

    glm::vec3 euler{-60,0,0};
    c_s.setDirection(euler);
    camera.assignBehavior(std::unique_ptr<Behavior>{new RTSCameraBehavior(7, 6)});
}

void Gameplay::updateCameraStart() {
    auto camSpatial = myCamera->component<SpatialComponent>();

    if(playerNumber == 1)
        camSpatial->set_position(glm::vec3{-7,5,10});
    else
         camSpatial->set_position(glm::vec3{7,5,-5});
}

void Gameplay::drawGrid(bool draw)
{
    renderEngine.setGrid(draw);
}

void Gameplay::createUnit(const char* mesh, const char* type_name, int price)
{
    if(currencyEngine.currency >= price) {
        ghe->activateGroup("empty");
        if(currentlyBuildingEntity == nullptr)
        {
            std::shared_ptr<Texture> worker_tex = textureManager.resource("building.png");
            std::shared_ptr<Mesh> worker_mesh = meshManager.resource(mesh);
            currentlyBuildingEntity = &scene.assign(type_name);
            currentlyBuildingEntity->assign<SpatialComponent>(renderEngine.GetTilePosition());
            currentlyBuildingEntity->assign<ModelComponent>(worker_mesh, worker_tex);
            currentlyBuildingEntity->assign<OwnerComponent>(playerNumber);
        }
        currencyEngine.currency -= price;
    }
}

void Gameplay::createWorker()
{
    createUnit("worker.obj", "worker", workerPrice);
}

void Gameplay::createOrbitalDropBeacon()
{
    createUnit("house.obj", "OrbitalDropBeacon", orbitalDropBeaconPrice);
}

void Gameplay::createBasicInfantrie()
{
    createUnit("basicInfantry.obj", "basicInfantrie", basicInfanteriePrice);
}

void Gameplay::processBuildingUnits(bool left_pressed)
{
    if(currentlyBuildingEntity == nullptr) return;
    currentlyBuildingEntity->component<SpatialComponent>()->position = renderEngine.GetTilePosition();
    if(left_pressed)
    {
        std::string type = currentlyBuildingEntity->name;
        scene.deleteEntity(currentlyBuildingEntity);
        currentlyBuildingEntity = nullptr;

        if(type == "worker")
            createWorker(renderEngine.GetTilePosition());
        if(type == "OrbitalDropBeacon")
            buildOrbitalDropBeacon(renderEngine.GetTilePosition());
        if(type == "basicInfantrie")
            createBasicInfantrie(renderEngine.GetTilePosition());
    }
}

void Gameplay::createWorker(glm::vec3 position)
{
    std::shared_ptr<Texture> worker_tex = textureManager.resource("worker_blue.png");
    std::shared_ptr<Mesh> worker_mesh = meshManager.resource("worker.obj");

    Entity& worker = scene.assign("worker");
    worker.assign<SpatialComponent>(position);
    worker.assign<ModelComponent>(worker_mesh, worker_tex);
    worker.assign<HealthComponent>(5);
    worker.assign<CurrencyComponent>(workerPrice);
    worker.assign<OwnerComponent>(playerNumber);

    workerBuild = true;

    NetworkPacket np(worker.id, BUILD);
    np.set(0, WORKER);
    np.set(1, position.x);
    np.set(2, position.y);
    np.set(3, position.z);

    client->write(np.char_array(), np.size());
}

void Gameplay::createGhostWorker(glm::vec3 position, int id)
{
    std::shared_ptr<Texture> worker_tex = textureManager.resource("worker_red.png");
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
    position.y = 0.0;
    std::shared_ptr<Texture> basicInfantrie_tex = textureManager.resource("ally.png");
    std::shared_ptr<Mesh> basicInfantrie_mesh = meshManager.resource("basicInfantry.obj");

    Entity& basicInfantrie = scene.assign("basicInfantrie");
    basicInfantrie.assign<SpatialComponent>(position);
    basicInfantrie.assign<ModelComponent>(basicInfantrie_mesh, basicInfantrie_tex);
    basicInfantrie.assign<AttackComponent>(2, 5, 0.5f, 2);
    basicInfantrie.assign<OwnerComponent>(playerNumber);
    basicInfantrie.assign<HealthComponent>(10);
    basicInfantrie.assign<CurrencyComponent>(basicInfanteriePrice);
    basicInfantrie.assign<AOEComponent>(1); //is square
    currencyEngine.currency -= basicInfanteriePrice;

    NetworkPacket np(basicInfantrie.id, BUILD);
    np.set(0, B_INFANTRY);
    np.set(1, position.x);
    np.set(2, position.y);
    np.set(3, position.z);

    client->write(np.char_array(), np.size());
}

void Gameplay::createGhostBasicInfantrie(glm::vec3 position, int id)
{
    position.y = 0.0;
    std::shared_ptr<Texture> basicInfantrie_tex = textureManager.resource("enemy.png");
    std::shared_ptr<Mesh> basicInfantrie_mesh = meshManager.resource("basicInfantry.obj");

    Entity& basicInfantrie = scene.assign("basicInfantrie", id);
    basicInfantrie.assign<SpatialComponent>(position);
    basicInfantrie.assign<ModelComponent>(basicInfantrie_mesh, basicInfantrie_tex);
    basicInfantrie.assign<OwnerComponent>(otherPlayerNumber);
    basicInfantrie.assign<HealthComponent>(10);
    basicInfantrie.assign<AttackComponent>(2, 1, 0.5f, 2);
    basicInfantrie.assign<CurrencyComponent>(basicInfanteriePrice);

    std::cout << "Builded a unit via network with ID: " << id << std::endl;
}

void Gameplay::buildCentralIntelligenceCore()
{
    std::shared_ptr<Mesh> CentralIntelligenceCore_mesh = meshManager.resource("ciCore.obj");
    std::shared_ptr<Texture> CentralIntelligenceCore_tex = textureManager.resource("ally.png");

    if(playerNumber == 1) {
        Entity& ciCore = scene.assign("ACiCore");
        glm::vec3 position = glm::vec3{-7,0,7};
        ciCore.assign<SpatialComponent>(position);
        ciCore.assign<ModelComponent>(CentralIntelligenceCore_mesh, CentralIntelligenceCore_tex);
        ciCore.assign<EnergyComponent>(-50);
        ciCore.assign<HealthComponent>(30);
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
        ciCore.assign<EnergyComponent>(-50);
        ciCore.assign<HealthComponent>(30);
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
    std::shared_ptr<Texture> CentralIntelligenceCore_tex = textureManager.resource("enemy.png");

    if (playerNumber == 1) {
        Entity& cCore = scene.assign("BCiCore", id);
        cCore.assign<SpatialComponent>(position);
        cCore.assign<ModelComponent>(CentralIntelligenceCore_mesh, CentralIntelligenceCore_tex);
        cCore.assign<HealthComponent>(30);
        cCore.assign<OwnerComponent>(otherPlayerNumber);
    } else {
        Entity& cCore = scene.assign("BCiCore", id);
        cCore.assign<SpatialComponent>(position);
        cCore.assign<ModelComponent>(CentralIntelligenceCore_mesh, CentralIntelligenceCore_tex);
        cCore.assign<HealthComponent>(30);
        cCore.assign<OwnerComponent>(otherPlayerNumber);
    }
}

void Gameplay::buildOrbitalDropBeacon(glm::vec3 position)
{
    position.y = 0.0;
    std::shared_ptr<Texture> t = textureManager.resource("ally.png");
    std::shared_ptr<Mesh> house_mesh = meshManager.resource("house.obj");

    Entity& house = scene.assign("OrbitalDropBeacon");
    house.assign<SpatialComponent>(position);
    house.assign<ModelComponent>(house_mesh, t);
    house.assign<EnergyComponent>(-100);
    house.assign<OwnerComponent>(playerNumber);
    house.assign<HealthComponent>(20);
    house.assign<CurrencyComponent>(orbitalDropBeaconPrice);

    barracksBuild = true;

    NetworkPacket np(house.id, BUILD);
    np.set(0, ORBITAL);
    np.set(1, position.x);
    np.set(2, position.y);
    np.set(3, position.z);

    client->write(np.char_array(), np.size());

    currencyEngine.currency -= orbitalDropBeaconPrice;
    buildingTimer = 0;
}

void Gameplay::buildGhostOrbitalDropBeacon(glm::vec3 position, int id)
{
    position.y = 0.0;
    std::shared_ptr<Texture> t = textureManager.resource("enemy.png");
    std::shared_ptr<Mesh> house_mesh = meshManager.resource("house.obj");

    Entity& house = scene.assign("OrbitalDropBeacon", id);
    house.assign<SpatialComponent>(position);
    house.assign<ModelComponent>(house_mesh, t);
    house.assign<EnergyComponent>(-100);
    house.assign<OwnerComponent>(otherPlayerNumber);
    house.assign<HealthComponent>(20);
    house.assign<CurrencyComponent>(orbitalDropBeaconPrice);

    std::cout << "Builded a unit via network with ID: " << id << std::endl;
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
                    if(i >= 0 && i <= 20 && y >= 0 && y <= 20) {
                        if(reset) {
                            if(tileMap->getType(i,y) == Tile::Type::AOE) {
                                tileMap->setType(i,y,Tile::Type::NONE);
                            }
                        } else {
                            if(tileMap->getType(i,y) == Tile::Type::NONE) {
                                tileMap->setType(i,y,Tile::Type::AOE);
                            }
                        }
                    }
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
                std::cout << owner->playerNumber << " " << playerNumber << std::endl;
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
    Entity* to_be_attacked = scene.getEntityAtPosition(renderEngine.GetTilePosition()).get();

    if(attacking_unit && to_be_attacked && attacking_unit != to_be_attacked)
    {
        if(attacking_unit->component<AttackComponent>() && to_be_attacked->component<HealthComponent>())
        {
            auto spatialAttacker = attacking_unit->component<SpatialComponent>();
            auto spatialAttackee = to_be_attacked->component<SpatialComponent>();

            if(spatialAttacker != nullptr || spatialAttackee != nullptr) {
                Laser* laser = new Laser(spatialAttacker->get_position(), spatialAttackee->get_position(), 0.3f);
                lasers.push_back(laser);

                attackEngine.attack(*to_be_attacked, *attacking_unit);

                NetworkPacket np(attacking_unit->id, ATTACK);
                np.set(0, to_be_attacked->id);
                client->write(np.char_array(), np.size());
            }
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
            auto spatialAttacker = attacking_unit->component<SpatialComponent>();
            auto spatialAttackee = to_be_attacked->component<SpatialComponent>();

            if(spatialAttacker != nullptr || spatialAttackee != nullptr) {
                attackEngine.attack(*to_be_attacked, *attacking_unit);

                Laser* laser = new Laser(spatialAttacker->get_position(), spatialAttackee->get_position(), 0.3f);
                lasers.push_back(laser);

                NetworkPacket np(attacking_unit->id, ATTACK);
                np.set(0, to_be_attacked->id);
                client->write(np.char_array(), np.size());
                myAttackTimer = 0;
            }
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
            auto spatialAttacker = attacking_unit->component<SpatialComponent>();
            auto spatialAttackee = to_be_attacked->component<SpatialComponent>();

            if(spatialAttacker != nullptr || spatialAttackee != nullptr) {
                Laser* laser = new Laser(spatialAttacker->get_position(), spatialAttackee->get_position(), 0.3f);
                lasers.push_back(laser);

                attackEngine.attack(*to_be_attacked, *attacking_unit);
            }
        }
    }
}

void Gameplay::automaticAttackCheck() {
    for(auto& firstEntityEntry : scene.entities) {
        std::shared_ptr<Entity>& firstEntity = firstEntityEntry.second;
        auto owner = firstEntity->component<OwnerComponent>();
        if(owner != nullptr) {
            if(owner->playerNumber == playerNumber) {
                auto attackcomponent = firstEntity->component<AttackComponent>();

                if ( attackcomponent != nullptr && attackcomponent->attackTimer <= 0) {
                    for (auto& secondEntityEntry : scene.entities) {
                        std::shared_ptr<Entity>& secondEntity = secondEntityEntry.second;

                        auto secondowner = secondEntity->component<OwnerComponent>();

                        if(secondowner != nullptr) {
                            if(secondowner->playerNumber == otherPlayerNumber) {
                                auto firstEntitySpatial     = firstEntity->component<SpatialComponent>();
                                auto secondEntitySpatial    = secondEntity->component<SpatialComponent>();
                                auto firstEntityRange       = firstEntity->component<AttackComponent>();

                                if (glm::distance(firstEntitySpatial->get_position(), secondEntitySpatial->get_position()) < firstEntityRange->range) {
                                    attackEntityLocal(firstEntity->id, secondEntity->id);
                                    attackcomponent->attackTimer = attackcomponent->attackSpeed;
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
    currentSelectedUnit = scene.getEntityAtPosition(position).get();
    setAOE();
}

void Gameplay::updateSelectedEntity(Entity* entity)
{
    if(entity != currentSelectedUnit)
    {
        currentSelectedUnit = entity;
        setAOE();
    }
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
    myAttackTimer += delta;

    std::vector<Laser*>::iterator iter = lasers.begin();

    while (iter != lasers.end())
    {
        (*iter)->duration -= delta;

        if ((*iter)->duration <= 0)
            iter = lasers.erase(iter);
        else
           ++iter;
    }

    for(auto& entity : scene.entities) {
        auto attackcomponent = entity.second->component<AttackComponent>();

        if(attackcomponent != nullptr)
            attackcomponent->attackTimer -= delta;
    }
}

void Gameplay::updateLaserDataToRenderEngine() {
    renderEngine.setLaserData(lasers);
}

void Gameplay::setTileMap(TileMap* tilemap)
{
    if (tilemap) {
        tileMap = tilemap;
        renderEngine.setTileMap(tileMap);
    }
}
