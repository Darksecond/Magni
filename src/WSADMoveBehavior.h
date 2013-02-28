#pragma once

#include "Behavior.h"

#include <GLM/glm.hpp>

namespace Ymir
{
    class WSADMoveBehavior : public Behavior
    {
    public:
        
        virtual void update(double delta);
    };
};