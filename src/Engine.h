#pragma once

#include "Component.h"

#include <memory>

namespace Ymir
{
    class Entity;
    
    class Engine
    {
    public:
        virtual void registerEntity(Entity& entity) = 0;
        virtual void unregisterEntity(Entity& entity) = 0;
        virtual void addComponent(Entity& entity, const BaseComponent::Type& component_type) = 0;
        virtual void update(double delta) = 0;
    };
};