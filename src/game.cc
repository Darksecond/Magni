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
}

void game::shutdown()
{
    _renderer.shutdown();
}

void game::build()
{
    _active_scene = std::make_shared<scene>();
    
    auto cam = _active_scene->add_game_object(std::make_shared<camera>("camera", (float)SCREEN_SIZE.x/SCREEN_SIZE.y));
    cam->set_behaviour(std::move(std::unique_ptr<fpscam_behaviour>(new fpscam_behaviour(*cam))));
    cam->set_collider(std::make_shared<bounding_sphere>(1));
    
    std::shared_ptr<Ymir::Texture> tex = resource_factory::instance().resource<Ymir::Texture>("wooden-crate.jpg", "texture");
    std::shared_ptr<material> mat = std::make_shared<material>(tex);
    
    Ymir::Mesh mesh = std::move(Ymir::Mesh::cube());
    std::shared_ptr<Ymir::Mesh> cube_mesh = std::make_shared<Ymir::Mesh>(std::move(mesh));
    
    auto cube_model = _active_scene->add_game_object(std::make_shared<model>("cube", cube_mesh, mat, glm::vec3(5.0f, 0.0f, 0.0f)));
    cube_model->set_collider(std::make_shared<bounding_sphere>(1));
    
    _active_scene->add_game_object(std::make_shared<light>("light1", glm::vec3(-5.0f, 1.0f, 10.0f), 20.0f));
    _active_scene->add_game_object(std::make_shared<light>("light2", glm::vec3(-5.0f, -1.0f, -10.0f), 20.0f));
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
    }
}

void game::stop()
{
    _running = false;
}