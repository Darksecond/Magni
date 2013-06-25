#pragma once

#include "behaviour.h"

class door_behaviour : public behaviour
{
    float _height;
    bool _moving;
public:
    door_behaviour();
    virtual void update();
    virtual void on_collision(game_object& other);
};