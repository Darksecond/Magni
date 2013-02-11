#include "RotateBehavior.h"
#include "Entity.h"
#include "SpatialComponent.h"

using namespace Ymir;

void RotateBehavior::update(double delta)
{
    const GLfloat degreesPerSecond = 45.0f;
    float gDegreesRotated = delta * degreesPerSecond;
    entity->component<SpatialComponent>()->direction = glm::rotate(entity->component<SpatialComponent>()->direction, gDegreesRotated, glm::vec3(1,0,0));
    while(gDegreesRotated > 360.0f) gDegreesRotated -= 360.0f;
}