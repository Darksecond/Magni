#include "SpatialComponent.h"

#include <glm/gtc/matrix_transform.hpp>

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


glm::mat4 SpatialComponent::matrix()
{
    glm::mat4 matrix = glm::mat4_cast(direction);
    matrix = glm::translate(matrix, -position);
    return matrix;
}