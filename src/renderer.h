#pragma once

#include "ResourceManager.h"
#include "Program.h"
#include "ProgramResourceLoader.h"
#include "Mesh.h"

#include "game_object.h"

#include <GLM/glm.hpp>
#include <memory>

class renderer
{
public:
    renderer(const glm::vec2& screen_size, Ymir::ResourceManager<Ymir::Program, Ymir::ProgramResourceLoader>& programManager, Ymir::ResourceManager<Ymir::Mesh>& meshManager);
    void boot();
    void shutdown();
    
    bool step(std::shared_ptr<game_object> world);
private:
    const glm::vec2 SCREEN_SIZE;
    Ymir::ResourceManager<Ymir::Program, Ymir::ProgramResourceLoader>& _programManager;
    Ymir::ResourceManager<Ymir::Mesh>& _meshManager;
};