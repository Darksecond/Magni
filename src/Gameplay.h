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
        public:
            Gameplay(EngineManager& engines, ResourceManager<Texture>& textureManager, ResourceManager<Mesh>& meshManager, RenderEngine& renderEngine, glm::vec2 screenSize);

            void createCamera();

            void createWorker(glm::vec3 position);
            void createBasicInfantrie(glm::vec3 position);
            void createAdvancedInfantrie();
            void createEngineer();

            void buildCentralIntelligenceCore(glm::vec3 position);
            void buildOrbitalDropBeacon(glm::vec3 position);
            void buildPowerCore();
            void buildAcademyOfAdvancedTechnologies();

            void sellEntity(Entity* aEntity);

            void winGame();
            void loseGame();

            Entity* getEntityAtPosition(glm::vec3 pos);
            void updateSelectedEntity(glm::vec3 position);
            Entity* getCurrentSelectedEntity();
            Scene& getScene();
    };
};
