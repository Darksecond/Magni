#include "SpatialComponent.h"

SpatialComponent::SpatialComponent(glm::vec3 pos) : position{pos}, direction{}
{
}

glm::quat& SpatialComponent::directionQuat()
{
    return direction;
}

glm::vec3 SpatialComponent::directionEuler()
{
    return glm::eulerAngles(direction);
}

void SpatialComponent::setDirection(glm::quat& quat)
{
    direction = quat;
}

void SpatialComponent::setDirection(glm::vec3& euler)
{
    direction = glm::quat{glm::radians(euler)};
}