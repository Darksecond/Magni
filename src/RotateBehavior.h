#pragma once

#include "Behavior.h"

namespace Ymir
{
    class RotateBehavior : public Behavior
    {
    public:
        bool front;
        float rotation;

        RotateBehavior(bool f = true) : front(f), rotation(0)
        {
        }


        virtual void update(double delta);
    };
};
