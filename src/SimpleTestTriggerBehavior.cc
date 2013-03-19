#include "SimpleTestTriggerBehavior.h"

#include <iostream>

#include "Entity.h"

using namespace Ymir;

void SimpleTestTriggerBehavior::update(double delta)
{
    time += delta;
    if(running)
        timer += delta;
    //std::cout << timer << std::endl;
}

void SimpleTestTriggerBehavior::receive(message_type_t type, const Entity& trigger)
{
    if(type == message_type_t::TRIGGER)
    {
        if(time - triggertime > 1)
            running = !running;
        triggertime = time;
        std::cout << "TRIGGERED: " << trigger.name << std::endl;
    }
}
