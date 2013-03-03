#pragma once

#include <memory>

#include "Entity.h"
#include "SphereColliderComponent.h"
#include "SpatialComponent.h"

namespace Ymir
{
    class Collider
    {
    public:
        const Entity& entity;
        Collider(const Entity& entity);
        static std::unique_ptr<Collider> fromEntity(const Entity& e);
        static bool needsComponent(const BaseComponent::Type type);
        bool operator==(const Collider& other) const;
        
        bool collides(Collider& other);
    };
};