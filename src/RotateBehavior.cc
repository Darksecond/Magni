#include "RotateBehavior.h"
#include "Entity.h"
#include "SpatialComponent.h"
#include "CarComponent.h"

using namespace Ymir;

void RotateBehavior::update(double delta)
{
    auto car = entity->parent->component<CarComponent>();
    auto spatial = entity->component<SpatialComponent>();
    if(car && spatial)
    {
        //entity->component<SpatialComponent>()->direction = glm::rotate(entity->component<SpatialComponent>()->forward(), car->steering, glm::vec3(0,1,0));
        
        const GLfloat degreesPerSecond = 45.0f;
        float gDegreesRotated = delta * degreesPerSecond;
        while(gDegreesRotated > 360.0f) gDegreesRotated -= 360.0f;
        
        //entity->component<SpatialComponent>()->direction = glm::rotate(entity->component<SpatialComponent>()->direction, gDegreesRotated, entity->component<SpatialComponent>()->right());
        
        rotation += delta * car->gas * 100;
        float wheel = 0;
        if(front)
            wheel = -car->steering * 10;
        
        glm::vec3 angle{rotation, wheel, 0};
        spatial->setDirection(angle);
    }
}