#pragma once

#include "Engine.h"

#include <list>

namespace Ymir
{
    class BehaviorEngine : public Engine
    {
        std::list<Entity*> entities;
    public:
        virtual void registerEntity(Entity& entity);
        virtual void unregisterEntity(Entity& entity);
        virtual void addComponent(Entity& entity, const BaseComponent::Type& component_type);
        virtual void update(int pass, double delta);
    };
};