#pragma once

#include "game_object.h"
#include "Mesh.h"
#include "material.h"

class model : public game_object
{
public:
    model(const std::string& name, std::shared_ptr<Ymir::Mesh> mesh, std::shared_ptr<material> m, const glm::vec3& position = glm::vec3(0.0f));
    
    virtual void accept(game_object_visitor& v);
    
    std::shared_ptr<Ymir::Mesh> mesh();
    std::shared_ptr<material> material();
private:
    std::shared_ptr<Ymir::Mesh> _mesh;
    std::shared_ptr<class material> _material;
};