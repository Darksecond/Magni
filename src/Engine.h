#pragma once

#include <memory>

#include "Entity.h"

namespace Ymir
{
    class Engine
    {
    public:
        virtual void registerEntity(Entity& entity) = 0;
        virtual void unregisterEntity(Entity& entity) = 0;
        virtual void addComponent(Entity& entity, const BaseComponent::Type& component_type) = 0;
        virtual void execute() = 0;
    };
};