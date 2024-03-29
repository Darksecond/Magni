#include "scene_builder.h"
#include "scene.h"
#include "game_object.h"
#include "camera.h"
#include "light.h"
#include "model.h"
#include "Mesh.h"
#include "resource_factory.h"
#include "material.h"
#include "Texture.h"
#include "behaviour_factory.h"

#include <stdexcept>
#include <stack>
#include <iostream>
#include <memory>

scene_builder::scene_builder(float aspect_ratio, const std::vector<module*>& modules) : _queue(), _depth(0), _aspect_ratio(aspect_ratio), _modules(modules)
{
}

abstract_scene_builder& scene_builder::camera(const std::string& name, const glm::vec3& position)
{
    node n;
    n.depth = _depth++;
    n.type = node_type::camera;
    n.name = name;
    n.position = position;
    n.aabb = nullptr;
    n.auto_aabb = false;
    n.behaviour = nullptr;
    n.active = false;
    _queue.push(std::move(n));
    return *this;
}

abstract_scene_builder& scene_builder::light(const std::string& name, const glm::vec3& position)
{
    node n;
    n.depth = _depth++;
    n.type = node_type::light;
    n.name = name;
    n.position = position;
    n.radius = 1; //default
    n.aabb = nullptr;
    n.auto_aabb = false;
    n.behaviour = nullptr;
    n.active = false;
    _queue.push(std::move(n));
    return *this;
}

abstract_scene_builder& scene_builder::radius(float r)
{
    _queue.back().radius = r;
    return *this;
}

abstract_scene_builder& scene_builder::model(const std::string& name, const glm::vec3& position)
{
    node n;
    n.depth = _depth++;
    n.type = node_type::model;
    n.name = name;
    n.position = position;
    n.mesh = nullptr;
    n.material = nullptr;
    n.aabb = nullptr;
    n.auto_aabb = false;
    n.behaviour = nullptr;
    n.active = false;
    _queue.push(std::move(n));
    return *this;
}

abstract_scene_builder& scene_builder::mesh(const std::string& mesh_name)
{
    _queue.back().mesh = resource_factory::instance().resource<Ymir::Mesh>(mesh_name, "mesh");
    return *this;
}

abstract_scene_builder& scene_builder::material(const std::string& material_name)
{
    _queue.back().material = std::make_shared<::material>(resource_factory::instance().resource<Ymir::Texture>(material_name, "texture"));
    return *this;
}

abstract_scene_builder& scene_builder::aabb(const std::shared_ptr<class aabb>& aabb)
{
    _queue.back().aabb = aabb;
    return *this;
}

abstract_scene_builder& scene_builder::auto_aabb()
{
    _queue.back().auto_aabb = true;
    return *this;
}

abstract_scene_builder& scene_builder::behaviour(const std::string& identifier)
{
    _queue.back().behaviour = behaviour_factory().makeBehaviour(identifier);
    return *this;
}

abstract_scene_builder& scene_builder::active()
{
    _queue.back().active = true;
    return *this;
}

abstract_scene_builder& scene_builder::group(const std::string& name)
{
    node n;
    n.depth = _depth++;
    n.type = node_type::world;
    n.name = name;
    n.position = glm::vec3();
    n.aabb = nullptr;
    n.auto_aabb = false;
    n.behaviour = nullptr;
    n.active = false;
    _queue.push(std::move(n));
    return *this;
}

abstract_scene_builder& scene_builder::end()
{
    if(_depth == 0)
        throw std::runtime_error("depth already at zero");
    
    --_depth;
    
    return *this;
}

std::shared_ptr<scene> scene_builder::get_scene()
{
    std::shared_ptr<scene> s = std::make_shared<scene>();
    std::stack<std::shared_ptr<game_object>> stack;
    
    while(!_queue.empty())
    {
        node& n = _queue.front();
        
        while(n.depth < stack.size())
            stack.pop();
        
        std::shared_ptr<game_object> parent = nullptr;
        if(!stack.empty())
            parent = stack.top();
        
        //create node in scene
        std::shared_ptr<game_object> object = nullptr;
        switch(n.type)
        {
            case node_type::world:
                object = std::make_shared<game_object>(n.name, n.position);
                break;
            case node_type::camera:
                object = std::make_shared<class camera>(n.name, _aspect_ratio, n.position);
                break;
            case node_type::light:
                object = std::make_shared<class light>(n.name, n.position, n.radius);
                break;
            case node_type::model:
            {
                object = std::make_shared<class model>(n.name, n.mesh, n.material, n.position);
                if(n.auto_aabb)
                {
                    n.aabb = std::make_shared<class aabb>(n.mesh->get_aabb());
                }
            }
                break;
        }
        
        if(object)
        {
            object->set_collider(n.aabb);
            if(n.behaviour)
                object->set_behaviour(n.behaviour);
            
            if(n.active)
            {
                for(auto m : _modules)
                    object->add_listener(m);
            }
            
            if(parent)
                s->add_game_object(object, parent);
            else
                s->add_game_object(object);
            
            stack.push(object);
        }
        _queue.pop();
    }
    
    return s;
}
