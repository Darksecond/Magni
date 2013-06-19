#pragma once

#include "abstract_scene_builder.h"
#include <queue>
#include "Mesh.h"
#include "material.h"

class scene;
class scene_builder : public abstract_scene_builder
{
public:
    scene_builder(float aspect_ratio);
    
    virtual abstract_scene_builder& camera(const std::string& name, const glm::vec3& position);
    
    virtual abstract_scene_builder& light(const std::string& name, const glm::vec3& position);
    virtual abstract_scene_builder& radius(float r);
    
    virtual abstract_scene_builder& model(const std::string& name, const glm::vec3& position);
    virtual abstract_scene_builder& mesh(const std::string& mesh_name);
    virtual abstract_scene_builder& material(const std::string& material_name);
    
    virtual abstract_scene_builder& group(const std::string& name);
    
    virtual abstract_scene_builder& end();
    virtual std::shared_ptr<scene> get_scene();
private:
    enum class node_type
    {
        world,
        light,
        camera,
        model,
    };
    
    struct node
    {
        node_type type;
        int depth;
        std::string name;
        glm::vec3 position;
        float radius;
        std::shared_ptr<Ymir::Mesh> mesh;
        std::shared_ptr<::material> material;
    };
    
    std::queue<node> _queue;
    int _depth;
    const float _aspect_ratio;
};