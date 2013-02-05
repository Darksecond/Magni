#pragma once

#include "Component.h"
#include "Program.h"

#include <GLM/glm.hpp>

class SpatialComponent : public iComponent<SpatialComponent>
{
public:
    glm::vec3 position;
    glm::vec3 direction;
    
    explicit SpatialComponent(glm::vec3 position);
};