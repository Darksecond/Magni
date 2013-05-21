#pragma once

#include "Component.h"

#include <stdint.h>

namespace Ymir
{
    class AttackComponent : public Component<AttackComponent>
    {
    public:
        AttackComponent(uint32_t a, uint32_t d, int thisrange) : attack(a), duration(d), range(thisrange) {}
        uint32_t attack;
        uint32_t duration;

        int range;
    };
};
