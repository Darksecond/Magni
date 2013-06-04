#include "HealthComponent.h"

using namespace Ymir;

    uint32_t HealthComponent::getHealth() {
        return health;
    }

void HealthComponent::setDebuff(uint32_t t) {
    setHealth(health - t);
}

void HealthComponent::setHealth(uint32_t t) {
    health = t;
}


    