//
//  EnergyBehaviour.h
//  Ymir
//
//  Created by Marvin Effing on 07-05-13.
//  Copyright (c) 2013 Tim Peters. All rights reserved.
//

#ifndef __Ymir__EnergyBehaviour__
#define __Ymir__EnergyBehaviour__

#include <iostream>

#include "Behavior.h"
#include "Entity.h"

namespace Ymir {
    class EnergyBehaviour : public Behavior {
    public:
        EnergyBehaviour(){}
        void debuff(Entity& d, uint32_t h);
        virtual void update(double delta);
    };
};

#endif /* defined(__Ymir__EnergyBehaviour__) */
