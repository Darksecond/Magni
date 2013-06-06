#pragma once

#include "game_object.h"

class light : public game_object
{
public:
    explicit light(const std::string& name, const glm::vec3& position = glm::vec3(0.0f));
    
    virtual void accept(game_object_visitor& v);
};