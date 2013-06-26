#pragma once

#include "key_behaviour.h"

class red_key_behaviour : public key_behaviour
{
public:
    red_key_behaviour() { _key = key_event::key_t::red; }
};