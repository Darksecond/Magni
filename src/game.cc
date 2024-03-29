#include "game.h"

#include "game_object.h"
#include "model.h"
#include "camera.h"
#include "light.h"
#include "spatial_updater_visitor.h"
#include "time.h"
#include "fps.h"
#include "fpscam_behaviour.h"
#include "resource_factory.h"
#include "Texture.h"
#include "Program.h"
#include "Mesh.h"
#include "DirectoryManifest.h"
#include "bounding_sphere.h"
#include "scene.h"
#include "scene_builder.h"
#include "scene_director.h"
#include "aabb.h"

#include <iostream>

const glm::ivec2 SCREEN_SIZE(800, 600);

game& game::instance()
{
    static game _instance;
    return _instance;
}

game::game() : _running(true), _renderer(SCREEN_SIZE), _collider()
{
}

void game::boot(const std::string& resource_dir)
{
    _manifest = std::make_shared<Ymir::DirectoryManifest>(resource_dir);
    
    resource_factory::instance().add_manifest(_manifest);
    
    _renderer.boot();
    _collider.boot();
    _audio.boot();
}

void game::shutdown()
{
    _renderer.shutdown();
    _collider.shutdown();
    _audio.shutdown();
}

void game::build()
{
    _renderer.build();
    _collider.build();
    _audio.build();
    
    std::vector<module*> modules;
    modules.push_back(&_renderer);
    modules.push_back(&_collider);
    
    scene_director director(std::make_shared<scene_builder>((float)SCREEN_SIZE.x/SCREEN_SIZE.y, modules));
    
    director.construct("default");
    _active_scene = director.get_scene();
}

void game::run()
{
    while(_running)
    {
        time::instance().step();
        fps::instance().update();
        
        if(_active_scene)
        {
            _active_scene->scene_graph_view()->update();
            
            spatial_updater_visitor spatial_updater;
            _active_scene->scene_graph_view()->accept(spatial_updater);
        }
        
        if(!_renderer.step(_active_scene))
            stop();
        
        if(!_collider.step(_active_scene))
            stop();
        
        if(!_audio.step(_active_scene))
            stop();
        
        _active_scene->remove_to_remove();
    }
}

void game::stop()
{
    _running = false;
}