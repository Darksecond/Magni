#pragma once

#include "behaviour.h"

class key_behaviour : public behaviour
{
public:
    virtual void update();
    virtual void on_collision(game_object& other);
protected:
    key_event::key_t _key;
};