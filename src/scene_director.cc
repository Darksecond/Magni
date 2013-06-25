#include "scene_director.h"

#include "scene_builder.h"
#include "resource_factory.h"

#include <string>
#include <fstream>
#include <sstream>
#include <GLM/glm.hpp>

scene_director::scene_director(std::shared_ptr<scene_builder> builder) : _scene(nullptr), _builder(builder)
{
}

void scene_director::construct(const std::string& identifier)
{
    auto file = resource_factory::instance().manifests().read(identifier + std::string(".lvl"));
    auto& stream = file->stream();
    std::string line;
    while(std::getline(stream, line))
    {
        std::stringstream iss(line);
        
        std::string type;
        iss >> type >> std::ws;
        if(type == "camera")
        {
            std::string name;
            float x, y, z;
            iss >> name >> std::ws >> x >> std::ws >> y >> std::ws >> z;
            glm::vec3 pos(x, y, z);
            _builder->camera(name, pos);
        }
        else if(type == "end")
        {
            _builder->end();
        }
        else if(type == "model")
        {
            std::string name;
            float x, y, z;
            iss >> name >> std::ws >> x >> std::ws >> y >> std::ws >> z;
            glm::vec3 pos(x, y, z);
            _builder->model(name, pos);
        }
        else if(type == "material")
        {
            std::string mat;
            iss >> mat;
            _builder->material(mat);
        }
        else if(type == "mesh")
        {
            std::string mesh;
            iss >> mesh;
            _builder->mesh(mesh);
        }
        else if(type == "light")
        {
            std::string name;
            float x, y, z;
            iss >> name >> std::ws >> x >> std::ws >> y >> std::ws >> z;
            glm::vec3 pos(x, y, z);
            _builder->light(name, pos);
        }
        else if(type == "radius")
        {
            float radius;
            iss >> radius;
            _builder->radius(radius);
        }
        else if(type == "aabb")
        {
            _builder->auto_aabb();
        }
        else if(type == "manual_aabb")
        {
            float x1, x2, y1, y2, z1, z2;
            iss >> x1 >> std::ws >> y1 >> std::ws >> z1 >> std::ws >> x2 >> std::ws >> y2 >> std::ws >> z2;
            glm::vec3 min(x1, y1, z1);
            glm::vec3 max(x2, y2, z2);
            _builder->aabb(std::make_shared<class aabb>(min, max));
        }
        else if(type == "behaviour")
        {
            std::string identifier;
            iss >> identifier;
            _builder->behaviour(identifier);
        }
        else if(type == "active")
        {
            _builder->active();
        }
    }
    
    _scene = _builder->get_scene();
}