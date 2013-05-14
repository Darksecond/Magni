#pragma once

#include "Behavior.h"
#include "Entity.h"

namespace Ymir
{
    class RTSCameraBehavior : public Behavior
    {
        void offsetOrientation(double delta);
    private:
        int lastScrollWheelIndex;
        int _width, _height;


    public:
        RTSCameraBehavior(int width, int height);
        virtual void update(double delta);
    };
};
