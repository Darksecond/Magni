#pragma once

#include "game_object.h"

//TEMP RESOURCE MANAGEMENT
#include "DirectoryManifest.h"
#include "Texture.h"
#include "Program.h"
#include "Mesh.h"
#include "renderer.h"

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
    std::shared_ptr<Ymir::DirectoryManifest> _manifest;
    
    renderer _renderer;
};