#pragma once

#include "door_behaviour.h"

class red_door_behaviour : public door_behaviour
{
public:
    red_door_behaviour() { _key = key_event::key_t::red; }
};