#pragma once

#include "behaviour.h"

class wall_behaviour : public behaviour
{
public:
    virtual void update();
    virtual void on_collision(game_object& other);
};