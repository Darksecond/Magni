#include "SpatialComponent.h"

#include <GLM/glm.hpp>
#include <GLM/gtc/matrix_transform.hpp>

using namespace Ymir;

SpatialComponent::SpatialComponent(glm::vec3 pos) : position{pos}, direction{}, scale{1.0, 1.0, 1.0}, parent{nullptr}
{
}

const glm::vec3 SpatialComponent::directionEuler() const
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


glm::mat4 SpatialComponent::matrix() const
{
    glm::mat4 matrix = glm::mat4();
    matrix = glm::translate(matrix, -position);
    matrix = matrix * glm::mat4_cast(direction);
    matrix = glm::scale(matrix, scale);
    
    if(parent == nullptr)
        return matrix;
    else
        return parent->matrix() * matrix;
}

glm::mat4 SpatialComponent::orientation() const
{
    if(parent == nullptr)
        return glm::mat4_cast(direction);
    else
        return glm::mat4_cast(parent->direction * direction);
}

glm::vec3 SpatialComponent::globalPosition() const
{
    if(parent == nullptr)
        return position;
    else
        return parent->position + parent->direction * position;
}

glm::vec3 SpatialComponent::forward() const
{
    return glm::cross(up(), right());
    //glm::vec4 forward = glm::inverse(orientation()) * glm::vec4(0, 0, -1, 1);
    //return glm::vec3(forward);
}

glm::vec3 SpatialComponent::right() const
{
    glm::vec4 right = orientation() * glm::vec4(1,0,0,1);
    return glm::vec3(right);
}

glm::vec3 SpatialComponent::up() const
{
    glm::vec4 up = orientation() * glm::vec4(0,1,0,1);
    return glm::vec3(up);
}