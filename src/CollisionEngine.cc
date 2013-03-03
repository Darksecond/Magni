#include "CollisionEngine.h"

#include <iostream>

using namespace Ymir;

void CollisionEngine::registerEntity(Entity& entity)
{
    colliders.registerEntity(entity);
}

void CollisionEngine::unregisterEntity(Entity& entity)
{
    colliders.unregisterEntity(entity);
}

void CollisionEngine::addComponent(Entity& entity, const BaseComponent::Type& component_type)
{
    colliders.addComponent(entity, component_type);
}

void CollisionEngine::update(int pass, double delta)
{
    if(pass == 0)
    {
        for(auto it = colliders.begin(); it != colliders.end(); ++it)
        {
            for(auto inner_it = it + 1; inner_it != colliders.end(); ++inner_it)
            {
                if(it->get()->collides(*inner_it->get()))
                {
                    //send message to both entities (depending on is trigger)
                    it->get()->entity.receive(message_type_t::COLLISION);
                    inner_it->get()->entity.receive(message_type_t::COLLISION);
                }
            }
        }
    }
}