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

#include "SpatialComponent.h"
#include "ModelComponent.h"
#include "ListenerComponent.h"
#include "CameraComponent.h"
#include "LightComponent.h"
#include "EnergyComponent.h"
#include "OwnerComponent.h"

#include "RTSCameraBehavior.h"
#include "EnergyBehaviour.h"

#include "CurrencyEngine.h"
namespace Ymir
{
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

            static const int INFTIMER = 3;
            static const int BUILDTIMER = 3;

            float infantryTimer, buildingTimer;
            int unitIdentifyCounter;
            TileMap* tileMap;

        public:
            int objectOwner;

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

            Gameplay(EngineManager& engineManager, CurrencyEngine& currencyEngine, ResourceManager<Texture>& textureManager, ResourceManager<Mesh>& meshManager, RenderEngine& renderEngine, glm::vec2 screenSize);

            void createCamera();

            void drawGrid(bool);

            void createWorker(glm::vec3 position);
            void createBasicInfantrie(glm::vec3 position);
            void createAdvancedInfantrie();
            void createEngineer();

            void buildCentralIntelligenceCore(glm::vec3 position);
            void buildOrbitalDropBeacon(glm::vec3 position);
            void buildPowerCore();
            void buildAcademyOfAdvancedTechnologies();

            void sellEntity(Entity* aEntity);
            void moveEntity();

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

            void setAOE();
    };
};
