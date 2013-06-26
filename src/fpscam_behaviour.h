#pragma once

#include "behaviour.h"

#include <GLM/glm.hpp>

class fpscam_behaviour : public behaviour
{
public:
    fpscam_behaviour();
    virtual void update();
    virtual void on_event(event_t type, void* data);
private:
    glm::vec2 _angles;
    bool keys[2];
};