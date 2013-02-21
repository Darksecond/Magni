#include "EngineManager.h"
#include "Entity.h"
#include "Engine.h"

using namespace Ymir;

void EngineManager::registerEntity(Entity& entity)
{
    for(auto& engine : engines)
    {
        engine->registerEntity(entity);
    }
}

void EngineManager::unregisterEntity(Entity& entity)
{
    for(auto& engine : engines)
    {
        engine->unregisterEntity(entity);
    }
}

void EngineManager::addComponent(Entity& entity, const BaseComponent::Type& component_type)
{
    for(auto& engine : engines)
    {
        engine->addComponent(entity, component_type);
    }
}

void EngineManager::update(double delta)
{
    //TODO introduce passes
    for(auto& engine : engines)
    {
        engine->update(delta);
    }
}

void EngineManager::assign(std::unique_ptr<Engine> engine)
{
    engines.push_back(std::move(engine));
}