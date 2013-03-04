#pragma once

#include "Component.h"

namespace Ymir
{
    class SphereColliderComponent : public Component<SphereColliderComponent>
    {
    public:
        SphereColliderComponent(float r) : radius{r}, trigger{false}
        {
        }
        
        float radius;
        bool trigger;
    };
}