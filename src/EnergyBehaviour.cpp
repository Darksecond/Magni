//
//  EnergyBehaviour.cpp
//  Ymir
//
//  Created by Marvin Effing on 07-05-13.
//  Copyright (c) 2013 Tim Peters. All rights reserved.
//

#include "EnergyBehaviour.h"
#include "HealthComponent.h"
#include "EnergyComponent.h"

#include <iostream>

using namespace Ymir;

void EnergyBehaviour::debuff(Entity& d, uint32_t h) {
    
    HealthComponent* health = d.component<HealthComponent>();
    
    if ((health-h) > 0 ) {
        health->health -= h;
    } else { //Entity will not receive debuff
        std::cout << "Debuff not applicable" << std::endl;
    }
}

void EnergyBehaviour::update(double delta) {

    auto energy = entity->component<EnergyComponent>();
    
    
}
