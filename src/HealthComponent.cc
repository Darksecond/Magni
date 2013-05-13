//
//  HealthComponent.cc
//  Ymir
//
//  Created by Marvin Effing on 06-05-13.
//  Copyright (c) 2013 Tim Peters. All rights reserved.
//

#include "HealthComponent.h"

using namespace Ymir;
    
uint32_t HealthComponent::getHealth() {
    return health;
}

/**
 * Sets (health) debuff for Entities with HealthComponent
 * TODO: overleg wat we hier mee gaan doen
 */
void HealthComponent::setDebuff(uint32_t d) {
//    uint32_t nieuw = (health - d);
//    if (health > 0 && nieuw > 0) {
//        
//    }
}