#pragma once

#include "Behavior.h"

namespace Ymir
{
    class WSADMoveBehavior : public Behavior
    {
    public:
        virtual void update(double delta);
    };
};