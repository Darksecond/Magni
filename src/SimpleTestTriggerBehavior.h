#pragma once

#include "Behavior.h"

namespace Ymir
{
    class SimpleTestTriggerBehavior : public Behavior
    {
        double time = 0.0;
        double triggertime = 0.0;
        double timer = 0.0;
        bool running = false;
    public:
        virtual void update(double delta);
        virtual void receive(message_type_t type, const Entity& trigger);
    };
};