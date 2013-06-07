#pragma once

#include "game_object.h"

//TEMP RESOURCE MANAGEMENT
#include "DirectoryManifest.h"
#include "ResourceManager.h"
#include "Texture.h"
#include "Program.h"
#include "ProgramResourceLoader.h"
#include "Mesh.h"

#include <string>
#include <memory>

class game
{
public:
    game();
    
    void boot(const std::string& resource_dir);
    void build();
    void run();
    void shutdown();
    void stop();
private:
    bool _running;
    std::shared_ptr<game_object> _world;
    
    //TEMP RESOURCE MANAGEMENT
    std::shared_ptr<Ymir::DirectoryManifest> manifest;
    Ymir::ResourceManager<Ymir::Texture> textureManager;
    Ymir::ResourceManager<Ymir::Program, Ymir::ProgramResourceLoader> programManager;
    Ymir::ResourceManager<Ymir::Mesh> meshManager;
};