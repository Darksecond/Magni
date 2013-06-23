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
    }
    
    _scene = _builder->get_scene();
}