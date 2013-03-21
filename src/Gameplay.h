#pragma once

#include <memory>

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

#include "SpatialComponent.h"
#include "ModelComponent.h"
#include "ListenerComponent.h"
#include "CameraComponent.h"
#include "LightComponent.h"

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
        public:
            Gameplay(EngineManager& engines, ResourceManager<Texture>& textureManager, ResourceManager<Mesh>& meshManager, glm::vec2 screenSize);

            void createCamera();

            void createWorker(glm::vec3 position);
            void createBasicInfantrie(glm::vec3 position);
            void createAdvancedInfantrie();
            void createEngineer();

            Scene& getScene();
    };
};


