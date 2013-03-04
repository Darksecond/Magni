#pragma once

#include "Behavior.h"

namespace Ymir
{
    class SimpleTestTriggerBehavior : public Behavior
    {
    public:
        virtual void update(double delta);
        virtual void receive(message_type_t type, const Entity& trigger);
    };
};