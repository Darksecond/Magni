#pragma once

#include "Program.h"
#include "Camera.h"
#include "LightComponent.h"
#include "SpatialComponent.h"
#include "Entity.h"

#include <GLM/glm.hpp>
#include <memory>

namespace Ymir
{
    class Light
    {
        LightComponent& _light;
        SpatialComponent& _spatial;
    public:
        static std::unique_ptr<Light> fromEntity(const Entity& e);
        
        Light(LightComponent& light, SpatialComponent& spatial);
        
        void attach(Program& p, const Camera& c) const;
        
        bool operator==(const Light& other) const;
    };
};
