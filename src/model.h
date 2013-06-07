#pragma once

#include "game_object.h"
#include "Mesh.h"

class model : public game_object
{
public:
    explicit model(const std::string& name, std::shared_ptr<Ymir::Mesh> mesh, const glm::vec3& position = glm::vec3(0.0f));
    
    virtual void accept(game_object_visitor& v);
    
    std::shared_ptr<Ymir::Mesh> mesh();
private:
    std::shared_ptr<Ymir::Mesh> _mesh;
};