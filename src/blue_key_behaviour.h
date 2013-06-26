#pragma once

#include "key_behaviour.h"

class blue_key_behaviour : public key_behaviour
{
public:
    blue_key_behaviour() { _key = key_event::key_t::blue; }
};