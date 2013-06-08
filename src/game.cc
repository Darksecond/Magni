#include "game.h"

#include "game_object.h"
#include "model.h"
#include "camera.h"
#include "light.h"
#include "spatial_updater_visitor.h"


#include <iostream>

const glm::vec2 SCREEN_SIZE(800, 600);

game::game() : _running(true), _world(), _renderer(SCREEN_SIZE, programManager, meshManager)
{
}

void game::boot(const std::string& resource_dir)
{
    _manifest = std::make_shared<Ymir::DirectoryManifest>(resource_dir);
    
    textureManager.addManifest(_manifest);
    programManager.addManifest(_manifest);
    meshManager.addManifest(_manifest);
    
    _renderer.boot();
}

void game::shutdown()
{
    _renderer.shutdown();
}

void game::build()
{
    _world = std::make_shared<game_object>("world");
    
    _world->add(std::make_shared<camera>("camera", SCREEN_SIZE.x/SCREEN_SIZE.y));
    
    Ymir::Mesh m = std::move(Ymir::Mesh::cube());
    std::shared_ptr<Ymir::Mesh> cube = std::make_shared<Ymir::Mesh>(std::move(m));
    _world->add(std::make_shared<model>("cube", cube, glm::vec3(0.0f, 1.0f, 0.0f)));
    
    _world->add(std::make_shared<light>("light", glm::vec3(0.0f, 1.0f, 0.0f)));
}

void game::run()
{
    while(_running)
    {
        //TODO moar!
        
        spatial_updater_visitor spatial_updater;
        _world->accept(spatial_updater);
        
        if(!_renderer.step(_world))
            stop();
    }
}

void game::stop()
{
    _running = false;
}