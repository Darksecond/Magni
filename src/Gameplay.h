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
#include "CurrencyEngine.h"

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

            float infantryTimer, buildingTimer;
            int unitIdentifyCounter;
            TileMap* tileMap;

        public:
            Client* client;
            int playernumber;

            int workerPrice;
            int basicInfanteriePrice;
            int orbitalDropBeaconPrice;

            enum {BUILD = 0, MOVE, WIN_LOSE, SELL, ATTACK};
            enum {WORKER = 0, B_INFANTRY, A_INFANTRY};

            Gameplay(EngineManager& engineManager, CurrencyEngine& currencyEngine, ResourceManager<Texture>& textureManager, ResourceManager<Mesh>& meshManager, RenderEngine& renderEngine, glm::vec2 screenSize, AttackEngine& attackEngine);

            void createCamera();

            void drawGrid(bool);

            void createWorker(glm::vec3 position);
            void createGhostWorker(glm::vec3 position, int id);
            void createBasicInfantrie(glm::vec3 position);
            void createAdvancedInfantrie();
            void createEngineer();

            void buildCentralIntelligenceCore(glm::vec3 position);
            void buildOrbitalDropBeacon(glm::vec3 position);
            void buildPowerCore();
            void buildAcademyOfAdvancedTechnologies();

            void sellEntity(Entity* aEntity);
            void removeEntity(int id);
            void moveEntity();
            void moveEntity(glm::vec3 position, int id);

            void attackEntity();
            void attackEntity(int attacking_unit, int to_be_attacked);

            bool centralIntelligenceCoreDestoyed();
            bool enemyCentralIntelligenceCoreDestroyed();
            void winGame();
            void loseGame();
            void switchOwner(int owner);

            Entity* getEntityAtPosition(glm::vec3 pos);
            void updateSelectedEntity(glm::vec3 position);
            Entity* getCurrentSelectedEntity();
            Scene& getScene();

            void updateTimer(float delta);

            void setTileMap(TileMap*);
    };
};
