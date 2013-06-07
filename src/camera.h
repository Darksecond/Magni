#pragma once

#include "game_object.h"

#include <GLM/glm.hpp>

class camera : public game_object
{
public:
    explicit camera(const std::string& name, const float aspect_ratio, const glm::vec3& position = glm::vec3(0.0f));
    
    virtual void accept(game_object_visitor& v);
    
    const glm::mat4& projection() const;
private:
    glm::mat4 _projection;
};