#include "EngineManager.h"
#include "Entity.h"
#include "Engine.h"

using namespace Ymir;

void EngineManager::registerEntity(Entity& entity)
{
    for(auto& engine : engines)
    {
        engine.second->registerEntity(entity);
    }
}

void EngineManager::unregisterEntity(Entity& entity)
{
    for(auto& engine : engines)
    {
        engine.second->unregisterEntity(entity);
    }
}

void EngineManager::addComponent(Entity& entity, const BaseComponent::Type& component_type)
{
    for(auto& engine : engines)
    {
        engine.second->addComponent(entity, component_type);
    }
}

void EngineManager::update(int pass, double delta)
{
    //TODO introduce passes
    for(auto& engine : engines)
    {
        engine.second->update(pass, delta);
    }
}