#pragma once

#include "Component.h"

#include <stdint.h>

namespace Ymir
{
    class AttackComponent : public Component<AttackComponent>
    {
    public:
        AttackComponent(uint32_t a, uint32_t d) : attack(a), duration(d) {}
        uint32_t attack;
        uint32_t duration;
    };
};