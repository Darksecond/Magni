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

#include <stdexcept>
#include <stack>
#include <iostream>

scene_builder::scene_builder(float aspect_ratio) : _queue(), _depth(0), _aspect_ratio(aspect_ratio)
{
}

abstract_scene_builder& scene_builder::camera(const std::string& name, const glm::vec3& position)
{
    node n;
    n.depth = _depth++;
    n.type = node_type::camera;
    n.name = name;
    n.position = position;
    _queue.push(n);
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

abstract_scene_builder& scene_builder::group(const std::string& name)
{
    node n;
    n.depth = _depth++;
    n.type = node_type::world;
    n.name = name;
    n.position = glm::vec3();
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
                object = std::make_shared<class model>(n.name, n.mesh, n.material, n.position);
                break;
        }
        
        if(object)
        {
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
