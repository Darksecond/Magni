#pragma once

#include "Component.h"

#include <memory>

namespace Ymir
{
    class CarComponent : public Component<CarComponent>
    {
    public:
        CarComponent() : gas{0}, steering{0}
        {
        }
        
        float gas;
        float steering;
    };
};