#pragma once

#include "Behavior.h"

namespace Ymir
{
    class SimpleTestTriggerBehavior : public Behavior
    {
        double time;
        double triggertime;
        double timer;
        bool running;
    public:
        SimpleTestTriggerBehavior() : time(0.0), triggertime(0.0), timer(0.0), running(false)
        {
        }

        virtual void update(double delta);
        virtual void receive(message_type_t type, const Entity& trigger);
    };
};
