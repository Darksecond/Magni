#pragma once

#include "Component.h"

#include <stdint.h>

namespace Ymir
{
    class HealthComponent : public Component<HealthComponent>
    {
    public:
        HealthComponent(uint32_t h) : health(h) {}
        int32_t health;
    };
};