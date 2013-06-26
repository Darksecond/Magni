#pragma once

#include "door_behaviour.h"

class blue_door_behaviour : public door_behaviour
{
public:
    blue_door_behaviour() { _key = key_event::key_t::blue; }
};