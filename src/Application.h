#pragma once

#ifdef __APPLE__
#include <GLEW/glew.h>
#include<GLFW/glfw.h>
#endif

#ifdef _WIN32
#include <GL/glew.h>
#include <GL/GLFW.h>
#endif // _WIN32

#include <GLM/glm.hpp>

#include "DirectoryManifest.h"
#include "ResourceManager.h"
#include "Texture.h"
#include "Program.h"
#include "ProgramResourceLoader.h"
#include "Mesh.h"
#include "Audio/Buffer.h"
#include "Cubemap.h"
#include "CubemapResourceLoader.h"
#include "Gameplay.h"
#include "TileMap.h"
#include "EngineManager.h"
#include "Renderer/RenderEngine.h"
#include "BehaviorEngine.h"
#include "CollisionEngine.h"
#include "AudioEngine.h"
#include "AttackEngine.h"
#include "Client.h"
#include "Server.h"
#include "NetworkPacket.h"

#include "EnergyEngine.h"
#include "CurrencyEngine.h"

namespace Ymir
{
    class Application {
        private:
            std::shared_ptr<DirectoryManifest> manifest;
            ResourceManager<Texture> textureManager;
            ResourceManager<Program, ProgramResourceLoader> programManager;
            ResourceManager<Mesh> meshManager;
            ResourceManager<Audio::Buffer> audioBufferManager;
            ResourceManager<Cubemap, CubemapResourceLoader> cubemapManager;

            Gameplay * gameplay;
            EngineManager * engines;
            RenderEngine * renderEngine;
            AttackEngine * attackEngine;
            CurrencyEngine * currencyEngine;
            glm::vec2 SCREEN_SIZE;
            double lastTime;

        public:
            Application();

            void createManagers(std::string resourceDirectory);
            void createEngines();
            void buildGame();
            void runGame();
    };
};
