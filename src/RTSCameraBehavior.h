#pragma once

#include "Behavior.h"
#include "Entity.h"

namespace Ymir
{
    class RTSCameraBehavior : public Behavior
    {
        void offsetOrientation(double delta);
    public:
        virtual void update(double delta);
    };
};
