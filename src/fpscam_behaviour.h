#pragma once

#include "behaviour.h"

class fpscam_behaviour : public behaviour
{
public:
    fpscam_behaviour(game_object& go);
    virtual void update();
};