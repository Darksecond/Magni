#pragma once

#include "Engine.h"
#include "NodeCache.h"
#include "Collider.h"

namespace Ymir
{
    class CollisionEngine : public Engine
    {
        NodeCache<Collider> colliders;
    public:
        virtual void registerEntity(Entity& entity);
        virtual void unregisterEntity(Entity& entity);
        virtual void addComponent(Entity& entity, const BaseComponent::Type& component_type);
        virtual void update(int pass, double delta);
    };
};