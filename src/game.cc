#include "game.h"

#include "game_object.h"
#include "model.h"
#include "camera.h"
#include "light.h"
#include "spatial_updater_visitor.h"
#include "time.h"
#include "fps.h"
#include "fpscam_behaviour.h"

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
    
    auto cam = std::make_shared<camera>("camera", SCREEN_SIZE.x/SCREEN_SIZE.y);
    cam->set_behaviour(std::move(std::unique_ptr<fpscam_behaviour>(new fpscam_behaviour(*cam))));
    _world->add(cam);
    
    std::shared_ptr<Ymir::Texture> tex = textureManager.resource("wooden-crate.jpg"); //replace with better texture
    std::shared_ptr<material> mat = std::make_shared<material>(tex);
    
    Ymir::Mesh mesh = std::move(Ymir::Mesh::cube());
    std::shared_ptr<Ymir::Mesh> cube_mesh = std::make_shared<Ymir::Mesh>(std::move(mesh));
    auto cube_model = std::make_shared<model>("cube", cube_mesh, mat, glm::vec3(5.0f, 0.0f, 0.0f));
    _world->add(cube_model);
    
    _world->add(std::make_shared<light>("light", glm::vec3(0.0f, 1.0f, 0.0f)));
}

void game::run()
{
    while(_running)
    {
        time::instance().step();
        fps::instance().update();
        //TODO moar!
        
        _world->update();
        
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