#pragma once

#include "Component.h"

#include <stdint.h>

namespace Ymir
{
    class HealthComponent : public Component<HealthComponent>
    {
    public:
        HealthComponent(uint32_t h) : health(h), startHealth(h) {
            health = h;
            startHealth = h;
        }
        int32_t health;
        int startHealth;
        uint32_t getHealth();
        void setDebuff(uint32_t);
        void setHealth(uint32_t);
    };
};
