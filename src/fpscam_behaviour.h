#pragma once

#include "behaviour.h"

#include <GLM/glm.hpp>

class fpscam_behaviour : public behaviour
{
public:
    fpscam_behaviour(game_object& go);
    virtual void update();
private:
    glm::vec2 _angles;
};