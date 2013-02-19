#pragma once

#include "Behavior.h"
#include "Entity.h"

namespace Ymir
{
    class FPSCameraBehavior : public Behavior
    {
        void offsetOrientation(float upAngle, float rightAngle);
    public:
        virtual void update(double delta);
    };
};