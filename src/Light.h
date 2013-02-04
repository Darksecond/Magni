#pragma once

#include "Program.h"
#include "Camera.h"
#include "LightComponent.h"
#include "SpatialComponent.h"

#include <GLM/glm.hpp>
#include <memory>

class Light
{
    std::shared_ptr<LightComponent> _light;
    std::shared_ptr<SpatialComponent> _spatial;
public:
    Light(std::shared_ptr<LightComponent> light, std::shared_ptr<SpatialComponent> spatial);
    
    void attach(Program& p, const Camera& c) const;
};