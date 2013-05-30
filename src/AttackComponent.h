#pragma once

#include "Component.h"

#include <stdint.h>

namespace Ymir
{
    class AttackComponent : public Component<AttackComponent>
    {
    public:
        AttackComponent(uint32_t a, uint32_t d, float as, int thisrange) : attack(a), duration(d), range(thisrange), attackSpeed(as), attackTimer(as) {}
        uint32_t attack, duration;
        float attackSpeed, attackTimer;
        int range;
    };
};
