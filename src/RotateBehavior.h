#pragma once

#include "Behavior.h"

namespace Ymir
{
    class RotateBehavior : public Behavior
    {
    public:
        virtual void update(double delta);
    };
};