#include "BehaviorEngine.h"

#include "Entity.h"

using namespace Ymir;

void BehaviorEngine::registerEntity(Entity& entity)
{
    entities.push_back(&entity);
}

void BehaviorEngine::unregisterEntity(Entity& entity)
{
    entities.remove(&entity);
}

void BehaviorEngine::addComponent(Entity& entity, const BaseComponent::Type& component_type)
{
}

void BehaviorEngine::update(int pass, double delta)
{
    if(pass == 0)
    {
        for(auto& entity : entities)
        {
            entity->update(delta);
        }
    }
}