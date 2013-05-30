#pragma once

#include <memory>
#include <iostream>
#include <string>

#include "Scene.h"
#include "EngineManager.h"
#include "Entity.h"
#include "ResourceManager.h"
#include "DirectoryManifest.h"
#include "Texture.h"
#include "Mesh.h"
#include "ProgramResourceLoader.h"
#include "CubemapResourceLoader.h"
#include "Audio/Buffer.h"
#include "Entity.h"
#include "Renderer/RenderEngine.h"
#include "AttackEngine.h"

#include "SpatialComponent.h"
#include "ModelComponent.h"
#include "ListenerComponent.h"
#include "CameraComponent.h"
#include "LightComponent.h"
#include "EnergyComponent.h"
#include "OwnerComponent.h"
#include "NetworkPacket.h"
#include "Client.h"
#include "RTSCameraBehavior.h"
#include "EnergyBehaviour.h"
#include "CurrencyEngine.h"
#include "GameHUDEngine.h"

namespace Ymir
{
    class Client;

    class Gameplay
    {
        private:
            ResourceManager<Texture>& textureManager;
            ResourceManager<Mesh>& meshManager;
            Scene scene;
            glm::vec2 screenSize;
            Entity* currentSelectedUnit;
            RenderEngine& renderEngine;
            CurrencyEngine& currencyEngine;
            AttackEngine& attackEngine;

            static const int INFTIMER = 3;
            static const int BUILDTIMER = 3;
            static const int ATTACKTIMER = 1;

            //hacky
            Entity* myCamera;

            bool barracksBuild, workerBuild;
            float infantryTimer, buildingTimer, myAttackTimer;
            int unitIdentifyCounter, myCoreID;
            TileMap* tileMap;

        public:
            GameHUDEngine* ghe = nullptr;
        Client* client;
            Entity* currentlyBuildingEntity = nullptr;
            int playerNumber, otherPlayerNumber;

            int workerPrice;
            int workerEnergy;

            int basicInfanteriePrice;
            int basicInfantryEnergy;

            int advancedInfantryPrice;
            int advancedInfantryEnergy;

            int powerCorePrice;
            int powerCoreEnergy;

            int orbitalDropBeaconPrice;
            int orbitalDropBeaconEnergy;

            enum {BUILD = 0, MOVE, WIN_LOSE, SELL, ATTACK, HELLO, PLAYER};
            enum {WORKER = 0, B_INFANTRY, A_INFANTRY, ENGINEER, ORBITAL, POWERCORE, ACADEMY, CICORE};

            Gameplay(EngineManager& engineManager, CurrencyEngine& currencyEngine, ResourceManager<Texture>& textureManager, ResourceManager<Mesh>& meshManager, RenderEngine& renderEngine, glm::vec2 screenSize, AttackEngine& attackEngine);

            void createCamera();
            void updateCameraStart();

            void drawGrid(bool);

            void createWorker();
            void processBuildingUnits(bool);
            void createWorker(glm::vec3 position);
            void createGhostWorker(glm::vec3 position, int id);
            void createBasicInfantrie(glm::vec3 position);
            void createGhostBasicInfantrie(glm::vec3 position, int id);
            void createAdvancedInfantrie(glm::vec3 position);
            void createGhostAdvancedInfantrie(glm::vec3 position, int id);
            void createEngineer(glm::vec3 position);
            void createGhostEngineer(glm::vec3 position, int id);

            void buildCentralIntelligenceCore();
            void buildGhostCentralIntelligenceCore(glm::vec3 position, int id);
            void buildOrbitalDropBeacon(glm::vec3 position);
            void buildGhostOrbitalDropBeacon(glm::vec3 position, int id);
            void buildPowerCore(glm::vec3 position);
            void buildGhostPowerCore(glm::vec3 position, int id);
            void buildAcademyOfAdvancedTechnologies(glm::vec3 position);
            void buildGhostAcademyOfAdvancedTechnologies(glm::vec3 position, int id);

            void sellEntity(Entity* aEntity);
            void removeEntity(int id);
            void moveEntity();
            void moveEntity(glm::vec3 position, int id);

            void attackEntity();
            void attackEntityLocal(int id_attacking_unit, int id_to_be_attacked);
            void attackEntity(int attacking_unit, int to_be_attacked);
            void automaticAttackCheck();

            bool centralIntelligenceCoreDestoyed();
            bool enemyCentralIntelligenceCoreDestroyed();
            void winGame();
            void loseGame();
            void switchOwner(int owner);

            void updateSelectedEntity(glm::vec3 position);
            void updateSelectedEntity(Entity* entity);
            Entity* getCurrentSelectedEntity();
            Scene& getScene();

            void updateTimer(float delta);

            void setTileMap(TileMap*);

            void setAOE(bool reset = false);
    };
};
