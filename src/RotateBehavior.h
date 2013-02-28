#pragma once

#include "Behavior.h"

namespace Ymir
{
    class RotateBehavior : public Behavior
    {
    public:
        bool front;
        RotateBehavior(bool f = true) : front{f}
        {
        }
        
        float rotation = 0;
        virtual void update(double delta);
    };
};