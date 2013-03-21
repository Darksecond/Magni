#pragma once

#include "Component.h"

namespace Ymir
{
    class EnergyComponent : public Component<EnergyComponent>
    {
    public:
        EnergyComponent(int e) : energy_consumption(e)
        {
        }
        
        /**
         * Positive is consumes, Negative is produces
         */
        int energy_consumption;
    };
};