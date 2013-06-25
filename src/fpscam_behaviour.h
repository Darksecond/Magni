#pragma once

#include "behaviour.h"

#include <GLM/glm.hpp>

class fpscam_behaviour : public behaviour
{
public:
    fpscam_behaviour();
    virtual void update();
private:
    glm::vec2 _angles;
};