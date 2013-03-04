#include "SimpleTestTriggerBehavior.h"

#include <iostream>

#include "Entity.h"

using namespace Ymir;

void SimpleTestTriggerBehavior::update(double delta)
{
}

void SimpleTestTriggerBehavior::receive(message_type_t type, const Entity& trigger)
{
    if(type == message_type_t::TRIGGER)
    {
        std::cout << "TRIGGERED: " << trigger.name << std::endl;
    }
}