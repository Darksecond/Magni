#include "Collider.h"

#include <iostream>

using namespace Ymir;

Collider::Collider(const Entity& e) : entity{e}
{
}

std::unique_ptr<Collider> Collider::fromEntity(const Entity& e)
{
    auto spatial = e.component<SpatialComponent>();
    auto sphere = e.component<SphereColliderComponent>();
    if(spatial != nullptr && sphere != nullptr)
    {
        return std::unique_ptr<Collider>{new Collider{e}};
    }
    return nullptr;
}

bool Collider::needsComponent(const BaseComponent::Type type)
{
    if(type == SpatialComponent::type() || type == SphereColliderComponent::type())
        return true;
    return false;
}

bool Collider::operator==(const Collider& other) const
{
    return &entity == &other.entity;
}

bool Collider::collides(Collider& other)
{
    glm::vec3 this_loc = entity.component<SpatialComponent>()->position;
    float rad = entity.component<SphereColliderComponent>()->radius;
    
    glm::vec3 other_loc = other.entity.component<SpatialComponent>()->position;
    float other_rad = other.entity.component<SphereColliderComponent>()->radius;
    
    float distance = glm::distance(this_loc, other_loc);
    if(distance < rad + other_rad)
    {
        return true;
    }
    return false;
}